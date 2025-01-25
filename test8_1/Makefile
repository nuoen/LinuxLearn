CROSS_COMPILE ?= aarch64-linux-gnu-
KERNEL_SRC    ?= ../../build 
ARCH          ?= arm64


TEST_OBJS := test8

all:${TEST_OBJS}
$(TEST_OBJS): test8.c
	$(CROSS_COMPILE)gcc -static -o $@ $<


clean:
	rm -f $(TEST_OBJS)