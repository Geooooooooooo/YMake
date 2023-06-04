#include "cmpl.h"

char* cstr(char* s) {
    register size_t tmp_ln = strlen(s);
    while (tmp_ln) {
        if (s[tmp_ln] == '/') s[tmp_ln] = '-';
        --tmp_ln;
    }
    return s;
}

void compile_gxx(YMakeList* list, char* _CurrentWDir) {
    int errors = 0;
    char comp[8] = { 0 };
    char clean_file[256] = { 0 };
    if (list->cmpl == GCC) {
        __builtin_strcpy(comp, "gcc");
    }
    else if (list->cmpl == GPP) {
        __builtin_strcpy(comp, "g++");
    }
    else if (list->cmpl == CC) {
        __builtin_strcpy(comp, "cc");
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

    printf("Assembly started (%s) ...\n", comp);

    FILE* tmp_file = fopen(tmp, "rb");
    if (tmp_file == NULL) {
        for (size_t i = 0; i < list->LengthCFILES; i++) {
            printf("\nCompile %s", list->CFILES[i]);

            strcpy(clean_file, list->CFILES[i]);
            sprintf(o_files, "%s%s/ymake-bin/%s.o ", o_files, _CurrentWDir, cstr(list->CFILES[i]));
            sprintf(tmp, "%s %s -o ymake-bin/%s.o -c", comp, clean_file, cstr(list->CFILES[i]));

            errors += system(tmp); 
            //system(tmp);
        }

        char cmd[2048];
        sprintf(cmd, "%s -o %s/%s %s", comp, list->OUT_DIR, list->OUT_FILE, o_files);

        errors += system(cmd); 

        puts("");

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

        if (st.st_mtime > _LastAppMod) {
            printf("Compile %s\n", list->CFILES[i]);

            strcpy(clean_file, list->CFILES[i]);
            sprintf(tmp, "%s %s -o ymake-bin/%s.o -c", comp, clean_file, cstr(list->CFILES[i]));
            errors += system(tmp);
        }

        sprintf(o_files, "%s%s/ymake-bin/%s.o ", o_files, _CurrentWDir, cstr(list->CFILES[i]));
    }

    sprintf(tmp, "rm %s/%s && %s -o %s/%s %s", 
        list->OUT_DIR, list->OUT_FILE, comp, list->OUT_DIR, list->OUT_FILE, o_files);
    errors += system(tmp); 

_end:
    if (!errors)
        printf("\nOutput file --> %s/%s\n", list->OUT_DIR, list->OUT_FILE);

    __builtin_free(tmp); 
    __builtin_free(o_files);
}