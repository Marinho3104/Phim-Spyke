
#include <iostream>

#include "./compiler.h"

int main() {

    compiler::Compiler* _compiler = new compiler::Compiler((char*) "test");

    delete _compiler;

}