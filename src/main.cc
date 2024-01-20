//  main.cc
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

#include <Argument.hpp>
#include <CPU.hpp>
#include <SimException.hpp>
#include <fstream>
#include <sys/stat.h>

using namespace std;

long getFileSize(std::string filename) {
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

int main(int argc, char **argv) {
    Argument *argument = nullptr;
    uint32 instruction;
    uint32* commands = nullptr;
    uint32* data = nullptr;

    cout << "Starting UNEXT/SIM..." << endl;
    try {
        argument = new Argument(argc, argv);
        argument->parse();

        data = new uint32 [32 * sizeof(uint32)];
        for (int i = 0; i < 32; i++) {
            data[i] = 0b0;
        }

        std::ifstream binary(argument->getFileName(), std::ios::binary | ios::in);
        if (!binary.is_open()) {
            throw SimException(CANT_OPEN_BINARY);
        }

        long fileSize = getFileSize(argument->getFileName());

        commands = new uint32[(fileSize / sizeof(uint32)) * sizeof(uint32)];
        int pc = argument->getPc();
        CPU cpu(pc, argument->isDebug());
        int index=0;
        while(binary.read((char *)&instruction,sizeof(instruction))) {
            commands[index++] = instruction;
        }
        binary.close();

        unsigned int instructionCount = argument->getInstructionCount();
        uint32 cycles = cpu.execute(data, commands, instructionCount==0 ? index : argument->getInstructionCount() );
        cout << "Done simulation @ " << cycles << " cycles." << endl;

        delete argument;
        delete data;
        delete commands;
    }
    catch (SimException &e) {
            std::cerr << "Unext Simulator Exception : " << e.what() << std::endl;
            return EXIT_FAILURE;
        }

    return EXIT_SUCCESS;
}