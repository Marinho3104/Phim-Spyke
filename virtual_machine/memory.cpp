#include "memory.h"

#include "memory_settings.h"

#include <sys/mman.h>
#include <iostream>
#include <string.h>


virtual_machine::Memory::~Memory() {

    if (top_stack_memory != -1) std::cout << "Not all memory deallocated" << std::endl; 

}

virtual_machine::Memory::Memory(void* __implicit_memory) : top_stack_memory(-1) {

    stack_memory = mmap(NULL, STACK_MEMORY_SIZE + HEAP_MEMORY_SIZE + IMPLICIT_VALUES_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    heap_memory = stack_memory + STACK_MEMORY_SIZE;
    implicit_value_memory = heap_memory + HEAP_MEMORY_SIZE;

    mempcpy(implicit_value_memory, __implicit_memory, 4096);

    // std::cout << "Implicit value -> " << *((int*) implicit_value_memory) << std::endl;
    // std::cout << "Implicit value -> " << *(((int*) implicit_value_memory) + 1) << std::endl;

    // std::cout << "Stack memory -> " << stack_memory << std::endl;
    // std::cout << "Heap memory -> " << heap_memory << std::endl;

}

int virtual_machine::Memory::allocateHeap(int __s) {

    std::cout << __s + STACK_MEMORY_SIZE << std::endl;

    if ((__s + STACK_MEMORY_SIZE) >= STACK_MEMORY_SIZE + HEAP_MEMORY_SIZE || __s < 0) return -1;

    return __s + STACK_MEMORY_SIZE;   

}

int virtual_machine::Memory::allocateStack(int __s) {

    std::cout << __s + top_stack_memory << std::endl;

    if ((__s + top_stack_memory) >= STACK_MEMORY_SIZE || __s < 0) return -1;

    int _address = top_stack_memory + 1;

    top_stack_memory += __s;

    return _address;


}

void virtual_machine::Memory::deallocateStack(int __s) {

    if (__s < -1 >= top_stack_memory) return;

    // For degub only
    while(top_stack_memory != __s) { void* _ = getRealAddress(top_stack_memory); *((char*) _) = 0; top_stack_memory--; }

    //top_stack_memory = __s;

}

void* virtual_machine::Memory::getRealAddress(int __a) {

    if (__a < 0 || __a > STACK_MEMORY_SIZE + HEAP_MEMORY_SIZE + IMPLICIT_VALUES_MEMORY_SIZE) return 0;

    return stack_memory + __a;

}

