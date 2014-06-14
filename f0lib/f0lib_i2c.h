// Authored by Farrell Farahbod, last revised on 2014-05-20
// This file is released into the public domain

#include "stm32f0xx.h"

/**
 * Possible GPIO usage:
 *
 * I2C1 CLK:	PB6 AF1		PB8 AF1
 * I2C1 SDA:	PB7 AF1		PB9 AF1
 * I2C2 CLK:	PB10 AF1	PF6 AF
 * I2C2 SDA:	PB11 AF1	PF7 AF
 */

/**
 * Configures the I2C peripheral by enabling its clock and setting up timing for Fast Mode (400kHz.)
 *
 * @param i2c	Either I2C1 or I2C2
 */
void i2c_setup(I2C_TypeDef *i2c);

/**
 * Writes a user-specified number of bytes to an I2C device
 *
 * @param i2c			Either I2C1 or I2C2
 * @param i2cAddress	I2C device address
 * @param byteCount		Number of bytes to write
 * @param firstReg		First register to write to
 * @param ...			Bytes to write
 */
void i2c_write_registers(I2C_TypeDef *i2c, uint8_t i2cAddress, uint8_t byteCount, uint8_t firstReg, ...);

/**
 * Read a user-specified number of bytes from an I2C device
 *
 * @param i2c			Either I2C1 or I2C2
 * @param i2cAddress	I2C device address
 * @param byteCount		Number of bytes to read
 * @param firstReg		First register to read from
 * @param ...			Pointers to uint8_t's where values will be stored
 */
void i2c_read_registers(I2C_TypeDef *i2c, uint8_t i2cAddress, uint8_t byteCount, uint8_t firstReg, ...);
