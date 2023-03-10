#pragma once

#ifndef VIRTUAL_MACHINE_DEFINITIONS_H
#define VIRTUAL_MACHINE_DEFINITIONS_H

#include <stdint.h>

typedef uint16_t address_t;

// Forward
namespace virtual_machine {

    struct Byte_Code_Store;
    struct Execution;
    struct Program;
    struct Memory;
    struct Stack;

}

// Forward
namespace byte_code {

    struct Byte_Code_Block;
    struct Compiled_Code;
    struct Byte_Code;

}

// Forward
namespace utils {

    template <typename> struct Linked_List;

}

#endif