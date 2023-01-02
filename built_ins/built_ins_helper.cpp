#include "./built_ins_helper.h"

#include "built_ins_definitions.h"
#include "token_definitions.h"

#include <iostream>
#include <string.h>


char* built_ins::getStructNameOfTokenId(int __token_id) {

    char* _struct_name = NULL;

    switch (__token_id)
    {
    case PRIMITIVE_TYPE_BYTE:

        _struct_name = (char*) malloc(5);

        strcpy(_struct_name, PRIMITIVE_TYPE_BYTE_NAME);

        break;

    case PRIMITIVE_TYPE_INT: case IMPLICIT_VALUE_INT:

        _struct_name = (char*) malloc(4);

        strcpy(_struct_name, PRIMITIVE_TYPE_INT_NAME);
        
        break;

    default: break;
    }

    return _struct_name;

}

int built_ins::getPrimitiveTypeSize(char* __name) {

    if (!strcmp((const char*) __name, PRIMITIVE_TYPE_BYTE_NAME)) return PRIMITIVE_TYPE_BYTE_SIZE;
    if (!strcmp((const char*) __name, PRIMITIVE_TYPE_INT_NAME)) return PRIMITIVE_TYPE_INT_SIZE;

    return -1;

} 

char* built_ins::getFunctionNameFromTokenId(int __token_id) {

    char* _function_name;

    switch (__token_id)
    {
    case FUNCTION_OPERATOR_PLUS:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_PLUS) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_PLUS);

        break;
    case FUNCTION_OPERATOR_MULTIPLICATION:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_NULTIPLICATION) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_NULTIPLICATION);

        break;
    
    default: std::cout << "Error getting Function Name for token -> " << __token_id << std::endl; exit(1); break;
    }

    return _function_name;

}    





