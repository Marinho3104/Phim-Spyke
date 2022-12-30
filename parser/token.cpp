#include "./token.h"

#include <iostream>
#include <string.h>

parser::Token::~Token() { free(identifier); }

parser::Token::Token(int_8 __id, int __column_position, int __final_column_position, int __line_position) : id(__id), identifier(NULL) 
    { position_information.column = __column_position; position_information.line = __line_position; position_information.final_column = __final_column_position; }

parser::Token::Token(int_8 __id, char* __identifier, int __column_position, int __final_column_position, int __line_position) : id(__id), identifier(__identifier) 
    { position_information.column = __column_position; position_information.line = __line_position; position_information.final_column = __final_column_position; }

parser::Token* parser::Token::getCopy() {

    parser::Token* _token_copy = (parser::Token*) malloc(sizeof(parser::Token));

    char* _identifier = NULL;

    if (identifier) {

        _identifier = (char*) malloc(strlen(identifier) + 1);

        strcpy(_identifier, identifier);

    }

    new (_token_copy) parser::Token(
        id, _identifier, position_information.column, position_information.final_column, position_information.line
    );    

    return _token_copy;

}

