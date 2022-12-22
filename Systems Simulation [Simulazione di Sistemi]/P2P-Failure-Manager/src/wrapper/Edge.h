/*
 * Edge.h
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
#include "graph_t.h"

/**
 * Edge:
 * This class defines the abstraction of a graph edge, with its properties
 * and the gboost identifier
 */
class Edge {
	private:
		EProp* ep;
		edge_descriptor ed;
		Graph* g;
		bool enabled;

	public:
		Edge(EProp* prop, edge_descriptor desc,Graph* grap);

        Vertex* getSource();
        Vertex* getDest();

        bool getEnabled();
        void setEnabled(bool enabled);

		edge_descriptor get_gboost_edge();
		void remove();
};
