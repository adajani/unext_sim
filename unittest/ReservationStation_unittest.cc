//  ReservationStation_unittest.cc
//
//  Author:
//       Ahmed Dajani <adajani@iastate.edu>
//
//  Copyright (c) 2023 Ahmad Dajani
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <ReservationStation_fixture.hpp>

/*
 * Top level testbench function
 * Test the Reservation Station
 *
 * Parameters
 *     uint32 results[6]      Results data in order : {ready busy, Vs, Vt, Func, Name}
 *     uint5 name             The name of the RS
 *     uint2 func             The func of the RS
 *     uint5 Qs               The Qs of the RS
 *     uint32 Vs              The Vs of the RS
 *     uint5 Qt               The Qt of the RS
 *     uint32 Vt              The Vt of the RS
 *     uint5 Qb               The cdb Q to broadcast
 *     uint32 Vb              The cdb V to broadcast
 */
void ReservationStationTest::test_ReservationStation(uint32 results[6], uint5 name, uint2 func, uint5 Qs, uint32 Vs, uint5 Qt, uint32 Vt, uint5 Qb, uint32 Vb) {

	// Create a reservation station
	ReservationStation rs(
		(uint2)(0b11 & (name >> 3)),		// First 2 bits
		(uint3)(0b111 & name)			// Last 3 bits
	);

	// Set a Qs or a Vs
	if (Qs != 0) {
		rs.setQs(Qs);
	}
	else {
		rs.setVs(Vs);
	}

	// Set a Qt or a Vt
	if (Qt != 0) {
		rs.setQt(Qt);
	}
	else {
		rs.setVt(Vt);
	}

	// Set func
	rs.setFunc(func);


	// Broadcast if given
	if (Qb != 0) {
		rs.checkBroadcast(Qb, Vb);
	}

	// Save ready state
	results[0] = (rs.isReady()) ? 1 : 0;
	// Save busy state
	results[1] = (rs.isBusy()) ? 1 : 0;
	// Save V of Rs
	results[2] = ((uint32)0b0) + rs.getVs();
	// Save V of Rt
	results[3] = ((uint32)0b0) + rs.getVt();
	// Save func
	results[4] = ((uint32)0b0) + rs.getFunc();
	// Save name
	results[5] = ((uint32)0b0) + rs.getName();
}

int ReservationStationTest::check(uint32 ready, uint32 busy, uint32 Vs, uint1 VsDontCare, uint32 Vt, uint1 VtDontCare, uint32 func, uint32 name, uint32 results[6]) {
	// Check isReady
	if(results[0] != ready){
		//printf("failed (isReady is %d instead of %d)", results[0], ready);
		return -1;
	}

	// Check isBusy
	else if(results[1] != busy){
		//printf("failed (isBusy is %d instead of %d)", results[1], busy);
		return -1;
	}

	// Check Vs
	else if(VsDontCare != 0b1 && results[2] != Vs){
		//printf("failed (Vs is %d instead of %d)", results[2], Vs);
		return -1;
	}

	// Check Vt
	else if(VtDontCare != 0b1 && results[3] != Vt){
		//printf("failed (Vt is %d instead of %d)", results[3], Vt);
		return -1;
	}
	
	// Check func
	else if(results[4] != func){
		//printf("failed (func is %d instead of %d)", results[4], func);
		return -1;
	}
	
	// Check name
	else if(results[5] != name){
		//printf("failed (name is %d instead of %d)", results[5], name);
		return -1;
	}

	// No error
	//printf("passed");
	return 1;
}

TEST_F(ReservationStationTest, RS_Qs_Qt) {
    //RS with Qs and Qt
    
    // Rs Name
    name = 0b01010;
	// Rs func
	func = 0b1;
	// Rs
	Qs = 0b1;
	Vs = 0b0;
	// Rt
	Qt = 0b10;
	Vt = 0b0;
	// Broadcast
	Qb = 0b0;
	Vb = 0b0;
	test_ReservationStation(results, name, func, Qs, Vs, Qt, Vt, Qb, Vb);
	// Check data
	EXPECT_EQ(check(
		0b0, // ready
		0b1, // busy
		0b0, // Vs
		0b1, // VsDontCare
		0b0, // Vt
		0b1, // VtDontCare
		0b1, // func
		0b010, // name
		results // results
	    ),
        1
    );
}


TEST_F(ReservationStationTest, RS_Vs_Vt) {
    //RS with Vs and Vt

    // Rs Name
	name = 0b01011;
	// Rs func
	func = 0b11;
	// Rs
	Qs = 0b0;
	Vs = 0b1;
	// Rt
	Qt = 0b0;
	Vt = 0b1;
	// Broadcast
	Qb = 0b0;
	Vb = 0b0;
	test_ReservationStation(results, name, func, Qs, Vs, Qt, Vt, Qb, Vb);
	// Check data
	EXPECT_EQ(check(
		0b1, // ready
		0b1, // busy
		0b1, // Vs
		0b0, // VsDontCare
		0b1, // Vt
		0b0, // VtDontCare
		0b11, // func
		0b011, // name
		results // results
	        ) , 1);
}

TEST_F(ReservationStationTest, RS_Vs_Qt_Vt) {
    //RS with Vs and Qt and broadcast Vt

    // Rs Name
	name = 0b01110;
	// Rs func
	func = 0b1;
	// Rs
	Qs = 0b0;
	Vs = 0b1;
	// Rt
	Qt = 0b110;
	Vt = 0b0;
	// Broadcast
	Qb = 0b110;
	Vb = 0b110;
	test_ReservationStation(results, name, func, Qs, Vs, Qt, Vt, Qb, Vb);
	// Check data
	EXPECT_EQ(check(
		0b1, // ready
		0b1, // busy
		0b1, // Vs
		0b0, // VsDontCare
		0b110, // Vt
		0b0, // VtDontCare
		0b1, // func
		0b110, // name
		results // results
	) ,1);
}

TEST_F(ReservationStationTest, RS_Qs_Vt_Vs) {
    //RS with Qs and Vt and broadcast Vs
    
    // Rs Name
	name = 0b01010;
	// Rs func
	func = 0b1;
	// Rs
	Qs = 0b110;
	Vs = 0b0;
	// Rt
	Qt = 0b0;
	Vt = 0b1;
	// Broadcast
	Qb = 0b110;
	Vb = 0b110;
	test_ReservationStation(results, name, func, Qs, Vs, Qt, Vt, Qb, Vb);
	// Check data
	EXPECT_EQ(check(
		0b1, // ready
		0b1, // busy
		0b110, // Vs
		0b0, // VsDontCare
		0b1, // Vt
		0b0, // VtDontCare
		0b1, // func
		0b010, // name
		results // results
	) ,1);
}

TEST_F(ReservationStationTest, RS_Qs_Vs_Vt) {
    //RS with Qs same as Qt and broadcast Vs which is also Vt

    // Rs Name
	name = 0b01010;
	// Rs func
	func = 0b1;
	// Rs
	Qs = 0b110;
	Vs = 0b0;
	// Rt
	Qt = 0b110;
	Vt = 0b1;
	// Broadcast
	Qb = 0b110;
	Vb = 0b110;
	test_ReservationStation(results, name, func, Qs, Vs, Qt, Vt, Qb, Vb);
	// Check data
	EXPECT_EQ(check(
		0b1, // ready
		0b1, // busy
		0b110, // Vs
		0b0, // VsDontCare
		0b110, // Vt
		0b0, // VtDontCare
		0b1, // func
		0b010, // name
		results // results
	) ,1);
}