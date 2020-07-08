#include "stdio.h"
#include "math.h"

#define SONG_LENGTH 1024 * 130

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

    double amps[10];
    int boundaries[9];

    double number[SONG_LENGTH];
    int result[SONG_LENGTH];
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
    for (i = 0; i < 9; i++)
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
    for (i = 0; i < 9; i++)
    {
        fprintf(fo, "%#0010x", boundaries[i]);
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
    }
    fprintf(fo, "};\n");
    fclose(fo);
    return 0;
}