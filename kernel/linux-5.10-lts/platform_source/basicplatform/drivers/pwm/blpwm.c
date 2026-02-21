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

#define START_HIGH_OFFSET 0x10

struct blpwm_regs {
	u32 clk_div_addr;
	u32 duty_cyle_addr;
	u32 out_ctrl_addr;
};

struct blpwm_data_private {
	struct blpwm_regs regs;
	unsigned int reserved;
};

struct blpwm_chip {
	struct pwm_chip chip;
	void __iomem *base;
	struct clk *clk;
	unsigned long clk_rate;
	bool clk_enabled;
	const struct blpwm_data_private *data;
	spinlock_t reg_sequence_lock;
};

static const struct blpwm_data_private blpwm_data = {
	.regs = {
		.clk_div_addr = 0x104,
		.duty_cyle_addr = 0x108,
		.out_ctrl_addr = 0x100,
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
	if ((period == 0) || (period >= 0xFFFFFFFF) \
		|| (duty_cycle > period) || (duty_cycle >= 0xFFFF)) {
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

	/* input 83 MB clock is divided by 83, then curr freq is 1MB */
	write_reg(hpc->base + hpc->data->regs.clk_div_addr, 0x52);
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

static int blpwm_apply(struct pwm_chip *chip, struct pwm_device *pwm,
	const struct pwm_state *state)
{
	struct blpwm_chip *hpc = NULL;

	if (!ptr_null_check(chip, pwm, state))
		return -EINVAL;

	hpc = to_blpwm_chip(chip);
	if (blpwm_clk_enable(hpc))
		return -ENODEV;

	if (state->enabled) {
		blpwm_config(hpc, pwm,
			state->period, state->duty_cycle, state->polarity);
		blpwm_enable(hpc);
		pwm->state.enabled = true;
	} else {
		blpwm_disable(hpc);
		pwm->state.enabled = false;
	}

	return 0;
}

static int blpwm_apply_state(struct pwm_chip *chip, struct pwm_device *pwm,
	const struct pwm_state *state)
{
	return blpwm_apply(chip, pwm, state);
}


static const struct pwm_ops blpwm_ops = {
	.get_state = blpwm_get_state,
	.apply = blpwm_apply_state,
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

static int blpwm_clk_prepare(struct blpwm_chip *hpc,
	struct platform_device *pdev)
{
	int ret;

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
	hpc->base = devm_ioremap_resource(&pdev->dev, r);
	if (IS_ERR(hpc->base)) {
		dev_err(&pdev->dev, "%s, ioremap_resource fail\n", __func__);
		return PTR_ERR(hpc->base);
	}

	ret = blpwm_clk_prepare(hpc, pdev);
	if (ret) {
		dev_err(&pdev->dev, "%s, blpwm_clk_prepare fail\n", __func__);
		return ret;
	}

	/* ip_prst is unrst default */

	hpc->data = device_id->data;
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