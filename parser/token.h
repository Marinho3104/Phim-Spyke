#pragma once

#ifndef PARSER_TOKEN_H
#define PARSER_TOKEN_H

#include "./parser_definitions.h"

namespace parser {

    struct Position_Information {

        int column, final_column, line;

    };

    struct Token {

        int_8 id;
        char* identifier;
        Position_Information position_information;

        ~Token(); Token(int_8, int, int, int); Token(int_8, char*, int, int, int);

    };


}

#endif