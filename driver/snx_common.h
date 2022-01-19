#ifdef	MODVERSIONS
#ifndef MODULE
#define	MODULE
#endif
#endif

#include "driver_extd.h"

#include <linux/version.h>
#ifndef KERNEL_VERSION
#define KERNEL_VERSION(ver, rel, seq)	((ver << 16) | (rel << 8) | seq)
#endif

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0))
#ifdef MODULE
#include <linux/config.h>
#ifdef MODVERSIONS
#include <linux/modversions.h>
#endif
#include <linux/module.h>
#else
#define	MOD_INC_USE_COUNT
#define MOD_DEC_USE_COUNT
#endif


#include <linux/autoconf.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/major.h>
#include <linux/string.h>
#include <linux/fcntl.h>
#include <linux/ptrace.h>
#include <linux/delay.h>
#include <asm/bitops.h>

#ifndef PCI_ANY_ID
#define PCI_ANY_ID (~0)
#endif
#endif

#include <linux/errno.h>
#include <linux/signal.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial.h>
#include <linux/serial_reg.h>
#include <linux/ioport.h>
#include <linux/mm.h>

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 39))
#include <linux/smp_lock.h>
#endif

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/tty_driver.h>
#include <linux/pci.h>
#include <linux/circ_buf.h>

#include <asm/uaccess.h>
//#include <asm/system.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/segment.h>
#include <asm/serial.h>
#include <linux/interrupt.h>


#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/moduleparam.h>
#include <linux/console.h>
#include <linux/sysrq.h>
//#include <linux/serialP.h>
#include <linux/delay.h>
#include <linux/device.h>
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 28))
#include <linux/kref.h>
#endif

#include <linux/parport.h>
#include <linux/ctype.h>
#include <linux/poll.h>


#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 17))
#include <linux/devfs_fs_kernel.h>
#endif


#include <linux/sched.h>

#include <linux/serial_8250.h>
#include <linux/cdev.h>

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0))
#include <linux/sched/signal.h>
#endif
// globe variable

extern int snx_board_count ;

/*-------------------------------------------------------------------------------

				for snx_main.c

-------------------------------------------------------------------------------*/
/*******************************************************
			SUNIX driver information
*******************************************************/
#define SNX_DRIVER_VERSION 		"V2.0.6.0"
#define SNX_DRIVER_DATE			"2021/11/26"
#define SNX_DRIVER_AUTHOR  		"SUNIX Co., Ltd.<info@sunix.com.tw>"
#define SNX_DRIVER_DESC  		"SUNIX Multi-I/O Board Driver Module"

#define SNX_DBG					0
#define SNX_DBG_BOARD			0
#define SNX_DBG_SERPORT			0
#define SNX_DBG_PARPORT			0

/*******************************************************
			SUNIX board information
*******************************************************/
#define	VENID_GOLDEN			0x1409
#define	DEVID_G_SERIAL			0x7168
#define DEVID_G_PARALL			0x7268
#define SUBVENID_GOLDEN			0x1409


#define	VENID_MATRIX			0x1FD4
#define	DEVID_M_SERIAL			0x0001
#define SUBVENID_MATRIX			0x0001


#define	VENID_SUN1999			0x1FD4
#define	DEVID_S_SERIAL			0x1999
//#define DEVID_S_PARALL		0x0000
#define DEVID_S_PARALL			0x1999
#define SUBVENID_SUN1999		0x1FD4


// golden board

#define SUBDEVID_TEST  			0x9999
// 4027A
#define SUBDEVID_4027A			0x4027
// 4027D
#define SUBDEVID_4027D			0x5027
// 4037A, 4037AL
#define SUBDEVID_4037A			0x4037
// 4037D
#define SUBDEVID_4037D			0x5037
// 4036A3V
#define SUBDEVID_4036A3V		0x0002
// 4056A, 4056P
#define SUBDEVID_4056A			0x4056
// 4056D
#define SUBDEVID_4056D			0x6056
// 4055WN, 4056WN, 4056DW
#define SUBDEVID_4056DW			0x4055
// 4066A
#define SUBDEVID_4066A			0x4066
// 4066R
#define SUBDEVID_4066R			0x5066
// 8139
#define SUBDEVID_8139			0x8138
// 8139S, 8139SI
#define SUBDEVID_8139S			0x9138
// 8159
#define SUBDEVID_8159			0x8156
// 8159S, 8159SI
#define SUBDEVID_8159S			0x9156
// 8169
#define SUBDEVID_8169			0x8166
// 8169S, 8169SI
#define SUBDEVID_8169S			0x9166
// 4008A
#define SUBDEVID_4008A			0x0103
// 4018A
#define SUBDEVID_4018A			0x0104
// 4079A
#define SUBDEVID_4079A			0x5079
// 4089A
#define SUBDEVID_4089A			0x4089
// 4096A
#define SUBDEVID_4096A			0x4096


// matrix board
// P1002
#define SUBDEVID_P1002			0x1002
// P1004
#define SUBDEVID_P1004			0x1004
// P1008
#define SUBDEVID_P1008			0x1008
// P1016
#define SUBDEVID_P1016			0x1016
// P2002
#define SUBDEVID_P2002			0x2002
// P2004
#define SUBDEVID_P2004			0x2004
// P2008
#define SUBDEVID_P2008			0x2008
// P3002
#define SUBDEVID_P3002			0x3002
// P3004
#define SUBDEVID_P3004			0x3004
// P3008
#define SUBDEVID_P3008			0x3008


// SUN1999 board
//5027A 5027H 5027AL 5027HL
#define SUBDEVID_5027A			0x0001
//5037A 5037H
#define SUBDEVID_5037A			0x0002
//5056A 5056AL 5056H 5056HL
#define SUBDEVID_5056A			0x0004
//5066A 5066H
#define SUBDEVID_5066A			0x0008
//5016A
#define SUBDEVID_5016A			0x0010


// SUN1999-multi I/O
//5069A 5069H
#define SUBDEVID_5069A			0x0101
//5079A 5079H
#define SUBDEVID_5079A			0x0102
//5099A 5099H
#define SUBDEVID_5099A			0x0104

// SUN1999-parallel board
//5008A/AL
#define SUBDEVID_5008A			0x0100

//IPC-P2102 IPC-P2102SI
#define SUBDEVID_P2102			0x0002
//IPC-P2104 IPC-P2104SI
#define SUBDEVID_P2104			0x0004
//IPC-P2108 IPC-P2108SI
#define SUBDEVID_P2108			0x0008
//IPC-P2116
#define SUBDEVID_P2116			0x0010

// 3_in_1
//IPC-P3104 IPC-P3104SI
#define SUBDEVID_P3104			0x0084

//IPC-P3108 IPC-P3104SI
#define SUBDEVID_P3108			0x0088

//CASH DRAWER CARD
#define SUBDEVID_CASH_2S		0x0002
#define SUBDEVID_CASH_4S		0x0004

//DIO CARD
#define SUBDEVID_DIO0802		0x0002

#define SUBDEVID_DIO1604		0x0004

#define SUBDEVID_DIO3204		0x0004


// for chip_flag
#define SUNNONE_HWID			0x0000
#define SUN1889_HWID  			0x0001
#define SUN1699_HWID  			0x0002
#define SUN1888_HWID			0x0004
#define SUN1689_HWID			0x0008
#define SUNMATX_HWID			0x0010
#define SUN1999_HWID			0x0020

// for board_flag
#define BOARDFLAG_NONE			0x0000
#define BOARDFLAG_REMAP			0x0001
#define BOARDFLAG_16PORTS		0x0002

// for port_flag
#define PORTFLAG_NONE			0x0000
#define PORTFLAG_REMAP			0x0001
#define PORTFLAG_16PORTS		0x0002

// for part_number
#define PART_NUMBER_NONE    0x0000

// for card_type )
#define CARD_TYPE_UART_ONLY    0x00
#define CARD_TYPE_UART_GINTR   0x01
#define CARD_TYPE_UART_GEXTR   0x02

// for Gpio_ch_cnt )
#define GPIO_NONE   		0
#define INTR_GPIO_6PORT   	6
#define EXTR_GPIO_8PORT   	8
#define EXTR_GPIO_16PORT  	16
#define EXTR_GPIO_32PORT   	32

// for port_flag
#define PORTFLAG_NONE			0x0000
#define PORTFLAG_REMAP			0x0001
#define PORTFLAG_16PORTS		0x0002


// board info
#define SNX_BOARDS_MAX  			4
#define SNX_PORT_ONBOARD_MAX 	16

#define SNX_SER_TOTAL_MAX			32
#define SNX_SER_ONBOARD_MAX  	16

// include lp0 and lp1 (mother board)
#define SNX_PAR_SUPPORT_MAX		2
#define SNX_PAR_TOTAL_MAX			4
#define SNX_PAR_ONBOARD_MAX  	2


/*******************************************************
				uart information
*******************************************************/
#define SUN1699_CLK_DIVIDER_DISABLE  		0x10

// 1889 uart fifo info
#define SUN1889_FIFOSIZE_16  				16
#define SUN1889_TRIGGER_LEVEL_16FIFO_01		1
#define SUN1889_TRIGGER_LEVEL_16FIFO_04  	4
#define SUN1889_TRIGGER_LEVEL_16FIFO_08  	8
#define SUN1889_TRIGGER_LEVEL_16FIFO_14  	14

#define SUN1889_FIFOSIZE_32  				32
#define SUN1889_TRIGGER_LEVEL_32FIFO_08  	8
#define SUN1889_TRIGGER_LEVEL_32FIFO_16  	16
#define SUN1889_TRIGGER_LEVEL_32FIFO_24  	24
#define SUN1889_TRIGGER_LEVEL_32FIFO_28  	28


// 1699 uart fifo info
#define SUN1699_FIFOSIZE_16  				16
#define SUN1699_TRIGGER_LEVEL_16FIFO_01  	1
#define SUN1699_TRIGGER_LEVEL_16FIFO_04  	4
#define SUN1699_TRIGGER_LEVEL_16FIFO_08  	8
#define SUN1699_TRIGGER_LEVEL_16FIFO_14  	14


#define SUN1699_FIFOSIZE_32  				32
#define SUN1699_TRIGGER_LEVEL_32FIFO_08  	8
#define SUN1699_TRIGGER_LEVEL_32FIFO_16  	16
#define SUN1699_TRIGGER_LEVEL_32FIFO_24  	24
#define SUN1699_TRIGGER_LEVEL_32FIFO_28  	28


#define SUN1699_FIFOSIZE_64  				64
#define SUN1699_TRIGGER_LEVEL_64FIFO_16  	16
#define SUN1699_TRIGGER_LEVEL_64FIFO_32  	32
#define SUN1699_TRIGGER_LEVEL_64FIFO_48  	48
#define SUN1699_TRIGGER_LEVEL_64FIFO_56 	56


// matrix uart fifo info
#define SUNMATX_FIFOSIZE_16  				16
#define SUNMATX_TRIGGER_LEVEL_16FIFO_01		1
#define SUNMATX_TRIGGER_LEVEL_16FIFO_04  	4
#define SUNMATX_TRIGGER_LEVEL_16FIFO_08  	8
#define SUNMATX_TRIGGER_LEVEL_16FIFO_14  	14

#define SUNMATX_FIFOSIZE_64  				64
#define SUNMATX_TRIGGER_LEVEL_64FIFO_01  	1
#define SUNMATX_TRIGGER_LEVEL_64FIFO_16  	16
#define SUNMATX_TRIGGER_LEVEL_64FIFO_32  	32
#define SUNMATX_TRIGGER_LEVEL_64FIFO_56  	56


// 1999 uart fifo info
#define SUN1999_FIFOSIZE_32  				32
#define SUN1999_TRIGGER_LEVEL_32FIFO_01		1
#define SUN1999_TRIGGER_LEVEL_32FIFO_8  	8
#define SUN1999_TRIGGER_LEVEL_32FIFO_16  	16
#define SUN1999_TRIGGER_LEVEL_32FIFO_28  	28

#define SUN1999_FIFOSIZE_128  				128
#define SUN1999_TRIGGER_LEVEL_128FIFO_01  	1
#define SUN1999_TRIGGER_LEVEL_128FIFO_32  	32
#define SUN1999_TRIGGER_LEVEL_128FIFO_64  	64
#define SUN1999_TRIGGER_LEVEL_128FIFO_112  112


// uart fifo setup
#define SUN1889_FIFOSIZE_SET  			SUN1889_FIFOSIZE_32
#define SUN1889_TRIGGER_LEVEL_SET  		SUN1889_TRIGGER_LEVEL_32FIFO_16

#define SUN1699_FIFOSIZE_SET  			SUN1699_FIFOSIZE_32
#define SUN1699_TRIGGER_LEVEL_SET  		SUN1699_TRIGGER_LEVEL_32FIFO_16

#define SUNMATX_FIFOSIZE_SET  			SUNMATX_FIFOSIZE_64
#define SUNMATX_TRIGGER_LEVEL_SET  		SUNMATX_TRIGGER_LEVEL_64FIFO_32

#define SUN1999_FIFOSIZE_SET  			SUN1999_FIFOSIZE_128
#define SUN1999_TRIGGER_LEVEL_SET  		SUN1999_TRIGGER_LEVEL_128FIFO_64


#define UART_SUN1889_FCR_16BYTE				0x00
#define UART_SUN1889_FCR_32BYTE				0x20

#define UART_SUN1699_FCR_16BYTE 			0x00
#define UART_SUN1699_FCR_32BYTE				0x20
#define UART_SUN1699_FCR_64BYTE				0x30

#define UART_SUNMATX_FCR_64BYTE				0x20

#define UART_SUN1999_FCR_128BYTE			0x20

#define UART_DEFAULT_FCR					0x00


#define DEFAULT_FIFOSIZE  					1
#define DEFAULT_TRIGGER_LEVEL  				1


// register status info
#define UART_LSR_ERR_IN_RFIFO  				0x80
#define UART_MCR_AFE  						0x20
#define UART_IIR_CTO  						0x0C


// interrupt vedtor offset
#define SUN1889_INTRSERVREG  				0x1C

// serial address length
#define SNX_SER_ADDRESS_LENGTH 				8

// parallel address length
#define SNX_PAR_ADDRESS_LENGTH 				8
#define SNX_PAR_STD_ADDR_LENGTH				3
#define SNX_PAR_ETD_ADDR_LENGTH				5


// PCI configuration bar 0 ~ 5
#define SNX_PCICFG_BAR_TOTAL				6


/*******************************************************
				miscellaneous Information
*******************************************************/
#define INTERRUPT_COUNT 128
#define WAKEUP_CHARS    256


#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 19))
#define SNXTERMIOS  ktermios
#else
#define SNXTERMIOS  termios
#endif


// for snx_ser_port->snx_type
#define SNX_SER_PORT_MAX_UART	5
#define SNX_SER_PORT_UNKNOWN	0
#define SNX_SER_PORT_SUN1889	1
#define SNX_SER_PORT_SUN1699	2
#define SNX_SER_PORT_SUNMATX	3
#define SNX_SER_PORT_SUN1999    4


// for snx_ser_port->setserial_flag
#define SNX_SER_BAUD_SETSERIAL	1
#define SNX_SER_BAUD_NOTSETSER	0


/*******************************************************
			struct define Information
*******************************************************/
// name length
#define SNX_BOARDNAME_LENGTH  15
#define SNX_DRIVERVERSION_LENGTH 15


struct snx_ser_port_info {
  char  				board_name_info[SNX_BOARDNAME_LENGTH];
  unsigned int  bus_number_info;
  unsigned int  dev_number_info;
  unsigned int  port_info;
  unsigned int  base_info;
  unsigned int  irq_info;
};


struct snx_par_port_info {
	char  			board_name_info[SNX_BOARDNAME_LENGTH];
	unsigned int  	bus_number_info;
	unsigned int  	dev_number_info;
	unsigned int  	port_info;
	unsigned int  	base_info;
	unsigned int	base_hi_info;
	unsigned int  	irq_info;
	unsigned int	minor;

};


typedef struct _PORT {
	char					type;

	int						bar1;
	unsigned char	offset1;
	unsigned char	length1;

	int						bar2;
	unsigned char	offset2;
	unsigned char	length2;

	unsigned int	intmask;
	unsigned int	chip_flag;

} PORT;


typedef struct _PCI_BOARD {
	unsigned int	vendor_id;
	unsigned int 	device_id;
	unsigned int	sub_vendor_id;
	unsigned int	sub_device_id;

	unsigned int	num_serport;
	unsigned int	num_parport;

	unsigned int	intr_vector_bar;
	unsigned char	intr_vector_offset;

	char					board_name[SNX_BOARDNAME_LENGTH];
	unsigned int	board_flag;
	unsigned int	part_number ;
	unsigned int	card_type ;
	int						gpio_ch_cnt ;

	PORT			port[SNX_PORT_ONBOARD_MAX];

} PCI_BOARD;


struct sunix_board;
struct sunix_ser_port;
struct sunix_par_port;


struct sunix_board {
	int						board_enum;
	int						board_number;
	unsigned int			bus_number;
	unsigned int			dev_number;

	unsigned int			ports;
	unsigned int			ser_port;
	unsigned int			par_port;

	unsigned int			ser_port_index;
	unsigned int			par_port_index;

	unsigned int			bar_addr[SNX_PCICFG_BAR_TOTAL];
	unsigned int			irq;

	unsigned int			board_flag;

	unsigned int			vector_mask;
	PCI_BOARD        	pb_info;
	struct pci_dev		*pdev;
	int						(*ser_isr)(struct sunix_board *, struct sunix_ser_port *);
	int						(*par_isr)(struct sunix_board *, struct sunix_par_port *);

	unsigned int      oem_id;
  int               uart_cnt;
  int               gpio_chl_cnt;
  unsigned char     board_uart_type;
  unsigned char     board_gpio_type;
  unsigned char     board_gpio_card_type;

};


/*-------------------------------------------------------------------------------

						for snx_serial.c

-------------------------------------------------------------------------------*/
/*******************************************************
			ioctl user define
*******************************************************/
#define SNX_IOCTL  0x900
#define SNX_SER_DUMP_PORT_INFO  (SNX_IOCTL + 50)
#define SNX_SER_DUMP_PORT_PERF  (SNX_IOCTL + 51)
#define SNX_SER_DUMP_DRIVER_VER (SNX_IOCTL + 52)
#define SNX_PAR_DUMP_PORT_INFO  (SNX_IOCTL + 53)
#define SNX_PAR_DUMP_DRIVER_VER (SNX_IOCTL + 54)


/*******************************************************
			serial define
*******************************************************/
#define PORT_SER_UNKNOWN		0
#define PORT_SER_8250			1
#define PORT_SER_16450			2
#define PORT_SER_16550			3
#define PORT_SER_16550A			4
#define PORT_SER_CIRRUS         5
#define PORT_SER_16650			6
#define PORT_SER_16650V2		7
#define PORT_SER_16750			8
#define PORT_SER_MAX_UART		8	/* max serial port ID */


#define SNX_USF_CLOSING_WAIT_INF	(0)
#define SNX_USF_CLOSING_WAIT_NONE	(65535)
#define SNX_UART_CONFIG_TYPE			(1 << 0)
#define SNX_UART_CONFIG_IRQ				(1 << 1)

#define SNX_UART_XMIT_SIZE 				4096

#define snx_ser_circ_empty(circ)		   			((circ)->head == (circ)->tail)
#define snx_ser_circ_clear(circ)		    		((circ)->head = (circ)->tail = 0)
#define snx_ser_circ_chars_pending(circ)    (CIRC_CNT((circ)->head, (circ)->tail, SNX_UART_XMIT_SIZE))
#define snx_ser_circ_chars_free(circ)       (CIRC_SPACE((circ)->head, (circ)->tail, SNX_UART_XMIT_SIZE))
#define snx_ser_tx_stopped(port)            ((port)->info->tty->stopped || (port)->info->tty->hw_stopped)


#if defined(__i386__) && (defined(CONFIG_M386) || defined(CONFIG_M486))
#define SNX_SERIAL_INLINE
#endif

#ifdef SNX_SERIAL_INLINE
#define _INLINE_ inline
#else
#define _INLINE_
#endif


#define SNX_UPIO_PORT				(0)
#define SNX_UPIO_MEM				(1)

#define SNX_UPF_SAK						(1 << 2)
#define SNX_UPF_SPD_MASK			(0x1030)
#define SNX_UPF_SPD_HI				(0x0010)
#define SNX_UPF_SPD_VHI				(0x0020)
#define SNX_UPF_SPD_CUST			(0x0030)
#define SNX_UPF_SPD_SHI				(0x1000)
#define SNX_UPF_SPD_WARP			(0x1010)
#define SNX_UPF_SKIP_TEST			(1 << 6)
#define SNX_UPF_HARDPPS_CD		(1 << 11)
#define SNX_UPF_LOW_LATENCY		(1 << 13)
#define SNX_UPF_BUGGY_UART		(1 << 14)
#define SNX_UPF_MAGIC_MULTIPLIER	(1 << 16)

#define SNX_UPF_CHANGE_MASK		(0x17fff)
#define SNX_UPF_USR_MASK			(SNX_UPF_SPD_MASK | SNX_UPF_LOW_LATENCY)


#define SNX_UIF_CHECK_CD			(1 << 25)
#define SNX_UIF_CTS_FLOW			(1 << 26)

#define SNX_UIF_NORMAL_ACTIVE		(1 << 29)
#define SNX_UIF_INITIALIZED			(1 << 31)


#define SNX_ENABLE_MS(port, cflag)    ((port)->flags & SNX_UPF_HARDPPS_CD || (cflag) & CRTSCTS || !((cflag) & CLOCAL))


#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
#define SNX_SER_DEVNUM(x)   ((x)->index)
#else
#define SNX_SER_DEVNUM(x)   (MINOR((x)->device) - (x)->driver.minor_start)
#endif


struct snx_ser_info;
struct snx_ser_port;

struct snx_ser_icount {
	__u32	cts;
	__u32	dsr;
	__u32	rng;
	__u32	dcd;
	__u32	rx;
	__u32	tx;
	__u32	frame;
	__u32	overrun;
	__u32	parity;
	__u32	brk;
	__u32	buf_overrun;
};


struct snx_ser_info {
	struct tty_struct		*tty;
	struct circ_buf			xmit;
	unsigned int			flags;
	unsigned char			*tmpbuf;
	struct semaphore		tmpbuf_sem;
	int						blocked_open;
	struct tasklet_struct	tlet;

	wait_queue_head_t		open_wait;
	wait_queue_head_t		delta_msr_wait;
};


struct snx_ser_driver {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 0))
	struct module	*owner;
	const char 		*driver_name;
#endif

	const char				*dev_name;
	int			 			major;
	int			 			minor;
	int			 			nr;
	struct snx_ser_state	*state;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
	struct tty_driver		*tty_driver;
#else
	struct tty_driver   tty_driver;
#endif
};


struct snx_ser_port {
	spinlock_t				lock;
	unsigned int			iobase;
	unsigned int			irq;
	unsigned int			uartclk;
	unsigned char			fifosize;
	unsigned char			x_char;
	unsigned char			iotype;

	unsigned int			read_status_mask;
	unsigned int			ignore_status_mask;
	struct snx_ser_info		*info;
	struct snx_ser_icount	icount;


	unsigned int			flags;
	unsigned int			mctrl;
	unsigned int			timeout;
	unsigned int			snx_type;
	unsigned int			type;
	unsigned int			custom_divisor;
	unsigned int			line;


	int						board_enum;
	unsigned int			bus_number;
	unsigned int			dev_number;
	PCI_BOARD				pb_info;
	unsigned int			vector;
	unsigned int			vector_mask;
	unsigned char			chip_flag;
	unsigned int			port_flag;
	unsigned int			baud_base;
	int						rx_trigger;
	unsigned char			ldisc_stop_rx;

	unsigned int			setserial_flag;

	int AHDC_State;
	int RS422_State;

    unsigned char			suspended;
	struct device			*dev;
};


struct snx_ser_state {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 0))
	struct tty_port			tport;
#endif

	unsigned int			close_delay;
	unsigned int			closing_wait;
	int								count;
	struct snx_ser_info	    *info;
	struct snx_ser_port		*port;
	struct semaphore		sem;
};


static inline int snx_ser_handle_break(struct snx_ser_port *port)
{
	struct snx_ser_info *info = port->info;

	if (info->flags & SNX_UPF_SAK) {
		do_SAK(info->tty);
	}
	return 0;
}


static inline void snx_ser_handle_dcd_change(struct snx_ser_port *port, unsigned int status)
{
	struct snx_ser_info *info = port->info;

	port->icount.dcd++;

	if (info->flags & SNX_UIF_CHECK_CD) {
		if (status) {
			wake_up_interruptible(&info->open_wait);
		} else if (info->tty) {
			tty_hangup(info->tty);
		}
	}
}


#include <linux/tty_flip.h>



/*******************************************************
				sunix serial port struct
*******************************************************/
struct sunix_ser_port {
	struct snx_ser_port port;
	struct timer_list	timer;
	struct list_head	list;

	unsigned int		capabilities;
	unsigned char		ier;
	unsigned char		lcr;
	unsigned char		mcr;
	unsigned char		mcr_mask;
	unsigned char		mcr_force;
	unsigned char		lsr_break_flag;
};


/*-------------------------------------------------------------------------------

							for snx_parallel.c

-------------------------------------------------------------------------------*/
#define SNX_CONFIG_PARPORT_1284
#define SNX_CONFIG_PARPORT_PC_FIFO

#ifdef SNX_CONFIG_PARPORT_1284
#undef SNX_CONFIG_PARPORT_1284
#endif

#ifdef SNX_CONFIG_PARPORT_PC_FIFO
#undef SNX_CONFIG_PARPORT_PC_FIFO
#endif

#define SNX_PAR_PORT_MAX_UART	5
#define SNX_PAR_PORT_UNKNOWN	0
#define SNX_PAR_PORT_SUN1888	1
#define SNX_PAR_PORT_SUN1689	2
#define SNX_PAR_PORT_SUNMATX	3
#define SNX_PAR_PORT_SUN1999    4


#define SNX_ECR(p) 		((p)->base_hi + 0x2)
#define SNX_CFGB(p)		((p)->base_hi + 0x1)
#define SNX_CFGA(p)		((p)->base_hi + 0x0)
#define SNX_FIFO(p)    	((p)->base_hi + 0x0)
#define SNX_EPPDATA(p) 	((p)->base    + 0x4)
#define SNX_EPPADDR(p) 	((p)->base    + 0x3)
#define SNX_DCR(p)  	((p)->base    + 0x2)
#define SNX_DSR(p)   	((p)->base    + 0x1)
#define SNX_DATA(p)     ((p)->base    + 0x0)


#define SNX_ROUND_UP(x, y) (((x)+(y)-1)/(y))

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef PARPORT_W91284PIC
#define PARPORT_W91284PIC (1 << 1)
#endif


#define SNX_PARPORT_INACTIVITY_O_NONBLOCK 1

/*******************************************************
				sunix parallel port struct
*******************************************************/

struct snx_parport;
struct snx_pardevice;


struct snx_parport_driver {
	const char 					*name;
	void 						(*attach) (struct snx_parport *);
	void 						(*detach) (struct snx_parport *);
	struct list_head 			list;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 0))
	struct snx_parport_driver	*next;
#endif
};


struct snx_pc_parport_state {
	unsigned int ctr;
	unsigned int ecr;
};


struct snx_parport_state {
	union {
		struct snx_pc_parport_state pc;
		void *misc;
	} u;
};


struct snx_parport_ops {
	void 			(*write_data)(struct snx_parport *, unsigned char);
	unsigned char 	(*read_data)(struct snx_parport *);

	void 			(*write_control)(struct snx_parport *, unsigned char);
	unsigned char 	(*read_control)(struct snx_parport *);
	unsigned char 	(*frob_control)(struct snx_parport *, unsigned char mask, unsigned char val);

	unsigned char 	(*read_status)(struct snx_parport *);

	void 			(*enable_irq)(struct snx_parport *);
	void 			(*disable_irq)(struct snx_parport *);

	void 			(*data_forward) (struct snx_parport *);
	void 			(*data_reverse) (struct snx_parport *);

	void 			(*init_state)(struct snx_pardevice *, struct snx_parport_state *);
	void 			(*save_state)(struct snx_parport *, struct snx_parport_state *);
	void 			(*restore_state)(struct snx_parport *, struct snx_parport_state *);

	size_t 			(*epp_write_data) (struct snx_parport *port, const void *buf, size_t len, int flags);
	size_t 			(*epp_read_data) (struct snx_parport *port, void *buf, size_t len, int flags);
	size_t 			(*epp_write_addr) (struct snx_parport *port, const void *buf, size_t len, int flags);
	size_t 			(*epp_read_addr) (struct snx_parport *port, void *buf, size_t len, int flags);

	size_t 			(*ecp_write_data) (struct snx_parport *port, const void *buf, size_t len, int flags);
	size_t 			(*ecp_read_data) (struct snx_parport *port, void *buf, size_t len, int flags);
	size_t 			(*ecp_write_addr) (struct snx_parport *port, const void *buf, size_t len, int flags);

	size_t 			(*compat_write_data) (struct snx_parport *port, const void *buf, size_t len, int flags);
	size_t 			(*nibble_read_data) (struct snx_parport *port, void *buf, size_t len, int flags);
	size_t 			(*byte_read_data) (struct snx_parport *port, void *buf, size_t len, int flags);
	struct module 	*owner;
};


struct snx_parport_device_info {
	parport_device_class 	class;
	const char 				*class_name;
	const char 				*mfr;
	const char 				*model;
	const char 				*cmdset;
	const char 				*description;
};


struct snx_pardevice {
	const char					*name;
	struct snx_parport			*port;
	int							daisy;
	int							(*preempt)(void *);
	void						(*wakeup)(void *);
	void						*private;
	void						(*irq_func)(int, void *, struct pt_regs *);
	unsigned int				flags;
	struct snx_pardevice		*next;
	struct snx_pardevice		*prev;
	struct snx_parport_state	*state;
	wait_queue_head_t			wait_q;
	unsigned long int			time;
	unsigned long int			timeslice;
	volatile long int			timeout;
	unsigned long				waiting;
	struct snx_pardevice		*waitprev;
	struct snx_pardevice		*waitnext;
	void						*sysctl_table;
};


struct snx_parport {
	unsigned long 					base;
	unsigned long 					base_hi;
	unsigned int 					size;
	const char 						*name;
	unsigned int 					modes;
	int 							irq;
	int 							dma;
	int 							muxport;
	int 							portnum;

	struct snx_parport 				*physport;
	struct snx_pardevice 			*devices;
	struct snx_pardevice 			*cad;

	int 							daisy;
	int								muxsel;

	struct snx_pardevice 			*waithead;
	struct snx_pardevice 			*waittail;

	struct list_head 				list;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 15, 0))
	struct timer_list timer;
#endif
	unsigned int 					flags;

	void 							*sysctl_table;
	struct snx_parport_device_info 	probe_info[5];
	struct ieee1284_info 			ieee1284;

	struct snx_parport_ops 			*ops;
	void 							*private_data;

	int 							number;
	spinlock_t 						pardevice_lock;
	spinlock_t 						waitlist_lock;
	rwlock_t 						cad_lock;

	int 							spintime;
	atomic_t 						ref_count;

	struct list_head 				full_list;
	struct snx_parport 				*slaves[3];

	struct snx_parport 				*next;

	dev_t							dev;

	struct semaphore		sem;
};


struct sunix_par_port {
	struct snx_parport 	*port;

	unsigned char 		ctr;
	unsigned char 		ctr_writable;
	int 				ecr;

	int 				fifo_depth;
	int 				pword;

	int 				read_intr_threshold;
	int 				write_intr_threshold;

	char 				*dma_buf;
	dma_addr_t 			dma_handle;
	struct list_head 	list;

	unsigned long 		base;
	unsigned long 		base_hi;
	int 				irq;
	int					portnum;
	unsigned int		snx_type;

	int					board_enum;
	unsigned int		bus_number;
	unsigned int		dev_number;

	PCI_BOARD			pb_info;

	unsigned char		chip_flag;
	unsigned int		port_flag;



};


/*-------------------------------------------------------------------------------

						function and variable extern

-------------------------------------------------------------------------------*/
// snx_devtable.c
extern PCI_BOARD snx_pci_board_conf[];

// snx_ieee1284_ops.c
extern size_t 	sunix_parport_ieee1284_write_compat(struct snx_parport *, const void *, size_t, int);
extern size_t 	sunix_parport_ieee1284_read_nibble(struct snx_parport *, void *, size_t, int);
extern size_t 	sunix_parport_ieee1284_read_byte(struct snx_parport *, void *, size_t, int);
extern size_t 	sunix_parport_ieee1284_ecp_write_data(struct snx_parport *, const void *, size_t, int);
extern size_t 	sunix_parport_ieee1284_ecp_read_data(struct snx_parport *, void *, size_t, int);
extern size_t 	sunix_parport_ieee1284_ecp_write_addr(struct snx_parport *, const void *, size_t, int);
extern size_t 	sunix_parport_ieee1284_epp_write_data(struct snx_parport *, const void *, size_t, int);
extern size_t 	sunix_parport_ieee1284_epp_read_data(struct snx_parport *, void *, size_t, int);
extern size_t 	sunix_parport_ieee1284_epp_write_addr(struct snx_parport *, const void *, size_t, int);
extern size_t 	sunix_parport_ieee1284_epp_read_addr(struct snx_parport *, void *, size_t, int);

// snx_ieee1284.c
extern int 		sunix_parport_wait_event(struct snx_parport *, signed long);
extern int 		sunix_parport_poll_peripheral(struct snx_parport *, unsigned char, unsigned char, int);
extern int 		sunix_parport_wait_peripheral(struct snx_parport *, unsigned char, unsigned char);
extern int 		sunix_parport_negotiate(struct snx_parport *, int);
extern ssize_t 	sunix_parport_write(struct snx_parport *, const void *, size_t);
extern ssize_t 	sunix_parport_read(struct snx_parport *, void *, size_t);
extern long 	sunix_parport_set_timeout(struct snx_pardevice *, long);

// snx_share.c
extern int 		sunix_parport_default_spintime;
extern int 		sunix_parport_register_driver(struct snx_parport_driver *);
extern void 	sunix_parport_unregister_driver(struct snx_parport_driver *);
extern void 	sunix_parport_put_port(struct snx_parport *);
extern void 	sunix_parport_announce_port(struct snx_parport *);
extern void 	sunix_parport_remove_port(struct snx_parport *);
extern void 	sunix_parport_unregister_device(struct snx_pardevice *);
extern int 		sunix_parport_claim(struct snx_pardevice *);
extern int 		sunix_parport_claim_or_block(struct snx_pardevice *);
extern void 	sunix_parport_release(struct snx_pardevice *);
extern struct snx_parport *sunix_parport_get_port(struct snx_parport *);
extern struct snx_parport *sunix_parport_register_port(struct sunix_par_port *,  struct snx_parport_ops *);
extern struct snx_parport *sunix_parport_find_number(int);
extern struct snx_parport *sunix_parport_find_base(unsigned long);
extern struct snx_pardevice *sunix_parport_register_device(
	struct snx_parport 	*port,
	const char 			*name,
	int 				(*pf)(void *),
	void 				(*kf)(void *),
	void 				(*irq_func)(int, void *, struct pt_regs *),
	int 				flags,
	void 				*handle
	);

// snx_parallel.c
extern int 		sunix_par_parport_init(void);
extern void 	sunix_par_parport_exit(void);

// snx_serial.c
extern int      sunix_ser_register_ports(struct snx_ser_driver *);
extern void     sunix_ser_unregister_ports(struct snx_ser_driver *);
extern int      sunix_ser_register_driver(struct snx_ser_driver *);
extern void     sunix_ser_unregister_driver(struct snx_ser_driver *);
extern int 		sunix_ser_interrupt(struct sunix_board *, struct sunix_ser_port *);
extern void     snx_ser_change_speed(struct snx_ser_state *, struct SNXTERMIOS *);

// snx_ppdev.c
extern int 	sunix_par_ppdev_init(void);
extern void sunix_par_ppdev_exit(void);

// snx_lp.c
extern int 	sunix_par_lp_init(void);
extern void sunix_par_lp_exit(void);

// snx_main.c
extern struct sunix_board			sunix_board_table[SNX_BOARDS_MAX];
extern struct sunix_ser_port	    sunix_ser_table[SNX_SER_TOTAL_MAX + 1];
extern struct sunix_par_port	    sunix_par_table[SNX_PAR_TOTAL_MAX];
extern char snx_ser_ic_table[SNX_SER_PORT_MAX_UART][10];
extern char snx_par_ic_table[SNX_PAR_PORT_MAX_UART][10];
extern char snx_port_remap[2][10];

extern int      snx_ser_startup(struct snx_ser_state *, int);
extern void     snx_ser_update_termios(struct snx_ser_state *);

/*-------------------------------------------------------------------------------

							parallel function

-------------------------------------------------------------------------------*/
#define sunix_parport_write_data(p, x)            sunix_parport_pc_write_data(p, x)
#define sunix_parport_read_data(p)               sunix_parport_pc_read_data(p)
#define sunix_parport_write_control(p, x)         sunix_parport_pc_write_control(p, x)
#define sunix_parport_read_control(p)            sunix_parport_pc_read_control(p)
#define sunix_parport_frob_control(p, m, v)        sunix_parport_pc_frob_control(p, m, v)
#define sunix_parport_read_status(p)             sunix_parport_pc_read_status(p)
#define sunix_parport_enable_irq(p)              sunix_parport_pc_enable_irq(p)
#define sunix_parport_disable_irq(p)             sunix_parport_pc_disable_irq(p)
#define sunix_parport_data_forward(p)            sunix_parport_pc_data_forward(p)
#define sunix_parport_data_reverse(p)            sunix_parport_pc_data_reverse(p)


static __inline__ void sunix_parport_pc_write_data(struct snx_parport *p, unsigned char d)
{
	outb(d, SNX_DATA(p));
}


static __inline__ unsigned char sunix_parport_pc_read_data(struct snx_parport *p)
{
	unsigned char val = inb(SNX_DATA (p));
	return val;
}


static __inline__ unsigned char __sunix_parport_pc_frob_control(struct snx_parport *p, unsigned char mask, unsigned char val)
{
	struct sunix_par_port *priv = p->physport->private_data;
	unsigned char ctr = priv->ctr;

	ctr = (ctr & ~mask) ^ val;
	ctr &= priv->ctr_writable;
	outb(ctr, SNX_DCR (p));
	priv->ctr = ctr;
	return ctr;
}


static __inline__ void sunix_parport_pc_data_reverse(struct snx_parport *p)
{
	__sunix_parport_pc_frob_control(p, 0x20, 0x20);
}


static __inline__ void sunix_parport_pc_data_forward(struct snx_parport *p)
{
	__sunix_parport_pc_frob_control(p, 0x20, 0x00);
}


static __inline__ void sunix_parport_pc_write_control (struct snx_parport *p, unsigned char d)
{
	const unsigned char wm = (PARPORT_CONTROL_STROBE | PARPORT_CONTROL_AUTOFD |	PARPORT_CONTROL_INIT | PARPORT_CONTROL_SELECT);

	if (d & 0x20) {
		sunix_parport_pc_data_reverse(p);
	}

	__sunix_parport_pc_frob_control(p, wm, d & wm);
}


static __inline__ unsigned char sunix_parport_pc_read_control(struct snx_parport *p)
{
	const unsigned char rm = (PARPORT_CONTROL_STROBE | PARPORT_CONTROL_AUTOFD |	PARPORT_CONTROL_INIT | PARPORT_CONTROL_SELECT);

	const struct sunix_par_port *priv = p->physport->private_data;
	return priv->ctr & rm;
}


static __inline__ unsigned char sunix_parport_pc_frob_control (struct snx_parport *p, unsigned char mask, unsigned char val)
{
	const unsigned char wm = (PARPORT_CONTROL_STROBE | PARPORT_CONTROL_AUTOFD | PARPORT_CONTROL_INIT | PARPORT_CONTROL_SELECT);

	if (mask & 0x20) {
		if (val & 0x20) {
			sunix_parport_pc_data_reverse(p);
		} else {
			sunix_parport_pc_data_forward(p);
		}
	}

	mask &= wm;
	val &= wm;

	return __sunix_parport_pc_frob_control(p, mask, val);
}


static __inline__ unsigned char sunix_parport_pc_read_status(struct snx_parport *p)
{
	return inb(SNX_DSR(p));
}


static __inline__ void sunix_parport_pc_disable_irq(struct snx_parport *p)
{
	__sunix_parport_pc_frob_control(p, 0x10, 0x00);
}


static __inline__ void sunix_parport_pc_enable_irq(struct snx_parport *p)
{
	__sunix_parport_pc_frob_control(p, 0x10, 0x10);
}


static __inline__ int sunix_parport_yield_blocking(struct snx_pardevice *dev)
{
	unsigned long int timeslip = (jiffies - dev->time);
	if ((dev->port->waithead == NULL) || (timeslip < dev->timeslice)) {
		return 0;
	}

	sunix_parport_release(dev);
	return sunix_parport_claim_or_block(dev);
}

