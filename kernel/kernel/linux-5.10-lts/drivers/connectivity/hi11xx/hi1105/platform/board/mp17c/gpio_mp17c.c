/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 *
 * Description: MP17c gpio board-level implementation.
 * Author: @CompanyNameTag
 */

#include <linux/device.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include "oal_hcc_bus.h"
#include "plat_debug.h"
#include "plat_gpio.h"
#include "plat_pm.h"
#include "plat_cust.h"
#include "plat_exception_rst.h"
#include "chr_user.h"
#include "board_mp17c.h"
#include "bfgx_core.h"
#include "gpio_mp17c.h"

uint8_t g_mp17c_gugong_stage = OAL_FALSE;
uint8_t g_mp17c_enter_gugong_fac_status = OAL_FALSE;
uint8_t g_tas_ctrl = 0;
// 特性开关
typedef enum  {
    SW_OFF,
    SW_ON
} switch_t;

// 特性类型
typedef enum  {
    TYPE_SPDT,
    TYPE_PA,
    TYPE_RSMC,
    TYPE_CLK,
    TYPE_QUERY,
    TYPE_FAC,
    TYPE_W_POWER,
    TYPE_TAS = 7, // gps天线切换，soc GPIO控制命令
} feature_t;

struct gpio_gugong {
    char *dts_type;
    char *gpio_name;
    int32_t number;
    enum of_gpio_flags init_mode;
    int32_t inited;
};

#define MP17C_GPS_TAS_GPIO_NUMM 2

/* gps天线调谐由SOC GPIO控制，分别为gpio350/351 transmit antenna switch：tas */
STATIC struct gpio_gugong g_gpios_tas[MP17C_GPS_TAS_GPIO_NUMM] = {
    {
        .dts_type = "gps_tas1",
        .gpio_name = "gpio_gps_tas1"
    },
    {
        .dts_type = "gps_tas2",
        .gpio_name = "gpio_gps_tas2"
    }
};

STATIC int32_t mp17c_get_tas_gpio_get_flags(int32_t index_gpio)
{
    /* 默认态 */
    int32_t flags = GPIOF_OUT_INIT_LOW;

    if (g_tas_ctrl > 0) { // BIT7-6控制位
        if (g_tas_ctrl & BIT6) { /* 继承dts */
            flags = g_gpios_tas[index_gpio].init_mode == 0 ? GPIOF_OUT_INIT_LOW : GPIOF_OUT_INIT_HIGH;
        } else if (g_tas_ctrl & BIT7) { /* 继承定制化 */
            flags = (g_tas_ctrl & BIT(index_gpio)) == 0 ? GPIOF_OUT_INIT_LOW : GPIOF_OUT_INIT_HIGH;
        } else {
            ps_print_err("tas gpio [%d], name [%s]\n",
                g_gpios_tas[index_gpio].number, g_gpios_tas[index_gpio].gpio_name);
        }
    }
    return flags;
}

STATIC void mp17c_tas_gpio_init(struct platform_device *pdev)
{
    struct device_node *of_node = NULL;
    int32_t num_gpios, index_gpio, index_dts, ret;
    const char *pin_name = NULL;

    if ((pdev == NULL) || (pdev->dev.of_node == NULL)) {
        return;
    }

    of_node = pdev->dev.of_node;
    num_gpios = of_gpio_named_count(of_node, "tas-gpios");
    if (num_gpios <= 0) {
        ps_print_info("transmit antenna switch not config gpios");
        return;
    }

    ps_print_info("transmit antenna switch gpio_num[%d]\n", num_gpios);

    for (index_gpio = 0, index_dts = 0; index_gpio < MP17C_GPS_TAS_GPIO_NUMM; index_gpio++) {
        if (index_dts >= num_gpios) {
            break;
        }

        ret = of_property_read_string_index(of_node, "tas,gpio-types", index_dts, &pin_name);
        if (ret < 0) {
            ps_print_err("get tas gpio types fail\n");
            return;
        }

        if (oal_strcmp(pin_name, g_gpios_tas[index_gpio].dts_type) != 0) {
            ps_print_err("tas dts [%s] not match [%s]", pin_name, g_gpios_tas[index_gpio].dts_type);
            continue;
        }

        g_gpios_tas[index_gpio].number = of_get_named_gpio_flags(of_node, "tas-gpios", index_dts,
                                                                 &g_gpios_tas[index_gpio].init_mode);
        index_dts++;

        ps_print_info("tas gpio name %s-%d, init_mode:%u", g_gpios_tas[index_gpio].gpio_name,
                      g_gpios_tas[index_gpio].number, g_gpios_tas[index_gpio].init_mode);

        ret = gpio_request_one(g_gpios_tas[index_gpio].number,
                               mp17c_get_tas_gpio_get_flags(index_gpio),
                               g_gpios_tas[index_gpio].gpio_name);
        if (ret < 0) {
            ps_print_err("tas gpio [%d], name [%s], request fail ret %d\n",
                         g_gpios_tas[index_gpio].number, g_gpios_tas[index_gpio].gpio_name, ret);
            return;
        }

        g_gpios_tas[index_gpio].inited = OAL_TRUE;
    }
}

/* gpio350   gpio351    status      场景
      0         1         0       默认状态
      1         0         1     横屏导航/故宫
      1         1         2        仙人掌
 */
STATIC void mp17c_tas_gpio_conf(uint8_t status)
{
    if ((g_gpios_tas[0].inited != OAL_TRUE) && (g_gpios_tas[1].inited != OAL_TRUE)) {
        ps_print_err("tas gpio not init");
        return;
    }

    switch (status) {
        case 0:
            gpio_direction_output(g_gpios_tas[0].number, GPIO_LEVEL_LOW);
            gpio_direction_output(g_gpios_tas[1].number, GPIO_LEVEL_HIGH);
            break;
        case 1:
            gpio_direction_output(g_gpios_tas[0].number, GPIO_LEVEL_HIGH);
            gpio_direction_output(g_gpios_tas[1].number, GPIO_LEVEL_LOW);
            break;
        case 2: // 2:status2 仙人掌场景
            gpio_direction_output(g_gpios_tas[0].number, GPIO_LEVEL_HIGH);
            gpio_direction_output(g_gpios_tas[1].number, GPIO_LEVEL_HIGH);
            break;
        default:
            ps_print_err("status %d not define!\n", status);
    }
}

uint8_t *mp17c_get_tas_gpio_ctrl(void)
{
    return &g_tas_ctrl;
}
EXPORT_SYMBOL_GPL(mp17c_get_tas_gpio_ctrl);

void mp17c_tas_gpio_reset(void)
{
    uint8_t i;
    if ((g_gpios_tas[0].inited != OAL_TRUE) && (g_gpios_tas[1].inited != OAL_TRUE)) {
        ps_print_err("tas gpio not init");
        return;
    }
    for (i = 0; i < MP17C_GPS_TAS_GPIO_NUMM; i++) {
        gpio_direction_output(g_gpios_tas[i].number,
                              (g_gpios_tas[i].init_mode == 0) ? GPIOF_OUT_INIT_LOW : GPIOF_OUT_INIT_HIGH);
    }
}

uint8_t mp17c_is_gugong_status(void)
{
    ps_print_info("gugong status is %d", g_mp17c_gugong_stage);
    return g_mp17c_gugong_stage;
}

STATIC void mp17c_tas_gps_gpio_deinit(void)
{
    int32_t index_gpio;

    for (index_gpio = 0; index_gpio < MP17C_GPS_TAS_GPIO_NUMM; index_gpio++) {
        if (g_gpios_tas[index_gpio].inited == OAL_TRUE) {
            gpio_free(g_gpios_tas[index_gpio].number);
            g_gpios_tas[index_gpio].inited = OAL_FALSE;
        }
    }
}

STATIC int32_t gugong_rsmc_config(struct conn_gpio_userdata *data)
{
    plat_cust_data_mp17c *plat_cust = NULL;

    plat_cust = (plat_cust_data_mp17c *)get_plat_cust_addr();
    if (plat_cust == NULL) {
        ps_print_err("gugong get plat cust fail \n");
        return -OAL_FAIL;
    }

    if (data->sub_type == SW_OFF) {
        g_mp17c_gugong_stage = OAL_FALSE;
    } else if (data->sub_type == SW_ON) {
        if ((board_subsys_is_shutdown(W_SYS) == OAL_TRUE) && (board_subsys_is_shutdown(B_SYS) == OAL_TRUE)) {
            g_mp17c_gugong_stage = OAL_TRUE;
        } else {
            chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                                 CHR_PLT_DRV_EVENT_GUGONG, CHR_PLAT_DRV_ERROR_GUGONG_OPEN_CMD_FAIL);
            ps_print_err("wifi status[%u], bt status[%u] , don't allow enter gugong!\n",
                         (board_subsys_is_shutdown(W_SYS) == OAL_TRUE) ? OAL_FALSE : OAL_TRUE,
                         (board_subsys_is_shutdown(B_SYS) == OAL_TRUE) ? OAL_FALSE : OAL_TRUE);
            return -OAL_FAIL;
        }
    }
    plat_cust->cfg.enter_rsmc = (g_mp17c_gugong_stage == OAL_FALSE) ? OAL_FALSE : OAL_TRUE;
    return OAL_SUCC;
}

STATIC void get_gugong_status_to_user(struct conn_gpio_userdata *data, uint8_t OAL_USER *userdata)
{
    ps_print_info("get gugong status %d\n", g_mp17c_gugong_stage);
    data->sub_type = (g_mp17c_gugong_stage == OAL_TRUE) ? OAL_TRUE : OAL_FALSE;
    if (copy_to_user(userdata, data, sizeof(struct conn_gpio_userdata))) {
        ps_print_err("copy_to_user is err!\n");
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                             CHR_PLT_DRV_EVENT_GUGONG, CHR_PLAT_DRV_ERROR_GUGONG_QUERY_STATUS_FAIL);
    }

    ps_core_show_bfgx_status();

    if (wlan_pm_is_poweron() == OAL_TRUE) {
        ps_print_info("wifi is open!");
    } else {
        ps_print_info("wifi is close!");
    }
}

STATIC void gugong_factory_config(uint32_t sub_type)
{
    plat_cust_data_mp17c *plat_cust = NULL;

    plat_cust = (plat_cust_data_mp17c *)get_plat_cust_addr();
    if (plat_cust == NULL) {
        ps_print_err("gugong get plat cust fail \n");
        return;
    }
    if (sub_type == SW_ON) {
        g_mp17c_enter_gugong_fac_status = OAL_TRUE;
        plat_cust->cfg.gugong_fac_status = OAL_TRUE;
        ps_print_info("it is factory status\n");
    } else {
        g_mp17c_enter_gugong_fac_status = OAL_FALSE;
        plat_cust->cfg.gugong_fac_status = OAL_FALSE;
        ps_print_info("it is not factory status\n");
    }
}

STATIC void gugong_status_reset(void)
{
    plat_cust_data_mp17c *plat_cust = NULL;

    plat_cust = (plat_cust_data_mp17c *)get_plat_cust_addr();
    if (plat_cust != NULL) {
        plat_cust->cfg.enter_rsmc = OAL_FALSE;
        plat_cust->cfg.gugong_fac_status = OAL_FALSE;
    }

    g_mp17c_gugong_stage = OAL_FALSE;
    g_mp17c_enter_gugong_fac_status = OAL_FALSE;
}

STATIC int32_t user_gpio_set(struct conn_gpio_userdata *data, uint8_t OAL_USER *userdata)
{
    struct pm_top* pm_top_data = pm_get_top();
    ps_print_info("gugong set gugong type main[%u] sub[%u]\n",
                  data->main_type, data->sub_type);

    if (data->main_type == TYPE_RSMC) {
        mutex_lock(&(pm_top_data->host_mutex));
        if (gugong_rsmc_config(data) != OAL_SUCC) {
            mutex_unlock(&(pm_top_data->host_mutex));
            return -OAL_FAIL;
        }
        mutex_unlock(&(pm_top_data->host_mutex));
    } else if (data->main_type == TYPE_QUERY) {
        get_gugong_status_to_user(data, userdata);
    } else if (data->main_type == TYPE_FAC) {
        gugong_factory_config(data->sub_type);
    } else if (data->main_type == TYPE_TAS) {
        mp17c_tas_gpio_conf(data->sub_type);
    }

    return OAL_SUCC;
}

STATIC int32_t mp17c_gugong_check_subsys_power_on(uint32_t subsys)
{
    if (g_mp17c_gugong_stage == OAL_TRUE) {
        // 产测模式, 在GNSS打开的情况下，重新开wifi
        if (g_mp17c_enter_gugong_fac_status == OAL_TRUE && (board_subsys_is_shutdown(G_SYS) == OAL_FALSE) &&
            subsys == W_SYS) {
            ps_print_info("enter facotry mode\n");
            return OAL_SUCC;
        }

        ps_print_err("already into gugong, not support subsys[%u] power up\n", subsys);
        if (subsys == W_SYS) {
            chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                                 CHR_PLT_DRV_EVENT_GUGONG, CHR_PLAT_DRV_ERROR_GUGONG_WIFI_POWER_ON);
        } else if (subsys == B_SYS) {
            chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                                 CHR_PLT_DRV_EVENT_GUGONG, CHR_PLAT_DRV_ERROR_GUGONG_BT_POWER_ON);
        }
        declare_dft_trace_key_info("gugong power exception", OAL_DFT_TRACE_EXCEP);
        plat_exception_handler(SUBSYS_BFGX, THREAD_GNSS, GUGONG_POWER_FAIL);
        return -OAL_FAIL;
    }

    if (g_mp17c_enter_gugong_fac_status == OAL_TRUE) {
        ps_print_err("subsys[%u] open, but the status is gugong fac state!\n", subsys);
        return -OAL_FAIL;
    }

    return OAL_SUCC;
}

int32_t mp17c_gugong_subsys_power_on(uint32_t subsys)
{
    int32_t ret;

    ret = mp17c_gugong_check_subsys_power_on(subsys);
    if (ret < 0) {
        gugong_status_reset();
    }

    return ret;
}

STATIC int32_t mp17c_gugong_check_subsys_power_off(uint32_t subsys)
{
    ps_print_info("gugong stage %d, fac status %d\n", g_mp17c_gugong_stage, g_mp17c_enter_gugong_fac_status);

    if (g_mp17c_gugong_stage == OAL_TRUE || g_mp17c_enter_gugong_fac_status == OAL_TRUE) {
        ps_print_err("subsys[%u] should closed before gugong and gugong fac mode\n", subsys);
        declare_dft_trace_key_info("gugong power exception", OAL_DFT_TRACE_EXCEP);
        plat_exception_handler(SUBSYS_BFGX, THREAD_GNSS, GUGONG_POWER_FAIL);
        return -OAL_FAIL;
    }

    return OAL_SUCC;
}

void mp17c_gugong_subsys_power_off(uint32_t subsys)
{
    int32_t ret;

    ret = mp17c_gugong_check_subsys_power_off(subsys);
    if (ret < 0) {
        gugong_status_reset();
    }
}

STATIC struct conn_gpio_node g_gpio_spec[] = {
    {
        .pin = {
            .name = PROC_NAME_GPIO_POWEN_ON,
            .is_key_gpio = OAL_TRUE,
            .type = CHIP_POWR,
            .init = {
                .req_sts = REQ_ONE,
                .flags = GPIOF_OUT_INIT_LOW
            },
        },
        .attr = {
            .mode = MODE_DTS | MODE_PMU_IO,
            .u = {
                .dts = {
                    .compatibale = DTS_NODE_HISI_MPXX,
                    .label_name = DTS_PROP_GPIO_MPXX_POWEN_ON
                }
            },
        }
    },
#ifndef BFGX_UART_DOWNLOAD_SUPPORT
    {
        .pin = {
            .name = PROC_NAME_GPIO_WLAN_POWEN_ON,
            .is_key_gpio = OAL_TRUE,
            .type = W_POWER,
            .init = {
                .req_sts = REQ_ONE,
                .flags = GPIOF_OUT_INIT_LOW
            },
        },
        .attr = {
            .mode = MODE_DTS,
            .u = {
                .dts = {
                    .compatibale = DTS_NODE_HISI_MPXX,
                    .label_name = DTS_PROP_GPIO_WLAN_POWEN_ON_ENABLE
                }
            },
        }
    },
#endif
    {
        .pin = {
            .name = PROC_NAME_GPIO_BFGX_POWEN_ON,
            .is_key_gpio = OAL_TRUE,
            .type = BFGX_POWER,
            .init = {
                .req_sts = REQ_ONE,
                .flags = GPIOF_OUT_INIT_LOW
            },
        },
        .attr = {
            .mode = MODE_DTS | MODE_PMU_IO,
            .u = {
                .dts = {
                    .compatibale = DTS_NODE_HISI_MPXX,
                    .label_name = DTS_PROP_GPIO_BFGX_POWEN_ON_ENABLE
                }
            },
        }
    },
    {
        .pin = {
            .name = PROC_NAME_GPIO_G_POWEN_ON,
            .is_key_gpio = OAL_TRUE,
            .type = G_POWER,
            .init = {
                .req_sts = REQ_ONE,
                .flags = GPIOF_OUT_INIT_LOW
            },
        },
        .attr = {
            .mode = MODE_DTS | MODE_PMU_IO,
            .u = {
                .dts = {
                    .compatibale = DTS_NODE_HISI_MPXX,
                    .label_name = DTS_PROP_GPIO_G_POWEN_ON_ENABLE
                }
            },
        }
    },
    {
        .pin = {
            .name = PROC_NAME_GPIO_SSI_CLK,
            .is_key_gpio = OAL_FALSE,
            .type = SSI_CLK,
            .init = {
                .req_sts = REQ_ONE,
                .flags = GPIOF_OUT_INIT_LOW
            },
        },
        .attr = {
            .mode = MODE_DTS,
            .u = {
                .dts = {
                    .compatibale = DTS_NODE_HISI_MPXX,
                    .label_name = DTS_PROP_GPIO_MPXX_GPIO_SSI_CLK
                }
            },
        }
    },
    {
        .pin = {
            .name = PROC_NAME_GPIO_SSI_DATA,
            .is_key_gpio = OAL_FALSE,
            .type = SSI_DATA,
            .init = {
                .req_sts = REQ_ONE,
                .flags = GPIOF_OUT_INIT_LOW
            },
        },
        .attr = {
            .mode = MODE_DTS,
            .u = {
                .dts = {
                    .compatibale = DTS_NODE_HISI_MPXX,
                    .label_name = DTS_PROP_GPIO_MPXX_GPIO_SSI_DATA
                }
            },
        }
    },
    {
        .pin = {
            .name = PROC_NAME_GPIO_HOST_WAKEUP_WLAN,
            .is_key_gpio = OAL_TRUE,
            .type = HOST_WKUP_W,
            .init = {
                .req_sts = REQ_ONE,
                .flags = GPIOF_OUT_INIT_LOW
            },
        },
        .attr = {
            .mode = MODE_DTS,
            .u = {
                .dts = {
                    .compatibale = DTS_NODE_MPXX_WIFI,
                    .label_name = DTS_PROP_GPIO_HOST_WAKEUP_WLAN
                }
            },
        }
    },
    {
        .pin = {
            .name = PROC_NAME_GPIO_GCPU_WAKEUP_HOST,
            .is_key_gpio = OAL_TRUE,
            .type = G_WKUP_HOST,
            .init = {
                .req_sts = REQ_ONE,
                .flags = GPIOF_IN
            },
        },
        .attr = {
            .mode = MODE_DTS,
            .u = {
                .dts = {
                    .compatibale = DTS_NODE_MPXX_ME,
                    .label_name = DTS_PROP_MPXX_GPIO_GCPU_WAKEUP_HOST
                }
            },
        },
        .irq = {
            .name = "gcpu_wake_host",
            .flags = IRQF_DISABLED | IRQF_TRIGGER_RISING | IRQF_NO_SUSPEND,
            .init_set_enable = OAL_FALSE
        },
    },
    {
        .pin = {
            .name = PROC_NAME_GPIO_BFGX_WAKEUP_HOST,
            .is_key_gpio = OAL_TRUE,
            .type = BFGX_WKUP_HOST,
            .init = {
                .req_sts = REQ_ONE,
                .flags = GPIOF_IN
            },
        },
        .attr = {
            .mode = MODE_DTS,
            .u = {
                .dts = {
                    .compatibale = DTS_NODE_MPXX_BFGX,
                    .label_name = DTS_PROP_MPXX_GPIO_BFGX_WAKEUP_HOST
                }
            },
        },
        .irq = {
            .name = "bfgx_wake_host",
            .flags = IRQF_DISABLED | IRQF_TRIGGER_RISING | IRQF_NO_SUSPEND,
            .init_set_enable = OAL_FALSE
        },
    },
    {
        .pin = {
            .name = PROC_NAME_GPIO_WLAN_WAKEUP_HOST,
            .is_key_gpio = OAL_TRUE,
            .type = W_WKUP_HOST,
            .init = {
                .req_sts = REQ_ONE,
                .flags = GPIOF_IN
            },
        },
        .attr = {
            .mode = MODE_DTS,
            .u = {
                .dts = {
                    .compatibale = DTS_NODE_MPXX_WIFI,
                    .label_name = DTS_PROP_MPXX_GPIO_WLAN_WAKEUP_HOST
                }
            },
        },
        .irq = {
            .name = "wlan_wake_host",
            .flags = IRQF_DISABLED | IRQF_TRIGGER_RISING | IRQF_NO_SUSPEND,
            .init_set_enable = OAL_FALSE
        },
    },
    {
        .pin = {
            .name = PROC_NAME_GPIO_WLAN_FLOWCTRL,
            .is_key_gpio = OAL_FALSE,
            .type = SDIO_FLOW_CTRL,
            .init = {
                .req_sts = REQ_ONE,
                .flags = GPIOF_IN
            },
        },
        .attr = {
            .mode = MODE_DTS,
            .u = {
                .dts = {
                    .compatibale = DTS_NODE_MPXX_WIFI,
                    .label_name = DTS_PROP_GPIO_WLAN_FLOWCTRL,
                }
            },
        },
        .irq = {
            .name = "wifi_gpio_flowctrl_intr",
            .flags = IRQF_NO_SUSPEND | IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING | IRQF_DISABLED,
            .init_set_enable = OAL_FALSE,
        },
    },
    {
        .pin = {
            .name = PROC_NAME_GPIO_BFGX_IR_CTRL,
            .is_key_gpio = OAL_FALSE,
            .type = IR_CTRL,
            .init = {
                .req_sts = REQ_ONE,
                .flags = GPIOF_OUT_INIT_LOW
            },
        },
        .attr = {
            .mode = MODE_DTS,
            .u = {
                .dts = {
                    .compatibale = DTS_NODE_HISI_MPXX,
                    .label_name = DTS_PROP_GPIO_BFGX_IR_CTRL,
                }
            },
        }
    }
};

STATIC struct usr_gpio_ops g_usr_ops = {
    .user_set = user_gpio_set
};

STATIC struct conn_gpio_chip g_drv_gpio_chip = {
    .node = g_gpio_spec,
    .node_num = oal_array_size(g_gpio_spec),
    .usr_ops = &g_usr_ops
};

int32_t mp17c_gpio_init(struct platform_device *pdev)
{
    int32_t ret;

    ret = conn_gpio_init(&g_drv_gpio_chip);
    if (ret < 0) {
        ps_print_err("mp17c gpio init fail\n");
        return ret;
    }
    mp17c_tas_gpio_init(pdev);
    return OAL_SUCC;
}

void mp17c_gpio_remove(struct platform_device *pdev)
{
    mp17c_tas_gps_gpio_deinit();
    conn_gpio_remove();
}
