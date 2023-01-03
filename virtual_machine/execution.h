#pragma once

#ifndef VIRTUAL_MACHINE_EXECUTION_H
#define VIRTUAL_MACHINE_EXECUTION_H

#include "virtual_machine_definitions.h"
#include "byte_code_definitions.h"
#include "linked_List.h"

namespace virtual_machine {

    struct Execution {

        utils::Linked_List <Stack*>* stacks; 
        int current_block, current_index;
        Program* program;

        ~Execution(); Execution(Program*);

        void addStack(int);

        void popStack();
        
        void execute();

    };

}

#endif