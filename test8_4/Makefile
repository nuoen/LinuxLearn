CROSS_COMPILE := aarch64-linux-gnu-
ARCH :=arm64

TEST_OBJS := test84

all:${TEST_OBJS}
$(TEST_OBJS): test84.c
	$(CROSS_COMPILE)gcc -static -o $@ $<


clean:
	rm -f $(TEST_OBJS)