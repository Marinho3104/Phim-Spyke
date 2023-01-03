#pragma once

#ifndef PARSER_CONVERTOR_H
#define PARSER_CONVERTOR_H

#include "convertor_definitions.h"
#include "parser_definitions.h"
#include "ast_definitions.h"

namespace parser {

    struct Convertor_Control {

        utils::Linked_List <byte_code::Byte_Code_Block*>* byte_code_blocks;

        byte_code::Byte_Code_Block* block_in_set;

        bool debug_mode;

        ~Convertor_Control(); Convertor_Control(bool);

        void print(const char*);

        void generate();

        void setBlock(utils::Linked_List<Ast_Node*>*, byte_code::Byte_Code_Block*);

        int allocBlock();

        byte_code::Compiled_Code* getCompiledByteCode();

        void setEndByteCode();

        void saveByteCode();


    };

}

#endif