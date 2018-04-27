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

#include "Parking.h"
#include "Plane_m.h"

namespace airport {

Define_Module(Parking);

cQueue parking_queue("parking_queue");
int count_pk;

void Parking::initialize()
{
    count_pk = 0;
}

void Parking::handleMessage(cMessage *msg)
{
    EV << "Adding plane on parking_queue\n";
    Plane *myMsg;
    myMsg = check_and_cast<Plane*>(msg);
    parking_queue.insert(myMsg);

    count_pk = 0;

    EV << "PRINTING PARKING QUEUE:\n";
    for(cQueue::Iterator iter(parking_queue,0); !iter.end(); iter++) {
          myMsg =(Plane*) iter();
          EV << count_pk++ << " - " <<myMsg->getId() << " " << myMsg->getEnter() <<"\n";
    }
}

}; // namespace
