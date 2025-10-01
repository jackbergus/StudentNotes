/*
 * VProp.h
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
#include <memory>
#include <string>
#include <stack>


class Vertex;
class Agent;
class Peer;

/**
 * Vertex:
 * this class defines the basic properties of the graph vertex
 */
typedef struct vpr_
{ 
	std::string name; 			/**< File name that loads the agent state */
	int id;					    /**< Vertex identificator */
	Vertex *self;				/**< Pointer to the element-holder */
	bool loaded;				/**< Since the vertex could be created ex-novo, this marks if the vertex was made without any Vertex object. Hence, the self object should be deallocated*/
	bool enabled;
	Peer* peer;

} VProp;

