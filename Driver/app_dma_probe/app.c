#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define MAX_PKT_SIZE 1024*4
#define PACKAGE_LENGTH 1024

int main(void)
{
	int fd;
	int *p;
	int* hardware_res;

	fd = open("/dev/dm_s2mm", O_RDWR|O_NDELAY);
	if (fd < 0)
	{
		printf("Cannot open /dev/dm_s2mm for write\n");
		return -1;
	}
	//hardware_res = (int *) malloc(PACKAGE_LENGTH*sizeof(int));
	p=(int*)mmap(0,MAX_PKT_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	printf("\nYOLO\n");
	//memcpy(hardware_res, p, MAX_PKT_SIZE);
	munmap(p, MAX_PKT_SIZE);
	close(fd);
	if (fd < 0)
	{
		printf("Cannot close /dev/dm_s2mm for write\n");
		return -1;
	}
	return 0;
}


