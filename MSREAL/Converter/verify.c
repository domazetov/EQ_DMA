#include <stdio.h>
#include <stdlib.h>

#define SONG_LENGTH 1024 * 138

int main()
{
    FILE *fp;
    int i;

    int *number = malloc(SONG_LENGTH * sizeof(int));

    fp = fopen("../audio_app/output.txt", "r");
    for (i = 0; i < SONG_LENGTH; i++)
    {
        fscanf(fp, "%x\n", &number[i]);
    }
    fclose(fp);

    for (i = 0; i < SONG_LENGTH; i += 1024)
    {
        if (number[1023 + i] == number[2047 + i])
        {
            printf("Error at: %#0010x and %#0010x\n", number[1023 + i], +number[2047 + i]);
        }
    }
    return 0;
}