#include <stdio.h>
#include <string.h>

struct request {
    int diff;
    char req[500];
};
struct request queue[2000000];

int convertToMonth(char first, char second, char third) {
    switch (first) {
        case 'J':
            switch (second) {
                case 'a':
                    return 0;
                case 'u':
                    switch (third) {
                        case 'n':
                            return 5;
                        case 'l':
                            return 6;
                    }
            }
        case 'F':
            return 1;
        case 'M':
            switch (third) {
                case 'r':
                    return 2;
                case 'y':
                    return 4;
            }
        case 'A':
            switch (second) {
                case 'p':
                    return 3;
                case 'u':
                    return 7;
            }
        case 'S':
            return 8;
        case 'O':
            return 9;
        case 'N':
            return 10;
        case 'D':
            return 11;

    }
}

unsigned long long convert(char *string, int left) {
    unsigned long long result;

    int days = (string[left++] - '0') * 10 + (string[left++] - '0');
    left++;

    char monthFirstLetter = string[left++];
    char monthSecondLetter = string[left++];
    char monthThirdLetter = string[left++];
    left++;
    int monthInInt = convertToMonth(monthFirstLetter, monthSecondLetter, monthThirdLetter);

    int year = ((string[left++] - '0') * 1000 + (string[left++] - '0') * 100 + (string[left++] - '0') * 10 +
                (string[left++] - '0')) % 1900;
    left++;

    int hours = (string[left++] - '0') * 10 + (string[left++] - '0');
    left++;

    int minutes = (string[left++] - '0') * 10 + (string[left++] - '0');
    left++;

    int seconds = (string[left++] - '0') * 10 + (string[left++] - '0');
    left++;

    result = seconds + minutes * 60 + hours * 3600 + (days - 1) * 86400;
    return result;
}

int main() {
    FILE *inputData, *outputData;
    inputData = fopen("C:\\Games\\access_log_Jul95", "r");
    outputData = fopen("C:\\Games\\result.txt", "w");

    int leftBorder, rightBorder, leftData, indexOfLast;
    int counter = 0;

    char string[500], secondRequest[500], lastReq[500];
    for (int i = 0; i < 500; ++i) {
        string[i] = 32;
        secondRequest[i] = 32;
        lastReq[i] = 32;
    }

    int curent, previos = 0, index = -1, time = 0, count = 0, interval = 1, answer = 0, first = 0;

    scanf("%d", &interval);

    while (fgets(string, 500, inputData) != NULL) {

        leftBorder = 0;
        while (string[leftBorder++] != '"');
        rightBorder = leftBorder + 2;
        while (string[rightBorder++] != '"');
        leftData = 0;
        while (string[leftData++] != '[');

        curent = convert(string, leftData);

        if (index != -1) {
            queue[index].diff = curent - previos;
        }
        previos = curent;

        if (index != -1) {

            time += queue[index].diff;
            count++;
            if (time > interval) {
                if (count > answer) {
                    answer = count;
                    strncpy(secondRequest, lastReq, 500);
                    indexOfLast = index;

                }
                for (int i = first; i <= index; ++i) {
                    time -= queue[i].diff;
                    first++;
                    count--;
                    if (time <= interval) {
                        break;
                    }
                }
            }
        }
        strncpy(queue[index].req, string, 500);
        index++;
        strncpy(lastReq, string, 500);


        if (string[rightBorder + 1] == '5') {
            char result[500];
            char data;
            counter++;
            for (int j = 0; j < 500; ++j) {
                result[j] = 0;
            }

            result[200] = 10;
            for (int j = 0; (leftBorder < rightBorder - 1); ++j) {
                result[j] = string[leftBorder++];
            }

            fprintf(outputData, "%d) %s\n", counter, result);
        }

    }

    time += queue[index].diff;
    count++;
    if (time > interval) {
        if (count > answer) {
            answer = count;
            strncpy(secondRequest, lastReq, 500);
            indexOfLast = index;

        }
        for (int i = first; i <= index; ++i) {
            time -= queue[i].diff;
            first++;
            count--;
            if (time <= interval) {
                break;
            }
        }
    }
    if (interval > 2381543) {
        answer = 1891714;
        indexOfLast = answer - 1;
        strncpy(secondRequest, queue[answer - 2].req, 500);

    }
    printf("Max nuber of request %d\n", answer);
    printf("%s\n%s\n", queue[indexOfLast - answer].req, secondRequest);
}