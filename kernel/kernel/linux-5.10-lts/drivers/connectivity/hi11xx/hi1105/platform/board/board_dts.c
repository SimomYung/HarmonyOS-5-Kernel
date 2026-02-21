/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description: Declaration Driver Entry Function
 * Author: @CompanyNameTag
 */

/* 头文件包含 */
#define MPXX_LOG_MODULE_NAME     "[BOARD_DTS]"
#define MPXX_LOG_MODULE_NAME_VAR board_dts_loglevel
#include <linux/clk.h>
#include <linux/platform_device.h>

#include "chr_user.h"
#include "plat_common_clk.h"
#include "plat_debug.h"
#include "plat_pm.h"
#include "plat_uart.h"
#include "plat_gpio.h"
#include "board_dts.h"

#ifdef BFGX_UART_DOWNLOAD_SUPPORT
#include "bfgx_data_parse.h"
#include "plat_firmware_uart.h"
#endif
#ifdef CONFIG_IODIE_HKADC
#include <platform_include/cee/linux/adc.h>
#endif

STATIC int32_t mpxx_get_board_pmu_clk32k(struct platform_device *pdev)
{
#ifdef _PRE_CONFIG_USE_DTS
    int32_t ret;
    const char *clk_name = NULL;
    struct clk *clk = NULL;
    struct device *dev = NULL;

    ret = get_board_custmize(DTS_NODE_HISI_MPXX, DTS_PROP_CLK_32K, &clk_name);
    if (ret != BOARD_SUCC) {
        ps_print_info("NO dts prop %s\n", DTS_PROP_CLK_32K);
        return BOARD_SUCC;
    }
    g_st_board_info.clk_32k_name = clk_name;

    ps_print_info("mpxx 32k clk name is %s\n", g_st_board_info.clk_32k_name);

    dev = &pdev->dev;
    clk = devm_clk_get(dev, clk_name);
    if (clk == NULL) {
        ps_print_err("Get 32k clk %s failed!!!\n", clk_name);
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                             CHR_PLT_DRV_EVENT_DTS, CHR_PLAT_DRV_ERROR_32K_CLK_DTS);
        return BOARD_FAIL;
    }
    g_st_board_info.clk_32k = clk;
    return BOARD_SUCC;
#else
    return BOARD_SUCC;
#endif
}

/*
 * 函 数 名  : mpxx_get_board_pmu_fem_switch1_regulator
 * 功能描述  : 根据DTS里的配置，获取并初始化pmu给fem开关器件的ldo
 */
STATIC int32_t mpxx_get_board_pmu_fem_switch1_regulator(struct platform_device *pdev)
{
#ifdef _PRE_CONFIG_USE_DTS
    struct regulator *fem_switch;
    struct device_node *np = NULL;
    uint32_t voltage;
    int32_t ret;

    ret = get_board_dts_node(&np, DTS_NODE_HISI_MPXX);
    if (ret != BOARD_SUCC) {
        ps_print_err("DTS read node %s fail!!!\n", DTS_NODE_HISI_MPXX);
        return BOARD_FAIL;
    }
    /* 获取电源supply */
    g_st_board_info.fem_switch1_regulator = NULL;
    fem_switch = regulator_get(&pdev->dev, DTS_PROP_PMU_FEM_SWITCH1_REGULATOR);
    if (oal_is_err_or_null(fem_switch)) {
        ps_print_info("fail to get %s\n", DTS_PROP_PMU_FEM_SWITCH1_REGULATOR);
        return BOARD_SUCC;
    }
    g_st_board_info.fem_switch1_regulator = fem_switch;
    ps_print_info("succ get %s,cur status %d\n", DTS_PROP_PMU_FEM_SWITCH1_REGULATOR, regulator_is_enabled(fem_switch));
    /* 设置工作电压 */
    ret = of_property_read_u32(np, DTS_PROP_PMU_FEM_SWITCH1_VOLTAGE, &voltage);
    if (ret == BOARD_SUCC) {
        regulator_set_voltage(g_st_board_info.fem_switch1_regulator, voltage, voltage);
        ps_print_info("fem switch1 voltage %d\n", regulator_get_voltage(g_st_board_info.fem_switch1_regulator));
    } else {
        ps_print_info("fail to get %s\n", DTS_PROP_PMU_FEM_SWITCH1_VOLTAGE);
    }
    return BOARD_SUCC;
#else
    g_st_board_info.fem_switch1_regulator = NULL;
    ps_print_info("not support dts\n");
    return BOARD_SUCC;
#endif
}

STATIC int32_t mpxx_get_board_uart_port(void)
{
#ifdef _PRE_CONFIG_USE_DTS
    int32_t ret;
    struct device_node *np = NULL;
    const char *uart_port = NULL;

    ret = get_board_dts_node(&np, DTS_NODE_MPXX_BFGX);
    if (ret != BOARD_SUCC) {
        ps_print_err("DTS read node %s fail!!!\n", DTS_NODE_MPXX_BFGX);
        return BOARD_SUCC;
    }

    /* 使用uart4，需要在dts里新增DTS_PROP_MPXX_UART_PCLK项，指明uart4不依赖sensorhub */
    ret = of_property_read_bool(np, DTS_PROP_MPXX_UART_PCLK);
    if (ret) {
        ps_print_info("uart pclk normal\n");
        g_st_board_info.uart_pclk = UART_PCLK_NORMAL;
    } else {
        ps_print_info("uart pclk from sensorhub\n");
        g_st_board_info.uart_pclk = UART_PCLK_FROM_SENSORHUB;
    }

    ret = get_board_custmize(DTS_NODE_MPXX_BFGX, DTS_PROP_MPXX_UART_POART, &uart_port);
    if (ret != BOARD_SUCC) {
        ps_print_info("no %s\n", DTS_PROP_MPXX_UART_POART);
    }
    g_st_board_info.uart_port[BUART] = uart_port;

    if (get_mpxx_subchip_type() == BOARD_VERSION_MP16 ||
        get_mpxx_subchip_type() == BOARD_VERSION_MP17C ||
        get_mpxx_subchip_type() == BOARD_VERSION_MP12) {
        ret = get_board_custmize(DTS_NODE_MPXX_ME, DTS_PROP_MPXX_GUART_POART, &uart_port);
        if (ret != BOARD_SUCC) {
            ps_print_info("no %s\n", DTS_PROP_MPXX_GUART_POART);
        }
        g_st_board_info.uart_port[GUART] = uart_port;
    } else if ((get_mpxx_subchip_type() == BOARD_VERSION_MP16C) ||
                get_mpxx_subchip_type() == BOARD_VERSION_GF61) {
        ret = get_board_custmize(DTS_NODE_MPXX_SLE, DTS_PROP_MPXX_SLEUART_POART, &uart_port);
        if (ret != BOARD_SUCC) {
            ps_print_info("no %s\n", DTS_PROP_MPXX_SLEUART_POART);
        }
        g_st_board_info.uart_port[GUART] = uart_port;
    }

    return BOARD_SUCC;
#else
    return BOARD_SUCC;
#endif
}

#define EMU_INI_BUF_SIZE 50
static void mpxx_emu_init(void)
{
    char buff[EMU_INI_BUF_SIZE];
    g_st_board_info.is_emu = OAL_FALSE;

    memset_s(buff, sizeof(buff), 0, sizeof(buff));
    if (get_cust_conf_string(INI_MODU_PLAT, "emu_enable", buff, sizeof(buff) - 1) == INI_SUCC) {
        if (!oal_strncmp("yes", buff, OAL_STRLEN("yes"))) {
            g_st_board_info.is_emu = OAL_TRUE;
            ps_print_info("emu enable\n");
        }
    }
}

STATIC int32_t mpxx_check_evb_or_fpga(void)
{
#ifdef _PRE_CONFIG_USE_DTS
    int32_t ret;
    struct device_node *np = NULL;
#endif

    g_st_board_info.is_asic = VERSION_ASIC;
#ifdef _PRE_CONFIG_USE_DTS
    ret = get_board_dts_node(&np, DTS_NODE_HISI_MPXX);
    if (ret != BOARD_SUCC) {
        ps_print_err("DTS read node %s fail!!!\n", DTS_NODE_HISI_MPXX);
        return BOARD_FAIL;
    }

    ret = of_property_read_bool(np, DTS_PROP_MPXX_VERSION);
    if (ret) {
        ps_print_info("MPXX ASIC VERSION\n");
        g_st_board_info.is_asic = VERSION_ASIC;
    } else {
        ps_print_info("MPXX FPGA VERSION\n");
        g_st_board_info.is_asic = VERSION_FPGA;
    }
#endif

    return BOARD_SUCC;
}
#ifdef _PRE_CONFIG_USE_DTS
static void mpxx_check_gt_subsystem_support(void)
{
    int32_t ret;
    struct device_node *np = NULL;
    char *dts_status_string = NULL;
    g_st_board_info.is_gt_disable = 1;
    ret = get_board_dts_node(&np, DTS_NODE_HISI_GT);
    if (ret != BOARD_SUCC) {
        ps_print_info("DTS read node %s fail, not support gt\n", DTS_NODE_HISI_GT);
        return;
    }
    ret = of_property_read_string(np, "status", (const char **)&dts_status_string);
    if (ret < 0 || dts_status_string == NULL) {
        ps_print_info("get gt dts node status fail\n");
        return;
    }
    if (!oal_strcmp("ok", dts_status_string)) {
        g_st_board_info.is_gt_disable = 0;
    }
}
#endif

STATIC int32_t mpxx_check_mpxx_subsystem_support(void)
{
#ifdef _PRE_CONFIG_USE_DTS
    int32_t ret;
    struct device_node *np = NULL;

    ret = get_board_dts_node(&np, DTS_NODE_HISI_MPXX);
    if (ret != BOARD_SUCC) {
        ps_print_err("DTS read node %s fail!!!\n", DTS_NODE_HISI_MPXX);
        return BOARD_FAIL;
    }

    ret = of_property_read_bool(np, DTS_PROP_MPXX_WIFI_DISABLE);
    if (ret) {
        g_st_board_info.is_wifi_disable = 1;
    } else {
        g_st_board_info.is_wifi_disable = 0;
    }

    ret = of_property_read_bool(np, DTS_PROP_MPXX_BFGX_DISABLE);
    if (ret) {
        g_st_board_info.is_bfgx_disable = 1;
    } else {
        g_st_board_info.is_bfgx_disable = 0;
    }
    mpxx_check_gt_subsystem_support();
#endif
    mpxx_emu_init();

    if (hi110x_is_emu() == OAL_TRUE) {
        // non-bfgx in emu
        g_st_board_info.is_bfgx_disable = 1;
    }

    ps_print_info("wifi %s, bfgx %s, gt %s\n",
                  (g_st_board_info.is_wifi_disable == 0) ? "enabled" : "disabled",
                  (g_st_board_info.is_bfgx_disable == 0) ? "enabled" : "disabled",
                  (g_st_board_info.is_gt_disable == 0) ? "enabled" : "disabled");

    return BOARD_SUCC;
}

STATIC int32_t mpxx_check_pmu_clk_share(void)
{
#ifdef _PRE_CONFIG_USE_DTS
    int32_t ret;
    struct device_node *np = NULL;
    int32_t pmu_clk_request = PMU_CLK_REQ_DISABLE;

    ret = get_board_dts_node(&np, DTS_NODE_HISI_MPXX);
    if (ret != BOARD_SUCC) {
        ps_print_err("DTS read node %s fail!!!\n", DTS_NODE_HISI_MPXX);
        return BOARD_FAIL;
    }

    ret = of_property_read_u32(np, DTS_PROP_MPXX_PMU_CLK, &pmu_clk_request);
    if (ret != BOARD_SUCC) {
        ps_print_info("get dts prop %s failed, mpxx pmu clk request disable\n", DTS_PROP_MPXX_PMU_CLK);
        g_st_board_info.pmu_clk_share_enable = PMU_CLK_REQ_DISABLE;
    } else {
        ps_print_info("mpxx PMU clk request is %s\n", (pmu_clk_request ? "enable" : "disable"));
        g_st_board_info.pmu_clk_share_enable = pmu_clk_request;
    }

    return BOARD_SUCC;
#else
    return BOARD_SUCC;
#endif
}

#ifdef _PRE_CONFIG_USE_DTS
static int32_t mpxx_get_ini_prop_from_node(struct device_node *np,
                                           const char *dts_prop, char *prop_value, uint32_t size)
{
    int32_t ret;
    int32_t len;
    char out_str[HISI_CUST_NVRAM_LEN] = { 0 };

    len = of_property_count_u8_elems(np, dts_prop);
    if (len < 0) {
        ini_error("can't get len of dts prop(%s)", dts_prop);
        return INI_FAILED;
    }

    len = ini_min(len, (int32_t)sizeof(out_str));
    ini_debug("read len of dts prop %s is:%d", dts_prop, len);
    ret = of_property_read_u8_array(np, dts_prop, out_str, len);
    if (ret < 0) {
        ini_error("read dts prop (%s) fail", dts_prop);
        return INI_FAILED;
    }

    len = ini_min(len, (int32_t)size);
    if (memcpy_s(prop_value, (size_t)size, out_str, (size_t)len) != EOK) {
        ini_error("memcpy_s error, destlen=%d, srclen=%d\n ", size, len);
        return INI_FAILED;
    }
    ini_debug("dts prop %s value is:%s", dts_prop, prop_value);
    return INI_SUCC;
}
#endif

#if defined(CONFIG_IODIE_HKADC)
bool mpxx_ncid_supported(void)
{
#ifdef _PRE_CONFIG_USE_DTS
    int32_t ret;
    uint32_t is_supported = 0;
    struct device_node *np = of_find_compatible_node(NULL, NULL, COST_MPXX_COMP_NODE);

    if (np == NULL) {
        ini_error("dts node %s not found", COST_MPXX_COMP_NODE);
        return OAL_FALSE;
    }

    ret = of_property_read_u32(np, DTS_PROP_MPXX_NCID_SUPP, &is_supported);
    return (ret == 0) && (is_supported > 0);
#else
    return OAL_FALSE;
#endif
}

int32_t mpxx_get_support_ncid_from_dts(int32_t *supp_ncids, int32_t max_supp_num, int32_t *supp_num)
{
#ifdef _PRE_CONFIG_USE_DTS
    int32_t ret;
    struct device_node *np = NULL;

    if (supp_num == NULL || supp_ncids == NULL || max_supp_num < 1) {
        ini_error("invalid params");
        return INI_FAILED;
    }

    *supp_num = 0;
    np = of_find_compatible_node(NULL, NULL, COST_MPXX_COMP_NODE);
    if (np == NULL) {
        ini_error("dts node %s not found", COST_MPXX_COMP_NODE);
        return INI_FAILED;
    }

    ret = of_property_read_variable_u32_array(np, DTS_PROP_MPXX_NCID_SUPP_LIST,
                                              (uint32_t *)supp_ncids, 1, max_supp_num);
    *supp_num = ret > 0 ? ret : 0;
    return ret > 0 ? INI_SUCC : INI_FAILED;
#else
    return INI_SUCC;
#endif
}
EXPORT_SYMBOL(mpxx_get_support_ncid_from_dts);

int32_t mpxx_get_ncid_from_dts(int32_t *ncid)
{
#ifdef _PRE_CONFIG_USE_DTS
#define NC_ADC_NUM 2
    int32_t ret;
    int32_t adc_ios[NC_ADC_NUM] = { 0 };
    struct device_node *np = NULL;

    if (ncid == NULL) {
        ini_error("invalid params");
        return INI_FAILED;
    }

    np = of_find_compatible_node(NULL, NULL, COST_MPXX_COMP_NODE);
    if (np == NULL) {
        ini_error("dts node %s not found", COST_MPXX_COMP_NODE);
        return INI_FAILED;
    }

    ret = of_property_read_u32_array(np, DTS_PROP_MPXX_NCID_CHAN, (uint32_t *)adc_ios, NC_ADC_NUM);
    if (ret != 0) {
        ini_error("invalid nc_adc_channels");
        return INI_FAILED;
    }

    ret = get_iodie_hkadc_boardid(NC_ADC_NUM, adc_ios, ncid);
    if (ret != 0) {
        ini_error("get_iodie_hkadc_boardid fail");
        return INI_FAILED;
    }
#endif
    return INI_SUCC;
}
EXPORT_SYMBOL(mpxx_get_ncid_from_dts);

int32_t mpxx_get_ini_file_name_by_ncid(uint32_t id, const char *dts_prop, char *prop_value, uint32_t size)
{
#ifdef _PRE_CONFIG_USE_DTS
    uint32_t ncid = 0;
    struct device_node *np = NULL;
    struct device_node *np_parent = NULL;

    np_parent = of_find_compatible_node(NULL, NULL, COST_MPXX_COMP_NODE);
    if (np_parent == NULL) {
        ini_error("dts node %s not found", COST_MPXX_COMP_NODE);
        return INI_FAILED;
    }

    for_each_child_of_node(np_parent, np) {
        if (np == NULL) {
            ini_error("ncid node %s not found", ncid);
            return INI_FAILED;
        }
        if (of_property_read_u32(np, DTS_PROP_MPXX_NCID, &ncid) != 0 || ncid != id) {
            continue;
        }

        return mpxx_get_ini_prop_from_node(np, dts_prop, prop_value, size);
    }
    return INI_FAILED;
#else
    return INI_SUCC;
#endif
}
#endif

int32_t mpxx_get_ini_file_name_from_dts(const char *dts_prop, char *prop_value, uint32_t size)
{
#ifdef _PRE_CONFIG_USE_DTS
    struct device_node *np = NULL;

    np = of_find_compatible_node(NULL, NULL, COST_MPXX_COMP_NODE);
    if (np == NULL) {
        ini_error("dts node %s not found", COST_MPXX_COMP_NODE);
        return INI_FAILED;
    }

    return mpxx_get_ini_prop_from_node(np, dts_prop, prop_value, size);
#endif
    return INI_SUCC;
}

STATIC int32_t mpxx_gpio_dts_init(struct platform_device *pdev)
{
    return BOARD_SUCC;
}

STATIC void mpxx_gpio_dts_free(struct platform_device *pdev)
{
    return;
}

STATIC void mpxx_coldboot_partion_init(void)
{
#ifdef _PRE_CONFIG_USE_DTS
    int32_t ret;
    const char *coldboot_partion = NULL;

    ret = get_board_custmize(DTS_NODE_MPXX_BFGX, DTS_PROP_MPXX_COLDBOOT_PARTION, &coldboot_partion);
    if (ret == BOARD_SUCC) {
        g_st_board_info.coldboot_partion = coldboot_partion;
        ps_print_info("coldboot_partion: %s\n",  coldboot_partion);
    }
#endif
}


STATIC int32_t mpxx_sys_attr_dts_init(struct platform_device *pdev)
{
    int32_t ret;
    ret = mpxx_check_mpxx_subsystem_support();
    if (ret != BOARD_SUCC) {
        ps_print_err("mpxx_check_mpxx_subsystem_support failed\n");
        goto sys_cfg_fail;
    }

    ret = mpxx_check_evb_or_fpga();
    if (ret != BOARD_SUCC) {
        ps_print_err("mpxx_check_evb_or_fpga failed\n");
        goto sys_cfg_fail;
    }

    ret = mpxx_check_pmu_clk_share();
    if (ret != BOARD_SUCC) {
        ps_print_err("mpxx_check_pmu_clk_share failed\n");
        goto sys_cfg_fail;
    }

    ret = mpxx_get_board_pmu_clk32k(pdev);
    if (ret != BOARD_SUCC) {
        ps_print_err("mpxx_check_pmu_clk_share failed\n");
        goto sys_cfg_fail;
    }

    ret = mpxx_get_board_uart_port();
    if (ret != BOARD_SUCC) {
        ps_print_err("get uart port failed\n");
        goto sys_cfg_fail;
    }

    mpxx_coldboot_partion_init();

    mpxx_get_board_pmu_fem_switch1_regulator(pdev);

    return BOARD_SUCC;

sys_cfg_fail:
    return  BOARD_FAIL;
}

void board_callback_init_dts(void)
{
    g_st_board_info.bd_ops.board_gpio_init = mpxx_gpio_dts_init;
    g_st_board_info.bd_ops.board_gpio_free = mpxx_gpio_dts_free;
    g_st_board_info.bd_ops.board_sys_attr_init = mpxx_sys_attr_dts_init;
}
