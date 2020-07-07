#include <stdio.h>
#include <stdlib.h>

#define SONG_LENGTH 1024 * 100

int main()
{
    FILE *fp;
    int i;

    int number[SONG_LENGTH];

    fp = fopen("output.txt", "r");
    for (i = 0; i < SONG_LENGTH; i++)
    {
        fscanf(fp, "%#0010x", &number[i]);
    }
    fclose(fp);

    for (i = 0; i < SONG_LENGTH; i += 1023)
    {
        if (number[1023 + i] == number[2046 + i])
        {
            printf("Error at: %#0010x and %#0010x\n", number[1023 + i], +number[2046 + i]);
        }
    }

    return 0;
}