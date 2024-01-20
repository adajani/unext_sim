//  SimException.cc
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


#include <SimException.hpp>

struct SimExceptionDict SimException::exceptionDict_m[] = {
    {MISSING_BINARY     , "Input Binary file is missing"},
    {MISSING_PC         , "PC is missing"},
    {CANT_OPEN_BINARY   , "Can't open Binary file"},
    {INVALID_PC         , "Invalid PC"},
    {MISSING_INSTRUCTION, "Instruction count is missing"},
    {INVALID_INSTRUCTION, "Invalid instruction count"},

    {UNKNOWN_ARGUMENT   , "Unknown input argument"}
};

SimException::SimException (SimExceptionId exceptionId) {
    exceptionId_m = exceptionId;
}

const char* SimException::what() const throw() {
    return exceptionDict_m[exceptionId_m].exceptionName;
}