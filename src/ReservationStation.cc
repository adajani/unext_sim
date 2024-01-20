//  ReservationStation.cc
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

#include <ReservationStation.hpp>

// Init
ReservationStation::ReservationStation (uint2 parentTag, uint3 tag) {
	// Set name
	this->tag = tag;
	// Set parent name
	this->parentTag = parentTag;
	// Clear ReservationStation
	this->clear();
};

// Clear
void ReservationStation::clear () {
	// Set not busy
	this->busy = false;
	// Set not running
	this->running = false;
	// Init registers
	this->s.V = 0b0;
	this->t.V = 0b0;
	this->s.Q = 0b0;
	this->t.Q = 0b0;
	// Set no values
	this->s_value = false;
	this->t_value = false;
	// Reset func
	this->func = 0b0;
};

// Check if busy
bool ReservationStation::isBusy () {
	return this->busy;
};

// Check if ready
bool ReservationStation::isReady () {
	return (this->s_value && this->t_value);
};

// Check if running
bool ReservationStation::isRunning () {
	return this->running;
};

// Set func
void ReservationStation::setFunc (uint2 func) {
	// Set func
	this->func = func;
}

// Set Q of ReservationStation
void ReservationStation::setQs (uint5 Q) {
	// Set Q
	this->s.Q = Q;
	// Set not loaded
	this->s_value = false;
	// Set as busy
	this->busy = true;
}

// Set Q of Rt
void ReservationStation::setQt (uint5 Q) {
	// Set Q
	this->t.Q = Q;
	// Set not loaded
	this->t_value = false;
	// Set as busy
	this->busy = true;
}

// Set V of ReservationStation
void ReservationStation::setVs (uint32 V) {
	// Set V
	this->s.V = V;
	// Set loaded
	this->s_value = true;
	// Set as busy
	this->busy = true;
}

// Set V of Rt
void ReservationStation::setVt (uint32 V) {
	// Set V
	this->t.V = V;
	// Set loaded
	this->t_value = true;
	// Set as busy
	this->busy = true;
}

// Set running
void ReservationStation::setAsRunning () {
	// Set running to true
	this->running = true;
}

// Check broadcast
void ReservationStation::checkBroadcast (uint5 Q, uint32 V) {
	// Don't check if not busy
	if (!this->busy) {
		return;
	}
	// Check for ReservationStation
	if (!this->s_value && Q == this->s.Q) {
		// Save value
		this->setVs(V);
	}
	// Check for Rt
	if (!this->t_value && Q == this->t.Q) {
		// Save value
		this->setVt(V);
	}

	// If broadcast this reservation station
	// Clear reservation
	if (Q == ((this->parentTag << 3) | this->tag)) {
		this->clear();
	}
}

// Get V of ReservationStation
uint32 ReservationStation::getVs () {
	return this->s.V;
}

// Get V of Rt
uint32 ReservationStation::getVt () {
	return this->t.V;
}

// Get name
uint3 ReservationStation::getName () {
	return this->tag;
}

// Get func
uint5 ReservationStation::getFunc () {
	return this->func;
}