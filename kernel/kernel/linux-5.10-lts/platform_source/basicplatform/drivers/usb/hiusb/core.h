/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Head file of HIUSB core
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2  of
 * the License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _HIUSB_CORE_H_
#define _HIUSB_CORE_H_

#include <linux/fwnode.h>
#include <linux/kfifo.h>
#include <linux/list.h>
#include <linux/platform_device.h>
#include <linux/property.h>
#include <linux/spinlock.h>
#include <linux/notifier.h>
#include <linux/version.h>
#include <linux/workqueue.h>
#include <linux/platform_drivers/usb/hiusb_client.h>
#include <linux/platform_drivers/usb/hiusb.h>
#include <linux/platform_drivers/usb/hiusb_common.h>
#include <linux/platform_drivers/usb/chip_usb.h>

#include <linux/phy/phy.h>
#include <linux/regmap.h>
#include <linux/platform_drivers/usb/usb_reg_cfg.h>

#define HIUSB_PORT_MAXSIZE     5
#define HIUSB_SINGLE_PORT_NUM  0

#define hiusb_start_device_event(port_state)                                     \
	((port_state)->cable.state.usb == HIUSB_CABLE_STATE_IN &&                \
	(port_state)->prop.state.usb_role == HIUSB_PROP_USB_DEVICE)

#define hiusb_start_host_event(port_state)                                       \
	((port_state)->cable.state.usb == HIUSB_CABLE_STATE_IN &&                \
	(port_state)->prop.state.usb_role == HIUSB_PROP_USB_HOST &&              \
	(port_state)->prop.state.hifi_state == HIUSB_PROP_HIFI_OFF)

#define hiusb_stop_device_event(port_state)                                      \
	((port_state)->cable.state.usb == HIUSB_CABLE_STATE_OUT &&               \
	(port_state)->prop.state.usb_role == HIUSB_PROP_USB_DEVICE)

#define hiusb_stop_host_event(port_state)                                        \
	((port_state)->cable.state.usb == HIUSB_CABLE_STATE_OUT &&               \
	(port_state)->prop.state.usb_role == HIUSB_PROP_USB_HOST)

#define hiusb_dp_in_event(port_state)                                            \
	((port_state)->cable.state.dp == HIUSB_CABLE_STATE_IN &&                 \
	(port_state)->prop.state.usb_role == HIUSB_PROP_USB_OFF)

#define hiusb_dp_out_event(port_state)                                           \
	((port_state)->cable.state.dp == HIUSB_CABLE_STATE_OUT &&                \
	(port_state)->prop.state.usb_role == HIUSB_PROP_USB_OFF)

#define hiusb_hifi_event(port_state)                                             \
	((port_state)->prop.state.usb_role == HIUSB_PROP_USB_HOST &&             \
	(port_state)->prop.state.hifi_state != HIUSB_PROP_HIFI_OFF)

#define hiusb_hifi_start_event(port_state)                                       \
	((port_state)->cable.state.usb == HIUSB_CABLE_STATE_IN &&                \
	(port_state)->prop.state.usb_role == HIUSB_PROP_USB_HOST &&              \
	(port_state)->prop.state.hifi_state == HIUSB_PROP_HIFI_START)

#define hiusb_hifi_start_reset_vbus_event(port_state)                            \
	((port_state)->cable.state.usb == HIUSB_CABLE_STATE_IN &&                \
	(port_state)->prop.state.usb_role == HIUSB_PROP_USB_HOST &&              \
	(port_state)->prop.state.hifi_state == HIUSB_PROP_HIFI_START_RESET_VBUS)

#define hiusb_hifi_stop_event(port_state)                                        \
	((port_state)->cable.state.usb == HIUSB_CABLE_STATE_OUT &&               \
	(port_state)->prop.state.usb_role == HIUSB_PROP_USB_HOST &&              \
	(port_state)->prop.state.hifi_state == HIUSB_PROP_HIFI_STOP)

#define hiusb_hifi_stop_reset_vbus_event(port_state)                             \
	((port_state)->cable.state.usb == HIUSB_CABLE_STATE_OUT &&               \
	(port_state)->prop.state.usb_role == HIUSB_PROP_USB_HOST &&              \
	(port_state)->prop.state.hifi_state == HIUSB_PROP_HIFI_STOP_RESET_VBUS)

#define hiusb_hifi_hibernate_event(port_state)                                   \
	((port_state)->cable.state.usb == HIUSB_CABLE_STATE_IN &&                \
	(port_state)->prop.state.usb_role == HIUSB_PROP_USB_HOST &&              \
	(port_state)->prop.state.hifi_state == HIUSB_PROP_HIFI_HIBERNATE)

#define hiusb_hifi_wakeup_event(port_state)                                      \
	((port_state)->cable.state.usb == HIUSB_CABLE_STATE_IN &&                \
	(port_state)->prop.state.usb_role == HIUSB_PROP_USB_HOST &&              \
	(port_state)->prop.state.hifi_state == HIUSB_PROP_HIFI_WAKEUP)

#define hiusb_set_device_in_state(port_state)                                      \
	do { \
		(port_state)->cable.state.dp = HIUSB_CABLE_STATE_OUT;              \
		(port_state)->cable.state.usb = HIUSB_CABLE_STATE_IN;              \
		(port_state)->cable.state.vbus = HIUSB_CABLE_STATE_IN;             \
		(port_state)->prop.state.usb_role = HIUSB_PROP_USB_DEVICE;         \
		(port_state)->prop.state.power_role = HIUSB_PROP_FEILD_VBUS_INPUT; \
		(port_state)->prop.state.lane_num = HIUSB_PROP_DP_2LANE;           \
		(port_state)->prop.state.hpd = HIUSB_PROP_DP_HPD_IN;               \
	} while (0)

#define hiusb_set_device_out_state(port_state)                                     \
	do { \
		(port_state)->cable.state.dp = HIUSB_CABLE_STATE_OUT;              \
		(port_state)->cable.state.usb = HIUSB_CABLE_STATE_OUT;             \
		(port_state)->cable.state.vbus = HIUSB_CABLE_STATE_OUT;            \
		(port_state)->prop.state.usb_role = HIUSB_PROP_USB_DEVICE;         \
		(port_state)->prop.state.power_role = HIUSB_PROP_FEILD_VBUS_INPUT; \
		(port_state)->prop.state.lane_num = HIUSB_PROP_DP_2LANE;           \
		(port_state)->prop.state.hpd = HIUSB_PROP_DP_HPD_OUT;              \
	} while (0)

enum hiusb_port_type {
	HIUSB_PORT_TYPEC,
	HIUSB_PORT_TYPEA,
};

enum hiusb_event_data_type {
	EVENT_DATA_PORT_STATE,
	EVENT_DATA_PORT_TYPE,
	EVENT_DATA_PORT_NAME,
	EVENT_DATA_FIELD,
	EVENT_DATA_MAX
};

struct hiusb_event_manager;
struct hiusb_client;
struct chip_usb_misc_ctrl;
struct hiusb_event;

/* save time stamp, for time consuming analysing */
struct hiusb_time_stamps {
	unsigned long start_device_time_stamp;
	unsigned long start_host_time_stamp;
	unsigned long stop_host_time_stamp;
	unsigned long start_hifiusb_time_stamp;
	unsigned long stop_hifiusb_time_stamp;
	unsigned long start_device_complete_time_stamp;
	unsigned long start_host_complete_time_stamp;
	unsigned long stop_host_complete_time_stamp;
	unsigned long start_hifiusb_complete_time_stamp;
	unsigned long stop_hifiusb_complete_time_stamp;
	unsigned long device_add_time_stamp;
};

struct hiusb_core {
	int device_portid;
	int default_portid;
	int fpga_flag;
	unsigned int soc_spec_efuse;
	struct mutex device_mode_mutex;
	struct notifier_block event_nb;
	struct notifier_block xhci_nb;
	struct notifier_block device_event_nb;

	struct chip_usb_reg_cfg *apb_bridge_reset;
	struct chip_usb_reg_cfg *apb_bridge_unreset;

	/* misc apb clk */
	bool stub_clk;
	struct clk **clks;
	int num_clocks;

	/* SR flag */
	bool s3_power_down;
	bool is_single_port;
	bool usb_s3_lpm3_volt;
	unsigned int s3_perifixup_count;
};

struct hiusb_port {
	struct device dev;
	int portid;
	enum usb_state usb_state;
	enum usb_state init_state;
	enum chip_charger_type charger_type;
	enum chip_charger_type fake_charger_type;
	enum hiusb_port_type type;
	struct wakeup_source *wake_lock;
	struct fwnode_handle *fwnode;
	struct mutex port_mutex;
	struct hiusb_port_state last_event_state;
	struct fwnode_handle *misc_ctrl;
	struct phy *usb2_phy;
	struct phy *usb3_phy;
	struct phy *usb3_lite_phy;
	struct list_head client_list;
	struct list_head shared_client_list;
	struct hiusb_client *cur_client;
	struct hiusb_event_manager *event_manager;
	atomic_t port_power_on;
	unsigned int speed;
	bool port_ready;

	/* for usb SR */
	/* SR: support remote wakeup, such as keyboard and mouse wake-up */
	unsigned support_remote_wakeup:1;
	/* SR: host mode does not hold wakelock */
	unsigned hiusb_host_wakelock_disable:1;
	/* SR: avoid entering SR in non off mode */
	unsigned support_sleep_on_low_power:1;
	/* SR:  when playing music(hifi hibernate), do not perform SR related power-off operations */
	int usb_connect_suspend;

	struct work_struct speed_change_work;

	/* for bc again */
	u32 bc_again_flag;
	u32 disable_bc_detect;
	u32 bc_unknown_again_flag;
	u32 bc_dcp_again_flag;
	unsigned int bc_again_delay_time;
	spinlock_t bc_again_lock;
	struct delayed_work bc_again_work;

	struct notifier_block event_nb;
	struct notifier_block xhci_nb;

	unsigned int vdp_src_enable;
	unsigned quirk_keep_u2_power_suspend : 1;

	unsigned int hifi_ip_first;
	int eventmask;

	int vbus_gpio;
	unsigned int is_fpga_flag;
	int fpga_usb_mode_gpio;
	int fpga_otg_drv_vbus_gpio;
	int fpga_phy_reset_gpio;
	int fpga_phy_switch_gpio;
	struct chip_usb_reg_cfg *fpga_xctrl_phy_unreset;
	struct chip_usb_reg_cfg *fpga_xctrl_phy_reset;

	unsigned int usb_lane_num;
	bool usb_tzpc_config;
	bool notify_state;
	bool support_device_mode;
	bool no_need_vbus_gpio;
	bool usb_init_flag_switch;

	struct dentry *debug_root;
	struct list_head debug_list;
	struct hiusb_time_stamps time_stamps;
	unsigned int init_flag;
};

union hiusb_event_data {
	u64 feild;
	struct hiusb_port_state state; /* for set state or prop event */
	u8 type; /* for switch client by type */
	char *name; /* for switch client by name */
};

typedef int (* hiusb_process_event_t)(struct hiusb_port *, struct hiusb_event *);

#define HIUSB_EVENT_SIZE 16
struct hiusb_event {
	union hiusb_event_data data;
	hiusb_process_event_t process_event;
};

#define MAX_HISI_USB_EVENT_COUNT 8
struct hiusb_event_manager {
	struct hiusb_port *port;
	DECLARE_KFIFO(event_fifo, struct hiusb_event,
					MAX_HISI_USB_EVENT_COUNT);
	spinlock_t event_lock;
	struct work_struct event_work;
	struct timer_list event_timer;
	bool is_timer_start;
};

typedef int (* hiusb_port_iter_fn_t)(struct hiusb_port *);

/* API of hiusb port */
int hiusb_port_init(struct module *owner);
void hiusb_port_exit(void);
struct hiusb_port *hiusb_port_create(struct device *parent, struct fwnode_handle *fwnode);
void hiusb_port_destroy(struct hiusb_port *port);
int hiusb_for_each_port(hiusb_port_iter_fn_t fn);
struct hiusb_port *fwnode_hiusb_port_get(struct fwnode_handle *fwnode);
struct hiusb_port *hiusb_get_port_by_id(int portid);
void hiusb_port_get(struct hiusb_port *port);
void hiusb_port_put(struct hiusb_port *port);
int hiusb_port_queue_event(struct hiusb_port *port, struct hiusb_event *event);
int hiusb_port_start_client(struct hiusb_port *port, u8 type);
int hiusb_port_stop_client(struct hiusb_port *port);
int hiusb_port_switch_client_by_type(struct hiusb_port *port, u8 type);
int hiusb_port_switch_client_by_name(struct hiusb_port *port, char *name);
int hiusb_port_set_dp_hpd(struct hiusb_port *port, bool is_in);
int hiusb_port_hibernate(struct hiusb_port *port);
int hiusb_port_init_hw(struct hiusb_port *port);
int hiusb_port_suspend_client(struct hiusb_port *port, bool keep_remote_wakeup);
int hiusb_port_resume_client(struct hiusb_port *port, bool keep_remote_wakeup);
int hiusb_port_wakeup(struct hiusb_port *port);
int hiusb_port_deinit_hw(struct hiusb_port *port, unsigned int keep_power);
void hiusb_port_set_vbus_power(struct hiusb_port *port, unsigned int is_on);
int hiusb_port_is_powerdown(struct hiusb_port *port);
void hiusb_port_set_charger_type(struct hiusb_port *port, enum chip_charger_type type);
int hiusb_entry_device_mode(int portid);
void hiusb_exit_device_mode(int portid);
bool hiusb_port_is_device_mode(struct hiusb_port *port);
bool hiusb_is_other_port_enumerate(int portid);
int hiusb_get_device_id(void);
int hiusb_get_default_portid(void);
int hiusb_set_default_portid(int portid);
int hiusb_port_ioctrl(struct hiusb_port *port, enum hiusb_port_client_operation operation_code, void* data);
void hiusb_port_wake_lock(struct hiusb_port *port);
void hiusb_port_wake_unlock(struct hiusb_port *port);

void hiusb_port_lock(struct hiusb_port *port);
void hiusb_port_unlock(struct hiusb_port *port);
int hiusb_port_usb2_init(struct hiusb_port *port);
int hiusb_port_usb2_exit(struct hiusb_port *port, unsigned int keep_power);
int hiusb_port_usb3_init(struct hiusb_port *port);
int hiusb_port_usb3_exit(struct hiusb_port *port);
void hiusb_port_phy_prepare(struct hiusb_port *port, union hiusb_cable cable, union hiusb_prop prop);
int hiusb_port_set_dpdm_pulldown(struct hiusb_port *port, bool pulldown);
enum chip_charger_type hiusb_port_detect_charger_type(struct hiusb_port *port);
int hiusb_bc_init(struct hiusb_port *port);
void hiusb_bc_exit(struct hiusb_port *port);
void hiusb_set_port_power_flag(struct hiusb_port *port, int val);
int hiusb_port_set_vdp_src(struct hiusb_port *port, bool enbale);

/* API of hiusb client */
int hiusb_client_init(struct module *owner);
void hiusb_client_exit(void);
struct hiusb_client *hiusb_client_get_by_index(struct fwnode_handle *port, int index);
struct hiusb_client *fwnode_hiusb_client_get(struct fwnode_handle *fwnode);
void hiusb_client_put(struct hiusb_client *client);
int hiusb_client_assign_port(struct hiusb_client *client, enum hiusb_port_usb_mode mode, struct device *parent);
int hiusb_client_deassign_port(struct hiusb_client *client);
int hiusb_client_init_hw_port(struct hiusb_client *client);
int hiusb_client_deinit_hw_port(struct hiusb_client *client, unsigned int keep_power);
int hiusb_client_hibernate_port(struct hiusb_client *client);
int hiusb_client_wakeup_port(struct hiusb_client *client);
int hiusb_client_port_ioctrl(struct hiusb_client *client, enum hiusb_port_client_operation operation_code, void* data);
int hiusb_client_suspend_port(struct hiusb_client *client, bool remote_wakeup);
int hiusb_client_resume_port(struct hiusb_client *client, bool remote_wakeup);

/* API of hiusb event */
struct hiusb_event_manager *hiusb_event_manager_create(struct hiusb_port *port);
void hiusb_event_manager_destroy(struct hiusb_event_manager *emanager);
int hiusb_emanager_queue_event(struct hiusb_event_manager *emanager,
					struct hiusb_event *event);

int hiusb_port_process_set_hifi_usb(struct hiusb_port *port,
					struct hiusb_event *event);
int hiusb_port_process_set_hifi_usb_reset_power(struct hiusb_port *port,
					struct hiusb_event *event);
int hiusb_port_process_set_hifi_hibernate(struct hiusb_port *port,
					struct hiusb_event *event);
int hiusb_port_process_set_hifi_wakeup(struct hiusb_port *port,
					struct hiusb_event *event);
int handle_start_ap_usb_event(struct hiusb_port *port, union hiusb_cable cable,
					union hiusb_prop prop);

int hiusb_proxy_hifi_init(void);
void hiusb_hifi_lock(void);
void hiusb_hifi_unlock(void);
int hiusb_plug_atomic_notifier_call_chain(unsigned long val, void *v);
void hiusb_hifi_reset_state(void);

void hiusb_port_notify_charger_type(struct hiusb_port *port);
bool hiusb_port_enumerate_allowed(struct hiusb_port *port);
bool hiusb_port_sleep_allowed(const struct hiusb_port *port);
bool hiusb_port_bc_again_allowed(const struct hiusb_port *port);
void hiusb_port_schedule_bc_again(struct hiusb_port *port);
void hiusb_port_cancel_bc_again(struct hiusb_port *port, int sync);
struct hiusb_port_handle* get_port_handle_by_port(struct hiusb_port *port);

bool hiusb_event_check(struct hiusb_port *port,
			    struct hiusb_port_state *last_state,
			    struct hiusb_port_state *new_state);
struct hiusb_port_handle* get_port_handle_by_port(struct hiusb_port *port);
struct hiusb_port_handle* get_default_port_handle(void);
void chip_usb_state_notify(enum usb_state state, struct hiusb_port *port);
int hiusb_device_register_notifier(struct hiusb_core *hiusb_core);
void hiusb_device_unregister_notifier(struct hiusb_core *hiusb_core);
enum chip_charger_type hiusb_port_get_charger_type(void);
const char *hiusb_charger_type_string(enum chip_charger_type type);
enum usb_state hiusb_port_get_usb_state(struct hiusb_port *port);
void *hiusb_get_client_class(void);
void *hiusb_get_port_class(void);
unsigned int hiusb_get_soc_spec_efuse(void);
void hiusb_set_port_state(struct hiusb_port *port, enum usb_state state);

#endif /* _HIUSB_CORE_H_ */
