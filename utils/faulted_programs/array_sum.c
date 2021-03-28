#include <stdio.h>

int main(void) {
    int array[] = { 1, 2, 3, 4, 5 };
    int i;
    int sum = 0;

    for (i = 0; i < 5; i++) {
        sum += array[i];
    }

    if (sum == 15)
        printf("RESULT OK");
    else
        printf("FAILURE!!");

    return 0;
}
