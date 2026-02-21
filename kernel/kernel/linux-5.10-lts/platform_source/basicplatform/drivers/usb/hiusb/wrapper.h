/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Header file or chip_usb_dubug.c.
 * Create: 2022-6-16
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */
#ifndef _HIUSB_WRAPPER_H_
#define _HIUSB_WRAPPER_H_
#include "core.h"
#include <linux/types.h>
#include <linux/workqueue.h>
#include <linux/timer.h>
#include <linux/notifier.h>
#include <linux/platform_drivers/usb/hiusb.h>
#include <linux/platform_drivers/usb/hiusb_common.h>
#include <linux/platform_drivers/usb/tca.h>

void hiusb_combophy_event_watchdog(struct timer_list *t);
int hiusb_port_dp_trigger_in_suspend_resume(struct hiusb_port *port, bool suspend_flag);
int hiusb_port_dp_trigger(struct hiusb_port *port, struct hiusb_port_state *state);
void hiusb_port_speed_change_work(struct work_struct *work);
void hiusb_port_notify_dp_done(struct hiusb_port *port, enum tcpc_mux_ctrl_type mode_type, int ret);
char *hiusb_get_dev_type_string(struct hiusb_port_state *state);
bool hiusb_port_is_dp_mode(struct hiusb_port_state *state);
void hiusb_port_notify_pd_vbus_ctrl(struct hiusb_port *port,
					enum chip_charger_type charger_type);
void hiusb_port_start_event_timer(struct hiusb_port *port);
void hiusb_port_stop_event_timer(struct hiusb_port *port);
enum tcpc_mux_ctrl_type hiusb_get_mode_type(struct hiusb_port_state *state);
void hiusb_pd_dpm_wakelock_ctrl(unsigned long event);
#endif /* _HIUSB_WRAPPER_H_ */

