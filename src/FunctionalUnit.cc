//  FunctionalUnit.cc
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


// Libraries
#include <FunctionalUnit.hpp>

// General FunctionalUnit

	// Init FunctionalUnit
	FunctionalUnit::FunctionalUnit (uint2 tag) {
		// Set name
		this->tag = tag;
	};

	// Subscribe on cdb
	void FunctionalUnit::subscribe (CommonDataBus::BusBind* cdb) {
		// Set cdb
		this->cdb = cdb;
	};




// Arithmetic FunctionalUnit

	// Init
	FunctionalArithmeticUnit::FunctionalArithmeticUnit (uint2 tag) : FunctionalUnit(tag) {
		this->rs = new ReservationStation*[FUNCTIONAL_ARITHMETIC_UNIT_RS];
		// Init each rs
		for (int i = 0; i < FUNCTIONAL_ARITHMETIC_UNIT_RS; ++i){
			// Set tag name
			this->rs[i] = new ReservationStation(tag, i + 1);
		}

		// Init calculate stages
		for (int i = 0; i < FUNCTIONAL_ARITHMETIC_UNIT_LATENCY; ++i){
			// Set tag name
			this->calculateStages[i].active = false;
			this->calculateStages[i].Q = 0b0;
			this->calculateStages[i].func = 0b0;
			this->calculateStages[i].Vs = 0b0;
			this->calculateStages[i].Vt = 0b0;
			this->calculateStages[i].R = 0b0;
		}
	};

	// Get availiable rs (if any)
	uint5 FunctionalArithmeticUnit::isAvailiable () {
		// For each rs
		for (int i = 0; i < FUNCTIONAL_ARITHMETIC_UNIT_RS; ++i){
			// If not busy
			if (!(this->rs[i]->isBusy())) {
				// Return full tag
				return ((this->tag << 3) | this->rs[i]->getName());
			}
		}

		// No rs availiable
		return 0b0;
	};

	// Issue a command
	void FunctionalArithmeticUnit::issue (uint2 func, uint5 Qs, uint32 Vs, uint5 Qt, uint32 Vt) {
		int index = -1;

		// For each rs
		for (int i = 0; i < FUNCTIONAL_ARITHMETIC_UNIT_RS; ++i){
			// If not busy
			if (!(this->rs[i]->isBusy())) {
				// Return full tag
				index = i;
				break;
			}
		}

		// Set func
		this->rs[index]->setFunc(func);

		// Set a Qs or a Vs
		if (Qs != 0) {
			this->rs[index]->setQs(Qs);
		}
		else {
			this->rs[index]->setVs(Vs);
		}

		// Set a Qt or a Vt
		if (Qt != 0) {
			this->rs[index]->setQt(Qt);
		}
		else {
			this->rs[index]->setVt(Vt);
		}
	}

	// Make calculations
	void FunctionalArithmeticUnit::calculate () {
		// Check if we need to request access
		if (
			// If we will have a result the next round
			this->calculateStages[FUNCTIONAL_ARITHMETIC_UNIT_LATENCY - 2].active ||
			// If we have the result but no access
			(this->calculateStages[FUNCTIONAL_ARITHMETIC_UNIT_LATENCY - 1].active && !this->cdb->access)
		) {
			this->cdb->request = true;
		} else {
			this->cdb->request = false;
		}

		bool noStall = 0;
		// If last data broadcasted or trash
		if (this->cdb->access || !(this->calculateStages[FUNCTIONAL_ARITHMETIC_UNIT_LATENCY - 1].active)){
			// No stall
			noStall = 1;
		}

		// Loop from bottom
		for (int i = FUNCTIONAL_ARITHMETIC_UNIT_LATENCY - 2; i >= 0; i--){
			// If no stall
			if (noStall) {
				// Send to next stage
				this->calculateStages[i + 1].active = this->calculateStages[i].active;
				this->calculateStages[i].active = false;
				this->calculateStages[i + 1].Q = this->calculateStages[i].Q;
				this->calculateStages[i].Q = 0b0;
				this->calculateStages[i + 1].func = this->calculateStages[i].func;
				this->calculateStages[i].func = 0b0;
				this->calculateStages[i + 1].Vs = this->calculateStages[i].Vs;
				this->calculateStages[i].Vs = 0b0;
				this->calculateStages[i + 1].Vt = this->calculateStages[i].Vt;
				this->calculateStages[i].Vt = 0b0;
				this->calculateStages[i + 1].R = this->calculateStages[i].R;
				this->calculateStages[i].R = 0b0;
			}

			// Stall next stage?
			noStall = (noStall || !(this->calculateStages[i].active));
		}

		// Check if load new job
		if (noStall) {
			this->loadJob();
		}
	};

	// Load job
	void FunctionalArithmeticUnit::loadJob () {
		int index = -1;
		bool found = false;

		// For each rs
		for (int i = 0; i < FUNCTIONAL_ARITHMETIC_UNIT_RS; ++i){
			if (this->rs[i]->isReady() && !this->rs[i]->isRunning()) {
				found = true;
				index = i;
				break;
			}
		}

		enum FunctionalArithmeticUnit_func {ADD=0b0, SUB=0b1};

		// If ready rs found
		if (found) {
			// Insert
			this->calculateStages[0].active = true;
			this->calculateStages[0].Q = ((this->tag << 3) | this->rs[index]->getName());
			this->calculateStages[0].func = this->rs[index]->getFunc();
			this->calculateStages[0].Vs = this->rs[index]->getVs();
			this->calculateStages[0].Vt = this->rs[index]->getVt();

			switch(this->calculateStages[0].func) {
				case FunctionalArithmeticUnit_func::ADD:
					this->calculateStages[0].R = this->calculateStages[0].Vs + this->calculateStages[0].Vt;
					break;

				case FunctionalArithmeticUnit_func::SUB:
					this->calculateStages[0].R = this->calculateStages[0].Vs - this->calculateStages[0].Vt;
					break;
				
				default:
					this->calculateStages[0].R = 0b0;
			}
		
			// Set as running
			this->rs[index]->setAsRunning();
		}
	};

	// Send cdb values to rs
	void FunctionalArithmeticUnit::checkBroadcast () {
		// For each rs
		for (int i = 0; i < FUNCTIONAL_ARITHMETIC_UNIT_RS; ++i){
			// Send data
			this->rs[i]->checkBroadcast(this->cdb->Q, this->cdb->V);
		}
	};

	// Prepare data for cdb
	void FunctionalArithmeticUnit::checkAccess () {
		// Check if we have access
		if (this->cdb->access) {
			// Write to cdb
			this->cdb->write_Q = this->calculateStages[FUNCTIONAL_ARITHMETIC_UNIT_LATENCY - 1].Q;
			this->cdb->write_V = this->calculateStages[FUNCTIONAL_ARITHMETIC_UNIT_LATENCY - 1].R;
		} else {
			this->cdb->write_Q = 0b0;
			this->cdb->write_V = 0b0;
		}
	};




// Logic FunctionalUnit

	// Init
	FunctionalLogicUnit::FunctionalLogicUnit (uint2 tag) : FunctionalUnit(tag) {
		this->rs = new ReservationStation*[FUNCTIONAL_LOGIC_UNIT_RS];
		// Init each rs
		for (int i = 0; i < FUNCTIONAL_LOGIC_UNIT_RS; ++i){
			// Set tag name
			this->rs[i] = new ReservationStation(tag, i + 1);
		}

		// Init calculate stages
		for (int i = 0; i < FUNCTIONAL_LOGIC_UNIT_LATENCY; ++i){
			// Set tag name
			this->calculateStages[i].active = false;
			this->calculateStages[i].Q = 0b0;
			this->calculateStages[i].func = 0b0;
			this->calculateStages[i].Vs = 0b0;
			this->calculateStages[i].Vt = 0b0;
			this->calculateStages[i].R = 0b0;
		}
	};

	// Get availiable rs (if any)
	uint5 FunctionalLogicUnit::isAvailiable () {
		// For each rs
		for (int i = 0; i < FUNCTIONAL_LOGIC_UNIT_RS; ++i){
			// If not busy
			if (!(this->rs[i]->isBusy())) {
				// Return full tag
				return ((this->tag << 3) | this->rs[i]->getName());
			}
		}

		// No rs availiable
		return 0b0;
	};

	// Issue a command
	void FunctionalLogicUnit::issue (uint2 func, uint5 Qs, uint32 Vs, uint5 Qt, uint32 Vt) {
		int index = -1;

		// For each rs
		for (int i = 0; i < FUNCTIONAL_LOGIC_UNIT_RS; ++i){
			// If not busy
			if (!(this->rs[i]->isBusy())) {
				// Return full tag
				index = i;
				break;
			}
		}

		// Set func
		this->rs[index]->setFunc(func);

		// Set a Qs or a Vs
		if (Qs != 0) {
			this->rs[index]->setQs(Qs);
		}
		else {
			this->rs[index]->setVs(Vs);
		}

		// Set a Qt or a Vt
		if (Qt != 0) {
			this->rs[index]->setQt(Qt);
		}
		else {
			this->rs[index]->setVt(Vt);
		}
	}

	// Make calculations
	void FunctionalLogicUnit::calculate () {
		// Check if we need to request access
		if (
			// If we will have a result the next round
			this->calculateStages[FUNCTIONAL_LOGIC_UNIT_LATENCY - 2].active ||
			// If we have the result but no access
			(this->calculateStages[FUNCTIONAL_LOGIC_UNIT_LATENCY - 1].active && !this->cdb->access)
		) {
			this->cdb->request = true;
		}
		else {
			this->cdb->request = false;
		}

		bool noStall = 0;
		// If last data broadcasted or trash
		if (this->cdb->access || !(this->calculateStages[FUNCTIONAL_LOGIC_UNIT_LATENCY - 1].active)){
			// No stall
			noStall = 1;
		}

		// Loop from bottom
		for (int i = FUNCTIONAL_LOGIC_UNIT_LATENCY - 2; i >= 0; i--){
			// If no stall
			if (noStall) {
				// Send to next stage
				this->calculateStages[i + 1].active = this->calculateStages[i].active;
				this->calculateStages[i].active = false;
				this->calculateStages[i + 1].Q = this->calculateStages[i].Q;
				this->calculateStages[i].Q = 0b0;
				this->calculateStages[i + 1].func = this->calculateStages[i].func;
				this->calculateStages[i].func = 0b0;
				this->calculateStages[i + 1].Vs = this->calculateStages[i].Vs;
				this->calculateStages[i].Vs = 0b0;
				this->calculateStages[i + 1].Vt = this->calculateStages[i].Vt;
				this->calculateStages[i].Vt = 0b0;
				this->calculateStages[i + 1].R = this->calculateStages[i].R;
				this->calculateStages[i].R = 0b0;
			}

			// Stall next stage?
			noStall = (noStall || !(this->calculateStages[i].active));
		}

		// Check if load new job
		if (noStall) {
			this->loadJob();
		}
	};

	// Load job
	void FunctionalLogicUnit::loadJob () {
		int index = -1;
		bool found = false;

		// For each rs
		for (int i = 0; i < FUNCTIONAL_LOGIC_UNIT_RS; ++i){
			if (this->rs[i]->isReady() && !this->rs[i]->isRunning()) {
				found = true;
				index = i;
				break;
			}
		}

		// If ready rs found
		if (found) {
			// Insert
			this->calculateStages[0].active = true;
			this->calculateStages[0].Q = ((this->tag << 3) | this->rs[index]->getName());
			this->calculateStages[0].func = this->rs[index]->getFunc();
			this->calculateStages[0].Vs = this->rs[index]->getVs();
			this->calculateStages[0].Vt = this->rs[index]->getVt();
			
			switch(this->calculateStages[0].func) {
				case FunctionalLogicUnit_func::AND:
					this->calculateStages[0].R = this->calculateStages[0].Vs & this->calculateStages[0].Vt;
					break;

				case FunctionalLogicUnit_func::OR:
					this->calculateStages[0].R = this->calculateStages[0].Vs | this->calculateStages[0].Vt;
					break;

				case FunctionalLogicUnit_func::NOT:
					this->calculateStages[0].R = ~this->calculateStages[0].Vs;
					break;

				default:
					this->calculateStages[0].R = 0b0;
			}
			
			// Set as running
			this->rs[index]->setAsRunning();
		}
	}

	// Send cdb values to rs
	void FunctionalLogicUnit::checkBroadcast () {
		// For each rs
		for (int i = 0; i < FUNCTIONAL_LOGIC_UNIT_RS; ++i){
			// Send data
			this->rs[i]->checkBroadcast(this->cdb->Q, this->cdb->V);
		}
	}

	// Prepare data for cdb
	void FunctionalLogicUnit::checkAccess () {
		// Check if we have access
		if (this->cdb->access) {
			// Write to cdb
			this->cdb->write_Q = this->calculateStages[FUNCTIONAL_LOGIC_UNIT_LATENCY - 1].Q;
			this->cdb->write_V = this->calculateStages[FUNCTIONAL_LOGIC_UNIT_LATENCY - 1].R;
		} else {
			this->cdb->write_Q = 0b0;
			this->cdb->write_V = 0b0;
		}
	}