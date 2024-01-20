//  FunctionalUnit.hpp
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


#ifndef __FUNCTIONAL_UNIT_HPP
	#define __FUNCTIONAL_UNIT_HPP

	#include <Types.hpp>
	#include <ReservationStation.hpp>
	#include <CommonDataBus.hpp>

	// Functional unit class
	class FunctionalUnit {
		public:
			static constexpr int FUNCTIONAL_ARITHMETIC_UNIT_RS = 3;
			static constexpr int FUNCTIONAL_ARITHMETIC_UNIT_LATENCY = 3;
			static constexpr int FUNCTIONAL_LOGIC_UNIT_RS = 2;
			static constexpr int FUNCTIONAL_LOGIC_UNIT_LATENCY = 2;
			
			// Functional unit name
			uint2 tag;

			// Common data bus bind
			CommonDataBus::BusBind* cdb;

			// Latency Stage
			struct PipelineStage {
				bool active;
				uint5 Q;
				uint2 func;
				uint32 Vs;
				uint32 Vt;
				uint32 R;
			};

			// Init
			FunctionalUnit (uint2 tag);

			// Listen on cdb
			void subscribe (CommonDataBus::BusBind* cdb);
	};

	// Arithmetic functional unit class
	class FunctionalArithmeticUnit : public FunctionalUnit {
		public:
			// Rs units
			ReservationStation **rs=nullptr;//;[FUNCTIONAL_ARITHMETIC_UNIT_RS_];

			// Calculate unit
			PipelineStage calculateStages[FUNCTIONAL_ARITHMETIC_UNIT_LATENCY];

			FunctionalArithmeticUnit (uint2 tag);

			// Get availiable rs (if any)
			uint5 isAvailiable ();

			// Issue command (save on an rs)
			void issue (uint2 func, uint5 Qs, uint32 Vs, uint5 Qt, uint32 Vt);

			// Calculate
			void calculate ();
			
			// Load job
			void loadJob ();

			// Send cdb values to rs
			void checkBroadcast ();

			// Prepare data for cdb
			void checkAccess ();

	};

	// Logic functional unit class
	class FunctionalLogicUnit : public FunctionalUnit {
		public:
			enum FunctionalLogicUnit_func {AND=0b0, OR=0b1, NOT=0b10};

			// Define super
			typedef FunctionalUnit super;

			// Rs units
			ReservationStation **rs=nullptr; //[FUNCTIONAL_LOGIC_UNIT_RS];

			// Calculate unit
			PipelineStage calculateStages[FUNCTIONAL_LOGIC_UNIT_LATENCY];

			FunctionalLogicUnit (uint2 tag);

			// Get availiable rs (if any)
			uint5 isAvailiable ();

			// Issue command (save on an rs)
			void issue (uint2 func, uint5 Qs, uint32 Vs, uint5 Qt, uint32 Vt);

			// Calculate
			void calculate ();
			
			// Load job
			void loadJob ();

			// Send cdb values to rs
			void checkBroadcast ();

			// Prepare data for cdb
			void checkAccess ();
	};
#endif