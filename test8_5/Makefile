CROSS_COMPILE := aarch64-linux-gnu-
ARCH :=arm64

TEST_OBJECT := test85

all:${TEST_OBJECT}
${TEST_OBJECT} : test85.c
	${CROSS_COMPILE}gcc -static -o $@ $<

clean:
	rm -f ${TEST_OBJECT}