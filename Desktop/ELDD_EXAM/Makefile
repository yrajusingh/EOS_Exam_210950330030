obj-m := ioctl.o
KERNELDIR = /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	gcc -o operation  operation.c
	

install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -rf operation 
