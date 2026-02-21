/*
 * fem_off_find.h
 *
 * fem_off_find header
 *
 * Copyright (c) 2025-2025 Huawei Technologies Co., Ltd.
 *
 */

#include "fem_off_find.h"
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/err.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include <huawei_platform/log/hw_log.h>
#define HWLOG_TAG fem_off_find_cfg
HWLOG_REGIST();

static const struct dc_characteristics dc_match[] = {
	{
        .id = DTS_PROP_PMU_FEM_SWITCH1_REGULATOR,
        .const_vout_min = FEM_SWITCH1_VOLTAGE_MIN,
	    .const_vout_typ = FEM_SWITCH1_VOLTAGE_TYP,
        .const_vout_max = FEM_SWITCH1_VOLTAGE_MAX,
    },
    {
        .id = DTS_PROP_PMU_FEM_SWITCH2_REGULATOR,
        .const_vout_min = FEM_SWITCH2_VOLTAGE_MIN,
	    .const_vout_typ = FEM_SWITCH2_VOLTAGE_TYP,
        .const_vout_max = FEM_SWITCH2_VOLTAGE_MAX,
    },
};

static int32_t check_voltage(const char *id, uint32_t inVol, uint32_t *outVol)
{
    int i;
    int length = sizeof(dc_match) / sizeof(dc_match[0]);
    *outVol = inVol;
    for (i = 0; i < length; i++) {
        if (strcmp(dc_match[i].id, id) == 0) {
            if (inVol < dc_match[i].const_vout_min || inVol > dc_match[i].const_vout_max) {
                hwlog_err("volage out of limit %d\n", inVol);
                *outVol = dc_match[i].const_vout_typ;
                return 0;
            }
            return 0;
        }
    }
    return -1;
}

static void fem_switch_set(struct device *dev, const char *id, const char *value, struct device_node *np)
{
    int32_t ret;
    uint32_t voltage;
    uint32_t outVol;
    struct regulator *fem_switch;

    fem_switch = regulator_get(dev, id);
    if (!fem_switch || IS_ERR(fem_switch)) {
        hwlog_err("fail to get regulator %s\n", id);
        return ;
    }

    ret = of_property_read_u32(np, value, &voltage);
    if (ret == 0 && check_voltage(id, voltage, &outVol) == 0) {
        regulator_set_voltage(fem_switch, outVol, outVol);
        hwlog_info("%s voltage %d\n", id, regulator_get_voltage(fem_switch));
    } else {
        hwlog_err("fail to get %s\n", value);
    }
}

static int fem_off_find_pdrv_probe(struct platform_device *pdev)
{
    struct device_node *np = NULL;
    hwlog_info("fem off find cfg exist\n");

    if (!pdev || !pdev->dev.of_node)
        return - ENODEV;

    np = pdev->dev.of_node;
    fem_switch_set(&pdev->dev, DTS_PROP_PMU_FEM_SWITCH1_REGULATOR, DTS_PROP_PMU_FEM_SWITCH1_VOLTAGE, np);
    fem_switch_set(&pdev->dev, DTS_PROP_PMU_FEM_SWITCH2_REGULATOR, DTS_PROP_PMU_FEM_SWITCH2_VOLTAGE, np);
    return 0;
}

static const struct of_device_id fem_off_find_match_dts[] = {
    {
        .compatible = "huawei,off_find_cfg",
        .data = NULL,
    },
    {},
};

static struct platform_driver fem_off_find_pdrv = {
    .probe = fem_off_find_pdrv_probe,
    .driver = {
        .name = "fem off find",
        .of_match_table = fem_off_find_match_dts,
    },
};

static int __init fem_off_find_driver_init(void)
{
    hwlog_info("fem off find init enter!");
    return platform_driver_register(&fem_off_find_pdrv);
}

static void __exit fem_off_find_exit(void)
{
    hwlog_info("fem off find init exit!");
    platform_driver_unregister(&fem_off_find_pdrv);
}

fs_initcall_sync(fem_off_find_driver_init);
module_exit(fem_off_find_exit);
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("fem off find module driver");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");