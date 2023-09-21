PREFIX  = nios2-elf-
CC      = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
GDB     = $(PREFIX)gdb

CFLAGS  = -g -O2 -ffunction-sections -fverbose-asm
CFLAGS += -fno-inline -mno-cache-volatile -mno-hw-mul -mno-hw-div
CFLAGS += -DSYSTEM_BUS_WIDTH=32 -DALT_SINGLE_THREADED
CFLAGS += -I ../lib -I ../src

ASFLAGS = -g

LDFLAGS  = -Tnios_cpp_build.ld -nostartfiles

# Change the following parameters to correspond to the system memory mapping
# JTAG UART address
CFLAGS  += -D_JTAG_UART_BASE=0x9020
# Memory organization
LDFLAGS += -Wl,--defsym -Wl,nasys_stack_top=0x5000
LDFLAGS += -Wl,--defsym -Wl,nasys_program_mem=0x200
LDFLAGS += -Wl,--defsym -Wl,nasys_data_mem=0x200
LDFLAGS += -Wl,--section-start -Wl,.reset=0x0
LDFLAGS += -Wl,--section-start -Wl,.exceptions=0x20

VPATH = ../lib ../src

EXE = test.elf
# startup/irq/print
OBJS  = reset.o nosys_crt0.o nios2_irq.o nios2_jtag_uart.o simple_printf.o
# User program
#OBJS += interval_timer.o pushbutton.o
OBJS += test.o

GDB_OPTS = -x gdbscript

# Replace by your SOF file
#SOFFILE = $(ALTERA_PATH)/University_Program/Computer_Systems/DE1-SoC/DE1-SoC_Computer/verilog/DE1_SoC_Computer.sof
SOFFILE = ../../hard/DE1_SoC.sof

# Where to look for the toolchain
ALTERA_PATH ?=  /comelec/softs/opt/altera/current#

PATH :=$(PATH):$(ALTERA_PATH)/nios2eds/bin/gnu/H-x86_64-pc-linux-gnu/bin
# Other nios/quartus tools
PATH :=$(PATH):$(ALTERA_PATH)/nios2eds/bin/
PATH :=$(PATH):/opt/comelec/bin
export $(PATH)

# bogus gdb TUI (ncurses too old?)
export TERM = xterm-basic
export TERMINFO = /usr/share/terminfo

# srec is needed if you want to user altera monitor program
SREC = $(patsubst %.elf, %.srec, $(EXE))

.PHONY: all clean terminal debug gdb-server fpga-conf

all: $(EXE)


$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.srec:%.elf
	$(OBJCOPY) -O srec $< $@

clean:
	rm -f $(OBJS)
	rm -f $(EXE)
	rm -f $(SREC)

debug: $(EXE)
	$(GDB) $(GDB_OPTS) $<

gdb-server:
	# attach to nios #0
	nios2-gdb-server  -i 0 --tcpport 1234 --tcppersist --reset-target

terminal:
	# attach to jtag uart #0
	nios2-terminal -i 0

fpga-conf:
	nios2-configure-sof -d 2 $(SOFFILE)
