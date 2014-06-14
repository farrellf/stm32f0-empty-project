// Authored by Farrell Farahbod, last revised on 2014-05-20
// This file is released into the public domain

#include "f0lib_adc.h"

/**
 * Setup the ADC with one or more channels.
 * The ADC ISR must be defined and used to read the acquired samples.
 *
 * @param channels	number of channels
 * @param ch1		first channel
 * @param ...		additional channels
 */
void adc_setup(uint8_t channels, enum GPIO_PIN ch1, ...) {
	va_list arglist;
	va_start(arglist, ch1);

	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;				// enable ADC clock
	
	ADC1->CR &= ~ADC_CR_ADEN;						// ensure ADC is off
	ADC1->CR |= ADC_CR_ADCAL;						// begin calibration
	while((ADC1->CR & ADC_CR_ADCAL) != 0);			// wait until calibration is done

ADC1->SMPR = 7; // sample at 14MHz / 252 = 55.5kHz

	gpio_setup(ch1, ANALOG, PUSH_PULL, FIFTY_MHZ, NO_PULL, AF0);
	if(ch1 <= PA7)
		ADC1->CHSELR = (1 << (ch1 % 16));
	else if(ch1 == PB0 || ch1 == PB1)
		ADC1->CHSELR = (1 << (ch1 % 16) + 8);
	else if(ch1 >= PC0)
		ADC1->CHSELR = (1 << (ch1 % 16) + 10);

	while(channels > 1) {
		enum GPIO_PIN pin = va_arg(arglist, int);
		gpio_setup(pin, ANALOG, PUSH_PULL, FIFTY_MHZ, NO_PULL, AF0);
		if(pin <= PA7)
			ADC1->CHSELR |= (1 << (pin % 16));
		else if(pin == PB0 || pin == PB1)
			ADC1->CHSELR |= (1 << (pin % 16) + 8);
		else if(pin >= PC0)
			ADC1->CHSELR |= (1 << (pin % 16) + 10);
		channels--;
	}
	
	ADC1->CFGR1 |= ADC_CFGR1_CONT | ADC_CFGR1_WAIT;	// continuous mode, wait until samples are read before starting next conversion
	ADC1->CR |= ADC_CR_ADEN;						// enable ADC
	while((ADC1->ISR & ADC_ISR_ADRDY) == 0);		// wait until ADC is ready

	ADC1->IER = ADC_IER_EOCIE;						// interrupt on end-of-conversion
	NVIC_EnableIRQ(ADC1_COMP_IRQn);					// enable the ISR
	
	ADC1->CR |= ADC_CR_ADSTART;						// start ADC conversions
}
