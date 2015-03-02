obj-m :=hello.o
KDIR :=/lib/modules/3.13.0-43-generic/build
PWD :=$(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) 