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
    
    void execute_BYTE_CODE_CALL(byte_code::Byte_Code*, Execution*);

    void execute_BYTE_CODE_LOAD_IMPLICIT_VALUE(int, Execution*);

    void execute_BYTE_CODE_MEMORY_COPY(int, Execution*);

    void execute_BYTE_CODE_SET_INTO_STACK(int, Execution*);

    void execute_BYTE_CODE_GET_FROM_STACK(int, Execution*);

    void execute_BYTE_CODE_CLEAN_STACK(int, Execution*);

    void execute_BYTE_CODE_COPY_LAST_PREVIOUS_STACK_DATA(int, Execution*);

}

#endif