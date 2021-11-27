#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct uint1024_t {
    uint8_t digits[128];
};

void showUndefinedBehavior() {
    printf("Undefined Bahvior");
    exit(1);
}

void scanfValue(char *integer, struct uint1024_t *result) {
    for (int i = 0; i < 128; ++i) {
        result->digits[i] = 0;
    }

    int firstIndex, index, digit, remainderNow, remainderLast, pointerEights, indexOfResult;
    firstIndex = index = remainderLast = indexOfResult = 0;
    pointerEights = 7;
    int eights[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    while (integer[firstIndex] != 'f' && integer[firstIndex] != 0 && integer[firstIndex] != '0') {
        index = firstIndex;
        while (integer[index] != 'f' && integer[index] != 0 && index < 309) {
            digit = integer[index] - '0';
            remainderNow = digit % 2;
            if (digit / 2 == 0) {
                if (index == firstIndex) {
                    integer[index] = 'f';
                } else {
                    integer[index] = (remainderLast * 10) / 2 + '0';
                }
            } else {
                if (remainderLast == 0) {
                    integer[index] = digit / 2 + '0';
                } else {
                    integer[index] = (digit + 10) / 2 + '0';
                }
            }
            remainderLast = remainderNow;
            index++;
        }
        eights[pointerEights] = remainderLast;
        pointerEights -= 1;
        if (pointerEights == -1) {
            pointerEights = 7;
            int ten = 0;
            int power = 128;
            for (int i = 0; i < 8; ++i) {
                ten += eights[i] * power;
                eights[i] = 0;
                power = power / 2;
            }
            result->digits[indexOfResult++] = ten;
        }
        remainderLast = remainderNow = 0;
        while (integer[firstIndex] == '0' || integer[firstIndex] == 'f') {
            firstIndex++;
        }
    }

    int ten = 0;
    int power = 128;
    for (int i = 0; i < 8; ++i) {
        ten += eights[i] * power;
        power = power / 2;
    }
    result->digits[indexOfResult] = ten;
}

void printfValue(struct uint1024_t *output) {
    int chekForZero = 1;
    int pointerOutput = 127;
    char lineOutput[309];
    for (int i = 0; i < 309; ++i) {
        lineOutput[i] = 'f';
        if (i < 128 && output->digits[i] != 0) {
            chekForZero = 0;
        }
    }

    if (chekForZero == 1) {
        printf("%d", 0);
    } else {
        while (output->digits[pointerOutput] == 0) {
            pointerOutput--;
        }
        for (int i = pointerOutput; i >= 0; i--) {
            int integer = output->digits[i];
            int eights[8] = {0, 0, 0, 0, 0, 0, 0, 0};
            for (int j = 7; j >= 0; j--) {
                eights[j] = integer % 2;
                integer = integer / 2;
            }
            for (int j = 0; j < 8; ++j) {
                if (lineOutput[0] == 'f') {
                    if (eights[j] == 1) {
                        lineOutput[0] = '1';
                    }
                } else {
                    int pointerForMul, remainderNow, remainderLast;
                    pointerForMul = remainderLast = remainderNow = 0;
                    while (lineOutput[pointerForMul] != 'f' && pointerForMul < 309) {
                        int digit = lineOutput[pointerForMul] - '0';
                        if (pointerForMul == 0) {
                            lineOutput[pointerForMul] = (digit * 2 % 10 + eights[j]) + '0';
                        } else {
                            lineOutput[pointerForMul] = (digit * 2 % 10 + remainderLast) + '0';
                        }
                        remainderNow = digit * 2 / 10;
                        if (remainderNow > 0) {
                            if (lineOutput[pointerForMul + 1] == 'f') {
                                lineOutput[pointerForMul + 1] = '0';
                            }
                        }
                        remainderLast = remainderNow;
                        pointerForMul++;
                    }
                }
            }
        }
        for (int i = 308; i >= 0; i--) {
            if (lineOutput[i] != 'f') {
                printf("%c", lineOutput[i]);
            }
        }
    }

    printf("\n");
    for (int i = 0; i < 128; ++i) {
        output->digits[i] = 0;
    }
}

struct uint1024_t fromUint(unsigned int x) {
    struct uint1024_t output;
    for (int i = 0; i < 128; i++) {
        output.digits[i] = 0;
    }

    int pointer = 0;
    while (x > 0) {
        output.digits[pointer++] = x % 256;
        x = x / 256;
    }

    output.digits[pointer] = x;
    return output;

}

struct uint1024_t addOp(struct uint1024_t firstTerm, struct uint1024_t secondTerm) {
    struct uint1024_t result;
    int reminder = 0;
    for (int i = 0; i < 128; ++i) {
        result.digits[i] = 0;
    }

    for (int i = 0; i < 128; ++i) {
        result.digits[i] = (firstTerm.digits[i] + secondTerm.digits[i] + reminder) % 256;
        reminder = (firstTerm.digits[i] + secondTerm.digits[i] + reminder) / 256;
        if (i == 127 && reminder > 0) {
            showUndefinedBehavior();
        }
    }

    return result;
}

struct uint1024_t SubtrOp(struct uint1024_t firstSubtracted, struct uint1024_t secondSubtracted) {
    struct uint1024_t result;
    for (int i = 0; i < 128; ++i) {
        result.digits[i] = 0;
    }

    for (int i = 0; i < 128; i++) {
        if (firstSubtracted.digits[i] < secondSubtracted.digits[i]) {
            int j = i + 1;
            while (firstSubtracted.digits[j] == 0 && j < 128) {
                firstSubtracted.digits[j] = 255;
                j++;
            }
            if (j < 128) {
                firstSubtracted.digits[j]--;
            } else {
                showUndefinedBehavior();
            }
            result.digits[i] = (256 + firstSubtracted.digits[i] - secondSubtracted.digits[i]);
        } else {
            result.digits[i] = (firstSubtracted.digits[i] - secondSubtracted.digits[i]);
        }
    }

    return result;
}

struct uint1024_t MultOp(struct uint1024_t firstMul, struct uint1024_t secondMul) {
    struct uint1024_t result;
    for (int i = 0; i < 128; ++i) {
        result.digits[i] = 0;
    }

    for (int i = 0; i < 128; ++i) {
        int remainder = 0;
        for (int j = 0; j < 128 || remainder != 0; ++j) {
            if (i + j < 128) {
                unsigned int x = result.digits[i + j] + firstMul.digits[i] * secondMul.digits[j] + remainder;
                result.digits[i + j] = x % 256;
                remainder = x / 256;
            } else if (remainder > 0) {
                showUndefinedBehavior();
            }
        }
    }

    return result;
}

int main() {
    char inputValueA[309], inputValueB[309], input[11];
    struct uint1024_t A, B, C, D, E;

    for (int i = 0; i < 309; ++i) {
        inputValueA[i] = 'f';
        inputValueB[i] = 'f';
    }

    printf("input А:");
    scanf("%s", inputValueA);
    scanfValue(inputValueA, &A);
    printf("input B:");
    scanf("%s", inputValueB);
    scanfValue(inputValueB, &B);
    printf("Available commands: '+', '-', '*', 'generate()', 'exit()'\n");
    scanf("%s", input);

    while (strcmp("exit()", input) != 0) {
        if (strcmp("generate()", input) == 0) {
            unsigned int x;
            printf("input x:");
            scanf("%u", &x);
            D = fromUint(x);
            printf("select operation for A and x: '+', '-', '*'\n");
            char operation[2];
            scanf("%s", operation);
            if (strcmp("+", operation) == 0) {
                C = addOp(A, D);
                printfValue(&C);
            } else if (strcmp("-", operation) == 0) {
                C = SubtrOp(A, D);
                printfValue(&C);
            } else if (strcmp("*", operation) == 0) {
                C = MultOp(A, D);
                printfValue(&C);
            }
            printf("select operation for A and B: '+', '-', '*'\n");
        } else if (strcmp("+", input) == 0) {
            C = addOp(A, B);
            printfValue(&C);
        } else if (strcmp("-", input) == 0) {
            C = SubtrOp(A, B);
            printfValue(&C);
        } else if (strcmp("*", input) == 0) {
            C = MultOp(A, B);
            printfValue(&C);
        }
        scanf("%s", input);
    }

    return 0;
}