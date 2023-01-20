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

    case PRIMITIVE_TYPE_BOOL:

        _struct_name = (char*) malloc(5);

        strcpy(_struct_name, PRIMITIVE_TYPE_BOOL_NAME);
        
        break;
    case PRIMITIVE_TYPE_VOID:

        _struct_name = (char*) malloc(5);

        strcpy(_struct_name, PRIMITIVE_TYPE_VOID_NAME);
        
        break;
    case PRIMITIVE_TYPE_POINTER:

        _struct_name = (char*) malloc(8);

        strcpy(_struct_name, PRIMITIVE_TYPE_POINTER_NAME);
        
        break;
    case PRIMITIVE_TYPE_SPYKE:

        _struct_name = (char*) malloc(6);

        strcpy(_struct_name, PRIMITIVE_TYPE_SPYKE_NAME);
        
        break;

    default: break;
    }

    return _struct_name;

}

int built_ins::getPrimitiveTypeSize(char* __name) {

    if (!__name) return -1;

    if (!strcmp((const char*) __name, PRIMITIVE_TYPE_BYTE_NAME)) return PRIMITIVE_TYPE_BYTE_SIZE;
    if (!strcmp((const char*) __name, PRIMITIVE_TYPE_INT_NAME)) return PRIMITIVE_TYPE_INT_SIZE;
    if (!strcmp((const char*) __name, PRIMITIVE_TYPE_VOID_NAME)) return PRIMITIVE_TYPE_VOID_SIZE;
    if (!strcmp((const char*) __name, PRIMITIVE_TYPE_BOOL_NAME)) return PRIMITIVE_TYPE_BOOL_SIZE;
    if (!strcmp((const char*) __name, PRIMITIVE_TYPE_POINTER_NAME)) return PRIMITIVE_TYPE_POINTER_SIZE;
    if (!strcmp((const char*) __name, PRIMITIVE_TYPE_SPYKE_NAME)) return PRIMITIVE_TYPE_SPYKE_SIZE;

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
    case FUNCTION_OPERATOR_MINUS:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_MINUS) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_MINUS);

        break;
    case FUNCTION_OPERATOR_MULTIPLICATION:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_MULTIPLICATION) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_MULTIPLICATION);

        break;
    case FUNCTION_OPERATOR_DIVISION:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_DIVISION) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_DIVISION);

        break;
    case FUNCTION_OPERATOR_MODULOS:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_MODULOS) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_MODULOS);

        break;
    case FUNCTION_OPERATOR_EQUAL:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_EQUAL) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_EQUAL);

        break;  
    case FUNCTION_OPERATOR_PLUS_ASSIGN:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_PLUS_ASSIGN) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_PLUS_ASSIGN);

        break; 
    case FUNCTION_OPERATOR_MINUS_ASSIGN:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_MINUS_ASSIGN) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_MINUS_ASSIGN);

        break; 
    case FUNCTION_OPERATOR_MULTIPLICATION_ASSIGN:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_MULTIPLICATION_ASSIGN) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_MULTIPLICATION_ASSIGN);

        break; 
    case FUNCTION_OPERATOR_DIVISION_ASSIGN:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_DIVISION_ASSIGN) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_DIVISION_ASSIGN);

        break; 
    case FUNCTION_OPERATOR_MODULOS_ASSIGN:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_MODULOS_ASSIGN) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_MODULOS_ASSIGN);

        break; 
    case FUNCTION_OPERATOR_EQUAL_TO:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_EQUAL_TO) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_EQUAL_TO);

        break; 
    case FUNCTION_OPERATOR_NOT_EQUAL_TO:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_NOT_EQUAL_TO) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_NOT_EQUAL_TO);

        break; 
    case FUNCTION_OPERATOR_GREATER_THAN:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_GREATER_THAN) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_GREATER_THAN);

        break; 
    case FUNCTION_OPERATOR_LESS_THAN:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_LESS_THAN) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_LESS_THAN);

        break;
    case FUNCTION_OPERATOR_GREATER_THAN_EQUAL_TO:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_GREATER_THAN_EQUAL_TO) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_GREATER_THAN_EQUAL_TO);

        break;  
    case FUNCTION_OPERATOR_LESS_THAN_EQUAL_TO:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_LESS_THAN_EQUAL_TO) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_LESS_THAN_EQUAL_TO);

        break;

    case FUNCTION_OPERATOR_BITWISE_AND:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_BITWISE_AND) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_BITWISE_AND);

        break; 
    case FUNCTION_OPERATOR_BITWISE_OR:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_BITWISE_OR) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_BITWISE_OR);

        break; 
    case FUNCTION_OPERATOR_BITWISE_XOR:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_BITWISE_XOR) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_BITWISE_XOR);

        break;
    case FUNCTION_OPERATOR_BITWISE_LEFT_SHIFT:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_BITWISE_LEFT_SHIFT) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_BITWISE_LEFT_SHIFT);

        break; 
    case FUNCTION_OPERATOR_BITWISE_RIGHT_SHIFT:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_BITWISE_RIGHT_SHIFT) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_BITWISE_RIGHT_SHIFT);

        break; 

    case OPEN_BRACKET:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_BRACKET) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_BRACKET);

        break; 

    case FUNCTION_OPERATOR_INCREMENT:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_INCREMENT) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_INCREMENT);

        break; 

    case FUNCTION_OPERATOR_DECREMENT:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_DECREMENT) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_DECREMENT);

        break; 

    case FUNCTION_OPERATOR_INCREMENT_LEFT:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_INCREMENT_LEFT) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_INCREMENT_LEFT);

        break; 

    case FUNCTION_OPERATOR_DECREMENT_LEFT:
        
        _function_name = (char*) malloc(strlen(FUNCTION_NAME_FUNCTION_OPERATOR_DECREMENT_LEFT) + 1);

        strcpy(_function_name, FUNCTION_NAME_FUNCTION_OPERATOR_DECREMENT_LEFT);

        break; 

    default: std::cout << "Error getting Function Name for token -> " << __token_id << std::endl; exit(1); break;
    }

    return _function_name;

}    


char** built_ins::getPrimitivesNames() {
    
    char** _names = (char**) malloc(8 * 8);

    _names[7] = 0;
    _names[0] = getStructNameOfTokenId(PRIMITIVE_TYPE_VOID);
    _names[1] = getStructNameOfTokenId(PRIMITIVE_TYPE_BYTE);
    _names[2] = getStructNameOfTokenId(PRIMITIVE_TYPE_BOOL);
    _names[3] = getStructNameOfTokenId(PRIMITIVE_TYPE_INT);
    _names[4] = getStructNameOfTokenId(PRIMITIVE_TYPE_SPYKE);
    _names[5] = getStructNameOfTokenId(PRIMITIVE_TYPE_POINTER);
    _names[6] = (char*) malloc(strlen("temp_file") + 1);
    strcpy(
        _names[6],
        "temp_file"
    );

    return _names;

}



