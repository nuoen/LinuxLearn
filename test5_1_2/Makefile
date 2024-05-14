obj-m += mytest.o
CURRENT_PATH:=${shell pwd}
LINUX_KERNEL:=$(shell uname -r)
LINUX_KERNEL_PATH:=/usr/src/linux-headers-$(LINUX_KERNEL)

all:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) modules    #编译模块
#[Tab]              内核的路径       当前目录编译完放哪  表明编译的是内核模块

clean:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) clean      #清理模块