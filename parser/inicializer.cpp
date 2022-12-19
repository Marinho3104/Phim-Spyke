#include "./inicializer.h"

#include "./tokenizer.h" // Tokenizer control

#include <iostream>

// Declarations for parser
namespace parser {

    char* code = 0;

    Tokenizer_Control* tokenizer_control = 0;

}


void parser::inicializeVariables() {

    tokenizer_control = new Tokenizer_Control(1);

}

void parser::deleteVariables() {

    delete tokenizer_control;

}


