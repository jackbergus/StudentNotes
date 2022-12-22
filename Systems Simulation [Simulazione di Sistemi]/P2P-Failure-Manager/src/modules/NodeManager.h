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

#ifndef __SDS_TEST1_NODEMANAGER_H_
#define __SDS_TEST1_NODEMANAGER_H_

#include <omnetpp.h>
#include "Graph.h"

#define NODE_MANAGER_ID (-1)

/**
 * TODO - Generated class
 */
class NodeManager: public cSimpleModule {
    Graph g;
    static NodeManager* nm;
    std::deque<Vertex *> inactive_queue;
    int thresholdDegree;
    bool dinamicThresholdDegree = false;
    int seed;
    int n_crashed = 0;
    bool firstLog = false;
private:
    void logDegree();

public:
    static NodeManager* getInstance() { return nm; };
    bool isEqualBelowThreshold(Vertex *vertex);
    int calculateDinamicThreshold();
    Vertex *getVertexFromId(int id);
    Vertex *getVertexFromDescriptor(vertex_descriptor vd);
    bool isIn2Hop(Vertex *source, Vertex *dest);
    GraphAdjIterator getNeighbours(Vertex* caller);
    void establishNewLink(Vertex *src, Vertex *dst);

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
};

#endif
