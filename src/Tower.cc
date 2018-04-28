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

#include "Tower.h"

namespace airport {

Define_Module(Tower);

cMessage *temp_msg;
bool free;

void Tower::initialize()
{
    temp_msg = nullptr;
    free = true;
}

void Tower::handleMessage(cMessage *msg)
{
   if(strcmp(msg->getName(), "Free") == 0){
       free = true;
      /* EV << "Sending OK to landing_queue\n";
       temp_msg = new cMessage("OK");
       send(temp_msg, "out_land");*/
   }
   else if(strcmp(msg->getName(), "newplane_land") == 0){
       if(free) {
           free = false;
           EV << "Sending OK to landing_queue\n";
           temp_msg = new cMessage("OK");
           send(temp_msg, "out_land");
       }
       else {
           temp_msg = new cMessage("REQ_LAND");
           send(temp_msg, "out_pista");
       }
   }
   else if(strcmp(msg->getName(), "newplane_takeoff") == 0){
       if(free) {
           free= false;
           EV << "Sending OK to takeoff_queue\n";
           temp_msg = new cMessage("OK");
           send(temp_msg, "out_takeoff");
       }
       else {
           temp_msg = new cMessage("REQ_TAKEOFF");
           send(temp_msg, "out_pista");
       }
   }

}

}; // namespace
