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

#include "Landing.h"
#include "Plane_m.h"

namespace airport {

Define_Module(Landing);

cQueue landing_queue("landing_queue");
int count;

void Landing::initialize()
{
   count = 0;
}

void Landing::handleMessage(cMessage *msg)
{
    if(strcmp(msg->getName(), "OK") == 0) {
        if(!landing_queue.isEmpty())
            EV << "Start Landing\n";
        else
            EV<< "The landing_queue is empty\n";
    }
    else {
        EV << "Adding plane on landing_queue\n";
        Plane *myMsg;
        myMsg = check_and_cast<Plane*>(msg);
        landing_queue.insert(myMsg);
        count = 0;

        EV << "PRINTING LANDING QUEUE:\n";
        for(cQueue::Iterator iter(landing_queue,0); !iter.end(); iter++) {
            myMsg =(Plane*) iter();
            EV << count++ << " - " <<myMsg->getId() << " " << myMsg->getEnter() <<"\n";
        }
    }
}

}; // namespace
