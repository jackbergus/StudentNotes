/*
 * Edge.cpp
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

#include "Edge.h"
#include "Graph.h"

Edge::Edge(EProp* prop, edge_descriptor desc, Graph* grap) {
    ep = prop;
    prop->self = this;
    ed = desc;
    g = grap;
}

edge_descriptor Edge::get_gboost_edge() {
    return ed;
}

Vertex* Edge::getSource() {
    if ((!ep) || (!g))
        return nullptr;

    return g->toVertex(ed.m_source);
}

Vertex* Edge::getDest() {
    if ((!ep) || (!g))
        return nullptr;

    return g->toVertex(ed.m_target);
}

void Edge::setEnabled(bool enabled) {
    if ((!ep) || (!g))
        return;

    graph_t *gr = g->get_gboost_graph();
    (*gr)[(ed)].enabled = enabled;
}

bool Edge::getEnabled() {
    if ((!ep) || (!g))
        return false;

    graph_t *gr = g->get_gboost_graph();
    return (*gr)[(ed)].enabled;
}

void Edge::remove() {
    if ((!ep) || (!g)) {
        return;
    }
    Graph* tmp = g;
    ep = nullptr;
    g = nullptr;
    tmp->remove(this);
}
