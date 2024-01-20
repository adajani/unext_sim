//  register_fixture.hpp
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

    using ::testing::Test;

    class RegisterTest : public Test {           
        protected:
            RegisterFile rf;

            uint32* data = nullptr;
            bool* action= nullptr;
            uint5* R= nullptr;
            uint32* value = nullptr;

            void test_RegisterFile(uint32 data[32], bool action[32], uint5 R[32], uint32 value[32], int length);

        public:
            void SetUp(/*std::string fileName*/) {
                // Test data variables
                data = new uint32 [32 * sizeof(uint32)];
                action = new bool [32 * sizeof(bool)];
                R = new uint5 [32 * sizeof(uint5)];
                value = new uint32 [32 * sizeof(uint32)];
                
                // Init data
                for (int i = 0; i < 32; ++i){
                    data[i] = 0b0;
                    action[i] = true;
                    R[i] = 0b0;
                    value[i] = 0b0;
                }
            }

            void TearDown() override {
                delete data;
                delete action;
                delete R;
                delete value;
            }
    };
#endif