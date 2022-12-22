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

package modules;

//
// TODO auto-generated module
//
simple NodeManager 
{
    // Il NodeManager è connesso a tutti i nodi da lui creati per poter inviare
    // i messaggi di crash o aggiunta di un nodo.
    // La connessione è solo nel verso NodeManager -> Client. 
	parameters:
	    string filename;
	    int thresholdDegree;
	    int seed;
	    
	gates:
	    input event_crash;
	    input event_connect;
		output peers[]; 
}






