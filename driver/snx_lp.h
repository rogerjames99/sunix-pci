#ifndef _LINUX_SNX_LP_H
#define _LINUX_SNX_LP_H


#define SNX_LP_EXIST				0x0001
#define SNX_LP_SELEC				0x0002
#define SNX_LP_BUSY					0x0004
#define SNX_LP_BUSY_BIT_POS 		2
#define SNX_LP_OFFL					0x0008
#define SNX_LP_NOPA					0x0010
#define SNX_LP_ERR					0x0020
#define SNX_LP_ABORT				0x0040
#define SNX_LP_CAREFUL				0x0080
#define SNX_LP_ABORTOPEN			0x0100

#define SNX_LP_TRUST_IRQ_ 	 		0x0200
#define SNX_LP_NO_REVERSE  			0x0400
#define SNX_LP_DATA_AVAIL  			0x0800


#define SNX_LP_PBUSY				0x80
#define SNX_LP_PACK					0x40
#define SNX_LP_POUTPA				0x20
#define SNX_LP_PSELECD				0x10
#define SNX_LP_PERRORP				0x08


#define SNX_LP_INIT_CHAR 			1000
#define SNX_LP_INIT_WAIT 			1
#define SNX_LP_INIT_TIME 			2


#define SNX_LPCHAR   				0x0601
#define SNX_LPTIME   				0x0602
#define SNX_LPABORT  				0x0604
#define SNX_LPSETIRQ 				0x0605
#define SNX_LPGETIRQ 				0x0606
#define SNX_LPWAIT   				0x0608


#define SNX_LPCAREFUL				0x0609
#define SNX_LPABORTOPEN				0x060a
#define SNX_LPGETSTATUS				0x060b
#define SNX_LPRESET					0x060c

#ifdef SNX_LP_STATS
#define SNX_LPGETSTATS				0x060d
#endif

#define SNX_LPGETFLAGS	 			0x060e
#define SNX_LPSETTIMEOUT 			0x060f


#define SNX_LP_TIMEOUT_INTERRUPT	(60 * HZ)
#define SNX_LP_TIMEOUT_POLLED		(10 * HZ)


#define __KERNEL__ 				1

#ifdef __KERNEL__

#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 24))
#include <asm/semaphore.h>
#else
#include <linux/mutex.h>
#endif

#define SNX_LP_PARPORT_UNSPEC		-4
#define SNX_LP_PARPORT_AUTO			-3
#define SNX_LP_PARPORT_OFF			-2
#define SNX_LP_PARPORT_NONE			-1

#define SNX_LP_F(minor)				snx_lp_table[(minor)].flags
#define SNX_LP_CHAR(minor)			snx_lp_table[(minor)].chars
#define SNX_LP_TIME(minor)			snx_lp_table[(minor)].time
#define SNX_LP_WAIT(minor)			snx_lp_table[(minor)].wait
#define SNX_LP_IRQ(minor)			snx_lp_table[(minor)].dev->port->irq


#ifdef SNX_LP_STATS
#define SNX_LP_STAT(minor)			snx_lp_table[(minor)].stats
#endif
#define SNX_LP_BUFFER_SIZE PAGE_SIZE

#define SNX_LP_BASE(x)				snx_lp_table[(x)].dev->port->base


#ifdef SNX_LP_STATS
struct snx_lp_stats {
	unsigned long 	chars;
	unsigned long 	sleeps;
	unsigned int 	maxrun;
	unsigned int 	maxwait;
	unsigned int 	meanwait;
	unsigned int 	mdev;
};
#endif




struct snx_lp_struct {
	struct snx_pardevice 	*dev;
	unsigned long 			flags;
	unsigned int 			chars;
	unsigned int 			time;
	unsigned int 			wait;
	char 					*lp_buffer;

#ifdef SNX_LP_STATS
	unsigned int 			lastcall;
	unsigned int 			runchars;
	struct snx_lp_stats		stats;
#endif

	wait_queue_head_t 		waitq;
	unsigned int 			last_error;
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 24))
	struct semaphore 		port_mutex;
#else
	struct mutex 			port_mutex;
#endif
	wait_queue_head_t 		dataq;
	long 					timeout;
	unsigned int 			best_mode;
	unsigned int 			current_mode;
	unsigned long 			bits;
};


#define SNX_LP_PINTEN				0x10
#define SNX_LP_PSELECP				0x08
#define SNX_LP_PINITP				0x04
#define SNX_LP_PAUTOLF				0x02
#define SNX_LP_PSTROBE				0x01

#define SNX_LP_DUMMY				0x00

#define SNX_LP_DELAY 				50

#endif
#endif

