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

virtual_machine::Execution::Execution(Program* __program) : program(__program), current_block(-1), current_index(0)
    { stacks = new utils::Linked_List <Stack*>(); }

void virtual_machine::Execution::addStack(int __new_code_block_index) {

    Stack* _stack = (Stack*) malloc(sizeof(Stack));

    new (_stack) Stack(
        program->memory->top_stack_memory + 1,
        current_block, current_index
    );

    current_block = __new_code_block_index;
    current_index = 0;

    stacks->add(
        _stack
    );

}

void virtual_machine::Execution::popStack() { 
    
    program->memory->deallocateStack(stacks->last->object->inicial_position - 1); 

    current_block = stacks->last->object->previous_block_index;
    current_index = stacks->last->object->previous_index;
    
    delete stacks->remove(stacks->count); 

}

void virtual_machine::Execution::execute() {

    // execute global name space

    addStack(0);

    byte_code::Byte_Code* _current_byte_code;

    while((_current_byte_code = program->byte_code->getByteCode(current_block, current_index))->code != BYTE_CODE_END_CODE_BLOCK) {

        virtual_machine::executeByteCode(_current_byte_code, this);

        current_index ++;

    }

    current_block = program->entry_point;
    current_index = 0;

    addStack(current_block);

    // execute entry point

    while((_current_byte_code = program->byte_code->getByteCode(current_block, current_index))->code != BYTE_CODE_END_CODE_BLOCK) {

        virtual_machine::executeByteCode(_current_byte_code, this);

        current_index ++;

    }

    popStack();
    
    popStack();

    std::cout << "Stack count -> " << stacks->count << std::endl;  
    std::cout << "Current block -> " << current_block << std::endl;  

}