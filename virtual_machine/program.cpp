#include "program.h"

#include "byte_code_store.h"
#include "execution.h"
#include "byte_code.h"
#include "execution.h"
#include "opcodes.h"
#include "memory.h"

#include <iostream>


virtual_machine::Program::~Program() { delete memory; delete byte_code; delete execution; }

virtual_machine::Program::Program(byte_code::Compiled_Code* __compiled_code, int __entry_point) : entry_point(__entry_point) {

    memory = new Memory(__compiled_code->implicit_values_data);

    byte_code = new Byte_Code_Store(__compiled_code);

    execution = new Execution(this);

}

void virtual_machine::Program::execute() { execution->execute(); }

