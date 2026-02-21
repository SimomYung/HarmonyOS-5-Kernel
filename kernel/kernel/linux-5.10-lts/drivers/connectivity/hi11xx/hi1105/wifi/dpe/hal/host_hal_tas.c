/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * 功能说明 : host hal tas相关功能
 * 作    者 : wifi
 * 创建日期 : 2012年9月18日
 */
#if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)
#ifdef _PRE_CONFIG_USE_DTS
#include <linux/of.h>
#include <linux/of_gpio.h>
#endif
#include "oal_ext_if.h"
#include "board.h"
#include "board_pm.h"
#include "host_hal_tas.h"

int32_t g_wifi_tas_enable  = WIFI_TAS_DISABLE;
int32_t g_wifi_tas_state = 0;
int32_t g_wifi_tas_gpio; /* mp13 product */

/*
 * Prototype    : hal_whal_wifi_tas_setifi_tas_gpio_init
 * Description  : 设置TAS状态
 */
int32_t hal_wifi_tas_set(int value)
{
    if (g_wifi_tas_enable == WIFI_TAS_DISABLE) {
        return 0;
    }

    oal_io_print("wifi tas gpio set %s\n", value ? "high" : "low");

    if (value) {
        return oal_gpio_direction_output(g_wifi_tas_gpio, GPIO_HIGHLEVEL);
    } else {
        return oal_gpio_direction_output(g_wifi_tas_gpio, GPIO_LOWLEVEL);
    }
}

/*
 * Prototype    : hal_get_wifi_tas_gpio_state
 * Description  : 获取当前TAS状态
 */
int32_t hal_get_wifi_tas_gpio_state(void)
{
    if (g_wifi_tas_enable == WIFI_TAS_DISABLE) {
        return 0;
    }

    return oal_gpio_get_value(g_wifi_tas_gpio);
}

/*
 * Prototype    : hal_wifi_tas_gpio_init
 * Description  : tas gpio初始化
 */
int32_t hal_wifi_tas_gpio_init(void)
{
#ifdef _PRE_CONFIG_USE_DTS
    int32_t ret;
    int32_t physical_gpio = 0;
    struct device_node *np = NULL;

    ret = get_board_dts_node(&np, DTS_NODE_MPXX_WIFI);
    if (ret != BOARD_SUCC) {
        oal_io_print("DTS read node %s fail!!!\n", DTS_NODE_MPXX_WIFI);
        return BOARD_SUCC;
    }

    ret = of_property_read_bool(np, DTS_PROP_WIFI_TAS_EN);
    if (ret) {
        oal_io_print("mpxx wifi tas enable\n");
        g_wifi_tas_enable = WIFI_TAS_ENABLE;
    } else {
        oal_io_print("mpxx wifi tas not enable\n");
        g_wifi_tas_enable = WIFI_TAS_DISABLE;
        return BOARD_SUCC;
    }

    /* wifi tas control gpio request */
    ret = get_board_gpio(DTS_NODE_MPXX_WIFI, DTS_PROP_GPIO_WIFI_TAS, &physical_gpio);
    if (ret != BOARD_SUCC) {
        oal_io_print("get dts prop %s failed\n", DTS_PROP_GPIO_WIFI_TAS);
        return BOARD_SUCC;
    }

    g_wifi_tas_gpio = physical_gpio;
    oal_io_print("mpxx wifi tas gpio is %d\n", g_wifi_tas_gpio);

    ret = of_property_read_u32(np, DTS_PROP_MPXX_WIFI_TAS_STATE, &g_wifi_tas_state);
    if (ret) {
        oal_io_print("read prop [%s] fail, ret=%d\n", DTS_PROP_MPXX_WIFI_TAS_STATE, ret);
    }
    if (g_wifi_tas_state) {
        ret = gpio_request_one(physical_gpio, GPIOF_OUT_INIT_HIGH, PROC_NAME_GPIO_WIFI_TAS);
    } else {
        ret = gpio_request_one(physical_gpio, GPIOF_OUT_INIT_LOW, PROC_NAME_GPIO_WIFI_TAS);
    }
    if (ret) {
        oal_io_print("%s gpio_request failed\n", PROC_NAME_GPIO_WIFI_TAS);
        return BOARD_FAIL;
    }
#endif

    return BOARD_SUCC;
}

void hal_wifi_tas_gpio_free(void)
{
    oal_gpio_free(g_wifi_tas_gpio);
}

/*
 * Prototype    : hal_tas_hibernate
 * Description  : tas模块S4休眠处理，释放tas gpio
 */
static void hal_tas_hibernate(void)
{
    if (g_wifi_tas_enable == WIFI_TAS_DISABLE) {
        return;
    }

    hal_wifi_tas_gpio_free();
}

/*
 * Prototype    : hal_tas_hibernate
 * Description  : tas模块S4恢复处理，重新申请tas gpio
 */
static void hal_tas_restore(void)
{
    int32_t ret;
    if (g_wifi_tas_enable == WIFI_TAS_DISABLE) {
        return;
    }

    if (g_wifi_tas_state) {
        ret = oal_gpio_request_one(g_wifi_tas_gpio, GPIOF_OUT_INIT_HIGH, PROC_NAME_GPIO_WIFI_TAS);
    } else {
        ret = oal_gpio_request_one(g_wifi_tas_gpio, GPIOF_OUT_INIT_LOW, PROC_NAME_GPIO_WIFI_TAS);
    }
    if (ret) {
        oal_io_print("%s gpio_request failed\n", PROC_NAME_GPIO_WIFI_TAS);
        return;
    }
}

static void hal_tas_suspend(void)
{
#ifdef _PRE_CONFIG_HISI_S3S4_POWER_STATE
    /* 继承收编ARMPC项目处理逻辑, s3和s4一样GPIO都会下电，需要保存恢复 */
    hal_tas_hibernate();
#endif
}

static void hal_tas_resume(void)
{
#ifdef _PRE_CONFIG_HISI_S3S4_POWER_STATE
    /* 继承收编ARMPC项目处理逻辑, s3和s4一样GPIO都会下电，需要保存恢复 */
    hal_tas_restore();
#endif
}

/*
 * Prototype    : hal_tas_pm_notify_func
 * Description  : tas模块低功耗处理回调
 */
static int hal_tas_pm_notify_func(unsigned long event, pm_notify_param* param, void *data)
{
    switch (event) {
        case PM_SUSPEND_PREPARE:
            hal_tas_suspend();
            break;
        case PM_HIBERNATION_PREPARE:
             hal_tas_hibernate();
            break;
        case PM_POST_SUSPEND:
            hal_tas_resume();
            break;
        case PM_POST_HIBERNATION:
            hal_tas_restore();
            break;
        default:
            break;
    }
    return BOARD_SUCC;
}

static pm_notify_node g_tas_node = {
    .name = "wifi_tas_node",
    .priority = WIFI_NOITFY_PRIO,
    .cb_func = hal_tas_pm_notify_func,
};

/*
 * Prototype    : hal_wifi_tas_init
 * Description  : tas模块初始化
 */
void hal_wifi_tas_init(void)
{
    hal_wifi_tas_gpio_init();

    /* 注册接收低功耗PM事件 */
    if (g_wifi_tas_enable == WIFI_TAS_ENABLE) {
        board_pm_register_notify_cb(&g_tas_node);
    }
}

/*
 * Prototype    : hal_wifi_tas_deinit
 * Description  : tas模块去初始化
 */
void hal_wifi_tas_deinit(void)
{
    if (g_wifi_tas_enable == WIFI_TAS_ENABLE) {
        board_pm_unregister_notify_cb(&g_tas_node);
    }
    hal_wifi_tas_gpio_free();
    g_wifi_tas_enable = WIFI_TAS_DISABLE;
}

#endif // #if (_PRE_OS_VERSION_LINUX == _PRE_OS_VERSION)