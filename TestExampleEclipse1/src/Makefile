ifneq ($(KERNELRELEASE),)
obj-m := translate.o
	
else 
KSIR :=/lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
	
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
endif