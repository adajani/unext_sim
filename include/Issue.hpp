//  Issue.hpp
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


#ifndef __ISSUE_HPP
#define __ISSUE_HPP

#include <Types.hpp>
#include <FunctionalUnit.hpp>
#include <CommonDataBus.hpp>
#include <Register.hpp>
#include <ReorderBuffer.hpp>

// Issue class
class Issue {
	private:
		// Define command codes
		enum commandCodes{ISSUE_LU_CODE = 0b00, ISSUE_AU_CODE = 0b01};

	public:
		RegisterFile *rf=nullptr;
		ReorderBuffer *rb=nullptr;
		FunctionalArithmeticUnit *fau=nullptr;
		FunctionalLogicUnit *flu=nullptr;
		CommonDataBus *cdb=nullptr;

		Issue ();

		// Command issue
		bool issue (uint2 type, uint2 func, uint5 Rd, uint5 Rs, uint5 Rt);

};


#endif // _ISSUE_H_