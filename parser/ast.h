#pragma once

#ifndef PARSER_AST_H
#define PARSER_AST_H

#include "parser_definitions.h"

namespace parser {

    struct Declaration_Tracker {

        utils::Linked_List <Ast_Node_Variable_Declaration*>* variable_declarations;
        utils::Linked_List <Ast_Node_Function_Declaration*>* function_declarations;
        utils::Linked_List <Ast_Node_Struct_Declaration*>* struct_declarations;

        utils::Linked_List <int>* declaration_ids;
        utils::Linked_List <char*>* names;

        int* off;

        ~Declaration_Tracker(); Declaration_Tracker(int*);

        Ast_Node_Variable_Declaration* getVariableDeclaration(int);

        Ast_Node_Function_Declaration* getFunctionDeclaration(int, utils::Linked_List <Ast_Node*>*);
        
        Ast_Node_Struct_Declaration* getStructDeclaration(int);

        int getDeclarationId(char*);

        void addName(char*);

        void addStaticName(char*, int);

    };

    struct Name_Space {

        Declaration_Tracker* declaration_tracker;
        utils::Linked_List <char*>* scope;

        ~Name_Space(); Name_Space(utils::Linked_List <char*>*, int*);

    };

    struct Name_Space_Control {

        utils::Linked_List <Name_Space*>* name_space_collection;
        int declaration_off;

        ~Name_Space_Control(); Name_Space_Control();

        void loadGlobalNameSpace();

        void addNameSpace(Name_Space*);

        void addNameSpace(utils::Linked_List <char*>*);

        Name_Space* getNameSpace(utils::Linked_List <char*>*);

        Name_Space* getNameSpaceOrAdd(utils::Linked_List <char*>*);

        Name_Space* getPreviousNameSpace(Name_Space*);

    };

    struct Ast_Control {

        utils::Linked_List <Ast_Node_Name_Space*>* name_space_node_collection;
        utils::Linked_List <char*>* implicit_value_collection;
        Name_Space_Control* name_space_control;

        utils::Linked_List <Ast_Node_Name_Space*>* name_space_chain;
        utils::Linked_List <Ast_Node_Code_Block*>* code_block_chain;

        int current_token_position;

        int debug_information_tab;
        bool debug_mode;

        ~Ast_Control(); Ast_Control(bool);

        Token* getToken(int);

        void print(const char*);

        void generate();

        void addImplicitValue(char*);

        void addNameSpaceNodeToChain(Ast_Node_Name_Space*); void addNameSpaceNodeToChain(Name_Space*); void popNameSpaceChainFromChain();

        void addCodeBlockNodeToChain(Ast_Node_Code_Block*); void popCodeBlockChainFromChain();

        Ast_Node_Name_Space* getNodeNameSpace(Name_Space*);

    };

}

#endif