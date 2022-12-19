#include "./compiler.h"

#include "./inicializer.h"
#include "./tokenizer.h"

#include <iostream>

compiler::Compiler::~Compiler() {

}

compiler::Compiler::Compiler(char* __code) {
    
    generateBuiltIns();

}

void compiler::Compiler::generateBuiltIns() {

    parser::inicializeVariables();

    parser::tokenizer_control->generate();

    parser::deleteVariables();

}








