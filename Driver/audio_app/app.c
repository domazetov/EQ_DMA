#include "coeficients.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define AUDIO_LENGTH 1024
#define PACKAGE_LENGTH 1024
#define NUMBER_OF_AMPLIFICATIONS 10
#define NUMBER_OF_BOUNDARIES 18
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
	char start[] = "start";
	int audiohex_size = 0;

	//int *array = (int *)malloc(PACKAGE_LENGTH * sizeof(int));

	int *hardware_res;
	int *input;

	printf("Equalizer started!\n");

	audiohex = fopen("input.txt", "r");
	if (audiohex == NULL)
	{
		printf("Cannot open input.txt\n");
		exit(EXIT_FAILURE);
	}

	fseek(audiohex, 0, SEEK_END);
	audiohex_size = ftell(audiohex);
	fseek(audiohex, 0, SEEK_SET);
	printf("Number of input.txt lines: %d\n", audiohex_size / 12);
	audiohex_size = audiohex_size / 12;

	input = (int *)malloc(audiohex_size * sizeof(int));

	for (i = 0; i < audiohex_size; i++) //for (i = 0; i < AUDIO_LENGTH; i++)
	{
		fscanf(audiohex, "%x", &input[i]);
	}
	fclose(audiohex);

	for (i = 0; i < (NUMBER_OF_AMPLIFICATIONS + NUMBER_OF_BOUNDARIES); i++)
	{
		fp = fopen("/dev/eq_in", "w");
		if (fp == NULL)
		{
			printf("Cannot open /dev/eq_in for write\n");
			exit(EXIT_FAILURE);
		}
		if (i < NUMBER_OF_AMPLIFICATIONS)
			fprintf(fp, "%d,%d\n", i, p[i]);
		else
			fprintf(fp, "%d,%d\n", i, pr[i - 10]);
		fclose(fp);
		if (fp == NULL)
		{
			printf("Cannot close /dev/eq_in\n");
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

	hardware_res = (int *)malloc(audiohex_size * sizeof(int));

	rx = (int *)mmap(NULL, audiohex_size,
					 PROT_READ | PROT_WRITE, MAP_SHARED, rx_proxy_fd, 0);

	tx = (int *)mmap(NULL, audiohex_size,
					 PROT_READ | PROT_WRITE, MAP_SHARED, tx_proxy_fd, 0);

	if ((rx == MAP_FAILED) || (tx == MAP_FAILED))
	{
		printf("Failed to mmap\n");
		exit(EXIT_FAILURE);
	}

	//for (count = 0; count < AUDIO_LENGTH / PACKAGE_LENGTH; count++)
	//{
	//memcpy(array, input + count * 1024, 1024 * sizeof(int));

	memcpy(tx, input, audiohex_size);

	write(tx_proxy_fd, &start, sizeof(start));
	usleep(200);

	ssize_t size = read(rx_proxy_fd, &val, sizeof(val));
	size = read(rx_proxy_fd, &val, sizeof(val));
	usleep(200);
	memcpy(hardware_res, rx, audiohex_size);

	for (i = 0; i < audiohex_size; i++)
	{
		fprintf(fp, "%#0010x\n", hardware_res[i]);
	}
	//}

	munmap(tx, MAX_PKT_SIZE);
	munmap(rx, MAX_PKT_SIZE);
	fclose(fp);
	close(tx_proxy_fd);
	close(rx_proxy_fd);

	printf("Equalizer completed!\n");
	return 0;
}
