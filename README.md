# YMake
Утилита для автоматической сборки проектов на C/C++

Пример использования:
$ ymake init
$ ymake

Так же должен быть создан файл конфигурации с именем YMakeList.txt
Пример файла:

COMPILER GCC
OUT_FILE test1
OUT_DIR /home/
CFILE t1.c
CFILE t2.c

На выходе создастся файл исполняемый test1
