#include "coeficients.h"
#include "result.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define PACKAGE_LENGTH 1024
#define NUMBER_OF_PACKAGES 215
#define NUMBER_OF_AMPLIFICATIONS 10
#define NUMBER_OF_BOUNDARIES 9
#define MAX_PKT_SIZE 1024 * 4

int main(void)
{
	FILE *fp;
	int fd;
	int fr;
	int *p;
	int *r;
	int i;
	int j;
	unsigned int *hardware_res;
	unsigned int rezultat;
	// kada testiramo sve pakete for(i = 0; i < NUMBER_OF_PACKAGES; i++)

	for (i = 0; i < (NUMBER_OF_AMPLIFICATIONS + NUMBER_OF_BOUNDARIES); i++)
	{
		fp = fopen("/dev/eq", "w");
		if (fp == NULL)
		{
			printf("Cannot open /dev/eq for write\n");
			return -1;
		}
		if (i < NUMBER_OF_AMPLIFICATIONS)
			fprintf(fp, "%d,%#08x\n", i, p[i]);
		else
			fprintf(fp, "%d,%#08x\n", i, pr[i - 10]);
		fclose(fp);
		if (fp == NULL)
		{
			printf("Cannot close /dev/eq\n");
			return -1;
		}
	}

	fd = open("/dev/dm", O_RDWR | O_NDELAY);
	if (fd < 0)
	{
		printf("Cannot open /dev/dm for write\n");
		return -1;
	}
	p = (int *)mmap(0, MAX_PKT_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	memcpy(p, audio, MAX_PKT_SIZE);
	munmap(p, MAX_PKT_SIZE);
	close(fd);
	if (fd < 0)
	{
		printf("Cannot close /dev/dm for write\n");
		return -1;
	}

	fr = open("/dev/dm_s2mm", O_RDWR | O_NDELAY);
	if (fr < 0)
	{
		printf("Cannot open /dev/dm_s2mm for write\n");
		return -1;
	}
	hardware_res = (int *)malloc(PACKAGE_LENGTH * sizeof(int));
	r = (int *)mmap(0, MAX_PKT_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fr, 0);
	memcpy(hardware_res, r, MAX_PKT_SIZE);
	munmap(r, MAX_PKT_SIZE);
	close(fr);
	if (fr < 0)
	{
		printf("Cannot close /dev/dm_s2mm for write\n");
		return -1;
	}
	printf("\n\n***************%d*************\n\n", sizeof(hardware_res));
	return 0;
}
