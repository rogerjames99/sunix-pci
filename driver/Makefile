# Copyright 2006 - 2019  SUNIX Co., Ltd. all right reserved
# for kernel 2.6
ifeq "$(findstring 2.6, $(shell uname -a))" "2.6"
DRIVERNAME := snx
all:	modules

install: modules
	mkdir -p /lib/modules/$(shell uname -r)/kernel/drivers/char/
	cp -f ./$(DRIVERNAME).ko /lib/modules/$(shell uname -r)/kernel/drivers/char/
	mkdir -p /lib/modules/$(shell uname -r)/misc/
	cp -f ./$(DRIVERNAME).ko /lib/modules/$(shell uname -r)/misc/
	depmod -a


ifneq ($(KERNELRELEASE),)

obj-m += $(DRIVERNAME).o
$(DRIVERNAME)-y := snx_main.o snx_devtable.o \
				   snx_serial.o \
				   snx_parallel.o snx_share.o snx_ieee1284.o snx_ieee1284_ops.o \
				   snx_ppdev.o \
				   snx_lp.o
else
KDIR	:= /lib/modules/$(shell uname -r)/build
PWD	:= $(shell pwd)


modules:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean: clean_local


endif


clean_local:
	rm -f *~
	rm -f *.o
	rm -rf ./.tmp_versions
	rm -f *.mod*
	rm -f *.ko
	rm -f .snx*
	rm -f $(DRIVERNAME)
	rm -f Module*
	rm -f module*
	rm -f /lib/modules/$(shell uname -r)/kernel/drivers/char/snx_golden.ko
	rm -f /lib/modules/$(shell uname -r)/misc/snx_golden.ko
	rm -f /lib/modules/$(shell uname -r)/kernel/drivers/char/snx_matrix.ko
	rm -f /lib/modules/$(shell uname -r)/misc/snx_matrix.ko
	rm -f /lib/modules/$(shell uname -r)/kernel/drivers/char/$(DRIVERNAME).ko
	rm -f /lib/modules/$(shell uname -r)/misc/$(DRIVERNAME).ko
#endif


# for kernel 5.0
else ifeq "$(findstring 5., $(shell uname -a))" "5."
DRIVERNAME := snx
all:	modules
install: modules
	mkdir -p /lib/modules/$(shell uname -r)/kernel/drivers/char/
	cp -f ./$(DRIVERNAME).ko /lib/modules/$(shell uname -r)/kernel/drivers/char/
	mkdir -p /lib/modules/$(shell uname -r)/misc/
	cp -f ./$(DRIVERNAME).ko /lib/modules/$(shell uname -r)/misc/
	depmod -a


ifneq ($(KERNELRELEASE),)

obj-m += $(DRIVERNAME).o
$(DRIVERNAME)-y := snx_main.o snx_devtable.o \
				   snx_serial.o \
				   snx_parallel.o snx_share.o snx_ieee1284.o snx_ieee1284_ops.o \
				   snx_ppdev.o \
				   snx_lp.o
else
KDIR	:= /lib/modules/$(shell uname -r)/build
PWD	:= $(shell pwd)


modules:
	$(MAKE) -C $(KDIR) KBUILD_EXTMOD=$(PWD) modules

clean: clean_local


endif


clean_local:
	rm -f *~
	rm -f *.o
	rm -rf ./.tmp_versions
	rm -f *.mod*
	rm -f *.ko
	rm -f .snx*
	rm -f $(DRIVERNAME)
	rm -f Module*
	rm -f module*
	rm -f /lib/modules/$(shell uname -r)/kernel/drivers/char/snx_golden.ko
	rm -f /lib/modules/$(shell uname -r)/misc/snx_golden.ko
	rm -f /lib/modules/$(shell uname -r)/kernel/drivers/char/snx_matrix.ko
	rm -f /lib/modules/$(shell uname -r)/misc/snx_matrix.ko
	rm -f /lib/modules/$(shell uname -r)/kernel/drivers/char/$(DRIVERNAME).ko
	rm -f /lib/modules/$(shell uname -r)/misc/$(DRIVERNAME).ko
#endif



# for kernel 4.0
else ifeq "$(findstring 4., $(shell uname -a))" "4."
DRIVERNAME := snx
all:	modules
install: modules
	mkdir -p /lib/modules/$(shell uname -r)/kernel/drivers/char/
	cp -f ./$(DRIVERNAME).ko /lib/modules/$(shell uname -r)/kernel/drivers/char/
	mkdir -p /lib/modules/$(shell uname -r)/misc/
	cp -f ./$(DRIVERNAME).ko /lib/modules/$(shell uname -r)/misc/
	depmod -a


ifneq ($(KERNELRELEASE),)

obj-m += $(DRIVERNAME).o
$(DRIVERNAME)-y := snx_main.o snx_devtable.o \
				   snx_serial.o \
				   snx_parallel.o snx_share.o snx_ieee1284.o snx_ieee1284_ops.o \
				   snx_ppdev.o \
				   snx_lp.o
else
KDIR	:= /lib/modules/$(shell uname -r)/build
PWD	:= $(shell pwd)


modules:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean: clean_local


endif


clean_local:
	rm -f *~
	rm -f *.o
	rm -rf ./.tmp_versions
	rm -f *.mod*
	rm -f *.ko
	rm -f .snx*
	rm -f $(DRIVERNAME)
	rm -f Module*
	rm -f module*
	rm -f /lib/modules/$(shell uname -r)/kernel/drivers/char/snx_golden.ko
	rm -f /lib/modules/$(shell uname -r)/misc/snx_golden.ko
	rm -f /lib/modules/$(shell uname -r)/kernel/drivers/char/snx_matrix.ko
	rm -f /lib/modules/$(shell uname -r)/misc/snx_matrix.ko
	rm -f /lib/modules/$(shell uname -r)/kernel/drivers/char/$(DRIVERNAME).ko
	rm -f /lib/modules/$(shell uname -r)/misc/$(DRIVERNAME).ko
#endif

# for kernel 3.0
else ifeq "$(findstring 3., $(shell uname -a))" "3."
DRIVERNAME := snx
all:	modules

install: modules
	mkdir -p /lib/modules/$(shell uname -r)/kernel/drivers/char/
	cp -f ./$(DRIVERNAME).ko /lib/modules/$(shell uname -r)/kernel/drivers/char/
	mkdir -p /lib/modules/$(shell uname -r)/misc/
	cp -f ./$(DRIVERNAME).ko /lib/modules/$(shell uname -r)/misc/
	depmod -a


ifneq ($(KERNELRELEASE),)

obj-m += $(DRIVERNAME).o
$(DRIVERNAME)-y := snx_main.o snx_devtable.o \
				   snx_serial.o \
				   snx_parallel.o snx_share.o snx_ieee1284.o snx_ieee1284_ops.o \
				   snx_ppdev.o \
				   snx_lp.o
else
KDIR	:= /lib/modules/$(shell uname -r)/build
PWD	:= $(shell pwd)


modules:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean: clean_local


endif


clean_local:
	rm -f *~
	rm -f *.o
	rm -rf ./.tmp_versions
	rm -f *.mod*
	rm -f *.ko
	rm -f .snx*
	rm -f $(DRIVERNAME)
	rm -f Module*
	rm -f module*
	rm -f /lib/modules/$(shell uname -r)/kernel/drivers/char/snx_golden.ko
	rm -f /lib/modules/$(shell uname -r)/misc/snx_golden.ko
	rm -f /lib/modules/$(shell uname -r)/kernel/drivers/char/snx_matrix.ko
	rm -f /lib/modules/$(shell uname -r)/misc/snx_matrix.ko
	rm -f /lib/modules/$(shell uname -r)/kernel/drivers/char/$(DRIVERNAME).ko
	rm -f /lib/modules/$(shell uname -r)/misc/$(DRIVERNAME).ko
endif


#for kernel 2.4
ifeq "$(findstring 2.4, $(shell uname -a))" "2.4"
DRIVERNAME :=snx
CC=gcc -c

ARCH=$(shell uname -m | sed s/i.86/i86/)


ifeq	($(ARCH),i86)
PARMS= -DMODULE -D__KERNEL__ -Wall -Wstrict-prototypes -O2 \
-fomit-frame-pointer -I/lib/modules/$(shell uname -r)/build/include
else
PARMS= -DMODULE -D__KERNEL__ -Wall -Wstrict-prototypes -O2 \
-fomit-frame-pointer -pipe -I/lib/modules/$(shell uname -r)/build/include -ffixed-8
endif


ifeq	($(MAKECMDGOALS), install)
PARMS+= -DMODVERSIONS
endif


ifeq	"$(findstring SMP, $(shell uname -a))" "SMP"
PARMS+= -D__SMP__
endif


all:	snx.o

install: snx.o
	mkdir -p /lib/modules/$(shell uname -r)/kernel/drivers/char/
	cp -f ./$(DRIVERNAME).o /lib/modules/$(shell uname -r)/kernel/drivers/char/
	mkdir -p /lib/modules/$(shell uname -r)/misc/
	cp -f ./$(DRIVERNAME).o /lib/modules/$(shell uname -r)/misc/
	depmod -a

snx.o: snx_main.o snx_devtable.o snx_serial.o snx_parallel.o snx_share.o \
	   snx_ieee1284.o snx_ieee1284_ops.o snx_ppdev.o snx_lp.o

	ld -r -o $@ $^



snx_main.o:
	$(CC) $(PARMS) snx_main.c

snx_devtable.o:
	$(CC) $(PARMS) snx_devtable.c

snx_serial.o:
	$(CC) $(PARMS) snx_serial.c

snx_parallel.o:
	$(CC) $(PARMS) snx_parallel.c

snx_share.o:
	$(CC) $(PARMS) snx_share.c

snx_ieee1284.o:
	$(CC) $(PARMS) snx_ieee1284.c

snx_ieee1284_ops.o:
	$(CC) $(PARMS) snx_ieee1284_ops.c

snx_ppdev.o:
	$(CC) $(PARMS) snx_ppdev.c

snx_lp.o:
	$(CC) $(PARMS) snx_lp.c



clean:
	rm -f *~
	rm -f *.o
	rm -rf ./.tmp_versions
	rm -f .snx_*
	rm -f $(DRIVERNAME).mod*
	rm -f $(DRIVERNAME).o
	rm -f $(DRIVERNAME)
	rm -f module*
	rm -f Module*
	rm -f /lib/modules/$(shell uname -r)/kernel/drivers/char/snx_golden.o
	rm -f /lib/modules/$(shell uname -r)/misc/snx_golden.o
	rm -f /lib/modules/$(shell uname -r)/kernel/drivers/char/snx_matrix.o
	rm -f /lib/modules/$(shell uname -r)/misc/snx_matrix.o
	rm -f /lib/modules/$(shell uname -r)/kernel/drivers/char/$(DRIVERNAME).o
	rm -f /lib/modules/$(shell uname -r)/misc/$(DRIVERNAME).o


endif



