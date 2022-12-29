#pragma once

#ifndef PARSER_AST_NODES_H
#define PARSER_AST_NODES_H

#include "parser_definitions.h"

namespace parser {

    struct Ast_Node {

        Ast_Node_Variable_Declaration* representive_declaration;
        int node_type;

        virtual ~Ast_Node(); Ast_Node(Ast_Node_Variable_Declaration*, int);

    };

    struct Ast_Node_Name_Space : Ast_Node {

        utils::Linked_List <Ast_Node*>* declarations;
        bool struct_name_space;

        ~Ast_Node_Name_Space(); Ast_Node_Name_Space(); Ast_Node_Name_Space(Ast_Node_Variable_Declaration*);

        static Ast_Node_Name_Space* generate(Name_Space*);
        
        static Ast_Node_Name_Space* generate();

        void setDeclarations();

        int getDeclarationId(char*);

        parser::Ast_Node_Variable_Declaration* getVariableDeclaration(int);

        parser::Ast_Node_Function_Declaration* getFunctionDeclaration(int, utils::Linked_List <Ast_Node*>*);
        
        parser::Ast_Node_Struct_Declaration* getStructDeclaration(int);

    };

    struct Ast_Node_Code_Block : Ast_Node {

        Ast_Node_Code_Block* previous_node_code_block;
        Declarations_Tracker* declarations_tracker;
        utils::Linked_List <Ast_Node*>* code;
        Name_Space* name_space;
        int off;

        ~Ast_Node_Code_Block(); Ast_Node_Code_Block();

        static void generate();

        static void setUp();

        void setCode();

        int getDeclarationId(char*);

        parser::Ast_Node_Variable_Declaration* getVariableDeclaration(int);

        parser::Ast_Node_Function_Declaration* getFunctionDeclaration(int, utils::Linked_List <Ast_Node*>*);
        
        parser::Ast_Node_Struct_Declaration* getStructDeclaration(int);
        
    };

    struct Ast_Node_Variable_Declaration : Ast_Node {

        parser::Type_Information* type;
        int declaration_id;
        bool global;
        
        ~Ast_Node_Variable_Declaration(); Ast_Node_Variable_Declaration(parser::Type_Information*, int, bool);

        static utils::Linked_List <Ast_Node*>* generateFunctionParameter();

        static utils::Linked_List <Ast_Node*>* generate();

    };

    struct Ast_Node_Function_Declaration : Ast_Node {

        utils::Linked_List <Ast_Node*>* parameters;
        Ast_Node_Code_Block* body;
        Name_Space* name_space;
        int declaration_id;

        ~Ast_Node_Function_Declaration(); Ast_Node_Function_Declaration(parser::Ast_Node_Variable_Declaration*, utils::Linked_List <Ast_Node*>*, Ast_Node_Code_Block*, Name_Space*, int);
        
        static Ast_Node_Function_Declaration* generate(bool);

        static utils::Linked_List <Ast_Node*>* getParameters(bool);

    };

    struct Ast_Node_Struct_Declaration : Ast_Node {

        Ast_Node_Name_Space* functions;
        Ast_Node_Code_Block* fields;
        int declaration_id;

        ~Ast_Node_Struct_Declaration(); Ast_Node_Struct_Declaration(Ast_Node_Name_Space*, Ast_Node_Code_Block*, int);

        static Ast_Node_Struct_Declaration* generate();

        void set();

        void setFunctions();

        void setFields();

    };

    struct Ast_Node_Expression : Ast_Node {

        Ast_Node_Expression* expression;
        bool destroy_value;
        Ast_Node* value;
        int token_id;

        ~Ast_Node_Expression(); Ast_Node_Expression(Ast_Node_Expression*, Ast_Node*, int);

        Ast_Node_Variable_Declaration* getResultDeclaration();

        static Ast_Node_Expression* generate(int);

        static Ast_Node* getValue(int);

    };

    struct Ast_Node_Variable : Ast_Node {

        int declaration_id;

        ~Ast_Node_Variable(); Ast_Node_Variable(Ast_Node_Variable_Declaration*, int);

        static Ast_Node_Variable* generate();

    };

    struct Ast_Node_Value : Ast_Node {

        int implicit_value_position;

        ~Ast_Node_Value(); Ast_Node_Value(Ast_Node_Variable_Declaration*, int);

        static Ast_Node_Value* generate();

    };

    struct Ast_Node_Pointer_Operation : Ast_Node {

        bool destroy_value;
        int pointer_level;
        Ast_Node* value;

        ~Ast_Node_Pointer_Operation(); Ast_Node_Pointer_Operation(Ast_Node_Variable_Declaration*, int, Ast_Node*);

        static Ast_Node_Pointer_Operation* generate();

    };

    struct Ast_Node_Parenthesis : Ast_Node {

        Ast_Node_Expression* expression;

        ~Ast_Node_Parenthesis(); Ast_Node_Parenthesis(Ast_Node_Expression*);

        static Ast_Node_Parenthesis* generate();

    };

    struct Ast_Node_Function_Call : Ast_Node {

        utils::Linked_List <Ast_Node_Expression*>* parameters;
        Ast_Node_Function_Declaration* function_declaration; 

        ~Ast_Node_Function_Call(); Ast_Node_Function_Call(utils::Linked_List <Ast_Node_Expression*>*, Ast_Node_Function_Declaration*);

        static Ast_Node_Function_Call* generate(Ast_Node_Expression*, Name_Space*);

        static utils::Linked_List <Ast_Node_Expression*>* getParameters(Ast_Node_Expression*);

        static utils::Linked_List <Ast_Node*>* getResultParameters(utils::Linked_List <Ast_Node_Expression*>*);

    };

    struct Ast_Node_Accessing : Ast_Node {

        Ast_Node* value, *accessing;
        Ast_Node_Accessing* next;

        ~Ast_Node_Accessing(); Ast_Node_Accessing(Ast_Node*, Ast_Node*, Ast_Node_Accessing*);

        static Ast_Node_Accessing* generate(Ast_Node*);

    };

}

#endif