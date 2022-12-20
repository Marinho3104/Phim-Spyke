#include "./inicializer.h"

#include "./tokenizer.h" // Tokenizer control
#include "ast.h" // Ast Control

#include <iostream>

// Declarations for parser
namespace parser {

    char* code = 0;

    Tokenizer_Control* tokenizer_control = 0;
    Ast_Control* ast_control = 0;

}


void parser::inicializeVariables() {

    tokenizer_control = new Tokenizer_Control(1);
    ast_control = new Ast_Control(1);

}

void parser::deleteVariables() {

    delete tokenizer_control;
    delete ast_control;

}


