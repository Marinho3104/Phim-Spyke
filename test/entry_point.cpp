
#define COMMON "common.h"

#include COMMON

#include <iostream>

#include "./compiler.h"

#include "memory.h"

#include "byte_code.h"


int main() {

    char* _contract_data = utils::getFileContent((char*) "./test/my_contract.ph");

    compiler::Compiler* _compiler = new compiler::Compiler(_contract_data);

    delete _compiler;

    free(_contract_data);

    std::cout << "Reading back:" << std::endl;

    byte_code::Compiled_Code* _compiled_code_readed = byte_code::Compiled_Code::getByFile();

}