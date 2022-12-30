#pragma once

#ifndef BUILT_IN_HELPER_H
#define BUILT_IN_HELPER_H

namespace built_ins {

    char* getStructNameOfTokenId(int);

    int getPrimitiveTypeSize(char*);

    char* getFunctionNameFromTokenId(int);

}

#endif