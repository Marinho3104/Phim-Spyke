#include "memory.h"

#include "memory_settings.h"

#include <sys/mman.h>
#include <iostream>


virtual_machine::Memory::~Memory() {}

virtual_machine::Memory::Memory() : top_stack_memory(-1) {

    stack_memory = mmap(NULL, STACK_MEMORY_SIZE + HEAP_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    heap_memory = stack_memory + STACK_MEMORY_SIZE;

    std::cout << "Stack memory -> " << stack_memory << std::endl;
    std::cout << "Heap memory -> " << heap_memory << std::endl;

}

int virtual_machine::Memory::allocateStack(int __s) {

    if ((__s + top_stack_memory) >= STACK_MEMORY_SIZE || __s < 0) return -1;

    int _address = top_stack_memory + 1;

    top_stack_memory += __s;

    return _address;


}

void virtual_machine::Memory::deallocateStack(int __s) {

    if (__s < 0) return;

    if (__s > top_stack_memory) __s = top_stack_memory;

    top_stack_memory -= __s;

}

void* virtual_machine::Memory::getRealAddress(int __a) {

    if (__a < 0 || __a > STACK_MEMORY_SIZE + HEAP_MEMORY_SIZE) return 0;

    return stack_memory + __a;

}

