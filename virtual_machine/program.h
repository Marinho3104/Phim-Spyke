#pragma once 

#ifndef VIRTUAL_MACHINE_PROGRAM_H
#define VIRTUAL_MACHINE_PROGRAM_H

#include "virtual_machine_definitions.h"

namespace virtual_machine {

    struct Program {

        Byte_Code_Store* byte_code;
        Execution* execution;
        int entry_point;
        Memory* memory;

        ~Program(); Program(byte_code::Compiled_Code*, int);

        void execute();

    };

}

#endif