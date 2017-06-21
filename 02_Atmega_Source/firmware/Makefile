# Name: Makefile
# Author: Muersel Yildiz
# Copyright: @VeNSeY Technology
# License:

# This is a prototype Makefile. Modify it according to your needs.
# You should at least check the settings for
# DEVICE ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
# OBJECTS ...... The object files created from your source files. This list is
#                usually the same as the list of source files with suffix ".o".
# PROGRAMMER ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected. We recommend that you leave it undefined and
#                add settings like this to your ~/.avrduderc file:
#                   default_programmer = "stk500v2"
#                   default_serial = "avrdoper"
# FUSES ........ Parameters for avrdude to flash the fuses appropriately.

PORT       = /dev/tty.SLAB_USBtoUART
DEVICE     = atmega328p
F_CPU      = 16000000UL
CLOCK      = 16000000UL
PROGRAMMER = -c stk500v2 -P $(PORT)
OBJECTS    = main_application.o power_control.o communication_handler.o gpio_twi.o  


#########################################################################
# For the sake of safe operation, we plan to keep internal oscillator
# on and neglect the oscillator circuitry for now... 
#########################################################################

#FUSES      = -U lfuse:w:0xe2:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m
#FUSES      = -U lfuse:w:0xe2:m -U hfuse:w:0xf9:m -U efuse:w:0xff:m
#FUSES      = -U lfuse:w:0xe2:m -U hfuse:w:0xf8:m -U efuse:w:0xff:m
#FUSES      = -U lfuse:w:0x62:m -U hfuse:w:0xf9:m -U efuse:w:0xff:m -B22
#FUSES      = -U lfuse:w:0x62:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m
#FUSES      = -U lfuse:w:0x5f:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m
FUSES      = -U lfuse:w:0xff:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m #this is for external 16Mhz

#FUSES      = -U lfuse:w:0x5f:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m # using external 16Mhz 
#FUSES      = -U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xff:m

LDFLAG     = -I../include/

#########################################################################
# For computing fuse byte values for other devices and options see
# the fuse bit calculator at http://www.engbedded.com/fusecalc/
#########################################################################

# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) $(LDFLAG)

# symbolic targets:
all:	main.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -e -U flash:w:main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean:
	rm -f main.hex main.elf $(OBJECTS)

# file targets:
main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(DEVICE) main.elf
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E main.c
