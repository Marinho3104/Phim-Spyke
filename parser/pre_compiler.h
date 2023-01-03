#pragma once

#ifndef PARSER_PRE_COMPILER_H
#define PARSER_PRE_COMPILER_H

#include "parser_definitions.h"

namespace parser {

    struct Pre_Compiler_Instruction {

        utils::Linked_List <Token*>* instructions;

        ~Pre_Compiler_Instruction(); Pre_Compiler_Instruction(utils::Linked_List <Token*>*);

    };

    struct Pre_Compiler_Define { // not corrected

        utils::Linked_List <Token*>* names_to_find, *name_to_replace;

        ~Pre_Compiler_Define(); Pre_Compiler_Define(utils::Linked_List <Pre_Compiler_Instruction*>*);

        void addNewPair(Token*, Token*);

        Token* getFindToken(char*);

        int getTokenPositionByLine(int);

        void replace();

    };

    struct Pre_Compiler_Control {

        utils::Linked_List <Pre_Compiler_Instruction*>* pre_compiler_instructions;
        Pre_Compiler_Define* pre_compiler_define;

        int current_position;

        bool debug_mode;

        ~Pre_Compiler_Control(); Pre_Compiler_Control(bool);

        void print(const char*);

        void preCompile();

    };

}

#endif