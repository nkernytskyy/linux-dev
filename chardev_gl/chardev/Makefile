CONFIG_MODULE_SIG=n


CFLAGS += -g -DDEBUG
ccflags-y += ${CFLAGS}
CC += ${CFLAGS}

obj-m += chardev.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
