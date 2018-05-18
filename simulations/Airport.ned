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

package airport.simulations;

import airport.Tower;
import airport.Sky;
import airport.Parking;
import airport.Airstrip;
import airport.Landing;
import airport.Takeoff;

//
// Two instances (tic and toc) of Txc connected.
//

network Airport
{
    @display("bgb=338,641");
    submodules:

        tower: Tower {
            @display("p=265,417");
        }
        sky: Sky {
            //procTime = 5s;
            @display("p=168,579");
        }
        parking: Parking {
            @display("p=168,87");
        }
        airstrip: Airstrip {
            @display("p=71,235");
        }
        landing: Landing {
            @display("p=71,417");
        }
        takeoff: Takeoff {

            @display("p=265,235");
        }
    connections:

        sky.out --> {  delay = 0ms; } --> landing.in_sky;

        landing.out_tower --> {  delay = 100ms; } --> tower.in_land;
        landing.in_tower <-- {  delay = 100ms; } <-- tower.out_land;

        landing.out_airstrip --> {  delay = 100ms; } --> airstrip.in_landing;
        landing.in_airstrip <-- {  delay = 100ms; } <-- airstrip.out_landing;

        airstrip.out_parking --> {  delay = 100ms; } --> parking.in_airstrip;
        airstrip.in_parking <-- {  delay = 100ms; } <-- parking.out_airstrip;

        airstrip.out_tower --> {  delay = 100ms; } --> tower.in_airstrip;
        airstrip.in_tower <-- {  delay = 100ms; } <-- tower.out_airstrip;

        takeoff.out_parking --> {  delay = 100ms; } --> parking.in_takeoff;
        takeoff.in_parking <-- {  delay = 100ms; } <-- parking.out_takeoff;

        takeoff.out_airstrip --> {  delay = 100ms; } --> airstrip.in_takeoff;
        takeoff.in_airstrip <-- {  delay = 100ms; } <-- airstrip.out_takeoff;

        takeoff.out_tower --> {  delay = 100ms; } --> tower.in_takeoff;
        takeoff.in_tower <-- {  delay = 100ms; } <-- tower.out_takeoff;

}
