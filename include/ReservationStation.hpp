//  ReservationStation.hpp
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



#ifndef __RESERVATION_STATION_HPP
#define __RESERVATION_STATION_HPP

#include <Types.hpp>

class ReservationStation {
	public:
		// Reservetion Station name
		uint3 tag;
		uint2 parentTag;
		// Func variable
		uint2 func;

		// Busy
		bool busy;
		bool running;

		// Renaming Register
		//union Register { 
		class Register {
			public:
				Register() = default;
				uint5 Q;
				uint32 V;
		};

		// Actual renaming registers
		Register s, t;
		// Value loaded
		bool s_value, t_value;

		ReservationStation (uint2 parenTag, uint3 tag);

		// Clear
		void clear ();
		// Check if busy
		bool isBusy ();
		// Check if ready
		bool isReady ();
		// Check if running
		bool isRunning ();

		// Set func
		void setFunc (uint2 func);
		// Set Q of Rs
		void setQs (uint5 Q);
		// Set Q of Rt
		void setQt (uint5 Q);
		// Set V of Rs
		void setVs (uint32 V);
		// Set V of Rt
		void setVt (uint32 V);
		
		// Set running
		void setAsRunning ();

		// Check broadcast
		void checkBroadcast (uint5 Q, uint32 V);

		// Get V of Rs
		uint32 getVs ();
		// Get V of Rt
		uint32 getVt ();
		// Get name
		uint5 getName ();
		// Get func
		uint5 getFunc ();
};

#endif