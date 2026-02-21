/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Device driver for scharger FCP protocol
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/delay.h>
#include <linux/regmap.h>
#include <linux/power_supply.h>
#include <linux/module.h>
#include <securec.h>

#include <chipset_common/hwpower/protocol/adapter_protocol.h>
#include <chipset_common/hwpower/protocol/adapter_protocol_scp.h>
#include <chipset_common/hwpower/protocol/adapter_protocol_fcp.h>

#include "hisi_fcp_drv.h"

#define scharger_err(fmt, args...) pr_err("[hisi_fcp]" fmt, ## args)
#define scharger_evt(fmt, args...) pr_warn("[hisi_fcp]" fmt, ## args)
#define scharger_inf(fmt, args...) pr_info("[hisi_fcp]" fmt, ## args)
#define scharger_dbg(fmt, args...) pr_debug("[hisi_fcp]" fmt, ## args)

static struct fcp *g_scharger_fcp = NULL;
static struct fcp_buck_ops *g_buck_ops = NULL;

int scharger_fcp_buck_ops_register(struct fcp_buck_ops *ops)
{
	int ret = 0;

	if (ops != NULL) {
		g_buck_ops = ops;
	} else {
		scharger_err("scharger scp buck ops register fail!\n");
		ret = -EPERM;
	}
	return ret;
}

static int is_vusb_plugout(struct fcp *fcp)
{
	unsigned int val = 0;
	int ret;

	if (g_buck_ops == NULL) {
		scharger_err("%s buck_ops is NULl!\n", __func__);
		return 1;
	}

	ret = regmap_read(fcp->regmap, g_buck_ops->regs.irq_vusb_plugout.reg, &val);
	if (ret != 0) {
		scharger_err("%s fail, ret %d.\n", __func__, ret);
		return 1;
	}

	if(val & g_buck_ops->regs.irq_vusb_plugout.mask)
		return 1;
	else
		return 0;
}

/* check adapter voltage is around expected voltage */
static int fcp_adapter_vol_check(struct fcp *fcp, int adapter_vol_mv)
{
	int i;
	int adc_vol = 0;

	if (g_buck_ops == NULL) {
		scharger_err("%s buck_ops is NULl!\n", __func__);
		return -1;
	}

	if ((adapter_vol_mv < FCP_ADAPTER_MIN_VOL) ||
		(adapter_vol_mv > FCP_ADAPTER_MAX_VOL)) {
		scharger_err("%s: check vol out of range, input vol = %dmV\n", __func__,
			     adapter_vol_mv);
		return -1;
	}

	for (i = 0; i < FCP_ADAPTER_VOL_CHECK_TIMEOUT; i++) {
		if (g_buck_ops->get_vbus != NULL)
			adc_vol = g_buck_ops->get_vbus();
		if ((adc_vol > (adapter_vol_mv - FCP_ADAPTER_VOL_CHECK_ERROR)) &&
			(adc_vol < (adapter_vol_mv + FCP_ADAPTER_VOL_CHECK_ERROR))) {
			break;
		}
		msleep(FCP_ADAPTER_VOL_CHECK_POLLTIME);
	}

	if (i == FCP_ADAPTER_VOL_CHECK_TIMEOUT) {
		scharger_err("%s: check vol timeout, input vol = %dmV\n", __func__,
			     adapter_vol_mv);
		return -1;
	}
	scharger_inf("%s: check vol success, input vol = %dmV, spent %dms\n",
		     __func__, adapter_vol_mv, i * FCP_ADAPTER_VOL_CHECK_POLLTIME);
	return 0;
}

static int fcp_ping_success(struct fcp *fcp)
{
	unsigned int reg_val = 0;
	int i, ret, vusb_plugout;

	if (g_buck_ops == NULL) {
		scharger_err("%s buck_ops is NULl!\n", __func__);
		return CHG_FCP_ADAPTER_DETECT_FAIL;
	}

	/* enable fcp_en */
	regmap_update_bits(fcp->regmap, g_buck_ops->regs.cntl, CHG_FCP_EN_MSK,
		 TRUE << CHG_FCP_EN_SHIFT);

	/* detect hisi fcp charger, wait for ping succ */
	for (i = 0; i < CHG_FCP_DETECT_MAX_COUT; i++) {
		ret = regmap_read(fcp->regmap, g_buck_ops->regs.statuis, &reg_val);
		vusb_plugout = is_vusb_plugout(fcp);
		scharger_err("%s:wait for ping succ:0x%x\n", __func__, reg_val);
		if (ret != 0) {
			scharger_err("%s:read det attach err,ret:%d\n", __func__, ret);
			continue;
		}
		if (vusb_plugout) {
			scharger_err("%s:vbus uv happen, adapter plug out!:%d\n", __func__,
				vusb_plugout);
			break;
		}
		if (((reg_val & (CHG_FCP_DVC_MSK | CHG_FCP_ATTATCH_MSK)) ==
			CHG_FCP_SLAVE_GOOD)) {
			break;
		}

		msleep(CHG_FCP_POLL_TIME);
	}
	if (i == CHG_FCP_DETECT_MAX_COUT || vusb_plugout) {
		regmap_update_bits(fcp->regmap, g_buck_ops->regs.cntl,
			 CHG_FCP_EN_MSK, FALSE << CHG_FCP_EN_SHIFT);
		regmap_update_bits(fcp->regmap, g_buck_ops->regs.fcp_ctrl,
			 CHG_FCP_CMP_EN_MSK, FALSE << CHG_FCP_CMP_EN_SHIFT);
		regmap_update_bits(fcp->regmap, g_buck_ops->regs.fcp_ctrl,
			 CHG_FCP_DET_EN_MSK, FALSE << CHG_FCP_DET_EN_SHIFT);
		scharger_err("fcp adapter detect failed,reg[0x%x]=0x%x\n",
			 g_buck_ops->regs.statuis, reg_val);
		return CHG_FCP_ADAPTER_DETECT_FAIL; /* not fcp adapter */
	}
	scharger_inf("fcp adapter detect ok\n");
	return CHG_FCP_ADAPTER_DETECT_SUCC;
}

static int reg_judge(u8 reg_v1, u8 reg_v2, u8 reg_v3, u8 reg_v4)
{
	if ((reg_v1 & CHG_FCP_ACK) && (reg_v1 & CHG_FCP_CMDCPL) &&
		!(reg_v2 & (CHG_FCP_CRCRX | CHG_FCP_PARRX))) {
		return 0;
	} else if (((reg_v1 & CHG_FCP_CRCPAR) ||
		(reg_v3 & CHG_FCP_INIT_HAND_FAIL) ||
		(reg_v4 & CHG_FCP_ENABLE_HAND_FAIL)) &&
		(reg_v2 & CHG_FCP_PROTSTAT)) {
		scharger_inf("%s:FCP_TRANSFER_FAIL,slave status changed:ISR1=0x%x,ISR2=0x%x,ISR3=0x%x,ISR4=0x%x\n",
			     __func__, reg_v1, reg_v2, reg_v3, reg_v4);
		return -1;
	} else if (reg_v1 & CHG_FCP_NACK) {
		scharger_inf(
			"%s:FCP_TRANSFER_FAIL,slave nack: ISR1=0x%x,ISR2=0x%x\n",
			__func__, reg_v1, reg_v2);
		return -1;
	} else if ((reg_v2 & CHG_FCP_CRCRX) || (reg_v2 & CHG_FCP_PARRX) ||
		(reg_v3 & CHG_FCP_TAIL_HAND_FAIL)) {
		scharger_inf("%s:FCP_TRANSFER_FAIL, CRCRX_PARRX_ERROR:ISR1=0x%x,ISR2=0x%x, ISR3=0x%x\n",
			     __func__, reg_v1, reg_v2, reg_v3);
		return -1;
	} else {
		return 1; /* 1: indicates failure of reg judge */
	}
}

static int fcp_cmd_transfer_check(struct fcp *fcp)
{
	unsigned int reg_val1 = 0;
	unsigned int reg_val2 = 0;
	unsigned int reg_val3 = 0;
	unsigned int reg_val4 = 0;
	int i = 0;
	int ret = 0;

	if (g_buck_ops == NULL) {
		scharger_err("%s buck_ops is NULl!\n", __func__);
		return -1;
	}

	/* read accp interrupt registers until value is not zero */
	do {
		usleep_range(12000, 13000); /* (12000, 13000): sleep range */
		ret += regmap_read(fcp->regmap, g_buck_ops->regs.isr1, &reg_val1);
		ret += regmap_read(fcp->regmap, g_buck_ops->regs.isr2, &reg_val2);
		ret += regmap_read(fcp->regmap, g_buck_ops->regs.fcp_irq3, &reg_val3);
		ret += regmap_read(fcp->regmap, g_buck_ops->regs.fcp_irq4, &reg_val4);
		if (ret != 0) {
			scharger_err("%s:reg read failed!\n", __func__);
			break;
		}
		if (reg_val1 || reg_val2) {
			ret = reg_judge((u8)reg_val1, (u8)reg_val2, (u8)reg_val3, (u8)reg_val4);
			if (ret == -1 || ret == 0)
				return ret;
			else
				scharger_inf("%s:FCP_TRANSFER_FAIL, ISR1=0x%x, ISR2=0x%x, ISR3=0x%x, total time = %dms\n",
					     __func__, reg_val1, reg_val2,
					     reg_val3, i * 10); /* 10: unit transfer */
		}
		i++;
	} while (i < FCP_ACK_RETRY_CYCLE);

	scharger_inf("%s:fcp adapter transfer time out,total time %d ms\n",
		__func__, i * 10); /* 10 time difference */
	return -1;
}

/* Description: disable accp protocol and enable again */
static void fcp_protocol_restart(struct fcp *fcp)
{
	unsigned int reg_val = 0;
	int ret = 0, i;

	if (g_buck_ops == NULL) {
		scharger_err("%s buck_ops is NULl!\n", __func__);
		return;
	}

	/* disable accp protocol */
	mutex_lock(&fcp->fcp_detect_lock);
	ret += regmap_update_bits(fcp->regmap, g_buck_ops->regs.cntl, CHG_FCP_EN_MSK,
			 FALSE << CHG_FCP_EN_SHIFT);
	usleep_range(9000, 10000); /* (9000, 10000): sleep range */
	ret += regmap_update_bits(fcp->regmap, g_buck_ops->regs.cntl, CHG_FCP_EN_MSK,
			 TRUE << CHG_FCP_EN_SHIFT);

	/* detect hisi fcp charger, wait for ping succ */
	for (i = 0; i < FCP_RESTART_TIME; i++) {
		usleep_range(9000, 10000); /* (9000, 10000): sleep range */
		ret += regmap_read(fcp->regmap, g_buck_ops->regs.statuis, &reg_val);
		if (ret != 0) {
			scharger_err("%s:read det attach err,ret:%d\n", __func__, ret);
			continue;
		}

		if (((reg_val & (CHG_FCP_DVC_MSK | CHG_FCP_ATTATCH_MSK)) ==
			CHG_FCP_SLAVE_GOOD))
			break;
	}

	if (i == FCP_RESTART_TIME) {
		scharger_err("%s:wait for slave fail\n", __func__);
		mutex_unlock(&fcp->fcp_detect_lock);
		return;
	}
	mutex_unlock(&fcp->fcp_detect_lock);
	scharger_err("%s:disable and enable fcp protocol accp status is 0x%x\n",
		__func__, reg_val);
}

static int fcp_adapter_reg_read(	struct fcp *fcp, u8 *val, u8 reg)
{
	int ret = 0;
	int i;
	unsigned int reg_val1 = 0;
	unsigned int reg_val2 = 0;

	if (g_buck_ops == NULL) {
		scharger_err("%s buck_ops is NULl!\n", __func__);
		return -1;
	}

	mutex_lock(&fcp->accp_adapter_reg_lock);
	for (i = 0; i < FCP_RETRY_TIME; i++) {
		/* before send cmd, clear accp interrupt registers */
		ret  += regmap_read(fcp->regmap, g_buck_ops->regs.isr1, &reg_val1);
		ret  += regmap_read(fcp->regmap, g_buck_ops->regs.isr2, &reg_val2);

		if (reg_val1 != 0)
			ret += regmap_write(fcp->regmap, g_buck_ops->regs.isr1, reg_val1);
		if (reg_val2 != 0)
			ret += regmap_write(fcp->regmap, g_buck_ops->regs.isr2, reg_val2);

		ret += regmap_write(fcp->regmap, g_buck_ops->regs.cmd , CHG_FCP_CMD_SBRRD);
		ret += regmap_write(fcp->regmap, g_buck_ops->regs.addr, (unsigned int)reg);
		ret += regmap_update_bits(fcp->regmap, g_buck_ops->regs.cntl,
				 CHG_FCP_SNDCMD_MSK, CHG_FCP_EN);
		if (ret != 0) {
			scharger_err("%s: write error ret is %d\n", __func__, ret);
			mutex_unlock(&fcp->accp_adapter_reg_lock);
			return -1;
		}

		/* check cmd transfer success or fail */
		if (fcp_cmd_transfer_check(fcp) == 0) {
			/* recived data from adapter */
			ret  += regmap_read(fcp->regmap, g_buck_ops->regs.fcp_rdata0, (unsigned int*)val);
			break;
		}
		fcp_protocol_restart(fcp);
	}
	if (i >= FCP_RETRY_TIME) {
		scharger_err("%s:ack error,retry %d times\n", __func__, i);
		ret = -1;
	}
	mutex_unlock(&fcp->accp_adapter_reg_lock);

	return ret;
}

static int fcp_adapter_reg_write(struct fcp *fcp, u8 val, u8 reg)
{
	int ret = 0;
	int i;
	unsigned int reg_val1 = 0;
	unsigned int reg_val2 = 0;

	if (g_buck_ops == NULL) {
		scharger_err("%s buck_ops is NULl!\n", __func__);
		return FCP_FAIL;
	}

	mutex_lock(&fcp->accp_adapter_reg_lock);
	for (i = 0; i < FCP_RETRY_TIME; i++) {
		/* before send cmd, clear accp interrupt registers */
		scharger_err("%s fcp_retry_time:%d\n", __func__, i);
		ret += regmap_read(fcp->regmap, g_buck_ops->regs.isr1, &reg_val1);
		ret += regmap_read(fcp->regmap, g_buck_ops->regs.isr2, &reg_val2);
		if (reg_val1 != 0)
			ret += regmap_write(fcp->regmap, g_buck_ops->regs.isr1, reg_val1);
		if (reg_val2 != 0)
			ret += regmap_write(fcp->regmap, g_buck_ops->regs.isr2, reg_val2);

		ret += regmap_write(fcp->regmap, g_buck_ops->regs.cmd, CHG_FCP_CMD_SBRWR);
		ret += regmap_write(fcp->regmap, g_buck_ops->regs.addr, reg);

		ret += regmap_write(fcp->regmap, g_buck_ops->regs.data0, val);
		ret += regmap_update_bits(fcp->regmap, g_buck_ops->regs.cntl,
				 CHG_FCP_SNDCMD_MSK, CHG_FCP_EN);
		if (ret != 0) {
			scharger_err("%s: write error ret is %d\n", __func__, ret);
			mutex_unlock(&fcp->accp_adapter_reg_lock);
			return FCP_FAIL;
		}

		/* check cmd transfer success or fail */
		if (fcp_cmd_transfer_check(fcp) == 0)
			break;
		fcp_protocol_restart(fcp);
	}
	if (i >= FCP_RETRY_TIME) {
		scharger_err("%s:ack error,retry %d times\n", __func__, i);
		ret = FCP_FAIL;
	}

	mutex_unlock(&fcp->accp_adapter_reg_lock);
	return ret;
}

/*
 * Return: 0: success
 *        -1: other fail
 *         1:fcp adapter but detect fail
 */
static int fcp_detect_adapter(void *dev_data)
{
	unsigned int reg_val1 = 0;
	unsigned int reg_val2 = 0;
	int ret = 0;
	int i, vusb_plugout;
	struct fcp *fcp = (struct fcp *)dev_data;

	if (fcp == NULL) {
		scharger_err("%s, fcp is null.\n", __func__);
		return -ENOMEM;
	}

	if (g_buck_ops == NULL) {
		scharger_err("%s buck_ops is NULl!\n", __func__);
		return -1;
	}

	mutex_lock(&fcp->fcp_detect_lock);
	ret += regmap_read(fcp->regmap, g_buck_ops->regs.statuis, &reg_val2);

	scharger_err("%s:REG_STATUIS:0x%x\n", __func__, reg_val2);
	if (ret != 0) {
		scharger_err("%s:read det attach err,ret:%d\n", __func__, ret);
		mutex_unlock(&fcp->fcp_detect_lock);
		return -1;
	}

	if ((reg_val2 & (CHG_FCP_DVC_MSK | CHG_FCP_ATTATCH_MSK)) ==
		CHG_FCP_SLAVE_GOOD) {
		mutex_unlock(&fcp->fcp_detect_lock);
		scharger_inf("fcp adapter detect ok\n");
		return CHG_FCP_ADAPTER_DETECT_SUCC;
	}
	ret += regmap_update_bits(fcp->regmap, g_buck_ops->regs.fcp_ctrl,
			 CHG_FCP_DET_EN_MSK, TRUE << CHG_FCP_DET_EN_SHIFT);
	ret += regmap_update_bits(fcp->regmap, g_buck_ops->regs.fcp_ctrl,
			 CHG_FCP_CMP_EN_MSK, TRUE << CHG_FCP_CMP_EN_SHIFT);
	if (ret != 0) {
		scharger_err("%s:FCP enable detect fail,ret:%d\n", __func__, ret);
		(void)regmap_update_bits(fcp->regmap, g_buck_ops->regs.fcp_ctrl,
				 CHG_FCP_CMP_EN_MSK, FALSE << CHG_FCP_CMP_EN_SHIFT);
		(void)regmap_update_bits(fcp->regmap, g_buck_ops->regs.fcp_ctrl,
				 CHG_FCP_DET_EN_MSK, FALSE << CHG_FCP_DET_EN_SHIFT);

		mutex_unlock(&fcp->fcp_detect_lock);
		return -1;
	}
	/* wait for fcp_set */
	for (i = 0; i < CHG_FCP_DETECT_MAX_COUT; i++) {
		ret += regmap_read(fcp->regmap, g_buck_ops->regs.fcp_adap_ctrl, &reg_val1);
		vusb_plugout = is_vusb_plugout(fcp);
		scharger_err("%s:REG_FCP_ADAP_CTRL 0x%x\n", __func__, reg_val1);
		if (ret != 0) {
			scharger_err("%s:read det attach err,ret:%d\n", __func__, ret);
			continue;
		}
		if (vusb_plugout) {
			scharger_err("%s:vusb plugout, adapter plug out!\n", __func__);
			break;
		}
		if (reg_val1 & CHG_FCP_SET_STATUS_MSK)
			break;
		msleep(CHG_FCP_POLL_TIME);
	}
	if (i == CHG_FCP_DETECT_MAX_COUT || vusb_plugout) {
		regmap_update_bits(fcp->regmap, g_buck_ops->regs.fcp_ctrl, CHG_FCP_CMP_EN_MSK,
			 FALSE << CHG_FCP_CMP_EN_SHIFT);
		regmap_update_bits(fcp->regmap, g_buck_ops->regs.fcp_ctrl, CHG_FCP_DET_EN_MSK,
			 FALSE << CHG_FCP_DET_EN_SHIFT);
		mutex_unlock(&fcp->fcp_detect_lock);
		scharger_err("%s:CHG_FCP_ADAPTER_DETECT_OTHER return\n", __func__);
		return CHG_FCP_ADAPTER_DETECT_OTHER;
	}

	/* detect hisi fcp charger, enable fcp_en and wait for ping succ */
	ret = fcp_ping_success(fcp);
	mutex_unlock(&fcp->fcp_detect_lock);
	return ret;
}

/************base read write end****************************/
static int fcp_reg_read_block(int reg, int *val, int num, void *dev_data)
{
	int ret, i;
	u8 data = 0;
	struct fcp *fcp = (struct fcp *)dev_data;

	scharger_dbg("%s, reg 0x%x, num %d +\n", __func__, reg, num);

	if (fcp == NULL) {
		scharger_err("%s, fcp is null.\n", __func__);
		return -ENOMEM;
	}

	if (!val) {
		scharger_err("val is null\n");
		return -ENOMEM;
	}

	for (i = 0; i < num; i++) {
		ret = fcp_adapter_reg_read(fcp, &data, reg + i);
		if (ret != 0) {
			scharger_err("fcp read failed(reg=0x%x)\n", reg + i);
			return -1;
		}
		val[i] = data;
	}

	scharger_dbg("%s, reg 0x%x, val 0x%x, num %d -\n", __func__, reg, *val, num);
	return 0;
}

int scharger_scp_read_block(int reg, int *val, int num)
{
	struct fcp *fcp = g_scharger_fcp;

	if (fcp == NULL)
		return -ENOMEM;

	return fcp_reg_read_block(reg, val, num, fcp);
}

static int fcp_reg_write_block(int reg, const int *val, int num,
	void *dev_data)
{
	int ret, i;
	struct fcp *fcp = (struct fcp *)dev_data;

	if (fcp == NULL) {
		scharger_err("%s, fcp is null.\n", __func__);
		return -ENOMEM;
	}

	if (val == NULL) {
		scharger_err("%s val is null\n", __func__);
		return -1;
	}

	for (i = 0; i < num; i++) {
		ret = fcp_adapter_reg_write(fcp, val[i], reg + i);
		if (ret != 0) {
			scharger_err("%s fcp write failed(reg=0x%x)\n",__func__, reg + i);
			return -1;
		}
	}

	return 0;
}

/* 0: success, other: fail */
static int fcp_master_reset(void *dev_data)
{
	int ret = 0;
	struct fcp *fcp = (struct fcp *)dev_data;

	if (fcp == NULL) {
		scharger_err("%s, fcp is null.\n", __func__);
		return -ENOMEM;
	}

	if (g_buck_ops == NULL) {
		scharger_err("%s buck_ops is NULl!\n", __func__);
		return -1;
	}

	ret += regmap_write(fcp->regmap, g_buck_ops->regs.fcp_soft_rst_ctrl,
		 CHG_FCP_SOFT_RST_VAL);

	msleep(10); /* 10: need 10mS for next read */
	ret += regmap_write(fcp->regmap, g_buck_ops->regs.fcp_soft_rst_ctrl,
		 CHG_FCP_SOFT_RST_DEFAULT);
	/* clear fcp_en and fcp_master_rst */
	ret += regmap_write(fcp->regmap, g_buck_ops->regs.cntl, 0);
	if (ret != 0)
		return -1;

	return 0;
}

/* 0: success, other: fail */
static int fcp_adapter_reset(void *dev_data)
{
	int ret;
	struct fcp *fcp = (struct fcp *)dev_data;

	if (fcp == NULL) {
		scharger_err("%s, fcp is null.\n", __func__);
		return -ENOMEM;
	}

	if (g_buck_ops == NULL) {
		scharger_err("%s buck_ops is NULl!\n", __func__);
		return -1;
	}

	if (g_buck_ops->set_vbus_vset != NULL)
		g_buck_ops->set_vbus_vset(VBUS_VSET_5V);

	ret = regmap_write(fcp->regmap, g_buck_ops->regs.cntl, CHG_FCP_EN_MSK |
			    CHG_FCP_MSTR_RST_MSK);
	if (ret != 0) {
		scharger_err("%s:send rst cmd failed\n", __func__);
		return ret;
	}

	ret = fcp_adapter_vol_check(fcp, FCP_ADAPTER_RST_VOL);
	if (ret != 0) {
		/* clear fcp_en and fcp_master_rst */
		ret = regmap_write(fcp->regmap, g_buck_ops->regs.cntl, 0);
		scharger_err("%s:adc check adapter output voltage failed\n", __func__);
		if (ret != 0)
			return ret;
		return ret;
	}
	/* clear fcp_en and fcp_master_rst */
	ret = regmap_write(fcp->regmap, g_buck_ops->regs.cntl, 0);
	if (ret != 0)
		return -1;
	return 0;
}

static int fcp_read_switch_status(void *dev_data)
{
	return 0;
}

static int fcp_stop_charge_config(void *dev_data)
{
	struct fcp *fcp = (struct fcp *)dev_data;

	if (fcp == NULL) {
		scharger_err("%s, fcp is null.\n", __func__);
		return -ENOMEM;
	}

	if (g_buck_ops == NULL) {
		scharger_err("%s buck_ops is NULl!\n", __func__);
		return -1;
	}

	scharger_dbg("fcp_master_reset");
	(void)fcp_master_reset(dev_data);
	regmap_update_bits(fcp->regmap, g_buck_ops->regs.fcp_ctrl, CHG_FCP_DET_EN_MSK, FALSE);

	return 0;
}

static int is_fcp_charger_type(void *dev_data)
{
	unsigned int reg_val = 0;
	int ret;
	struct fcp *fcp = (struct fcp *)dev_data;

	if (fcp == NULL) {
		scharger_err("%s, fcp is null.\n", __func__);
		return -ENOMEM;
	}

	if (g_buck_ops == NULL) {
		scharger_err("%s buck_ops is NULl!\n", __func__);
		return -1;
	}

	ret = regmap_read(fcp->regmap, g_buck_ops->regs.fcp_adap_ctrl, &reg_val);
	if (ret != 0) {
		scharger_err("%s reg read fail!\n", __func__);
		return FALSE;
	}
	if ((reg_val & FCP_SET_D60M_R_MSK))
		return TRUE;
	return FALSE;
}

#ifdef CONFIG_DFX_DEBUG_FS
int fcp_drv_test_detect_adapter(void)
{
	int ret;
	ret = fcp_detect_adapter(g_scharger_fcp);
	if (ret != 0)
		scharger_err("%s fcp_detect_adapter fail\n", __func__);

	return ret;
}

int fcp_drv_test_read(int reg)
{
	int ret;
	unsigned int val = 0;
	ret = fcp_adapter_reg_read(g_scharger_fcp, (u8 *)&val, (u8)reg);
	if (ret != 0) {
		scharger_err("%s fcp_adapter_reg_read fail\n", __func__);
		return ret;
	}
	scharger_err("%s reg:%x,val:%x\n", __func__, reg, val);

	return val;
}

int fcp_drv_test_write(int reg, int val)
{
	int ret;

	ret = fcp_adapter_reg_write(g_scharger_fcp, (u8)val, (u8)reg);
	if (ret != 0) {
		scharger_err("%s fcp_adapter_reg_write fail\n", __func__);
		return ret;
	}
	scharger_err("%s reg:%x,val:%x\n", __func__, reg, val);

	return fcp_drv_test_read(reg);
}
#endif

static int fcp_adapter_reg_multi_read(u8 reg, u8 *val, u8 num, void *dev_data)
{
	int ret = 0;
	int i;
	unsigned int reg_val1 = 0;
	unsigned int reg_val2 = 0;
	unsigned int data_len;
	u8 *p = NULL;
	struct fcp *fcp = (struct fcp *)dev_data;

	if (fcp == NULL) {
		scharger_err("%s, fcp is null.\n", __func__);
		return -ENOMEM;
	}

	if (!val) {
		scharger_err("%s val is NULL!\n", __func__);
		return -ENOMEM;
	}

	if (g_buck_ops == NULL) {
		scharger_err("%s buck_ops is NULl!\n", __func__);
		return -1;
	}

	mutex_lock(&fcp->accp_adapter_reg_lock);

	data_len = (num < FCP_DATA_LEN) ? num : FCP_DATA_LEN;
	p = val;

	for (i = 0; i < FCP_RETRY_TIME; i++) {
		/* before send cmd, clear accp interrupt registers */
		ret  += regmap_read(fcp->regmap, g_buck_ops->regs.isr1, &reg_val1);
		ret  += regmap_read(fcp->regmap, g_buck_ops->regs.isr2, &reg_val2);

		if (reg_val1 != 0)
			ret += regmap_write(fcp->regmap, g_buck_ops->regs.isr1, reg_val1);
		if (reg_val2 != 0)
			ret += regmap_write(fcp->regmap, g_buck_ops->regs.isr2, reg_val2);

		ret += regmap_write(fcp->regmap, g_buck_ops->regs.cmd, CHG_FCP_CMD_MBRRD);
		ret += regmap_write(fcp->regmap, g_buck_ops->regs.addr, reg);
		ret += regmap_write(fcp->regmap, g_buck_ops->regs.fcp_length, data_len);
		ret += regmap_update_bits(fcp->regmap, g_buck_ops->regs.cntl, CHG_FCP_SNDCMD_MSK, 1);
		if (ret != 0) {
			scharger_err("%s: read error ret is %d\n", __func__, ret);
			mutex_unlock(&fcp->accp_adapter_reg_lock);
			return FCP_FAIL;
		}

		/* check cmd transfer success or fail */
		if (fcp_cmd_transfer_check(fcp) == 0) {
			/* recived data from adapter */
			ret += regmap_bulk_read(fcp->regmap, g_buck_ops->regs.fcp_rdata0, p, data_len);
			break;
		}
		fcp_protocol_restart(fcp);
		if (is_vusb_plugout(fcp))
			i = FCP_RETRY_TIME;
	}
	if (i >= FCP_RETRY_TIME) {
		scharger_err("%s:ack error,retry %d times\n", __func__, i);
		ret = -1;
	}
	mutex_unlock(&fcp->accp_adapter_reg_lock);

	if (ret != 0)
		return ret;

	num -= data_len;
	if (num) {
		p += data_len;
		reg += data_len;
		ret = fcp_adapter_reg_multi_read(reg, p, num, fcp);
		if (ret != 0) {
			scharger_err("%s: read error ret is %d\n", __func__, ret);
			return -1;
		}
	}

	return 0;
}

static struct hwfcp_ops fcp_protocol_ops = {
	.chip_name = "scharger_v700",
	.reg_read = fcp_reg_read_block,
	.reg_write = fcp_reg_write_block,
	.detect_adapter = fcp_detect_adapter,
	.soft_reset_master = fcp_master_reset,
	.soft_reset_slave = fcp_adapter_reset,
	.get_master_status = fcp_read_switch_status,
	.stop_charging_config = fcp_stop_charge_config,
	.is_accp_charger_type = is_fcp_charger_type,
};

static int fcp_pre_init(void *dev_data)
{
	return 0;
}

static struct hwscp_ops scp_protocol_ops = {
	.chip_name = "scharger_v700",
	.reg_read = fcp_reg_read_block,
	.reg_write = fcp_reg_write_block,
	.reg_multi_read = fcp_adapter_reg_multi_read,
	.detect_adapter = fcp_detect_adapter,
	.soft_reset_master = fcp_master_reset,
	.soft_reset_slave = fcp_adapter_reset,
	.pre_init = fcp_pre_init,
};

static int fcp_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct fcp *fcp = NULL;
	int ret = 0;

	fcp = devm_kzalloc(dev, sizeof(*fcp), GFP_KERNEL);
	if (fcp == NULL) {
		dev_err(dev, "[%s]fcp is null.\n", __func__);
		return -ENOMEM;
	}

	fcp->dev = dev;
	fcp->regmap = dev_get_regmap(dev->parent, NULL);
	if (!fcp->regmap) {
		dev_err(dev, "Parent regmap unavailable.\n");
		return -ENXIO;
	}

	mutex_init(&fcp->accp_adapter_reg_lock);
	mutex_init(&fcp->fcp_detect_lock);

	fcp_protocol_ops.dev_data = (void *)fcp;
	scp_protocol_ops.dev_data = (void *)fcp;

	hwfcp_ops_register(&fcp_protocol_ops);
	hwscp_ops_register(&scp_protocol_ops);
	g_scharger_fcp = fcp;

	pr_info("%s ret %d\n", __func__, ret);
	return ret;
}

static void fcp_shutdown(struct platform_device *pdev)
{
	g_scharger_fcp = NULL;
}

const static struct of_device_id of_scharger_fcp_match_tbl[] = {
	{
		.compatible = "hisilicon,scharger-fcp-drv",
		.data = NULL,
	},
	{},
};

static struct platform_driver fcp_driver = {
	.driver = {
		.name = "scharger_fcp_drv",
		.owner = THIS_MODULE,
		.of_match_table = of_scharger_fcp_match_tbl,
	},
	.probe = fcp_probe,
	.shutdown = fcp_shutdown,
};

static int __init fcp_init(void)
{
	return platform_driver_register(&fcp_driver);
}

static void __exit fcp_exit(void)
{
	platform_driver_unregister(&fcp_driver);
}

fs_initcall(fcp_init);
module_exit(fcp_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("fcp protocol driver");
