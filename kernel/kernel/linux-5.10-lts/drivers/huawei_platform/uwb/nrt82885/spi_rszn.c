// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/compat.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/acpi.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
#include <linux/gpio.h>

#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/delay.h>

#include <huawei_platform/log/hw_log.h>

#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/platform_device.h>
#include <linux/of_platform.h>
#include <linux/regulator/consumer.h>
#include <linux/clk.h>
#include "rszn_gpioinit.h"

#define RSZN_MAJOR				153 /* assigned */

#define WAIT_TIMEOUT			(HZ/10)

#define DEV_NAME					"rszn"

#define SPI_IOC_RD_POWER		_IOR(SPI_IOC_MAGIC, 6, u32)
#define SPI_IOC_WR_POWER		_IOW(SPI_IOC_MAGIC, 6, u32)

#define SPI_IOC_RD_WAIT		 	_IOR(SPI_IOC_MAGIC, 7, u32)
#define SPI_IOC_WR_WAIT		 	_IOW(SPI_IOC_MAGIC, 7, u32)

#define SPI_IOC_RD_RESET		_IOR(SPI_IOC_MAGIC, 8, u32)
#define SPI_IOC_WR_RESET		_IOW(SPI_IOC_MAGIC, 8, u32)

#define SPI_IOC_RD_WAKEUP	   	_IOR(SPI_IOC_MAGIC, 9, u32)
#define SPI_IOC_WR_WAKEUP	   	_IOW(SPI_IOC_MAGIC, 9, u32)

#define SPI_IOC_UPGRADE_FORCED  _IOW(SPI_IOC_MAGIC, 10, u32)

#define SPI_IOC_RELEASE_READ    _IOR(SPI_IOC_MAGIC, 11, u32)

#define SPI_IOC_WAIT_INTERUPT   _IOR(SPI_IOC_MAGIC, 12, u32)

/* Bit masks for spi_device.mode management.  Note that incorrect
 * settings for some settings can cause *lots* of trouble for other
 * devices on a shared bus:
 *
 *  - CS_HIGH ... this device will be active when it shouldn't be
 *  - 3WIRE ... when active, it won't behave as it should
 *  - NO_CS ... there will be no explicit message boundaries; this
 *	is completely incompatible with the shared bus model
 *  - READY ... transfers may proceed when they shouldn't.
 *
 * REVISIT should changing those flags be privileged?
 */
#define SPI_MODE_MASK		(SPI_CPHA | SPI_CPOL | SPI_CS_HIGH \
				| SPI_LSB_FIRST | SPI_3WIRE | SPI_LOOP \
				| SPI_NO_CS | SPI_READY | SPI_TX_DUAL \
				| SPI_TX_QUAD | SPI_TX_OCTAL | SPI_RX_DUAL \
				| SPI_RX_QUAD | SPI_RX_OCTAL)

/* misc definition */
#define MIN_CMD_LEN			 	4
#define MAX_RXBUFFER_SIZE	    259
#define CMD_PAYLOAD_LEN_IDX     3

#define PMU_LDO_54_STR 			"2v85ldo54"
#define PMU_LDO_53_STR 			"simldo53"
#define PMU_LDO_25_STR 			"1v2ldo25"
#define PMU_LDO_3_STR 			"rfpower"

#define PMU_32KB_CLK_STR 		"clk_pmu32kb"
#define CHIP_TYPE_BUFF_SIZE		40
#define CHIP_TYPE_FILE_ATTR		0664

static unsigned int rszn_major;
static char rszn_chip_type[CHIP_TYPE_BUFF_SIZE];
static char rszn_32kclk_name[CHIP_TYPE_BUFF_SIZE];

static unsigned int g_pmu38m_div_flag = 0;
static unsigned int g_2v85gpio_flag = 0;
/* adapter for TV, 38M4 and 32k clk are supplied from External crystal */
static unsigned int g_uwb_extra_clk_supply = 0;
/* adapter for TV, do not need ldo53 */
static unsigned int g_rszn_ldo53_not_supply = 0;
/* adapter for TV, 38M4 and 32k clk are supplied from External crystal */
static unsigned int g_rszn_32kclk_supply = 0;
/* adapter for TV, ldo_reg25 adjust to 1.25V */
static unsigned int g_rszn_ldo25_to_1v25 = 0;
/* adapter for TV, spi read with irq blocked */
static unsigned int g_rszn_block_read = 0;
/* adapter for TV, ldo_reg54 always on */
static unsigned int g_rszn_ldo58_always_on = 0;
static int g_clk_38m4_enable_reg = 0;
rszn_uwb_power_supply g_uwb_pmu_rszn = {NULL, NULL, NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0};

static LIST_HEAD(device_list);
static DEFINE_MUTEX(device_list_lock);

static unsigned bufsiz = 4096;
module_param(bufsiz, uint, S_IRUGO);
MODULE_PARM_DESC(bufsiz, "data bytes in biggest supported SPI message");

/*-------------------------------------------------------------------------*/
static ssize_t rszn_sync(struct rszn_data *rszn, struct spi_message *message)
{
	int status;
	struct spi_device *spi;

	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);

	spin_lock_irq(&rszn->spi_lock);
	spi = rszn->spi;
	spin_unlock_irq(&rszn->spi_lock);

	if (spi == NULL) {
		status = -ESHUTDOWN;
		pr_err("rszn spi is NULL\n");
	} else {
		status = spi_sync(spi, message);
	}

	if (status == 0)
		status = message->actual_length;

	return status;
}

static inline ssize_t rszn_sync_write(struct rszn_data *rszn, size_t len)
{
	struct spi_transfer	t = {
		.tx_buf		= rszn->tx_buffer,
		.len		= len,
		.speed_hz	= rszn->speed_hz,
	};
	struct spi_message m;

	if (len > bufsiz) {
		pr_err("rszn %s len: %d more than bufsiz: %d", __func__, len, bufsiz);
		return -EMSGSIZE;
	}

	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);
	spi_message_init(&m);
	spi_message_add_tail(&t, &m);

	return rszn_sync(rszn, &m);
}

static inline ssize_t rszn_sync_read(struct rszn_data *rszn, size_t len)
{
	struct spi_transfer	t = {
		.rx_buf		= rszn->rx_buffer,
		.len		= len,
		.speed_hz	= rszn->speed_hz,
	};
	struct spi_message	m;

	if (len > bufsiz) {
		pr_err("rszn %s len: %d more than bufsiz: %d", __func__, len, bufsiz);
		return -EMSGSIZE;
	}

	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);

	spi_message_init(&m);
	spi_message_add_tail(&t, &m);

	return rszn_sync(rszn, &m);
}

/*-------------------------------------------------------------------------*/
/* Read-only message with current device setup */
static ssize_t rszn_read_payload(struct rszn_data *rszn, char __user *buf, size_t count)
{
	ssize_t status = MIN_CMD_LEN;

	if (count > bufsiz) {
		pr_err("rszn %s error count %u!", __func__, count);
		return -EMSGSIZE;
	}

	if (count) {
		unsigned long missing;

		status = rszn_sync_read(rszn, count);
		if (status == count) {
			missing = copy_to_user(&buf[MIN_CMD_LEN], rszn->rx_buffer, status);
			if (missing != 0) {
				status = -EFAULT;
				pr_err("rszn %s copy_to_user failed second!", __func__);
			} else {
				status += MIN_CMD_LEN;
			}
		} else {
			pr_err("rszn %s rszn_sync_read failed second! expect: %d, get: %d", __func__, count, status);
			status = -EMSGSIZE;					
		}
	}

	return status;
}

static int rszn_wait_forever(struct rszn_data *rszn)
{
	int	retval = 0;
	unsigned long flags;
	if (g_rszn_block_read) {
		if (rszn->wait_irq == WAIT_IRQ_ON) {
			retval = wait_event_interruptible(rszn->wait, rszn->wait_irq);
		}
		if (retval != 0) {
			spin_lock_irqsave(&rszn->irq_lock, flags);
			rszn->wait_irq = WAIT_IRQ_ON;
			spin_unlock_irqrestore(&rszn->irq_lock, flags);
			pr_info("###%s warn retval:%d #### .... \n", __func__, retval);
		}
	}
	return retval;
}

/* Read-only message with current device setup */
static ssize_t rszn_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	struct rszn_data	*rszn;
	ssize_t		   status = 0;
	unsigned long   	flags;

	unused(f_pos);
	pr_info("###%s: %d; rszn enter, count: %d #### .... \n", __func__, __LINE__, count);
	rszn = filp->private_data;
   	if (!rszn) {
		pr_err("rszn %s rszn not exist!", __func__);
		return -EFAULT;
	}

	/* chipselect only toggles at start or end of operation */
	if (count > bufsiz || count != MAX_RXBUFFER_SIZE) {
		pr_err("rszn %s error count %u!", __func__, count);
		return -EMSGSIZE;
	}

	mutex_lock(&rszn->buf_lock);

	rszn_cs_low(rszn);
	status = rszn_sync_read(rszn, MIN_CMD_LEN);
	if (status == MIN_CMD_LEN) {
		unsigned long missing;

		missing = copy_to_user(buf, rszn->rx_buffer, status);
		if (missing != 0) {
			status = -EFAULT;
			pr_err("rszn %s copy_to_user failed first", __func__);
		} else {
			if (status == MIN_CMD_LEN)
				status = rszn_read_payload(rszn, buf, rszn->rx_buffer[CMD_PAYLOAD_LEN_IDX]);
		}
	} else {
		pr_err("rszn %s rszn_sync_read failed first! expect: %d, get: %d", __func__, count, status);
		status = -EMSGSIZE;
	}
	rszn_cs_high(rszn);
	spin_lock_irqsave(&rszn->irq_lock, flags);
	rszn->wait_irq = WAIT_IRQ_ON;
	spin_unlock_irqrestore(&rszn->irq_lock, flags);
	mutex_unlock(&rszn->buf_lock);

	return status;
}

/* Write-only message with current device setup */
static ssize_t rszn_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	struct rszn_data	*rszn;
	ssize_t				status;
	unsigned long		missing;

	unused(f_pos);

	pr_info("###%s: %d; rszn enter, count: %d #### .... \n", __func__, __LINE__, count);

	rszn = filp->private_data;

	if (!rszn) {
		pr_err("rszn %s: rszn not exist", __func__);
		return -EFAULT;
	}
	/* chipselect only toggles at start or end of operation */
	if (count > bufsiz) {
		pr_err("rszn %s  write count: %d more than bufsiz: %d", __func__, count, bufsiz);
		return -EMSGSIZE;
	}

	mutex_lock(&rszn->buf_lock);

	missing = copy_from_user(rszn->tx_buffer, buf, count);
	if (missing == 0) {
		rszn_cs_low(rszn);
		status = rszn_sync_write(rszn, count);
		rszn_cs_high(rszn);
	} else {
		status = -EFAULT;
		pr_err("rszn %s copy_to_user failed!", __func__);
	}

	mutex_unlock(&rszn->buf_lock);

	return status;
}

static inline void rszn_message_construct_set(struct rszn_data *rszn, struct spi_transfer *k_tmp,
	struct spi_ioc_transfer *u_tmp)
{
	k_tmp->cs_change = !!u_tmp->cs_change;
	k_tmp->tx_nbits = u_tmp->tx_nbits;
	k_tmp->rx_nbits = u_tmp->rx_nbits;
	k_tmp->bits_per_word = u_tmp->bits_per_word;
	k_tmp->delay.value = u_tmp->delay_usecs;
	k_tmp->delay.unit = SPI_DELAY_UNIT_USECS;
	k_tmp->speed_hz = u_tmp->speed_hz;
	k_tmp->word_delay.value = u_tmp->word_delay_usecs;
	k_tmp->word_delay.unit = SPI_DELAY_UNIT_USECS;
	if (!k_tmp->speed_hz)
		k_tmp->speed_hz = rszn->speed_hz;
}

static inline int rszn_message_construct(struct rszn_data *rszn, struct spi_ioc_transfer *u_xfers,
	unsigned int n_xfers, struct spi_transfer *k_xfers, struct spi_message *msg)
{
	struct spi_transfer		 	*k_tmp;
	struct spi_ioc_transfer	 	*u_tmp;
	unsigned int				n;
	unsigned int				rx_total = 0;
	unsigned int				tx_total = 0;
	u8						  	*tx_buf = rszn->tx_buffer;
	u8							*rx_buf = rszn->rx_buffer;
	int						 	total = 0;

	for (n = n_xfers, k_tmp = k_xfers, u_tmp = u_xfers; n; n--, k_tmp++, u_tmp++) {
		unsigned int len_aligned = ALIGN(u_tmp->len, ARCH_KMALLOC_MINALIGN);

		k_tmp->len = u_tmp->len;
		total += k_tmp->len;

		if (total > INT_MAX || k_tmp->len > INT_MAX) {
			pr_err("rszn %s total or k_tmp->len more than INT_MAX, %d, %d!", __func__, total, k_tmp->len);
			return -EMSGSIZE;
		}

		if (u_tmp->rx_buf) {
			rx_total += len_aligned;
			if (rx_total > bufsiz) {
				pr_err("rszn %s rx_total more than bufsiz!", __func__);
				return -EMSGSIZE;
			}
			k_tmp->rx_buf = rx_buf;
			rx_buf += len_aligned;
		}

		if (u_tmp->tx_buf) {
			tx_total += len_aligned;
			if (tx_total > bufsiz) {
				pr_err("rszn %s tx_total more than bufsiz!", __func__);
				return -EMSGSIZE;
			}
			k_tmp->tx_buf = tx_buf;
			if (copy_from_user(tx_buf, (const u8 __user *)(uintptr_t)u_tmp->tx_buf, u_tmp->len)) {
				pr_err("rszn %s copy_from_user failed!", __func__);
				return -EFAULT;
			}
			tx_buf += len_aligned;
		}
		rszn_message_construct_set(rszn, k_tmp, u_tmp);
		spi_message_add_tail(k_tmp, msg);
	}

	return total;
}

static int rszn_message(struct rszn_data *rszn, struct spi_ioc_transfer *u_xfers, unsigned n_xfers)
{
	struct spi_message			msg;
	struct spi_transfer			*k_xfers, *k_tmp;
	struct spi_ioc_transfer		*u_tmp;
	unsigned					total;
	unsigned int 				n;
	int							status = -EFAULT;

	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);
	spi_message_init(&msg);
	k_xfers = kcalloc(n_xfers, sizeof(*k_tmp), GFP_KERNEL);
	if (k_xfers == NULL) {
		pr_err("rszn %s kcalloc failed!", __func__);
		return -ENOMEM;
	}

	total = rszn_message_construct(rszn, u_xfers, n_xfers, k_xfers, &msg);
	if (total < 0) {
		status = total;
		pr_err("rszn %s rszn_message_construct failed!", __func__);
		goto done;
	}

	status = rszn_sync(rszn, &msg);
	if (status < 0) {
		pr_err("rszn %s rszn_sync failed!", __func__);
		goto done;
	}

	/* copy any rx data out of bounce buffer */
	for (n = n_xfers, k_tmp = k_xfers, u_tmp = u_xfers; n; n--, k_tmp++, u_tmp++) {
		if (u_tmp->rx_buf) {
			if (copy_to_user((u8 __user *)(uintptr_t) u_tmp->rx_buf,
				k_tmp->rx_buf, u_tmp->len)) {
				status = -EFAULT;
				pr_err("rszn %s copy_to_user failed!", __func__);
				goto done;
			}
		}
	}
	status = total;

done:
	kfree(k_xfers);

	return status;
}

static struct spi_ioc_transfer *rszn_get_ioc_message(unsigned int cmd, struct spi_ioc_transfer __user *u_ioc,
	unsigned *n_ioc)
{
	u32 tmp;

	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);
	/* Check type, command number and direction */
	if (_IOC_TYPE(cmd) != SPI_IOC_MAGIC
			|| _IOC_NR(cmd) != _IOC_NR(SPI_IOC_MESSAGE(0))
			|| _IOC_DIR(cmd) != _IOC_WRITE) {
		pr_err("rszn %s check type failed!", __func__);
		return ERR_PTR(-ENOTTY);
	}

	tmp = _IOC_SIZE(cmd);
	if ((tmp % sizeof(struct spi_ioc_transfer)) != 0) {

		pr_err("rszn %s _IOC_SIZE failed!", __func__);
		return ERR_PTR(-EINVAL);
	}
	*n_ioc = tmp / sizeof(struct spi_ioc_transfer);
	if (*n_ioc == 0) {
		pr_err("rszn %s n_ioc failed!", __func__);
		return NULL;
	}

	/* copy into scratch area */
	return memdup_user(u_ioc, tmp);
}

static int rszn_ioctl_rd_wait(struct rszn_data *rszn, unsigned long arg)
{
	int			 retval = 0;
	u32			 time;
	unsigned long   flags;

	retval = get_user(time, (__u32 __user *)arg);

	if (retval == 0) {
		mutex_lock(&rszn->wait_lock);
		if (rszn->irq_gpio > 0) {
			spin_lock_irqsave(&rszn->irq_lock, flags);
			switch (rszn->wait_irq) {
			case WAIT_IRQ_ON:
				spin_unlock_irqrestore(&rszn->irq_lock, flags);
				retval = wait_event_interruptible_timeout(rszn->wait, rszn->wait_irq, (time * HZ)/1000);
				retval = (retval == 0) ? -1 : retval;
				break;
			case WAIT_IRQ_OFF:
				spin_unlock_irqrestore(&rszn->irq_lock, flags);
				retval = put_user(!rszn->wait_irq, (__u32 __user *)arg);
				break;
			default:
				spin_unlock_irqrestore(&rszn->irq_lock, flags);
				wait_event_interruptible_timeout(rszn->wait, rszn->wait_irq, WAIT_TIMEOUT);
				retval = put_user(rszn->wait_irq, (__u32 __user *)arg);
				break;
			}
		}
		mutex_unlock(&rszn->wait_lock);
	}

	return retval;
}

static int rszn_ioctl_wr_wait(struct rszn_data *rszn, unsigned long arg)
{
	int			 	retval = UWB_SUCC;
	u32			 	time;
	unsigned long   flags;

	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);
	retval = get_user(time, (__u32 __user*)arg);

	if (retval == 0) {
		mutex_lock(&rszn->wait_lock);

		spin_lock_irqsave(&rszn->irq_lock, flags);
		switch (rszn->wait_irq) {
		case WAIT_IRQ_OFF:
			spin_unlock_irqrestore(&rszn->irq_lock, flags);
			retval = UWB_FAIL;
			break;
		default:
			spin_unlock_irqrestore(&rszn->irq_lock, flags);
			break;
		}

		mutex_unlock(&rszn->wait_lock);
	}

	return retval;
}

static int uwb_power_manage(struct rszn_data *spidev, rszn_uwb_power_supply uwbldo, char sw)
{
	int ret = 0;

	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);
	uwbldo.gpio_enable2v85 = g_2v85gpio_flag;
	uwbldo.freq2div38m = g_pmu38m_div_flag;
	uwbldo.clk_38m4_enable_reg = g_clk_38m4_enable_reg;
	uwbldo.clk32kb = g_uwb_pmu_rszn.clk32kb;
	uwbldo.extra_clk_supply = g_uwb_extra_clk_supply;
	uwbldo.ldo53_not_supply = g_rszn_ldo53_not_supply;
	uwbldo.ldo25_1v25_supply = g_rszn_ldo25_to_1v25;
	switch (sw) {
	case UWB_SWITCH_OPEN:
		ret = rszn_uwb_open_init(spidev, uwbldo);
		break;
	case UWB_SWITCH_CLOSE:
		ret = rszn_uwb_close_deinit(spidev, uwbldo);
		break;
	default:
		ret = -1;
		break;
	}
	return ret;
}

static int rszn_wakeup_cycle(struct rszn_data *rszn)
{
	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);

	rszn_wakeup_low(rszn);
	msleep(WAKEUP_DELAY);
	rszn_wakeup_high(rszn);
	msleep(WAKEUP_DELAY);
	rszn_wakeup_low(rszn);

	return UWB_SUCC;
}

static int rszn_power(struct rszn_data *rszn, int on)
{
	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);
	if ((on != POWER_OFF) && (on != POWER_ON) && (on != POWER_POST_SLEEP)) {
		pr_err("%s: invalid op\n", __func__);
		return UWB_FAIL;
	}
	if (on == POWER_OFF && rszn->power == POWER_OFF) {
		pr_info("%s: already poweroff\n", __func__);
		return UWB_SUCC;
	}
	if (on == POWER_ON && rszn->power == POWER_ON) {
		pr_info("%s: already poweron\n", __func__);
		return UWB_SUCC;
	}
	if (on == POWER_POST_SLEEP && rszn->power == POWER_POST_SLEEP) {
		pr_info("%s: already powerpostsleep\n", __func__);
		return UWB_SUCC;
	}
	if (on == POWER_OFF) {
		pr_info("###%s: %d; POWER_OFF #### .... \n", __func__, __LINE__);
		uwb_power_manage(rszn, g_uwb_pmu_rszn, UWB_SWITCH_CLOSE);
		rszn->power = POWER_OFF;
	}
	if (on == POWER_ON) {
		pr_info("###%s: %d; POWER_ON #### .... \n", __func__, __LINE__);
		uwb_power_manage(rszn, g_uwb_pmu_rszn, UWB_SWITCH_OPEN);
		rszn->power = POWER_ON;
	}
	if (on == POWER_POST_SLEEP) {
		pr_info("###%s: %d; POWER_POST_SLEEP #### .... \n", __func__, __LINE__);
		uwb_power_manage(rszn, g_uwb_pmu_rszn, UWB_SWITCH_CLOSE);
		rszn->power = POWER_POST_SLEEP;
    }

	return UWB_SUCC;
}

static int rszn_reset(struct rszn_data *rszn, u32 reset_flag)
{
	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);

    if (reset_flag == EXIT_SLEEP_RESET)
		rszn_power(rszn, POWER_ON);

	rszn_wakeup_cycle(rszn);
	/* set cs high to idle */
	rszn_cs_high(rszn);
	rszn_uwb_hardware_reset(rszn);
	msleep(POWER_DELAY);
	rszn_wakeup_low(rszn);

	return UWB_SUCC;
}

static int rszn_ioctl_wr_power(struct rszn_data *rszn, unsigned long arg)
{
	int retval;
	u32 tmp;

	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);
	retval = get_user(tmp, (__u32 __user*)arg);
	if (retval == 0)
		retval = rszn_power(rszn, tmp);

	return retval;
}

static int rszn_ioctl_wr_reset(struct rszn_data *rszn, unsigned long arg)
{
	int retval = 0;
	u32 tmp;

	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);
	retval = get_user(tmp, (__u32 __user*)arg);
	if (retval == 0) {
		if (tmp) {
			retval = rszn_reset(rszn, tmp);
		} else {
			pr_err("###%s: %d; rszn not a reset! #### .... \n", __func__, __LINE__);
		}
	}

	return retval;
}

static int rszn_ioctl_wr_wakeup(struct rszn_data *rszn, unsigned long arg)
{
	int retval;
	u32 tmp;

	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);
	retval = get_user(tmp, (__u32 __user*)arg);
	if (retval == 0) {
		if (tmp) {
			retval = rszn_wakeup_cycle(rszn);
		} else {
			pr_err("###%s: %d; rszn not a wakeup! #### .... \n", __func__, __LINE__);
		}
	}

	return retval;
}

static int rszn_ioctl_upgrade_forced(struct rszn_data *rszn, unsigned long arg)
{
	int			 retval = UWB_SUCC;

	unused(arg);
	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);

	/* wake up chip first */
	rszn_wakeup_cycle(rszn);
	rszn_cs_high(rszn);

	/* reset cycle */
	rszn_uwb_hardware_reset(rszn);

	/* reset cs low to enter forcing upgrading */
	rszn_cs_low(rszn);

	/* set wakeup low */
	rszn_wakeup_low(rszn);

	/* keep cs low for 500ms, then set cs high */
	msleep(FORCE_UG_DELAY_MS);
	rszn_cs_high(rszn);

	return retval;
}

static int rszn_ioctl_wr_bits_per_word(struct rszn_data *rszn, struct spi_device *spi, unsigned long arg)
{
	int retval;
	u32 tmp;

	unused(rszn);
	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);
	retval = get_user(tmp, (__u8 __user *)arg);
	if (retval == 0) {
		u8 save = spi->bits_per_word;
		spi->bits_per_word = tmp;
		retval = spi_setup(spi);
		if (retval < 0) {
			spi->bits_per_word = save;
			pr_err("rszn %s bits_per_word set failed! default:%d", __func__, save);
		}
	}

	return retval;
}

static int rszn_ioctl_wr_mode(struct rszn_data *rszn, struct spi_device *spi, unsigned int cmd, unsigned long arg)
{
	int retval;
	u32 tmp;

	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);
	if (cmd == SPI_IOC_WR_MODE) {
		retval = get_user(tmp, (__u8 __user *)arg);
	} else {
		retval = get_user(tmp, (__u32 __user *)arg);
	}
	if (retval == 0) {
		struct spi_controller *ctlr = spi->controller;
		u32 save = spi->mode;

		if (tmp & ~SPI_MODE_MASK) {
			pr_err("rszn %s err arg!", __func__);
			return  -EINVAL;
		}

		if (ctlr->use_gpio_descriptors &&
			ctlr->cs_gpiods &&
			ctlr->cs_gpiods[spi->chip_select])
			tmp |= SPI_CS_HIGH;

		tmp |= spi->mode & ~SPI_MODE_MASK;
		spi->mode = (u16)tmp;
		retval = spi_setup(spi);
		if (retval < 0) {
			spi->mode = save;
			pr_err("rszn %s set failed! default:%d", __func__, save);
		}
	}

	return retval;
}

static int rszn_ioctl_wr_lsb_first(struct rszn_data *rszn, struct spi_device *spi, unsigned long arg)
{
	int retval;
	u32 tmp;

	unused(rszn);
	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);
	retval = get_user(tmp, (__u8 __user *)arg);
	if (retval == 0) {
		u32	save = spi->mode;

		if (tmp) {
			spi->mode |= SPI_LSB_FIRST;
		} else {
			spi->mode &= ~SPI_LSB_FIRST;
		}
		retval = spi_setup(spi);
		if (retval < 0) {
			spi->mode = save;
			pr_err("rszn %s set failed! default:%d", __func__, save);
		}
	}

	return retval;
}

static int rszn_ioctl_wr_speed_hz(struct rszn_data *rszn, struct spi_device *spi, unsigned long arg)
{
	int retval;
	u32 tmp;

	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);
	retval = get_user(tmp, (__u32 __user *)arg);
	if (retval == 0) {
		u32	save = spi->max_speed_hz;

		spi->max_speed_hz = tmp;
		retval = spi_setup(spi);
		if (retval == 0)
			rszn->speed_hz = tmp;
		spi->max_speed_hz = save;
	}

	return retval;
}

static int rszn_ioctl_release_read(struct rszn_data *rszn)
{
    int retval = 0;
    unsigned long flags;

    spin_lock_irqsave(&rszn->irq_lock, flags);
    if (rszn->wait_irq == WAIT_IRQ_ON) {
        rszn->wait_irq = WAIT_IRQ_OFF;
    }
    spin_unlock_irqrestore(&rszn->irq_lock, flags);
    wake_up_interruptible(&rszn->wait);
    return retval;
}

static int rszn_ioctl_default(struct rszn_data *rszn, struct spi_device *spi, unsigned int cmd, unsigned long arg)
{
	int					 	retval;
	unsigned int			n_ioc = 0;
	struct spi_ioc_transfer *ioc = NULL;

	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);
	unused(spi);

	ioc = rszn_get_ioc_message(cmd, (struct spi_ioc_transfer __user *)arg, &n_ioc);
	if (IS_ERR(ioc)) {
		retval = PTR_ERR(ioc);
		pr_err("rszn %s rszn_ioctl_default failed!", __func__);
		return retval;
	}
	if (!ioc) {
		pr_err("rszn %s rszn_ioctl_default failed! ioc is NULL", __func__);
		return 0;
	}
	retval = rszn_message(rszn, ioc, n_ioc);
	kfree(ioc);

	return retval;
}

static int rszn_ioctl_switch_extend(struct rszn_data *rszn, struct spi_device *spi, unsigned int cmd,
	unsigned long arg)
{
	int retval;

	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);
	switch (cmd) {
	case SPI_IOC_RD_POWER:
		retval = put_user(rszn->power, (__u32 __user *)arg);
		break;
	case SPI_IOC_RD_RESET:
		if (rszn->reset_gpio > 0) {
			retval = put_user(gpio_get_value(rszn->reset_gpio), (__u32 __user *)arg);
		} else {
			retval = put_user(0, (__u32 __user *)arg);
		}
		break;
	case SPI_IOC_RD_WAKEUP:
		if (rszn->wakeup_gpio > 0) {
			retval = put_user(gpio_get_value(rszn->wakeup_gpio), (__u32 __user *)arg);
		} else {
			retval = put_user(1, (__u32 __user *)arg);
		}
		break;
	case SPI_IOC_WR_POWER:
		retval = rszn_ioctl_wr_power(rszn, arg);
		break;
	case SPI_IOC_WR_RESET:
		retval = rszn_ioctl_wr_reset(rszn, arg);
		break;
	case SPI_IOC_WR_WAKEUP:
		retval = rszn_ioctl_wr_wakeup(rszn, arg);
		break;
	case SPI_IOC_UPGRADE_FORCED:
		retval = rszn_ioctl_upgrade_forced(rszn, arg);
		break;
	default:
		retval = rszn_ioctl_default(rszn, spi, cmd, arg);
		break;
	}

	return retval;
}

static int rszn_ioctl_switch(struct rszn_data *rszn, struct spi_device *spi, unsigned int cmd, unsigned long arg)
{
	int retval;

	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);

	mutex_lock(&rszn->buf_lock);

	switch (cmd) {
	case SPI_IOC_RD_MODE:
		retval = put_user(spi->mode & SPI_MODE_MASK, (__u8 __user *)arg);
		break;
	case SPI_IOC_RD_MODE32:
		retval = put_user(spi->mode & SPI_MODE_MASK, (__u32 __user *)arg);
		break;
	case SPI_IOC_RD_LSB_FIRST:
		retval = put_user(spi->mode & SPI_MODE_MASK, (__u8 __user *)arg);
		break;
	case SPI_IOC_RD_BITS_PER_WORD:
		retval = put_user(spi->bits_per_word, (__u8 __user *)arg);
		break;
	case SPI_IOC_RD_MAX_SPEED_HZ:
		retval = put_user(rszn->speed_hz, (__u32 __user *)arg);
		break;
	case SPI_IOC_WR_MODE:
	case SPI_IOC_WR_MODE32:
		retval = rszn_ioctl_wr_mode(rszn, spi, cmd, arg);
		break;
	case SPI_IOC_WR_LSB_FIRST:
		retval = rszn_ioctl_wr_lsb_first(rszn, spi, arg);
		break;
	case SPI_IOC_WR_BITS_PER_WORD:
		retval = rszn_ioctl_wr_bits_per_word(rszn, spi, arg);
		break;
	case SPI_IOC_WR_MAX_SPEED_HZ:
		retval = rszn_ioctl_wr_speed_hz(rszn, spi, arg);
		break;
	default:
		retval = rszn_ioctl_switch_extend(rszn, spi, cmd, arg);
		break;
	}

	mutex_unlock(&rszn->buf_lock);

	return retval;
}

static long rszn_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int				 retval;
	struct rszn_data	*rszn;
	struct spi_device   *spi;

	if (_IOC_TYPE(cmd) != SPI_IOC_MAGIC) {
		pr_err("rszn rszn_ioctl err cmd failed!");
		return -ENOTTY;
	}
	rszn = filp->private_data;
	if (cmd == SPI_IOC_RD_WAIT) {
		return rszn_ioctl_rd_wait(rszn, arg);
	} else if (cmd == SPI_IOC_WR_WAIT) {
		return rszn_ioctl_wr_wait(rszn, arg);
	} else if (cmd == SPI_IOC_RELEASE_READ) {
		return rszn_ioctl_release_read(rszn);
	} else if (cmd == SPI_IOC_WAIT_INTERUPT) {
		return rszn_wait_forever(rszn);
	}
	spin_lock_irq(&rszn->irq_lock);
	spi = spi_dev_get(rszn->spi);
	spin_unlock_irq(&rszn->irq_lock);
	if (spi == NULL) {
		pr_err("###%s; spi is NULL #### .... \n", __func__);
		return -ESHUTDOWN;
	}
	retval = rszn_ioctl_switch(rszn, spi, cmd, arg);
	spi_dev_put(spi);

	return retval;
}

static void rszn_buffer_kfree(struct rszn_data *rszn)
{
	if (!rszn->users) {
		if (rszn->tx_buffer) {
			kfree(rszn->tx_buffer);
			rszn->tx_buffer = NULL;
		}

		if (rszn->rx_buffer) {
			kfree(rszn->rx_buffer);
			rszn->rx_buffer = NULL;
		}
	}
}

static int rszn_buffer_kmalloc(struct rszn_data *rszn)
{
	int status = UWB_SUCC;

	if (!rszn->tx_buffer) {
		rszn->tx_buffer = kmalloc(bufsiz, GFP_KERNEL);
		if (!rszn->tx_buffer) {
			pr_err("rszn %s tx_buffer ENOMEM", __func__);
			status = -ENOMEM;
			goto exit;
		}
	}

	if (!rszn->rx_buffer) {
		rszn->rx_buffer = kmalloc(bufsiz, GFP_KERNEL);
		if (!rszn->rx_buffer) {
			pr_err("rszn %s rx_buffer ENOMEM", __func__);
			kfree(rszn->tx_buffer);
			rszn->tx_buffer = NULL;
			status = -ENOMEM;
			goto exit;
		}
	}

exit:

	return status;
}

static int rszn_open(struct inode *inode, struct file *filp)
{
	struct rszn_data	*rszn;
	int					status = -ENXIO;

	mutex_lock(&device_list_lock);

	list_for_each_entry(rszn, &device_list, device_entry) {
		if (rszn->devt == inode->i_rdev) {
			status = 0;
			break;
		}
	}

	if (status) {
		pr_info("rszn %s nothing for minor %d", __func__, iminor(inode));
		goto exit;
	}

	status = rszn_buffer_kmalloc(rszn);
	if (status != UWB_SUCC)
		goto exit;

	rszn->users++;
	filp->private_data = rszn;
	stream_open(inode, filp);

exit:
	mutex_unlock(&device_list_lock);

	return status;
}

static int rszn_release(struct inode *inode, struct file *filp)
{
	struct rszn_data	*rszn;
	int					dofree;
	unused(inode);

	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);
	mutex_lock(&device_list_lock);
	rszn = filp->private_data;
	filp->private_data = NULL;

	spin_lock_irq(&rszn->spi_lock);
	/* ... after we unbound from the underlying device? */
	dofree = (rszn->spi == NULL);
	spin_unlock_irq(&rszn->spi_lock);

	/* last close? */
	rszn->users--;
	if (!rszn->users) {
		rszn_buffer_kfree(rszn);
		if (dofree) {
			kfree(rszn);
		} else {
			rszn->speed_hz = rszn->spi->max_speed_hz;
		}
	}

	mutex_unlock(&device_list_lock);

	return 0;
}

static const struct file_operations rszn_fops = {
	.owner		  	=	THIS_MODULE,
	/* REVISIT switch to aio primitives, so that userspace
	 * gets more complete API coverage.  It'll simplify things
	 * too, except for the locking.
	 */
	.write		  	=	rszn_write,
	.read		   	=   rszn_read,
	.unlocked_ioctl =   rszn_ioctl,
	.open		   	=	rszn_open,
	.release		=	rszn_release,
	.llseek		 	=	no_llseek,
};

/*-------------------------------------------------------------------------*/

/* The main reason to have this class is to make mdev/udev create the
 * /dev/rsznB.C character device nodes exposing our userspace API.
 * It also simplifies memory management.
 */

static struct class *rszn_class;

static const struct of_device_id rszn_dt_ids[] = {
	{ .compatible = "huawei,rszn" },
	{},
};
MODULE_DEVICE_TABLE(of, rszn_dt_ids);

/*-------------------------------------------------------------------------*/

static int rszn_free(struct rszn_data *rszn)
{
	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);

	rszn_free_gpios(rszn);
	mutex_lock(&device_list_lock);
	spin_lock_irq(&rszn->spi_lock);
	rszn->spi = NULL;
	spin_unlock_irq(&rszn->spi_lock);
	list_del(&rszn->device_entry);
	device_destroy(rszn_class, rszn->devt);
	if (rszn->users == 0)
		kfree(rszn);
	mutex_unlock(&device_list_lock);

	return 0;
}

static void rszn_data_init(struct rszn_data *rszn, struct spi_device *spi)
{
	pr_info("###%s: rszn enter running line: %d #### .... \n", __func__, __LINE__);

	rszn->spi = spi;
	rszn->speed_hz = spi->max_speed_hz;
	spin_lock_init(&rszn->spi_lock);
	spin_lock_init(&rszn->irq_lock);
	init_waitqueue_head(&rszn->wait);
	mutex_init(&rszn->buf_lock);
	mutex_init(&rszn->wait_lock);
	mutex_init(&rszn->wake_lock);

	INIT_LIST_HEAD(&rszn->device_entry);
}

static int rszn_register_device(struct rszn_data *rszn)
{
	int status;

	/* If we can allocate a minor number, hook up this device.
	 * Reusing minors is fine so long as udev or mdev is working.
	 */
	pr_info("###%s: rszn enter running line: %d #### .... \n", __func__, __LINE__);
	mutex_lock(&device_list_lock);
	if (rszn_major) {
		rszn->devt = MKDEV(rszn_major, 0);
		status = register_chrdev_region(rszn->devt, 1, DEV_NAME);
		pr_info("###%s: register_chrdev_region: %d #### .... \n", __func__, __LINE__);
	} else {
		status = alloc_chrdev_region(&rszn->devt, 0, 1, DEV_NAME);
		rszn_major = MAJOR(rszn->devt);
		pr_info("###%s: alloc_chrdev_region: %d #### .... \n", __func__, __LINE__);
	}
	if (status == 0) {
		cdev_init(&rszn->cdev, &rszn_fops);
		status = cdev_add(&rszn->cdev, rszn->devt, 1);
		if (status == 0) {
			rszn->device = device_create(rszn_class, NULL, rszn->devt, NULL, DEV_NAME);
			if (IS_ERR(rszn->device)) {
				cdev_del(&rszn->cdev);
				unregister_chrdev_region(rszn->devt, 1);
				status = PTR_ERR(rszn->device);
				pr_err("%s: create device failed\n", __func__);
			}
		} else {
			unregister_chrdev_region(rszn->devt, 1);
			pr_err("%s: add cdev failed\n", __func__);
		}
	} else {
		pr_err("%s: register chrdev region failed\n", __func__);
	}
	if (status == 0)
		list_add(&rszn->device_entry, &device_list);
	mutex_unlock(&device_list_lock);

	return status;
}


static int uwb_perip_component_init(struct rszn_data *spidev, struct spi_device *spi)
{
	pr_info("###%s: rszn enter running line: %d #### .... \n", __func__, __LINE__);
	if (spidev == NULL || spi == NULL) {
		pr_err("rszn: UwbPowerInit input Invalid!\n");
		return UWB_FAIL;
	}
	g_uwb_pmu_rszn.ldo_reg53 = devm_regulator_get(&spi->dev, PMU_LDO_53_STR);
	if (IS_ERR(g_uwb_pmu_rszn.ldo_reg53)) {
		pr_err("rszn: %s: devm_regulator_get ldoReg53 failed !\n", __func__);
		if (g_rszn_ldo53_not_supply) {
			pr_err("rszn: %s: no need ldoReg53 continue !\n", __func__);
		} else {
			return UWB_FAIL;
		}
	}
	g_uwb_pmu_rszn.ldo_reg54 = devm_regulator_get(&spi->dev, PMU_LDO_54_STR);
	if (IS_ERR(g_uwb_pmu_rszn.ldo_reg54)) {
		pr_err("rszn: %s: devm_regulator_get ldoReg54 failed !\n", __func__);
		return UWB_FAIL;
	}
	g_uwb_pmu_rszn.ldo_reg25 = devm_regulator_get(&spi->dev, PMU_LDO_25_STR);
	if (IS_ERR(g_uwb_pmu_rszn.ldo_reg25)) {
		pr_err("rszn: %s: devm_regulator_get ldoReg25 failed !\n", __func__);
		return UWB_FAIL;
	}
	if (g_rszn_32kclk_supply) {
		g_uwb_pmu_rszn.clk32kb = devm_clk_get(&spi->dev, rszn_32kclk_name);
		if (IS_ERR(g_uwb_pmu_rszn.clk32kb)) {
			pr_err("rszn: %s: devm_clk_get clk32kb fail",  __func__);
			return UWB_FAIL;
		}
	}

	return UWB_SUCC;
}

static int rszn_get_dts_config(const char *node_name, const char *prop_name, char *out_str, int out_str_len)
{
	struct device_node *np = NULL;
	const char *out_value;
	int ret = -1;

	pr_info("###%s: rszn enter running line: %d #### .... \n", __func__, __LINE__);
	for_each_node_with_property(np, node_name) {
		ret = of_property_read_string(np, prop_name, (const char **)&out_value);
		if (ret != 0) {
			pr_err("%s: cann't get prop values with prop_name: %s\n", __func__, prop_name);
			continue;
		}
		if (out_value == NULL) {
			pr_err("%s: error out_value = NULL\n", __func__);
			ret = -1;
		} else if (strlen(out_value) >= out_str_len) {
			pr_err("%s: error out_value:%d >= out_str_len:%d\n", __func__,
					strlen(out_value), out_str_len);
			ret = -1;
		} else {
			pr_info("%s: =%s\n", __func__, out_str);
			if (strncpy_s(out_str, out_str_len, out_value, strlen(out_value)) != EOK)
				pr_err("rszn_get_dts_config strncpy fail\n");
		}
	}

	return ret;
}

static int rszn_get_dts_config_u32(const char *node_name, const char *prop_name, u32 *pvalue)
{
	struct device_node *np = NULL;
	int ret = -1;

	pr_info("###%s: rszn enter running line: %d #### .... \n", __func__, __LINE__);
	for_each_node_with_property(np, node_name) {
		ret = of_property_read_u32(np, prop_name, pvalue);
		if (ret != 0) {
			pr_err("%s: can not get prop values with prop_name: %s\n", __func__, prop_name);
		} else {
			pr_info("%s: %s=%d\n", __func__, prop_name, *pvalue);
		}
	}
	return ret;
}

static ssize_t rszn_chip_type_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return snprintf_s(buf, sizeof(rszn_chip_type), sizeof(rszn_chip_type) - 1, "%s", rszn_chip_type);
}

static ssize_t rszn_chip_type_store(struct device *dev, struct device_attribute *attr,
				const char *buf, size_t count)
{
	pr_info("%s: %s count=%lu\n", __func__, buf, count);

	if (strncpy_s(rszn_chip_type, CHIP_TYPE_BUFF_SIZE, buf, sizeof(rszn_chip_type) - 1) != EOK)
		pr_err("rszn %s strncpy fail", __func__);

	return (ssize_t)count;
}

static struct device_attribute rszn_driver_attr[] = {
	__ATTR(uwb_chip_type, CHIP_TYPE_FILE_ATTR, rszn_chip_type_show, rszn_chip_type_store),
};

static int rszn_create_sysfs_interfaces(struct device *dev)
{
	int i;

	pr_info("###%s: rszn enter running line: %d #### .... \n", __func__, __LINE__);

	for (i = 0; i < ARRAY_SIZE(rszn_driver_attr); i++) {
		if (device_create_file(dev, &rszn_driver_attr[i])) {
			/* @note create failed, rollback to last index before remove */
			i--;
			goto error;
		}
	}
	return UWB_SUCC;

error:
	for (; i >= 0; i--)
		device_remove_file(dev, &rszn_driver_attr[i]);

	pr_err("###%s: unable to create sysfs interface #### .... \n", __func__);

	return UWB_FAIL;
}

static int rszn_remove_sysfs_interfaces(struct device *dev)
{
	int i;

	pr_info("###%s: rszn enter running line: %d #### .... \n", __func__, __LINE__);

	for (i = 0; i < ARRAY_SIZE(rszn_driver_attr); i++)
		device_remove_file(dev, &rszn_driver_attr[i]);
		
	return UWB_SUCC;
}

static void rszn_set_chip_type_name(void)
{
	memset_s(rszn_chip_type, sizeof(rszn_chip_type), 0, sizeof(rszn_chip_type));
	if (rszn_get_dts_config("uwb_chip_type", "uwb_chip_type",
					rszn_chip_type, sizeof(rszn_chip_type)) < 0) {
		memset_s(rszn_chip_type, sizeof(rszn_chip_type), 0, sizeof(rszn_chip_type));
		strcpy_s(rszn_chip_type, CHIP_TYPE_BUFF_SIZE, DEV_NAME);
		pr_err("%s: can't get uwb_chip_type, default %s\n", __func__, DEV_NAME);
	}

	pr_info("%s: chip type name: %s\n", __func__, rszn_chip_type);
}

static void rszn_set_pmu38m_div_flag(void)
{
	int ret = rszn_get_dts_config_u32("pmu_38mclk", "pmu_38mclk", &g_pmu38m_div_flag);
	if (ret != 0) {
		g_pmu38m_div_flag = 0;
		pr_err("%s: can't pmu38m_div_flag\n", __func__);
	}

	pr_info("%s: pmu38m_div_flag:%d\n", __func__, g_pmu38m_div_flag);
}

static void rszn_set_2v85gpio_flag(void)
{
	int ret = rszn_get_dts_config_u32("2v85gpio_enbale", "2v85gpio_enbale", &g_2v85gpio_flag);
	if (ret != 0) {
		g_2v85gpio_flag = 0;
		pr_err("%s: can't 2v85gpio_enbale\n", __func__);
	}

	pr_info("%s: 2v85gpio_enbale:%d\n", __func__, g_2v85gpio_flag);
}

static void rszn_set_extclk_supply_flag(void)
{
	int ret = rszn_get_dts_config_u32("extclk_supply", "extclk_supply", &g_uwb_extra_clk_supply);
	if (ret != 0) {
		g_uwb_extra_clk_supply = 0;
		pr_err("%s: can't extclk_supply\n", __func__);
	}

	pr_info("%s: extclk_supply:%d\n", __func__, g_uwb_extra_clk_supply);
}

static void rszn_set_ldo53_supply_flag(void)
{
	int ret = rszn_get_dts_config_u32("lod53_not_supply", "lod53_not_supply", &g_rszn_ldo53_not_supply);
	if (ret != 0) {
		g_rszn_ldo53_not_supply = 0;
		pr_err("%s: can't lod53_not_supply\n", __func__);
	}

	pr_info("%s: lod53_not_supply:%d\n", __func__, g_rszn_ldo53_not_supply);
}

static void rszn_set_ldo25_1v25_supply_flag(void)
{
	int ret = rszn_get_dts_config_u32("ldo25_1v25_supply", "ldo25_1v25_supply", &g_rszn_ldo25_to_1v25);
	if (ret != 0) {
		g_rszn_ldo25_to_1v25 = 0;
		pr_err("%s: can't ldo25_1v25_supply\n", __func__);
	}

	pr_info("%s: ldo25_1v25_supply:%d\n", __func__, g_rszn_ldo25_to_1v25);
}

static void rszn_set_block_read_flag(void)
{
	int ret = rszn_get_dts_config_u32("block_read_supply", "block_read_supply", &g_rszn_block_read);
	if (ret != 0) {
		g_rszn_block_read = 0;
		pr_err("%s: can't block_read_supply\n", __func__);
	}

	pr_info("%s: block_read_supply:%d\n", __func__, g_rszn_block_read);
}

static void rszn_set_ldo58_always_on_flag(void)
{
	int ret = rszn_get_dts_config_u32("ldo58_2v85_always_on", "ldo58_2v85_always_on", &g_rszn_ldo58_always_on);
	if (ret != 0) {
		g_rszn_ldo58_always_on = 0;
		pr_err("%s: can't ldo58_2v85_always_on\n", __func__);
	}

	pr_info("%s: ldo58_2v85_always_on:%d\n", __func__, g_rszn_ldo58_always_on);
}

static void rszn_set_38m4_clk_enable(void)
{
	int ret;

	ret = rszn_get_dts_config_u32("38m4clk_enable_reg", "38m4clk_enable_reg", &g_clk_38m4_enable_reg);
	if (ret != 0) {
		g_clk_38m4_enable_reg = 0;
		pr_err("%s: can't read 38m4clk_enable_reg, use default reg.\n", __func__);
		return;
	}

	pr_info("%s: 38m4clk_enable_reg:%d\n", __func__, g_clk_38m4_enable_reg);
}

static void rszn_set_pmu_32k_clk_name(void)
{
	memset_s(rszn_32kclk_name, sizeof(rszn_32kclk_name), 0, sizeof(rszn_32kclk_name));
	if (rszn_get_dts_config("clock-32k", "clock-32k",
		rszn_32kclk_name, sizeof(rszn_32kclk_name)) < 0) {
		memset_s(rszn_32kclk_name, sizeof(rszn_32kclk_name), 0, sizeof(rszn_32kclk_name));
		strcpy_s(rszn_32kclk_name, CHIP_TYPE_BUFF_SIZE, DEV_NAME);
		pr_err("%s: can't get 32k_clk, default %s\n", __func__, DEV_NAME);
	} else {
		g_rszn_32kclk_supply = 1;
	}

	pr_info("%s: default 32k_clk: %s\n", __func__, rszn_32kclk_name);
}

static int rszn_create_device_attr(struct rszn_data *rszn)
{
	int status = UWB_FAIL;

	pr_info("###%s: rszn enter line: %d #### .... \n", __func__, __LINE__);

	rszn_set_chip_type_name();
	rszn_set_pmu38m_div_flag();
	rszn_set_2v85gpio_flag();
	rszn_set_extclk_supply_flag();
	rszn_set_ldo53_supply_flag();
	rszn_set_ldo25_1v25_supply_flag();
	rszn_set_block_read_flag();
	rszn_set_ldo58_always_on_flag();
	rszn_set_pmu_32k_clk_name();
	rszn_set_38m4_clk_enable();

	status = rszn_create_sysfs_interfaces(rszn->device);
	if (status < 0) {
		pr_err("%s: failed to rszn_create_sysfs_interfaces\n", __func__);
		return UWB_FAIL;
	}
	if (sysfs_create_link(NULL, &rszn->device->kobj, "uwb") < 0) {
		rszn_remove_sysfs_interfaces(rszn->device);
		pr_err("%s: failed to sysfs_create_link\n", __func__);
		return UWB_FAIL;
	}
	pr_info("###%s: rszn exit line: %d #### .... \n", __func__, __LINE__);

	return UWB_SUCC;
}

static int rszn_device_init(struct rszn_data *rszn, struct spi_device *spi)
{
	int status = 0;

	status = rszn_register_device(rszn);
	if (status < 0) {
		pr_err("%s: cann't rszn_register_device \n", __func__);
		return status;
	}
	status = rszn_create_device_attr(rszn);
	if (status < 0) {
		pr_err("%s: cann't rszn_create_device_attr \n", __func__);
		return status;
	}
	status = rszn_request_gpios(rszn, spi);
	if (status < 0) {
		pr_err("%s: cann't request gpios\n", __func__);
		return status;
	}
	status = rszn_request_irq(rszn, spi);
	if (status < 0) {
		pr_err("%s: cann't rszn_request_irq\n", __func__);
		return status;
	}
	status = uwb_perip_component_init(rszn, spi);
	if (status < 0) {
		pr_err("%s: cann't ComponentInit\n", __func__);
		return status;
	}

	return status;
}

static int rszn_probe(struct spi_device *spi)
{
	struct rszn_data *rszn;
	int status;

	pr_info("###%s: rszn enter running line: %d #### .... \n", __func__, __LINE__);

	/*
	 * rszn should never be referenced in DT without a specific
	 * compatible string, it is a Linux implementation thing
	 * rather than a description of the hardware.
	 */
	WARN(spi->dev.of_node && of_device_is_compatible(spi->dev.of_node, DEV_NAME),
		 "%pOF: buggy DT: rszn listed directly in DT\n", spi->dev.of_node);

	/* Allocate driver data */
	rszn = kzalloc(sizeof(struct rszn_data), GFP_KERNEL);
	if (!rszn) {
		pr_err("%s: cann't kzalloc \n", __func__);
		return -ENOMEM;
	}
	/* Initialize the driver data */
	rszn_data_init(rszn, spi);
	status = rszn_device_init(rszn, spi);
	if (status == 0) {
		pr_info("###%s: rszn enter running line: %d #### .... \n", __func__, __LINE__);
		spi_set_drvdata(spi, rszn);
	} else {
		pr_info("###%s: rszn enter running line: %d #### .... \n", __func__, __LINE__);
		rszn_free(rszn);
	}
	if (g_rszn_ldo58_always_on) {
		rszn_uwb_ldo_enable2v85(g_uwb_pmu_rszn);
	}
	pr_info("###%s: rszn exit running line: %d #### .... \n", __func__, __LINE__);

	return status;
}

static int rszn_remove(struct spi_device *spi)
{
	struct rszn_data *rszn = spi_get_drvdata(spi);

	pr_info("###%s: rszn enter running line: %d #### .... \n", __func__, __LINE__);
	rszn_free(rszn);

	return UWB_SUCC;
}

static const struct spi_device_id rszn_spi_ids[] = {
	{ .name = DEV_NAME },
	{},
};
MODULE_DEVICE_TABLE(spi, rszn_spi_ids);

static const struct of_device_id rszn_of_ids[] = {
	{ .compatible = "huawei,rszn" },
	{},
};
MODULE_DEVICE_TABLE(of, rszn_of_ids);

static struct spi_driver rszn_spi_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name = DEV_NAME,
		.of_match_table = of_match_ptr(rszn_of_ids),
	},
	.probe =	rszn_probe,
	.remove =	rszn_remove,
	.id_table =	rszn_spi_ids,

	/* NOTE:  suspend/resume methods are not necessary here.
	 * We don't do anything except pass the requests to/from
	 * the underlying controller.  The refrigerator handles
	 * most issues; the controller driver handles the rest.
	 */
};

/*-------------------------------------------------------------------------*/

static int __init rszn_init(void)
{
	int status;

	pr_info("%s: rszn enter\n", __func__);

	rszn_class = class_create(THIS_MODULE, DEV_NAME);
	if (IS_ERR(rszn_class)) {
		pr_err("rszn class_create error!\n");
		return PTR_ERR(rszn_class);
	}

	status = spi_register_driver(&rszn_spi_driver);
	if (status < 0) {
		pr_err("rszn spi_register_driver error!\n");
		class_destroy(rszn_class);
	}

	pr_info("%s: rszn exit\n", __func__);

	return status;
}
module_init(rszn_init);


static void __exit rszn_exit(void)
{
	spi_unregister_driver(&rszn_spi_driver);
	class_destroy(rszn_class);
	pr_info("###%s: rszn driver exit #### .... \n", __func__);
}
module_exit(rszn_exit);

MODULE_AUTHOR("rszn, <rszn@huawei.com>");
MODULE_DESCRIPTION("Huawei rszn UWB SPI driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("spi:rszn");

