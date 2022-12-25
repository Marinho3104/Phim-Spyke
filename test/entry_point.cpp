
#include "common.h"

#include <iostream>

#include "./compiler.h"


int main() {

    char* _contract_data = utils::getFileContent((char*) "./test/my_contract.ph");

    compiler::Compiler* _compiler = new compiler::Compiler(_contract_data);

    delete _compiler;

    free(_contract_data);

}