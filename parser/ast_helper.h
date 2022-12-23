#pragma once

#ifndef PARSER_AST_HELPER_H
#define PARSER_AST_HELPER_H

#include "parser_definitions.h"

namespace parser {

    struct Type_Information {

        parser::Ast_Node_Struct_Declaration* user_defined_declaration;
        int pointer_level, reference_level, token_id;
        parser::Name_Space* name_space;

        ~Type_Information(); Type_Information(parser::Ast_Node_Struct_Declaration*, int, parser::Name_Space*, utils::Linked_List <int>*);

        bool operator==(Type_Information*); bool operator!=(Type_Information*);

        Type_Information* getCopy();

        static Type_Information* generate();

        static Type_Information* generate(Type_Information*);

        int getByteSize();

    };


    int getNodeType();

    utils::Linked_List <int>* getPointerOperations();

    utils::Linked_List <char*>* getNameSpaceScopeFromKeyWord();

    utils::Linked_List <char*>* getNameSpaceScope();

    utils::Linked_List <char*>* getScope();

    parser::Name_Space* getNameSpace();

    parser::Declaration_Tracker* getDeclarationTracker();

    int getPrimitveTypeSize(int);

    void ignoreCodeBlock();

    template <typename type>
    utils::Linked_List <type*>* getSpecificNodesFromLinkedList(utils::Linked_List <Ast_Node*>*, int);

}

#endif