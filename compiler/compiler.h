#pragma once

#ifndef COMPILER_H
#define COMPILER_H

#include "./compiler_definitions.h"

namespace compiler {

    struct Compiler {

        char* code;

        ~Compiler(); Compiler(char*);

        void generateBuiltIns();

    };

}

#endif

