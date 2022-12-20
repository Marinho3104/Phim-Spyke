#include "ast_nodes.h"

#include "ast_definitions.h"
#include "linked_List.h"
#include "ast.h"

#include <iostream>


parser::Ast_Node::~Ast_Node() {}

parser::Ast_Node::Ast_Node(int __node_type, int __declaration_id) : node_type(__node_type), declaration_id(__declaration_id) {}


parser::Ast_Node_Name_Space::~Ast_Node_Name_Space() {

}

parser::Ast_Node_Name_Space::Ast_Node_Name_Space(Name_Space* __name_space) : Ast_Node(AST_NODE_NAME_SPACE, -1), name_space(__name_space) {}

void parser::Ast_Node_Name_Space::generate(Name_Space* __name_space) {

    parser::ast_control->print("Ast Node Name Space");

    Ast_Node_Name_Space* _node_name_space = (Ast_Node_Name_Space*) malloc(sizeof(Ast_Node_Name_Space));

    new (_node_name_space) Ast_Node_Name_Space(
        __name_space
    );

    parser::ast_control->name_space_node_collection->add(_node_name_space);
    parser::ast_control->addNameSpaceNodeToChain(_node_name_space);

    _node_name_space->setDeclarations();

    parser::ast_control->popNameSpaceChainFromChain();

    parser::ast_control->print("Ast Node Name Space");
}

void parser::Ast_Node_Name_Space::generate() {

}

void parser::Ast_Node_Name_Space::setDeclarations() {



}





