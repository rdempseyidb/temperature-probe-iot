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

#ifndef SPI_H__
#define SPI_H__

#include "hw.h"

extern uint8_t spi_read_one(uint16_t addr, uint8_t bsb);
extern uint16_t spi_read_two(uint16_t addr, uint8_t bsb);
extern uint32_t spi_read_four(uint16_t addr, uint8_t bsb);
extern void spi_write_one(uint16_t addr, uint8_t bsb, uint8_t b1);
extern void spi_write_two(uint16_t addr, uint8_t bsb, uint8_t b2, uint8_t b1);
extern void spi_write_four(uint16_t addr, uint8_t bsb, uint8_t b4, uint8_t b3, uint8_t b2, uint8_t b1);

#endif