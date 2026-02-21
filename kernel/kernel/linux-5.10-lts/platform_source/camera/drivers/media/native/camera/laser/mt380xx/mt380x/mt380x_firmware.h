/*
 * Linux kernel modules for MT380X FlightSense TOF sensor
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 1 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#ifndef MT380X_FIRMWARE_H
#define MT380X_FIRMWARE_H

#include "mt380x.h"
#define FIRMWARE_NUM 8000
extern uint8_t mt380x_firmware_data[FIRMWARE_NUM];
uint32_t firmware_loading(mt380x_dev dev);

#endif

