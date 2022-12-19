#pragma once

#ifndef PARSER_DEFINITIONS_H
#define PARSER_DEFINITIONS_H

typedef unsigned char int_8;


// Extern variables
namespace parser {

    extern char* code;

}

// Forward
namespace parser {

    struct Tokenizer_Control;
    struct Token;

}

// Forward
namespace utils {

    template <typename> struct Linked_List;

}

#endif