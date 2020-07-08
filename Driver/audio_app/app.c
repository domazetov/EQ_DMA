#include "coeficients.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define AUDIO_LENGTH 1024 * 1877
#define PACKAGE_LENGTH 1024
#define NUMBER_OF_AMPLIFICATIONS 10
#define NUMBER_OF_BOUNDARIES 9
#define MAX_PKT_SIZE 1024 * 4

int main(void)
{
	FILE *fp;
	FILE *audiohex;
	int rx_proxy_fd, tx_proxy_fd;
	int *rx;
	int *tx;
	int i, count;
	unsigned int val;
	char audiof[8];

	int *array = (int *)malloc(PACKAGE_LENGTH * sizeof(int));

	unsigned int *hardware_res;

	printf("Equalizer started!\n");

	for (i = 0; i < (NUMBER_OF_AMPLIFICATIONS + NUMBER_OF_BOUNDARIES); i++)
	{
		fp = fopen("/dev/eq_in", "w");
		if (fp == NULL)
		{
			printf("Cannot open /dev/eq for write\n");
			return -1;
		}
		if (i < NUMBER_OF_AMPLIFICATIONS)
			fprintf(fp, "%d,%d\n", i, p[i]);
		else
			fprintf(fp, "%d,%d\n", i, pr[i - 10]);
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

	for (count = 0; count < AUDIO_LENGTH / PACKAGE_LENGTH; count++)
	{
		memcpy(array, audio + count * 1024, 1024 * sizeof(int));

		memcpy(tx, array, MAX_PKT_SIZE);

		sprintf(audiof, "0x%x", array[0]);
		printf("###############%s###############\n", audiof);

		write(tx_proxy_fd, &audiof, sizeof(audiof));
		write(tx_proxy_fd, &audiof, sizeof(audiof));
		usleep(200);

		ssize_t size = read(rx_proxy_fd, &val, sizeof(val));
		size = read(rx_proxy_fd, &val, sizeof(val));
		usleep(200);
		memcpy(hardware_res, rx, MAX_PKT_SIZE);

		for (i = 0; i < PACKAGE_LENGTH; i++)
		{
			fprintf(fp, "%#0010x\n", hardware_res[i]);
		}
	}

	munmap(tx, MAX_PKT_SIZE);
	munmap(rx, MAX_PKT_SIZE);
	fclose(fp);
	close(tx_proxy_fd);
	close(rx_proxy_fd);

	printf("Equalizer completed!\n");
	return 0;
}
