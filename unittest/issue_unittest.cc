//  issue_unittest.cc
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

#include <issue_fixture.hpp>

/*
 * Top level testbench function
 * Test the command Issue unit
 *
 * Parameters
 *     uint32 results[1]      CPU executed cycles
 *     uint32 data[32]        Register file's registers initialize
 *     uint32 commands[64]    The commands of our program
 *     int run               Program's commands to run
 *     int rest              Cycles for the cpu to rest
 */
void IssueTest::test_Issue(uint32 results[1], uint32 data[32], uint32 commands[64], int run, int rest) {
	// Prepare issue
	Issue issue;

	// Load the given data 
	// on the register file's registers
	// BOOM! Content switch!
	for (int i = 1; i < 32; ++i) { // Ignore R0
		issue.rf->registers[i].value = data[i];
	}

	// Decoded command variables
	uint2 type;
	uint2 func;
	uint5 Rd;
	uint5 Rs;
	uint5 Rt;

	// Simulate the program counter
	int pc = 0;
	// Count cpu cycles
	int cycles = 0;


	while (pc < run) {
		// Decode my command
		decodeCommand(commands[pc], &type, &func, &Rd, &Rs, &Rt);
		// Try to issue command
		if(issue.issue(type, func, Rd, Rs, Rt)){
			if(_TESTBENCH_PRINTS_)
				std::cout << (cycles + 1) << "Command "<<(pc + 1)<<" issued "<< type << " "
                          << " " << func
                          << " " <<  Rd
                          << " " <<  Rs
                          << Rt << std::endl;
			// Yeah! Command is running
			pc++;
		} else {
			if(_TESTBENCH_PRINTS_)
				std::cout << (cycles + 1) << " Stall" << std::endl;
		}
		cycles++;

		// Print the reorder buffer's data
		//printReorderBuffer(&issue);

		// If it stuck
		if(cycles > run*100){
			// Kill it with fire
			break;
		}
	}

	// Rest the cpu
	// last command should be nop
	for (int i = 0; i < rest; ++i) {
		// Decode last command
		decodeCommand(commands[run], &type, &func, &Rd, &Rs, &Rt);
		// Run last command
		issue.issue(type, func, Rd, Rs, Rt);
		cycles++;

		// Print the reorder buffer's data
		//printReorderBuffer(&issue);
	}

	// Save register data
	// AGAIN! Save Content 4 switch!
	for (int i = 0; i < 32; ++i) {
		data[i] = issue.rf->registers[i].value;
	}

	// Return cycles
	results[0] = cycles;
}

// Decode the given command
void IssueTest::decodeCommand(uint32 command, uint2* type, uint2* func, uint5* Rd, uint5* Rs, uint5* Rt) {
	// Yeah, yeah I know this is not the right format
	// ... but this is what I want now
	// TODO: Convert the command format on a Mips-Like format
	*type = 0b11 & (command >> 30);
	*func = 0b11 & (command >> 28);
	*Rd = 0b11111 & (command >> 23);
	*Rs = 0b11111 & (command >> 18);
	*Rt = 0b11111 & (command >> 13);
}

TEST_F(IssueTest, UnrelatedCommandsWithNOP) {
    // Command         ADD R5   R1   R2   
    commands[pc] = ((0b0100001010000100010) << 13);pc++;
    // Command         NOP R0   R0   R0   
    commands[pc] = ((0b1100000000000000000) << 13);pc++;
    // Command         NOP R0   R0   R0   
    commands[pc] = ((0b1100000000000000000) << 13);pc++;
    // Command         NOP R0   R0   R0   
    commands[pc] = ((0b1100000000000000000) << 13);pc++;
    // Command         NOP R0   R0   R0   
    commands[pc] = ((0b1100000000000000000) << 13);pc++;
    // Command         NOP R0   R0   R0   
    commands[pc] = ((0b1100000000000000000) << 13);pc++;
    // Command         ADD R6   R3   R4   
    commands[pc] = ((0b0100001100001100100) << 13);pc++;
    // Command         NOP R0   R0   R0   
    commands[pc] = ((0b1100000000000000000) << 13);pc++;
    // Command         NOP R0   R0   R0   
    commands[pc] = ((0b1100000000000000000) << 13);pc++;
    // Command         NOP R0   R0   R0   
    commands[pc] = ((0b1100000000000000000) << 13);pc++;
    // Command         NOP R0   R0   R0   
    commands[pc] = ((0b1100000000000000000) << 13);pc++;
    // Command         NOP R0   R0   R0   
    commands[pc] = ((0b1100000000000000000) << 13);pc++;
    // Command         ADD R7   R1   R3   
    commands[pc] = ((0b0100001110000100101) << 13);pc++;
    // Command         NOP R0   R0   R0   
    commands[pc] = ((0b1100000000000000000) << 13);pc++;
    // Command         NOP R0   R0   R0   
    commands[pc] = ((0b1100000000000000000) << 13);pc++;
    // Command         NOP R0   R0   R0   
    commands[pc] = ((0b1100000000000000000) << 13);pc++;
    // Command         NOP R0   R0   R0   
    commands[pc] = ((0b1100000000000000000) << 13);pc++;
    // Command         NOP R0   R0   R0   
    commands[pc] = ((0b1100000000000000000) << 13);pc++;
    // Command         AND R7   R1   R3   
    commands[pc] = ((0b0000001110000100101) << 13);pc++;
    // Command         NOP R0   R0   R0   
    commands[pc] = ((0b1100000000000000000) << 13);pc++;
    // Command         NOP R0   R0   R0   
    commands[pc] = ((0b1100000000000000000) << 13);pc++;
    // Command         NOP R0   R0   R0   
    commands[pc] = ((0b1100000000000000000) << 13);pc++;
    // Command         NOP R0   R0   R0   
    commands[pc] = ((0b1100000000000000000) << 13);pc++;
    // Command         NOP R0   R0   R0   
    commands[pc] = ((0b1100000000000000000) << 13);pc++;
    // Command         NOP R0   R0   R0   
    commands[pc] = ((0b1100000000000000000) << 13);pc++;   
    
    // Run it!
	test_Issue(results, data, commands, pc - 1, 10);
    
    EXPECT_EQ(data[5] , 0b11);
    EXPECT_EQ(data[6] , 0b111);
    EXPECT_EQ(data[7] , 1);
}

TEST_F(IssueTest, UnrelatedCommands) {
    // Command         ADD R8   R1   R2   
	commands[pc] = ((0b0100010000000100010) << 13);pc++;
	// Command         ADD R9   R3   R4   
	commands[pc] = ((0b0100010010001100100) << 13);pc++;
	// Command         ADD R10  R1   R3   
	commands[pc] = ((0b0100010100000100101) << 13);pc++;
	// Command         NOP R0   R0   R0   
	commands[pc] = ((0b1100000000000000000) << 13);pc++;
	// Run it!
	test_Issue(results, data, commands, pc - 1, 10);
	
    EXPECT_EQ(data[8] , 0b11);
    EXPECT_EQ(data[9] , 0b111);
   // EXPECT_EQ(data[10] , 0b100);//1
}

TEST_F(IssueTest, MultiUnrelatedCommands) {
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
	// Run it!
	test_Issue(results, data, commands, pc - 1, 10);

    EXPECT_EQ(data[11] , 3);
    EXPECT_EQ(data[12], 6);
    EXPECT_EQ(data[13] , 10);
    EXPECT_EQ(data[14] , 3);
}