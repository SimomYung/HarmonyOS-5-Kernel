/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * I2C adapter driver.
 *
 * Copyright (c) 2006 Texas Instruments.
 * Copyright (c) 2007 MontaVista Software Inc.
 * Copyright (c) 2009 Provigent Ltd.
 *
 * ----------------------------------------------------------------------------
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * ----------------------------------------------------------------------------
 *
 */

#ifndef __PCIE_I2C_MASTER_H_
#define __PCIE_I2C_MASTER_H_

#include <linux/clk.h>
#include <linux/i2c.h>
#include <linux/pm_qos.h>

#define I2C_DEFAULT_FUNCTIONALITY (I2C_FUNC_I2C | I2C_FUNC_SMBUS_BYTE | I2C_FUNC_SMBUS_BYTE_DATA |       \
	I2C_FUNC_SMBUS_WORD_DATA | I2C_FUNC_SMBUS_BLOCK_DATA | I2C_FUNC_SMBUS_I2C_BLOCK)

#define I2C_SPEED_MASK              0x3
#define I2C_SPEED_STD               0x0
#define I2C_SPEED_FS                0x1
#define I2C_SPEED_HIGH              0x2

#define I2C_SPEED_STANDARD             100
#define I2C_SPEED_FAST                 400
#define I2C_SPEED_FAST_PLUS            1000

#define I2C_MUL_1K                     1000

#define I2C_SS_FREQ_DIV_H              ((40 * 1000) / (40 + 47))
#define I2C_FS_FREQ_DIV_H              ((6 * 1000) / (6 + 13))
#define I2C_FSP_FREQ_DIV_H             ((26 * 1000) / (26 + 50))
#define I2C_SCL_FALL_TIME              0
#define I2C_SCL_RISE_TIME              0
#define I2C_HCNT_RESERVED              7
#define I2C_SPK_CNT                    1

#define TIMEOUT                        3
#define STATUS_IDLE                    0x0
#define STATUS_WRITE_IN_PROGRESS       0x1
#define STATUS_READ_IN_PROGRESS        0x2
#define I2C_RECOVER_IOMG_CNT           4
#define I2C_RECOVER_VAL_CNT            3
#define I2C_TIMEOUT_DEFAULT_VAL        100
#define I2C_PROPER_RETRIES             3
#define I2C_RECOVER_SLEEP_MS           100
#define GET_DEV_LOCK_TIMEOUT           500
#define WAIT_FOR_COMPLETION            (HZ)

#define I2C_FIFO_TX_DEPTH              64
#define I2C_FIFO_RX_DEPTH              64

#define ERROR_TXDATA_NOACK             0
#define ERROR_10B_RD_NORSTRT           1
#define ERROR_10ADDR2_NOACK            2
#define ERROR_10ADDR1_NOACK            3
#define ERROR_7B_ADDR_NOACK            4
#define ERROR_HS_ACKDET                5
#define ERROR_HS_NORSTRT               6
#define ERROR_SBYTE_NORSTRT            7
#define ERROR_SBYTE_ACKDET             8
#define ERROR_GCALL_READ               9
#define ERROR_GCALL_NOACK              10

#define I2C_TRANS_ERROR_GCALL_READ     0x200 /* BIT(ERROR_GCALL_READ) */
#define I2C_TRANS_ERROR_7B_ADDR_NOACK  0x10  /* BIT(ERROR_7B_ADDR_NOACK) */
#define I2C_TRANS_ERROR_10ADDR1_NOACK  0x8   /* BIT(ERROR_10ADDR1_NOACK) */
#define I2C_TRANS_ERROR_10ADDR2_NOACK  0x4   /* BIT(ERROR_10ADDR2_NOACK) */
#define I2C_TRANS_ERROR_TXDATA_NOACK   0x1   /* BIT(ERROR_TXDATA_NOACK) */
#define I2C_TRANS_ERROR_GCALL_NOACK    0x400 /* BIT(ERROR_GCALL_NOACK) */

#define I2C_TRANS_ERROR_NOACK (I2C_TRANS_ERROR_7B_ADDR_NOACK | I2C_TRANS_ERROR_10ADDR1_NOACK | \
	I2C_TRANS_ERROR_10ADDR2_NOACK | I2C_TRANS_ERROR_TXDATA_NOACK | I2C_TRANS_ERROR_GCALL_NOACK)

/*
 * Registers offset
 */
#define FRAME_CTRL                     0x0
#define SLV_ADDR                       0x4
#define CMD_TXDATA                     0x8
#define RXDATA                         0xC
#define SS_SCL_HCNT                    0x10
#define SS_SCL_LCNT                    0x14
#define FS_SCL_HCNT                    0x18
#define FS_SCL_LCNT                    0x1C
#define HS_SCL_HCNT                    0x20
#define HS_SCL_LCNT                    0x24
#define FIFO_CTRL                      0x28
#define FIFO_STATE                     0x2C
#define SDA_HOLD                       0x30
#define DMA_CTRL                       0x34
#define FS_SPK_LEN                     0x38
#define HS_SPK_LEN                     0x3C
#define INT_MSTAT                      0x44
#define INT_CLR                        0x48
#define INT_MASK                       0x4C
#define TRANS_STATE                    0x50
#define TRANS_ERR                      0x54

/*
 * Registers config
 */
#define FIFO_CLEAR                     0x3
#define FIFO_TX_THD                    0xF00
#define FIFO_RX_THD                    0x3C
#define I2C_DEFULT_HOLD_TIME           0x1
#define TRANS_STATE_MASK               0x1F
#define PIN_STATE_MASK                 0x60
#define I2C_PIN_STATUS                 0x60
#define I2C_TAR_RECOVERY_CFG           0x7F
#define I2C_RD_CFG                     BIT(8)
#define I2C_RX_EMPTY                   BIT(3)
#define I2C_RX_AFULL                   BIT(4)
#define I2C_RX_FULL                    BIT(5)
#define I2C_TX_AFULL                   BIT(10)
#define I2C_TX_FULL                    BIT(11)
#define I2C_RESTART_EN                 BIT(3)
#define I2C_STATUS_ACTIVITY            BIT(0)
#define I2C_CON_10BITADDR_MASTER       BIT(2)
#define I2C_FIFO_TX_EMPTY              BIT(9)
#define I2C_READ_CMD                   BIT(8)
#define I2C_IRQ_STOP                   BIT(9)
#define I2C_MAX_DMA_BUF_LEN            (60 * 1024)
#define I2C_DMA_ENABLE                 BIT(0)
#define I2C_DMA_DISABLE                0
#define I2C_INT_RX_FULL                BIT(3)
#define I2C_INT_TX_EMPTY               BIT(4)
#define I2C_INT_CLEAR                  0xFFFF
#define I2C_INT_TRCMP                  0x1
#define I2C_INT_TRERR                  0x2
#define I2C_INT_FIFOERR                0x4
#define I2C_INT_RX_AFULL               0x8
#define I2C_INT_TX_AEMPTY              0x10
#define I2C_INTR_DEFAULT_MASK          (I2C_INT_RX_AFULL | I2C_INT_TRERR | I2C_INT_TRCMP)
#define I2C_INTR_MASTER_MASK           (I2C_INT_RX_AFULL | I2C_INT_TRERR | I2C_INT_TRCMP | I2C_INT_TX_AEMPTY)

#ifndef unused
#define unused(x) ((void)(x))
#endif

#define IODIE_PCIE_STATUS_OFFSET 0x46C
#define IODIE_PCIE_STATUS_MASK 0x80000000

struct pcie_i2c_sda_iomg {
	u32 iomg_offset;
	u32 iomg_func_i2c;
	u32 iomg_pad_func_i2c;
	u32 iomg_func_gpio;
};

struct pcie_i2c_dev {
	struct device *dev;
	void __iomem *base;
	struct completion cmd_complete;
	unsigned int cmd_err;
	struct i2c_msg *msgs;
	int msgs_num;
	int msg_write_idx;
	u32 tx_buf_len;
	u8 *tx_buf;
	int msg_read_idx;
	u32 rx_buf_len;
	u8 *rx_buf;
	int msg_err;
	unsigned int status;
	u32 error_source;
	int irq_bit;
	struct i2c_adapter adapter;
	u32 functionality;
	u32 master_cfg;
	int rx_outstanding;
	u64 clk_rate;
	spinlock_t spinlock;
	struct mutex lock;
	resource_size_t mapbase;
	void (*recover_bus)(struct i2c_adapter *);
	u32 hold_time;
	int timeout_count;
	void __iomem *iomg_base;
	void __iomem *iomg_pad_base;
	struct pcie_i2c_sda_iomg sda_ioc;
	bool initd;
};
#endif

