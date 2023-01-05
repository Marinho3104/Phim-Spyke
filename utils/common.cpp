#include "./common.h"

#include <iostream>
#include <string.h>
#include <fstream>


char* utils::getFileContent(char* __path) {

    std::ifstream ifs(__path);
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
        (std::istreambuf_iterator<char>()    ) );

    char* _file_content = (char*) malloc(strlen(content.c_str()) + 1);

    strcpy(_file_content, (char*) content.c_str());

    return _file_content;

}

char** utils::splitString(char* __str, char __token) {

    int _appearence = tokenAppearences(__str, __token);

    char** _splitted_tokens = (char**) malloc(8 * (_appearence + 1)), **_return = _splitted_tokens;
    char* _splitted_string;
    int _string_length;

    _splitted_tokens[8 * _appearence] = 0;

    while(*__str) {

        _string_length = tokenPosition(__str, __token);

        _splitted_string = (char*) malloc(_string_length + 1);

        *(_splitted_tokens++) = _splitted_string;

        _splitted_string[_string_length] = '\0';

        strncpy(
            _splitted_string,
            __str,
            _string_length
        );

        __str += _string_length + 1;

    }

    return _return;

}

int utils::tokenAppearences(char* __str, char __token) {

    int _count = 0;

    while(*__str) if (*(__str++) == __token) _count++;

    return _count;

}

int utils::tokenPosition(char* __str, char __token) {

    int _count = 0;

    while(*__str) if (*(__str++) == __token) return _count; else _count++;

    return -1; 

}
