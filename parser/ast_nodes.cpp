#include "ast_nodes.h"

#include "exception_handle.h"
#include "tokenizer.h"

#include "token_definitions.h"
#include "ast_definitions.h"
#include "linked_List.h"
#include "ast_helper.h"
#include "token.h"
#include "ast.h"

#include <iostream>


parser::Ast_Node::~Ast_Node() {}

parser::Ast_Node::Ast_Node(int __node_type, int __declaration_id) : node_type(__node_type), declaration_id(__declaration_id) {}


parser::Ast_Node_Name_Space::~Ast_Node_Name_Space() {

}

parser::Ast_Node_Name_Space::Ast_Node_Name_Space(Name_Space* __name_space) : Ast_Node(AST_NODE_NAME_SPACE, -1), name_space(__name_space) {}

void parser::Ast_Node_Name_Space::generate(Name_Space* __name_space) {

    parser::ast_control->debug_information_tab++;
    parser::ast_control->print("Ast Node Name Space\n");

    Ast_Node_Name_Space* _node_name_space = (Ast_Node_Name_Space*) malloc(sizeof(Ast_Node_Name_Space));

    new (_node_name_space) Ast_Node_Name_Space(
        __name_space
    );

    parser::ast_control->name_space_node_collection->add(_node_name_space);
    parser::ast_control->addNameSpaceNodeToChain(_node_name_space);

    _node_name_space->setDeclarations();

    parser::ast_control->popNameSpaceChainFromChain();

    parser::ast_control->print("Ast Node Name Space\n");
    parser::ast_control->debug_information_tab--;
    
}

void parser::Ast_Node_Name_Space::generate() {

    parser::ast_control->current_token_position++;

    utils::Linked_List <char*>* _scope = getNameSpaceScopeFromKeyWord();

    Name_Space* _name_space = parser::ast_control->name_space_control->getNameSpaceOrAdd(_scope);

    _scope->destroy_content = 0; delete _scope;

    if (parser::ast_control->getToken(0)->id != OPEN_BRACES) parser::exception_handle->runExceptionAstControl("Expected token '{'");

    parser::ast_control->current_token_position++;

    generate(_name_space);

}

void parser::Ast_Node_Name_Space::setDeclarations() {

    int _node_type;

    while(ast_control->current_token_position < tokenizer_control->tokens_collection->count) { 

        switch (_node_type = getNodeType())
        {
        case -1: parser::ast_control->current_token_position++; goto out; break;
        case AST_NODE_NAME_SPACE: Ast_Node_Name_Space::generate(); break;
        case AST_NODE_VARIABLE_DECLARATION: Ast_Node_Variable_Declaration::generate(); break;
        default: break;
        }

    }

out:;

}


parser::Ast_Node_Variable_Declaration::~Ast_Node_Variable_Declaration() { delete type; }

parser::Ast_Node_Variable_Declaration::Ast_Node_Variable_Declaration(int __declaration_id, parser::Type_Information* __type_information) 
    : Ast_Node(AST_NODE_VARIABLE_DECLARATION, __declaration_id) {}

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Variable_Declaration::generate() {

    utils::Linked_List <parser::Ast_Node*>* _nodes = new utils::Linked_List <parser::Ast_Node*>();
    parser::Ast_Node_Variable_Declaration* _variable_declaration;
    int _declaration_id;

    parser::Type_Information* _type = parser::Type_Information::generate();

    while(parser::ast_control->getToken(0)->id != END_INSTRUCTION) {

        parser::ast_control->debug_information_tab++;
        parser::ast_control->print("Ast Node Variable Declaration\n");

        if (parser::ast_control->getToken(0)->id != IDENTIFIER) exception_handle->runExceptionAstControl("Expected Identifier");

        // Add to declarations

        _variable_declaration = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));
        new (_variable_declaration) Ast_Node_Variable_Declaration(_declaration_id, _type);

        // Add to declarations

        switch (parser::ast_control->getToken(0)->id)
        {
        case FUNCTION_OPERATOR_EQUAL: exception_handle->runException("Equal in variable declaration not implement yet"); break;
        case COMMA: parser::ast_control->current_token_position++; _type = parser::Type_Information::generate(_type); break;
        case END_INSTRUCTION: break;
        default: break;
        }
        
        parser::ast_control->print("Ast Node Variable Declaration\n");
        parser::ast_control->debug_information_tab--;

    }


    return _nodes;

}





