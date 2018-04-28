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

#include "Takeoff.h"

namespace airport {

Define_Module(Takeoff);

cQueue takeoff_queue("takeoff_queue");
int count_to;
//simtime_t timerto;


void Takeoff::initialize()
{
    count_to = 0;
    plane = nullptr;
    notify = nullptr;
    beep = nullptr;
    //timerto = par("procTime");
}

void Takeoff::handleMessage(cMessage *msg)
{
    if(strcmp(msg->getName(), "OK") == 0) {
        if(!takeoff_queue.isEmpty()) {
             EV << "Start Take-off\n";
             cObject* obj_plane;
             obj_plane = takeoff_queue.pop();
             plane = check_and_cast<Plane*>(obj_plane);
             //plane = obj_plane->
             send(plane, "out_pista");
         }
         else
             EV<< "The takeoff_queue is empty\n";
        }
     else {
         EV << "Adding plane on takeoff_queue\n";
         Plane *myMsg;
         myMsg = check_and_cast<Plane*>(msg);
         takeoff_queue.insert(myMsg);
         count_to = 0;
         plane = myMsg;
         EV << "PRINTING TAKE-OFF QUEUE:\n";
         for(cQueue::Iterator iter(takeoff_queue,0); !iter.end(); iter++) {
            myMsg =(Plane*) iter();
            EV << count_to++ << " - " <<myMsg->getId() << " " << myMsg->getEnter() <<"\n";
         }
         notify = new cMessage("newplane_takeoff");
         send(notify, "out_tower");
     }
}

}; // namespace
