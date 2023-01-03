#include "stack.h"

#include "linked_List.h"

#include <iostream>


virtual_machine::Stack::~Stack() { delete stack; }

virtual_machine::Stack::Stack(int __inicial_position, int __previous_block_index, int __previous_index) 
    : inicial_position(__inicial_position), previous_block_index(__previous_block_index), previous_index(__previous_index) 
        { stack = new utils::Linked_List <int>(); }

void virtual_machine::Stack::addToStack(int __value) { stack->add(__value); }

int virtual_machine::Stack::popFromStack() {

    int _value = stack->last->object;

    delete stack->remove(stack->count);

    return _value;

}


