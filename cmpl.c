#include "cmpl.h"

void compile_gcc(YMakeList* list, char* _CurrentWDir) {
    char* tmp = (char*)malloc(2048 * sizeof(char));
    if (tmp == NULL) {
        return;
    }
    tmp[0] = 0;

    char* o_files = (char*)malloc(2048 * sizeof(char));
    if (o_files == NULL) {
        return;
    }
    o_files[0] = 0;

    sprintf(tmp, "%s/%s", _CurrentWDir, list->OUT_FILE);

    FILE* tmp_file = fopen(tmp, "rb");
    if (tmp_file == NULL) {
        for (size_t i = 0; i < list->LengthCFILES; i++) {
            sprintf(o_files, "%s%s/ymake-bin/%s.o ", o_files, _CurrentWDir, list->CFILES[i]);
            sprintf(tmp, "gcc %s -o ymake-bin/%s.o -c", list->CFILES[i], list->CFILES[i]);
            system(tmp);
        }

        char cmd[2048];
        sprintf(cmd, "gcc -o %s/%s %s", list->OUT_DIR, list->OUT_FILE, o_files);
        system(cmd);  

        return; 
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
            printf("Compile a %s -> %s.o\n", list->CFILES[i], list->CFILES[i]);

            sprintf(tmp, "gcc %s -o ymake-bin/%s.o -c", list->CFILES[i], list->CFILES[i]);
            system(tmp);
        }

        sprintf(o_files, "%s%s/ymake-bin/%s.o ", o_files, _CurrentWDir, list->CFILES[i]);
    }

    sprintf(tmp, "rm %s && gcc -o %s/%s %s", list->OUT_FILE, list->OUT_DIR, list->OUT_FILE, o_files);
    system(tmp); 

    free(tmp); 
    free(o_files);
}