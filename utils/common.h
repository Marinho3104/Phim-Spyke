#pragma once

#ifndef PARSER_COMMON_H
#define PARSER_COMMON_H

namespace utils {

    char* getFilesContent(char*, char**);

    char* getFileContent(char*);

    char** splitString(char*, char);

    int tokenAppearences(char*, char);

    int tokenPosition(char*, char);

}

#endif