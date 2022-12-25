#include "ast_nodes.h"

#include "exception_handle.h"
#include "tokenizer.h"

#include "token_definitions.h"
#include "tokenizer_helper.h"
#include "ast_definitions.h"
#include "linked_List.h"
#include "ast_helper.h"
#include "token.h"
#include "ast.h"

#include <iostream>


parser::Ast_Node::~Ast_Node() {}

parser::Ast_Node::Ast_Node(int __node_type, int __declaration_id) : node_type(__node_type), declaration_id(__declaration_id) {}


parser::Ast_Node_Name_Space::~Ast_Node_Name_Space() { delete declarations; }

parser::Ast_Node_Name_Space::Ast_Node_Name_Space(Name_Space* __name_space) : Ast_Node(AST_NODE_NAME_SPACE, -1), name_space(__name_space) 
    { declarations = new utils::Linked_List <Ast_Node*>(); }

void parser::Ast_Node_Name_Space::generate(Name_Space* __name_space) {

    parser::ast_control->debug_information_tab++;
    parser::ast_control->print("Ast Node Name Space\n");

    Ast_Node_Name_Space* _node_name_space = (Ast_Node_Name_Space*) malloc(sizeof(Ast_Node_Name_Space));

    new (_node_name_space) Ast_Node_Name_Space(
        __name_space
    );

    parser::ast_control->name_space_node_collection->add(_node_name_space);
    parser::ast_control->addNameSpaceNodeToChain(_node_name_space);

    _node_name_space->setDeclarations();

    parser::ast_control->popNameSpaceChainFromChain();

    parser::ast_control->print("Ast Node Name Space End\n");
    parser::ast_control->debug_information_tab--;
    
}

void parser::Ast_Node_Name_Space::generate() {

    parser::ast_control->current_token_position++;

    utils::Linked_List <char*>* _scope = getNameSpaceScopeFromKeyWord();

    Name_Space* _name_space = parser::ast_control->name_space_control->getNameSpaceOrAdd(_scope);

    _scope->destroy_content = 0; delete _scope;

    if (parser::ast_control->getToken(0)->id != OPEN_BRACES) parser::exception_handle->runExceptionAstControl("Expected token '{'");

    parser::ast_control->current_token_position++;

    generate(_name_space);

}

void parser::Ast_Node_Name_Space::setDeclarations() {

    utils::Linked_List <Ast_Node*>* _temp;
    int _node_type;

    while(ast_control->current_token_position < tokenizer_control->tokens_collection->count) { 

        switch (_node_type = getNodeType())
        {
        case -1: parser::ast_control->current_token_position++; goto out; break;
        case AST_NODE_NAME_SPACE: Ast_Node_Name_Space::generate(); break;
        case AST_NODE_STRUCT_DECLARATION: declarations->add(Ast_Node_Struct_Declaration::generate()); break;
        case AST_NODE_VARIABLE_DECLARATION: 
            _temp = Ast_Node_Variable_Declaration::generate(); 
            declarations->join(
                _temp
            );
            _temp->destroy_content = 0; delete _temp;
            break;
        case AST_NODE_FUNCTION_DECLARATION:

            declarations->add(
                Ast_Node_Function_Declaration::generate(NULL)
            );

            break; 

        default: exception_handle->runExceptionAstControl("Node not supported in Name Space Node"); break;
        }

    }

out:;

}

int parser::Ast_Node_Name_Space::getDeclarationId(char* __name) {

    int _declaration_id = name_space->declaration_tracker->getDeclarationId(__name);

    if (_declaration_id == -1) {

        Name_Space* _previous_name_space = parser::ast_control->name_space_control->getPreviousNameSpace(name_space);

        if (_previous_name_space) return parser::ast_control->getNodeNameSpace(_previous_name_space)->getDeclarationId(__name);
    }

    return _declaration_id;

}

parser::Ast_Node_Variable_Declaration* parser::Ast_Node_Name_Space::getVariableDeclaration(int __declaration_id) {

    parser::Ast_Node_Variable_Declaration* _variable_declaration = name_space->declaration_tracker->getVariableDeclaration(__declaration_id);

    if (!_variable_declaration) {

        Name_Space* _previous_name_space = parser::ast_control->name_space_control->getPreviousNameSpace(name_space);

        if (_previous_name_space) return parser::ast_control->getNodeNameSpace(_previous_name_space)->getVariableDeclaration(__declaration_id);
    }

    return _variable_declaration;

}

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Name_Space::getFunctionDeclaration(int __declaration_id, utils::Linked_List <Ast_Node*>* __parameters) {

    parser::Ast_Node_Function_Declaration* _function_declaration = name_space->declaration_tracker->getFunctionDeclaration(__declaration_id, __parameters);

    if (!_function_declaration) {

        Name_Space* _previous_name_space = parser::ast_control->name_space_control->getPreviousNameSpace(name_space);

        if (_previous_name_space) return parser::ast_control->getNodeNameSpace(_previous_name_space)->getFunctionDeclaration(__declaration_id, __parameters);
    }

    return _function_declaration;

}

parser::Ast_Node_Struct_Declaration* parser::Ast_Node_Name_Space::getStructDeclaration(int __declaration_id) {

    parser::Ast_Node_Struct_Declaration* _struct_declaration = name_space->declaration_tracker->getStructDeclaration(__declaration_id);

    if (!_struct_declaration) {

        Name_Space* _previous_name_space = parser::ast_control->name_space_control->getPreviousNameSpace(name_space);

        if (_previous_name_space) return parser::ast_control->getNodeNameSpace(_previous_name_space)->getStructDeclaration(__declaration_id);
    }

    return _struct_declaration;

}


parser::Ast_Node_Code_Block::~Ast_Node_Code_Block() { delete code; delete declaration_tracker; }

parser::Ast_Node_Code_Block::Ast_Node_Code_Block(Ast_Node_Code_Block* __previous_code_block, Name_Space* __name_space) 
    : Ast_Node(AST_NODE_CODE_BLOCK, -1), previous_code_block(__previous_code_block), name_space(__name_space), declaration_off(previous_code_block ? previous_code_block->declaration_off : 0)
        { code = new utils::Linked_List <Ast_Node*>(); declaration_tracker = new Declaration_Tracker(&declaration_off); }

void parser::Ast_Node_Code_Block::generate() {

    ast_control->debug_information_tab++;
    ast_control->print("Generate Code Block Body\n");

    ast_control->code_block_chain->last->object->setCode();

    ast_control->popCodeBlockChainFromChain();

    ast_control->print("Generate Code Block Body End\n");
    ast_control->debug_information_tab--;

}

void parser::Ast_Node_Code_Block::setUp() {

    parser::ast_control->debug_information_tab++;
    ast_control->print("Set Code Block up\n");

    utils::Data_Linked_List <Ast_Node_Code_Block*>* _data_linked_list_previous_code_block = ast_control->code_block_chain->last;
    Ast_Node_Code_Block* _previous_code_block = _data_linked_list_previous_code_block ? _data_linked_list_previous_code_block->object : NULL;

    parser::Ast_Node_Code_Block* _node_code_block = (parser::Ast_Node_Code_Block*) malloc(sizeof(parser::Ast_Node_Code_Block));
    new (_node_code_block) parser::Ast_Node_Code_Block(_previous_code_block, ast_control->name_space_chain->last->object->name_space);

    if (!_previous_code_block) _node_code_block->declaration_off = parser::ast_control->name_space_control->declaration_off;

    ast_control->addCodeBlockNodeToChain(_node_code_block);

    ast_control->print("Set Code Block up end\n");
    parser::ast_control->debug_information_tab--;

}

void parser::Ast_Node_Code_Block::setCode() {

    utils::Linked_List <Ast_Node*>* _temp;
    int _node_type;

    while(ast_control->current_token_position < tokenizer_control->tokens_collection->count) { 

        switch (_node_type = getNodeType())
        {
        case -1: parser::ast_control->current_token_position++; goto out; break;
        case AST_NODE_VARIABLE_DECLARATION: 
            _temp = Ast_Node_Variable_Declaration::generate(); 
            code->join(
                _temp
            );
            _temp->destroy_content = 0; delete _temp;
            break;
        case AST_NODE_VARIABLE: case AST_NODE_VALUE: case AST_NODE_FUNCTION_CALL:

            code->add(
                Ast_Node_Expression::generate(_node_type)
            );

            if (parser::ast_control->getToken(0)->id != END_INSTRUCTION) exception_handle->runExceptionAstControl("Expected token ';'");

            else parser::ast_control->current_token_position++;

            break;

        default: exception_handle->runExceptionAstControl("Node not supported in Code Block Node"); break;
        }

        

    }

out:;

}
 
int parser::Ast_Node_Code_Block::getDeclarationId(char* __name) {

    int _declaration_id = declaration_tracker->getDeclarationId(__name);

    if (_declaration_id == -1) {

        if (previous_code_block) return previous_code_block->getDeclarationId(__name);

        return parser::ast_control->getNodeNameSpace(name_space)->getDeclarationId(__name);

    }

    return _declaration_id;

}

parser::Ast_Node_Variable_Declaration* parser::Ast_Node_Code_Block::getVariableDeclaration(int __declaration_id) {

    parser::Ast_Node_Variable_Declaration* _variable_declaration = declaration_tracker->getVariableDeclaration(__declaration_id);

    if (!_variable_declaration) {

        if (previous_code_block) return previous_code_block->getVariableDeclaration(__declaration_id);

        return parser::ast_control->getNodeNameSpace(name_space)->getVariableDeclaration(__declaration_id);

    }

    return _variable_declaration;

}

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Code_Block::getFunctionDeclaration(int __declaration_id, utils::Linked_List <Ast_Node*>* __parameters) {

    parser::Ast_Node_Function_Declaration* _function_declaration = declaration_tracker->getFunctionDeclaration(__declaration_id, __parameters);

    if (!_function_declaration) {

        if (previous_code_block) return previous_code_block->getFunctionDeclaration(__declaration_id, __parameters);

        return parser::ast_control->getNodeNameSpace(name_space)->getFunctionDeclaration(__declaration_id, __parameters);

    }

    return _function_declaration;

}

parser::Ast_Node_Struct_Declaration* parser::Ast_Node_Code_Block::getStructDeclaration(int __declaration_id) {

    parser::Ast_Node_Struct_Declaration* _struct_declaration = declaration_tracker->getStructDeclaration(__declaration_id);

    if (!_struct_declaration) {

        if (previous_code_block) return previous_code_block->getStructDeclaration(__declaration_id);

        return parser::ast_control->getNodeNameSpace(name_space)->getStructDeclaration(__declaration_id);

    }

    return _struct_declaration;

}


parser::Ast_Node_Variable_Declaration::~Ast_Node_Variable_Declaration() { delete type; }

parser::Ast_Node_Variable_Declaration::Ast_Node_Variable_Declaration(int __declaration_id, parser::Type_Information* __type_information, bool __global) 
    : Ast_Node(AST_NODE_VARIABLE_DECLARATION, __declaration_id), type(__type_information), global(__global) {}

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Variable_Declaration::generate() { // handle []

    utils::Linked_List <parser::Ast_Node*>* _nodes = new utils::Linked_List <parser::Ast_Node*>();
    parser::Ast_Node_Variable_Declaration* _variable_declaration;
    int _declaration_id;
    bool _is_static;

    if (_is_static = (parser::ast_control->getToken(0)->id == STATIC)) parser::ast_control->current_token_position++;

    parser::Type_Information* _type = parser::Type_Information::generate();

    while(parser::ast_control->getToken(0)->id != END_INSTRUCTION) {

        parser::ast_control->debug_information_tab++;
        parser::ast_control->print("Ast Node Variable Declaration\n");

        if (parser::ast_control->getToken(0)->id != IDENTIFIER) exception_handle->runExceptionAstControl("Expected Identifier");

        if (getDeclarationTracker()->getDeclarationId(parser::ast_control->getToken(0)->identifier) != -1)

            exception_handle->runExceptionAstControl("Redefenition of variable name");

        if (_is_static) {

            _declaration_id = (*parser::ast_control->name_space_chain->last->object->name_space->declaration_tracker->off)++;

            getDeclarationTracker()->addStaticName(parser::ast_control->getToken(0)->identifier, _declaration_id);

        }

        else {

            getDeclarationTracker()->addName(parser::ast_control->getToken(0)->identifier);
            _declaration_id = getDeclarationTracker()->getDeclarationId(parser::ast_control->getToken(0)->identifier);
    
        }

        parser::ast_control->current_token_position++;

        bool _global = _is_static ? 1 : !parser::ast_control->code_block_chain->last || !parser::ast_control->code_block_chain->last->object;

        _variable_declaration = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));
        new (_variable_declaration) Ast_Node_Variable_Declaration(
            _declaration_id, 
            _type,
            _global
        );

        if (_is_static) { 
        
            parser::ast_control->name_space_chain->last->object->name_space->declaration_tracker->variable_declarations->add(_variable_declaration);
            parser::ast_control->name_space_chain->last->object->declarations->add(_variable_declaration);
            
        }
         
        else {

            getDeclarationTracker()->variable_declarations->add(_variable_declaration);
            _nodes->add(_variable_declaration);

        }

        switch (parser::ast_control->getToken(0)->id)
        {
        case FUNCTION_OPERATOR_EQUAL: exception_handle->runException("Equal in variable declaration not implement yet"); break;
        case COMMA: parser::ast_control->current_token_position++; _type = parser::Type_Information::generate(_type); break;
        case END_INSTRUCTION: break;
        default: break;
        }

        char buffer[100];

        parser::ast_control->debug_information_tab++;

        sprintf(buffer, "Declaration Id -> %d", _declaration_id);
        parser::ast_control->print(buffer);

        sprintf(buffer, "Is Static -> %s\n", _is_static ? "yes" : "no");
        parser::ast_control->print(buffer);

        parser::ast_control->debug_information_tab--;

        parser::ast_control->print("Ast Node Variable Declaration End\n");
        parser::ast_control->debug_information_tab--;

    }

    parser::ast_control->current_token_position++;

    return _nodes;

}

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Variable_Declaration::generateFunctionParameter() {

    ast_control->debug_information_tab++;
    ast_control->print("Function Parameter Created\n");
    
    utils::Linked_List <parser::Ast_Node*>* _nodes = new utils::Linked_List <parser::Ast_Node*>();

    parser::Type_Information* _type = parser::Type_Information::generate();

    int _declaration_id = -1;

    switch (ast_control->getToken(0)->id)
    {
    case IDENTIFIER:
            
            if (getDeclarationTracker()->getDeclarationId(ast_control->getToken(0)->identifier) != -1)

                exception_handle->runExceptionAstControl("Redefinition of variable name");

            getDeclarationTracker()->addName(ast_control->getToken(0)->identifier);
            _declaration_id = getDeclarationTracker()->getDeclarationId(ast_control->getToken(0)->identifier);
            ast_control->current_token_position++;

            break;
    case COMMA: case CLOSE_PARENTHESIS: break;
    default: exception_handle->runExceptionAstControl("Unexpected token");
    }

    Ast_Node_Variable_Declaration* _variable_declaration = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));
    new (_variable_declaration) Ast_Node_Variable_Declaration(_declaration_id, _type, 0);

    _nodes->add(
        _variable_declaration
    );

    getDeclarationTracker()->variable_declarations->add(
        _variable_declaration
    );

    switch (ast_control->getToken(0)->id)
    {
    case FUNCTION_OPERATOR_EQUAL: exception_handle->runException("Not done equal Function Parameter generate"); break;
    case COMMA: case CLOSE_PARENTHESIS: break;
    default: exception_handle->runExceptionAstControl("Unexpected token"); break;
    }

    char buffer[100];

    sprintf(buffer, "Declaration Id -> %d\n", _declaration_id);
    
    parser::ast_control->debug_information_tab++;
    parser::ast_control->print(buffer);
    parser::ast_control->debug_information_tab--;

    ast_control->print("Function Parameter End\n");
    ast_control->debug_information_tab--;

    return _nodes;


}


parser::Ast_Node_Function_Declaration::~Ast_Node_Function_Declaration() { delete parameters; delete return_type; if (body) body->~Ast_Node_Code_Block(); free(body); }

parser::Ast_Node_Function_Declaration::Ast_Node_Function_Declaration(
    int __declaration_id, utils::Linked_List <Ast_Node*>* __parameters, parser::Type_Information* __return_type, Ast_Node_Code_Block* __body, Name_Space* __name_space) 
        : Ast_Node(AST_NODE_FUNCTION_DECLARATION, __declaration_id), parameters(__parameters), return_type(__return_type), body(__body), name_space(__name_space) {}

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Function_Declaration::generate(parser::Ast_Node_Variable_Declaration* __struct_variable_declaration) {

    ast_control->debug_information_tab++;
    parser::ast_control->print("Ast Node Function Declaration\n");

    int _declaration_id;

    parser::Type_Information* _return_type = parser::Type_Information::generate();

    Name_Space* _function_declaration_name_space = parser::getNameSpace();

    if (ast_control->getToken(0)->id != IDENTIFIER) exception_handle->runExceptionAstControl("Expected token identifier");

    if (_function_declaration_name_space) ast_control->addNameSpaceNodeToChain(_function_declaration_name_space);

    int _backup_state = ast_control->current_token_position;
    char* _function_declaration_name = ast_control->getToken(0)->identifier;
    parser::ast_control->current_token_position++;

    parser::Ast_Node_Code_Block::setUp();
    Ast_Node_Code_Block* _body = ast_control->code_block_chain->last->object;

    if (__struct_variable_declaration) { 
        
        getDeclarationTracker()->addName( (char*) "this");

        __struct_variable_declaration->declaration_id = getDeclarationTracker()->getDeclarationId((char*) "this");
    
        getDeclarationTracker()->variable_declarations->add(
            __struct_variable_declaration
        );

    }

    utils::Linked_List <Ast_Node*>* _parameters = getParameters();

    if (__struct_variable_declaration) _parameters->insert(__struct_variable_declaration, 0);

    ast_control->addCodeBlockNodeToChain(NULL);

    if ((_declaration_id = getDeclarationTracker()->getDeclarationId(_function_declaration_name)) == -1) {

        getDeclarationTracker()->addName(_function_declaration_name);

        _declaration_id = getDeclarationTracker()->getDeclarationId(_function_declaration_name);

    }

    else {

        parser::Ast_Node_Function_Declaration* _previous_function_declaration = 
            getDeclarationTracker()->getFunctionDeclaration(_declaration_id, _parameters);

        if (_previous_function_declaration && _previous_function_declaration->body) {

            ast_control->current_token_position = _backup_state;
        
            exception_handle->runExceptionAstControl("Redefenition of function name");

        }

    }

    ast_control->popCodeBlockChainFromChain();

    parser::Ast_Node_Function_Declaration* _function_declaration = (parser::Ast_Node_Function_Declaration*) malloc(sizeof(parser::Ast_Node_Function_Declaration));
    new (_function_declaration) parser::Ast_Node_Function_Declaration(_declaration_id, _parameters, _return_type, _body, _function_declaration_name_space);
    
    ast_control->addCodeBlockNodeToChain(NULL);

    getDeclarationTracker()->function_declarations->add(_function_declaration);

    ast_control->popCodeBlockChainFromChain();

    switch (ast_control->getToken(0)->id)
    {
    case END_INSTRUCTION: 
        _body->~Ast_Node_Code_Block(); free(_body); _function_declaration->body = NULL; ast_control->popCodeBlockChainFromChain(); 
        ast_control->current_token_position++; break;
    case OPEN_BRACES: ast_control->current_token_position++; parser::Ast_Node_Code_Block::generate(); break;
    default: exception_handle->runExceptionAstControl("Unexpected token");
    }

    if (_function_declaration_name_space) ast_control->popNameSpaceChainFromChain();

    char buffer[100];

    parser::ast_control->debug_information_tab++;

    sprintf(buffer, "Declaration Id -> %d", _declaration_id);
    parser::ast_control->print(buffer);

    sprintf(buffer, "Parameters count -> %d\n", _parameters->count);
    parser::ast_control->print(buffer);

    parser::ast_control->debug_information_tab--;

    parser::ast_control->print("Ast Node Function Declaration End\n");
    parser::ast_control->debug_information_tab--;  

    return _function_declaration;

}

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Function_Declaration::getParameters() {

    ast_control->debug_information_tab++;
    ast_control->print("Generating Function Parameters\n");

    utils::Linked_List <parser::Ast_Node*>* _nodes = new utils::Linked_List <parser::Ast_Node*>(), *_temp;

    ast_control->current_token_position++;

    while(ast_control->getToken(0)->id != CLOSE_PARENTHESIS) {

        _temp = parser::Ast_Node_Variable_Declaration::generateFunctionParameter();

        _nodes->join(
            _temp
        );

        _temp->destroy_content = 0; delete _temp;

        switch (ast_control->getToken(0)->id)
        {
        case COMMA: ast_control->current_token_position++; break;
        case CLOSE_PARENTHESIS: break;
        default: exception_handle->runExceptionAstControl("Unexpected token");
        }

    }

    ast_control->current_token_position++;

    ast_control->print("Generating Function Parameters End\n");
    ast_control->debug_information_tab--;

    return _nodes;

}

int parser::Ast_Node_Function_Declaration::getByteSize() { return return_type->getByteSize(); }


parser::Ast_Node_Struct_Declaration::~Ast_Node_Struct_Declaration() {
    if (fields) fields->~Ast_Node_Code_Block(); free(fields);
}

parser::Ast_Node_Struct_Declaration::Ast_Node_Struct_Declaration(int __declaration_id, Ast_Node_Name_Space* __functions, Ast_Node_Code_Block* __fields) 
    : Ast_Node(AST_NODE_STRUCT_DECLARATION, __declaration_id), fields(__fields), functions(__functions) {}

parser::Ast_Node_Struct_Declaration* parser::Ast_Node_Struct_Declaration::generate() {

    ast_control->debug_information_tab++;
    parser::ast_control->print("Ast Node Struct Declaration\n");

    parser::ast_control->current_token_position++;

    int _declaration_id;
    char* _struct_name;

    switch (parser::ast_control->getToken(0)->id)
    {
    case IDENTIFIER:

        _struct_name = parser::ast_control->getToken(0)->identifier;

        if ((_declaration_id = getDeclarationTracker()->getDeclarationId(parser::ast_control->getToken(0)->identifier)) != -1) {

            parser::Ast_Node_Struct_Declaration* _struct_declaration = getDeclarationTracker()->getStructDeclaration(_declaration_id);

            if (!_struct_declaration || _struct_declaration->functions) 

                exception_handle->runExceptionAstControl("Redefenition of name"); 

        }
        
        else { 
            
            getDeclarationTracker()->addName(parser::ast_control->getToken(0)->identifier); 
            _declaration_id = getDeclarationTracker()->getDeclarationId(parser::ast_control->getToken(0)->identifier); 
        
        }

        parser::ast_control->current_token_position++;

        break;
    case OPEN_BRACES: exception_handle->runExceptionAstControl("Not done struct declaration of variable"); break;
    default: exception_handle->runExceptionAstControl("Unexpected token"); break;
    }

    Ast_Node_Name_Space* _functions = NULL;
    Ast_Node_Code_Block* _fields = NULL;

    if (parser::ast_control->getToken(0)->id == OPEN_BRACES) {

        utils::Linked_List <char*>* _scope_name_space = new utils::Linked_List <char*>(); 
        _scope_name_space->destroy_content = 0;

        _scope_name_space->join(
            parser::ast_control->name_space_chain->last->object->name_space->scope
        );

        _scope_name_space->add(
            _struct_name
        );

        Name_Space* _struct_name_space = (Name_Space*) malloc(sizeof(Name_Space));
        new (_struct_name_space) Name_Space(_scope_name_space, getDeclarationTracker()->off);

        delete _scope_name_space;

        _functions = (Ast_Node_Name_Space*) malloc(sizeof(Ast_Node_Name_Space));
        new (_functions) Ast_Node_Name_Space(_struct_name_space);

        _fields = (Ast_Node_Code_Block*) malloc(sizeof(Ast_Node_Code_Block));
        new (_fields) Ast_Node_Code_Block(
            ast_control->code_block_chain->last ? ast_control->code_block_chain->last->object : NULL,
            ast_control->name_space_chain->last->object->name_space
        );

        parser::ast_control->name_space_node_collection->add(
            _functions
        );

        parser::ast_control->name_space_control->addNameSpace(
            _struct_name_space
        );

    }

    parser::Ast_Node_Struct_Declaration* _struct_declaration = (parser::Ast_Node_Struct_Declaration*) malloc(sizeof(parser::Ast_Node_Struct_Declaration));
    new (_struct_declaration) parser::Ast_Node_Struct_Declaration(_declaration_id, _functions, _fields);

    getDeclarationTracker()->struct_declarations->add(
        _struct_declaration
    );

    switch (parser::ast_control->getToken(0)->id)
    {
    case OPEN_BRACES: _struct_declaration->set(); break;
    case END_INSTRUCTION: goto out; break;
    default: exception_handle->runExceptionAstControl("Unexpected token"); break;
    }

    switch (parser::ast_control->getToken(0)->id)
    {
    case IDENTIFIER: exception_handle->runExceptionAstControl("Not done"); break;
    case END_INSTRUCTION: parser::ast_control->current_token_position++; break;
    default: exception_handle->runExceptionAstControl("Expected token ';'");
    }

out:;

    ast_control->debug_information_tab++;

    char buffer[100];

    sprintf(buffer, "Struct Functions and Static Variables count -> %d\n", _struct_declaration->functions->declarations->count);
    parser::ast_control->print(buffer);

    sprintf(buffer, "Struct Fields count -> %d\n", _struct_declaration->fields->code->count);
    parser::ast_control->print(buffer);

    ast_control->debug_information_tab--;

    parser::ast_control->print("Ast Node Struct Declaration End\n");
    ast_control->debug_information_tab--;

    return _struct_declaration;

}


void parser::Ast_Node_Struct_Declaration::set() {

    int _backup_state = ++parser::ast_control->current_token_position;

    setFields(0);

    parser::ast_control->current_token_position = _backup_state;

    setFields(1);

    parser::ast_control->current_token_position = _backup_state;

    setFunctions();

    parser::ast_control->current_token_position++;

}

void parser::Ast_Node_Struct_Declaration::setFunctions() {

    Ast_Node_Variable_Declaration* _struct_variable_declaration;
    utils::Linked_List <Ast_Node*>* _temp;

    ast_control->addNameSpaceNodeToChain(functions);

    while(parser::ast_control->getToken(0)->id != CLOSE_BRACES) {

        switch (getNodeType())
        {
        case AST_NODE_VARIABLE_DECLARATION: 
        
            while(ast_control->getToken(0)->id != END_INSTRUCTION) ast_control->current_token_position++;

            ast_control->current_token_position++; 

            break;

        case AST_NODE_FUNCTION_DECLARATION: 

            _struct_variable_declaration = 0;

            if (parser::ast_control->getToken(0)->id != STATIC) {

                _struct_variable_declaration = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));

                new (_struct_variable_declaration) Ast_Node_Variable_Declaration(
                    -1,
                    new Type_Information(
                        this, IDENTIFIER, NULL, NULL
                    ),
                    0
                );

                _struct_variable_declaration->type->pointer_level++;

            } else parser::ast_control->current_token_position++;

            functions->declarations->add(
                parser::Ast_Node_Function_Declaration::generate(_struct_variable_declaration)
            );
            
            break;

        default: exception_handle->runExceptionAstControl("Unexpected Node type"); break;
        }

    }

    ast_control->popNameSpaceChainFromChain();

}

void parser::Ast_Node_Struct_Declaration::setFields(bool __static) {

    utils::Linked_List <Ast_Node*>* _temp;

    if (__static) ast_control->addNameSpaceNodeToChain(functions);
    else ast_control->addCodeBlockNodeToChain(fields);

    while(parser::ast_control->getToken(0)->id != CLOSE_BRACES) {

        switch (getNodeType())
        {
        case AST_NODE_VARIABLE_DECLARATION:

            if (parser::ast_control->getToken(0)->id == STATIC && !__static || parser::ast_control->getToken(0)->id != STATIC && __static) {

                while(ast_control->getToken(0)->id != END_INSTRUCTION) ast_control->current_token_position++;

                ast_control->current_token_position++; 

                break;

            } 
        
            _temp = Ast_Node_Variable_Declaration::generate();

            fields->code->join(
                _temp
            );

            _temp->destroy_content = 0; delete _temp;

            break;

        case AST_NODE_FUNCTION_DECLARATION: 
        
            while(ast_control->getToken(0)->id != END_INSTRUCTION && ast_control->getToken(0)->id != OPEN_BRACES) 
            
                parser::ast_control->current_token_position++;

            if (ast_control->getToken(0)->id == END_INSTRUCTION) parser::ast_control->current_token_position++;
        
            else ignoreCodeBlock();

            break;
        default: exception_handle->runExceptionAstControl("Unexpected Node type"); break;
        }

    }

    if (__static) ast_control->popNameSpaceChainFromChain(); 
    else ast_control->popCodeBlockChainFromChain();

}


parser::Ast_Node_Expression::~Ast_Node_Expression() { 
    if (value) value->~Ast_Node(); free(value);
    if (expression) expression->~Ast_Node_Expression(); free(expression);
}

parser::Ast_Node_Expression::Ast_Node_Expression(Ast_Node_Expression* __expression, Ast_Node* __value, int __token_id) 
    : Ast_Node(AST_NODE_EXPRESSION, -1), expression(__expression), value(__value), token_id(__token_id) {}

parser::Ast_Node_Variable_Declaration* parser::Ast_Node_Expression::getVariableDeclaration() {

    switch (value->node_type)
    {
    case AST_NODE_VARIABLE: return ((Ast_Node_Variable*) value)->declaration; break;
    case AST_NODE_VALUE: return ((Ast_Node_Value*) value)->declaration; break;
    default: break;
    }

    exception_handle->runExceptionAstControl("Error getting Variable declaration of this");

    return 0;

}

parser::Ast_Node_Variable_Declaration* parser::Ast_Node_Expression::getResultType() {

    utils::Linked_List <Representation*>* _expressions = new utils::Linked_List <Representation*>();
    Ast_Node_Expression* _current_expression =  this;

    while(_current_expression) {

        Representation* _representation = (Representation*) malloc(sizeof(Representation));

        new (_representation) Representation(
            _current_expression->getVariableDeclaration(), _current_expression->getPriorityLevel()
        );

        _expressions->add(_representation);

        _current_expression = _current_expression->expression;

    }

    int _priority_level = 1;

    while(_expressions->count != 1) {

        for (int _ = 0; _ < _expressions->count; _++) {

            if (_expressions->operator[](_)->priority_level != _priority_level) continue;

            

        }

        _priority_level++;

    }

    parser::Ast_Node_Variable_Declaration* _variable_declaration = _expressions->first->object->declaration;

    delete _expressions;

    return _variable_declaration;

}

int parser::Ast_Node_Expression::getPriorityLevel() {

    if (token_id >= FUNCTION_OPERATOR_MULTIPLICATION && token_id <= FUNCTION_OPERATOR_MODULOS) return 1;
    if (token_id >= FUNCTION_OPERATOR_PLUS && token_id <= FUNCTION_OPERATOR_MINUS) return 2;
    if (token_id >= FUNCTION_OPERATOR_BITWISE_AND && token_id <= FUNCTION_OPERATOR_BITWISE_RIGHT_SHIFT) return 3;
    if (token_id >= FUNCTION_OPERATOR_AND && token_id <= FUNCTION_OPERATOR_OR) return 4;

    return 5;

    //     if (__token_id >= TOKEN_MULTIPLICATION && __token_id <= TOKEN_MODULUS) return 2;
    // if (__token_id >= TOKEN_ADDITION && __token_id <= TOKEN_SUBTRACTION) return 3;
    // if (__token_id >= TOKEN_BITWISEAND && __token_id <= TOKEN_BITWISERIGHTSHIFT) return 4;
    // if (__token_id >= TOKEN_AND && __token_id <= TOKEN_NOT) return 5;

    // return 6;

}

parser::Ast_Node_Expression* parser::Ast_Node_Expression::generate(int __value_node_type) {

    ast_control->debug_information_tab++;
    parser::ast_control->print("Ast Node Expression\n");

    parser::Ast_Node* _value = getValue(__value_node_type);
    int _token_id = isFunctionOperator(parser::ast_control->getToken(0)->id) ? parser::ast_control->getToken(0)->id : -1;

    if (_token_id != -1) parser::ast_control->current_token_position++;

    parser::Ast_Node_Expression* _expression_node = (parser::Ast_Node_Expression*) malloc(sizeof(parser::Ast_Node_Expression));
    new (_expression_node) parser::Ast_Node_Expression(
        _token_id != -1 ? Ast_Node_Expression::generate(getNodeType()) : NULL,
        _value,
        _token_id
    );

    parser::ast_control->print("Ast Node Expression End\n");
    ast_control->debug_information_tab--;

    return _expression_node;

}

parser::Ast_Node* parser::Ast_Node_Expression::getValue(int __value_node_type) {

    switch (__value_node_type)
    {
    case AST_NODE_VARIABLE: return Ast_Node_Variable::generate(); break;
    case AST_NODE_VALUE: return Ast_Node_Value::generate(); break;
    case AST_NODE_FUNCTION_CALL: return Ast_Node_Function_Call::generate(); break;
    default: break;
    }

    exception_handle->runExceptionAstControl("node type not supported in expression");

    return 0;

}




parser::Ast_Node_Variable::~Ast_Node_Variable() {}

parser::Ast_Node_Variable::Ast_Node_Variable(Ast_Node_Variable_Declaration* __declaration) 
    : Ast_Node(AST_NODE_VARIABLE, -1), declaration(__declaration) {}

parser::Ast_Node_Variable* parser::Ast_Node_Variable::generate() {

    ast_control->debug_information_tab++;
    parser::ast_control->print("Ast Node Variable\n");

    Name_Space* _name_space = getNameSpace();

    if (_name_space) { ast_control->addNameSpaceNodeToChain(_name_space); ast_control->addCodeBlockNodeToChain(NULL); }

    if (ast_control->getToken(0)->id != IDENTIFIER) exception_handle->runExceptionAstControl("Expected a identifier");

    int _declaration_id = parser::getDeclarationId(ast_control->getToken(0)->identifier);

    if (_declaration_id == -1) exception_handle->runExceptionAstControl("No declaration with given name");
    parser::ast_control->current_token_position++;

    Ast_Node_Variable_Declaration* _declaration = parser::getVariableDeclaration(_declaration_id);

    parser::Ast_Node_Variable* _variable_node = (parser::Ast_Node_Variable*) malloc(sizeof(parser::Ast_Node_Variable));
    new (_variable_node) parser::Ast_Node_Variable(
        _declaration
    );


    ast_control->debug_information_tab++;
    char buffer[100];

    sprintf(buffer, "Declaration id -> %d\n", _declaration_id);
    parser::ast_control->print(buffer);

    sprintf(buffer, "Declaration is %s\n", _declaration->global ? "global" : "local");
    parser::ast_control->print(buffer);

    ast_control->debug_information_tab--;

    parser::ast_control->print("Ast Node Variable End\n");
    ast_control->debug_information_tab--;

    if (_name_space) { ast_control->popNameSpaceChainFromChain(); ast_control->popCodeBlockChainFromChain(); }

    return _variable_node;

}


parser::Ast_Node_Value::~Ast_Node_Value() { delete declaration; }

parser::Ast_Node_Value::Ast_Node_Value(int __implicit_value_position, int __token_id) 
    : Ast_Node(AST_NODE_VALUE, -1), implicit_value_position(__implicit_value_position) {

        declaration = new Ast_Node_Variable_Declaration(
            -1, 
            new Type_Information(__token_id), 
            0
        );

    }

parser::Ast_Node_Value* parser::Ast_Node_Value::generate() {

    ast_control->debug_information_tab++;
    parser::ast_control->print("Ast Node Value\n");
    
    parser::Ast_Node_Value* _value_node = (parser::Ast_Node_Value*) malloc(sizeof(parser::Ast_Node_Value));

    new (_value_node) parser::Ast_Node_Value(
        parser::ast_control->addImplicitValue(
            parser::ast_control->getToken(0)->identifier
        ),
        parser::ast_control->getToken(0)->id
    );
    parser::ast_control->current_token_position++;

    parser::ast_control->print("Ast Node Value End\n");
    ast_control->debug_information_tab--;

    return _value_node;

}


parser::Ast_Node_Function_Call::~Ast_Node_Function_Call() { delete parameters; }

parser::Ast_Node_Function_Call::Ast_Node_Function_Call(
    utils::Linked_List <Ast_Node_Expression*>* __parameters, Ast_Node_Function_Declaration* __declaration) 
        : Ast_Node(AST_NODE_FUNCTION_CALL, -1), parameters(__parameters), declaration(__declaration) {}

parser::Ast_Node_Function_Call* parser::Ast_Node_Function_Call::generate() {

    ast_control->debug_information_tab++;
    parser::ast_control->print("Ast Node Function Call\n");

    Name_Space* _name_space = getNameSpace();

    if (_name_space) { ast_control->addNameSpaceNodeToChain(_name_space); ast_control->addCodeBlockNodeToChain(NULL); }
    
    if (parser::ast_control->getToken(0)->id != IDENTIFIER) exception_handle->runExceptionAstControl("Expected token identifier");

    int _declaration_id = parser::getDeclarationId(parser::ast_control->getToken(0)->identifier);

    if (_declaration_id == -1) exception_handle->runExceptionAstControl("No function declared with given name");
    parser::ast_control->current_token_position++;

    utils::Linked_List <Ast_Node_Expression*>* _parameters = getParameters();

    utils::Linked_List <Ast_Node*>* _parameters_results = getParametersResults(_parameters);

    Ast_Node_Function_Declaration* _declaration = parser::getFunctionDeclaration(_declaration_id, _parameters_results);

    if (!_declaration) exception_handle->runExceptionAstControl("No function declaration with given parameters");

    parser::Ast_Node_Function_Call* _function_call_node = (parser::Ast_Node_Function_Call*) malloc(sizeof(parser::Ast_Node_Function_Call));

    new (_function_call_node) parser::Ast_Node_Function_Call(
        _parameters, _declaration
    );

    if (_name_space) { ast_control->popNameSpaceChainFromChain(); ast_control->popCodeBlockChainFromChain(); }

    delete _parameters_results;
    
    parser::ast_control->print("Ast Node Function Call End\n");
    ast_control->debug_information_tab--;

    return _function_call_node;

}

utils::Linked_List <parser::Ast_Node_Expression*>* parser::Ast_Node_Function_Call::getParameters() {

    utils::Linked_List <parser::Ast_Node_Expression*>* _parameters = new utils::Linked_List <parser::Ast_Node_Expression*>();

    ast_control->current_token_position++;

    while(ast_control->getToken(0)->id != CLOSE_PARENTHESIS) {

        _parameters->add(
            Ast_Node_Expression::generate(
                getNodeType()
            )
        );

        switch (parser::ast_control->getToken(0)->id)
        {
        case COMMA: parser::ast_control->current_token_position++;
        case CLOSE_PARENTHESIS: break;        
        default: exception_handle->runExceptionAstControl("Unexpected token"); break;
        }

    }

    ast_control->current_token_position++;

    return _parameters;

}

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Function_Call::getParametersResults(
    utils::Linked_List <parser::Ast_Node_Expression*>* __parameters) {

        utils::Linked_List <parser::Ast_Node*>* _results = new utils::Linked_List <parser::Ast_Node*>(); _results->destroy_content = 0;

        for (int _ = 0; _ < __parameters->count; _++)

            _results->add(__parameters->operator[](_)->getResultType());

        return _results;

}

