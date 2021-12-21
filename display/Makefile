TARGET=es6display

# Build as module
obj-m := $(TARGET).o

# Default to cross compile to the board
# override with `make native CROSS_COMPILE='' ARCH=x86`
ARCH := arm
CROSS_COMPILE := /usr/local/xtools/arm-unknown-linux-uclibcgnueabi/bin/arm-linux-

CC= $(CROSS_COMPILE)gcc
WORKINGDIR := $(PWD)
CROSS_KDIR := /home/student/felabs/sysdev/tinysystem/linux-2.6.34
NATIVE_KDIR := /lib/modules/$(shell uname -r)/build

export
cross:
	@echo "Cross compiling"
	$(MAKE) -C $(CROSS_KDIR) M=$(WORKINGDIR) modules	

native:
	@echo "Building for this system"
	$(MAKE) -C $(NATIVE_KDIR) M=$(WORKINGDIR) modules

clean:
	$(MAKE) -C $(NATIVE_KDIR) M=$(WORKINGDIR) clean
	$(MAKE) -C $(CROSS_KDIR) M=$(WORKINGDIR) clean
