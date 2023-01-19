#include "ast_helper.h"

#include "ast.h"

#include "built_ins_definitions.h"
#include "token_definitions.h"
#include "built_ins_helper.h"
#include "exception_handle.h"
#include "tokenizer_helper.h"
#include "ast_definitions.h"
#include "linked_List.h"
#include "ast_nodes.h"
#include "token.h"

#include <iostream>
#include <string.h>


parser::Type_Information::~Type_Information() {}

parser::Type_Information::Type_Information(parser::Ast_Node_Struct_Declaration* __declaration, utils::Linked_List <int>* __pointer_operations) 
    : declaration(__declaration), pointer_level(0), reference_level(0) {

        if (!__pointer_operations) return;

        for (int _ = 0; _ < __pointer_operations->count; _++)

            if (__pointer_operations->operator[](_) == POINTER) {

                if (reference_level) exception_handle->runExceptionAstControl("Cannot make a pointer of a reference");

                pointer_level++;

            }

            else if (reference_level++) exception_handle->runExceptionAstControl("Just support 1 reference");

}

bool parser::Type_Information::operator==(Type_Information* __to_comp) {

    bool _declaration_comp = !strcmp(declaration->struct_name, __to_comp->declaration->struct_name);

    std::cout << "Comp" << std::endl;
    std::cout << "Struct Name this -> " << declaration->struct_name << std::endl;
    std::cout << "Struct Name to_comp -> " << __to_comp->declaration->struct_name << std::endl;
    std::cout << "this pointer level -> " << pointer_level << std::endl;
    std::cout << "to_comp pointer level -> " << __to_comp->pointer_level << std::endl;
    std::cout << (declaration == __to_comp->declaration) << std::endl;
    std::cout << (pointer_level == __to_comp->pointer_level) << std::endl;
    std::cout << (reference_level == __to_comp->reference_level) << std::endl;
    std::cout << (__to_comp->isPointerStruct() && pointer_level) << std::endl;
    std::cout << (isPointerStruct() && __to_comp->pointer_level) << std::endl;
    std::cout << isSpykeStruct() << std::endl;
    std::cout << __to_comp->isSpykeStruct() << std::endl;
    std::cout << "Comp" << std::endl;

    return isSpykeStruct() || __to_comp->isSpykeStruct() || ((__to_comp->isPointerStruct() && pointer_level) || (isPointerStruct() && __to_comp->pointer_level)) || (
        declaration == __to_comp->declaration &&
        pointer_level == __to_comp->pointer_level
        // reference_level == __to_comp->reference_level
    );

}

bool parser::Type_Information::operator!=(Type_Information* __to_comp) { return !operator==(__to_comp); }

parser::Type_Information* parser::Type_Information::generate(Type_Information* __type) {

    utils::Linked_List <int>* _pointer_operations = getPointerOperations();

    parser::Type_Information* _type_information = new parser::Type_Information(__type->declaration, _pointer_operations);

    delete _pointer_operations;

    return _type_information;

}

parser::Type_Information* parser::Type_Information::generate(int __implicit_type_id) {

    int _backup_state = parser::ast_control->current_position;

    utils::Linked_List <char*>* _built_ins_scope = new utils::Linked_List <char*>(); _built_ins_scope->destroy_content = 0;

    _built_ins_scope->add(
        (char*) "built_ins"
    );

    Name_Space* _name_space = parser::ast_control->name_space_control->getNameSpace(_built_ins_scope);

    if (!_name_space) exception_handle->runException("built_ins node not defined");

    delete _built_ins_scope;

    parser::ast_control->addToChain(_name_space, NULL);

    char* _struct_name = built_ins::getStructNameOfTokenId(__implicit_type_id);

    int _declaration_id = getCurrentDeclarationTracker()->getDeclarationId(_struct_name);

    Ast_Node_Struct_Declaration* _declaration = getCurrentDeclarationTracker()->getStructDeclaration(_declaration_id);

    if (_declaration_id == -1 || !_declaration) {

        free(_struct_name);

        parser::ast_control->current_position = _backup_state;

        if (_name_space) parser::ast_control->popFromChain();

        exception_handle->runExceptionAstControl("Undefined type");

    }

    parser::Type_Information* _type_information = new parser::Type_Information(
        _declaration, NULL
    );

    free(_struct_name);

    if (_name_space) parser::ast_control->popFromChain();

    return _type_information;

}

parser::Type_Information* parser::Type_Information::generate() {

    int _backup_state = parser::ast_control->current_position;

    Name_Space* _name_space = getNameSpace();

    if (_name_space) parser::ast_control->addToChain(_name_space, NULL);

    int _token_id = parser::ast_control->getToken(0)->id;
    Ast_Node_Struct_Declaration* _declaration = NULL;
    char* _struct_name;

    if (isPrimitive(_token_id)) {

        utils::Linked_List <char*>* _built_ins_scope = new utils::Linked_List <char*>(); _built_ins_scope->destroy_content = 0;

        _built_ins_scope->add(
            (char*) "built_ins"
        );

        _name_space = parser::ast_control->name_space_control->getNameSpace(_built_ins_scope);

        if (!_name_space) exception_handle->runException("built_ins node not defined");

        parser::ast_control->current_position = _backup_state;

        delete _built_ins_scope;

        parser::ast_control->addToChain(_name_space, NULL);

        _struct_name = built_ins::getStructNameOfTokenId(_token_id);

    }

    else if (_token_id == IDENTIFIER) {

        _struct_name = (char*) malloc(strlen(parser::ast_control->getToken(0)->identifier) + 1);

        strcpy(_struct_name, parser::ast_control->getToken(0)->identifier);
  
    }

    else parser::exception_handle->runExceptionAstControl("Unexpected token");

    parser::ast_control->current_position++;

    int _declaration_id = getCurrentDeclarationTracker()->getDeclarationId(_struct_name);

    _declaration = getCurrentDeclarationTracker()->getStructDeclaration(_declaration_id);

    if (_declaration_id == -1 || !_declaration) {

        free(_struct_name);

        parser::ast_control->current_position = _backup_state;

        if (_name_space) parser::ast_control->popFromChain();

        exception_handle->runExceptionAstControl("Undefined type");

    }
    
    utils::Linked_List <int>* _pointer_operations = getPointerOperations();

    parser::Type_Information* _type_information = new parser::Type_Information(
        _declaration, _pointer_operations
    );
    
    std::cout << "Pointer level -> " << _type_information->pointer_level << std::endl;

    delete _pointer_operations;

    free(_struct_name);

    if (_name_space) parser::ast_control->popFromChain();

    return _type_information;

}

parser::Type_Information* parser::Type_Information::generatePrimitiveType(int __primitive_type_token) {

    utils::Linked_List <char*>* _built_ins_scope = new utils::Linked_List <char*>(); _built_ins_scope->destroy_content = 0;

    _built_ins_scope->add(
        (char*) "built_ins"
    );

    Name_Space* _name_space = parser::ast_control->name_space_control->getNameSpace(_built_ins_scope);

    if (!_name_space) exception_handle->runException("built_ins node not defined");

    delete _built_ins_scope;

    parser::ast_control->addToChain(_name_space, NULL);

    char* _struct_name = built_ins::getStructNameOfTokenId(__primitive_type_token);

    int _declaration_id = getCurrentDeclarationTracker()->getDeclarationId(_struct_name);

    Ast_Node_Struct_Declaration* _declaration = getCurrentDeclarationTracker()->getStructDeclaration(_declaration_id);

    std::cout << _declaration_id << std::endl;
    std::cout << _declaration << std::endl;

    if (_declaration_id == -1 || !_declaration) {

        exception_handle->runExceptionAstControl("Undefined type");

    }
    
    parser::Type_Information* _type_information = new parser::Type_Information(
        _declaration, NULL
    );
    
    free(_struct_name);

    if (_name_space) parser::ast_control->popFromChain();

    return _type_information;

}

parser::Type_Information* parser::Type_Information::getCopy() {

    parser::Type_Information* _copy = new parser::Type_Information(declaration, NULL);

    _copy->pointer_level = pointer_level;
    _copy->reference_level = reference_level;

    return _copy;

}

bool parser::Type_Information::isPointerStruct() { return !strncmp(declaration->struct_name, PRIMITIVE_TYPE_POINTER_NAME, strlen(PRIMITIVE_TYPE_POINTER_NAME)); }

bool parser::Type_Information::isSpykeStruct() { return !strncmp(declaration->struct_name, PRIMITIVE_TYPE_SPYKE_NAME, strlen(PRIMITIVE_TYPE_SPYKE_NAME)); }

int parser::Type_Information::getSize() {

    if (!pointer_level) return declaration->getSize();

    return PRIMITIVE_TYPE_POINTER_SIZE;

}

bool parser::Type_Information::haveConstructorDefined() {

    if (pointer_level) {

        Type_Information* _pointer_type = Type_Information::generatePrimitiveType(PRIMITIVE_TYPE_POINTER);

        bool _rtr = _pointer_type->haveConstructorDefined();

        delete _pointer_type;

        return _rtr;

    }

    return declaration->haveContructorDefined();


}


parser::Expression_Result_Helper::~Expression_Result_Helper() {}

parser::Expression_Result_Helper::Expression_Result_Helper(Ast_Node_Expression* __expression) 
    : declaration(__expression->representive_declaration), expression(__expression), function_result_value(0), token_id(__expression->token_id) {}


int parser::getNodeType() { 

    std::cout << "Node type -> " << (int) parser::ast_control->getToken(0)->id << std::endl;

    int _backup_state;

    switch (parser::ast_control->getToken(0)->id)
    {
    case CLOSE_BRACES: return -1; break;
    case END_INSTRUCTION: return -2; break;
    case CLOSE_PARENTHESIS: return -3; break;
    case OPEN_BRACES: return AST_NODE_CODE_BLOCK; break;
    case COMMA: return -4; break;
    case CLOSE_BRACKET: return -5; break; //next -7
    case BYTE_CODE: return AST_NODE_BYTE_CODE; break;
    case SIZE_OF: return AST_NODE_SIZE_OF; break;
    case RETURN: return AST_NODE_RETURN; break;
    case NAMESPACE: return AST_NODE_NAME_SPACE; break;
    case STRUCT: return AST_NODE_STRUCT_DECLARATION; break;
    case OPEN_PARENTHESIS: 

        _backup_state = parser::ast_control->current_position;

        parser::ast_control->current_position++;

        try { delete Type_Information::generate(); }
        catch(...) { parser::ast_control->current_position = _backup_state; return AST_NODE_PARENTHESIS; }

        parser::ast_control->current_position = _backup_state;
        
        return AST_NODE_CAST;

    case POINTER: case ADDRESS: return AST_NODE_POINTER_OPERATION; break;
    case ACCESSING: case ACCESSING_POINTER: return  AST_NODE_ACCESSING; break;
    case IF: return AST_NODE_IF; break;
    case ELSE_IF: return AST_NODE_ELSE_IF; break;
    case ELSE: return AST_NODE_ELSE; break;
    default: break;
    }

    _backup_state = parser::ast_control->current_position;

    if (isPrimitive(parser::ast_control->getToken(0)->id) || isNameSpaceScope() || parser::ast_control->getToken(0)->id == IDENTIFIER || parser::ast_control->getToken(0)->id == STATIC) {

        if (parser::ast_control->getToken(0)->id == STATIC) parser::ast_control->current_position++;

        int _node_type;

        if (isPrimitive(parser::ast_control->getToken(0)->id)) delete Type_Information::generate();

        else

            try { delete Type_Information::generate(); }
            catch(...) { 

                getNameSpace();

                _node_type = (parser::ast_control->getToken(1)->id == OPEN_PARENTHESIS) ? AST_NODE_FUNCTION_CALL : AST_NODE_VARIABLE; 
                goto reset_return;
            }

        getNameSpace();

        switch (parser::ast_control->getToken(1)->id)
        {
        case FUNCTION_OPERATOR_EQUAL: case COMMA: case END_INSTRUCTION: case OPEN_BRACKET: _node_type = AST_NODE_VARIABLE_DECLARATION; break;
        case OPEN_PARENTHESIS: _node_type = AST_NODE_FUNCTION_DECLARATION; break;
        default: exception_handle->runExceptionAstControl("Unexpected token - getNodeType() isPrimitive"); break;
        }
    
    reset_return:

        parser::ast_control->current_position = _backup_state;

        return _node_type;

    }

    if (isImplicitValueOrIdentifier(parser::ast_control->getToken(0)->id)) return AST_NODE_VALUE;

    if (isFunctionOperatorSingleArgument(parser::ast_control->getToken(0)->id)) return -6;

excpection:
    exception_handle->runExceptionAstControl("Unknow token");

    return -1;

}


utils::Linked_List <char*>* parser::getScope() {

    utils::Linked_List <char*>* _scope = new utils::Linked_List <char*>(); _scope->destroy_content = 0;

    int _expected = IDENTIFIER;

    while(parser::ast_control->getToken(0)->id == _expected) {

        if (_expected == IDENTIFIER) {

            _scope->add(
                parser::ast_control->getToken(0)->identifier
            );

            _expected = DOUBLE_COLON;

        } else _expected = IDENTIFIER;

        parser::ast_control->current_position++;

    }

    if (_expected == IDENTIFIER) exception_handle->runExceptionAstControl("Expected token identifier - getScope()");

    return _scope;

}

utils::Linked_List <char*>* parser::getNameSpaceScope() {

    if (!isNameSpaceScope()) return NULL;

    bool _is_global = parser::ast_control->getToken(0)->id == DOUBLE_COLON;

    if (_is_global) parser::ast_control->current_position++;

    utils::Linked_List <char*>* _scope = getScope();

    if (!_is_global)

        _scope->join(
            getCurrentScope(), 0
        );

    utils::Data_Linked_List <char*>* _data_linked_list = _scope->remove(_scope->count);
    _data_linked_list->destroy_content = 0;

    parser::ast_control->current_position--;
    
    delete _data_linked_list;

    return _scope;

}

parser::Name_Space* parser::getNameSpace() {

    utils::Linked_List <char*>* _name_space_scope = getNameSpaceScope();

    if(!_name_space_scope) return NULL;

    Name_Space* _name_space = parser::ast_control->name_space_control->getNameSpace(_name_space_scope);

    if (!_name_space) parser::exception_handle->runExceptionAstControl("Undefined Name Space");

    delete _name_space_scope;

    return _name_space;

}


utils::Linked_List <int>* parser::getPointerOperations() {

    utils::Linked_List <int>* _pointer_operations = new utils::Linked_List <int>();

    if (parser::ast_control->getToken(0)->id == FUNCTION_OPERATOR_MULTIPLICATION || parser::ast_control->getToken(0)->id == FUNCTION_OPERATOR_BITWISE_AND)

        parser::ast_control->getToken(0)->id = parser::ast_control->getToken(0)->id == FUNCTION_OPERATOR_MULTIPLICATION ? POINTER : ADDRESS;
      
    while(parser::ast_control->getToken(0)->id == POINTER || parser::ast_control->getToken(0)->id == ADDRESS) 
        { _pointer_operations->add(parser::ast_control->getToken(0)->id); parser::ast_control->current_position++; }

    return _pointer_operations;

}  

void parser::ignoreCodeBlock() {

    if (ast_control->getToken(0)->id == END_INSTRUCTION) {

        parser::ast_control->current_position++;

        return;

    }

    while(ast_control->getToken(0)->id != CLOSE_BRACES) {

        ast_control->current_position++;
    
        if (ast_control->getToken(0)->id == OPEN_BRACES) ignoreCodeBlock();

    }

    ast_control->current_position++;

}


parser::Name_Space* parser::getCurrentNameSpace() {

    utils::Data_Linked_List <parser::Name_Space*>* _current_data_linked_list = 
        parser::ast_control->name_space_chain->getDataLinkedList(parser::ast_control->name_space_chain->count);

    while(!_current_data_linked_list->object) {

        _current_data_linked_list = _current_data_linked_list->previous;

        if (!_current_data_linked_list) exception_handle->runException("Internal error");

    }

    return _current_data_linked_list->object;

}

utils::Linked_List <char*>* parser::getCurrentScope() { return getCurrentNameSpace()->scope; }

parser::Declarations_Tracker* parser::getCurrentDeclarationTracker() {

    if (parser::ast_control->code_block_chain->last->object) return parser::ast_control->code_block_chain->last->object->declarations_tracker;

    return parser::ast_control->name_space_chain->last->object->declarations_tracker;

}



int parser::getDeclarationId(char* __name) {

    if (parser::ast_control->code_block_chain->last->object) return parser::ast_control->code_block_chain->last->object->getDeclarationId(__name);

    return parser::ast_control->name_space_chain->last->object->name_space_nodes->last->object->getDeclarationId(__name);

}

parser::Ast_Node_Variable_Declaration* parser::getVariableDeclaration(int __declaration_id) {

    if (parser::ast_control->code_block_chain->last->object) return parser::ast_control->code_block_chain->last->object->getVariableDeclaration(__declaration_id);

    return parser::ast_control->name_space_chain->last->object->name_space_nodes->last->object->getVariableDeclaration(__declaration_id);

}

parser::Ast_Node_Function_Declaration* parser::getFunctionDeclaration(int __declaration_id, utils::Linked_List <Ast_Node*>* __parameters) {

    if (parser::ast_control->code_block_chain->last->object) return parser::ast_control->code_block_chain->last->object->getFunctionDeclaration(__declaration_id, __parameters);

    return parser::ast_control->name_space_chain->last->object->name_space_nodes->last->object->getFunctionDeclaration(__declaration_id, __parameters);

}

parser::Ast_Node_Struct_Declaration* parser::getStructDeclaration(int __declaration_id) {

    if (parser::ast_control->code_block_chain->last->object) return parser::ast_control->code_block_chain->last->object->getStructDeclaration(__declaration_id);

    return parser::ast_control->name_space_chain->last->object->name_space_nodes->last->object->getStructDeclaration(__declaration_id);

}


bool parser::isNameSpaceScope() {

    return (
        (parser::ast_control->getToken(0)->id == DOUBLE_COLON && parser::ast_control->getToken(1)->id == IDENTIFIER) ||
        (parser::ast_control->getToken(0)->id == IDENTIFIER && parser::ast_control->getToken(1)->id == DOUBLE_COLON)
    );

}

bool parser::isGlobalDeclaration() { return !parser::ast_control->code_block_chain->last->object; } 


int parser::getOperatorPriority(int __token_id) { // order may not be correct TODO

    if (__token_id == OPEN_BRACKET || __token_id == FUNCTION_OPERATOR_NOT || __token_id == FUNCTION_OPERATOR_BITWISE_NOT) return 0;
    if (__token_id >= FUNCTION_OPERATOR_MULTIPLICATION && __token_id <= FUNCTION_OPERATOR_MODULOS) return 1;
    if (__token_id >= FUNCTION_OPERATOR_PLUS && __token_id <= FUNCTION_OPERATOR_MINUS) return 2;
    if (__token_id >= FUNCTION_OPERATOR_BITWISE_AND && __token_id <= FUNCTION_OPERATOR_BITWISE_RIGHT_SHIFT) return 3;
    if (__token_id >= FUNCTION_OPERATOR_AND && __token_id <= FUNCTION_OPERATOR_OR) return 4;
    if (__token_id >= FUNCTION_OPERATOR_EQUAL_TO && __token_id <= FUNCTION_OPERATOR_LESS_THAN_EQUAL_TO) return 5;

    return 6;

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


