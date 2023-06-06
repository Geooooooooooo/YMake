#include "cmpl.h"

#define CHECK_BIT(var, pos) ((var) & (1 << (pos)))

#define YMAKE_VERSION   "1.0"

int main(int argc, char* argv[]) {
    _Bool flag = YMAKE_FLAG_STD;
    _Bool f_optimizer;
    _Bool args_status_f = 0;

    if (argc > 1) {
        int k = 1;
        while (k < argc) {
            to_lower_case(argv[k]);
            if (__builtin_strcmp(argv[k], "-i") == 0 || __builtin_strcmp(argv[k], "--init") == 0) {
                system("mkdir ymake-bin");
                return 0;
            }
            else if (__builtin_strcmp(argv[k], "-r") == 0 || __builtin_strcmp(argv[k], "--remove") == 0) {
                DIR* dir = opendir("ymake-bin");
                if (dir) {
                    system("rm -r ymake-bin");
                    closedir(dir);
                }
                return 0;
            }
            else if (__builtin_strcmp(argv[k], "-f") == 0 || __builtin_strcmp(argv[k], "--full") == 0) {
                if (args_status_f) {
                    printf("Warning: reusing the [-f, --full] argument\n");
                }

                args_status_f = 1;
                flag = YMAKE_FLAG_FULL;
            }
            else if (__builtin_strcmp(argv[k], "-v") == 0 || __builtin_strcmp(argv[k], "--version") == 0) {
                printf("ymake version: %s\n", YMAKE_VERSION);

                return 0;
            }
            else if (__builtin_strcmp(argv[k], "-h") == 0 || __builtin_strcmp(argv[k], "--help") == 0) {
                printf(
                    "ymake [OPTIONS, ...]\n"
                    "OPTIONS:\n"
                    "  -h, --help        info about ymake options\n"
                    "  -i, --init        initialization for ymake\n"
                    "  -r, --remove      remove ymake work directory\n"
                    "  -v, --version     version of ymake\n"
                    "Build OPTIONS:\n"
                    "  -f, --full        rebuild all project files\n"
                );

                return 0;
            }
            else {
                printf("Error: unknown argument: %s\nUse 'ymake -h' for more information\n", argv[k]);
                return 1;
            }

            ++k;
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

    if (list.OUT_DIR == NULL) 
        list.OUT_DIR = _CurrentWDir;

    if (list.optimizer == -1) 
        list.optimizer = 0;

    if (list.OUT_FILE == NULL) {
        fprintf(list.logs, "Warning: The output file is not specified, by default (y.out)\n\n");
        list.OUT_FILE = (char*)malloc(6 * sizeof(char));

        __builtin_strcpy(list.OUT_FILE, "y.out");
    } 

    if (list.CFILES == NULL) {
        fprintf(list.logs, "Error: No source file is specified\n");
        goto _free;
    }

    compile_gxx(&list, _CurrentWDir, flag);

_free:
    if (list.CFILES != NULL)   
        for (size_t i = 0; i < list.LengthCFILES; i++)
            __builtin_free(list.CFILES[i]);
    
    if (_CurrentWDir != list.OUT_DIR)
        __builtin_free(list.OUT_DIR);

    __builtin_free(list.OUT_FILE);

    return 0;
}
