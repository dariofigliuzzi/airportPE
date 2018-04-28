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

cMessage *temp_msg_land;    //per invio OK a Landing
cMessage *temp_msg_takeoff; //per invio OK a Takeoff
cMessage *req;              //per invio richiesta disponibilità pista a Pista
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
   //Gestione messaggio "Free" da Pista
   if(strcmp(msg->getName(), "Free") == 0){
       free = true;
   }

   //Gestione richieste dalle code di landing e takeoff
   else if(strcmp(msg->getName(), "newplane_land") == 0 || strcmp(msg->getName(), "newplane_takeoff") == 0){

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
}

}; // namespace
