//  ReorderBuffer.hpp
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


#ifndef __REORDER_BUFFER_HPP
#define __REORDER_BUFFER_HPP

// Libraries
#include <Types.hpp>
#include <Register.hpp>
#include <CommonDataBus.hpp>

// Reorder Buffer class
class ReorderBuffer {
	private:
		static constexpr int REORDER_BUFFER_SIZE = 5;
	public:	
		// Buffered Command stage
		struct BufferedCommand {
			bool active;

			bool done;
			uint5 tag;
			uint5 destination;
			uint32 result;

			uint32 command;
			uint5 exception_status;
			uint32 PC;
		};
		// List of commands
		BufferedCommand buffer[REORDER_BUFFER_SIZE];
		// Commands indexes
		int bufferTop;
		int bufferBottom;

		// Register
		struct RegisterStatus {
			bool reference;
			uint5 tag;
		};
		// List of registers
		RegisterStatus registers[32];

		// Register File pointer
		RegisterFile* rf;
		
		ReorderBuffer (RegisterFile* rf);

		// Check if register has reference
		bool hasReference (uint5 reg);
		// Get register's reference
		uint5 readReference (uint5 reg);

		// Check if it is full
		bool isFull ();
		// Load Command
		void load (uint5 tag, uint5 Rd, uint32 command, uint32 pc);


		// Common data bus bind
		CommonDataBus::BusBind* cdb;

		// Listen on CommonDataBus
		void subscribe (CommonDataBus::BusBind* CommonDataBus);

		// Send CommonDataBus values to rs
		void checkBroadcast ();

		// Save command on register file
		void checkBuffer ();
};


#endif