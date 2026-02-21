/*
 * blow_res_update.h
 *
 * blow detection result notify lpc
 *
 * Copyright (c) 2024-2025 Huawei Technologies CO., Ltd.
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

#ifndef BLOW_RES_UPDATE_H
#define BLOW_RES_UPDATE_H

#include "dsp_om.h"

int blow_res_update_init(void);
void blow_res_update_deinit(void);
void rcv_dsp_blow_cognition_res_msg(enum socdsp_om_work_id work_id, const uint8_t *data, uint32_t len);

#endif /* __BLOW_RES_UPDATE_H__ */