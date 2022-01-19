/*
 *                                                                                      	
 *                      Port Information Dump Program                     
 *                                                                                      
 *				Copyright 2006 - 2011  SUNIX Co., Ltd. all right reserved
 *                                                                                        
 *                                                              Version: 1.2.3.1           	
 *                                                              Date: 2016/04/26        
 */
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
#define SNX_SER_DUMP_PORT_INFO  (SNX_IOCTL + 50)
#define SNX_SER_DUMP_PORT_PERF  (SNX_IOCTL + 51)
#define SNX_SER_DUMP_DRIVER_VER (SNX_IOCTL + 52)
#define SNX_PAR_DUMP_PORT_INFO  (SNX_IOCTL + 53)
#define SNX_PAR_DUMP_DRIVER_VER (SNX_IOCTL + 54)


#define SNX_BOARDNAME_LENGTH  		15
#define SNX_DRIVERVERSION_LENGTH 	15

    
struct snx_ser_port_info 
{
 	char  			board_name_info[SNX_BOARDNAME_LENGTH];
 	unsigned int  	bus_number_info;
 	unsigned int  	dev_number_info;
 	unsigned int  	port_info;
 	unsigned int 	base_info;		
  	unsigned int  	irq_info;	
};


struct snx_par_port_info 
{
 	char  			board_name_info[SNX_BOARDNAME_LENGTH];
 	unsigned int  	bus_number_info;
 	unsigned int  	dev_number_info;
 	unsigned int  	port_info;
 	unsigned int  	base_info;
 	unsigned int	base_hi_info;		
 	unsigned int  	irq_info;
 	unsigned int		minor;	
};


struct snx_info
{
 	struct snx_ser_port_info 	ser_port_info[SNX_SER_TOTAL_MAX];
	struct snx_par_port_info 	par_port_info[SNX_PAR_TOTAL_MAX];
	
	char 						driver_ver[SNX_DRIVERVERSION_LENGTH];
	
	int 						ser_count;
	int 						par_count;
	
	int 						ser_status1;
	int							ser_status2;
	int							par_status1;
	int							par_status2;
};


static int get_ser_port_info(struct snx_info *infop)
{
	char ser_name[14] = "/dev/ttySNX0";
	
	int ser_fd;
	
	ser_fd = open(ser_name, O_RDWR | O_NOCTTY);		
	
	if (ser_fd > 0)
	{
		infop->ser_status1 = ioctl(ser_fd, SNX_SER_DUMP_PORT_INFO, &infop->ser_port_info);
		infop->ser_status2 = ioctl(ser_fd, SNX_SER_DUMP_DRIVER_VER, &infop->driver_ver);	
		
		close(ser_fd);
		return 0;
	}
	
	return 0;
}


static int get_par_port_info(struct snx_info *infop)
{
	char par_name[14] = "/dev/parport0";
	int par_fd;
	int i;
		
	for (i = 0; i < SNX_PAR_TOTAL_MAX; i++)
	{
		infop->par_port_info[i].minor = i ;
		par_name[12] = '0' + i;
		
		par_fd = open(par_name, O_WRONLY);
	
    	if (par_fd > 0)
    	{
        	infop->par_status1 = ioctl(par_fd, SNX_PAR_DUMP_PORT_INFO, &infop->par_port_info[i]);
        	infop->par_status2 = ioctl(par_fd, SNX_PAR_DUMP_DRIVER_VER, &infop->driver_ver);
        	
        	close(par_fd);
    	}	
	}
	
	return 0;
}


static void print_info(struct snx_info *infop)
{
	int i;
	int ret;
	
	ret = system("clear");
	
	for (i = 0; i < SNX_SER_TOTAL_MAX; i++)
	{
		if (infop->ser_port_info[i].base_info)
		{
			infop->ser_count++;
		}
	}	

    
    for (i = 0; i < SNX_PAR_TOTAL_MAX; i++)
    {
        if (infop->par_port_info[i].base_info)
        {
            infop->par_count++;
        }
    }      
    
    
	if ((infop->ser_count > 0) || (infop->par_count > 0))
	{
	    printf("\n================ Found %2d SUNIX port , list informations ====================\n", infop->ser_count + infop->par_count);
	    if ((infop->ser_status2 == 0)||(infop->par_status2 == 0))
		    printf("                                             SUNIX driver ver -- %s\n\n", infop->driver_ver);
	    else
		    printf("\n");

	    for (i = 0; i < SNX_SER_TOTAL_MAX; i++)
	    {
		    if (!(infop->ser_port_info[i].base_info))
			    continue;
	
		    printf("ttySNX%d --\n", infop->ser_port_info[i].port_info); 
		    printf("SUNIX %s Series (bus:%d device:%2d) , base address = %4x,    irq = %2d\n\n",
				    infop->ser_port_info[i].board_name_info,
				    infop->ser_port_info[i].bus_number_info,
				    infop->ser_port_info[i].dev_number_info,
				    infop->ser_port_info[i].base_info,
				    infop->ser_port_info[i].irq_info
				    );
	    }
		    
	    for (i = 0; i < SNX_PAR_TOTAL_MAX; i++)
	    {
		    if (!(infop->par_port_info[i].base_info))
			    continue;
	
		    printf("lp%d, parport%d --\n", infop->par_port_info[i].port_info, infop->par_port_info[i].port_info); 
		    printf("SUNIX %s Series (bus:%d device:%2d) , base addr = %4x, extd addr = %4x\n\n",
				    infop->par_port_info[i].board_name_info,
				    infop->par_port_info[i].bus_number_info,
				    infop->par_port_info[i].dev_number_info,
				    infop->par_port_info[i].base_info,
				    infop->par_port_info[i].base_hi_info
				    );
	    }		    
	}
	else
	{
        printf("\n============ No any port been found, type 'lsmod' to check driver ===========\n\n");
	}

	printf("=============================================================================\n");
}


int main(void)
{
	int status = 0;
	struct snx_info info;
	
	memset(&info, 0, (sizeof(struct snx_info)));	
	
	status = get_ser_port_info(&info);
	if (status < 0)
	{
	}	
		
	status = get_par_port_info(&info);
	if (status < 0)
	{
	}		

	print_info(&info);
	
 	return 0;
}

