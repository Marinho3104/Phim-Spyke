#include "byte_code_execution.h"

#include "byte_code_store.h"
#include "byte_code.h"
#include "execution.h"
#include "program.h"
#include "opcodes.h"
#include "memory.h"
#include "stack.h"

#include <iostream>


void virtual_machine::executeByteCode(byte_code::Byte_Code* __byte_code, virtual_machine::Execution* __execution) {

    // std::cout << "Code -> " << (int) __byte_code->code << " Argument -> " << __byte_code->argument << std::endl;

    switch (__byte_code->code)
    {
    case BYTE_CODE_STACK_MEMORY_ALLOCATE: execute_BYTE_CODE_STACK_MEMORY_ALLOCATE(__byte_code->argument, __execution); break;
    case BYTE_CODE_LOAD: execute_BYTE_CODE_LOAD(__byte_code->argument, __execution); break;
    case BYTE_CODE_LOAD_GLOBAL: execute_BYTE_CODE_LOAD_GLOBAL(__byte_code->argument, __execution); break;
    case BYTE_CODE_CALL: execute_BYTE_CODE_CALL(__byte_code->argument, __execution); break;
    default: std::cout << "error " << std::endl; exit(1); break;
    }

}

void virtual_machine::execute_BYTE_CODE_STACK_MEMORY_ALLOCATE(int __arg, Execution* __execution) {

    std::cout << "Allocate memory" << std::endl;

    if (__execution->program->memory->allocateStack(__arg) == -1) {std::cout << "Error" << std::endl; exit(1);} // Raise proper error 

}

void virtual_machine::execute_BYTE_CODE_LOAD(int __arg, Execution* __execution) {

    std::cout << "Load " << __arg << std::endl;

    __execution->stacks->last->object->addToStack(
        __execution->stacks->last->object->inicial_position + __arg
    );

    __execution->stacks->last->object->stack->printContent();

    std::cout << " --- Load --- " << std::endl;

}

void virtual_machine::execute_BYTE_CODE_LOAD_GLOBAL(int __arg, Execution* __execution) {

    std::cout << "Load Global" << std::endl;

    __execution->stacks->last->object->addToStack(
        __arg
    );

    __execution->stacks->last->object->stack->printContent();

    std::cout << " --- Load Global --- " << std::endl;

}

void virtual_machine::execute_BYTE_CODE_CALL(int __arg, Execution* __execution) { __execution->addStack(__arg); }





