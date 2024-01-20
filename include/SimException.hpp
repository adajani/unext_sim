//  SimException.hpp
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

#ifndef __SIM_EXCEPTION_HPP
    #define __SIM_EXCEPTION_HPP
    #include <iostream>

    enum SimExceptionId {
        MISSING_BINARY,
        MISSING_PC,       
        CANT_OPEN_BINARY,
        INVALID_PC,
        MISSING_INSTRUCTION,
        INVALID_INSTRUCTION,

        //Else
        UNKNOWN_ARGUMENT
    };

    struct SimExceptionDict {
        unsigned int exceptionId;
        const char *exceptionName;
    };

    class SimException : public std::exception {
        public:
            SimException(SimExceptionId exceptionId);
            const char* what() const throw();
        private:
            SimExceptionId exceptionId_m;
            static struct SimExceptionDict exceptionDict_m[];
    };
#endif