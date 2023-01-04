#pragma once

#ifndef VIRTUAL_MACHINE_STACK_H
#define VIRTUAL_MACHINE_STACK_H

#include "virtual_machine_definitions.h"
#include "byte_code_definitions.h"
#include "linked_List.h"

namespace virtual_machine {

    struct Stack {

        int inicial_position;

        utils::Linked_List <int>* stack;

        ~Stack(); Stack(int);

        void addToStack(int);

        int popFromStack();

    };

}

#endif