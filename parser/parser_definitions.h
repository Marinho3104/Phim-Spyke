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

    // Pre Compiler
    struct Pre_Compiler_Control;

    // Convertor 
    struct Convertor_Control;

    // Ast 
    struct Expression_Result_Helper;
    struct Declarations_Tracker;
    struct Type_Information;
    struct Ast_Control;
    struct Name_Space;

    // Ast Nodes
    struct Ast_Node;
    struct Ast_Node_If;
    struct Ast_Node_Cast;
    struct Ast_Node_Else;
    struct Ast_Node_Value;
    struct Ast_Node_Return;
    struct Ast_Node_Else_If;
    struct Ast_Node_Variable;
    struct Ast_Node_Accessing;
    struct Ast_Node_Byte_Code;
    struct Ast_Node_Name_Space;
    struct Ast_Node_Code_Block;
    struct Ast_Node_Expression;
    struct Ast_Node_Parenthesis;
    struct Ast_Node_Function_Call;
    struct Ast_Node_Function_Size_Of;
    struct Ast_Node_Pointer_Operation;
    struct Ast_Node_Variable_Declaration;
    struct Ast_Node_Function_Declaration;
    struct Ast_Node_Struct_Declaration;

}

// Forward
namespace utils {

    template <typename> struct Linked_List;

}

// Forward
namespace byte_code {

    struct Byte_Code;
    struct Compiled_Code;
    struct Byte_Code_Block;

}

// Extern variables
namespace parser {

    extern Pre_Compiler_Control* pre_compiler_control;
    extern Tokenizer_Control* tokenizer_control;
    extern Convertor_Control* convertor_control;
    extern Exception_Handle* exception_handle;
    extern Ast_Control* ast_control;
    extern char* code;
}

#endif