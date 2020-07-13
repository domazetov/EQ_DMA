#include "coeficients.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define PACKAGE_NUMBER 1292
#define PACKAGE_LENGTH 1024
//#define MAX_PKT_SIZE 1024 * 4
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

	int *pkg = (int *)malloc(PACKAGE_NUMBER * PACKAGE_LENGTH * sizeof(int));

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

	for (i = 0; i < audiohex_size; i++) //for (i = 0; i < AUDIO_LENGTH; i++)
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

	printf("Checking input size:\n");
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
		printf("count: %d\n", count);
	}
	package = audiohex_size;
	printf("x1\n");
	for (k = 0; k < count; i++)
	{
		package = package - (PACKAGE_LENGTH * PACKAGE_NUMBER);
		countbuffer[k] = (PACKAGE_LENGTH * PACKAGE_NUMBER);
		printf("%d\n", k);
	}
	countbuffer[count] = package;
	printf("x2\n");
	for (k = 0; k < count + 1; k++)
	{
		printf("countbuff %d\n", countbuffer[k]);
	}

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

		// for (count = 0; count < audiohex_size; count = count + (PACKAGE_NUMBER * PACKAGE_LENGTH))
		// {
		memcpy(pkg, input + k, PACKAGE_NUMBER * PACKAGE_LENGTH * sizeof(int));

		memcpy(tx_mmap, pkg, countbuffer[k] * 4);
		write(tx_fd, &start, sizeof(start));

		usleep(200);

		read_end = read(rx_fd, &end, sizeof(end));
		while (end != '1')
		{
			read_end = read(rx_fd, &end, sizeof(end));
			usleep(2000);
		}
		printf("Equalizing completed: %c\n", end);

		usleep(2000);
		memcpy(hardware_res, rx_mmap, countbuffer[k] * 4);

		for (i = 0; i < countbuffer[k]; i++)
		{
			fprintf(out, "%#0010x\n", hardware_res[i]);
		}
		//}

		munmap(tx_mmap, countbuffer[k] * 4);
		munmap(rx_mmap, countbuffer[k] * 4);
		usleep(1000);
	}
	fclose(out);
	close(tx_fd);
	close(rx_fd);
	return 0;
}
