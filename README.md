# UNEXT Simulator: RISC like out of order processeor

This repository contains a simple implementation of the 32 bit RISC out of order processor.

# Development toolchains:
The proposed algorithm was developed under Debian Linux 64bit operating system(version 11.6), with the following  toolchains:

| Tool name | Version  | Description  |
| :---:   | :-: | :-: |
| g++ | 10.2.1 | C++ compiler (standard 0x17) |
| ld | 2.35.2 | GNU linker |
| make | 4.3 |  GNU make (builder) |

# How to compile:
Locate the root source code, then type the folowing:
```
$ make clean
$ make -j4
```
If the compilation process proceeds successfully, you will see "unext_sim" binary under build directory.

# Usage:
```
UnextSim CPU simulator
./unext_sim -file [-debug] [-pc]
where:
        -file           [required]      Input binary file.
        -pc             [optinal ]      Input binary file, integer >=0, default(0).
        -instruction    [optinal ]      Instruction count >=0, default(0). If =0 execute all instrucitons.
        -debug          [optinal ]      Show debugging info, defualt(false)
```

# How to Run
```
$ build/unext_sim -file app/test.bin
```

# Unittest:
Locate the root source code, then type the folowing:
```
$ make unittest
$ ./build/unittest
```
Note: The unittest is based on Google gtest.

# Author
Ahmed Dajani,
Email: adajani@iastate.edu