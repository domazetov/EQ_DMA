#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define TEST_SIZE 1024

int main(void)
{
	printf("DMA test\n");
	// If memory map is defined send image directly via mmap
	int rx_proxy_fd, tx_proxy_fd;
	int counter, i, x = 0;
	int *rx;
	int *tx;

	int input[TEST_SIZE];
	int output[TEST_SIZE];

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

	rx = (int *)mmap(NULL, TEST_SIZE,
					 PROT_READ | PROT_WRITE, MAP_SHARED, rx_proxy_fd, 0);

	tx = (int *)mmap(NULL, TEST_SIZE,
					 PROT_READ | PROT_WRITE, MAP_SHARED, tx_proxy_fd, 0);

	if ((rx == MAP_FAILED) || (tx == MAP_FAILED))
	{
		printf("Failed to mmap\n");
		exit(EXIT_FAILURE);
	}

	for (counter = 0; counter < 2; counter++)
	{
		for (i = 0; i < 5; i++)
		{
			input[x] = counter + i;
			x++;
		}
	}

	memcpy(tx, input, TEST_SIZE);

	memcpy(output, rx, TEST_SIZE);

	munmap(tx, TEST_SIZE);
	munmap(rx, TEST_SIZE);
	x = 0;
	for (counter = 0; counter < 2; counter++)
	{
		for (i = 0; i < 5; i++)
		{
			printf("input: %d	output: %d\n", input[x], output[x]);
			x++;
		}
	}

	close(tx_proxy_fd);
	close(rx_proxy_fd);

	printf("DMA test complete\n");
	return 0;
}
