#include "tokenizer_helper.h"

#include "token_definitions.h"
#include "linked_List.h"
#include "tokenizer.h"
#include "token.h"

#include <iostream>
#include <string.h>

int parser::getTokenSymbol(const char*& __current_position) {

    if (!strncmp(__current_position, "<<=", 3)) { __current_position += 3; return FUNCTION_OPERATOR_BITWISE_LEFT_SHIFT_ASSIGN; }
    if (!strncmp(__current_position, ">>=", 3)) { __current_position += 3; return FUNCTION_OPERATOR_BITWISE_RIGHT_SHIFT_ASSIGN; }

    if (!strncmp(__current_position, "::", 2)) { __current_position += 2; return DOUBLE_COLON; }
    if (!strncmp(__current_position, "//", 2)) { __current_position += 2; return COMMENT_LINE; }
    if (!strncmp(__current_position, "/*", 2)) { __current_position += 2; return COMMENT; }
    if (!strncmp(__current_position, "*/", 2)) { __current_position += 2; return COMMENT_END; }

    if (!strncmp(__current_position, "+=", 2)) { __current_position += 2; return FUNCTION_OPERATOR_PLUS_ASSIGN; }
    if (!strncmp(__current_position, "-=", 2)) { __current_position += 2; return FUNCTION_OPERATOR_MINUS_ASSIGN; }
    if (!strncmp(__current_position, "*=", 2)) { __current_position += 2; return FUNCTION_OPERATOR_MULTIPLICATION_ASSIGN; }
    if (!strncmp(__current_position, "/=", 2)) { __current_position += 2; return FUNCTION_OPERATOR_DIVISION_ASSIGN; }
    if (!strncmp(__current_position, "%=", 2)) { __current_position += 2; return FUNCTION_OPERATOR_MODULOS_ASSIGN; }

    if (!strncmp(__current_position, "++", 2)) { __current_position += 2; return FUNCTION_OPERATOR_INCREMENT; }
    if (!strncmp(__current_position, "--", 2)) { __current_position += 2; return FUNCTION_OPERATOR_DECREMENT; }

    if (!strncmp(__current_position, "&&", 2)) { __current_position += 2; return FUNCTION_OPERATOR_AND; }
    if (!strncmp(__current_position, "||", 2)) { __current_position += 2; return FUNCTION_OPERATOR_OR; }

    if (!strncmp(__current_position, "==", 2)) { __current_position += 2; return FUNCTION_OPERATOR_EQUAL_TO; }
    if (!strncmp(__current_position, "!=", 2)) { __current_position += 2; return FUNCTION_OPERATOR_NOT_EQUAL_TO; }
    if (!strncmp(__current_position, ">=", 2)) { __current_position += 2; return FUNCTION_OPERATOR_GREATER_THAN_EQUAL_TO; }
    if (!strncmp(__current_position, "<=", 2)) { __current_position += 2; return FUNCTION_OPERATOR_LESS_THAN_EQUAL_TO; }

    if (!strncmp(__current_position, "<<", 2)) { __current_position += 2; return FUNCTION_OPERATOR_BITWISE_LEFT_SHIFT; }
    if (!strncmp(__current_position, ">>", 2)) { __current_position += 2; return FUNCTION_OPERATOR_BITWISE_RIGHT_SHIFT; }

    if (!strncmp(__current_position, "&=", 2)) { __current_position += 2; return FUNCTION_OPERATOR_BITWISE_AND_ASSIGN; }
    if (!strncmp(__current_position, "|=", 2)) { __current_position += 2; return FUNCTION_OPERATOR_BITWISE_OR_ASSIGN; }
    if (!strncmp(__current_position, "^=", 2)) { __current_position += 2; return FUNCTION_OPERATOR_BITWISE_XOR_ASSIGN; }

    if (!strncmp(__current_position, "b+", 2)) { __current_position += 2; return BINARY_ADDITION; }
    if (!strncmp(__current_position, "b-", 2)) { __current_position += 2; return BINARY_SUBTRACTION; }
    if (!strncmp(__current_position, "b*", 2)) { __current_position += 2; return BINARY_MULTIPLICATION; }
    if (!strncmp(__current_position, "b/", 2)) { __current_position += 2; return BINARY_DIVISION; }
    if (!strncmp(__current_position, "b%", 2)) { __current_position += 2; return BINARY_MODULOS; }

    if (!strncmp(__current_position, "->", 2)) { __current_position += 2; return ACCESSING_POINTER; }

    if (!strncmp(__current_position, ";", 1)) { __current_position += 1; return END_INSTRUCTION; }

    if (!strncmp(__current_position, "(", 1)) { __current_position += 1; return OPEN_PARENTHESIS; }
    if (!strncmp(__current_position, ")", 1)) { __current_position += 1; return CLOSE_PARENTHESIS; }

    if (!strncmp(__current_position, "{", 1)) { __current_position += 1; return OPEN_BRACES; }
    if (!strncmp(__current_position, "}", 1)) { __current_position += 1; return CLOSE_BRACES; }

    if (!strncmp(__current_position, "[", 1)) { __current_position += 1; return OPEN_BRACKET; }
    if (!strncmp(__current_position, "]", 1)) { __current_position += 1; return CLOSE_BRACKET; }

    if (!strncmp(__current_position, ",", 1)) { __current_position += 1; return COMMA; }
    if (!strncmp(__current_position, ":", 1)) { __current_position += 1; return COLON; }
    if (!strncmp(__current_position, "?", 1)) { __current_position += 1; return QUESTION_MARK; }
    if (!strncmp(__current_position, "\"", 1)) { __current_position += 1; return QUOTATION_MARK; }
    if (!strncmp(__current_position, "'", 1)) { __current_position += 1; return SINGLE_QUOTATION_MARK; }
    if (!strncmp(__current_position, "#", 1)) { __current_position += 1; return PRE_PROCESSOR; }

    if (!strncmp(__current_position, "*", 1)) { __current_position += 1; return POINTER; }
    if (!strncmp(__current_position, "&", 1)) { __current_position += 1; return ADDRESS; }

    if (!strncmp(__current_position, "+", 1)) { __current_position++; return FUNCTION_OPERATOR_PLUS; }
    if (!strncmp(__current_position, "-", 1)) { __current_position++; return FUNCTION_OPERATOR_MINUS; }
    if (!strncmp(__current_position, "*", 1)) { __current_position++; return FUNCTION_OPERATOR_MULTIPLICATION; } // Never reatch
    if (!strncmp(__current_position, "/", 1)) { __current_position++; return FUNCTION_OPERATOR_DIVISION; }
    if (!strncmp(__current_position, "%", 1)) { __current_position++; return FUNCTION_OPERATOR_MODULOS; }

    if (!strncmp(__current_position, "=", 1)) { __current_position++; return FUNCTION_OPERATOR_EQUAL; }

    if (!strncmp(__current_position, "!", 1)) { __current_position++; return FUNCTION_OPERATOR_NOT; }

    if (!strncmp(__current_position, ">", 1)) { __current_position += 1; return FUNCTION_OPERATOR_GREATER_THAN; }
    if (!strncmp(__current_position, "<", 1)) { __current_position += 1; return FUNCTION_OPERATOR_LESS_THAN; }

    if (!strncmp(__current_position, "~", 1)) { __current_position += 1; return FUNCTION_OPERATOR_BITWISE_NOT; }

    if (!strncmp(__current_position, "&", 1)) { __current_position += 1; return FUNCTION_OPERATOR_BITWISE_AND; } // Never reatch
    if (!strncmp(__current_position, "|", 1)) { __current_position += 1; return FUNCTION_OPERATOR_BITWISE_OR; }
    if (!strncmp(__current_position, "^", 1)) { __current_position += 1; return FUNCTION_OPERATOR_BITWISE_XOR; }

    if (!strncmp(__current_position, ".", 1)) { __current_position += 1; return ACCESSING; }

    return 0;

}

int parser::getTokenKeyWord(const char*& __current_position) {

    if (!strncmp(__current_position, "void", 4)) { __current_position += 4; return PRIMITIVE_TYPE_VOID; }
    if (!strncmp(__current_position, "byte", 4)) { __current_position += 4; return PRIMITIVE_TYPE_BYTE; }
    if (!strncmp(__current_position, "int", 3)) { __current_position += 3; return PRIMITIVE_TYPE_INT; }
    if (!strncmp(__current_position, "bool", 4)) { __current_position += 4; return PRIMITIVE_TYPE_BOOL; }
    if (!strncmp(__current_position, "pointer", 7)) { __current_position += 7; return PRIMITIVE_TYPE_POINTER; }
    if (!strncmp(__current_position, "spyke", 5)) { __current_position += 5; return PRIMITIVE_TYPE_SPYKE; }
    
    if (!strncmp(__current_position, "namespace", 9)) { __current_position += 9; return NAMESPACE; }
    if (!strncmp(__current_position, "struct", 6)) { __current_position += 6; return STRUCT; }
    if (!strncmp(__current_position, "contract", 8)) { __current_position += 8; return CONTRACT; }
    if (!strncmp(__current_position, "static", 6)) { __current_position += 6; return STATIC; }
    if (!strncmp(__current_position, "exec", 4)) { __current_position += 4; return BYTE_CODE; }
    if (!strncmp(__current_position, "return", 6)) { __current_position += 6; return RETURN; }

    if (tokenizer_control->tokens_collection->count && !strncmp(__current_position - 1, "#define", 7)) { __current_position += 6; return PRE_COMPILER_DEFINE; }
    if (tokenizer_control->tokens_collection->count && !strncmp(__current_position - 1, "#include", 8)) { __current_position += 7; return PRE_COMPILER_INCLUDE; }
    if (tokenizer_control->tokens_collection->count && !strncmp(__current_position - 1, "#ifdef", 6)) { __current_position += 5; return PRE_COMPILER_IF_DEF; }
    if (tokenizer_control->tokens_collection->count && !strncmp(__current_position - 1, "#ifndef", 7)) { __current_position += 6; return PRE_COMPILER_IF_N_DEF; }
    if (tokenizer_control->tokens_collection->count && !strncmp(__current_position - 1, "#else", 5)) { __current_position += 4; return PRE_COMPILER_ELSE; }
    if (tokenizer_control->tokens_collection->count && !strncmp(__current_position - 1, "#endif", 6)) { __current_position += 5; return PRE_COMPILER_END_IF; }

    if (!strncmp(__current_position, "if", 2)) { __current_position += 2; return IF; }
    if (!strncmp(__current_position, "else if", 7)) { __current_position += 7; return ELSE_IF; }
    if (!strncmp(__current_position, "else", 4)) { __current_position += 4; return ELSE; }
    if (!strncmp(__current_position, "sizeof", 6)) { __current_position += 6; return SIZE_OF; }
    if (!strncmp(__current_position, "while", 5)) { __current_position += 5; return WHILE; }
    if (!strncmp(__current_position, "do", 2)) { __current_position += 2; return DO; }
    if (!strncmp(__current_position, "break", 5)) { __current_position += 5; return BREAK; }
    if (!strncmp(__current_position, "continue", 8)) { __current_position += 8; return CONTINUE; }

    return 0;

}

void parser::handleCommentLine(char*& __current_position) { while(*(__current_position)++ != '\n'); }

void parser::handleComment(char*& __current_position) { while(*(__current_position)++ != '*' || *(__current_position) != '/'); __current_position++; }

void parser::handleQuotationMark(Token* __token, char*& __current_position) {

    char* _backup_state = __current_position;

    while( *(__current_position - 1) == '\\' || *__current_position != '"' ) __current_position++;

    char* _data = (char*) malloc(__current_position - _backup_state + 1);

    strncpy(_data, _backup_state, __current_position - _backup_state);

    _data[__current_position - _backup_state] = '\0';

    new (__token) Token(
        IMPLICIT_VALUE_STRING, 
        _data,
        _backup_state - tokenizer_control->inicial_column_address, 
        __current_position - tokenizer_control->inicial_column_address, 
        tokenizer_control->current_line
    );

    __current_position++;

}

void parser::handleSingleQuotationMark(Token* __token, char*& __current_position) {

    char* _backup_state = __current_position;

    while( *(__current_position - 1) == '\\' || *__current_position != '\'' ) __current_position++;

    char* _data = (char*) malloc(__current_position - _backup_state + 1);

    strncpy(_data, _backup_state, __current_position - _backup_state);

    _data[__current_position - _backup_state] = '\0';

    new (__token) Token(
        IMPLICIT_VALUE_CHARACTER,
        _data, 
        _backup_state - tokenizer_control->inicial_column_address, 
        __current_position - tokenizer_control->inicial_column_address, 
        tokenizer_control->current_line
    );

    __current_position++;

}

void parser::handlePointerOrAddress(Token* __token) {

    if (parser::isImplicitValueOrIdentifier(parser::tokenizer_control->tokens_collection->last->object->id) || 
        parser::tokenizer_control->tokens_collection->last->object->id == CLOSE_PARENTHESIS) // Not best way

        __token->id = __token->id == POINTER ? FUNCTION_OPERATOR_MULTIPLICATION : FUNCTION_OPERATOR_BITWISE_AND;

}

int parser::getImplicitValueTypeOrIdentifier(char* __data) {

    if (isInt(__data)) return IMPLICIT_VALUE_INT;


    return IDENTIFIER;

}


bool parser::isPrimitive(int __token_id) { return __token_id >= PRIMITIVE_TYPE_VOID && __token_id <= PRIMITIVE_TYPE_BOOL || __token_id == PRIMITIVE_TYPE_POINTER || __token_id == PRIMITIVE_TYPE_SPYKE; }

bool parser::isImplicitValueOrIdentifier(int __token_id) { return __token_id >= IDENTIFIER && __token_id <= IMPLICIT_VALUE_STRING; }

bool parser::isAccessingOperator(int __token_id) { return __token_id == ACCESSING || __token_id == ACCESSING_POINTER; }

bool parser::isFunctionOperator(int __token_id) { return __token_id >= FUNCTION_OPERATOR_PLUS && __token_id <= FUNCTION_OPERATOR_BITWISE_RIGHT_SHIFT_ASSIGN || __token_id == FUNCTION_OPERATOR_EQUAL || __token_id == OPEN_BRACKET || __token_id == FUNCTION_OPERATOR_BITWISE_NOT; }

bool parser::isFunctionOperatorSingleArgument(int __token_id) {

    return __token_id == FUNCTION_OPERATOR_INCREMENT || __token_id == FUNCTION_OPERATOR_DECREMENT || __token_id == FUNCTION_OPERATOR_NOT || __token_id == FUNCTION_OPERATOR_BITWISE_NOT;

}


bool parser::isInt(char* __data) {

    while(*__data) {

        if (
            (*__data < 48 || *__data > 57) // && *(__data) != 95
        ) return 0;

        __data++;
        
    }

    return 1;

}
