#include "./tokenizer.h"

#include "./linked_List.h"

#include <iostream>


parser::Tokenizer_Control::~Tokenizer_Control() {}

parser::Tokenizer_Control::Tokenizer_Control(bool __debug_mode) : debug_mode(__debug_mode) {
    tokens_collection = new utils::Linked_List <Token*>();
}

void parser::Tokenizer_Control::print(const char* __information) {

    if (!debug_mode) return;

    std::cout << __information << std::endl;

}

void parser::Tokenizer_Control::addToken(Token* __token) {

    if (!__token) return;

    tokens_collection->add(
        __token
    );

}

void parser::Tokenizer_Control::generate() {
    
    print("\t\t\n--- Tokenizer Control ---\n");

    while(*code_copy) {



    }

}