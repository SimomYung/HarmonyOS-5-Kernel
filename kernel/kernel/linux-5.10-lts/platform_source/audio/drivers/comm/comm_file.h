/*
 *
 * Copyright (c) 2023-2023 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __COMM_FILE_H__
#define __COMM_FILE_H__

int audio_write_file(const char *name, const char *data, unsigned int size, unsigned short flag);
int audio_create_directory(const char *path);

#endif /* __COMM_FILE_H__ */