#pragma once


#ifndef PARSER_EXCEPTION_HANDLE_H
#define PARSER_EXCEPTION_HANDLE_H

#include "parser_definitions.h"

namespace parser {


    struct Exception_Handle {

        utils::Linked_List <char*>* code_by_lines;

        ~Exception_Handle(); Exception_Handle();

        void runException(const char*);

        void runExceptionAstControl(const char*);

    };

}

#endif