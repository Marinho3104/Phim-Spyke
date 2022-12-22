#include "ast.h"

#include "exception_handle.h"
#include "tokenizer.h"

#include "linked_List.h"
#include "ast_nodes.h"

#include <iostream>
#include <string.h>

parser::Declaration_Tracker::~Declaration_Tracker() {
    delete variable_declarations; delete function_declarations; delete struct_declarations;
    delete names; delete declaration_ids;
}

parser::Declaration_Tracker::Declaration_Tracker(int* __off) : off(__off) {

    variable_declarations = new utils::Linked_List <Ast_Node_Variable_Declaration*>(); variable_declarations->destroy_content = 0;
    function_declarations = new utils::Linked_List <Ast_Node_Function_Declaration*>(); function_declarations->destroy_content = 0;
    struct_declarations = new utils::Linked_List <Ast_Node_Struct_Declaration*>(); struct_declarations->destroy_content = 0;

    names = new utils::Linked_List <char*>(); declaration_ids = new utils::Linked_List <int>();

}

parser::Ast_Node_Variable_Declaration* parser::Declaration_Tracker::getVariableDeclaration(int __declaration_id) {

    for (int _ = 0; _ < variable_declarations->count; _++)

        if (variable_declarations->operator[](_)->declaration_id == __declaration_id) return variable_declarations->operator[](_);

    return NULL;

}

parser::Ast_Node_Function_Declaration* parser::Declaration_Tracker::getFunctionDeclaration(int __declaration_id, utils::Linked_List <Ast_Node*>* __parameters) {

}

parser::Ast_Node_Struct_Declaration* parser::Declaration_Tracker::getStructDeclaration(int __declaration_id) {
    
    for (int _ = 0; _ < struct_declarations->count; _++)

        if (struct_declarations->operator[](_)->declaration_id == __declaration_id) return struct_declarations->operator[](_);

    return NULL;

}

int parser::Declaration_Tracker::getDeclarationId(char* __name) {

    int _position = names->getPosition(__name, NULL);

    return _position != -1 ? declaration_ids->operator[](_position) : -1;

}

void parser::Declaration_Tracker::addName(char* __name) {

    if (getDeclarationId(__name) != -1) return;

    char* _name_copy = (char*) malloc(strlen(__name) + 1);

    strcpy(_name_copy, __name);

    names->add(_name_copy); declaration_ids->add((*off)++);

}

void parser::Declaration_Tracker::addStaticName(char* __name, int __declaration_id) {

    if (getDeclarationId(__name) != -1) return;

    char* _name_copy = (char*) malloc(strlen(__name) + 1);

    strcpy(_name_copy, __name);

    names->add(_name_copy); declaration_ids->add(__declaration_id);

}



parser::Name_Space::~Name_Space() { delete scope; delete declaration_tracker; }

parser::Name_Space::Name_Space(utils::Linked_List <char*>* __scope, int* __off) : scope(new utils::Linked_List <char*>()) { 
    
    declaration_tracker = new Declaration_Tracker(__off); 
    char* _data;

    if (__scope)

        for (int _ = 0; _ < __scope->count; _++) {

            _data = (char*) malloc( strlen(__scope->operator[](_)) + 1);

            strcpy(_data, __scope->operator[](_));

            scope->add(_data);

        }

}


parser::Name_Space_Control::~Name_Space_Control() { delete name_space_collection; } 

parser::Name_Space_Control::Name_Space_Control() : declaration_off(0) 
    { name_space_collection = new utils::Linked_List <Name_Space*>(); loadGlobalNameSpace(); }

void parser::Name_Space_Control::loadGlobalNameSpace() {

    Name_Space* _name_space = (Name_Space*) malloc(sizeof(Name_Space));
    new (_name_space) Name_Space(NULL, &declaration_off);

    addNameSpace(_name_space);
    
}

void parser::Name_Space_Control::addNameSpace(Name_Space* __name_space) {

    if (getNameSpace(__name_space->scope)) return;

    name_space_collection->add(__name_space);

}

void parser::Name_Space_Control::addNameSpace(utils::Linked_List <char*>* __scope) {

    if (getNameSpace(__scope)) return;

    Name_Space* _name_space = (Name_Space*) malloc(sizeof(Name_Space));
    new (_name_space) Name_Space(__scope, &declaration_off);

    name_space_collection->add(_name_space);

}

parser::Name_Space* parser::Name_Space_Control::getNameSpace(utils::Linked_List <char*>* __scope) {

    for (int _ = 0; _ < name_space_collection->count; _++)

        if (name_space_collection->operator[](_)->scope->operator==(__scope)) return name_space_collection->operator[](_);

    return NULL;

}

parser::Name_Space* parser::Name_Space_Control::getNameSpaceOrAdd(utils::Linked_List <char*>* __scope) {

    if (parser::Name_Space* _name_space = getNameSpace(__scope)) return _name_space;

    addNameSpace(__scope);

    return name_space_collection->last->object;

}

parser::Name_Space* parser::Name_Space_Control::getPreviousNameSpace(Name_Space* __name_space) {

    int _original_name_space_scope_length = __name_space->scope->count;
    parser::Name_Space* _name_space = NULL;

    while(__name_space->scope->count-- && !_name_space) _name_space = getNameSpace(__name_space->scope);

    __name_space->scope->count = _original_name_space_scope_length;    

    return _name_space;

}


parser::Ast_Control::~Ast_Control() {
    delete name_space_node_collection; delete implicit_value_collection;
    delete name_space_control; 
    delete name_space_chain; delete code_block_chain;
}

parser::Ast_Control::Ast_Control(bool __debug_mode) : debug_mode(__debug_mode), current_token_position(0), debug_information_tab(-1) {

    name_space_node_collection = new utils::Linked_List <Ast_Node_Name_Space*>();
    implicit_value_collection = new utils::Linked_List <char*>();

    name_space_control = new Name_Space_Control();

    name_space_chain = new utils::Linked_List <Ast_Node_Name_Space*>();
    code_block_chain = new utils::Linked_List <Ast_Node_Code_Block*>();

}

parser::Token* parser::Ast_Control::getToken(int __off) {

    if (current_token_position + __off >= tokenizer_control->tokens_collection->count) 
        __off = tokenizer_control->tokens_collection->count - 1 - current_token_position;

    return tokenizer_control->tokens_collection->operator[](current_token_position + __off); 

}

void parser::Ast_Control::print(const char* __information) {
    
    if (!debug_mode) return;

    for (int _ = 0; _ < debug_information_tab; _++) std::cout << "\t";

    std::cout << "\t" << __information << std::endl;

}

void parser::Ast_Control::generate() {

    print("");
    print("\t---> Ast Control <---");
    print("");

    Ast_Node_Name_Space::generate(name_space_control->name_space_collection->operator[](0));

}

void parser::Ast_Control::addNameSpaceNodeToChain(Ast_Node_Name_Space* __node_name_space) { name_space_chain->add(__node_name_space); }

void parser::Ast_Control::addNameSpaceNodeToChain(Name_Space* __name_space) { addNameSpaceNodeToChain(getNodeNameSpace(__name_space)); }

void parser::Ast_Control::popNameSpaceChainFromChain() {
    utils::Data_Linked_List <Ast_Node_Name_Space*>* _data_linked_list = name_space_chain->remove(name_space_chain->count);
    _data_linked_list->destroy_content = 0; delete _data_linked_list;
}

void parser::Ast_Control::addCodeBlockNodeToChain(Ast_Node_Code_Block* __code_block) { code_block_chain->add(__code_block); }

void parser::Ast_Control::popCodeBlockChainFromChain() {
    utils::Data_Linked_List <Ast_Node_Code_Block*>* _data_linked_list = code_block_chain->remove(code_block_chain->count);
    _data_linked_list->destroy_content = 0; delete _data_linked_list;
}

parser::Ast_Node_Name_Space* parser::Ast_Control::getNodeNameSpace(Name_Space* __name_space) {

    for (int _ = 0; _ < name_space_node_collection->count; _++)

        if (name_space_node_collection->operator[](_)->name_space == __name_space) return name_space_node_collection->operator[](_);

    parser::exception_handle->runException("Cannot find a Name Space Node with given Name Space");

    return NULL;

}

