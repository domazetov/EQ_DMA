#include "coeficients.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

//#define AUDIO_LENGTH 1024
//#define PACKAGE_LENGTH 1024
//#define MAX_PKT_SIZE 1024 * 4
#define NUMBER_OF_AMPLIFICATIONS 10
#define NUMBER_OF_BOUNDARIES 18

int main(void)
{
	FILE *dae;
	FILE *audiohex;
	int rx_fd, tx_fd;
	int *rx_mmap;
	int *tx_mmap;
	int i, count;
	char end;
	char start[] = "start";
	int audiohex_size = 0;
	ssize_t read_end;

	//int *array = (int *)malloc(PACKAGE_LENGTH * sizeof(int));

	unsigned int *hardware_res;
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
		printf("Unable to open DMA MM2S");
		exit(EXIT_FAILURE);
	}

	rx_fd = open("/dev/dma_rx", O_RDWR);
	if (rx_fd < 1)
	{
		printf("Unable to open DMA S2MM");
		exit(EXIT_FAILURE);
	}

	dae = fopen("output.txt", "w+");

	rx_mmap = (int *)mmap(NULL, audiohex_size * 4,
						  PROT_READ | PROT_WRITE, MAP_SHARED, rx_fd, 0);

	tx_mmap = (int *)mmap(NULL, audiohex_size * 4,
						  PROT_READ | PROT_WRITE, MAP_SHARED, tx_fd, 0);

	if ((rx_mmap == MAP_FAILED) || (tx_mmap == MAP_FAILED))
	{
		printf("Failed to mmap\n");
		exit(EXIT_FAILURE);
	}

	memcpy(tx_mmap, input, audiohex_size * 4);
	write(tx_fd, &start, sizeof(start));

	usleep(200);

	read_end = read(rx_fd, &end, sizeof(end));
	while (end != '1')
	{
		read_end = read(rx_fd, &end, sizeof(end));
		usleep(2000);
	}
	printf("Equalizing completed: %c\n", end);

	usleep(200);
	memcpy(hardware_res, rx_mmap, audiohex_size * 4);

	for (i = 0; i < audiohex_size; i++)
	{
		fprintf(dae, "%#0010x\n", hardware_res[i]);
	}

	munmap(tx_mmap, audiohex_size * 4);
	munmap(rx_mmap, audiohex_size * 4);
	fclose(dae);
	close(tx_fd);
	close(rx_fd);

	return 0;
}
