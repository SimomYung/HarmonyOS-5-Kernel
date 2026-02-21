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

#ifndef __MT380X_PLATFORM_H
#define __MT380X_PLATFORM_H

#include "mt380x.h"

int32_t mt380x_write_byte(mt380x_dev dev, uint8_t reg, uint8_t data);
int32_t mt380x_read_byte(mt380x_dev dev, uint8_t reg, uint8_t *data);
int32_t mt380x_write_multibytes(mt380x_dev dev, uint8_t reg, uint8_t *data, uint8_t count);
int32_t mt380x_read_multibytes(mt380x_dev dev, uint8_t reg, uint8_t *data, uint8_t count);

#endif
