#include "./inicializer.h"

#include "exception_handle.h" // Exception Handle
#include "./tokenizer.h" // Tokenizer control
#include "ast.h" // Ast Control

#include <iostream>

// Declarations for parser
namespace parser {

    char* code = 0;

    Tokenizer_Control* tokenizer_control = 0;
    Exception_Handle* exception_handle = 0;
    Ast_Control* ast_control = 0;

}


void parser::inicializeVariablesBuiltIns() {

    tokenizer_control = new Tokenizer_Control(1);
    exception_handle = new Exception_Handle();
    ast_control = new Ast_Control(1);

}

void parser::inicializeVariables() {

    tokenizer_control = new Tokenizer_Control(1);
    exception_handle = new Exception_Handle();
    ast_control = new Ast_Control(ast_control, 1);

}

void parser::deleteVariables() {

    delete tokenizer_control;
    delete exception_handle;
    delete ast_control;

}

void parser::deleteVariablesBuiltIns() {

    delete tokenizer_control;
    delete exception_handle;

}


