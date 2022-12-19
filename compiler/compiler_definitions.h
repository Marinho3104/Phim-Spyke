#pragma once

#ifndef COMPILER_DEFINITIONS_H
#define COMPILER_DEFINITIONS_H

// Forward
namespace parser {

    struct Tokenizer_Control;

}

// Extern Variables
namespace parser {

    extern Tokenizer_Control* tokenizer_control;
    extern char* code;

}

#endif