/*
 * Vertex.cpp
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

#include "Peer.h"
#include "Edge.h"
#include "Vertex.h"
#include "Graph.h"

Vertex::Vertex(VProp* prop, vertex_descriptor desc, Graph* grap) {
    vp = prop;
    vp->self = this;
    vd = desc;
    g = grap;
}

vertex_descriptor Vertex::get_gboost_vertex() {
    return vd;
}

void Vertex::setName(std::string new_name) {
    if ((!vp) || (!g))
        return;

    vp->name = new_name;
}

std::string Vertex::getName() {
    if ((!vp) || (!g)) {
        std::string e { };
        return e;
    }

    return vp->name;
}

void Vertex::setPeer(Peer* p) {
    if ((!vp) || (!g))
        return;

    vp->peer = p;
}

Peer* Vertex::getPeer() {
    if ((!vp) || (!g))
        return nullptr;

    return vp->peer;
}

Edge *Vertex::getEdge(Vertex* adj) {
    graph_t *gr = g->get_gboost_graph();
    std::pair<edge_descriptor, bool> ed_b = boost::edge(vd, adj->vd, *gr);

    if (ed_b.second == false) {
        ed_b = boost::edge(adj->vd, vd, *gr);
        if (ed_b.second == false)
            return nullptr;
    }

    return (*gr)[ed_b.first].self;
}

void Vertex::setEnabled(bool enabled) {
    if ((!vp) || (!g))
        return;

    vp->enabled = enabled;
}

bool Vertex::getEnabled() {
    if ((!vp) || (!g))
        return false;

    return vp->enabled;
}

int Vertex::getId() {
    if ((!vp) || (!g)) {
        return -1;
    }

    return vp->id;
}

long Vertex::nAdjacents() {
    if ((!vp) || (!g))
        return -1L;

    int size = 0;
    for (vertex_descriptor x: g->Adjacency(*this) ) {
    	size++;
    }

    return size;
}

class truePredicate {
    graph_t graph;
    Vertex *ver;
public:
    truePredicate(graph_t g, Vertex *v) :
            graph { g }, ver {v} {
    }
    bool operator()(edge_descriptor e) {
    	std::cout << "before... " <<std::endl;
    	if (graph[e].self==nullptr) std::cerr << "WARNING!!!" <<std::endl;
    	std::cout << "...and after " << std::endl;
        return (graph[e].self->getSource() == ver) || (graph[e].self->getDest() == ver);
    }
};

bool Vertex::isConnected(Vertex* v) {
	return g->hasEdge(*this, *v);
}

void Vertex::removeAllEdges() {
	g->removeAllEdges(this);
}

void Vertex::remove() {
    if ((!vp) || (!g)) {
        return;
    }
    Graph* tmp = g;
    vp = nullptr;
    g = nullptr;
    tmp->remove(this);
}

Graph* Vertex::getGraph() {
    return g;
}

