//  CPU.hpp
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


#ifndef __CPU_HPP
    #define __CPU_HPP

    #include <Types.hpp>
    #include <Issue.hpp>

    class CPU {
        private:
            int pc;
            // Count cpu cycles
	        int cycles = 0;
            bool isDebug_m;

        public:
            Issue *issue=nullptr;
            
            CPU(int pc, bool debug);

            void decodeCpuCommand(uint32 command, uint2* type, uint2* func, uint5* Rd, uint5* Rs, uint5* Rt);
            uint32 execute(uint32 data[32], uint32 *commands, int run);
    };
#endif

