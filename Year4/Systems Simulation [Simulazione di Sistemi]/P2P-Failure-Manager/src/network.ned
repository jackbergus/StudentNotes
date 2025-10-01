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

import modules.NodeManager;
import org.omnetpp.queueing.Source;


network Network
{
    @display("bgb=379,177");

    types:
    submodules:
        nodeManager: NodeManager {
            @display("p=78,122");
        }
        failure_source: Source {
            @display("p=28,23");
            interArrivalTime = exponential(10s); // setting a real random time  - non mi interessa che questi tempi siano diversi         
        }
        join_source: Source {
            @display("p=94,23");
            interArrivalTime = exponential(1200s); // setting a real random time
            startTime = 0s; //Forces the first recording of the state of the network, without doing anything significant
        }
    connections:
        failure_source.out --> { @display("ls=red,1"); } --> nodeManager.event_crash;
        join_source.out --> { @display("ls=red,1"); } --> nodeManager.event_connect;
}
