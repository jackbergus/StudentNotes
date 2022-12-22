//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "NodeManager.h"
#include "Peer.h"
#include <string>
#include <iostream>
#include <memory>
#include <cstdlib>
#include "graph_t.h"
#include "Graph.h"
#include "Vertex.h"
#include "Edge.h"
#include "NeighbourCrashedAdvice_m.h"




Define_Module(NodeManager);



/**
 * istanza del singleton
 */
NodeManager* NodeManager::nm = nullptr;

void NodeManager::logDegree() {
	// Logging dei degree
	for (vertex_descriptor v : g.Vertices()) {
		g.toVertex(v)->getPeer()->logDegree();
	}
}

std::string get_working_path() {
   char temp[PATH_MAX];
   return ( getcwd(temp, PATH_MAX) ? std::string( temp ) : std::string("") );
}

int NodeManager::calculateDinamicThreshold() {
	int average = 0;
	long sum = 0;
	int freq = 0;
	int nMax = 0;
	std::map<int, int> map_freq;

	for (vertex_descriptor v : g.Vertices()) {
		sum += g.toVertex(v)->nAdjacents();
		map_freq[g.toVertex(v)->nAdjacents()] = 0;
	}
	for (vertex_descriptor v : g.Vertices()) {
		map_freq[g.toVertex(v)->nAdjacents()]++;
	}
	for (vertex_descriptor v : g.Vertices()) {
		if (nMax < map_freq[g.toVertex(v)->nAdjacents()]) {
			nMax = map_freq[g.toVertex(v)->nAdjacents()];
			freq = g.toVertex(v)->nAdjacents();
		}
	}
	average = ceil(sum / (double)g.nVertices());

	#define MAX(a,b) (((a)>(b))?(a):(b))
	return MAX(ceil((average + freq) / 2.0), 2);
}

void NodeManager::initialize() {
	seed = par("seed");
	nm = this;
	/* leggo il parametro filename dal .ini */
	const char * fn = par("filename");
    thresholdDegree = par("thresholdDegree");

    if (thresholdDegree <= 0)
    	dinamicThresholdDegree = true;


	std::string filename { fn };
	filename = get_working_path() + "/../simulations/" + filename;

	std::cout << filename << std::endl;
	g.read_graphml(filename);

	int nPeers = g.nVertices();
	setGateSize("peers", nPeers);
	for (vertex_descriptor v : g.Vertices()) {
		std::string id_str = std::to_string(g.toVertex(v)->getId());
		Peer *peerTmp = (Peer *) cModuleType::get("modules.Peer")->create(id_str.c_str(), getParentModule());
		peerTmp->setGateSize("neighbours", nPeers);
		peerTmp->buildInside();
		peerTmp->scheduleStart(simTime());
		g.toVertex(v)->setPeer(peerTmp);
		peerTmp->setVertex(g.toVertex(v));
		gate("peers", g.toVertex(v)->getId())->connectTo(peerTmp->gate("manager"));
		gate("peers", g.toVertex(v)->getId())->setDisplayString("ls=grey,1");
	}


	for (edge_descriptor e : g.Edges()) {
		Vertex *source = g.toEdge(e)->getSource();
		Vertex *dest = g.toEdge(e)->getDest();
		//std::cout << source->getId() << " <--> " << dest->getId() << std::endl;

		source->getPeer()->gate("neighbours$o", dest->getId())->connectTo(dest->getPeer()->gate("neighbours$i", source->getId()));
		dest->getPeer()->gate("neighbours$o", source->getId())->connectTo(source->getPeer()->gate("neighbours$i", dest->getId()));
	}
}

/**
 * funzione che si occupa di gestire i messaggi che giungono dalle due sorgenti che generano i messaggi di crash e join dei nodi
 *
 * @param msg - il messaggio
 */
void NodeManager::handleMessage(cMessage *msg) {

	// Logging dei degree
	if (!firstLog) {
		for (vertex_descriptor v : g.Vertices()) {
			g.toVertex(v)->getPeer()->logDegree();
		}
		firstLog = true;
	}


	if (g.hasAPartition()) {
		std::cout << ">>>>>>>>>>>>>>>>>> g.hasAPartition()  TD: " << thresholdDegree << std::endl;
		endSimulation(); //stops the simulation
	}



	/*
	 * messaggio per il crash di un nodo
	 */
	if (strcmp(msg->getArrivalGate()->getName(), "event_crash") == 0) {
		std::cout << msg->getArrivalGate()->getName() << " - ";

		int adjMax = 0;
		Vertex *vMax = nullptr;
		Vertex *tmpVert = nullptr;


		// ottengo il vertice con adiacenza massima (il nodo da far crashare)
		for (vertex_descriptor v : g.Vertices()) {
			tmpVert = g.toVertex(v);
			if (tmpVert->nAdjacents() > adjMax) {
				adjMax = tmpVert->nAdjacents();
				vMax = tmpVert;
			}
		}
		if (vMax == nullptr) {
			std::cout << "No more vectors/edges to remove: " << std::endl;
			logDegree();//Final Log
			endSimulation(); //stops the simulation
			return;
		}

		if (dinamicThresholdDegree)
			thresholdDegree = calculateDinamicThreshold();

		n_crashed++;

		std::cout << "Remove: " << vMax->getPeer()->getName() << std::endl;

		vMax->getPeer()->getDisplayString().setTagArg("t",0,"crashed");
		vMax->getPeer()->setDisplayString("p=0,0");

		inactive_queue.push_back(vMax);

		vMax->setEnabled(false);


		for (vertex_descriptor v : g.Adjacency(*vMax)) {
			tmpVert = g.toVertex(v);
			tmpVert->getPeer()->gate("neighbours$i", vMax->getId())->disconnect();
			vMax->getPeer()->gate("neighbours$o", tmpVert->getId())->disconnect();
			tmpVert->getPeer()->gate("neighbours$o", vMax->getId())->disconnect();
			vMax->getPeer()->gate("neighbours$i", tmpVert->getId())->disconnect();
		}

		// Creo il messaggio
		NeighbourCrashedAdvice *neighbourCrashedAdvice = new NeighbourCrashedAdvice;
		neighbourCrashedAdvice->setSource(NODE_MANAGER_ID); // Is NodeManager
		neighbourCrashedAdvice->setCrashedNode(vMax->getId());
		neighbourCrashedAdvice->setCrashedNodeNeighbourArraySize(vMax->nAdjacents());
		int i =0;
		std::cout << "createNCAMessage [" << vMax->getId() << "]: neighbour ids: ";
		for (vertex_descriptor v : g.Adjacency(*vMax)) {
			std::cout << g.toVertex(v)->getId() << " - ";
			neighbourCrashedAdvice->setCrashedNodeNeighbour(i, g.toVertex(v)->getId());
			i++;
		}
		std::cout << std::endl;

		//Invio il messaggio agli ex-adiacenti del nodo che ha fatto crash
		NeighbourCrashedAdvice **n  = new NeighbourCrashedAdvice *[vMax->nAdjacents()];
		int idx = 0;
		for (vertex_descriptor v : g.Adjacency(*vMax)) {
			n[idx] = neighbourCrashedAdvice->dup();
			n[idx]->setDestination(g.toVertex(v)->getId());
			send(n[idx], "peers", g.toVertex(v)->getId());
			idx++;
		}

		vMax->removeAllEdges();
		logDegree();
	} else
		std::cout << "[ERROR]: Unknown message - " << msg->getArrivalGate()->getName() << std::endl;
}



bool NodeManager::isEqualBelowThreshold(Vertex *vertex) {
	return vertex->nAdjacents() <= thresholdDegree;
}

GraphAdjIterator NodeManager::getNeighbours(Vertex* caller) {
	return g.Adjacency(*caller);
}

void NodeManager::establishNewLink(Vertex *src, Vertex *dst) {
	cGate *gate = src->getPeer()->gate("neighbours$o",dst->getId());
	Peer *pSource = src->getPeer();
	Peer *pDest = dst->getPeer();
	if (! gate->isConnected()) {
		std::cout << "Is connecte" <<std::endl;
		gate->connectTo(pDest->gate("neighbours$i",src->getId()));
		pDest->gate("neighbours$o",src->getId())->connectTo(pSource->gate("neighbours$i",dst->getId()));
		g.addEdge(*src,*dst);
		logDegree();
	} else std::cerr << "Not Connected" << std::endl;

}

Vertex *NodeManager::getVertexFromId(int id) {
	return g.getVertexFromId(id);
}

Vertex *NodeManager::getVertexFromDescriptor(vertex_descriptor vd) {
	return g.toVertex(vd);
}

bool NodeManager::isIn2Hop(Vertex *source, Vertex *dest) {

	for (vertex_descriptor v : g.Adjacency(*source)) {
		if (g.toVertex(v)->getId() == dest->getId())
			return true;

		for (vertex_descriptor v2 : g.Adjacency(*(g.toVertex(v)))) {
			if (g.toVertex(v2)->getId() == dest->getId())
				return true;
		}
	}
	return false;
}



/**
 * funzione chiamata (solo) quando la simulazione è terminata correttamente
 */
void NodeManager::finish() {
	// RECORD STATISTICS HERE!

}
