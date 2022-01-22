# Copyright 2006 - 2019  SUNIX Co., Ltd. all right reserved
all: driver_make snxdump_make snxterm_make snxtest_make

install: driver_install snxdump_install snxterm_install snxtest_install

clean: driver_clean snxdump_clean snxterm_clean snxtest_clean snxmknod_clean local_clean


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


snxtest_make:
	cd snxtest;\
	make

snxtest_install:
	cd snxtest;\
	make install

snxtest_clean:
	cd snxtest;\
	make clean;


snxmknod_clean:
	cd snxmknod;\
	rm -f *~


local_clean:
	rm -f *~
