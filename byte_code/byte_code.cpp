#include "byte_code.h"

#include "linked_List.h"
#include "opcodes.h"

#include "tokenizer_helper.h"

#include <iostream>
#include <string.h>


byte_code::Byte_Code::~Byte_Code() {}

byte_code::Byte_Code::Byte_Code(char __code, int __argument) : code(__code), argument(__argument) {}


byte_code::Byte_Code_Block::~Byte_Code_Block() { delete block; }

byte_code::Byte_Code_Block::Byte_Code_Block(utils::Linked_List <Byte_Code*>* __block) : block(__block), current_allocation_size(0) {}


byte_code::Compiled_Code::~Compiled_Code() { delete blocks; free(implicit_values_data); }

byte_code::Compiled_Code::Compiled_Code(utils::Linked_List <Byte_Code_Block*>* __blocks, void* __implicit_values_data) 
    : blocks(__blocks), implicit_values_data(__implicit_values_data), implicit_data_length(0) {}

byte_code::Compiled_Code::Compiled_Code(utils::Linked_List <Byte_Code_Block*>* __blocks, utils::Linked_List <char*>* __implicit_values)
    : blocks(__blocks), implicit_data_length(0) {

        for (int _ = 0; _ < __implicit_values->count; _++) {

            if (parser::isInt(__implicit_values->operator[](_))) implicit_data_length += 4;

            else implicit_data_length += strlen(__implicit_values->operator[](_));
        
        }

        implicit_values_data = malloc(implicit_data_length);
        void* _copy = implicit_values_data;

        char* _data;
        int _length;

        for (int _ = 0; _ < __implicit_values->count; _++) {

            _data = __implicit_values->operator[](_);

            _length = strlen(_data);

            if (parser::isInt(_data)) {

                int _value_temp = atoi(_data);

                _data = (char*) &_value_temp;

                _length = 4;

            }

            memcpy(_copy, _data, _length);

            _copy = _copy + _length;

        }

}

void byte_code::Compiled_Code::saveInFile() {

    FILE* _file;

    _file = fopen("byte_code.marinho", "wb");

    for (int _ = 0; _ < blocks->count; _++)

        for (int __ = 0;__ < blocks->operator[](_)->block->count; __++) {

            fwrite(&blocks->operator[](_)->block->operator[](__)->code, 1, 1, _file);
            fwrite(&blocks->operator[](_)->block->operator[](__)->argument, 4, 1, _file);

        }

    char _code = BYTE_CODE_END_CODE_BLOCK;
    int _argument = 0;

    fwrite(&_code, 1, 1, _file);
    fwrite(&_argument, 4, 1, _file);

    fwrite(implicit_values_data, implicit_data_length, 1, _file);

    fclose(_file);

}

byte_code::Compiled_Code* byte_code::Compiled_Code::getByFile() {

    FILE* _file = fopen("byte_code.marinho", "rb");

    utils::Linked_List <byte_code::Byte_Code*>* _byte_code_block = new utils::Linked_List <byte_code::Byte_Code*>();
    utils::Linked_List <Byte_Code_Block*>* _blocks = new utils::Linked_List <Byte_Code_Block*>();

    byte_code::Byte_Code* _current_byte_code;
    byte_code::Byte_Code_Block* _block; 

    int _argument;
    char _code;

    fseek(_file, 0, SEEK_END);

    int _lenght = ftell(_file);

    fseek(_file, 0, SEEK_SET);

    while(fread(&_code, 1, 1, _file)) {

        fread(&_argument, 4, 1, _file);

        _current_byte_code = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

        new (_current_byte_code) byte_code::Byte_Code(
            _code, _argument
        );

        _byte_code_block->add(
            _current_byte_code
        );

        if (_current_byte_code->code == BYTE_CODE_END_CODE_BLOCK) {

            if (_byte_code_block->count == 1) break;

            _block = (byte_code::Byte_Code_Block*) malloc(sizeof(byte_code::Byte_Code_Block));

            new (_block) byte_code::Byte_Code_Block(
                _byte_code_block
            );

            _blocks->add(
                _block
            );

            _byte_code_block = new utils::Linked_List <byte_code::Byte_Code*>();

        }

        // std::cout << "Code -> " << (int) _code << " Argument -> " << _argument << std::endl;

    }

    int _current_position = ftell(_file);

    void* _implicit_values_data = malloc(4096);

    for (int _ = 0; _ < 4096; _++)

        ((char*) _implicit_values_data)[_] = 0; // Not needed

    fread(_implicit_values_data, _lenght - _current_position - 1, 1, _file); // error when no implicit values TODO

    byte_code::Compiled_Code* _compiled_code = new byte_code::Compiled_Code(
        _blocks, _implicit_values_data
    );

    delete _byte_code_block;

    fclose(_file);

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


