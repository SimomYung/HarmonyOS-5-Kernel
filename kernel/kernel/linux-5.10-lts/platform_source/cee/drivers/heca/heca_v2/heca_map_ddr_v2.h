/*
 * heca_map_ddr_v2.h
 *
 * hisilicon efficinecy control algorithm ipc communication.
 *
 * Copyright (c) 2019-2020 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef _HECA_MAP_DDR_V2_H_
#define _HECA_MAP_DDR_V2_H_
 
struct freq_ctrl_heca {
	unsigned int enabled;
};

#include <m3_rdr_ddr_map.h>
 
#define HECA_MAP_DATA_OFFSET			(0x0)
#define HECA_MAP_DATA_ADDR				(M3_RDR_HECA_ADDR + HECA_MAP_DATA_OFFSET)
#define HECA_MAP_DATA_SIZE				(0x1000)
 
#define HECA_MAP_GAME_STAT_OFFSET		(HECA_MAP_DATA_OFFSET + HECA_MAP_DATA_SIZE)
#define HECA_MAP_GAME_STAT_ADDR			(M3_RDR_HECA_ADDR + HECA_MAP_GAME_STAT_OFFSET)
#define HECA_MAP_GAME_STAT_SIZE			(0xD10)
 
#define HECA_MAP_STAT_OFFSET		(HECA_MAP_GAME_STAT_OFFSET + HECA_MAP_GAME_STAT_SIZE)
#define HECA_MAP_STAT_ADDR		(M3_RDR_HECA_ADDR + HECA_MAP_STAT_OFFSET)
#define HECA_MAP_STAT_SIZE		(0x300)
 
#define HECA_MAP_RESERVED_ADDR			(HECA_MAP_STAT_ADDR + HECA_MAP_STAT_SIZE)
 
#if ((HECA_MAP_RESERVED_ADDR) > ((M3_RDR_HECA_ADDR) + (M3_RDR_HECA_SIZE)))
#error "heca ddr size overflow!"
#endif
 
#define heca_map_ddr_addr(name, type, addr) {							\
		u32 size = (u32) HECA_MAP_##name##_SIZE;						\
		addr = (!M3_RDR_SYS_CONTEXT_BASE_ADDR || sizeof(type) > size) ?	\
			NULL : (void *) HECA_MAP_##name##_ADDR;						\
	}
 
#endif