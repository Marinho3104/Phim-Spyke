#include "exception_handle.h"

#include "ast.h"

#include "linked_List.h"
#include "common.h"
#include "token.h"

#include <iostream>
#include <string.h>

parser::Exception_Handle::~Exception_Handle() { delete code_by_lines; }

parser::Exception_Handle::Exception_Handle() {

    code_by_lines = new utils::Linked_List <char*>();

    char** _splitted_string = utils::splitString(code, '\n'), **_temp = _splitted_string;

    while(*_splitted_string) 

        code_by_lines->add(
            *(_splitted_string++)
        );

    free(_temp);

}

void parser::Exception_Handle::runException(const char* __information) {

    std::cout << __information << std::endl;

    throw "";

}

void parser::Exception_Handle::runExceptionAstControl(const char* __information) {

    Token* _token = ast_control->getToken(0);

    std::cout << "\n\nError:\n" << std::endl;

    std::cout << "\t" << __information << ":" << std::endl;

    std::cout << _token->position_information.line << ":" << _token->position_information.column << std::endl;

    std::cout << "\t\t" << code_by_lines->operator[](_token->position_information.line) << std::endl;

    std::cout << "\t\t";

    for (int _ = 0; _ < _token->position_information.column; _++)

        std::cout << " ";

    for (int _ = 0; _ < _token->position_information.final_column - _token->position_information.column; _++)

        std::cout << "^";

    std::cout << std::endl;

    throw "";

}





