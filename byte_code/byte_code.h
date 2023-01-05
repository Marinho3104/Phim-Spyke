#pragma once

#ifndef BYTE_CODE_H
#define BYTE_CODE_H

#include "byte_code_definitions.h"

namespace byte_code {

    struct Byte_Code {

        char code;
        int argument;

        ~Byte_Code(); Byte_Code(char, int);

    };

    struct Byte_Code_Block {

        utils::Linked_List <Byte_Code*>* block;
        int current_allocation_size;

        ~Byte_Code_Block(); Byte_Code_Block(utils::Linked_List <Byte_Code*>*);

    };

    struct Compiled_Code {

        utils::Linked_List <Byte_Code_Block*>* blocks;
        void* implicit_values_data;
        int implicit_data_length;

        ~Compiled_Code(); Compiled_Code(utils::Linked_List <Byte_Code_Block*>*, void*); 
        
        Compiled_Code(utils::Linked_List <Byte_Code_Block*>*, utils::Linked_List <char*>*);

        void saveInFile();

        static Compiled_Code* getByFile();

        void print();

    };

}

#endif