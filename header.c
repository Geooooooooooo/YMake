#include "header.h"

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

YMakeList* read_ymakelist(char* __YFile) {
    YMakeList* cd = (YMakeList*)__builtin_malloc((size_t)(sizeof(YMakeList)));
    if (cd == NULL) {
        puts("MemoryAllocationError");
        return (YMakeList*)(0);
    }

    cd->CFILES = (char**)__builtin_malloc(sizeof(char**));
    if (cd->CFILES == NULL) {
        puts("MemoryAllocationError");
        return (YMakeList*)(0);
    }

    cd->cmpl = -2;
    cd->LengthCFILES = 0;
    cd->OUT_FILE = NULL;
    cd->OUT_DIR = NULL;

    char word[1024];
    size_t sz = strlen(__YFile);
    size_t cntr = 0;
    size_t l;

    while (cntr < sz) {
        next_word(__YFile, word, &cntr, sz);
        to_lower_case(word);

        if (__builtin_strcmp(word, "compiler") == 0) {
            if (cd->cmpl != -2) {
                puts("Logical Error: reusing the COMPILER parameter\n");
                return (YMakeList*)(0);
            }

            next_word(__YFile, word, &cntr, sz);
            to_lower_case(word);

            if (__builtin_strcmp(word, "gcc") == 0) {
                cd->cmpl = GCC;
            }
            else if (__builtin_strcmp(word, "cc") == 0) {
                cd->cmpl = CC;
            }
            else if (__builtin_strcmp(word, "g++") == 0) {
                cd->cmpl = GPP;
            }
            else if (__builtin_strcmp(word, "clang") == 0) {
                cd->cmpl = CLANG;
            }
            else {
                printf("Parameter Error: invalid argument for COMPILER %s\n", word);
                return (YMakeList*)(0);
            }
        }
        else if (__builtin_strcmp(word, "out_file") == 0) {
            if (cd->OUT_FILE != NULL) {
                puts("Logical Error: reusing the OUT_FILE parameter\n");
                return (YMakeList*)(0);
            }

            next_word(__YFile, word, &cntr, sz);

            cd->OUT_FILE = (char*)__builtin_malloc(strlen(word));
            if (cd->OUT_FILE == NULL) {
                puts("MemoryAllocationError");
                return (YMakeList*)(0);
            }

            register size_t tmp_sz = strlen(word);
            for (l = 0; l < tmp_sz; l++) cd->OUT_FILE[l] = word[l];
            cd->OUT_FILE[l++] = 0;
        }
        else if (__builtin_strcmp(word, "out_dir") == 0) {
            if (cd->OUT_DIR != NULL) {
                puts("Logical Error: reusing the OUT_DIR parameter\n");
                return (YMakeList*)(0);
            }

            next_word(__YFile, word, &cntr, sz);

            cd->OUT_DIR = (char*)__builtin_malloc(strlen(word));
            if (cd->OUT_DIR == NULL) {
                puts("MemoryAllocationError");
                return (YMakeList*)(0);
            } 

            register size_t tmp_sz = strlen(word);
            for (l = 0; l < tmp_sz; l++) cd->OUT_DIR[l] = word[l];
            cd->OUT_DIR[l] = 0;
        }
        else if (__builtin_strcmp(word, "cfile") == 0) {
            next_word(__YFile, word, &cntr, sz);

            register size_t tmp_sz = strlen(word);
            cd->CFILES = (char**)__builtin_realloc(cd->CFILES, (cd->LengthCFILES+2) * sizeof(char**));

            cd->CFILES[cd->LengthCFILES] = (char*)__builtin_malloc(tmp_sz * sizeof(char));
            if (cd->CFILES[cd->LengthCFILES] == NULL) {
                puts("MemoryAllocationError");
                return (YMakeList*)(0);
            }

            for (l = 0; l < tmp_sz; l++) cd->CFILES[cd->LengthCFILES][l] = word[l];
            cd->CFILES[cd->LengthCFILES][l] = 0;

            ++cd->LengthCFILES;
        }
    }

    if (cd->cmpl == -2) {
        cd->cmpl = (Compiler)GPP;
    }

    return cd;
}

void to_lower_case(char* str) {
    register size_t len = strlen(str);
    for (register size_t i = 0; i < len; ++i) {
        str[i] = tolower(str[i]);
    }
}