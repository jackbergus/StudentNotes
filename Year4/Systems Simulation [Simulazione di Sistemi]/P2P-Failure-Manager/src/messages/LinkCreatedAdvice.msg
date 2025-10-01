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

cplusplus
{{
        #include "ProtocolMessage_m.h"
}}
message ProtocolMessage;
//
// messaggio per l'informazione dell'avvenuta creazione di un link da parte del mittente con il nodo specificato nel campo "otherNode"
//
class LinkCreatedAdvice extends ProtocolMessage
{
    // il nodo con cui il mittente ha stabilito un collegamento
    int otherNode;
}