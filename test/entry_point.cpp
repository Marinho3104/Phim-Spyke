
#define COMMON "common.h"

#include COMMON

#include <iostream>

#include "./compiler.h"

#include "memory.h"


int main() {

    virtual_machine::Memory* _memory = new virtual_machine::Memory();

    std::cout << _memory->allocateStack(4096) << std::endl;
    std::cout << _memory->getRealAddress(4096) << std::endl;

    delete _memory;

    return 0;

    char* _contract_data = utils::getFileContent((char*) "./test/my_contract.ph");

    compiler::Compiler* _compiler = new compiler::Compiler(_contract_data);

    delete _compiler;

    free(_contract_data);

}