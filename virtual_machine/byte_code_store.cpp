#include "byte_code_store.h"

#include "byte_code_definitions.h"
#include "linked_List.h"
#include "byte_code.h"

#include <iostream>


virtual_machine::Byte_Code_Store::~Byte_Code_Store() { 
    
    byte_code::Byte_Code** _current_block = blocks;

    while(*_current_block) { free(*_current_block); _current_block = _current_block + 1; }
    
    free(blocks); 
    
}

virtual_machine::Byte_Code_Store::Byte_Code_Store(byte_code::Compiled_Code* __compiled_code) {

    blocks = (byte_code::Byte_Code**) malloc(8 * (__compiled_code->blocks->count + 1));
    blocks[__compiled_code->blocks->count] = NULL;
    byte_code::Byte_Code* _current_block;

    for (int _ = 0; _ < __compiled_code->blocks->count; _++) {

        blocks[_] = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code) * __compiled_code->blocks->operator[](_)->block->count);

        _current_block = blocks[_];

        for (int __ = 0; __ < __compiled_code->blocks->operator[](_)->block->count; __++) {

            new (_current_block) byte_code::Byte_Code(
                __compiled_code->blocks->operator[](_)->block->operator[](__)->code,
                __compiled_code->blocks->operator[](_)->block->operator[](__)->argument
            );

            _current_block ++;

        }

    }

}

byte_code::Byte_Code* virtual_machine::Byte_Code_Store::getByteCode(int __block_index, int __index) { return &blocks[__block_index][__index]; }


