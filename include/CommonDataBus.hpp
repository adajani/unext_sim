//  CommonDataBus.hpp
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


#ifndef __COMMON_DATA_BUS_HPP
    #define __COMMON_DATA_BUS_HPP
    #include <Types.hpp>

    class CommonDataBus {
        public:
            // Bind cdb with functional unit
            class BusBind {
                public:
                    BusBind() = default;
                    uint5 Q;
                    uint32 V;
                    uint5 write_Q;
                    uint32 write_V;
                    bool request;
                    bool access;
            };
        private:
            static constexpr int CDB_UNITS_COUNT = 4;
            
            // Number of units
            uint8 units;
            // Access Token
            uint8 token;
            bool active;

            // CDB values for debugging
            uint5 Q;
            uint32 V;

            // List of binds
            BusBind unit_list[CDB_UNITS_COUNT];
        public:
            CommonDataBus();

            // Bind a unit
            BusBind* bind ();

            // Broadcast data to the bus
            void broadcast ();

            // Bind Bus
            void grandAccess ();
    };
#endif