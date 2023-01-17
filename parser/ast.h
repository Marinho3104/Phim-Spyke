#pragma once

#ifndef PARSER_AST_H
#define PARSER_AST_H

#include "parser_definitions.h"

namespace parser {

    struct Declarations_Tracker {

        utils::Linked_List <parser::Ast_Node_Variable_Declaration*>* variable_declaration;
        utils::Linked_List <parser::Ast_Node_Function_Declaration*>* function_declaration;
        utils::Linked_List <parser::Ast_Node_Struct_Declaration*>* struct_declaration;

        utils::Linked_List <int>* declaration_ids;
        utils::Linked_List <char*>* names;

        int* off;

        ~Declarations_Tracker(); Declarations_Tracker(int*);

        Ast_Node_Variable_Declaration* getVariableDeclaration(int);

        Ast_Node_Function_Declaration* getFunctionDeclaration(int, utils::Linked_List <Ast_Node*>*);
        
        Ast_Node_Struct_Declaration* getStructDeclaration(int);

        int getDeclarationId(char*);

        void addName(char*, int);

        void addName(char*);

    };

    struct Name_Space {

        Declarations_Tracker* declarations_tracker;
        utils::Linked_List <Ast_Node_Name_Space*>* name_space_nodes;
        utils::Linked_List <char*>* scope;

        ~Name_Space(); 
        
        Name_Space(utils::Linked_List <char*>*, int*); Name_Space(utils::Linked_List <char*>*, parser::Ast_Node_Name_Space*, int*);

    };

    struct Name_Space_Control {

        utils::Linked_List <Name_Space*>* name_space_collection;
        int off;

        ~Name_Space_Control(); Name_Space_Control();

        void setGlobalNameSpace();

        void addNameSpace(utils::Linked_List <char*>*);

        Name_Space* getNameSpace(utils::Linked_List <char*>*);

        Name_Space* getNameSpace(Ast_Node_Name_Space*);

        Name_Space* getNameSpaceOrAdd(utils::Linked_List <char*>*);

        Name_Space* getPreviousNameSpace(Name_Space*);
        
    };

    struct Ast_Control {

        utils::Linked_List <char*>* implicit_values_collection;
        parser::Ast_Node_Name_Space* global_name_space;

        Name_Space_Control* name_space_control;

        utils::Linked_List <Name_Space*>* name_space_chain;
        utils::Linked_List <Ast_Node_Code_Block*>* code_block_chain;

        utils::Linked_List <Ast_Node*>* current_nodes_list;

        int current_position;

        int debug_mode_tab;
        bool debug_mode;

        ~Ast_Control(); Ast_Control(bool);

        Token* getToken(int);

        void print(const char*, int);

        void generate();

        int addImplicitValue(char*);

        void addToChain(Name_Space*, Ast_Node_Code_Block*); void popFromChain();

    };

}

#endif