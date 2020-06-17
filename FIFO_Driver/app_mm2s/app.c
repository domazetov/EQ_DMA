#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
//#include "test.h"
#define MAX_PKT_SIZE 4

//comment to send pixels as commands via regular write function of char driver
//leave uncommented to write directly to memory (faster)
#define MMAP

int main(void)
{
	int x, y;
#ifdef MMAP
	// If memory map is defined send image directly via mmap
	int fd;
	int *p;
	int image = 420;
	fd = open("/dev/mm2s_dma", O_RDWR | O_NDELAY);
	if (fd < 0)
	{
		printf("Cannot open /dev/mm2s_dev for write\n");
		return -1;
	}
	printf("Size of image %d\n", sizeof(image));
	p = (int *)mmap(0, 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	memcpy(p, &image, MAX_PKT_SIZE);
	munmap(p, MAX_PKT_SIZE);
	printf("Wrote %d to mm2s_dev\n", image);
	close(fd);
	if (fd < 0)
	{
		printf("Cannot close /dev/mm2s_dma for write\n");
		return -1;
	}

#else
	// Send via regualar driver interface
	FILE *fp;

	fp = fopen("/dev/mm2s_dma", "w");
	if (fp == NULL)
	{
		printf("Cannot open /dev/mm2s_dma for write\n");
		return -1;
	}
	fprintf(fp, "%d\n", image[0]);
	fclose(fp);
	if (fp == NULL)
	{
		printf("Cannot close /dev/mm2s_dma\n");
		return -1;
	}

#endif
	return 0;
}
