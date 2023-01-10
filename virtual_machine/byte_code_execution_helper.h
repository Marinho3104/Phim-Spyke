#pragma once

#ifndef BYTE_CODE_EXECUTION_HELPER_H
#define BYTE_CODE_EXECUTION_HELPER_H

#include "virtual_machine_definitions.h"

namespace virtual_machine {

    void binary_add_1_bytes(Execution* __execution);

    void binary_add_4_bytes(Execution* __execution);


    void binary_sub_1_bytes(Execution* __execution);

    void binary_sub_4_bytes(Execution* __execution);


    void binary_equal_to_1_bytes(Execution* __execution);

    void binary_equal_to_4_bytes(Execution* __execution);

}

#endif