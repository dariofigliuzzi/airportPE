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

package airport;

//
// Immediately sends out any message it receives. It can optionally generate
// a message at the beginning of the simulation, to bootstrap the process.
//
simple Parking
{
    parameters:
        bool sendInitialMessage = default(false);
        double procTime @unit(s) = default(240s); //tempo in cui rimane parcheggiato
        @signal[arrivalLength](type="long");
        @signal[arrivalId](type="long");
        @statistic[ParkingRecId](title="ID"; source="arrivalId"; record=vector);
        @statistic[ParkingRecLength](title="PARKING QUEUE LENGTH"; source="arrivalLength"; record=vector);
    gates:
        input in_airstrip;
        output out_airstrip;
        
        input in_takeoff;
        output out_takeoff;
}
