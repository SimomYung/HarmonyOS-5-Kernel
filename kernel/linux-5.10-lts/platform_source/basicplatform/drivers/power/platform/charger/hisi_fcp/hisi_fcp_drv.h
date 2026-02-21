/*
 * hisi_ufcs_drv.h
 *
 * hisi fcp driver
 *
 * Copyright (c) 2023-2023 Huawei Technologies Co., Ltd.
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

#ifndef _HISI_FCP_DRV_H_
#define _HISI_FCP_DRV_H_

#include <linux/device.h>
#include <linux/regmap.h>

struct reg_info {
	u32 reg;
	u8 mask;
};

struct fcp_reg {
	struct reg_info irq_vusb_plugout;
	u32 cntl;
	u32 statuis;
	u32 fcp_ctrl;
	u32 isr1;
	u32 isr2;
	u32 fcp_irq3;
	u32 fcp_irq4;
	u32 cmd;
	u32 addr;
	u32 fcp_rdata0;
	u32 data0;
	u32 fcp_adap_ctrl;
	u32 fcp_soft_rst_ctrl;
	u32 fcp_length;
};

struct fcp_buck_ops {
	struct fcp_reg regs;
	int (*set_vbus_vset)(u32 value);
	int (*get_vbus)(void);
};

struct fcp {
	struct device *dev;
	struct regmap *regmap;
	unsigned int base;
	struct mutex fcp_detect_lock;
	struct mutex accp_adapter_reg_lock;
};

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define FCP_ADAPTER_5V                  5000
#define FCP_ADAPTER_9V                  9000
#define FCP_ADAPTER_12V                 12000
#define FCP_ADAPTER_MAX_VOL             (FCP_ADAPTER_12V)
#define FCP_ADAPTER_MIN_VOL             (FCP_ADAPTER_5V)
#define FCP_ADAPTER_RST_VOL             (FCP_ADAPTER_5V)
#define FCP_ADAPTER_VOL_CHECK_ERROR     500
#define FCP_ADAPTER_VOL_CHECK_POLLTIME  100
#define FCP_ADAPTER_VOL_CHECK_TIMEOUT   10

#define FCP_ACK_RETRY_CYCLE             10

#define FCP_RESTART_TIME                4
#define FCP_RETRY_TIME                  5
#define FCP_FAIL                        (-1)

#define CHG_FCP_ADAPTER_DETECT_FAIL     1
#define CHG_FCP_ADAPTER_DETECT_SUCC     0
#define CHG_FCP_ADAPTER_DETECT_OTHER    (-1)
#define CHG_FCP_POLL_TIME               100 /* 100ms */
#define CHG_FCP_DETECT_MAX_COUT         20 /* fcp detect MAX COUT */

#define CHG_FCP_ATTATCH_SHIFT           (0)
#define CHG_FCP_ATTATCH_MSK             (0x1 << CHG_FCP_ATTATCH_SHIFT)
#define CHG_FCP_DVC_SHIFT               (6)
#define CHG_FCP_DVC_MSK                 (0x3 << CHG_FCP_DVC_SHIFT)
#define CHG_FCP_SLAVE_GOOD              (CHG_FCP_DVC_MSK | CHG_FCP_ATTATCH_MSK)

#define CHG_FCP_SET_STATUS_SHIFT        (0)
#define CHG_FCP_SET_STATUS_MSK          (0x1 << CHG_FCP_SET_STATUS_SHIFT)

#define CHG_FCP_CMDCPL                  (1 << 7)
#define CHG_FCP_ACK                     (1 << 6)
#define CHG_FCP_NACK                    (1 << 4)
#define CHG_FCP_CRCPAR                  (1 << 3)

#define CHG_FCP_CRCRX                   (1 << 4)
#define CHG_FCP_PARRX                   (1 << 3)
#define CHG_FCP_PROTSTAT                (1 << 1)

#define CHG_FCP_TAIL_HAND_FAIL          (1 << 1)
#define CHG_FCP_INIT_HAND_FAIL          (1 << 3)
#define CHG_FCP_ENABLE_HAND_FAIL        (1 << 4)

#define VBUS_VSET_5V                    5
#define VBUS_VSET_9V                    9
#define VBUS_VSET_12V                   12
#define FCP_DATA_LEN                    8

#define CHG_FCP_SNDCMD_SHIFT            (0)
#define CHG_FCP_SNDCMD_MSK              (0x1 << CHG_FCP_SNDCMD_SHIFT)
#define CHG_FCP_MSTR_RST_SHIFT          (2)
#define CHG_FCP_MSTR_RST_MSK            (0x1 << CHG_FCP_MSTR_RST_SHIFT)

#define CHG_FCP_EN                      1

#define CHG_FCP_EN_SHIFT                (3)
#define CHG_FCP_EN_MSK                  (0x1 << CHG_FCP_EN_SHIFT)

#define CHG_FCP_CMP_EN_SHIFT            (2)
#define CHG_FCP_CMP_EN_MSK              (0x01 << CHG_FCP_CMP_EN_SHIFT)
#define CHG_FCP_DET_EN_SHIFT            (3)
#define CHG_FCP_DET_EN_MSK              (0x01 << CHG_FCP_DET_EN_SHIFT)

#define FCP_SET_D60M_R_SHIFT            (0)
#define FCP_SET_D60M_R_MSK              (0 << FCP_SET_D60M_R_SHIFT)

#define FCP_RETRY_TIME     5

/* adapter cmd */
#define CHG_FCP_CMD_SBRRD               0x0c
#define CHG_FCP_CMD_SBRWR               0x0b
#define CHG_FCP_CMD_MBRRD               0x1c
#define CHG_FCP_CMD_MBRWR               0x1b

#define CHG_FCP_SOFT_RST_VAL            0xAC
#define CHG_FCP_SOFT_RST_DEFAULT        0x5A

int scharger_scp_read_block(int reg, int *val, int num);
int scharger_fcp_buck_ops_register(struct fcp_buck_ops *ops);
#endif