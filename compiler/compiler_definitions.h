#pragma once

#ifndef COMPILER_DEFINITIONS_H
#define COMPILER_DEFINITIONS_H

// Forward
namespace parser {

    struct Tokenizer_Control;
    struct Ast_Control;

}

// Extern Variables
namespace parser {

    extern Tokenizer_Control* tokenizer_control;
    extern Ast_Control* ast_control;
    extern char* code;

}

#endif