/*
 * Copyright (C) 2016 Richtek Technology Corp.
 * Author: TH <tsunghan_tsai@richtek.com>
 *
 * Power Delivery Core Driver
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <linux/of.h>
#include <linux/slab.h>
#include <linux/kernel.h>

#define LOG_TAG "[pd][pd_core.c]"

#include "include/tcpci.h"
#include "include/pd_core.h"
#include "include/pd_tcpm.h"
#include "include/pd_dpm_core.h"
#include "include/tcpci_typec.h"
#include "include/tcpci_event.h"
#include "include/pd_policy_engine.h"

#include <securec.h>

#define MSG_ID_VALID 8

/* From DTS */
#ifdef CONFIG_KIRIN_PD_REV30_BAT_INFO
static void pd_parse_pdata_bat_info(
	pd_port_t *pd_port, struct device_node *sub,
	struct pd_battery_info *bat_info)
{
	int ret = 0;
	u32 design_cap;
	uint32_t vid, pid;
#ifdef CONFIG_USB_PD_REV30_MFRS_INFO_LOCAL
	const char *mstring;
	struct pd_manufacturer_info *mfrs_info = &bat_info->mfrs_info;
#endif /* CONFIG_USB_PD_REV30_MFRS_INFO_LOCAL */
	struct pd_battery_capabilities *bat_cap = &bat_info->bat_cap;

	ret = of_property_read_u32(sub, "bat,vid", (u32 *)&vid);
	if (ret < 0) {
		E("%s get pd vid fail\n", __func__);
		vid = PD_IDH_VID(pd_port->id_vdos[0]);
	}

	ret = of_property_read_u32(sub, "bat,pid", (u32 *)&pid);
	if (ret < 0) {
		E("%s get pd pid fail\n", __func__);
		pid = PD_PRODUCT_PID(pd_port->id_vdos[2]);
	}

#ifdef CONFIG_USB_PD_REV30_MFRS_INFO_LOCAL
	mfrs_info->vid = vid;
	mfrs_info->pid = pid;

	ret = of_property_read_string(sub, "bat,mfrs", &mstring);
	if (ret < 0) {
		E("%s get bat,mfrs fail\n", __func__);
		mstring = "no_bat_mfrs_string";
	}
	ret = snprintf_s(mfrs_info->mfrs_string,
		sizeof(mfrs_info->mfrs_string), sizeof(mfrs_info->mfrs_string)-1, "%s", mstring);
	if (ret < 0 || ret >= sizeof(mfrs_info->mfrs_string))
		D("%s-%d snprintf fail\n", __func__, __LINE__);
#endif

	ret = of_property_read_u32(sub, "bat,design_cap", &design_cap);
	if (ret < 0) {
		bat_cap->bat_design_cap = PD_BCDB_BAT_CAP_UNKNOWN;
		E("%s get bat,dsn_cat fail\n", __func__);
	} else {
		bat_cap->bat_design_cap = (uint16_t)
			PD_BCDB_BAT_CAP_RAW(design_cap);
	}

	bat_cap->vid = vid;
	bat_cap->pid = pid;
	bat_cap->bat_last_full_cap = PD_BCDB_BAT_CAP_UNKNOWN;

	bat_info->bat_status = BSDO(
		BSDO_BAT_CAP_UNKNOWN, BSDO_BAT_INFO_IDLE);
}

static int pd_parse_pdata_bats(pd_port_t *pd_port, struct device_node *np)
{
	u32 val = 0;
	int ret = 0, i;
	struct device_node *sub;
	char temp_string[26];

	ret = of_property_read_u32(np, "bat,nr", &val);
	if (ret < 0) {
		E("%s get pd bat NR fail\n", __func__);
		pd_port->bat_nr = 0;
		return 0;
	}

	pd_port->bat_nr = val;
	D("%s Battery NR = %d\n", __func__, pd_port->bat_nr);

	pd_port->fix_bat_info = devm_kzalloc(&pd_port->tcpc_dev->dev,
		sizeof(struct pd_battery_info)*pd_port->bat_nr,
		GFP_KERNEL);

	if (!pd_port->fix_bat_info) {
		E("%s get fix_bat_info memory fail\n", __func__);
		return -ENOMEM;
	}

	for (i = 0; i < pd_port->bat_nr; i++) {
		ret = snprintf_s(temp_string, sizeof(temp_string), sizeof(temp_string)-1, "bat-info%d", i);
		if (ret < 0 || ret >= sizeof(temp_string))
			D("%s-%d snprintf fail\n", __func__, __LINE__);
		sub = of_find_node_by_name(np, temp_string);
		if (!sub) {
			E("%s get sub bat node fail\n", __func__);
			return -ENODEV;
		}

		pd_parse_pdata_bat_info(
			pd_port, sub, &pd_port->fix_bat_info[i]);
	}

	for (i = 0; i < pd_port->bat_nr; i++)
		D("%s fix_bat_info[%d].mfrs_info.vid = 0x%x, .mfrs_info.pid = 0x%x, .mfrs_string = %s, .bat_design_cap = %d\n",
			__func__, i,
			pd_port->fix_bat_info[i].mfrs_info.vid,
			pd_port->fix_bat_info[i].mfrs_info.pid,
			pd_port->fix_bat_info[i].mfrs_info.mfrs_string,
			PD_BCDB_BAT_CAP_VAL(
			pd_port->fix_bat_info[i].bat_cap.bat_design_cap));

	return 0;
}

struct pd_battery_info *hisi_pd_get_battery_info(
	pd_port_t *pd_port, enum pd_battery_reference ref)
{
#ifdef CONFIG_KIRIN_PD_REV30_BAT_INFO
	if (ref < hisi_pd_get_fix_battery_nr(pd_port))
		return &pd_port->fix_bat_info[ref];
#endif

	/* TODO: for swap battery */
	return NULL;
}
#endif

#ifdef CONFIG_KIRIN_PD_REV30_COUNTRY_AUTHORITY
static int pd_parse_pdata_country(
	pd_port_t *pd_port,  struct device_node *sub,
	struct pd_country_authority *country_info)
{
	u32 val;
	int ret = 0, j;
	u32 *temp_u32;

	ret = of_property_read_u32(sub, "pd,country_code", &val);
	if (ret < 0) {
		E("%s get country code fail\n", __func__);
		return -ENODEV;
	}

	country_info->code = (uint16_t) val;

	ret = of_property_read_u32(sub, "pd,country_len", &val);
	if (ret < 0) {
		E("%s get country len fail\n", __func__);
		return -ENODEV;
	}

	country_info->len = (uint16_t) val;

	country_info->data = devm_kzalloc(
		&pd_port->tcpc_dev->dev,
		sizeof(uint8_t)*country_info->len,
		GFP_KERNEL);

	if (!country_info->data) {
		E("%s get country info data mem fail\n",
			__func__);
		return -ENOMEM;
	}

	temp_u32 = devm_kzalloc(&pd_port->tcpc_dev->dev,
		sizeof(u32)*country_info->len, GFP_KERNEL);

	ret = of_property_read_u32_array(sub, "pd,country_data",
		temp_u32,
		country_info->len);
	if (ret < 0)
		E("%s get country data fail\n", __func__);

	for (j = 0; j < country_info->len; j++)
		country_info->data[j] = (uint8_t) temp_u32[j];

	devm_kfree(&pd_port->tcpc_dev->dev, temp_u32);

	return 0;
}

static int pd_parse_pdata_countries(
	pd_port_t *pd_port, struct device_node *np)
{
	int ret = 0, i, j;
	struct device_node *sub;
	char temp_string[26];

	ret = of_property_read_u32(np, "pd,country_nr",
			(u32 *)&pd_port->country_nr);
	if (ret < 0) {
		E("%s get country nr fail\n", __func__);
		pd_port->country_nr = 0;
		return 0;
	}

	D("%s Country NR = %d\n", __func__, pd_port->country_nr);

	pd_port->country_info = devm_kzalloc(&pd_port->tcpc_dev->dev,
		sizeof(struct pd_country_authority)*pd_port->country_nr,
		GFP_KERNEL);

	if (!pd_port->country_info) {
		E("%s get country info memory fail\n", __func__);
		return -ENOMEM;
	}

	for (i = 0; i < pd_port->country_nr; i++) {
		ret = snprintf_s(temp_string, sizeof(temp_string), sizeof(temp_string)-1, "country%d", i);
		if (ret < 0 || ret >= sizeof(temp_string))
			D("%s-%d snprintf fail\n", __func__, __LINE__);
		sub = of_find_node_by_name(np, temp_string);
		if (!sub) {
			E("%s get sub country node fail\n",
				__func__);
			return -ENODEV;
		}

		ret = pd_parse_pdata_country(pd_port,  sub,
			&pd_port->country_info[i]);
		if (ret < 0)
			return ret;
	}

	for (i = 0; i < pd_port->country_nr; i++) {
		D("%s country_info[%d].code = 0x%x, .len = %d\n",
			__func__, i,
			pd_port->country_info[i].code,
			pd_port->country_info[i].len);
		for (j = 0; j < pd_port->country_info[i].len; j++)
			D("%s country_info[%d].data[%d] = 0x%x\n",
				__func__, i, j,
				pd_port->country_info[i].data[j]);
	}
	return 0;
}
#endif

#ifdef CONFIG_KIRIN_PD_REV30_SRC_CAP_EXT_LOCAL
static void pd_parse_log_src_cap_ext(struct pd_source_cap_ext *cap)
{
	D("%s vid = 0x%x, pid = 0x%x, xid = 0x%x, fw_ver = 0x%x, hw_ver = 0x%0x\n",
		__func__,
		cap->vid, cap->pid, cap->xid,
		cap->fw_ver, cap->hw_ver);

	D("%s voltage_regulation = %d, hold_time_ms = %d, compliance = 0x%x, touch_current = 0x%x, peak_current = %d %d %d\n",
		__func__,
		cap->voltage_regulation,
		cap->hold_time_ms,
		cap->compliance,
		cap->touch_current,
		cap->peak_current[0],
		cap->peak_current[1],
		cap->peak_current[2]);

	D("%s touch_temp = %d, source_inputs = 0x%x, batteries = 0x%x, srs_source_pdp = 0x%x\n",
		__func__,
		cap->touch_temp,
		cap->source_inputs,
		cap->batteries,
		cap->srs_source_pdp);
}
#endif

static void pd_parse_pdata_src_cap_ext(
	pd_port_t *pd_port, struct device_node *np)
{
#ifdef CONFIG_KIRIN_PD_REV30_SRC_CAP_EXT_LOCAL
	int ret = 0;

	ret = of_property_read_u8_array(np, "pd,source-cap-ext",
		(u8 *)&pd_port->src_cap_ext, PD_SCEDB_SIZE);
	if (ret < 0)
		pr_notice("%s get source-cap-ext fail\n", __func__);
	else
		pd_parse_log_src_cap_ext(&pd_port->src_cap_ext);

#ifdef CONFIG_KIRIN_PD_REV30_BAT_INFO
	pd_port->src_cap_ext.batteries =
		PD_SCEDB_BATTERIES(0, pd_port->bat_nr);

	if (pd_port->src_cap_ext.batteries)
		pd_port->src_cap_ext.source_inputs |= PD_SCEDB_INPUT_INT;
#endif
#endif
}

static void pd_parse_pdata_snk_cap_ext(
	pd_port_t *pd_port, struct device_node *np)
{
	int ret = 0;

	ret = of_property_read_u8_array(np, "pd,sink-cap-ext",
		(u8 *)&pd_port->snk_cap_ext, PD_SKEDB_SIZE);
	if (ret < 0)
		pr_notice("%s get sink-cap-ext fail\n", __func__);

#ifdef CONFIG_KIRIN_PD_REV30_BAT_INFO
	pd_port->snk_cap_ext.battery_info =
		PD_SKEDB_BATTERIES(0, pd_port->bat_nr);
#endif
}

static void pd_parse_pdata_mfrs(
	pd_port_t *pd_port, struct device_node *np)
{
	int ret = 0;
	uint32_t vid, pid;
#ifdef CONFIG_KIRIN_PD_REV30_MFRS_INFO_LOCAL
	const char *mstring;
#endif

#ifdef CONFIG_KIRIN_PD_REV30_MFRS_INFO_LOCAL
	struct pd_manufacturer_info *mfrs_info = &pd_port->mfrs_info;
#endif

	ret = of_property_read_u32(np, "pd,vid", (u32 *)&vid);
	if (ret < 0) {
		E("%s get pd vid fail\n", __func__);
		vid = PD_IDH_VID(pd_port->id_vdos[0]);
	}

	ret = of_property_read_u32(np, "pd,pid", (u32 *)&pid);
	if (ret < 0) {
		E("%s get pd pid fail\n", __func__);
		pid = PD_PRODUCT_PID(pd_port->id_vdos[2]);
	}

	D("%s VID = 0x%x, PID = 0x%x\n", __func__, vid, pid);

#ifdef CONFIG_KIRIN_PD_REV30_MFRS_INFO_LOCAL
	mfrs_info->vid = vid;
	mfrs_info->pid = pid;

	ret = of_property_read_string(np, "pd,mfrs", &mstring);
	if (ret < 0) {
		E("%s get pd mfrs fail\n", __func__);
		mstring = "no_pd_mfrs_string";
	}
	ret = snprintf_s(mfrs_info->mfrs_string,
		sizeof(mfrs_info->mfrs_string), sizeof(mfrs_info->mfrs_string)-1, "%s", mstring);
	if (ret < 0 || ret >= sizeof(mfrs_info->mfrs_string))
		D("%s-%d snprintf fail\n", __func__, __LINE__);

	D("%s PD mfrs_string = %s\n",
		__func__, mfrs_info->mfrs_string);
#endif

#ifdef CONFIG_KIRIN_PD_REV30_SRC_CAP_EXT_LOCAL
	pd_port->src_cap_ext.vid = vid;
	pd_port->src_cap_ext.pid = pid;
#endif
	pd_port->snk_cap_ext.vid = vid;
	pd_port->snk_cap_ext.pid = pid;

	pd_port->id_vdos[0] &= ~PD_IDH_VID_MASK;
	pd_port->id_vdos[0] |= PD_IDH_VID(vid);

	pd_port->id_vdos[2] = VDO_PRODUCT(
		pid, PD_PRODUCT_BCD(pd_port->id_vdos[2]));

	pd_port->id_header = pd_port->id_vdos[0];
}

static int pd_parse_pdata(pd_port_t *pd_port)
{
	struct device_node *np = NULL;
	int ret;
	int i = 0;

	np = of_find_node_by_name(pd_port->tcpc_dev->dev.parent->of_node, "pd-data-objects");
	if (!np) {
		D("can not find pd-data-objects of node\n");
		return 0;
	}

	/* Source PDO */
	ret = of_property_read_u32(np, "pd,source-pdo-size",
			(u32 *)&pd_port->local_src_cap_default.nr);
	if (ret < 0)
		E("%s get source pdo size fail\n", __func__);

	ret = of_property_read_u32_array(np, "pd,source-pdo-data",
		(u32 *)pd_port->local_src_cap_default.pdos,
		pd_port->local_src_cap_default.nr);
	if (ret < 0)
		E("%s get source pdo data fail\n", __func__);

	/* Sink PDO */
	ret = of_property_read_u32(np, "pd,sink-pdo-size",
				(u32 *)&pd_port->local_snk_cap.nr);
	if (ret < 0)
		E("%s get sink pdo size fail\n", __func__);

	ret = of_property_read_u32_array(np, "pd,sink-pdo-data",
		(u32 *)pd_port->local_snk_cap.pdos,
			pd_port->local_snk_cap.nr);
	if (ret < 0)
		E("%s get sink pdo data fail\n", __func__);

#ifdef CONFIG_KIRIN_PD_REV30_PPS_SINK
	for (i = 0; i < pd_port->local_snk_cap.nr; i++){
		if (PDO_TYPE(pd_port->local_snk_cap.pdos[i]) != PDO_TYPE_APDO)
			pd_port->local_snk_cap_nr_pd20++;
	}
#endif

#ifdef CONFIG_KIRIN_PD_REV30_PPS_SINK
	pd_port->local_snk_cap_nr_pd30 = pd_port->local_snk_cap.nr;
#endif

	/* VDO */
	ret = of_property_read_u32(np, "pd,id-vdo-size",
				(u32 *)&pd_port->id_vdo_nr);
	if (ret < 0)
		E("%s get id vdo size fail\n", __func__);
	ret = of_property_read_u32_array(np, "pd,id-vdo-data",
		(u32 *)pd_port->id_vdos, pd_port->id_vdo_nr);
	if (ret < 0)
		E("%s get id vdo data fail\n", __func__);

#ifdef CONFIG_KIRIN_PD_REV30_BAT_INFO
		ret = pd_parse_pdata_bats(pd_port, np);
		if (ret < 0)
			return ret;
#endif

#ifdef CONFIG_KIRIN_PD_REV30_COUNTRY_AUTHORITY
		ret = pd_parse_pdata_countries(pd_port, np);
		if (ret < 0)
			return ret;
#endif

		pd_parse_pdata_src_cap_ext(pd_port, np);
		pd_parse_pdata_snk_cap_ext(pd_port, np);
		pd_parse_pdata_mfrs(pd_port, np);

	D("-\n");
	return 0;
}

#define DEFAULT_DP_ROLE_CAP			(MODE_DP_SRC)
#define DEFAULT_DP_FIRST_CONNECTED		(DPSTS_DFP_D_CONNECTED)
#define DEFAULT_DP_SECOND_CONNECTED		(DPSTS_DFP_D_CONNECTED)

#ifdef CONFIG_KIRIN_USB_PD_ALT_MODE_SUPPORT
static const struct {
	const char *prop_name;
	uint32_t mode;
} supported_dp_pin_modes[] = {
	{ "pin_assignment,mode_a", MODE_DP_PIN_A },
	{ "pin_assignment,mode_b", MODE_DP_PIN_B },
	{ "pin_assignment,mode_c", MODE_DP_PIN_C },
	{ "pin_assignment,mode_d", MODE_DP_PIN_D },
	{ "pin_assignment,mode_e", MODE_DP_PIN_E },
	{ "pin_assignment,mode_f", MODE_DP_PIN_F },
};

static const struct {
	const char *conn_mode;
	uint32_t val;
} dp_connect_mode[] = {
	{"both", DPSTS_BOTH_CONNECTED},
	{"dfp_d", DPSTS_DFP_D_CONNECTED},
	{"ufp_d", DPSTS_UFP_D_CONNECTED},
};

static void __dpm_alt_mode_parse_svid_connected(pd_port_t *pd_port, struct device_node *np)
{
	const char *connection = NULL;
	unsigned int i;

	pd_port->dp_first_connected = DEFAULT_DP_FIRST_CONNECTED;
	pd_port->dp_second_connected = DEFAULT_DP_SECOND_CONNECTED;

	if (of_property_read_string(np, "1st_connection", &connection) == 0) {
		for (i = 0; i < ARRAY_SIZE(dp_connect_mode); i++) {
			if (strcasecmp(connection, dp_connect_mode[i].conn_mode) == 0) {
				pd_port->dp_first_connected = dp_connect_mode[i].val;
				break;
			}
		}
	}

	if (of_property_read_string(np, "2nd_connection", &connection) == 0) {
		for (i = 0; i < ARRAY_SIZE(dp_connect_mode); i++) {
			if (strcasecmp(connection, dp_connect_mode[i].conn_mode) == 0) {
				pd_port->dp_second_connected = dp_connect_mode[i].val;
				break;
			}
		}
	}

	D("dp_first_connected %d, dp_second_connected %d\n",
		pd_port->dp_first_connected, pd_port->dp_second_connected);
}

static int dpm_alt_mode_parse_svid_data(
		pd_port_t *pd_port, svdm_svid_data_t *svid_data)
{
	struct device_node *np = NULL, *ufp_np = NULL, *dfp_np = NULL;
	uint32_t ufp_d_pin_cap = 0;
	uint32_t dfp_d_pin_cap = 0;
	uint32_t signal = MODE_DP_V13;
	uint32_t receptacle = 1;
	uint32_t usb2 = 0;
	int i;

	D("+\n");
	np = of_find_node_by_name(pd_port->tcpc_dev->dev.of_node, "display-port");
	if (!np) {
		pd_port->svid_data_cnt = 0;
		E("get display port data failed\n");
		return -1;
	}

	svid_data->svid = USB_SID_DISPLAYPORT;
	ufp_np = of_find_node_by_name(np, "ufp_d");
	dfp_np = of_find_node_by_name(np, "dfp_d");

	if (ufp_np) {
		for (i = 0; i < ARRAY_SIZE(supported_dp_pin_modes) - 1; i++) { /* don't support pin_f */
			if (of_property_read_bool(ufp_np, supported_dp_pin_modes[i].prop_name))
				ufp_d_pin_cap |= supported_dp_pin_modes[i].mode;
		}
		D("ufp_d_pin_cap 0x%x\n", ufp_d_pin_cap);
	}

	if (dfp_np) {
		D("dp, dfp_np\n");
		for (i = 0; i < ARRAY_SIZE(supported_dp_pin_modes); i++) {
			if (of_property_read_bool(dfp_np, supported_dp_pin_modes[i].prop_name))
				dfp_d_pin_cap |= supported_dp_pin_modes[i].mode;
		}
		D("dfp_d_pin_cap 0x%x\n", dfp_d_pin_cap);
	}

	if (of_property_read_bool(np, "signal,dp_v13"))
		signal |= MODE_DP_V13;
	if (of_property_read_bool(np, "signal,dp_gen2"))
		signal |= MODE_DP_GEN2;
	if (of_property_read_bool(np, "usbr20_not_used"))
		usb2 = 1;
	if (of_property_read_bool(np, "typec,receptacle"))
		receptacle = 1;

	svid_data->local_mode.mode_cnt = 1;
	svid_data->local_mode.mode_vdo[0] = VDO_MODE_DP(ufp_d_pin_cap, dfp_d_pin_cap,
			usb2, receptacle, signal,
			(ufp_d_pin_cap ? MODE_DP_SNK : 0) | (dfp_d_pin_cap ? MODE_DP_SRC : 0));

	D("mode_cnt 1, mode_vdo[0] 0x%x\n", svid_data->local_mode.mode_vdo[0]);

	__dpm_alt_mode_parse_svid_connected(pd_port, np);

	D("-\n");
	return 0;
}

#endif /* CONFIG_KIRIN_USB_PD_ALT_MODE_SUPPORT */

static void pd_core_parse_svid_data(pd_port_t *pd_port)
{
	uint8_t i = 0;

#ifdef CONFIG_KIRIN_USB_PD_ALT_MODE_SUPPORT
	int ret;
	ret = dpm_alt_mode_parse_svid_data(pd_port, &pd_port->svid_data[i]);
	if (ret == 0)
		i++;
#endif

	pd_port->svid_data_cnt = i;
	D("svid_data_cnt %d\n", pd_port->svid_data_cnt);

	WARN_ON(i > VDO_MAX_SVID_SIZE);
}

struct supported_dpm_cap {
	const char *prop_name;
	uint32_t val;
};

static const struct supported_dpm_cap supported_dpm_caps[] = {
	{"local_dr_power", DPM_CAP_LOCAL_DR_POWER},
	{"local_dr_data", DPM_CAP_LOCAL_DR_DATA},
	{"local_ext_power", DPM_CAP_LOCAL_EXT_POWER},
	{"local_usb_comm", DPM_CAP_LOCAL_USB_COMM},
	{"local_usb_suspend", DPM_CAP_LOCAL_USB_SUSPEND},
	{"local_high_cap", DPM_CAP_LOCAL_HIGH_CAP},
	{"local_give_back", DPM_CAP_LOCAL_GIVE_BACK},
	{"local_no_suspend", DPM_CAP_LOCAL_NO_SUSPEND},
	{"local_vconn_supply", DPM_CAP_LOCAL_VCONN_SUPPLY},

	{"attemp_discover_cable_dfp", DPM_CAP_ATTEMP_DISCOVER_CABLE_DFP},
	{"attemp_enter_dp_mode", DPM_CAP_ATTEMP_ENTER_DP_MODE},
	{"attemp_discover_cable", DPM_CAP_ATTEMP_DISCOVER_CABLE},
	{"attemp_discover_id", DPM_CAP_ATTEMP_DISCOVER_ID},

	{"pr_reject_as_source", DPM_CAP_PR_SWAP_REJECT_AS_SRC},
	{"pr_reject_as_sink", DPM_CAP_PR_SWAP_REJECT_AS_SNK},
	{"pr_check_gp_source", DPM_CAP_PR_SWAP_CHECK_GP_SRC},
	{"pr_check_gp_sink", DPM_CAP_PR_SWAP_CHECK_GP_SNK},

	{"dr_reject_as_dfp", DPM_CAP_DR_SWAP_REJECT_AS_DFP},
	{"dr_reject_as_ufp", DPM_CAP_DR_SWAP_REJECT_AS_UFP},

	{"snk_emark_discover", DPM_CAP_SNK_EMARK_DISCOVER},
	
	{"snk_prefer_low_voltage", DPM_CAP_SNK_PREFER_LOW_VOLTAGE},
	{"snk_ignore_mismatch_current", DPM_CAP_SNK_IGNORE_MISMATCH_CURRENT},
};

static void pd_charging_policy_init(pd_port_t *pd_port)
{
	uint32_t val = DPM_CHARGING_POLICY_MAX_POWER;
	struct device_node *np = NULL;

	/*
	 * default charging policy depends on pd_port caps:
	 *      1) DPM_CAP_SNK_PREFER_LOW_VOLTAGE
	 *      2) DPM_CAP_SNK_IGNORE_MISMATCH_CURRENT
	 */
	if (pd_port->dpm_caps & DPM_CAP_SNK_PREFER_LOW_VOLTAGE)
		val |= DPM_CHARGING_POLICY_PREFER_LOW_VOLTAGE;

	if (pd_port->dpm_caps & DPM_CAP_SNK_IGNORE_MISMATCH_CURRENT)
		val |= DPM_CHARGING_POLICY_IGNORE_MISMATCH_CURR;

	np = of_find_node_by_name(pd_port->tcpc_dev->dev.parent->of_node,
			"pd-data-objects");
	if (!np || of_property_read_u32(np, "pd,charging_policy", &val) < 0)
		I("%s get pd charging policy fail\n", __func__);
	mutex_lock(&pd_port->pd_lock);
	pd_port->dpm_charging_policy = val;
	pd_port->dpm_charging_policy_default = val;
	mutex_unlock(&pd_port->pd_lock);
	D("pdm charging policy 0x%x\n", val);
}

static void pd_core_power_flags_init(pd_port_t *pd_port)
{
	uint32_t src_flag, snk_flag;
	uint32_t val = 0;
	struct device_node *np = NULL;
	uint32_t i;
	pd_port_power_caps *snk_cap = &pd_port->local_snk_cap;
	pd_port_power_caps *src_cap = &pd_port->local_src_cap_default;

	D("+\n");
	np = of_find_node_by_name(pd_port->tcpc_dev->dev.parent->of_node, "dpm-caps");

	for (i = 0; i < ARRAY_SIZE(supported_dpm_caps); i++)
		if (of_property_read_bool(np, supported_dpm_caps[i].prop_name))
			pd_port->dpm_caps |= supported_dpm_caps[i].val;

	pd_port->dpm_caps |= DPM_CAP_ATTEMP_DISCOVER_CABLE_DFP;

	if (of_property_read_u32(np, "pr_check", &val) == 0)
		pd_port->dpm_caps |= DPM_CAP_PR_CHECK_PROP(val);
	else
		E("%s get pr_check data fail\n", __func__);

	if (of_property_read_u32(np, "dr_check", &val) == 0)
		pd_port->dpm_caps |= DPM_CAP_DR_CHECK_PROP(val);
	else
		E("%s get dr_check data fail\n", __func__);

	I("dpm_caps = 0x%08x\n", pd_port->dpm_caps);

	src_flag = 0;
	if (pd_port->dpm_caps & DPM_CAP_LOCAL_DR_POWER)
		src_flag |= PDO_FIXED_DUAL_ROLE;

	if (pd_port->dpm_caps & DPM_CAP_LOCAL_DR_DATA)
		src_flag |= PDO_FIXED_DATA_SWAP;

	if (pd_port->dpm_caps & DPM_CAP_LOCAL_EXT_POWER)
		src_flag |= PDO_FIXED_EXTERNAL;

	if (pd_port->dpm_caps & DPM_CAP_LOCAL_USB_COMM)
		src_flag |= PDO_FIXED_COMM_CAP;

	if (pd_port->dpm_caps & DPM_CAP_LOCAL_USB_SUSPEND)
		src_flag |= PDO_FIXED_SUSPEND;

	snk_flag = src_flag;
	if (pd_port->dpm_caps & DPM_CAP_LOCAL_HIGH_CAP)
		snk_flag |= PDO_FIXED_HIGH_CAP;

	snk_cap->pdos[0] |= snk_flag;
	src_cap->pdos[0] |= src_flag;
	D("snk_flag 0x%x, src_flag 0x%x\n", snk_flag, src_flag);

	pd_charging_policy_init(pd_port);
}

struct vender_defined_functions {
	const char *prop_name;
	uint32_t val;
};

static const struct vender_defined_functions supported_vdf_flags[] = {
	{"tx_discard_mask", VDF_FLAGS_TX_DISCARD_MASK},
	{"atc_cc_open_mask", VDF_FLAGS_ATC_CC_OPEN_MASK},
	{"sink_prs_quick", VDF_FLAGS_SINK_PRS_QUICK_MASK},
	{"pd_31_certification", VDF_FLAGS_PD_31_CERTIFICATION_MASK},
};

static void pd_core_vendor_defined_function_flags_init(pd_port_t *pd_port)
{
	struct device_node *np = NULL;
	uint32_t i;

	D("+\n");
	np = of_find_node_by_name(pd_port->tcpc_dev->dev.parent->of_node, "vdf-switch");

	for (i = 0; i < ARRAY_SIZE(supported_vdf_flags); i++)
		if (of_property_read_bool(np, supported_vdf_flags[i].prop_name))
			pd_port->vdf_flags |= supported_vdf_flags[i].val;

	I("vdf_flags = 0x%08x\n", pd_port->vdf_flags);
}

void hisi_pd_init_spec_revision(pd_port_t *pd_port)
{
	uint8_t version = 0;
	uint8_t revision = 0;
	tcpci_get_pd_version(pd_port->tcpc_dev, &version, &revision);

	pd_port->pd_revision[0] = PD_REV30;
	pd_port->pd_revision[1]= PD_REV30;
	pd_port->version = version;
	pd_port->tcpc_dev->retry_cnt = PD_RETRY_COUNT;

#ifdef CONFIG_KIRIN_PD_REV30
	if (revision < 0x30) {
		pd_port->pd_revision[0] = PD_REV20;
		pd_port->pd_revision[1] = PD_REV20;
	}
#endif
	I("init pd rev: 0x%x-0x%x\n", pd_port->pd_revision[0], pd_port->pd_revision[1]);
}

int hisi_pd_core_init(struct tcpc_device *tcpc_dev)
{
	int ret;
	pd_port_t *pd_port = &tcpc_dev->pd_port;

	mutex_init(&pd_port->pd_lock);
	mutex_init(&pd_port->tcpm_bk_lock);
	init_waitqueue_head(&pd_port->tcpm_bk_wait_que);

	pd_port->tcpc_dev = tcpc_dev;
	D("tcpc_dev %pK\n", pd_port->tcpc_dev);

	pd_port->pe_pd_state = PE_IDLE2;
	pd_port->pe_vdm_state = PE_IDLE2;
	D("pe_pd_state %d, pe_vdm_state %d\n",
			pd_port->pe_pd_state, pd_port->pe_vdm_state);

	pd_port->pd_connect_state = HISI_PD_CONNECT_NONE;
	D("pd_connect_state %d\n", pd_port->pd_connect_state);

	ret = pd_parse_pdata(pd_port);
	if (ret)
		return ret;

	pd_core_parse_svid_data(pd_port);
	pd_core_power_flags_init(pd_port);

	pd_core_vendor_defined_function_flags_init(pd_port);

	hisi_pd_init_spec_revision(pd_port);

	hisi_pd_dpm_core_init(pd_port);

	I("hisi_pd_core_init\n");

	return 0;
}

int pd_is_auto_discover_cable_id(pd_port_t *pd_port)
{
	D("+\n");
	if (pd_port->dpm_flags & DPM_FLAGS_CHECK_CABLE_ID) {
		if (pd_port->discover_id_counter < PD_DISCOVER_ID_COUNT)
			return true;

		pd_port->dpm_flags &= ~DPM_FLAGS_CHECK_CABLE_ID;
		return false;
	}

	return false;
}

void hisi_pd_extract_rdo_power(uint32_t rdo, uint32_t pdo,
			uint32_t *op_curr, uint32_t *max_curr)
{
	uint32_t op_power, max_power, vmin;

	D("+\n");

	switch (pdo & PDO_TYPE_MASK) {
	case PDO_TYPE_FIXED:
	case PDO_TYPE_VARIABLE:
		*op_curr = RDO_FIXED_VAR_EXTRACT_OP_CURR(rdo);
		*max_curr = RDO_FIXED_VAR_EXTRACT_MAX_CURR(rdo);
		break;

	case PDO_TYPE_BATTERY:
		vmin = PDO_BATT_EXTRACT_MIN_VOLT(pdo);
		op_power = RDO_BATT_EXTRACT_OP_POWER(rdo);
		max_power = RDO_BATT_EXTRACT_MAX_POWER(rdo);

		*op_curr = op_power / vmin;
		*max_curr = max_power / vmin;
		break;

	default:
		*op_curr = *max_curr = 0;
		break;
	}
	D("-\n");
}

uint32_t hisi_pd_reset_pdo_power(uint32_t pdo, uint32_t imax)
{
	uint32_t ioper;

	D("+\n");
	switch (pdo & PDO_TYPE_MASK) {
	case PDO_TYPE_FIXED:
		ioper = PDO_FIXED_EXTRACT_CURR(pdo);
		if (ioper > imax)
			return PDO_FIXED_RESET_CURR(pdo, imax);
		break;

	case PDO_TYPE_VARIABLE:
		ioper = PDO_VAR_EXTRACT_CURR(pdo);
		if (ioper > imax)
			return PDO_VAR_RESET_CURR(pdo, imax);
		break;

	case PDO_TYPE_BATTERY:
		E("No Support\n");
		break;

	default:
		break;
	}
	D("-\n");
	return pdo;
}

void hisi_pd_extract_pdo_power(uint32_t pdo,
		uint32_t *vmin, uint32_t *vmax, uint32_t *ioper)
{
	uint32_t pwatt;

	switch (pdo & PDO_TYPE_MASK) {
	case PDO_TYPE_FIXED:
		*ioper = PDO_FIXED_EXTRACT_CURR(pdo);
		*vmin = *vmax = PDO_FIXED_EXTRACT_VOLT(pdo);
		break;

	case PDO_TYPE_VARIABLE:
		*ioper = PDO_VAR_EXTRACT_CURR(pdo);
		*vmin = PDO_VAR_EXTRACT_MIN_VOLT(pdo);
		*vmax = PDO_VAR_EXTRACT_MAX_VOLT(pdo);
		break;

	case PDO_TYPE_BATTERY:
		*vmin = PDO_BATT_EXTRACT_MIN_VOLT(pdo);
		*vmax = PDO_BATT_EXTRACT_MAX_VOLT(pdo);
		pwatt = PDO_BATT_EXTRACT_OP_POWER(pdo);
		*ioper = pwatt / *vmin;
		break;

	default:
		*vmin = *vmax = *ioper = 0;
	}
}

bool pd_is_source_support_apdo(pd_port_t *pd_port)
{
#ifdef CONFIG_KIRIN_PD_REV30_PPS_SINK
	uint8_t i;
	uint32_t pdo;

	pd_port_power_caps *src_cap = &pd_port->remote_src_cap;

	for (i = 0; i < src_cap->nr; i++) {
		pdo = src_cap->pdos[i];

		if (PDO_TYPE(pdo) == PDO_TYPE_APDO)
			return true;
	}
#endif

	return false;
}

uint32_t hisi_pd_extract_cable_curr(uint32_t vdo)
{
	uint32_t cable_curr;

	D("+\n");
	switch (PD_VDO_CABLE_CURR(vdo)) {
	case CABLE_CURR_1A5:
		cable_curr = 1500; /* 1500: CABLE_CURR_1A5 */
		break;
	case CABLE_CURR_5A:
		cable_curr = 5000; /* 5000: CABLE_CURR_5A */
		break;
	default:
	case CABLE_CURR_3A:
		cable_curr = 3000; /* 3000: CABLE_CURR_3A */
		break;
	}
	D("cable cur_cap: %u\n", cable_curr);

	D("-\n");
	return cable_curr;
}

void hisi_pd_reset_svid_data(pd_port_t *pd_port)
{
	uint8_t i;
	svdm_svid_data_t *svid_data = NULL;

	D("+\n");
	for (i = 0; i < pd_port->svid_data_cnt; i++) {
		svid_data = &pd_port->svid_data[i];
		svid_data->exist = false;
		svid_data->remote_mode.mode_cnt = 0;
		svid_data->active_mode = 0;
	}

	/* clear */
	hisi_usb_pd_ufp_update_dock_svid(0);
	D("-\n");
}

int hisi_pd_reset_protocol_layer(pd_port_t *pd_port)
{
	int i;

	hisi_pd_notify_pe_reset_protocol(pd_port);

	pd_port->explicit_contract = 0;
	pd_port->local_selected_cap = 0;
	pd_port->remote_selected_cap = 0;
	pd_port->during_swap = 0;
	pd_port->dpm_ack_immediately = 0;

#ifdef CONFIG_KIRIN_PD_REV30_ALERT_REMOTE
	pd_port->remote_alert = 0;
#endif
#ifdef CONFIG_KIRIN_PD_REV30_ALERT_LOCAL
	pd_port->local_alert = 0;
#endif

#ifdef CONFIG_USB_PD_DFP_FLOW_DELAY
#ifdef CONFIG_USB_PD_DFP_FLOW_DELAY_RESET
	if (pd_port->pd_prev_connected)
		pd_port->dpm_dfp_flow_delay_done = 0;
#endif
#endif

#ifdef CONFIG_USB_PD_DFP_READY_DISCOVER_ID
	pd_port->vconn_return = false;
#endif

	for (i = 0; i < PD_SOP_NR; i++) {
		pd_port->msg_id_tx[i] = MSG_ID_VALID;
		pd_port->msg_id_rx[i] = MSG_ID_VALID;
		pd_port->msg_id_rx_init[i] = false;
	}

#ifdef CONFIG_USB_PD_IGNORE_PS_RDY_AFTER_PR_SWAP
	pd_port->msg_id_pr_swap_last = 0xff;
#endif

	return 0;
}

void hisi_pd_set_rx_enable(pd_port_t *pd_port, uint8_t enable)
{
	D("0x%x\n", enable);
	(void)tcpci_set_rx_enable(pd_port->tcpc_dev, enable);
}

void hisi_pd_set_vbus_detect(pd_port_t *pd_port, bool enable)
{
	tcpci_set_vbus_detect(pd_port->tcpc_dev, enable);
	return;
}

int hisi_pd_enable_vbus_valid_detection(pd_port_t *pd_port, bool wait_valid)
{
	D("wait_valid %d\n", wait_valid);

	if (wait_valid) {
		hisi_pd_notify_pe_wait_vbus_once(pd_port, PD_WAIT_VBUS_VALID_ONCE);
	} else {
		hisi_pd_notify_pe_wait_vbus_once(pd_port, PD_WAIT_VBUS_INVALID_ONCE);
	}

	return 0;
}

int hisi_pd_enable_vbus_safe0v_detection(pd_port_t *pd_port)
{
	D("WaitVSafe0V\n");
	hisi_pd_notify_pe_wait_vbus_once(pd_port, PD_WAIT_VBUS_SAFE0V_ONCE);
	return 0;
}

int hisi_pd_enable_vbus_stable_detection(pd_port_t *pd_port)
{
	D("WaitVStable\n");
	hisi_pd_notify_pe_wait_vbus_once(pd_port, PD_WAIT_VBUS_STABLE_ONCE);
	return 0;
}

int hisi_pd_set_data_role(pd_port_t *pd_port, uint8_t dr)
{
	int ret;

	D("+\n");
	pd_port->data_role = dr;

#ifdef CONFIG_DUAL_ROLE_USB_INTF
	/* dual role usb--> 0:ufp, 1:dfp */
	/* dual role usb --> 0: Device, 1: Host */
	pd_port->tcpc_dev->dual_role_dr = !(pd_port->data_role);

#ifdef CONFIG_REVERSE_CHARGE
	if (!(hisi_tcpc_support_reverse_sc(pd_port->tcpc_dev) && hisi_usb_is_reverse_charge_enable() == 1)) {
		pd_port->tcpc_dev->dual_role_mode = DUAL_ROLE_PROP_MODE_DFP;
	} else {
		pd_port->tcpc_dev->dual_role_mode = DUAL_ROLE_PROP_MODE_UFP;
	}
#else
	pd_port->tcpc_dev->dual_role_mode = pd_port->data_role;

#endif

	hisi_tcpc_dual_role_instance_changed(pd_port->tcpc_dev->dr_usb);
#endif /* CONFIG_DUAL_ROLE_USB_INTF */

	tcpci_notify_role_swap(pd_port->tcpc_dev, TCP_NOTIFY_DR_SWAP, dr);
	ret = tcpci_set_msg_header(pd_port->tcpc_dev,
			pd_port->power_role, pd_port->data_role, PD_REV20);
	D("-\n");
	return ret;
}

int hisi_pd_set_power_role(pd_port_t *pd_port, uint8_t pr)
{
	int ret;

	D("+\n");
	pd_port->power_role = pr;
	ret = tcpci_set_msg_header(pd_port->tcpc_dev, pd_port->power_role, pd_port->data_role, PD_REV20);
	if (ret)
		return ret;

	/* modiry power_role, and change cc setting */
	hisi_pd_notify_pe_pr_changed(pd_port);

#ifdef CONFIG_DUAL_ROLE_USB_INTF
	/* 0:sink, 1: source */
	pd_port->tcpc_dev->dual_role_pr = !(pd_port->power_role);
	hisi_tcpc_dual_role_instance_changed(pd_port->tcpc_dev->dr_usb);
#endif /* CONFIG_DUAL_ROLE_USB_INTF */

	tcpci_notify_role_swap(pd_port->tcpc_dev, TCP_NOTIFY_PR_SWAP, pr);
	D("-\n");
	return ret;
}

int hisi_pd_init_role(pd_port_t *pd_port, uint8_t pr, uint8_t dr, bool vr)
{
	int ret;

	pd_port->power_role = pr;
	pd_port->data_role = dr;
	pd_port->vconn_source = vr;

	ret = tcpci_set_msg_header(pd_port->tcpc_dev,
			pd_port->power_role, pd_port->data_role, PD_REV20);

	return ret;
}

int hisi_pd_set_vconn(pd_port_t *pd_port, int enable)
{
	int ret;

	D("+\n");
	pd_port->vconn_source = enable;

#ifdef CONFIG_DUAL_ROLE_USB_INTF
	pd_port->tcpc_dev->dual_role_vconn = pd_port->vconn_source;
	hisi_tcpc_dual_role_instance_changed(pd_port->tcpc_dev->dr_usb);
#endif /* CONFIG_DUAL_ROLE_USB_INTF */

	tcpci_notify_role_swap(pd_port->tcpc_dev,
				TCP_NOTIFY_VCONN_SWAP, enable);
	ret = tcpci_set_vconn(pd_port->tcpc_dev, enable);
	D("-\n");
	return ret;
}

static int pd_reset_modal_operation(pd_port_t *pd_port)
{
	uint8_t i;
	svdm_svid_data_t *svid_data = NULL;

	D("+\n");
	for (i = 0; i < pd_port->svid_data_cnt; i++) {
		svid_data = &pd_port->svid_data[i];

		if (svid_data->active_mode) {
			svid_data->active_mode = 0;
			tcpci_exit_mode(pd_port->tcpc_dev, svid_data->svid);
		}
	}

	pd_port->modal_operation = false;
	D("-\n");
	return 0;
}

int hisi_pd_reset_local_hw(pd_port_t *pd_port)
{
	D("+\n");
	hisi_pd_notify_pe_transit_to_default(pd_port);
	hisi_pd_unlock_msg_output(pd_port);

	pd_reset_pe_timer(pd_port);
#ifdef CONFIG_KIRIN_PD_REV30_PPS_SINK
	if (pd_port->request_apdo) {
		pd_port->request_apdo = false;
		hisi_pd_dpm_pps_request_heart_beat(pd_port);
	}
#endif
	hisi_pd_set_rx_enable(pd_port, PD_RX_CAP_PE_HARDRESET);

	pd_port->explicit_contract = false;
	pd_port->pd_connected  = false;
	pd_port->pe_ready = false;
	pd_port->dpm_ack_immediately = false;

	hisi_pd_cable_flag_clear(pd_port);

#ifdef CONFIG_USB_PD_HANDLE_PRDR_SWAP
	pd_port->postpone_pr_swap = false;
	pd_port->postpone_dr_swap = false;
#endif /* CONFIG_USB_PD_HANDLE_PRDR_SWAP */

	pd_reset_modal_operation(pd_port);

	hisi_pd_set_vconn(pd_port, false);

	if (pd_port->power_role == PD_ROLE_SINK) {
		pd_port->state_machine = PE_STATE_MACHINE_SINK;
		D("state_machine %d(%s)\n", pd_port->state_machine,
			pd_pe_state_machine_name(pd_port->state_machine));
		hisi_pd_set_data_role(pd_port, PD_ROLE_UFP);
	} else {
		pd_port->state_machine = PE_STATE_MACHINE_SOURCE;
		D("state_machine %d(%s)\n", pd_port->state_machine,
			pd_pe_state_machine_name(pd_port->state_machine));
		hisi_pd_set_data_role(pd_port, PD_ROLE_DFP);
	}

	hisi_pd_dpm_notify_pe_hardreset(pd_port);
	D("reset_local_hw\n");

	D("-\n");
	return 0;
}

int hisi_pd_enable_bist_test_mode(pd_port_t *pd_port, bool en)
{
	D("bist_test_mode=%d\n", en);
	return tcpci_set_bist_test_mode(pd_port->tcpc_dev, en);
}

/* ---- Handle PD Message ----*/

int hisi_pd_handle_soft_reset(pd_port_t *pd_port, uint8_t state_machine)
{
	int ret;

	D("+\n");
	pd_port->state_machine = state_machine;
	D("state_machine %s\n", pd_pe_state_machine_name(pd_port->state_machine));
	hisi_pd_reset_protocol_layer(pd_port);
	pd_notify_tcp_event_buf_reset(pd_port, TCP_DPM_RET_DROP_RECV_SRESET);
	ret = hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP, PD_CTRL_ACCEPT);
	D("-\n");
	return ret;
}

/* ---- Send PD Message ----*/

#ifdef CONFIG_DFX_DEBUG_FS
static void print_send_vdm_msg(uint16_t header, const uint32_t *payload)
{
	uint32_t vdo_count = header & (0x3 << 12);
	uint32_t vdm_hdr = payload[0];

	char *vdm_cmd_type[4] = {"REQ", "ACK", "NAK", "BUSY"};
	char vdm_cmd = vdm_hdr & 0x1f;
	char *vdm_cmd_name[32] = {
		"Reserved",
		"Discover Identity",
		"Discover SVIDs",
		"Discover Modes",
		"Enter Mode",
		"Exit Mode",
		"Attention",
	};

	D("VDO count %d\n", vdo_count >> 12); /* 12: vdo count */
	D("SVID 0x%x, vdm type %s, version %d, Object Position %d, "
		"Command Type %s, Command %d %s\n",
			vdm_hdr >> 16, /* 16: SVID */
			(vdm_hdr & (1 << 15)) ? "structed" : "unstructed", /* (1 << 15): vdm type */
			(vdm_hdr >> 13) & 0x3, /* (vdm_hdr >> 13) & 0x3: version */
			(vdm_hdr >> 8) & 0x7, /* (vdm_hdr >> 8) & 0x7: Object position */
			vdm_cmd_type[(int)((vdm_hdr >> 6) & 0x3)], /* (vdm_hdr >> 6) & 0x3): command type */
			vdm_cmd, /* command */
			(vdm_cmd < 7) ? vdm_cmd_name[(int)vdm_cmd] : "reserved"); /* (vdm_cmd < 7): vdm_cmd_name */

	return;
}

static void print_send_msg(uint8_t sop_type, uint8_t msg, uint16_t count,
		const uint32_t *data, uint16_t msg_hdr, bool ext)
{
	int i;
	char * sop_type_name[] = {
		"SOP",
		"SOP'",
		"SOP''",
		"SOP_DEBUG'",
		"SOP_DEBUG''",
		"HARD_RESET",
		"CABLE_RESET",
		"BIST_MODE_2"
	};

	char * const ctrl_msg_name[32] = {
		"Reserved",
		"GoodCRC",
		"GotoMin",
		"Accept",
		"Reject",
		"Ping",
		"PS_RDY",
		"Get_Source_Cap",
		"Get_Sink_Cap",
		"DR_Swap",
		"PR_Swap",
		"VCONN_Swap",
		"Wait",
		"Soft_Reset",
		"Data_Reset",
		"Data_Reset_CPL",
		"Not_Supported",
		"Get_Source_Cap_Extended",
		"Get_Status",
		"FR_Swap",
		"Get_PPS_Status",
		"Get_Country_Codes",
		"Get_Sink_Cap_Ext",
		"Get_Source_Info",
		"Get_Revision",
	};

	char * const data_msg_name[] = {
		"data0",
		"src_cap",
		"request",
		"bist",
		"sink_cap",
		"data5",
		"data6",
		"data7",
		"data8",
		"data9",
		"dataA",
		"dataB",
		"dataC",
		"dataD",
		"dataE",
		"vdm",
	};

	char *ext_msg[] = {
	"ext0",
	"ext_src_cap_ext"
	"ext_status",
	"ext_get_bat_cap",
	"get_bat_status",
	"ext_bat_cap",
	"ext_get_mfr_info",
	"ext_mfr_info",
	"ext_sec_req",
	"ext_sec_resp",
	"ext_fw_update_req",
	"ext_fw_update_resp",
	"ext_pps_sts",
	"ext_country_info",
	"ext_country_code",
	"ext_sink_cap_ext",
	};

	if (ext) {		
		D("%s, " "EXT_MSG, hdr 0x%x  ext_hdr 0x%x %s cnt: %u\n",
			sop_type_name[sop_type], msg_hdr, data[0], ext_msg[msg], count);
			for (i = 0; i < count; i++)
				D("ext msg[%d] 0x%x\n", i, data[i]);
		
	} else {
		if (count == 0) {
			D("%s, " "Control Msg %d(%s), hdr 0x%x\n",
				sop_type_name[sop_type], msg, ctrl_msg_name[msg], msg_hdr);
#ifdef CONFIG_KIRIN_PD_REV30
		} else if (msg == PD_DATA_VENDOR_DEF) {
			D("%s, " "VDM, hdr 0x%x, VDM hdr 0x%x\n",
				sop_type_name[sop_type], msg_hdr, data[0]);
			print_send_vdm_msg(msg_hdr, data);
#endif
		} else {
			D("%s, " "Data Msg %d(%s), hdr 0x%x\n",
				sop_type_name[sop_type], msg, data_msg_name[msg], msg_hdr);
			for (i = 0; i < count; i++)
				D("Data PDO[%d] 0x%x\n", i, data[i]);
		}
	}
}
#else
static inline void print_send_msg(uint8_t sop_type, uint8_t msg,
		uint16_t count,	const uint32_t *data, uint16_t msg_hdr, bool ext){}
#endif

int pd_send_message(pd_port_t *pd_port, uint8_t sop_type,
		uint8_t msg, bool ext, uint16_t count, const uint32_t *data)
{
	int ret;
	uint16_t msg_hdr;
	uint8_t pd_rev = PD_REV20;
	uint8_t type = PD_TX_STATE_WAIT_CRC_PD;
	struct tcpc_device *tcpc_dev = pd_port->tcpc_dev;

	if (tcpc_dev->typec_attach_old == TYPEC_UNATTACHED) {
		D("[SendMsg] Unattached\n");
		return 0;
	}

	if (tcpc_dev->pd_hard_reset_event_pending) {
		D("[SendMsg] HardReset Pending");
		return 0;
	}

	pd_rev = hisi_pd_get_rev(pd_port, sop_type);
	if (pd_rev > PD_REV20)
		pd_port->tcpc_dev->retry_cnt = PD_RETRY_COUNT_30;
	if (sop_type == TCPC_TX_SOP) {
		if (pd_port->vdf_flags & VDF_FLAGS_PD_31_CERTIFICATION_MASK) {
			if ((count > 0) && (msg == PD_DATA_SINK_CAP) && (!ext))
				pd_port->msg_id_tx[sop_type] = 0;
		}
		msg_hdr = PD_HEADER_SOP(msg, pd_rev, pd_port->power_role, pd_port->data_role,
				pd_port->msg_id_tx[sop_type], count);
	} else {
		msg_hdr = PD_HEADER_SOP_PRIME(msg, pd_rev, 0, pd_port->msg_id_tx[sop_type], count);
	}

	// ext msg
	msg_hdr = msg_hdr | ((ext) << 15);
	I("msg header: 0x%x\n", msg_hdr);

	if ((count > 0) && (msg == PD_DATA_VENDOR_DEF) && (!ext))
		type = PD_TX_STATE_WAIT_CRC_VDM;

	pd_port->msg_id_tx[sop_type] = (pd_port->msg_id_tx[sop_type] + 1) % 8; /* 8: message_id count */

	print_send_msg(sop_type, msg, count, data, msg_hdr, ext);

	hisi_pd_notify_pe_transmit_msg(pd_port, type);
	ret = tcpci_transmit(pd_port->tcpc_dev, sop_type, msg_hdr, data);
	if (ret < 0)
		E("[SendMsg] Failed, %d\n", ret);

	return ret;
}

int hisi_pd_send_ctrl_msg(pd_port_t *pd_port, uint8_t sop_type, uint8_t msg)
{
	return pd_send_message(pd_port, sop_type, msg, false, 0, NULL);
}

int hisi_pd_send_data_msg(pd_port_t *pd_port, uint8_t sop_type,
		uint8_t msg, uint8_t cnt, uint32_t *payload)
{
	return pd_send_message(pd_port, sop_type, msg, false, cnt, payload);
}

#ifdef CONFIG_KIRIN_PD_REV30
int hisi_pd_send_ext_msg(pd_port_t *pd_port,
		uint8_t sop_type, uint8_t msg, bool request,
		uint8_t chunk_nr, uint8_t size, uint8_t *data)
{
	uint8_t cnt;
	uint32_t payload[PD_DATA_OBJ_SIZE] = {0};
	uint16_t *ext_hdr = (uint16_t *)payload;

	cnt = ((size + PD_EXT_HEADER_PAYLOAD_INDEX - 1) / 4) + 1;
	payload[cnt-1] = 0;
	*ext_hdr = PD_EXT_HEADER_CK(size, (unsigned int)request, chunk_nr, 1);

	if (memcpy_s(&ext_hdr[1], EXT_MSG_DATA_SIZE(PD_DATA_OBJ_SIZE), data, size) != EOK) {
		E("memcpy_s failed\n");
	}

	return pd_send_message(pd_port, sop_type, msg, true, cnt, payload);
}

int hisi_pd_send_status(pd_port_t *pd_port)
{
	return hisi_pd_send_ext_msg(pd_port, TCPC_TX_SOP, PD_EXT_STATUS,
			false, 0, PD_SDB_SIZE, pd_port->local_status);
}

uint8_t *hisi_pd_get_ext_msg_payload(pd_event_t *pd_event)
{
	uint8_t *payload = NULL;
	pd_msg_t *pd_msg = pd_event->pd_msg;

	payload = (uint8_t *) pd_msg->payload;
	return payload + PD_EXT_HEADER_PAYLOAD_INDEX;
}
#endif

int hisi_pd_send_cable_soft_reset(pd_port_t *pd_port)
{
#ifdef CONFIG_USB_PD_RESET_CABLE
	if (!pd_port)
		return 0;

	/* reset_protocol_layer */
	pd_port->msg_id_tx[TCPC_TX_SOP_PRIME] = 0;
	pd_port->msg_id_rx[TCPC_TX_SOP_PRIME] = 0;
	pd_port->msg_id_rx_init[TCPC_TX_SOP_PRIME] = false;

	return hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP_PRIME, PD_CTRL_SOFT_RESET);
#endif
}

bool pd_put_pe_event(pd_port_t *pd_port, uint8_t pe_event)
{
	pd_event_t evt = {
		.event_type = PD_EVT_PE_MSG,
		.msg = pe_event,
		.pd_msg = NULL,
	};

	return hisi_pd_put_event(pd_port->tcpc_dev, &evt, false);
}

bool pd_put_dpm_event(pd_port_t *pd_port, uint8_t event)
{
	pd_event_t evt = {
		.event_type = PD_EVT_DPM_MSG,
		.msg = event,
		.pd_msg = NULL,
	};

	return hisi_pd_put_event(pd_port->tcpc_dev, &evt, false);
}

bool pd_put_dpm_pd_request_event(pd_port_t *pd_port, uint8_t event)
{
	pd_event_t evt = {
		.event_type = PD_EVT_DPM_MSG,
		.msg = PD_DPM_PD_REQUEST,
		.msg_sec = event,
		.pd_msg = NULL,
	};

	return hisi_pd_put_event(pd_port->tcpc_dev, &evt, false);
}

bool vdm_put_dpm_vdm_request_event(pd_port_t *pd_port, uint8_t event)
{
	bool ret;
	pd_event_t evt = {
		.event_type = PD_EVT_DPM_MSG,
		.msg = PD_DPM_VDM_REQUEST,
		.msg_sec = event,
		.pd_msg = NULL,
	};

	ret = hisi_pd_put_vdm_event(pd_port->tcpc_dev, &evt, false);
	if (ret)
		pd_port->reset_vdm_state = true;

	return ret;
}

bool pd_put_dpm_notify_event(pd_port_t *pd_port, uint8_t notify)
{
	pd_event_t evt = {
		.event_type = PD_EVT_DPM_MSG,
		.msg = PD_DPM_NOTIFIED,
		.msg_sec = notify,
		.pd_msg = NULL,
	};

	return hisi_pd_put_event(pd_port->tcpc_dev, &evt, false);
}

bool pd_put_dpm_ack_event(pd_port_t *pd_port)
{
	pd_event_t evt = {
		.event_type = PD_EVT_DPM_MSG,
		.msg = PD_DPM_ACK,
		.pd_msg = NULL,
	};

	return hisi_pd_put_event(pd_port->tcpc_dev, &evt, false);
}

bool pd_put_dpm_nak_event(pd_port_t *pd_port, uint8_t notify)
{
	pd_event_t evt = {
		.event_type = PD_EVT_DPM_MSG,
		.msg = PD_DPM_NAK,
		.msg_sec = notify,
		.pd_msg = NULL,
	};

	return hisi_pd_put_event(pd_port->tcpc_dev, &evt, false);
}

bool vdm_put_hw_event(
	struct tcpc_device *tcpc_dev, uint8_t hw_event)
{
	pd_event_t evt = {
		.event_type = PD_EVT_HW_MSG,
		.msg = hw_event,
		.pd_msg = NULL,
	};

	return hisi_pd_put_vdm_event(tcpc_dev, &evt, false);
}

bool vdm_put_dpm_event(pd_port_t *pd_port, uint8_t dpm_event, pd_msg_t *pd_msg)
{
	pd_event_t evt = {
		.event_type = PD_EVT_DPM_MSG,
		.msg = dpm_event,
		.pd_msg = pd_msg,
	};

	return hisi_pd_put_vdm_event(pd_port->tcpc_dev, &evt, false);
}

bool vdm_put_dpm_discover_cable_event(pd_port_t *pd_port)
{
	D("+\n");
	pd_port->pe_vdm_state = pd_port->pe_pd_state;
	return vdm_put_dpm_event(pd_port, PD_DPM_DISCOVER_CABLE_ID, NULL);
}

bool pd_put_hw_event(struct tcpc_device *tcpc_dev, uint8_t hw_event)
{
	pd_event_t evt = {
		.event_type = PD_EVT_HW_MSG,
		.msg = hw_event,
		.pd_msg = NULL,
	};

	return hisi_pd_put_event(tcpc_dev, &evt, false);
}

bool pd_put_cc_attached_event(struct tcpc_device *tcpc_dev, uint8_t type)
{
	pd_event_t evt = {
		.event_type = PD_EVT_HW_MSG,
		.msg = PD_HW_CC_ATTACHED,
		.msg_sec = type,
		.pd_msg = NULL,
	};

	return hisi_pd_put_event(tcpc_dev, &evt, false);
}

bool pd_put_cc_re_enable_pd_event(struct tcpc_device *tcpc_dev, uint8_t type)
{
	pd_event_t evt = {
		.event_type = PD_EVT_HW_MSG,
		.msg = PD_HW_CC_RE_ENABLE_PD,
		.msg_sec = type,
		.pd_msg = NULL,
	};

	return hisi_pd_put_event(tcpc_dev, &evt, false);
}

int hisi_pd_send_soft_reset(pd_port_t *pd_port, uint8_t state_machine)
{
	int ret;

	pd_port->state_machine = state_machine;
	D("state_machine %s\n", pd_pe_state_machine_name(pd_port->state_machine));
	hisi_pd_reset_protocol_layer(pd_port);
	pd_notify_tcp_event_buf_reset(pd_port, TCP_DPM_RET_DROP_SENT_SRESET);

	ret = hisi_pd_send_ctrl_msg(pd_port, TCPC_TX_SOP, PD_CTRL_SOFT_RESET);
	return ret;
}

int hisi_pd_send_hard_reset(pd_port_t *pd_port)
{
	int ret;
	struct tcpc_device *tcpc_dev = pd_port->tcpc_dev;

	D("Send HARD Reset\n");

	pd_port->hard_reset_counter++;
	hisi_pd_notify_pe_send_hard_reset(pd_port);

	ret = tcpci_transmit(tcpc_dev, TCPC_TX_HARD_RESET, 0, NULL);

	return ret;
}

int hisi_pd_send_bist_mode2(pd_port_t *pd_port)
{
	int ret;

	pd_notify_tcp_event_buf_reset(pd_port, TCP_DPM_RET_DROP_SENT_SRESET);

#ifdef CONFIG_USB_PD_TRANSMIT_BIST2
	D("BIST_MODE_2\n");
	ret = tcpci_transmit(pd_port->tcpc_dev, TCPC_TX_BIST_MODE_2, 0, NULL);
#else
	ret = tcpci_set_bist_carrier_mode(pd_port->tcpc_dev, 1 << 2); /* 1 << 2: set mode */
#endif
	tcpci_sc_buck_ctrl(pd_port->tcpc_dev, false);

	return ret;
}

int hisi_pd_disable_bist_mode2(pd_port_t *pd_port)
{
	D("\n");
	tcpci_sc_buck_ctrl(pd_port->tcpc_dev, true);
#ifdef CONFIG_USB_PD_TRANSMIT_BIST2
	return 0;
#else
	return tcpci_set_bist_carrier_mode(pd_port->tcpc_dev, 0);
#endif
}

/* ---- Send / Reply VDM Command ----*/
int hisi_pd_send_svdm_request(pd_port_t *pd_port,
		uint8_t sop_type, uint16_t svid, uint8_t vdm_cmd,
		uint8_t obj_pos, uint8_t cnt, uint32_t *data_obj,
		uint32_t timer_id)
{
#ifdef CONFIG_USB_PD_STOP_SEND_VDM_IF_RX_BUSY
	int rv;
	uint32_t alert_status;
#endif
	int ver = 0;
	int ret;
	errno_t ret_cpy;
	uint32_t payload[VDO_MAX_SIZE];

	D("+\n");

	if (hisi_pd_get_rev(pd_port, sop_type) > PD_REV20)
		ver = 1;
	payload[0] = VDO_S(svid, CMDT_INIT, vdm_cmd, obj_pos, ver);
	I("vdm header: 0x%x\n", payload[0]);

	if (data_obj && cnt > 0) {
		ret_cpy = memcpy_s(&payload[1], sizeof(uint32_t) * (VDO_MAX_SIZE - 1),
				data_obj, sizeof(uint32_t) * cnt);
		if (ret_cpy != EOK)
			I("cpy svdm payload fail!\n");
	}

#ifdef CONFIG_USB_PD_STOP_SEND_VDM_IF_RX_BUSY
	rv = tcpci_get_alert_status(pd_port->tcpc_dev, &alert_status);
	if (rv)
		return rv;

	if (alert_status & TCPC_REG_ALERT_RX_STATUS) {
		I("RX Busy, stop send VDM\n");
		return 0;
	}
#endif

	ret = hisi_pd_send_data_msg(pd_port, sop_type,
			PD_DATA_VENDOR_DEF, 1 + cnt, payload);
	if (ret == 0 && timer_id != 0)
		pd_enable_timer(pd_port, timer_id);

	D("-\n");
	return ret;
}

int hisi_pd_reply_svdm_request(pd_port_t *pd_port, pd_event_t *pd_event,
				uint8_t reply, uint8_t cnt, uint32_t *data_obj)
{
	int rv;
	uint32_t vdo;
	int ver = 0;
	uint32_t payload[VDO_MAX_SIZE] = {0};
	errno_t ret;
	struct tcpc_device *tcpc_dev = pd_port->tcpc_dev;

	D("+\n");
	if (!pd_port || !pd_event) {
		I("invalid pd_port or pd_event\n");
		return 0;
	}

	if (hisi_pd_get_rev(pd_port, TCPC_TX_SOP) > PD_REV20)
		ver = 1;
	vdo = pd_event->pd_msg->payload[0];
	payload[0] = VDO_S(PD_VDO_VID(vdo), reply, PD_VDO_CMD(vdo), PD_VDO_OPOS(vdo), ver);
	I("vdm header: 0x%x\n", payload[0]);

	if (data_obj && cnt > 0) {
		ret = memcpy_s(&payload[1], sizeof(uint32_t) * (VDO_MAX_SIZE - 1),
				data_obj, sizeof(uint32_t) * cnt);
		if (ret != EOK)
			I("cpy vdo payload fail!\n");
	}

#ifdef CONFIG_USB_PD_STOP_REPLY_VDM_IF_RX_BUSY
	if (tcpc_dev->ops->check_rx_busy(tcpc_dev)) {
		I("RX Receiving Data\n");
		return 0;
	}

	/*
	 * Ellisys TD.PD.VDMU.E17
	 * Do not send VDM when Rx busy.
	 */
	tcpci_mutex_lock(tcpc_dev);
	if (tcpc_dev->pd_pending_vdm_event) {
		tcpci_mutex_unlock(tcpc_dev);
		I("RX Busy, stop reply VDM\n");
		return 0;
	}
	tcpci_mutex_unlock(tcpc_dev);
#endif /* CONFIG_USB_PD_STOP_REPLY_VDM_IF_RX_BUSY */

	rv = hisi_pd_send_data_msg(pd_port, TCPC_TX_SOP, PD_DATA_VENDOR_DEF, cnt + 1, payload);
	D("-\n");

	return rv;
}

void hisi_pd_lock_msg_output(pd_port_t *pd_port)
{
	if (pd_port->msg_output_lock)
		return;
	pd_port->msg_output_lock = true;
}

void hisi_pd_unlock_msg_output(pd_port_t *pd_port)
{
	if (!pd_port->msg_output_lock)
		return;
	pd_port->msg_output_lock = false;
}

int hisi_pd_update_connect_state(pd_port_t *pd_port, uint8_t state)
{
	int ret;

	D("+\n");
	if (pd_port->pd_connect_state == state)
		return 0;

	switch (state) {
	case HISI_PD_CONNECT_TYPEC_ONLY:
		if (pd_port->power_role == PD_ROLE_SOURCE) {
			state = HISI_PD_CONNECT_TYPEC_ONLY_SRC;
		} else {
			switch (pd_port->tcpc_dev->typec_remote_rp_level) {
			case TYPEC_CC_VOLT_SNK_DFT:
				state = HISI_PD_CONNECT_TYPEC_ONLY_SNK_DFT;
				break;

			case TYPEC_CC_VOLT_SNK_1_5:
			case TYPEC_CC_VOLT_SNK_3_0:
				state = HISI_PD_CONNECT_TYPEC_ONLY_SNK;
				break;
			default:
				break;
			}
		}
		break;

	case HISI_PD_CONNECT_PE_READY:
		state = pd_port->power_role == PD_ROLE_SOURCE ?
			HISI_PD_CONNECT_PE_READY_SRC : HISI_PD_CONNECT_PE_READY_SNK;
		break;

	default:
		break;
	}

	pd_port->pd_connect_state = state;

	/* Call tcpci function, notify other subsystem. */
	ret = tcpci_notify_pd_state(pd_port->tcpc_dev, state);
	D("-\n");
	return ret;
}

#ifdef CONFIG_KIRIN_PD_REV30
void hisi_pd_set_sink_tx(pd_port_t *pd_port, uint8_t cc)
{
#ifdef CONFIG_KIRIN_PD_REV30_COLLISION_AVOID
	struct tcpc_device *tcpc_dev = pd_port->tcpc_dev;
	I("\n");
	if (cc == PD30_SINK_TX_OK &&
		pd_port->pd_traffic_control != PD_SINK_TX_OK) {
		I("sink_tx_ok\n");
		tcpci_set_cc(tcpc_dev, cc);
		pd_port->pd_traffic_control = PD_SINK_TX_OK;
		pd_disable_timer(pd_port, PD_TIMER_SINK_TX);
	} else if (cc == PD30_SINK_TX_NG &&
		pd_port->pd_traffic_control == PD_SINK_TX_OK) {
		I("sink_tx_ng\n");
		tcpci_set_cc(tcpc_dev, cc);
		pd_port->pd_traffic_control = PD_SINK_TX_NG;
		pd_enable_timer(pd_port, PD_TIMER_SINK_TX);
	}
#endif	/* CONFIG_KIRIN_PD_REV30_COLLISION_AVOID */
}

bool hisi_pd_put_sink_tx_event(struct tcpc_device *tcpc_dev, uint8_t cc_res)
{
#ifdef CONFIG_KIRIN_PD_REV30_COLLISION_AVOID
	pd_event_t evt = {
		.event_type = PD_EVT_HW_MSG,
		.msg = PD_HW_SINK_TX_CHANGE,
		.pd_msg = NULL,
	};

	evt.msg_sec = cc_res == TYPEC_CC_VOLT_SNK_3_0;
	return hisi_pd_put_event(tcpc_dev, &evt, false);
#endif	/* CONFIG_KIRIN_PD_REV30_COLLISION_AVOID */

	return true;
}

void hisi_pd_sync_sop_spec_revision(pd_port_t *pd_port, uint8_t rev)
{
	pd_port->remote_rev = rev;

	if (!pd_port->pd_connected) {
		pd_port->pd_revision[0] = min_t(uint8_t, pd_port->pd_revision[0], rev);
		pd_port->pd_revision[1] = min_t(uint8_t, pd_port->pd_revision[1], rev);
		I("pd rev: 0x%x-0x%x\n", pd_port->pd_revision[0], pd_port->pd_revision[1]);
	}
}

void hisi_pd_sync_sop_prime_spec_revision(pd_port_t *pd_port, uint8_t rev)
{
	pd_port->cable_communicated = true;
	pd_port->pd_revision[1] = min_t(uint8_t, pd_port->pd_revision[1], rev);
	I("cable rev: 0x%x\n", pd_port->pd_revision[1]);
}
#endif
