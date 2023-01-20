#include "ast_nodes.h"

#include "exception_handle.h"
#include "tokenizer.h"
#include "ast.h"

#include "opcodes.h"
#include "token_definitions.h"
#include "tokenizer_helper.h"
#include "built_ins_helper.h"
#include "ast_definitions.h"
#include "linked_List.h"
#include "ast_helper.h"
#include "token.h"

#include <iostream>
#include <string.h>

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

    _node_name_space = (parser::Ast_Node_Name_Space*) malloc(sizeof(parser::Ast_Node_Name_Space));

    new (_node_name_space) parser::Ast_Node_Name_Space();

    __name_space->name_space_nodes->add(_node_name_space);

    utils::Linked_List <Ast_Node*>* _previous_nodes_list = ast_control->current_nodes_list;
    ast_control->current_nodes_list = _node_name_space->declarations;

    parser::ast_control->addToChain(__name_space, NULL);

    _node_name_space->setDeclarations();

    parser::ast_control->popFromChain();    

    parser::ast_control->print("Ast Node Name Space\n", AST_DEBUG_MODE_DEC);

    ast_control->current_nodes_list = _previous_nodes_list;

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
            case AST_NODE_FUNCTION_CALL: declarations->add(Ast_Node_Function_Declaration::generate()); break;
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

        if (_name_space) 
        
            for (int _ = 0; _ < _name_space->name_space_nodes->count; _++)
                
                if ((_declaration_id = _name_space->name_space_nodes->operator[](_)->getDeclarationId(__name)) != -1)
                    
                    return _declaration_id;

    }

    return _declaration_id;

}

parser::Ast_Node_Variable_Declaration* parser::Ast_Node_Name_Space::getVariableDeclaration(int __declaration_id) {

    Name_Space* _name_space = parser::ast_control->name_space_control->getNameSpace(this);

    parser::Ast_Node_Variable_Declaration* _declaration = _name_space->declarations_tracker->getVariableDeclaration(__declaration_id);

    if (!_declaration) {

        _name_space = parser::ast_control->name_space_control->getPreviousNameSpace(_name_space);

        if (_name_space) 

            for (int _ = 0; _ < _name_space->name_space_nodes->count; _++)
                
                if (
                    _declaration = 
                        _name_space->name_space_nodes->operator[](_)->getVariableDeclaration(__declaration_id)
                ) return _declaration;
    
    }

    return _declaration;

}

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Name_Space::getFunctionDeclaration(int __declaration_id,  utils::Linked_List <Ast_Node*>* __paramaters) {

    Name_Space* _name_space = parser::ast_control->name_space_control->getNameSpace(this);

    parser::Ast_Node_Function_Declaration* _declaration = _name_space->declarations_tracker->getFunctionDeclaration(__declaration_id, __paramaters);

    if (!_declaration && !struct_name_space) {

        _name_space = parser::ast_control->name_space_control->getPreviousNameSpace(_name_space);

        if (_name_space)

            for (int _ = 0; _ < _name_space->name_space_nodes->count; _++)
                
                if (
                    _declaration = 
                        _name_space->name_space_nodes->operator[](_)->getFunctionDeclaration(__declaration_id, __paramaters)
                ) return _declaration;

    }

    return _declaration;

}

parser::Ast_Node_Struct_Declaration* parser::Ast_Node_Name_Space::getStructDeclaration(int __declaration_id) {

    Name_Space* _name_space = parser::ast_control->name_space_control->getNameSpace(this);

    parser::Ast_Node_Struct_Declaration* _declaration = _name_space->declarations_tracker->getStructDeclaration(__declaration_id);

    if (!_declaration) {

        _name_space = parser::ast_control->name_space_control->getPreviousNameSpace(_name_space);

        if (_name_space) 
        
            for (int _ = 0; _ < _name_space->name_space_nodes->count; _++)
                
                if (
                    _declaration = 
                        _name_space->name_space_nodes->operator[](_)->getStructDeclaration(__declaration_id)
                ) return _declaration;


    }

    return _declaration;

}



parser::Ast_Node_Code_Block::~Ast_Node_Code_Block() { delete code; delete declarations_tracker; }

parser::Ast_Node_Code_Block::Ast_Node_Code_Block() : Ast_Node(NULL, AST_NODE_CODE_BLOCK), off(0), body_position(0) { 

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

    utils::Linked_List <Ast_Node*>* _previous_nodes_list = ast_control->current_nodes_list;
    ast_control->current_nodes_list = code;

    while(ast_control->current_position < tokenizer_control->tokens_collection->count) { 

        switch (_node_type = getNodeType())
        {
            case -1: parser::ast_control->current_position++; goto out; break;
            case AST_NODE_BYTE_CODE: code->add(Ast_Node_Byte_Code::generate()); break;
            case AST_NODE_RETURN: code->add(Ast_Node_Return::generate()); break;
            case AST_NODE_IF: code->add(Ast_Node_If::generate()); break;
            case AST_NODE_ELSE_IF: code->add(Ast_Node_Else_If::generate()); break;
            case AST_NODE_WHILE: code->add(Ast_Node_While::generate()); break;
            case AST_NODE_DO_WHILE: code->add(Ast_Node_Do_While::generate()); break;
            case AST_NODE_ELSE: code->add(Ast_Node_Else::generate()); break;
            case AST_NODE_VARIABLE: case AST_NODE_VALUE: case AST_NODE_FUNCTION_CALL: case AST_NODE_POINTER_OPERATION: case AST_NODE_PARENTHESIS:
            case AST_NODE_CAST: case AST_NODE_SIZE_OF: case -6:
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

            default: exception_handle->runExceptionAstControl("Node not supported in Code Block Node"); break;
        }

    }

out:;

    ast_control->current_nodes_list = _previous_nodes_list;
    
}

int parser::Ast_Node_Code_Block::getDeclarationId(char* __name) {

    int _declaration_id = declarations_tracker->getDeclarationId(__name);

    if (_declaration_id == -1) {

        if (previous_node_code_block) return previous_node_code_block->getDeclarationId(__name);

        else 
        
            for (int _ = 0; _ < name_space->name_space_nodes->count; _++)
                
                if (
                    _declaration_id = 
                        name_space->name_space_nodes->operator[](_)->getDeclarationId(__name)
                ) return _declaration_id;
    }

    return _declaration_id;

}

parser::Ast_Node_Variable_Declaration* parser::Ast_Node_Code_Block::getVariableDeclaration(int __declaration_id) {

    parser::Ast_Node_Variable_Declaration* _declaration = declarations_tracker->getVariableDeclaration(__declaration_id);

    if (!_declaration) {

        if (previous_node_code_block) return previous_node_code_block->getVariableDeclaration(__declaration_id);

        else 
        
            for (int _ = 0; _ < name_space->name_space_nodes->count; _++)
                
                if (
                    _declaration = 
                        name_space->name_space_nodes->operator[](_)->getVariableDeclaration(__declaration_id)
                ) return _declaration;
    }

    return _declaration;

}

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Code_Block::getFunctionDeclaration(int __declaration_id, utils::Linked_List <Ast_Node*>* __parameters) {

    parser::Ast_Node_Function_Declaration* _declaration = declarations_tracker->getFunctionDeclaration(__declaration_id, __parameters);

    if (!_declaration) {

        if (previous_node_code_block) return previous_node_code_block->getFunctionDeclaration(__declaration_id, __parameters);

        else 
        
            for (int _ = 0; _ < name_space->name_space_nodes->count; _++)
                
                if (
                    _declaration = 
                        name_space->name_space_nodes->operator[](_)->getFunctionDeclaration(__declaration_id, __parameters)
                ) return _declaration;
    }

    return _declaration;

}

parser::Ast_Node_Struct_Declaration* parser::Ast_Node_Code_Block::getStructDeclaration(int __declaration_id) {
    
    parser::Ast_Node_Struct_Declaration* _declaration = declarations_tracker->getStructDeclaration(__declaration_id);

    if (!_declaration) {

        if (previous_node_code_block) return previous_node_code_block->getStructDeclaration(__declaration_id);

        else 

            for (int _ = 0; _ < name_space->name_space_nodes->count; _++)
                
                if (
                    _declaration = 
                        name_space->name_space_nodes->operator[](_)->getStructDeclaration(__declaration_id)
                ) return _declaration;
    }

    return _declaration;

}



parser::Ast_Node_Variable_Declaration::~Ast_Node_Variable_Declaration() { delete type; if (array_length) array_length->~Ast_Node(); free(array_length); }

parser::Ast_Node_Variable_Declaration::Ast_Node_Variable_Declaration(parser::Type_Information* __type_information, int __declaration_id, bool __global) 
    : Ast_Node(this, AST_NODE_VARIABLE_DECLARATION), type(__type_information), global(__global), declaration_id(__declaration_id), array_length(0), constructor_declaration(0) {}

parser::Ast_Node_Variable_Declaration::Ast_Node_Variable_Declaration(parser::Type_Information* __type_information, Ast_Node* __array_length, int __declaration_id, bool __global) 
    : Ast_Node(this, AST_NODE_VARIABLE_DECLARATION), type(__type_information), global(__global), declaration_id(__declaration_id), array_length(__array_length), constructor_declaration(0) 
        { if (array_length) type->pointer_level++;  }



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

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Variable_Declaration::generateStructDeclarations(Type_Information* __type_information) {

    utils::Linked_List <Ast_Node*>* _nodes = new utils::Linked_List <Ast_Node*>(); _nodes->destroy_content = 0;
    Ast_Node_Variable_Declaration* _variable_declaration;
    int _declaration_id, _global;

    Type_Information* _type_information = Type_Information::generate(__type_information);

    while(parser::ast_control->getToken(0)->id != END_INSTRUCTION) {

        parser::ast_control->print("Ast Node Variable Declaration\n", AST_DEBUG_MODE_INC);

        if (parser::ast_control->getToken(0)->id != IDENTIFIER) 
            parser::exception_handle->runExceptionAstControl("Expected token identifier - Ast_Node_Variable_Declaration::generate()");

        if (getCurrentDeclarationTracker()->getDeclarationId(parser::ast_control->getToken(0)->identifier) != -1)

            parser::exception_handle->runExceptionAstControl("Redefenition of name - Ast_Node_Variable_Declaration::generate()");

        getCurrentDeclarationTracker()->addName(parser::ast_control->getToken(0)->identifier);
        _declaration_id = getCurrentDeclarationTracker()->getDeclarationId(parser::ast_control->getToken(0)->identifier);


        parser::ast_control->current_position++;

        _global = isGlobalDeclaration();

        Ast_Node* _array_length = 0;

        if (parser::ast_control->getToken(0)->id == OPEN_BRACKET) {

            parser::ast_control->current_position++;

            _array_length = Ast_Node_Expression::generate(
                getNodeType()
            );

            if (parser::ast_control->getToken(0)->id != CLOSE_BRACKET) 

                exception_handle->runExceptionAstControl("Excpected token ']'");

            ((Ast_Node_Expression*) _array_length)->getResultDeclaration();

            parser::ast_control->current_position++;

        }


        _variable_declaration = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));
        new (_variable_declaration) Ast_Node_Variable_Declaration(
            _type_information, _array_length, _declaration_id, _global
        );
        

        getCurrentDeclarationTracker()->variable_declaration->add(_variable_declaration);
        _nodes->add(_variable_declaration);


        if (parser::ast_control->getToken(0)->id == FUNCTION_OPERATOR_EQUAL) {
            
            parser::ast_control->current_position++;

            utils::Linked_List <Ast_Node*>* _nodes_equal = equal(_variable_declaration);

            _nodes->join(
                _nodes_equal
            ); 

            delete _nodes_equal;
            
            break;
        }

        else if (_type_information->haveConstructorDefined()) {

            Ast_Node_Name_Space* _functions_name_space;
            char* _struct_name;

            if (_type_information->pointer_level) {

                Type_Information* _pointer_type = Type_Information::generatePrimitiveType(PRIMITIVE_TYPE_POINTER);

                _functions_name_space = _pointer_type->declaration->functions;

                _struct_name = (char*) "Pointer";

                delete _pointer_type;

            } 
            
            else {

                _functions_name_space = _type_information->declaration->functions;

                _struct_name = _type_information->declaration->struct_name;

            }

            ast_control->addToChain(
                ast_control->name_space_control->getNameSpace(_functions_name_space), 0
            );

            int _declaration_id = getDeclarationId(_struct_name);

            utils::Linked_List <Ast_Node*>* _params = new utils::Linked_List <Ast_Node*>(); 

            Ast_Node_Variable_Declaration* _variable_declaration_node = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));

            new (_variable_declaration_node) Ast_Node_Variable_Declaration(
                _type_information->getCopy(), -1, 0
            );
            _variable_declaration_node->type->pointer_level++;

            _params->add(
                _variable_declaration_node    
            );

            Ast_Node_Function_Declaration* _function_declaration = getFunctionDeclaration(_declaration_id, _params);

            if (!_function_declaration) exception_handle->runExceptionAstControl("No Constructor with given parameters");

            _variable_declaration->constructor_declaration = _function_declaration;

            delete _params;

            ast_control->popFromChain();

        }

        switch (parser::ast_control->getToken(0)->id)
        {
        case COMMA: parser::ast_control->current_position++; _type_information = parser::Type_Information::generate(_type_information); break;
        case END_INSTRUCTION: break;
        default: exception_handle->runExceptionAstControl("Unexpected token - Ast_Node_Variable_Declaration::generate() switch"); break;
        }

        char buffer[100];

        sprintf(buffer, "Declaration Id -> %d", _declaration_id);
        parser::ast_control->print(buffer, AST_DEBUG_MODE_INC);

        sprintf(buffer, "Is Static -> %s\n", "no");
        parser::ast_control->print(buffer, AST_DEBUG_MODE_DEC);

        parser::ast_control->print("Ast Node Variable Declaration End\n", AST_DEBUG_MODE_DEC);

    }

    parser::ast_control->current_position++;

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

        Ast_Node* _array_length = 0;

        if (parser::ast_control->getToken(0)->id == OPEN_BRACKET) {

            parser::ast_control->current_position++;

            _array_length = Ast_Node_Expression::generate(
                getNodeType()
            );

            if (parser::ast_control->getToken(0)->id != CLOSE_BRACKET) 

                exception_handle->runExceptionAstControl("Excpected token ']'");

            ((Ast_Node_Expression*) _array_length)->getResultDeclaration();

            parser::ast_control->current_position++;

        }


        _variable_declaration = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));
        new (_variable_declaration) Ast_Node_Variable_Declaration(
            _type_information, _array_length, _declaration_id, _global
        );
        
        if (_is_static) {

            getCurrentNameSpace()->declarations_tracker->variable_declaration->add(_variable_declaration);
            getCurrentNameSpace()->name_space_nodes->last->object->declarations->add(_variable_declaration);

        }

        else {

            getCurrentDeclarationTracker()->variable_declaration->add(_variable_declaration);
            _nodes->add(_variable_declaration);

        }

        if (parser::ast_control->getToken(0)->id == FUNCTION_OPERATOR_EQUAL) {
            
            parser::ast_control->current_position++;

            utils::Linked_List <Ast_Node*>* _nodes_equal = equal(_variable_declaration);

            if (_is_static) 

                getCurrentNameSpace()->name_space_nodes->last->object->declarations->join(_nodes_equal);

            else 
                _nodes->join(
                    _nodes_equal
                ); 

            delete _nodes_equal;
            
            break;
        }

        else if (_type_information->haveConstructorDefined()) {

            Ast_Node_Name_Space* _functions_name_space;
            char* _struct_name;

            if (_type_information->pointer_level) {

                Type_Information* _pointer_type = Type_Information::generatePrimitiveType(PRIMITIVE_TYPE_POINTER);

                _functions_name_space = _pointer_type->declaration->functions;

                _struct_name = (char*) "Pointer";

                delete _pointer_type;

            } 
            
            else {

                _functions_name_space = _type_information->declaration->functions;

                _struct_name = _type_information->declaration->struct_name;

            }

            ast_control->addToChain(
                ast_control->name_space_control->getNameSpace(_functions_name_space), 0
            );

            int _declaration_id = getDeclarationId(_struct_name);

            utils::Linked_List <Ast_Node*>* _params = new utils::Linked_List <Ast_Node*>(); 

            Ast_Node_Variable_Declaration* _variable_declaration_node = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));

            new (_variable_declaration_node) Ast_Node_Variable_Declaration(
                _type_information->getCopy(), -1, 0
            );
            _variable_declaration_node->type->pointer_level++;

            _params->add(
                _variable_declaration_node    
            );

            Ast_Node_Function_Declaration* _function_declaration = getFunctionDeclaration(_declaration_id, _params);

            if (!_function_declaration) exception_handle->runExceptionAstControl("No Constructor with given parameters");

            _variable_declaration->constructor_declaration = _function_declaration;

            delete _params;

            ast_control->popFromChain();

        }

        switch (parser::ast_control->getToken(0)->id)
        {
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

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Variable_Declaration::equal(Ast_Node_Variable_Declaration* __variable) {

    parser::ast_control->print("Ast Node Variable Declaration -- Equal\n", AST_DEBUG_MODE_INC);

    utils::Linked_List <Ast_Node*>* _nodes = new utils::Linked_List <Ast_Node*>(); _nodes->destroy_content = 0;

    Ast_Node_Variable* _variable = (Ast_Node_Variable*) malloc(sizeof(Ast_Node_Variable));
    new (_variable) Ast_Node_Variable(
        __variable, __variable->declaration_id
    );

    _nodes->add(_variable);

    Ast_Node* _value;

    switch (int _node_type = getNodeType())
    {
        case AST_NODE_VARIABLE: case AST_NODE_VALUE: case AST_NODE_FUNCTION_CALL: case AST_NODE_POINTER_OPERATION: case AST_NODE_PARENTHESIS:
            case AST_NODE_CAST: case AST_NODE_SIZE_OF:
        
                _value = Ast_Node_Expression::generate(_node_type);

                if (((Ast_Node_Expression*) _value)->getResultDeclaration()->type->getSize() != __variable->representive_declaration->type->getSize())

                    exception_handle->runExceptionAstControl("Not same size");

                break;
    default: exception_handle->runExceptionAstControl("Error equal variable declaration"); break;
    }

    _nodes->add(
        _value
    );

    Ast_Node_Byte_Code* _byte_code = (Ast_Node_Byte_Code*) malloc(sizeof(Ast_Node_Byte_Code));
    new (_byte_code) Ast_Node_Byte_Code(
        BYTE_CODE_MEMORY_COPY,
        __variable->representive_declaration->type->getSize()
    ); 

    _nodes->add(_byte_code);

    parser::ast_control->print("Ast Node Variable Declaration -- Equal End\n", AST_DEBUG_MODE_DEC);
    
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
    if (this_variable) this_variable->~Ast_Node_Variable_Declaration(); free(this_variable);
}

parser::Ast_Node_Function_Declaration::Ast_Node_Function_Declaration(
    Ast_Node_Function_Declaration* __forward, Ast_Node_Variable_Declaration* __this_variable, parser::Ast_Node_Variable_Declaration* __return_variable_declaration, utils::Linked_List <Ast_Node*>* __parameters, Ast_Node_Code_Block* __body, 
        Name_Space* __name_space, int __declaration_id, bool _is_struct, bool _is_static) : Ast_Node(__return_variable_declaration, AST_NODE_FUNCTION_DECLARATION), 
            parameters(__parameters), this_variable(__this_variable), body(__body), name_space(__name_space), declaration_id(__declaration_id), is_static(_is_static), is_struct(_is_struct), 
                body_position(-1), forward(__forward), is_constructor(0) {}

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Function_Declaration::generate(bool __struct_function) {

    parser::ast_control->print("Ast Node Function Declaration\n", AST_DEBUG_MODE_INC); 

    bool _is_static = 0;

    if (_is_static = parser::ast_control->getToken(0)->id == STATIC) 
        parser::ast_control->current_position++;
    
    Type_Information* _return_type = Type_Information::generate();

    Name_Space* _name_space = getNameSpace();

    if (ast_control->getToken(0)->id != IDENTIFIER) exception_handle->runExceptionAstControl("Expected token identifier - Ast_Node_Function_Declaration::generate()");

    if (_name_space) parser::ast_control->addToChain(_name_space, NULL);

    __struct_function = parser::ast_control->name_space_chain->last->object->name_space_nodes->last->object->struct_name_space;

    char* _function_declaration_name = ast_control->getToken(0)->identifier;
    parser::ast_control->current_position++;

    Ast_Node_Code_Block::setUp();

    parser::ast_control->print("--- --- --- --- ---\n", AST_DEBUG_MODE_STL);

    Ast_Node_Code_Block* _body = parser::ast_control->code_block_chain->last->object;
    Ast_Node_Variable_Declaration* _this_variable = NULL;

    if (__struct_function && !_is_static) {

        getCurrentDeclarationTracker()->addName( (char*) "this");

        Ast_Node_Variable_Declaration* _struct_variable_declaration = 
            parser::ast_control->code_block_chain->last->object->name_space->name_space_nodes->last->object->representive_declaration;

        if (!_struct_variable_declaration) 
            exception_handle->runExceptionAstControl("Struct Variable Declaration not defined - Ast_Node_Function_Declaration::getParameters()");

        _struct_variable_declaration = _struct_variable_declaration->getCopy();

        _struct_variable_declaration->declaration_id = getCurrentDeclarationTracker()->getDeclarationId((char*)"this");
        _struct_variable_declaration->type->pointer_level++;

        getCurrentDeclarationTracker()->variable_declaration->add(
            _struct_variable_declaration
        );

        _this_variable = _struct_variable_declaration;

    }

    utils::Linked_List <Ast_Node*>* _parameters = getParameters();

    parser::ast_control->print("--- --- --- --- ---\n", AST_DEBUG_MODE_STL);

    parser::ast_control->addToChain(
        parser::ast_control->code_block_chain->last->object->name_space, NULL
    );

    int _declaration_id = getCurrentDeclarationTracker()->getDeclarationId(_function_declaration_name);
    Ast_Node_Function_Declaration* _previous_function_declaration = NULL;

    if (_declaration_id == -1) {

        getCurrentDeclarationTracker()->addName(_function_declaration_name);

        _declaration_id = getCurrentDeclarationTracker()->getDeclarationId(_function_declaration_name);

    }

    else {

        _parameters->insert(_this_variable, 0);

        _parameters->getDataLinkedList(0)->destroy_content = 0;

        _previous_function_declaration = 
            getCurrentDeclarationTracker()->getFunctionDeclaration(_declaration_id, _parameters);

        if (_previous_function_declaration && _previous_function_declaration->body)

            exception_handle->runExceptionAstControl("Redefenition of function - Ast_Node_Function_Declaration::generate()");
    

        delete _parameters->remove(0);

    }

    Ast_Node_Variable_Declaration* _variable_declaration = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));

    new (_variable_declaration) Ast_Node_Variable_Declaration(
        _return_type, -1, 0
    );

    parser::Ast_Node_Function_Declaration* _function_declaration = (parser::Ast_Node_Function_Declaration*) malloc(sizeof(parser::Ast_Node_Function_Declaration));
    new (_function_declaration) parser::Ast_Node_Function_Declaration(
        _previous_function_declaration, 
        _this_variable,
        _variable_declaration, 
        _parameters, 
        _body, 
        _name_space, 
        _declaration_id,
        __struct_function,
        _is_static
    ); 

    if (!_previous_function_declaration) getCurrentDeclarationTracker()->function_declaration->add(_function_declaration);

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

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Function_Declaration::getParameters() {

    parser::ast_control->print("Ast Node Function Declaration Parameters\n", AST_DEBUG_MODE_INC);

    utils::Linked_List <parser::Ast_Node*>* _nodes = new utils::Linked_List <parser::Ast_Node*>(), *_temp;
    parser::ast_control->current_position++;

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

parser::Ast_Node_Function_Declaration* parser::Ast_Node_Function_Declaration::generate() {

    Type_Information* _return_type = Type_Information::generatePrimitiveType(PRIMITIVE_TYPE_VOID);

    Name_Space* _name_space = getNameSpace();

    if (ast_control->getToken(0)->id != IDENTIFIER) exception_handle->runExceptionAstControl("Expected token identifier - Ast_Node_Function_Declaration::generate()");

    if (_name_space) parser::ast_control->addToChain(_name_space, NULL);

    char* _struct_name = parser::ast_control->name_space_chain->last->object->scope->last->object;

    char* _function_declaration_name = ast_control->getToken(0)->identifier;

    Ast_Node_Code_Block::setUp();

    parser::ast_control->print("--- --- --- --- ---\n", AST_DEBUG_MODE_STL);

    Ast_Node_Code_Block* _body = parser::ast_control->code_block_chain->last->object;
    Ast_Node_Variable_Declaration* _this_variable = NULL;

    if (
        strcmp(_struct_name, _function_declaration_name) &&
        (*_function_declaration_name != '~' || strcmp(_struct_name, _function_declaration_name + 1))
    ) exception_handle->runExceptionAstControl("Unexpected token");
    parser::ast_control->current_position++;

    bool _is_constructor = !strcmp(_struct_name, _function_declaration_name);

    getCurrentDeclarationTracker()->addName( (char*) "this");

    Ast_Node_Variable_Declaration* _struct_variable_declaration = 
        parser::ast_control->code_block_chain->last->object->name_space->name_space_nodes->last->object->representive_declaration;

    if (!_struct_variable_declaration) 
        exception_handle->runExceptionAstControl("Struct Variable Declaration not defined - Ast_Node_Function_Declaration::getParameters()");

    _struct_variable_declaration = _struct_variable_declaration->getCopy();

    _struct_variable_declaration->declaration_id = getCurrentDeclarationTracker()->getDeclarationId((char*)"this");
    _struct_variable_declaration->type->pointer_level++;

    getCurrentDeclarationTracker()->variable_declaration->add(
        _struct_variable_declaration
    );

    _this_variable = _struct_variable_declaration;

    // Parameters
    utils::Linked_List <Ast_Node*>* _parameters = getParameters();

    // Destructor should not have parameters
    if (!_is_constructor && _parameters->count) exception_handle->runExceptionAstControl("Destructor should not have parameters");

    // Add name space to get declaration id and function declaration
    parser::ast_control->addToChain(
        parser::ast_control->code_block_chain->last->object->name_space, NULL
    );

    // declaration id
    int _declaration_id = getCurrentDeclarationTracker()->getDeclarationId(_function_declaration_name);
    
    // previous declaration
    Ast_Node_Function_Declaration* _previous_function_declaration = NULL;

    // if no declaration add function name
    if (_declaration_id == -1) {

        getCurrentDeclarationTracker()->addName(_function_declaration_name);

        _declaration_id = getCurrentDeclarationTracker()->getDeclarationId(_function_declaration_name);

    }

    // if there is a declaration already check if functinon scope already is declared or not
    else {

        _parameters->insert(_this_variable, 0);

        _parameters->getDataLinkedList(0)->destroy_content = 0;

        _previous_function_declaration = 
            getCurrentDeclarationTracker()->getFunctionDeclaration(_declaration_id, _parameters);

        if (_previous_function_declaration && _previous_function_declaration->body)

            exception_handle->runExceptionAstControl("Redefenition of function - Ast_Node_Function_Declaration::generate()");
    

        delete _parameters->remove(0);

    }

    // return variable declaration (always void)
    Ast_Node_Variable_Declaration* _variable_declaration = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));
    new (_variable_declaration) Ast_Node_Variable_Declaration(
        _return_type, -1, 0
    );

    // function declaration node
    parser::Ast_Node_Function_Declaration* _function_declaration = (parser::Ast_Node_Function_Declaration*) malloc(sizeof(parser::Ast_Node_Function_Declaration));
    new (_function_declaration) parser::Ast_Node_Function_Declaration(
        _previous_function_declaration, 
        _this_variable,
        _variable_declaration, 
        _parameters, 
        _body, 
        _name_space, 
        _declaration_id,
        1,
        0
    );
    if (_is_constructor) _function_declaration->is_constructor = 1;

    // dont add if there is already a forward declaration 
    if (!_previous_function_declaration) getCurrentDeclarationTracker()->function_declaration->add(_function_declaration);
    parser::ast_control->popFromChain();

    switch (parser::ast_control->getToken(0)->id)
    {
    case END_INSTRUCTION: // Forward declaration
        _body->~Ast_Node_Code_Block(); free(_body);
        parser::ast_control->popFromChain();
        parser::ast_control->current_position++;
        _function_declaration->body = NULL;    
        break;
    case OPEN_BRACES: // Function body 
        parser::ast_control->current_position++; Ast_Node_Code_Block::generate(); break;
    default: exception_handle->runExceptionAstControl("Unexpected token - Ast_Node_Function_Declaration::generate() switch"); break;
    }

    // Pop code block chain
    if (_name_space) parser::ast_control->popFromChain();

    // Debug info
    char buffer[100];

    sprintf(buffer, "Declaration Id -> %d", _declaration_id);
    parser::ast_control->print(buffer, AST_DEBUG_MODE_INC);

    sprintf(buffer, "Parameters count -> %d\n", _parameters->count);
    parser::ast_control->print(buffer, AST_DEBUG_MODE_DEC);

    parser::ast_control->print("Ast Node Function Declaration End\n", AST_DEBUG_MODE_DEC);

    return _function_declaration;

}



parser::Ast_Node_Struct_Declaration::~Ast_Node_Struct_Declaration() {
    if (functions) functions->~Ast_Node_Name_Space(); free(functions);
    if (fields) fields->~Ast_Node_Code_Block(); free(fields);
    if (variables_declarations) delete variables_declarations;
    delete representive_declaration;
}

parser::Ast_Node_Struct_Declaration::Ast_Node_Struct_Declaration(Ast_Node_Struct_Declaration* __forward, Ast_Node_Name_Space* __node_name_space, Ast_Node_Code_Block* __node_code_block, int __declaration_id, char* __struct_name) 
    : Ast_Node(NULL, AST_NODE_STRUCT_DECLARATION), variables_declarations(0), forward(__forward), functions(__node_name_space), fields(__node_code_block), declaration_id(__declaration_id), struct_name(__struct_name) {}

parser::Ast_Node_Struct_Declaration* parser::Ast_Node_Struct_Declaration::generate() {

    parser::ast_control->print("Ast Node Struct Declaration\n", AST_DEBUG_MODE_INC);
    
    parser::ast_control->current_position++;

    int _declaration_id;
    char* _struct_name = NULL;
    Ast_Node_Struct_Declaration* _forward = 0;

    switch (parser::ast_control->getToken(0)->id)
    {
    case IDENTIFIER: 
    
        _struct_name = parser::ast_control->getToken(0)->identifier;
        parser::ast_control->current_position++;

        _declaration_id = getCurrentDeclarationTracker()->getDeclarationId(_struct_name);

        parser::Ast_Node_Struct_Declaration* _struct_declaration;

        if (_declaration_id != -1) {

            _struct_declaration = getCurrentDeclarationTracker()->getStructDeclaration(_declaration_id);

            if (_struct_declaration && _struct_declaration->functions && _struct_declaration->fields && ast_control->getToken(0)->id != END_INSTRUCTION)

                exception_handle->runExceptionAstControl("Redefenition of struct declaration");

        }

        else {

            getCurrentDeclarationTracker()->addName(_struct_name);
            _declaration_id = getCurrentDeclarationTracker()->getDeclarationId(_struct_name);

        }

    
        break;

    case OPEN_BRACES: _declaration_id = -2; break;
    default: parser::exception_handle->runExceptionAstControl("Unexpected token"); break;
    }

    Ast_Node_Struct_Declaration* _node_struct_declaration = (Ast_Node_Struct_Declaration*) malloc(sizeof(Ast_Node_Struct_Declaration));
    new (_node_struct_declaration) Ast_Node_Struct_Declaration(
        0, NULL, NULL, _declaration_id, _struct_name
    );

    getCurrentDeclarationTracker()->struct_declaration->add(
        _node_struct_declaration
    );

    if (ast_control->getToken(0)->id == END_INSTRUCTION) { ast_control->current_position++; return _node_struct_declaration;}

    _node_struct_declaration->functions = (Ast_Node_Name_Space*) malloc(sizeof(Ast_Node_Name_Space));
    _node_struct_declaration->fields = (Ast_Node_Code_Block*) malloc(sizeof(Ast_Node_Code_Block));

    Type_Information* _type_information = new Type_Information(
        _node_struct_declaration, NULL
    );

    Ast_Node_Variable_Declaration* _struct_variable_declaration = new Ast_Node_Variable_Declaration(
        _type_information, -1, 0
    );
    _node_struct_declaration->representive_declaration = _struct_variable_declaration;

    new (_node_struct_declaration->functions) Ast_Node_Name_Space(
        _struct_variable_declaration
    );
    _node_struct_declaration->functions->struct_name_space = 1;

    new (_node_struct_declaration->fields) Ast_Node_Code_Block();

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
        _scope_name_space, _node_struct_declaration->functions, getCurrentDeclarationTracker()->off
    );

    _node_struct_declaration->fields->name_space = _name_space;

    delete _scope_name_space;

    parser::ast_control->name_space_control->name_space_collection->add(
        _name_space
    );

    switch (parser::ast_control->getToken(0)->id)
    {
    case OPEN_BRACES: 

        parser::ast_control->addToChain(_name_space, NULL);
        parser::ast_control->addToChain(NULL, _node_struct_declaration->fields);
        
        _node_struct_declaration->set(); 
        
        break;

    default: exception_handle->runExceptionAstControl("Unexpected token"); break;
    }

    if (parser::ast_control->getToken(0)->id == END_INSTRUCTION) parser::ast_control->current_position++;

    else {
        
        _node_struct_declaration->variables_declarations = Ast_Node_Variable_Declaration::generateStructDeclarations(_type_information);

        _node_struct_declaration->variables_declarations->destroy_content = 1;

    }
    
    parser::ast_control->print("Ast Node Struct Declaration End\n", AST_DEBUG_MODE_DEC);

    return _node_struct_declaration;

}

void parser::Ast_Node_Struct_Declaration::set() {

    checkBody();

    int _backup_state = ++parser::ast_control->current_position;

    setFields();

    parser::ast_control->popFromChain();

    parser::ast_control->current_position = _backup_state;

    setFunctions();

    parser::ast_control->popFromChain();

    parser::ast_control->current_position++;

}

void parser::Ast_Node_Struct_Declaration::setFunctions() {

    checkBody();

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

            functions->declarations->add(
                Ast_Node_Function_Declaration::generate(
                    1
                )
            );

            break;
        case AST_NODE_FUNCTION_CALL:

            functions->declarations->add(
                Ast_Node_Function_Declaration::generate()
            );

            break;

        default: exception_handle->runExceptionAstControl("Unexpected Node"); break;
        }

    }

    parser::ast_control->print("Ast Node Struct Set Functions End\n", AST_DEBUG_MODE_DEC);

}

void parser::Ast_Node_Struct_Declaration::setFields() {

    checkBody();

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
        case AST_NODE_FUNCTION_DECLARATION: case AST_NODE_FUNCTION_CALL:

            while(parser::ast_control->getToken(0)->id != CLOSE_PARENTHESIS) parser::ast_control->current_position++;

            parser::ast_control->current_position++;

            ignoreCodeBlock();

            break;
        
        default: exception_handle->runExceptionAstControl("Unexpected Node"); break;
        }

    }

    setDefaultFieldsAddress();

    parser::ast_control->print("Ast Node Struct Set Fields End\n", AST_DEBUG_MODE_DEC);

}

int parser::Ast_Node_Struct_Declaration::getSize() {

    checkBody();

    int _size = built_ins::getPrimitiveTypeSize(
        struct_name
    );

    if (_size != -1) return _size;

    _size = 0;

    for (int _ = 0; _ < fields->code->count; _++)

        if (fields->code->operator[](_)->node_type == AST_NODE_VARIABLE_DECLARATION)

            _size += fields->code->operator[](_)->representive_declaration->type->getSize();

    return _size;

}

int parser::Ast_Node_Struct_Declaration::getVariablesOff(Ast_Node_Variable* __variable_declaration) {

    checkBody();

    int _off = 0;

    for(int _  = 0; _ < fields->code->count; _++) {

        if (fields->code->operator[](_)->node_type != AST_NODE_VARIABLE_DECLARATION) continue;

        if (fields->code->operator[](_) == __variable_declaration->representive_declaration) return _off;

        _off += fields->code->operator[](_)->representive_declaration->type->getSize();

    }

    exception_handle->runException("Struct dont have given variable declaration");

    return -1;

}

void parser::Ast_Node_Struct_Declaration::setDefaultFieldsAddress() {

    int _address = 0;

    for (int _ = 0; _ < fields->code->count; _++) {

        if (fields->code->operator[](_)->node_type != AST_NODE_VARIABLE_DECLARATION) continue;

        ((Ast_Node_Variable_Declaration*)fields->code->operator[](_))->address = _address;

        _address += ((Ast_Node_Variable_Declaration*)fields->code->operator[](_))->type->getSize();

    }

}


bool parser::Ast_Node_Struct_Declaration::isStaticVariableDeclaration(Ast_Node_Variable_Declaration* __variable_declaration) {

    checkBody();

    for (int _ = 0; _ < functions->declarations->count; _++)

        if (
            functions->declarations->operator[](_)->node_type == AST_NODE_VARIABLE_DECLARATION &&
            functions->declarations->operator[](_) == __variable_declaration
        ) return 1;

    return 0;

}

bool parser::Ast_Node_Struct_Declaration::haveContructorDefined() {

    checkBody();

    for (int _ = 0; _ < functions->declarations->count; _++)

        if (
            functions->declarations->operator[](_)->node_type == AST_NODE_FUNCTION_DECLARATION &&
            ((Ast_Node_Function_Declaration*) functions->declarations->operator[](_))->is_constructor
        ) return 1;

    return 0;

}

void parser::Ast_Node_Struct_Declaration::checkBody() {

    if (functions || built_ins::getPrimitiveTypeSize(struct_name) != -1) return;

    exception_handle->runExceptionAstControl("Use of forward struct declaration"); 

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
        destroy_value(1) { organized_set = new utils::Linked_List <Ast_Node*>(); organized_set->destroy_content = 0; }

parser::Ast_Node_Variable_Declaration* parser::Ast_Node_Expression::getResultDeclaration() {

    this->organized_set->clean();

    utils::Linked_List <Expression_Result_Helper*>* _expressions_result_helper = new utils::Linked_List <Expression_Result_Helper*>();
    Ast_Node_Expression* _expression = this, *_last_expression;
    Expression_Result_Helper* _temp;

    while(_expression) {

        _temp = (Expression_Result_Helper*) malloc(sizeof(Expression_Result_Helper)); 

        new (_temp) Expression_Result_Helper(
            _expression
        );

        _expressions_result_helper->add(
            _temp
        );

        if (!_expression->expression) _last_expression = _expression;

        _expression = _expression->expression;

    }

    utils::Linked_List <Ast_Node*>* _parameters = new utils::Linked_List <Ast_Node*>(); _parameters->destroy_content = 0;
    Ast_Node* _first_argument_value, *_second_argument_value, *_third_argument_value;
    Ast_Node_Variable_Declaration* _first_argument, *_second_argument;
    Ast_Node_Function_Declaration* _function_declaration;
    Ast_Node_Name_Space* _struct_node_name_space;
    int _token_id, _declaration_id;
    char* _function_name;

    int _current_priority = 0;

    while (_expressions_result_helper->count != 1) {

        for (int _ = 0; _ < _expressions_result_helper->count; _++) { 

            if (
                getOperatorPriority(_expressions_result_helper->operator[](_)->token_id) == _current_priority
            ) {

                _second_argument = _expressions_result_helper->operator[](_ + 1)->declaration->getCopy();
                _first_argument = _expressions_result_helper->operator[](_)->declaration->getCopy();
                _token_id = _expressions_result_helper->operator[](_)->token_id;

                _second_argument_value = _expressions_result_helper->operator[](_ + 1)->expression->value;
                _first_argument_value = _expressions_result_helper->operator[](_)->expression->value;
                _third_argument_value = NULL;

                if (_first_argument->type->pointer_level) {

                    utils::Linked_List <char*>* _scope = new utils::Linked_List <char*>(); _scope->destroy_content = 0;

                    _scope->add("built_ins");
                    _scope->add("Pointer");

                    parser::ast_control->addToChain(
                        parser::ast_control->name_space_control->getNameSpace(
                            _scope
                        ),
                        NULL
                    );

                    delete _scope;

                    Ast_Node_Variable_Declaration* _node_variable = _expressions_result_helper->operator[](_)->declaration->getCopy();
                    _node_variable->type->pointer_level--;

                    Ast_Node_Pointer_Operation* _node_pointer = (Ast_Node_Pointer_Operation*) malloc(sizeof(Ast_Node_Pointer_Operation));
                    new (_node_pointer) Ast_Node_Pointer_Operation(
                        _node_variable, -1, NULL
                    );
                    _node_pointer->destroy_value = 0;

                    Ast_Node_Expression* _node_expression = (Ast_Node_Expression*) malloc(sizeof(Ast_Node_Expression));

                    new (_node_expression) Ast_Node_Expression(
                        NULL, _node_pointer, -1
                    );

                    Ast_Node_Function_Size_Of* __node_size_of = (Ast_Node_Function_Size_Of*) malloc(sizeof(Ast_Node_Function_Size_Of));

                    new (__node_size_of) Ast_Node_Function_Size_Of(
                        _node_expression
                    );

                    _third_argument_value = __node_size_of;

                    _parameters->add(_third_argument_value->representive_declaration);


                    Ast_Node_Expression* _node_expression_to_remove = (Ast_Node_Expression*) malloc(sizeof(Ast_Node_Expression));

                    new (_node_expression_to_remove) Ast_Node_Expression(
                        NULL, __node_size_of, -1
                    );

                    _last_expression->expression = _node_expression_to_remove;
                    _last_expression = _last_expression->expression;

                }

                else parser::ast_control->addToChain(
                    parser::ast_control->name_space_control->getNameSpace(
                        _first_argument->type->declaration->functions
                    ),
                    NULL
                );

                _function_name = built_ins::getFunctionNameFromTokenId(_token_id);

                std::cout << "Function name -> " << _function_name << std::endl;
                _declaration_id = getDeclarationId(_function_name);
                std::cout << "Declaration -> " << _declaration_id << std::endl;
                free(_function_name);

                if (_expressions_result_helper->operator[](_)->declaration->type->pointer_level);

                else _first_argument->type->pointer_level++;

                _parameters->insert(_first_argument, 0);
                _parameters->insert(_second_argument, 1);

                _function_declaration = getFunctionDeclaration(_declaration_id, _parameters);

                _parameters->clean();

                std::cout << "Function -> " << _function_declaration << std::endl;
                std::cout << "Declaration -> " << _declaration_id << std::endl;

                if (!_function_declaration || _declaration_id == -1) 

                    exception_handle->runExceptionAstControl("No function declaration with given name and given arguments");

                if (!_expressions_result_helper->operator[](_)->function_result_value && !_expressions_result_helper->operator[](_)->declaration->type->isSpykeStruct()) 

                    this->organized_set->add(
                        _first_argument_value
                    );

                if (!_expressions_result_helper->operator[](_ + 1)->function_result_value && !_expressions_result_helper->operator[](_ + 1)->declaration->type->isSpykeStruct())

                    this->organized_set->add(
                        _second_argument_value
                    );

                if (_third_argument_value)

                    this->organized_set->add(
                        _third_argument_value
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

                if (!_expressions_result_helper->operator[](_)->function_result_value && !_expressions_result_helper->operator[](_)->declaration->type->isSpykeStruct()) 

                    this->organized_set->add(
                        _expressions_result_helper->operator[](_)->expression->value
                    );

                _expressions_result_helper->operator[](_)->function_result_value = 1;

            }

        }

        _current_priority++;    

    }

    if (!this->organized_set->count) {

        this->organized_set->add(
            _expressions_result_helper->operator[](0)->expression->value
        );

    }

    Ast_Node_Variable_Declaration* _result = _expressions_result_helper->operator[](0)->declaration;

    delete _expressions_result_helper;

    delete _parameters;

    return _result;

}

parser::Ast_Node_Expression* parser::Ast_Node_Expression::generate(int __value_node_type) {

    parser::ast_control->print("Ast Node Expression\n", AST_DEBUG_MODE_INC);

    Ast_Node* _value;

    if (__value_node_type == -6) {

        _value = generate(nullptr);

    }


    else {

        _value = getValue(__value_node_type);

        if (__value_node_type == AST_NODE_EXPRESSION) {

            if (ast_control->getToken(0)->id != CLOSE_BRACKET) exception_handle->runExceptionAstControl("Excpected token ']'");

            ast_control->current_position++;
        
        }

    }

    std::cout << (int) parser::ast_control->getToken(0)->id << std::endl;

second_operator_check:
    int _token_id = isFunctionOperator(parser::ast_control->getToken(0)->id) ? parser::ast_control->getToken(0)->id : -1;

    if (_token_id != -1) { 
        
        if (isFunctionOperatorSingleArgument(_token_id)) {

            _value = generate(_value);

            goto second_operator_check;

        }

        parser::ast_control->current_position++;
    }

    else {

        switch (getNodeType())
        {
        case AST_NODE_ACCESSING: _value = Ast_Node_Accessing::generate(_value); goto second_operator_check; break;
        default: break;
        }
    }

    std::cout << "Token id -> " << _token_id << std::endl;
 
    int _next_node_type = _token_id == OPEN_BRACKET ? AST_NODE_EXPRESSION : getNodeType();

    parser::Ast_Node_Expression* _expression_node = (parser::Ast_Node_Expression*) malloc(sizeof(parser::Ast_Node_Expression));
    new (_expression_node) parser::Ast_Node_Expression(
        _token_id != -1 ? Ast_Node_Expression::generate(_next_node_type) : NULL,
        _value,
        _token_id
    );

    parser::ast_control->print("Ast Node Expression End\n", AST_DEBUG_MODE_DEC);

    return _expression_node;

}

parser::Ast_Node_Expression* parser::Ast_Node_Expression::generate(Ast_Node* __node) {

    parser::ast_control->print("Ast Node Expression Single Argument\n", AST_DEBUG_MODE_INC);

    int _token_id = parser::ast_control->getToken(0)->id;

    parser::ast_control->current_position++;

    if ((_token_id == FUNCTION_OPERATOR_INCREMENT || _token_id == FUNCTION_OPERATOR_DECREMENT) && !__node) 

        _token_id = _token_id == FUNCTION_OPERATOR_INCREMENT ?  FUNCTION_OPERATOR_INCREMENT_LEFT : FUNCTION_OPERATOR_DECREMENT_LEFT;

    if (!__node) __node = getValue(getNodeType());

    parser::Ast_Node_Expression* _expression_node = (parser::Ast_Node_Expression*) malloc(sizeof(parser::Ast_Node_Expression));
    new (_expression_node) parser::Ast_Node_Expression(
        NULL,
        __node,
        _token_id
    );

    
    utils::Linked_List <Ast_Node*>* _parameters = new utils::Linked_List <Ast_Node*>(); _parameters->destroy_content = 0;

    Ast_Node_Variable_Declaration* _value_declaration = __node->representive_declaration->getCopy();
    
    Ast_Node* _third_argument_value = 0;

    if (_value_declaration->type->pointer_level) {

        utils::Linked_List <char*>* _scope = new utils::Linked_List <char*>(); _scope->destroy_content = 0;

        _scope->add("built_ins");
        _scope->add("Pointer");

        parser::ast_control->addToChain(
            parser::ast_control->name_space_control->getNameSpace(
                _scope
            ),
            NULL
        );

        delete _scope;

        Ast_Node_Variable_Declaration* _node_variable = _value_declaration->getCopy();
        _node_variable->type->pointer_level--;

        Ast_Node_Pointer_Operation* _node_pointer = (Ast_Node_Pointer_Operation*) malloc(sizeof(Ast_Node_Pointer_Operation));
        new (_node_pointer) Ast_Node_Pointer_Operation(
            _node_variable, -1, NULL
        );
        _node_pointer->destroy_value = 0;

        Ast_Node_Expression* _node_expression = (Ast_Node_Expression*) malloc(sizeof(Ast_Node_Expression));

        new (_node_expression) Ast_Node_Expression(
            NULL, _node_pointer, -1
        );

        Ast_Node_Function_Size_Of* __node_size_of = (Ast_Node_Function_Size_Of*) malloc(sizeof(Ast_Node_Function_Size_Of));

        new (__node_size_of) Ast_Node_Function_Size_Of(
            _node_expression
        );

        _third_argument_value = __node_size_of;

        _parameters->add(_third_argument_value->representive_declaration);


        Ast_Node_Expression* _node_expression_to_remove = (Ast_Node_Expression*) malloc(sizeof(Ast_Node_Expression));

        new (_node_expression_to_remove) Ast_Node_Expression(
            NULL, __node_size_of, -1
        );

        _expression_node->expression = _node_expression_to_remove;

    }

    else parser::ast_control->addToChain(
        parser::ast_control->name_space_control->getNameSpace(
            _value_declaration->type->declaration->functions
        ),
        NULL
    );

    char* _function_name = built_ins::getFunctionNameFromTokenId(_token_id);

    std::cout << "Function name -> " << _function_name << std::endl;
    int _declaration_id = getDeclarationId(_function_name);
    std::cout << "Declaration -> " << _declaration_id << std::endl;
    free(_function_name);

    if (_value_declaration->type->pointer_level);

    else _value_declaration->type->pointer_level++;

    _parameters->insert(_value_declaration, 0);

    Ast_Node_Function_Declaration* _function_declaration = getFunctionDeclaration(_declaration_id, _parameters);

    std::cout << "Function -> " << _function_declaration << std::endl;
    std::cout << "Declaration -> " << _declaration_id << std::endl;

    if (!_function_declaration || _declaration_id == -1) 

        exception_handle->runExceptionAstControl("No function declaration with given name and given arguments");

    if (!_value_declaration->type->isSpykeStruct()) 

        _expression_node->organized_set->add(
            __node
        );

    if (_third_argument_value)

        _expression_node->organized_set->add(
            _third_argument_value
        );    
    

    _expression_node->organized_set->add(
        _function_declaration
    );

    _expression_node->representive_declaration = _function_declaration->representive_declaration;

    _value_declaration->~Ast_Node_Variable_Declaration(); free(_value_declaration);

    delete _parameters;

    ast_control->popFromChain();

    parser::ast_control->print("Ast Node Expression Single Argument End\n", AST_DEBUG_MODE_DEC);

    return _expression_node;

}

parser::Ast_Node* parser::Ast_Node_Expression::getValue(int __value_node_type) {

    switch (__value_node_type)
    {
    case AST_NODE_EXPRESSION: return Ast_Node_Expression::generate(getNodeType()); break;
    case AST_NODE_VARIABLE: return Ast_Node_Variable::generate(); break;
    case AST_NODE_VALUE: return Ast_Node_Value::generate(); break;
    case AST_NODE_FUNCTION_CALL: return Ast_Node_Function_Call::generate(0, 0); break;
    case AST_NODE_POINTER_OPERATION: return Ast_Node_Pointer_Operation::generate(); break;
    case AST_NODE_PARENTHESIS: return Ast_Node_Parenthesis::generate(); break;
    case AST_NODE_CAST: return Ast_Node_Cast::generate(); break;
    case AST_NODE_SIZE_OF: return Ast_Node_Function_Size_Of::generate(); break;
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

    Ast_Node_Variable_Declaration* _variable_declaration = getVariableDeclaration(_declaration_id);

    std::cout << "Declaration id -> " << _declaration_id << std::endl;
    std::cout << "Declaration -> " << _variable_declaration << std::endl;

    if (_declaration_id == -1 || !_variable_declaration) 

        exception_handle->runExceptionAstControl("Undefined variable with given name - Ast_Node_Variable::generate()");

    parser::ast_control->current_position++;

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
    case AST_NODE_FUNCTION_CALL: _value = Ast_Node_Function_Call::generate(0, 0); break;
    default: exception_handle->runExceptionAstControl("Error getting Pointer operator value - Ast_Node_Pointer_Operation::Ast_Node_Pointer_Operation() switch"); break;
    }

    Type_Information* _type_information = _value->representive_declaration->type->getCopy();
    _type_information->pointer_level += _pointer_level;

    if (_type_information->pointer_level < 0 && !_type_information->isPointerStruct()) exception_handle->runExceptionAstControl("Pointer level below 0 - Ast_Node_Pointer_Operation::Ast_Node_Pointer_Operation()");

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

void parser::Ast_Node_Pointer_Operation::check() {

    // std::cout << "Pointer Level <- " << pointer_level <<  std::endl;

    delete representive_declaration->type;

    representive_declaration->type = value->representive_declaration->type->getCopy();
    representive_declaration->type->pointer_level += pointer_level;

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

parser::Ast_Node_Function_Call* parser::Ast_Node_Function_Call::generate(Ast_Node_Expression* __expression, Name_Space* __function_name_space) {

    parser::ast_control->print("Ast Node Function Call\n", AST_DEBUG_MODE_INC);

    Name_Space* _name_space = getNameSpace();

    if (parser::ast_control->getToken(0)->id != IDENTIFIER)
        exception_handle->runExceptionAstControl("Expected token identifier - Ast_Node_Function_Call::generate()");

    char* _function_name = parser::ast_control->getToken(0)->identifier;
    parser::ast_control->current_position++;

    utils::Linked_List <Ast_Node_Expression*>* _parameters = getParameters(__expression);

    utils::Linked_List <Ast_Node*>* _parameters_results = getResultParameters(_parameters);

    if (__function_name_space) parser::ast_control->addToChain(__function_name_space, NULL);
    else if (_name_space) parser::ast_control->addToChain(_name_space, NULL);

    int _declaration_id = getDeclarationId(_function_name);

    if (__expression) _parameters_results->first->object->representive_declaration->type->pointer_level++; 

    Ast_Node_Function_Declaration* _function_declaration = getFunctionDeclaration(_declaration_id, _parameters_results);

    if (_declaration_id == -1 || !_function_declaration)
     
        exception_handle->runExceptionAstControl("No declaration of function with given name and parameters");
    
    if (_function_declaration->is_constructor)

        exception_handle->runExceptionAstControl("Cannot call a function constructor directly");
         
    if (__expression) _parameters_results->first->object->representive_declaration->type->pointer_level--; 

    parser::Ast_Node_Function_Call* _node_function_call = (Ast_Node_Function_Call*) malloc(sizeof(Ast_Node_Function_Call));

    new (_node_function_call) parser::Ast_Node_Function_Call(
        _parameters, _function_declaration
    );

    if (_name_space || __function_name_space) parser::ast_control->popFromChain();

    delete _parameters_results;

    parser::ast_control->print("Ast Node Function Call End\n", AST_DEBUG_MODE_DEC);

    return _node_function_call;

}

utils::Linked_List <parser::Ast_Node_Expression*>* parser::Ast_Node_Function_Call::getParameters(Ast_Node_Expression* __expression) {

    parser::ast_control->print("Ast Node Function Call Parameters\n", AST_DEBUG_MODE_INC);

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

    parser::ast_control->print("Ast Node Function Call Parameters End\n", AST_DEBUG_MODE_INC);

    return _parameters;

}

utils::Linked_List <parser::Ast_Node*>* parser::Ast_Node_Function_Call::getResultParameters(utils::Linked_List <Ast_Node_Expression*>* __parameters) {

    utils::Linked_List <parser::Ast_Node*>* _result_parameters = new utils::Linked_List <parser::Ast_Node*>();

    for (int _ = 0; _ < __parameters->count; _++)

        _result_parameters->add(
            __parameters->operator[](_)->getResultDeclaration()->getCopy()
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

        if (_value_pointer_level) {

            Ast_Node_Variable_Declaration* _node_variable_declaration = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));

            new (_node_variable_declaration) Ast_Node_Variable_Declaration(
                _value_before_accessing->representive_declaration->type->getCopy(),
                -1, 0
            );
            _node_variable_declaration->type->pointer_level--;

            Ast_Node_Pointer_Operation* _node_pointer_operation = (Ast_Node_Pointer_Operation*) malloc(sizeof(Ast_Node_Pointer_Operation));

            new (_node_pointer_operation) Ast_Node_Pointer_Operation(
                _node_variable_declaration, -1, _value_before_accessing
            );
            _node_pointer_operation->destroy_value = 0;

            _value_before_accessing = _node_pointer_operation;

        }

        _expression = (Ast_Node_Expression*) malloc(sizeof(Ast_Node_Expression));

        new (_expression) Ast_Node_Expression(
            NULL, _value_before_accessing, -1
        );

        if (!_value_pointer_level) _expression->destroy_value = 0;

        _accessing = Ast_Node_Function_Call::generate(
            _expression,
            parser::ast_control->name_space_control->getNameSpace(__value->representive_declaration->type->declaration->functions)    
        );

        break;

    default: break;
    }

    parser::Ast_Node_Accessing* _node_accessing = (Ast_Node_Accessing*) malloc(sizeof(Ast_Node_Accessing));

    new (_node_accessing) parser::Ast_Node_Accessing(
        __value, _accessing, 
        isAccessingOperator(parser::ast_control->getToken(0)->id) ? Ast_Node_Accessing::generate(_accessing) : NULL
    );

    _node_accessing->setRepresensentiveValue();

    parser::ast_control->print("Ast Node Accessing End\n", AST_DEBUG_MODE_DEC);

    return _node_accessing;

}

void parser::Ast_Node_Accessing::setRepresensentiveValue() {

    Ast_Node_Accessing* _current_acessing = this;

    while(_current_acessing->next) _current_acessing = _current_acessing->next;

    representive_declaration = _current_acessing->accessing->representive_declaration;

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


parser::Ast_Node_Cast::~Ast_Node_Cast() { 
    representive_declaration->~Ast_Node_Variable_Declaration(); free(representive_declaration); 
    value->~Ast_Node(); free(value);
}

parser::Ast_Node_Cast::Ast_Node_Cast(Ast_Node_Variable_Declaration* __declaration, Type_Information* __type_information, Ast_Node* __value) 
    : Ast_Node(__declaration, AST_NODE_CAST), type_cast(__type_information), value(__value) {}

parser::Ast_Node_Cast* parser::Ast_Node_Cast::generate() {

    parser::ast_control->print("Ast Node Cast\n", AST_DEBUG_MODE_INC);

    parser::ast_control->current_position++;

    Type_Information* _cast_type = Type_Information::generate();

    parser::ast_control->current_position++;

    Ast_Node* _value = Ast_Node_Expression::getValue(getNodeType());

    Ast_Node_Variable_Declaration* _declaration = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));
    new (_declaration) Ast_Node_Variable_Declaration(
        _cast_type, -1, 0
    );

    if (_cast_type->declaration->haveContructorDefined()) {

        ast_control->addToChain(
            ast_control->name_space_control->getNameSpace(_cast_type->declaration->functions), 0
        );

        char* _struct_name;

        if (_cast_type->pointer_level) {

            ast_control->popFromChain();

            Type_Information* _pointer_type = Type_Information::generatePrimitiveType(PRIMITIVE_TYPE_POINTER);

            ast_control->addToChain(
                ast_control->name_space_control->getNameSpace(_pointer_type->declaration->functions), 0
            );

            delete _pointer_type;

            _struct_name = "Pointer";

        }

        else _struct_name = _cast_type->declaration->struct_name;

        std::cout << "Struct name -> " << _struct_name << std::endl;

        int _declaration_id = getDeclarationId(_struct_name);

        utils::Linked_List <Ast_Node*>* _params = new utils::Linked_List <Ast_Node*>();

        Ast_Node_Variable_Declaration* _variable_declaration_node = _cast_type->declaration->representive_declaration->getCopy();
        _variable_declaration_node->type->pointer_level++;

        _params->add(
            _variable_declaration_node    
        );

        Ast_Node_Variable_Declaration* _representive_declaration;

        if (_value->node_type == AST_NODE_EXPRESSION) _representive_declaration = ((Ast_Node_Expression*)_value)->getResultDeclaration();
        else _representive_declaration = _value->representive_declaration;

        _params->add(
            _representive_declaration, 0
        ); 

        Ast_Node_Function_Declaration* _function_declaration = getFunctionDeclaration(_declaration_id, _params);

        if (!_function_declaration) exception_handle->runExceptionAstControl("No Constructor with given parameters");

        _declaration->constructor_declaration = _function_declaration;

        delete _params->remove(_params->count);

        delete _params;

        ast_control->popFromChain();


        // exit(1);

    }

    else exception_handle->runExceptionAstControl("No constructor defined to make the conversion");

    Ast_Node_Cast* _node_cast = (Ast_Node_Cast*) malloc(sizeof(Ast_Node_Cast));

    new (_node_cast) Ast_Node_Cast(
        _declaration, _cast_type, _value
    );

    parser::ast_control->print("Ast Node Cast\n", AST_DEBUG_MODE_DEC);

    return _node_cast;

}


parser::Ast_Node_If::~Ast_Node_If() {
    condition->~Ast_Node_Expression(); free(condition);
    delete body;
}

parser::Ast_Node_If::Ast_Node_If(Ast_Node_Expression* __condition, utils::Linked_List <Ast_Node*>* __body) 
    : Ast_Node(0, AST_NODE_IF), condition(__condition), body(__body), next(0) {}

parser::Ast_Node_If* parser::Ast_Node_If::generate() {

    parser::ast_control->print("Ast Node If\n", AST_DEBUG_MODE_INC);

    parser::ast_control->current_position++;

    if (parser::ast_control->getToken(0)->id != OPEN_PARENTHESIS)

        exception_handle->runException("Excpected token '('");

    parser::ast_control->current_position++;

    Ast_Node_Expression* _condition_expression = Ast_Node_Expression::generate(getNodeType()); 

    Ast_Node_Variable_Declaration* _expression_result = _condition_expression->getResultDeclaration();

    Ast_Node_Variable_Declaration* _variable_declaration_node = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));
    new (_variable_declaration_node) Ast_Node_Variable_Declaration(
        Type_Information::generatePrimitiveType(PRIMITIVE_TYPE_BOOL), -1, 0
    );
    _condition_expression->organized_set->insert(
        _variable_declaration_node, 0
    );
    ast_control->to_remove->add(_variable_declaration_node);

    Ast_Node_Variable* _variable_node = (Ast_Node_Variable*) malloc(sizeof(Ast_Node_Variable));
    new (_variable_node) Ast_Node_Variable(
        _variable_declaration_node, -1
    );
    _condition_expression->organized_set->insert(
        _variable_node, 1
    );

    ast_control->to_remove->add(_variable_node);

    parser::ast_control->addToChain(
        ast_control->name_space_control->getNameSpace(_variable_declaration_node->type->declaration->functions),
        NULL
    );

    int _declaration_id = getDeclarationId(_variable_declaration_node->type->declaration->struct_name);

    Ast_Node_Function_Declaration* _function_declaration;

    utils::Linked_List <Ast_Node*>* _parameters = new utils::Linked_List <Ast_Node*>();

    _parameters->add(
        _variable_declaration_node->type->declaration->representive_declaration->getCopy()
    );
    _parameters->operator[](0)->representive_declaration->type->pointer_level++;

    _parameters->add(
        _expression_result, 0
    );

    _function_declaration = getFunctionDeclaration(
        _declaration_id, _parameters
    );

    ast_control->popFromChain();

    delete _parameters->remove(1);

    delete _parameters;

    if (_declaration_id == -1 || !_function_declaration)

        exception_handle->runExceptionAstControl("No Bool Constructor with given argument");

    _condition_expression->organized_set->add(
        _function_declaration
    );

    _condition_expression->organized_set->add(
        _variable_node
    );

    if (parser::ast_control->getToken(0)->id != CLOSE_PARENTHESIS)

        exception_handle->runException("Excpected token ')'");

    parser::ast_control->current_position++;

    utils::Linked_List <Ast_Node*>* _body = new utils::Linked_List <Ast_Node*>(), *_temp;

    switch (int _node_type = getNodeType())
    {
        case -2: parser::ast_control->current_position++; break;
        case AST_NODE_CODE_BLOCK:

            parser::ast_control->current_position++;

            Ast_Node_Code_Block::setUp();
            
            _body->add(
                parser::ast_control->code_block_chain->last->object
            );

            parser::ast_control->code_block_chain->last->object->setCode();

            parser::ast_control->popFromChain();

            break;

        case AST_NODE_BYTE_CODE: _body->add(Ast_Node_Byte_Code::generate()); break;
        case AST_NODE_RETURN: _body->add(Ast_Node_Return::generate()); break;
        case AST_NODE_VARIABLE: case AST_NODE_VALUE: case AST_NODE_FUNCTION_CALL: case AST_NODE_POINTER_OPERATION: case AST_NODE_PARENTHESIS:
        case AST_NODE_CAST:
            _body->add(Ast_Node_Expression::generate(_node_type));
            if (parser::ast_control->getToken(0)->id != END_INSTRUCTION) parser::exception_handle->runExceptionAstControl("Excpected token ';' aqui");
            parser::ast_control->current_position++;
            break;
        case AST_NODE_VARIABLE_DECLARATION:
            
                _temp = Ast_Node_Variable_Declaration::generate();

                _body->join(_temp);

                delete _temp;

                break;

        default: exception_handle->runExceptionAstControl("Node not supported in If Node"); break;
    }

    Ast_Node_If* _node_if = (Ast_Node_If*) malloc(sizeof(Ast_Node_If));

    new (_node_if) Ast_Node_If(
        _condition_expression,
        _body
    );

    parser::ast_control->print("Ast Node If End\n", AST_DEBUG_MODE_DEC);

    return _node_if;

}


parser::Ast_Node_Else_If::~Ast_Node_Else_If() {
    condition->~Ast_Node_Expression(); free(condition);
    delete body;
}

parser::Ast_Node_Else_If::Ast_Node_Else_If(Ast_Node_Expression* __condition, utils::Linked_List <Ast_Node*>* __body) 
    : Ast_Node(0, AST_NODE_ELSE_IF), condition(__condition), body(__body), next(0) {}

parser::Ast_Node_Else_If* parser::Ast_Node_Else_If::generate() {
    
    parser::ast_control->print("Ast Node Else If\n", AST_DEBUG_MODE_INC);

    if (
        !ast_control->current_nodes_list->count ||
        (ast_control->current_nodes_list->last->object->node_type != AST_NODE_IF &&
        ast_control->current_nodes_list->last->object->node_type != AST_NODE_ELSE_IF)
    ) exception_handle->runExceptionAstControl("Excpected node type AST_NODE_IF or AST_NODE_ELSE_IF");

    updateNext();

    parser::ast_control->current_position++;

    if (parser::ast_control->getToken(0)->id != OPEN_PARENTHESIS)

        exception_handle->runException("Excpected token '('");

    parser::ast_control->current_position++;

    Ast_Node_Expression* _condition_expression = Ast_Node_Expression::generate(getNodeType()); 

    Ast_Node_Variable_Declaration* _expression_result = _condition_expression->getResultDeclaration();

    Ast_Node_Variable_Declaration* _variable_declaration_node = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));
    new (_variable_declaration_node) Ast_Node_Variable_Declaration(
        Type_Information::generatePrimitiveType(PRIMITIVE_TYPE_BOOL), -1, 0
    );
    _condition_expression->organized_set->insert(
        _variable_declaration_node, 0
    );
    ast_control->to_remove->add(_variable_declaration_node);

    Ast_Node_Variable* _variable_node = (Ast_Node_Variable*) malloc(sizeof(Ast_Node_Variable));
    new (_variable_node) Ast_Node_Variable(
        _variable_declaration_node, -1
    );
    _condition_expression->organized_set->insert(
        _variable_node, 1
    );

    ast_control->to_remove->add(_variable_node);

    parser::ast_control->addToChain(
        ast_control->name_space_control->getNameSpace(_variable_declaration_node->type->declaration->functions),
        NULL
    );

    int _declaration_id = getDeclarationId(_variable_declaration_node->type->declaration->struct_name);

    Ast_Node_Function_Declaration* _function_declaration;

    utils::Linked_List <Ast_Node*>* _parameters = new utils::Linked_List <Ast_Node*>();

    _parameters->add(
        _variable_declaration_node->type->declaration->representive_declaration->getCopy()
    );
    _parameters->operator[](0)->representive_declaration->type->pointer_level++;

    _parameters->add(
        _expression_result, 0
    );

    _function_declaration = getFunctionDeclaration(
        _declaration_id, _parameters
    );

    ast_control->popFromChain();

    delete _parameters->remove(1);

    delete _parameters;

    if (_declaration_id == -1 || !_function_declaration)

        exception_handle->runExceptionAstControl("No Bool Constructor with given argument");

    _condition_expression->organized_set->add(
        _function_declaration
    );

    _condition_expression->organized_set->add(
        _variable_node
    );

    if (parser::ast_control->getToken(0)->id != CLOSE_PARENTHESIS)

        exception_handle->runException("Excpected token ')'");

    parser::ast_control->current_position++;

    utils::Linked_List <Ast_Node*>* _body = new utils::Linked_List <Ast_Node*>(), *_temp;

    switch (int _node_type = getNodeType())
    {
        case -2: parser::ast_control->current_position++; break;
        case AST_NODE_CODE_BLOCK:

            parser::ast_control->current_position++;

            Ast_Node_Code_Block::setUp();
            
            _body->add(
                parser::ast_control->code_block_chain->last->object
            );

            parser::ast_control->code_block_chain->last->object->setCode();

            parser::ast_control->popFromChain();

            break;

        case AST_NODE_BYTE_CODE: _body->add(Ast_Node_Byte_Code::generate()); break;
        case AST_NODE_RETURN: _body->add(Ast_Node_Return::generate()); break;
        case AST_NODE_VARIABLE: case AST_NODE_VALUE: case AST_NODE_FUNCTION_CALL: case AST_NODE_POINTER_OPERATION: case AST_NODE_PARENTHESIS:
        case AST_NODE_CAST:
            _body->add(Ast_Node_Expression::generate(_node_type));
            if (parser::ast_control->getToken(0)->id != END_INSTRUCTION) parser::exception_handle->runExceptionAstControl("Excpected token ';' aqui");
            parser::ast_control->current_position++;
            break;
        case AST_NODE_VARIABLE_DECLARATION:
            
                _temp = Ast_Node_Variable_Declaration::generate();

                _body->join(_temp);

                delete _temp;

                break;

        default: exception_handle->runExceptionAstControl("Node not supported in If Node"); break;
    }

    Ast_Node_Else_If* _node_else_if = (Ast_Node_Else_If*) malloc(sizeof(Ast_Node_Else_If));

    new (_node_else_if) Ast_Node_Else_If(
        _condition_expression,
        _body
    );

    parser::ast_control->print("Ast Node Else If End\n", AST_DEBUG_MODE_DEC);

    return _node_else_if;
    
}

void parser::Ast_Node_Else_If::updateNext() {

    switch (ast_control->current_nodes_list->last->object->node_type)
    {
    case AST_NODE_IF: ((Ast_Node_If*) ast_control->current_nodes_list->last->object)->next = 1; break;
    case AST_NODE_ELSE_IF: ((Ast_Node_Else_If*) ast_control->current_nodes_list->last->object)->next = 1; break;
    default: break;
    }

}

 
parser::Ast_Node_Else::~Ast_Node_Else() { delete body; }

parser::Ast_Node_Else::Ast_Node_Else(utils::Linked_List <Ast_Node*>* __body) 
    : Ast_Node(0, AST_NODE_ELSE), body(__body) {}

parser::Ast_Node_Else* parser::Ast_Node_Else::generate() {
    
    parser::ast_control->print("Ast Node Else\n", AST_DEBUG_MODE_INC);

    Ast_Node_Else_If::updateNext();

    parser::ast_control->current_position++;

    utils::Linked_List <Ast_Node*>* _body = new utils::Linked_List <Ast_Node*>(), *_temp;

    switch (int _node_type = getNodeType())
    {
        case -2: parser::ast_control->current_position++; break;
        case AST_NODE_CODE_BLOCK:

            parser::ast_control->current_position++;

            Ast_Node_Code_Block::setUp();
            
            _body->add(
                parser::ast_control->code_block_chain->last->object
            );

            parser::ast_control->code_block_chain->last->object->setCode();

            parser::ast_control->popFromChain();

            break;

        case AST_NODE_BYTE_CODE: _body->add(Ast_Node_Byte_Code::generate()); break;
        case AST_NODE_RETURN: _body->add(Ast_Node_Return::generate()); break;
        case AST_NODE_VARIABLE: case AST_NODE_VALUE: case AST_NODE_FUNCTION_CALL: case AST_NODE_POINTER_OPERATION: case AST_NODE_PARENTHESIS:
        case AST_NODE_CAST:
            _body->add(Ast_Node_Expression::generate(_node_type));
            if (parser::ast_control->getToken(0)->id != END_INSTRUCTION) parser::exception_handle->runExceptionAstControl("Excpected token ';' aqui");
            parser::ast_control->current_position++;
            break;
        case AST_NODE_VARIABLE_DECLARATION:
            
                _temp = Ast_Node_Variable_Declaration::generate();

                _body->join(_temp);

                delete _temp;

                break;

        default: exception_handle->runExceptionAstControl("Node not supported in Else Node"); break;
    }

    Ast_Node_Else* _node_else = (Ast_Node_Else*) malloc(sizeof(Ast_Node_Else));

    new (_node_else) Ast_Node_Else(
        _body
    );

    parser::ast_control->print("Ast Node Else End\n", AST_DEBUG_MODE_DEC);

    return _node_else;

}


parser::Ast_Node_Function_Size_Of::~Ast_Node_Function_Size_Of() { size_of->~Ast_Node(); free(size_of); representive_declaration->~Ast_Node_Variable_Declaration(); free(representive_declaration); }

parser::Ast_Node_Function_Size_Of::Ast_Node_Function_Size_Of(Ast_Node* __size_of) : Ast_Node(0, AST_NODE_SIZE_OF), size_of(__size_of) {

    Ast_Node_Variable_Declaration* __variable_declaration = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));

    new (__variable_declaration) Ast_Node_Variable_Declaration(
        Type_Information::generatePrimitiveType(PRIMITIVE_TYPE_INT), -1, 0
    );

    representive_declaration = __variable_declaration;

}

parser::Ast_Node_Function_Size_Of* parser::Ast_Node_Function_Size_Of::generate() {

    parser::ast_control->print("Ast Node Size Of\n", AST_DEBUG_MODE_INC);

    parser::ast_control->current_position++;

    if (parser::ast_control->getToken(0)->id != OPEN_PARENTHESIS) 
        exception_handle->runExceptionAstControl("Excpected token '('");

    parser::ast_control->current_position++;

    Ast_Node_Expression* _expression = Ast_Node_Expression::generate(getNodeType());

    if (parser::ast_control->getToken(0)->id != CLOSE_PARENTHESIS) 
        exception_handle->runExceptionAstControl("Excpected token ')'");

    parser::ast_control->current_position++;

    parser::Ast_Node_Function_Size_Of* _size_of_node = (parser::Ast_Node_Function_Size_Of*) malloc(sizeof(parser::Ast_Node_Function_Size_Of));

    new (_size_of_node) parser::Ast_Node_Function_Size_Of(
        _expression
    ); 

    parser::ast_control->print("Ast Node Size Of End\n", AST_DEBUG_MODE_DEC);

    return _size_of_node;

}

int parser::Ast_Node_Function_Size_Of::getSizeOf() {

    int _size;

    switch (size_of->node_type)
    {
    case AST_NODE_EXPRESSION:

        _size = ((Ast_Node_Expression*) size_of)->getResultDeclaration()->type->getSize();

        break;
    
    default: std::cout << "Error size of function " << std::endl; exit(1); break;
    }

    return _size;

}


parser::Ast_Node_While::~Ast_Node_While() {
    condition->~Ast_Node_Expression(); free(condition);
    delete body;
}

parser::Ast_Node_While::Ast_Node_While(Ast_Node_Expression* __condition, utils::Linked_List <Ast_Node*>* __body) 
    : Ast_Node(0, AST_NODE_WHILE), condition(__condition), body(__body) {}

parser::Ast_Node_While* parser::Ast_Node_While::generate() {

    parser::ast_control->print("Ast Node While\n", AST_DEBUG_MODE_INC);
    Ast_Node_Expression* _condition_expression;

    {
        parser::ast_control->current_position++;

        if (parser::ast_control->getToken(0)->id != OPEN_PARENTHESIS)

            exception_handle->runException("Excpected token '('");

        parser::ast_control->current_position++;

        _condition_expression = Ast_Node_Expression::generate(getNodeType()); 

        Ast_Node_Variable_Declaration* _expression_result = _condition_expression->getResultDeclaration();

        Ast_Node_Variable_Declaration* _variable_declaration_node = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));
        new (_variable_declaration_node) Ast_Node_Variable_Declaration(
            Type_Information::generatePrimitiveType(PRIMITIVE_TYPE_BOOL), -1, 0
        );
        _condition_expression->organized_set->insert(
            _variable_declaration_node, 0
        );
        ast_control->to_remove->add(_variable_declaration_node);

        Ast_Node_Variable* _variable_node = (Ast_Node_Variable*) malloc(sizeof(Ast_Node_Variable));
        new (_variable_node) Ast_Node_Variable(
            _variable_declaration_node, -1
        );
        _condition_expression->organized_set->insert(
            _variable_node, 1
        );

        ast_control->to_remove->add(_variable_node);

        parser::ast_control->addToChain(
            ast_control->name_space_control->getNameSpace(_variable_declaration_node->type->declaration->functions),
            NULL
        );

        int _declaration_id = getDeclarationId(_variable_declaration_node->type->declaration->struct_name);

        Ast_Node_Function_Declaration* _function_declaration;

        utils::Linked_List <Ast_Node*>* _parameters = new utils::Linked_List <Ast_Node*>();

        _parameters->add(
            _variable_declaration_node->type->declaration->representive_declaration->getCopy()
        );
        _parameters->operator[](0)->representive_declaration->type->pointer_level++;

        _parameters->add(
            _expression_result, 0
        );

        _function_declaration = getFunctionDeclaration(
            _declaration_id, _parameters
        );

        ast_control->popFromChain();

        delete _parameters->remove(1);

        delete _parameters;

        if (_declaration_id == -1 || !_function_declaration)

            exception_handle->runExceptionAstControl("No Bool Constructor with given argument");

        _condition_expression->organized_set->add(
            _function_declaration
        );

        _condition_expression->organized_set->add(
            _variable_node
        );

        if (parser::ast_control->getToken(0)->id != CLOSE_PARENTHESIS)

            exception_handle->runException("Excpected token ')'");

        parser::ast_control->current_position++;

    }

    utils::Linked_List <Ast_Node*>* _body = new utils::Linked_List <Ast_Node*>(), *_temp;

    switch (int _node_type = getNodeType())
    {
        case -2: parser::ast_control->current_position++; break;
        case AST_NODE_CODE_BLOCK:

            parser::ast_control->current_position++;

            Ast_Node_Code_Block::setUp();
            
            _body->add(
                parser::ast_control->code_block_chain->last->object
            );

            parser::ast_control->code_block_chain->last->object->setCode();

            parser::ast_control->popFromChain();

            break;

        case AST_NODE_BYTE_CODE: _body->add(Ast_Node_Byte_Code::generate()); break;
        case AST_NODE_RETURN: _body->add(Ast_Node_Return::generate()); break;
        case AST_NODE_VARIABLE: case AST_NODE_VALUE: case AST_NODE_FUNCTION_CALL: case AST_NODE_POINTER_OPERATION: case AST_NODE_PARENTHESIS:
        case AST_NODE_CAST:
            _body->add(Ast_Node_Expression::generate(_node_type));
            if (parser::ast_control->getToken(0)->id != END_INSTRUCTION) parser::exception_handle->runExceptionAstControl("Excpected token ';' aqui");
            parser::ast_control->current_position++;
            break;
        case AST_NODE_VARIABLE_DECLARATION:
            
                _temp = Ast_Node_Variable_Declaration::generate();

                _body->join(_temp);

                delete _temp;

                break;

        default: exception_handle->runExceptionAstControl("Node not supported in If Node"); break;
    }

    parser::Ast_Node_While* _node_while = (Ast_Node_While*) malloc(sizeof(Ast_Node_While));

    new (_node_while) Ast_Node_While(
        _condition_expression,
        _body
    );

    parser::ast_control->print("Ast Node While\n", AST_DEBUG_MODE_DEC);

    return _node_while;

}


parser::Ast_Node_Do_While::~Ast_Node_Do_While() {
    condition->~Ast_Node_Expression(); free(condition);
    delete body;
}

parser::Ast_Node_Do_While::Ast_Node_Do_While(Ast_Node_Expression* __condition, utils::Linked_List <Ast_Node*>* __body) 
    : Ast_Node(0, AST_NODE_DO_WHILE), condition(__condition), body(__body) {}

parser::Ast_Node_Do_While* parser::Ast_Node_Do_While::generate() {

    parser::ast_control->print("Ast Node Do While\n", AST_DEBUG_MODE_INC);
    Ast_Node_Expression* _condition_expression;

    parser::ast_control->current_position++;

    utils::Linked_List <Ast_Node*>* _body = new utils::Linked_List <Ast_Node*>(), *_temp;

    std::cout << "Node type -> " << getNodeType() << std::endl;

    switch (int _node_type = getNodeType())
    {
        case -2: parser::ast_control->current_position++; break;
        case AST_NODE_CODE_BLOCK:

            parser::ast_control->current_position++;

            Ast_Node_Code_Block::setUp();
            
            _body->add(
                parser::ast_control->code_block_chain->last->object
            );

            parser::ast_control->code_block_chain->last->object->setCode();

            parser::ast_control->popFromChain();

            break;

        case AST_NODE_BYTE_CODE: _body->add(Ast_Node_Byte_Code::generate()); break;
        case AST_NODE_RETURN: _body->add(Ast_Node_Return::generate()); break;
        case AST_NODE_VARIABLE: case AST_NODE_VALUE: case AST_NODE_FUNCTION_CALL: case AST_NODE_POINTER_OPERATION: case AST_NODE_PARENTHESIS:
        case AST_NODE_CAST:
            _body->add(Ast_Node_Expression::generate(_node_type));
            if (parser::ast_control->getToken(0)->id != END_INSTRUCTION) parser::exception_handle->runExceptionAstControl("Excpected token ';' aqui");
            parser::ast_control->current_position++;
            break;
        case AST_NODE_VARIABLE_DECLARATION:
            
                _temp = Ast_Node_Variable_Declaration::generate();

                _body->join(_temp);

                delete _temp;

                break;

        default: exception_handle->runExceptionAstControl("Node not supported in Do While Node"); break;
    }

    {
        parser::ast_control->current_position++;

        if (parser::ast_control->getToken(0)->id != OPEN_PARENTHESIS)

            exception_handle->runException("Excpected token '('");

        parser::ast_control->current_position++;

        _condition_expression = Ast_Node_Expression::generate(getNodeType()); 

        Ast_Node_Variable_Declaration* _expression_result = _condition_expression->getResultDeclaration();

        Ast_Node_Variable_Declaration* _variable_declaration_node = (Ast_Node_Variable_Declaration*) malloc(sizeof(Ast_Node_Variable_Declaration));
        new (_variable_declaration_node) Ast_Node_Variable_Declaration(
            Type_Information::generatePrimitiveType(PRIMITIVE_TYPE_BOOL), -1, 0
        );
        _condition_expression->organized_set->insert(
            _variable_declaration_node, 0
        );
        ast_control->to_remove->add(_variable_declaration_node);

        Ast_Node_Variable* _variable_node = (Ast_Node_Variable*) malloc(sizeof(Ast_Node_Variable));
        new (_variable_node) Ast_Node_Variable(
            _variable_declaration_node, -1
        );
        _condition_expression->organized_set->insert(
            _variable_node, 1
        );

        ast_control->to_remove->add(_variable_node);

        parser::ast_control->addToChain(
            ast_control->name_space_control->getNameSpace(_variable_declaration_node->type->declaration->functions),
            NULL
        );

        int _declaration_id = getDeclarationId(_variable_declaration_node->type->declaration->struct_name);

        Ast_Node_Function_Declaration* _function_declaration;

        utils::Linked_List <Ast_Node*>* _parameters = new utils::Linked_List <Ast_Node*>();

        _parameters->add(
            _variable_declaration_node->type->declaration->representive_declaration->getCopy()
        );
        _parameters->operator[](0)->representive_declaration->type->pointer_level++;

        _parameters->add(
            _expression_result, 0
        );

        _function_declaration = getFunctionDeclaration(
            _declaration_id, _parameters
        );

        ast_control->popFromChain();

        delete _parameters->remove(1);

        delete _parameters;

        if (_declaration_id == -1 || !_function_declaration)

            exception_handle->runExceptionAstControl("No Bool Constructor with given argument");

        _condition_expression->organized_set->add(
            _function_declaration
        );

        _condition_expression->organized_set->add(
            _variable_node
        );

        if (parser::ast_control->getToken(0)->id != CLOSE_PARENTHESIS)

            exception_handle->runException("Excpected token ')'");

        parser::ast_control->current_position++;

    }

    if (parser::ast_control->getToken(0)->id != END_INSTRUCTION)

            exception_handle->runException("Excpected token ';'");

    parser::ast_control->current_position++;
    
    parser::Ast_Node_Do_While* _node_do_while = (Ast_Node_Do_While*) malloc(sizeof(Ast_Node_Do_While));

    new (_node_do_while) Ast_Node_Do_While(
        _condition_expression,
        _body
    );

    parser::ast_control->print("Ast Node Do While\n", AST_DEBUG_MODE_DEC);

    return _node_do_while;

}







