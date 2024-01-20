#  Makefile
#
#  Author:
#       Ahmed Dajani <adajani@iastate.edu>
#
#  Copyright (c) 2023 Ahmed Dajani
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.

SRC_DIR=./src
BLD_DIR=./build
INC_DIR=./include
UNITTEST_SRC_DIR=./unittest

CC=g++
#-pg flag tells the compiler to include information needed for profiling
#./build/unext_sim
#gprof ./build/unext_sim gmon.out > analysis.txt
CPPFLAGS=-g -std=c++17 -Wall -I$(INC_DIR) -O2 -pg

UNITTEST_LD_FLAGS=-lgtest -lpthread -lgtest_main -L/usr/lib
UNITTEST_CPP_FLAGS=-I$(UNITTEST_SRC_DIR)

SRC=$(filter-out $(SRC_DIR)/main.cc, $(wildcard $(SRC_DIR)/*.cc))
OBJ=$(SRC:$(SRC_DIR)/%.cc=$(BLD_DIR)/%.o)

UNITTEST_SRC=$(filter-out $(UNITTEST_SRC_DIR)/main_unittest.cc, $(wildcard $(UNITTEST_SRC_DIR)/*.cc))
UNITTEST_OBJ=$(UNITTEST_SRC:$(UNITTEST_SRC_DIR)/%.cc=$(BLD_DIR)/%.o)

unext_sim: $(OBJ) $(BLD_DIR)/main.o
	$(CC) $(CPPFLAGS) $^ -o $(BLD_DIR)/$@

unittest: $(OBJ) $(UNITTEST_OBJ) $(BLD_DIR)/main_unittest.o
	$(CC) $(CPPFLAGS) $^ -o $(BLD_DIR)/$@ $(UNITTEST_LD_FLAGS)

$(BLD_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BLD_DIR)/%.o: $(UNITTEST_SRC_DIR)/%.cc
	$(CC) $(CPPFLAGS) $(UNITTEST_CPP_FLAGS) -c $< -o $@

perf:
	valgrind --tool=callgrind $(BLD_DIR)/unext_sim -file ./app/test.bin
	ls -t callgrind.out.* | head -n1 | xargs python3 -m gprof2dot -f callgrind | dot -Tsvg -o output.svg

clean:
	@$(RM) -rf $(BLD_DIR)/*.o
	@$(RM) -rf $(BLD_DIR)/unext_sim*
	@$(RM) -rf $(BLD_DIR)/gmon.out
	@$(RM) -rf $(BLD_DIR)/unittest
	@$(RM) -rf callgrind.out.*
	@$(RM) -rf output.svg