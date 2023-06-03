#include "header.h"

int main(int argc, char* argv[]) {
    char _CurrentWDir[256];
    if (getcwd(_CurrentWDir, sizeof(_CurrentWDir)) == NULL) {
       perror("getcwd() error");
       return 1;
    }

    char* __YFile = read_file("YMakeList.txt");

    if (__YFile == (char*)(0)) {
        printf("No YMakeList in current directory!\n");
        return -1;
    }

    YMakeList* list = read_ymakelist(__YFile);

    puts("CFILES:");
    for (size_t i = 0; i < list->LengthCFILES; i++) {
        puts(list->CFILES[i]);
    }

    printf("OUT_FILE: %s\nOUT_DIR: %s\n", list->OUT_FILE, list->OUT_DIR);

    while (list->LengthCFILES) {
        __builtin_free(list->CFILES[list->LengthCFILES]);
        --list->LengthCFILES;
    }
    __builtin_free(list->OUT_DIR);
    __builtin_free(list->OUT_FILE);
    __builtin_free(list);

    return 0;
}
