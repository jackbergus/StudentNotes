/*
 * utils.h
 * This file is part of P2P-Failure-Manager_SDS
 *
 * Copyright (C) 2022 - Giacomo Bergami, Giulio Biagini, Gianluca Iselli
 *
 *  P2P-Failure-Manager_SDS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 *  P2P-Failure-Manager_SDS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with  P2P-Failure-Manager_SDS. If not, see <http://www.gnu.org/licenses/>.
 */



#ifndef UTILS_H_
#define UTILS_H_

#include <list>
#include <omnetpp.h>
#include "NeighbourCrashedAdvice_m.h"
#include "NodeManager.h"
#include "Vertex.h"



/**
 * funzione che si occupa di modificare l'array dei vicini del nodo crashato tornandolo sottoforma di lista ordinata random
 * nella quale sono stati eliminati i nodi che compaiono nei primi e secondi vicini del nodo in input ed il nodo stesso
 *
 * @param neighbourCrashedAdvice - il puntatore al messaggio che contiene l'array dei vicini
 * @param destination - il vertice a cui �� arrivato il messaggio
 * @param nodeManager - un puntatore al node manager
 * @param seed - viene passata una particolare istanza del generatore casuale per il seed della corrente simulazione
 * @return l'array dei vicini sottoforma di lista ordinata random
 */
static inline std::list<int> getRandomNeighbourList(NeighbourCrashedAdvice* neighbourCrashedAdvice, Vertex *destination, NodeManager *nodeManager,cRNG* seed) {
	std::list<int>::iterator position;
	std::list<int> neighbourList;

	// scorro l'array dei vicini del nodo crashato
	for (int i = 0; i < neighbourCrashedAdvice->getCrashedNodeNeighbourArraySize(); i++) {
		int neighbour = neighbourCrashedAdvice->getCrashedNodeNeighbour(i);
		// se non rientra nei primi o secondi vicini del vertice destinazione e non �� il vertice destinazione (comunque un vicino del nodo crashato, quindi presente nella lista)
		if ((!nodeManager->isIn2Hop(destination, nodeManager->getVertexFromId(neighbour))) && neighbour != destination->getId()) {
			// lo inserisco in lista
			position = neighbourList.begin();

			std::advance(position, (neighbourList.empty() ? 0 : seed->intRand(neighbourList.size())));

			neighbourList.insert(position, neighbour);
		}
	}
	return neighbourList;
}


#endif /* UTILS_H_ */
