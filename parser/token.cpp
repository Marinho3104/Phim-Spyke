#include "./token.h"

#include <iostream>


parser::Token::~Token() { free(identifier); }

parser::Token::Token(int_8 __id, int __column_position, int __final_column_position, int __line_position) : id(__id), identifier(NULL) 
    { position_information.column = __column_position; position_information.line = __line_position; position_information.final_column = __final_column_position; }

parser::Token::Token(int_8 __id, char* __identifier, int __column_position, int __final_column_position, int __line_position) : id(__id), identifier(__identifier) 
    { position_information.column = __column_position; position_information.line = __line_position; position_information.final_column = __final_column_position; }


