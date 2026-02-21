/*
 * hw_pd_ops.h
 *
 * Header file of external operation interface of huawei PD driver
 *
 * Copyright (c) 2019-2023 Huawei Technologies Co., Ltd.
 * Author: HUAWEI, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef _HW_PD_OPS_H_
#define _HW_PD_OPS_H_

#include <chipset_common/hwusb/hw_pd/hw_pd_pps_charger.h>
#include <chipset_common/hwpower/reverse_charge/reverse_charge_pd.h>

struct pd_dpm_ops {
	void (*pd_dpm_detect_emark_cable)(void *client); /* start detect emark cable(c2c) */
	bool (*pd_dpm_get_hw_dock_svid_exist)(void *client); /* get if device has huawei svid */
	int (*pd_dpm_notify_direct_charge_status)(void *client, bool mode); /* get direct charge status(in or out) */
	void (*pd_dpm_set_cc_mode)(int mode); /* set cc mode(DFP, UFP) */
	int (*pd_dpm_get_cc_state)(void); /* get cc mode(DFP, UFP) */
	int (*pd_dpm_disable_pd)(void *client, bool disable); /* disable pd state machine */
	bool (*pd_dpm_check_cc_vbus_short)(void); /* check if cc vbus short */
	void (*pd_dpm_enable_drp)(int mode); /* enable typec drp */
	void (*pd_dpm_reinit_chip)(void *client); /* reinit typec chip config */
	int (*data_role_swap)(void *client); /* start data role swap */
	bool (*pd_dpm_inquire_usb_comm)(void *client); /* Obtain the communication capability of the PD */
#ifdef CONFIG_REVERSE_CHARGE
	bool (*pd_dpm_get_rev_sc_dev_flag)(void); /* get negotiate result */
	void (*pd_dpm_set_rev_sc_dev_flag)(bool flag); /* set negotiate result */
	void (*pd_dpm_set_rev_sc_switch_flag)(int rever_point); /* ui set switch mode */
	int (*pd_dpm_is_reverse_charge_enable)(void); /* enter reverse charger flag */
	bool (*pd_dpm_is_rev_sc_plugin)(void); /* c to c cable plug in */
	bool (*pd_dpm_is_rev_sc_plugout)(void); /* c to c cable plug out */
#endif /* CONFIG_REVERSE_CHARGE */
	int (*pd_dpm_get_src_extcap)(void *client); /* Obtains whether the extended src_cap is supported */
	void (*pd_dpm_set_src_cap)(void *client, struct otg_cap *allow_ext, int send); /* Setting the extended src_cap */
	void (*pd_dpm_set_src_psrdelay)(void *client, int delay); /* Delay sending the ps_rdy message */
	int (*pd_dpm_notify_cable_uvdm)(void *client, uint32_t *data, uint8_t cnt); /* Obtain the uvdm of PD */
	void (*pd_dpm_notify_9v2a_enable)(void *client); /* enable 9v2a capability of the PD */
	int (*pd_dpm_pps_set_state)(void *client, enum pps_set_state state, int mv, int ma); /* set the PPS status */
	/* obtain the APDO */
	int (*pd_dpm_pps_get_apdo_caps)(void *client, struct pps_apdo_cap *caps, int num, struct pps_pdo_raw *raw);
	/* query the current output voltage and current of the charger */
	int (*pd_dpm_pps_get_status)(void *client, int *mv, int *ma);
	int (*pd_dpm_get_dp_state)(void *client); /* get dp status */
	int (*pd_dpm_get_src_cap)(void *client);
};

struct cc_check_ops {
	int (*is_cable_for_direct_charge)(void);
};

struct water_det_ops {
	int (*is_water_detected)(void);
	void (*set_dp_pin)(int high);
};

#ifdef CONFIG_TYPEC_CAP_CUSTOM_SRC2
struct cable_vdo_ops {
	int (*is_cust_src2_cable)(void);
};
#endif

/*
 * Following funtions are used for calling extern interfaces
 * (defined in pd_dpm_ops) for operating to PD/TYPEC chip.
 */
void hw_pd_start_data_role_swap(void); /* data_role_swap */
void hw_pd_emark_detect(void); /* pd_dpm_detect_emark_cable */
void hw_pd_emark_finish(void); /* pd_dpm_inquire_usb_comm & data_role_swap */
int hw_pd_disable_pd_comm(bool disable); /* pd_dpm_disable_pd */
int hw_pd_set_cc_mode(int mode); /* pd_dpm_set_cc_mode */
void hw_pd_set_drp_state(int mode); /* pd_dpm_enable_drp */
void hw_pd_reinit_chip(void); /* pd_dpm_reinit_chip */
bool hw_pd_get_hw_docksvidexist(void); /* pd_dpm_get_hw_dock_svid_exist */
int hw_pd_set_dc_status(bool dc); /* pd_dpm_notify_direct_charge_status */
int hw_pd_query_cc_state(unsigned int *cc); /* pd_dpm_get_cc_state */
int hw_pd_check_cc_short(bool *check); /* pd_dpm_check_cc_vbus_short */
int hw_pd_reg_dpm_ops(struct pd_dpm_ops *ops, void *client); /* Reg new callback functions */
bool hw_pd_support_host_attach(void);
int hw_pd_power_check(void); /* check device weather support power out */

/*
 * Following funtions are used for calling extern interfaces
 * (defined in water_det_ops) for doing wather check operation.
 */
int hw_pd_check_wather_detect(void); /* is_water_detected */
int hw_pd_reg_water_ops(struct water_det_ops* ops); /* Reg new callback functions */

/*
 * Following funtions are used for calling extern interfaces (defined
 * in cable_vdo_ops) for checking whether cable is direct charge.
 */
int hw_pd_dc_cable_detect(void); /* is_cable_for_direct_charge */
int hw_pd_reg_cc_ops(struct cc_check_ops* ops); /* Reg new callback functions */

/*
 * Following funtions are used for calling extern interfaces for
 * (defined in cc_check_ops) checking whether cable is custom designed.
 */
#ifdef CONFIG_TYPEC_CAP_CUSTOM_SRC2
int hw_pd_reg_cablevdo_ops(struct cable_vdo_ops *ops); /* Reg new callback functions */
bool hw_pd_is_custsrc2_cable(void); /* is_cust_src2_cable */
#endif

int hw_pd_pps_get_apdo_caps(struct pps_apdo_cap *caps, int num, struct pps_pdo_raw *raw);
int hw_pd_pps_set_state(enum pps_set_state state, int mv, int ma);
int hw_pd_pps_get_status(int *mv, int *ma);

int hw_pd_get_dp_state(void);
int hw_pd_get_src_cap(void);

bool hw_pd_ops_check_src_cap(void);
void hw_pd_ops_set_src_psrdelay(int delay);
int hw_pd_ops_update_src_cap(u32 otg_vol, u32 otg_cur, int send);

#ifdef CONFIG_REVERSE_CHARGE
int hw_pd_get_reverse_charge_state(void);
bool hw_pd_get_rev_sc_dev_flag(void);
void hw_pd_set_rev_sc_dev_flag(bool flag);
void hw_pd_set_rev_sc_switch_flag(int rever_point);
bool hw_pd_is_rev_sc_plugin(void);
bool hw_pd_is_rev_sc_plugout(void);
#endif /* CONFIG_REVERSE_CHARGE */

#endif

