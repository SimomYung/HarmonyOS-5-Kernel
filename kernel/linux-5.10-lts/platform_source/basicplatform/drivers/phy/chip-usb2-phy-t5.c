/*
 * usb2-phy-t5.c
 * Copyright (c) 2020-2020 Huawei Technologies Co., Ltd.
 * Create:2019-09-24
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

#include <linux/atomic.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of_address.h>
#include <linux/phy/phy.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/workqueue.h>
#include <linux/version.h>
#include <linux/platform_drivers/usb/chip_usb.h>
#include <linux/platform_drivers/usb/hiusb_phy.h>
#include <linux/platform_drivers/usb/usb_misc_ctrl.h>
#include <linux/platform_drivers/usb/chip_usb_helper.h>
#include <linux/platform_drivers/usb/usb_reg_cfg.h>
#include <linux/ktime.h>

#define USBC_HUB_REG_CLK_U2PHY_FREQ_EXAM_COUNTER_MASK 0xFFFF
#define USBC_HUB_REG_CLK_U2PHY_FREQ_EXAM_COUNTER_SHIFT 0
#define MIN_FREQ_EXAM_COUNTER			2045
#define MAX_FREQ_EXAM_COUNTER			2051
#define MAX_UTMI_RETRY_SUCCESS_TIMES		3
#define ENHANCE_UTMI_CLK_RETRY_MAX		2000
#define SMALL_CYCLE_RETRY			10
#define ENHANCE_UTMI_CLK_ERR_CODE		55

#define USB2_T5_DEBUG(format, arg...) \
		pr_debug("[USB2_T5][D][%s]"format, __func__, ##arg)

#define USB2_T5_INFO(format, arg...) \
	pr_err("[USB2_T5][I][%s]"format, __func__, ##arg)

#define USB2_T5_ERR(format, arg...) \
	pr_err("[USB2_T5][E][%s]"format, __func__, ##arg)

#define chip_usb2_phy_to_phy_priv(pointer) \
	container_of(pointer, struct phy_priv, usb_phy)

struct bc_detect_cfg {
	struct chip_usb_reg_cfg **bc12_detect_dcd;
	int bc12_detect_dcd_num;
	struct chip_usb_reg_cfg **bc12_check_fsvplus;
	int bc12_check_fsvplus_num;
	struct chip_usb_reg_cfg **bc12_detect_dcd_end;
	int bc12_detect_dcd_end_num;
	struct chip_usb_reg_cfg **bc12_primary_detect;
	int bc12_primary_detect_num;
	struct chip_usb_reg_cfg **bc12_check_chrdet;
	int bc12_check_chrdet_num;
	struct chip_usb_reg_cfg **bc12_detect_sdp_stop;
	int bc12_detect_sdp_stop_num;
	struct chip_usb_reg_cfg **bc12_secondary_detect;
	int bc12_secondary_detect_num;
	struct chip_usb_reg_cfg **bc12_secondary_detect_done;
	int bc12_secondary_detect_done_num;
	struct chip_usb_reg_cfg **bc12_detect_cdp_done;
	int bc12_detect_cdp_done_num;
	struct chip_usb_reg_cfg **bc12_detect_dcd_fail;
	int bc12_detect_dcd_fail_num;
	struct chip_usb_reg_cfg **bc12_primary_detect_fail;
	int bc12_primary_detect_fail_num;
	struct chip_usb_reg_cfg **bc12_detect_close;
	int bc12_detect_close_num;
};

struct phy_priv {
	struct device *dev;
	struct hiusb_usb2phy usb_phy;

	enum phy_mode mode;
	bool disable_bc;
	bool is_fpga;
	struct clk **clks;
	int num_clks;
	struct chip_usb_reg_cfg **reset;
	int reset_num;
	struct chip_usb_reg_cfg **unreset;
	int unreset_num;
	struct chip_usb_reg_cfg **apb_reset;
	int apb_reset_num;
	struct chip_usb_reg_cfg **apb_unreset;
	int apb_unreset_num;
	struct chip_usb_reg_cfg **phy_no_suspend;
	int phy_no_suspend_num;
	struct chip_usb_reg_cfg **phy_para;
	int phy_para_num;
	struct chip_usb_reg_cfg **phy_para_back2default;
	int phy_para_back2default_num;
	struct chip_usb_reg_cfg **vbus_valid;
	int vbus_valid_num;
	struct chip_usb_reg_cfg **vbus_invalid;
	int vbus_invalid_num;

	bool utmi_clk_need_enhance;
	struct chip_usb_reg_cfg *bist_ctrl0_close;
	struct chip_usb_reg_cfg *bist_ctrl6_close;
	struct chip_usb_reg_cfg *utmi_ovrd_ctrl1_en;
	struct chip_usb_reg_cfg *bist_ctrl1;
	struct chip_usb_reg_cfg *bist_ctrl5;
	struct chip_usb_reg_cfg *utmi_ovrd_ctrl1_close;
	struct chip_usb_reg_cfg *bist_ctrl0_open;

	struct chip_usb_reg_cfg *hiusbc_freq_exam_pulse_len;
	struct chip_usb_reg_cfg *hiusbc_freq_exam_en_low;
	struct chip_usb_reg_cfg *hiusbc_freq_exam_en_high;
	struct chip_usb_reg_cfg *hiusbc_freq_exam_done;
	struct chip_usb_reg_cfg *hiusbc_freq_exam_counter;

	bool set_eye_param_enable;
	struct chip_usb_reg_cfg **host_eye_param;
	int host_eye_param_num;
	struct chip_usb_reg_cfg **device_eye_param;
	int device_eye_param_num;
	bool calibrate_enable;
	struct chip_usb_reg_cfg *calibrate_param;
	bool bc12_detect_enable;
	struct bc_detect_cfg bc_detect_cfg;

	struct chip_usb_reg_cfg **dpdm_pullup;
	int dpdm_pullup_num;
	struct chip_usb_reg_cfg **dpdm_pulldown;
	int dpdm_pulldown_num;
	struct chip_usb_reg_cfg **enable_vdp_src;
	int enable_vdp_src_num;
	struct chip_usb_reg_cfg **disable_vdp_src;
	int disable_vdp_src_num;
	struct chip_usb_reg_cfg *phy_iddq;
	bool is_vdp_src_enabled;

	struct work_struct vdp_src_work;
	atomic_t desired_vdp_src;
};

static int usb2_phy_open_clk(struct phy_priv *priv)
{
	int ret;

	USB2_T5_INFO("+\n");

	if (priv->is_fpga)
		return 0;

	ret = chip_usb_init_clks(priv->clks, priv->num_clks);
	if (ret)
		USB2_T5_ERR("open clk failed %d\n", ret);

	USB2_T5_INFO("-\n");
	return ret;
}

static void set_usb2_eye_diagram_param(struct phy_priv *priv)
{
	struct chip_usb_reg_cfg **reg_cfgs = NULL;
	struct chip_usb_reg_cfg *eye_cfgs = NULL;
	int check_num = 0;
	int ret = 0;
	int i = 0;

	if (priv->is_fpga)
		return;

	if (!(priv->set_eye_param_enable)) {
		USB2_T5_INFO("No need to configure eye diagram\n");
		return;
	}

	if (priv->mode == PHY_MODE_USB_HOST) {
		chip_usb_reg_write_array(priv->host_eye_param, priv->host_eye_param_num);
		reg_cfgs = priv->host_eye_param;
		check_num = priv->host_eye_param_num;
		ret = chip_usb_reg_test_cfg(*(priv->host_eye_param));
	} else if (priv->mode == PHY_MODE_USB_DEVICE) {
		chip_usb_reg_write_array(priv->device_eye_param, priv->device_eye_param_num);
		reg_cfgs = priv->device_eye_param;
		check_num = priv->device_eye_param_num;
		ret = chip_usb_reg_test_cfg(*(priv->device_eye_param));
	}

	if (reg_cfgs && ret) {
		for (i = 0; i < check_num; i++) {
			eye_cfgs = reg_cfgs[i];
			USB2_T5_INFO("set phy diagram param: 0x%x\n", eye_cfgs->value);
		}
	} else {
		USB2_T5_ERR("usb parameters set failed\n");
	}

	return;
}

static int _usb2_phy_init(struct phy_priv *priv)
{
	int ret;

	USB2_T5_INFO("+\n");

	ret = misc_ctrl_init();
	if (ret)
		return ret;

	ret = usb2_phy_open_clk(priv);
	if (ret)
		goto exit_misc_ctrl;

	if (priv->apb_unreset) {
		USB2_T5_INFO("do phy apb_unreset\n");
		ret = chip_usb_reg_write_array(priv->apb_unreset, priv->apb_unreset_num);
		if (ret) {
			USB2_T5_ERR("phy apb_unreset failed %d\n", ret);
			goto exit_misc_ctrl;
		}
	}

	ret = chip_usb_reg_write_array(priv->unreset, priv->unreset_num);
	if (ret) {
		USB2_T5_ERR("phy unreset failed %d\n", ret);
		goto exit_misc_ctrl;
	}

	ret = chip_usb_reg_write_array(priv->vbus_valid, priv->vbus_valid_num);
	if (ret) {
		USB2_T5_ERR("set vbus valid failed %d\n", ret);
		goto exit_misc_ctrl;
	}

	USB2_T5_INFO("[PHY_MODE_USB_%s]: set phy-eye-diagram-param\n",
			(priv->mode == PHY_MODE_USB_HOST) ? "HOST" : "DEVICE");
	/* set eye param */
	set_usb2_eye_diagram_param(priv);

	USB2_T5_INFO("-\n");
	return ret;

exit_misc_ctrl:
	misc_ctrl_exit();

	USB2_T5_ERR("failed to init phy ret: %d\n", ret);
	return ret;
}

static int usb2_phy_init(struct phy *phy)
{
	struct hiusb_usb2phy *usb2_phy = NULL;
	struct phy_priv *priv = NULL;

	if (!phy)
		return -ENODEV;

	usb2_phy = phy_get_drvdata(phy);
	if (!usb2_phy)
		return -ENODEV;

	priv = chip_usb2_phy_to_phy_priv(usb2_phy);

	return _usb2_phy_init(priv);
}

static bool utmi_clk_is_need_enhanced(struct hiusb_usb2phy *usb2_phy)
{
	struct phy_priv *priv = NULL;

	if (!usb2_phy)
		return false;

	priv = chip_usb2_phy_to_phy_priv(usb2_phy);
	
	/* Need to introduce a special chip avoidance process to make USB UTMI clock more stable */
	if (priv->utmi_clk_need_enhance) {
		USB2_T5_INFO("usb utmi phy clk need to be enhanced\n");
		return true;
	}

	return false;
}
static bool utmi_freq_exam_is_good(struct hiusb_usb2phy *usb2_phy)
{
	int ret, i;
	unsigned int reg_val;
	u8 u2_done = 0;
	bool freq_offset_is_good = false;
	u32 utmi_clk_counter;
	struct phy_priv *priv = NULL;

	USB2_T5_DEBUG("+\n");

	if (!usb2_phy)
		return false;

	priv = chip_usb2_phy_to_phy_priv(usb2_phy);	
	if (priv->hiusbc_freq_exam_pulse_len) {
		ret = chip_usb_reg_write(priv->hiusbc_freq_exam_pulse_len);
		if (ret)
			USB2_T5_ERR("set FREQ_EXAM_PULSE_LEN failed\n");
		if (!chip_usb_reg_read_cfg(priv->hiusbc_freq_exam_pulse_len, &reg_val))
			USB2_T5_DEBUG("FREQ_EXAM_PULSE_LEN is 0x%x!\n", reg_val);
	}

	for (i = 1; i <= MAX_UTMI_RETRY_SUCCESS_TIMES; ++i) {
		/* enable phy clk freq exam */
		if (priv->hiusbc_freq_exam_en_low) {
			ret = chip_usb_reg_write(priv->hiusbc_freq_exam_en_low);
			if (ret)
				USB2_T5_ERR("time %d : FREQ_EXAM_EN write 0 failed!\n", i);
			if (!chip_usb_reg_read_cfg(priv->hiusbc_freq_exam_en_low, &reg_val))
				USB2_T5_DEBUG("time %d : FREQ_EXAM_EN now is 0x%x\n", i, reg_val);
		}
		
		if (priv->hiusbc_freq_exam_en_high) {
			ret = chip_usb_reg_write(priv->hiusbc_freq_exam_en_high);
			if (ret)
				USB2_T5_ERR("time %d : FREQ_EXAM_EN write 1 failed!\n", i);
			if (!chip_usb_reg_read_cfg(priv->hiusbc_freq_exam_en_high, &reg_val))
				USB2_T5_DEBUG("time %d : FREQ_EXAM_EN now is 0x%x\n", i, reg_val);
		}
			
		udelay(50);
		if (!chip_usb_reg_read_cfg(priv->hiusbc_freq_exam_done, &reg_val)) {
			USB2_T5_DEBUG("time %d : FREQ_EXAM_DONE is 0x%x\n", i, reg_val);
			if ((reg_val & (1 << USBC_HUB_REG_CLK_U2PHY_FREQ_EXAM_COUNTER_SHIFT)))
				u2_done = 1;
		}

		/* show phy clk freq result */
		if (!chip_usb_reg_read_cfg(priv->hiusbc_freq_exam_counter, &reg_val)) {
			utmi_clk_counter = (reg_val & USBC_HUB_REG_CLK_U2PHY_FREQ_EXAM_COUNTER_MASK);
			USB2_T5_DEBUG("time %d : FREQ_EXAM_COUNTER is 0x%x, bit0-15:0x%x\n", i, reg_val, utmi_clk_counter);
			if (u2_done && (utmi_clk_counter >= MIN_FREQ_EXAM_COUNTER && utmi_clk_counter <= MAX_FREQ_EXAM_COUNTER)) {
				freq_offset_is_good = true;		
				USB2_T5_DEBUG("time %d :utmi clk freq offset is good!!!\n", i);
			} else {
				freq_offset_is_good = false;		
				USB2_T5_DEBUG("time %d :utmi clk freq offset is failed!!! u2_done %d\n", i, u2_done);
				break;
			}
		}
	}

	return freq_offset_is_good;
}

static bool _phy_hs_bist_is_good(struct hiusb_usb2phy *usb2_phy)
{
	int ret;
	bool is_good = false;
	struct phy_priv *priv = NULL;

	USB2_T5_DEBUG("+\n");

	if (!usb2_phy)
		return false;

	priv = chip_usb2_phy_to_phy_priv(usb2_phy);
	if (priv->bist_ctrl0_close) {
		ret = chip_usb_reg_write(priv->bist_ctrl0_close);
		if (ret) {
			USB2_T5_ERR("config bist_ctrl0_close failed\n");
			goto back_to_default;
		}
	}

	if (priv->bist_ctrl6_close) {
		ret = chip_usb_reg_write(priv->bist_ctrl6_close);
		if (ret) {
			USB2_T5_ERR("config bist_ctrl6_close failed\n");
			goto back_to_default;
		}
	}

	if (priv->utmi_ovrd_ctrl1_en) {
		ret = chip_usb_reg_write(priv->utmi_ovrd_ctrl1_en);
		if (ret) {
			USB2_T5_ERR("config utmi_ovrd_ctrl1_en failed\n");
			goto back_to_default;
		}
	}

	if (priv->bist_ctrl1) {
		ret = chip_usb_reg_write(priv->bist_ctrl1);
		if (ret) {
			USB2_T5_ERR("config bist_ctrl1 failed\n");
			goto back_to_default;
		}
	}

	if (priv->bist_ctrl0_open) {
		ret = chip_usb_reg_write(priv->bist_ctrl0_open);
		if (ret) {
			USB2_T5_ERR("config bist_ctrl0_open failed\n");
			goto back_to_default;
		}
	}

	udelay(50);

	unsigned int reg_val = 0;
	chip_usb_reg_read_cfg(priv->bist_ctrl5, &reg_val);
	USB2_T5_DEBUG("HS BIST (bist_ctrl5) val is 0x%x!\n", reg_val);

	if (priv->bist_ctrl5 && (reg_val == 0x5)) {
		USB2_T5_DEBUG("HS BIST is good!\n");
		is_good = true;
	}

back_to_default:
	ret = chip_usb_reg_write(priv->utmi_ovrd_ctrl1_close);
	if (ret)
		USB2_T5_ERR("config utmi_ovrd_ctrl1_close failed\n");
	ret = chip_usb_reg_write(priv->bist_ctrl0_close);
	if (ret)
		USB2_T5_ERR("config bist_ctrl0_close failed\n");
	return is_good;
}

static bool phy_hs_bist_is_good(struct hiusb_usb2phy *usb2_phy)
{
	int i;
	for (i = 1; i <= MAX_UTMI_RETRY_SUCCESS_TIMES; ++i) {
		if (!_phy_hs_bist_is_good(usb2_phy)) {
		    USB2_T5_DEBUG("time %d: hs_bist failed!!!\n", i);
		    return false;
		}
	}
	return true;
}

static int usb2_phy_set_no_suspend(struct hiusb_usb2phy *usb2_phy)
{
	int ret = 0;
	struct phy_priv *priv = NULL;
	USB2_T5_DEBUG("+\n");

	if (!usb2_phy)
		return -ENODEV;

	priv = chip_usb2_phy_to_phy_priv(usb2_phy);
	if (priv->phy_no_suspend) {
		ret = chip_usb_reg_write_array(priv->phy_no_suspend, priv->phy_no_suspend_num);
		if (ret)
			USB2_T5_ERR("close phy suspend reg failed %d\n", ret);
	}

	USB2_T5_DEBUG("-\n");
	return ret;
}

static int usb2_phy_set_para(struct hiusb_usb2phy *usb2_phy)
{
	int ret = 0;
	struct phy_priv *priv = NULL;
	USB2_T5_DEBUG("+\n");

	if (!usb2_phy)
		return -ENODEV;

	priv = chip_usb2_phy_to_phy_priv(usb2_phy);
	if (priv->phy_para) {
		ret = chip_usb_reg_write_array(priv->phy_para, priv->phy_para_num);
		if (ret)
			USB2_T5_ERR("phy para failed %d\n", ret);
	}

	USB2_T5_DEBUG("-\n");
	return ret;
}

static int usb2_phy_set_para_back2default(struct hiusb_usb2phy *usb2_phy)
{
	int ret = 0;
	struct phy_priv *priv = NULL;
	USB2_T5_DEBUG("+\n");

	if (!usb2_phy)
		return -ENODEV;

	priv = chip_usb2_phy_to_phy_priv(usb2_phy);	
	if (priv->phy_para_back2default) {
		ret = chip_usb_reg_write_array(priv->phy_para_back2default, priv->phy_para_back2default_num);
		if (ret)
			USB2_T5_ERR("phy para back2default failed %d\n", ret);
	}

	USB2_T5_DEBUG("-\n");
	return ret;
}

static int usb2_phy_close_clk(struct phy_priv *priv)
{
	USB2_T5_INFO("+\n");

	if (priv->is_fpga)
		return 0;

	chip_usb_shutdown_clks(priv->clks, priv->num_clks);

	USB2_T5_INFO("-\n");
	return 0;
}

static void usb2_phy_config_iddq(struct phy_priv *priv)
{
	int ret;

	if (priv->phy_iddq) {
		USB2_T5_INFO("config phy iddq\n");
		ret = chip_usb_reg_write(priv->phy_iddq);
		if (ret) {
			USB2_T5_ERR("config phy iddq failed\n");
			return;
		}
	}
}

static int _usb2_phy_exit(struct phy_priv *priv)
{
	int ret;

	USB2_T5_INFO("+\n");

	ret = chip_usb_reg_write_array(priv->vbus_invalid,
			priv->vbus_invalid_num);
	if (ret)
		USB2_T5_ERR("set vbus invalid failed %d\n", ret);

	usb2_phy_config_iddq(priv);
	ret = chip_usb_reg_write_array(priv->reset,
			priv->reset_num);
	if (ret)
		USB2_T5_ERR("phy reset failed %d\n", ret);

	if (priv->apb_reset) {
		ret = chip_usb_reg_write_array(priv->apb_reset,
				priv->apb_reset_num);
		if (ret)
			USB2_T5_ERR("phy apb_reset failed %d\n", ret);
	}

	ret = usb2_phy_close_clk(priv);
	if (ret)
		USB2_T5_ERR("close clk failed %d\n", ret);

	misc_ctrl_exit();

	USB2_T5_INFO("-\n");
	return ret;
}

static int usb2_phy_exit(struct phy *phy)
{
	struct hiusb_usb2phy *usb2_phy = NULL;
	struct phy_priv *priv = NULL;

	if (!phy)
		return -ENODEV;

	usb2_phy = phy_get_drvdata(phy);
	if (!usb2_phy)
		return -ENODEV;

	priv = chip_usb2_phy_to_phy_priv(usb2_phy);

	return _usb2_phy_exit(priv);
}

static int usb2_phy_set_mode(struct phy *phy, enum phy_mode mode)
{
	struct hiusb_usb2phy *usb2_phy = NULL;
	struct phy_priv *priv = NULL;

	if (!phy)
		return -ENODEV;

	usb2_phy = (struct hiusb_usb2phy *)phy_get_drvdata(phy);
	if (!usb2_phy)
		return -ENODEV;

	priv = chip_usb2_phy_to_phy_priv(usb2_phy);

	priv->mode = mode;

	return 0;
}

static int usb2_phy_set_mode_ext(struct phy *phy, enum phy_mode mode,
				 int submode)
{
	return usb2_phy_set_mode(phy, mode);
}

static int _usb2_phy_calibrate(struct phy_priv *priv)
{
	int ret;

	USB2_T5_INFO("+\n");

	ret = chip_usb_reg_write(priv->calibrate_param);
	if (ret)
		USB2_T5_ERR("phy_calibrate failed %d\n", ret);

	USB2_T5_INFO("-\n");

	return ret;
}

static int usb2_phy_calibrate(struct phy *phy)
{
	struct hiusb_usb2phy *usb2_phy = NULL;
	struct phy_priv *priv = NULL;

	if (!phy)
		return -ENODEV;

	usb2_phy = phy_get_drvdata(phy);
	if (!usb2_phy)
		return -ENODEV;

	priv = chip_usb2_phy_to_phy_priv(usb2_phy);
	if (priv->calibrate_enable)
		return _usb2_phy_calibrate(priv);

	return 0;
}

static struct phy_ops usb2_phy_ops = {
	.init		= usb2_phy_init,
	.exit		= usb2_phy_exit,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)
	.set_mode	= usb2_phy_set_mode_ext,
#else
	.set_mode	= usb2_phy_set_mode,
#endif
	.calibrate	= usb2_phy_calibrate,
	.owner		= THIS_MODULE,
};

static int is_dcd_timeout(struct phy_priv *priv)
{
	unsigned long jiffies_expire;
	int ret = 0;
	int i = 0;

	jiffies_expire = jiffies + msecs_to_jiffies(900);
	msleep(50);
	while (i < 10) {
		ret = chip_usb_reg_test_cfg(*(priv->bc_detect_cfg.bc12_check_fsvplus));
		if (ret)
			i++;
		else
			i = 0;

		msleep(10);

		if (time_after(jiffies, jiffies_expire)) {
			USB2_T5_ERR("DCD timeout!\n");
			ret = -1;
			break;
		}
	}

	return ret;
}

static enum chip_charger_type bc12_detect_charger_type(struct phy_priv *priv)
{
	enum chip_charger_type charger_type = CHARGER_TYPE_UNKNOWN;
	int ret;

	chip_usb_reg_write_array(priv->bc_detect_cfg.bc12_detect_dcd,
						priv->bc_detect_cfg.bc12_detect_dcd_num);

	ret = is_dcd_timeout(priv);
	if (ret <= 0) {
		USB2_T5_ERR("bc12_check_fsvplus fail\n");
		chip_usb_reg_write_array(priv->bc_detect_cfg.bc12_detect_dcd_fail,
							priv->bc_detect_cfg.bc12_detect_dcd_fail_num);
		goto close_bc12_detect;
	}

	chip_usb_reg_write_array(priv->bc_detect_cfg.bc12_detect_dcd_end,
						priv->bc_detect_cfg.bc12_detect_dcd_end_num);
	msleep(2);
	chip_usb_reg_write_array(priv->bc_detect_cfg.bc12_primary_detect,
						priv->bc_detect_cfg.bc12_primary_detect_num);
	msleep(30);

	ret = chip_usb_reg_test_cfg(*(priv->bc_detect_cfg.bc12_check_chrdet));
	if (ret <= 0) {
		USB2_T5_ERR("charger type = SDP\n");
		chip_usb_reg_write_array(priv->bc_detect_cfg.bc12_primary_detect_fail,
							priv->bc_detect_cfg.bc12_primary_detect_fail_num);
		charger_type = CHARGER_TYPE_SDP;
		goto close_bc12_detect;
	}

	chip_usb_reg_write_array(priv->bc_detect_cfg.bc12_detect_sdp_stop,
						priv->bc_detect_cfg.bc12_detect_sdp_stop_num);
	msleep(2);
	chip_usb_reg_write_array(priv->bc_detect_cfg.bc12_secondary_detect,
						priv->bc_detect_cfg.bc12_secondary_detect_num);
	msleep(30);

	ret = chip_usb_reg_test_cfg(*(priv->bc_detect_cfg.bc12_check_chrdet));
	if (ret <= 0) {
		USB2_T5_ERR("charger type = CDP\n");
		chip_usb_reg_write_array(priv->bc_detect_cfg.bc12_secondary_detect_done,
							priv->bc_detect_cfg.bc12_secondary_detect_done_num);
		chip_usb_reg_write_array(priv->bc_detect_cfg.bc12_detect_cdp_done,
							priv->bc_detect_cfg.bc12_detect_cdp_done_num);
		charger_type = CHARGER_TYPE_CDP;
	} else {
		USB2_T5_ERR("charger type = DCP\n");
		chip_usb_reg_write_array(priv->bc_detect_cfg.bc12_secondary_detect_done,
							priv->bc_detect_cfg.bc12_secondary_detect_done_num);
		charger_type = CHARGER_TYPE_DCP;
	}

close_bc12_detect:
	chip_usb_reg_write_array(priv->bc_detect_cfg.bc12_detect_close,
						priv->bc_detect_cfg.bc12_detect_close_num);
	return charger_type;
}

static enum chip_charger_type detect_charger_type(
		struct hiusb_usb2phy *usb2_phy)
{
	struct phy_priv *priv = NULL;
	struct phy *phy = NULL;
	enum chip_charger_type charger_type = CHARGER_TYPE_SDP;

	if (!usb2_phy)
		return CHARGER_TYPE_NONE;

	priv = chip_usb2_phy_to_phy_priv(usb2_phy);

	if (priv->disable_bc)
		return CHARGER_TYPE_SDP;

	phy = priv->usb_phy.phy;

	mutex_lock(&phy->mutex);
	if (phy->init_count == 0) {
		mutex_unlock(&phy->mutex);
		USB2_T5_ERR("phy has not been initialized\n");
		return CHARGER_TYPE_NONE;
	}

	if (priv->bc12_detect_enable)
		charger_type = bc12_detect_charger_type(priv);

	mutex_unlock(&phy->mutex);

	return charger_type;
}

static int set_dpdm_pulldown(struct hiusb_usb2phy *usb2_phy, bool pulldown)
{
	struct phy_priv *priv = NULL;
	struct phy *phy = NULL;
	int ret;

	if (!usb2_phy)
		return -EINVAL;

	priv = chip_usb2_phy_to_phy_priv(usb2_phy);
	phy = priv->usb_phy.phy;

	if (priv->is_fpga || priv->disable_bc ||
		!priv->dpdm_pullup || !priv->dpdm_pulldown)
		return 0;

	USB2_T5_INFO("+\n");
	mutex_lock(&phy->mutex);
	if (phy->init_count == 0) {
		mutex_unlock(&phy->mutex);
		USB2_T5_ERR("phy has not been initialized\n");
		return -ENODEV;
	}

	if (pulldown)
		ret = chip_usb_reg_write_array(priv->dpdm_pulldown, priv->dpdm_pulldown_num);
	else
		ret = chip_usb_reg_write_array(priv->dpdm_pullup, priv->dpdm_pullup_num);

	if (ret)
		USB2_T5_ERR("%s failed %d\n", pulldown ? "pulldown" : "pullup", ret);

	mutex_unlock(&phy->mutex);
	USB2_T5_INFO("-\n");
	return ret;
}

static int _set_vdp_src(struct phy_priv *priv, bool enable)
{
	struct phy *phy = priv->usb_phy.phy;
	int ret;

	if (priv->is_fpga || priv->disable_bc ||
		!priv->enable_vdp_src || !priv->disable_vdp_src)
		return 0;

	if ((enable && priv->is_vdp_src_enabled) ||
		(!enable && !priv->is_vdp_src_enabled))
		return 0;

	USB2_T5_INFO("+\n");
	mutex_lock(&phy->mutex);
	if (phy->init_count == 0) {
		mutex_unlock(&phy->mutex);
		USB2_T5_ERR("phy has not been initialized\n");
		return -ENODEV;
	}

	if (enable)
		ret = chip_usb_reg_write_array(priv->enable_vdp_src, priv->enable_vdp_src_num);
	else
		ret = chip_usb_reg_write_array(priv->disable_vdp_src, priv->disable_vdp_src_num);

	if (ret)
		USB2_T5_ERR("%s vdp_src failed %d\n", enable ? "enable" : "disable", ret);
	else
		priv->is_vdp_src_enabled = enable ? true : false;

	mutex_unlock(&phy->mutex);
	USB2_T5_INFO("-\n");
	return ret;
}

static int set_vdp_src(struct hiusb_usb2phy *usb2_phy, bool enable)
{
	struct phy_priv *priv = NULL;

	if (!usb2_phy)
		return -EINVAL;

	priv = chip_usb2_phy_to_phy_priv(usb2_phy);

	if (priv->is_fpga)
		return 0;

	if (in_atomic() || irqs_disabled()) {
		if (enable)
			atomic_set(&priv->desired_vdp_src, 1);
		else
			atomic_set(&priv->desired_vdp_src, 0);

		if (!queue_work(system_power_efficient_wq,
					&priv->vdp_src_work)) {
			USB2_T5_INFO("vdp src work already in queue\n");
			return -EAGAIN;
		}
		return 0;
	} else {
		return _set_vdp_src(priv, enable);
	}
}

static void vdp_src_work_fn(struct work_struct *work)
{
	int ret;
	struct phy_priv *priv = container_of(work, struct phy_priv, vdp_src_work);

	ret = _set_vdp_src(priv, atomic_read(&priv->desired_vdp_src) == 1);
	if (ret)
		USB2_T5_ERR("set vdp src failed %d\n", ret);
}

static int get_bc12_ops_resource(struct phy_priv *priv)
{
	struct device *dev = priv->dev;
	int ret;

	ret = get_chip_usb_reg_cfg_array(dev, "bc12-detect-dcd",
			&(priv->bc_detect_cfg.bc12_detect_dcd), &(priv->bc_detect_cfg.bc12_detect_dcd_num));
	if (ret)
		goto exit_bc12_detect_dcd;

	ret = get_chip_usb_reg_cfg_array(dev, "bc12-check-fsvplus",
			&(priv->bc_detect_cfg.bc12_check_fsvplus), &(priv->bc_detect_cfg.bc12_check_fsvplus_num));
	if (ret)
		goto exit_bc12_check_fsvplus;

	ret = get_chip_usb_reg_cfg_array(dev, "bc12-detect-dcd-end",
			&(priv->bc_detect_cfg.bc12_detect_dcd_end), &(priv->bc_detect_cfg.bc12_detect_dcd_end_num));
	if (ret)
		goto exit_bc12_detect_dcd_end;

	ret = get_chip_usb_reg_cfg_array(dev, "bc12-primary-detect",
			&(priv->bc_detect_cfg.bc12_primary_detect), &(priv->bc_detect_cfg.bc12_primary_detect_num));
	if (ret)
		goto exit_bc12_primary_detect;

	ret = get_chip_usb_reg_cfg_array(dev, "bc12-check-chrdet",
			&(priv->bc_detect_cfg.bc12_check_chrdet), &(priv->bc_detect_cfg.bc12_check_chrdet_num));
	if (ret)
		goto exit_bc12_check_chrdet;

	ret = get_chip_usb_reg_cfg_array(dev, "bc12-detect-sdp-stop",
			&(priv->bc_detect_cfg.bc12_detect_sdp_stop), &(priv->bc_detect_cfg.bc12_detect_sdp_stop_num));
	if (ret)
		goto exit_bc12_detect_sdp_stop;

	ret = get_chip_usb_reg_cfg_array(dev, "bc12-secondary-detect",
			&(priv->bc_detect_cfg.bc12_secondary_detect), &(priv->bc_detect_cfg.bc12_secondary_detect_num));
	if (ret)
		goto exit_bc12_secondary_detect;

	ret = get_chip_usb_reg_cfg_array(dev, "bc12-secondary-detect-done",
			&(priv->bc_detect_cfg.bc12_secondary_detect_done), &(priv->bc_detect_cfg.bc12_secondary_detect_done_num));
	if (ret)
		goto exit_bc12_secondary_detect_done;

	ret = get_chip_usb_reg_cfg_array(dev, "bc12-detect-cdp-done",
			&(priv->bc_detect_cfg.bc12_detect_cdp_done), &(priv->bc_detect_cfg.bc12_detect_cdp_done_num));
	if (ret)
		goto exit_bc12_detect_cdp_done;

	ret = get_chip_usb_reg_cfg_array(dev, "bc12-detect-dcd-fail",
			&(priv->bc_detect_cfg.bc12_detect_dcd_fail), &(priv->bc_detect_cfg.bc12_detect_dcd_fail_num));
	if (ret)
		goto exit_bc12_detect_dcd_fail;

	ret = get_chip_usb_reg_cfg_array(dev, "bc12-primary-detect-fail",
			&(priv->bc_detect_cfg.bc12_primary_detect_fail), &(priv->bc_detect_cfg.bc12_primary_detect_fail_num));
	if (ret)
		goto exit_bc12_primary_detect_fail;

	ret = get_chip_usb_reg_cfg_array(dev, "bc12-detect-close",
			&(priv->bc_detect_cfg.bc12_detect_close), &(priv->bc_detect_cfg.bc12_detect_close_num));
	if (ret)
		goto exit_bc12_detect_close;

	return ret;

exit_bc12_detect_close:
	free_chip_usb_reg_cfg_array(priv->bc_detect_cfg.bc12_primary_detect_fail,
			priv->bc_detect_cfg.bc12_primary_detect_fail_num);
exit_bc12_primary_detect_fail:
	free_chip_usb_reg_cfg_array(priv->bc_detect_cfg.bc12_detect_dcd_fail, priv->bc_detect_cfg.bc12_detect_dcd_fail_num);
exit_bc12_detect_dcd_fail:
	free_chip_usb_reg_cfg_array(priv->bc_detect_cfg.bc12_detect_cdp_done, priv->bc_detect_cfg.bc12_detect_cdp_done_num);
exit_bc12_detect_cdp_done:
	free_chip_usb_reg_cfg_array(priv->bc_detect_cfg.bc12_secondary_detect_done,
			priv->bc_detect_cfg.bc12_secondary_detect_done_num);
exit_bc12_secondary_detect_done:
	free_chip_usb_reg_cfg_array(priv->bc_detect_cfg.bc12_secondary_detect, priv->bc_detect_cfg.bc12_secondary_detect_num);
exit_bc12_secondary_detect:
	free_chip_usb_reg_cfg_array(priv->bc_detect_cfg.bc12_detect_sdp_stop, priv->bc_detect_cfg.bc12_detect_sdp_stop_num);
exit_bc12_detect_sdp_stop:
	free_chip_usb_reg_cfg_array(priv->bc_detect_cfg.bc12_check_chrdet, priv->bc_detect_cfg.bc12_check_chrdet_num);
exit_bc12_check_chrdet:
	free_chip_usb_reg_cfg_array(priv->bc_detect_cfg.bc12_primary_detect, priv->bc_detect_cfg.bc12_primary_detect_num);
exit_bc12_primary_detect:
	free_chip_usb_reg_cfg_array(priv->bc_detect_cfg.bc12_detect_dcd_end, priv->bc_detect_cfg.bc12_detect_dcd_end_num);
exit_bc12_detect_dcd_end:
	free_chip_usb_reg_cfg_array(priv->bc_detect_cfg.bc12_check_fsvplus, priv->bc_detect_cfg.bc12_check_fsvplus_num);
exit_bc12_check_fsvplus:
	free_chip_usb_reg_cfg_array(priv->bc_detect_cfg.bc12_detect_dcd, priv->bc_detect_cfg.bc12_detect_dcd_num);
exit_bc12_detect_dcd:
	USB2_T5_ERR("get bc detect resouce failed %d\n", ret);
	return ret;
}

static int get_ops_resource(struct phy_priv *priv)
{
	struct device *dev = priv->dev;
	int ret;

	USB2_T5_INFO("+\n");

	ret = get_chip_usb_reg_cfg_array(dev, "phy-reset",
			&priv->reset, &priv->reset_num);
	if (ret) {
		USB2_T5_ERR("get phy reset failed\n");
		return -ENOENT;
	}

	ret = get_chip_usb_reg_cfg_array(dev, "phy-unreset",
			&priv->unreset, &priv->unreset_num);
	if (ret) {
		USB2_T5_ERR("get phy unrest failed %d\n", ret);
		goto exit_unreset;
	}

	ret = get_chip_usb_reg_cfg_array(dev, "phy-apb-reset",
			&priv->apb_reset, &priv->apb_reset_num);
	if (ret)
		USB2_T5_INFO("get phy apb-reset failed %d\n", ret);

	ret = get_chip_usb_reg_cfg_array(dev, "phy-apb-unreset",
			&priv->apb_unreset, &priv->apb_unreset_num);
	if (ret)
		USB2_T5_INFO("get phy apb-unrest failed %d\n", ret);

	ret = get_chip_usb_reg_cfg_array(dev, "phy-para",
			&priv->phy_para, &priv->phy_para_num);
	if (ret)
		USB2_T5_INFO("get phy-para failed %d\n", ret);

	ret = get_chip_usb_reg_cfg_array(dev, "phy-no-suspend",
			&priv->phy_no_suspend, &priv->phy_no_suspend_num);
	if (ret)
		USB2_T5_INFO("get phy-no-suspend failed %d\n", ret);

	ret = get_chip_usb_reg_cfg_array(dev, "phy-para-back2default",
			&priv->phy_para_back2default, &priv->phy_para_back2default_num);
	if (ret)
		USB2_T5_INFO("get phy-para-back2default failed %d\n", ret);

	ret = get_chip_usb_reg_cfg_array(dev, "vbus-valid",
				&priv->vbus_valid, &priv->vbus_valid_num);
	if (ret) {
		USB2_T5_ERR("get vbus valid failed %d\n", ret);
		goto exit_vbus_valid;
	}

	ret = get_chip_usb_reg_cfg_array(dev, "vbus-invalid",
				&priv->vbus_invalid, &priv->vbus_invalid_num);
	if (ret) {
		USB2_T5_ERR("get vbus invalid failed %d\n", ret);
		goto exit_vbus_invalid;
	}

	priv->bist_ctrl0_close = of_get_chip_usb_reg_cfg(dev->of_node, "bist_ctrl0_close");
	if (!priv->bist_ctrl0_close)
		USB2_T5_INFO("bist_ctrl0_close not found\n");

	priv->bist_ctrl6_close = of_get_chip_usb_reg_cfg(dev->of_node, "bist_ctrl6_close");
	if (!priv->bist_ctrl6_close)
		USB2_T5_INFO("bist_ctrl6_close not found\n");

	priv->utmi_ovrd_ctrl1_en = of_get_chip_usb_reg_cfg(dev->of_node, "utmi_ovrd_ctrl1_en");
	if (!priv->utmi_ovrd_ctrl1_en)
		USB2_T5_INFO("utmi_ovrd_ctrl1_en not found\n");

	priv->bist_ctrl1 = of_get_chip_usb_reg_cfg(dev->of_node, "bist_ctrl1");
	if (!priv->bist_ctrl1)
		USB2_T5_INFO("bist_ctrl1 not found\n");

	priv->bist_ctrl5 = of_get_chip_usb_reg_cfg(dev->of_node, "bist_ctrl5");
	if (!priv->bist_ctrl5)
		USB2_T5_INFO("bist_ctrl5 not found\n");

	priv->utmi_ovrd_ctrl1_close = of_get_chip_usb_reg_cfg(dev->of_node, "utmi_ovrd_ctrl1_close");
	if (!priv->utmi_ovrd_ctrl1_close)
		USB2_T5_INFO("utmi_ovrd_ctrl1_close not found\n");

	priv->bist_ctrl0_open = of_get_chip_usb_reg_cfg(dev->of_node, "bist_ctrl0_open");
	if (!priv->bist_ctrl0_open)
		USB2_T5_INFO("bist_ctrl0_open not found\n");

	priv->hiusbc_freq_exam_pulse_len = of_get_chip_usb_reg_cfg(dev->of_node, "hiusbc_freq_exam_pulse_len");
	if (!priv->hiusbc_freq_exam_pulse_len)
		USB2_T5_INFO("hiusbc_freq_exam_pulse_len not found\n");
	priv->hiusbc_freq_exam_en_low = of_get_chip_usb_reg_cfg(dev->of_node, "hiusbc_freq_exam_en_low");
	if (!priv->hiusbc_freq_exam_en_low)
		USB2_T5_INFO("hiusbc_freq_exam_en_low not found\n");
	priv->hiusbc_freq_exam_en_high = of_get_chip_usb_reg_cfg(dev->of_node, "hiusbc_freq_exam_en_high");
	if (!priv->hiusbc_freq_exam_en_high)
		USB2_T5_INFO("hiusbc_freq_exam_en_high not found\n");
	priv->hiusbc_freq_exam_done = of_get_chip_usb_reg_cfg(dev->of_node, "hiusbc_freq_exam_done");
	if (!priv->hiusbc_freq_exam_done)
		USB2_T5_INFO("hiusbc_freq_exam_done not found\n");
	priv->hiusbc_freq_exam_counter = of_get_chip_usb_reg_cfg(dev->of_node, "hiusbc_freq_exam_counter");
	if (!priv->hiusbc_freq_exam_counter)
		USB2_T5_INFO("hiusbc_freq_exam_counter not found\n");

	priv->utmi_clk_need_enhance = of_property_read_bool(dev->of_node, "utmi-clk-need-enhance");

	priv->set_eye_param_enable = of_property_read_bool(dev->of_node, "set-eye-param-enable");
	if (priv->set_eye_param_enable) {
		ret = get_chip_usb_reg_cfg_array(dev, "host-eye-param",
				&priv->host_eye_param, &priv->host_eye_param_num);
		if (ret) {
			USB2_T5_ERR("get host eye param failed %d\n", ret);
			goto exit_host_eye_param;
		}
		ret = get_chip_usb_reg_cfg_array(dev, "device-eye-param",
				&priv->device_eye_param, &priv->device_eye_param_num);
		if (ret) {
			USB2_T5_ERR("get  device eye param failed %d\n", ret);
			goto exit_device_eye_param;
		}
	}

	priv->calibrate_enable = of_property_read_bool(dev->of_node, "calibrate-enable");
	if (priv->calibrate_enable) {
		priv->calibrate_param = of_get_chip_usb_reg_cfg(dev->of_node, "calibrate-para");
		if (!priv->calibrate_param) {
			ret = -EINVAL;
			USB2_T5_ERR("get phy_calibrate_param failed %d\n", ret);
			goto exit_calibrate_param;
		}
	}

	priv->bc12_detect_enable = of_property_read_bool(dev->of_node, "bc12-detect-enable");
	if (priv->bc12_detect_enable) {
		ret = get_bc12_ops_resource(priv);
		if (ret) {
			USB2_T5_ERR("get bc1.2 ops resource failed %d\n", ret);
			goto exit_bc12_resource;
		}
	}

	ret = get_chip_usb_reg_cfg_array(dev, "dpdm-pullup",
				&priv->dpdm_pullup, &priv->dpdm_pullup_num);
	if (ret)
		USB2_T5_ERR("get dpdm-pullup failed %d\n", ret);

	ret = get_chip_usb_reg_cfg_array(dev, "dpdm-pulldown",
				&priv->dpdm_pulldown, &priv->dpdm_pulldown_num);
	if (ret)
		USB2_T5_ERR("get dpdm-pulldown failed %d\n", ret);

	ret = get_chip_usb_reg_cfg_array(dev, "enable-vdp-src",
				&priv->enable_vdp_src, &priv->enable_vdp_src_num);
	if (ret)
		USB2_T5_ERR("get enable-vdp-src failed %d\n", ret);

	ret = get_chip_usb_reg_cfg_array(dev, "disable-vdp-src",
				&priv->disable_vdp_src, &priv->disable_vdp_src_num);
	if (ret)
		USB2_T5_ERR("get disable-vdp-src failed %d\n", ret);

	priv->phy_iddq = of_get_chip_usb_reg_cfg(dev->of_node, "phy_iddq");
	if (!priv->phy_iddq)
		USB2_T5_INFO("phy_iddq not found\n");

	USB2_T5_INFO("-\n");
	return 0;

exit_bc12_resource:
	if (priv->calibrate_enable)
		of_remove_chip_usb_reg_cfg(priv->calibrate_param);
exit_calibrate_param:
	if (priv->set_eye_param_enable)
		free_chip_usb_reg_cfg_array(priv->device_eye_param, priv->device_eye_param_num);
exit_device_eye_param:
	if (priv->set_eye_param_enable)
		free_chip_usb_reg_cfg_array(priv->host_eye_param, priv->host_eye_param_num);
exit_host_eye_param:
	free_chip_usb_reg_cfg_array(priv->vbus_invalid, priv->vbus_invalid_num);
exit_vbus_invalid:
	free_chip_usb_reg_cfg_array(priv->vbus_valid, priv->vbus_valid_num);
exit_vbus_valid:
	free_chip_usb_reg_cfg_array(priv->unreset, priv->unreset_num);
exit_unreset:
	free_chip_usb_reg_cfg_array(priv->reset, priv->reset_num);
	USB2_T5_ERR("- %d\n", ret);
	return ret;
}

static void put_bc12_ops_resource(struct phy_priv *priv)
{
	if (!(priv->bc12_detect_enable))
		return;

	free_chip_usb_reg_cfg_array(priv->bc_detect_cfg.bc12_detect_close,
					priv->bc_detect_cfg.bc12_detect_close_num);
	free_chip_usb_reg_cfg_array(priv->bc_detect_cfg.bc12_primary_detect_fail,
					priv->bc_detect_cfg.bc12_primary_detect_fail_num);
	free_chip_usb_reg_cfg_array(priv->bc_detect_cfg.bc12_detect_dcd_fail,
					priv->bc_detect_cfg.bc12_detect_dcd_fail_num);
	free_chip_usb_reg_cfg_array(priv->bc_detect_cfg.bc12_detect_cdp_done,
					priv->bc_detect_cfg.bc12_detect_cdp_done_num);
	free_chip_usb_reg_cfg_array(priv->bc_detect_cfg.bc12_secondary_detect_done,
					priv->bc_detect_cfg.bc12_secondary_detect_done_num);
	free_chip_usb_reg_cfg_array(priv->bc_detect_cfg.bc12_secondary_detect,
					priv->bc_detect_cfg.bc12_secondary_detect_num);
	free_chip_usb_reg_cfg_array(priv->bc_detect_cfg.bc12_detect_sdp_stop,
					priv->bc_detect_cfg.bc12_detect_sdp_stop_num);
	free_chip_usb_reg_cfg_array(priv->bc_detect_cfg.bc12_check_chrdet,
					priv->bc_detect_cfg.bc12_check_chrdet_num);
	free_chip_usb_reg_cfg_array(priv->bc_detect_cfg.bc12_primary_detect,
					priv->bc_detect_cfg.bc12_primary_detect_num);
	free_chip_usb_reg_cfg_array(priv->bc_detect_cfg.bc12_detect_dcd_end,
					priv->bc_detect_cfg.bc12_detect_dcd_end_num);
	free_chip_usb_reg_cfg_array(priv->bc_detect_cfg.bc12_check_fsvplus,
					priv->bc_detect_cfg.bc12_check_fsvplus_num);
	free_chip_usb_reg_cfg_array(priv->bc_detect_cfg.bc12_detect_dcd,
					priv->bc_detect_cfg.bc12_detect_dcd_num);
	return;
}

static void put_enhance_utmi_clk_ops_resource(struct phy_priv *priv)
{
	if (priv->phy_no_suspend)
		free_chip_usb_reg_cfg_array(priv->phy_no_suspend, priv->phy_no_suspend_num);
	if (priv->phy_para)
		free_chip_usb_reg_cfg_array(priv->phy_para, priv->phy_para_num);
	if (priv->phy_para_back2default)
		free_chip_usb_reg_cfg_array(priv->phy_para_back2default, priv->phy_para_back2default_num);
	if (priv->bist_ctrl0_close)
		of_remove_chip_usb_reg_cfg(priv->bist_ctrl0_close);
	if (priv->bist_ctrl6_close)
		of_remove_chip_usb_reg_cfg(priv->bist_ctrl6_close);
	if (priv->utmi_ovrd_ctrl1_en)
		of_remove_chip_usb_reg_cfg(priv->utmi_ovrd_ctrl1_en);
	if (priv->bist_ctrl1)
		of_remove_chip_usb_reg_cfg(priv->bist_ctrl1);
	if (priv->bist_ctrl5)
		of_remove_chip_usb_reg_cfg(priv->bist_ctrl5);
	if (priv->utmi_ovrd_ctrl1_close)
		of_remove_chip_usb_reg_cfg(priv->utmi_ovrd_ctrl1_close);
	if (priv->bist_ctrl0_open)
		of_remove_chip_usb_reg_cfg(priv->bist_ctrl0_open);

	if (priv->hiusbc_freq_exam_pulse_len)
		of_remove_chip_usb_reg_cfg(priv->hiusbc_freq_exam_pulse_len);
	if (priv->hiusbc_freq_exam_en_low)
		of_remove_chip_usb_reg_cfg(priv->hiusbc_freq_exam_en_low);
	if (priv->hiusbc_freq_exam_en_high)
		of_remove_chip_usb_reg_cfg(priv->hiusbc_freq_exam_en_high);
	if (priv->hiusbc_freq_exam_done)
		of_remove_chip_usb_reg_cfg(priv->hiusbc_freq_exam_done);
	if (priv->hiusbc_freq_exam_counter)
		of_remove_chip_usb_reg_cfg(priv->hiusbc_freq_exam_counter);
	return;
}

static void put_ops_resource(struct phy_priv *priv)
{
	USB2_T5_INFO("+\n");

	free_chip_usb_reg_cfg_array(priv->reset, priv->reset_num);

	free_chip_usb_reg_cfg_array(priv->unreset, priv->unreset_num);

	free_chip_usb_reg_cfg_array(priv->vbus_valid,
					priv->vbus_valid_num);
	free_chip_usb_reg_cfg_array(priv->vbus_invalid,
					priv->vbus_invalid_num);
	if (priv->set_eye_param_enable) {
		free_chip_usb_reg_cfg_array(priv->host_eye_param,
						priv->host_eye_param_num);
		free_chip_usb_reg_cfg_array(priv->device_eye_param,
						priv->device_eye_param_num);
	}

	if (priv->calibrate_enable)
		of_remove_chip_usb_reg_cfg(priv->calibrate_param);

	put_bc12_ops_resource(priv);

	if (priv->dpdm_pullup)
		free_chip_usb_reg_cfg_array(priv->dpdm_pullup, priv->dpdm_pullup_num);
	if (priv->dpdm_pulldown)
		free_chip_usb_reg_cfg_array(priv->dpdm_pulldown, priv->dpdm_pulldown_num);
	if (priv->enable_vdp_src)
		free_chip_usb_reg_cfg_array(priv->enable_vdp_src, priv->enable_vdp_src_num);
	if (priv->disable_vdp_src)
		free_chip_usb_reg_cfg_array(priv->disable_vdp_src, priv->disable_vdp_src_num);
	if (priv->phy_iddq)
		of_remove_chip_usb_reg_cfg(priv->phy_iddq);
	if (priv->apb_reset)
		free_chip_usb_reg_cfg_array(priv->apb_reset, priv->apb_reset_num);
	if (priv->apb_unreset)
		free_chip_usb_reg_cfg_array(priv->apb_unreset, priv->apb_unreset_num);
	
	put_enhance_utmi_clk_ops_resource(priv);
	USB2_T5_INFO("-\n");
}

static int get_clk_resource(struct phy_priv *priv)
{
	struct device *dev = priv->dev;
	int ret;

	if (priv->is_fpga)
		return 0;

	ret = chip_usb_get_clks(dev->of_node, &priv->clks, &priv->num_clks);
	if (ret)
		USB2_T5_ERR("get clk fail %d\n", ret);

	return ret;
}

static void put_clk_resource(struct phy_priv *priv)
{
	int ret;
	if (priv->is_fpga)
		return;

	ret = chip_usb_put_clks(priv->clks, priv->num_clks);
	if (ret)
		USB2_T5_ERR("put clk fail %d\n", ret);
}

static int get_dts_resource(struct phy_priv *priv)
{
	priv->is_fpga = of_property_read_bool(priv->dev->of_node, "is-fpga");
	priv->disable_bc = of_property_read_bool(priv->dev->of_node,
					"disable-bc");

	USB2_T5_INFO("is_fpga %d, disable_bc %d!\n",
		priv->is_fpga, priv->disable_bc);

	if (get_clk_resource(priv)) {
		USB2_T5_ERR("get clk failed\n");
		return -EINVAL;
	}

	if (get_ops_resource(priv)) {
		put_clk_resource(priv);
		USB2_T5_ERR("get ops failed\n");
		return -EINVAL;
	}

	return 0;
}

static void put_dts_resource(struct phy_priv *priv)
{
	put_clk_resource(priv);

	put_ops_resource(priv);
}

static int usb2_phy_suspend(struct hiusb_usb2phy *usb2_phy)
{
	struct phy_priv *priv = NULL;
	int ret;

	if (!usb2_phy)
		return -EINVAL;

	priv = chip_usb2_phy_to_phy_priv(usb2_phy);
	ret = usb2_phy_close_clk(priv);
	if (ret)
		USB2_T5_ERR("close clk failed %d\n", ret);

	return ret;
}

static int usb2_phy_resume(struct hiusb_usb2phy *usb2_phy)
{
	struct phy_priv *priv = NULL;
	int ret;

	if (!usb2_phy)
		return -EINVAL;

	priv = chip_usb2_phy_to_phy_priv(usb2_phy);
	ret = usb2_phy_open_clk(priv);
	if (ret)
		USB2_T5_ERR("open clk failed %d\n", ret);

	return ret;
}

static int hiusb_usb2phy_enhance_clk(struct hiusb_usb2phy *usb2_phy)
{
	int ret;
	struct phy *phy;
	pr_debug("+\n");
	if (!usb2_phy || !usb2_phy->phy)
		return -ENODEV;

	phy = usb2_phy->phy;
	if (phy->power_count > 0) {
		pr_debug("do phy poweroff\n");
		if (phy_power_off(phy))
			pr_err("usb2 phy poweroff failed\n");
	}

	udelay(500);

	ret = usb2_phy_set_para(usb2_phy);
	if (ret)
		pr_err("config phy para failed %d\n", ret);

	if (phy->power_count == 0) {
		pr_debug("do phy poweron\n");
		if (phy_power_on(phy))
			pr_err("usb2 phy poweron failed\n");
	}

	udelay(300);
	ret = usb2_phy_set_para_back2default(usb2_phy);
	if (ret)
		pr_err("config phy para back2default failed %d\n", ret);

	udelay(100);
	pr_debug("-\n");
	return ret;
}

static int phy_enhance_utmi_clk(struct hiusb_usb2phy *usb2_phy)
{
	int ret, count;
	pr_info("+\n");

	if (utmi_freq_exam_is_good(usb2_phy) && phy_hs_bist_is_good(usb2_phy)) {
		pr_info("[usb_utmi_clk][first check good]\n");
		ret = usb2_phy_set_no_suspend(usb2_phy);
		if (ret)
			pr_err("config phy close suspend failed\n");
		return ret;
	}
	for (count = 1; count <= ENHANCE_UTMI_CLK_RETRY_MAX; ++count) {
		ret = hiusb_usb2phy_enhance_clk(usb2_phy);
		if (ret) {
			pr_err("enhance t5 phy clk fail\n");
			return ret;
		}
		if(utmi_freq_exam_is_good(usb2_phy) && phy_hs_bist_is_good(usb2_phy)) {
			pr_info("[usb_utmi_clk][retry check good]after retry %d times, utmi clk is good!!!!\n", count);
			return 0;
		}
	}

	return ENHANCE_UTMI_CLK_ERR_CODE;
}

static int enhance_utmi_clk(struct hiusb_usb2phy *usb2_phy)
{
	int ret, enhance_ret;
	ktime_t start, diff;
	pr_info("[usb_utmi_clk][begin]enhance utmi clk begin!\n");

	start = ktime_get();
	ret = 0;
	enhance_ret = phy_enhance_utmi_clk(usb2_phy);
	if (enhance_ret == ENHANCE_UTMI_CLK_ERR_CODE) {
		pr_info("[usb_utmi_clk][retry N times not good]after retry %d times, still failed, will force go enum!\n",
				ENHANCE_UTMI_CLK_RETRY_MAX);
	} else if (enhance_ret < 0){
		pr_err("utmi enhance process failed! will ignore it and go enum!\n");
		ret = enhance_ret;
	} else if (enhance_ret == 0) {
		pr_info("enhance utmi clk success, will do enum!\n");
	}

	diff = ktime_sub(ktime_get(), start);
	pr_info("[usb_utmi_clk][end]enhance utmi clk pass %lld us, will do enum!\n", ktime_to_us(diff));
	return ret;
}

static int usb2_phy_enhance(struct hiusb_usb2phy *usb2_phy)
{
	int ret = 0;
	if (!utmi_clk_is_need_enhanced(usb2_phy)) {
		pr_info("[usb_utmi_clk]no need enhance utmi clk!\n");
		return ret;
	}

	return enhance_utmi_clk(usb2_phy);
}

static int usb2_phy_t5_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct phy_provider *phy_provider = NULL;
	struct phy *phy = NULL;
	struct phy_priv *priv = NULL;
	int ret;

	USB2_T5_INFO("+\n");
	if (!misc_ctrl_is_ready()) {
		USB2_T5_ERR("misc ctrl is not ready\n");
		return -EPROBE_DEFER;
	}

	pm_runtime_set_active(dev);
	pm_runtime_enable(dev);
	pm_runtime_no_callbacks(dev);
	ret = pm_runtime_get_sync(dev);
	if (ret < 0) {
		USB2_T5_ERR("pm_runtime_get_sync failed\n");
		goto err_pm_put;
	}

	pm_runtime_forbid(dev);

	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv) {
		ret = -ENOMEM;
		goto err_pm_allow;
	}

	priv->dev = dev;

	ret = get_dts_resource(priv);
	if (ret) {
		USB2_T5_ERR("get_dts_resource failed\n");
		goto err_pm_allow;
	}

	phy = devm_phy_create(dev, NULL, &usb2_phy_ops);
	if (IS_ERR(phy)) {
		ret = PTR_ERR(phy);
		goto err_get_dts;
	}

	priv->usb_phy.phy = phy;
	priv->usb_phy.detect_charger_type = detect_charger_type;
	priv->usb_phy.set_dpdm_pulldown = set_dpdm_pulldown;
	priv->usb_phy.set_vdp_src = set_vdp_src;
	priv->usb_phy.phy_suspend = usb2_phy_suspend;
	priv->usb_phy.phy_resume = usb2_phy_resume;
	priv->usb_phy.phy_enhance = usb2_phy_enhance;
	priv->mode = PHY_MODE_INVALID;
	priv->is_vdp_src_enabled = false;
	phy_set_drvdata(phy, &priv->usb_phy);
	INIT_WORK(&priv->vdp_src_work, vdp_src_work_fn);
	atomic_set(&priv->desired_vdp_src, 0);

	phy_provider = devm_of_phy_provider_register(dev, of_phy_simple_xlate);
	if (IS_ERR(phy_provider)) {
		ret = PTR_ERR(phy_provider);
		goto err_get_dts;
	}

	platform_set_drvdata(pdev, priv);

	USB2_T5_ERR("-\n");
	return 0;

err_get_dts:
	put_dts_resource(priv);
err_pm_allow:
	pm_runtime_allow(dev);
err_pm_put:
	pm_runtime_put_sync(dev);
	pm_runtime_disable(dev);

	USB2_T5_INFO("ret %d\n", ret);
	return ret;
}

static int usb2_phy_t5_remove(struct platform_device *pdev)
{
	struct phy_priv *priv = (struct phy_priv *)platform_get_drvdata(pdev);

	if (cancel_work_sync(&priv->vdp_src_work))
		USB2_T5_INFO("vdp src work canceled\n");

	put_dts_resource(priv);
	pm_runtime_allow(&pdev->dev);
	pm_runtime_put_sync(&pdev->dev);
	pm_runtime_disable(&pdev->dev);

	return 0;
}

static const struct of_device_id usb2_phy_t5_of_match[] = {
	{ .compatible = "hisilicon,usb2-phy-t5", },
	{ }
};
MODULE_DEVICE_TABLE(of, usb2_phy_t5_of_match);

static struct platform_driver usb2_phy_t5_driver = {
	.probe = usb2_phy_t5_probe,
	.remove = usb2_phy_t5_remove,
	.driver = {
		.name = "usb2-phy-t5",
		.of_match_table = usb2_phy_t5_of_match,
	}
};
module_platform_driver(usb2_phy_t5_driver);

MODULE_AUTHOR("Hu Wang");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("USB2 PHY T5 Driver");
