typedef struct __CompileDependencies {
    char        *OUT_FILE;
    char        *OUT_DIR;
    char        **CFILES;
    unsigned long LengthCFILES;
} CompileDependencies;