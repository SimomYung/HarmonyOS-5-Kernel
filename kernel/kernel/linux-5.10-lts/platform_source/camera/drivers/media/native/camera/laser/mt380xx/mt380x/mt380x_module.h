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

int mt380x_probe(struct i2c_client *client,
	const struct i2c_device_id *id);
int mt380x_remove(struct i2c_client *client);
long mt380x_ioctl(void *hw_data, unsigned int cmd, void *p);

#endif
