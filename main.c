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

#include <stdio.h>
#include <stdlib.h>

#include "lcd.h"
#include "wiz6100.h"
#include "ds18b20.h"

static void init_hw()
{
    //Config osc
    //defaults to 8MHz
    //Turn off ADC
    ADCON0bits.ADON = 0;
    //Turn off capsense module
    CPSCON0bits.CPSON = 0;
    //Turn off CCP
    CCP1CONbits.CCP1M = 0;
    CCP2CONbits.CCP2M = 0;
    //Turn off AUSART
    RCSTAbits.SPEN = 0;
    //set all ports to DI/O
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELD = 0x00;
    ANSELE = 0x00;

    TRISA = 0xff;
    TRISB = 0xff;
    TRISC = 0xff;
    TRISD = 0x00;
    TRISE = 0xff;

    PORTD = 0x00;
}

static void init_lcd()
{
    lcd_init();
    lcd_goto(1, 1);
    lcd_puts("WIZnet IoT Design");
    lcd_goto(2, 1);
    lcd_puts("Contest 2019");
    lcd_goto(4, 1);
    lcd_puts("V1.2");
    
    __delay_ms(500);
    __delay_ms(500);
    __delay_ms(500);
    __delay_ms(500);

    lcd_cmd(LCD_CLEAR_DISPLAY);
    lcd_cmd(LCD_CURSOR_HOME);
}

static void init_spi()
{
    SSPCONbits.SSPEN = 0;
    SSPCONbits.SSPM = 0b0000;

    SSPCONbits.CKP = 0;
    SSPSTATbits.SMP = 0;
    SSPSTATbits.CKE = 1;

    //C2, C3 & C5 output
    TRISC = 0b11010011;
    PORTC = 0;

    SSPCONbits.SSPEN = 1;
}

static void init_wiz()
{
    wiz6100_init();
    wiz6100_cfg_net();
}

void main(void) {
    char buf[40];
    uint16_t connects;
    uint32_t ipaddr;
    int16_t temp;
    
    init_hw();
    init_lcd();
    init_spi();
    
    init_wiz();
    
    lcd_goto(1, 1);
    lcd_puts("Waiting for link");
    for (;;)
    {
        if (wiz6100_read_status() == 0x01) break;
    }
    lcd_goto(1, 1);
    lcd_puts("Got link        ");
    
    ipaddr = wiz6100_read_ip();
    sprintf(buf, "IP: %u.%u.%u.%u",
            (unsigned)((ipaddr >> 24) & 0xff),
            (unsigned)((ipaddr >> 16) & 0xff),
            (unsigned)((ipaddr >>  8) & 0xff),
            (unsigned)((ipaddr >>  0) & 0xff));
    lcd_goto(2, 1);
    lcd_puts(buf);

    wiz6100_init_server();

    connects = 0;
    while (1)
    {
        wiz6100_wait_for_connect();
                
        connects++;
        sprintf(buf, "Connects: %u    ", connects);
        lcd_goto(4, 1);
        lcd_puts(buf);
        
        temp = ds18b20_read_temp();
        temp = temp * 9 / 5 + (32 << 4);
        temp = temp >> 4;
        sprintf(buf, "Temp: %d  ", temp);
        lcd_goto(3, 1);
        lcd_puts(buf);

        wiz6100_puts("HTTP/1.0 200 OK\r\n");
        wiz6100_puts("Connection: close\r\n");
        wiz6100_puts("Content-Type: application/json\r\n\r\n");
        sprintf(buf, "{\"temperature\": %d}", temp);
        wiz6100_puts(buf);
        wiz6100_send();
        wiz6100_close();
    
    }
    
    return;
}
