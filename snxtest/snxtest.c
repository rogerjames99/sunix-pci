#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <unistd.h>


#define SNX_SER_TOTAL_MAX	32
#define SNX_PAR_TOTAL_MAX	4


#define SNX_IOCTL  0x900
#define SNX_PPCLAIM	(SNX_IOCTL + 0x8b)

#define SNX_BOARDNAME_LENGTH  		15
#define SNX_DRIVERVERSION_LENGTH 	15

static int claim()
{
    char par_name[14] = "/dev/parport0";
    int par_fd;
    int i;

    for (i = 0; i < SNX_PAR_TOTAL_MAX; i++)
    {
	par_name[12] = '0' + i;
	printf("Trying %s\n", par_name);
	par_fd = open(par_name, O_WRONLY);
	if (par_fd > 0)
	{
	    printf("Trying SNX_PPCLAIM on %s\n", par_name);
	    if (0 > ioctl(par_fd, SNX_PPCLAIM))
		perror("SNX_PPCLAIM failed");
	    else
		printf("SNX_PPCLAIM success\n");
        }
	else
            perror("open failed");
    }
    return par_fd;
}

int main(void)
{
    int par_fd = claim();
    if (par_fd < 0)
    {
        printf("Fucked\n");
        return -1;
    }

    sleep(120);
    close(par_fd);
    return 0;
}
