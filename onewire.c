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

#include "onewire.h"

#define DQ PORTAbits.RA0
#define DQT TRISAbits.TRISA0

void onew_reset()
{
    DQT = 1;
    DQ = 0;
    DQT = 0;
    __delay_us(500);
    DQT = 1;
    __delay_us(70);
    __delay_us(400);
}

void onew_send(uint8_t data)
{
    int8_t i;
    for (i = 0; i < 8; i++)
    {
        DQ = 0;
        DQT = 0;
        __delay_us(3);
        if (data&0x01)
        {
            DQT = 1;
        }
        else
        {
            NOP();
        }
        __delay_us(60);
        DQT = 1;
        __delay_us(2);
        data >>= 1;
    }
}

static uint8_t onew_read_one()
{
    uint8_t res = 0x00;
    int8_t i;
    for (i = 0; i < 8; i++)
    {
        res >>= 1;
        DQ = 0;
        DQT = 0;
        __delay_us(6);
        DQT = 1;
        __delay_us(4);
        if (DQ)
        {
            res |= 0x80;
        }
        __delay_us(50);
    }
    return res;
}

int16_t onew_read_two()
{
    int16_t res = 0;
    uint16_t b;
    b = onew_read_one();
    res |= b;
    b = onew_read_one();
    res |= (b << 8);
    onew_reset();
    return res;
}