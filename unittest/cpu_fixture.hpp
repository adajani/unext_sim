//  cpu_fixture.hpp
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

#ifndef __REGISTER_FIXTURE_HPP
    #define __REGISTER_FIXTURE_HPP

    #include <gtest/gtest.h>
    #include <Register.hpp>
    #include <Types.hpp>
    #include <CPU.hpp>

    using ::testing::Test;

    class CPUTest : public Test {           
        protected:
            CPU *cpu = nullptr;

	        int pc = 0;
	        uint32* results = nullptr;
	
            // Init register file data
            uint32* data = nullptr;
	
            // Init program's commands
            uint32* commands = nullptr;
  

        public:
            void SetUp() {
                cpu = new CPU(0, false);
                data = new uint32 [32 * sizeof(uint32)];
	            commands = new uint32[64 * sizeof(uint32)];

                for (int i = 0; i < 32; ++i){
                    data[i] = 0b0;
                }
                // Sample register file data
                data[1] = 0b1;
                data[2] = 0b10;
                data[3] = 0b11;
                data[4] = 0b100;
                data[30] = 0b1; 

                for (int i = 0; i < 64; ++i){
                    commands[i] = 0b0;
                }    
            }

            void TearDown() override {
                delete cpu;
                delete data;
                delete commands;
            }
    };
#endif