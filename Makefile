ifeq ($(KERNELRELEASE),)
	KERNELRELEASE = /lib/modules/$(shell uname -r)/build
endif

obj-m += crandom.o

all:
	make -C $(KERNELRELEASE) M=$(PWD) modules

clean:
	make -C $(KERNELRELEASE) M=$(PWD) clean
