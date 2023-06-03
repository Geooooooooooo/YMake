#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <dirent.h>
#include <string.h>

#include "header.h"

char* read_file(char* filename) {
    char* source;
    
    FILE *file_p;
    if ((file_p = fopen(filename, "r")) == NULL) {
        return (char*)(0);
    }

    fseek(file_p, 0L, SEEK_END);
    size_t length = (size_t)ftell(file_p);
    fseek(file_p, 0L, SEEK_SET);

    source = (char*)__builtin_malloc((size_t)((length + 2) * sizeof(char)));
    if (source == NULL) {
        puts("MemoryAllocationError");
        return (char*)(0);
    }

    for (size_t i = 0; i < length; i++) {
        source[i] = fgetc(file_p);
    }
    source[length] = '\0';

    fclose(file_p);

    return source;
}

inline void next_word(char* str, char* target, size_t* counter, size_t sz) {
    __builtin_strcpy(target, "");

    while (str[*counter] == ' ' || str[*counter] == '\n') 
        ++(*counter);
    
    size_t cntr = 0;
    for (; *counter < sz; (*counter)++) {
        if (str[*counter] == ' ' || str[*counter] == '\n') {
             target[cntr] = 0;
            while (str[*counter] == ' ' || str[*counter] == '\n') 
                ++(*counter);
            return;
        }
        else {
            target[cntr] = str[*counter];
            ++cntr;
        }
    }
}

CompileDependencies* read_ymakelist(char* __YFile) {
    CompileDependencies* cd = (CompileDependencies*)__builtin_malloc((size_t)(sizeof(CompileDependencies)));
    if (cd == NULL) {
        puts("MemoryAllocationError");
        return (CompileDependencies*)(0);
    }

    cd->LengthCFILES = 0;
    cd->CFILES = (char**)__builtin_malloc(sizeof(char**));
    cd->OUT_FILE = NULL;
    cd->OUT_DIR = NULL;

    char word[1024];
    size_t sz = strlen(__YFile);
    size_t cntr = 0;
    while (cntr < sz) {
        next_word(__YFile, word, &cntr, sz);

        if (__builtin_strcmp(word, "OUT_FILE") == 0) {
            if (cd->OUT_FILE != NULL) {
                // Error
                return (CompileDependencies*)(0);
            }

            next_word(__YFile, word, &cntr, sz);

            cd->OUT_FILE = (char*)__builtin_malloc(strlen(word));
            if (cd->OUT_FILE == NULL) {
                puts("MemoryAllocationError");
                return (CompileDependencies*)(0);
            }

            register size_t tmp_sz = strlen(word);
            size_t l;
            for (l = 0; l < tmp_sz; l++)
                cd->OUT_FILE[l] = word[l];
            cd->OUT_FILE[l++] = 0;
        }
        else if (__builtin_strcmp(word, "OUT_DIR") == 0) {
            if (cd->OUT_DIR != NULL) {
                // Error
                return (CompileDependencies*)(0);
            }

            next_word(__YFile, word, &cntr, sz);

            cd->OUT_DIR = (char*)__builtin_malloc(strlen(word));
            if (cd->OUT_DIR == NULL) {
                puts("MemoryAllocationError");
                return (CompileDependencies*)(0);
            } 

            register size_t tmp_sz = strlen(word);
            size_t l;
            for (l = 0; l < tmp_sz; l++)
                cd->OUT_DIR[l] = word[l];
            cd->OUT_DIR[l++] = 0;
        }
        else if (__builtin_strcmp(word, "CFILE") == 0) {
            next_word(__YFile, word, &cntr, sz);

            register size_t tmp_sz = strlen(word);

            cd->CFILES = (char**)__builtin_realloc(cd->CFILES, (cd->LengthCFILES+2) * sizeof(char**));
            cd->CFILES[cd->LengthCFILES] = (char*)__builtin_malloc(tmp_sz * sizeof(char));
            if (cd->CFILES[cd->LengthCFILES] == NULL) {
                puts("MemoryAllocationError");
                return (CompileDependencies*)(0);
            }

            size_t l;
            for (l = 0; l < tmp_sz; l++)
                cd->CFILES[cd->LengthCFILES][l] = word[l];
            cd->CFILES[cd->LengthCFILES][l] = 0;

            ++cd->LengthCFILES;
        }
    }

    return cd;
}

int main(int argc, char* argv[]) {
    char* __YFile = read_file("YMakeList.txt");

    if (__YFile == (char*)(0)) {
        printf("No YMakeList in current directory!\n");
        return -1;
    }

    CompileDependencies* cd = read_ymakelist(__YFile);

    puts("CFILES:");
    for (size_t i = 0; i < cd->LengthCFILES; i++) {
        puts(cd->CFILES[i]);
    }

    printf("OUT_FILE: %s\nOUT_DIR: %s\n", cd->OUT_FILE, cd->OUT_DIR);
}
