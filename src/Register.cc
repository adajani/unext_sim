//  Register.cc
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

#include <Register.hpp>
#include <iostream>

RegisterFile::RegisterFile () {
	// Init registers
	for (int i = 0; i < 32; ++i){
		this->registers[i].value = 0b0;
	}
};

// Read a register
uint32 RegisterFile::read (uint5 reg) {
	// Return value
	return this->registers[reg].value;
};

// Write a register
void RegisterFile::write (uint5 reg, uint32 value) {
	// If not R0
	if (reg != 0b0) {
		// Save
		this->registers[reg].value = value;
	}
};