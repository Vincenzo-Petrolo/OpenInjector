#include <stdio.h>

int main() {
    int array[] = { 4837, 1222, 3, 4, 5 };
    int i;
    int sum = 0;

    for (i = 0; i < 5; i++) {
        sum += array[i];
    }

    if (sum == 6071)
        printf("RESULT: OK\n");
    else
        printf("RESULT: FAILURE\n");

    return 0;
}
