#include "./common.h"

#include <iostream>
#include <string.h>
#include <fstream>


char* utils::getFilesContent(char* __path, char** __names) {

    char* _rtr = (char*) malloc(1), *_temp, *_file_content, *_path;

    *_rtr = '\0';

    while(*__names) {

        _path = (char*) malloc(strlen(__path) + strlen(*__names) + 4);

        strncpy(_path, __path, strlen(__path));
        strncpy(_path + strlen(__path), *__names, strlen(*__names));
        strcpy(_path + strlen(__path) + strlen(*__names), ".ph");

        _file_content = getFileContent(_path);

        _temp = _rtr;

        _rtr = (char*) malloc(strlen(_temp) + strlen(_file_content) + 1);

        strncpy(_rtr, _temp, strlen(_temp));
        strncpy(_rtr + strlen(_temp), _file_content, strlen(_file_content) + 1);

        __names++;

        free(_file_content);
        free(_temp);
        free(_path);

    }

    return _rtr;

}

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

    char** _splitted_tokens = (char**) malloc(sizeof(char*) * (_appearence + 2)), **_return = _splitted_tokens;
    char* _splitted_string;
    int _string_length;

    _splitted_tokens[_appearence + 1] = 0;

    while(*__str) {

        _string_length = tokenPosition(__str, __token);

        std::cout << "String length -> " << _string_length << std::endl;

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

    return _count - 1; 

}
