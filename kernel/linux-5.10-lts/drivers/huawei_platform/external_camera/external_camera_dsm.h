/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.

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

#ifndef EXTERNAL_CAMERA_DSM_H
#define EXTERNAL_CAMERA_DSM_H

void external_camera_register_dsm_client(void);
void external_camera_unregister_dsm_client(void);
void external_camera_dsm_client_notify(int dsm_id, const char *fmt, ...);
#endif

