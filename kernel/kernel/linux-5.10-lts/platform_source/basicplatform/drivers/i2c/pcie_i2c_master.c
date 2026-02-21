/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 *
 * I2C Master adapter driver (master only).
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

#include <linux/delay.h>
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/i2c.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/sched.h>
#include <linux/slab.h>
#ifdef CONFIG_I2C_FFA_SUPPORT
#include <platform_include/see/ffa/ffa_plat_drv.h>
#include <platform_include/see/ffa/ffa_msg_id.h>
#endif
#include <platform_include/smart/drivers/io_die/int_parse.h>
#include <platform_include/smart/drivers/io_die/iodie_pcie.h>
#include "pcie_i2c_master.h"


static char *error_sources[] = {
	[ERROR_7B_ADDR_NOACK] =
		"slave address not acknowledged (7bit mode)",
	[ERROR_10ADDR1_NOACK] =
		"first address byte not acknowledged (10bit mode)",
	[ERROR_10ADDR2_NOACK] =
		"second address byte not acknowledged (10bit mode)",
	[ERROR_TXDATA_NOACK] =
		"data not acknowledged",
	[ERROR_GCALL_NOACK] =
		"no acknowledgement for a general call",
	[ERROR_GCALL_READ] =
		"read after general call",
	[ERROR_HS_ACKDET] =
	"the high-speed master code was achnowledged",
	[ERROR_SBYTE_ACKDET] =
		"start byte acknowledged",
	[ERROR_HS_NORSTRT] =
	"the restart is disabled and the user is trying to use the master to transfer data in high-speed mode",
	[ERROR_SBYTE_NORSTRT] =
		"trying to send start byte when restart is disabled",
};

static u32 pcie_i2c_readl(struct pcie_i2c_dev *d, u32 offset)
{
	u32 value;

	value = readl_relaxed(d->base + offset);

	return value;
}

static void pcie_i2c_writel(struct pcie_i2c_dev *d, u32 val, u32 offset)
{
	writel_relaxed(val, d->base + offset);
}
 
static void pcie_i2c_reset_controller(struct pcie_i2c_dev *d)
{
#ifdef CONFIG_I2C_FFA_SUPPORT
	int ret;
	struct ffa_send_direct_data args = {0};

	args.data0 = IOMCU_I2C_RST_FID;
	args.data1 = d->mapbase;

	ret = ffa_platdrv_send_msg(&args);
	if (ret != 0)
		dev_err(d->dev, "%s failed to send_msg, ret:%d!\n", __func__, ret);
#else
	unused(d);
#endif
}

static void pcie_i2c_fifo_clear(struct pcie_i2c_dev *d)
{
	u32 fifo_ctrl = 0;

	fifo_ctrl = pcie_i2c_readl(d, FIFO_CTRL);
	fifo_ctrl |= FIFO_CLEAR;
	pcie_i2c_writel(d, fifo_ctrl, FIFO_CTRL);
	fifo_ctrl = fifo_ctrl & (~FIFO_CLEAR);
	pcie_i2c_writel(d, fifo_ctrl, FIFO_CTRL);
}

static void pcie_i2c_speed_config(struct pcie_i2c_dev *d)
{
	u32 hcnt, lcnt;
	u32 speed_mode;
	u32 min_cnt;
	u32 input_clock_khz;

	/* set i2c speed */
	speed_mode = d->master_cfg & I2C_SPEED_MASK;
	input_clock_khz = d->clk_rate / I2C_MUL_1K;

	/* minimum value of hcnt, commonly used to SS/FS/pcie */
	min_cnt = I2C_SPK_CNT + I2C_HCNT_RESERVED;

	/* standard-mode */
	hcnt = ((input_clock_khz / I2C_SPEED_STANDARD) * I2C_SS_FREQ_DIV_H) / I2C_MUL_1K \
	- I2C_SCL_FALL_TIME * input_clock_khz * I2C_MUL_1K;
	lcnt = (input_clock_khz / I2C_SPEED_STANDARD) - hcnt \
		- I2C_SCL_RISE_TIME * input_clock_khz * I2C_MUL_1K;
	hcnt = (hcnt > min_cnt) ? (hcnt - min_cnt) : 1;
	pcie_i2c_writel(d, hcnt, SS_SCL_HCNT);
	pcie_i2c_writel(d, lcnt, SS_SCL_LCNT);
	dev_info(d->dev, "i2c std speed hcnt %u lcnt %u\n", hcnt, lcnt);

	/* fast-mode */
	hcnt = ((input_clock_khz / I2C_SPEED_FAST) * I2C_FS_FREQ_DIV_H) / I2C_MUL_1K \
		- I2C_SCL_FALL_TIME * input_clock_khz * I2C_MUL_1K;
	lcnt = (input_clock_khz/ I2C_SPEED_FAST) - hcnt \
		- I2C_SCL_RISE_TIME * input_clock_khz * I2C_MUL_1K;
	hcnt = (hcnt > min_cnt) ? (hcnt - min_cnt) : 1;
	pcie_i2c_writel(d, hcnt, FS_SCL_HCNT);
	pcie_i2c_writel(d, lcnt, FS_SCL_LCNT);
	dev_info(d->dev, "i2c fast speed hcnt %u lcnt %u\n", hcnt, lcnt);

	/* fast-plus-mode */
	hcnt = ((input_clock_khz / I2C_SPEED_FAST_PLUS) * I2C_FSP_FREQ_DIV_H) / I2C_MUL_1K \
		- I2C_SCL_FALL_TIME * input_clock_khz * I2C_MUL_1K;
	lcnt = (input_clock_khz / I2C_SPEED_FAST_PLUS) - hcnt \
		- I2C_SCL_RISE_TIME * input_clock_khz * I2C_MUL_1K;
	hcnt = (hcnt > min_cnt) ? (hcnt - min_cnt) : 1;
	pcie_i2c_writel(d, hcnt, HS_SCL_HCNT);
	pcie_i2c_writel(d, lcnt, HS_SCL_LCNT);
	dev_info(d->dev, "i2c fast-plus speed hcnt %u lcnt %u\n", hcnt, lcnt);
};

/**
 * pcie_i2c_init_master() - Initialize the I2C master hardware
 * @d: device private data
 *
 * This functions configures and enables the I2C master.
 * This function is called during I2C init function, and in case of timeout at
 * run time.
 */
static void pcie_i2c_init_master(struct pcie_i2c_dev *d)
{
	pcie_i2c_speed_config(d);

	/* Configure SDA Hold Time */
	pcie_i2c_writel(d, d->hold_time, SDA_HOLD);

	/* configure the i2c master */
	pcie_i2c_writel(d, d->master_cfg, FRAME_CTRL);

	pcie_i2c_fifo_clear(d);

	/* Configure FIFO */
	pcie_i2c_writel(d, FIFO_TX_THD | FIFO_RX_THD, FIFO_CTRL);
}

static void pcie_i2c_disable_int(struct pcie_i2c_dev *d)
{
	pcie_i2c_writel(d, 0, INT_MASK);
}

static void reset_i2c_controller(struct pcie_i2c_dev *d)
{
	spin_lock(&d->spinlock);

	pcie_i2c_reset_controller(d);
	pcie_i2c_init_master(d);
	pcie_i2c_disable_int(d);

	spin_unlock(&d->spinlock);
}

static void i2c_scl_recover_bus(struct i2c_adapter *adap)
{
	struct pcie_i2c_dev *d = i2c_get_adapdata(adap);
	struct pcie_i2c_sda_iomg *sda_ioc = NULL;

	if (!d) {
		pr_err("%s:i2c ctrler doesn't be init\n", __func__);
		return;
	}

	if (d->iomg_base == NULL) {
		dev_err(d->dev, "%s:i2c iomg doesn't be remap\n", __func__);
		return;
	}

	sda_ioc = &d->sda_ioc;
	if ((!sda_ioc) || (sda_ioc->iomg_func_i2c == 0)) {
		dev_err(d->dev, "%s:i2c iomg_func or iomg_pad_func err\n", __func__);
		return;
	}
	dev_info(d->dev, "bus prepares recovery ...\n");

	/* switch sda to gpio */
	writel(sda_ioc->iomg_func_gpio, d->iomg_base + sda_ioc->iomg_offset);

	udelay(10);
	/* config slave address to 0x7F */
	pcie_i2c_writel(d, I2C_TAR_RECOVERY_CFG, SLV_ADDR);

	/* recived data from bus */
	pcie_i2c_writel(d, I2C_RD_CFG, CMD_TXDATA);

	msleep(I2C_RECOVER_SLEEP_MS);

	/* switch sda to i2c */
	writel(sda_ioc->iomg_func_i2c, d->iomg_base + sda_ioc->iomg_offset);

	pcie_i2c_init_master(d);

	dev_info(d->dev, "bus recovered completely!\n");
}

/*
 * Waiting for bus not busy
 */
static int pcie_i2c_wait_bus_not_busy(struct pcie_i2c_dev *d)
{
	int timeout = TIMEOUT;
	u32 stat;

	while (timeout > 0) {
		stat = pcie_i2c_readl(d, TRANS_STATE);
		if (!(stat & TRANS_STATE_MASK))
			return 0;
		timeout--;
		usleep_range(1000, 1100);
	}

	dev_err(d->dev, "timeout waiting for bus ready 0x%x\n", stat);

	reset_i2c_controller(d);

	return -ETIMEDOUT;
}

static void pcie_i2c_xfer_init(struct pcie_i2c_dev *d)
{
	struct i2c_msg *msgs = d->msgs;
	u32 ic_con;

	/* If the slave address is ten bit address, enable 10BITADDR */
	ic_con = pcie_i2c_readl(d, FRAME_CTRL);
	if (msgs[d->msg_write_idx].flags & I2C_M_TEN)
		ic_con |= I2C_CON_10BITADDR_MASTER;
	else
		ic_con &= ~I2C_CON_10BITADDR_MASTER;

	pcie_i2c_writel(d, ic_con, FRAME_CTRL);
	pcie_i2c_writel(d, msgs[d->msg_write_idx].addr, SLV_ADDR);

	/* Enforce disabled interrupts (due to HW issues) */
	pcie_i2c_disable_int(d);

	/* Clear and enable interrupts */
	pcie_i2c_writel(d, I2C_INT_CLEAR, INT_CLR);
	pcie_i2c_writel(d, I2C_INTR_DEFAULT_MASK, INT_MASK);
}

static int pcie_i2c_msg_check(struct pcie_i2c_dev *d, struct i2c_msg *msgs)
{
	u32 addr = msgs[d->msg_write_idx].addr;
	int ret = 0;
	/*
	 * If target address has changed, we need to
	 * reprogram the target address in the I2C
	 * adapter when we are done with this transfer.
	 */
	if (msgs[d->msg_write_idx].addr != addr) {
		dev_err(d->dev,
			"%s: invalid target address\n", __func__);
		d->msg_err = -EINVAL;
		return -EINVAL;
	}

	if (msgs[d->msg_write_idx].len == 0) {
		dev_err(d->dev,
			"%s: invalid message length\n", __func__);
		d->msg_err = -EINVAL;
		return -EINVAL;
	}

	return ret;
}

static void pcie_i2c_send(struct pcie_i2c_dev *d)
{
	struct i2c_msg *msgs = d->msgs;
	unsigned int tx_limit, rx_limit;
	u32 buf_len = d->tx_buf_len;
	u8 *buf = d->tx_buf;
	bool need_restart = false;
	int ret;
	u32 flags = msgs[d->msg_write_idx].flags;

	for (; d->msg_write_idx < d->msgs_num; d->msg_write_idx++) {
		ret = pcie_i2c_msg_check(d, msgs);
		if (ret)
			break;
		if ((d->status & STATUS_WRITE_IN_PROGRESS) == 0) {
			/* new i2c_msg */
			buf = msgs[d->msg_write_idx].buf;
			buf_len = msgs[d->msg_write_idx].len;

			/* If both IC_EMPTYFIFO_HOLD_MASTER_EN and
			* IC_RESTART_EN are set, we must manually
			* set restart bit between messages.
			*/
			if ((d->master_cfg & I2C_RESTART_EN) &&
					(d->msg_write_idx > 0))
				need_restart = true;
		}
		tx_limit = I2C_FIFO_TX_DEPTH;
		rx_limit = I2C_FIFO_RX_DEPTH;

		while (buf_len > 0 && tx_limit > 0 && rx_limit > 0) {
			u32 cmd = 0;
			u32 stat;

			stat = pcie_i2c_readl(d, FIFO_STATE);
			if (stat & (I2C_TX_FULL | I2C_RX_FULL))
				break;
			/*
			 * If IC_EMPTYFIFO_HOLD_MASTER_EN is set we must
			 * manually set the stop bit. However, it cannot be
			 * detected from the registers so we set it always
			 * when writing/reading the last byte.
			 */

			if (d->msg_write_idx == d->msgs_num - 1 &&
				buf_len == 1 && !(flags & I2C_M_RECV_LEN))
				cmd |= BIT(9);

			if (need_restart) {
				cmd |= BIT(10);
				need_restart = false;
			}

			if (msgs[d->msg_write_idx].flags & I2C_M_RD) {
				pcie_i2c_writel(d, cmd | I2C_READ_CMD, CMD_TXDATA);
				rx_limit--;
				d->rx_outstanding++;
			} else {
				pcie_i2c_writel(d, cmd | *buf++, CMD_TXDATA);
			}
			buf_len--;
			tx_limit--;
		}

		d->tx_buf = buf;
		d->tx_buf_len = buf_len;

		if ((buf_len > 0) || (flags & I2C_M_RECV_LEN)) {
			/* more bytes to be written */
			d->status |= STATUS_WRITE_IN_PROGRESS;
			break;
		} else {
			d->status &= ~STATUS_WRITE_IN_PROGRESS;
		}
	}
}
/*
 * Initiate (and continue) low level master read/write transaction.
 * This function is only called from pcie_i2c_isr, and pumping i2c_msg
 * messages into the tx buffer.  Even if the size of i2c_msg data is
 * longer than the size of the tx buffer, it handles everything.
 */
static void pcie_i2c_xfer_msg(struct pcie_i2c_dev *d)
{
	u32 intr_mask;

	if (d->cmd_err == (unsigned int)I2C_INT_TRERR)
		return;

	intr_mask = I2C_INTR_MASTER_MASK;

	pcie_i2c_send(d);
	/*
	 * If i2c_msg index search is completed, we don't need TX_EMPTY
	 * interrupt any more.
	 */
	if (d->msg_write_idx == d->msgs_num)
		intr_mask &= ~I2C_INT_TX_AEMPTY;

	if (d->msg_err)
		intr_mask = 0;

	pcie_i2c_writel(d, intr_mask, INT_MASK);
}

static u8 pcie_i2c_recv_len(struct pcie_i2c_dev *d, u8 len)
{
	struct i2c_msg *msgs = d->msgs;
	u32 flags = msgs[d->msg_read_idx].flags;

	/*
	 * Adjust the buffer length and mask the flag
	 * after receiving the first byte.
	 */
	len += (flags & I2C_CLIENT_PEC) ? 2 : 1;
	d->tx_buf_len = len - min_t(u8, len, d->rx_outstanding);
	msgs[d->msg_read_idx].len = len;
	msgs[d->msg_read_idx].flags &= ~I2C_M_RECV_LEN;

	return len;
}

static void pcie_i2c_recv(struct pcie_i2c_dev *d)
{
	struct i2c_msg *msgs = d->msgs;
	u32 stat;

	if (d->cmd_err == (unsigned int)I2C_INT_TRERR)
		return;

	for (; d->msg_read_idx < d->msgs_num; d->msg_read_idx++) {
		u32 len, tmp;
		u8 *buf = NULL;

		if ((msgs[d->msg_read_idx].flags & I2C_M_RD) == 0)
			continue;

		if ((d->status & STATUS_READ_IN_PROGRESS) == 0) {
			len = msgs[d->msg_read_idx].len;
			buf = msgs[d->msg_read_idx].buf;
		} else {
			len = d->rx_buf_len;
			buf = d->rx_buf;
		}

		for (; len > 0; len--) {
			u32 flags = msgs[d->msg_read_idx].flags;

			stat = pcie_i2c_readl(d, FIFO_STATE);
			if (stat & I2C_RX_EMPTY)
				break;
			tmp = pcie_i2c_readl(d, RXDATA);
			*buf = tmp & 0xff;
			/* Ensure length byte is a valid value */
			if (flags & I2C_M_RECV_LEN)
				len = pcie_i2c_recv_len(d, *buf);
			buf++;
			d->rx_outstanding--;
		}

		if (len > 0) {
			d->status |= STATUS_READ_IN_PROGRESS;
			d->rx_buf_len = len;
			d->rx_buf = buf;
			return;
		} else {
			d->status &= ~STATUS_READ_IN_PROGRESS;
		}
	}
}

static int pcie_i2c_handle_transfer_error(struct pcie_i2c_dev *d)
{
	unsigned long error_source = d->error_source;
	unsigned int i;

	for_each_set_bit(i, &error_source, ARRAY_SIZE(error_sources))
		dev_err(d->dev, "%s: %s\n", __func__, error_sources[i]);

	if (error_source & (unsigned int)I2C_TRANS_ERROR_NOACK) {
		for_each_set_bit(i, &error_source, ARRAY_SIZE(error_sources))
			dev_dbg(d->dev,
				"%s: %s\n", __func__, error_sources[i]);
		return -EREMOTEIO;
	} else if (error_source & (unsigned int)I2C_TRANS_ERROR_GCALL_READ) {
		return -EINVAL; /* wrong msgs[] data */
	} else {
		return -EIO;
	}
}

static void pcie_i2c_bus_recovery(struct i2c_adapter *adap)
{
	struct pcie_i2c_dev *d = i2c_get_adapdata(adap);

	if (d->recover_bus)
		d->recover_bus(adap);
	else
		dev_err(d->dev, "no recovered routine\n");
}

static int pcie_i2c_prepare_xfer(struct pcie_i2c_dev *d,
	struct i2c_msg msgs[], int num)
{
	u32 pin_state, tmp;
	int ret;

	if (d->initd == 0)
		pcie_i2c_init_master(d);

	ret = pcie_i2c_wait_bus_not_busy(d);
	if (ret)
		return ret;

	tmp = pcie_i2c_readl(d, TRANS_STATE);
	pin_state = tmp & PIN_STATE_MASK;
	if (pin_state != I2C_PIN_STATUS) {
		dev_err(d->dev,
			"pins status are not correct! state 0x%x\n", pin_state);
		goto err;
	}

	reinit_completion(&d->cmd_complete);
	d->msgs = msgs;
	d->msgs_num = num;
	d->cmd_err = 0;
	d->msg_write_idx = 0;
	d->msg_read_idx = 0;
	d->msg_err = 0;
	d->status = STATUS_IDLE;
	d->error_source = 0;
	d->rx_outstanding = 0;

	pcie_i2c_fifo_clear(d);

	return 0;

err:
	pcie_i2c_bus_recovery(&(d->adapter));

	return -EINVAL;
}

static int pcie_i2c_wait_for_completion(struct pcie_i2c_dev *d)
{
	int ret;
	u32 irq_state = 0;

	ret = wait_for_completion_timeout(&d->cmd_complete, WAIT_FOR_COMPLETION);
	if (ret == 0) {
		irq_state = pcie_i2c_readl(d, INT_MSTAT);
		dev_err(d->dev, "dev timed out, irq state 0x%x\n", irq_state);
		pcie_i2c_writel(d, 0, INT_MASK);
		reset_i2c_controller(d);
		ret = -ETIMEDOUT;
	}
	return ret;
}

static int pcie_i2c_xfer(struct i2c_adapter *adap, struct i2c_msg msgs[], int num)
{
	struct pcie_i2c_dev *d = i2c_get_adapdata(adap);
	int ret;
	u32 stat;

	if (!d) {
		pr_err("%s:i2c ctrler doesn't be init\n", __func__);
		return -ENODEV;
	}
	mutex_lock(&d->lock);
	if (iodie_normal_request(IODIE_USER_I2C)) {
		dev_err(d->dev, "iodie pcie status err\n");
		mutex_unlock(&d->lock);
		return -ENODEV;;
	}

	ret = pcie_i2c_prepare_xfer(d, msgs, num);
	if (ret) {
		iodie_normal_release(IODIE_USER_I2C);
		mutex_unlock(&d->lock);
		return ret;
	}

	/* start the transfers */
	pcie_i2c_xfer_init(d);

	stat = pcie_i2c_readl(d, FIFO_STATE);
	if (stat & I2C_FIFO_TX_EMPTY)
		pcie_i2c_xfer_msg(d);

	/* wait for tx to complete */
	ret = pcie_i2c_wait_for_completion(d);
	if (-ETIMEDOUT == ret)
		goto done;

	if (d->msg_err) {
		ret = d->msg_err;
		goto done;
	}

	/* no error */
	if (likely(d->cmd_err == 0)) {
		stat = pcie_i2c_readl(d, INT_MSTAT);
		if ((d->status &
			(STATUS_READ_IN_PROGRESS | STATUS_WRITE_IN_PROGRESS)) ||
			(I2C_INT_TX_EMPTY == (stat & I2C_INT_TX_EMPTY))) {
			dev_err(d->dev,
				"write or read is not complete, status 0x%x,\n", d->status);
			ret = -EAGAIN;
		} else {
			ret = num;
			d->timeout_count = 0;
		}
		goto done;
	}

	/* We have an error */
	if (d->cmd_err == (unsigned int)I2C_INT_TRERR) {
		ret = pcie_i2c_handle_transfer_error(d);
		goto done;
	}
	ret = -EIO;

done:
	if (-ETIMEDOUT == ret) {
		pcie_i2c_bus_recovery(adap);
		ret = -EAGAIN;
	}

	iodie_normal_release(IODIE_USER_I2C);
	mutex_unlock(&d->lock);
	if (ret < 0)
		dev_err(d->dev, "error info %d, slave addr 0x%x.\n",
			ret, msgs[0].addr);

	return ret;
}

static u32 pcie_i2c_func(struct i2c_adapter *adap)
{
	struct pcie_i2c_dev *d = i2c_get_adapdata(adap);

	return d->functionality;
}

static struct i2c_algorithm pcie_i2c_algo = {
	.master_xfer = pcie_i2c_xfer,
	.functionality = pcie_i2c_func,
};

static int pcie_i2c_new_dev(struct pcie_i2c_dev **i2c_dev,
	struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct pcie_i2c_dev *d = NULL;
	u32 hold_time = 0;
	struct resource *iores = NULL;
	u64 clk_rate = 0;
	u32 irq_bit = 0;
	int ret;

	d = devm_kzalloc(dev, sizeof(struct pcie_i2c_dev), GFP_KERNEL);
	if (!d)
		return -ENOMEM;

	d->dev = get_device(dev);

	iores = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!iores) {
		pr_err("%s get resource fail\n", __func__);
		return -EINVAL;
	}

	d->mapbase = iores->start;

	d->base = devm_ioremap(dev, iores->start, resource_size(iores));
	if (!d->base) {
		pr_err("%s ioremap fail\n", __func__);
		return -EADDRNOTAVAIL;
	}

	ret = of_property_read_u64(dev->of_node, "clock-rate", &clk_rate);
	if (ret) {
		dev_info(dev, "doesn't have clock-rate property!\n");
		return -EINVAL;
	}
	dev_info(dev, "clock rate is %llu\n", clk_rate);
	if (clk_rate == 0)
		return -EINVAL;
	d->clk_rate = clk_rate;

	ret = of_property_read_u32(dev->of_node, "irq-bit", &irq_bit);
	if (ret) {
		dev_info(dev, "doesn't have irq-bit property!\n");
		return -EINVAL;
	}
	d->irq_bit = irq_bit;

	ret = of_property_read_u32(dev->of_node, "hold-time", &hold_time);
	if (ret)
		hold_time = I2C_DEFULT_HOLD_TIME;
	dev_info(dev, "%s hold_time=%u\n", __func__, hold_time);
	d->hold_time = hold_time;

	*i2c_dev = d;
	return 0;
}

static void pcie_i2c_dev_recover_cfg(struct pcie_i2c_dev *d,
	struct device *dev)
{
	int ret;
	u32 iomg_data[I2C_RECOVER_IOMG_CNT] = {0};
	u32 func_data[I2C_RECOVER_VAL_CNT] = {0};

	ret = of_property_read_u32_array(dev->of_node, "i2c-ioc-base",
		&iomg_data[0], ARRAY_SIZE(iomg_data));
	if (ret) {
		dev_err(dev, "doesn't have i2c-ioc-base property!\n");
		return;
	} else {
		d->iomg_base = devm_ioremap(dev, iomg_data[1], iomg_data[3]);
	}

	ret = of_property_read_u32_array(dev->of_node, "i2c-recover-val",
		&func_data[0], ARRAY_SIZE(func_data));
	if (ret) {
		dev_err(dev, "doesn't have i2c-recover-val property!\n");
		return;
	}

	d->sda_ioc.iomg_offset = func_data[0];
	d->sda_ioc.iomg_func_i2c = func_data[1];
	d->sda_ioc.iomg_func_gpio = func_data[2];
	d->recover_bus = i2c_scl_recover_bus;
	return;
}

static u32 pcie_i2c_read_clear_intrbits(struct pcie_i2c_dev *dev)
{
	u32 stat, dummy;

	stat = pcie_i2c_readl(dev, INT_MSTAT);
	if (stat & I2C_INT_TRCMP)
		pcie_i2c_writel(dev, I2C_INT_TRCMP, INT_CLR);
	if (stat & I2C_INT_FIFOERR)
		pcie_i2c_writel(dev, I2C_INT_FIFOERR, INT_CLR);
	if (stat & I2C_INT_RX_AFULL)
		pcie_i2c_writel(dev, I2C_INT_RX_AFULL, INT_CLR);
	if (stat & I2C_INT_TX_AEMPTY)
		pcie_i2c_writel(dev, I2C_INT_TX_AEMPTY, INT_CLR);
	if (stat & I2C_INT_TRERR) {
		dummy = pcie_i2c_readl(dev, TRANS_ERR);
		dev->error_source = dummy;
		pcie_i2c_writel(dev, I2C_INT_TRERR, INT_CLR);
	}

	return stat;
}

static int pcie_i2c_irq_handler_master(struct pcie_i2c_dev *dev)
{
	u32 stat;

	stat = pcie_i2c_read_clear_intrbits(dev);
	if (stat & I2C_INT_TRERR)
		dev->cmd_err |= I2C_INT_TRERR;

	if (stat & I2C_INT_RX_AFULL)
		pcie_i2c_recv(dev);

	if (stat & I2C_INT_TX_AEMPTY)
		pcie_i2c_xfer_msg(dev);

	if ((stat & I2C_INT_TRCMP)) {
			pcie_i2c_recv(dev);
		pcie_i2c_writel(dev, 0, INT_MASK);
		complete(&dev->cmd_complete);
	}

	return 0;
}

static void pcie_i2c_isr(void *data)
{
	struct pcie_i2c_dev *d = data;

	if (!d) {
		pr_err("%s: i2c contrller do not be init.\n", __func__);
		return;
	}
	spin_lock(&d->spinlock);
	pcie_i2c_irq_handler_master(d);
	spin_unlock(&d->spinlock);
	return;
}

static void pcie_i2c_fill_i2c_dev(struct pcie_i2c_dev *d, struct device *dev)
{
	int ret;
	u32 speed_mode = 0;

	ret = of_property_read_u32(dev->of_node, "speed-mode", &speed_mode);
	if (ret) {
		dev_info(dev, "no speed-mode property, use default!\n");
		speed_mode = I2C_SPEED_FS;
	} else {
		if ((speed_mode != I2C_SPEED_STD) && (speed_mode != I2C_SPEED_FS) &&
			(speed_mode != I2C_SPEED_HIGH))
			speed_mode = I2C_SPEED_FS;

		dev_info(dev, "speed mode is %d!\n", speed_mode);
	}

	d->functionality = I2C_DEFAULT_FUNCTIONALITY | I2C_FUNC_10BIT_ADDR;

	d->master_cfg = I2C_RESTART_EN | speed_mode;
}

static int pcie_i2c_init_adapter(struct i2c_adapter *adap,
	struct platform_device *pdev, struct pcie_i2c_dev *d)
{
	int ret;
	struct device *dev = &pdev->dev;
	int retries = 0;
	int timeout = 0;

	ret = of_property_read_s32(dev->of_node, "retries", &retries);
	if (ret) {
		dev_info(dev, "no retries ,use default!\n");
		retries = I2C_PROPER_RETRIES;
	}

	ret = of_property_read_s32(dev->of_node, "timeout", &timeout);
	if (ret) {
		dev_info(dev, "no timeout ,use default!\n");
		timeout = I2C_TIMEOUT_DEFAULT_VAL;
	}

	i2c_set_adapdata(adap, d);
	adap->owner = THIS_MODULE;
	adap->class = I2C_CLASS_HWMON;
	strlcpy(adap->name, "Hisilicon PCIE I2C adapter", sizeof(adap->name));
	adap->algo = &pcie_i2c_algo;
	adap->dev.parent = dev;
	adap->dev.of_node = dev->of_node;

	adap->nr = pdev->id;
	adap->retries = retries;
	adap->timeout = timeout;
	dev_info(dev, "adap->retries = %d adap->timeout = %d\n ", adap->retries,
		adap->timeout);

	ret = i2c_add_numbered_adapter(adap);
	if (ret) {
		dev_err(dev, "failure adding adapter\n");
		return ret;
	}

	return 0;
}

static int i2c_get_pcie_status(void)
{
	struct device_node *sctrl_np = NULL;
	void __iomem *sctrlbase = NULL;
	int ret = 0;

	sctrl_np = of_find_compatible_node(NULL, NULL, "hisilicon,sysctrl");
	if (sctrl_np == NULL) {
		pr_err("%s fail to find sctrl node\n", __func__);
		return -1;
	}

	sctrlbase = of_iomap(sctrl_np, 0);
	if (sctrlbase == NULL) {
		pr_err("%s failed to iomap\n", __func__);
		return -1;
	}

	if ((readl(sctrlbase + IODIE_PCIE_STATUS_OFFSET)) & IODIE_PCIE_STATUS_MASK) {
		ret = 0;
	} else {
		pr_err("%s pcie status err\n", __func__);
		ret = -1;
	}

	iounmap(sctrlbase);
	return ret;
}

static int pcie_i2c_master_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct pcie_i2c_dev *d = NULL;
	int ret;

	ret = i2c_get_pcie_status();
	if (ret < 0 )
		return -ENODEV;

	if ((pcie_i2c_new_dev(&d, pdev) != 0) || !d) {
		pr_err("%s: new dev fail\n", __func__);
		return -ENODEV;
	}

	pcie_i2c_fill_i2c_dev(d, dev);
	pcie_i2c_dev_recover_cfg(d, dev);

	init_completion(&d->cmd_complete);
	mutex_init(&d->lock);
	spin_lock_init(&d->spinlock);

	pcie_i2c_init_master(d);
	pcie_i2c_disable_int(d);
	d->initd = 1;
	ret = iodie_ree_int_parse_register(d->irq_bit, pcie_i2c_isr, d);
	if (ret != 0) {
		dev_err(dev, "%s: register irq fail\n", __func__);
		goto irq_err;
	}

	if (pcie_i2c_init_adapter(&d->adapter, pdev, d)) {
		dev_err(dev, "%s: init adapter fail\n", __func__);
		goto clr_err;
	}

	platform_set_drvdata(pdev, d);

	dev_info(dev, "i2c probe succeed!\n");
	return 0;

clr_err:
	iodie_ree_int_parse_unregister(d->irq_bit);
irq_err:
	put_device(dev);

	return -1;
}

static int pcie_i2c_master_remove(struct platform_device *pdev)
{
	struct pcie_i2c_dev *d = platform_get_drvdata(pdev);

	if (!d) {
		pr_err("%s: get drvdata failed\n", __func__);
		return -EINVAL;
	}
	iodie_ree_int_parse_unregister(d->irq_bit);
	platform_set_drvdata(pdev, NULL);
	i2c_del_adapter(&d->adapter);
	put_device(&pdev->dev);

	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id pcie_i2c_master_of_match[] = {
	{
		.compatible = "hisilicon,iodie-pcie-i2c",
	},
	{},
};
MODULE_DEVICE_TABLE(of, pcie_i2c_master_of_match);
#endif

#ifdef CONFIG_PM
static int pcie_i2c_master_suspend(struct device *dev)
{
	unsigned long time, timeout;
	struct platform_device *pdev = to_platform_device(dev);
	struct pcie_i2c_dev *d = platform_get_drvdata(pdev);

	if (!d) {
		pr_err("%s: get drvdata failed\n", __func__);
		return -EINVAL;
	}

	dev_info(&pdev->dev, "%s: suspend +\n", __func__);

	timeout = jiffies + msecs_to_jiffies(GET_DEV_LOCK_TIMEOUT);
	while (!mutex_trylock(&d->lock)) {
		time = jiffies;
		if (time_after(time, timeout)) {
			dev_info(&pdev->dev,
				"%s: mutex_trylock timeout fail\n", __func__);
			return -EAGAIN;
		}

		/* sleep between 1000 and 2000 us */
		usleep_range(1000, 2000);
	}
	d->initd = 0;
	dev_info(&pdev->dev, "%s: suspend -\n", __func__);
	return 0;
}

static int pcie_i2c_master_resume(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct pcie_i2c_dev *d = platform_get_drvdata(pdev);

	if (!d) {
		pr_err("%s: get drvdata failed\n", __func__);
		return -EINVAL;
	}

	if (iodie_normal_request(IODIE_USER_I2C)) {
		dev_err(d->dev, "iodie pcie status err\n");
		d->initd = 0;
		mutex_unlock(&d->lock);
		return 0;
	}

	dev_info(&pdev->dev, "%s: resume +\n", __func__);

	pcie_i2c_init_master(d);
	pcie_i2c_disable_int(d);
	d->initd = 1;
	iodie_normal_release(IODIE_USER_I2C);
	mutex_unlock(&d->lock);

	dev_info(&pdev->dev, "%s: resume -\n", __func__);
	return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(
	pcie_i2c_master_dev_pm_ops, pcie_i2c_master_suspend, pcie_i2c_master_resume);

static struct platform_driver pcie_i2c_master_driver = {
	.probe = pcie_i2c_master_probe,
	.remove = pcie_i2c_master_remove,
	.driver = {
		.name = "pcie_i2c_master",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(pcie_i2c_master_of_match),
		.pm = &pcie_i2c_master_dev_pm_ops,
	},
};

static int __init pcie_i2c_init(void)
{
	return platform_driver_register(&pcie_i2c_master_driver);
}

static void __exit pcie_i2c_exit(void)
{
	platform_driver_unregister(&pcie_i2c_master_driver);
}

subsys_initcall(pcie_i2c_init);
module_exit(pcie_i2c_exit);

