#define F_CPU 9600000

#include <avr/delay.h>
#include <avr/io.h>
#include <avr/fuse.h>
#include <avr/pgmspace.h>

#include "colors.h"

// WS2811 LED strip driver - attiny13a edition
//
// PB3 = data out
#define STRIP_DO PB3
#define STRIP_DDB DDB3
// strips have 60 LEDs per meter and are 5m long = 300 LEDs
// Each IC controls 3 LEDs = 100 24-bit colors to send
#define STRIP_LEN 100


// Timing:
// 1 clock cycle at 9.6MHz is 104.167ns
// The oscillator is a bit slower at 20C, 9.5MHz, i.e. 105.263ns
//
// 0 code: 0.5us high, 2.0us low (+/- 150ns each)
//   -> 5 cycles high, 19 cycles low
// 1 code: 1.2us high, 1.3us low (+/- 150ns each)
//   -> 11 cycles high, 13 cycles low
//   24 cycles between rising edges
// reset: >=50us low

void send1() {
  // call into here costs 3 cycles
  // sbi costs 2 cycles
  // ret from previous send costs 4 cycles
  PORTB |= (1<<STRIP_DO);
  // 11 cycles high - cbi=2 -> 9 nops
  __asm volatile("nop");
  __asm volatile("nop");
  __asm volatile("nop");
  __asm volatile("nop");
  __asm volatile("nop");
  __asm volatile("nop");
  __asm volatile("nop");
  __asm volatile("nop");
  __asm volatile("nop");
  PORTB &= ~(1<<STRIP_DO);
  // 13 cycles low - (ret+call+sbi)=9 -> 4 nops
  __asm volatile("nop");
  __asm volatile("nop");
  __asm volatile("nop");
  __asm volatile("nop");
}

void send0() {
  PORTB |= (1<<STRIP_DO);
  // 5 cycles high - cbi=2 -> 3 nops
  __asm volatile("nop");
  __asm volatile("nop");
  __asm volatile("nop");
  PORTB &= ~(1<<STRIP_DO);
  // 19 cycles low - (ret+call+sbi)=9 -> 10 nops
  __asm volatile("nop");
  __asm volatile("nop");
  __asm volatile("nop");
  __asm volatile("nop");
  __asm volatile("nop");
  __asm volatile("nop");
  __asm volatile("nop");
  __asm volatile("nop");
  __asm volatile("nop");
  __asm volatile("nop");
}

void reset() {
  PORTB &= ~(1<<STRIP_DO);
  _delay_us(50);
}

int main() {
  // Clear prescale bits for 9.6MHz
  CLKPR = 0x80;
  CLKPR = 0x00;

  DDRB |= (1<<STRIP_DDB);

  unsigned char ci = 0;
  while (1) {
    for (int i=0; i<STRIP_LEN; i++) {
      // send 24-bit RGB color
      // blue for some reason
      unsigned char cb = pgm_read_byte(&(col_b[(ci+i)%COLS_LEN]));
      for (int bi = 0; bi < 8; bi++) {
        if (cb & (1<<(7-bi))) {
          send1();
        } else {
          send0();
        }
      }
      // red for some reason
      unsigned char cr = pgm_read_byte(&(col_r[(ci+i)%COLS_LEN]));
      for (int ri = 0; ri < 8; ri++) {
        if (cr & (1<<(7-ri))) {
          send1();
        } else {
          send0();
        }
      }
      // green for some reason
      unsigned char cg = pgm_read_byte(&(col_g[(ci+i)%COLS_LEN]));
      for (int gi = 0; gi < 8; gi++) {
        if (cg & (1<<(7-gi))) {
          send1();
        } else {
          send0();
        }
      }
    }
    reset();
    ci = (ci+1)%COLS_LEN;

    _delay_ms(25);
  }
}
