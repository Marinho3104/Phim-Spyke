#include "ast_nodes.h"

#include "exception_handle.h"
#include "tokenizer.h"
#include "ast.h"

#include "token_definitions.h"
#include "tokenizer_helper.h"
#include "built_ins_helper.h"
#include "ast_definitions.h"
#include "linked_List.h"
#include "ast_helper.h"
#include "token.h"

#include <iostream>


parser::Ast_Node::~Ast_Node() {}

parser::Ast_Node::Ast_Node(Ast_Node_Variable_Declaration* __representive_declaration, int __node_type) 
    : representive_declaration(__representive_declaration), node_type(__node_type) {}



parser::Ast_Node_Name_Space::~Ast_Node_Name_Space() { delete declarations; }

parser::Ast_Node_Name_Space::Ast_Node_Name_Space() : Ast_Node(NULL, AST_NODE_NAME_SPACE), struct_name_space(0)
    { declarations = new utils::Linked_List <Ast_Node*>(); }

parser::Ast_Node_Name_Space::Ast_Node_Name_Space(Ast_Node_Variable_Declaration* __variable_declaration) 
    : Ast_Node(__variable_declaration, AST_NODE_NAME_SPACE), struct_name_space(0) { declarations = new utils::Linked_List <Ast_Node*>(); }

parser::Ast_Node_Name_Space* parser::Ast_Node_Name_Space::generate(Name_Space* __name_space) {

    parser::ast_control->print("Ast Node Name Space\n", AST_DEBUG_MODE_INC);

    parser::Ast_Node_Name_Space* _node_name_space;

    if (__name_space->name_space_node) _node_name_space = __name_space->name_space_node;

    else {

        _node_name_space = (parser::Ast_Node_Name_Space*) malloc(sizeof(parser::Ast_Node_Name_Space));

        new (_node_name_space) parser::Ast_Node_Name_Space();

        __name_space->name_space_node = _node_name_space;

    }

    parser::ast_control->addToChain(__name_space, NULL);

    _node_name_space->setDeclarations();

    parser::ast_control->popFromChain();    

    parser::ast_control->print("Ast Node Name Space\n", AST_DEBUG_MODE_DEC);

    return _node_name_space;

}

parser::Ast_Node_Name_Space* parser::Ast_Node_Name_Space::generate() {

    parser::ast_control->current_position++; // namespace

    utils::Linked_List <char*>* _scope = getScope();

    _scope->join(
        getCurrentScope(), 0
    );

    parser::Name_Space* _name_space = parser::ast_control->name_space_control->getNameSpaceOrAdd(_scope);

    delete _scope;

    if (parser::ast_control->getToken(0)->id != OPEN_BRACES) exception_handle->runExceptionAstControl("Expected token '{'");
    parser::ast_control->current_position++;

    return generate(_name_space);

}

void parser::Ast_Node_Name_Space::setDeclarations() {

    utils::Linked_List <Ast_Node*>* _temp;
    int _node_type;

    while(ast_control->current_position < tokenizer_control->tokens_collection->count) { 

        switch (_node_type = getNodeType())
        {
            case -1: parser::ast_control->current_position++; goto out; break;
            case AST_NODE_NAME_SPACE: declarations->add(Ast_Node_Name_Space::generate()); break;
            case AST_NODE_FUNCTION_DECLARATION: declarations->add(Ast_Node_Function_Declaration::generate(0)); break;
            case AST_NODE_STRUCT_DECLARATION: declarations->add(Ast_Node_Struct_Declaration::generate()); break;
            case AST_NODE_BYTE_CODE: declarations->add(Ast_Node_Byte_Code::generate()); break;
            case AST_NODE_VARIABLE_DECLARATION:
                
                    _temp = Ast_Node_Variable_Declaration::generate();

                    declarations->join(_temp);

                    delete _temp;

                    break;

            default: exception_handle->runExceptionAstControl("Node not supported in Name Space Node"); break;
        }

    }

out:;

}

int parser::Ast_Node_Name_Space::getDeclarationId(char* __name) {

    Name_Space* _name_space = parser::ast_control->name_space_control->getNameSpace(this);

    int _declaration_id = _name_space->declarations_tracker->getDeclarationId(__name);

    if (_declaration_id == -1) {

        _name_space = parser::ast_control->name_space_control->getPreviousNameSpace(_name_space);

        if (_name_space) return _name_space->name_space_node->getDeclarationId(__name);

    }

    return _declaration_id;

}

parser::Ast_Node_Variable_Declaration* parser::Ast_Node_Name_Space::getVariableDeclaration(int __declaration_id) {

    Name_Space* _name_space = parser::ast_control->name_space_control->getNameSpace(this);

    parser::Ast_Node_Variable_Declaration* _declaration = _name_space->declarations_tracker->getVariableDeclaration(__declaration_id);

    if (!_declaration) {

        _name_space = parser::ast_control->name_space_control->getPreviousNameSpace(_name_space);

        if (_name_space) return _name_space->name_space_node->getVariableDeclaration(__declaration_id);

    }

    return _declaration;

}

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Name_Space::getFunctionDeclaration(int __declaration_id,  utils::Linked_List <Ast_Node*>* __paramaters) {

    Name_Space* _name_space = parser::ast_control->name_space_control->getNameSpace(this);

    parser::Ast_Node_Function_Declaration* _declaration = _name_space->declarations_tracker->getFunctionDeclaration(__declaration_id, __paramaters);

    if (!_declaration && !struct_name_space) {

        _name_space = parser::ast_control->name_space_control->getPreviousNameSpace(_name_space);

        if (_name_space) return _name_space->name_space_node->getFunctionDeclaration(__declaration_id, __paramaters);

    }

    return _declaration;

}

parser::Ast_Node_Struct_Declaration* parser::Ast_Node_Name_Space::getStructDeclaration(int __declaration_id) {

    Name_Space* _name_space = parser::ast_control->name_space_control->getNameSpace(this);

    parser::Ast_Node_Struct_Declaration* _declaration = _name_space->declarations_tracker->getStructDeclaration(__declaration_id);

    if (!_declaration) {

        _name_space = parser::ast_control->name_space_control->getPreviousNameSpace(_name_space);

        if (_name_space) return _name_space->name_space_node->getStructDeclaration(__declaration_id);

    }

    return _declaration;

}



parser::Ast_Node_Code_Block::~Ast_Node_Code_Block() { delete code; delete declarations_tracker; }

parser::Ast_Node_Code_Block::Ast_Node_Code_Block() : Ast_Node(NULL, AST_NODE_CODE_BLOCK), off(0) { 

    previous_node_code_block = parser::ast_control->code_block_chain->last->object;
    name_space = parser::ast_control->name_space_chain->last->object;

    if (previous_node_code_block) off = previous_node_code_block->off;

    code = new utils::Linked_List <Ast_Node*>(); 
    
    declarations_tracker = new Declarations_Tracker(&off); 
        
}

void parser::Ast_Node_Code_Block::generate() {

    parser::ast_control->print("Ast Node Code Block Generator\n", AST_DEBUG_MODE_INC);

    parser::ast_control->code_block_chain->last->object->setCode();

    parser::ast_control->popFromChain();

    parser::ast_control->print("Ast Node Code Block Generator End\n", AST_DEBUG_MODE_DEC);

}

void parser::Ast_Node_Code_Block::setUp() {

    parser::ast_control->print("Ast Node Code Block Set Up\n", AST_DEBUG_MODE_INC);

    parser::Ast_Node_Code_Block* _node_code_block = (parser::Ast_Node_Code_Block*) malloc(sizeof(parser::Ast_Node_Code_Block));
    new (_node_code_block) parser::Ast_Node_Code_Block();

    parser::ast_control->addToChain(NULL, _node_code_block);

    parser::ast_control->print("Ast Node Code Block Set Up End\n", AST_DEBUG_MODE_DEC);

}

void parser::Ast_Node_Code_Block::setCode() {

    utils::Linked_List <Ast_Node*>* _temp;
    int _node_type;

    while(ast_control->current_position < tokenizer_control->tokens_collection->count) { 

        switch (_node_type = getNodeType())
        {
            case -1: parser::ast_control->current_position++; goto out; break;
            case AST_NODE_BYTE_CODE: code->add(Ast_Node_Byte_Code::generate()); break;
            case AST_NODE_RETURN: code->add(Ast_Node_Return::generate()); break;
            case AST_NODE_VARIABLE: case AST_NODE_VALUE: case AST_NODE_FUNCTION_CALL: case AST_NODE_POINTER_OPERATION: case AST_NODE_PARENTHESIS:
                code->add(Ast_Node_Expression::generate(_node_type));
                // std::cout << (int) parser::ast_control->getToken(0)->id << std::endl; 
                if (parser::ast_control->getToken(0)->id != END_INSTRUCTION) parser::exception_handle->runExceptionAstControl("Excpected token ';' aqui");
                parser::ast_control->current_position++;
                break;
            case AST_NODE_VARIABLE_DECLARATION:
                
                    _temp = Ast_Node_Variable_Declaration::generate();

                    code->join(_temp);

                    delete _temp;

                    break;

            default: exception_handle->runExceptionAstControl("Node not supported in Name Space Node"); break;
        }

    }

out:;
}

int parser::Ast_Node_Code_Block::getDeclarationId(char* __name) {

    int _declaration_id = declarations_tracker->getDeclarationId(__name);

    if (_declaration_id == -1) {

        if (previous_node_code_block) return previous_node_code_block->getDeclarationId(__name);

        else return name_space->name_space_node->getDeclarationId(__name);

    }

    return _declaration_id;

}

parser::Ast_Node_Variable_Declaration* parser::Ast_Node_Code_Block::getVariableDeclaration(int __declaration_id) {

    parser::Ast_Node_Variable_Declaration* _declaration = declarations_tracker->getVariableDeclaration(__declaration_id);

    if (!_declaration) {

        if (previous_node_code_block) return previous_node_code_block->getVariableDeclaration(__declaration_id);

        else return name_space->name_space_node->getVariableDeclaration(__declaration_id);

    }

    return _declaration;

}

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Code_Block::getFunctionDeclaration(int __declaration_id, utils::Linked_List <Ast_Node*>* __parameters) {

    parser::Ast_Node_Function_Declaration* _declaration = declarations_tracker->getFunctionDeclaration(__declaration_id, __parameters);

    if (!_declaration) {

        if (previous_node_code_block) return previous_node_code_block->getFunctionDeclaration(__declaration_id, __parameters);

        else return name_space->name_space_node->getFunctionDeclaration(__declaration_id, __parameters);

    }

    return _declaration;

}

parser::Ast_Node_Struct_Declaration* parser::Ast_Node_Code_Block::getStructDeclaration(int __declaration_id) {
    
    parser::Ast_Node_Struct_Declaration* _declaration = declarations_tracker->getStructDeclaration(__declaration_id);

    if (!_declaration) {

        if (previous_node_code_block) return previous_node_code_block->getStructDeclaration(__declaration_id);

        else return name_space->name_space_node->getStructDeclaration(__declaration_id);

    }

    return _declaration;

}



parser::Ast_Node_Variable_Declaration::~Ast_Node_Variable_Declaration() { delete type; }

parser::Ast_Node_Variable_Declaration::Ast_Node_Variable_Declaration(parser::Type_Information* __type_information, int __declaration_id, bool __global) 
    : Ast_Node(this, AST_NODE_VARIABLE_DECLARATION), type(__type_information), global(__global), declaration_id(__declaration_id) {}

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Variable_Declaration::generateFunctionParameter() {

    parser::ast_control->print("Function Parameter\n", AST_DEBUG_MODE_INC);

    utils::Linked_List <parser::Ast_Node*>* _nodes = new utils::Linked_List <parser::Ast_Node*>(); _nodes->destroy_content = 0;

    Type_Information* _type_information = Type_Information::generate();

    int _declaration_id = -1;

    switch (parser::ast_control->getToken(0)->id)
    {
    case IDENTIFIER: 
    
        if (getCurrentDeclarationTracker()->getDeclarationId(parser::ast_control->getToken(0)->identifier) != -1)

            exception_handle->runExceptionAstControl("Redefenition of variable name - Ast_Node_Variable_Declaration::generateFunctionParameter()");

        getCurrentDeclarationTracker()->addName(parser::ast_control->getToken(0)->identifier);
        _declaration_id = getCurrentDeclarationTracker()->getDeclarationId(parser::ast_control->getToken(0)->identifier);
        parser::ast_control->current_position++;

        break;

    case COMMA: case CLOSE_PARENTHESIS: break;
    default: exception_handle->runExceptionAstControl("Unexpected token - Ast_Node_Variable_Declaration::generateFunctionParameter()"); break;
    }

    Ast_Node_Variable_Declaration* _variable_declaration = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));

    new (_variable_declaration) Ast_Node_Variable_Declaration(
        _type_information, _declaration_id, 0
    );

    _nodes->add(
        _variable_declaration
    );

    getCurrentDeclarationTracker()->variable_declaration->add(
        _variable_declaration
    );

    switch (parser::ast_control->getToken(0)->id)
    {
    case FUNCTION_OPERATOR_EQUAL: exception_handle->runExceptionAstControl("Equal not done"); break;
    case COMMA: case CLOSE_PARENTHESIS: break;
    default: exception_handle->runExceptionAstControl("Unexpected token"); break;
    }

    char buffer[100];

    sprintf(buffer, "Declaration Id -> %d\n", _declaration_id);
    
    parser::ast_control->print(buffer, AST_DEBUG_MODE_INC);
    parser::ast_control->debug_mode_tab--;

    parser::ast_control->print("Function Parameter End\n", AST_DEBUG_MODE_DEC);

    return _nodes;

}   

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Variable_Declaration::generate() {

    utils::Linked_List <Ast_Node*>* _nodes = new utils::Linked_List <Ast_Node*>(); _nodes->destroy_content = 0;
    Ast_Node_Variable_Declaration* _variable_declaration;
    int _declaration_id;

    bool _is_static = parser::ast_control->getToken(0)->id == STATIC, _global;

    if (_is_static) parser::ast_control->current_position++;

    Type_Information* _type_information = Type_Information::generate();

    while(parser::ast_control->getToken(0)->id != END_INSTRUCTION) {

        parser::ast_control->print("Ast Node Variable Declaration\n", AST_DEBUG_MODE_INC);

        if (parser::ast_control->getToken(0)->id != IDENTIFIER) 
            parser::exception_handle->runExceptionAstControl("Expected token identifier - Ast_Node_Variable_Declaration::generate()");

        if (getCurrentDeclarationTracker()->getDeclarationId(parser::ast_control->getToken(0)->identifier) != -1)

            parser::exception_handle->runExceptionAstControl("Redefenition of name - Ast_Node_Variable_Declaration::generate()");

        if (_is_static) {

            _declaration_id = (*getCurrentNameSpace()->declarations_tracker->off)++;

            getCurrentDeclarationTracker()->addName(parser::ast_control->getToken(0)->identifier, _declaration_id);

        }

        else {

            getCurrentDeclarationTracker()->addName(parser::ast_control->getToken(0)->identifier);
            _declaration_id = getCurrentDeclarationTracker()->getDeclarationId(parser::ast_control->getToken(0)->identifier);

        }

        parser::ast_control->current_position++;

        _global = _is_static ? 1 : isGlobalDeclaration();

        _variable_declaration = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));
        new (_variable_declaration) Ast_Node_Variable_Declaration(
            _type_information, _declaration_id, _global
        );
        
        if (_is_static) {

            getCurrentNameSpace()->declarations_tracker->variable_declaration->add(_variable_declaration);
            getCurrentNameSpace()->name_space_node->declarations->add(_variable_declaration);

        }

        else {

            getCurrentDeclarationTracker()->variable_declaration->add(_variable_declaration);
            _nodes->add(_variable_declaration);

        }

        switch (parser::ast_control->getToken(0)->id)
        {
        case FUNCTION_OPERATOR_EQUAL: exception_handle->runExceptionAstControl("Equal not done yet - Ast_Node_Variable_Declaration::generate()"); break;
        case COMMA: parser::ast_control->current_position++; _type_information = parser::Type_Information::generate(_type_information); break;
        case END_INSTRUCTION: break;
        default: exception_handle->runExceptionAstControl("Unexpected token - Ast_Node_Variable_Declaration::generate() switch"); break;
        }

        char buffer[100];

        sprintf(buffer, "Declaration Id -> %d", _declaration_id);
        parser::ast_control->print(buffer, AST_DEBUG_MODE_INC);

        sprintf(buffer, "Is Static -> %s\n", _is_static ? "yes" : "no");
        parser::ast_control->print(buffer, AST_DEBUG_MODE_DEC);

        parser::ast_control->print("Ast Node Variable Declaration End\n", AST_DEBUG_MODE_DEC);

    }

    parser::ast_control->current_position++;

    return _nodes;

}

parser::Ast_Node_Variable_Declaration* parser::Ast_Node_Variable_Declaration::getCopy() {

    parser::Ast_Node_Variable_Declaration* _variable_declaration_copy = 
        (parser::Ast_Node_Variable_Declaration*) malloc(sizeof(parser::Ast_Node_Variable_Declaration));

    new (_variable_declaration_copy) parser::Ast_Node_Variable_Declaration(
        type->getCopy(), declaration_id, global
    );

    return _variable_declaration_copy;

}




parser::Ast_Node_Function_Declaration::~Ast_Node_Function_Declaration() { 
    representive_declaration->~Ast_Node_Variable_Declaration(); free(representive_declaration); delete parameters;  
    if (body) body->~Ast_Node_Code_Block(); free(body);
}

parser::Ast_Node_Function_Declaration::Ast_Node_Function_Declaration(
    parser::Ast_Node_Variable_Declaration* __return_variable_declaration, utils::Linked_List <Ast_Node*>* __parameters, Ast_Node_Code_Block* __body, 
        Name_Space* __name_space, int __declaration_id) : Ast_Node(__return_variable_declaration, AST_NODE_FUNCTION_DECLARATION), 
            parameters(__parameters), body(__body), name_space(__name_space), declaration_id(__declaration_id), is_static(0), is_struct(0) {}

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Function_Declaration::generate(bool __struct_function) {

    parser::ast_control->print("Ast Node Function Declaration\n", AST_DEBUG_MODE_INC);

    Type_Information* _return_type = Type_Information::generate();

    Name_Space* _name_space = getNameSpace();

    if (ast_control->getToken(0)->id != IDENTIFIER) exception_handle->runExceptionAstControl("Expected token identifier - Ast_Node_Function_Declaration::generate()");

    if (_name_space) parser::ast_control->addToChain(_name_space, NULL);

    char* _function_declaration_name = ast_control->getToken(0)->identifier;
    parser::ast_control->current_position++;

    Ast_Node_Code_Block::setUp();

    parser::ast_control->print("--- --- --- --- ---\n", AST_DEBUG_MODE_STL);

    Ast_Node_Code_Block* _body = parser::ast_control->code_block_chain->last->object;

    utils::Linked_List <Ast_Node*>* _parameters = getParameters(__struct_function);

    parser::ast_control->print("--- --- --- --- ---\n", AST_DEBUG_MODE_STL);

    parser::ast_control->addToChain(
        parser::ast_control->code_block_chain->last->object->name_space, NULL
    );

    int _declaration_id = getCurrentDeclarationTracker()->getDeclarationId(_function_declaration_name);

    if (_declaration_id == -1) {

        getCurrentDeclarationTracker()->addName(_function_declaration_name);

        _declaration_id = getCurrentDeclarationTracker()->getDeclarationId(_function_declaration_name);

    }

    else {

        Ast_Node_Function_Declaration* _previous_function_declaration = 
            getCurrentDeclarationTracker()->getFunctionDeclaration(_declaration_id, _parameters);

        if (_previous_function_declaration && _previous_function_declaration->body)

            exception_handle->runExceptionAstControl("Redefenition of function - Ast_Node_Function_Declaration::generate()");

    }

    Ast_Node_Variable_Declaration* _variable_declaration = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));

    new (_variable_declaration) Ast_Node_Variable_Declaration(
        _return_type, -1, 0
    );

    parser::Ast_Node_Function_Declaration* _function_declaration = (parser::Ast_Node_Function_Declaration*) malloc(sizeof(parser::Ast_Node_Function_Declaration));
    new (_function_declaration) parser::Ast_Node_Function_Declaration(_variable_declaration, _parameters, _body, _name_space, _declaration_id);
    
    getCurrentDeclarationTracker()->function_declaration->add(_function_declaration);

    parser::ast_control->popFromChain();

    switch (parser::ast_control->getToken(0)->id)
    {
    case END_INSTRUCTION: 
        _body->~Ast_Node_Code_Block(); free(_body);
        parser::ast_control->popFromChain();
        parser::ast_control->current_position++;
        _function_declaration->body = NULL;    
        break;
    case OPEN_BRACES: parser::ast_control->current_position++; Ast_Node_Code_Block::generate(); break;
    default: exception_handle->runExceptionAstControl("Unexpected token - Ast_Node_Function_Declaration::generate() switch"); break;
    }

    if (_name_space) parser::ast_control->popFromChain();

    parser::ast_control->print("--- --- --- --- ---\n", AST_DEBUG_MODE_STL);

    char buffer[100];

    sprintf(buffer, "Declaration Id -> %d", _declaration_id);
    parser::ast_control->print(buffer, AST_DEBUG_MODE_INC);

    sprintf(buffer, "Parameters count -> %d\n", _parameters->count);
    parser::ast_control->print(buffer, AST_DEBUG_MODE_DEC);

    parser::ast_control->print("Ast Node Function Declaration End\n", AST_DEBUG_MODE_DEC);

    return _function_declaration;

}

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Function_Declaration::getParameters(bool __struct_function) {
    
    parser::ast_control->print("Ast Node Function Declaration Parameters\n", AST_DEBUG_MODE_INC);

    utils::Linked_List <parser::Ast_Node*>* _nodes = new utils::Linked_List <parser::Ast_Node*>(), *_temp;

    parser::ast_control->current_position++;

    if (__struct_function) {

        Ast_Node_Variable_Declaration* _struct_variable_declaration = 
            parser::ast_control->code_block_chain->last->object->name_space->name_space_node->representive_declaration;

        if (!_struct_variable_declaration) 
            exception_handle->runExceptionAstControl("Struct Variable Declaration not defined - Ast_Node_Function_Declaration::getParameters()");

        getCurrentDeclarationTracker()->addName( (char*) "this");
        int _declaration_id = getCurrentDeclarationTracker()->getDeclarationId((char*) "this");

        Ast_Node_Variable_Declaration* _copy = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));

        new (_copy) Ast_Node_Variable_Declaration(
            _struct_variable_declaration->representive_declaration->type->getCopy(), _declaration_id, 0
        );
        _copy->type->pointer_level++;

        _nodes->add(
            _copy
        );

        getCurrentDeclarationTracker()->variable_declaration->add(
            _copy
        );

    }

    while(parser::ast_control->getToken(0)->id != CLOSE_PARENTHESIS) {

        _temp = Ast_Node_Variable_Declaration::generateFunctionParameter();

        _nodes->join(
            _temp
        );

        delete _temp;

        switch (ast_control->getToken(0)->id)
        {
        case COMMA: ast_control->current_position++; break;
        case CLOSE_PARENTHESIS: break;
        default: exception_handle->runExceptionAstControl("Unexpected token");
        }

    }

    parser::ast_control->current_position++;

    parser::ast_control->print("Ast Node Function Declaration Parameters End\n", AST_DEBUG_MODE_DEC);

    return _nodes;

}



parser::Ast_Node_Struct_Declaration::~Ast_Node_Struct_Declaration() {
    if (functions) functions->~Ast_Node_Name_Space(); free(functions);
    if (fields) fields->~Ast_Node_Code_Block(); free(fields);
    delete representive_declaration;
}

parser::Ast_Node_Struct_Declaration::Ast_Node_Struct_Declaration(Ast_Node_Name_Space* __node_name_space, Ast_Node_Code_Block* __node_code_block, int __declaration_id) 
    : Ast_Node(NULL, AST_NODE_STRUCT_DECLARATION), functions(__node_name_space), fields(__node_code_block), declaration_id(__declaration_id) {}

parser::Ast_Node_Struct_Declaration* parser::Ast_Node_Struct_Declaration::generate() {

    parser::ast_control->print("Ast Node Struct Declaration\n", AST_DEBUG_MODE_INC);
    
    parser::ast_control->current_position++;

    int _declaration_id;
    char* _struct_name = NULL;

    switch (parser::ast_control->getToken(0)->id)
    {
    case IDENTIFIER: 
    
        _struct_name = parser::ast_control->getToken(0)->identifier;
        parser::ast_control->current_position++;

        _declaration_id = getCurrentDeclarationTracker()->getDeclarationId(_struct_name);

        if (_declaration_id != -1) {

            parser::Ast_Node_Struct_Declaration* _struct_declaration = getCurrentDeclarationTracker()->getStructDeclaration(_declaration_id);

            if (_struct_declaration && _struct_declaration->functions && _struct_declaration->fields)

                exception_handle->runExceptionAstControl("Redefenition of struct declaration");

        }

        else {

            getCurrentDeclarationTracker()->addName(_struct_name);
            _declaration_id = getCurrentDeclarationTracker()->getDeclarationId(_struct_name);

        }

    
        break;

    case OPEN_BRACES: parser::exception_handle->runExceptionAstControl("Struct Variable declaration not done yet"); break;
    default: parser::exception_handle->runExceptionAstControl("Unexpected token"); break;
    }

    Ast_Node_Name_Space* _functions = (Ast_Node_Name_Space*) malloc(sizeof(Ast_Node_Name_Space));
    Ast_Node_Code_Block* _fields = (Ast_Node_Code_Block*) malloc(sizeof(Ast_Node_Code_Block));

    Ast_Node_Struct_Declaration* _node_struct_declaration = (Ast_Node_Struct_Declaration*) malloc(sizeof(Ast_Node_Struct_Declaration));
    new (_node_struct_declaration) Ast_Node_Struct_Declaration(
        _functions, _fields, _declaration_id
    );

    getCurrentDeclarationTracker()->struct_declaration->add(
        _node_struct_declaration
    );

    Type_Information* _type_information = new Type_Information(
        _node_struct_declaration, NULL
    );

    Ast_Node_Variable_Declaration* _struct_variable_declaration = new Ast_Node_Variable_Declaration(
        _type_information, -1, 0
    );
    _node_struct_declaration->representive_declaration = _struct_variable_declaration;

    new (_functions) Ast_Node_Name_Space(
        _struct_variable_declaration
    );
    _functions->struct_name_space = 1;

    new (_fields) Ast_Node_Code_Block();

    utils::Linked_List <char*>* _scope_name_space = new utils::Linked_List <char*>(); _scope_name_space->destroy_content = 0;

    _scope_name_space->join(
        getCurrentScope()
    );

    if (_struct_name) // TODO for not struct name
        _scope_name_space->add(
            _struct_name
        );

    Name_Space* _name_space = (Name_Space*) malloc(sizeof(Name_Space));
    new (_name_space) Name_Space(
        _scope_name_space, _functions, getCurrentDeclarationTracker()->off
    );

    _fields->name_space = _name_space;

    delete _scope_name_space;

    parser::ast_control->name_space_control->name_space_collection->add(
        _name_space
    );

    switch (parser::ast_control->getToken(0)->id)
    {
    case OPEN_BRACES: 

        parser::ast_control->addToChain(_name_space, NULL);
        parser::ast_control->addToChain(NULL, _fields);
        
        _node_struct_declaration->set(); 
        
        break;

    // case END_INSTRUCTION: goto out; break;
    default: exception_handle->runExceptionAstControl("Unexpected token"); break;
    }

    switch (parser::ast_control->getToken(0)->id)
    {
    case IDENTIFIER: exception_handle->runExceptionAstControl("Variable declaration in struct not done"); break;
    case END_INSTRUCTION: parser::ast_control->current_position++; break;
    default: exception_handle->runExceptionAstControl("Unexpected token"); break;
    }
    
    parser::ast_control->print("Ast Node Struct Declaration End\n", AST_DEBUG_MODE_DEC);

    return _node_struct_declaration;

}

void parser::Ast_Node_Struct_Declaration::set() {

    int _backup_state = ++parser::ast_control->current_position;

    setFields();

    parser::ast_control->popFromChain();

    parser::ast_control->current_position = _backup_state;

    setFunctions();

    parser::ast_control->popFromChain();

    parser::ast_control->current_position++;

}

void parser::Ast_Node_Struct_Declaration::setFunctions() {

    parser::ast_control->print("Ast Node Struct Set Functions\n", AST_DEBUG_MODE_INC);
    bool _is_static;

    while(parser::ast_control->getToken(0)->id != CLOSE_BRACES) {

        switch (getNodeType())
        {
        case AST_NODE_VARIABLE_DECLARATION:

            while(parser::ast_control->getToken(0)->id != END_INSTRUCTION) parser::ast_control->current_position++;

            parser::ast_control->current_position++;

            break;

        case AST_NODE_FUNCTION_DECLARATION:

            _is_static = parser::ast_control->getToken(0)->id == STATIC;

            if (_is_static) parser::ast_control->current_position++;
        
            functions->declarations->add(
                Ast_Node_Function_Declaration::generate(
                    !_is_static
                )
            );

            ((Ast_Node_Function_Declaration*)functions->declarations->last->object)->is_static = _is_static;
            ((Ast_Node_Function_Declaration*)functions->declarations->last->object)->is_struct = 1;

            break;

        default: exception_handle->runExceptionAstControl("Unexpected Node"); break;
        }

    }

    parser::ast_control->print("Ast Node Struct Set Functions End\n", AST_DEBUG_MODE_DEC);

}

void parser::Ast_Node_Struct_Declaration::setFields() {

    parser::ast_control->print("Ast Node Struct Set Fields\n", AST_DEBUG_MODE_INC);

    utils::Linked_List <Ast_Node*>* _temp;

    while(parser::ast_control->getToken(0)->id != CLOSE_BRACES) {

        switch (getNodeType())
        {
        case AST_NODE_VARIABLE_DECLARATION:
            
            _temp = Ast_Node_Variable_Declaration::generate();

            fields->code->join(
                _temp
            );

            delete _temp;

            break;
        case AST_NODE_FUNCTION_DECLARATION:

            while(parser::ast_control->getToken(0)->id != CLOSE_PARENTHESIS) parser::ast_control->current_position++;

            parser::ast_control->current_position++;

            ignoreCodeBlock();

            break;
        
        default: exception_handle->runExceptionAstControl("Unexpected Node"); break;
        }

    }

    parser::ast_control->print("Ast Node Struct Set Fields End\n", AST_DEBUG_MODE_DEC);

}

int parser::Ast_Node_Struct_Declaration::getSize() {

    int _size = built_ins::getPrimitiveTypeSize(
        parser::ast_control->name_space_control->getNameSpace(
            functions
        )->scope->last->object
    );

    if (_size != -1) return _size;

    _size = 0;

    for (int _ = 0; _ < fields->code->count; _++)

        if (fields->code->operator[](_)->node_type == AST_NODE_VARIABLE_DECLARATION)

            _size += fields->code->operator[](_)->representive_declaration->type->getSize();

    return _size;

}

int parser::Ast_Node_Struct_Declaration::getVariablesOff(Ast_Node_Variable* __variable_declaration) {

    int _off = 0;

    for(int _  = 0; _ < fields->code->count; _++) {

        if (fields->code->operator[](_)->node_type != AST_NODE_VARIABLE_DECLARATION) continue;

        if (fields->code->operator[](_) == __variable_declaration->representive_declaration) return _off;

        _off += fields->code->operator[](_)->representive_declaration->type->getSize();

    }

    exception_handle->runException("Struct dont have given variable declaration");

    return -1;

}




parser::Ast_Node_Expression::~Ast_Node_Expression() {
    if (expression) expression->~Ast_Node_Expression(); free(expression);
    if (value && destroy_value) { 
        value->~Ast_Node(); 
        free(value); }
    delete organized_set;
}

parser::Ast_Node_Expression::Ast_Node_Expression(Ast_Node_Expression* __expression, Ast_Node* __value, int __token_id)
    : Ast_Node(__value->representive_declaration, AST_NODE_EXPRESSION), expression(__expression), value(__value), token_id(__token_id), 
        destroy_value(1) { organized_set = new utils::Linked_List <Ast_Node*>(); organized_set->destroy_content = 0;     }

parser::Ast_Node_Variable_Declaration* parser::Ast_Node_Expression::getResultDeclaration() {

    utils::Linked_List <Expression_Result_Helper*>* _expressions_result_helper = new utils::Linked_List <Expression_Result_Helper*>();
    Ast_Node_Expression* _expression = this;
    Expression_Result_Helper* _temp;

    while(_expression) {

        _temp = (Expression_Result_Helper*) malloc(sizeof(Expression_Result_Helper)); 

        new (_temp) Expression_Result_Helper(
            _expression
        );

        _expressions_result_helper->add(
            _temp
        );

        _expression = _expression->expression;

    }

    utils::Linked_List <Ast_Node*>* _parameters = new utils::Linked_List <Ast_Node*>(); _parameters->destroy_content = 0;
    Ast_Node_Variable_Declaration* _first_argument, *_second_argument;
    Ast_Node_Function_Declaration* _function_declaration;
    Ast_Node_Name_Space* _struct_node_name_space;
    int _token_id, _declaration_id;
    char* _function_name;

    int _current_priority = 1;

    while (_expressions_result_helper->count != 1) {

        for (int _ = 0; _ < _expressions_result_helper->count; _++) { // Wrong TODO

            if (
                getOperatorPriority(_expressions_result_helper->operator[](_)->token_id) == _current_priority
            ) {

                _second_argument = _expressions_result_helper->operator[](_ + 1)->declaration->getCopy();
                _first_argument = _expressions_result_helper->operator[](_)->declaration->getCopy();
                _token_id = _expressions_result_helper->operator[](_)->token_id;

                parser::ast_control->addToChain(
                    parser::ast_control->name_space_control->getNameSpace(
                        _first_argument->type->declaration->functions
                    ),
                    NULL
                );

                _function_name = built_ins::getFunctionNameFromTokenId(_token_id);
                _declaration_id = getDeclarationId(_function_name);
                free(_function_name);

                _first_argument->type->pointer_level++;

                _parameters->add(_first_argument);
                _parameters->add(_second_argument);

                _function_declaration = getFunctionDeclaration(_declaration_id, _parameters);

                _parameters->clean();

                if (!_function_declaration || _declaration_id == -1) 

                    exception_handle->runExceptionAstControl("No function declaration with given name and given arguments");

                _first_argument->type->pointer_level--;

                if (!_expressions_result_helper->operator[](_)->function_result_value) 

                    this->organized_set->add(
                        _expressions_result_helper->operator[](_)->expression->value
                    );

                if (!_expressions_result_helper->operator[](_ + 1)->function_result_value)

                    this->organized_set->add(
                        _expressions_result_helper->operator[](_ + 1)->expression->value
                    );    
                

                this->organized_set->add(
                    _function_declaration
                );

                _expressions_result_helper->operator[](_)->token_id = _expressions_result_helper->operator[](_ + 1)->token_id;
                _expressions_result_helper->operator[](_)->declaration = _function_declaration->representive_declaration;
                _expressions_result_helper->operator[](_)->function_result_value = 1;

                delete _expressions_result_helper->remove(_ + 1);
    
                parser::ast_control->popFromChain();

                if (_expressions_result_helper->count != 1) _--;

                _first_argument->~Ast_Node_Variable_Declaration(); free(_first_argument);
                _second_argument->~Ast_Node_Variable_Declaration(); free(_second_argument);

            }

            else {

                if (!_expressions_result_helper->operator[](_)->function_result_value) 

                    this->organized_set->add(
                        _expressions_result_helper->operator[](_)->expression->value
                    );

                _expressions_result_helper->operator[](_)->function_result_value = 1;

            }

        }

        _current_priority++;    

    }

    if (!this->organized_set->count)

        this->organized_set->add(
            _expressions_result_helper->operator[](0)->expression->value
        );

    Ast_Node_Variable_Declaration* _result = _expressions_result_helper->operator[](0)->declaration;

    delete _expressions_result_helper;

    delete _parameters;

    return _result;

}

parser::Ast_Node_Expression* parser::Ast_Node_Expression::generate(int __value_node_type) {

    parser::ast_control->print("Ast Node Expression\n", AST_DEBUG_MODE_INC);

    Ast_Node* _value = getValue(__value_node_type);

    std::cout << (int) parser::ast_control->getToken(0)->id << std::endl;

second_operator_check:
    int _token_id = isFunctionOperator(parser::ast_control->getToken(0)->id) ? parser::ast_control->getToken(0)->id : -1;

    if (_token_id != -1) parser::ast_control->current_position++;

    else 

        switch (getNodeType())
        {
        case AST_NODE_ACCESSING: _value = Ast_Node_Accessing::generate(_value); goto second_operator_check; break;
        default: break;
        }

    parser::Ast_Node_Expression* _expression_node = (parser::Ast_Node_Expression*) malloc(sizeof(parser::Ast_Node_Expression));
    new (_expression_node) parser::Ast_Node_Expression(
        _token_id != -1 ? Ast_Node_Expression::generate(getNodeType()) : NULL,
        _value,
        _token_id
    );

    parser::ast_control->print("Ast Node Expression End\n", AST_DEBUG_MODE_DEC);

    return _expression_node;

}

parser::Ast_Node* parser::Ast_Node_Expression::getValue(int __value_node_type) {

    switch (__value_node_type)
    {
    case AST_NODE_VARIABLE: return Ast_Node_Variable::generate(); break;
    case AST_NODE_VALUE: return Ast_Node_Value::generate(); break;
    case AST_NODE_FUNCTION_CALL: return Ast_Node_Function_Call::generate(NULL, NULL); break;
    case AST_NODE_POINTER_OPERATION: return Ast_Node_Pointer_Operation::generate(); break;
    case AST_NODE_PARENTHESIS: return Ast_Node_Parenthesis::generate(); break;
    default: break;
    }

    exception_handle->runExceptionAstControl("Invalid node type to expression");

    return NULL;

}



parser::Ast_Node_Variable::~Ast_Node_Variable() {}

parser::Ast_Node_Variable::Ast_Node_Variable(Ast_Node_Variable_Declaration* __declaration, int __declaration_id) 
    : Ast_Node(__declaration, AST_NODE_VARIABLE), declaration_id(__declaration_id) {}

parser::Ast_Node_Variable* parser::Ast_Node_Variable::generate() {

    parser::ast_control->print("Ast Node Variable\n", AST_DEBUG_MODE_INC);

    Name_Space* _name_space = getNameSpace();

    if (_name_space) parser::ast_control->addToChain(_name_space, NULL);

    if (ast_control->getToken(0)->id != IDENTIFIER) exception_handle->runExceptionAstControl("Expected a identifier - Ast_Node_Variable::generate()");

    int _declaration_id = getDeclarationId(parser::ast_control->getToken(0)->identifier);

    parser::ast_control->current_position++;

    Ast_Node_Variable_Declaration* _variable_declaration = getVariableDeclaration(_declaration_id);

    if (_declaration_id == -1 || !_variable_declaration) 

        exception_handle->runExceptionAstControl("Undefined variable with given name - Ast_Node_Variable::generate()");

    Ast_Node_Variable* _node_variable = (Ast_Node_Variable*) malloc(sizeof(Ast_Node_Variable));

    new (_node_variable) Ast_Node_Variable(
        _variable_declaration, _declaration_id
    ); 

    if (_name_space) parser::ast_control->popFromChain();

    char buffer[100];

    sprintf(buffer, "Declaration id -> %d\n", _declaration_id);
    parser::ast_control->print(buffer, AST_DEBUG_MODE_INC);

    sprintf(buffer, "Declaration is %s\n", _variable_declaration->global ? "global" : "local");
    parser::ast_control->print(buffer, AST_DEBUG_MODE_DEC);

    parser::ast_control->print("Ast Node Variable End\n", AST_DEBUG_MODE_DEC);

    return _node_variable;

}



parser::Ast_Node_Value::~Ast_Node_Value() { representive_declaration->~Ast_Node_Variable_Declaration(); free(representive_declaration); }

parser::Ast_Node_Value::Ast_Node_Value(Ast_Node_Variable_Declaration* __declaration, int __implicit_value_position) 
    : Ast_Node(__declaration, AST_NODE_VALUE), implicit_value_position(__implicit_value_position) {}

parser::Ast_Node_Value* parser::Ast_Node_Value::generate() {

    parser::ast_control->print("Ast Node Value\n", AST_DEBUG_MODE_INC);

    Ast_Node_Variable_Declaration* _node_variable_declaration = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));

    new (_node_variable_declaration) Ast_Node_Variable_Declaration(
        Type_Information::generate(parser::ast_control->getToken(0)->id),
        -1, 0
    );

    parser::Ast_Node_Value* _value_node = (parser::Ast_Node_Value*) malloc(sizeof(parser::Ast_Node_Value));

    std::cout << "Node value -> " << parser::ast_control->getToken(0)->identifier << std::endl;

    new (_value_node) parser::Ast_Node_Value(
        _node_variable_declaration,
        parser::ast_control->addImplicitValue(
            parser::ast_control->getToken(0)->identifier
        )
    );
    parser::ast_control->current_position++;

    parser::ast_control->print("Ast Node Value End\n", AST_DEBUG_MODE_DEC);

    return _value_node;

}



parser::Ast_Node_Pointer_Operation::~Ast_Node_Pointer_Operation() { 
    representive_declaration->~Ast_Node_Variable_Declaration(); free(representive_declaration); 
    if (destroy_value) { value->~Ast_Node(); free(value); }    
}

parser::Ast_Node_Pointer_Operation::Ast_Node_Pointer_Operation(Ast_Node_Variable_Declaration* __declaration, int __pointer_level, Ast_Node* __value)
    : Ast_Node(__declaration, AST_NODE_POINTER_OPERATION), destroy_value(1), pointer_level(__pointer_level), value(__value) {}

parser::Ast_Node_Pointer_Operation* parser::Ast_Node_Pointer_Operation::generate() {

    parser::ast_control->print("Ast Node Pointer Operation\n", AST_DEBUG_MODE_INC);

    int _pointer_level = 0;
    Ast_Node* _value;

    while(parser::ast_control->getToken(0)->id == POINTER || parser::ast_control->getToken(0)->id == ADDRESS) 
        { _pointer_level += parser::ast_control->getToken(0)->id == POINTER ? -1 : 1; parser::ast_control->current_position++; }

    switch (getNodeType())
    {
    case AST_NODE_VARIABLE: _value = Ast_Node_Variable::generate(); break;
    case AST_NODE_VALUE: _value = Ast_Node_Value::generate(); break;
    case AST_NODE_PARENTHESIS: _value = Ast_Node_Parenthesis::generate(); break;
    case AST_NODE_FUNCTION_CALL: _value = Ast_Node_Function_Call::generate(NULL, NULL); break;
    default: exception_handle->runExceptionAstControl("Error getting Pointer operator value - Ast_Node_Pointer_Operation::Ast_Node_Pointer_Operation() switch"); break;
    }

    Type_Information* _type_information = _value->representive_declaration->type->getCopy();
    _type_information->pointer_level += _pointer_level;

    if (_type_information->pointer_level < 0) exception_handle->runExceptionAstControl("Pointer level below 0 - Ast_Node_Pointer_Operation::Ast_Node_Pointer_Operation()");

    Ast_Node_Variable_Declaration* _node_variable_declaration = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));

    new (_node_variable_declaration) Ast_Node_Variable_Declaration(
        _type_information,
        -1, 0
    );

    Ast_Node_Pointer_Operation* _node_pointer_operation = (Ast_Node_Pointer_Operation*) malloc(sizeof(Ast_Node_Pointer_Operation));

    new (_node_pointer_operation) Ast_Node_Pointer_Operation(
        _node_variable_declaration, _pointer_level, _value
    );

    parser::ast_control->print("Ast Node Pointer Operation End\n", AST_DEBUG_MODE_DEC);

    return _node_pointer_operation;

}



parser::Ast_Node_Parenthesis::~Ast_Node_Parenthesis() { if (expression) expression->~Ast_Node_Expression(); free(expression); } 

parser::Ast_Node_Parenthesis::Ast_Node_Parenthesis(Ast_Node_Expression* __expression) 
    : Ast_Node(__expression->getResultDeclaration(), AST_NODE_PARENTHESIS), expression(__expression) {}

parser::Ast_Node_Parenthesis* parser::Ast_Node_Parenthesis::generate() {

    parser::ast_control->print("Ast Node Parenthesis\n", AST_DEBUG_MODE_INC);

    parser::ast_control->current_position++;

    Ast_Node_Parenthesis* _node_parenthesis = (Ast_Node_Parenthesis*) malloc(sizeof(Ast_Node_Parenthesis));

    new (_node_parenthesis) Ast_Node_Parenthesis(
        Ast_Node_Expression::generate(
            getNodeType()
        )
    );

    parser::ast_control->current_position++;

    parser::ast_control->print("Ast Node Parenthesis End\n", AST_DEBUG_MODE_DEC);

    return _node_parenthesis;

}


parser::Ast_Node_Function_Call::~Ast_Node_Function_Call() { delete parameters; }

parser::Ast_Node_Function_Call::Ast_Node_Function_Call(utils::Linked_List <Ast_Node_Expression*>* __parameters, Ast_Node_Function_Declaration* __function_declaration)
    : Ast_Node(__function_declaration->representive_declaration, AST_NODE_FUNCTION_CALL), parameters(__parameters), function_declaration(__function_declaration) {}

parser::Ast_Node_Function_Call* parser::Ast_Node_Function_Call::generate(Ast_Node_Expression* __expression, Name_Space* __function_call_name_space) {

    parser::ast_control->print("Ast Node Function Call\n", AST_DEBUG_MODE_INC);

    Name_Space* _name_space = NULL; 

    Ast_Node_Code_Block* _parameters_node_code_block = parser::ast_control->code_block_chain->last->object;
    
    if (!__function_call_name_space) _name_space = getNameSpace();
    else _name_space = __function_call_name_space;

    if (_name_space) parser::ast_control->addToChain(_name_space, NULL);

    if (parser::ast_control->getToken(0)->id != IDENTIFIER) exception_handle->runExceptionAstControl("Expected token identifier - Ast_Node_Function_Call::generate()");

    int _declaration_id = getDeclarationId(parser::ast_control->getToken(0)->identifier);
    parser::ast_control->current_position++;

    if (_parameters_node_code_block) 

        parser::ast_control->addToChain(
            NULL,
            _parameters_node_code_block
        );
    
    utils::Linked_List <Ast_Node_Expression*>* _parameters = getParameters(__expression);

    if (_parameters_node_code_block) parser::ast_control->popFromChain();
        
    utils::Linked_List <Ast_Node*>* _parameters_results = getResultParameters(_parameters);

    Ast_Node_Function_Declaration* _function_declaration = getFunctionDeclaration(_declaration_id, _parameters_results);

    if (_declaration_id == -1 || !_function_declaration)
     
        exception_handle->runExceptionAstControl("No declaration of function with given name and parameters");

    parser::Ast_Node_Function_Call* _node_function_call = (Ast_Node_Function_Call*) malloc(sizeof(Ast_Node_Function_Call));

    new (_node_function_call) parser::Ast_Node_Function_Call(
        _parameters, _function_declaration
    );

    if (_name_space) parser::ast_control->popFromChain();

    delete _parameters_results;

    parser::ast_control->print("Ast Node Function Call End\n", AST_DEBUG_MODE_DEC);

    return _node_function_call;

}

utils::Linked_List <parser::Ast_Node_Expression*>* parser::Ast_Node_Function_Call::getParameters(Ast_Node_Expression* __expression) {

    utils::Linked_List <parser::Ast_Node_Expression*>* _parameters = new utils::Linked_List <parser::Ast_Node_Expression*>();

    parser::ast_control->current_position++;

    if (__expression) _parameters->add(__expression);

    while(parser::ast_control->getToken(0)->id != CLOSE_PARENTHESIS) {

        _parameters->add(
            Ast_Node_Expression::generate(
                getNodeType()
            )
        );

        switch (parser::ast_control->getToken(0)->id)
        {
        case COMMA: parser::ast_control->current_position++; break;
        case CLOSE_PARENTHESIS: break;
        default: exception_handle->runExceptionAstControl("Unexpected token - Ast_Node_Function_Call::getParameters()"); break;
        }

    }

    parser::ast_control->current_position++;

    return _parameters;

}

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Function_Call::getResultParameters(utils::Linked_List <Ast_Node_Expression*>* __parameters) {

    utils::Linked_List <parser::Ast_Node*>* _result_parameters = new utils::Linked_List <parser::Ast_Node*>(); _result_parameters->destroy_content = 0;

    for (int _ = 0; _ < __parameters->count; _++)

        _result_parameters->add(
            __parameters->operator[](_)->getResultDeclaration()
        );

    return _result_parameters;

}


parser::Ast_Node_Accessing::~Ast_Node_Accessing() {
    value->~Ast_Node(); free(value);
    if (next) next->~Ast_Node_Accessing(); 
    else { accessing->~Ast_Node(); free(accessing); } 
    free(next); 
}

parser::Ast_Node_Accessing::Ast_Node_Accessing(Ast_Node* __value, Ast_Node* __accessing, Ast_Node_Accessing* __next) 
    : Ast_Node(__value->representive_declaration, AST_NODE_ACCESSING), value(__value), accessing(__accessing), next(__next) {}

parser::Ast_Node_Accessing* parser::Ast_Node_Accessing::generate(Ast_Node* __value) {

    parser::ast_control->print("Ast Node Accessing\n", AST_DEBUG_MODE_INC);

    int _value_pointer_level = __value->representive_declaration->type->pointer_level;

    int _token_id = parser::ast_control->getToken(0)->id;
    parser::ast_control->current_position++;

    if (
        _value_pointer_level && _token_id != ACCESSING_POINTER ||
        !_value_pointer_level && _token_id != ACCESSING
    ) exception_handle->runExceptionAstControl("Unmatching accessing - Ast_Node_Accessing::generate()");

    if (parser::ast_control->getToken(0)->id != IDENTIFIER) 

        exception_handle->runExceptionAstControl("Expected token identifier");
    
    Ast_Node* _accessing, *_value_before_accessing; 
    Ast_Node_Expression* _expression;

    switch (getNodeType())
    {
    case AST_NODE_VARIABLE: 
    
        parser::ast_control->addToChain(NULL, __value->representive_declaration->type->declaration->fields);

        _accessing = Ast_Node_Variable::generate();

        parser::ast_control->popFromChain();

        break;

    case AST_NODE_FUNCTION_CALL: 

        _value_before_accessing = __value;

        if (!_value_pointer_level) {

            Ast_Node_Variable_Declaration* _node_variable_declaration = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));

            new (_node_variable_declaration) Ast_Node_Variable_Declaration(
                _value_before_accessing->representive_declaration->type->getCopy(),
                -1, 0
            );
            _node_variable_declaration->type->pointer_level++;

            Ast_Node_Pointer_Operation* _node_pointer_operation = (Ast_Node_Pointer_Operation*) malloc(sizeof(Ast_Node_Pointer_Operation));

            new (_node_pointer_operation) Ast_Node_Pointer_Operation(
                _node_variable_declaration, 1, _value_before_accessing
            );
            _node_pointer_operation->destroy_value = 0;

            _value_before_accessing = _node_pointer_operation;

        }

        _expression = (Ast_Node_Expression*) malloc(sizeof(Ast_Node_Expression));

        new (_expression) Ast_Node_Expression(
            NULL, _value_before_accessing, -1
        );

        if (_value_pointer_level) _expression->destroy_value = 0;

        _accessing = Ast_Node_Function_Call::generate(
            _expression, 
            parser::ast_control->name_space_control->getNameSpace(_value_before_accessing->representive_declaration->type->declaration->functions)
        );

        break;

    default: break;
    }

    parser::Ast_Node_Accessing* _node_accessing = (Ast_Node_Accessing*) malloc(sizeof(Ast_Node_Accessing));

    new (_node_accessing) parser::Ast_Node_Accessing(
        __value, _accessing, 
        isAccessingOperator(parser::ast_control->getToken(0)->id) ? Ast_Node_Accessing::generate(_accessing) : NULL
    );

    parser::ast_control->print("Ast Node Accessing End\n", AST_DEBUG_MODE_DEC);

    return _node_accessing;

}


parser::Ast_Node_Byte_Code::~Ast_Node_Byte_Code() {

}

parser::Ast_Node_Byte_Code::Ast_Node_Byte_Code(char __code, int __argument) 
    : Ast_Node(NULL, AST_NODE_BYTE_CODE), code(__code), argument(__argument) {}

parser::Ast_Node_Byte_Code* parser::Ast_Node_Byte_Code::generate() {

    parser::ast_control->print("Ast Node Byte Code\n", AST_DEBUG_MODE_INC);

    parser::ast_control->current_position++;

    char _code = atoi(parser::ast_control->getToken(0)->identifier);
    parser::ast_control->current_position++;

    int _argument = atoi(parser::ast_control->getToken(0)->identifier);

    parser::ast_control->current_position++;

    parser::ast_control->current_position++;

    parser::Ast_Node_Byte_Code* _node_byte_code = (parser::Ast_Node_Byte_Code*) malloc(sizeof(parser::Ast_Node_Byte_Code));

    std::cout << "Code -> " << (int) _code << std::endl;
    std::cout << "Argument -> " << _argument << std::endl;

    new (_node_byte_code) parser::Ast_Node_Byte_Code(
        _code, _argument
    );

    parser::ast_control->print("Ast Node Byte Code\n", AST_DEBUG_MODE_DEC);

    return _node_byte_code;

}



parser::Ast_Node_Return::~Ast_Node_Return() { expression->~Ast_Node_Expression(); free(expression); }

parser::Ast_Node_Return::Ast_Node_Return(Ast_Node_Expression* __expression) : Ast_Node(__expression->getResultDeclaration(), AST_NODE_RETURN), expression(__expression) {}

parser::Ast_Node_Return* parser::Ast_Node_Return::generate() {

    parser::ast_control->print("Ast Node Return\n", AST_DEBUG_MODE_INC);

    parser::ast_control->current_position++;

    Ast_Node_Expression* _expression = Ast_Node_Expression::generate(getNodeType());

    parser::ast_control->current_position++;

    Ast_Node_Return* _node_return = (Ast_Node_Return*) malloc(sizeof(Ast_Node_Return));

    new (_node_return) Ast_Node_Return(_expression);

    parser::ast_control->print("Ast Node Return\n", AST_DEBUG_MODE_DEC);

    return _node_return;

}



