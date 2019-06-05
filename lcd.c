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

#include "lcd.h"

#define LCDPORT PORTD
#define LCDTRIS TRISD
#define RW_PIN PORTDbits.RD1
#define RS_PIN PORTDbits.RD2
#define EN_PIN PORTDbits.RD3

#define EN_HI do { EN_PIN = 1; __delay_us(2); } while(0)
#define EN_LOW do { EN_PIN = 0; __delay_us(2); } while(0)

static void busywait()
{
    uint8_t busyflag;
    LCDTRIS = 0xf0;
    LCDPORT = 0xf0;
    RW_PIN = 1;
    RS_PIN = 0;
    busyflag = 1;
    do
    {
        EN_HI;
        if ( (LCDPORT&0x80) == 0 )
            busyflag = 0;
        EN_LOW;
        EN_HI;    // clock in low nibble
        EN_LOW;
    } while (busyflag);
    
    LCDPORT = 0;    // clears RW as well
    LCDTRIS = 0x00;
}

static void lcd_sendbyte(uint8_t b)
{
    uint8_t t;

    RW_PIN = 0;

    t = b&0xf0U;
    LCDPORT |= t;
    t |= 0x0f;
    LCDPORT &= t;
    EN_HI;
    EN_LOW;

    t = b<<4U;
    LCDPORT |= t;
    t |= 0x0f;
    LCDPORT &= t;
    EN_HI;
    EN_LOW;

    busywait();
}

void lcd_cmd(uint8_t cmd)
{
    RS_PIN = 0;
    lcd_sendbyte(cmd);
}

void lcd_data(uint8_t data)
{
    RS_PIN = 1;
    lcd_sendbyte(data);
}

void lcd_init()
{
    __delay_ms(15);

    //function set
    LCDPORT = 0x30; //also clears RS,RW,EN
    EN_HI;
    EN_LOW;
    __delay_ms(4);
    __delay_us(100);

    //function set
    LCDPORT = 0x30;
    EN_HI;
    EN_LOW;
    __delay_us(100);

    //function set
    LCDPORT = 0x30;
    EN_HI;
    EN_LOW;
    __delay_us(100);

    //function set, 4bit
    LCDPORT = 0x20;
    EN_HI;
    EN_LOW;
    busywait();

    //function set, 4bit, 2lines
    lcd_cmd(LCD_FUNCTION_SET);

    //clear display (1.52ms)
    lcd_cmd(LCD_CLEAR_DISPLAY);

    //display & cursor home (1.52ms)
    lcd_cmd(LCD_CURSOR_HOME);

    //entry mode, incr no shift
    lcd_cmd(LCD_ENTRY_MODE);

    //display on/off & cursor, display, underline, blink
    lcd_cmd(LCD_DISPLAY_MODE);    
}

void lcd_puts(const char* str)
{
    while (*str)
        lcd_data(*str++);
}

void lcd_goto(uint8_t row, uint8_t col)
{
    // row 1 = 0x00 - 0x13
    // row 2 = 0x40 - 0x53
    // row 3 = 0x14 - 0x27
    // row 4 = 0x54 - 0x67
    uint8_t ddr_addr;
    ddr_addr = col - 1U;
    if (row == 2)
    {
        ddr_addr += 0x40;
    }
    else if (row == 3)
    {
        ddr_addr += 0x14;
    }
    else if (row == 4)
    {
        ddr_addr += 0x54;
    }
    ddr_addr &= 0b01111111;
    ddr_addr |= 0b10000000;
    lcd_cmd(ddr_addr);
}
