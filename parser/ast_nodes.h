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

        Ast_Node_Code_Block* previous_code_block;
        Declaration_Tracker* declaration_tracker;
        utils::Linked_List <Ast_Node*>* code;
        Name_Space* name_space;
        int declaration_off;

        ~Ast_Node_Code_Block(); Ast_Node_Code_Block(Ast_Node_Code_Block*, Name_Space*);

        static void generate();

        static void setUp();

        void setCode();
        
    };

    struct Ast_Node_Variable_Declaration : Ast_Node {

        parser::Type_Information* type;

        ~Ast_Node_Variable_Declaration(); Ast_Node_Variable_Declaration(int, parser::Type_Information*);

        static utils::Linked_List <parser::Ast_Node*>* generate();

        static utils::Linked_List <parser::Ast_Node*>* generateFunctionParameter();

    };

    struct Ast_Node_Function_Declaration : Ast_Node {

        utils::Linked_List <Ast_Node*>* parameters;
        parser::Type_Information* return_type;
        Ast_Node_Code_Block* body;
        Name_Space* name_space;

        ~Ast_Node_Function_Declaration(); Ast_Node_Function_Declaration(int, utils::Linked_List <Ast_Node*>*, parser::Type_Information*, Ast_Node_Code_Block*, Name_Space*);

        static Ast_Node_Function_Declaration* generate();

        static utils::Linked_List <Ast_Node*>* getParameters();

        int getByteSize();

    };

    struct Ast_Node_Struct_Declaration : Ast_Node {

    };


}

#endif