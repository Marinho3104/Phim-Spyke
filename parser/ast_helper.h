#pragma once

#ifndef PARSER_AST_HELPER_H
#define PARSER_AST_HELPER_H

#include "parser_definitions.h"

namespace parser {

    struct Type_Information {

        parser::Ast_Node_Struct_Declaration* declaration;
        int pointer_level, reference_level;

        ~Type_Information(); Type_Information(parser::Ast_Node_Struct_Declaration*, utils::Linked_List <int>*);

        bool operator==(Type_Information*); bool operator!=(Type_Information*);

        static Type_Information* generate(Type_Information*);

        static Type_Information* generate(int);

        static Type_Information* generate();

        static Type_Information* generatePrimitiveType(int);

        Type_Information* getCopy();

        bool isPointerStruct();

        bool isSpykeStruct();

        bool haveConstructorDefined();

        int getSize();

    };

    struct Expression_Result_Helper {

        Ast_Node_Variable_Declaration* declaration;
        Ast_Node_Expression* expression;
        bool function_result_value;
        int token_id;

        ~Expression_Result_Helper(); Expression_Result_Helper(Ast_Node_Expression*);

    };
    
    int getNodeType();


    utils::Linked_List <char*>* getScope();

    utils::Linked_List <char*>* getNameSpaceScope();

    parser::Name_Space* getNameSpace();


    utils::Linked_List <int>* getPointerOperations();  

    void ignoreCodeBlock();


    Name_Space* getCurrentNameSpace();

    utils::Linked_List <char*>* getCurrentScope();

    parser::Declarations_Tracker* getCurrentDeclarationTracker();


    int getDeclarationId(char*);

    parser::Ast_Node_Variable_Declaration* getVariableDeclaration(int);

    parser::Ast_Node_Function_Declaration* getFunctionDeclaration(int, utils::Linked_List <Ast_Node*>*);
    
    parser::Ast_Node_Struct_Declaration* getStructDeclaration(int);


    bool isNameSpaceScope();

    bool isGlobalDeclaration();


    int getOperatorPriority(int);


    template <typename type>
    utils::Linked_List <type*>* getSpecificNodesFromLinkedList(utils::Linked_List <Ast_Node*>*, int);

}

#endif