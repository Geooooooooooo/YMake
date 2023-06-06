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

#define YMAKE_FLAG_STD   0
#define YMAKE_FLAG_FULL    1

typedef enum __Compiler {
    Invalid, 
    GCC,    CC, 
    GPP,    CLANG
} Compiler;

typedef struct __CompileDependencies {
   char        *OUT_FILE;
    char        *OUT_DIR;
    char        **CFILES;
    Compiler    cmpl;
    int         optimizer;
    unsigned long LengthCFILES;
    FILE        *logs;
} YMakeList;

void next_word(char*, char*, size_t*, size_t);
YMakeList read_ymakelist(char*);
char* read_file(char*);
void to_lower_case(char*);

#endif //__HEADER_H__