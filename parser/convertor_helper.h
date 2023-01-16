#pragma once

#ifndef PARSER_CONVERTOR_HELPER_H
#define PARSER_CONVERTOR_HELPER_H

#include "parser_definitions.h"

namespace parser {

    byte_code::Byte_Code* getByteCodeOfExpressionTokenId(int);

    utils::Linked_List <byte_code::Byte_Code*>* getByteCodeOfNode(Ast_Node*);

    utils::Linked_List <byte_code::Byte_Code*>* getByteCodeOfNodeVariableDeclaration(Ast_Node_Variable_Declaration*);
    
    void getByteCodeOfNodeFunctionDeclaration(Ast_Node_Function_Declaration*);

    utils::Linked_List <byte_code::Byte_Code*>* getByteCodeOfNodeStructDeclaration(Ast_Node_Struct_Declaration*);

    utils::Linked_List <byte_code::Byte_Code*>* getByteCodeOfNodeExpression(Ast_Node_Expression*);
    
    byte_code::Byte_Code* getByteCodeOfNodeVariable(Ast_Node_Variable*);

    byte_code::Byte_Code* getByteCodeOfNodeValue(Ast_Node_Value*);

    utils::Linked_List <byte_code::Byte_Code*>* getByteCodeOfNodePointerOperation(Ast_Node_Pointer_Operation*);

    utils::Linked_List <byte_code::Byte_Code*>* getByteCodeOfNodeParenthesis(Ast_Node_Parenthesis*);

    utils::Linked_List<byte_code::Byte_Code*>* getByteCodeOfNodeFunctionCall(Ast_Node_Function_Call*);

    utils::Linked_List <byte_code::Byte_Code*>* getByteCodeOfNodeAccessing(Ast_Node_Accessing*);

    byte_code::Byte_Code* getByteCodeOfNodeByteCode(Ast_Node_Byte_Code*);

    utils::Linked_List <byte_code::Byte_Code*>* getByteCodeOfNodeReturn(Ast_Node_Return*);

    utils::Linked_List <byte_code::Byte_Code*>* getByteCodeOfNodeIf(Ast_Node_If*);

    byte_code::Byte_Code* getByteCodeOfNodeFunctionSizeOf(Ast_Node_Function_Size_Of*);

    utils::Linked_List <byte_code::Byte_Code*>* getByteCodeOfNodeCast(Ast_Node_Cast*);

    int getImplicitValueOff(int);

}

#endif