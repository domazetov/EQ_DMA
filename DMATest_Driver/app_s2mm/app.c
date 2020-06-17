#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#define MAX_PKT_SIZE 1024

//comment to send pixels as commands via regular read function of char driver
//leave uncommented to read directly to memory (faster)
#define MMAP

int main(void)
{
	int x, y;
#ifdef MMAP
	// If memory map is defined send image directly via mmap
	int fd;
	int *p;
	unsigned int *output;
	fd = open("/dev/s2mm_dma", O_RDWR | O_NDELAY);
	if (fd < 0)
	{
		printf("Cannot open /dev/s2mm_dev for read\n");
		return -1;
	}
	output = (int *)malloc(MAX_PKT_SIZE * sizeof(int));
	p = (int *)mmap(0, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	memcpy(output, p, MAX_PKT_SIZE);
	munmap(p, MAX_PKT_SIZE);
	close(fd);

	if (fd < 0)
	{
		printf("Cannot close /dev/s2mm_dma for read\n");
		return -1;
	}

	printf("***********%d***********", output);
#else
	// Send via regualar driver interface
	FILE *fp;
	unsigned int output2;
	fp = fopen("/dev/s2mm_dma", "r");
	if (fp == NULL)
	{
		printf("Cannot open /dev/s2mm_dma for read\n");
		return -1;
	}
	fscanf(fp, "%d\n", output2);
	fclose(fp);

	if (fp == NULL)
	{
		printf("Cannot close /dev/s2mm_dma\n");
		return -1;
	}

	printf("***********%d***********", output2);

#endif
	return 0;
}
