#include "ast.h"

#include "tokenizer.h"

#include "ast_definitions.h"
#include "linked_List.h"
#include "ast_helper.h"
#include "ast_nodes.h"

#include <iostream>
#include <string.h>


parser::Declarations_Tracker::~Declarations_Tracker() {
    
    delete variable_declaration; delete function_declaration; delete struct_declaration;
    delete declaration_ids; delete names;

}

parser::Declarations_Tracker::Declarations_Tracker(int* __off) : off(__off) {

    variable_declaration = new utils::Linked_List <parser::Ast_Node_Variable_Declaration*>(); variable_declaration->destroy_content = 0;
    function_declaration = new utils::Linked_List <parser::Ast_Node_Function_Declaration*>(); function_declaration->destroy_content = 0;
    struct_declaration = new utils::Linked_List <parser::Ast_Node_Struct_Declaration*>(); struct_declaration->destroy_content = 0;

    declaration_ids = new utils::Linked_List <int>();
    names = new utils::Linked_List <char*>();

}

parser::Ast_Node_Variable_Declaration* parser::Declarations_Tracker::getVariableDeclaration(int __declaration_id) {

    for (int _ = 0; _ < variable_declaration->count; _++)

        if (variable_declaration->operator[](_)->declaration_id == __declaration_id) return variable_declaration->operator[](_);

    return NULL;

}

parser::Ast_Node_Function_Declaration* parser::Declarations_Tracker::getFunctionDeclaration(int __declaration_id, utils::Linked_List <Ast_Node*>* __parameters) {

    utils::Linked_List <Ast_Node_Variable_Declaration*>* _given_variable_declarations =
        getSpecificNodesFromLinkedList<parser::Ast_Node_Variable_Declaration>(__parameters, AST_NODE_VARIABLE_DECLARATION), *_founded_variable_declarations;
    parser::Ast_Node_Function_Declaration* _function_declaration = NULL;

    for (int _ = 0; _ < function_declaration->count; _++)

        if (
            function_declaration->operator[](_)->declaration_id == __declaration_id
        ) {

            _function_declaration = function_declaration->operator[](_);

            _founded_variable_declarations = 
                getSpecificNodesFromLinkedList<parser::Ast_Node_Variable_Declaration>(_function_declaration->parameters, AST_NODE_VARIABLE_DECLARATION);

            if (
                _function_declaration->is_struct && !_function_declaration->is_static
            ) {

                std::cout << "Added this var" << std::endl;

                _founded_variable_declarations->insert(
                    _function_declaration->this_variable, 0
                );
            }

            std::cout << "Given count -> " << _given_variable_declarations->count << std::endl;
            std::cout << "Founded count -> " << _founded_variable_declarations->count << std::endl;

            if (_given_variable_declarations->count != _founded_variable_declarations->count) _function_declaration = NULL;

            for (int _ = 0; _ < _given_variable_declarations->count && _function_declaration; _++) {

                std::cout << "loop " << std::endl;
                std::cout << _given_variable_declarations->operator[](_)->type << std::endl;
                std::cout << _founded_variable_declarations->operator[](_)->type << std::endl;

                if (
                    _given_variable_declarations->operator[](_)->
                    type->
                    operator!=(
                        _founded_variable_declarations->operator[](_)->type
                    ) 
                ) _function_declaration = NULL;

            }

            delete _founded_variable_declarations;

            if (_function_declaration) break;

        }

    delete _given_variable_declarations;

    return _function_declaration;

}

parser::Ast_Node_Struct_Declaration* parser::Declarations_Tracker::getStructDeclaration(int __declaration_id) {

    for (int _ = 0; _ < struct_declaration->count; _++)

        if (struct_declaration->operator[](_)->declaration_id == __declaration_id) return struct_declaration->operator[](_);

    return NULL;


}

int parser::Declarations_Tracker::getDeclarationId(char* __name) {

    int _position = names->getPosition(__name, NULL);

    return _position != -1 ? declaration_ids->operator[](_position) : -1;

}

void parser::Declarations_Tracker::addName(char* __name, int __declaration_id) {

    char* _name_copy = (char*) malloc(strlen(__name) + 1);

    strcpy(_name_copy, __name);

    names->add(_name_copy); declaration_ids->add(__declaration_id);

}

void parser::Declarations_Tracker::addName(char* __name) {

    char* _name_copy = (char*) malloc(strlen(__name) + 1);

    strcpy(_name_copy, __name);

    names->add(_name_copy); declaration_ids->add((*off)++);

}



parser::Name_Space::~Name_Space() { 
    delete scope; 
    delete declarations_tracker;
    delete name_space_nodes;
}

parser::Name_Space::Name_Space(utils::Linked_List <char*>* __scope, int* __off) : name_space_nodes(new utils::Linked_List <Ast_Node_Name_Space*>()) {

    declarations_tracker = new Declarations_Tracker(__off);
    name_space_nodes->destroy_content = 0;
    scope = new utils::Linked_List <char*>();

    char* _temp_data;

    if (__scope) 

        for (int _ = 0; _ < __scope->count; _++) {

            _temp_data = (char*) malloc(strlen(__scope->operator[](_)) + 1);

            strcpy(_temp_data, __scope->operator[](_));

            scope->add(_temp_data);

        }
    

}

parser::Name_Space::Name_Space(utils::Linked_List <char*>* __scope, Ast_Node_Name_Space* __name_space_node, int* __off) : name_space_nodes(new utils::Linked_List <Ast_Node_Name_Space*>()) {

    declarations_tracker = new Declarations_Tracker(__off);
    name_space_nodes->destroy_content = 0;
    scope = new utils::Linked_List <char*>();

    char* _temp_data;

    if (__scope) 

        for (int _ = 0; _ < __scope->count; _++) {

            _temp_data = (char*) malloc(strlen(__scope->operator[](_)) + 1);

            strcpy(_temp_data, __scope->operator[](_));

            scope->add(_temp_data);

        }

    name_space_nodes->add(__name_space_node);

}



parser::Name_Space_Control::~Name_Space_Control() { delete name_space_collection; }

parser::Name_Space_Control::Name_Space_Control() : off(0) {
    name_space_collection = new utils::Linked_List <Name_Space*>(); // name_space_collection->destroy_content = 0;
    setGlobalNameSpace();
}

void parser::Name_Space_Control::setGlobalNameSpace() {

    Name_Space* _global_name_space = (Name_Space*) malloc(sizeof(Name_Space));

    new (_global_name_space) Name_Space(NULL, &off);

    name_space_collection->add(_global_name_space);

}

void parser::Name_Space_Control::addNameSpace(utils::Linked_List <char*>* __scope) {

    if (getNameSpace(__scope)) return;

    Name_Space* _name_space = (Name_Space*) malloc(sizeof(Name_Space));

    new (_name_space) Name_Space(__scope, &off);

    name_space_collection->add(_name_space);

}

parser::Name_Space* parser::Name_Space_Control::getNameSpace(utils::Linked_List <char*>* __scope) {

    for (int _ = 0; _ < name_space_collection->count; _++)

        if (
            name_space_collection->operator[](_)->scope->operator==(
                __scope
            )
        ) return name_space_collection->operator[](_);

    return NULL;

}

parser::Name_Space* parser::Name_Space_Control::getNameSpace(Ast_Node_Name_Space* __node_name_space) {

    for (int _ = 0; _ < name_space_collection->count; _++)

        for (int __ = 0; __ < name_space_collection->operator[](_)->name_space_nodes->count; __++)

            if (
                name_space_collection->operator[](_)->name_space_nodes->operator[](__) == __node_name_space
            ) return name_space_collection->operator[](_);

    return NULL;

}

parser::Name_Space* parser::Name_Space_Control::getNameSpaceOrAdd(utils::Linked_List <char*>* __scope) {

    if (parser::Name_Space* _name_space = getNameSpace(__scope)) return _name_space;

    addNameSpace(__scope);

    return name_space_collection->last->object;

}

parser::Name_Space* parser::Name_Space_Control::getPreviousNameSpace(Name_Space* __name_space) {
    
    utils::Linked_List <char*>* _name_space_scope = new utils::Linked_List <char*>(); _name_space_scope->destroy_content = 0;
    _name_space_scope->join(__name_space->scope);
    
    Name_Space* _name_space = NULL;

    while(!_name_space && _name_space_scope->count--) _name_space = getNameSpace(_name_space_scope);

    delete _name_space_scope;

    return _name_space;

}



parser::Ast_Control::~Ast_Control() { 
    
    global_name_space->~Ast_Node_Name_Space(); free(global_name_space); 
    
    delete name_space_control;
    delete name_space_chain; delete code_block_chain;

    delete implicit_values_collection;
    
}

parser::Ast_Control::Ast_Control(bool __debug_mode) : current_position(0), debug_mode_tab(), debug_mode(__debug_mode), global_name_space(NULL) {

    name_space_control = new Name_Space_Control();

    name_space_chain = new utils::Linked_List <Name_Space*>();
    code_block_chain = new utils::Linked_List <Ast_Node_Code_Block*>();

    implicit_values_collection = new utils::Linked_List <char*>();

}

parser::Token* parser::Ast_Control::getToken(int __off) {

    if (current_position + __off >= tokenizer_control->tokens_collection->count) 
        __off = tokenizer_control->tokens_collection->count - 1 - current_position;

    return tokenizer_control->tokens_collection->operator[](current_position + __off); 

}

void parser::Ast_Control::print(const char* __information, int __action) {

    if (!debug_mode) return;

    if (__action == AST_DEBUG_MODE_INC) debug_mode_tab++;

    for (int _ = 0; _ < debug_mode_tab; _++) std::cout << "\t";

    std::cout << __information << std::endl;

    if (__action == AST_DEBUG_MODE_DEC) debug_mode_tab--;

}

void parser::Ast_Control::generate() {

    print("", AST_DEBUG_MODE_STL);
    print("\t\t---> Ast Control <---", AST_DEBUG_MODE_STL);
    print("", AST_DEBUG_MODE_STL);

    global_name_space = Ast_Node_Name_Space::generate(
        name_space_control->name_space_collection->operator[](0)
    ); 

}

int parser::Ast_Control::addImplicitValue(char* __implicit_value) {

    int _position;

    if ((_position = implicit_values_collection->getPosition(__implicit_value, NULL)) != -1) return _position;

    char* _value_copy = (char*) malloc(strlen(__implicit_value) + 1);

    strcpy(_value_copy, __implicit_value);

    return implicit_values_collection->add(_value_copy);

}

void parser::Ast_Control::addToChain(Name_Space* __name_space, Ast_Node_Code_Block* __node_code_block) {
    name_space_chain->add(__name_space); code_block_chain->add(__node_code_block);
}

void parser::Ast_Control::popFromChain()  {
    utils::Data_Linked_List <Name_Space*>* _data_linked_list_name_space = name_space_chain->remove(name_space_chain->count);
    _data_linked_list_name_space->destroy_content = 0; delete _data_linked_list_name_space;
    utils::Data_Linked_List <Ast_Node_Code_Block*>* _data_linked_list_node_code_block = code_block_chain->remove(code_block_chain->count);
    _data_linked_list_node_code_block->destroy_content = 0; delete _data_linked_list_node_code_block;
}




