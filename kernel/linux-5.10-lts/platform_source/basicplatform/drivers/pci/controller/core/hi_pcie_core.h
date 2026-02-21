/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: PCIe controller core.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#ifndef HI_PCIE_CORE_H
#define HI_PCIE_CORE_H

#include <linux/types.h>
#include <linux/list.h>
#include <linux/pci.h>
#include <linux/platform_device.h>
#include <linux/notifier.h>

#include "../hi_pcie_defines.h"
#include "../core/hi_pcie_common.h"
#include "../plat/hi_pcie_idle.h"
#include "../phy/hi_pcie_phy.h"
#include "../hi_pcie_external_api.h"
#include "../hi_pcie_internal_api.h"

struct hi_pcie;
struct hi_pcie_port;

enum {
	BOARD_FPGA = 0,
	BOARD_EMU = 1,
	BOARD_ASIC = 2,
};

enum {
	PCIE_LOOPBACK_NONE = 0,
	PCIE_LOOPBACK_LOCAL,
	PCIE_LOOPBACK_REMOTE,
	PCIE_LOOPBACK_AP,
	PCIE_LOOPBACK_DM,
};

enum {
	HI_PCIE_EVENT_LINKDOWN = 0,
	HI_PCIE_EVENT_AXI_TIMEOUT,
	HI_PCIE_EVENT_MAX,
#ifdef CONFIG_PCIE_HCD_LDK
	HI_PCIE_EVENT_UDK_INIT = 0xFF,
#endif
	HI_PCIE_EVENT_INVALID = 0xFFFFFFFF,
};

enum {
	PCIE_DEVICE_DEFAULT = 0,
	PCIE_DEVICE_LOOPBACK,
	PCIE_DEVICE_HI110X,
	PCIE_DEVICE_INVAL,
};

enum {
	PCIE_PLL_FSM_CLOSE = 0,
	PCIE_PLL_FSM_EXCLUSIVE_OPEN,
	PCIE_PLL_FSM_SHARED_OPEN,
	PCIE_PLL_FSM_MAX,
};

enum {
	PCIE_ERRCODE_OK = 0,
	PCIE_ERRCODE_LINKUP_FAILED,
	PCIE_ERRCODE_LINKDOWN,
};

enum {
	PCIE_LANE_START = 0,
	PCIE_LANE_X1 = 1,
	PCIE_LANE_X2 = 2,
	PCIE_LANE_X4 = 4,
	PCIE_LANE_X8 = 8,
};

struct hi_pcie_ctrl_ops {
	int (*init)(struct platform_device *pdev, struct hi_pcie *pcie);
	int (*setup)(struct hi_pcie *pcie);
	int (*ltssm_enable)(struct hi_pcie *pcie);
	int (*probe)(struct hi_pcie *pcie);
	void (*remove_devices)(struct hi_pcie *pcie);
	int (*low_power)(struct hi_pcie_port *port, bool enable);
	int (*save_state)(struct hi_pcie_port *port);
	int (*restore_state)(struct hi_pcie_port *port);
	int (*reset_port)(struct hi_pcie_port *port);
	int (*unreset_port)(struct hi_pcie_port *port);
	int (*change_lane)(struct hi_pcie_port *port, u32 lane);
	int (*change_speed)(struct hi_pcie_port *port, u16 speed);
	int (*enter_l2)(struct hi_pcie_port *port);
	int (*exit_l2)(struct hi_pcie_port *port);
	u32 (*read_conf)(struct hi_pcie_port *port, u32 offset, u32 size);
	int (*write_conf)(struct hi_pcie_port *port, u32 offset, u32 size, u32 val);
	int (*shutdown_prepare)(struct hi_pcie_port *port);
};

struct hi_pcie_dfx_ops {
	void (*show_link_state)(struct hi_pcie_port *port);
	u32 (*get_link_state)(struct hi_pcie_port *port);
	u32 (*get_link_speed)(struct hi_pcie_port *port);
	u32 (*get_link_lane)(struct hi_pcie_port *port);
	int (*enter_loopback)(struct hi_pcie_port *port, u32 type);
	int (*exit_loopback)(struct hi_pcie_port *port);
	void (*dump_port_info)(struct hi_pcie_port *port, u32 type);
	void (*dump_ctrl_info)(struct hi_pcie *pcie, u32 type);
	void (*dump_ltssm_trace)(struct hi_pcie_port *port, u32 reset);
	void (*dump_tlpm_trace)(struct hi_pcie_port *port);
	int (*enter_compliance)(struct hi_pcie_port *port, u32 speed, u32 deemp);
	int (*exit_compliance)(struct hi_pcie_port *port);
};

struct hi_pcie_ep_ops {
	int (*raise_irq)(struct hi_pcie_port *port, u32 type, u32 interrupt);
};

struct hi_pcie_transfer_ops {
	int (*read_by_dma)(struct hi_pcie_port *port, u32 queue, struct hi_pcie_dma_para *para);
	int (*write_by_dma)(struct hi_pcie_port *port, u32 queue, struct hi_pcie_dma_para *para);
	int (*cfg_ecam_atu)(struct hi_pcie_port *port);
	int (*read_by_cpu)(struct hi_pcie_port *port, void *buf, u32 size);
	int (*write_by_cpu)(struct hi_pcie_port *port, void *buf, u32 size);
	void (*setup_inbound)(struct hi_pcie *pcie, u32 index, u32 base, u32 target, u32 size);
	void (*setup_outbound)(struct hi_pcie *pcie, u32 index, u32 base, u32 target, u32 size);
};

struct hi_pcie_platform_ops {
	int (*plat_init)(struct platform_device *pdev, struct hi_pcie *pcie);
	int (*plat_on)(struct hi_pcie *pcie);
	int (*plat_off)(struct hi_pcie *pcie);
	int (*start_link)(struct hi_pcie_port *port);
#ifdef CONFIG_HIPCIE_IDLE
	int (*idle_init)(struct hi_pcie *pcie);
	int (*enter_idle)(struct hi_pcie *pcie, u32 port_id);
	int (*exit_idle)(struct hi_pcie *pcie, u32 port_id);
#endif
	int (*init_phy)(struct hi_pcie *pcie);
	int (*get_ssid_by_dev)(struct hi_pcie *pcie, struct pci_dev *pdev);
	int (*open_sscg)(struct hi_pcie *pcie, u32 pll_cfg);
	int (*dump_plat_info)(struct hi_pcie *pcie);
};

struct hi_pcie {
	struct list_head node;
	void *ctrl;
	void *plat;
	u32 ctrl_id;
	struct device *dev;
	struct platform_device *pdev;
	struct hi_pcie_ctrl_ops *ctrl_ops;
	struct hi_pcie_dfx_ops *dfx_ops;
	struct hi_pcie_ep_ops *ep_ops;
	struct hi_pcie_platform_ops *plat_ops;
	struct hi_pcie_transfer_ops *transfer_ops;
	u32 mode;
	struct mutex port_lock;
	struct list_head ports;
	bool auto_enumerate;
	bool bypass_smmu;
	bool aspm_dis;
	bool enable_async_suspend;
#ifdef CONFIG_PCIE_HCD_LDK	
	bool udk_local_irq;
#endif	
	u32 fnpll_fsm_ctrl;
	u32 port_num; // number in config file
	u32 chip_type;
	u32 mult_ctrler;
	u32 loopback_type;
	u32 ep_l1ss_ctrl2;
	u32 ep_ltr_latency;
	u32 l1ss_ctrl1;
	u32 err_code;

	void __iomem *ctrl_base;
	struct pci_host_bridge *host_bridge;

	atomic_t pwroff_by_usr;
	atomic_t is_ready;  /* driver is ready */
	atomic_t is_power_on;
	atomic_t is_enumerated;
	struct mutex pm_lock;

	struct hi_pcie_phy phy;
	struct mutex power_lock;
	struct hi_pcie_phy_ops *phy_ops;
	struct hi_pcie_phy_dfx_ops *phy_dfx_ops;
	struct hi_pcie_ecam ecam;
	struct hi_pcie_idle *idle;
	struct atomic_notifier_head events[HI_PCIE_EVENT_MAX];
};

struct hi_pcie_port {
	struct list_head node;
	struct hi_pcie *pcie;
	void *priv;
	u32 id;
	u32 port_id;
	u32 num_lanes;
	u32 device_type;
	u32 reset_gpio;
	u32 functions;
	u32 pf;
	u32 ep_device_type;
	u32 dev_ven_id;
	u32 loopback_partner;
	u32 t_ref2perst[2];
	u32 t_perst2access[2];
	u16 limit_speed;
	bool disable;
	bool disable_l0s;
	atomic_t is_link_up;
	struct pci_dev *self_dev;
	struct pci_dev *target_dev;
	struct pci_saved_state *saved_state;
	int (*poweron_callback)(void *data);
	int (*poweroff_callback)(void *data);
	void *power_callback_data;
	struct atomic_notifier_head events[HI_PCIE_EVENT_MAX];
	struct work_struct events_work[HI_PCIE_EVENT_MAX];
};

void pcie_core_add(struct hi_pcie *pcie);
void pcie_core_remove(u32 id);
void pcie_core_port_add(struct hi_pcie *pcie, struct hi_pcie_port *port);

struct hi_pcie *pcie_core_get(u32 id);
struct hi_pcie *pcie_core_get_by_dev(struct device *dev);
struct hi_pcie_port *pcie_core_port_get(u32 id, u32 port);
struct hi_pcie_port *pcie_core_port_new(struct platform_device *pdev,
	struct hi_pcie *pcie, u32 id, void *priv);
int pcie_core_port_parse_config(struct hi_pcie_port *port);
int hi_pcie_enter_idle(struct hi_pcie_port *port);
int hi_pcie_register_ops(struct platform_device *pdev, struct hi_pcie *pcie);
void hi_pcie_invoke_event_handler(struct hi_pcie *pcie, u32 pdx, u32 event);
int pcie_core_setup(struct hi_pcie *pcie);
int pcie_core_do_startup(struct hi_pcie *pcie);
int pcie_core_establish_link(struct hi_pcie *pcie);
void pcie_core_remove_device(struct hi_pcie_port *port);
void pcie_core_rescan_device(struct hi_pcie_port *port);
int pcie_core_pm_up(struct hi_pcie *pcie);
int pcie_core_pm_down(struct hi_pcie *pcie);
int pcie_core_usr_resume(struct hi_pcie *pcie, int power_ops);
int pcie_core_usr_suspend(struct hi_pcie *pcie, int power_ops);
int pcie_core_low_power(struct hi_pcie *pcie, bool enable);
void pcie_core_config_l0sl1(struct hi_pcie_port *port, u32 aspm);
int pcie_core_config_l1ss(struct hi_pcie_port *port, u32 aspm);
int pcie_core_power_ctrl(struct hi_pcie *pcie, u32 power_flag);
int pcie_core_probe_devices(struct hi_pcie *pcie);
int pcie_core_save_state(struct hi_pcie *pcie);
void pcie_core_dump_debug_info(struct hi_pcie_port *port);
void pcie_core_flush_work(struct hi_pcie *pcie);

#ifdef CONFIG_FMEA_FAULT_INJECTION
#ifdef CONFIG_HIPCIE_TEST
int stub_pcie_fault_injection(void);
#endif
#endif
#endif
