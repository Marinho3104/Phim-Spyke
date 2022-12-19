#pragma once

#ifndef COMPILER_H
#define COMPILER_H

#include "./compiler_definitions.h"

namespace compiler {

    struct Compiler {

        ~Compiler(); Compiler(char*);

        void generateBuiltIns();

    };

}

#endif

