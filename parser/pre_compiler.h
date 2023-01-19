#pragma once

#ifndef PARSER_PRE_COMPILER_H
#define PARSER_PRE_COMPILER_H

#include "parser_definitions.h"

namespace parser {

    struct Pre_Compiler_Define {

        char* name_to_find;
        
        Token* token_to_replace;

        ~Pre_Compiler_Define(); Pre_Compiler_Define(char*, Token*);

        static void addNewDefine();

        static void handle();

        static int getDefinedPosition(char*);

    };

    struct Pre_Compiler_Include {

        static void handle();

    };

    struct Pre_Compiler_If_Def {

        static void handle();


    };

    struct Pre_Compiler_If_N_Def {
        
        static void handle();

    };

    struct Pre_Compiler_Else {

        static void handle(bool);

    };

    struct Pre_Compiler_End_If {

        static void handle();

    };

    struct Pre_Compiler_Control {

        utils::Linked_List <Pre_Compiler_Define*>* defines;

        int current_position;

        bool debug_mode;

        ~Pre_Compiler_Control(); Pre_Compiler_Control(bool);

        void print(const char*);

        void preCompile();

        void generate(bool);

        void ignoreBlock();

    };

}

#endif