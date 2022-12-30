#include "./inicializer.h"

#include "exception_handle.h" // Exception Handle
#include "pre_compiler.h" // Pre Compiler Control
#include "./tokenizer.h" // Tokenizer control
#include "convertor.h" // Convertor
#include "ast.h" // Ast Control

#include <iostream>

// Declarations for parser
namespace parser {

    char* code = 0;

    Pre_Compiler_Control* pre_compiler_control = 0;
    Tokenizer_Control* tokenizer_control = 0;
    Convertor_Control* convertor_control = 0;
    Exception_Handle* exception_handle = 0;
    Ast_Control* ast_control = 0;

}


void parser::inicializeVariables() {

    pre_compiler_control = new Pre_Compiler_Control(1);
    tokenizer_control = new Tokenizer_Control(1);
    exception_handle = new Exception_Handle();
    convertor_control = new Convertor_Control(1);
    ast_control = new Ast_Control(1);

}

void parser::deleteVariables() {

    delete pre_compiler_control;
    delete tokenizer_control;
    delete convertor_control;
    delete exception_handle;
    delete ast_control;

}


