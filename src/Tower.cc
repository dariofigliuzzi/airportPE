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

cMessage *temp_msg_land;
cMessage *temp_msg_takeoff;
cMessage *req;
bool free;

void Tower::initialize()
{
    temp_msg_land = nullptr;
    temp_msg_takeoff = nullptr;
    req = nullptr;
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
   else if(strcmp(msg->getName(), "newplane_land") == 0 || strcmp(msg->getName(), "newplane_takeoff") == 0){
      /* if(free) {
           free = false;
           //EV << "Sending OK to landing_queue\n";
           EV << "Sending OK to queues\n";

           temp_msg_land = new cMessage("OK");
           send(temp_msg_land, "out_land");
           temp_msg_takeoff = new cMessage("OK");
           send(temp_msg_takeoff, "out_takeoff");
       }
       else {
           //temp_msg = new cMessage("REQ_LAND");
           req = new cMessage("REQ");
           send(req, "out_pista");
       }*/
       req = new cMessage("REQ");
       send(req, "out_pista");
       if(free){
           EV << "Sending OK to queues\n";
           if(strcmp(msg->getName(), "newplane_land") == 0){
               free = false;
               temp_msg_land = new cMessage("OK");
               send(temp_msg_land, "out_land");
           }
           else if(strcmp(msg->getName(), "newplane_takeoff") == 0) {
               free = false;
               temp_msg_takeoff = new cMessage("OK");
               send(temp_msg_takeoff, "out_takeoff");
           }
       }
   }
  /* else if(strcmp(msg->getName(), "newplane_takeoff") == 0){
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
   }*/

}

}; // namespace
