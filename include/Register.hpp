//  Register.hpp
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


#ifndef __REGISTER_HPP
#define __REGISTER_HPP

#include <Types.hpp>

class RegisterFile {
	public:

		// Register
		class Register {
			public:
				Register() = default;
				uint32 value;
		};

		// List of registers
		Register registers[32];
		
		RegisterFile ();

		// Read a register
		uint32 read (uint5 reg);

		// Write a register
		void write (uint5 reg, uint32 value);
};


#endif // _REGISTER_FILE_H_