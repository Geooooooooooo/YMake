#include "cmpl.h"

int main(int argc, char* argv[]) {
    if (argc == 2) {
        to_lower_case(argv[1]);
        if (__builtin_strcmp(argv[1], "init") == 0) {
            system("mkdir ymake-bin");
            return 0;
        }
    }

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

    DIR* dir = opendir("ymake-bin");
    if (!dir) {
        printf("Error: directory ymake-bin does not exist\nUse ymake init\n");

        return -1;
    }
    closedir(dir);

    YMakeList* list = read_ymakelist(__YFile);

    if (list->OUT_DIR == NULL) {
        list->OUT_DIR = _CurrentWDir;
    }

    if (list->cmpl == GCC || list->cmpl == GPP || list->cmpl == CC) {
        compile_gxx(list, _CurrentWDir);
    }
    else {
        printf("Compiler not found or not supported\n\n");
    }

_free:
    for (size_t i = 0; i < list->LengthCFILES; i++)
        __builtin_free(list->CFILES[i]);
    
    if (_CurrentWDir != list->OUT_DIR)
        __builtin_free(list->OUT_DIR);

    __builtin_free(list->OUT_FILE);
    __builtin_free(list);

    return 0;
}
