//  Argument.cpp
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

#include <cstring> //strcasecmp
#include <cstdlib> //exit
#include <iostream> //cout
#include <Argument.hpp>
#include <SimException.hpp>

Argument::Argument(int argc, char **argv) {
    argc_m = argc;
    argv_m = argv;
    debug_m = false;
    pc_m = 0;
    instructionCount_m = 0; 
}

void Argument::showUsage() {
    std::cout << "UnextSim CPU simulator"  << std::endl
              << "./unext_sim -file [-debug] [-pc]" << std::endl
              << "where:" << std::endl
              << "\t-file           [required]\tInput binary file." << std::endl
              << "\t-pc             [optinal ]\tInput binary file, integer >=0, default("<<pc_m<<")." << std::endl
              << "\t-instruction    [optinal ]\tInstruction count >=0, default("<<instructionCount_m<<"). If =0 execute all instrucitons." << std::endl
              << "\t-debug          [optinal ]\tShow debugging info, defualt(false)" << std::endl;
    exit(EXIT_SUCCESS);
}

void Argument::parse() {
    if(argc_m < 2) {
        showUsage();
    }

    for(int index=1; index<argc_m; index++) {
        if(strcasecmp(argv_m[index], "-help") == 0) {
            showUsage();
        } else if(strcasecmp(argv_m[index], "-file") == 0) {
            index += 1;
            char *file_name = argv_m[index];
            if(file_name == NULL) {
                throw SimException(MISSING_BINARY);
            } 
            fileName_m.assign(file_name);
        } else if(strcasecmp(argv_m[index], "-pc") == 0) {
            index += 1;
            char *pc = argv_m[index];
            if(pc == NULL) {
                throw SimException(MISSING_BINARY);
            } 
            pc_m = atoi(pc);
            if(pc_m < 0 ) {
                throw SimException(INVALID_PC);
            }
        }  else if(strcasecmp(argv_m[index], "-instruction") == 0) {
            index += 1;
            char *instruction = argv_m[index];
            if(instruction == NULL) {
                throw SimException(MISSING_INSTRUCTION);
            } 
            instructionCount_m = atoi(instruction);
            if(instructionCount_m < 0 ) {
                throw SimException(INVALID_PC);
            }
        } else if(strcasecmp(argv_m[index],"-debug")==0) {
            debug_m = true;
        } else {
            throw SimException(UNKNOWN_ARGUMENT);
        }
    }
}

std::string Argument::getFileName() {
    return fileName_m;
}

bool Argument::isDebug() {
    return debug_m;
}

int Argument::getPc() {
    return pc_m;
}

unsigned int Argument::getInstructionCount() {
    return instructionCount_m;
}