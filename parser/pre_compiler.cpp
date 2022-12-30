#include "pre_compiler.h"

#include "pre_compiler_definitions.h"
#include "token_definitions.h"
#include "exception_handle.h"
#include "tokenizer.h"
#include "token.h"


#include "linked_List.h"

#include <iostream>
#include <string.h>

parser::Pre_Compiler_Define::~Pre_Compiler_Define() { delete names_to_find; delete name_to_replace; }

parser::Pre_Compiler_Define::Pre_Compiler_Define(utils::Linked_List <Pre_Compiler_Instruction*>* __pre_compiler_instructions) { 
    names_to_find = new utils::Linked_List <Token*>(); name_to_replace = new utils::Linked_List <Token*>(); 
    names_to_find->destroy_content = 0; name_to_replace->destroy_content = 0;
    utils::Data_Linked_List <Token*>* _data_linked_list;
    Token* _replace_token;

    for (int _ = 0; _ < __pre_compiler_instructions->count; _++)

        if (
            !strcmp((const char*) __pre_compiler_instructions->operator[](_)->instructions->operator[](0)->identifier, PRE_COMPILER_DEFINE)
        ) {

            if (__pre_compiler_instructions->operator[](_)->instructions->count < 2) exception_handle->runException("Few arguments for instruction define");

            if (__pre_compiler_instructions->operator[](_)->instructions->count != 2) 
                
                _replace_token = __pre_compiler_instructions->operator[](_)->instructions->operator[](2);

            else _replace_token = NULL;


            addNewPair(
                __pre_compiler_instructions->operator[](_)->instructions->operator[](1),
                _replace_token
            );


        }

    replace();

}

void parser::Pre_Compiler_Define::addNewPair(Token* __to_find, Token* __to_replace) { 

    names_to_find->add(__to_find); 
    
    name_to_replace->add(__to_replace); 
    
}

parser::Token* parser::Pre_Compiler_Define::getFindToken(char* __token_identifier) {

    for (int _ = 0; _ < names_to_find->count; _++)

        if (
            !strcmp(
                (const char*) names_to_find->operator[](_)->identifier,
                (const char*) __token_identifier
            )
        ) return names_to_find->operator[](_);

    return NULL;

}

int parser::Pre_Compiler_Define::getTokenPositionByLine(int __line_position) {

    int _position = 0;

    while(_position < tokenizer_control->tokens_collection->count) {

        if (
            tokenizer_control->tokens_collection->operator[](_position)->position_information.line > __line_position
        ) return _position;

        _position++;

    }

    return -1;

}

void parser::Pre_Compiler_Define::replace() {

    Token* _founded_token;
    int _position;

    for (int _ = names_to_find->count - 1; _ >= 0; _--) {

        _position = getTokenPositionByLine(names_to_find->operator[](_)->position_information.line);

        std::cout << _position << std::endl;

        while(_position < tokenizer_control->tokens_collection->count) {

            if (tokenizer_control->tokens_collection->operator[](_position)->id == IDENTIFIER)
                std::cout << tokenizer_control->tokens_collection->operator[](_position)->identifier << std::endl;

            if (tokenizer_control->tokens_collection->operator[](_position)->id == IDENTIFIER && 
                !strcmp(tokenizer_control->tokens_collection->operator[](_position)->identifier, 
                names_to_find->operator[](_)->identifier)
            ) {

                _founded_token = tokenizer_control->tokens_collection->operator[](_position);

                tokenizer_control->tokens_collection->getDataLinkedList(_position)->object = name_to_replace->operator[](_)->getCopy();

                _founded_token->~Token(); free(_founded_token);

            }

            _position++;

        }


    }

}



parser::Pre_Compiler_Instruction::~Pre_Compiler_Instruction() { delete instructions; }

parser::Pre_Compiler_Instruction::Pre_Compiler_Instruction(utils::Linked_List <Token*>* __instructions) : instructions(__instructions) {}


parser::Pre_Compiler_Control::~Pre_Compiler_Control() { delete pre_compiler_instructions; }

parser::Pre_Compiler_Control::Pre_Compiler_Control(bool __debug_mode) : current_position(0), debug_mode(__debug_mode), pre_compiler_define(NULL)
    { pre_compiler_instructions = new utils::Linked_List <Pre_Compiler_Instruction*>(); }

void parser::Pre_Compiler_Control::print(const char* __information) {

    if (!debug_mode) return;

    std::cout << "\t" << __information << std::endl;

}

void parser::Pre_Compiler_Control::preCompile() {

    print("");
    print("\t--> Pre Compiler Control <---");
    print("");

    utils::Data_Linked_List <Token*>* _data_linked_list, *_pre_processor;
    Pre_Compiler_Instruction* _pre_compiler_instruction;
    utils::Linked_List <Token*>* _instructions_set;

    while(current_position < tokenizer_control->tokens_collection->count) {

        if (tokenizer_control->tokens_collection->operator[](current_position++)->id != PRE_PROCESSOR) continue;

        _instructions_set = new utils::Linked_List <Token*>();

        --current_position;

        _pre_processor = tokenizer_control->tokens_collection->remove(current_position);

        while(tokenizer_control->tokens_collection->operator[](current_position)->position_information.line == _pre_processor->object->position_information.line) {

            _data_linked_list = tokenizer_control->tokens_collection->remove(current_position);
            _data_linked_list->destroy_content = 0;

            _instructions_set->add(
                _data_linked_list->object
            );

            delete _data_linked_list;

        }

        delete _pre_processor;

        _pre_compiler_instruction = (Pre_Compiler_Instruction*) malloc(sizeof(Pre_Compiler_Instruction));

        new (_pre_compiler_instruction) Pre_Compiler_Instruction(
            _instructions_set
        );

        pre_compiler_instructions->add(
            _pre_compiler_instruction
        );

    }

    pre_compiler_define = new Pre_Compiler_Define(
        pre_compiler_instructions
    );

    delete pre_compiler_define;



}



