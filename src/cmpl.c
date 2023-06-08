#include "cmpl.h"

int errors = 0;
char compiler[8] = { 0 };
char no_changed_string[256]= { 0 };
char optimization[16] = { 0 };
char tmp[2048] = { 0 };
char object_files[2048] = { 0 };
struct stat st;

char* change_string(char* s) {
    register size_t tmp_ln = strlen(s);
    while (tmp_ln) {
        if (s[tmp_ln] == '/') s[tmp_ln] = '-';
        --tmp_ln;
    } return s;
}

void compile_gxx(YMakeList* list, char* _CurrentWDir, int flag) {
    if (list->compiler == GCC) 
        __builtin_strcpy(compiler, "gcc");
    else if (list->compiler == GPP)
        __builtin_strcpy(compiler, "g++");
    else if (list->compiler == CC)
        __builtin_strcpy(compiler, "cc");
    else if (list->compiler == CLANG)
        __builtin_strcpy(compiler, "clang");
    else {
        fputs("Error: Compiler not found or not supported", list->log_file);
        return;
    }

    sprintf(tmp, "%s/%s", list->output_dir, list->output_file);

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(list->log_file, "Assembly started (%s) ... %d-%02d-%02d %02d:%02d:%02d\n", 
        compiler, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    FILE* tmp_file = fopen(tmp, "rb");
    if (tmp_file == NULL || flag == YMAKE_FLAG_FULL) {
        for (size_t i = 0; i < list->files_length; i++) {
            fprintf(list->log_file, "\nCompile %s", list->src_files[i]);

            sprintf(optimization, "%d", list->optimizer);
            strcpy(no_changed_string, list->src_files[i]);
            sprintf(object_files, "%s%s/ymake-bin/%s.o ", object_files, _CurrentWDir, change_string(list->src_files[i]));
            sprintf(tmp, "%s %s -o ymake-bin/%s.o -c -O%s", 
                compiler, no_changed_string, change_string(list->src_files[i]), optimization);

            errors += system(tmp); 
        }

        sprintf(tmp, "%s -o %s/%s %s", compiler, list->output_dir, list->output_file, object_files);

        errors += system(tmp); 

        fputs("", list->log_file);

    } else {
        fclose(tmp_file);
        sprintf(tmp, "%s/%s", list->output_dir, list->output_file);

        stat(tmp, &st);

        long _LastAppMod = st.st_mtime;
        
        for (size_t i = 0; i < list->files_length; i++) {
            sprintf(tmp, "%s/%s", _CurrentWDir, list->src_files[i]);
            stat(tmp, &st);

            strcpy(no_changed_string, list->src_files[i]);
            sprintf(tmp, "%s/ymake-bin/%s.o", _CurrentWDir, change_string(list->src_files[i]));
            register FILE* fast_check_to_exist = fopen(tmp, "rb");

            if (st.st_mtime > _LastAppMod || fast_check_to_exist == NULL) {
                fprintf(list->log_file, "\nCompile %s", no_changed_string);

                sprintf(optimization, "%d", list->optimizer);
                sprintf(tmp, "%s %s -o ymake-bin/%s.o -c -O%s", 
                    compiler, no_changed_string, change_string(list->src_files[i]), optimization);

                errors += system(tmp);

                ((fast_check_to_exist != NULL) ? fclose(fast_check_to_exist) : 0);
            }
            else {
                fclose(fast_check_to_exist);
            }

            sprintf(object_files, "%s%s/ymake-bin/%s.o ", object_files, _CurrentWDir, change_string(list->src_files[i]));
        }

        sprintf(tmp, "rm %s/%s && %s -o %s/%s %s", 
            list->output_dir, list->output_file, compiler, list->output_dir, list->output_file, object_files);
        errors += system(tmp); 
    }

    if (!errors)
        fprintf(list->log_file, "\nOutput file --> %s/%s\n", list->output_dir, list->output_file);
}