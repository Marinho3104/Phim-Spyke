#pragma once

#ifndef PARSER_DEFINITIONS_H
#define PARSER_DEFINITIONS_H

typedef unsigned char int_8;

// Forward
namespace parser {

    // Token
    struct Token;
    
    // Tokenizer
    struct Tokenizer_Control;

    // Exception Handle
    struct Exception_Handle;

    // Ast 
    struct Declaration_Tracker;
    struct Type_Information;
    struct Ast_Control;
    struct Name_Space;

    // Ast Nodes
    struct Ast_Node;
    struct Ast_Node_Name_Space;
    struct Ast_Node_Code_Block;
    struct Ast_Node_Variable_Declaration;
    struct Ast_Node_Function_Declaration;
    struct Ast_Node_Struct_Declaration;

}

// Forward
namespace utils {

    template <typename> struct Linked_List;

}

// Extern variables
namespace parser {

    extern Tokenizer_Control* tokenizer_control;
    extern Exception_Handle* exception_handle;
    extern Ast_Control* ast_control;
    extern char* code;
}

#endif