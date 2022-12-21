#include "ast_helper.h"

#include "built_ins_definitions.h"
#include "token_definitions.h"
#include "exception_handle.h"
#include "tokenizer_helper.h"
#include "ast_definitions.h"
#include "linked_List.h"
#include "ast_nodes.h"
#include "token.h"
#include "ast.h"

#include <iostream>


parser::Type_Information::~Type_Information() {} 

parser::Type_Information::Type_Information(
    parser::Ast_Node_Struct_Declaration* __user_defined_declaration, int __token_id, parser::Name_Space* __name_space, utils::Linked_List <int>* __pointer_operations) 
        : user_defined_declaration(__user_defined_declaration), token_id(__token_id), name_space(__name_space), pointer_level(0), reference_level(0) {

            if (!__pointer_operations) return;

            for (int _ = 0; _ < __pointer_operations->count; _++)

                if (__pointer_operations->operator[](_) == POINTER) {

                    if (reference_level) exception_handle->runExceptionAstControl("Can make a pointer of reference");

                    ++pointer_level;

                }

                else 

                    if (++reference_level) exception_handle->runExceptionAstControl("Can make a reference of a double reference");

}

bool parser::Type_Information::operator==(Type_Information* __to_compare) {

    return (
        user_defined_declaration == __to_compare->user_defined_declaration &&
        pointer_level == __to_compare->pointer_level && 
        reference_level == __to_compare->reference_level && 
        token_id == __to_compare->token_id && 
        name_space == __to_compare->name_space
    );

} 

bool parser::Type_Information::operator!=(Type_Information* __to_compare) { return !operator==(__to_compare); }

parser::Type_Information* parser::Type_Information::getCopy() {

    parser::Type_Information* _copy = new parser::Type_Information(
        user_defined_declaration, token_id, name_space, NULL
    );

    _copy->pointer_level = pointer_level;
    _copy->reference_level = reference_level;

    return _copy;

}

parser::Type_Information* parser::Type_Information::generate() {

    parser::Name_Space* _name_space = getNameSpace();

    if (_name_space) { parser::ast_control->addNameSpaceNodeToChain(_name_space); parser::ast_control->addCodeBlockNodeToChain(NULL); }

    parser::Ast_Node_Struct_Declaration* _user_defined_declaration = NULL;

    int _id = parser::ast_control->getToken(0)->id;
    parser::ast_control->current_token_position++;

    if (isPrimitive(_id));
    else if (_id == IDENTIFIER) { parser::exception_handle->runException("Not implemented yet"); } // Need to change Multiplication and bitwise and
    else parser::exception_handle->runExceptionAstControl("Unexpected token");

    utils::Linked_List <int>* _pointer_operations = getPointerOperations();

    parser::Type_Information* _type_information = new parser::Type_Information(
        _user_defined_declaration, _id, _name_space, _pointer_operations
    );

    delete _pointer_operations;

    return _type_information;

}

parser::Type_Information* parser::Type_Information::generate(Type_Information* __type_information) {

    utils::Linked_List <int>* _pointer_operations = getPointerOperations();

    Type_Information* _type_information = new Type_Information(
        __type_information->user_defined_declaration, __type_information->token_id, __type_information->name_space, _pointer_operations 
    );

    delete _pointer_operations;

    return _type_information;

}

int parser::Type_Information::getByteSize() {

    if (!pointer_level) return token_id == IDENTIFIER ? -1 : getPrimitveTypeSize(token_id);

    return PRIMITIVE_TYPE_POINTER_SIZE;

}

int parser::getNodeType() {

    int _backup_state;

    switch (parser::ast_control->getToken(0)->id)
    {
    case CLOSE_BRACES: return -1; break;
    case NAMESPACE: return AST_NODE_NAME_SPACE; break;
    case STATIC: _backup_state = parser::ast_control->current_token_position; parser::ast_control->current_token_position++; break;
    default: _backup_state = parser::ast_control->current_token_position; break;
    }

    
    if (isPrimitive(parser::ast_control->getToken(0)->id)) {

        Type_Information* _type = Type_Information::generate();
        int _node_type;

        getNameSpace();

        switch (parser::ast_control->getToken(1)->id)
        {
        case FUNCTION_OPERATOR_EQUAL: case COMMA: case END_INSTRUCTION: _node_type = AST_NODE_VARIABLE_DECLARATION; break;
        // case OPEN_PARENTHESES: _node_type = AST_NODE_FUNCTION_DECLARATION; break;
        default: exception_handle->runExceptionAstControl("Unexpected token"); break;
        }

        delete _type;

        parser::ast_control->current_token_position = _backup_state;

        return _node_type;

    }

    Name_Space* _name_space = getNameSpace();

    exception_handle->runExceptionAstControl("Error getting node type");

    return -1;

}

utils::Linked_List <int>* parser::getPointerOperations() {

    utils::Linked_List <int>* __pointer_operations = new utils::Linked_List <int>();

    while(parser::ast_control->getToken(0)->id == POINTER || parser::ast_control->getToken(0)->id == ADDRESS)

        __pointer_operations->add(parser::ast_control->getToken(0)->id);

    return __pointer_operations;

}

utils::Linked_List <char*>* parser::getNameSpaceScopeFromKeyWord() {

    if (parser::ast_control->getToken(0)->id != IDENTIFIER) exception_handle->runExceptionAstControl("Expected token identifier");

    utils::Linked_List <char*>* _scope = getScope();

    _scope->join(
        parser::ast_control->name_space_chain->last->object->name_space->scope,
        0
    );

    return _scope;

}

utils::Linked_List <char*>* parser::getNameSpaceScope() {

    if (parser::ast_control->getToken(0)->id != DOUBLE_COLON && parser::ast_control->getToken(1)->id != DOUBLE_COLON) return NULL;
    if (parser::ast_control->getToken(0)->id != IDENTIFIER && parser::ast_control->getToken(1)->id != IDENTIFIER) return NULL;

    bool _is_global = parser::ast_control->getToken(0)->id == DOUBLE_COLON;

    utils::Linked_List <char*>* _scope = getScope();

    if (!_is_global) _scope->join(parser::ast_control->name_space_chain->last->object->name_space->scope, 0);

    return _scope;

}

utils::Linked_List <char*>* parser::getScope() {

    utils::Linked_List <char*>* _scope = new utils::Linked_List <char*>();

    int _expected = parser::ast_control->getToken(0)->id;

    while(parser::ast_control->getToken(0)->id == _expected) {

        if (_expected == IDENTIFIER) {

            _scope->add(
                parser::ast_control->getToken(0)->identifier
            );

            _expected = DOUBLE_COLON;

        } else _expected = IDENTIFIER;

        parser::ast_control->current_token_position++;

    }

    if (_expected == IDENTIFIER) exception_handle->runExceptionAstControl("Expected token identifier");

    return _scope;

}

parser::Name_Space* parser::getNameSpace() {

    utils::Linked_List <char*>* _name_space_scope = getNameSpaceScope();

    if (!_name_space_scope) return NULL;

    utils::Data_Linked_List <char*>* _data_linked_list = _name_space_scope->remove(_name_space_scope->count);

    _data_linked_list->destroy_content = 0; delete _data_linked_list;

    parser::ast_control->current_token_position--;

    parser::Name_Space* _name_space = parser::ast_control->name_space_control->getNameSpace(_name_space_scope);

    if (!_name_space) exception_handle->runExceptionAstControl("Undefined Name space");

    return _name_space;

}

parser::Name_Space* parser::getCurrentNameSpace() {

    if (parser::ast_control->code_block_chain->last && parser::ast_control->code_block_chain->last->object)

        return NULL; // parser::ast_control->code_block_chain->last->object->

    return parser::ast_control->name_space_chain->last->object->name_space;

}

int parser::getPrimitveTypeSize(int __primitive_type_id) {

    switch (__primitive_type_id)
    {
    case PRIMITIVE_TYPE_INT: return PRIMITIVE_TYPE_INT_SIZE; break;
    case PRIMITIVE_TYPE_BYTE: return PRIMITIVE_TYPE_BYTE_SIZE; break;
    case PRIMITIVE_TYPE_BOOL: return PRIMITIVE_TYPE_BOOL_SIZE; break;
    default: break;
    }

    exception_handle->runException("Unexpected primitve type");

    return -1;

}
