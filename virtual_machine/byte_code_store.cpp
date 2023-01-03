#include "byte_code_store.h"

#include "byte_code_definitions.h"
#include "linked_List.h"
#include "byte_code.h"

#include <iostream>


virtual_machine::Byte_Code_Store::~Byte_Code_Store() {}

virtual_machine::Byte_Code_Store::Byte_Code_Store(byte_code::Compiled_Code* __compiled_code) {

    blocks = (void**) malloc(8 * __compiled_code->blocks->count);

    void* _current_block;
    int _argument;
    char _code;

    for (int _ = 0; _ < __compiled_code->blocks->count; _++) {

        blocks[_] = malloc(BYTE_CODE_LENGTH * __compiled_code->blocks->operator[](_)->block->count);

        _current_block = blocks[_];

        for (int __ = 0; __ < __compiled_code->blocks->operator[](_)->block->count; __++) {

            *((char*) (&_current_block)) = __compiled_code->blocks->operator[](_)->block->operator[](__)->code;
            *((int*) (&_current_block)) = __compiled_code->blocks->operator[](_)->block->operator[](__)->argument;
           
            _current_block = _current_block + 5;

        } 

    }

}


