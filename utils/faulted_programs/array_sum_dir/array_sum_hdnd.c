#include <stdio.h>

int main() {
    int array1[] = { 4837, 1222, 3, 4, 5 };
    int array2[] = { 4837, 1222, 3, 4, 5 };
    int array3[] = { 4837, 1222, 3, 4, 5 };
    int i;
    int sum1 = 0;
    int sum2 = 0;
    int sum3 = 0;
    int expected = 6071;
    int tot_match = 0;
    for (i = 0; i < 5; i++) {
        sum1 += array1[i];
        sum2 += array2[i];
        sum3 += array3[i];
    }
    /*Decido a maggioranza*/

    if (sum1 == expected)
        tot_match++;
    if (sum2 == expected)
        tot_match++;
    if (sum3 == expected)
        tot_match++;

    if (tot_match >= 2)
        printf("RESULT OK\n");
    else
        printf("RESULT FAILURE\n");

    return 0;
}
