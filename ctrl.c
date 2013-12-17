#include <avr/io.h>
#include <util/delay.h>

int is_pressed(int port, int pin) {
  if(bit_is_clear(port, pin)) {
    _delay_ms(100);
      if(bit_is_clear(port, pin)) return 1;
  }
  return 0;
}

int main(void) {
	int ready = 0;

  DDRB = (1 << DDB4) | (1 << DDB0);
  PORTB = 0;
  while(1) {
    while(!is_pressed(PINB, PB3)) {
      _delay_ms(100);
    }
    PORTB |= (1 << PB0); // ready

    while(!is_pressed(PINB, PB2)) {
      _delay_ms(100);
    }
    PORTB |= (1<<PB4); // start brewing
    PORTB &= ~(1 << PB0); // not ready

    _delay_ms(2000); // Wait

    PORTB &= ~(1<<PB4); // stop brewing
  }


	return 0;
}