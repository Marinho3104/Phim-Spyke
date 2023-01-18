#pragma once

#ifndef VIRTUAL_MACHINE_BYTE_CODE_EXECUTION
#define VIRTUAL_MACHINE_BYTE_CODE_EXECUTION

#include "virtual_machine_definitions.h"
#include "byte_code_definitions.h"

namespace virtual_machine {

    void executeByteCode(byte_code::Byte_Code*, Execution*, int&);

    void execute_BYTE_CODE_STACK_MEMORY_ALLOCATE(int, Execution*);

    void execute_BYTE_CODE_HEAP_MEMORY_ALLOCATE(int, Execution*);

    void execute_BYTE_CODE_LOAD(int, Execution*);

    void execute_BYTE_CODE_LOAD_GLOBAL(int, Execution*);
    
    void execute_BYTE_CODE_CALL(byte_code::Byte_Code*, Execution*);

    void execute_BYTE_CODE_LOAD_IMPLICIT_VALUE(int, Execution*);

    void execute_BYTE_CODE_MEMORY_COPY(int, Execution*);

    void execute_BYTE_CODE_SET_INTO_STACK(int, Execution*);

    void execute_BYTE_CODE_GET_FROM_STACK(int, Execution*);

    void execute_BYTE_CODE_CLEAN_STACK(int, Execution*);

    void execute_BYTE_CODE_COPY_PREVIOUS_STACK_DATA(int, Execution*);

    void execute_BYTE_CODE_COPY_PREVIOUS_STACK_DATA_REMOVE(int, Execution*);

    void execute_BYTE_CLOSE_STACK_FRAME(int, Execution*);

    void execute_BYTE_BINARY_ADD(int, Execution*);

    void execute_BYTE_BINARY_SUB(int, Execution*);

    void execute_BYTE_CODE_BINARY_EQUAL_TO(int, Execution*);

    void execute_BYTE_CODE_BINARY_NOT_EQUAL_TO(int, Execution*);

    void execute_BYTE_CODE_BINARY_GREATER_THAN(int, Execution*);

    void execute_BYTE_CODE_BINARY_LESS_THAN(int, Execution*);

    void execute_BYTE_CODE_BINARY_GREATER_THAN_EQUAL_TO(int, Execution*);

    void execute_BYTE_CODE_BINARY_LESS_THAN_EQUAL_TO(int, Execution*);

    void execute_BYTE_CODE_IF(int, Execution*, int&);

    void execute_BYTE_CODE_CALL_SUB(int, Execution*);

    void execute_BYTE_BINARY_MUL(int, Execution*);

    void execute_BYTE_BINARY_DIV(int, Execution*);

    void execute_BYTE_BINARY_MOD(int, Execution*);

    void execute_BYTE_BINARY_INC(int, Execution*);
    
    void execute_BYTE_BINARY_DEC(int, Execution*);

    void execute_BYTE_CODE_EXECUTE_PREVIOUS_STACK(byte_code::Byte_Code*, Execution*, int&);

    void execute_BYTE_CODE_JUMP(byte_code::Byte_Code*, Execution*, int&);

}

#endif