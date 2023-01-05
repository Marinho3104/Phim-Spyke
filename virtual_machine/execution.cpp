#include "execution.h"
 
#include "byte_code_execution.h"
#include "byte_code_store.h"
#include "linked_List.h"
#include "byte_code.h"
#include "opcodes.h"
#include "program.h"
#include "memory.h"
#include "stack.h"

#include <iostream>


virtual_machine::Execution::~Execution() { delete stacks; }

virtual_machine::Execution::Execution(Program* __program) : program(__program)
    { stacks = new utils::Linked_List <Stack*>(); }

void virtual_machine::Execution::addStack() {

    Stack* _stack = (Stack*) malloc(sizeof(Stack));

    new (_stack) Stack(
        program->memory->top_stack_memory + 1
    );

    stacks->add(
        _stack
    );

}

void virtual_machine::Execution::popStack() { 
    
    program->memory->deallocateStack(stacks->last->object->inicial_position - 1); 

    delete stacks->remove(stacks->count); 

}

void virtual_machine::Execution::executeBlock(int __block_index, bool __stack_chage) {

    byte_code::Byte_Code* _current_byte_code;
    int _current_index = 0;

    if (__stack_chage) addStack();

    while((_current_byte_code = program->byte_code->getByteCode(__block_index, _current_index))->code != BYTE_CODE_END_CODE_BLOCK) {

        virtual_machine::executeByteCode(_current_byte_code, this);

        _current_index ++;

    }

    if (__stack_chage) popStack();

}


void virtual_machine::Execution::execute() {

    addStack();

    executeBlock(0, 0);

    executeBlock(program->entry_point, 1);

    popStack();

}