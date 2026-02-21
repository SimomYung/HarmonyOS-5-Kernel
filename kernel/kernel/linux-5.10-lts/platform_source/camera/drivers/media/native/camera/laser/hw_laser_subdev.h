/*
 * laser_common.h
 *
 * SOC camera driver source file
 *
 * Copyright (C) 2025-2025 Huawei Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef _LASER_COMMON_H_
#define _LASER_COMMON_H_

int laser_probe(struct i2c_client *client,
	const struct i2c_device_id *id);
int laser_remove(struct i2c_client *client);

#endif
