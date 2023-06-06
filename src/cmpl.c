#include "cmpl.h"

char* cstr(char* s) {
    register size_t tmp_ln = strlen(s);
    while (tmp_ln) {
        if (s[tmp_ln] == '/') s[tmp_ln] = '-';
        --tmp_ln;
    }
    return s;
}

void compile_gxx(YMakeList* list, char* _CurrentWDir, int flag) {
    int errors = 0;
    char comp[8] = { 0 };
    char clean_file[256] = { 0 };
    char optimiz[16] = { 0 };

    if (list->cmpl == GCC) {
        __builtin_strcpy(comp, "gcc");
    }
    else if (list->cmpl == GPP) {
        __builtin_strcpy(comp, "g++");
    }
    else if (list->cmpl == CC) {
        __builtin_strcpy(comp, "cc");
    }
    else if (list->cmpl == CLANG) {
        __builtin_strcpy(comp, "clang");
    }
    else {
        fprintf(list->logs, "Error: Compiler not found or not supported\n");
        return;
    }

    char* tmp = (char*)__builtin_malloc(2048 * sizeof(char));
    if (tmp == NULL) 
        return;

    char* o_files = (char*)__builtin_malloc(2048 * sizeof(char));
    if (o_files == NULL)
        return;

    tmp[0] = 0;
    o_files[0] = 0;

    sprintf(tmp, "%s/%s", list->OUT_DIR, list->OUT_FILE);

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(list->logs, "Assembly started (%s) ... %d-%02d-%02d %02d:%02d:%02d\n", 
        comp, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    FILE* tmp_file = fopen(tmp, "rb");
    if (tmp_file == NULL || flag == YMAKE_FLAG_FULL) {
        for (size_t i = 0; i < list->LengthCFILES; i++) {
            fprintf(list->logs, "\nCompile %s", list->CFILES[i]);

            sprintf(optimiz, "%d", list->optimizer);
            strcpy(clean_file, list->CFILES[i]);
            sprintf(o_files, "%s%s/ymake-bin/%s.o ", o_files, _CurrentWDir, cstr(list->CFILES[i]));
            sprintf(tmp, "%s %s -o ymake-bin/%s.o -c -O%s", 
                comp, clean_file, cstr(list->CFILES[i]), optimiz);

            errors += system(tmp); 
        }

        char cmd[2048];
        sprintf(cmd, "%s -o %s/%s %s", comp, list->OUT_DIR, list->OUT_FILE, o_files);

        errors += system(cmd); 

        fputs("", list->logs);

       goto _end;
    }

    fclose(tmp_file);
    sprintf(tmp, "%s/%s", list->OUT_DIR, list->OUT_FILE);

    struct stat st;
    stat(tmp, &st);

    long _LastAppMod = st.st_mtime;
    
    for (size_t i = 0; i < list->LengthCFILES; i++) {
        sprintf(tmp, "%s/%s", _CurrentWDir, list->CFILES[i]);
        stat(tmp, &st);

        strcpy(clean_file, list->CFILES[i]);
        sprintf(tmp, "%s/ymake-bin/%s.o", _CurrentWDir, cstr(list->CFILES[i]));
        register FILE* fast_check_to_exist = fopen(tmp, "rb");

        if (st.st_mtime > _LastAppMod || fast_check_to_exist == NULL) {
            fprintf(list->logs, "Compile %s\n", clean_file);

            sprintf(optimiz, "%d", list->optimizer);
            sprintf(tmp, "%s %s -o ymake-bin/%s.o -c -O%s", 
                comp, clean_file, cstr(list->CFILES[i]), optimiz);

            errors += system(tmp);

            ((fast_check_to_exist != NULL) ? fclose(fast_check_to_exist) : 0);
        }
        else {
            fclose(fast_check_to_exist);
        }

        sprintf(o_files, "%s%s/ymake-bin/%s.o ", o_files, _CurrentWDir, cstr(list->CFILES[i]));
    }

    sprintf(tmp, "rm %s/%s && %s -o %s/%s %s", 
        list->OUT_DIR, list->OUT_FILE, comp, list->OUT_DIR, list->OUT_FILE, o_files);
    errors += system(tmp); 

_end:
    if (!errors)
        fprintf(list->logs, "\nOutput file --> %s/%s\n", list->OUT_DIR, list->OUT_FILE);

    __builtin_free(tmp); 
    __builtin_free(o_files);
}