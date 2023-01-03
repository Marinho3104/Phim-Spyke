#include "convertor.h"

#include "convertor_helper.h"
#include "linked_List.h"
#include "ast_nodes.h"
#include "byte_code.h"
#include "opcodes.h"
#include "ast.h"

#include <iostream>

parser::Convertor_Control::~Convertor_Control() {}

parser::Convertor_Control::Convertor_Control(bool __debug_mode) : debug_mode(__debug_mode) 
    { byte_code_blocks = new utils::Linked_List <byte_code::Byte_Code_Block*>(); }

void parser::Convertor_Control::print(const char* __information) {

    if (!debug_mode) return;

    std::cout << "\t" << __information << "\n" << std::endl;

}

void parser::Convertor_Control::generate() {

    print("");
    print("\t---> Convertor Control <---");
    print("");

    allocBlock();

    setBlock(parser::ast_control->global_name_space->declarations, NULL);

    setEndByteCode();

}

void parser::Convertor_Control::setBlock(utils::Linked_List<Ast_Node*>* __declarations, byte_code::Byte_Code_Block* __block) {

    byte_code::Byte_Code_Block* _byte_code_block = 
        __block ? __block : byte_code_blocks->last->object;

    block_in_set = _byte_code_block;

    utils::Linked_List <byte_code::Byte_Code*>* _temp;

    for (int _ = 0; _ < __declarations->count; _++) {

        _temp = parser::getByteCodeOfNode(
            __declarations->operator[](_)
        );

        _byte_code_block->block->join(
            _temp
        );

        delete _temp;
        

    }

}

int parser::Convertor_Control::allocBlock() {

    byte_code::Byte_Code_Block* _byte_code_block = (byte_code::Byte_Code_Block*) malloc(sizeof(byte_code::Byte_Code_Block));
    utils::Linked_List <byte_code::Byte_Code*>* _byte_code = new utils::Linked_List <byte_code::Byte_Code*>(), *_temp;

    new (_byte_code_block) byte_code::Byte_Code_Block(
        _byte_code
    );

    return byte_code_blocks->add(
        _byte_code_block
    );

}


byte_code::Compiled_Code* parser::Convertor_Control::getCompiledByteCode() {

    byte_code::Compiled_Code* _compiled_code = new byte_code::Compiled_Code(
        byte_code_blocks, ast_control->implicit_values_collection
    );

    _compiled_code->print();

    delete _compiled_code;

    return NULL;

}

void parser::Convertor_Control::setEndByteCode() {

    byte_code::Byte_Code* _end_code_block;

    for (int _ = 0; _ < byte_code_blocks->count; _++) {

        _end_code_block = (byte_code::Byte_Code*) malloc(sizeof(byte_code::Byte_Code));

        new (_end_code_block) byte_code::Byte_Code(
            BYTE_CODE_END_CODE_BLOCK, 0
        );

        byte_code_blocks->operator[](_)->block->add(
            _end_code_block
        );

    }

}

void parser::Convertor_Control::saveByteCode() {

    FILE* _file;

    _file = fopen("byte_code.marinho", "wb");

    for (int _ = 0; _ < byte_code_blocks->count; _++)

        for (int __ = 0;__ < byte_code_blocks->operator[](_)->block->count; __++) {

            fwrite(&byte_code_blocks->operator[](_)->block->operator[](__)->code, 1, 1, _file);
            fwrite(&byte_code_blocks->operator[](_)->block->operator[](__)->argument, 4, 1, _file);

        }

    fclose(_file);

}