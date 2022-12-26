#include "./built_ins_helper.h"

#include "token_definitions.h"

#include <iostream>
#include <string.h>

bool built_ins::isPrimitiveTypeName(const char* __name) {

    if (!strcmp(__name, "Byte")) return 1;
    if (!strcmp(__name, "Int")) return 1;

    return 0;

}

char* built_ins::getFunctionNameFromFunctionOperatorId(int __token_id) {

    char* _function_name = NULL;

    switch (__token_id)
    {
    case FUNCTION_OPERATOR_PLUS:

        _function_name = (char*) malloc(4);

        strcpy(_function_name, (char*) "add");

        return _function_name;
        
        break;
    
    default: break;
    }

    std::cout << "Error getting Function Name" << std::endl;

    throw "";

    return NULL;

}

