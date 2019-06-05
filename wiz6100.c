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

#include <string.h>

#include "spi.h"
#include "wiz6100.h"

#define WIZ6100_RSTn PORTCbits.RC2

#define NETLCKR 0x41f5
#define SHAR 0x4120
#define SUBR 0x4134
#define GAR 0x4130
#define SIPR 0x4138
#define Sn_MR 0x0000
#define Sn_PORTR 0x0114
#define Sn_CR 0x0010
#define Sn_SR 0x0030
#define Sn_RX_RSR 0x0224
#define Sn_TX_WR 0x020c
#define Sn_IR 0x0020
#define Sn_IRCLR 0x0028
#define PHYSR 0x3000
#define COMMONR_BSB 0x00
#define SOCK0R_BSB 0x01
#define SOCK0TXB_BSB 0x02
#define NETLCK_UNLOCK 0x3a
#define NETLCK_LOCK 0x00
#define Sn_MR_TCP4 0x01
#define Sn_CR_OPEN 0x01    
#define Sn_CR_LISTEN 0x02
#define Sn_SR_ESTABLISHED 0x17
#define Sn_CR_DISCONNECT 0x08
#define Sn_CR_CLOSE 0x10
#define Sn_CR_SEND 0x20
#define Sn_IR_SENDOK 0b00010000
#define Sn_IR_DISCON 0b00000010

void wiz6100_init()
{
    NOP();
    WIZ6100_RSTn = 1;
    __delay_ms(60);
    __delay_us(300);
}

uint8_t wiz6100_read_status()
{
    return spi_read_one(PHYSR, COMMONR_BSB);
}

void wiz6100_cfg_net()
{
    //unlock net cfg
    spi_write_one(NETLCKR, COMMONR_BSB, NETLCK_UNLOCK);
    //set MAC
    spi_write_four(SHAR, COMMONR_BSB, 0x02, 0x00, 0x00, 0x00);
    spi_write_two((SHAR+4), COMMONR_BSB, 0x00, 0x04);
    //set default gateway
    spi_write_four(GAR, COMMONR_BSB, 192, 168, 1, 1);
    //set subnet mask
    spi_write_four(SUBR, COMMONR_BSB, 255, 255, 255, 0);
    //set ip
    spi_write_four(SIPR, COMMONR_BSB, 192, 168, 1, 51);
    //lock net cfg
    spi_write_one(NETLCKR, COMMONR_BSB, NETLCK_LOCK);
}

uint32_t wiz6100_read_ip()
{
    return spi_read_four(SIPR, COMMONR_BSB);
}

void wiz6100_init_server()
{
    spi_write_one(Sn_MR, SOCK0R_BSB, Sn_MR_TCP4);
    spi_write_two(Sn_PORTR, SOCK0R_BSB, 0x05, 0xdd); //1501
}

static uint16_t s0_tx_wr;

void wiz6100_wait_for_connect()
{
    spi_write_one(Sn_CR, SOCK0R_BSB, Sn_CR_OPEN);
    while (spi_read_one(Sn_CR, SOCK0R_BSB)) { }
    
    spi_write_one(Sn_CR, SOCK0R_BSB, Sn_CR_LISTEN);
    while (spi_read_one(Sn_CR, SOCK0R_BSB)) { }
    
    while (spi_read_one(Sn_SR, SOCK0R_BSB) != Sn_SR_ESTABLISHED) { }
    
    while (spi_read_two(Sn_RX_RSR, SOCK0R_BSB) == 0) { }
    
    s0_tx_wr = 0xffff;
}

void wiz6100_close()
{
    spi_write_one(Sn_CR, SOCK0R_BSB, Sn_CR_DISCONNECT);
    while (spi_read_one(Sn_CR, SOCK0R_BSB)) { }
    
    while (!(spi_read_one(Sn_IR, SOCK0R_BSB)&Sn_IR_DISCON)) { }
    spi_write_one(Sn_IRCLR, SOCK0R_BSB, Sn_IR_DISCON);
    
    spi_write_one(Sn_CR, SOCK0R_BSB, Sn_CR_CLOSE);
    while (spi_read_one(Sn_CR, SOCK0R_BSB)) { }
    
}

void wiz6100_puts(const char* data)
{
    uint8_t dlen;
    if (s0_tx_wr == 0xffff)
    {
        s0_tx_wr = spi_read_two(Sn_TX_WR, SOCK0R_BSB);
    }
    dlen = strlen(data);
    while (dlen >= 4)
    {
        spi_write_four(s0_tx_wr, SOCK0TXB_BSB, *data, *(data+1), *(data+2), *(data+3));
        dlen -= 4;
        data += 4;
        s0_tx_wr += 4;
    }
    switch (dlen)
    {
        case 3:
        case 2:
            spi_write_two(s0_tx_wr, SOCK0TXB_BSB, *data, *(data+1));
            data += 2;
            s0_tx_wr += 2;
            if (dlen == 2) break;
            /* FALLTHRU */
        case 1:
            spi_write_one(s0_tx_wr, SOCK0TXB_BSB, *data);
            data += 1;
            s0_tx_wr += 1;
            break;
        case 0:
        default:
            break;
    }

}

void wiz6100_send()
{
    spi_write_two(Sn_TX_WR, SOCK0R_BSB, (s0_tx_wr>>8), (s0_tx_wr&0xff));
    
    spi_write_one(Sn_CR, SOCK0R_BSB, Sn_CR_SEND);
    while (spi_read_one(Sn_CR, SOCK0R_BSB)) { }
    
    while (!(spi_read_one(Sn_IR, SOCK0R_BSB)&Sn_IR_SENDOK)) { }
    spi_write_one(Sn_IRCLR, SOCK0R_BSB, Sn_IR_SENDOK);
    
}
