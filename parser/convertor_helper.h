#pragma once

#ifndef PARSER_CONVERTOR_HELPER_H
#define PARSER_CONVERTOR_HELPER_H

#include "parser_definitions.h"

namespace parser {

    utils::Linked_List <byte_code::Byte_Code*>* getByteCodeOfNode(Ast_Node*);

    byte_code::Byte_Code* getByteCodeOfNodeVariableDeclaration(Ast_Node_Variable_Declaration*);
    
    void getByteCodeOfNodeFunctionDeclaration(Ast_Node_Function_Declaration*);


}

#endif