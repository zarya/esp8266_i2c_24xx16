/*
    Driver for the 24xx16 series serial EEPROM from Microchip
    This driver depends on the I2C driver https://github.com/zarya/esp8266_i2c_driver/

    Device start address 0x50 to 0x57

    Copyright (C) 2014 Rudy Hardeman (zarya)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include "i2c/i2c.h"
#include "i2c_24xx16/i2c_24xx16.h"

#include "ets_sys.h"
#include "osapi.h"

/**
 * Read a single byte from the EEPROM
 * uint8 address    : The i2c address
 * uint8 location   : The memory location to read
 */
uint8 ICACHE_FLASH_ATTR
24xx16_readByte(uint8 address, uint8 location)
{
    i2c_start();
    uint8 write_address
    write_address = address << 1;
    write_address |= 1;
    i2c_writeByte(write_address);
    if (!i2c_check_ack())
        return 0;

    i2c_writeByte(location);
    if (!i2c_check_ack())
        return 0;

    i2c_start();
    i2c_writeByte(address << 1);     
    if (!i2c_check_ack())
        return 0;
    data = i2c_readByte();
    i2c_stop();
}

/**
 * Read multiple bytes from the EEPROM
 * uint8 address    : The i2c address
 * uint8 location   : The memory location to read
 * uint8 len        : Number of bytes to read
 */
char ICACHE_FLASH_ATTR
*24xx16_readPage(uint8 address, uint8 location, uint8 len)
{
    char data[len];

    i2c_start();
    uint8 write_address
    write_address = address << 1;
    write_address |= 1;
    i2c_writeByte(write_address);
    if (!i2c_check_ack())
        return 0;

    i2c_writeByte(location);
    if (!i2c_check_ack())
        return 0;

    i2c_start();
    i2c_writeByte(address << 1);     
    if (!i2c_check_ack())
        return 0;

    uint8 i;
    for (i = 0; i < len; i++)
    {
        data[i] = i2c_readByte();
        i2c_sendAck(1);
    }

    i2c_stop();
    return data
}

/**
 * Write a byte to the I2C EEPROM
 * uint8 address    : I2C Device address
 * uint8 location   : Memory location
 * uint8 data       : Data to write to the EEPROM
 */
uint8 ICACHE_FLASH_ATTR
24xx16_writeByte(uint8 address, uint8 location, uint8 data)
{
    i2c_start();
    i2c_writeByte(address << 1);     
    if (!i2c_check_ack())
        return 0;

    i2c_writeByte(data[i]);
    i2c_checkAck();
    if (!i2c_check_ack())
        return 0;

    i2c_stop();
    return 1;
}

/**
 * Read a number of pages from the EEPROM
 * uint8 address    : Address of the module
 * uint8 location   : Start on this memory address
 * char data        : The data to be writen to the EEPROM
 * uint8 len        : The lenght of the data
 */
uint8 ICACHE_FLASH_ATTR
24xx16_writePage(uint8 address, uint8 location, char data[], uint8 len)
{
    i2c_start();
    i2c_writeByte(address << 1);     
    if (!i2c_check_ack())
        return 0;

    uint8 i;
    for (i = 0; i < len; i++)
    {
        i2c_writeByte(data[i]);
        i2c_checkAck();
        if (!i2c_check_ack())
            return 0;
    }

    i2c_stop();
    return 1;
}
