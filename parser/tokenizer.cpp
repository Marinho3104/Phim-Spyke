#include "./tokenizer.h"

#include "token_definitions.h"
#include "tokenizer_helper.h"
#include "linked_List.h"
#include "token.h"

#include <iostream>
#include <string.h>

parser::Tokenizer_Control::~Tokenizer_Control() { delete tokens_collection; }

parser::Tokenizer_Control::Tokenizer_Control(bool __debug_mode) : debug_mode(__debug_mode), code_copy(parser::code), current_line(0), inicial_column_address(0) {
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

    char _buffer[50];

    sprintf(_buffer, "\tToken added with id -> %d", __token->id);

    // print(_buffer);

}

void parser::Tokenizer_Control::setNewToken() { if (setTokenSymbol()); else if(setTokenKeyWord()); else setTokenIdentifier(); }

bool parser::Tokenizer_Control::setTokenSymbol() { // Handle tokens

    char* _backup_state = code_copy;

    int __token_id = parser::getTokenSymbol( (const char*&)  code_copy);

    if (!__token_id) return 0;

    Token* _token = (Token*) malloc(sizeof(Token));

    new (_token) Token(
        __token_id, _backup_state - inicial_column_address, code_copy - inicial_column_address, current_line
    );

    addToken(_token);

    return 1;

}

bool parser::Tokenizer_Control::setTokenKeyWord() {

    char* _backup_state = code_copy;

    int __token_id = parser::getTokenKeyWord( (const char*&)  code_copy);

    if (!__token_id) return 0;

    Token* _token = (Token*) malloc(sizeof(Token));

    new (_token) Token(
        __token_id, _backup_state - inicial_column_address, code_copy - inicial_column_address, current_line
    );

    addToken(_token);

    return 1;

}

void parser::Tokenizer_Control::setTokenIdentifier() {

    char* _backup_state = code_copy;
    bool _setTokenState;

    do code_copy++;
    while(*code_copy > 32 && !(_setTokenState = setTokenSymbol()));

    if (_setTokenState) 
        
        code_copy -= 
            tokens_collection->last->object->position_information.final_column - tokens_collection->last->object->position_information.column;

    char* _data = (char*) malloc(code_copy - _backup_state + 1);

    strncpy(_data, _backup_state, code_copy - _backup_state);
    _data[code_copy - _backup_state] = '\0';

    Token* _token = (Token*) malloc(sizeof(Token));
    new (_token) Token(
        IDENTIFIER, _data, *_backup_state, *code_copy, current_line
    );

    if (_setTokenState) {

        code_copy += tokens_collection->last->object->position_information.final_column - tokens_collection->last->object->position_information.column;

        tokens_collection->insert(_token, tokens_collection->count - 2);

    }

    else addToken(_token);

}

void parser::Tokenizer_Control::generate() {
    
    print("");
    print("\t\t---> Tokenizer Control <---");
    print("");

    while(*code_copy) {

        /* Ignore control characters */ 
        if (*code_copy <= 32) { 
            if (*code_copy == '\n') { current_line++; inicial_column_address = code_copy + 1; }
            code_copy++; continue; 
        }

        setNewToken();

    }

    char _buffer[50];

    for (int _ = 0; _ < tokens_collection->count; _++) {

        sprintf(_buffer, "\tToken added with id -> %d", tokens_collection->operator[](_)->id);

        print(_buffer);

    }



}