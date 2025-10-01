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

#include "Peer.h"
#include "Vertex.h"
#include <iostream>
#include <list>
#include "ProtocolMessageUtils.h"
#include "NodeManager.h"
#include "NeighbourCrashedAdvice_m.h"
#include "LinkCreationRequest_m.h"
#include "LinkCreatedAdvice_m.h"
#include "LinkCreationAnswer_m.h"
#include "utils.h"


Define_Module(Peer);



void Peer::initialize() {
	seed = par("seed");
	metric.setName(getName());
	nodeManager = NodeManager::getInstance();

}

void Peer::setVertex(Vertex *vertex) {
	this->vertex = vertex;
}

void Peer::handleMessage(cMessage *msg) {

	NeighbourCrashedAdvice *neighbourCrashedAdvice;
	LinkCreationRequest *linkCreationRequest;
	LinkCreationAnswer *linkCreationAnswer;
	LinkCreatedAdvice *linkCreatedAdvice;

	/*
	 * messaggio per l'avviso del crash di un nodo
	 */
	if ((neighbourCrashedAdvice = dynamic_cast<NeighbourCrashedAdvice*>(msg)) != NULL) {


		if ((neighbourCrashedAdvice->getCrashedNodeNeighbourArraySize()!=0)) {
			if (neighbourCrashedAdvice->getSource() == NODE_MANAGER_ID) {

				// ottengo la lista dei vicini del nodo crashato nella quale sono già stati eliminati i miei primi e secondi vicini ed anche il mio id
				std::list<int> crashedNodeNeighbourList = getRandomNeighbourList(neighbourCrashedAdvice, vertex, nodeManager,getRNG(seed));

				int len = crashedNodeNeighbourList.size();
				int i = 0;
				double initTime = simTime().dbl();
				NeighbourCrashedAdvice **neighbourCrashedAdvice2 = new NeighbourCrashedAdvice*[len];

				for (int elem : crashedNodeNeighbourList) {
					neighbourCrashedAdvice2[i] = new NeighbourCrashedAdvice;
					neighbourCrashedAdvice2[i]->setSource(vertex->getId());
					neighbourCrashedAdvice2[i]->setDestination(vertex->getId());
					neighbourCrashedAdvice2[i]->setCrashedNodeNeighbourArraySize(1);
					neighbourCrashedAdvice2[i]->setCrashedNodeNeighbour(0,elem);
					//Mi auto-invio il messaggio
					initTime += exponential(0.01);
					scheduleAt(initTime, neighbourCrashedAdvice2[i]);
					i++;
				}

			} else if (nodeManager->isEqualBelowThreshold(vertex))  {
				// leggo l'i-esimo vicino e creo il messaggio di link creation request
				int neighbour = neighbourCrashedAdvice->getCrashedNodeNeighbour(0);
				LinkCreationRequest *linkCreationRequest = new LinkCreationRequest();
				linkCreationRequest->setSource(vertex->getId());
				linkCreationRequest->setDestination(neighbour);

				// se non si sono già connessi invio il messaggio
				if (!nodeManager->getVertexFromId(neighbour)->isConnected(vertex))
					sendDirect(linkCreationRequest, nodeManager->getVertexFromId(neighbour)->getPeer()->gate("neighbours$i", neighbour));
			}
		}

	}
	/*
	 * messaggio per la richiesta di connessione con un nodo
	 */
	else if ((linkCreationRequest = dynamic_cast<LinkCreationRequest*>(msg)) != NULL) {
		int sender = linkCreationRequest->getSource();
		std::cout << "sender: " << sender << "  id: " << vertex->getId() << std::endl;
		Vertex *vSender = nodeManager->getVertexFromId(sender);
		if (nodeManager->isIn2Hop(vertex,vSender)) {
			LinkCreationAnswer *linkCreationAnswer = new LinkCreationAnswer();
			linkCreationAnswer->setSource(vertex->getId());
			linkCreationAnswer->setDestination(sender);
			linkCreationAnswer->setAnswer(answer_e::REFUSED);
			sendDirect(linkCreationAnswer,nodeManager->getVertexFromId(sender)->getPeer(),"direct" );
		} else {
			LinkCreationAnswer *linkCreationAnswer = new LinkCreationAnswer();
			linkCreationAnswer->setSource(vertex->getId());
			linkCreationAnswer->setDestination(sender);
			linkCreationAnswer->setAnswer(answer_e::ACCEPTED);
			sendDirect(linkCreationAnswer,nodeManager->getVertexFromId(sender)->getPeer(),"direct" );
			LinkCreatedAdvice** lcd = new LinkCreatedAdvice*[vertex->nAdjacents()];
			int index = 0;
			for (auto v: nodeManager->getNeighbours(vertex)) {
				Vertex *w = nodeManager->getVertexFromDescriptor(v);
				int newdest = w->getId();
				lcd[index] = new LinkCreatedAdvice();
				lcd[index]->setSource(vertex->getId());
				lcd[index]->setDestination(newdest);
				lcd[index]->setOtherNode(sender);
				sendDirect(lcd[index],nodeManager->getVertexFromId(newdest)->getPeer(),"direct" );
				index++;
			}
			std::cout << "Reconnection of " << vertex->getId() << " with " << sender << std::endl;
			nodeManager->establishNewLink(vertex,nodeManager->getVertexFromId(sender));
		}
	}
	/*
	 * messaggio per la risposta alla richiesta di connessione con un nodo
	 */
	else if ((linkCreationAnswer = dynamic_cast<LinkCreationAnswer*>(msg)) != NULL) {
		if (linkCreationAnswer->getAnswer()==answer_e::ACCEPTED) {
			int sender = linkCreationAnswer->getSource();
			LinkCreatedAdvice** lcd = new LinkCreatedAdvice*[vertex->nAdjacents()];
			int index = 0;
			for (auto v: nodeManager->getNeighbours(vertex)) {
				Vertex *w = nodeManager->getVertexFromDescriptor(v);
				int newdest = w->getId();
				lcd[index] = new LinkCreatedAdvice();
				lcd[index]->setSource(vertex->getId());
				lcd[index]->setDestination(newdest);
				lcd[index]->setOtherNode(sender);
				sendDirect(lcd[index],nodeManager->getVertexFromId(newdest)->getPeer(),"direct" );
				index++;
			}
			nodeManager->establishNewLink(vertex,nodeManager->getVertexFromId(sender));
		}
	}
	/*
	 * messaggio per la notifica dell'avvenuta connessione tra due nodi
	 */
	else if ((linkCreatedAdvice = dynamic_cast<LinkCreatedAdvice*>(msg)) != NULL) {

	}

	delete msg;

}

void Peer::logDegree() {
	metric.record(vertex->nAdjacents());
}

/**
 * funzione chiamata (solo) quando la simulazione �� terminata correttamente
 */
void Peer::finish() {
	// RECORD STATISTICS HERE!
}
