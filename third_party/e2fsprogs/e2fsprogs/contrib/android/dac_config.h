/*
 * Copyright (c) 2021-2024 Huawei Device Co., Ltd.
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51 Franklin
 * Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef __DAC_CONFIG
#define __DAC_CONFIG
#include <stdint.h>

#ifdef __cpluscplus
extern "C" {
#endif

int LoadDacConfig(const char* fn);
void GetDacConfig(const char* path, int dir, char* targetOutPath,
        unsigned* uid, unsigned* gid, unsigned* mode,
        uint64_t* capabilities);

#ifdef __cpluscplus
}
#endif
#endif
