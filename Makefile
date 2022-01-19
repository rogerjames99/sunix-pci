# Copyright 2006 - 2019  SUNIX Co., Ltd. all right reserved
all: driver_make snxdump_make snxterm_make 

install: driver_install snxdump_install snxterm_install  

clean: driver_clean snxdump_clean snxterm_clean snxmknod_clean local_clean


driver_make:
	cd driver;\
	make

driver_install:
	cd driver;\
	make install

driver_clean:
	cd driver;\
	make clean
	

snxdump_make:
	cd snxdump;\
	make

snxdump_install:
	cd snxdump;\
	make install

snxdump_clean:
	cd snxdump;\
	make clean;



snxterm_make:
	cd snxterm;\
	make

snxterm_install:
	cd snxterm;\
	make install

snxterm_clean:
	cd snxterm;\
	make clean;


snxmknod_clean:
	cd snxmknod;\
	rm -f *~


local_clean:
	rm -f *~
