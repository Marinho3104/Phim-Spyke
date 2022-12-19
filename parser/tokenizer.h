#pragma once

#ifndef PARSER_TOKENIZER_H
#define PARSER_TOKENIZER_H

#include "./parser_definitions.h"

namespace parser {

    struct Tokenizer_Control {

        utils::Linked_List <Token*>* tokens_collection;
        char* code_copy;

        bool debug_mode;

        ~Tokenizer_Control(); Tokenizer_Control(bool);

        void print(const char*);

        void addToken(Token*);

        void generate();

    };

}

#endif
