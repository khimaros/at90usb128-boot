#include <avr/boot.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#define F_CPU 16000000UL
#include <util/delay.h>

/* Bootloader start address in bytes */
#define BOOTLOADER_START (FLASHEND - bootloader_size() + 1)

/* Reset all modified ports and drawdown resistors */
void reset_ports(void) {
        DDRC = 0;
        DDRD = 0;
        PORTC = 0;
        PORTD = 0;
}

/* Release USB devices */
void reset_usb(void) {
        UDCON = 1;
        USBCON = (1 << FRZCLK);
        UCSR1B = 0;
        _delay_ms(5);
}

/* Reset state to prepare for bootloader jump */
void reset_bootloader(void) {
        cli();
        reset_usb();
        reset_ports();
}

/* Boot section size in bytes */
static inline uint16_t bootloader_size(void) {
        uint8_t fuse_bits = boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);
        switch ((fuse_bits >> 1) & 3) {
                case 0: return 8192;
                case 1: return 4096;
                case 2: return 2048;
                case 3: return 1024;
        }
        return 8192;
}

/* Jump to the start of the bootloader section */
void bootloader_jump(void) {
        ((void (*)(void))( (uint16_t)(BOOTLOADER_START / 2) ))();
}

int main(void) {
        reset_bootloader();
        bootloader_jump();
}
