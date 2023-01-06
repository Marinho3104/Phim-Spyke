
#define COMMON "common.h"

#include COMMON

#include <iostream>

#include "linked_List.h"
#include "./compiler.h"

#include "memory.h"

#include "byte_code.h"
 
#include "byte_code_store.h"
#include "program.h"

struct ola {

    static void test(ola*);
    
    void test();

};

void ola::test(ola*) {

}
    
void ola::test() {
    
}


int main() {

    char* _contract_data = utils::getFileContent((char*) "./test/my_contract.ph");

    compiler::Compiler* _compiler = new compiler::Compiler(_contract_data);

    delete _compiler;

    free(_contract_data);

    std::cout << "Vm:" << std::endl;

    byte_code::Compiled_Code* _compiled_code_readed = byte_code::Compiled_Code::getByFile();

    // _compiled_code_readed->print();

    // exit(1);

    virtual_machine::Program* _program = new virtual_machine::Program(_compiled_code_readed, _compiled_code_readed->blocks->count - 1);

    _program->execute();

    delete _compiled_code_readed;

    delete _program;

}