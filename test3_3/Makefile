# Makefile for cross-compiling with ARM64

# Set the cross-compiler tools
CC = aarch64-linux-gnu-gcc
AS = aarch64-linux-gnu-as
LD = aarch64-linux-gnu-ld

all:
	$(CC) -o test3_3 testc.c tests.S --static -g

# Clean the build
clean:
	rm -f test *.o
