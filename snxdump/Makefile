# Copyright 2006 - 2019  SUNIX Co., Ltd. all right reserved
#CC=gcc -fmessage-length=0 -Wall -D_FORTIFY_SOURCE=2 -fstack-protector -O2 -pipe -o
CC=gcc -O2 -pipe -o

all: snxdump

install: snxdump
	cp -p snxdump /usr/bin


snxdump: snxdump.c
	$(CC) snxdump snxdump.c	


clean:  
	rm -f snxdump
	rm -f *~ *.o

