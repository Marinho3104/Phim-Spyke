#include "convertor_helper.h"

#include "ast_definitions.h"
#include "linked_List.h"
#include "byte_code.h"
#include "ast.h"
#include "ast_helper.h"
#include "opcodes.h"
#include "convertor.h"
#include "ast_nodes.h"

#include <iostream>

utils::Linked_List <byte_code::Byte_Code*>* parser::getByteCodeOfNode(Ast_Node* __node) {

    utils::Linked_List <byte_code::Byte_Code*>* _byte_code = new utils::Linked_List <byte_code::Byte_Code*>();
    _byte_code->destroy_content = 0;

    switch (__node->node_type)
    {
    case AST_NODE_NAME_SPACE:
        
        parser::convertor_control->setBlock(
            ((Ast_Node_Name_Space*) __node)->declarations
        );
    
        break;
    
    case AST_NODE_CODE_BLOCK: break;
    case AST_NODE_VARIABLE_DECLARATION: 
        
        _byte_code->add(
            getByteCodeOfNodeVariableDeclaration(
                (Ast_Node_Variable_Declaration*) __node
            )
        ); 
        
        break;
        
    case AST_NODE_FUNCTION_DECLARATION:

        getByteCodeOfNodeFunctionDeclaration(
            (Ast_Node_Function_Declaration*) __node
        );

        break;
        
    default: break;
    }

    return _byte_code;

}

byte_code::Byte_Code* parser::getByteCodeOfNodeVariableDeclaration(Ast_Node_Variable_Declaration* __node_variable_declaration) {

    parser::convertor_control->print("Node Variable Declaration - Byte Code");

    byte_code::Byte_Code* _byte_code = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new (_byte_code) byte_code::Byte_Code(
        BYTE_CODE_STACK_MEMORY_ALLOCATE,
        __node_variable_declaration->type->getSize()
    );

    return _byte_code;


}

void parser::getByteCodeOfNodeFunctionDeclaration(Ast_Node_Function_Declaration* __node_function_declaration) {

    parser::convertor_control->print("Node Function Declaration - Byte Code");

    parser::convertor_control->setBlock(
        __node_function_declaration->body->code
    );

}

