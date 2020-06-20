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
	int rx_proxy_fd, tx_proxy_fd;
	int *rx;
	int *tx;
	int i;

	unsigned int *hardware_res;
	// kada testiramo sve pakete for(i = 0; i < NUMBER_OF_PACKAGES; i++)
	printf("Equalizer started!\n");
	for (i = 0; i < (NUMBER_OF_AMPLIFICATIONS + NUMBER_OF_BOUNDARIES); i++)
	{
		fp = fopen("/dev/equalizer", "w");
		if (fp == NULL)
		{
			printf("Cannot open /dev/equalizer for write\n");
			return -1;
		}
		if (i < NUMBER_OF_AMPLIFICATIONS)
			fprintf(fp, "%d,%#08x\n", i, p[i]);
		else
			fprintf(fp, "%d,%#08x\n", i, pr[i - 10]);
		fclose(fp);
		if (fp == NULL)
		{
			printf("Cannot close /dev/equalizer\n");
			return -1;
		}
	}

	tx_proxy_fd = open("/dev/dma_tx", O_RDWR);

	if (tx_proxy_fd < 1)
	{
		printf("Unable to open DMA MM2S");
		exit(EXIT_FAILURE);
	}

	rx_proxy_fd = open("/dev/dma_rx", O_RDWR);
	if (rx_proxy_fd < 1)
	{
		printf("Unable to open DMA S2MM");
		exit(EXIT_FAILURE);
	}

	fp = fopen("output.txt", "w+");

	hardware_res = (int *)malloc(PACKAGE_LENGTH * sizeof(int));

	rx = (int *)mmap(NULL, MAX_PKT_SIZE,
					 PROT_READ | PROT_WRITE, MAP_SHARED, rx_proxy_fd, 0);

	tx = (int *)mmap(NULL, MAX_PKT_SIZE,
					 PROT_READ | PROT_WRITE, MAP_SHARED, tx_proxy_fd, 0);

	if ((rx == MAP_FAILED) || (tx == MAP_FAILED))
	{
		printf("Failed to mmap\n");
		exit(EXIT_FAILURE);
	}

	memcpy(tx, audio, TEST_SIZE);
	memcpy(hardware_res, rx, TEST_SIZE);

	munmap(tx, TEST_SIZE);
	munmap(rx, TEST_SIZE);

	close(tx_proxy_fd);
	close(rx_proxy_fd);

	for (i = 0; i < TEST_SIZE; i++)
	{
		fprintf(fp, "%d\n", hardware_res[i]);
	}
	fclose(fp);

	printf("Equalizer completed!\n");
	return 0;
}
