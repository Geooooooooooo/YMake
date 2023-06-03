#ifndef __HEADER_H__
#define __HEADER_H__

#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <dirent.h>
#include <string.h>

typedef struct __CompileDependencies {
    char        *OUT_FILE;
    char        *OUT_DIR;
    char        **CFILES;
    unsigned long LengthCFILES;
} YMakeList;

void next_word(char*, char*, size_t*, size_t);
YMakeList* read_ymakelist(char*);
char* read_file(char*);

#endif //__HEADER_H__