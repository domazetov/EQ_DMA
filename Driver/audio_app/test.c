#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "audiohex.h"

int main()
{
    int *array = (int *)malloc(1024 * sizeof(int));
    int i;
    for (i = 0; i < 4; i++)
    {
        memcpy(array, audio + i * 1024, 1024 * sizeof(int));

        printf("%x\n", array[1023]);
    }

    return 0;
}