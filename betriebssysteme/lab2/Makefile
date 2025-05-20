# ganz oben in der Makefile: automatisch den ersten linux-source-Ordner finden
LINUX_SRC := $(shell ls -d /usr/src/linux-source-* | head -n1)

obj-m := mod_syscall_counter.o

# Header mit Syscall-Namen generieren – hier wird LINUX_SRC als Argument übergeben
syscall_names.h: gen_syscall_names.sh
	@echo "Generating syscall_names.h from $(LINUX_SRC))"
	bash gen_syscall_names.sh $(LINUX_SRC) > $@

all: syscall_names.h
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f syscall_names.h
	

#halloooooo