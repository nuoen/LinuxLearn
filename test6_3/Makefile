BASEINCLUDE ?= /lib/modules/`uname -r`/build

idemo-objs := rwtest.o

obj-m	:=   idemo.o
all : 
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) modules;

clean:
	$(MAKE) -C $(BASEINCLUDE) M=$(PWD) clean;
	rm -f *.ko;