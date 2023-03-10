#pragma once

#ifndef VIRTUAL_MACHINE_BYTE_CODE_STORE_H
#define VIRTUAL_MACHINE_BYTE_CODE_STORE_H

#include "virtual_machine_definitions.h"
#include "byte_code_definitions.h"

namespace virtual_machine {

    struct Byte_Code_Store {

        byte_code::Byte_Code** blocks;

        ~Byte_Code_Store(); Byte_Code_Store(byte_code::Compiled_Code*);

        byte_code::Byte_Code* getByteCode(int, int);

    };

}

#endif