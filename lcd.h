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

#ifndef LCD_H__
#define LCD_H__

#include "hw.h"

#define LCD_CLEAR_DISPLAY 0x01
#define LCD_CURSOR_HOME 0x02
#define LCD_ENTRY_MODE 0x06 //incr curs, no shift
#define LCD_DISPLAY_MODE 0x0f //display on, underline on, blink
#define LCD_FUNCTION_SET 0x28 //4bit, 2lines

extern void lcd_cmd(uint8_t cmd);
extern void lcd_data(uint8_t data);
extern void lcd_init();
extern void lcd_puts(const char* str);
//row is 1-based, col is 1-based
extern void lcd_goto(uint8_t row, uint8_t col);

#endif
