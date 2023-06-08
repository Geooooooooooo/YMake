#include "header.h"

#define CHECK_BIT(var, pos) ((var) & (1 << (pos)))
#define YMAKE_VERSION   "1.1"

inline void next_word(char* str, char* target, size_t* counter, size_t sz) {
    __builtin_strcpy(target, "");

    size_t cntr = 0;

    while (str[*counter] == ' ' || str[*counter] == '\n') 
        ++(*counter);

    if (str[*counter] == '{' || str[*counter] == '}') {
        target[cntr] = str[*counter];
        target[++cntr] = 0;
        ++(*counter);

        return;
    }
    else if (str[*counter] == '#') {
        while (str[*counter] != '\n' && str[*counter] != '\0') {
            ++(*counter);
        } 
        
        ++(*counter);
    }
    
    for (; *counter < sz; (*counter)++) {
        if (str[*counter] == ' ' || str[*counter] == '\n' || str[*counter] == '\0' || str[*counter] == '{' || str[*counter] == '}') {
             target[cntr] = 0;
            while (str[*counter] == ' ' || str[*counter] == '\n' || str[*counter] == '\0') 
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

YMakeList read_ymakelist(char* __YFile) {
    YMakeList cd;
    _Bool firstAlloc = 1;

    cd.log_file = stdout;
    cd.compiler = -2;
    cd.files_length = 0;
    cd.output_file = NULL;
    cd.output_dir = NULL;
    cd.src_files = NULL;
    cd.optimizer = -1;

    char word[1024];
    size_t sz = strlen(__YFile);
    size_t cntr = 0;
    size_t l;

    while (cntr < sz) {
        next_word(__YFile, word, &cntr, sz);            
        to_lower_case(word);

        if (__builtin_strcmp(word, "compiler") == 0) {
            if (cd.compiler != -2) {
                puts("Logical Error: reusing the COMPILER parameter\n");
                return cd;
            }

            next_word(__YFile, word, &cntr, sz);
            to_lower_case(word);

            if (__builtin_strcmp(word, "gcc") == 0) 
                cd.compiler = GCC;
            else if (__builtin_strcmp(word, "cc") == 0) 
                cd.compiler = CC;
            else if (__builtin_strcmp(word, "g++") == 0) 
                cd.compiler = GPP;
            else if (__builtin_strcmp(word, "clang") == 0) 
                cd.compiler = CLANG;
            else {
                printf("Parameter Error: invalid argument for COMPILER %s\n", word);
                return cd;
            }
        }
        else if (__builtin_strcmp(word, "logs") == 0) {
            if (cd.log_file != stdout) {
                printf("Warning: re-specification of the logging parameter\n");
            }

            next_word(__YFile, word, &cntr, sz);

            cd.log_file = fopen(word, "w+");
        }
        else if (__builtin_strcmp(word, "optimization") == 0) {
            if (cd.optimizer != -1) {
                printf("Warning: re-specification of the optimization parameter\n");
            }

            next_word(__YFile, word, &cntr, sz);

            int op = atoi(word);
            switch (op) {
            case 0: 
                printf("Error: invalid optimizations argument\n");
                return cd;
            default: 
                cd.optimizer = op;
                break;
            }
        }
        else if (__builtin_strcmp(word, "out_file") == 0) {
            if (cd.output_file != NULL) {
                puts("Logical Error: reusing the OUT_FILE parameter\n");
                return cd;
            }

            next_word(__YFile, word, &cntr, sz);

            cd.output_file = (char*)__builtin_malloc(strlen(word));
            if (cd.output_file == NULL) {
                puts("MemoryAllocationError");
                return cd;
            }

            register size_t tmp_sz = strlen(word);
            for (l = 0; l < tmp_sz; l++) cd.output_file[l] = word[l];
            cd.output_file[l++] = 0;
        }
        else if (__builtin_strcmp(word, "out_dir") == 0) {
            if (cd.output_dir != NULL) {
                puts("Logical Error: reusing the OUT_DIR parameter\n");
                return cd;
            }

            next_word(__YFile, word, &cntr, sz);

            cd.output_dir = (char*)__builtin_malloc(strlen(word));
            if (cd.output_dir == NULL) {
                puts("MemoryAllocationError");
                return cd;
            } 

            register size_t tmp_sz = strlen(word);
            for (l = 0; l < tmp_sz; l++) cd.output_dir[l] = word[l];
            cd.output_dir[l] = 0;
        }
        else if (__builtin_strcmp(word, "cfiles") == 0) {
            next_word(__YFile, word, &cntr, sz);
            char inc_folder[256] = { 0 };

            to_lower_case(word);
            if (__builtin_strcmp(word, "in") == 0) {
                next_word(__YFile, word, &cntr, sz);
                __builtin_strcpy(inc_folder, word);

                next_word(__YFile, word, &cntr, sz);
            }
            
            if (__builtin_strcmp(word, "{") != 0) {
                printf("Error: Expected '{' after CFILES\n");
                return cd;
            }

            next_word(__YFile, word, &cntr, sz);
            while (__builtin_strcmp(word, "}") != 0) {
                register size_t tmp_sz = strlen(word) + strlen(inc_folder);

                if (firstAlloc) {
                    cd.src_files = (char**)__builtin_malloc(1 * sizeof(char**)); 
                    if (cd.src_files == NULL) {
                        puts("MemoryAllocationError");
                        return cd;
                    }
                    firstAlloc = 0;
                }
                else {
                    cd.src_files = (char**)__builtin_realloc(cd.src_files, (cd.files_length+2) * sizeof(char**));
                }

                cd.src_files[cd.files_length] = (char*)__builtin_malloc(tmp_sz * sizeof(char));
                if (cd.src_files[cd.files_length] == NULL) {
                    puts("MemoryAllocationError");
                    return cd;
                }
                
                tmp_sz -= strlen(word);
                for (l = 0; l < tmp_sz; l++) cd.src_files[cd.files_length][l] = inc_folder[l];
                
                size_t k = 0;
                tmp_sz += strlen(word);
                for (; l < tmp_sz; l++) cd.src_files[cd.files_length][l] = word[k++];
                cd.src_files[cd.files_length][l] = 0;

                ++cd.files_length;
                
                next_word(__YFile, word, &cntr, sz);
            }
        }
        else {
            if (word[0] != ' ' && word[0] != '\n' && word[0] != '\0') {
                printf("Error: unknow parameter: %s\n", word);
                return cd;
            }
        }
    }

    if (cd.compiler == -2) {
        cd.compiler = (Compiler)GPP;
    }

    return cd;
}

void to_lower_case(char* str) {
    register size_t len = strlen(str);
    for (register size_t i = 0; i < len; ++i) {
        str[i] = tolower(str[i]);
    }
}

int read_args(int argc, char* argv[], _Bool* flag) {
    if (argc > 1) {
        _Bool args_status_f = 0;
        int k = 1;
        while (k < argc) {
            to_lower_case(argv[k]);
            if (__builtin_strcmp(argv[k], "-i") == 0 || __builtin_strcmp(argv[k], "--init") == 0) {
                system("mkdir ymake-bin");
                return 1;
            }
            else if (__builtin_strcmp(argv[k], "-r") == 0 || __builtin_strcmp(argv[k], "--remove") == 0) {
                DIR* dir = opendir("ymake-bin");
                if (dir) {
                    system("rm -r ymake-bin");
                    closedir(dir);
                }
                return 1;
            }
            else if (__builtin_strcmp(argv[k], "-f") == 0 || __builtin_strcmp(argv[k], "--full") == 0) {
                if (args_status_f) {
                    printf("Warning: reusing the [-f, --full] argument\n");
                }

                args_status_f = 1;
                *flag = YMAKE_FLAG_FULL;
            }
            else if (__builtin_strcmp(argv[k], "-v") == 0 || __builtin_strcmp(argv[k], "--version") == 0) {
                printf("ymake version: %s\n", YMAKE_VERSION);

                return 1;
            }
            else if (__builtin_strcmp(argv[k], "-s") == 0 || __builtin_strcmp(argv[k], "--compilers") == 0) {
                printf(
                    "Supported compilers:\n"
                    "  gcc      g++\n"
                    "  cc       clang\n"
                );

                return 1;
            }
            else if (__builtin_strcmp(argv[k], "-h") == 0 || __builtin_strcmp(argv[k], "--help") == 0) {
                printf(
                    "ymake [OPTIONS, ...]\n"
                    "OPTIONS:\n"
                    "  -h, --help         info about ymake options\n"
                    "  -i, --init         initialization for ymake\n"
                    "  -r, --remove       remove ymake work directory\n"
                    "  -v, --version      version of ymake\n"
                    "  -c, --clean        clear ymake work directory\n"
                    "  -s, --compilers    shows supported compilers\n"
                    "Build OPTIONS:\n"
                    "  -f, --full         build all project files\n"
                );

                return 1;
            }
            else if (__builtin_strcmp(argv[k], "-c") == 0 || __builtin_strcmp(argv[k], "--clean") == 0) {
                DIR* dir = opendir("ymake-bin");
                if (dir == NULL) {
                    printf("Error: directory ymake-bin does not exist\nUse 'ymake --init'\n");
                    return 2;
                }

                struct dirent* next_file;
                char filepath[256];

                while ((next_file = readdir(dir)) != NULL) {
                    sprintf(filepath, "%s/%s", "ymake-bin", next_file->d_name);
                    remove(filepath);
                } closedir(dir);

                return 1;
            }
            else {
                printf("Error: unknown argument: %s\nUse 'ymake -h' for more information\n", argv[k]);
                return 2;
            }

            ++k;
        }
    }

    return 0;
}