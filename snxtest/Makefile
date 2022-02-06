CC=gcc -O2 -pipe -o

all: snxtest

install: snxtest
	cp -p snxtest /usr/local/bin


snxtest: snxtest.c
	$(CC) snxtest snxtest.c	


clean:  
	rm -f snxtest
	rm -f *~ *.o

