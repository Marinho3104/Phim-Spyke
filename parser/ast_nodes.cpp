#include "ast_nodes.h"

#include "exception_handle.h"
#include "tokenizer.h"

#include "token_definitions.h"
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
        case AST_NODE_VARIABLE_DECLARATION: 
            _temp = Ast_Node_Variable_Declaration::generate(); 
            declarations->join(
                _temp
            );
            _temp->destroy_content = 0; delete _temp;
            break;
        case AST_NODE_FUNCTION_DECLARATION:

            declarations->add(
                Ast_Node_Function_Declaration::generate()
            );

            break;

        default: exception_handle->runExceptionAstControl("Node not supported in Name Space Node"); break;
        }

        

    }

out:;

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
        default: exception_handle->runExceptionAstControl("Node not supported in Name Space Node"); break;
        }

        

    }

out:;

}


parser::Ast_Node_Variable_Declaration::~Ast_Node_Variable_Declaration() { delete type; }

parser::Ast_Node_Variable_Declaration::Ast_Node_Variable_Declaration(int __declaration_id, parser::Type_Information* __type_information) 
    : Ast_Node(AST_NODE_VARIABLE_DECLARATION, __declaration_id), type(__type_information) {}

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

        _variable_declaration = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));
        new (_variable_declaration) Ast_Node_Variable_Declaration(_declaration_id, _type);

        if (_is_static) parser::ast_control->name_space_chain->last->object->declarations->add(_variable_declaration);
         
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
    new (_variable_declaration) Ast_Node_Variable_Declaration(_declaration_id, _type);

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

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Function_Declaration::generate() {

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
    utils::Linked_List <Ast_Node*>* _parameters = getParameters();

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
        
            exception_handle->runExceptionAstControl("Redefenition of variable name");

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


