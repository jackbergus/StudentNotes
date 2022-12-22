/*
 * Graph.h
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

#pragma once
#include <boost/graph/iteration_macros.hpp>
#include "graph_t.h"

class Vertex;
class Edge;
class GraphVertexIterator;
class GraphAdjIterator;
class GraphEdgeIterator;

typedef boost::graph_traits<graph_t>::vertex_iterator GVIt;
typedef boost::graph_traits<graph_t>::edge_iterator GEIt;
typedef boost::graph_traits<graph_t>::adjacency_iterator GAIt;

//class VertexIterator
//{
//	graph_t *graph;
//	GVIt end;
//	GVIt count;
//
//public:
//	VertexIterator(GVIt b, GVIt e, graph_t *g) : end{e}, count{b}, graph{g} {};
//
//	Vertex *end() const {
//		return nullptr;
//	}
//
//	Vertex *begin() const {
//		return (*graph)[*count].self;
//	}
//};

class Graph
{
	graph_t g;
	int v_id = 0;
	int e_id = 0;
	boost::minstd_rand gen;
	std::map<int, Vertex *> map_vertex;
	typedef boost::plod_iterator<boost::minstd_rand, graph_t> SFGen;
	void init_extern(bool withname);

public:

	Graph() {
	}
	Graph(long unsigned int n_Vert, double a, double b) :
		g { SFGen(gen, n_Vert, a, b), SFGen(), n_Vert } {
		init_extern(false);
	}
	Graph(std::string filename);
	Graph(const Graph& l) {
		g = l.g;
		v_id = l.v_id;
		e_id = l.e_id;
	}
//	Graph(const Graph&& l) {
//		g = l.g;
//		v_id = l.v_id;
//		e_id = l.e_id;
//	}

	~Graph();

	void removeAllEdges(Vertex* v);
	Vertex addVertex(std::string label);

	Edge addEdge(Vertex src, Vertex dst/*, std::string label*/);

	void read_graphml(std::string filename);
	void write(std::ostream out_stream); //Writes in GraphML format
	void std_out_write(); //Outputs in GraphML format

	void remove(Vertex* v);
	void remove(Edge* v);

	graph_t* get_gboost_graph();

	Vertex *getVertexFromId(int id);

	GraphVertexIterator Vertices();
	GraphEdgeIterator Edges();

	bool hasAPartition();

	int nVertices();
	GraphAdjIterator Adjacency(Vertex v);
	bool hasEdge(Vertex src, Vertex dst);

	//Iterators
	Vertex* toVertex(vertex_descriptor vic); //casts a GraphVertexIterator into an Agent
	Edge* toEdge(edge_descriptor eic);
	//int vid(vertex_descriptor vic);

};

class GraphVertexIterator
{
	GVIt beg;
	GVIt endalust;
public:
	GraphVertexIterator(GVIt fst, GVIt snd) :
		beg { fst }, endalust { snd } {
	}
	;
	GVIt begin() const {
		return beg;
	}
	GVIt end() const {
		return endalust;
	}

};

//GraphEdgeIterator

class GraphAdjIterator
{
	GAIt beg;
	GAIt endalust;
public:
	GraphAdjIterator(GAIt fst, GAIt snd) :
		beg { fst }, endalust { snd } {
	}
	;
	GAIt begin() const {
		return beg;
	}
	GAIt end() const {
		return endalust;
	}

};

class GraphEdgeIterator
{
	GEIt beg;
	GEIt endalust;
public:
	GraphEdgeIterator(GEIt fst, GEIt snd) :
		beg { fst }, endalust { snd } {
	}
	;
	GEIt begin() const {
		return beg;
	}
	GEIt end() const {
		return endalust;
	}

};
