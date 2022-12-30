#include "./compiler.h"

#include "pre_compiler.h"
#include "tokenizer.h"
#include "convertor.h"
#include "ast.h"

#include "inicializer.h"
#include "common.h"

#include <iostream>

compiler::Compiler::~Compiler() {

}

compiler::Compiler::Compiler(char* __code) : code(__code) { generateBuiltIns(); /* generate(); */ }

void compiler::Compiler::generateBuiltIns() {

    parser::code = utils::getFileContent((char*) "./built_ins/byte.ph");

    parser::inicializeVariables();

    parser::tokenizer_control->generate();

    parser::pre_compiler_control->preCompile();

    parser::ast_control->generate();

    parser::convertor_control->generate();

    parser::convertor_control->getCompiledByteCode();

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








