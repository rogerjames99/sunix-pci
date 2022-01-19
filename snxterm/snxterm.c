/*
 *                                                                                     	
 *                              Linux Dumb Terminal                               
 *                                                                                      
 *           Copyright 2006 - 2011  SUNIX Co., Ltd. all right reserved                                                    
 *                                                                                          
 *                                                              Version: 1.0.3.0           	
 *                                                              Date: 2009/04/06        	
 */
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>

#include "snxterm.h"

static int n_mainMENU    = sizeof(mainTable) / sizeof(mainTable[0]);

static void windowsInit(void);
static void mainFunction(int);
static void serialPortSetup(void);
static void commPamSetup(void);
static void scanDeviceName(void);
static void termioConfSetup(void);
static int  openDevice(void);
static void closeDevice(void);


static int  printMainMenu(int);
static void printMainMenu_L(int);
static void printPortBasic(int, int);
static void printCommPam(void);
static int  printDefaultPattern(void);
static void printErrMsg(int);
static void printMessage(void);


static void *readTerm(void);
static void *sentTerm(void);
static void dumbTerm1(void);
static void dumbTerm2(void);


static void sendPattern(void);

struct handler_thread
{
	pthread_t 	thread;
	int			thr_id;
};

static struct handler_thread *r_thread;
static struct handler_thread *w_thread;


struct sunix_term_conf 
{
	int				fd;				// device node to open
	struct 		termios	t;				// termio config

	int				open_flag;		// open flag, see snxterm.h
	int     	termio_flag;	// termio flag, see snxterm.h
	int     	mode_flag;		// mode flag, see snxterm.h
	
	int				flowctlSelect;	
	int				baudrateSelect;
	int				databitSelect;
	int				paritySelect;
	int				stopbitSelect;
	
	unsigned long 	count;
	int				curX;
	int				curY;
};

static struct sunix_term_conf	term;
pthread_mutex_t term_mutex = PTHREAD_MUTEX_INITIALIZER;


static WINDOW  	*menu_win;	// main menu window
static WINDOW		*port_win;	// serial port setup window														
static WINDOW		*comm_win;	// communication setup window														
static WINDOW  	*mesg_win;	// message window
static WINDOW  	*term_win;	// transmit and receive data window
static WINDOW  	*eror_win;	// error message window
static WINDOW		*daut_win;	// default pattern window


int  main(void)
{
	// clear struct
	memset(&term, 0, sizeof(struct sunix_term_conf));

	// init termios
	term.t.c_cflag     = B9600 | CS8 | CLOCAL | CREAD | CRTSCTS;
	term.t.c_iflag     = IGNPAR;
 	term.t.c_cc[VMIN]  = 1;
 	term.t.c_cc[VTIME] = 0;

	// init each flag
	term.open_flag     = OF_FIRSTOPEN;
	term.termio_flag   = TF_FIRSTSET;
	term.mode_flag     = 0;
	 
	// init each select
	term.flowctlSelect  = 0;
	term.baudrateSelect = 5;
	term.databitSelect  = 3;
	term.paritySelect   = 0;
	term.stopbitSelect  = 0;

	term.count = 0;
	term.curX = 0;
	term.curY = 0;	

  //curses start
	initscr();
	
	noecho();
	cbreak();

	windowsInit();	
	mainFunction(1);	
	//curses end
	endwin();
	closeDevice();	
	return 0;
}

static void windowsInit(void)
{	
	// create each window
	menu_win = newwin(WIN_MAIN_H, WIN_MAIN_W, (WIN_TERM_H - WIN_MAIN_H)/2, (WIN_TERM_W - WIN_MAIN_W)/2);
	port_win = newwin(WIN_PORT_H, WIN_PORT_W, WIN_PORT_STR_Y, WIN_PORT_STR_X);	
	comm_win = newwin(WIN_COMM_H, WIN_COMM_W, WIN_COMM_STR_Y, WIN_COMM_STR_X);
	mesg_win = newwin(WIN_MESG_H, WIN_MESG_W, WIN_MESG_STR_Y, WIN_MESG_STR_X);
	term_win = newwin(WIN_DUMB_H, WIN_DUMB_W, WIN_DUMB_STR_Y, WIN_DUMB_STR_X);	
	eror_win = newwin(WIN_EROR_H, WIN_EROR_W, WIN_EROR_STR_Y, WIN_EROR_STR_X);
	daut_win = newwin(WIN_DAUT_H, WIN_DAUT_W, WIN_DAUT_STR_Y, WIN_DAUT_STR_X);	
		
	// enable some window keypad function	
	keypad(menu_win, TRUE);	
	keypad(port_win, TRUE);
	keypad(comm_win, TRUE);
	keypad(term_win, TRUE);			
	keypad(eror_win, TRUE);
	keypad(daut_win, TRUE);	
}

static void mainFunction(int select)
{
	int mainMenuSlt = select;
	int close = 0;

	while (1)
	{
		// display mesg and term window
		box(mesg_win, 0, 0);
		mvwprintw(mesg_win,2,67,"Ver:%s",SNXTERMVERSION);	
		box(term_win, 0, 0);
		wrefresh(mesg_win);
		wrefresh(term_win);

		// display 'menu' window, and return the select
		mainMenuSlt = printMainMenu(mainMenuSlt);

		switch (mainMenuSlt)
		{	
			// user press "ESC" to exit program
			case MAIN_EXITPROGRAM:
			{
				close = 1;
				break;
			}

			// user select "Serail port setup"
			case MAIN_COMMSETUP:
			{		
				serialPortSetup();
				break;
			}

			// user select "Dumb terminal (ASCII mode)"	
			case MAIN_DUMBTERM_ASCII:
			{
				if (openDevice())
				{	
					// if open device fail, do something...	
				}
				else
				{	
					term.mode_flag = MF_ASCIIMODE;		
					werase(menu_win);
					wrefresh(menu_win);						
					dumbTerm1();
				}
				mainMenuSlt = 2;
				break;	
			}

			
			// user select "Dumb terminal (Hex mode)"
			case MAIN_DUMBTERM_HEX:
			{
				if (openDevice())
				{
					// if open device fail, do something...
				}
				else
				{	
					term.mode_flag = MF_HEXMODE;
					werase(menu_win);
					wrefresh(menu_win);						
					dumbTerm1();
				}
				mainMenuSlt = 3;
				break;	
			}
	
			// user select "Send pattern"										
			case MAIN_SENDPATTERN:
			{
				if (openDevice())
				{
					// if open device fail, do something...
				}
				else
				{	
					term.mode_flag = MF_PATTERNMODE;	
					werase(menu_win);
					wrefresh(menu_win);						
					sendPattern();	
				}				
					
				mainMenuSlt = 4;
				break;
			}	
		}
	
		if (close != 0)
		{
			break;
		}
	}



//	free(r_thread);
//	free(w_thread);
	
	endwin();

	// close device node if opened
	closeDevice();
	
	clear();
}


static void serialPortSetup(void)
{
	int ch;
	int close = 0;
	printPortBasic(0,0);
	
	while (1)
	{
		ch = wgetch(port_win);
		switch (ch)
		{
			// user want to change device node name
			case 'a':
			case 'A':
			{
				scanDeviceName();
				break;
			}

			// user want to change termio setting	
			case 'b':
			case 'B':
			{
				commPamSetup();
				// exit commPamSetup(), display 'menu' and 'port' window
				printMainMenu_L(1);
				break;
			}

			// user want to change flow control
			case 'c':
			case 'C':	
			{
				term.flowctlSelect++;
				if (term.flowctlSelect > 2)
				{
					term.flowctlSelect = 0;
				}	
				term.t.c_cflag &= ~CRTSCTS;
				term.t.c_iflag &= ~(IXON|IXOFF);
				

				if (term.flowctlSelect == 0)
				{
					term.t.c_cflag |=  CRTSCTS;
				}
				else if (term.flowctlSelect == 1)
				{
					term.t.c_iflag |= (IXON|IXOFF);
                    term.t.c_cc[VSTART] = 0x11;
                    term.t.c_cc[VSTOP] = 0x13;
				}

				break;
			}

	
			case KEY_SENTER:
			{
				close = 1;
				break;
			}
		}
				
		printPortBasic(0,0);

		if (close != 0)
		{
			break;
		}
	}	
	
	werase(port_win);
	wrefresh(port_win);
}


static void commPamSetup(void)
{
	int ch;
	int close = 0;

	// display 'comm' window for setup 
	printCommPam();
	
	while (1)
	{
		ch = wgetch(comm_win);
		switch (ch)
		{
			case 'a':
			case 'A':
			{
				term.t.c_cflag &= ~(B300|B1200|B2400|B4800|B9600|B19200|B38400|B57600|B115200|B230400|B460800|B921600);
				term.t.c_cflag |= B300;
				term.baudrateSelect = 1;
				break;
			}
	
			case 'b':
			case 'B':
			{
				term.t.c_cflag &= ~(B300|B1200|B2400|B4800|B9600|B19200|B38400|B57600|B115200|B230400|B460800|B921600);
				term.t.c_cflag |= B1200;
				term.baudrateSelect = 2;
				break;
			}
		
			case 'c':
			case 'C':
			{
				term.t.c_cflag &= ~(B300|B1200|B2400|B4800|B9600|B19200|B38400|B57600|B115200|B230400|B460800|B921600);
				term.t.c_cflag |= B2400;
				term.baudrateSelect = 3;
				break;
			}
	
			case 'd':
			case 'D':
			{
				term.t.c_cflag &= ~(B300|B1200|B2400|B4800|B9600|B19200|B38400|B57600|B115200|B230400|B460800|B921600);
				term.t.c_cflag |= B4800;
				term.baudrateSelect = 4;
				break;
			}
	
			case 'e':
			case 'E':
			{
				term.t.c_cflag &= ~(B300|B1200|B2400|B4800|B9600|B19200|B38400|B57600|B115200|B230400|B460800|B921600);
				term.t.c_cflag |= B9600;
				term.baudrateSelect = 5;
				break;					
			}
	
			case 'f':
			case 'F':
			{
				term.t.c_cflag &= ~(B300|B1200|B2400|B4800|B9600|B19200|B38400|B57600|B115200|B230400|B460800|B921600);
				term.t.c_cflag |= B19200;
				term.baudrateSelect = 6;
				break;				
			}
	
			case 'g':
			case 'G':
			{
				term.t.c_cflag &= ~(B300|B1200|B2400|B4800|B9600|B19200|B38400|B57600|B115200|B230400|B460800|B921600);
				term.t.c_cflag |= B38400;
				term.baudrateSelect = 7;
				break;				
			}
	
			case 'h':
			case 'H':
			{
				term.t.c_cflag &= ~(B300|B1200|B2400|B4800|B9600|B19200|B38400|B57600|B115200|B230400|B460800|B921600);
				term.t.c_cflag |= B57600;
				term.baudrateSelect = 8;
				break;				
			}
	
			case 'i':
			case 'I':
			{
				term.t.c_cflag &= ~(B300|B1200|B2400|B4800|B9600|B19200|B38400|B57600|B115200|B230400|B460800|B921600);
				term.t.c_cflag |= B115200;
				term.baudrateSelect = 9;
				break;				
			}
	
			case 'j':
			case 'J':
			{
				term.t.c_cflag &= ~(B300|B1200|B2400|B4800|B9600|B19200|B38400|B57600|B115200|B230400|B460800|B921600);
				term.t.c_cflag |= B230400;
				term.baudrateSelect = 10;
				break;				
			}
	
			case 'k':
			case 'K':
			{
				term.t.c_cflag &= ~(B300|B1200|B2400|B4800|B9600|B19200|B38400|B57600|B115200|B230400|B460800|B921600);
				term.t.c_cflag |= B460800;
				term.baudrateSelect = 11;
				break;				
			}
	
			case 'l':
			case 'L':
			{
				term.t.c_cflag &= ~(B300|B1200|B2400|B4800|B9600|B19200|B38400|B57600|B115200|B230400|B460800|B921600);
				term.t.c_cflag |= B921600;
				term.baudrateSelect = 12;
				break;				
			}
	
			case 'm':
			case 'M':
			{
				term.t.c_cflag &= ~(PARENB|PARODD);			
				term.paritySelect = 0;
				break;		
			}
	
			case 'n':
			case 'N':
			{
				term.t.c_cflag &= ~(PARENB|PARODD);
				term.t.c_cflag |= PARENB;			
				term.paritySelect = 1;
				break;	
			}
	
			case 'o':
			case 'O':
			{
				term.t.c_cflag &= ~(PARENB|PARODD);
				term.t.c_cflag |= (PARENB|PARODD);			
				term.paritySelect = 2;
				break;									
			}
	
			case 'p':
			case 'P':
			{
				term.t.c_cflag &= ~CSTOPB;			
				term.stopbitSelect = 0;
				break;	
			}
	
			case 'q':
			case 'Q':
			{
				term.t.c_cflag |= CSTOPB;	
				term.stopbitSelect = 1;
				break;					
			}
	
			case 'r':
			case 'R':
			{
				term.t.c_cflag &= ~(CS5|CS6|CS7|CS8);
				term.t.c_cflag |= CS5;
				term.databitSelect = 0;
				break;					
			}

			case 's':
			case 'S':
			{
				term.t.c_cflag &= ~(CS5|CS6|CS7|CS8);
				term.t.c_cflag |= CS6;
				term.databitSelect = 1;
				break;					
			}
	
			case 't':
			case 'T':
			{
				term.t.c_cflag &= ~(CS5|CS6|CS7|CS8);
				term.t.c_cflag |= CS7;
				term.databitSelect = 2;
				break;					
			}
	
			case 'u':
			case 'U':
			{
				term.t.c_cflag &= ~(CS5|CS6|CS7|CS8);
				term.t.c_cflag |= CS8;
				term.databitSelect = 3;
				break;					
			}
											
			case KEY_SENTER:
			{
				close = 1;
				break;
			}	
		}
		
		// display 'comm' window after setup finishing
		printCommPam();
		
		if (close != 0)
		{
			break;
		}
	}	
	
	werase(comm_win);
	wrefresh(comm_win);

	// display 'mesg' and 'term' window
	box(mesg_win, 0, 0);
	mvwprintw(mesg_win,2,67,"Ver:%s",SNXTERMVERSION);	
	box(term_win, 0, 0);
	wrefresh(mesg_win);
	wrefresh(term_win);	
}


static void scanDeviceName(void)
{
	int ch;
	int i;
	int close = 0;
	int curX = 31; // position 31 will be '0' of '/dev/ttyS0'
	int curY = 1;
	
	wmove(port_win, curY, curX);
	
	while (1)
	{
		ch = wgetch(port_win);
		switch (ch)
		{
			case KEY_LEFT:
			{
				curX--;	
				if (curX < 22)
				{
					curX = 36;
				}		
				break;
			}
					
			case KEY_RIGHT:
			{
				curX++;
				if (curX > 36)
				{
					curX = 22;
				}								
				break;	
			}
				
			case KEY_SENTER:
			{
				close = 1;				
				break;	
			}
	
			case KEY_SBACKSPACE:
			{
				getyx(port_win, curY, curX);
					
				if(curX == 22)
				{
					deviceName[14] = ' ';
				}
				else if ((curX - 23) >= 0)
				{									
					deviceName[curX - 23] = ' ';		
				}
	
				curX--;
				if (curX < 22)
				{
					curX = 36;
				}

				break;
			}
				
			case KEY_SDEL:
			{
				getyx(port_win, curY, curX);
				
				for (i = (curX - 22); i < 15; i++)
				{
					if (i >= 14)
					{
						deviceName[i] = ' ';
					}
					else
					{	
						deviceName[i] = deviceName[i+1];
					}
				}
				break;	
			}
			
			default:
			{
				if ((ch >= 0x20) && (ch <= 0x7E))
				{
					getyx(port_win, curY, curX);
					if ((curX - 22) >= 0)
					{
						deviceName[curX - 22] = ch;
						curX++;

						if (curX > 36)
						{
							curX = 22;
						}
					}
				}
			}								
		}
		
		printPortBasic(curX,curY);	
		
		if (close != 0)
		{
			break;
		}
	}		
}


static int openDevice(void)
{
	int i;
	int hasChar = 0;
	bzero(&fd, sizeof(fd));
	
	// copy device node name to fd[]
	for (i = 0; i < 15; i++)
	{
		if ((deviceName[14 - i] == ' ') && (hasChar == 0))
		{
			continue;
		}
		else
		{			
			hasChar = 1;	
			fd[14-i] = deviceName[14-i];
		}
	}

	term.open_flag &= ~OF_OPENERR;


	closeDevice();


	term.fd = open(fd, O_RDWR | O_NOCTTY);

	// if open fail
	if (term.fd < 0)
	{
		if (errno == 16) // device node busy
		{
			printErrMsg(ERRTYPE_OPEN_BUSY);
			term.open_flag |= OF_OPENERR;
			return RETURN_ERROR;				
		}
		else if (errno == 2) // no such file
		{
			printErrMsg(ERRTYPE_OPEN_NOFILE);
			term.open_flag |= OF_OPENERR;
			return RETURN_ERROR;				
		}
		else if (errno == 19) // no such device (no base address)
		{
			printErrMsg(ERRTYPE_OPEN_NOBASEADDR);
			term.open_flag |= OF_OPENERR;
			return RETURN_ERROR;				
		}
		else
		{
			printErrMsg(ERRTYPE_OPEN_OTHER);
			term.open_flag |= OF_OPENERR;
			return RETURN_ERROR;	
		}
	}


	termioConfSetup();

	return RETURN_OK;	
}

static void closeDevice(void)
{
	if(term.fd > 0)
	{
		tcflush(term.fd, TCIOFLUSH);
		close(term.fd);
	}
}


static void termioConfSetup(void)
{
	tcflush(term.fd, TCIOFLUSH);
	tcsetattr(term.fd, TCSANOW, &term.t);	
}


static void *sentTerm(void)
{
	int i;
	int j;	
	int ret;

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	term.count = 0;
	curs_set(0);

	while (1)
	{	
		ret = write(term.fd, "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVW", 40);
		term.count++;
		
		for (i = 0; i < (700 * term.baudrateSelect); i++)
		{
            if (i == 100)
            {
				pthread_mutex_lock(&term_mutex);
				printMessage();
                mvwprintw(mesg_win, 3, 37,"%d",term.count);
                wrefresh(mesg_win); 
				pthread_mutex_unlock(&term_mutex);  
            }
            
			for (j = 0; j < 800; j++)
			{
			}
		}
	}

	return 0 ;
}


static void *readTerm(void)
{
	unsigned char ch[RECVBUFFSIZE];
	int number;
	int i;
	int curX = 1;
	int curY = 1;
	
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	

	box(term_win, 0, 0);
	wmove(term_win, curY, curX);
	wrefresh(term_win);
	
	while(1)
	{
		bzero(ch, sizeof(ch));
		number = read(term.fd, &ch, RECVBUFFSIZE);
		
		if (number > 0)
		{		
			if (term.mode_flag & MF_PATTERNMODE)
			{
				for (i = 0; i < number; i++)
				{
					pthread_mutex_lock(&term_mutex);
					mvwprintw(term_win, curY, curX, "%c", ch[i]);
					pthread_mutex_unlock(&term_mutex);
					if (curX > (WIN_DUMB_W - 3))
					{					
						curX = 1;
						curY++;
				
						if (curY > (WIN_DUMB_H - 2))
						{
							curY = 1;
							//werase(term_win);	
							box(term_win, 0, 0);
							wrefresh(term_win);	
						}
					}
					else
					{
						curX++;
					}	
				}
			}
			else if (term.mode_flag & MF_ASCIIMODE)
			{
				for (i = 0; i < number; i++)
				{
					mvwprintw(term_win, curY, curX, "%c", ch[i]);

					if (curX > (WIN_DUMB_W - 3))
					{					
						curX = 1;
						curY++;
				
						if (curY > (WIN_DUMB_H - 2))
						{
							curY = 1;
							werase(term_win);	
							box(term_win, 0, 0);
							wrefresh(term_win);	
						}
					}
					else
					{
						curX++;
					}					
				}				
			}
			else if (term.mode_flag & MF_HEXMODE)
			{
				for (i = 0; i < number; i++)
				{				
					mvwprintw(term_win, curY, curX, "%2x", ch[i]);

					if (curX > (WIN_DUMB_W - 4))
					{						
						curX = 1;
						curY++;	
			
						if (curY > (WIN_DUMB_H - 2))
						{
							curY = 1;
							werase(term_win);	
							box(term_win, 0, 0);
							wrefresh(term_win);	
						}
					}
					else	
					{
						curX = curX + 2;
					}	
				}
			}
			
			wmove(term_win, curY, curX);
			wrefresh(term_win);	

						
		}
	}
	return 0 ;
}


static void dumbTerm1(void)
{
	pthread_attr_t custom_attr;
	char ch;
	int close = 0;	
	int ret;
	
	printMessage();

	r_thread = (struct handler_thread *)malloc(sizeof(struct handler_thread));	


	pthread_attr_init(&custom_attr);
	pthread_attr_setstacksize(&custom_attr, PTHREAD_STACK_MIN);
	r_thread->thr_id = pthread_create(&r_thread->thread, &custom_attr, (void *)readTerm, (void *)NULL);
	
	while (1)
	{	
		ch = wgetch(term_win);
		switch (ch)
		{	
			case KEY_CTL_N:
			{
				pthread_cancel(r_thread->thread);
				close = 1;
				break;
			}

			default:
			{
				ret = write(term.fd, &ch, 1);
				break;
			}
		}
		
		if (close != 0)
		{
			break;
		}
	}
	
	pthread_join(r_thread->thread, NULL);

	term.mode_flag &= ~(MF_ASCIIMODE | MF_HEXMODE);
	werase(mesg_win);	
	wrefresh(mesg_win);	
	werase(term_win);	
	wrefresh(term_win);
	tcflush(term.fd, TCIOFLUSH);
	free(r_thread);	
}

static void dumbTerm2(void)
{
	pthread_attr_t custom_attr;
	char ch;
	int close = 0;

	printMessage();

	w_thread = (struct handler_thread *)malloc(sizeof(struct handler_thread));
	r_thread = (struct handler_thread *)malloc(sizeof(struct handler_thread));

			
	pthread_attr_init(&custom_attr);
	pthread_attr_setstacksize(&custom_attr, PTHREAD_STACK_MIN);

	r_thread->thr_id = pthread_create(&r_thread->thread, &custom_attr, (void *)readTerm, (void *)NULL);
	w_thread->thr_id = pthread_create(&w_thread->thread, &custom_attr, (void *)sentTerm, (void *)NULL);
		
	
	while (1)
	{	
		ch = wgetch(term_win);
		switch (ch)
		{	
			case KEY_CTL_N:
			{
				pthread_cancel(r_thread->thread);
				pthread_cancel(w_thread->thread);
				close = 1;
				break;
			}
		}
		
		
		if (close != 0)
		{
			break;
		}
	}
	

	pthread_join(r_thread->thread, NULL);
	pthread_join(w_thread->thread, NULL);

	term.mode_flag &= ~(MF_ASCIIMODE | MF_HEXMODE | MF_PATTERNMODE);

	//werase(mesg_win);	
	//wrefresh(mesg_win);	
	werase(term_win);	
	wrefresh(term_win);		
	tcflush(term.fd, TCIOFLUSH);	
	free(w_thread);
	free(r_thread);	
}


static void sendPattern(void)
{
	int close = 0;
	int ret_patn;
	
	printMessage();

	while (1)
	{		
		ret_patn = printDefaultPattern();

		if (ret_patn == 1)
		{
			close = 1;
		}	
		else if (ret_patn == 2)
		{
			term.mode_flag = MF_PATTERNMODE;
			dumbTerm2();
		}
		
		if (close != 0)
		{
			break;
		}	
	}
    werase(mesg_win); 
    wrefresh(mesg_win);       
    
}


/*----------------------------- print function -----------------------------*/


static int printMainMenu(int select)
{
	int highlight = select;
	int choice = 0;
	int ch;

	curs_set(0);	
	printMainMenu_L(highlight);
		
	while (1)
	{	
		ch = wgetch(menu_win);
		switch (ch)
		{	
			case KEY_UP:
			{
				if (highlight == 1)
				{
					highlight = n_mainMENU;
				}
				else
				{
					--highlight;
				}
				break;
			}

	
			case KEY_DOWN:
			{
				if (highlight == n_mainMENU)
				{
					highlight = 1;
				}
				else
				{ 
					++highlight;
				}
				break;
			}

			case KEY_SESC:
			{
				choice = MAIN_EXITPROGRAM;
				break;
			}
	
			case KEY_SENTER:
			{
				choice = highlight;
				break;
			}
		}	

		printMainMenu_L(highlight);
		
		if (choice != 0)
		{
			break;
		}
	}
	
	curs_set(1);	
	return choice;		
}


static void printMainMenu_L(int select)
{
	int y = 1;
	int i;	

	box(menu_win, 0, 0);
	mvwprintw(menu_win, 0,10,"[ main menu ]");

	for (i = 0; i < n_mainMENU; ++i)
	{	
		mvwprintw(menu_win, y, 1, "%s", mainTable[i]);
		++y;
	}

	y = 1;	
	for (i = 0; i < n_mainMENU; ++i)
	{	
		if (select == (i + 1)) 
		{	
			wattron(menu_win, A_REVERSE); 
			mvwprintw(menu_win, y, 1, "%s", mainTable[i]);		
			wattroff(menu_win, A_REVERSE);
		}
		++y;		
	}	
	
	wrefresh(menu_win);	
}


static void printPortBasic(int curX, int curY)
{
	int i;
	int y;

	box(port_win, 0, 0);
	
	y = 1;
	for (i = 0; i < (sizeof(portSetupTable) / sizeof(char *)); i++)
	{	
		mvwprintw(port_win, y, 1, "%s", portSetupTable[i]);
		++y;
	}	
	
	y = 1;
	for (i = 0; i < 3; i++)
	{
		if (i == 0)
		{
			mvwprintw(port_win, y, 22, "%s", deviceName);
		}
		else if (i == 1)
		{	
			mvwprintw(port_win, y, 22, "%s", baudTable[term.baudrateSelect]);
			wprintw(port_win, "%s", dataTable[term.databitSelect]);
			wprintw(port_win, "%s", pariTable[term.paritySelect]);
			wprintw(port_win, "%s", stopTable[term.stopbitSelect]);
		}
		else if (i == 2)
		{
			mvwprintw(port_win, y, 22, "%s", flowTable[term.flowctlSelect]);
		}
			
		y++;		
	}
	
	mvwprintw(port_win, 5, 2,"Select item, or <Enter> to exit? ");
	
	if ((curX != 0) || (curY != 0))
	{
		wmove(port_win, curY, curX);
	}

	wrefresh(port_win);
	
}


static void printCommPam(void)
{
	box(comm_win, 0, 0);
	mvwprintw(comm_win, 0,10,"[ comm param setup ]");
	
	mvwprintw(comm_win, 2, 2," Current: %s%s%s%s"	,baudTable[term.baudrateSelect]
							,dataTable[term.databitSelect]
							,pariTable[term.paritySelect]
							,stopTable[term.stopbitSelect]);
							
	mvwprintw(comm_win, 4, 2," Baud rate    Parity    Data bit   ");
	mvwprintw(comm_win, 6, 2," A: 300       M: None   R: 5       ");
	mvwprintw(comm_win, 7, 2," B: 1200      N: Even   S: 6       ");						
	mvwprintw(comm_win, 8, 2," C: 2400      O: Odd    T: 7       ");
	mvwprintw(comm_win, 9, 2," D: 4800                U: 8       ");
	mvwprintw(comm_win,10, 2," E: 9600                           ");
	mvwprintw(comm_win,11, 2," F: 19200                          ");
	mvwprintw(comm_win,12, 2," G: 38400     Stop bit             ");
	mvwprintw(comm_win,13, 2," H: 57600                          ");
	mvwprintw(comm_win,14, 2," I: 115200    P: 1                 ");
	mvwprintw(comm_win,15, 2," J: 230400    Q: 2                 ");
	mvwprintw(comm_win,16, 2," K: 460800                         ");
	mvwprintw(comm_win,17, 2," L: 921600                         ");
	
	mvwprintw(comm_win,20, 2," Choice, or <Enter> to exit? ");
	wrefresh(comm_win);
}


static void printMessage(void)
{	
	werase(mesg_win);
	box(mesg_win, 0, 0);

	if (term.mode_flag & MF_ASCIIMODE)
	{
		mvwprintw(mesg_win,1,2,"Sending and Receiving chars (ASCII Mode)                   Ctrl-N: main menu");
	}
	else if (term.mode_flag & MF_HEXMODE)
	{
		mvwprintw(mesg_win,1,2,"Sending and Receiving chars (Hex Mode)                     Ctrl-N: main menu");	
	}
	else if (term.mode_flag & MF_PATTERNMODE)
	{
		mvwprintw(mesg_win,1,2,"Sending and Receiving chars (Pattern Mode)                 Ctrl-N: main menu");		
	}	
	
		
	mvwprintw(mesg_win,2, 2,"Device=%s,",fd);
		
	mvwprintw(mesg_win,2,25,"Setting=%s, %s, %s, %s, %s",
							baudTable[term.baudrateSelect],
							dataTable[term.databitSelect],
							pariTable[term.paritySelect],
							stopTable[term.stopbitSelect],
							flowTable[term.flowctlSelect]
							);		
	mvwprintw(mesg_win,2,67,"Ver:%s",SNXTERMVERSION);	

	if (term.mode_flag & MF_PATTERNMODE)
	{
		mvwprintw(mesg_win, 3, 2,"Data length:40, Transmitting count:");		
	}
					
	wrefresh(mesg_win);
}


static void printErrMsg(int errType)
{
	curs_set(0);	
	box(eror_win, 0, 0);

	if (errType == ERRTYPE_OPEN_OTHER)
	{
		mvwprintw(eror_win, 0,13,"[error msg]");
		mvwprintw(eror_win, 1, 2,"%s",fd);
		mvwprintw(eror_win, 1,20,"open fail");
		wrefresh(eror_win);
	}
	else if (errType == ERRTYPE_OPEN_BUSY)
	{
		mvwprintw(eror_win, 0,13,"[error msg]");
		mvwprintw(eror_win, 1, 2,"%s",fd);
		mvwprintw(eror_win, 1,20,"device busy");
		wrefresh(eror_win);
	}
	else if (errType == ERRTYPE_OPEN_NOFILE)
	{
		mvwprintw(eror_win, 0,13,"[error msg]");
		mvwprintw(eror_win, 1, 2,"%s",fd);
		mvwprintw(eror_win, 1,20,"no such file");
		wrefresh(eror_win);
	}
	else if (errType == ERRTYPE_OPEN_NOBASEADDR)
	{
		mvwprintw(eror_win, 0,13,"[error msg]");
		mvwprintw(eror_win, 1, 2,"%s",fd);
		mvwprintw(eror_win, 1,20,"no io address");
		wrefresh(eror_win);
	}	
		
		
	wgetch(eror_win);	
	werase(eror_win);
	wrefresh(eror_win);
	curs_set(1);
}


static int printDefaultPattern(void)
{
	int ch;
	int status = 0;
	
	box(mesg_win, 0, 0);
	mvwprintw(mesg_win,2,67,"Ver:%s",SNXTERMVERSION);
	wrefresh(mesg_win);
	
	box(term_win, 0, 0);
	wrefresh(term_win);
	
	box(daut_win, 0, 0);
	curs_set(0);		 
	
	mvwprintw(daut_win,0, 9,"[ default defined pattern ]");
	mvwprintw(daut_win, 5, 1,"  <Enter> to send         <Esc> to exit  ");	
	wattron(daut_win, A_REVERSE); 
	mvwprintw(daut_win, 2, 2,"0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVW");	
	wattroff(daut_win, A_REVERSE);		
	wrefresh(daut_win);

	while (1)
	{	
		ch = wgetch(daut_win);
		switch (ch)
		{		
			case KEY_SESC:
			{
				status = 1;
				break;
			}
				
			case KEY_SENTER:	
			{
				status = 2;	
				break;
			}
		}

		if (status != 0)
		{
			break;
		}
	}
	
	curs_set(1);
	werase(daut_win);
	wrefresh(daut_win);
	return status;
}











