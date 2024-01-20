//  register_unittest.cc
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

#include <register_fixture.hpp>

/*
 * Top level testbench function
 * Test the Register File
 *
 * Parameters
 *     uint32 data[32]        Register file's registers initialize
 *     bool action[32]       Read R[i] if action[i] is true or write if false
 *     uint32 R[32]           Register list
 *     uint32 value[32]       Value to read or write
 *     int length            Number of register file accesses
 */
void RegisterTest::test_RegisterFile(uint32 data[32], bool action[32], uint5 R[32], uint32 value[32], int length) {
	// Load the given data 
	// on the register file's registers
	for (int i = 1; i < 32; ++i) { // Ignore R0
		rf.registers[i].value = data[i];
	}

	// Access register file
	for (int i = 0; i < length; ++i) {
		// Read register
		if (action[i]) {
			value[i] = rf.read(R[i]);
		}
		// Write register
		else {
			rf.write(R[i], value[i]);
		}
	}

	// Save register data
	for (int i = 0; i < 32; ++i) {
		data[i] = rf.registers[i].value;
	}
}

TEST_F(RegisterTest, UnrelatedAccesses) {
    int l=0;
	// Register data
	data[1] = 0b1;
	data[2] = 0b10;
	data[3] = 0b11;
	data[4] = 0b100;
	// Make accesses
	// 0. value = read(R=1)
	action[l] = true;  R[l] = 0b001; value[l] = 0b00; l++;
	// 1. value = read(R=2)
	action[l] = true;  R[l] = 0b010; value[l] = 0b00; l++;
	// 2. write(R=6, value=3)
	action[l] = false; R[l] = 0b110; value[l] = 0b11; l++;
	l = 3;
	// Run it!
	test_RegisterFile(data, action, R, value, l);
	
    EXPECT_EQ(value[0] ,  0b1);
    EXPECT_EQ(value[1] , 0b10);
    EXPECT_EQ(data[6] , 0b11);
}

TEST_F(RegisterTest, RelatedAccesses) {
	data[1] = 0b1;
	data[2] = 0b10;
	data[3] = 0b11;
	data[4] = 0b100;
	// Make accesses
	int l = 0;
	// 0. value = read(R=1)
	action[l] = true;  R[l] = 0b01; value[l] = 0b000; l++;
	// 1. write(R=1, value=4)
	action[l] = false; R[l] = 0b01; value[l] = 0b100; l++;
	// 2. value = read(R=2)
	action[l] = true;  R[l] = 0b10; value[l] = 0b000; l++;
	// 3. write(R=2, value=5)
	action[l] = false; R[l] = 0b10; value[l] = 0b101; l++;
	// Run it!
	test_RegisterFile(data, action, R, value, l);
	
    EXPECT_EQ(value[0] , 0b1);
    EXPECT_EQ(data[1] , 0b100);
    EXPECT_EQ(value[2], 0b10);
    EXPECT_EQ(data[2] , 0b101);
}
