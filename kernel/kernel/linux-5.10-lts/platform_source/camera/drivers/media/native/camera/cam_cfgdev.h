/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2016-2020. All rights reserved.
 * Description: cam_cfgdev header file.
 * Create: 2016-04-01
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef __CAM_CFGDEV_H__
#define __CAM_CFGDEV_H__

extern struct dsm_client *client_camera_user;
void camera_lcd_mipi_switch_status_store(int panel_id);
void camera_lcd_start_switch_mipi_store(int panel_id);

#endif /* __CAM_CFGDEV_H__ */