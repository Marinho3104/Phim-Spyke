#pragma once

#ifndef PARSER_TOKENIZER_HELPER_H
#define PARSER_TOKENIZER_HELPER_H

#include "parser_definitions.h"

namespace parser {

    int getTokenSymbol(const char*&);

    int getTokenKeyWord(const char*&);

    void handleCommentLine(char*&);

    void handleComment(char*&);

    void handleQuotationMark(Token*, char*&);
    
    void handleSingleQuotationMark(Token*, char*&);

    void handlePointerOrAddress(Token*);

    int getImplicitValueTypeOrIdentifier(char*);

    bool isPrimitive(int);
    bool isImplicitValueOrIdentifier(int);
    bool isAccessingOperator(int);
    bool isFunctionOperator(int);
    bool isFunctionOperatorSingleArgument(int);

    bool isInt(char*);

}

#endif