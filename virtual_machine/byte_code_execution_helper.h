#pragma once

#ifndef BYTE_CODE_EXECUTION_HELPER_H
#define BYTE_CODE_EXECUTION_HELPER_H

#include "virtual_machine_definitions.h"

namespace virtual_machine {

    void binary_add_1_bytes(Execution*);

    void binary_add_2_bytes(Execution*);

    void binary_add_4_bytes(Execution*);


    void binary_sub_1_bytes(Execution*);

    void binary_sub_2_bytes(Execution*);

    void binary_sub_4_bytes(Execution*);


    void binary_equal_to_1_bytes(Execution*);

    void binary_equal_to_2_bytes(Execution*);

    void binary_equal_to_4_bytes(Execution*);


    void binary_mul_1_bytes(Execution*);

    void binary_mul_2_bytes(Execution*);

    void binary_mul_4_bytes(Execution*);


    void binary_div_1_bytes(Execution*);

    void binary_div_2_bytes(Execution*);

    void binary_div_4_bytes(Execution*);


    void binary_mod_1_bytes(Execution*);

    void binary_mod_2_bytes(Execution*);

    void binary_mod_4_bytes(Execution*);


    void binary_inc_1_bytes(Execution*);

    void binary_inc_2_bytes(Execution*);

    void binary_inc_4_bytes(Execution*);


    void binary_dec_1_bytes(Execution*);

    void binary_dec_2_bytes(Execution*);

    void binary_dec_4_bytes(Execution*);

}

#endif