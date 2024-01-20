//  argument.hpp
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

#ifndef __ARGUMENT_HPP
    #define __ARGUMENT_HPP
    #include <iostream>

    class Argument {
        private:
            int argc_m;
            char **argv_m;
            std::string fileName_m;
            bool debug_m;
            int pc_m;
            unsigned int instructionCount_m;
            void showUsage();
        public:
            Argument(int argc, char **argv);
            ~Argument() = default;
            void parse();
            std::string getFileName();
            bool isDebug();
            int getPc();
            unsigned int getInstructionCount();
    };
#endif