/*
    Wiz610io/DS18B20 IoT temperature probe
    Copyright (C) 2019  Bob Dempsey

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "hw.h"

#include "spi.h"

uint8_t spi_read_one(uint16_t addr, uint8_t bsb)
{
    uint8_t tmp;
    //write addr hi
    SSPBUF = (addr >> 8);
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
    //write addr lo
    SSPBUF = addr;
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
    //write cntl
    tmp = 0b00000001 | (bsb << 3);
    SSPBUF = tmp;
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
    //read data
    SSPBUF = 0x00;
    while (!SSPSTATbits.BF) {}
    return SSPBUF;
}

uint16_t spi_read_two(uint16_t addr, uint8_t bsb)
{
    uint16_t tmp;
    //write addr hi
    SSPBUF = (addr >> 8);
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
    //write addr lo
    SSPBUF = addr;
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
    //write cntl
    tmp = 0b00000010 | (bsb << 3);
    SSPBUF = tmp;
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
    //read data
    SSPBUF = 0x00;
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
    tmp <<= 8;
    SSPBUF = 0x00;
    while (!SSPSTATbits.BF) {}
    tmp |= SSPBUF;
    return tmp;
}

uint32_t spi_read_four(uint16_t addr, uint8_t bsb)
{
    uint32_t tmp;
    //write addr hi
    SSPBUF = (addr >> 8);
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
    //write addr lo
    SSPBUF = addr;
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
    //write cntl
    tmp = 0b00000011 | (bsb << 3);
    SSPBUF = tmp;
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
    //read data
    SSPBUF = 0x00;
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
    tmp <<= 8;
    SSPBUF = 0x00;
    while (!SSPSTATbits.BF) {}
    tmp |= SSPBUF;
    tmp <<= 8;
    SSPBUF = 0x00;
    while (!SSPSTATbits.BF) {}
    tmp |= SSPBUF;
    tmp <<= 8;
    SSPBUF = 0x00;
    while (!SSPSTATbits.BF) {}
    tmp |= SSPBUF;
    return tmp;
}

void spi_write_one(uint16_t addr, uint8_t bsb, uint8_t b1)
{
    uint8_t tmp;
    //write addr hi
    SSPBUF = (addr >> 8);
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
    //write addr lo
    SSPBUF = addr;
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
    //write cntl
    tmp = 0b00000101 | (bsb << 3);
    SSPBUF = tmp;
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
    //write data
    SSPBUF = b1;
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
}

void spi_write_two(uint16_t addr, uint8_t bsb, uint8_t b2, uint8_t b1)
{
    uint8_t tmp;
    //write addr hi
    SSPBUF = (addr >> 8);
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
    //write addr lo
    SSPBUF = addr;
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
    //write cntl
    tmp = 0b00000110 | (bsb << 3);
    SSPBUF = tmp;
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
    //write data
    SSPBUF = b2;
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
    SSPBUF = b1;
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
}

void spi_write_four(uint16_t addr, uint8_t bsb, uint8_t b4, uint8_t b3, uint8_t b2, uint8_t b1)
{
    uint8_t tmp;
    //write addr hi
    SSPBUF = (addr >> 8);
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
    //write addr lo
    SSPBUF = addr;
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
    //write cntl
    tmp = 0b00000111 | (bsb << 3);
    SSPBUF = tmp;
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
    //write data
    SSPBUF = b4;
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
    SSPBUF = b3;
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
    SSPBUF = b2;
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
    SSPBUF = b1;
    while (!SSPSTATbits.BF) {}
    tmp = SSPBUF;
}
