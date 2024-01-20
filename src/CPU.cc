//  CPU.cc
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


#include <CPU.hpp>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
using namespace std;



// TODO: Things to imporve/fix
// - Change command format to original
// - Define options for max cycles and end cases

// Help functions
// Decode the given command
void CPU::decodeCpuCommand(uint32 command, uint2* type, uint2* func, uint5* Rd, uint5* Rs, uint5* Rt) {
	// This is not the original MIPS command format ... I simlified it for testing
	// TODO: Convert the command format to a original Mips-Like format
	*type = 0b11 & (command >> 30);
	*func = 0b11 & (command >> 28);
	*Rd = 0b11111 & (command >> 23);
	*Rs = 0b11111 & (command >> 18);
	*Rt = 0b11111 & (command >> 13);
}

/*
 * Top level function
 * Run the cpu
 *
 * Parameters
 *     uint32 results[1]      CPU executed cycles
 *     uint32 data[32]        Register file's registers initialize
 *     uint32 commands[64]    The commands of our program
 *     int run               Program's commands to run
 *
 * CPU will stop when R31 has the value of 0b1.
 * If we run out of commands for execution, the CPU will keep running the last given command.
 * So it is adviced to give a NOP as the last command of the program.
 */
CPU::CPU(int pc, bool debug) {
	isDebug_m = debug;
	issue = new Issue();

	// Simulate the program counter
	this->pc = pc;
}

uint32 CPU::execute(uint32 data[32], uint32 *commands, int run) {
	for (int i = 1; i < 32; ++i) { // Ignore R0
		issue->rf->registers[i].value = data[i];
	}

	// Decoded command variables
	uint2 type;
	uint2 func;
	uint5 Rd;
	uint5 Rs;
	uint5 Rt;

	while (pc < run) {
		// Decode my command
		decodeCpuCommand(commands[pc], &type, &func, &Rd, &Rs, &Rt);
		// Try to issue command
		if(issue->issue(type, func, Rd, Rs, Rt)){
			if(isDebug_m)
				printf("  [%d] Command %d issued [%d %d %d %d %d]\n", cycles + 1, pc + 1, type, func, Rd, Rs, Rt);
			pc++;
		} else {
			if(isDebug_m)
				printf("  [%d] Stall\n", cycles + 1);
		}
		cycles++;

		// If it stuck
		if(cycles > run*100) {
			if(isDebug_m)
				printf("stopped @ %d cycles > %d\n", cycles, run*100);
			break;
		}
	}

	// Save register data
	for (int i = 0; i < 32; ++i) {
		data[i] = issue->rf->registers[i].value;
	}

	return cycles;
}