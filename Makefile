MCU=at90usb1286
CC=avr-gcc

define FLASH_DEVICE
dfu-programmer $(MCU) erase
dfu-programmer $(MCU) flash $<
dfu-programmer $(MCU) launch
endef

all: boot.hex
.PHONY: clean install

%.hex: %.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

%.asm: %.c
	avr-gcc -S -Os -mmcu=$(MCU) $< -o $@

%.elf: %.c
	avr-gcc -g -Os -mmcu=$(MCU) $< -o $@

install: boot.hex
	$(FLASH_DEVICE) 

clean:
	-rm -rf *.elf *.hex *.asm
