#pragma once

#ifndef VIRTUAL_MACHINE_BYTE_CODE_EXECUTION
#define VIRTUAL_MACHINE_BYTE_CODE_EXECUTION

#include "virtual_machine_definitions.h"
#include "byte_code_definitions.h"

namespace virtual_machine {

    void executeByteCode(byte_code::Byte_Code*, Execution*);

    void execute_BYTE_CODE_STACK_MEMORY_ALLOCATE(int, Execution*);

    void execute_BYTE_CODE_LOAD(int, Execution*);

    void execute_BYTE_CODE_LOAD_GLOBAL(int, Execution*);
    
    void execute_BYTE_CODE_CALL(int, Execution*);

}

#endif