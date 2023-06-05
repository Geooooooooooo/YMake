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

    YMakeList list = read_ymakelist(__YFile);

    if (list.OUT_DIR == NULL) {
        list.OUT_DIR = _CurrentWDir;
    }

    if (list.OUT_FILE == NULL) {
        fprintf(list.logs, "Warning: The output file is not specified, by default (y.out)\n\n");
        list.OUT_FILE = (char*)malloc(6 * sizeof(char));

        __builtin_strcpy(list.OUT_FILE, "y.out");
    } 

    if (list.CFILES == NULL) {
        fprintf(list.logs, "Error: No source file is specified\n");
        goto _free;
    }

    if (list.cmpl == GCC || list.cmpl == GPP || list.cmpl == CC) {
        compile_gxx(&list, _CurrentWDir);
    }
    else {
        fprintf(list.logs, "Error: Compiler not found or not supported\n");
    }

_free:
    if (list.CFILES != NULL)   
        for (size_t i = 0; i < list.LengthCFILES; i++)
            __builtin_free(list.CFILES[i]);
    
    if (_CurrentWDir != list.OUT_DIR)
        __builtin_free(list.OUT_DIR);

    __builtin_free(list.OUT_FILE);

    return 0;
}
