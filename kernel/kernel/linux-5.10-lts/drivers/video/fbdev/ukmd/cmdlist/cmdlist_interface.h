/**
 * @file cmdlist_interface.h
 * @brief Implementing interface function for cmdlist node
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef CMDLIST_INTERFACE_H
#define CMDLIST_INTERFACE_H

#include <linux/types.h>
#include "cmdlist_common.h"

#define INVALID_CMDLIST_ID 0

// Notice: to reuse cmdlist_id on cmdlist user mode, we need 1 bit(bit 63) to indicate
// cmdlist user mode or kernel mode.
// Warning: Require that kernel mode cmdlist_id in [0, uint32 max],
// user mode cmdlist_id in [0, uint63 max](bit63 is used)
#define CMDLIST_ID_USER_BIT (1ULL << 63)
#define CMDLIST_ID_USER_MODE(cmdlist_id) (CMDLIST_ID_USER_BIT | (cmdlist_id))
#define CMDLIST_VA_OFFSET(cmdlist_id) ((~CMDLIST_ID_USER_BIT) & (cmdlist_id))

struct cmdlist_private;

/**
 * @brief According to the scene id and cmdlist id for
 *        DM configuration dma register address
 *
 * @param dev_id cmdlist device id, indicate dpu_cmdlist or aae_cmdlist
 * @param scene_id hardware scene id: 0~3 online composer scene, 4~6 offline composer scene
 * @param cmdlist_id cmdlist_id should be the scene header
 * @return dma_addr_t DM dma register addres which be loaded by dacc hardware
 */
dma_addr_t ukmd_cmdlist_get_dma_addr(uint32_t dev_id, uint32_t scene_id, uint32_t cmdlist_id);

/**
 * @brief Get cmdlist node mode, user mode means we create node and manager it at user space
 *        otherwise, we create node and manager it at kernel space
 *
 * @param dev_id cmdlist device id, indicate dpu_cmdlist or aae_cmdlist
 * @return true for cmdlist kernel mode, false for cmdlist user mode
 */
bool ukmd_cmdlist_is_kernel_mode(uint32_t dev_id);

/**
 * @brief According to dma address offset to get true dma address
 *        DM configuration dma register address
 *
 * @param dev_id cmdlist device id, indicate dpu_cmdlist or aae_cmdlist
 * @param scene_id hardware scene id: 0~3 online composer scene, 4~6 offline composer scene
 * @param cmdlist_id when use_kernel_mode, cmdlist_id is scene cmdlist id, 
 *                   otherwise is dma address offset from base dma address
 * @return uint64_t DM dma register addres which be loaded by dacc hardware
 */
uint64_t ukmd_cmdlist_user_get_dma_addr(uint32_t dev_id, uint32_t scene_id, uint64_t cmdlist_id);

/**
 * @brief According to dev_id to get if cmdlist is using iova or pa
 *
 * @param dev_id cmdlist device id, indicate dpu_cmdlist or aae_cmdlist
 * @return true for virtual address enable
 */
bool ukmd_cmdlist_is_va_enable(uint32_t dev_id);

/**
 * @brief According to scene id and cmdlist id release all cmdlist node in the scene cmdlist,
 *        locked for the principle
 *
 * @param dev_id cmdlist device id, indicate dpu_cmdlist or aae_cmdlist
 * @param scene_id hardware scene id: 0~3 online composer scene, 4~6 offline composer scene
 * @param cmdlist_id cmdlist_id should be the scene header
 */
void ukmd_cmdlist_release_locked(uint32_t dev_id, uint32_t scene_id, uint32_t cmdlist_id);

/**
 * @brief When we use cmdlist kernel mode, According to scene id and cmdlist id release
 *        all cmdlist node in the scene cmdlist, locked for the principle
 *
 * @param dev_id cmdlist device id, indicate dpu_cmdlist or aae_cmdlist
 * @param scene_id hardware scene id: 0~3 online composer scene, 4~6 offline composer scene
 * @param cmdlist_id cmdlist_id should be the scene header
 */
void ukmd_cmdlist_present_release_locked(uint32_t dev_id, uint32_t scene_id, uint64_t cmdlist_id);

/**
 * @brief Create cmdlist client for user to generate register configration
 *        locked for the principle
 *
 * @param dev_id cmdlist device id, indicate dpu_cmdlist or aae_cmdlist
 * @param sceneId which scene hardware work in
 * @param nodeType node type of user need
 * @param dstDmaAddr where would be writed for DM Header Node
 * @param size payload size user need to consider
 */
uint32_t cmdlist_create_user_client(uint32_t dev_id,
	uint32_t scene_id, uint32_t node_type, uint32_t dst_dma_addr, uint32_t size);

void *cmdlist_get_payload_addr(uint32_t dev_id, uint32_t scene_id, uint32_t cmdlist_id);

uint32_t cmdlist_get_phy_addr(uint32_t dev_id, uint32_t scene_id, uint32_t cmdlist_id);

int32_t cmdlist_append_client(uint32_t dev_id, uint32_t scene_id, uint32_t scene_header_id, uint32_t cmdlist_id);

int32_t cmdlist_flush_client(uint32_t dev_id, uint32_t scene_id, uint32_t cmdlist_id);

int32_t ukmd_cmdlist_clear_client(uint32_t dev_id, uint32_t scene_id, uint32_t cmdlist_id);

bool cmdlist_is_ready(uint32_t dev_id);

void ukmd_set_reg(uint32_t dev_id,
	uint32_t scene_id, uint32_t cmdlist_id, uint32_t addr, uint32_t value);
#ifdef CONFIG_UKMD_DEBUG_ENABLE
int32_t ukmd_cmdlist_dump_scene(uint32_t dev_id, uint32_t scene_id);

int32_t ukmd_cmdlist_dump_by_id(uint32_t dev_id, uint32_t scene_id, uint32_t cmdlist_id);

void ukmd_cmdlist_dump_dm(uint32_t dev_id, uint32_t scene_id, uint32_t cmdlist_id);
#endif
#endif
