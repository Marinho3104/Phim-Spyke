#include "byte_code_execution_helper.h"

#include "linked_List.h"
#include "execution.h"
#include "program.h"
#include "memory.h"
#include "stack.h"

#include <iostream>
#include <string.h>

void virtual_machine::binary_add_1_bytes(Execution* __execution) {

    void* _to_add = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());
    void* _to_add_1 = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());
    void* _save = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());

    int _result = *((char*) _to_add) + *((char*) _to_add_1);

    std::cout << "First value to add ->" << *((char*) _to_add) << std::endl;
    std::cout << "Second value to add ->" << *((char*) _to_add_1) << std::endl;
    std::cout << "Result value before ->" << *((char*) _save) << std::endl;

    memcpy(
        _save,
        &_result,
        4
    );

    std::cout << "Result value after ->" << *((char*) _save) << std::endl;

}

void virtual_machine::binary_add_4_bytes(Execution* __execution) {

    void* _to_add = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());
    void* _to_add_1 = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());
    void* _save = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());

    int _result = *((int*) _to_add) + *((int*) _to_add_1);

    std::cout << "First value to add ->" << *((int*) _to_add) << std::endl;
    std::cout << "Second value to add ->" << *((int*) _to_add_1) << std::endl;
    std::cout << "Result value before ->" << *((int*) _save) << std::endl;

    memcpy(
        _save,
        &_result,
        4
    );

    std::cout << "Result value after ->" << *((int*) _save) << std::endl;

}


void virtual_machine::binary_sub_1_bytes(Execution* __execution) {

    void* _to_sub = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());
    void* _to_sub_1 = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());
    void* _save = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());

    int _result = *((int*) _to_sub) - *((int*) _to_sub_1);

    std::cout << "First value to add ->" << *((int*) _to_sub) << std::endl;
    std::cout << "Second value to add ->" << *((int*) _to_sub_1) << std::endl;
    std::cout << "Result value before ->" << *((int*) _save) << std::endl;

    memcpy(
        _save,
        &_result,
        4
    );

    std::cout << "Result value after ->" << *((int*) _save) << std::endl;

}

void virtual_machine::binary_sub_4_bytes(Execution* __execution) {

    void* _to_sub = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());
    void* _to_sub_1 = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());
    void* _save = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());

    int _result = *((char*) _to_sub) - *((char*) _to_sub_1);

    std::cout << "First value to add ->" << *((char*) _to_sub) << std::endl;
    std::cout << "Second value to add ->" << *((char*) _to_sub_1) << std::endl;
    std::cout << "Result value before ->" << *((char*) _save) << std::endl;

    memcpy(
        _save,
        &_result,
        4
    );

    std::cout << "Result value after ->" << *((char*) _save) << std::endl;

}


void virtual_machine::binary_equal_to_1_bytes(Execution* __execution) {

    void* _to_sub = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());
    void* _to_sub_1 = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());
    void* _save = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());

    bool _result = *((char*) _to_sub) == *((char*) _to_sub_1);

    std::cout << "First value to add ->" << *((char*) _to_sub) << std::endl;
    std::cout << "Second value to add ->" << *((char*) _to_sub_1) << std::endl;
    std::cout << "Result value before ->" << *((bool*) _save) << std::endl;

    memcpy(
        _save,
        &_result,
        1
    );

    std::cout << "Result value after ->" << *((bool*) _save) << std::endl;
    
}

void virtual_machine::binary_equal_to_4_bytes(Execution* __execution) {

    void* _to_sub = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());
    void* _to_sub_1 = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());
    void* _save = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());

    bool _result = *((int*) _to_sub) == *((int*) _to_sub_1);

    std::cout << "First value to add ->" << *((int*) _to_sub) << std::endl;
    std::cout << "Second value to add ->" << *((int*) _to_sub_1) << std::endl;
    std::cout << "Result value before ->" << *((bool*) _save) << std::endl;

    memcpy(
        _save,
        &_result,
        1
    );

    std::cout << "Result value after ->" << *((bool*) _save) << std::endl;

}

void virtual_machine::binary_mul_1_bytes(Execution* __execution) {

    void* _to_add = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());
    void* _to_add_1 = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());
    void* _save = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());

    int _result = *((char*) _to_add) * *((char*) _to_add_1);

    std::cout << "First value to add ->" << *((char*) _to_add) << std::endl;
    std::cout << "Second value to add ->" << *((char*) _to_add_1) << std::endl;
    std::cout << "Result value before ->" << *((char*) _save) << std::endl;

    memcpy(
        _save,
        &_result,
        4
    );

    std::cout << "Result value after ->" << *((char*) _save) << std::endl;

}

void virtual_machine::binary_mul_4_bytes(Execution* __execution) {

    void* _to_add = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());
    void* _to_add_1 = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());
    void* _save = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());

    int _result = *((int*) _to_add) * *((int*) _to_add_1);

    std::cout << "First value to add ->" << *((int*) _to_add) << std::endl;
    std::cout << "Second value to add ->" << *((int*) _to_add_1) << std::endl;
    std::cout << "Result value before ->" << *((int*) _save) << std::endl;

    memcpy(
        _save,
        &_result,
        4
    );

    std::cout << "Result value after ->" << *((int*) _save) << std::endl;


}


void virtual_machine::binary_div_1_bytes(Execution* __execution) {

    void* _to_add = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());
    void* _to_add_1 = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());
    void* _save = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());

    int _result = *((char*) _to_add) * *((char*) _to_add_1);

    std::cout << "First value to add ->" << *((char*) _to_add) << std::endl;
    std::cout << "Second value to add ->" << *((char*) _to_add_1) << std::endl;
    std::cout << "Result value before ->" << *((char*) _save) << std::endl;

    memcpy(
        _save,
        &_result,
        4
    );

    std::cout << "Result value after ->" << *((char*) _save) << std::endl;

}

void virtual_machine::binary_div_4_bytes(Execution* __execution) {

    void* _to_add = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());
    void* _to_add_1 = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());
    void* _save = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());

    int _result = *((int*) _to_add) / *((int*) _to_add_1);

    std::cout << "First value to add ->" << *((int*) _to_add) << std::endl;
    std::cout << "Second value to add ->" << *((int*) _to_add_1) << std::endl;
    std::cout << "Result value before ->" << *((int*) _save) << std::endl;

    memcpy(
        _save,
        &_result,
        4
    );

    std::cout << "Result value after ->" << *((int*) _save) << std::endl;


}


void virtual_machine::binary_mod_1_bytes(Execution* __execution) {

    void* _to_add = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());
    void* _to_add_1 = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());
    void* _save = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());

    int _result = *((char*) _to_add) % *((char*) _to_add_1);

    std::cout << "First value to add ->" << *((char*) _to_add) << std::endl;
    std::cout << "Second value to add ->" << *((char*) _to_add_1) << std::endl;
    std::cout << "Result value before ->" << *((char*) _save) << std::endl;

    memcpy(
        _save,
        &_result,
        4
    );

    std::cout << "Result value after ->" << *((char*) _save) << std::endl;

}

void virtual_machine::binary_mod_4_bytes(Execution* __execution) {

    void* _to_add = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());
    void* _to_add_1 = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());
    void* _save = __execution->program->memory->getRealAddress(__execution->stacks->last->object->popFromStack());

    int _result = *((int*) _to_add) % *((int*) _to_add_1);

    std::cout << "First value to add ->" << *((int*) _to_add) << std::endl;
    std::cout << "Second value to add ->" << *((int*) _to_add_1) << std::endl;
    std::cout << "Result value before ->" << *((int*) _save) << std::endl;

    memcpy(
        _save,
        &_result,
        4
    );

    std::cout << "Result value after ->" << *((int*) _save) << std::endl;


}




