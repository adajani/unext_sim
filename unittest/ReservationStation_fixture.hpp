//  ReservationStation_fixture.hpp
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

#ifndef __RESERVATION_STATION_FIXTURE_HPP
    #define __RESERVATION_STATION_FIXTURE_HPP

    #include <gtest/gtest.h>
    #include <Types.hpp>
    #include <ReservationStation.hpp>

    using ::testing::Test;

    class ReservationStationTest : public Test {           
        protected:
            uint32* results =nullptr;
            // Rs Name
            uint5 name = 0b0;
            // Rs func
            uint2 func = 0b0;
            // Rs
            uint3 Qs = 0b0;
            uint32 Vs = 0b0;
            // Rt
            uint3 Qt = 0b0;
            uint32 Vt = 0b0;
            // Broadcast
            uint3 Qb = 0b0;
            uint32 Vb = 0b0;


            void test_ReservationStation(uint32 results[6], uint5 name, uint2 func, uint5 Qs, uint32 Vs, uint5 Qt, uint32 Vt, uint5 Qb, uint32 Vb);
            int check(uint32 ready, uint32 busy, uint32 Vs, uint1 VsDontCare, uint32 Vt, uint1 VtDontCare, uint32 func, uint32 name, uint32 results[6]);

        public:
            void SetUp() {
                results = new uint32[6 * sizeof(uint32)]; 
            }

            void TearDown() override {
                delete results;
            }
    };
#endif