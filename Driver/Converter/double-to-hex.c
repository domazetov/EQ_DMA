#include "stdio.h"
#include "math.h"
#include "stdlib.h"

#define SONG_LENGTH 1024 * 2

int convert(double val)
{
    return lround(val * 0x7FFFFF) & 0xFFFFFF;
}

int main()
{
    FILE *fa;
    FILE *fp;
    FILE *fpr;
    FILE *fo;

    FILE *foa;

    double amps[10];
    int boundaries[18];

    double *number = malloc(SONG_LENGTH * sizeof(double));
    int *result = malloc(SONG_LENGTH * sizeof(int));
    ;
    int i, k = 0;

    fp = fopen("amps.txt", "r");
    for (i = 0; i < 10; i++)
    {
        fscanf(fp, "%lf", &amps[i]);
    }
    fclose(fp);

    fa = fopen("audiodouble.txt", "r");
    for (i = 0; i < SONG_LENGTH; i++)
    {
        fscanf(fp, "%lf", &number[i]);
    }
    fclose(fa);

    fpr = fopen("freq.txt", "r");
    for (i = 0; i < 18; i++)
    {
        fscanf(fp, "%d", &boundaries[i]);
    }
    fclose(fpr);

    fo = fopen("../audio_app/coeficients.h", "w");

    fprintf(fo, "unsigned int p[] = \n{\n");
    for (i = 0; i < 10; i++)
    {
        result[i] = convert(amps[i]);
        fprintf(fo, "%#0010x", result[i]);
        if (i != 10 - 1)
        {
            fprintf(fo, ",");
            k++;
            if (k == 5)
            {
                k = 0;
                fprintf(fo, "\n");
            }
        }
    }
    k = 0;
    fprintf(fo, "};\n\n\n");

    fprintf(fo, "unsigned int pr[] = \n{\n");
    for (i = 0; i < 18; i++)
    {
        fprintf(fo, "%#0010x", boundaries[i]);
        if (i != 18)
        {
            fprintf(fo, ",");
            k++;
            if (k == 5)
            {
                k = 0;
                fprintf(fo, "\n");
            }
        }
    }
    k = 0;
    fprintf(fo, "};\n");
    fclose(fo);

    foa = fopen("../audio_app/input.txt", "w");
    for (i = 0; i < SONG_LENGTH; i++)
    {
        result[i] = convert(number[i]);
        fprintf(foa, "%#0010x\n", result[i]);
    }
    fclose(foa);
    return 0;
}