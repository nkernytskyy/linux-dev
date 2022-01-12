ccflags-y := \
	-std=gnu99 \
	-Wno-declaration-after-statement \
	-Wno-unused-result \
	-Wno-unused-function

obj-m += src/ioregion.o

KERNEL_SRC = /lib/modules/$(shell uname -r)/build

all:
	make -C $(KERNEL_SRC) M=$(PWD) modules

# install:
# 	make -C $(KERNEL_SRC) M=$(PWD) modules_install

clean:
	make -C $(KERNEL_SRC) M=$(PWD) clean
