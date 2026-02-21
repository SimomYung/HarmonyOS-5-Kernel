/*
 * lvc_mos.h
 *
 * lvc mos driver
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
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

#ifndef _LVC_MOS_H_
#define _LVC_MOS_H_

struct lvc_mos_ic_ops {
	void *dev_data;
	int (*ic_enable)(bool enable);
	int (*get_bat_btb_voltage)(void *dev_data);
	int (*get_vbus_voltage)(int *vbus, void *dev_data);
	int (*get_ic_ibus)(int *ibus, void *dev_data);
	int (*get_ic_vusb)(int *vusb, void *dev_data);
};

int lvc_mos_ic_ops_register(struct lvc_mos_ic_ops *ops);

#endif /* _LVC_MOS_H_ */
