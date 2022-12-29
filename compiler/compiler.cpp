#include "./compiler.h"

#include "inicializer.h"
#include "tokenizer.h"
#include "common.h"
#include "ast.h"

#include <iostream>

compiler::Compiler::~Compiler() {

}

compiler::Compiler::Compiler(char* __code) : code(__code) { generateBuiltIns(); /* generate(); */ }

void compiler::Compiler::generateBuiltIns() {

    parser::code = utils::getFileContent((char*) "./built_ins/byte.ph");

    parser::inicializeVariables();

    parser::tokenizer_control->generate();

    parser::ast_control->generate();

    parser::deleteVariables();

    free(parser::code);

}

void compiler::Compiler::generate() {

    std::cout << "------------- Code Compiler -------------" << std::endl;

    parser::code = code;

    parser::inicializeVariables();

    parser::tokenizer_control->generate();

    parser::ast_control->generate();

    parser::deleteVariables();

}








