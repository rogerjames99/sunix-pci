// define return value
#define RETURN_ERROR	1
#define RETURN_OK		0


// main menu choice value
#define MAIN_COMMSETUP		1
#define MAIN_DUMBTERM_ASCII	2
#define MAIN_DUMBTERM_HEX	3
#define MAIN_SENDPATTERN	4
#define MAIN_EXITPROGRAM	5

// comm menu choice value
#define RET_BAUDRATE	2
#define RET_PARITY		3
#define RET_DATABIT		4
#define RET_STOPBIT		5
#define RET_FLOWCTL		6

// define Key value
#define KEY_SESC		27
#define KEY_SENTER		10
#define KEY_SDEL		330
#define KEY_SBACKSPACE	263
#define KEY_CTL_D		4
#define KEY_CTL_N		14


// define each WINDOW H and W, start x and y
#define WIN_TERM_H		24
#define WIN_TERM_W		80

#define WIN_MAIN_H 		6
#define WIN_MAIN_W 		32

#define WIN_PORT_H		7
#define WIN_PORT_W		38
#define WIN_PORT_STR_X	7
#define WIN_PORT_STR_Y	12

#define WIN_COMM_H		22
#define WIN_COMM_W		38
#define WIN_COMM_STR_X	30
#define WIN_COMM_STR_Y	1

#define WIN_MESG_H		5
#define WIN_MESG_W		80
#define WIN_MESG_STR_X 	0
#define WIN_MESG_STR_Y 	0

#define WIN_DUMB_H 		19
#define WIN_DUMB_W 		80
#define WIN_DUMB_STR_X	0
#define WIN_DUMB_STR_Y	5

#define WIN_EROR_H		3
#define WIN_EROR_W		36
#define WIN_EROR_STR_X	22
#define WIN_EROR_STR_Y 	19

#define WIN_DAUT_H		7
#define WIN_DAUT_W		44
#define WIN_DAUT_STR_X 	18
#define WIN_DAUT_STR_Y 	8


// define error type 
#define ERRTYPE_OPEN_OTHER		1
#define ERRTYPE_OPEN_BUSY		2
#define ERRTYPE_OPEN_NOFILE		3
#define ERRTYPE_OPEN_NOBASEADDR	4

// define FLAG value
// open_flag
#define OF_FIRSTOPEN		0x0001
#define OF_OPENED			0x0002
#define OF_DEVICECHANGE		0x0004
#define OF_OPENERR			0x0008

// termio_flag
#define TF_FIRSTSET			0x0001
#define TF_CHANGE			0x0002

// mode_flag
#define MF_ASCIIMODE		0x0001
#define MF_HEXMODE			0x0002
#define MF_PATTERNMODE		0x0004


// define receive buffer size
#define RECVBUFFSIZE		80

// define pthread stack size
#ifndef PTHREAD_STACK_MIN
#define PTHREAD_STACK_MIN	16384
#endif

// define each menu text
static char *mainTable[] = 
{ 
	" Serial port setup            ",
	" Dumb terminal (ASCII mode)   ",
	" Dumb terminal (Hex mode)     ",
	" Send pattern                 ",
};
		
		
static char *portSetupTable[] = 
{
	" A - Serial Device :                ",
	" B - Bps/Par/Bits  :                ",    
	" C - Flow Control  :                ",
};
		
static char *baudTable[] = 
{
	"0      ",
	"300    ",
	"1200   ",
	"2400   ",
	"4800   ",
	"9600   ",
	"19200  ",
	"38400  ",
	"57600  ",
	"115200 ",
	"230400 ",
	"460800 ",
	"921600 ",
};
		
static char *pariTable[] = 
{
	"N",
	"E",
	"O",
};
			
static char *dataTable[] = 
{
	"5",
	"6",
	"7",
	"8",
};
		
static char *stopTable[] = 
{
	"1",
	"2",
};

static char *flowTable[] = 
{
	"RTS/CTS  ",
	"Xon/Xoff ",
	"None     ",  
};
		

// device node name for user input		
static char deviceName[15] = "/dev/ttyS0     ";

// device node name for open used
static char fd[15];

#define SNXTERMVERSION "1.0.3.0"


