//  cpu_unittest.hpp
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

#include <cpu_fixture.hpp>

TEST_F(CPUTest, Simple) {
	pc = 0;
	// Command         ADD R11  R1   R2   
	commands[pc] = ((0b0100010110000100010) << 13); pc++;
	// Command         ADD R12  R11  R3   
	commands[pc] = ((0b0100011000101100011) << 13); pc++;
	// Command         ADD R13  R12  R4   
	commands[pc] = ((0b0100011010110000100) << 13); pc++;
	// Command         SUB R14  R11  R1   
	commands[pc] = ((0b0101011100101100001) << 13); pc++;
	// Command         OR  R14  R3   R2   
	commands[pc] = ((0b0001011100001100010) << 13); pc++;
	// Command         NOP R0   R0   R0   
	commands[pc] = ((0b1100000000000000000) << 13); pc++;

	// End Command (Write 0b1 on R31)
	// Command         ADD R31  R0   R30  
	commands[pc] = ((0b0001111110000011110) << 13); pc++;
	// Command         NOP R0   R0   R0   
	commands[pc] = ((0b1100000000000000000) << 13); pc++;

    uint32 cycles = cpu->execute(data, commands, pc - 1);

    EXPECT_EQ(cycles, 7);

    uint32 expected[]={0,1,2,3,4,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0};
    for(int i=0; i<32; i++){
		EXPECT_EQ(data[i], expected[i]);
    }

}