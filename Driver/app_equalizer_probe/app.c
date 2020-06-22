#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "coeficients.h"

#define NUMBER_OF_AMPLIFICATIONS 10
#define NUMBER_OF_BOUNDARIES 9

int main(void)
{
	FILE* fp;
    int i;

    for(i = 0; i < (NUMBER_OF_AMPLIFICATIONS+NUMBER_OF_BOUNDARIES); i++)
    {
        fp = fopen("/dev/eq", "w");
        if(fp == NULL)
        {
            printf("Cannot open /dev/eq for write\n");
				return -1;
        }
        if(i < NUMBER_OF_AMPLIFICATIONS)
            fprintf(fp,"%d,%#08x\n",i,p[i]);
        else
            fprintf(fp,"%d,%#08x\n",i,pr[i - 10]);
        fclose(fp);
		if(fp == NULL)
		{
			printf("Cannot close /dev/eq\n");
			return -1;
		}
    }
	return 0;
}