#include "./compiler.h"

#include "inicializer.h"
#include "tokenizer.h"
#include "common.h"

#include <iostream>

compiler::Compiler::~Compiler() {

}

compiler::Compiler::Compiler(char* __code) : code(__code) {
    
    parser::code = utils::getFileContent((char*) "./built_ins/byte.ph");

    generateBuiltIns();

    free(parser::code);

}

void compiler::Compiler::generateBuiltIns() {

    parser::inicializeVariables();

    parser::tokenizer_control->generate();

    parser::deleteVariables();

}








