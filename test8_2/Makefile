# Makefile for cross-compiling a Linux kernel module for ARM64

# Kernel source tree
KERNEL_SRC := /home/nuoen/linux-6.7.12/build

# Cross-compile toolchain prefix
CROSS_COMPILE := aarch64-linux-gnu-

# Kernel architecture
ARCH := arm64

# Module name
obj-m := test82.o

# Default target
all:
	make -C $(KERNEL_SRC) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

# Clean target
clean:
	make -C $(KERNEL_SRC) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean

.PHONY: all clean
