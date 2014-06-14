// Authored by Farrell Farahbod, last revised on 2014-05-20
// This file is released into the public domain

#include "f0lib_i2c.h"
#include "stdarg.h"

/**
 * Configures the I2C peripheral by enabling its clock and setting up timing for Fast Mode (400kHz.)
 *
 * @param i2c	Either I2C1 or I2C2
 */
void i2c_setup(I2C_TypeDef *i2c) {
	if(i2c == I2C1) {
			// enable i2c1 clock, reset i2c1
			RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
			RCC->APB1RSTR |= RCC_APB1RSTR_I2C1RST;
			RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;

			// i2c1 clock source == sysclk == 48mhz
			RCC->CFGR3 |= RCC_CFGR3_I2C1SW;

			// setup timing for 400hz fast mode
			#define PRESC	15
			#define SCLDEL	1
			#define SDADEL	1
			#define SCLH	14	
			#define SCLL	14
			I2C1->TIMINGR = (SCLL << 0) | (SCLH << 8) | (SDADEL << 16) | (SCLDEL << 20) | (PRESC << 28);

			I2C1->CR1 |= 1; // enable periph
	} else if(i2c == I2C2) {
			// FIXME
	}
}

/**
 * Writes a user-specified number of bytes to an I2C device
 *
 * @param i2c			Either I2C1 or I2C2
 * @param i2cAddress	I2C device address
 * @param byteCount		Number of bytes to write
 * @param firstReg		First register to write to
 * @param ...			Bytes to write
 */
void i2c_write_registers(I2C_TypeDef *i2c, uint8_t i2cAddress, uint8_t byteCount, uint8_t firstReg, ...) {
	va_list arglist;
	va_start(arglist, firstReg);

	i2c->CR2 = (i2cAddress << 1) | I2C_CR2_AUTOEND | ((byteCount+1) << 16);
	i2c->CR2 |= I2C_CR2_START;

	while((i2c->ISR & I2C_ISR_TXIS) == 0); // wait for empty tx buffer
		i2c->TXDR = firstReg | (1<<7); // MSB set = auto-increment

	while(byteCount-- > 0) {
		while((i2c->ISR & I2C_ISR_TXIS) == 0); // wait for empty tx buffer
		i2c->TXDR = va_arg(arglist, int);
	}
}

/**
 * Read a user-specified number of bytes from an I2C device
 *
 * @param i2c			Either I2C1 or I2C2
 * @param i2cAddress	I2C device address
 * @param byteCount		Number of bytes to read
 * @param firstReg		First register to read from
 * @param ...			Pointers to uint8_t's where values will be stored
 */
void i2c_read_registers(I2C_TypeDef *i2c, uint8_t i2cAddress, uint8_t byteCount, uint8_t firstReg, ...) {
	va_list arglist;
	va_start(arglist, firstReg);

	i2c->CR2 = (i2cAddress << 1) | (1 << 16); // 1 byte transfer
	i2c->CR2 |= I2C_CR2_START;

	while((i2c->ISR & I2C_ISR_TXIS) == 0); // wait for empty tx buffer
	i2c->TXDR = (firstReg | (1<<7));// 7 LSBs = reg address, MSB=1 auto-increment

	while((i2c->ISR & I2C_ISR_TC) == 0); // wait for write to complete
	i2c->CR2 = (i2cAddress << 1) | I2C_CR2_RD_WRN | (byteCount << 16);
	i2c->CR2 |= I2C_CR2_START;

	while(byteCount-- > 0) {
		while((i2c->ISR & I2C_ISR_RXNE) == 0); // wait for byte received
		*va_arg(arglist, uint8_t *) = i2c->RXDR;
	}
}
