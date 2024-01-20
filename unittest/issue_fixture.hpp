//  issue_fixture.hpp
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

#ifndef __ISSUE_FIXTURE_HPP
    #define __ISSUE_FIXTURE_HPP

    #include <gtest/gtest.h>
    #include <Register.hpp>
    #include <Types.hpp>
    #include <Issue.hpp>

    using ::testing::Test;

    #define _TESTBENCH_PRINTS_ 0

    class IssueTest : public Test {           
        protected:
            int pc = 0;
            uint32* results = nullptr;

            // Demo register file data
            uint32* data  = nullptr;

            // Demo program to run
            uint32* commands  = nullptr;

            void decodeCommand(uint32 command, uint2* type, uint2* func, uint5* Rd, uint5* Rs, uint5* Rt);
            //void printReorderBuffer(Issue* issue);
            void test_Issue(uint32 results[1], uint32 data[32], uint32 commands[64], int run, int rest);


        public:
            void SetUp() {
                results = new uint32 [1 * sizeof(uint32)];
                data = new uint32 [32 * sizeof(uint32)];
	            commands = new uint32 [64 * sizeof(uint32)];

                for (int i = 0; i < 32; ++i){
                    data[i] = 0b0;
                }

                for (int i = 0; i < 32; ++i){
                    commands[i] = 0b0;
                }

                // Register data
                data[1] = 0b1;
                data[2] = 0b10;
                data[3] = 0b11;
                data[4] = 0b100;
            }

            void TearDown() override {
                delete results;
                delete data;
                delete commands;
            }
    };
#endif