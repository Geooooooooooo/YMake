#include "cmpl.h"

void compile_gcc(YMakeList* list, char* _CurrentWDir) {
    char* tmp = (char*)__builtin_malloc(2048 * sizeof(char));
    if (tmp == NULL) 
        return;

    char* o_files = (char*)__builtin_malloc(2048 * sizeof(char));
    if (o_files == NULL)
        return;

    tmp[0] = 0;
    o_files[0] = 0;

    sprintf(tmp, "%s/%s", _CurrentWDir, list->OUT_FILE);

    printf("Assembly started ...\n");

    FILE* tmp_file = fopen(tmp, "rb");
    if (tmp_file == NULL) {
        for (size_t i = 0; i < list->LengthCFILES; i++) {
            printf("\nCompile %s", list->CFILES[i]);
            sprintf(o_files, "%s%s/ymake-bin/%s.o ", o_files, _CurrentWDir, list->CFILES[i]);
            sprintf(tmp, "gcc %s -o ymake-bin/%s.o -c", list->CFILES[i], list->CFILES[i]);
            system(tmp);
        }

        char cmd[2048];
        sprintf(cmd, "gcc -o %s/%s %s", list->OUT_DIR, list->OUT_FILE, o_files);
        system(cmd);  

        puts("");

       goto _end;
    }

    fclose(tmp_file);
    sprintf(tmp, "%s/%s", _CurrentWDir, list->OUT_FILE);

    struct stat st;
    stat(tmp, &st);

    long _LastAppMod = st.st_mtime;
    
    for (size_t i = 0; i < list->LengthCFILES; i++) {
        sprintf(tmp, "%s/%s", _CurrentWDir,  list->CFILES[i]);

        stat(tmp, &st);

        if (st.st_mtime > _LastAppMod) {
            printf("Compile %s\n", list->CFILES[i]);
            sprintf(tmp, "gcc %s -o ymake-bin/%s.o -c", list->CFILES[i], list->CFILES[i]);
            system(tmp);
        }

        sprintf(o_files, "%s%s/ymake-bin/%s.o ", o_files, _CurrentWDir, list->CFILES[i]);
    }

    sprintf(tmp, "rm %s && gcc -o %s/%s %s", list->OUT_FILE, list->OUT_DIR, list->OUT_FILE, o_files);
    system(tmp); 

_end:
    printf("\nOutput file --> %s/%s\n", list->OUT_DIR, list->OUT_FILE);

    __builtin_free(tmp); 
    __builtin_free(o_files);
}