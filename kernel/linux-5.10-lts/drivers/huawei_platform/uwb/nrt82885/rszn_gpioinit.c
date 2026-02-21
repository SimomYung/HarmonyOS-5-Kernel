/*
 * vendor_exception.c
 *
 * Copyright (C) 2018 Huawei Device Co., Ltd.
 * This file is licensed under GPL.
 *
 * Register vendor exceptions.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/irq.h>
#include <linux/jiffies.h>
#include <linux/uaccess.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/miscdevice.h>
#include <linux/spinlock.h>
#include <linux/reboot.h>
#include <linux/clk.h>
#include <linux/pm_wakeup.h>
#include <linux/of.h>
#include <linux/gpio.h>
#include <linux/of_platform.h>
#include <linux/of_device.h>
#include <linux/of_gpio.h>
#include <linux/regulator/consumer.h>
#include <linux/mtd/hisi_nve_interface.h>

#include "rszn_gpioinit.h"
#include "securec.h"
#include <linux/mfd/hisi_pmic.h>
#include <chipset_common/hwpower/hardware_ic/boost_5v.h>
#include <huawei_platform/log/hw_log.h>
#include <linux/clk.h>

#define PR_TAG UWB

#define PMU_HI_6421 1
#define PMU_HI_6423 2
#define PMU_SIM_0 0
#define PMU_SIM_1 1
#define DELAY_AFTER_2V8 10
#define DELAY_AFTER_1V2 20
#define DELAY_AFTER_32K 5
#define DELAY_AFTER_38M4 5
#define DELAY_POWER_OFF_AFTER_1V2 10
#define DELAY_POWER_OFF_AFTER_RST 2

static rszn_pmu_reg_control_stu g_uwb_38m_clk_enb_by_hi6421 = {0x043, 0x01};
static rszn_pmu_reg_control_stu g_uwb_38m_clk_freq_by_hi6421 = {0x0aa, 0x4};
static rszn_pmu_reg_control_stu g_uwb_38m_clk_vpp_adj_by_hi6421 = {0x0a9, 0x02};

static int
rszn_uwb_pmureg_operate_by_byte(int pmuid, int regaddr, int byte)
{
	int value = 0;

	pr_info("UWB: %s, in\n", __func__);

	if (regaddr <= 0 || byte < 0) {
		pr_err("UWB: %s: reg_addr[%x], op[%d], error!\n", __func__, regaddr, byte);
		return UWB_FAIL;
	}
	if (pmuid == PMU_HI_6421) {
		pmic_write_reg(regaddr, byte);
		pr_info("UWB: %s: pmic_write_reg: 0x%x, value: 0x%x\n", __func__, regaddr, byte);

		value = pmic_read_reg(regaddr);
		pr_info("UWB: %s: pmic_read_reg: 0x%x, value: 0x%x\n", __func__, regaddr, value);
		return UWB_SUCC;
	}
	if (pmuid == PMU_HI_6423) {
		sub_pmic_reg_write(regaddr, byte);
		pr_info("UWB: %s: sub_pmic_reg_write: 0x%x, value: 0x%x\n", __func__, regaddr, byte);

		value = sub_pmic_reg_read(regaddr);
		pr_info("UWB: %s: sub_pmic_reg_read: 0x%x, value: 0x%x\n", __func__, regaddr, value);
		return UWB_SUCC;
	}
	pr_err("UWB: %s: no match pmuid\n", __func__);

	return UWB_FAIL;
}

void
rszn_uwb_hardware_reset(struct rszn_data *rszn)
{
	if (rszn->reset_gpio > 0)
		gpio_set_value(rszn->reset_gpio, UWB_GPIO_LOW);
	msleep(RESET_DELAY);
	if (rszn->reset_gpio > 0) {
		gpio_set_value(rszn->reset_gpio, UWB_GPIO_HIGH);
	}
}

static int
rszn_uwb_ldo_enable(rszn_uwb_power_supply uwbldo)
{
	int status = UWB_FAIL;

	/* 2v85 adjust */
	status = regulator_set_voltage(uwbldo.ldo_reg54, 2850000, 2850000);
	if (status) {
		pr_err("UWB:%s, 2v85 poweradj fail, error: 0x%x\n", __func__, status);
		return UWB_FAIL;
	}
	
	/* 1v2 adjust */
	status = regulator_set_voltage(uwbldo.ldo_reg25, 1200000, 1200000);
	if (status) {
		pr_err("UWB:%s, 1v2 poweradj fail, error: 0x%x\n", __func__, status);
		return UWB_FAIL;
	}

	/* 2v85 power on */
	status = regulator_enable(uwbldo.ldo_reg54);
	if (status) {
		pr_err("UWB:%s, 2v85 poweron fail, error: 0x%x\n", __func__, status);
		return UWB_FAIL;
	}

	msleep(DELAY_AFTER_2V8);

	/* 1v2 power on */
	status = regulator_enable(uwbldo.ldo_reg25);
	if (status) {
		pr_err("UWB:%s, 2v85 poweron fail, error: 0x%x\n", __func__, status);
		return UWB_FAIL;
	}
	
	status = regulator_enable(uwbldo.ldo_reg53);
	if (status) {
		pr_err("UWB:%s, ldo53 poweron fail, error: 0x%x\n", __func__, status);
		return UWB_FAIL;
	}

	return UWB_SUCC;
}

static int
rszn_uwb_extra_clk32k_disable(rszn_uwb_power_supply uwbldo)
{
	pr_info("UWB: %s, in\n", __func__);

	if (uwbldo.clk32kb == NULL) {
		pr_err("UWB:%s, input error\n", __func__);
		return UWB_FAIL;
	}
	clk_disable_unprepare(uwbldo.clk32kb);
	msleep(DELAY_AFTER_32K);

	return UWB_SUCC;
}

static int
rszn_uwb_extra_clk38m4_disable(rszn_uwb_power_supply uwbldo)
{
	int ret = UWB_FAIL;

	pr_info("UWB: %s, in\n", __func__);

	ret = rszn_uwb_pmureg_operate_by_byte(PMU_HI_6421, g_uwb_38m_clk_enb_by_hi6421.addr, CLR_BIT);
	if (ret != UWB_SUCC) {
		pr_err("UWB: %s, 38m disable fail, error: 0x%x\n", __func__, ret);
		return UWB_FAIL;
	}
	msleep(DELAY_AFTER_38M4);

	return UWB_SUCC;
}

static int
rszn_uwb_ldo_extra_clk_disable(rszn_uwb_power_supply uwbldo)
{
	int status = UWB_FAIL;

	pr_info("UWB: %s, in\n", __func__);

	status = regulator_disable(uwbldo.ldo_reg25);
	if (status) {
		pr_err("UWB:%s, ldo25 poweroff fail, error: 0x%x\n", __func__, status);
		return UWB_FAIL;
	}
	msleep(DELAY_POWER_OFF_AFTER_1V2);

	status = rszn_uwb_extra_clk38m4_disable(uwbldo);
	if (status == UWB_FAIL) {
		pr_err("UWB: %s, rszn_uwb_extra_clk38m4_disable fail\n", __func__);
		return status;
	}

	status = rszn_uwb_extra_clk32k_disable(uwbldo);
	if (status == UWB_FAIL) {
		pr_err("UWB: %s, rszn_uwb_extra_clk32k_disable fail\n", __func__);
		return status;
	}

	status = regulator_disable(uwbldo.ldo_reg54);
	if (status) {
		pr_err("UWB:%s, ldo54 poweroff fail, error: 0x%x\n", __func__, status);
		return UWB_FAIL;
	}

	status = regulator_disable(uwbldo.ldo_reg53);
	if (status) {
		pr_err("UWB:%s, ldo53 poweroff fail, error: 0x%x\n", __func__, status);
		return UWB_FAIL;
	}

	return UWB_SUCC;
}

static int
rszn_uwb_chip_poweron(struct rszn_data *spidev, rszn_uwb_power_supply uwbldo)
{
	int ret = UWB_FAIL;

	pr_info("UWB: %s, rszn in\n", __func__);

	if (uwbldo.ldo_reg25 == NULL  ||
		uwbldo.ldo_reg54 == NULL || uwbldo.ldo_reg53 == NULL) {
		pr_err("UWB:%s, input error\n", __func__);
		return UWB_FAIL;
	}
	ret = rszn_uwb_ldo_enable(uwbldo);
	if (ret != UWB_SUCC) {
		pr_err("UWB:%s, rszn_uwb_ldo_enable fail, error: 0x%x\n", __func__, ret);
		return UWB_FAIL;
	}

	/* 2v85 power on */
	if (uwbldo.gpio_enable2v85 == 1) {
		gpio_direction_output(spidev->power_gpio, SET_BIT);
	} else {
		pr_err("UWB:%s, ldo54 not need gpio\n", __func__);
	}
	msleep(DELAY_AFTER_1V2);

	return UWB_SUCC;
}

static int
rszn_uwb_chip_poweroff_clk_disable(struct rszn_data *spidev, rszn_uwb_power_supply uwbldo)
{
	int ret = UWB_FAIL;

	pr_info("UWB: %s, in\n", __func__);

	if (uwbldo.ldo_reg25 == NULL) {
		pr_err("UWB:%s, input error\n", __func__);
		return UWB_FAIL;
	}
	ret = rszn_uwb_ldo_extra_clk_disable(uwbldo);
	if (ret != UWB_SUCC) {
		pr_err("UWB:%s, UwbLdoDisable fail, error: 0x%x\n", __func__, ret);
		return UWB_FAIL;
	}
	if (uwbldo.gpio_enable2v85 == 1) {
		gpio_direction_output(spidev->power_gpio, CLR_BIT); // 2v85 power off
	} else {
		pr_err("UWB:%s, ldo54 not need gpio\n", __func__);
	}

	return UWB_SUCC;
}

int
rszn_uwb_extra_clk_enable(rszn_uwb_power_supply uwbldo)
{
	int ret = UWB_FAIL;
	int value;

	pr_info("UWB: %s, in\n", __func__);

	if (uwbldo.clk32kb == NULL) {
		pr_err("UWB:%s, input error\n", __func__);
		return UWB_FAIL;
	}

	if (uwbldo.clk_38m4_enable_reg != 0)
		g_uwb_38m_clk_enb_by_hi6421.addr = uwbldo.clk_38m4_enable_reg;
	/* enable 32k clk */
	value = pmic_read_reg(0x4a);
	pr_info("UWB: %s: value1: 0x%x\n", __func__, value);
	ret = clk_prepare_enable(uwbldo.clk32kb);
	if (ret) {
		pr_err("%s: 32K clk enable fail fail, err: %d", ret);
		return UWB_FAIL;
	}
	value = pmic_read_reg(0x4a);
	pr_info("UWB: %s: value2: 0x%x\n", __func__, value);
	msleep(DELAY_AFTER_32K);

	/* enable 38m clk */
	if (uwbldo.freq2div38m == 1) {
		ret = rszn_uwb_pmureg_operate_by_byte(PMU_HI_6421, g_uwb_38m_clk_freq_by_hi6421.addr, g_uwb_38m_clk_freq_by_hi6421.value);
		pr_info("UWB:%s, 78.6M need 2 div\n", __func__);
	}
	ret = rszn_uwb_pmureg_operate_by_byte(PMU_HI_6421, g_uwb_38m_clk_vpp_adj_by_hi6421.addr, g_uwb_38m_clk_vpp_adj_by_hi6421.value);
	if (ret != UWB_SUCC) {
		pr_err("UWB: %s, 38m clk adjust Vpp = 1V, error: 0x%x\n", __func__, ret);
		return UWB_FAIL;
	}
	ret = rszn_uwb_pmureg_operate_by_byte(PMU_HI_6421, g_uwb_38m_clk_enb_by_hi6421.addr, SET_BIT);
	if (ret != UWB_SUCC) {
		pr_err("UWB: %s, 38m clk enable fail, error: 0x%x\n", __func__, ret);
		return UWB_FAIL;
	}
	msleep(DELAY_AFTER_38M4);

	return UWB_SUCC;
}

static void
rszn_uwb_gpio_init(struct rszn_data *spidev)
{
	pr_info("UWB: %s, in\n", __func__);

	if (spidev->wakeup_gpio > 0)
		gpio_set_value(spidev->wakeup_gpio, UWB_GPIO_LOW);
	msleep(WAKEUP_DELAY);
	if (spidev->wakeup_gpio > 0)
		gpio_set_value(spidev->wakeup_gpio, UWB_GPIO_HIGH);
	msleep(WAKEUP_DELAY);
	if (spidev->reset_gpio > 0)
		gpio_set_value(spidev->reset_gpio, UWB_GPIO_HIGH);
	msleep(RESET_ON_DELAY);
	if (spidev->cs_gpio > 0)
		gpio_direction_output(spidev->cs_gpio, UWB_GPIO_HIGH);
}

static void
rszn_uwb_gpio_deinit(struct rszn_data *spidev)
{
	pr_info("UWB: %s, in\n", __func__);

	if (spidev->wakeup_gpio > 0)
		gpio_set_value(spidev->wakeup_gpio, UWB_GPIO_LOW);
	if (spidev->reset_gpio > 0)
		gpio_set_value(spidev->reset_gpio, UWB_GPIO_LOW);
	if (spidev->cs_gpio > 0)
		gpio_direction_input(spidev->cs_gpio);
}

int
rszn_uwb_open_init(struct rszn_data *spidev, rszn_uwb_power_supply uwbldo)
{
	int ret = UWB_FAIL;

	pr_info("UWB: %s, in\n", __func__);

	if (spidev == NULL) {
		pr_err("UWB: %s, uwb_open_init spidev NULL\n", __func__);
		return ret;
	}

	ret = rszn_uwb_chip_poweron(spidev, uwbldo);
	if (ret == UWB_FAIL) {
		pr_err("UWB: %s, rszn_uwb_chip_poweron fail\n", __func__);
		return ret;
	}

	ret = rszn_uwb_extra_clk_enable(uwbldo);
	if (ret == UWB_FAIL) {
		pr_err("UWB: %s, rszn_uwb_extra_clk_enable fail\n", __func__);
		return ret;
	}
	rszn_uwb_gpio_init(spidev);

	return UWB_SUCC;
}

int
rszn_uwb_close_deinit(struct rszn_data *spidev, rszn_uwb_power_supply uwbldo)
{
	int ret = UWB_FAIL;

	pr_info("UWB: %s, in\n", __func__);

	if (spidev == NULL) {
		pr_err("UWB: %s, uwb_close_deinit spidev NULL\n", __func__);
		return ret;
	}
	rszn_uwb_gpio_deinit(spidev);
	msleep(DELAY_POWER_OFF_AFTER_RST);
	ret = rszn_uwb_chip_poweroff_clk_disable(spidev, uwbldo);
	if (ret == UWB_FAIL) {
		pr_err("UWB: %s, rszn_uwb_chip_poweroff fail\n", __func__);
		return ret;
	}

	return UWB_SUCC;
}

void
rszn_cs_high(struct rszn_data *rszn)
{
	if (rszn->cs_gpio > 0)
        gpio_direction_output(rszn->cs_gpio, UWB_GPIO_HIGH);
}

void
rszn_cs_low(struct rszn_data *rszn)
{
	if (rszn->cs_gpio > 0)
        gpio_direction_output(rszn->cs_gpio, UWB_GPIO_LOW);
}

void
rszn_reset_high(struct rszn_data *rszn)
{
	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);
	if (rszn->reset_gpio > 0)
		gpio_set_value(rszn->reset_gpio, UWB_GPIO_HIGH);
}

void
rszn_reset_low(struct rszn_data *rszn)
{
	pr_info("###%s: %d; rszn enter #### .... \n", __func__, __LINE__);
	if (rszn->reset_gpio > 0)
		gpio_set_value(rszn->reset_gpio, UWB_GPIO_LOW);
}

int
rszn_wakeup_high(struct rszn_data *rszn)
{
	if (rszn->wakeup_gpio > 0)
		gpio_set_value(rszn->wakeup_gpio, UWB_GPIO_HIGH);

	return UWB_SUCC;
}

int
rszn_wakeup_low(struct rszn_data *rszn)
{
	if (rszn->wakeup_gpio > 0)
		gpio_set_value(rszn->wakeup_gpio, UWB_GPIO_LOW);

	return UWB_SUCC;
}

static int
rszn_request_gpio_irq(struct rszn_data *rszn)
{
	int				 	status  = -1;
	struct spi_device   *spi	= rszn->spi;

	rszn->irq_gpio = of_get_named_gpio(spi->dev.of_node, "irq_gpio", 0);
	if (rszn->irq_gpio > 0) {
		status = gpio_request(rszn->irq_gpio, "rszn-irq");
		if (status < 0) {
			pr_err("RSZN %s failed!", __func__);
			rszn->irq_gpio = 0;
			rszn_free_gpios(rszn);

			return status;
		}
		gpio_direction_input(rszn->irq_gpio);
	}

	return status;
}

static int
rszn_request_gpio_reset(struct rszn_data *rszn)
{
	int				 	status  = -1;
	struct spi_device   *spi	= rszn->spi;

	rszn->reset_gpio = of_get_named_gpio(spi->dev.of_node, "rst_gpio", 0);
	if (rszn->reset_gpio > 0) {
		status = gpio_request(rszn->reset_gpio, "rszn-reset");
		if (status < 0) {
			pr_err("RSZN %s failed!", __func__);
			rszn->reset_gpio = 0;
			rszn_free_gpios(rszn);

			return status;
		}
		gpio_direction_output(rszn->reset_gpio, UWB_GPIO_LOW);
	}

	return status;
}

static int
rszn_request_gpio_wakeup(struct rszn_data *rszn)
{
	int				 	status  = -1;
	struct spi_device   *spi	= rszn->spi;

	rszn->wakeup_gpio = of_get_named_gpio(spi->dev.of_node, "wke_gpio", 0);
	if (rszn->wakeup_gpio > 0) {
		status = gpio_request(rszn->wakeup_gpio, "rszn-wakeup");
		if (status < 0) {
			pr_err("RSZN %s failed!", __func__);
			rszn->reset_gpio = 0;
			rszn_free_gpios(rszn);

			return status;
		}
		gpio_direction_output(rszn->wakeup_gpio, UWB_GPIO_LOW);
	}

	return status;
}

static int
rszn_request_gpio_cs(struct rszn_data *rszn)
{
	int status = -1;

	rszn->cs_gpio = of_get_named_gpio(rszn->spi->dev.of_node, "cs_gpio", 0);
	if (rszn->cs_gpio > 0) {
		status = gpio_request(rszn->cs_gpio, "rszn-cs");
		if (status < 0) {
			pr_err("RSZN %s failed!", __func__);
			rszn->reset_gpio = 0;
			rszn_free_gpios(rszn);

			return status;
		}
        gpio_direction_input(rszn->cs_gpio);
	}

	return status;
}

int
rszn_request_gpios(struct rszn_data *rszn, struct spi_device *spi)
{
	int status;

	unused(spi);

	status = rszn_request_gpio_reset(rszn);
	if (status < 0) {
		pr_err("RSZN %s rszn_request_gpio_reset error!", __func__);
		return status;
	}

	status = rszn_request_gpio_wakeup(rszn);
	if (status < 0) {
		pr_err("RSZN %s rszn_request_gpio_wakeup error!", __func__);
		return status;
	}

	status = rszn_request_gpio_cs(rszn);
	if (status < 0) {
		pr_err("RSZN %s rszn_request_gpio_cs error!", __func__);
		return status;
	}

	status = rszn_request_gpio_irq(rszn);

	return status;
}

int
rszn_free_gpios(struct rszn_data *rszn)
{
	if (rszn->power_gpio > 0) {
		gpio_direction_output(rszn->power_gpio, POWER_OFF);
		gpio_free(rszn->power_gpio);
		rszn->power_gpio = 0;
	}

	if (rszn->reset_gpio > 0) {
		gpio_direction_output(rszn->reset_gpio, UWB_GPIO_LOW);
		gpio_free(rszn->reset_gpio);
		rszn->reset_gpio = 0;
	}

	if (rszn->wakeup_gpio > 0) {
		gpio_direction_output(rszn->wakeup_gpio, UWB_GPIO_LOW);
		gpio_free(rszn->wakeup_gpio);
		rszn->wakeup_gpio = 0;
	}

	if (rszn->cs_gpio > 0) {
        gpio_direction_input(rszn->cs_gpio);
		gpio_free(rszn->cs_gpio);
		rszn->reset_gpio = 0;
	}

	if (rszn->irq_gpio > 0) {
		gpio_free(rszn->irq_gpio);
		rszn->irq_gpio =0 ;
	}

	return 0;
}

static irqreturn_t
rszn_interrupt(int irq, void *dev_id)
{
	struct rszn_data	*rszn = dev_id;
	unsigned long	   	flags;

	unused(irq);

	spin_lock_irqsave(&rszn->irq_lock, flags);
	if (rszn->wait_irq == WAIT_IRQ_ON) {
		pr_err("RSZN %s wait_irq off!", __func__);
		rszn->wait_irq = WAIT_IRQ_OFF;
	}
	spin_unlock_irqrestore(&rszn->irq_lock, flags);
	wake_up_interruptible(&rszn->wait);

	return IRQ_HANDLED;
}

int
rszn_request_irq(struct rszn_data *rszn, struct spi_device *spi)
{
	int			 	status = 0;
	unsigned long   flags;

	rszn->irq = gpio_to_irq(rszn->irq_gpio);
	pr_err("RSZN %s rszn->irq num:%d\n", __func__, rszn->irq);
	if (rszn->irq < 0) {
		status = rszn->irq;
	} else {
		spin_lock_irqsave(&rszn->irq_lock, flags);
		rszn->wait_irq = WAIT_IRQ_ON;
		spin_unlock_irqrestore(&rszn->irq_lock, flags);

		status = devm_request_irq(&spi->dev, rszn->irq, rszn_interrupt, IRQF_TRIGGER_RISING | IRQF_ONESHOT,
			"rszn", rszn);
		if (status < 0) {
			pr_err("RSZN %s error !\n", __func__);
			rszn->irq = 0;
		} else {
			enable_irq(rszn->irq);
		}
	}

	return status;
}