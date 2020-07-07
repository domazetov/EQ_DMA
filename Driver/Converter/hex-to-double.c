#include "stdio.h"
#include "math.h"
#include "string.h"
#include "stdlib.h"
#define SONG_LENGTH 1024 * 100

const float Q = 1.0 / 0x7fffff;

double convert(int val)
{
    if (val & 0x800000)
    {
        val |= ~0xffffff;
    }
    return val * Q;
}
int main()
{
    FILE *fp;
    FILE *fo;
    int i;
    double result[SONG_LENGTH];

    int number[SONG_LENGTH];

    fp = fopen("output.txt", "r");
    for (i = 0; i < SONG_LENGTH; i++)
    {
        fscanf(fp, "%x\n", &number[i]);
    }
    fclose(fp);

    fo = fopen("outputdouble.txt", "w");

    for (i = 0; i < SONG_LENGTH; i++)
    {
        result[i] = convert(number[i]);
        fprintf(fo, "%.10lf\n", result[i]);
    }
    fclose(fo);

    return 0;
}