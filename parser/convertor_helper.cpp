#include "convertor_helper.h"

#include "token_definitions.h"
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

        parser::convertor_control->allocBlock();
        
        parser::convertor_control->setBlock(
            ((Ast_Node_Name_Space*) __node)->declarations
        );
    
        goto return_; break;
    
    case AST_NODE_CODE_BLOCK: goto return_; break;
    case AST_NODE_VARIABLE_DECLARATION: 
        
        _byte_code->add(
            getByteCodeOfNodeVariableDeclaration(
                (Ast_Node_Variable_Declaration*) __node
            )
        ); 
        
        goto return_; break;
        
    case AST_NODE_FUNCTION_DECLARATION:

        getByteCodeOfNodeFunctionDeclaration(
            (Ast_Node_Function_Declaration*) __node
        );

        goto return_; break;
        
    case AST_NODE_STRUCT_DECLARATION:

        getByteCodeOfNodeStructDeclaration(
            (Ast_Node_Struct_Declaration*) __node
        );

        goto return_; break;
    
    case AST_NODE_VARIABLE:

        _byte_code->add(
            getByteCodeOfNodeVariable(
                (Ast_Node_Variable*) __node
            )
        );

        goto return_; break;

    case AST_NODE_VALUE:

        _byte_code->add(
            getByteCodeOfNodeValue(
                (Ast_Node_Value*) __node
            )
        );

        goto return_; break;

    case AST_NODE_FUNCTION_CALL:

        _byte_code->add(
            getByteCodeOfNodeFunctionCall(
                (Ast_Node_Function_Call*) __node
            )
        );

        goto return_; break;

    default: break;
    }

    delete _byte_code;

    switch (__node->node_type)
    {
    case AST_NODE_EXPRESSION: 

        ((Ast_Node_Expression*) __node)->getResultDeclaration();
    
        _byte_code = getByteCodeOfNodeExpression(
            (Ast_Node_Expression*) __node
        );
        
        break;
    case AST_NODE_POINTER_OPERATION:

        _byte_code = getByteCodeOfNodePointerOperation(
            (Ast_Node_Pointer_Operation*) __node
        );

        break;
    case AST_NODE_PARENTHESIS:

        _byte_code = getByteCodeOfNodeParenthesis(
            (Ast_Node_Parenthesis*) __node
        );

        break;
    case AST_NODE_ACCESSING:

        _byte_code = getByteCodeOfNodeAccessing(
            (Ast_Node_Accessing*) __node
        );

        break;

    default: break;
    }

return_:
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

    __node_function_declaration->body_position = parser::convertor_control->allocBlock();

    parser::convertor_control->setBlock(
        __node_function_declaration->body->code
    );

}

void parser::getByteCodeOfNodeStructDeclaration(Ast_Node_Struct_Declaration* __node_struct_declaration) {

    parser::convertor_control->print("Node Struct Declaration - Byte Code");

    parser::convertor_control->allocBlock();

    parser::convertor_control->setBlock(
        __node_struct_declaration->functions->declarations
    );

}

utils::Linked_List <byte_code::Byte_Code*>* parser::getByteCodeOfNodeExpression(Ast_Node_Expression* __node_expression) {

    parser::convertor_control->print("Node Expression - Byte Code");

    utils::Linked_List <byte_code::Byte_Code*>* _byte_code = new utils::Linked_List <byte_code::Byte_Code*>(), *_temp;
    byte_code::Byte_Code* _call_function;

    _byte_code->destroy_content = 0;

    // std::cout << "Organized set -> " << __node_expression->organized_set << std::endl;

    for (int _ = 0; _ < __node_expression->organized_set->count; _++) {

        switch (__node_expression->organized_set->operator[](_)->node_type)
        {
        case AST_NODE_FUNCTION_DECLARATION:
            
            _call_function = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

            new (_call_function) byte_code::Byte_Code(
                BYTE_CODE_CALL,
                ((Ast_Node_Function_Declaration*) __node_expression->organized_set->operator[](_))->body_position
            );

            _byte_code->add(
                _call_function
            );

            break;
        
        default:

            _temp = getByteCodeOfNode(
                __node_expression->organized_set->operator[](_)
            );

            _byte_code->join(
                _temp
            );

            delete _temp;

            break;
        }

    }

    return _byte_code;

}

byte_code::Byte_Code* parser::getByteCodeOfNodeVariable(Ast_Node_Variable* __node_variable) {

    parser::convertor_control->print("Node Variable - Byte Code");

    byte_code::Byte_Code* _byte_code = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new (_byte_code) byte_code::Byte_Code(
        __node_variable->representive_declaration->global ? BYTE_CODE_LOAD_GLOBAL_VARIABLE : BYTE_CODE_LOAD_VARIABLE,
        __node_variable->declaration_id
    );

    return _byte_code;

}

byte_code::Byte_Code* parser::getByteCodeOfNodeValue(Ast_Node_Value* __node_value) {

    parser::convertor_control->print("Node Value - Byte Code");

    byte_code::Byte_Code* _byte_code = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new (_byte_code) byte_code::Byte_Code(
        BYTE_CODE_LOAD_IMPLICIT_VALUE,
        __node_value->implicit_value_position
    );

    return _byte_code;  

}

utils::Linked_List <byte_code::Byte_Code*>* parser::getByteCodeOfNodePointerOperation(Ast_Node_Pointer_Operation* __node_pointer_operation) {

    parser::convertor_control->print("Node Pointer Operation - Byte Code");

    utils::Linked_List <byte_code::Byte_Code*>* _pointer_operation = getByteCodeOfNode(
        __node_pointer_operation->value
    );

    if (__node_pointer_operation->pointer_level) {

        byte_code::Byte_Code* _byte_code_chng = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

        char _code = __node_pointer_operation->pointer_level > 0 ? BYTE_CODE_POINTER_UP : BYTE_CODE_POINTER_DOWN;
        int _chng = __node_pointer_operation->pointer_level;

        if (_code == BYTE_CODE_POINTER_DOWN) _chng *= -1;

        new (_byte_code_chng) byte_code::Byte_Code(
            _code, _chng
        );

        _pointer_operation->add(
            _byte_code_chng
        );

    }

    return _pointer_operation;

}

utils::Linked_List <byte_code::Byte_Code*>* parser::getByteCodeOfNodeParenthesis(Ast_Node_Parenthesis* __node_parenthesis) {

    parser::convertor_control->print("Node Parenthesis - Byte Code");

    return getByteCodeOfNodeExpression(
        __node_parenthesis->expression
    );

}

byte_code::Byte_Code* parser::getByteCodeOfNodeFunctionCall(Ast_Node_Function_Call* __node_function_call) {

    parser::convertor_control->print("Node Function Call - Byte Code");

    byte_code::Byte_Code* _byte_code = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

    new (_byte_code) byte_code::Byte_Code(
        BYTE_CODE_CALL,
        __node_function_call->function_declaration->body_position
    );

    return _byte_code;

}

utils::Linked_List <byte_code::Byte_Code*>* parser::getByteCodeOfNodeAccessing(Ast_Node_Accessing* __node_accessing) {

    parser::convertor_control->print("Node Accessing - Byte Code");

    utils::Linked_List <byte_code::Byte_Code*>* _byte_code = new utils::Linked_List <byte_code::Byte_Code*>();

    switch (__node_accessing->accessing->node_type)
    {
    case AST_NODE_VARIABLE:
        
        std::cout << "Off -> " << __node_accessing->value->representive_declaration->type->declaration->getVariablesOff((Ast_Node_Variable*)__node_accessing->accessing) << std::endl;
        break;

    case AST_NODE_FUNCTION_CALL:



        break;
    
    default:
        break;
    }

    exit(1);

    return _byte_code;

}



