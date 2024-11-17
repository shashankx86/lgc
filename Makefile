# Makefile for the lgc kernel module
obj-m += lgc.o
lgc-objs := lgc_main.o lgc_config.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules
	gcc -o test_lgc test_lgc.c

clean:
	make -C $(KDIR) M=$(PWD) clean
	rm -f test_lgc
