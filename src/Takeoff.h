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

#ifndef __AIRPORT_TCX_H
#define __AIRPORT_TCX_H

#include <omnetpp.h>
#include "Plane_m.h"

using namespace omnetpp;

namespace airport {

/**
 * Implements the Txc simple module. See the NED file for more information.
 */
class Takeoff : public cSimpleModule
{
  private:
    cMessage *beep;
    cMessage* notify;
    Plane *plane;

    simsignal_t arrivalSignalId;
    simsignal_t arrivalSignalLength;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual Plane* get_info(Plane* p);
};

}; // namespace

#endif
