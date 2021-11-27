#include <stdio.h>
#include <string.h>

int CountLine(FILE *file) {
    fseek(file, 0, 0);
    char line[100];
    long lineCounter = 0;
    while (fgets(line, 100, file) != NULL) {
        lineCounter++;
    }
    return lineCounter;
}

int CountByte(FILE *file) {
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    return size;
}

int CountWord(FILE *file) {
    fseek(file, 0, 0);
    int symbol, isSpace = 0;
    long counter = 0;
    while (!feof(file)) {
        symbol = fgetc(file);

        if (symbol != ' ' && symbol != '\n' && isSpace == 0 && symbol != -1 && symbol != '\t') {
            counter++;
            isSpace = 1;
        } else if (symbol == ' ' || symbol == '\n') {
            isSpace = 0;
        }
    }
    return counter;
}

void ShowError() {
    printf("Неверный формат ввода. Ввод осущетвляется в следующем виде: WordCont.exe [OPTION] filename\n"
           "\t-l, --lines вывод количества строк\n\t-c, --bytes вывод раземра файла в байтах\n\t-w, --words вывод количества строк\n");
}

int main(int argc, char *argv[]) {
    char command[1000], fileName[1000];
    int isLine = 0, isByte = 0, isWord = 0, isAttention = 0;

    if (argc < 3 || argc > 5) {
        ShowError();
    } else {
        FILE *myFile;
        strcpy(fileName, argv[argc - 1]);
        myFile = fopen(fileName, "r");
        if (!myFile) {
            printf("Указанный файл не сущетсвует, введите полный путь к файлу");
        } else {
            for (int i = 1; i < argc - 1; ++i) {
                strcpy(command, argv[i]);

                if ((strcmp(command, "-l") == 0 || strcmp(command, "--lines") == 0) && isLine == 0) {
                    printf("Количество строк: %d\n", CountLine(myFile));
                    isLine = 1;

                } else if ((strcmp(command, "-c") == 0 || strcmp(command, "--bytes") == 0) && isByte == 0) {
                    printf("Размер в байтах: %d\n", CountByte(myFile));
                    isByte = 1;

                } else if ((strcmp(command, "-w") == 0 || strcmp(command, "--words") == 0) && isWord == 0) {
                    printf("Количество слов: %d\n", CountWord(myFile));
                    isWord = 1;

                } else if (isAttention == 0) {
                    ShowError();
                    isAttention = 1;
                }
            }
        }
    }
    return 0;
}





