//  Issue.cc
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

#include <Issue.hpp>

// Init Issue
Issue::Issue () {
	this->rf = new RegisterFile();
	this->rb = new ReorderBuffer(this->rf);
	this->fau= new FunctionalArithmeticUnit(ISSUE_AU_CODE);
	this->flu= new FunctionalLogicUnit(ISSUE_LU_CODE);
	this->cdb= new CommonDataBus();

	// Bind units with cdb
	this->rb->subscribe(this->cdb->bind());
	this->fau->subscribe(this->cdb->bind());
	this->flu->subscribe(this->cdb->bind());
};

// Issue
bool Issue::issue (uint2 type, uint2 func, uint5 Rd, uint5 Rs, uint5 Rt) {
	// Common data bus broadcast data
	this->cdb->broadcast();
	// Get data from common data bus
	this->rb->checkBroadcast();
	this->fau->checkBroadcast();
	this->flu->checkBroadcast();
	// Reorder buffer result write
	this->rb->checkBuffer();

	// Init command issued flag
	bool issued = false;

	// Init command's reservation station tag variable
	uint5 tag = 0b00;

	// Init command's registers' data variables
	bool Rs_data_dirty = false;
	uint5 Rs_data_Q = 0b0;
	uint32 Rs_data_V = 0b0;
	bool Rt_data_dirty = false;
	uint5 Rt_data_Q = 0b0;
	uint32 Rt_data_V = 0b0;


	// If arithmetic command
	if (type == ISSUE_AU_CODE) {

		// Get avaliable rs
		tag = this->fau->isAvailiable();

		// If a reservation station is availiable
		// and if reorder buffer is not full
		if (tag != 0b0 && !this->rb->isFull()) {

			// Get Rs register data
			if (this->rb->hasReference(Rs)) {
				// Reference data
				Rs_data_dirty = true;
				Rs_data_Q = this->rb->readReference(Rs);
			} else {
				// Value data
				Rs_data_V = this->rf->read(Rs);
			}

			// Get Rt register data
			if (this->rb->hasReference(Rt)) {
				// Reference data
				Rt_data_dirty = true;
				Rt_data_Q = this->rb->readReference(Rt);
			} else {
				// Value data
				Rt_data_V = this->rf->read(Rt);
			}

			// Issue the command
			this->fau->issue(
				func,
				( Rs_data_dirty ? Rs_data_Q : 0b0),
				(!Rs_data_dirty ? Rs_data_V : 0b0),
				( Rt_data_dirty ? Rt_data_Q : 0b0),
				(!Rt_data_dirty ? Rt_data_V : 0b0)
			);

			// Add command on Reorder buffer
			this->rb->load(tag, Rd, 0b0, 0b0);
			// Set command issued flag
			issued = true;
		}
	}

	// If logic command
	else if (type == ISSUE_LU_CODE) {

		// Get avaliable rs
		tag = this->flu->isAvailiable();

		// If a reservation station is availiable
		// and if reorder buffer is not full
		if (tag != 0b0 && !this->rb->isFull()) {

			// Get Rs register data
			if (this->rb->hasReference(Rs)) {
				// Reference data
				Rs_data_dirty = true;
				Rs_data_Q = this->rb->readReference(Rs);
			} else {
				// Value data
				Rs_data_V = this->rf->read(Rs);
			}

			// Get Rt register data
			if (this->rb->hasReference(Rt)) {
				// Reference data
				Rt_data_dirty = true;
				Rt_data_Q = this->rb->readReference(Rt);
			} else {
				// Value data
				Rt_data_V = this->rf->read(Rt);
			}

			// Issue the command
			this->flu->issue(
				func,
				( Rs_data_dirty ?Rs_data_Q : 0b0),
				(!Rs_data_dirty ?Rs_data_V : 0b0),
				( Rt_data_dirty ?Rt_data_Q : 0b0),
				(!Rt_data_dirty ?Rt_data_V : 0b0)
			);

			// Add command on Reorder buffer
			this->rb->load(tag, Rd, 0b0, 0b0);

			// Set command issued flag
			issued = true;
		}
	}

	// Unknown command
	else {
		// On unknown commands do nothing
		// TODO: In the future maybe raise a cpu exception

		// Set command issued flag
		issued = true;
	}

	// Make calculations on the functional units
	this->fau->calculate();
	this->flu->calculate();

	// Common data bus grand access
	this->cdb->grandAccess();
	// Functional units check common data bus accesses
	this->fau->checkAccess();
	this->flu->checkAccess();

	// Return issued flag
	return issued;
};