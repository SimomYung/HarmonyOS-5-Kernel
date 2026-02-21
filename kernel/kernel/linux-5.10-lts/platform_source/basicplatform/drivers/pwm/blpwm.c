/*
 * blpwm.c
 *
 * pwm module, Provide pwm interface for pmu&soc
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
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
 */

#include <linux/pwm.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/clk.h>
#include <linux/of_device.h>
#include <linux/types.h>
#include <linux/version.h>
#include <linux/io.h>
#include <linux/bitops.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/time64.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/pinctrl/consumer.h>

#define START_HIGH_OFFSET 0x10
#define RESET_REG_CNT 4
#define REG_MAX_IN_CNT 65535
#define NS 1000000000
#define PWM_MAX_CLK_DIV 0x100
#define MAX_TIMEOUT 5000000
#define GET_HIGH_16BIT(val)        \
        	(((val) & (0xFFFF << 16)) >> 16)

struct blpwm_reset_regs {
	u32 reset_enable;
	u32 reset_disable;
	u32 reset_status;
	u32 reset_bit;
};

struct blpwm_regs {
	u32 clk_out_div_addr;
	u32 clk_in_div_addr;
	u32 duty_cyle_addr;
	u32 out_ctrl_addr;
	u32 in_count_addr;
	u32 in_enable_addr;
};

struct blpwm_data_private {
	struct blpwm_regs regs;
	unsigned int reserved;
};

struct blpwm_pinctrl {
	u32 pinctrl_used;
	struct pinctrl *pin_ctrl;
	u32 pinctrl_flag;
};

struct blpwm_chip {
	struct pwm_chip chip;
	struct device *dev;
	void __iomem *base;
	struct clk *clk;
	unsigned long clk_rate;
	bool clk_enabled;
	u32 clk_div;
	const struct blpwm_data_private *data;
	void __iomem *reset_base;
	struct blpwm_reset_regs reset_regs;
    u32 min_in_freq;
    u32 dis_power_up_output;
	spinlock_t reg_sequence_lock;
	struct blpwm_pinctrl pinctrl;
};

static const struct blpwm_data_private blpwm_data = {
	.regs = {
		.clk_out_div_addr = 0x104,
		.duty_cyle_addr = 0x108,
		.out_ctrl_addr = 0x100,
		.clk_in_div_addr = 0x4,
		.in_count_addr = 0x8,
		.in_enable_addr = 0x0,
	},
	.reserved = 0,
};

static inline struct blpwm_chip *to_blpwm_chip(const struct pwm_chip *c)
{
	return container_of(c, struct blpwm_chip, chip);
}

static inline void write_reg(void *reg_addr, u32 reg_val)
{
	writel(reg_val, reg_addr);
}

static inline void blpwm_bypass_mode_disable(const struct blpwm_chip *hpc)
{
    u32 val;
    val = readl(hpc->base + hpc->data->regs.out_ctrl_addr) & ~BIT(2);
	write_reg(hpc->base + hpc->data->regs.out_ctrl_addr, val);
}

static inline void blpwm_disable(const struct blpwm_chip *hpc)
{
	write_reg(hpc->base + hpc->data->regs.out_ctrl_addr, 0x0);
}

static inline void blpwm_enable(const struct blpwm_chip *hpc)
{
	write_reg(hpc->base + hpc->data->regs.out_ctrl_addr, 0x1);
}

static void blpwm_config(const struct blpwm_chip *hpc,
	struct pwm_device *pwm,
	u64 period_ns, u64 duty_ns, enum pwm_polarity polarity)
{
	u64 period;
	u64 duty_cycle;
	u64 start_high;
	u64 start_low;
	u32 val;

	/* user input period is ns, blpwm mini period is us */
	period = period_ns / 1000;
	duty_cycle = duty_ns / 1000;
	if ((period == 0) || (period >= 0xFFFFFFFF) || \
		(duty_cycle > period) || (duty_cycle >= 0xFFFF)) {
		dev_err(hpc->chip.dev, "%s, period %d or duty %d is invalid\n", \
			__func__, period, duty_cycle);
		return;
	}

	start_high = duty_cycle;
	start_low = period - start_high;
	if (start_low >= 0xFFFF) {
		dev_err(hpc->chip.dev, "%s, start_low %d is invalid\n", \
			__func__, start_low);
		return;
	}

	write_reg(hpc->base + hpc->data->regs.clk_out_div_addr, hpc->clk_div);
	
	val = (start_high << 16) + start_low;
	write_reg(hpc->base + hpc->data->regs.duty_cyle_addr, val);
	dev_info(hpc->chip.dev, "%s, duty_cyle value is %d\n", __func__, val);

	pwm->state.period = period_ns;
	pwm->state.duty_cycle = duty_ns;
	pwm->state.polarity = PWM_POLARITY_NORMAL; /* non support polarity cfg */
}

/*
 * Helper function to manage clk enable state.
 * 1.Without clk enabled, the pwm reg cannot be read.
 * 2.pwm_apply maybe called with enable and disable state not the same counts.
 *
 * @param hpc driver data.
 * @return int 0 ok, otherwise fail.
 */
static int blpwm_clk_enable(struct blpwm_chip *hpc)
{
	int ret;

	if (hpc->clk_enabled)
		return 0;
	ret = clk_enable(hpc->clk);
	if (ret) {
		dev_err(hpc->chip.dev, "%s, clk_enable fail\n", __func__);
		return ret;
	}
	hpc->clk_enabled = true;
	return 0;
}

static void blpwm_clk_disable(struct blpwm_chip *hpc)
{
	if (!hpc->clk_enabled)
		return;
	clk_disable(hpc->clk);
	hpc->clk_enabled = false;
}


static bool ptr_null_check(struct pwm_chip *chip, struct pwm_device *pwm,
	const struct pwm_state *state)
{
	if (!chip || !pwm || !state)
		return false;

	return true;
}

static void blpwm_get_state(struct pwm_chip *chip, struct pwm_device *pwm,
	struct pwm_state *state)
{
	struct blpwm_chip *hpc = NULL;

	if (!ptr_null_check(chip, pwm, state))
		return;

	state->period = pwm->state.period;
	state->duty_cycle = pwm->state.duty_cycle;
	state->polarity = pwm->state.polarity;
	state->enabled = pwm->state.enabled;
}

static int blpwm_pins_select(struct blpwm_chip *hpc, const char *name)
{
    int status;
	struct device *dev = hpc->dev;
    struct pinctrl_state *pin_state = NULL;
	
	if (!hpc->pinctrl.pinctrl_used)
		return 0;

	if (hpc->pinctrl.pinctrl_flag == 0) {
		hpc->pinctrl.pin_ctrl = devm_pinctrl_get(dev);
		if (IS_ERR(hpc->pinctrl.pin_ctrl)) {
			dev_err(dev, "blpwm pinctrl property isn't exist!\n");
			return -EFAULT;
		}
		hpc->pinctrl.pinctrl_flag = 1;
	}

	pin_state = pinctrl_lookup_state(hpc->pinctrl.pin_ctrl, name);
	if (IS_ERR(pin_state)) {
		devm_pinctrl_put(hpc->pinctrl.pin_ctrl);
		hpc->pinctrl.pinctrl_flag = 0;
		dev_err(dev, "could not get pin state\n");
		return -EFAULT;
	}

	status = pinctrl_select_state(hpc->pinctrl.pin_ctrl, pin_state);
	if (status) {
        devm_pinctrl_put(hpc->pinctrl.pin_ctrl);
		hpc->pinctrl.pinctrl_flag = 0;
		dev_err(dev, "could not set pins\n");
		return -EFAULT;
	}
	return 0;
}

static int blpwm_apply(struct pwm_chip *chip, struct pwm_device *pwm,
	const struct pwm_state *state)
{
	struct blpwm_chip *hpc = NULL;
	int status;

	if (!ptr_null_check(chip, pwm, state))
		return -EINVAL;

	hpc = to_blpwm_chip(chip);
	if (blpwm_clk_enable(hpc))
		return -ENODEV;

	if (state->enabled) {
		status = blpwm_pins_select(hpc, PINCTRL_STATE_DEFAULT);
		if (status) {
			dev_err(hpc->dev, "could not set default pins\n");
			return -EIO;
		}

		blpwm_config(hpc, pwm,
			state->period, state->duty_cycle, state->polarity);
		blpwm_enable(hpc);
		pwm->state.enabled = true;
	} else {
		blpwm_disable(hpc);
		pwm->state.enabled = false;

		status = blpwm_pins_select(hpc, PINCTRL_STATE_IDLE);
		if (status)
			dev_err(hpc->dev, "could not set idle(gpio) pins\n");
	}

	return 0;
}

static int blpwm_apply_state(struct pwm_chip *chip, struct pwm_device *pwm,
	const struct pwm_state *state)
{
	return blpwm_apply(chip, pwm, state);
}

static int blpwm_capture(struct pwm_chip *chip, struct pwm_device *pwm, struct pwm_capture *result, unsigned long timeout)
{
	struct blpwm_chip *hpc = NULL;
	u32 clk_div = 1;
    u32 input_cont = 0;
	u32 period, hi_duty_cycle, low_duty_cycle;

	if (!pwm || !result)
		return -EINVAL;

	hpc = to_blpwm_chip(chip);
	if (blpwm_clk_enable(hpc))
		return -ENODEV;
    
    if (!hpc->min_in_freq) {
        dev_err(hpc->chip.dev, "%s doesn't have min input frequency\n", __func__);
		return -EINVAL;
    }

    if (timeout > MAX_TIMEOUT) {
        dev_err(hpc->chip.dev, "%s The timeout is longer than 5s.\n", __func__);
		return -EINVAL;
    }

	/* Calculate the frequency division. */
	while(clk_div <= PWM_MAX_CLK_DIV && ((hpc->clk_rate / clk_div) / hpc->min_in_freq) / 2 > REG_MAX_IN_CNT)
		clk_div++;

	if (clk_div == PWM_MAX_CLK_DIV) {
		dev_err(hpc->chip.dev, "%s clk_div out of range\n", __func__);
		return -EINVAL;
	}
    write_reg(hpc->base + hpc->data->regs.clk_in_div_addr, clk_div - 1);
	dev_info(hpc->chip.dev, "blpwm clk_div[%d]  clk_rate[%d]\n", clk_div, hpc->clk_rate);
	
	write_reg(hpc->base + hpc->data->regs.in_enable_addr, 0x1);
    usleep_range(timeout, timeout + 10);
    input_cont = readl(hpc->base + hpc->data->regs.in_count_addr);
	write_reg(hpc->base + hpc->data->regs.in_enable_addr, 0x0);

	low_duty_cycle = input_cont & 0xFFFF;
	hi_duty_cycle = GET_HIGH_16BIT(input_cont);
	period = low_duty_cycle + hi_duty_cycle;

    /* The unit time returned to the upper layer is nanosecond. */
    /* Convert the number of sampling periods to the input period. */
	period *= (NS / (hpc->clk_rate / clk_div));
	hi_duty_cycle *= (NS / (hpc->clk_rate / clk_div));
	result->period = period;
	result->duty_cycle = hi_duty_cycle;
	return 0;
}

static const struct pwm_ops blpwm_ops = {
	.get_state = blpwm_get_state,
	.apply = blpwm_apply_state,
	.capture = blpwm_capture,
	.owner = THIS_MODULE,
};

static const struct of_device_id blpwm_dt_ids[] = {
	{ .compatible = "hisilicon,blpwm", .data = &blpwm_data },
	{},
};

static int add_blpwm_chip(struct blpwm_chip *hpc, struct device *pdev)
{
	int ret;

	hpc->chip.dev = pdev;
	hpc->chip.ops = &blpwm_ops;
	hpc->chip.base = -1;
	hpc->chip.npwm = 1;

	hpc->chip.of_xlate = NULL;
	hpc->chip.of_pwm_n_cells = 0; /* of_xlate is NULL of_pwmchip_add will set of_pwm_n_cells = 2 */

	ret = pwmchip_add(&hpc->chip);
	if (ret) {
		dev_err(pdev, "%s, pwmchip_add failed\n", __func__);
		return ret;
	}
	return 0;
}

static int blpwm_reset_cfg(struct blpwm_chip *hpc,
	struct platform_device *pdev)
{
	int ret;
	u32 reset_reg_arry[RESET_REG_CNT];

	ret = of_property_read_u32_array(pdev->dev.of_node, "reset-reg-base", &reset_reg_arry[0], ARRAY_SIZE(reset_reg_arry));
	if (ret) {
		dev_info(&pdev->dev, "doesn't have reset-reg-base property!\n");
		return -EINVAL;
	} else {
		hpc->reset_base = devm_ioremap(&pdev->dev, reset_reg_arry[1], reset_reg_arry[3]);
		dev_info(&pdev->dev, "i2c reset register phy_addr is: %x\n", reset_reg_arry[1]);
	}

	ret = of_property_read_u32_array(pdev->dev.of_node, "reset-controller-reg", &reset_reg_arry[0], ARRAY_SIZE(reset_reg_arry));
	if (ret) {
		dev_info(&pdev->dev, "doesn't have reset-controller-reg property!\n");
		return -EINVAL;
	} else {
		hpc->reset_regs.reset_enable = reset_reg_arry[0];
		hpc->reset_regs.reset_disable = reset_reg_arry[1];
		hpc->reset_regs.reset_status = reset_reg_arry[2];
		hpc->reset_regs.reset_bit = reset_reg_arry[3];
	}
	/* disable reset */
	write_reg(hpc->reset_base + hpc->reset_regs.reset_disable, BIT(hpc->reset_regs.reset_bit));
	return 0;
}

static int blpwm_clk_prepare(struct blpwm_chip *hpc,
	struct platform_device *pdev)
{
	int ret;
	u64 clk_rate = 0;

	hpc->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(hpc->clk)) {
		dev_err(&pdev->dev, "%s, clk_get fail\n", __func__);
		return PTR_ERR(hpc->clk);
	}

	ret = clk_prepare_enable(hpc->clk);
	if (ret) {
		dev_err(&pdev->dev,
			"%s, clk_prepare_enable fail\n", __func__);
		return ret;
	}

    if (hpc->dis_power_up_output)
	    blpwm_bypass_mode_disable(hpc);

	ret = of_property_read_u64(pdev->dev.of_node, "clock-rate", &clk_rate);
	if (clk_rate)
		ret = clk_set_rate(hpc->clk, clk_rate);

	if (ret)
		dev_err(&pdev->dev, "%s, doesn't have clock-rate or clock_rate failed.[0x%x]\n", __func__, ret);

	hpc->clk_rate = clk_get_rate(hpc->clk);
	clk_disable(hpc->clk);
	hpc->clk_enabled = false;
	if (!hpc->clk_rate) {
		clk_unprepare(hpc->clk);
		dev_err(&pdev->dev, "%s, clk_rate is 0\n", __func__);
		return -EINVAL;
	}
	return 0;
}

static void blpwm_clk_unprepare(struct blpwm_chip *hpc)
{
	clk_unprepare(hpc->clk);
}

static int get_dts_resource(struct device *dev, struct blpwm_chip *hpc)
{
	struct device_node *np = dev->of_node;
	int ret;
 
	ret = of_property_read_u32(np, "clk_output_div", &hpc->clk_div);
	if (ret) {
		/* historical platfrom input 83 MB clock is divided by 83, then curr freq is 1MB
		 * if doesn't have "clk_div" property, inherit historical data 
		 */
		hpc->clk_div = 0x52;
		dev_err(dev, "blpwm clk_div property isn't exist, use default value!\n");
	}

    ret = of_property_read_u32(np, "min_input_freq_hz", &hpc->min_in_freq);
	if (ret)
		dev_err(dev, "blpwm min_input_freq_hz isn't exist!\n");

    ret = of_property_read_u32(np, "disable_power_up_output", &hpc->dis_power_up_output);
	if (ret)
		dev_info(dev, "blpwm turn on the power-on output.\n");

	ret = of_property_read_u32(np, "pinctrl-used", &hpc->pinctrl.pinctrl_used);
	if (ret) {
		dev_info(dev, "blpwm output-pin doesn't need to be switched.\n");
		hpc->pinctrl.pinctrl_used = 0;
	}
	
	return 0;
}

static int blpwm_probe(struct platform_device *pdev)
{
	const struct of_device_id *device_id = NULL;
	struct blpwm_chip *hpc = NULL;
	struct resource *r = NULL;
	int ret;

	if (!pdev)
		return -EINVAL;

	device_id = of_match_device(blpwm_dt_ids, &pdev->dev);
	if (!device_id) {
		dev_err(&pdev->dev, "%s, no matched device\n", __func__);
		return -EINVAL;
	}

	hpc = devm_kzalloc(&pdev->dev, sizeof(*hpc), GFP_KERNEL);
	if (!hpc)
		return -ENOMEM;

	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	hpc->dev = &pdev->dev;
	hpc->pinctrl.pinctrl_flag = 0;
	hpc->base = devm_ioremap_resource(&pdev->dev, r);
	if (IS_ERR(hpc->base)) {
		dev_err(&pdev->dev, "%s, ioremap_resource fail\n", __func__);
		return PTR_ERR(hpc->base);
	}

	ret = blpwm_reset_cfg(hpc, pdev);
	if (ret)
		dev_err(&pdev->dev, "%s, blpwm_reset_cfg fail\n", __func__);

	ret = get_dts_resource(&pdev->dev, hpc);
	if (ret)
		dev_err(&pdev->dev, "%s, get_dts_resource fail\n", __func__);

	hpc->data = device_id->data;

	ret = blpwm_clk_prepare(hpc, pdev);
	if (ret) {
		dev_err(&pdev->dev, "%s, blpwm_clk_prepare fail\n", __func__);
		return ret;
	}

	/* ip_prst is unrst default */
	ret = add_blpwm_chip(hpc, &pdev->dev);
	if (ret) {
		dev_err(&pdev->dev, "%s, blpwm [%s] register failed\n", __func__, pdev->name);
		goto fail_end;
	}
	platform_set_drvdata(pdev, hpc);
	dev_info(&pdev->dev, "%s, blpwm [%s] register success\n", __func__, pdev->name);
	return 0;

fail_end:
	blpwm_clk_unprepare(hpc);
	return ret;
}

static int blpwm_remove(struct platform_device *pdev)
{
	int ret;
	struct blpwm_chip *hpc = NULL;

	if (!pdev)
		return -EINVAL;

	hpc = platform_get_drvdata(pdev);
	if (!hpc) {
		dev_err(&pdev->dev, "%s, hpc is NULL\n", __func__);
		return -EINVAL;
	}

	blpwm_clk_disable(hpc);
	blpwm_clk_unprepare(hpc);

	ret = pwmchip_remove(&hpc->chip);
	if (ret) {
		dev_err(&pdev->dev, "%s, pwmchip_remove fail\n", __func__);
		return ret;
	}
	return 0;
}

static struct platform_driver blpwm_driver = {
	.driver = {
		.name = "blpwm",
		.of_match_table = blpwm_dt_ids,
	},
	.probe = blpwm_probe,
	.remove = blpwm_remove,
};
module_platform_driver(blpwm_driver);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("hisi kirin soc blpwm driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");