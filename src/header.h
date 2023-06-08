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

#define YMAKE_FLAG_STD      0
#define YMAKE_FLAG_FULL     1
#define ulong               unsigned long 

typedef enum __Compiler {
    Invalid, 
    GCC,    CC, 
    GPP,    CLANG
} Compiler;

typedef struct __CompileDependencies {
    char        *output_file;
    char        *output_dir;
    char        **src_files;
    Compiler    compiler;
    int         optimizer;
    ulong       files_length;
    FILE        *log_file;
} YMakeList;

void        next_word(char*, char*, size_t*, size_t);
YMakeList   read_ymakelist(char*);
char*       read_file(char*);
void        to_lower_case(char*);
int         read_args(int, char**, _Bool*);

#endif //__HEADER_H__