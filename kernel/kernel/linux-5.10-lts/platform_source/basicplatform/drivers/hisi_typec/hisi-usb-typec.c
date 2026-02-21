/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Hisilicon USB Type-C framework.
 * Author: hisilicon
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#include <linux/gpio.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/of_platform.h>
#include <linux/of_gpio.h>
#include <linux/spinlock.h>
#include <linux/notifier.h>
#include <linux/fb.h>
#include <linux/debugfs.h>

#ifdef CONFIG_DP_AUX_SWITCH
#include <huawei_platform/dp_aux_switch/dp_aux_switch.h>
#endif

#ifdef CONFIG_ADAPTER_PROTOCOL_PD
#include <chipset_common/hwpower/protocol/adapter_protocol_pd.h>
#endif

#ifdef CONFIG_ADAPTER_PROTOCOL_UVDM
#include <chipset_common/hwpower/protocol/adapter_protocol_uvdm.h>
#endif

#ifdef CONFIG_HUAWEI_CHARGER_AP
#include <huawei_platform/power/huawei_charger.h>
#endif

#ifdef CONFIG_HUAWEI_DSM
#include <chipset_common/hwpower/common_module/power_dsm.h>
#endif

#ifdef CONFIG_TCPC_CLASS
#include <huawei_platform/usb/hw_pd_dev.h>
#endif

#ifdef CONFIG_HUAWEI_USB
#include <chipset_common/hwusb/hw_usb.h>
#endif

#ifdef CONFIG_DP_AUX_SWITCH
#include <huawei_platform/dp_aux_switch/dp_aux_switch.h>
#endif

#include <chipset_common/hwpower/protocol/adapter_protocol.h>
#include <chipset_common/hwpower/adapter/adapter_detect.h>
#include <chipset_common/hwpower/direct_charge/direct_charge_ic_manager.h>
#include <chipset_common/hwusb/hw_pd/hw_pd_pps_charger.h>

#include <linux/platform_drivers/usb/hifi_usb.h>
#include <linux/platform_drivers/usb/chip_usb.h>
#include <linux/platform_drivers/usb/tca.h>
#include <linux/platform_drivers/usb/hisi_tcpm.h>
#include <linux/platform_drivers/usb/hisi_typec.h>
#include <linux/platform_drivers/usb/hisi_tcpc_ops.h>

#include "hisi-usb-typec.h"
#include "usb-typec.h"

#include <securec.h>

#define ANALOG_SINGLE_CC_STS_MASK 0xF
#define ANALOG_CC2_STS_OFFSET 4
#define ANALOG_CC_WITH_UNDEFINED_RESISTANCE 1

#ifdef CONFIG_KIRIN_TYPEC_DBG
#define D(format, arg...) \
		pr_info("[usb_typec]" format, ##arg)
#else
#define D(format, arg...) do {} while (0)
#endif
#define I(format, arg...) \
		pr_info("[usb_typec]" format, ##arg)
#define E(format, arg...) \
		pr_err("[ERR][usb_typec]" format, ##arg)

struct usb_typec {
	struct platform_device *pdev;
	struct tcpc_device *tcpc_dev;

	struct mutex lock;
	struct tcp_ny_vbus_state vbus_state;
	struct tcp_ny_typec_state typec_state;
	struct tcp_ny_pd_state pd_state;
	struct tcp_ny_ama_dp_hpd_state ama_dp_hpd_state;
	struct tcp_ny_uvdm uvdm_msg;

	uint16_t dock_svid;

	int power_role;
	int data_role;
	int vconn;
	int audio_accessory;

	bool direct_charge_cable;
	bool direct_charging;
	int pd_adapter_voltage;

	uint8_t rt_cc1;
	uint8_t rt_cc2;
	unsigned long time_stamp_cc_alert;
	unsigned long time_stamp_typec_attached;
	unsigned long time_stamp_pd_attached;

	/* for handling tcpc notify */
	struct notifier_block tcpc_nb;
	struct list_head tcpc_notify_list;
	spinlock_t tcpc_notify_list_lock;
	unsigned int tcpc_notify_count;
	struct work_struct tcpc_notify_work;
	struct workqueue_struct *hisi_typec_wq;

	enum tcpc_mux_ctrl_type dp_mux_type;

	struct dentry *debugfs_root;

	struct notifier_block wakelock_control_nb;

	bool check_hard_reset_state;
#ifdef CONFIG_HISI_ADAPTER_DETECT
	bool support_adapter_detect;
#endif
};

static struct usb_typec *_typec;

#ifdef CONFIG_DFX_DEBUG_FS
static char *remote_rp_level_string(unsigned char rp_level)
{
	switch (rp_level) {
	case TYPEC_CC_VOLT_OPEN: return "open";
	case TYPEC_CC_VOLT_RA: return "Ra";
	case TYPEC_CC_VOLT_RD: return "Rd";
	case TYPEC_CC_VOLT_SNK_DFT: return "Rp_Default";
	case TYPEC_CC_VOLT_SNK_1_5: return "Rp_1.5A";
	case TYPEC_CC_VOLT_SNK_3_0: return "Rp_3.0A";
	default: return "illegal value";
	}
}

static int usb_typec_status_show(struct seq_file *s, void *data)
{
	struct usb_typec *typec = s->private;

	if (!typec)
		return -ENOENT;

	seq_printf(s, "power_role             %s\n",
		   (typec->power_role == PD_ROLE_SOURCE) ? "SOURCE" :
		   (typec->power_role == PD_ROLE_SINK) ? "PD_SINK" : "SINK");
	seq_printf(s, "data_role              %s\n",
		   (typec->data_role == PD_ROLE_UFP) ? "UFP" : "DFP");
	seq_printf(s, "vconn                  %s\n",
		    typec->vconn ? "Y" : "N");
	seq_printf(s, "audio_accessory        %s\n",
		   typec->audio_accessory ? "Y" : "N");

	seq_printf(s, "orient                 %s\n",
		   typec->typec_state.polarity ? "fliped" : "normal");

	seq_printf(s, "tcpc_notify_count      %u\n", typec->tcpc_notify_count);
	seq_printf(s, "dp_mux_type            %u\n", typec->dp_mux_type);

	seq_printf(s, "direct_charge_cable    %s\n",
		   typec->direct_charge_cable ? "Y" : "N");
	seq_printf(s, "direct_charging        %s\n",
		   typec->direct_charging ? "Y" : "N");

	seq_printf(s, "remote rp level        %s\n",
		   remote_rp_level_string(typec->typec_state.rp_level));
	seq_printf(s, "dock_svid              0x%04x\n", typec->dock_svid);
	seq_printf(s, "pd_adapter_voltage     %d\n", typec->pd_adapter_voltage);

	seq_printf(s, "vbus.ma                %d\n", typec->vbus_state.ma);
	seq_printf(s, "vbus.mv                %d\n", typec->vbus_state.mv);
	seq_printf(s, "vbus.type              0x%x\n", typec->vbus_state.type);

	return 0;
}

static int usb_typec_status_open(struct inode *inode, struct file *file)
{
	return single_open(file, usb_typec_status_show, inode->i_private);
}

static const struct file_operations usb_typec_status_fops = {
	.open			= usb_typec_status_open,
	.read			= seq_read,
	.llseek			= seq_lseek,
	.release		= single_release,
};

static int usb_typec_perf_show(struct seq_file *s, void *data)
{
	struct usb_typec *typec = s->private;

	if (!typec)
		return -ENOENT;

	if (typec->data_role == PD_ROLE_DFP) {
		seq_printf(s, "attached.source     %u ms\n",
			jiffies_to_msecs(typec->time_stamp_typec_attached
					- typec->time_stamp_cc_alert));
	} else if (typec->data_role == PD_ROLE_UFP) {
		seq_printf(s, "attached.sink       %u ms\n",
			jiffies_to_msecs(typec->time_stamp_typec_attached
					- typec->time_stamp_cc_alert));
	}

	if (typec->pd_state.connected != HISI_PD_CONNECT_NONE)
		seq_printf(s, "pd connect(%u)      %u ms\n",
			typec->pd_state.connected,
			jiffies_to_msecs(typec->time_stamp_pd_attached
					- typec->time_stamp_cc_alert));

	return 0;
}

static int usb_typec_perf_open(struct inode *inode, struct file *file)
{
	return single_open(file, usb_typec_perf_show, inode->i_private);
}

static const struct file_operations usb_typec_perf_fops = {
	.open			= usb_typec_perf_open,
	.read			= seq_read,
	.llseek			= seq_lseek,
	.release		= single_release,
};

static int usb_typec_cable_show(struct seq_file *s, void *data)
{
#define PD_VDO_MAX_SIZE 7

	struct usb_typec *typec = s->private;
	uint32_t vdos[PD_VDO_MAX_SIZE] = {0};
	int i;
#ifdef CONFIG_KIRIN_POWER_DELIVERY_SUPPORT
	int ret;
#endif

	if (!typec)
		return -ENOENT;

#ifdef CONFIG_KIRIN_POWER_DELIVERY_SUPPORT
	ret = hisi_tcpm_discover_cable(typec->tcpc_dev, vdos, PD_VDO_MAX_SIZE);
	if (ret != TCPM_SUCCESS) {
		E("hisi_tcpm_discover_cable error ret %d\n", ret);
		return 0;
	}
#endif

	for (i = 0; i < PD_VDO_MAX_SIZE; i++)
		seq_printf(s, "vdo[%d] 0x%08x\n", i, vdos[i]);

	return 0;
}

static int usb_typec_cable_open(struct inode *inode, struct file *file)
{
	return single_open(file, usb_typec_cable_show, inode->i_private);
}

static const struct file_operations usb_typec_cable_fops = {
	.open = usb_typec_cable_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int usb_typec_debugfs_init(struct usb_typec *typec)
{
	struct dentry *root = NULL;
	struct dentry *file = NULL;

	root = debugfs_create_dir("hisi_usb_typec", usb_debug_root);
	if (IS_ERR_OR_NULL(root))
		return -ENOMEM;

	file = debugfs_create_file("status", S_IRUGO, root,
				typec, &usb_typec_status_fops);
	if (!file)
		goto err;

	file = debugfs_create_file("perf", S_IRUGO, root,
				typec, &usb_typec_perf_fops);
	if (!file)
		goto err;

	file = debugfs_create_file("cable", S_IRUGO, root,
				typec, &usb_typec_cable_fops);
	if (!file)
		goto err;

	typec->debugfs_root = root;
	return 0;

err:
	E("typec debugfs init failed\n");
	debugfs_remove_recursive(root);
	return -ENOMEM;
}

static void usb_typec_debugfs_exit(struct usb_typec *typec)
{
	if (!typec->debugfs_root)
		return;

	debugfs_remove_recursive(typec->debugfs_root);
}
#else
static inline int usb_typec_debugfs_init(struct usb_typec *typec) { return 0; }
static inline void usb_typec_debugfs_exit(struct usb_typec *typec) {}
#endif

int hisi_usb_typec_otg_pwr_src(void)
{
	return pd_dpm_get_otg_channel();
}

#ifdef CONFIG_ADAPTER_PROTOCOL
static bool adapter_type_qtr_typec(int adapter_type)
{
	if (adapter_type == ADAPTER_TYPE_QTR_C_20V3A ||
			adapter_type == ADAPTER_TYPE_QTR_C_10V4A)
		return true;

	return false;
}
#endif

bool hisi_usb_typec_force_vconn(void)
{
#ifdef CONFIG_ADAPTER_PROTOCOL
	int adapter_type = ADAPTER_TYPE_UNKNOWN;
	int ret;

	if (hisi_tcpm_support_snk_detect_emark(_typec->tcpc_dev))
		return false;

	if (adapter_detect_get_runtime_protocol_type() == BIT(ADAPTER_PROTOCOL_UFCS)) {
		D("not qtr adapter\n");
		return false;
	}

	ret = adapter_get_adp_type(ADAPTER_PROTOCOL_SCP, &adapter_type);
	if (ret) {
		D("get adapter type failed\n");
		return false;
	}

	if (adapter_type_qtr_typec(adapter_type)) {
		D("Force Vconn\n");
		return true;
	}
#endif
	return false;
}

#ifdef CONFIG_REVERSE_CHARGE
bool hisi_usb_get_rev_sc_dev_flag(void)
{
	struct usb_typec *typec = _typec;

	if (!typec)
		return false;
	
	return hisi_usb_get_pd_neg_event(typec->tcpc_dev);
}
 
void hisi_usb_set_rev_sc_dev_flag(bool flag)
{
	struct usb_typec *typec = _typec;

	if (!typec)
		return;

	hisi_usb_set_pd_neg_event(typec->tcpc_dev, flag);
}

bool hisi_usb_first_attach_rscp(void)
{
	struct usb_typec *typec = _typec;

	if (!typec)
		return false;

	return get_first_attach(typec->tcpc_dev);
}
#endif

/*
 * Turn on/off vconn power.
 * enable:0 - off, 1 - on
 */
void hisi_usb_typec_set_vconn(int enable)
{
	struct usb_typec *typec = _typec;

	I(" Vconn enable: %d\n", enable);
	typec->vconn = enable;

	pd_dpm_handle_pe_event(PD_DPM_PE_EVT_SOURCE_VCONN, &enable);
}

static bool hisi_usb_typec_ready(struct usb_typec *typec)
{
	if (!typec || !typec->tcpc_dev) {
		E("hisi usb tcpc not ready\n");
		return false;
	}

	return true;
}

static void handle_typec_unattached(struct usb_typec *typec,
		struct tcp_ny_typec_state *typec_state)
{
	if (typec->audio_accessory)
		typec->audio_accessory = 0;

	if (typec->data_role == PD_ROLE_UFP)
		typec->data_role = PD_ROLE_UNATTACHED;
	else if (typec->data_role == PD_ROLE_DFP)
		typec->data_role = PD_ROLE_UNATTACHED;

	typec->power_role = PD_ROLE_UNATTACHED;

	typec->direct_charge_cable = false;
	typec->direct_charging = false;
}

static void handle_typec_attached_sink(struct usb_typec *typec,
		struct tcp_ny_typec_state *typec_state)
{
	if (typec->data_role == PD_ROLE_UFP) {
		D("Already UFP\n");
		return;
	}

	typec->data_role = PD_ROLE_UFP;
}

static void handle_typec_attached_source(struct usb_typec *typec,
		struct tcp_ny_typec_state *typec_state)
{
	if (typec->data_role == PD_ROLE_DFP) {
		D("Already DFP\n");
		return;
	}

	typec->data_role = PD_ROLE_DFP;
}

static inline void handle_typec_attached_audio_accessory(
		struct usb_typec *typec,
		struct tcp_ny_typec_state *typec_state)
{
	typec->audio_accessory = true;
}

static inline void handle_typec_attached_debug_accessory(
		struct usb_typec *typec,
		struct tcp_ny_typec_state *typec_state)
{
}

static inline void handle_typec_attached_debug_accessory_sink(
		struct usb_typec *typec,
		struct tcp_ny_typec_state *typec_state)
{
	typec->data_role = PD_ROLE_UFP;
}

/*
 * Handle typec state change event
 */
static void hisi_usb_typec_state_change(struct tcp_ny_typec_state *typec_state)
{
	struct usb_typec *typec = _typec;
	int ret;

#ifdef CONFIG_REVERSE_CHARGE
	if (typec_state->new_state == TYPEC_UNATTACHED) {
		if (!timer_called_by_timeout(typec->tcpc_dev))
			set_rchg_unattach_timer(typec->tcpc_dev);

		if (get_state_path(typec->tcpc_dev) == 0)
			clear_rchg_rever_flag(typec->tcpc_dev);
	}
#endif

	mutex_lock(&typec->lock);
	I("typec_state: %s --> %s / %s / %s\n",
		typec_attach_type_name(typec_state->old_state),
		typec_attach_type_name(typec_state->new_state),
		typec_state->polarity ? "fliped" : "normal",
		tcpm_cc_voltage_status_string(typec_state->rp_level));

	ret = memcpy_s(&typec->typec_state, sizeof(struct tcp_ny_typec_state),
		typec_state, sizeof(typec->typec_state));
	if (ret != EOK)
		E("memcpy_s failed\n");

	switch (typec_state->new_state) {
	case TYPEC_UNATTACHED:
		handle_typec_unattached(typec, typec_state);
		break;
	case TYPEC_ATTACHED_SNK:
		handle_typec_attached_sink(typec, typec_state);
		break;
	case TYPEC_ATTACHED_SRC:
		handle_typec_attached_source(typec, typec_state);
		break;
	case TYPEC_ATTACHED_AUDIO:
		handle_typec_attached_audio_accessory(typec, typec_state);
		break;
	case TYPEC_ATTACHED_DEBUG:
		handle_typec_attached_debug_accessory(typec, typec_state);
		break;
	case TYPEC_ATTACHED_DBGACC_SNK:
	case TYPEC_ATTACHED_CUSTOM_SRC:
		handle_typec_attached_debug_accessory_sink(typec, typec_state);
		break;
	case TYPEC_ATTACHED_VBUS_ONLY:
	case TYPEC_DETTACHED_VBUS_ONLY:
		break;
	default:
		E("unknown new_sate %u\n", typec_state->new_state);
		break;
	}

	typec->time_stamp_typec_attached = jiffies;

	mutex_unlock(&typec->lock);
}

#ifdef CONFIG_REVERSE_CHARGE
void hisi_usb_set_rev_sc_switch_flag(int rever_flag)
{
	struct usb_typec *typec = _typec;

	if(!typec)
		return;

	tcpc_drive_path_prop(rever_flag, typec->tcpc_dev);
}
 
int hisi_usb_is_reverse_charge_enable(void)
{
	struct usb_typec *typec = _typec;

	if(!typec)
		return 0;

	return get_reverse_charge_enable(typec->tcpc_dev) == true ? 1 : 0;
}
/*
 * Used only for reverse super charging feature
 * Provide charge to handle unplugging
 */
bool hisi_usb_reverse_charge_unattach_state(void)
{
	struct usb_typec *typec = _typec;

	if(!typec)
		return false;

	D("send charge pd unattach state %d\n", get_pd_unattach_state(typec->tcpc_dev));
	return get_pd_unattach_state(typec->tcpc_dev);
}
#endif

/*
 * Handle PD state change, save PD state
 * pd_state:Should be one of hisi_pd_connect_result.
 */
static void hisi_usb_typec_pd_state_change(struct tcp_ny_pd_state *pd_state)
{
	struct usb_typec *typec = _typec;

	mutex_lock(&typec->lock);
	typec->pd_state.connected = pd_state->connected;
	typec->time_stamp_pd_attached = jiffies;
	mutex_unlock(&typec->lock);
}

/*
 * Handle data role swap event
 */
static void hisi_usb_typec_data_role_swap(u8 role)
{
	struct usb_typec *typec = _typec;

	mutex_lock(&typec->lock);

	if (typec->data_role == PD_ROLE_UNATTACHED) {
		E("Unattached!\n");
		goto done;
	}

	if (typec->data_role == role) {
		D("Data role not change!\n");
		goto done;
	}

	if (role == PD_ROLE_DFP)
		typec->data_role = PD_ROLE_DFP;
	else
		typec->data_role = PD_ROLE_UFP;
done:
	mutex_unlock(&typec->lock);
}

/*
  * Handle source vbus operation
 */
static void hisi_usb_typec_source_vbus(struct tcp_ny_vbus_state *vbus_state)
{
	struct usb_typec *typec = _typec;
	int ret;

	mutex_lock(&typec->lock);

	/* Must save vbus_state first */
	ret = memcpy_s(&typec->vbus_state, sizeof(struct tcp_ny_vbus_state),
		vbus_state, sizeof(typec->vbus_state));
	if (ret != EOK)
		E("memcpy_s failed\n");

	if (vbus_state->mv != 0)
		typec->power_role = PD_ROLE_SOURCE;

	mutex_unlock(&typec->lock);
}

bool hisi_usb_typec_sourcing_vbus(void)
{
	struct usb_typec *typec = _typec;

	if (!typec)
		return false;

	if (typec->power_role == PD_ROLE_SOURCE)
		return typec->vbus_state.mv != 0;
	else
		return false;
}

/*
 * Handle sink vbus operation
 */
static void hisi_usb_typec_sink_vbus(struct tcp_ny_vbus_state *vbus_state)
{
	struct usb_typec *typec = _typec;

	mutex_lock(&typec->lock);

	/* save vbus_state. */
	typec->vbus_state.ma = vbus_state->ma;
	typec->vbus_state.mv = vbus_state->mv;
	typec->vbus_state.type = vbus_state->type;

	if (vbus_state->mv != 0)
		typec->power_role = PD_ROLE_SINK;

	mutex_unlock(&typec->lock);
}

bool hisi_usb_typec_during_hardreset(void)
{
	if (!_typec)
		return false;

	if (!_typec->check_hard_reset_state)
		return false;

#ifdef CONFIG_KIRIN_POWER_DELIVERY_SUPPORT
	return hisi_tcpm_during_hardreset(_typec->tcpc_dev);
#else
	return false;
#endif
}

#ifdef CONFIG_HISI_ADAPTER_DETECT
bool hisi_usb_typec_support_adapter_detect(void)
{
	if (!_typec)
		return false;

	if (_typec->support_adapter_detect)
		return true;

	return false;
}
#endif

static void hisi_usb_typec_disable_vbus_control(
		struct tcp_ny_vbus_state *vbus_state)
{
	struct usb_typec *typec = _typec;

	mutex_lock(&typec->lock);

	typec->vbus_state.ma = vbus_state->ma;
	typec->vbus_state.mv = vbus_state->mv;
	typec->vbus_state.type = vbus_state->type;

	typec->power_role = PD_ROLE_UNATTACHED;
	mutex_unlock(&typec->lock);
}

static int hisi_usb_pd_dp_state_notify(uint32_t irq_type, uint32_t mode_type,
		uint32_t dev_type, uint32_t typec_orien)
{
	struct pd_dpm_combphy_event event;

	event.irq_type = irq_type;
	event.mode_type = mode_type;
	event.dev_type = dev_type;
	event.typec_orien = typec_orien;
	return pd_dpm_handle_combphy_event(event);
}

static void hisi_usb_set_last_hpd_status(struct usb_typec *typec, bool status)
{
	pd_dpm_set_last_hpd_status(status);
}

void hisi_usb_pd_dp_state_change(struct tcp_ny_ama_dp_state *ama_dp_state)
{
	struct usb_typec *typec = _typec;
	int ret;

#ifdef CONFIG_DP_AUX_SWITCH
	/* add aux switch */
	dp_aux_switch_op(ama_dp_state->polarity);

	/* add aux uart switch */
	dp_aux_uart_switch_enable();
#endif

#ifndef MODE_DP_PIN_A
#define MODE_DP_PIN_A 0x01
#define MODE_DP_PIN_B 0x02
#define MODE_DP_PIN_C 0x04
#define MODE_DP_PIN_D 0x08
#define MODE_DP_PIN_E 0x10
#define MODE_DP_PIN_F 0x20
#else
#error
#endif

	mutex_lock(&typec->lock);

	if ((MODE_DP_PIN_C == ama_dp_state->pin_assignment)
			|| (MODE_DP_PIN_E == ama_dp_state->pin_assignment))
		typec->dp_mux_type = TCPC_DP;
	else if ((MODE_DP_PIN_D == ama_dp_state->pin_assignment)
			|| (MODE_DP_PIN_F == ama_dp_state->pin_assignment))
		typec->dp_mux_type = TCPC_USB31_AND_DP_2LINE;

	mutex_unlock(&typec->lock);

	ret = hisi_usb_pd_dp_state_notify(TCA_IRQ_HPD_OUT, TCPC_NC,
			TCA_ID_RISE_EVENT, ama_dp_state->polarity);
	if (ret)
		E("RISE EVENT ret %d\n", ret);

	pd_dpm_set_combphy_status(typec->dp_mux_type);

	ret = hisi_usb_pd_dp_state_notify(TCA_IRQ_HPD_IN, typec->dp_mux_type,
			TCA_ID_FALL_EVENT, ama_dp_state->polarity);
	if (ret)
		E("FALL_EVENT ret %d\n", ret);
}

void hisi_usb_pd_dp_hpd_state_change(
		struct tcp_ny_ama_dp_hpd_state *ama_dp_hpd_state)
{
	struct usb_typec *typec = _typec;
	int ret;

	I("irq %u, state %u\n", ama_dp_hpd_state->irq, ama_dp_hpd_state->state);
	mutex_lock(&typec->lock);
	typec->ama_dp_hpd_state.irq = ama_dp_hpd_state->irq;
	typec->ama_dp_hpd_state.state = ama_dp_hpd_state->state;
	mutex_unlock(&typec->lock);

	if (!ama_dp_hpd_state->state) {
		ret = hisi_usb_pd_dp_state_notify(TCA_IRQ_HPD_OUT, typec->dp_mux_type,
			TCA_DP_OUT, typec->typec_state.polarity);
		if (ret)
			E("DP_OUT ret %d\n", ret);

		hisi_usb_set_last_hpd_status(typec, false);
#ifdef CONFIG_HUAWEI_USB
		hw_usb_send_event(DP_CABLE_OUT_EVENT);
#endif
	} else {
		ret = hisi_usb_pd_dp_state_notify(TCA_IRQ_HPD_IN, typec->dp_mux_type,
			TCA_DP_IN, typec->typec_state.polarity);
		if (ret)
			E("TCA_DP_IN ret %d\n", ret);

		hisi_usb_set_last_hpd_status(typec, true);
#ifdef CONFIG_HUAWEI_USB
		hw_usb_send_event(DP_CABLE_IN_EVENT);
#endif
	}

	if (ama_dp_hpd_state->irq) {
		ret = hisi_usb_pd_dp_state_notify(TCA_IRQ_SHORT, typec->dp_mux_type,
			TCA_DP_IN, typec->typec_state.polarity);
		if (ret)
			E("IRQ_SHORT ret %d\n", ret);
	}
}

void hisi_usb_pd_ufp_update_dock_svid(uint16_t svid)
{
	struct usb_typec *typec = _typec;

	typec->dock_svid = svid;
}

/* Monitor cc status which pass through CCDebounce */
void hisi_usb_typec_cc_status_change(uint8_t cc1, uint8_t cc2)
{
#ifdef CONFIG_TCPC_CLASS
	pd_dpm_handle_pe_event(PD_DPM_PE_ABNORMAL_CC_CHANGE_HANDLER, NULL);
#endif
}

/*
 * transfer pid/vid to hw_pd
 * payload[0]: reserved
 * payload[1]: id header vdo
 * payload[2]: reserved
 * payload[3]: product vdo
 */
void hisi_usb_typec_dfp_inform_id(uint32_t *payload, uint32_t size)
{
	uint32_t bcd = 0;
	uint32_t vid = 0;
	uint32_t pid = 0;

	if (payload && size >= 4) { /* 4: bcd or vid or pid */
		bcd = *(payload + 3) & PD_DPM_HW_PDO_MASK; /* 3: bcd */
		vid = *(payload + 1) & PD_DPM_HW_PDO_MASK; /* 1: vid */
		pid = (*(payload + 3) >> PD_DPM_PDT_VID_OFFSET) & /* 3: pid */
			PD_DPM_HW_PDO_MASK;
	}

	pd_set_product_id_info(vid, pid, bcd);
}

/*
 * Monitor the raw cc status
 */
void hisi_usb_typec_cc_alert(uint8_t cc1, uint8_t cc2)
{
	struct usb_typec *typec = _typec;

	/* for huawei type-c headset, wakeup AP. */
	check_hifi_usb_status(HIFI_USB_TCPC);

	/* only record the time of first cc connection. */
	if ((typec->rt_cc1 == TYPEC_CC_VOLT_OPEN) &&
			(typec->rt_cc2 == TYPEC_CC_VOLT_OPEN)) {
		if ((cc1 != TYPEC_CC_VOLT_OPEN) ||
			(cc2 != TYPEC_CC_VOLT_OPEN)) {
			D("update time_stamp_cc_alert\n");
			typec->time_stamp_cc_alert = jiffies;
		}
	}

	/* record real cc status */
	typec->rt_cc1 = cc1;
	typec->rt_cc2 = cc2;
}

bool hisi_usb_typec_charger_type_pd(void)
{
	return pd_dpm_get_pd_finish_flag();
}

#define TYPEC_DSM_BUF_SIZE_256	256
void hisi_usb_typec_cc_ovp_dmd_report(void)
{
	int ret;
	char msg_buf[TYPEC_DSM_BUF_SIZE_256] = { 0 };

	ret = snprintf_s(msg_buf, TYPEC_DSM_BUF_SIZE_256,
			TYPEC_DSM_BUF_SIZE_256 - 1,
			"%s\n",
			"vbus ovp happened");
	if (ret < 0)
		E("Fill cc ovp dmd msg\n");

#ifdef CONFIG_HUAWEI_DSM
	power_dsm_report_dmd(POWER_DSM_BATTERY,
			POWER_DSM_ERROR_NO_TYPEC_CC_OVP, (void *)msg_buf);
#endif
}

struct tcpc_notify {
	struct list_head node;
	struct tcp_notify notify;
	unsigned long tcpc_notify_type;
};

#define TCPC_NOTIFY_MAX_COUNT 4096
static int queue_notify(struct usb_typec *typec, unsigned long action,
		const void *data)
{
	struct tcpc_notify *noti = NULL;

	if (typec->tcpc_notify_count > TCPC_NOTIFY_MAX_COUNT) {
		E("tcpc_notify_list too long, %u\n", typec->tcpc_notify_count);
		return -EBUSY;
	}

	noti = kzalloc(sizeof(*noti), GFP_KERNEL);
	if (!noti) {
		E("No memory!\n");
		return -ENOMEM;
	}

	noti->tcpc_notify_type = action;
	if (memcpy_s(&noti->notify, sizeof(struct tcp_notify),
			data, sizeof(noti->notify)) != EOK)
		E("memcpy_s failed\n");

	spin_lock(&typec->tcpc_notify_list_lock);
	list_add_tail(&noti->node, &typec->tcpc_notify_list);
	typec->tcpc_notify_count++;
	spin_unlock(&typec->tcpc_notify_list_lock);

	return 0;
}

static struct tcpc_notify *get_notify(struct usb_typec *typec)
{
	struct tcpc_notify *noti = NULL;

	spin_lock(&typec->tcpc_notify_list_lock);
	noti = list_first_entry_or_null(&typec->tcpc_notify_list,
				struct tcpc_notify, node);

	if (noti) {
		list_del_init(&noti->node);
		typec->tcpc_notify_count--;
	}
	spin_unlock(&typec->tcpc_notify_list_lock);

	return noti;
}

static void free_notify(struct tcpc_notify *noti)
{
	kfree(noti);
}

#ifdef CONFIG_KIRIN_PD_REV30
static bool hisi_typec_adapter_support_pd(void)
{
	struct usb_typec *typec = _typec;

	if (!hisi_usb_typec_ready(typec))
		return false;

	return hisi_tcpm_support_pd(typec->tcpc_dev);
}

static bool hisi_typec_adapter_support_pps(void)
{
	struct usb_typec *typec = _typec;

	if (!hisi_typec_adapter_support_pd())
		return false;

	return hisi_tcpm_support_apdo(typec->tcpc_dev);
}

static int hisi_typec_set_pdo(void *client, enum pps_set_state type, int mv, int ma)
{
	int ret = TCPM_SUCCESS;
	struct usb_typec *typec = _typec;

	if (!hisi_typec_adapter_support_pd()) {
		D("Not Support PDO");
		return 0;
	}

	switch (type) {
	case PPS_SET_STATE_START:
		ret = tcpm_set_apdo_charging_policy(typec->tcpc_dev,
			DPM_CHARGING_POLICY_PPS, mv, ma, NULL);
		D("PPS_SET_STATE_START ret %d mv %d ma %d\n", ret, mv, ma);
		break;

	case PPS_SET_STATE_STOP:
		ret = tcpm_set_pd_charging_policy(typec->tcpc_dev,
			DPM_CHARGING_POLICY_VSAFE5V, NULL);
		D("PPS_SET_STATE_STOP ret %d\n", ret);
		break;

	case PPS_SET_STATE_ADJUST:
		ret = tcpm_adjust_apdo_charging_policy(typec->tcpc_dev,
			mv, ma, NULL);
		D("PPS_SET_STATE_ADJUST ret %d mv %d ma %d\n", ret, mv, ma);
		break;

	default:
		D("invalid pps request\n");
	}

	return ret;
}

static int hisi_typec_get_apdo_cap(struct tcpc_device *tcpc,
		enum adapter_cap_type type, struct adapter_power_cap *cap)
{
	struct tcpm_power_cap_val apdo_cap = {0};
	uint8_t cap_i = 0;
	int ret;
	int idx = 0;

	if (!hisi_typec_adapter_support_pps()) {
		D("Not Support APDO\n");
		return 0;
	}

	while (1) {
		ret = tcpm_inquire_pd_source_apdo(tcpc,
				TCPM_POWER_CAP_APDO_TYPE_PPS,
				&cap_i, &apdo_cap);
		if (ret != TCPM_SUCCESS) {
			D("End %d\n", ret);
			break;
		}

		cap->pwr_limit[idx] = apdo_cap.pwr_limit;
		cap->ma[idx] = apdo_cap.ma;
		cap->max_mv[idx] = apdo_cap.max_mv;
		cap->min_mv[idx] = apdo_cap.min_mv;
		cap->maxwatt[idx] = apdo_cap.max_mv * apdo_cap.ma;
		cap->minwatt[idx] = apdo_cap.min_mv * apdo_cap.ma;
		cap->type[idx] = HISI_PD_APDO;

		idx++;
		if (idx >= ADAPTER_CAP_MAX_NR) {
			D("CAP NR > %d\n", ADAPTER_CAP_MAX_NR);
			break;
		}
	}
	cap->nr = idx;

#ifdef CONFIG_KIRIN_TYPEC_DBG
	for (idx = 0; idx < cap->nr; idx++) {
		D("pps_cap[%d:%u], %u mv ~ %u mv, %u ma pl:%u pdp:%u\n",
			idx, cap->nr,
			cap->min_mv[idx], cap->max_mv[idx],
			cap->ma[idx],
			cap->pwr_limit[idx], cap->pdp);
	}

	if (cap_i == 0)
		I("no APDO for pps\n");
#endif
	return 0;
}

static int hisi_typec_get_pd_cap(struct tcpc_device *tcpc,
		enum adapter_cap_type type, struct adapter_power_cap *cap)
{
	struct tcpm_remote_power_cap pd_cap = {0};
	int i;

	if (!hisi_typec_adapter_support_pd()) {
		D("Not Support PD\n");
		return 0;
	}

	pd_cap.nr = 0;
	pd_cap.selected_cap_idx = 0;
	tcpm_get_remote_power_cap(tcpc, &pd_cap);

	if (pd_cap.nr != 0) {
		cap->nr = pd_cap.nr;
		cap->selected_cap_idx = pd_cap.selected_cap_idx - 1;

		for (i = 0; i < pd_cap.nr; i++) {
			cap->ma[i] = pd_cap.ma[i];
			cap->max_mv[i] = pd_cap.max_mv[i];
			cap->min_mv[i] = pd_cap.min_mv[i];
			cap->maxwatt[i] =
				cap->max_mv[i] * cap->ma[i];
			if (pd_cap.type[i] == 0)
				cap->type[i] = HISI_PD;
			else if (pd_cap.type[i] == 3)
				cap->type[i] = HISI_PD_APDO;
			else
				cap->type[i] = HISI_CAP_TYPE_UNKNOWN;
			cap->type[i] = pd_cap.type[i];

			D("cap[%d], mv:[%u,%u] ma:%d watt:[%u %u] type:%u\n",
				i,
				cap->min_mv[i], cap->max_mv[i],
				cap->ma[i],
				cap->minwatt[i], cap->maxwatt[i],
				cap->type[i]);
		}
	}

	return 0;
}

static int hisi_typec_get_adapter_cap(enum adapter_cap_type type,
		struct adapter_power_cap *cap)
{
	struct usb_typec *typec = _typec;
	int ret = 0;

	if (!cap)
		return ret;

	if (type == HISI_PD_APDO)
		ret = hisi_typec_get_apdo_cap(typec->tcpc_dev, type, cap);
	else if (type == HISI_PD)
		ret = hisi_typec_get_pd_cap(typec->tcpc_dev, type, cap);
	else
		D("Not Support\n");

	return ret;
}

static int hisi_typec_set_pps_start(int mv, int ma)
{
	return hisi_typec_set_pdo(NULL, HISI_PD_APDO_START, mv, ma);
}

static int hisi_typec_set_pps_end(void)
{
	return hisi_typec_set_pdo(NULL, HISI_PD_APDO_END, 0, 0);
}

static int hisi_typec_get_apdo_caps(void *client,
	struct pps_apdo_cap *apdo_caps, int num, struct pps_pdo_raw *pdo_cap)
{
	int ret = TCPM_SUCCESS;
	struct adapter_power_cap cap = {0};
	uint8_t i;

	if (!apdo_caps || !pdo_cap || num < 0 || (num > ADAPTER_CAP_MAX_NR) )
		return -EINVAL;

	/* PDO */
	ret = hisi_typec_get_adapter_cap(HISI_PD, &cap);
	if (ret) {
		E("Fail to get PDO\n");
		return ret;
	}

	/* check */
	for (i = 0; i < cap.nr; i++)
		pdo_cap->data[i] = cap.max_mv[i];

	/* APDO */
	ret = hisi_typec_get_adapter_cap(HISI_PD_APDO, &cap);
	if (ret) {
		E("Fail to get APDO\n");
		return ret;
	}

	if (num > ADAPTER_CAP_MAX_NR)
		num = ADAPTER_CAP_MAX_NR;

	num = num > cap.nr ?  cap.nr : num;
	for (i = 0; i < num; i++) {
		apdo_caps[i].vol_min = cap.min_mv[i];
		apdo_caps[i].vol_max = cap.max_mv[i];
		apdo_caps[i].cur = cap.ma[i];
		ret++;
	}

	return ret;
}

static int hisi_typec_get_max_current(void)
{
	struct adapter_power_cap cap = {0};
	uint8_t i;
	int ret;
	int cur;

	ret = hisi_typec_get_adapter_cap(HISI_PD_APDO, &cap);
	cur = cap.ma[0];

	for (i = 0; i < cap.nr; i++) {
		if (cur < cap.ma[i])
			cur = cap.ma[i];
	}

	return cur;
}

static int hisi_typec_get_temp(void)
{
	struct usb_typec *typec = _typec;
	int ret;
	struct pd_status status = {0};

	if (!hisi_typec_adapter_support_pd())
		return 0;

	ret = tcpm_dpm_pd_get_status(typec->tcpc_dev, NULL, &status);
	if (ret) {
		E("get status fail\n");
		return ret;
	}

	return status.internal_temp;
}

static int hisi_typec_get_pps_adapter_output(void *client, int *mv, int *ma)
{
	struct usb_typec *typec = _typec;
	int ret;
	struct pd_pps_status status = {0};

	if (!mv || !ma || !hisi_typec_adapter_support_pps()) {
		D("Adaptor not support APDO\n");
		return 0;
	}
	ret = tcpm_dpm_pd_get_pps_status(typec->tcpc_dev, NULL, &status);
	if (ret) {
		*mv = 0;
		*ma = 0;
		E("get pps status fail, get from ic\n");
	}

	if (status.output_mv <= 0 || status.output_ma <= 0){
		if (dcm_get_ic_vusb(SC_MODE, CHARGE_MULTI_IC, mv)||
			dcm_get_ic_ibus(SC_MODE, CHARGE_MULTI_IC, ma)) {
			*mv = 0;
			*ma = 0;
			E("get ic statu fail\n");
			ret = 1;
		} else {
			ret = 0;
		}
	}
	else
	{
		*mv = status.output_mv;
		*ma = status.output_ma;
	}

	return ret;
}

static void hisi_typec_receive_src_alert(uint32_t ado)
{
	uint32_t alert_type = TCPM_ADO_ALERT_TYPE(ado);

	I("Get alert: 0x%x\n", ado);

	if (alert_type & TCPM_ADO_ALERT_OCP)
		D("Alert OCP\n");

	if (alert_type & TCPM_ADO_ALERT_OVP)
		D("Alert OVP\n");

	if (alert_type & TCPM_ADO_ALERT_OTP)
		D("Alert OTP\n");

	return;
}
#endif

static int hisi_usb_typec_update_wake_lock(bool wake_lock_en)
{
	struct usb_typec *typec = _typec;

	if (!hisi_usb_typec_ready(typec)) {
		E("hisi usb typec is not ready\n");
		return -EPERM;
	}

	return hisi_tcpm_typec_set_wake_lock(typec->tcpc_dev, wake_lock_en);
}

static int hisi_typec_wake_lock_call(struct notifier_block *dpm_nb,
		unsigned long event, void *data)
{
	switch (event) {
	case PD_WAKE_LOCK:
		hisi_usb_typec_update_wake_lock(true);
		break;

	case PD_WAKE_UNLOCK:
		hisi_usb_typec_update_wake_lock(false);
		break;

	default:
		E("unknown event (%lu)\n", event);
		break;
	}

	return NOTIFY_OK;
}

static void hisi_usb_typec_issue_hardreset(void *dev_data)
{
	struct usb_typec *typec = _typec;
	int ret;

	if (!hisi_usb_typec_ready(typec))
		return;

#ifdef CONFIG_KIRIN_POWER_DELIVERY_SUPPORT
	ret = hisi_tcpm_hard_reset(typec->tcpc_dev);
	if (ret != TCPM_SUCCESS)
		E("hisi_tcpm_hard_reset ret %d\n", ret);
#endif
}

#define HISI_USB_PD_HW_DOCK_SVID 0x12D1
static bool hisi_usb_pd_get_hw_dock_svid_exist(void *client)
{
	struct usb_typec *typec = _typec;

	if (!typec) {
		E("hisi-tcpc not ready\n");
		return false;
	}

	return (typec->dock_svid == HISI_USB_PD_HW_DOCK_SVID);
}

static int hisi_usb_typec_mark_direct_charging(void *data, bool direct_charging)
{
	struct usb_typec *typec = _typec;

	if (!hisi_usb_typec_ready(typec))
		return -ENODEV;

	I("%s\n", direct_charging ? "true" : "false");
	(void)hisi_tcpm_typec_notify_direct_charge(typec->tcpc_dev,
				direct_charging);
	typec->direct_charging = direct_charging;

	return 1;
}

static void hisi_usb_typec_set_pd_adapter_voltage(int voltage_mv, void *dev_data)
{
	struct usb_typec *typec = _typec;

	if (!hisi_usb_typec_ready(typec))
		return;

	typec->pd_adapter_voltage = voltage_mv;

#ifdef CONFIG_KIRIN_POWER_DELIVERY_SUPPORT
	hisi_tcpm_request_voltage(typec->tcpc_dev, voltage_mv);
#endif
}

static void hisi_usb_typec_send_uvdm(uint32_t *data, uint8_t cnt,
		bool wait_resp, void *dev_data, int sop_type)
{
	struct usb_typec *typec = _typec;

	if (!hisi_usb_typec_ready(typec))
		return;

	if (!data)
		return;

	hisi_tcpm_send_uvdm(typec->tcpc_dev, cnt, data, wait_resp, &sop_type);
}

static int hisi_usb_get_cc_state(void)
{
	struct usb_typec *typec = _typec;

	if (!hisi_usb_typec_ready(typec))
		return 0;

	return typec->rt_cc1 | (typec->rt_cc2 << 2);
}

static bool hisi_usb_check_cc_vbus_short(void)
{
	struct usb_typec *typec = _typec;
	uint8_t cc, cc1, cc2;

	if (!hisi_usb_typec_ready(typec))
		return false;

	cc = hisi_tcpc_get_cc_from_analog_ch(typec->tcpc_dev);
	cc1 = cc & ANALOG_SINGLE_CC_STS_MASK;
	cc2 = (cc >> ANALOG_CC2_STS_OFFSET) & ANALOG_SINGLE_CC_STS_MASK;

	I("analog CC status: 0x%x\n", cc);

	return (cc1 == ANALOG_CC_WITH_UNDEFINED_RESISTANCE
		|| cc2 == ANALOG_CC_WITH_UNDEFINED_RESISTANCE);
}

static void hisi_usb_set_cc_mode(int mode)
{
	struct usb_typec *typec = _typec;

	if (!hisi_usb_typec_ready(typec))
		return;

	hisi_tcpm_force_cc_mode(typec->tcpc_dev, mode);
}

static int hisi_usb_pd_dpm_data_role_swap(void *client)
{
	struct usb_typec *typec = _typec;

	if (!hisi_usb_typec_ready(typec))
		return -1;

	D("DataRole Swap\n");
	return hisi_tcpm_data_role_swap(typec->tcpc_dev);
}

static bool hisi_usb_pd_usb_comm(void *client)
{
	struct usb_typec *typec = _typec;

	return hisi_tcpm_usb_comm(typec->tcpc_dev);
}

static void hisi_usb_detect_emark_cable(void *client)
{
	struct usb_typec *typec = _typec;

	if (!hisi_usb_typec_ready(typec))
		return;

	D("en emark_cable det\n");
	hisi_tcpm_detect_emark_cable(typec->tcpc_dev);
}

static void hisi_usb_force_enable_drp(int mode)
{
	struct usb_typec *typec = _typec;

	if (!hisi_usb_typec_ready(typec))
		return;

	if (typec->typec_state.new_state == TYPEC_UNATTACHED)
		hisi_tcpm_force_cc_mode(typec->tcpc_dev, mode);
}

static int hisi_usb_set_pd_disable_status(void *client, bool disable)
{
	struct usb_typec *typec = _typec;

	if (!hisi_usb_typec_ready(typec))
		return -1;

#ifdef CONFIG_KIRIN_POWER_DELIVERY_SUPPORT
	if (disable)
		hisi_pd_put_cc_detached_event(typec->tcpc_dev);
	else
		hisi_tcpm_put_cc_attach_event(typec->tcpc_dev);
#endif

	return 0;
}

/*
 * Check the cable for direct charge or not.
 */
static int hisi_usb_typec_direct_charge_cable_detect(void)
{
	struct usb_typec *typec = _typec;
	uint8_t cc1, cc2;
	int ret;

	if (!hisi_usb_typec_ready(typec))
		return -1;

	ret = hisi_tcpm_inquire_remote_cc(typec->tcpc_dev, &cc1, &cc2, true);
	if (ret) {
		E("inquire remote cc failed\n");
		return -1;
	}

	if ((cc1 == TYPEC_CC_VOLT_SNK_DFT) &&
			(cc2 == TYPEC_CC_VOLT_SNK_DFT)) {
		I("using \"direct charge cable\" !\n");
		typec->direct_charge_cable = true;
		return 0;
	}

	I("not \"direct charge cable\" !\n");
	typec->direct_charge_cable = false;

	return -1;
}

static int __tcpc_notifier_work(struct tcpc_notify *noti)
{
	struct usb_typec *typec = _typec;
	int ret = 0;
	struct pd_dpm_typec_state dpm_tc_state = {0};
	struct pd_dpm_vbus_state dpm_vbus_state = {0};
	struct pd_dpm_swap_state dpm_swap_state = {0};
	struct pd_dpm_pd_state dpm_pd_state = {0};
	struct pd_dpm_cable_info dpm_cable_vdo = {0};
	struct pd_dpm_cable_data dpm_uvdm_data;
#ifdef CONFIG_HISI_ADAPTER_DETECT
	struct pd_dpm_adapter_data adapter_data = {0};
#endif
	int i;

	I("tcpc_notify_type %lu\n", noti->tcpc_notify_type);

	switch (noti->tcpc_notify_type) {
	case TCP_NOTIFY_DIS_VBUS_CTRL:
		hisi_usb_typec_disable_vbus_control(&noti->notify.vbus_state);

		dpm_vbus_state.mv = noti->notify.vbus_state.mv;
		dpm_vbus_state.ma = noti->notify.vbus_state.ma;
		dpm_vbus_state.vbus_type = noti->notify.vbus_state.type;
		ret = pd_dpm_handle_pe_event(PD_DPM_PE_EVT_DIS_VBUS_CTRL, (void *)&dpm_vbus_state);

		break;
	case TCP_NOTIFY_SOURCE_VBUS:

#ifdef CONFIG_REVERSE_CHARGE
		if (hisi_usb_is_reverse_charge_enable() == 1) {
			msleep(1000);
			if (rchg_check_cc_state(typec->tcpc_dev))
				return ret;
		}
#endif
		hisi_usb_typec_source_vbus(&noti->notify.vbus_state);
		dpm_vbus_state.mv = noti->notify.vbus_state.mv;
		dpm_vbus_state.ma = noti->notify.vbus_state.ma;
		dpm_vbus_state.vbus_type = noti->notify.vbus_state.type;
		ret = pd_dpm_handle_pe_event(PD_DPM_PE_EVT_SOURCE_VBUS, (void *)&dpm_vbus_state);

		break;
	case TCP_NOTIFY_SINK_VBUS:
		hisi_usb_typec_sink_vbus(&noti->notify.vbus_state);

		dpm_vbus_state.mv = noti->notify.vbus_state.mv;
		dpm_vbus_state.ma = noti->notify.vbus_state.ma;
		dpm_vbus_state.vbus_type = noti->notify.vbus_state.type;
		dpm_vbus_state.ext_power = noti->notify.vbus_state.ext_power;
		dpm_vbus_state.remote_rp_level = noti->notify.vbus_state.remote_rp_level;
		ret = pd_dpm_handle_pe_event(PD_DPM_PE_EVT_SINK_VBUS, (void *)&dpm_vbus_state);

		break;
	case TCP_NOTIFY_PR_SWAP:
		dpm_swap_state.new_role = noti->notify.swap_state.new_role;
		ret = pd_dpm_handle_pe_event(PD_DPM_PE_EVT_PR_SWAP, (void *)&dpm_swap_state);

		break;
	case TCP_NOTIFY_DR_SWAP:
		hisi_usb_typec_data_role_swap(noti->notify.swap_state.new_role);

		dpm_swap_state.new_role = noti->notify.swap_state.new_role;
		ret = pd_dpm_handle_pe_event(PD_DPM_PE_EVT_DR_SWAP, (void *)&dpm_swap_state);

		break;
	case TCP_NOTIFY_TYPEC_STATE:
		hisi_usb_typec_state_change(&noti->notify.typec_state);

		dpm_tc_state.polarity = noti->notify.typec_state.polarity;
		dpm_tc_state.old_state = noti->notify.typec_state.old_state;
		dpm_tc_state.new_state = noti->notify.typec_state.new_state;
		ret = pd_dpm_handle_pe_event(PD_DPM_PE_EVT_TYPEC_STATE, (void *)&dpm_tc_state);

		break;
	case TCP_NOTIFY_PD_STATE:
		hisi_usb_typec_pd_state_change(&noti->notify.pd_state);

		dpm_pd_state.connected =noti->notify.pd_state.connected;
		ret = pd_dpm_handle_pe_event(PD_DPM_PE_EVT_PD_STATE, (void *)&dpm_pd_state);

		break;
	case TCP_NOTIFY_CABLE_VDO:
		dpm_cable_vdo.cable_vdo = noti->notify.cable_vdo.vdo;
		dpm_cable_vdo.cable_vdo_ext = noti->notify.cable_vdo.vdo_ext;
		ret = pd_dpm_handle_pe_event(PD_DPM_PE_CABLE_VDO, (void *)&dpm_cable_vdo);

		break;
	case TCP_NOTIFY_UVDM:
		dpm_uvdm_data.cnt = noti->notify.uvdm_msg.uvdm_cnt;
		for (i = 0; i < noti->notify.uvdm_msg.uvdm_cnt; i++)
			dpm_uvdm_data.msg[i] = noti->notify.uvdm_msg.uvdm_data[i];

		ret = pd_dpm_handle_pe_event(PD_DPM_PE_CABLE_DATA, (void *)&dpm_uvdm_data);

		break;

#ifdef CONFIG_HISI_ADAPTER_DETECT
	case TCP_NOTIFY_ADAPTER_ABNORMAL:
		adapter_data.status = noti->notify.adapter_status.adapter_cc_without_vbus;
		ret = pd_dpm_handle_pe_event(PD_DPM_PE_ADAPTER_DATA, (void *)&adapter_data);
		break;
#endif

#ifdef CONFIG_KIRIN_PD_REV30
	case TCP_NOTIFY_ALERT:
		hisi_typec_receive_src_alert(noti->notify.alert_msg.ado);
		break;
#endif

	default:
		break;
	}

	return ret;
}

/*
 * Record optional_max_power by max_power value om pdo.
 * max_power:max power value from pdo.
 */
void hisi_usb_typec_max_power(int max_power)
{
	pd_dpm_set_max_power(max_power);
}

/*
 * Reset hw_pd product type when src startup.
 */
void hisi_usb_typec_reset_product(void)
{
	pd_set_product_type(PD_DPM_INVALID_VAL);
}

static bool hisi_usb_support_9v_src_cap(void *dev_data)
{
	struct usb_typec *typec = _typec;
 
	if (!hisi_usb_typec_ready(typec))
		return false;
 
	uint32_t src_cap_9v_mask  = 0xFFC00;
	uint32_t src_cap_9v  = 0x2D000;
 
	return hisi_tcpm_support_src_cap(typec->tcpc_dev, src_cap_9v, src_cap_9v_mask);
}

static int hisi_usb_receive_src_cap(void *client)
{
	struct usb_typec *typec = _typec;
	(void)(client);

	if (!hisi_usb_typec_ready(typec))
		return false;
 
	return hisi_tcpm_receive_src_cap(typec->tcpc_dev);
}

#ifdef CONFIG_REVERSE_CHARGE
void hisi_usb_rpd_update_src_cap(void *client, struct otg_cap *allow_ext, int send)
{
	struct usb_typec *typec = _typec;

	if (!typec || !allow_ext)
		return;

	hisi_usb_rpd_set_src_cap(typec->tcpc_dev, allow_ext, send);
}
 
int hisi_usb_rpd_get_src_extcap(void *client)
{
	struct usb_typec *typec = _typec;

	if (!typec)
		return 0;

	return hisi_usb_get_src_extcap(typec->tcpc_dev);
}
#endif

static void tcpc_notifier_work(struct work_struct *work)
{
	struct usb_typec *typec =
			container_of(work, struct usb_typec, tcpc_notify_work);
	struct tcpc_notify *noti = NULL;
	int ret;

	while (1) {
		noti = get_notify(typec);
		if (!noti)
			break;

		ret = __tcpc_notifier_work(noti);
		if (ret)
			D("__tcpc_notifier_work ret %d\n", ret);

		free_notify(noti);
	}
}

static int tcpc_notifier_call(struct notifier_block *nb,
			unsigned long action, void *data)
{
	struct usb_typec *typec = container_of(nb, struct usb_typec, tcpc_nb);
	int ret;

	ret = queue_notify(typec, action, data);
	if (ret) {
		E("queue_notify failed!\n");
		return NOTIFY_DONE;
	}

	/* Returns %false if @work was already on a queue, %true otherwise. */
	ret = queue_work(typec->hisi_typec_wq, &typec->tcpc_notify_work);

	return NOTIFY_OK;
}

/*
 * Save tcpc_device pointer for futher use.
 * tcpc_dev:Pointer of tcpc_device structure.
 */
void hisi_usb_typec_register_tcpc_device(struct tcpc_device *tcpc_dev)
{
	struct usb_typec *typec = _typec;
	int ret;

	if (!_typec) {
		E("hisi usb typec is not ready\n");
		return;
	}

	/* save the tcpc handler */
	typec->tcpc_dev = tcpc_dev;

	INIT_LIST_HEAD(&typec->tcpc_notify_list);
	spin_lock_init(&typec->tcpc_notify_list_lock);
	typec->tcpc_notify_count = 0;
	INIT_WORK(&typec->tcpc_notify_work, tcpc_notifier_work);

	typec->tcpc_nb.notifier_call = tcpc_notifier_call;
	ret = hisi_tcpm_register_tcpc_dev_notifier(tcpc_dev, &typec->tcpc_nb);
	if (ret)
		E("register tcpc notifier failed ret %d\n", ret);

	ret = usb_typec_debugfs_init(typec);
	if (ret)
		E("debugfs init failed ret %d\n", ret);
}

/*
 * Check the cable for direct charge or not.
 */
static struct cc_check_ops direct_charge_cable_check_ops = {
	.is_cable_for_direct_charge = hisi_usb_typec_direct_charge_cable_detect,
};

static struct pd_dpm_ops hisi_device_pd_dpm_ops = {
	.pd_dpm_get_hw_dock_svid_exist = hisi_usb_pd_get_hw_dock_svid_exist,
	.pd_dpm_notify_direct_charge_status =
			hisi_usb_typec_mark_direct_charging,
	.pd_dpm_get_cc_state = hisi_usb_get_cc_state,
	.pd_dpm_set_cc_mode = hisi_usb_set_cc_mode,
	.pd_dpm_enable_drp = hisi_usb_force_enable_drp,
	.pd_dpm_disable_pd = hisi_usb_set_pd_disable_status,
	.pd_dpm_check_cc_vbus_short = hisi_usb_check_cc_vbus_short,
	.pd_dpm_detect_emark_cable = hisi_usb_detect_emark_cable,
	.data_role_swap = hisi_usb_pd_dpm_data_role_swap,
	.pd_dpm_inquire_usb_comm = hisi_usb_pd_usb_comm,
	.pd_dpm_get_src_cap = hisi_usb_receive_src_cap,

#ifdef CONFIG_REVERSE_CHARGE
	.pd_dpm_get_rev_sc_dev_flag = hisi_usb_get_rev_sc_dev_flag,
	.pd_dpm_set_rev_sc_dev_flag = hisi_usb_set_rev_sc_dev_flag,
	.pd_dpm_set_rev_sc_switch_flag = hisi_usb_set_rev_sc_switch_flag,
	.pd_dpm_is_reverse_charge_enable = hisi_usb_is_reverse_charge_enable,
	.pd_dpm_is_rev_sc_plugin = hisi_usb_first_attach_rscp,
	.pd_dpm_is_rev_sc_plugout = hisi_usb_reverse_charge_unattach_state,
	.pd_dpm_set_src_cap = hisi_usb_rpd_update_src_cap,
	.pd_dpm_get_src_extcap = hisi_usb_rpd_get_src_extcap,
#endif
#ifdef CONFIG_KIRIN_PD_REV30_PPS_SINK
	.pd_dpm_pps_set_state = hisi_typec_set_pdo,
	.pd_dpm_pps_get_apdo_caps = hisi_typec_get_apdo_caps,
	.pd_dpm_pps_get_status = hisi_typec_get_pps_adapter_output,
#endif
};

#ifdef CONFIG_ADAPTER_PROTOCOL_PD
static struct hwpd_ops hisi_device_pd_protocol_ops = {
	.chip_name = "scharger_v600",
	.hard_reset_master = hisi_usb_typec_issue_hardreset,
	.support_9v_src_cap = hisi_usb_support_9v_src_cap,
	.set_output_voltage = hisi_usb_typec_set_pd_adapter_voltage,
};
#endif

#ifdef CONFIG_ADAPTER_PROTOCOL_UVDM
static struct hwuvdm_ops hisi_device_uvdm_protocol_ops = {
	.chip_name = "scharger_v600",
	.send_data = hisi_usb_typec_send_uvdm,
};
#endif

static int hisi_typec_register_pd_dpm(struct usb_typec *typec)
{
	int ret;
	void *data = (void *)typec;

	D("+\n");
	ret = pd_dpm_ops_register(&hisi_device_pd_dpm_ops, data);
	if (ret) {
		I("Need not hisi pd\n");
		return -EBUSY;
	}

#ifdef CONFIG_ADAPTER_PROTOCOL_PD
	hisi_device_pd_protocol_ops.dev_data = data;
	ret = hwpd_ops_register(&hisi_device_pd_protocol_ops);
	if (ret) {
			I("pd protocol register failed\n");
			return -EBUSY;
	}
#endif

#ifdef CONFIG_ADAPTER_PROTOCOL_UVDM
	hisi_device_uvdm_protocol_ops.dev_data = data;
	ret = hwuvdm_ops_register(&hisi_device_uvdm_protocol_ops);
	if (ret)
		E("uvdm protocol register failed\n");
#endif

#ifdef HISI_USB_PD_PPS
	hisi_typec_pps_protocol_ops.dev_data = data;
	ret = pps_protocol_ops_register(&hisi_typec_pps_protocol_ops);
	if (ret)
		E("pps protocol register failed\n");
#endif

	ret = cc_check_ops_register(&direct_charge_cable_check_ops);
	if (ret) {
		E("cc_check_ops register failed!\n");
		return ret;
	}

	typec->wakelock_control_nb.notifier_call = hisi_typec_wake_lock_call;
	ret = register_pd_wake_unlock_notifier(&typec->wakelock_control_nb);
	if (ret) {
		E("register_pd_wake_unlock_notifier ret %d\n", ret);
		return ret;
	}

	D("-\n");
	return 0;
}

/*
 * Register pd dpm ops for hw_pd driver.
 */
int hisi_usb_typec_register_pd_dpm(void)
{
	int ret = 0;
	struct usb_typec *typec = _typec;

	D("+\n");
	if (!typec) {
		E("adapter typec is not ready\n");
		return -EPERM;
	}

	hisi_typec_register_pd_dpm(typec);

	D("-\n");
	return ret;
}

static int typec_probe(struct platform_device *pdev)
{
	struct usb_typec *typec = NULL;
	int ret;

	typec = devm_kzalloc(&pdev->dev, sizeof(*typec), GFP_KERNEL);
	if (!typec) {
		E("typec alloc fail\n");
		return -ENOMEM;
	}

	typec->power_role = PD_ROLE_UNATTACHED;
	typec->data_role = PD_ROLE_UNATTACHED;
	typec->vconn = PD_ROLE_VCONN_OFF;
	typec->audio_accessory = 0;

	mutex_init(&typec->lock);

	typec->hisi_typec_wq = create_singlethread_workqueue("hisi_usb_typec");

	if (of_find_property(pdev->dev.of_node, "check_hardreset_state", &ret))
		typec->check_hard_reset_state = true;
	else
		typec->check_hard_reset_state = false;

#ifdef CONFIG_HISI_ADAPTER_DETECT
	if (of_find_property(pdev->dev.of_node, "support_adapter_detect", &ret))
		typec->support_adapter_detect = true;
	else
		typec->support_adapter_detect = false;
#endif

	_typec = typec;
	ret = of_platform_populate(pdev->dev.of_node, NULL, NULL, &pdev->dev);
	if (ret) {
		E("populate child failed ret %d\n", ret);
		_typec = NULL;
		return ret;
	}

	return 0;
}

static int typec_remove(struct platform_device *pdev)
{
	struct usb_typec *typec = _typec;

	of_platform_depopulate(&pdev->dev);
	_typec = NULL;
	usb_typec_debugfs_exit(typec);

	return 0;
}

#ifdef CONFIG_PM
static int typec_suspend(struct device *dev)
{
	D("-\n");
	return 0;
}

static int typec_resume(struct device *dev)
{
	D("-\n");
	return 0;
}

static const struct dev_pm_ops typec_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(typec_suspend, typec_resume)
};
#define TYPEC_PM_OPS	(&typec_pm_ops)
#else
#define TYPEC_PM_OPS	(NULL)
#endif /* CONFIG_PM */

static const struct of_device_id typec_match_table[] = {
	{.compatible = "hisilicon,hisi-usb-typec",},
	{},
};

static struct platform_driver typec_driver = {
	.driver = {
		.name = "hisi-usb-typec",
		.owner = THIS_MODULE,
		.of_match_table = typec_match_table,
		.pm = TYPEC_PM_OPS,
	},
	.probe = typec_probe,
	.remove = typec_remove,
};

static int __init hisi_typec_init(void)
{
	return platform_driver_register(&typec_driver);
}

static void __exit hisi_typec_exit(void)
{
	platform_driver_unregister(&typec_driver);
}

subsys_initcall(hisi_typec_init);
module_exit(hisi_typec_exit);

MODULE_AUTHOR("Xiaowei Song <songxiaowei@hisilicon.com>");
MODULE_DESCRIPTION("Hisilicon USB Type-C Driver");
MODULE_LICENSE("GPL");
