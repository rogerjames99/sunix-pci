/*
 *
 *                SUNIX Multi-I/O Board Device Driver
 *
 *
 *
 *	Driver for SUNIX Multi-I/O Board device driver
 *	Based on drivers/char/serial.c, parport_pc.c, ppdev.c and lp.c
 *	by Linus Torvalds, Theodore Ts'o.
 *
 * 	This program is free software; you can redistribute it and/or modify
 * 	it under the terms of the GNU General Public License as published by
 * 	the Free Software Foundation; either version 2 of the License, or
 * 	(at your option) any later version.
 *
 * 	This program is distributed in the hope that it will be useful,
 * 	but WITHOUT ANY WARRANTY; without even the implied warranty of
 * 	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * 	GNU General Public License for more details.
 *
 * 	You should have received a copy of the GNU General Public License
 * 	along with this program; if not, write to the Free Software
 * 	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *
 *                                                    		Version: 2.0.6.0
 *                                                          Date: 2021/11/26
 */
#include "snx_common.h"
#include "driver_extd.h"


MODULE_AUTHOR(SNX_DRIVER_AUTHOR);
MODULE_DESCRIPTION(SNX_DRIVER_DESC);
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 4, 18))
MODULE_LICENSE("GPL");
#endif

extern struct sunix_board			sunix_board_table[SNX_BOARDS_MAX];
extern struct sunix_ser_port	    sunix_ser_table[SNX_SER_TOTAL_MAX + 1];
extern struct sunix_par_port	    sunix_par_table[SNX_PAR_TOTAL_MAX];

char snx_ser_ic_table[SNX_SER_PORT_MAX_UART][10] = {
	{"UNKNOWN"},
	{"SUN1889"},
	{"SUN1699"},
	{"SUNMATX"},
	{"SUN1999"}
};

char snx_par_ic_table[SNX_PAR_PORT_MAX_UART][10] = {
	{"UNKNOWN"},
	{"SUN1888"},
	{"SUN1689"},
	{"SUNMATX"},
	{"SUN1999"}
};

char snx_port_remap[2][10] = {
	{"NON-REMAP"},
	{"REMAP"}
};

enum{
// golden-serial
	GOLDEN_BOARD_TEST = 0,
	GOLDEN_BOARD_4027A,
	GOLDEN_BOARD_4027D,
	GOLDEN_BOARD_4037A,
	GOLDEN_BOARD_4037D,
	GOLDEN_BOARD_4036A3V,
	GOLDEN_BOARD_4056A,
	GOLDEN_BOARD_4056D,
	GOLDEN_BOARD_4056DW,
	GOLDEN_BOARD_4066A,
	GOLDEN_BOARD_4066R,
	GOLDEN_BOARD_8139,
	GOLDEN_BOARD_8139S,
	GOLDEN_BOARD_8159,
	GOLDEN_BOARD_8159S,
	GOLDEN_BOARD_8169,
	GOLDEN_BOARD_8169S,

// golden-parallel, support by system parport_pc driver
/*
	GOLDEN_BOARD_4008A,
	GOLDEN_BOARD_4018A,
*/

// golden-multi I/O
	GOLDEN_BOARD_4079A,
	GOLDEN_BOARD_4089A,
	GOLDEN_BOARD_4096A,

// matrix-serial
	MATRIX_BOARD_P1002,
	MATRIX_BOARD_P1004,
	MATRIX_BOARD_P1008,
	MATRIX_BOARD_P1016,
	MATRIX_BOARD_P2002,
	MATRIX_BOARD_P2004,
	MATRIX_BOARD_P2008,
	MATRIX_BOARD_P3002,
	MATRIX_BOARD_P3004,
	MATRIX_BOARD_P3008,

// sun1999-serial RS232
	SUN1999_BOARD_5027A,
	SUN1999_BOARD_5037A,
	SUN1999_BOARD_5056A,
	SUN1999_BOARD_5066A,
	SUN1999_BOARD_5016A,

	//sun1999-multi I/O
	SUN1999_BOARD_5069A,
	SUN1999_BOARD_5079A,
	SUN1999_BOARD_5099A,

	//sun1999-parallel

	SUN1999_BOARD_5008A,

	//sun1999-serial RS422/485
	SUN1999_BOARD_P2102,
	SUN1999_BOARD_P2104,
	SUN1999_BOARD_P2108,
	SUN1999_BOARD_P2116,

	//sun1999 3_in_1
	SUN1999_BOARD_P3104,
	SUN1999_BOARD_P3108,

	//cash drawer card
	SUN1999_BOARD_CASH_2S,
	SUN1999_BOARD_CASH_4S,

	//DIO
	SUN1999_BOARD_DIO0802,
	SUN1999_BOARD_DIO1604,
	SUN1999_BOARD_DIO3204,

};


#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
static  struct pci_device_id	sunix_pci_board_id[] = {
// golden-serial
	{VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_TEST, 		0,	0,	GOLDEN_BOARD_TEST},
	{VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4027A, 	0,	0,	GOLDEN_BOARD_4027A},
	{VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4027D, 	0,	0,	GOLDEN_BOARD_4027D},
	{VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4037A, 	0,	0,	GOLDEN_BOARD_4037A},
	{VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4037D, 	0,	0,	GOLDEN_BOARD_4037D},
	{VENID_GOLDEN, 	DEVID_G_SERIAL, SUBVENID_GOLDEN, 	SUBDEVID_4036A3V,	0, 	0,	GOLDEN_BOARD_4036A3V},
	{VENID_GOLDEN, 	DEVID_G_SERIAL, SUBVENID_GOLDEN, 	SUBDEVID_4056A,  	0, 	0,	GOLDEN_BOARD_4056A},
	{VENID_GOLDEN, 	DEVID_G_SERIAL, SUBVENID_GOLDEN, 	SUBDEVID_4056D,  	0, 	0,	GOLDEN_BOARD_4056D},
	{VENID_GOLDEN, 	DEVID_G_SERIAL, SUBVENID_GOLDEN, 	SUBDEVID_4056DW, 	0, 	0,	GOLDEN_BOARD_4056DW},
	{VENID_GOLDEN, 	DEVID_G_SERIAL, SUBVENID_GOLDEN, 	SUBDEVID_4066A,  	0, 	0,	GOLDEN_BOARD_4066A},
	{VENID_GOLDEN, 	DEVID_G_SERIAL, SUBVENID_GOLDEN, 	SUBDEVID_4066R,  	0, 	0,	GOLDEN_BOARD_4066R},
	{VENID_GOLDEN, 	DEVID_G_SERIAL, SUBVENID_GOLDEN, 	SUBDEVID_8139,   	0, 	0,	GOLDEN_BOARD_8139},
	{VENID_GOLDEN,	DEVID_G_SERIAL, SUBVENID_GOLDEN, 	SUBDEVID_8139S,  	0, 	0,	GOLDEN_BOARD_8139S},
	{VENID_GOLDEN, 	DEVID_G_SERIAL, SUBVENID_GOLDEN, 	SUBDEVID_8159,   	0, 	0,	GOLDEN_BOARD_8159},
	{VENID_GOLDEN, 	DEVID_G_SERIAL, SUBVENID_GOLDEN, 	SUBDEVID_8159S,  	0, 	0,	GOLDEN_BOARD_8159S},
	{VENID_GOLDEN, 	DEVID_G_SERIAL, SUBVENID_GOLDEN, 	SUBDEVID_8169,   	0, 	0,	GOLDEN_BOARD_8169},
	{VENID_GOLDEN, 	DEVID_G_SERIAL, SUBVENID_GOLDEN, 	SUBDEVID_8169S,  	0, 	0,	GOLDEN_BOARD_8169S},

// golden-parallel, support by system parport_pc driver
/*
	{VENID_GOLDEN,	DEVID_G_PARALL,	SUBVENID_GOLDEN,	SUBDEVID_4008A, 	0,	0,	GOLDEN_BOARD_4008A},
	{VENID_GOLDEN,	DEVID_G_PARALL,	SUBVENID_GOLDEN,	SUBDEVID_4018A, 	0,	0,	GOLDEN_BOARD_4018A},
*/

// golden-multi I/O
	{VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4079A, 	0,	0,	GOLDEN_BOARD_4079A},
	{VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4089A, 	0,	0,	GOLDEN_BOARD_4089A},
	{VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4096A, 	0,	0,	GOLDEN_BOARD_4096A},

// matrix-serial
	{VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P1002, 	0,	0,	MATRIX_BOARD_P1002},
	{VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P1004, 	0,	0,	MATRIX_BOARD_P1004},
	{VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P1008, 	0,	0,	MATRIX_BOARD_P1008},
	{VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P1016, 	0,	0,	MATRIX_BOARD_P1016},
	{VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P2002, 	0,	0,	MATRIX_BOARD_P2002},
	{VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P2004, 	0,	0,	MATRIX_BOARD_P2004},
	{VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P2008, 	0,	0,	MATRIX_BOARD_P2008},
	{VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P3002, 	0,	0,	MATRIX_BOARD_P3002},
	{VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P3004, 	0,	0,	MATRIX_BOARD_P3004},
	{VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P3008, 	0,	0,	MATRIX_BOARD_P3008},

	// sun1999-serial RS232
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_5027A, 	0,	0,	SUN1999_BOARD_5027A},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_5037A, 	0,	0,	SUN1999_BOARD_5037A},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_5056A, 	0,	0,	SUN1999_BOARD_5056A},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_5066A, 	0,	0,	SUN1999_BOARD_5066A},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_5016A, 	0,	0,	SUN1999_BOARD_5016A},

	// sun1999-multi I/O
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_5069A, 	0,	0,	SUN1999_BOARD_5069A},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_5079A, 	0,	0,	SUN1999_BOARD_5079A},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_5099A, 	0,	0,	SUN1999_BOARD_5099A},

	// sun1999-parallel
	{VENID_SUN1999,	DEVID_S_PARALL,	SUBVENID_SUN1999,	SUBDEVID_5008A, 	0,	0,	SUN1999_BOARD_5008A},

	// sun1999-serial RS422/485
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_P2102, 	0,	0,	SUN1999_BOARD_P2102},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_P2104, 	0,	0,	SUN1999_BOARD_P2104},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_P2108, 	0,	0,	SUN1999_BOARD_P2108},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_P2116, 	0,	0,	SUN1999_BOARD_P2116},

	// sun1999 3_in_1
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_P3104, 	0,	0,	SUN1999_BOARD_P3104},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_P3108, 	0,	0,	SUN1999_BOARD_P3108},

	//cash drawer card  2S
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_CASH_2S, 0,	0,	SUN1999_BOARD_CASH_2S},

	//cash drawer card  4S
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_CASH_4S, 0,	0,	SUN1999_BOARD_CASH_4S},

	//DIO
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_DIO0802, 0,	0,	SUN1999_BOARD_DIO0802},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_DIO1604, 0,	0,	SUN1999_BOARD_DIO1604},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_DIO3204, 0,	0,	SUN1999_BOARD_DIO3204},

	{0}
};
MODULE_DEVICE_TABLE(pci, sunix_pci_board_id);
#else
typedef struct{
	unsigned short vendor;
	unsigned short device;
	unsigned short subvendor;
	unsigned short subdevice;
	unsigned short driver_data;
	unsigned short part_number;
} sunix_pciInfo;

static sunix_pciInfo sunix_pci_board_id[] = {
// golden-serial
	{VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_TEST,		GOLDEN_BOARD_TEST},
	{VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4027A,		GOLDEN_BOARD_4027A},
	{VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4027D,		GOLDEN_BOARD_4027D},
	{VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4037A,		GOLDEN_BOARD_4037A},
	{VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4037D,		GOLDEN_BOARD_4037D},
	{VENID_GOLDEN, 	DEVID_G_SERIAL, SUBVENID_GOLDEN, 	SUBDEVID_4036A3V, GOLDEN_BOARD_4036A3V},
	{VENID_GOLDEN, 	DEVID_G_SERIAL, SUBVENID_GOLDEN, 	SUBDEVID_4056A,  	GOLDEN_BOARD_4056A},
	{VENID_GOLDEN, 	DEVID_G_SERIAL, SUBVENID_GOLDEN, 	SUBDEVID_4056D,  	GOLDEN_BOARD_4056D},
	{VENID_GOLDEN, 	DEVID_G_SERIAL, SUBVENID_GOLDEN, 	SUBDEVID_4056DW, 	GOLDEN_BOARD_4056DW},
	{VENID_GOLDEN,	DEVID_G_SERIAL, SUBVENID_GOLDEN, 	SUBDEVID_4066A,  	GOLDEN_BOARD_4066A},
	{VENID_GOLDEN, 	DEVID_G_SERIAL, SUBVENID_GOLDEN, 	SUBDEVID_4066R,  	GOLDEN_BOARD_4066R},
	{VENID_GOLDEN, 	DEVID_G_SERIAL, SUBVENID_GOLDEN, 	SUBDEVID_8139,   	GOLDEN_BOARD_8139},
	{VENID_GOLDEN, 	DEVID_G_SERIAL, SUBVENID_GOLDEN, 	SUBDEVID_8139S,  	GOLDEN_BOARD_8139S},
	{VENID_GOLDEN, 	DEVID_G_SERIAL, SUBVENID_GOLDEN, 	SUBDEVID_8159,   	GOLDEN_BOARD_8159},
	{VENID_GOLDEN, 	DEVID_G_SERIAL, SUBVENID_GOLDEN, 	SUBDEVID_8159S,  	GOLDEN_BOARD_8159S},
	{VENID_GOLDEN, 	DEVID_G_SERIAL, SUBVENID_GOLDEN, 	SUBDEVID_8169,   	GOLDEN_BOARD_8169},
	{VENID_GOLDEN, 	DEVID_G_SERIAL, SUBVENID_GOLDEN, 	SUBDEVID_8169S,  	GOLDEN_BOARD_8169S},

// golden-parallel, support by system parport_pc driver
/*
	{VENID_GOLDEN,	DEVID_G_PARALL,	SUBVENID_GOLDEN,	SUBDEVID_4008A,		GOLDEN_BOARD_4008A},
	{VENID_GOLDEN,	DEVID_G_PARALL,	SUBVENID_GOLDEN,	SUBDEVID_4018A,		GOLDEN_BOARD_4018A},
*/

// golden-multi I/O
	{VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4079A,		GOLDEN_BOARD_4079A},
	{VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4089A,		GOLDEN_BOARD_4089A},
	{VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4096A,		GOLDEN_BOARD_4096A},

// matrix-serial
	{VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P1002, 	MATRIX_BOARD_P1002},
	{VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P1004, 	MATRIX_BOARD_P1004},
	{VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P1008, 	MATRIX_BOARD_P1008},
	{VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P1016, 	MATRIX_BOARD_P1016},
	{VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P2002, 	MATRIX_BOARD_P2002},
	{VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P2004, 	MATRIX_BOARD_P2004},
	{VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P2008, 	MATRIX_BOARD_P2008},
	{VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P3002, 	MATRIX_BOARD_P3002},
	{VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P3004, 	MATRIX_BOARD_P3004},
	{VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P3008, 	MATRIX_BOARD_P3008},

	// sun1999-serial
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_5027A, 	SUN1999_BOARD_5027A},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_5037A, 	SUN1999_BOARD_5037A},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_5056A, 	SUN1999_BOARD_5056A},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_5066A, 	SUN1999_BOARD_5066A},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_5016A, 	SUN1999_BOARD_5016A},

	// sun1999-multi I/O
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_5069A, 	SUN1999_BOARD_5069A},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_5079A, 	SUN1999_BOARD_5079A},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_5099A, 	SUN1999_BOARD_5099A},

	// sun1999-parallel
	{VENID_SUN1999,	DEVID_S_PARALL,	SUBVENID_SUN1999,	SUBDEVID_5008A, 	SUN1999_BOARD_5008A},

	// sun1999-serial RS422/485
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_P2102, 	SUN1999_BOARD_P2102},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_P2104, 	SUN1999_BOARD_P2104},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_P2108, 	SUN1999_BOARD_P2108},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_P2116, 	SUN1999_BOARD_P2116},

	// sun1999 3_in_1
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_P3104, 	SUN1999_BOARD_P3104},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_P3108, 	SUN1999_BOARD_P3108},

	//cash drawer card
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_CASH_2S, SUN1999_BOARD_CASH_2S},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_CASH_4S, SUN1999_BOARD_CASH_4S},

	//DIO
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_DIO0802, SUN1999_BOARD_DIO0802},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_DIO1604, SUN1999_BOARD_DIO1604},
	{VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_DIO3204, SUN1999_BOARD_DIO3204},

	{0}
};
#endif


struct sunix_board			sunix_board_table[SNX_BOARDS_MAX];
struct sunix_ser_port	    sunix_ser_table[SNX_SER_TOTAL_MAX + 1];
struct sunix_par_port	    sunix_par_table[SNX_PAR_TOTAL_MAX];

static int snx_ser_port_total_cnt;
static int snx_par_port_total_cnt;

int snx_board_count;

static struct snx_ser_driver	sunix_ser_reg = {
	.dev_name = "ttySNX",
	.major = 0,
	.minor = 0,
	.nr = (SNX_SER_TOTAL_MAX + 1),
};


#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 18))
static irqreturn_t sunix_interrupt(int irq, void *dev_id)
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
static irqreturn_t sunix_interrupt(int irq, void *dev_id, struct pt_regs *regs)
#else
static void sunix_interrupt(int irq, void *dev_id, struct pt_regs *regs)
#endif
{
	struct sunix_ser_port *sp = NULL;
	struct sunix_par_port *pp = NULL;
	struct sunix_board *sb = NULL;
	int i;
	int status = 0;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
	int handled = IRQ_NONE;
#endif

	for (i = 0; i < SNX_BOARDS_MAX; i++) {

		if (dev_id == &(sunix_board_table[i])) {
			sb = dev_id;
			break;
		}
	}

	if (i == SNX_BOARDS_MAX)
		status = 1;

	if (!sb)
		status = 1;

	if (sb->board_enum <= 0)
		status = 1;

	if (status != 0) {

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
		return handled;
#else
		return;
#endif
	}

	if ((sb->ser_port > 0) && (sb->ser_isr != NULL)) {
		sp = &sunix_ser_table[sb->ser_port_index];

		if (!sp) {
			status = 1;
		}
		status = sb->ser_isr(sb, sp);
	}

	if ((sb->par_port > 0) && (sb->par_isr != NULL)) {
		pp = &sunix_par_table[sb->par_port_index];

		if (!pp)
			status = 1;

		status = sb->par_isr(sb, pp);
	}

	if (status != 0) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
		return handled;
#else
		return;
#endif
	}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
	handled = IRQ_HANDLED;
	return handled;
#endif
}

static int snx_pci_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	return 0;
}

static int snx_suspend_one(struct pci_dev *pdev, pm_message_t state)
{
	return  0;
}

static int snx_set_port_termios(struct snx_ser_state *state)
{
	struct tty_struct *tty = state->info->tty;
	struct SNXTERMIOS *termios;

	int retval = 0;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 0))
	termios = &tty->termios;
#else
	termios = tty->termios;
#endif

	retval = snx_ser_startup(state, 0);

	if (retval == 0)
		snx_ser_update_termios(state);

	return 0;
}

static int snx_resume_port_termios(struct snx_ser_info *info)
{
	struct snx_ser_state *state = NULL;
	struct tty_struct *tty = info->tty ;

	state = tty->driver_data;
	snx_set_port_termios(state);

	return 0;
}


static int snx_resume_port(struct sunix_ser_port *sp)
{
	struct snx_ser_port *port = &sp->port;
	struct snx_ser_info *info = port->info;

	if (info)
		snx_resume_port_termios(info);

	return 0;
}

static int snx_resume_one(struct pci_dev *pdev)
{
	struct sunix_board *sb = pci_get_drvdata(pdev);
	struct sunix_ser_port *sp = NULL;
	int j;

	if (sb == NULL)
		return 0;

	for (j = 0; j < sb->ser_port; j++) {
		sp = &sunix_ser_table[j];

		if (sp == NULL)
			return 0;

		if (sp->port.suspended == 1)
			snx_resume_port(sp);
	}

	return 0;
}


static int sunix_pci_board_probe(void)
{
	struct sunix_board *sb;
	struct pci_dev *pdev = NULL;
	struct pci_dev *pdev_array[4] = {NULL, NULL, NULL, NULL};

	int sunix_pci_board_id_cnt;
	int tablecnt;
	int boardcnt;
	int i;
	unsigned short int sub_device_id;
	unsigned short int device_part_number;
	unsigned int bar3_base_add;

	int status;
	unsigned int bar3_Byte5;
	unsigned int bar3_Byte6;
	unsigned int bar3_Byte7;
	unsigned int oem_id;
	unsigned char uart_type;
	unsigned char gpio_type;
	unsigned char gpio_card_type;
	int gpio_ch_cnt;

	// clear and init some variable
	memset(sunix_board_table, 0, SNX_BOARDS_MAX * sizeof(struct sunix_board));

	for (i = 0; i < SNX_BOARDS_MAX; i++) {
		sunix_board_table[i].board_enum = -1;
		sunix_board_table[i].board_number = -1;
	}

	sunix_pci_board_id_cnt = (sizeof(sunix_pci_board_id) / sizeof(sunix_pci_board_id[0])) - 1;

	// search golden serial and multi-I/O board
	pdev = NULL;
	tablecnt = 0;
	boardcnt = 0;
	sub_device_id = 0;
	status = 0;

	while (tablecnt < sunix_pci_board_id_cnt) {

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
		pdev = pci_get_device(VENID_GOLDEN, DEVID_G_SERIAL, pdev);
#else
		pdev = pci_find_device(VENID_GOLDEN, DEVID_G_SERIAL, pdev);
#endif

		if (pdev == NULL) {
			tablecnt++;
			continue;
		}

		if ((tablecnt > 0) && ((pdev == pdev_array[0]) ||
		(pdev == pdev_array[1]) ||
		(pdev == pdev_array[2]) ||
		(pdev == pdev_array[3]))) {
			continue;
		}

		pci_read_config_word(pdev, 0x2e, &sub_device_id);

		if (sub_device_id == 0) {
			printk("SNX Error: SUNIX Board (bus:%d device:%d), in configuration space,\n", pdev->bus->number, PCI_SLOT(pdev->devfn));
			printk("           subdevice id isn't vaild.\n\n");
			status = -EIO;
			return status;
		}

		if (sub_device_id != sunix_pci_board_id[tablecnt].subdevice)
			continue;

		if (pdev == NULL) {
			printk("SNX Error: PCI device object is an NULL pointer !\n\n");
			status = -EIO;
			return status;
		} else {

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0))
#elif (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 3))
		pci_disable_device(pdev);
#endif
			status = pci_enable_device(pdev);

			if (status != 0) {
				printk("SNX Error: SUNIX Board Enable Fail !\n\n");
				status = -ENXIO;
				return status;
			}
		}

		if (snx_pci_board_conf[tablecnt].part_number != 0x00) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0))
#elif (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 3))
			pci_disable_device(pdev);
#endif
			continue;
		}

		boardcnt++;
		if (boardcnt > SNX_BOARDS_MAX) {
			printk("\n");
			printk("SNX Error: SUNIX Driver Module Support Four Boards In Maximum !\n\n");
			status = -ENOSPC;
			return status;
		}

		sb = &sunix_board_table[boardcnt-1];
		pdev_array[boardcnt-1] = pdev;
		sb->pdev = pdev;
		sb->bus_number = pdev->bus->number;
		sb->dev_number = PCI_SLOT(pdev->devfn);
		sb->board_enum = (int)sunix_pci_board_id[tablecnt].driver_data;
		sb->pb_info = snx_pci_board_conf[sb->board_enum];
		sb->board_flag = sb->pb_info.board_flag;
		sb->board_number  =  boardcnt - 1;
	}

	// search golden parallel board
	pdev = NULL;
	tablecnt = 0;
	sub_device_id = 0;
	status = 0;

	while (tablecnt < sunix_pci_board_id_cnt) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
		pdev = pci_get_device(VENID_GOLDEN, DEVID_G_PARALL, pdev);
#else
		pdev = pci_find_device(VENID_GOLDEN, DEVID_G_PARALL, pdev);
#endif
		if (pdev == NULL) {
			tablecnt++;
			continue;
		}

		if ((tablecnt > 0) && ((pdev == pdev_array[0]) ||
			(pdev == pdev_array[1]) ||
			(pdev == pdev_array[2]) ||
			(pdev == pdev_array[3]))) {
			continue;
		}

		pci_read_config_word(pdev, 0x2e, &sub_device_id);

		if (sub_device_id == 0) {
			printk("SNX Error: SUNIX Board (bus:%d device:%d), in configuration space,\n", pdev->bus->number, PCI_SLOT(pdev->devfn));
			printk("           subdevice id isn't vaild.\n\n");
			status = -EIO;
			return status;
		}

		if (sub_device_id != sunix_pci_board_id[tablecnt].subdevice) {
			continue;
		}

		if (pdev == NULL) {
			printk("SNX Error: PCI device object is an NULL pointer !\n\n");
			status = -EIO;
			return status;
		} else {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0))
#elif (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 3))
			pci_disable_device(pdev);
#endif
			status = pci_enable_device(pdev);

			if (status != 0) {
				printk("SNX Error: SUNIX Board Enable Fail !\n\n");
				status = -ENXIO;
				return status;
			}
		}

		boardcnt++;
		if (boardcnt > SNX_BOARDS_MAX) {
			printk("\n");
			printk("SNX Error: SUNIX Driver Module Support Four Boards In Maximum !\n\n");
			status = -ENOSPC;
			return status;
		}

		sb = &sunix_board_table[boardcnt-1];
		pdev_array[boardcnt-1] = pdev;
		sb->pdev = pdev;
		sb->bus_number = pdev->bus->number;
		sb->dev_number = PCI_SLOT(pdev->devfn);
		sb->board_enum = (int)sunix_pci_board_id[tablecnt].driver_data;
		sb->pb_info = snx_pci_board_conf[sb->board_enum];
		sb->board_flag = sb->pb_info.board_flag;
		sb->board_number = boardcnt - 1;
	}

	// search matrix serial board
	pdev = NULL;
	tablecnt = 0;
	sub_device_id = 0;
	status = 0;

	while (tablecnt < sunix_pci_board_id_cnt) {

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
	pdev = pci_get_device(VENID_MATRIX, DEVID_M_SERIAL, pdev);
#else
	pdev = pci_find_device(VENID_MATRIX, DEVID_M_SERIAL, pdev);
#endif

		if (pdev == NULL) {
			tablecnt++;
			continue;
		}

		if ((tablecnt > 0) && ((pdev == pdev_array[0]) ||
		(pdev == pdev_array[1]) ||
		(pdev == pdev_array[2]) ||
		(pdev == pdev_array[3]))) {
			continue;
		}

		pci_read_config_word(pdev, 0x2e, &sub_device_id);

		if (sub_device_id == 0) {
			printk("SNX Error: SUNIX Board (bus:%d device:%d), in configuration space,\n", pdev->bus->number, PCI_SLOT(pdev->devfn));
			printk("           subdevice id isn't vaild.\n\n");
			status = -EIO;
			return status;
		}

		if (sub_device_id != sunix_pci_board_id[tablecnt].subdevice)
			continue;
		if (pdev == NULL) {
			printk("SNX Error: PCI device object is an NULL pointer !\n\n");
			status = -EIO;
			return status;
		} else {

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0))

#elif (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 3))
	pci_disable_device(pdev);
#endif

			status = pci_enable_device(pdev);

			if (status != 0) {
				printk("SNX Error: SUNIX Board Enable Fail !\n\n");
				status = -ENXIO;
				return status;
			}
		}

		boardcnt++;
		if (boardcnt > SNX_BOARDS_MAX) {
			printk("\n");
			printk("SNX Error: SUNIX Driver Module Support Four Boards In Maximum !\n\n");
			status = -ENOSPC;
			return status;
		}

		sb = &sunix_board_table[boardcnt-1];
		pdev_array[boardcnt-1] = pdev;
		sb->pdev = pdev;
		sb->bus_number = pdev->bus->number;
		sb->dev_number = PCI_SLOT(pdev->devfn);
		sb->board_enum = (int)sunix_pci_board_id[tablecnt].driver_data;
		sb->pb_info = snx_pci_board_conf[sb->board_enum];
		sb->board_flag = sb->pb_info.board_flag;
		sb->board_number = boardcnt - 1;
    }

	// search sun1999 muti I/O board
	pdev = NULL;
	tablecnt = 0;
	sub_device_id = 0;
	status = 0;
	device_part_number = 0;
	bar3_base_add = 0;
	bar3_Byte5 = 0;
	bar3_Byte6 = 0;
	bar3_Byte7 = 0;
	oem_id = 0;
	uart_type = 0;
	gpio_type = 0;
	gpio_card_type = 0;
	gpio_ch_cnt = 0;

	while (tablecnt < sunix_pci_board_id_cnt) {

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
		pdev = pci_get_device(VENID_SUN1999, DEVID_S_SERIAL, pdev);
#else
		pdev = pci_find_device(VENID_SUN1999, DEVID_S_SERIAL, pdev);
#endif

		if (pdev == NULL) {
			tablecnt++;
			continue;
		}

		if ((tablecnt > 0) &&
		((pdev == pdev_array[0]) ||
		(pdev == pdev_array[1]) ||
		(pdev == pdev_array[2]) ||
		(pdev == pdev_array[3]))) {
			continue;
		}

		pci_read_config_word(pdev, 0x2e, &sub_device_id);

		if (sub_device_id == 0) {
			printk("SNX Error: SUNIX Board (bus:%d device:%d), in configuration space,\n", pdev->bus->number, PCI_SLOT(pdev->devfn));
			printk("           subdevice id isn't vaild.\n\n");
			status = -EIO;
			return status;
		}

		if (sub_device_id != sunix_pci_board_id[tablecnt].subdevice)
		continue;

		if (pdev == NULL) {
			printk("SNX Error: PCI device object is an NULL pointer !\n\n");
			status = -EIO;
			return status;
		} else {

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0))
#elif (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 3))
		pci_disable_device(pdev);
#endif
			status = pci_enable_device(pdev);

			if (status != 0) {
				printk("SNX Error: SUNIX Board Enable Fail !\n\n");
				status = -ENXIO;
				return status;
			}
		}

		bar3_base_add = pci_resource_start(pdev, 3);
		device_part_number = inb(bar3_base_add + 5);
		bar3_Byte5 = device_part_number;
		bar3_Byte6 = inb(bar3_base_add + 0x06);
		bar3_Byte7 = inb(bar3_base_add + 0x07);
		gpio_card_type = ((bar3_Byte7 & 0x18)>>3);
		oem_id = (bar3_Byte5 | (bar3_Byte6 << 8) | (bar3_Byte7 << 16));
		uart_type = ((bar3_Byte5 & 0xc0)>>6);
		gpio_ch_cnt = ((bar3_Byte7 & 0x60)>>5);
		gpio_type = ((bar3_Byte7 & 0x80)>>7);

		if ((gpio_ch_cnt == 0x00) && (gpio_card_type == 0x01)) {
			gpio_ch_cnt = 6 ;
		} else if ((gpio_ch_cnt == 0x00) && (gpio_card_type == 0x02)) {
			gpio_ch_cnt = 8 ;
		} else if (gpio_ch_cnt == 0x01) {
			gpio_ch_cnt = 16 ;
		} else if (gpio_ch_cnt == 0x02) {
			gpio_ch_cnt = 32 ;
		}

		if (device_part_number != snx_pci_board_conf[tablecnt].part_number) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0))
#elif (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 3))
			pci_disable_device(pdev);
#endif
			continue;
		} else if (gpio_card_type != snx_pci_board_conf[tablecnt].card_type) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0))
#elif (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 3))
			pci_disable_device(pdev);
#endif
			continue;
		} else if (gpio_ch_cnt != snx_pci_board_conf[tablecnt].gpio_ch_cnt) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0))
#elif (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 3))
			pci_disable_device(pdev);
#endif
			continue;
		}

		boardcnt++;
		if (boardcnt > SNX_BOARDS_MAX) {
			printk("\n");
			printk("SNX Error: SUNIX Driver Module Support Four Boards In Maximum !\n\n");
			status = -ENOSPC;
			return status;
		}

		sb = &sunix_board_table[boardcnt-1];
		pdev_array[boardcnt-1] = pdev;
		sb->pdev = pdev;
		sb->bus_number = pdev->bus->number;
		sb->dev_number = PCI_SLOT(pdev->devfn);
		sb->board_enum = (int)sunix_pci_board_id[tablecnt].driver_data;
		sb->pb_info = snx_pci_board_conf[sb->board_enum];
		sb->board_flag = sb->pb_info.board_flag;
		sb->board_number = boardcnt - 1;
		sb->oem_id = oem_id ;
		sb->uart_cnt = sb->pb_info.num_serport ;
		sb->gpio_chl_cnt = gpio_ch_cnt ;
		sb->board_uart_type = uart_type ;
		sb->board_gpio_card_type = gpio_card_type ;
		sb->board_gpio_type = gpio_type ;
	}

	// search SUN1999 parallel board
	pdev = NULL;
	tablecnt = 0;
	sub_device_id = 0;
	status = 0;

	while (tablecnt < sunix_pci_board_id_cnt) {

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0))
		pdev = pci_get_device(VENID_SUN1999, DEVID_S_PARALL, pdev);
#else
		pdev = pci_find_device(VENID_SUN1999, DEVID_S_PARALL, pdev);
#endif

		if (pdev == NULL) {
			tablecnt++;
			continue;
		}

		if ((tablecnt > 0) &&
		((pdev == pdev_array[0]) ||
		(pdev == pdev_array[1]) ||
		(pdev == pdev_array[2]) ||
		(pdev == pdev_array[3]))) {
			continue;
		}

		pci_read_config_word(pdev, 0x2e, &sub_device_id);

		if (sub_device_id == 0) {
			printk("SNX Error: SUNIX Board (bus:%d device:%d), in configuration space,\n", pdev->bus->number, PCI_SLOT(pdev->devfn));
			printk("           subdevice id isn't vaild.\n\n");
			status = -EIO;
			return status;
		}

		if (sub_device_id != sunix_pci_board_id[tablecnt].subdevice)
			continue;

		if (pdev == NULL) {
			printk("SNX Error: PCI device object is an NULL pointer !\n\n");
			status = -EIO;
			return status;
		} else {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 9, 0))
#elif (LINUX_VERSION_CODE > KERNEL_VERSION(2, 4, 3))
			pci_disable_device(pdev);
#endif
			status = pci_enable_device(pdev);

			if (status != 0) {
				printk("SNX Error: SUNIX Board Enable Fail !\n\n");
				status = -ENXIO;
				return status;
			}
		}

		boardcnt++;
		if (boardcnt > SNX_BOARDS_MAX) {
			printk("\n");
			printk("SNX Error: SUNIX Driver Module Support Four Boards In Maximum !\n\n");
			status = -ENOSPC;
			return status;
		}

		sb = &sunix_board_table[boardcnt-1];
		pdev_array[boardcnt-1] = pdev;
		sb->pdev = pdev;
		sb->bus_number = pdev->bus->number;
		sb->dev_number = PCI_SLOT(pdev->devfn);
		sb->board_enum = (int)sunix_pci_board_id[tablecnt].driver_data;
		sb->pb_info = snx_pci_board_conf[sb->board_enum];
		sb->board_flag = sb->pb_info.board_flag;
		sb->board_number = boardcnt - 1;
	}

	// print info
	if (boardcnt == 0) {
		printk("SNX Info : No SUNIX Multi-I/O Board Found !\n\n");
		status = -ENXIO;
		return status;
	} else {

		for (i = 0; i < SNX_BOARDS_MAX; i++) {
			sb = &sunix_board_table[i];

			if (sb->board_enum > 0) {
				printk("\n");

				if ((sb->pb_info.num_serport > 0) && (sb->pb_info.num_parport > 0)) {
					printk("SNX Info : Found SUNIX %s Series Board (%dS%dP),\n", sb->pb_info.board_name, sb->pb_info.num_serport, sb->pb_info.num_parport);
				} else if ((sb->pb_info.num_serport) > 0) {
					printk("SNX Info : Found SUNIX %s Series Board (%dS),\n", sb->pb_info.board_name, sb->pb_info.num_serport);
				} else {
					printk("SNX Info : Found SUNIX %s Series Board (%dP),\n", sb->pb_info.board_name, sb->pb_info.num_parport);
				}
				printk("           bus number:%d, device number:%d\n\n", sb->bus_number, sb->dev_number);
			}
		}
		snx_board_count  = boardcnt ;
    }

	return status;
}


static int sunix_get_pci_board_conf(void)
{
    struct sunix_board *sb = NULL;
    struct pci_dev *pdev = NULL;
    int status = 0;
    int i;
    int j;

	for (i = 0; i < SNX_BOARDS_MAX; i++) {
		sb = &sunix_board_table[i];

		if (sb->board_enum > 0) {
			pdev = sb->pdev;
			sb->ports = sb->pb_info.num_serport + sb->pb_info.num_parport;
			sb->ser_port = sb->pb_info.num_serport;
			sb->par_port = sb->pb_info.num_parport;
			snx_ser_port_total_cnt = snx_ser_port_total_cnt + sb->ser_port;
			snx_par_port_total_cnt = snx_par_port_total_cnt + sb->par_port;

			if (snx_ser_port_total_cnt > SNX_SER_TOTAL_MAX) {
				printk("SNX Error: Too much serial port, maximum %d ports can be supported !\n\n", SNX_SER_TOTAL_MAX);
				status = -EIO;
				return status;
			}

			if (snx_par_port_total_cnt > SNX_PAR_SUPPORT_MAX) {
				printk("SNX Error: Too much parallel port, maximum %d ports can be supported !\n\n", SNX_PAR_SUPPORT_MAX);
				status = -EIO;
				return status;
			}

			for (j = 0; j < SNX_PCICFG_BAR_TOTAL; j++) {
				sb->bar_addr[j] = pci_resource_start(pdev, j);
			}

			sb->irq = sb->pdev->irq;

			if (sb->irq <= 0) {
				printk("SNX Error: SUNIX Board %s Series (bus:%d device:%d), in configuartion space, irq isn't valid !\n\n",
				sb->pb_info.board_name,
				sb->bus_number,
				sb->dev_number);

				status = -EIO;
				return status;
			}
		}
	}

	return status;
}


static int sunix_assign_resource(void)
{
	struct sunix_board *sb = NULL;
	struct sunix_ser_port *sp = NULL;
	struct sunix_par_port *pp = NULL;

	int status = 0;
	int i;
	int j;
	int k;
	int ser_n;
	int ser_port_index = 0;

	memset(sunix_ser_table, 0, (SNX_SER_TOTAL_MAX + 1) * sizeof(struct sunix_ser_port));
	memset(sunix_par_table, 0, (SNX_PAR_TOTAL_MAX) * sizeof(struct sunix_par_port));

	for (i = 0; i < SNX_BOARDS_MAX; i++) {
		sb = &sunix_board_table[i];

		if (sb->board_enum > 0) {
			if (sb->ser_port > 0) {
				sb->vector_mask = 0;

				// assign serial port resource
				ser_n = sb->ser_port_index = ser_port_index;

				sp = &sunix_ser_table[ser_n];

				if (sp == NULL) {
					status = -ENXIO;
					printk("SNX Error: Serial port table address error !\n");
					return status;
				}

				for (j = 0; j < sb->ser_port; j++, ser_n++, sp++) {
					sp->port.chip_flag = sb->pb_info.port[j].chip_flag;
					sp->port.iobase = sb->bar_addr[sb->pb_info.port[j].bar1] + sb->pb_info.port[j].offset1;

					if ((sb->board_flag & BOARDFLAG_REMAP) == BOARDFLAG_REMAP) {
						sp->port.vector = 0;
						sb->vector_mask = 0x00;
					} else {

						sp->port.vector = sb->bar_addr[sb->pb_info.intr_vector_bar] + sb->pb_info.intr_vector_offset;
						sb->vector_mask |= (1 << j);
					}
				}

				ser_port_index = ser_port_index + sb->ser_port;
			}


			// assign parallel port resource
			if (sb->par_port > 0) {
				k = 0;

				for (j = 0; j < SNX_PAR_TOTAL_MAX; j++) {
					if ((k + 1) > sb->par_port)
						break;

					if (j >= SNX_PAR_TOTAL_MAX) {
						status = -EACCES;
						printk("SNX Error: Too much parallel port, maximum %d ports can be supported !\n\n", SNX_PAR_TOTAL_MAX);
						return status;
					}

					pp = &sunix_par_table[j];

					if (pp == NULL) {
						status = -ENXIO;
						printk("SNX Error: Parallel port table address error !\n");
						return status;
					}

					if (pp->chip_flag != SUNNONE_HWID) {
						continue;
					} else {
						pp->chip_flag = sb->pb_info.port[k + sb->ser_port].chip_flag;

						pp->base = sb->bar_addr[sb->pb_info.port[k + sb->ser_port].bar1] + sb->pb_info.port[k + sb->ser_port].offset1;
						pp->base_hi = sb->bar_addr[sb->pb_info.port[k + sb->ser_port].bar2] + sb->pb_info.port[k + sb->ser_port].offset2;

						pp->bus_number = sb->bus_number;
						pp->dev_number = sb->dev_number;
						pp->board_enum = sb->board_enum;
						k++;
					}
				}
			}
		}
	}

	return status;
}


static int sunix_ser_port_table_init(void)
{
	struct sunix_board *sb = NULL;
	struct sunix_ser_port *sp = NULL;
	int status = 0;
	int i;
	int j;
	int n;
	int AHDC_State = 0;
	int RS422_State = 0;

	for (i = 0; i < SNX_BOARDS_MAX; i++) {

		sb = &sunix_board_table[i];

		if (sb == NULL) {
			status = -ENXIO;
			printk("SNX Error: Board table pointer error !\n");
			return status;
		}

		if ((sb->board_enum > 0) && (sb->ser_port > 0)) {
			n = sb->ser_port_index;
			sp = &sunix_ser_table[n];

			if (sp == NULL) {
				status = -ENXIO;
				printk("SNX Error: Serial port table pointer error !\n");
				return status;
			}

			for (j = 0; j < sb->ser_port; j++, n++, sp++) {
				if (j < 4) {
					AHDC_State = inb(sb->bar_addr[3]+2) & 0x0F & (0x01 << (((j+1)-1) % 4));
					RS422_State = inb(sb->bar_addr[3]+3) & 0xF0 & (0x10 << (((j+1)-1) % 4));
				} else if (j < 8) {
					AHDC_State = inb(sb->bar_addr[1] + 0x32) & 0x0F & (0x01 << (((j+1)-1) % 4)) ;
					RS422_State = inb(sb->bar_addr[1] + 0x33) & 0xF0 & (0x10 << (((j+1)-1) % 4)) ;
				}

				RS422_State = ((RS422_State & 0xF0) >> 4);
				sp->port.AHDC_State = AHDC_State >> (((j+1)-1)%4);
				sp->port.RS422_State = RS422_State >> (((j+1)-1)%4);

				sp->port.board_enum	= sb->board_enum;
				sp->port.bus_number	= sb->bus_number;
				sp->port.dev_number	= sb->dev_number;
				sp->port.baud_base = 921600;
				sp->port.pb_info = sb->pb_info;
				sp->port.irq = sb->irq;
				sp->port.line = n;
				sp->port.uartclk = sp->port.baud_base * 16;
				sp->port.iotype = SNX_UPIO_PORT;
//                sp->port.flags = ASYNC_SHARE_IRQ;
                sp->port.flags = UPF_SHARE_IRQ;
				sp->port.ldisc_stop_rx = 0;
				spin_lock_init(&sp->port.lock);

				if (sp->port.chip_flag == SUN1889_HWID) {
					sp->port.snx_type = SNX_SER_PORT_SUN1889;
					sp->port.type = PORT_SER_16650V2;
					sp->port.fifosize = SUN1889_FIFOSIZE_SET;
					sp->port.rx_trigger = SUN1889_TRIGGER_LEVEL_SET;
				} else if (sp->port.chip_flag == SUN1699_HWID) {
					sp->port.snx_type = SNX_SER_PORT_SUN1699;
					sp->port.type = PORT_SER_16650V2;
					sp->port.fifosize = SUN1699_FIFOSIZE_SET;
					sp->port.rx_trigger = SUN1699_TRIGGER_LEVEL_SET;
				} else if (sp->port.chip_flag == SUNMATX_HWID) {
					sp->port.snx_type = SNX_SER_PORT_SUNMATX;
					sp->port.type = PORT_SER_16750;
					sp->port.fifosize = SUNMATX_FIFOSIZE_SET;
					sp->port.rx_trigger = SUNMATX_TRIGGER_LEVEL_SET;
				} else if (sp->port.chip_flag == SUN1999_HWID) {
					sp->port.snx_type = SNX_SER_PORT_SUN1999;
					sp->port.type = PORT_SER_16750;
					sp->port.fifosize = SUN1999_FIFOSIZE_SET;
					sp->port.rx_trigger = SUN1999_TRIGGER_LEVEL_SET;
				} else {
					sp->port.snx_type = SNX_SER_PORT_UNKNOWN;
					sp->port.type = PORT_SER_16450;
					sp->port.fifosize = DEFAULT_FIFOSIZE;
					sp->port.rx_trigger = DEFAULT_TRIGGER_LEVEL;
				}


				if ((sb->pb_info.board_flag & BOARDFLAG_REMAP) == BOARDFLAG_REMAP) {
					sp->port.vector_mask = 0;
					sp->port.port_flag = PORTFLAG_REMAP;
				} else {
					sp->port.vector_mask = sb->vector_mask;
					sp->port.port_flag = PORTFLAG_NONE;
				}

				if ((sb->pb_info.board_flag & BOARDFLAG_16PORTS) == BOARDFLAG_16PORTS) {
						sp->port.port_flag |= PORTFLAG_16PORTS;
				}

				sp->port.setserial_flag = SNX_SER_BAUD_NOTSETSER;
			}

			sb->ser_isr = sunix_ser_interrupt;
		} else {
			sb->ser_isr = NULL;
		}
	}


	// release io resource
	for (i = 0; i < SNX_SER_TOTAL_MAX; i++) {
		sp = &sunix_ser_table[i];

		if (sp->port.iobase > 0) {
			release_region(sp->port.iobase, SNX_SER_ADDRESS_LENGTH);
		}
	}
    return status;
}


static int sunix_par_port_table_init(void)
{
	struct sunix_board *sb = NULL;
	struct sunix_par_port *pp = NULL;
    int status = 0;
	int i;
	int j;
	int k;

	for (i = 0; i < SNX_BOARDS_MAX; i++) {
		sb = &sunix_board_table[i];

		if (sb == NULL) {
			status = -ENXIO;
			printk("SNX Error: Board table pointer error !\n");
			return status;
		}

		if ((sb->board_enum > 0) && (sb->par_port > 0)) {
			k = 0;
			for (j = 0; j < SNX_PAR_TOTAL_MAX; j++) {
				pp = &sunix_par_table[j];

				if (pp == NULL) {
					status = -ENXIO;
					printk("SNX Error: Parallel port table pointer error !\n");
					return status;
				}

				if ((k + 1) > sb->par_port) {
					break;
				}


				if ((pp->bus_number == sb->bus_number) &&
					(pp->dev_number == sb->dev_number) &&
					(pp->board_enum	== sb->board_enum)) {

					pp->pb_info = sb->pb_info;
					//pp->irq 				= sb->irq;
					pp->irq	= PARPORT_IRQ_NONE;
					pp->portnum = j;

					if (pp->chip_flag == SUN1888_HWID) {
						pp->snx_type =
						SNX_PAR_PORT_SUN1888;
					} else if (pp->chip_flag ==
						SUN1689_HWID) {
						pp->snx_type =
						SNX_PAR_PORT_SUN1689;
					} else if (pp->chip_flag ==
						SUNMATX_HWID) {
						pp->snx_type =
						SNX_PAR_PORT_SUNMATX;
					} else if (pp->chip_flag ==
						SUN1999_HWID) {
						pp->snx_type =
						SNX_PAR_PORT_SUN1999;
					} else {
						pp->snx_type =
						SNX_PAR_PORT_UNKNOWN;
					}

					if ((sb->pb_info.board_flag &
						BOARDFLAG_REMAP) ==
						BOARDFLAG_REMAP) {
						pp->port_flag = PORTFLAG_REMAP;
					} else {
						pp->port_flag = PORTFLAG_NONE;
					}
					sb->par_isr = NULL;
					k++;
				}
			}
		}
	}


	// release io resource
	for (i = 0; i < SNX_PAR_TOTAL_MAX; i++) {
		pp = &sunix_par_table[i];

		if (pp->base > 0) {
			release_region(pp->base, SNX_PAR_ADDRESS_LENGTH);


			release_region(pp->base, SNX_PAR_STD_ADDR_LENGTH);

			release_region(pp->base +
			SNX_PAR_STD_ADDR_LENGTH,
			SNX_PAR_ETD_ADDR_LENGTH);

			if (pp->base_hi > 0) {
				release_region(pp->base_hi,
				SNX_PAR_ADDRESS_LENGTH);

				release_region(pp->base_hi,
				SNX_PAR_STD_ADDR_LENGTH);

				release_region(pp->base_hi +
				SNX_PAR_STD_ADDR_LENGTH,
				SNX_PAR_ETD_ADDR_LENGTH);
			}
		}
	}

	return status;
}

int sunix_register_irq(void)
{
	struct sunix_board *sb = NULL;
	int status = 0;
	int i;

	for (i = 0; i < SNX_BOARDS_MAX; i++) {
		sb = &sunix_board_table[i];

		if (sb == NULL) {
			status = -ENXIO;
			pr_err("SNX Error: Board table pointer error !\n");
			return status;
		}

			if (sb->board_enum > 0) {
#if (LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 18))
				status = request_irq(sb->irq,
				sunix_interrupt,
				IRQF_SHARED,
				"snx", sb);
#else
					status = request_irq(sb->irq,
					sunix_interrupt,
					SA_SHIRQ,
					"snx", sb);
#endif

			if (status) {
				pr_err("SNX Error: SUNIX Multi-I/O %s Board(bus:%d device:%d), request\n",
				sb->pb_info.board_name,
				sb->bus_number,
				sb->dev_number);
				pr_err("           IRQ %d fail, IRQ %d may be conflit with another device.\n",
				sb->irq, sb->irq);
				return status;
			}
		}
	}

	return status;
}


void sunix_release_irq(void)
{

	struct sunix_board *sb = NULL;
	int i;

	for (i = 0; i < SNX_BOARDS_MAX; i++) {
		sb = &sunix_board_table[i];

		if (sb->board_enum > 0)
			free_irq(sb->irq, sb);
	}
}

static struct pci_driver snx_pci_driver = {
	.name			= "snx",
	.probe			= snx_pci_probe,
	.suspend		= snx_suspend_one,
	.resume			= snx_resume_one,
	.id_table		= sunix_pci_board_id,
};

static int __init snx_init(void)
{
	int status = 0;

	pr_err("\n\n");
	pr_err("=====================  SUNIX Device Driver Module Install  =====================\n");
	pr_err("\n");
	pr_err("SNX Info : Loading SUNIX Multi-I/O Board Driver Module\n");
	pr_err("                                                       -- Date :    %s\n",
	SNX_DRIVER_DATE);
	pr_err("                                                       -- Version : %s\n\n",
	SNX_DRIVER_VERSION);

	snx_ser_port_total_cnt = snx_par_port_total_cnt = 0;

	status = pci_register_driver(&snx_pci_driver);
	if (status != 0)
		goto step7_fail;

	status = sunix_pci_board_probe();
	if (status != 0)
		goto step1_fail;

	status = sunix_get_pci_board_conf();
	if (status != 0)
		goto step1_fail;

	status = sunix_assign_resource();
	if (status != 0)
		goto step1_fail;

	status = sunix_ser_port_table_init();
	if (status != 0)
		goto step1_fail;

	status = sunix_par_port_table_init();
	if (status != 0)
		goto step1_fail;

	status = sunix_register_irq();
	if (status != 0)
		goto step1_fail;

	status = sunix_ser_register_driver(&sunix_ser_reg);
	if (status != 0)
		goto step2_fail;

	status = sunix_ser_register_ports(&sunix_ser_reg);
	if (status != 0)
		goto step3_fail;


	if (snx_par_port_total_cnt > 0) {
		status = sunix_par_parport_init();
		if (status != 0)
			goto step4_fail;

		status = sunix_par_ppdev_init();
		if (status != 0)
			goto step5_fail;

		status = sunix_par_lp_init();
		if (status != 0)
			goto step6_fail;
	}

#if SNX_DBG
	sunix_debug();
#endif


	pr_err("================================================================================\n");
	return status;


	if (snx_par_port_total_cnt > 0) {
step7_fail:

		pci_unregister_driver(&snx_pci_driver);
step6_fail:

		sunix_par_ppdev_exit();


step5_fail:

		sunix_par_parport_exit();


step4_fail:

		sunix_ser_unregister_ports(&sunix_ser_reg);
	}

step3_fail:

	sunix_ser_unregister_driver(&sunix_ser_reg);


step2_fail:

	sunix_release_irq();


step1_fail:

	pr_err("SNX Error: Couldn't Loading SUNIX Multi-I/O Board Driver Module correctly,\n");
	pr_err("           please reboot system and try again. If still can't loading driver,\n");
	pr_err("           contact support.\n\n");
	pr_err("================================================================================\n");
	return status;

}


static void __exit snx_exit(void)
{
	pr_err("\n\n");
	pr_err("====================  SUNIX Device Driver Module Uninstall  ====================\n");
	pr_err("\n");

	if (snx_par_port_total_cnt > 0) {
		sunix_par_lp_exit();

		sunix_par_ppdev_exit();

		sunix_par_parport_exit();
	}

	sunix_ser_unregister_ports(&sunix_ser_reg);

	sunix_ser_unregister_driver(&sunix_ser_reg);

	sunix_release_irq();
	pci_unregister_driver(&snx_pci_driver);
	pr_err("SNX Info : Unload SUNIX Multi-I/O Board Driver Module Done.\n");
	pr_err("================================================================================\n");
}

module_init(snx_init);
module_exit(snx_exit);
