#pragma once

#ifndef VIRTUAL_MACHINE_EXECUTION_H
#define VIRTUAL_MACHINE_EXECUTION_H

#include "virtual_machine_definitions.h"
#include "byte_code_definitions.h"
#include "linked_List.h"

namespace virtual_machine {

    struct Execution {

        utils::Linked_List <Stack*>* stacks; 
        Program* program;

        ~Execution(); Execution(Program*);

        void addStack();

        void popStack();

        void executeBlock(int, bool);
        
        void execute();

    };

}

#endif