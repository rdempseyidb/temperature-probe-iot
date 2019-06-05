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

#ifndef WIZ6100_H__
#define WIZ6100_H__

#include "hw.h"

extern void wiz6100_init();
extern uint8_t wiz6100_read_status();
extern void wiz6100_cfg_net();
extern uint32_t wiz6100_read_ip();
extern void wiz6100_init_server();
extern void wiz6100_wait_for_connect();
extern void wiz6100_puts(const char* data);
extern void wiz6100_close();
extern void wiz6100_send();

#endif