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

#include "Pista.h"

namespace airport {

Define_Module(Pista);

bool free_strip;

void Pista::initialize()
{
    free_strip = true;
    cMessage *tmp_msg = new cMessage("Free");

       if(free_strip) {
           send(tmp_msg, "out_tower");
           free_strip = false;
       }

}

void Pista::handleMessage(cMessage *msg)
{
    EV << "--->Landing\n";
    //inserire schedulazione "dopo tl send()"
    send(msg, "out_parking");
}

}; // namespace
