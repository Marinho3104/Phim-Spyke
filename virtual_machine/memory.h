#pragma once

#ifndef VIRTUAL_MACHINE_MEMORY_H
#define VIRTUAL_MACHINE_MEMORY_H

namespace virtual_machine {

    /*  Emulate memory needed in program execution (a.k.a contract) in virtual machine
    *
    *       Stack memory and heap memory
    */
    struct Memory {

        void* stack_memory, *heap_memory;
        int top_stack_memory;


        ~Memory(); Memory();

        /**/
        int allocateStack(int);

        /**/
        void deallocateStack(int);

        void* getRealAddress(int);

    };

}

#endif