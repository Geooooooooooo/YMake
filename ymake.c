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

    if (list->OUT_DIR == NULL) {
        list->OUT_DIR = _CurrentWDir;
    }

    char tmp[1024];
    sprintf(tmp, "%s%s", _CurrentWDir, list->OUT_FILE);

    FILE* tmp_file = fopen(tmp, "r");
    if (tmp_file == NULL) {
        // compile all files

        goto _free;
    }

    // compare and compile last changet files

    sprintf(tmp, "%sYMakeList.txt", _CurrentWDir);

    struct stat attr;
    stat(tmp, &attr);


    
    for (size_t i = 0; i < list->LengthCFILES; i++) {
        __builtin_strcpy(tmp, "");
        sprintf(tmp, "%s%s",_CurrentWDir,  list->CFILES[i]);
        
        stat(tmp, &attr);

        printf("Last modified of %s -> %d\n", list->CFILES[i], attr.st_ctime);
    }

    //sha512sum test.c | awk {'print $1'}
    //FILE* 

_free:
    for (size_t i = 0; i < list->LengthCFILES; i++)
        __builtin_free(list->CFILES[i]);
    
    if (_CurrentWDir != list->OUT_DIR)
        __builtin_free(list->OUT_DIR);

    __builtin_free(list->OUT_FILE);
    __builtin_free(list);

    return 0;
}
