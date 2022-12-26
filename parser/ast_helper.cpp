#include "ast_helper.h"

#include "built_ins_definitions.h"
#include "token_definitions.h"
#include "built_ins_helper.h"
#include "exception_handle.h"
#include "tokenizer_helper.h"
#include "ast_definitions.h"
#include "linked_List.h"
#include "ast_nodes.h"
#include "token.h"
#include "ast.h"

#include <iostream>
#include <string.h> 


parser::Type_Information::~Type_Information() { if (declaration) delete declaration; } 

parser::Type_Information::Type_Information(
    parser::Ast_Node_Struct_Declaration* __user_defined_declaration, int __token_id, parser::Name_Space* __name_space, utils::Linked_List <int>* __pointer_operations) 
        : user_defined_declaration(__user_defined_declaration), token_id(__token_id), name_space(__name_space), pointer_level(0), reference_level(0), declaration(NULL) {

            if (!__pointer_operations) return;

            for (int _ = 0; _ < __pointer_operations->count; _++)

                if (__pointer_operations->operator[](_) == POINTER) {

                    if (reference_level) exception_handle->runExceptionAstControl("Can make a pointer of reference");

                    ++pointer_level;

                }

                else 

                    if (++reference_level) exception_handle->runExceptionAstControl("Can make a reference of a double reference");

}

parser::Type_Information::Type_Information(int __implicit_value_id)
    : user_defined_declaration(0), token_id(getPrimitiveTypeOfImplicitValue(__implicit_value_id)), name_space(0), pointer_level(0), reference_level(0), declaration(NULL) {

        utils::Linked_List <char*>* _built_ins_scope = new utils::Linked_List <char*>();
        _built_ins_scope->destroy_content = 0;
        _built_ins_scope->add(
            (char*) "built_ins"
        );

        Name_Space* _name_space = parser::ast_control->name_space_control->getNameSpace(_built_ins_scope);

        if (_name_space) { parser::ast_control->addNameSpaceNodeToChain(_name_space); parser::ast_control->addCodeBlockNodeToChain(NULL); }

        delete _built_ins_scope;

        char* _struct_name = getPrimitiveTypeStructName(token_id);

        int _declaration_id = parser::getDeclarationId(_struct_name);

        user_defined_declaration = parser::getStructDeclaration(_declaration_id);

        if (_declaration_id == -1 || !user_defined_declaration) {

            parser::ast_control->current_token_position--;
        
            if (_name_space) { parser::ast_control->popNameSpaceChainFromChain(); parser::ast_control->popCodeBlockChainFromChain(); }

            exception_handle->runExceptionAstControl("Undefined type");

        }

        if (parser::ast_control->getToken(0)->id == FUNCTION_OPERATOR_MULTIPLICATION || parser::ast_control->getToken(0)->id == FUNCTION_OPERATOR_BITWISE_AND)

            parser::ast_control->getToken(0)->id = parser::ast_control->getToken(0)->id == FUNCTION_OPERATOR_MULTIPLICATION ? POINTER : ADDRESS;

        free(_struct_name);

        if (_name_space) { parser::ast_control->popNameSpaceChainFromChain(); parser::ast_control->popCodeBlockChainFromChain(); }

}

bool parser::Type_Information::operator==(Type_Information* __to_compare) {

    // std::cout << "-------- Compare -------" << std::endl;
    // std::cout <<(user_defined_declaration == __to_compare->user_defined_declaration)  << std::endl;
    // std::cout <<(pointer_level == __to_compare->pointer_level)  << std::endl;
    // std::cout <<(reference_level == __to_compare->reference_level)  << std::endl;
    // std::cout <<(token_id == __to_compare->token_id)  << std::endl;
    // std::cout <<(name_space == __to_compare->name_space)  << std::endl;
    // std::cout << "--------       -------" << std::endl;

    return (
        user_defined_declaration == __to_compare->user_defined_declaration &&
        pointer_level == __to_compare->pointer_level && 
        reference_level == __to_compare->reference_level && 
        // token_id == __to_compare->token_id &&
        name_space == __to_compare->name_space
    );

} 

bool parser::Type_Information::operator!=(Type_Information* __to_compare) { return !operator==(__to_compare); }

void parser::Type_Information::setDeclarationVariable() {

    declaration  = new Ast_Node_Variable_Declaration(
        -1, this->getCopy(), 0
    );

}

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

    int _id = parser::ast_control->getToken(0)->id, _backup_state = parser::ast_control->current_token_position;
    parser::ast_control->current_token_position++;

    if (isPrimitive(_id) && !_name_space) {

        utils::Linked_List <char*>* _built_ins_scope = new utils::Linked_List <char*>();
        _built_ins_scope->destroy_content = 0;
        _built_ins_scope->add(
            (char*) "built_ins"
        );

        _name_space = parser::ast_control->name_space_control->getNameSpace(_built_ins_scope);

        if (_name_space) { parser::ast_control->addNameSpaceNodeToChain(_name_space); parser::ast_control->addCodeBlockNodeToChain(NULL); }

        delete _built_ins_scope;

        char* _struct_name = getPrimitiveTypeStructName(_id);

        int _declaration_id = parser::getDeclarationId(_struct_name);

        _user_defined_declaration = parser::getStructDeclaration(_declaration_id);

        if (_declaration_id == -1 || !_user_defined_declaration) {

            parser::ast_control->current_token_position--;
        
            if (_name_space) { parser::ast_control->popNameSpaceChainFromChain(); parser::ast_control->popCodeBlockChainFromChain(); }

            exception_handle->runExceptionAstControl("Undefined type");

        }

        if (parser::ast_control->getToken(0)->id == FUNCTION_OPERATOR_MULTIPLICATION || parser::ast_control->getToken(0)->id == FUNCTION_OPERATOR_BITWISE_AND)

            parser::ast_control->getToken(0)->id = parser::ast_control->getToken(0)->id == FUNCTION_OPERATOR_MULTIPLICATION ? POINTER : ADDRESS;

        free(_struct_name);

    }
    else if (_id == IDENTIFIER) {

        if (built_ins::isPrimitiveTypeName( (const char*) parser::ast_control->getToken(-1)->identifier) && !_name_space) {

            utils::Linked_List <char*>* _built_ins_scope = new utils::Linked_List <char*>();
            _built_ins_scope->destroy_content = 0;
            _built_ins_scope->add(
                (char*) "built_ins"
            );

            _name_space = parser::ast_control->name_space_control->getNameSpace(_built_ins_scope);

            if (_name_space) { parser::ast_control->addNameSpaceNodeToChain(_name_space); parser::ast_control->addCodeBlockNodeToChain(NULL); }

            delete _built_ins_scope;

        }

        int _declaration_id = parser::getDeclarationId(parser::ast_control->getToken(-1)->identifier);

        _user_defined_declaration = parser::getStructDeclaration(_declaration_id);

        if (_declaration_id == -1 || !_user_defined_declaration) {

            parser::ast_control->current_token_position--;
        
            if (_name_space) { parser::ast_control->popNameSpaceChainFromChain(); parser::ast_control->popCodeBlockChainFromChain(); }

            exception_handle->runExceptionAstControl("Undefined type");

        }

        if (parser::ast_control->getToken(0)->id == FUNCTION_OPERATOR_MULTIPLICATION || parser::ast_control->getToken(0)->id == FUNCTION_OPERATOR_BITWISE_AND)

            parser::ast_control->getToken(0)->id = parser::ast_control->getToken(0)->id == FUNCTION_OPERATOR_MULTIPLICATION ? POINTER : ADDRESS;

    }
    else parser::exception_handle->runExceptionAstControl("Unexpected token");

    utils::Linked_List <int>* _pointer_operations = getPointerOperations();

    parser::Type_Information* _type_information = new parser::Type_Information(
        _user_defined_declaration, _id, _name_space, _pointer_operations
    );

    delete _pointer_operations;

    if (_name_space) { parser::ast_control->popNameSpaceChainFromChain(); parser::ast_control->popCodeBlockChainFromChain(); }

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


parser::Representation::Representation(parser::Ast_Node_Variable_Declaration* __declaration, int __priority_level, int __token_id) 
            : declaration(__declaration), priority_level(__priority_level), token_id(__token_id) {}

int parser::getNodeType() {

    int _backup_state;

    // std::cout << "Id -> "  << (int) parser::ast_control->getToken(0)->id << std::endl;

    switch (parser::ast_control->getToken(0)->id)
    {
    case CLOSE_BRACES: return -1; break;
    case NAMESPACE: return AST_NODE_NAME_SPACE; break;
    case STRUCT: return AST_NODE_STRUCT_DECLARATION; break;
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
        case OPEN_PARENTHESIS: _node_type = AST_NODE_FUNCTION_DECLARATION; break;
        default: exception_handle->runExceptionAstControl("Unexpected token here"); break;
        }

        delete _type;

        parser::ast_control->current_token_position = _backup_state;

        return _node_type;

    }

    if (parser::ast_control->getToken(0)->id == IDENTIFIER || parser::ast_control->getToken(0)->id == DOUBLE_COLON) {

        int _node_type;

        try { delete Type_Information::generate(); }
        catch(...) { _node_type = (parser::ast_control->getToken(1)->id == OPEN_PARENTHESIS) ? AST_NODE_FUNCTION_CALL : AST_NODE_VARIABLE; goto reset_return; }

        getNameSpace();

        switch (parser::ast_control->getToken(1)->id)
        {
        case FUNCTION_OPERATOR_EQUAL: case COMMA: case END_INSTRUCTION: _node_type = AST_NODE_VARIABLE_DECLARATION; break;
        case OPEN_PARENTHESIS: _node_type = AST_NODE_FUNCTION_DECLARATION; break;
        default: exception_handle->runExceptionAstControl("Unexpected token hehre"); break;
        }

    reset_return:

        parser::ast_control->current_token_position = _backup_state;

        return _node_type;

    }

    if (isFunctionOperator(parser::ast_control->getToken(0)->id)) return AST_NODE_EXPRESSION;

    if (isImplicitValueOrIdentifier(parser::ast_control->getToken(0)->id)) return AST_NODE_VALUE;

    exception_handle->runExceptionAstControl("Error getting node type");

    return -1;

}

char* parser::getPrimitiveTypeStructName(int __primitive_type) {

    char* _;

    switch (__primitive_type)
    {
    case PRIMITIVE_TYPE_BYTE:
        
        _ = (char*) malloc(5);

        strcpy(_, "Byte");

        return _;

        break;

    case PRIMITIVE_TYPE_INT:
        
        _ = (char*) malloc(4);

        strcpy(_, "Int");

        return _;

        break;   

    default: break;
    }

    exception_handle->runException("Error getting primitive type");

    return 0;

}

utils::Linked_List <int>* parser::getPointerOperations() {

    utils::Linked_List <int>* __pointer_operations = new utils::Linked_List <int>();

    while(parser::ast_control->getToken(0)->id == POINTER || parser::ast_control->getToken(0)->id == ADDRESS) {

        __pointer_operations->add(parser::ast_control->getToken(0)->id);

        parser::ast_control->current_token_position++;

    }

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

    if ((parser::ast_control->getToken(0)->id != DOUBLE_COLON || parser::ast_control->getToken(1)->id != IDENTIFIER) &&
        (parser::ast_control->getToken(0)->id != IDENTIFIER || parser::ast_control->getToken(1)->id != DOUBLE_COLON)) return NULL;

    // if (parser::ast_control->getToken(0)->id != DOUBLE_COLON && parser::ast_control->getToken(1)->id != DOUBLE_COLON) return NULL;
    // if (parser::ast_control->getToken(0)->id != IDENTIFIER && parser::ast_control->getToken(1)->id != IDENTIFIER) return NULL;

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
    
    _name_space_scope->destroy_content = 0;

    utils::Data_Linked_List <char*>* _data_linked_list = _name_space_scope->remove(_name_space_scope->count);

    _data_linked_list->destroy_content = 0; delete _data_linked_list;

    parser::ast_control->current_token_position--;

    parser::Name_Space* _name_space = parser::ast_control->name_space_control->getNameSpace(_name_space_scope);

    if (!_name_space) exception_handle->runExceptionAstControl("Undefined Name space");

    delete _name_space_scope;

    return _name_space;

}

parser::Declaration_Tracker* parser::getDeclarationTracker() {

    if (parser::ast_control->code_block_chain->last && parser::ast_control->code_block_chain->last->object)

        return parser::ast_control->code_block_chain->last->object->declaration_tracker;

    return parser::ast_control->name_space_chain->last->object->name_space->declaration_tracker;

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

int parser::getPrimitiveTypeOfImplicitValue(int __implicit_value_type_id) {

    switch (__implicit_value_type_id)
    {
    case IMPLICIT_VALUE_INT: return PRIMITIVE_TYPE_INT; break;
    // case IMPLICIT_VALUE_CHARACTER: return PRIMIk; break;
    // case IMPLICIT_VALUE_STRING: return PRIMITVE_; break;
    default: break;
    }

    exception_handle->runException("Error getting primitive type of implicit value");

    return -1;

}


void parser::ignoreCodeBlock() {

    while(ast_control->getToken(0)->id != CLOSE_BRACES) {

        ast_control->current_token_position++;
    
        if (ast_control->getToken(0)->id == OPEN_BRACES) ignoreCodeBlock();

    }

    ast_control->current_token_position++;

}

int parser::getDeclarationId(char* __name) {

    if (parser::ast_control->code_block_chain->last && parser::ast_control->code_block_chain->last->object)

        return parser::ast_control->code_block_chain->last->object->getDeclarationId(__name);

    return parser::ast_control->name_space_chain->last->object->getDeclarationId(__name);

}

parser::Ast_Node_Variable_Declaration* parser::getVariableDeclaration(int __declaration_id) {

    if (parser::ast_control->code_block_chain->last && parser::ast_control->code_block_chain->last->object)

        return parser::ast_control->code_block_chain->last->object->getVariableDeclaration(__declaration_id);

    return parser::ast_control->name_space_chain->last->object->getVariableDeclaration(__declaration_id);

}

parser::Ast_Node_Function_Declaration* parser::getFunctionDeclaration(int __declaration_id, utils::Linked_List <Ast_Node*>* __parameters) {

    if (parser::ast_control->code_block_chain->last && parser::ast_control->code_block_chain->last->object)

        return parser::ast_control->code_block_chain->last->object->getFunctionDeclaration(__declaration_id, __parameters);

    return parser::ast_control->name_space_chain->last->object->getFunctionDeclaration(__declaration_id, __parameters);

}

parser::Ast_Node_Struct_Declaration* parser::getStructDeclaration(int __declaration_id) {

    if (parser::ast_control->code_block_chain->last && parser::ast_control->code_block_chain->last->object)

        return parser::ast_control->code_block_chain->last->object->getStructDeclaration(__declaration_id);

    return parser::ast_control->name_space_chain->last->object->getStructDeclaration(__declaration_id);

}

template <typename type>
utils::Linked_List <type*>* parser::getSpecificNodesFromLinkedList(utils::Linked_List <Ast_Node*>* __linked_list, int __node_type_id) {

    utils::Linked_List <type*>* _nodes = new utils::Linked_List <type*>();
    _nodes->destroy_content = 0;

    for (int _  = 0; _ < __linked_list->count; _++)

        if (__linked_list->operator[](_)->node_type == __node_type_id)

            _nodes->add(
                (type*) __linked_list->operator[](_)
            );

    return _nodes;

}

template 
    utils::Linked_List <parser::Ast_Node_Variable_Declaration*>* 
        parser::getSpecificNodesFromLinkedList<parser::Ast_Node_Variable_Declaration>(utils::Linked_List <Ast_Node*>*, int);


