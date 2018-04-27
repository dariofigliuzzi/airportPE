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
import airport.Pista;
import airport.Landing;
import airport.Takeoff;

//
// Two instances (tic and toc) of Txc connected.
//

network Airport
{
    submodules:

        tower: Tower;
        sky: Sky{
            procTime = 5s;
        }
        parking: Parking;
        pista: Pista;
        landing: Landing;
        takeoff: Takeoff;
        
    connections:
        
        sky.out --> {  delay = 0ms; } --> landing.in_sky;

        landing.out_tower --> {  delay = 100ms; } --> tower.in_land;
        landing.in_tower <-- {  delay = 100ms; } <-- tower.out_land;

        landing.out_pista --> {  delay = 100ms; } --> pista.in_landing;
        landing.in_pista <-- {  delay = 100ms; } <-- pista.out_landing;

        pista.out_parking --> {  delay = 100ms; } --> parking.in_pista;
        pista.in_parking <-- {  delay = 100ms; } <-- parking.out_pista;
        
        pista.out_tower --> {  delay = 100ms; } --> tower.in_pista;
        pista.in_tower <-- {  delay = 100ms; } <-- tower.out_pista;

        takeoff.out_parking --> {  delay = 100ms; } --> parking.in_takeoff;
        takeoff.in_parking <-- {  delay = 100ms; } <-- parking.out_takeoff;

        takeoff.out_pista --> {  delay = 100ms; } --> pista.in_takeoff;
        takeoff.in_pista <-- {  delay = 100ms; } <-- pista.out_takeoff;
        
}
