#include "./compiler.h"

#include "built_ins_helper.h"
#include "pre_compiler.h"
#include "tokenizer.h"
#include "convertor.h"
#include "byte_code.h"
#include "ast.h"

#include "inicializer.h"
#include "common.h"

#include <iostream>

compiler::Compiler::~Compiler() {

}

compiler::Compiler::Compiler(char* __code) : code(__code) { generateBuiltIns(); /* generate(); */ }

void compiler::Compiler::generateBuiltIns() {

    char** _names = built_ins::getPrimitivesNames();

    parser::code = utils::getFilesContent((char*) "./built_ins/code/", _names);

    char** _names_cpy = _names;

    while(*_names) free(*(_names++));
    free(_names_cpy);

    parser::inicializeVariables();

    parser::tokenizer_control->generate();

    parser::pre_compiler_control->preCompile();

    parser::ast_control->generate();

    parser::convertor_control->generate();

    byte_code::Compiled_Code* _compiled_code = parser::convertor_control->getCompiledByteCode();

    _compiled_code->print();

    _compiled_code->saveInFile();

    delete _compiled_code;

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








