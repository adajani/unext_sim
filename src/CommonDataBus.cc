//  CommonDataBus.cc
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


#include <CommonDataBus.hpp>

CommonDataBus::CommonDataBus () {
	// Init units number
	this->units = 0;
	// Init Access token
	this->token = -1;
	this->active = false;

	this->Q = 0b0;
	this->V = 0b0;

	// For each unit
	for (int i = 0; i < CDB_UNITS_COUNT; i++){
		// Init data
		this->unit_list[i].write_Q = 0;
		this->unit_list[i].write_V = 0;
		this->unit_list[i].request = false;
		this->unit_list[i].access = false;
	}
};

// Bind Bus
CommonDataBus::BusBind* CommonDataBus::bind () {
	// One additional unit
	this->units++;
	// Return
	return &(this->unit_list[this->units - 1]);
};

// Broadcast data to the bus
void CommonDataBus::broadcast () {
	uint5 Q = 0b0;
	uint32 V = 0b0;

	// If broadcasting
	if(this->active){
		Q = this->unit_list[this->token].write_Q;
		V = this->unit_list[this->token].write_V;
	}

	// For each unit
	for (int i = 0; i < CDB_UNITS_COUNT; i++){
		// Send data
		this->unit_list[i].Q = Q;
		this->unit_list[i].V = V;
	}

	// Set local values
	this->Q = Q;
	this->V = V;
}

// Bind Bus
void CommonDataBus::grandAccess () {
	bool granted = false;
	uint8 token = this->token;
	uint8 index = 0;

	// For each unit
	for (int i = 1; i <= CDB_UNITS_COUNT; i++){
		// Don't get out of range
		index = (token + i) % CDB_UNITS_COUNT;

		// If access not given and asked for access
		if (!granted && this->unit_list[index].request) {
			granted = true;
			this->unit_list[index].access = true;
			this->unit_list[index].request = false;
			this->token = index;
		}
		else {
			this->unit_list[index].access = false;
		}
	}

	// CommonDataBus state
	if (granted) {
		this->active = true;
	}
	else {
		this->active = false;
	}

};