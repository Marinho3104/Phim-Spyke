#include "byte_code_execution.h"

#include "memory_settings.h"

#include "byte_code_execution_helper.h"
#include "byte_code_store.h"
#include "byte_code.h"
#include "execution.h"
#include "program.h"
#include "opcodes.h"
#include "memory.h"
#include "stack.h"

#include <iostream>
#include <string.h>


void virtual_machine::executeByteCode(byte_code::Byte_Code* __byte_code, virtual_machine::Execution* __execution, int& __current_index) {

    // std::cout << "Code -> " << (int) __byte_code->code << " Argument -> " << __byte_code->argument << std::endl;

    std::cout << std::endl;

    switch (__byte_code->code)
    {
    case BYTE_CODE_STACK_MEMORY_ALLOCATE: execute_BYTE_CODE_STACK_MEMORY_ALLOCATE(__byte_code->argument, __execution); break;
    case BYTE_CODE_LOAD: execute_BYTE_CODE_LOAD(__byte_code->argument, __execution); break;
    case BYTE_CODE_LOAD_GLOBAL: execute_BYTE_CODE_LOAD_GLOBAL(__byte_code->argument, __execution); break;
    case BYTE_CODE_CALL: execute_BYTE_CODE_CALL(__byte_code, __execution); break;
    case BYTE_CODE_LOAD_IMPLICIT_VALUE: execute_BYTE_CODE_LOAD_IMPLICIT_VALUE(__byte_code->argument, __execution); break;
    case BYTE_CODE_MEMORY_COPY: execute_BYTE_CODE_MEMORY_COPY(__byte_code->argument, __execution); break;
    case BYTE_CODE_SET_INTO_STACK: execute_BYTE_CODE_SET_INTO_STACK(__byte_code->argument, __execution); break;
    case BYTE_CODE_GET_FROM_STACK: execute_BYTE_CODE_GET_FROM_STACK(__byte_code->argument, __execution); break;
    case BYTE_CODE_CLEAN_STACK: execute_BYTE_CODE_CLEAN_STACK(__byte_code->argument, __execution); break;
    case BYTE_CODE_COPY_PREVIOUS_STACK_DATA: execute_BYTE_CODE_COPY_PREVIOUS_STACK_DATA(__byte_code->argument, __execution); break;
    case BYTE_CODE_COPY_PREVIOUS_STACK_DATA_REMOVE: execute_BYTE_CODE_COPY_PREVIOUS_STACK_DATA_REMOVE(__byte_code->argument, __execution); break;
    case BYTE_CODE_CLOSE_STACK_FRAME: execute_BYTE_CLOSE_STACK_FRAME(__byte_code->argument, __execution); break;
    case BYTE_CODE_BINARY_ADD: execute_BYTE_BINARY_ADD(__byte_code->argument, __execution); break;
    case BYTE_CODE_BINARY_EQUAL_TO: execute_BYTE_CODE_BINARY_EQUAL_TO(__byte_code->argument, __execution); break;
    case BYTE_CODE_BINARY_NOT_EQUAL_TO: execute_BYTE_CODE_BINARY_NOT_EQUAL_TO(__byte_code->argument, __execution); break;
    case BYTE_CODE_BINARY_GREATER_THAN: execute_BYTE_CODE_BINARY_GREATER_THAN(__byte_code->argument, __execution); break;
    case BYTE_CODE_BINARY_LESS_THAN: execute_BYTE_CODE_BINARY_LESS_THAN(__byte_code->argument, __execution); break;
    case BYTE_CODE_BINARY_GREATER_THAN_EQUAL_TO: execute_BYTE_CODE_BINARY_GREATER_THAN_EQUAL_TO(__byte_code->argument, __execution); break;
    case BYTE_CODE_BINARY_LESS_THAN_EQUAL_TO: execute_BYTE_CODE_BINARY_LESS_THAN_EQUAL_TO(__byte_code->argument, __execution); break;
    case BYTE_CODE_IF: execute_BYTE_CODE_IF(__byte_code->argument, __execution, __current_index); break;
    case BYTE_CODE_CALL_SUB: execute_BYTE_CODE_CALL_SUB(__byte_code->argument, __execution); break;
    case BYTE_CODE_BINARY_MUL: execute_BYTE_BINARY_MUL(__byte_code->argument, __execution); break;
    case BYTE_CODE_BINARY_DIV: execute_BYTE_BINARY_DIV(__byte_code->argument, __execution); break;
    case BYTE_CODE_BINARY_MOD: execute_BYTE_BINARY_MOD(__byte_code->argument, __execution); break;
    case BYTE_CODE_BINARY_INC: execute_BYTE_BINARY_INC(__byte_code->argument, __execution); break;
    case BYTE_CODE_BINARY_DEC: execute_BYTE_BINARY_DEC(__byte_code->argument, __execution); break;

    case BYTE_CODE_NOP: std::cout << "NOP" << std::endl; break;
    default: std::cout << "error " << std::endl; exit(1); break;
    }

}

void virtual_machine::execute_BYTE_CODE_STACK_MEMORY_ALLOCATE(int __arg, Execution* __execution) {

    std::cout << "STACK_MEMORY_ALLOCATE" << std::endl;
    std::cout << "Allocate size -> " << __arg << std::endl;
    int _address;

    if ((_address = __execution->program->memory->allocateStack(__arg)) == -1) {std::cout << "Error" << std::endl; exit(1);} // Raise proper error 

    // __execution->stacks->last->object->addToStack(
    //     _address
    // );

}

void virtual_machine::execute_BYTE_CODE_HEAP_MEMORY_ALLOCATE(int __arg, Execution* __execution) {

    std::cout << "HEAP_MEMORY_ALLOCATE" << std::endl;
    std::cout << "Allocate size -> " << __arg << std::endl;


}

void virtual_machine::execute_BYTE_CODE_LOAD(int __arg, Execution* __execution) {

    std::cout << "LOAD" << std::endl;
    std::cout << "arg -> " << __arg << std::endl;
    std::cout << "Loaded value address -> " << __execution->stacks->last->object->inicial_position + __arg << std::endl;
    std::cout << "Loaded value -> " << 
        *((int*)__execution->program->memory->getRealAddress(__execution->stacks->last->object->inicial_position + __arg)) << std::endl;

    __execution->stacks->last->object->addToStack(
        __execution->stacks->last->object->inicial_position + __arg
    );

    __execution->stacks->last->object->stack->printContent();

}

void virtual_machine::execute_BYTE_CODE_LOAD_GLOBAL(int __arg, Execution* __execution) {

    std::cout << "LOAD_GLOBAL" << std::endl;
    std::cout << "arg -> " << __arg << std::endl;
    std::cout << "Loaded value address -> " << __execution->stacks->first->object->inicial_position + __arg << std::endl;
    std::cout << "Loaded value -> " << 
        *((int*)__execution->program->memory->getRealAddress(__execution->stacks->first->object->inicial_position + __arg)) << std::endl;

    __execution->stacks->last->object->addToStack(
        __execution->stacks->first->object->inicial_position + __arg
    );

    __execution->stacks->last->object->stack->printContent();

}

void virtual_machine::execute_BYTE_CODE_CALL(byte_code::Byte_Code* __byte_code, Execution* __execution) { 

    std::cout << "CALL " << __byte_code->argument << std::endl;
    __execution->stacks->last->object->stack->printContent();

    __execution->addStack();

    __execution->executeBlock(__byte_code->argument, 0); 
    
    __execution->popStack();

}

void virtual_machine::execute_BYTE_CODE_LOAD_IMPLICIT_VALUE(int __arg, Execution* __execution) {

    std::cout << "LOAD_IMPLICIT_VALUE " << std::endl;

    __execution->stacks->last->object->addToStack(
        STACK_MEMORY_SIZE + HEAP_MEMORY_SIZE + __arg
    );

    __execution->stacks->last->object->stack->printContent();

}

void virtual_machine::execute_BYTE_CODE_MEMORY_COPY(int __arg, Execution* __execution) { // receive copy

    std::cout << "MEMORY_COPY" << std::endl;

    __execution->stacks->last->object->stack->printContent();

    int _last_arg = __execution->stacks->last->object->popFromStack();
    int _last_last_arg = __execution->stacks->last->object->popFromStack();

    std::cout << "Size to copy -> " << __arg << std::endl;

    std::cout << "To be copied address -> " << _last_arg << std::endl;
    std::cout << "To received address -> " << _last_last_arg << std::endl;

    std::cout << "To be copied -> " << *((address_t*) __execution->program->memory->getRealAddress(_last_arg)) << std::endl;
    std::cout << "To received -> " << *((address_t*) __execution->program->memory->getRealAddress(_last_last_arg)) << std::endl;

    memcpy(
        __execution->program->memory->getRealAddress(_last_last_arg),
        __execution->program->memory->getRealAddress(_last_arg),
        __arg
    );

    std::cout << "To be copied -> " << *((address_t*) __execution->program->memory->getRealAddress(_last_arg)) << std::endl;
    std::cout << "To received -> " << *((address_t*) __execution->program->memory->getRealAddress(_last_last_arg)) << std::endl;

}

void virtual_machine::execute_BYTE_CODE_SET_INTO_STACK(int __arg, Execution* __execution) {

    std::cout << "SET_INTO_STACK" << std::endl;

    int _last_arg = __execution->stacks->last->object->popFromStack();

    int _to_add = 0;

    memcpy(
        &_to_add, __execution->program->memory->getRealAddress(_last_arg), 2
    );

    __execution->stacks->last->object->addToStack(
        _to_add
    );

    __execution->stacks->last->object->stack->printContent();

    std::cout << "Value added " << _to_add << std::endl;
    std::cout << "Value -> " << *((int*) __execution->program->memory->getRealAddress(_to_add)) << std::endl;

}

void virtual_machine::execute_BYTE_CODE_GET_FROM_STACK(int __arg, Execution* __execution) {

    std::cout << "GET_FROM_STACK" << std::endl;

    __execution->stacks->last->object->stack->printContent();

    int _last_address = __execution->stacks->last->object->popFromStack();
    int _last_last_address = __execution->stacks->last->object->popFromStack();

    std::cout << "Last address -> " << _last_address << std::endl;
    std::cout << "Last last address -> " << _last_last_address << std::endl;
    std::cout << "Last Address value before -> " << *((int*) __execution->program->memory->getRealAddress(_last_address)) << std::endl;

    memcpy(
        __execution->program->memory->getRealAddress(_last_address),
        &_last_last_address,
        2
    );

    std::cout << "Last Address value after -> " << *((int*) __execution->program->memory->getRealAddress(_last_address)) << std::endl;

    __execution->stacks->last->object->addToStack(
        _last_address
    );

}

void virtual_machine::execute_BYTE_CODE_CLEAN_STACK(int, Execution* __execution) { 

    std::cout << "CLEAN_STACK" << std::endl;

    __execution->stacks->last->object->stack->clean();

}

void virtual_machine::execute_BYTE_CODE_COPY_PREVIOUS_STACK_DATA(int __arg, Execution* __execution) {

    std::cout << "COPY_LAST_PREVIOUS_STACK_DATA" << std::endl;

    utils::Data_Linked_List <int>* __data_linked_list = __execution->stacks->last->previous->object->stack->last;

    for (int _ = 0; _ < __arg; _++) __data_linked_list = __data_linked_list->previous;

    __execution->stacks->last->object->stack->printContent();

    __execution->stacks->last->object->stack->add(
        __data_linked_list->object
    );

    __execution->stacks->last->object->stack->printContent();

}

void virtual_machine::execute_BYTE_CODE_COPY_PREVIOUS_STACK_DATA_REMOVE(int __arg, Execution* __execution) {

    std::cout << "COPY_LAST_PREVIOUS_STACK_DATA_REMOVE -- arg -> " << __arg << std::endl;

    utils::Data_Linked_List <int>* __data_linked_list = __execution->stacks->last->previous->object->stack->last;

    for (int _ = 0; _ < __arg; _++) __data_linked_list = __data_linked_list->previous;

    __execution->stacks->last->object->stack->printContent();

    __execution->stacks->last->object->stack->add(
        __data_linked_list->object
    );

    __execution->stacks->last->object->stack->printContent();

    std::cout << "Previous stack after remove ->"; 
    __execution->stacks->last->previous->object->stack->printContent();

    delete __execution->stacks->last->previous->object->stack->remove(
        __execution->stacks->last->previous->object->stack->count - 1 -__arg
    );

    std::cout << "Previous stack before remove ->"; 
    __execution->stacks->last->previous->object->stack->printContent();
    
}

void virtual_machine::execute_BYTE_CLOSE_STACK_FRAME(int, Execution* __execution) {

    std::cout << "CLOSE_STACK_FRAME" << std::endl;


}

void virtual_machine::execute_BYTE_BINARY_ADD(int __arg, Execution* __execution) {

    std::cout << "BINARY_ADD" << std::endl;

    switch (__arg)
    {
    case 1: binary_add_1_bytes(__execution); break;
    case 2: binary_add_2_bytes(__execution); break;
    case 4: binary_add_4_bytes(__execution); break;
    default: std::cout << "Error" << std::endl; exit(1); break;
    }

}

void virtual_machine::execute_BYTE_BINARY_SUB(int __arg, Execution* __execution) {

    std::cout << "BINARY_SUB" << std::endl;

    switch (__arg)
    {
    case 1: binary_sub_1_bytes(__execution); break;
    case 2: binary_sub_2_bytes(__execution); break;
    case 4: binary_sub_4_bytes(__execution); break;
    default: std::cout << "Error" << std::endl; exit(1); break;
    }

}

void virtual_machine::execute_BYTE_CODE_BINARY_EQUAL_TO(int __arg, Execution* __execution) {

    std::cout << "BINARY_EQUAL_TO" << std::endl;

    switch (__arg)
    {
    case 1: binary_equal_to_1_bytes(__execution); break;
    case 2: binary_equal_to_2_bytes(__execution); break;
    case 4: binary_equal_to_4_bytes(__execution); break;
    default: std::cout << "Error" << std::endl; exit(1); break;
    }

}

void virtual_machine::execute_BYTE_CODE_BINARY_NOT_EQUAL_TO(int, Execution*) {

}

void virtual_machine::execute_BYTE_CODE_BINARY_GREATER_THAN(int, Execution*) {

}

void virtual_machine::execute_BYTE_CODE_BINARY_LESS_THAN(int, Execution*) {

}

void virtual_machine::execute_BYTE_CODE_BINARY_GREATER_THAN_EQUAL_TO(int, Execution*) {

}

void virtual_machine::execute_BYTE_CODE_BINARY_LESS_THAN_EQUAL_TO(int, Execution*) {

}

void virtual_machine::execute_BYTE_CODE_IF(int __arg, Execution* __execution, int& __current_index) {

    std::cout << "IF" << std::endl;

    int _condition_address = __execution->stacks->last->object->popFromStack();
    void* _condition = __execution->program->memory->getRealAddress(_condition_address);

    std::cout << "Condition -> " << *((bool*) _condition) << std::endl;

    if (
        !*((bool*) _condition) 
    ) __current_index++;

}

void virtual_machine::execute_BYTE_CODE_CALL_SUB(int __arg, Execution* __execution) {

    std::cout << "CALL_SUB" << std::endl;

    __execution->stacks->last->object->stack->printContent();

    __execution->addStack();

    __execution->stacks->last->object->inicial_position = __execution->stacks->last->previous->object->inicial_position;

    __execution->executeBlock(__arg, 0); 
    
    __execution->popStack();

}

void virtual_machine::execute_BYTE_BINARY_MUL(int __arg, Execution* __execution) {

    std::cout << "BINARY_MUL" << std::endl;

    switch (__arg)
    {
    case 1: binary_mul_1_bytes(__execution); break;
    case 2: binary_mul_2_bytes(__execution); break;
    case 4: binary_mul_4_bytes(__execution); break;
    default: std::cout << "Error" << std::endl; exit(1); break;
    }

}

void virtual_machine::execute_BYTE_BINARY_DIV(int __arg, Execution* __execution) {

    std::cout << "BINARY_DIV" << std::endl;

    switch (__arg)
    {
    case 1: binary_div_1_bytes(__execution); break;
    case 2: binary_div_2_bytes(__execution); break;
    case 4: binary_div_4_bytes(__execution); break;
    default: std::cout << "Error" << std::endl; exit(1); break;
    }

}

void virtual_machine::execute_BYTE_BINARY_MOD(int __arg, Execution* __execution) {

    std::cout << "BINARY_MOD" << std::endl;

    switch (__arg)
    {
    case 1: binary_mod_1_bytes(__execution); break;
    case 2: binary_mod_2_bytes(__execution); break;
    case 4: binary_mod_4_bytes(__execution); break;
    default: std::cout << "Error" << std::endl; exit(1); break;
    }

}

void virtual_machine::execute_BYTE_BINARY_INC(int __arg, Execution* __execution) {

    std::cout << "BINARY_INC" << std::endl;

    switch (__arg)
    {
    case 1: binary_inc_1_bytes(__execution); break;
    case 2: binary_inc_2_bytes(__execution); break;
    case 4: binary_inc_4_bytes(__execution); break;
    default: std::cout << "Error" << std::endl; exit(1); break;
    }

}

void virtual_machine::execute_BYTE_BINARY_DEC(int __arg, Execution* __execution) {

    std::cout << "BINARY_DEC" << std::endl;

    switch (__arg)
    {
    case 1: binary_dec_1_bytes(__execution); break;
    case 2: binary_dec_2_bytes(__execution); break;
    case 4: binary_dec_4_bytes(__execution); break;
    default: std::cout << "Error" << std::endl; exit(1); break;
    }

}




