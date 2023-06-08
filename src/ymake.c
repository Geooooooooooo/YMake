#include "cmpl.h"

int main(int argc, char* argv[]) {
    _Bool flag = YMAKE_FLAG_STD;

    if (read_args(argc, argv, &flag) != 0) {
        return 0;
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
        printf("Error: directory ymake-bin does not exist\nUse 'ymake --init'\n");

        return -1;
    }
    closedir(dir);

    YMakeList list = read_ymakelist(__YFile);

    if (list.output_file == NULL) 
        list.output_dir = _CurrentWDir;

    if (list.optimizer == -1) 
        list.optimizer = 0;

    if (list.output_file == NULL) {
        fprintf(list.log_file, "Warning: The output file is not specified, by default (y.out)\n\n");
        list.output_file = (char*)malloc(6 * sizeof(char));
        if (list.output_file == NULL) {
            fputs("MemoryAllocationError\n", list.log_file);
        }

        __builtin_strcpy(list.output_file, "y.out");
    } 

    if (list.src_files == NULL) {
        fprintf(list.log_file, "Error: No source file is specified\n");
        goto _free;
    }

    register size_t tmp_check = strlen(list.output_dir) - 1;
    if (list.output_dir[tmp_check] == '/') list.output_dir[tmp_check] = 0;

    compile_gxx(&list, _CurrentWDir, flag);

_free:
    if (list.src_files != NULL)   
        for (size_t i = 0; i < list.files_length; i++)
            __builtin_free(list.src_files[i]);
    
    if (_CurrentWDir != list.output_dir)
        __builtin_free(list.output_dir);

    __builtin_free(list.output_file);

    return 0;
}
