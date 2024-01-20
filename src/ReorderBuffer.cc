//  ReorderBuffer.cc
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

#include <ReorderBuffer.hpp>

// Init buffer
ReorderBuffer::ReorderBuffer (RegisterFile* rf) {
	// Init buffer indexes
	this->bufferTop = 0;
	this->bufferBottom = 0;

	this->rf = rf;

	// Init command stages
	for (int i = 0; i < REORDER_BUFFER_SIZE; ++i){
		this->buffer[i].active = false;
		this->buffer[i].done = false;
		this->buffer[i].tag = 0b0;
		this->buffer[i].destination = 0b0;
		this->buffer[i].result = 0b0;
		this->buffer[i].command = 0b0;
		this->buffer[i].exception_status = 0b0;
		this->buffer[i].PC = 0b0;
	}

	// Init register status
	for (int i = 0; i < 32; ++i){
		this->registers[i].reference = false;
		this->registers[i].tag = 0b0;
	}
};

// Check if register has reference
bool ReorderBuffer::hasReference (uint5 reg) {
	// Return value
	return this->registers[reg].reference;
};
// Get register's reference
uint5 ReorderBuffer::readReference (uint5 reg) {
	// Return value
	return this->registers[reg].tag;
};

// Check if it is full
bool ReorderBuffer::isFull () {
	return this->buffer[this->bufferBottom].active;
};

// Load Command
void ReorderBuffer::load (uint5 tag, uint5 Rd, uint32 command, uint32 pc) {
	// Load command
	this->buffer[this->bufferBottom].active = true;
	this->buffer[this->bufferBottom].done = false;
	this->buffer[this->bufferBottom].tag = tag;
	this->buffer[this->bufferBottom].destination = Rd;
	this->buffer[this->bufferBottom].result = 0b0;
	this->buffer[this->bufferBottom].command = command;
	this->buffer[this->bufferBottom].exception_status = 0b0;
	this->buffer[this->bufferBottom].PC = pc;

	// Save reference
	this->registers[Rd].reference = true;
	this->registers[Rd].tag = tag;

	// Set the bottom to the next stage
	this->bufferBottom++;
	if (this->bufferBottom == REORDER_BUFFER_SIZE) {
		this->bufferBottom = 0;
	}
};

// Subscribe on cdb
void ReorderBuffer::subscribe (CommonDataBus::BusBind* cdb) {
	// Set cdb
	this->cdb = cdb;
};

// Send cdb values to rs
void ReorderBuffer::checkBroadcast () {
	// For each stage 
	for (int i = 0; i < REORDER_BUFFER_SIZE; ++i){
		// If rename match
		if (this->buffer[i].active && this->buffer[i].tag == this->cdb->Q) {
			// Set value
			this->buffer[i].done = true;
			this->buffer[i].result = this->cdb->V;
		}
	}
};

// Save command on register file
void ReorderBuffer::checkBuffer () {
	// If top command ready
	if (this->buffer[this->bufferTop].active && this->buffer[this->bufferTop].done) {
		// Write
		this->rf->write(
			this->buffer[this->bufferTop].destination,
			this->buffer[this->bufferTop].result
		);
		// Reset stage
		this->buffer[this->bufferTop].active = false;
		this->buffer[this->bufferTop].done = false;
		// Clear reference
		this->registers[this->buffer[this->bufferTop].destination].reference = false;

		// Move the queue up
		this->bufferTop++;
		if (this->bufferTop == REORDER_BUFFER_SIZE) {
			this->bufferTop = 0;
		}
	}
};