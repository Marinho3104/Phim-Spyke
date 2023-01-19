#include "pre_compiler.h"

#include "token_definitions.h"
#include "exception_handle.h"
#include "tokenizer.h"
#include "token.h"
#include "common.h"

#include "linked_List.h"

#include <iostream>
#include <string.h>



parser::Pre_Compiler_Define::~Pre_Compiler_Define() { token_to_replace->~Token(); free(token_to_replace); free(name_to_find); }

parser::Pre_Compiler_Define::Pre_Compiler_Define(char* __name_to_find, Token* __token_to_replace) 
    : name_to_find(__name_to_find), token_to_replace(__token_to_replace) {}

void parser::Pre_Compiler_Define::addNewDefine() {

    utils::Data_Linked_List <Token*>* _data_linked_list;

    delete tokenizer_control->tokens_collection->remove(pre_compiler_control->current_position);

    delete tokenizer_control->tokens_collection->remove(pre_compiler_control->current_position);

    char* _name_to_find = (char*) malloc(strlen(tokenizer_control->tokens_collection->operator[](pre_compiler_control->current_position)->identifier) + 1);
    
    strcpy(
        _name_to_find,
        tokenizer_control->tokens_collection->operator[](pre_compiler_control->current_position)->identifier
    );
    
    delete tokenizer_control->tokens_collection->remove(pre_compiler_control->current_position); 

    Token* _token_to_replace = tokenizer_control->tokens_collection->operator[](pre_compiler_control->current_position);
    _data_linked_list = tokenizer_control->tokens_collection->remove(pre_compiler_control->current_position); _data_linked_list->destroy_content = 0;
    delete _data_linked_list;

    // Miss remove rest at same line TODO

    int _position = getDefinedPosition(_name_to_find);

    if (_position == -1) {

        Pre_Compiler_Define* _define = (Pre_Compiler_Define*) malloc(sizeof(Pre_Compiler_Define));

        new (_define) Pre_Compiler_Define(
            _name_to_find, _token_to_replace
        );

        pre_compiler_control->defines->add(_define);

    }

    else {

        pre_compiler_control->defines->operator[](_position)->~Pre_Compiler_Define();

        pre_compiler_control->defines->operator[](_position)->name_to_find = _name_to_find;
        pre_compiler_control->defines->operator[](_position)->token_to_replace = _token_to_replace;

    }

}

void parser::Pre_Compiler_Define::handle() {

    for (int _ = 0; _ < pre_compiler_control->defines->count; _++) {

        if (
            !strcmp(
                pre_compiler_control->defines->operator[](_)->name_to_find,
                tokenizer_control->tokens_collection->operator[](pre_compiler_control->current_position)->identifier
            )
        ) {

            tokenizer_control->tokens_collection->getDataLinkedList(pre_compiler_control->current_position)->object->id = 
                pre_compiler_control->defines->operator[](_)->token_to_replace->id;

            free(tokenizer_control->tokens_collection->getDataLinkedList(pre_compiler_control->current_position)->object->identifier);

            tokenizer_control->tokens_collection->getDataLinkedList(pre_compiler_control->current_position)->object->identifier = 
                (char*) malloc(strlen(pre_compiler_control->defines->operator[](_)->token_to_replace->identifier) + 1);

            strcpy(
                tokenizer_control->tokens_collection->getDataLinkedList(pre_compiler_control->current_position)->object->identifier,
                pre_compiler_control->defines->operator[](_)->token_to_replace->identifier
            );

        }

    }

}

int parser::Pre_Compiler_Define::getDefinedPosition(char* __name_to_find) {

    for (int _ = 0; _ < pre_compiler_control->defines->count; _++)

        if (
            !strcmp(
                pre_compiler_control->defines->operator[](_)->name_to_find, __name_to_find
            ) 
        ) return _;
    
    return -1;

}


void parser::Pre_Compiler_Include::handle() {

    std::cout << "Handle include" << std::endl;

    delete tokenizer_control->tokens_collection->remove(pre_compiler_control->current_position);

    delete tokenizer_control->tokens_collection->remove(pre_compiler_control->current_position);

    char* _include_path = (char*) malloc(strlen(tokenizer_control->tokens_collection->operator[](pre_compiler_control->current_position)->identifier) + 1);
    
    strcpy(
        _include_path,
        tokenizer_control->tokens_collection->operator[](pre_compiler_control->current_position)->identifier
    );

    delete tokenizer_control->tokens_collection->remove(pre_compiler_control->current_position);
    
    char* _include_code = utils::getFileContent(_include_path);


    Tokenizer_Control* _tokenizer_control = tokenizer_control;
    char* _code = code;

    code = _include_code;
    tokenizer_control = new Tokenizer_Control(1);
    tokenizer_control->tokens_collection->destroy_content = 0;

    tokenizer_control->generate();

    _tokenizer_control->tokens_collection->join(
        tokenizer_control->tokens_collection,
        pre_compiler_control->current_position
    );

    delete tokenizer_control;
    free(code);

    tokenizer_control = _tokenizer_control;
    code = _code;

    free(_include_path);
     
}


void parser::Pre_Compiler_If_Def::handle() {

    std::cout << "Handle If Define" << std::endl;

    delete tokenizer_control->tokens_collection->remove(pre_compiler_control->current_position);

    delete tokenizer_control->tokens_collection->remove(pre_compiler_control->current_position);

    char* _defined_name = (char*) malloc(strlen(tokenizer_control->tokens_collection->operator[](pre_compiler_control->current_position)->identifier) + 1);
    
    strcpy(
        _defined_name,
        tokenizer_control->tokens_collection->operator[](pre_compiler_control->current_position)->identifier
    );

    delete tokenizer_control->tokens_collection->remove(pre_compiler_control->current_position);

    if (Pre_Compiler_Define::getDefinedPosition(_defined_name) != -1) {

        pre_compiler_control->generate(1);

        if (tokenizer_control->tokens_collection->operator[](pre_compiler_control->current_position + 1)->id == PRE_COMPILER_ELSE)

            parser::Pre_Compiler_Else::handle(1);

        else parser::Pre_Compiler_End_If::handle();

    } else {

        pre_compiler_control->ignoreBlock();

        if (tokenizer_control->tokens_collection->operator[](pre_compiler_control->current_position + 1)->id == PRE_COMPILER_ELSE)

            parser::Pre_Compiler_Else::handle(0);

        else parser::Pre_Compiler_End_If::handle();

    }



    free(_defined_name);

}


void parser::Pre_Compiler_If_N_Def::handle() {

    std::cout << "Handle If Not Define" << std::endl;

    delete tokenizer_control->tokens_collection->remove(pre_compiler_control->current_position);

    delete tokenizer_control->tokens_collection->remove(pre_compiler_control->current_position);

    char* _defined_name = (char*) malloc(strlen(tokenizer_control->tokens_collection->operator[](pre_compiler_control->current_position)->identifier) + 1);
    
    strcpy(
        _defined_name,
        tokenizer_control->tokens_collection->operator[](pre_compiler_control->current_position)->identifier
    );

    delete tokenizer_control->tokens_collection->remove(pre_compiler_control->current_position);

    if (Pre_Compiler_Define::getDefinedPosition(_defined_name) == -1) {

        pre_compiler_control->generate(1);

        if (tokenizer_control->tokens_collection->operator[](pre_compiler_control->current_position + 1)->id == PRE_COMPILER_ELSE)

            parser::Pre_Compiler_Else::handle(1);

        else parser::Pre_Compiler_End_If::handle();

    } else {

        pre_compiler_control->ignoreBlock();

        if (tokenizer_control->tokens_collection->operator[](pre_compiler_control->current_position + 1)->id == PRE_COMPILER_ELSE)

            parser::Pre_Compiler_Else::handle(0);

        else parser::Pre_Compiler_End_If::handle();

    }



    free(_defined_name);

}


void parser::Pre_Compiler_Else::handle(bool __ignore) {

    std::cout << "Pre Compiler Else" << std::endl;

    delete tokenizer_control->tokens_collection->remove(pre_compiler_control->current_position);

    delete tokenizer_control->tokens_collection->remove(pre_compiler_control->current_position);

    if (__ignore) pre_compiler_control->ignoreBlock();

    else {

        while(pre_compiler_control->current_position < tokenizer_control->tokens_collection->count) {

            if (tokenizer_control->tokens_collection->operator[](pre_compiler_control->current_position)->id == PRE_PROCESSOR) {

                std::cout << (int) tokenizer_control->tokens_collection->operator[](pre_compiler_control->current_position + 1)->id << std::endl;

                bool _break = 0;

                switch (tokenizer_control->tokens_collection->operator[](pre_compiler_control->current_position + 1)->id)
                {
                case PRE_COMPILER_DEFINE: Pre_Compiler_Define::addNewDefine(); break;
                case PRE_COMPILER_INCLUDE: Pre_Compiler_Include::handle(); break;
                case PRE_COMPILER_IF_DEF: Pre_Compiler_If_Def::handle(); break;
                case PRE_COMPILER_ELSE: case PRE_COMPILER_END_IF: _break = 1; break;
                default: break;
                }

                if (_break) break;

            }

            else {

                switch (tokenizer_control->tokens_collection->operator[](pre_compiler_control->current_position)->id)
                {
                case IDENTIFIER: Pre_Compiler_Define::handle(); break;
                default: break;
                }

                pre_compiler_control->current_position++;

            }

        }

    }
    
    Pre_Compiler_End_If::handle();

}


void parser::Pre_Compiler_End_If::handle() {

    std::cout << "Pre Compiler End If" << std::endl;

    delete tokenizer_control->tokens_collection->remove(pre_compiler_control->current_position);

    delete tokenizer_control->tokens_collection->remove(pre_compiler_control->current_position);

}

parser::Pre_Compiler_Control::~Pre_Compiler_Control() { delete defines; }

parser::Pre_Compiler_Control::Pre_Compiler_Control(bool __debug_mode) : current_position(0), debug_mode(__debug_mode) 
    { defines = new utils::Linked_List <Pre_Compiler_Define*>(); }

void parser::Pre_Compiler_Control::print(const char* __information) {

    if (!debug_mode) return;

    std::cout << "\t" << __information << std::endl;

}

void parser::Pre_Compiler_Control::preCompile() {

    print("");
    print("\t--> Pre Compiler Control <---");
    print("");

    generate(0);    

}

void parser::Pre_Compiler_Control::generate(bool __condition_block) {

    while(current_position < tokenizer_control->tokens_collection->count) {

        if (tokenizer_control->tokens_collection->operator[](current_position)->id == PRE_PROCESSOR) {

            std::cout << (int) tokenizer_control->tokens_collection->operator[](current_position + 1)->id << std::endl;

            bool _break = 0;

            switch (tokenizer_control->tokens_collection->operator[](current_position + 1)->id)
            {
            case PRE_COMPILER_DEFINE: Pre_Compiler_Define::addNewDefine(); break;
            case PRE_COMPILER_INCLUDE: Pre_Compiler_Include::handle(); break;
            case PRE_COMPILER_IF_DEF: Pre_Compiler_If_Def::handle(); break;
            case PRE_COMPILER_IF_N_DEF: Pre_Compiler_If_N_Def::handle(); break;
            case PRE_COMPILER_ELSE: case PRE_COMPILER_END_IF: if (__condition_block) _break = 1; else {std::cout<<"Error pre compiler" << std::endl; exit(1);} break;
            default: break;
            }

            if (_break) break;

        }

        else {

            switch (tokenizer_control->tokens_collection->operator[](current_position)->id)
            {
            case IDENTIFIER: Pre_Compiler_Define::handle(); break;
            default: break;
            }

            current_position++;

        }

    }


}


void parser::Pre_Compiler_Control::ignoreBlock() {
    std::cout << "Ignore Block" << std::endl;

    while(pre_compiler_control->current_position < tokenizer_control->tokens_collection->count) {

        if (tokenizer_control->tokens_collection->operator[](pre_compiler_control->current_position)->id == PRE_PROCESSOR) {

            bool _break = 0;

            switch (tokenizer_control->tokens_collection->operator[](pre_compiler_control->current_position + 1)->id)
            {
            case PRE_COMPILER_ELSE: case PRE_COMPILER_END_IF: _break = 1; break;            
            default: break;
            }

            if (_break) break;

        }

        else delete tokenizer_control->tokens_collection->remove(current_position);

    }
}
