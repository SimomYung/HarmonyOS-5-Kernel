/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 *
 * Description: MP12 gpio board-level implementation.
 * Author: @CompanyNameTag
 */

#include <linux/device.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include "oal_hcc_bus.h"
#include "plat_debug.h"
#include "plat_gpio.h"
#include "oal_ext_if.h"
#include "plat_pm.h"
#include "plat_exception_rst.h"
#include "chr_user.h"
#include "bfgx_core.h"
#include "plat_pm_wlan.h"
#include "gpio_mp12.h"


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
            .mode = MODE_DTS,
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
            .mode = MODE_DTS,
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
            .mode = MODE_DTS,
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
    }
};

STATIC struct conn_gpio_chip g_drv_gpio_chip = {
    .node = g_gpio_spec,
    .node_num = oal_array_size(g_gpio_spec)
};

int32_t mp12_gpio_init(struct platform_device *pdev)
{
    int32_t ret;

    ret = conn_gpio_init(&g_drv_gpio_chip);
    if (ret < 0) {
        ps_print_err("mp12 gpio init fail\n");
        return ret;
    }

    return OAL_SUCC;
}

void mp12_gpio_remove(struct platform_device *pdev)
{
    conn_gpio_remove();
}
