
CROSS_COMPILE ?= arm-linux-

AS              = $(CROSS_COMPILE)as
LD              = $(CROSS_COMPILE)ld
CC              = $(CROSS_COMPILE)gcc
CPP             = $(CC) -E
AR              = $(CROSS_COMPILE)ar
NM              = $(CROSS_COMPILE)nm
LDR             = $(CROSS_COMPILE)ldr
STRIP   = $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
RANLIB  = $(CROSS_COMPILE)RANLIB

CONFIG_START = 0x1000000

TARGET = header

OBJ := _start.o udivsi3.o divsi3.o div0.o
OBJ += header.o
OBJ += serial.o
OBJ += ms_nf.o sonix_nf.o
OBJ += ms_sf.o sonix_sf.o
OBJ +=  vsprintf.o ctype.o string.o

CPPFLAGS := -Wall -Os -DCONFIG_START=$(CONFIG_START)
CFLAGS += -Wall -Os -DCONFIG_START=$(CONFIG_START)
all: $(TARGET).bin

$(TARGET).bin : $(TARGET)
	$(OBJCOPY) --gap-fill=0xff -O binary $< $@
$(TARGET): $(OBJ)
	$(LD) -Bstatic -T ./header.lds -Ttext $(CONFIG_START) $(OBJ) -o $(TARGET)


.PHONY: clean
clean:
	-rm -rf $(OBJ)
	-rm -rf header header.bin
