CONFIG_MODULE_SIG=n

CFLAGS += -g -DDEBUG
ccflags-y += ${CFLAGS}
CC += ${CFLAGS}

obj-m += chardev.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
install:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules_install
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
test:
	#sudo dmesg -C
	sudo insmod chardev.ko
	sudo rmmod chardev.ko
	dmesg | tail
test_probe:
	#sudo dmesg -C
	sudo depmod -a
	sudo modprobe chardev
	sudo modprobe -r chardev
	dmesg | tail
