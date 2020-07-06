#include "stdio.h"
#include "math.h"

#define SONG_LENGTH 102400

int convert(double val)
{
    return lround(val * 0x7FFFFF) & 0xFFFFFF;
}

int main()
{
    FILE *fp;
    FILE *fo;
    double number[SONG_LENGTH];
    int result[SONG_LENGTH];
    int i, k = 0;

    fp = fopen("audiodouble.txt", "r");
    for (i = 0; i < SONG_LENGTH; i++)
    {
        fscanf(fp, "%lf", &number[i]);
    }
    fclose(fp);

    fo = fopen("audiohex.h", "w");

    fprintf(fo, "unsigned int audio[] = \n{\n");
    for (i = 0; i < SONG_LENGTH; i++)
    {
        result[i] = convert(number[i]);
        fprintf(fo, "%#0010x", result[i]);
        if (i != SONG_LENGTH - 1)
        {
            fprintf(fo, ",");
            k++;
            if (k == 5)
            {
                k = 0;
                fprintf(fo, "\n");
            }
        }
        //printf("Double: %.10lf     Int: %#10x\n", number[i], result[i]);
    }
    fprintf(fo, "};\n");
    fclose(fo);
    return 0;
}