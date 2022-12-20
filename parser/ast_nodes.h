#pragma once

#ifndef PARSER_AST_NODES_H
#define PARSER_AST_NODES_H

#include "parser_definitions.h"

namespace parser {

    struct Ast_Node {
        int node_type, declaration_id;
        virtual ~Ast_Node(); Ast_Node(int, int);
    };

    struct Ast_Node_Name_Space : Ast_Node {

        utils::Linked_List <Ast_Node*>* declarations;
        Name_Space* name_space;

        ~Ast_Node_Name_Space(); Ast_Node_Name_Space(Name_Space*);

        static void generate(Name_Space*);

        static void generate();

        void setDeclarations();

    };

    struct Ast_Node_Code_Block : Ast_Node {
        
    };

    struct Ast_Node_Variable_Declaration : Ast_Node {

        parser::Type_Information* type;

        ~Ast_Node_Variable_Declaration(); Ast_Node_Variable_Declaration(int, parser::Type_Information*);

        static utils::Linked_List <parser::Ast_Node*>* generate();

    };

    struct Ast_Node_Function_Declaration : Ast_Node {

    };

    struct Ast_Node_Struct_Declaration : Ast_Node {

    };


}

#endif