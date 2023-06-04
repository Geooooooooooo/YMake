#ifndef __HEADER_H__
#define __HEADER_H__

#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef enum __Compiler {
    Invalid, 
    GCC,    CC, 
    GPP,    CLANG
} Compiler;

typedef struct __CompileDependencies {
    Compiler    cmpl;
    char        *OUT_FILE;
    char        *OUT_DIR;
    char        **CFILES;
    unsigned long LengthCFILES;
} YMakeList;

void next_word(char*, char*, size_t*, size_t);
YMakeList read_ymakelist(char*);
char* read_file(char*);
void to_lower_case(char*);

#endif //__HEADER_H__