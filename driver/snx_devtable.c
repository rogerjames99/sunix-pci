#include "snx_common.h"


PCI_BOARD snx_pci_board_conf[] = {
	// mode none
	{
		// VenID		DevID			SubVenID			SubSysID			SerPort	ParPort	IntrBar	IntrOffset	Name		BoardFlag   part_number
		VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_TEST,		0,		0,		0,		0x00,		"none",		BOARDFLAG_NONE,	PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	'n',	-1,		0,		0,			-1,		0,		0,		0x0000,		SUNNONE_HWID	},
		},
	},

	// mode 4027A
	{
		VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4027A,		1,		0,		0,		0x1C,		"4027",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1889_HWID	},
		},
	},

	// mode 4027D
	{
		VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4027D,		1,		0,		0,		0x00,		"4027",		BOARDFLAG_REMAP, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	2,		0,		8,			-1,		0,		0,		0x0000,		SUN1699_HWID	},
		},
	},

	// mode 4037A, 4037AL
	{
		VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4037A,		2,		0,		0,		0x1C,		"4037",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1889_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1889_HWID	},
		},
	},

	// mode 4037D
	{
		VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4037D,		2,		0,		0,		0x00,		"4037",		BOARDFLAG_REMAP, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	2,		0,		8,			-1,		0,		0,		0x0000,		SUN1699_HWID	},
			{	's',	3,		0,		8,			-1,		0,		0,		0x0000,		SUN1699_HWID	},
		},
	},

	// mode 4036A3V
	{
		VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4036A3V,	2,		0,		0,		0x1C,		"4036",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1889_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1889_HWID	},
		},
	},


	// mode 4056A, 4056P
	{
		VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4056A,		4,		0,		0,		0x1C,		"4056",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1889_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1889_HWID	},
			{	's',	1,		0,		8,			-1,		0,		0,		0x0004,		SUN1699_HWID	},
			{	's',	1,		8,		8,			-1,		0,		0,		0x0008,		SUN1699_HWID	},
		},
	},


	// mode 4056D
	{
		VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4056D,		4,		0,		0,		0x00,		"4056",		BOARDFLAG_REMAP, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	2,		0,		8,			-1,		0,		0,		0x0000,		SUN1699_HWID	},
			{	's',	3,		0,		8,			-1,		0,		0,		0x0000,		SUN1699_HWID	},
			{	's',	4,		0,		8,			-1,		0,		0,		0x0000,		SUN1699_HWID	},
			{	's',	5,		0,		8,			-1,		0,		0,		0x0000,		SUN1699_HWID	},
		},
	},

	// mode 4055WN, 4056WN, 4056DW
	{
		VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4056DW,	4,		0,		0,		0x1C,		"4056",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1889_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1889_HWID	},
			{	's',	1,		0,		8,			-1,		0,		0,		0x0004,		SUN1699_HWID	},
			{	's',	1,		8,		8,			-1,		0,		0,		0x0008,		SUN1699_HWID	},
		},
	},

	// mode 4066A
	{
		VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4066A,		8,		0,		0,		0x1C,		"4066",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1889_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1889_HWID	},
			{	's',	1,		0,		8,			-1,		0,		0,		0x0004,		SUN1699_HWID	},
			{	's',	1,		8,		8,			-1,		0,		0,		0x0008,		SUN1699_HWID	},
			{	's',	2,		0,		8,			-1,		0,		0,		0x0010,		SUN1699_HWID	},
			{	's',	3,		0,		8,			-1,		0,		0,		0x0020,		SUN1699_HWID	},
			{	's',	4,		0,		8,			-1,		0,		0,		0x0040,		SUN1699_HWID	},
			{	's',	5,		0,		8,			-1,		0,		0,		0x0080,		SUN1699_HWID	},
		},
	},

	// mode 4066R
	{
		VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4066R,		8,		0,		0,		0x1C,		"4066",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1889_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1889_HWID	},
			{	's',	1,		0,		8,			-1,		0,		0,		0x0004,		SUN1699_HWID	},
			{	's',	1,		8,		8,			-1,		0,		0,		0x0008,		SUN1699_HWID	},
			{	's',	2,		0,		8,			-1,		0,		0,		0x0010,		SUN1699_HWID	},
			{	's',	3,		0,		8,			-1,		0,		0,		0x0020,		SUN1699_HWID	},
			{	's',	4,		0,		8,			-1,		0,		0,		0x0040,		SUN1699_HWID	},
			{	's',	5,		0,		8,			-1,		0,		0,		0x0080,		SUN1699_HWID	},
		},
	},

	// mode 8139
	{
		VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_8139,		2,		0,		0,		0x1C,		"8139",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1889_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1889_HWID	},
		},
	},

	// mode 8139S, 8139SI
	{
		VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_8139S,		2,		0,		0,		0x1C,		"8139",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1889_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1889_HWID	},
		},
	},

	// mode 8159
	{
		VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_8159,		4,		0,		0,		0x1C,		"8159",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1889_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1889_HWID	},
			{	's',	1,		0,		8,			-1,		0,		0,		0x0004,		SUN1699_HWID	},
			{	's',	1,		8,		8,			-1,		0,		0,		0x0008,		SUN1699_HWID	},
		},
	},

// mode 8159S, 8159SI
	{
		VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_8159S,		4,		0,		0,		0x1C,		"8159",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1889_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1889_HWID	},
			{	's',	1,		0,		8,			-1,		0,		0,		0x0004,		SUN1699_HWID	},
			{	's',	1,		8,		8,			-1,		0,		0,		0x0008,		SUN1699_HWID	},
		},
	},

	// mode 8169
	{
		VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_8169,		8,		0,		0,		0x1C,		"8169",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1889_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1889_HWID	},
			{	's',	1,		0,		8,			-1,		0,		0,		0x0004,		SUN1699_HWID	},
			{	's',	1,		8,		8,			-1,		0,		0,		0x0008,		SUN1699_HWID	},
			{	's',	2,		0,		8,			-1,		0,		0,		0x0010,		SUN1699_HWID	},
			{	's',	3,		0,		8,			-1,		0,		0,		0x0020,		SUN1699_HWID	},
			{	's',	4,		0,		8,			-1,		0,		0,		0x0040,		SUN1699_HWID	},
			{	's',	5,		0,		8,			-1,		0,		0,		0x0080,		SUN1699_HWID	},
		},
	},

	// mode 8169S, 8169SI
	{
		VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_8169S,		8,		0,		0,		0x1C,		"8169",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1889_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1889_HWID	},
			{	's',	1,		0,		8,			-1,		0,		0,		0x0004,		SUN1699_HWID	},
			{	's',	1,		8,		8,			-1,		0,		0,		0x0008,		SUN1699_HWID	},
			{	's',	2,		0,		8,			-1,		0,		0,		0x0010,		SUN1699_HWID	},
			{	's',	3,		0,		8,			-1,		0,		0,		0x0020,		SUN1699_HWID	},
			{	's',	4,		0,		8,			-1,		0,		0,		0x0040,		SUN1699_HWID	},
			{	's',	5,		0,		8,			-1,		0,		0,		0x0080,		SUN1699_HWID	},
		},
	},


	/* support by system parport_pc driver
	// mode 4008A
	{
		VENID_GOLDEN,	DEVID_G_PARALL,	SUBVENID_GOLDEN,	SUBDEVID_4008A,		0,		1,		0,		0x00,		"4008",		BOARDFLAG_NONE, PART_NUMBER_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	'p',	0,		0,		8,			 1,		0,		8,		0x0000,		SUN1888_HWID	},
		},
	},

	// mode 4018A
	{
		VENID_GOLDEN,	DEVID_G_PARALL,	SUBVENID_GOLDEN,	SUBDEVID_4018A,		0,		2,		0,		0x00,		"4018",		BOARDFLAG_NONE, PART_NUMBER_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	'p',	0,		0,		8,			 1,		0,		8,		0x0000,		SUN1888_HWID	},
			{	'p',	2,		0,		8,			 3,		0,		8,		0x0000,		SUN1888_HWID	},
		},
	},
	*/

	// mode 4079A
	{
		VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4079A,		2,		1,		0,		0x1C,		"4079",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
		//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1889_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1889_HWID	},
			{	'p',	2,		0,		8,			 3,		0,		8,		0x0000,		SUN1689_HWID	},
		},
	},

	// mode 4089A
	{
		VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4089A,		2,		2,		0,		0x1C,		"4089",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1889_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1889_HWID	},
			{	'p',	2,		0,		8,			 3,		0,		8,		0x0000,		SUN1689_HWID	},
			{	'p',	4,		0,		8,			 5,		0,		8,		0x0000,		SUN1689_HWID	},
		},
	},

	// mode 4096A
	{
		VENID_GOLDEN,	DEVID_G_SERIAL,	SUBVENID_GOLDEN,	SUBDEVID_4096A,		4,		2,		0,		0x1C,		"4096",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1889_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1889_HWID	},
			{	's',	1,		0,		8,			-1,		0,		0,		0x0004,		SUN1699_HWID	},
			{	's',	1,		8,		8,			-1,		0,		0,		0x0008,		SUN1699_HWID	},
			{	'p',	2,		0,		8,			 3,		0,		8,		0x0000,		SUN1689_HWID	},
			{	'p',	4,		0,		8,			 5,		0,		8,		0x0000,		SUN1689_HWID	},
		},
	},

	// mode P1002
	{
		VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P1002,		2,		0,		1,		0x00,		"1002",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUNMATX_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUNMATX_HWID	},
		},
	},

	// mode P1004
	{
		VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P1004,		4,		0,		1,		0x00,		"1004",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUNMATX_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUNMATX_HWID	},
			{	's',	0,		16,		8,			-1,		0,		0,		0x0004,		SUNMATX_HWID	},
			{	's',	0,		24,		8,			-1,		0,		0,		0x0008,		SUNMATX_HWID	},
		},
	},

	// mode P1008
	{
		VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P1008,		8,		0,		1,		0x00,		"1008",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUNMATX_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUNMATX_HWID	},
			{	's',	0,		16,		8,			-1,		0,		0,		0x0004,		SUNMATX_HWID	},
			{	's',	0,		24,		8,			-1,		0,		0,		0x0008,		SUNMATX_HWID	},
			{	's',	0,		32,		8,			-1,		0,		0,		0x0010,		SUNMATX_HWID	},
			{	's',	0,		40,		8,			-1,		0,		0,		0x0020,		SUNMATX_HWID	},
			{	's',	0,		48,		8,			-1,		0,		0,		0x0040,		SUNMATX_HWID	},
			{	's',	0,		56,		8,			-1,		0,		0,		0x0080,		SUNMATX_HWID	},
		},
	},

	// mode P1016
	{
		VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P1016,		16,		0,		1,		0x00,		"1016",		BOARDFLAG_NONE | BOARDFLAG_16PORTS, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUNMATX_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUNMATX_HWID	},
			{	's',	0,		16,		8,			-1,		0,		0,		0x0004,		SUNMATX_HWID	},
			{	's',	0,		24,		8,			-1,		0,		0,		0x0008,		SUNMATX_HWID	},
			{	's',	0,		32,		8,			-1,		0,		0,		0x0010,		SUNMATX_HWID	},
			{	's',	0,		40,		8,			-1,		0,		0,		0x0020,		SUNMATX_HWID	},
			{	's',	0,		48,		8,			-1,		0,		0,		0x0040,		SUNMATX_HWID	},
			{	's',	0,		56,		8,			-1,		0,		0,		0x0080,		SUNMATX_HWID	},
			{	's',	0,		64,		8,			-1,		0,		0,		0x0100,		SUNMATX_HWID	},
			{	's',	0,		72,		8,			-1,		0,		0,		0x0200,		SUNMATX_HWID	},
			{	's',	0,		80,		8,			-1,		0,		0,		0x0400,		SUNMATX_HWID	},
			{	's',	0,		88,		8,			-1,		0,		0,		0x0800,		SUNMATX_HWID	},
			{	's',	0,		96,		8,			-1,		0,		0,		0x1000,		SUNMATX_HWID	},
			{	's',	0,		104,	8,			-1,		0,		0,		0x2000,		SUNMATX_HWID	},
			{	's',	0,		112,	8,			-1,		0,		0,		0x4000,		SUNMATX_HWID	},
			{	's',	0,		120,	8,			-1,		0,		0,		0x8000,		SUNMATX_HWID	},
		},
	},

	// mode P2002
	{
		VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P2002,		2,		0,		1,		0x00,		"2002",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUNMATX_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUNMATX_HWID	},
		},
	},

	// mode P2004
	{
		VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P2004,		4,		0,		1,		0x00,		"2004",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUNMATX_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUNMATX_HWID	},
			{	's',	0,		16,		8,			-1,		0,		0,		0x0004,		SUNMATX_HWID	},
			{	's',	0,		24,		8,			-1,		0,		0,		0x0008,		SUNMATX_HWID	},
		},
	},

	// mode P2008
	{
		VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P2008,		8,		0,		1,		0x00,		"2008",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUNMATX_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUNMATX_HWID	},
			{	's',	0,		16,		8,			-1,		0,		0,		0x0004,		SUNMATX_HWID	},
			{	's',	0,		24,		8,			-1,		0,		0,		0x0008,		SUNMATX_HWID	},
			{	's',	0,		32,		8,			-1,		0,		0,		0x0010,		SUNMATX_HWID	},
			{	's',	0,		40,		8,			-1,		0,		0,		0x0020,		SUNMATX_HWID	},
			{	's',	0,		48,		8,			-1,		0,		0,		0x0040,		SUNMATX_HWID	},
			{	's',	0,		56,		8,			-1,		0,		0,		0x0080,		SUNMATX_HWID	},
		},
	},

	// mode P3002
	{
		VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P3002,		2,		0,		1,		0x00,		"3002",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUNMATX_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUNMATX_HWID	},
		},
	},

	// mode P3004
	{
		VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P3004,		4,		0,		1,		0x00,		"3004",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUNMATX_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUNMATX_HWID	},
			{	's',	0,		16,		8,			-1,		0,		0,		0x0004,		SUNMATX_HWID	},
			{	's',	0,		24,		8,			-1,		0,		0,		0x0008,		SUNMATX_HWID	},
		},
	},

	// mode P3008
	{
		VENID_MATRIX,	DEVID_M_SERIAL,	SUBVENID_MATRIX,	SUBDEVID_P3008,		8,		0,		1,		0x00,		"3008",		BOARDFLAG_NONE, PART_NUMBER_NONE, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUNMATX_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUNMATX_HWID	},
			{	's',	0,		16,		8,			-1,		0,		0,		0x0004,		SUNMATX_HWID	},
			{	's',	0,		24,		8,			-1,		0,		0,		0x0008,		SUNMATX_HWID	},
			{	's',	0,		32,		8,			-1,		0,		0,		0x0010,		SUNMATX_HWID	},
			{	's',	0,		40,		8,			-1,		0,		0,		0x0020,		SUNMATX_HWID	},
			{	's',	0,		48,		8,			-1,		0,		0,		0x0040,		SUNMATX_HWID	},
			{	's',	0,		56,		8,			-1,		0,		0,		0x0080,		SUNMATX_HWID	},
		},
	},


	// mode 5027A
	{
		VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_5027A,		1,		0,		3,		0x00,		"5027",		BOARDFLAG_NONE, 0x01, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
		//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1999_HWID	},
		},
	},

	// mode 5037A
	{
		VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_5037A,		2,		0,		3,		0x00,		"5037",		BOARDFLAG_NONE, 0x02, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1999_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1999_HWID	},
		},
	},

	// mode 5056A
	{
		VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_5056A,		4,		0,		3,		0x00,		"5056",		BOARDFLAG_NONE, 0x04, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
		//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1999_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1999_HWID	},
			{	's',	0,		16,		8,			-1,		0,		0,		0x0004,		SUN1999_HWID	},
			{	's',	0,		24,		8,			-1,		0,		0,		0x0008,		SUN1999_HWID	},
		},
	},

	// mode 5066A
	{
		VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_5066A,		8,		0,		3,		0x00,		"5066",		BOARDFLAG_NONE, 0x08, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
		//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1999_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1999_HWID	},
			{	's',	0,		16,		8,			-1,		0,		0,		0x0004,		SUN1999_HWID	},
			{	's',	0,		24,		8,			-1,		0,		0,		0x0008,		SUN1999_HWID	},
			{	's',	1,		0,		8,			-1,		0,		0,		0x0001,		SUN1999_HWID	},
			{	's',	1,		8,		8,			-1,		0,		0,		0x0002,		SUN1999_HWID	},
			{	's',	1,		16,		8,			-1,		0,		0,		0x0004,		SUN1999_HWID	},
			{	's',	1,		24,		8,			-1,		0,		0,		0x0008,		SUN1999_HWID	},
		},
	},

	// mode 5016
	{
		VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_5016A,		16,		0,		3,		0x00,		"5016",		BOARDFLAG_NONE | BOARDFLAG_16PORTS, 0x10, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1999_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1999_HWID	},
			{	's',	0,		16,		8,			-1,		0,		0,		0x0004,		SUN1999_HWID	},
			{	's',	0,		24,		8,			-1,		0,		0,		0x0008,		SUN1999_HWID	},
			{	's',	1,		0,		8,			-1,		0,		0,		0x0001,		SUN1999_HWID	},
			{	's',	1,		8,		8,			-1,		0,		0,		0x0002,		SUN1999_HWID	},
			{	's',	1,		16,		8,			-1,		0,		0,		0x0004,		SUN1999_HWID	},
			{	's',	1,		24,		8,			-1,		0,		0,		0x0008,		SUN1999_HWID	},
			{	's',	1,		64,		8,			-1,		0,		0,		0x0010,		SUN1999_HWID	},
			{	's',	1,		72,		8,			-1,		0,		0,		0x0020,		SUN1999_HWID	},
			{	's',	1,		80,		8,			-1,		0,		0,		0x0040,		SUN1999_HWID	},
			{	's',	1,		88,		8,			-1,		0,		0,		0x0080,		SUN1999_HWID	},
			{	's',	1,		128,		8,			-1,		0,		0,		0x0100,		SUN1999_HWID	},
			{	's',	1,		136,		8,			-1,		0,		0,		0x0200,		SUN1999_HWID	},
			{	's',	1,		144,		8,			-1,		0,		0,		0x0400,		SUN1999_HWID	},
			{	's',	1,		152,		8,			-1,		0,		0,		0x0800,		SUN1999_HWID	},
		},
	},

	// mode 5069A 5069H
	{
		VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_5069A,		1,		1,		3,		0x00,		"5069",		BOARDFLAG_NONE, 0x01, 0x00, GPIO_NONE,
		{
		//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1999_HWID	},
			{	'p',	1,		0,		8,			 2,		0,		0,		0x0000,		SUN1999_HWID	},
		},
	},


	// mode 5079A
	{
		VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_5079A,		2,		1,		3,		0x00,		"5079",		BOARDFLAG_NONE, 0x02, 0x00, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1999_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1999_HWID	},
			{	'p',	1,		0,		8,			 2,		0,		0,		0x0000,		SUN1999_HWID	},
		},
	},

	// mode 5099A
	{
		VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_5099A,		4,		1,		3,		0x00,		"5099",		BOARDFLAG_NONE, 0x04, 0x00, GPIO_NONE,
		{
		//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1999_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1999_HWID	},
			{	's',	0,		16,		8,			-1,		0,		0,		0x0004,		SUN1999_HWID	},
			{	's',	0,		24,		8,			-1,		0,		0,		0x0008,		SUN1999_HWID	},
			{	'p',	1,		0,		8,			 2,		0,		0,		0x0000,		SUN1999_HWID	},
		},
	},

	// mode 5008A
	{
		VENID_SUN1999,	DEVID_S_PARALL,	SUBVENID_SUN1999,	SUBDEVID_5008A,		0,		1,		0,		0x00,		"5008",		BOARDFLAG_NONE, 0x00, 0x00, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags

			{	'p',	1,		0,		8,			 2,		0,		0,		0x0000,		SUN1999_HWID	},
		},
	},

	// mode P2102
	{
		VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_P2102,		2,		0,		3,		0x00,		"P2102",		BOARDFLAG_NONE, 0x42, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1999_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1999_HWID	},
		},
	},

	// mode P2104
	{
		VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_P2104,		4,		0,		3,		0x00,		"P2104",		BOARDFLAG_NONE, 0x44, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
		//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1999_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1999_HWID	},
			{	's',	0,		16,		8,			-1,		0,		0,		0x0004,		SUN1999_HWID	},
			{	's',	0,		24,		8,			-1,		0,		0,		0x0008,		SUN1999_HWID	},
		},
	},

	// mode P2108
	{
		VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_P2108,		8,		0,		3,		0x00,		"P2108",		BOARDFLAG_NONE, 0x48, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
		//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1999_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1999_HWID	},
			{	's',	0,		16,		8,			-1,		0,		0,		0x0004,		SUN1999_HWID	},
			{	's',	0,		24,		8,			-1,		0,		0,		0x0008,		SUN1999_HWID	},
			{	's',	1,		0,		8,			-1,		0,		0,		0x0001,		SUN1999_HWID	},
			{	's',	1,		8,		8,			-1,		0,		0,		0x0002,		SUN1999_HWID	},
			{	's',	1,		16,		8,			-1,		0,		0,		0x0004,		SUN1999_HWID	},
			{	's',	1,		24,		8,			-1,		0,		0,		0x0008,		SUN1999_HWID	},
		},
	},

	// mode P2116
	{
		VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_P2116,		16,		0,		3,		0x00,		"P2116",		BOARDFLAG_NONE | BOARDFLAG_16PORTS, 0x50, CARD_TYPE_UART_ONLY, GPIO_NONE,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1999_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1999_HWID	},
			{	's',	0,		16,		8,			-1,		0,		0,		0x0004,		SUN1999_HWID	},
			{	's',	0,		24,		8,			-1,		0,		0,		0x0008,		SUN1999_HWID	},
			{	's',	1,		0,		8,			-1,		0,		0,		0x0001,		SUN1999_HWID	},
			{	's',	1,		8,		8,			-1,		0,		0,		0x0002,		SUN1999_HWID	},
			{	's',	1,		16,		8,			-1,		0,		0,		0x0004,		SUN1999_HWID	},
			{	's',	1,		24,		8,			-1,		0,		0,		0x0008,		SUN1999_HWID	},
			{	's',	1,		64,		8,			-1,		0,		0,		0x0010,		SUN1999_HWID	},
			{	's',	1,		72,		8,			-1,		0,		0,		0x0020,		SUN1999_HWID	},
			{	's',	1,		80,		8,			-1,		0,		0,		0x0040,		SUN1999_HWID	},
			{	's',	1,		88,		8,			-1,		0,		0,		0x0080,		SUN1999_HWID	},
			{	's',	1,		128,		8,			-1,		0,		0,		0x0100,		SUN1999_HWID	},
			{	's',	1,		136,		8,			-1,		0,		0,		0x0200,		SUN1999_HWID	},
			{	's',	1,		144,		8,			-1,		0,		0,		0x0400,		SUN1999_HWID	},
			{	's',	1,		152,		8,			-1,		0,		0,		0x0800,		SUN1999_HWID	},
		},
	},

	// mode IPC-P3104
	{
		VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_P3104,		4,		0,		3,		0x00,		"P3104",		BOARDFLAG_NONE, 0x84, 0x00, GPIO_NONE,
		{
		//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1999_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1999_HWID	},
			{	's',	0,		16,		8,			-1,		0,		0,		0x0004,		SUN1999_HWID	},
			{	's',	0,		24,		8,			-1,		0,		0,		0x0008,		SUN1999_HWID	},
		},
	},

	// mode IPC-P3108
	{
		VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_P3108,		8,		0,		3,		0x00,		"P3108",		BOARDFLAG_NONE, 0x88, 0x00, GPIO_NONE,
		{
		//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1999_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1999_HWID	},
			{	's',	0,		16,		8,			-1,		0,		0,		0x0004,		SUN1999_HWID	},
			{	's',	0,		24,		8,			-1,		0,		0,		0x0008,		SUN1999_HWID	},
			{	's',	1,		0,		8,			-1,		0,		0,		0x0001,		SUN1999_HWID	},
			{	's',	1,		8,		8,			-1,		0,		0,		0x0002,		SUN1999_HWID	},
			{	's',	1,		16,		8,			-1,		0,		0,		0x0004,		SUN1999_HWID	},
			{	's',	1,		24,		8,			-1,		0,		0,		0x0008,		SUN1999_HWID	},
		},
	},

	// CDK1037
	{
		VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_CASH_2S,		2,		0,		3,		0x00,		"CDK1037",		BOARDFLAG_NONE, 0x02, CARD_TYPE_UART_GINTR, INTR_GPIO_6PORT,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1999_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1999_HWID	},
		},
	},

	// CDK1056
	{
		VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_CASH_4S,		4,		0,		3,		0x00,		"CDK1056",		BOARDFLAG_NONE, 0x04, CARD_TYPE_UART_GINTR, INTR_GPIO_6PORT,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1999_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1999_HWID	},
			{	's',	0,		16,		8,			-1,		0,		0,		0x0004,		SUN1999_HWID	},
			{	's',	0,		24,		8,			-1,		0,		0,		0x0008,		SUN1999_HWID	},
		},
	},

	// mode DIO-0802
	{
		VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_DIO0802,		2,		0,		3,		0x00,		"DIO0802",		BOARDFLAG_NONE, 0x02, CARD_TYPE_UART_GEXTR, EXTR_GPIO_8PORT,
		{
			//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1999_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1999_HWID	},
		},
	},

// mode DIO-1604
	{
		VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_DIO1604,		4,		0,		3,		0x00,		"DIO1604",		BOARDFLAG_NONE, 0x04, CARD_TYPE_UART_GEXTR, EXTR_GPIO_16PORT,
		{
		//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1999_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1999_HWID	},
			{	's',	0,		16,		8,			-1,		0,		0,		0x0004,		SUN1999_HWID	},
			{	's',	0,		24,		8,			-1,		0,		0,		0x0008,		SUN1999_HWID	},
		},
	},

	// mode DIO-3204
	{
		VENID_SUN1999,	DEVID_S_SERIAL,	SUBVENID_SUN1999,	SUBDEVID_DIO3204,		4,		0,		3,		0x00,		"DIO3204",		BOARDFLAG_NONE, 0x04, CARD_TYPE_UART_GEXTR, EXTR_GPIO_32PORT,
		{
		//	type	bar1	ofs1	len1		bar2	ofs2	len2	intmask		flags
			{	's',	0,		0,		8,			-1,		0,		0,		0x0001,		SUN1999_HWID	},
			{	's',	0,		8,		8,			-1,		0,		0,		0x0002,		SUN1999_HWID	},
			{	's',	0,		16,		8,			-1,		0,		0,		0x0004,		SUN1999_HWID	},
			{	's',	0,		24,		8,			-1,		0,		0,		0x0008,		SUN1999_HWID	},
		},
	},
};

