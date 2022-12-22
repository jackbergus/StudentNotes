/*
 * Graph.cpp
 * This file is part of socialsim
 *
 * Copyright (C) 2014 - Giacomo Bergami
 *
 * socialsim is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * socialsim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with socialsim. If not, see <http://www.gnu.org/licenses/>.
 */

#include <memory>
#include <map>
#include "Peer.h"
#include "Graph.h"
#include "Vertex.h"
#include "Edge.h"



Graph::~Graph() {
    BGL_FORALL_VERTICES( v, g, graph_t ){
        if (g[v].loaded) //Removes the allocated vertices
        	delete g[v].self;
    }
}

void Graph::removeAllEdges(Vertex* v) {
	std::cout << "Ok" <<std::endl;
	std::list<vertex_descriptor> lv;

	for (auto u:Adjacency(*v)) {
		lv.push_back(u);
	}
	for (auto vd:lv) {
		boost::remove_edge(v->get_gboost_vertex(),vd,g);
	}

}

void Graph::init_extern(bool withname) {

    v_id = 0;
    e_id = 0;

    BGL_FORALL_VERTICES( v, g, graph_t ) {

        g[v].id = v_id++;
        g[v].self = new Vertex(&g[v], v, this);
        g[v].enabled = true;
        map_vertex[g[v].id] = g[v].self;
    }

    BGL_FORALL_EDGES( e, g, graph_t ) {
        g[e].self = new Edge(&g[e], e, this);
        g[e].id = e_id++;
        g[e].enabled = true;
    }
}

Vertex *Graph::getVertexFromId(int id) {
	return map_vertex[id];
}


Graph::Graph(std::string filename) {
    boost::dynamic_properties dp;
    dp.property("vertex_name", get(&VProp::name, g));
    dp.property("vertex_cmdb_id", get(&VProp::id, g));
    dp.property("edge_id", get(&EProp::id, g));
    std::ifstream fin { filename.c_str() };
    boost::read_graphml(fin, g, dp);
    init_extern(true);
}

void Graph::read_graphml(std::string filename) {
    boost::dynamic_properties dp;
    dp.property("vertex_name", get(&VProp::name, g));
    dp.property("vertex_cmdb_id", get(&VProp::id, g));
    dp.property("edge_id", get(&EProp::id, g));
    std::ifstream fin { filename.c_str() };
    boost::read_graphml(fin, g, dp);
    init_extern(true);
}

Vertex Graph::addVertex(std::string label) {
    VProp vp;
    vp.name = label;
    vp.id = v_id++;
    vp.loaded = false;
    vertex_descriptor v0 = boost::add_vertex(g);
    g[v0] = vp;
    Vertex v { &vp, v0, this }; //updates vp pointer to vertex
    return v;
}

Edge Graph::addEdge(Vertex src, Vertex dst) {
    EProp ep;
    ep.id = e_id++;
    /* ep.name = label; */
    edge_descriptor e0 = boost::add_edge(src.get_gboost_vertex(), dst.get_gboost_vertex(), ep, g).first;
    Edge e { &ep, e0, this };
    return e;
}


bool Graph::hasAPartition() {
	std::map<vertex_descriptor, int> map_visited;
	std::deque<vertex_descriptor> node_stack;
	bool found = false;

	vertex_descriptor vic;

	// prendo il primo nodo -> uno a caso! L'importante �� che sia attivo
	BGL_FORALL_VERTICES( v, g, graph_t ) {
		if (!found && toVertex(v)->getEnabled()) {
			node_stack.push_back(v);
			found = true;
		}
		map_visited[v] = 0;
	}

	// faccio la bfs sul grafo marcando i nodi visitati nella hashmap map_visited
	while (!node_stack.empty()) {
		vic = node_stack.front();	// leggo il primo della lista
		node_stack.pop_front();		// e lo rimuovo
		if (map_visited[vic] != 1) {
			map_visited[vic] = 1;
			for (vertex_descriptor v : Adjacency(*toVertex(vic)))
				node_stack.push_back(v);
		}
	}

	// itero su ogni nodo e se trovo un nodo attivo che non �� marcato nella hash
	// allora ho un grafo sconnesso.
	BGL_FORALL_VERTICES( v, g, graph_t ) {
		if (toVertex(v)->getEnabled() && map_visited[v] != 1)
			return true;
	}
	return false;
}


void Graph::write(std::ostream out_stream) {
    /*boost::dynamic_properties dp;
    dp.property("vertex_name", get(&VProp::name, g));
    dp.property("vertex_cmdb_id", get(&VProp::id, g));
    dp.property("edge_id", get(&EProp::id, g));
    write_graphml(out_stream, g, dp);*/
}

void Graph::std_out_write() {
    /*boost::dynamic_properties dp;
    dp.property("vertex_name", get(&VProp::name, g));
    dp.property("vertex_cmdb_id", get(&VProp::id, g));
    dp.property("edge_id", get(&EProp::id, g));
    write_graphml(std::cout, g, dp);*/
}

void Graph::remove(Vertex* v) {
    if (!v)
        return;
    boost::remove_vertex(v->get_gboost_vertex(), g);
    v->remove();
    //only if the objecy was dynamically allocated
    if (g[v->get_gboost_vertex()].loaded) {
        delete v;
    }
}

void Graph::remove(Edge* e) {
    if (!e)
        return;
    boost::remove_edge(e->get_gboost_edge(), g);
    e->remove();
}

GraphVertexIterator Graph::Vertices() {
    std::pair<GVIt, GVIt> vxit = vertices(g);
    GraphVertexIterator gvit { vxit.first, vxit.second };
    return gvit;
}

int Graph::nVertices() {
    return boost::num_vertices(g);
}

GraphAdjIterator Graph::Adjacency(Vertex v) {
    std::pair<GAIt, GAIt> vxit = adjacent_vertices(v.get_gboost_vertex(), g);
    GraphAdjIterator gvit { vxit.first, vxit.second };
    return gvit;
}

GraphEdgeIterator Graph::Edges() {
    std::pair<GEIt, GEIt> eit = edges(g);
    GraphEdgeIterator gvit { eit.first, eit.second };
    return gvit;
}

bool Graph::hasEdge(Vertex src, Vertex dst) {
    for (auto i : Adjacency(src)) {
        if ((i) == dst.get_gboost_vertex())
            return true;
    }
    return false;
}

Vertex* Graph::toVertex(vertex_descriptor vic) {
    return g[vic].self;
}

Edge* Graph::toEdge(edge_descriptor eic) {
    return g[eic].self;
}

graph_t* Graph::get_gboost_graph() {
    return &g;
}

