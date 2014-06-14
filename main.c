#include "stm32f0xx.h"
#include "f0lib/f0lib.h"

void main(void) {

	// setup a status LED
	gpio_setup(PC2, OUTPUT, PUSH_PULL, FIFTY_MHZ, NO_PULL, AF0);

	// blink it forever, really fast
	while(1) {
		gpio_high(PC2);
		gpio_low(PC2);
	}

}
