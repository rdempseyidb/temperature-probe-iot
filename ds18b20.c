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

#include "ds18b20.h"
#include "onewire.h"

int16_t ds18b20_read_temp()
{
    int16_t res = 0;
    //reset
    onew_reset();
    //presence
    NOP();
    //skip ROM
    onew_send(DS18B20_CMD_SKIP_ROM);
    //convert
    onew_send(DS18B20_CMD_CONVERT_T);
    __delay_ms(750);

    //read scratch
    onew_reset();
    NOP();
    onew_send(DS18B20_CMD_SKIP_ROM);
    onew_send(DS18B20_CMD_READ_SCRATCHPAD);
    res = onew_read_two();
    return res;
}
