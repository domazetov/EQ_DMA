#include "coeficients.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define PACKAGE_NUMBER 1292
#define PACKAGE_LENGTH 1024
#define NUMBER_OF_AMPLIFICATIONS 10
#define NUMBER_OF_BOUNDARIES 18

int main(void)
{
	FILE *dae;
	FILE *audiohex;
	FILE *out;
	int rx_fd, tx_fd;
	int *rx_mmap;
	int *tx_mmap;
	unsigned int i, count, k;
	char end;
	char start[] = "start";
	unsigned int audiohex_size = 0;
	unsigned int package = 0;
	ssize_t read_end;
	int countbuffer[10];

	unsigned int *hardware_res;
	int *input;

	printf("Equalizer started!\n");

	audiohex = fopen("input.txt", "r");
	if (audiohex == NULL)
	{
		printf("Unable to open input.txt\n");
		exit(EXIT_FAILURE);
	}

	fseek(audiohex, 0, SEEK_END);
	audiohex_size = ftell(audiohex);
	fseek(audiohex, 0, SEEK_SET);
	printf("Number of input.txt lines: %d\n", audiohex_size / 12);
	audiohex_size = audiohex_size / 12;

	input = (int *)malloc(audiohex_size * sizeof(int));
	hardware_res = (int *)malloc(audiohex_size * sizeof(int));

	for (i = 0; i < audiohex_size; i++)
	{
		fscanf(audiohex, "%x", &input[i]);
	}
	fclose(audiohex);

	for (i = 0; i < (NUMBER_OF_AMPLIFICATIONS + NUMBER_OF_BOUNDARIES); i++)
	{
		dae = fopen("/dev/dae", "w");
		if (dae == NULL)
		{
			printf("Cannot open /dev/dae for write\n");
			exit(EXIT_FAILURE);
		}
		if (i < NUMBER_OF_AMPLIFICATIONS)
			fprintf(dae, "%d,%d\n", i, p[i]);
		else
			fprintf(dae, "%d,%d\n", i, pr[i - 10]);
		fclose(dae);
		if (dae == NULL)
		{
			printf("Cannot close /dev/dae\n");
			return -1;
		}
	}

	tx_fd = open("/dev/dma_tx", O_RDWR);

	if (tx_fd < 1)
	{
		printf("Unable to open DMA MM2S!\n");
		exit(EXIT_FAILURE);
	}

	rx_fd = open("/dev/dma_rx", O_RDWR);
	if (rx_fd < 1)
	{
		printf("Unable to open DMA S2MM!\n");
		exit(EXIT_FAILURE);
	}

	out = fopen("output.txt", "w+");
	if (out == NULL)
	{
		printf("Unable to output.txt\n");
		exit(EXIT_FAILURE);
	}

	if (audiohex_size >= (PACKAGE_LENGTH * PACKAGE_NUMBER))
	{
		package = audiohex_size;
		if (package > (PACKAGE_LENGTH * PACKAGE_NUMBER))
		{
			do
			{
				package = package - (PACKAGE_LENGTH * PACKAGE_NUMBER);
				count++;
			} while (package > (PACKAGE_LENGTH * PACKAGE_NUMBER));
		}
		printf("Number of packages: %d\n", count + 1);
	}
	package = audiohex_size;
	for (k = 0; k < count; k++)
	{
		package = package - (PACKAGE_LENGTH * PACKAGE_NUMBER);
		countbuffer[k] = (PACKAGE_LENGTH * PACKAGE_NUMBER);
	}
	countbuffer[count] = package;

	for (k = 0; k < count + 1; k++)
	{

		rx_mmap = (int *)mmap(NULL, countbuffer[k] * 4,
							  PROT_READ | PROT_WRITE, MAP_SHARED, rx_fd, 0);

		tx_mmap = (int *)mmap(NULL, countbuffer[k] * 4,
							  PROT_READ | PROT_WRITE, MAP_SHARED, tx_fd, 0);

		if ((rx_mmap == MAP_FAILED) || (tx_mmap == MAP_FAILED))
		{
			printf("Failed to mmap\n");
			exit(EXIT_FAILURE);
		}

		memcpy(tx_mmap, input + k * (PACKAGE_NUMBER * PACKAGE_LENGTH), countbuffer[k] * 4);
		write(tx_fd, &start, sizeof(start));

		usleep(200);

		read_end = read(rx_fd, &end, sizeof(end));
		while (end != '1')
		{
			read_end = read(rx_fd, &end, sizeof(end));
			usleep(2000);
		}
		printf("Equalizing completed for package %d.\n", k + 1);

		//usleep(2000);
		memcpy(hardware_res, rx_mmap, countbuffer[k] * 4);

		for (i = 0; i < countbuffer[k]; i++)
		{
			fprintf(out, "%#0010x\n", hardware_res[i]);
		}

		munmap(tx_mmap, countbuffer[k] * 4);
		munmap(rx_mmap, countbuffer[k] * 4);
		usleep(1000);
	}
	printf("Equalizer done!\n");
	fclose(out);
	close(tx_fd);
	close(rx_fd);
	return 0;
}
