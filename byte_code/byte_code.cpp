#include "byte_code.h"

#include "linked_List.h"
#include "opcodes.h"

#include <iostream>


byte_code::Byte_Code::~Byte_Code() {}

byte_code::Byte_Code::Byte_Code(char __code, int __argument) : code(__code), argument(__argument) {}


byte_code::Byte_Code_Block::~Byte_Code_Block() { delete block; }

byte_code::Byte_Code_Block::Byte_Code_Block(utils::Linked_List <Byte_Code*>* __block) : block(__block), current_allocation_size(0) {}


byte_code::Compiled_Code::~Compiled_Code() { delete blocks; delete implicit_values; }

byte_code::Compiled_Code::Compiled_Code(utils::Linked_List <Byte_Code_Block*>* __blocks, utils::Linked_List <char*>* __implicit_values) 
    : blocks(__blocks), implicit_values(__implicit_values) {}

byte_code::Compiled_Code* byte_code::Compiled_Code::getByFile() {

    FILE* _file = fopen("byte_code.marinho", "rb");

    utils::Linked_List <byte_code::Byte_Code*>* _byte_code_linked_list = new utils::Linked_List <byte_code::Byte_Code*>();
    utils::Linked_List <Byte_Code_Block*>* _blocks = new utils::Linked_List <Byte_Code_Block*>();

    byte_code::Byte_Code_Block* _byte_code_block; // 
    byte_code::Byte_Code* _current_byte_code;

    int _argument;
    char _code;

    while(fread(&_code, 1, 1, _file)) {

        fread(&_argument, 4, 1, _file);

        _current_byte_code = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

        new (_current_byte_code) byte_code::Byte_Code(
            _code, _argument
        );

        _byte_code_linked_list->add(
            _current_byte_code
        );

        if (_current_byte_code->code == BYTE_CODE_END_CODE_BLOCK) {

            _byte_code_block = (byte_code::Byte_Code_Block*) malloc(sizeof(byte_code::Byte_Code_Block));

            new (_byte_code_block) byte_code::Byte_Code_Block(
                _byte_code_linked_list
            );

            _blocks->add(
                _byte_code_block
            );

            _byte_code_linked_list = new utils::Linked_List <byte_code::Byte_Code*>();

        }

    }

    delete _byte_code_linked_list;

    fclose(_file);

    byte_code::Compiled_Code* _compiled_code = new byte_code::Compiled_Code(_blocks, NULL);

    return _compiled_code;

}

void byte_code::Compiled_Code::print() {

    std::cout << "\n\nCompiled Byte Code:\n" << std::endl;

    for (int _ = 0; _ < blocks->count; _++) {

        std::cout << "\tBlock -> " << _ << std::endl << std::endl;

        for (int __ = 0; __ < blocks->operator[](_)->block->count; __++)

            std::cout << "\t\tByte Code -> " << (int) blocks->operator[](_)->block->operator[](__)->code << " " 
                << blocks->operator[](_)->block->operator[](__)->argument << std::endl;

        std::cout << std::endl;

    }

}


