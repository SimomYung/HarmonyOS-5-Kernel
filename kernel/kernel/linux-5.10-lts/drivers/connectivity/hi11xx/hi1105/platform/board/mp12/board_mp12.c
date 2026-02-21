/**
 * Copyright (c) @CompanyNameMagicTag 2016-2023. All rights reserved.
 *
 * Description: Driver Entry Function Declaration.
 * Author: @CompanyNameTag
 */

/* 头文件包含 */
#define MPXX_LOG_MODULE_NAME     "[MP12_BOARD]"
#define MPXX_LOG_MODULE_NAME_VAR mp12_board_loglevel

#include "board.h"
#include "board_dts.h"
#include "bfgx_dev.h"
#include "bfgx_core.h"
#include "chr_user.h"
#include "oam_dsm.h"
#include "plat_debug.h"
#include "plat_pm.h"
#include "plat_uart.h"
#include "plat_firmware.h"
#include "plat_firmware_download.h"
#include "plat_cust.h"
#include "plat_common_clk.h"
#include "board_mp12.h"
#ifdef BFGX_UART_DOWNLOAD_SUPPORT
#include "bfgx_data_parse.h"
#include "plat_firmware_uart.h"
#endif
#include "plat_cust_mp12.h"

/* mp12 */
#define FIRMWARE_MP12_PILOT_CFG_DIR "/vendor/firmware/mp12/pilot"
#define FIRMWARE_MP12_FPGA_CFG_DIR  "/vendor/firmware/mp12/fpga"

/* gnss dpe switch */
#define INI_GNSS_DPE_SWICH "gnss_dpe_switch"

STATIC cfg_map_info g_mp12_cfg_patch_in_vendor[] = {
    {
        .idx = BFGX_AND_WIFI_CFG,
        .cfg_path = BFGX_AND_WIFI_CFG_FILE,
    },
    {
        .idx = WIFI_CFG,
        .cfg_path = WIFI_CFG_FILE,
    },
    {
        .idx = BGI_CFG,
        .cfg_path = BGI_CFG_FILE
    },
    {
        .idx = IR_CFG,
        .cfg_path = IR_CFG_FILE
    },
    {
        .idx = BGI_COMB_CFG,
        .cfg_path = BGI_COMB_CFG_FILE
    },
    {
        .idx = WIFI_COMB_CFG,
        .cfg_path = WIFI_COMB_CFG_FILE,
    },
    {
        .idx = IR_COMB_CFG,
        .cfg_path = IR_COMB_CFG_FILE,
    }
};

chip_firmware_cfg_str g_mp12_cfg_str[] = {
    {
        .chip_nr = BOARD_VERSION_MP12,
        .is_asic = VERSION_FPGA,
        .cfg_map_size = oal_array_size(g_mp12_cfg_patch_in_vendor),
        .cfg_map = g_mp12_cfg_patch_in_vendor,
        .pre_directory = FIRMWARE_MP12_FPGA_CFG_DIR
    }, {
        .chip_nr = BOARD_VERSION_MP12,
        .is_asic = VERSION_ASIC,
        .cfg_map_size = oal_array_size(g_mp12_cfg_patch_in_vendor),
        .cfg_map = g_mp12_cfg_patch_in_vendor,
        .pre_directory = FIRMWARE_MP12_PILOT_CFG_DIR
    }
};

STATIC bool bgi_is_shutdown(void)
{
    struct ps_core_s *ps_core_d = ps_get_core_reference(BUART);
    if (ps_core_d == NULL) {
        ps_print_info("bfgx not init\n");
        return true;
    }

    return ps_core_chk_bfgx_active(ps_core_d) ? false : true;
}

bool mp12_susbsys_is_shutdown(uint32_t subsystem)
{
    bool is_open = OAL_FALSE;
    if (subsystem == W_SYS) {
        is_open = wlan_is_shutdown();
    } else if (subsystem == B_SYS) {
        is_open = bgi_is_shutdown();
    } else {
        ps_print_err("invalid system:%d error\n", subsystem);
    }
    return is_open;
}

STATIC void board_bfgx_plat_cust_set_mp12(struct ps_core_s *ps_core_d)
{
    plat_cust_data_mp12 *plat_cust = NULL;
    struct ps_plat_s *ps_plat_d = NULL;

    ps_plat_d = (struct ps_plat_s *)ps_core_d->ps_plat;
    if (ps_plat_d == NULL) {
        return;
    }

    plat_cust = (plat_cust_data_mp12 *)get_plat_cust_addr();
    if (plat_cust == NULL) {
        ps_print_err("plat_cust buf not init, please check\n");
        return;
    }

    plat_cust->cfg.buart_rate = ps_plat_d->baud_rate;
    ps_print_info("[%s]baud_rate set = %lu\n", index2name(ps_core_d->uart_index), ps_plat_d->baud_rate);
}

STATIC int32_t mp12_board_w_power_on(void)
{
    if (bgi_is_shutdown()) {
        ps_print_info("wifi bootup pull up pwr_en\n");
        board_chip_power_on();
    }

    if (board_sys_enable(W_SYS)) {
        ps_print_info("wlan sys enable fail");
        return WIFI_POWER_BFGX_OFF_PULL_WLEN_FAIL;
    }

    if (mpxx_firmware_download_mode() == MODE_COMBO) {
        if (bgi_is_shutdown()) {
            board_sys_enable(B_SYS);
        }
    }

    return OAL_SUCC;
}

STATIC int32_t mp12_board_bgi_power_on(void)
{
    if (wlan_is_shutdown()) {
        ps_print_info("bcore bootup pull up pwr_en!\n");
        board_chip_power_on();
    }

    board_sys_enable(B_SYS);
    return  SUCC;
}

STATIC int32_t mp12_board_power_on(uint32_t ul_subsystem)
{
    if (ul_subsystem == W_SYS) {
        return mp12_board_w_power_on();
    } else if (ul_subsystem == B_SYS) {
        return mp12_board_bgi_power_on();
    } else {
        ps_print_err("power input system:%d error\n", ul_subsystem);
        return WIFI_POWER_FAIL;
    }
}

STATIC void mp12_board_w_power_off(void)
{
    board_sys_disable(W_SYS);
    if (bgi_is_shutdown()) {
        ps_print_info("wifi close pull down pwr_en\n");
        board_chip_power_off();
    }
}

STATIC void mp12_board_bgi_power_off(void)
{
    board_sys_disable(B_SYS);
    if (wlan_is_shutdown()) {
        ps_print_info("bi close pull down pwr_en\n");
        board_sys_disable(W_SYS); // 混合加载模式
        board_chip_power_off();
    }
}

STATIC int32_t mp12_board_power_off(uint32_t ul_subsystem)
{
    if (ul_subsystem == W_SYS) {
        mp12_board_w_power_off();
    } else if (ul_subsystem == B_SYS) {
        mp12_board_bgi_power_off();
    } else {
        ps_print_err("power input system:%d error\n", ul_subsystem);
        return -EFAIL;
    }

    return SUCC;
}

#ifndef BFGX_UART_DOWNLOAD_SUPPORT
STATIC int32_t mp12_bfgx_download_handler(struct ps_core_s *ps_core_d, uint32_t cfg, uint32_t sys)
{
    int32_t ret;
    struct pm_top* pm_top_data = pm_get_top();

    if (wlan_is_shutdown()) {
        ret = board_sys_enable(W_SYS);
        if (ret != SUCC) {
            return -EFAIL;
        }
    }

    if (mpxx_firmware_download_mode() == MODE_COMBO &&
        oal_atomic_read(&ps_core_d->ir_only) == 0) {
        if (bgi_is_shutdown() == true) {
            board_sys_enable(B_SYS);
        }
    }

    ret = firmware_download_function(cfg, hcc_get_bus(HCC_EP_WIFI_DEV));
    if (ret != (int32_t)BFGX_POWER_SUCCESS) {
        hcc_bus_disable_state(pm_top_data->wlan_pm_info->pst_bus, OAL_BUS_STATE_ALL);
        ps_print_err("bfgx download firmware fail!\n");
        return (ret == -OAL_EINTR) ? BFGX_POWER_DOWNLOAD_FIRMWARE_INTERRUPT : BFGX_POWER_DOWNLOAD_FIRMWARE_FAIL;
    }

    if (wlan_is_shutdown()) {
        hcc_bus_disable_state(pm_top_data->wlan_pm_info->pst_bus, OAL_BUS_STATE_ALL);
        board_sys_disable(W_SYS);
    }

    return OAL_SUCC;
}

STATIC int32_t mp12_get_bfgx_comb_dl_cfg(struct ps_core_s *ps_core_d, uint32_t sys, uint32_t *whitch_cfg)
{
    if (oal_atomic_read(&ps_core_d->ir_only) == 1) {
        *whitch_cfg = IR_COMB_CFG;
    } else {
        // 混合加载时wifi上电已经加载完所有子系统FRW
        if (wlan_is_shutdown() == false) {
            ps_print_info("board already power on");
            return BFGX_POWER_ALREADY_DOWNLOAD;
        }
        *whitch_cfg = BGI_COMB_CFG;
    }
    return BFGX_POWER_SUCCESS;
}

STATIC int32_t mp12_get_bfgx_separate_cfg(struct ps_core_s *ps_core_d, uint32_t sys, uint32_t *whitch_cfg)
{
    if (oal_atomic_read(&ps_core_d->ir_only) == 1) {
        *whitch_cfg = IR_CFG;
    } else {
        *whitch_cfg = BGI_CFG;
    }
    return BFGX_POWER_SUCCESS;
}

STATIC int32_t mp12_bfgx_download_by_wifi(struct ps_core_s *ps_core_d, uint32_t sys)
{
    int32_t ret;
    uint32_t whitch_cfg;
    int32_t download_mode;

    download_mode = mpxx_firmware_download_mode();
    if ((download_mode >= MODE_DOWNLOAD_BUTT) || (download_mode < 0)) {
        ps_print_err("download mode[%d] not recognize", download_mode);
        return BFGX_POWER_FAILED;
    }

    if (download_mode == MODE_COMBO) {
        ret = mp12_get_bfgx_comb_dl_cfg(ps_core_d, sys, &whitch_cfg);
    } else {
        ret = mp12_get_bfgx_separate_cfg(ps_core_d, sys, &whitch_cfg);
    }

    if (ret == BFGX_POWER_ALREADY_DOWNLOAD) {
        return BFGX_POWER_SUCCESS;
    }

    if (mp12_bfgx_download_handler(ps_core_d, whitch_cfg, sys) != OAL_SUCC) {
        ps_print_info("wifi download bfgx frw fail");
        return BFGX_POWER_DOWNLOAD_FIRMWARE_FAIL;
    }

    return BFGX_POWER_SUCCESS;
}
#else
STATIC int32_t mp12_bfgx_download_by_uart(struct ps_core_s *ps_core_d)
{
    int32_t error;

    st_tty_recv = ps_recv_patch;
    error = ps_core_d->pm_data->download_patch(ps_core_d);
    if (error) { /* if download patch err,and close uart */
        ps_print_err(" download_patch is failed!\n");
        st_tty_recv = ps_core_recv;
        return error;
    }

    ps_print_suc("download_patch is successfully!\n");
    st_tty_recv = ps_core_recv;
    return error;
}

STATIC int32_t mp12_bfgx_firmware_download(struct ps_core_s *ps_core_d)
{
    int32_t error;

    error = mp12_bfgx_download_by_uart(ps_core_d);

    return error;
}
#endif

STATIC void mp12_judge_ir_mode(struct ps_core_s *ps_core_d)
{
    int32_t download_mode;

    download_mode = mpxx_firmware_download_mode();
    if (download_mode == MODE_COMBO) {
        if (wlan_is_shutdown() && bgi_is_shutdown()) {
            oal_atomic_set(&ps_core_d->ir_only, 1);
        }
    } else if (download_mode == MODE_SEPARATE) {
        if (bgi_is_shutdown()) {
            oal_atomic_set(&ps_core_d->ir_only, 1);
        }
    }

#ifdef BFGX_UART_DOWNLOAD_SUPPORT
    // uart加载单红外bin场景, 需要修改uart波特率
    if (oal_atomic_read(&ps_core_d->ir_only) != 0) {
        g_ir_only_baud_rate = UART_BAUD_RATE_2M;
    }
#endif
}

STATIC int32_t mp12_bfgx_dev_power_on(struct ps_core_s *ps_core_d, uint32_t sys, uint32_t subsys)
{
    int32_t ret;
    int32_t error = BFGX_POWER_SUCCESS;

    if ((ps_core_d == NULL) || (ps_core_d->pm_data == NULL)) {
        ps_print_err("ps_core_d is err\n");
        return BFGX_POWER_FAILED;
    }

    if (subsys == BFGX_IR) {
        mp12_judge_ir_mode(ps_core_d);
        ps_print_info("ir mode[%d]\n", oal_atomic_read(&ps_core_d->ir_only));
    }

    board_bfgx_plat_cust_set_mp12(ps_core_d);
    ret = mp12_board_power_on(sys);
    if (ret) {
        ps_print_err("mp12_board_power_on bfg failed, ret=%d\n", ret);
        error = BFGX_POWER_PULL_POWER_GPIO_FAIL;
        goto bfgx_power_on_fail;
    }

    bfgx_gpio_intr_enable(ps_core_d->pm_data, OAL_TRUE);
    if (open_tty_drv(ps_core_d) != BFGX_POWER_SUCCESS) {
        ps_print_err("open tty fail!\n");
        error = BFGX_POWER_TTY_OPEN_FAIL;
        goto bfgx_power_on_fail;
    }
#ifndef BFGX_UART_DOWNLOAD_SUPPORT
    error = mp12_bfgx_download_by_wifi(ps_core_d, sys);
    if (error != BFGX_POWER_SUCCESS) {
        goto bfgx_power_on_fail;
    }
#else
    error = mp12_bfgx_firmware_download(ps_core_d);
    if (error != BFGX_POWER_SUCCESS) {
        release_tty_drv(ps_core_d);
        goto bfgx_power_on_fail;
    }
#endif
    return BFGX_POWER_SUCCESS;

bfgx_power_on_fail:
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
    if (error != BFGX_POWER_DOWNLOAD_FIRMWARE_INTERRUPT) {
        hw_mpxx_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_MPXX_DOWNLOAD_FIRMWARE,
                                  "bcpu download firmware failed,wifi %s,ret=%d,process:%s\n",
                                  wlan_is_shutdown() ? "off" : "on", error, current->comm);
    }
#endif
    (void)mp12_board_power_off(sys);
    return error;
}

STATIC int32_t mp12_bfgx_dev_power_off(struct ps_core_s *ps_core_d, uint32_t sys, uint32_t subsys)
{
    struct pm_drv_data *pm_data = NULL;

    ps_print_info("%s sys[%d] subsys[%d] \n", __func__, sys, subsys);
    if ((ps_core_d == NULL) || (ps_core_d->pm_data == NULL)) {
        ps_print_err("ps_core_d is err\n");
        return BFGX_POWER_FAILED;
    }

    pm_data = ps_core_d->pm_data;
    bfgx_gpio_intr_enable(pm_data, OAL_FALSE);

    if (wait_bfgx_memdump_complete(ps_core_d) != EXCEPTION_SUCCESS) {
        ps_print_err("wait memdump complete failed\n");
    }

    if (release_tty_drv(ps_core_d) != SUCCESS) {
        /* 代码执行到此处，说明六合一所有业务都已经关闭，无论tty是否关闭成功，device都要下电 */
        ps_print_err("wifi off, close tty is err!");
    }

    pm_data->bfgx_dev_state = BFGX_SLEEP;
    pm_data->uart_state = UART_NOT_READY;

    (void)mp12_board_power_off(sys);

    if (subsys == BFGX_IR) {
        oal_atomic_set(&ps_core_d->ir_only, 0);
    }
    return BFGX_POWER_SUCCESS;
}

STATIC int32_t mp12_wlan_firmware_download(void)
{
    int32_t ret;
    if ((mpxx_firmware_download_mode() == MODE_COMBO)) {
        if (bgi_is_shutdown()) {
            ret = firmware_download_function(BFGX_AND_WIFI_CFG, hcc_get_bus(HCC_EP_WIFI_DEV));
            board_sys_disable(B_SYS);
        }  else {
            ret = firmware_download_function(WIFI_COMB_CFG, hcc_get_bus(HCC_EP_WIFI_DEV));
        }
    } else {
        ret = firmware_download_function(WIFI_CFG, hcc_get_bus(HCC_EP_WIFI_DEV));
    }

    return ret;
}

STATIC int32_t mp12_wlan_power_on(void)
{
    int32_t ret, error;
    struct wlan_pm_s *wlan_pm_info = wlan_pm_get_drv();

    error = mp12_board_power_on(W_SYS);
    if (error != SUCC) {
        ps_print_err("board power on fail\n");
        goto wifi_power_fail;
    }

    hcc_bus_power_action(hcc_get_bus(HCC_EP_WIFI_DEV), HCC_BUS_POWER_PATCH_LOAD_PREPARE);

    error = mp12_wlan_firmware_download();
    if (error != (int32_t)WIFI_POWER_SUCCESS) {
        ps_print_err("firmware download fail\n");
        if (error == -OAL_EINTR) {
            error = WIFI_POWER_ON_FIRMWARE_DOWNLOAD_INTERRUPT;
        } else if (error == -OAL_ENOPERMI) {
            error = WIFI_POWER_ON_FIRMWARE_FILE_OPEN_FAIL;
        } else {
            error = WIFI_POWER_BFGX_OFF_FIRMWARE_DOWNLOAD_FAIL;
        }
        goto wifi_power_fail;
    } else {
        wlan_pm_info->wlan_power_state = POWER_STATE_OPEN;
    }

    ret = hcc_bus_power_action(hcc_get_bus(HCC_EP_WIFI_DEV), HCC_BUS_POWER_PATCH_LAUCH);
    if (ret != 0) {
        declare_dft_trace_key_info("wlan_poweron HCC_BUS_POWER_PATCH_LAUCH by gpio_fail", OAL_DFT_TRACE_FAIL);
        error = WIFI_POWER_BFGX_OFF_BOOT_UP_FAIL;
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                             CHR_PLT_DRV_EVENT_OPEN, CHR_PLAT_DRV_ERROR_WCPU_BOOTUP);
        goto wifi_power_fail;
    }

    return WIFI_POWER_SUCCESS;
wifi_power_fail:
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
    if (error != (int32_t)WIFI_POWER_ON_FIRMWARE_DOWNLOAD_INTERRUPT &&
        error != (int32_t)WIFI_POWER_ON_FIRMWARE_FILE_OPEN_FAIL) {
        hw_mpxx_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_MPXX_DOWNLOAD_FIRMWARE,
                                  "%s: failed to download firmware, bfgx %s, error=%d\n",
                                  __FUNCTION__, bgi_is_shutdown() ? "off" : "on", error);
    }
#endif
    return error;
}

STATIC int32_t mp12_wlan_power_off(void)
{
    struct wlan_pm_s *wlan_pm_info = wlan_pm_get_drv();

    /* 先关闭SDIO TX通道 */
    hcc_bus_disable_state(hcc_get_bus(HCC_EP_WIFI_DEV), OAL_BUS_STATE_TX);

    /* wakeup dev,send poweroff cmd to wifi */
    if (wlan_pm_poweroff_cmd() != OAL_SUCC) {
        /* wifi self close 失败了也继续往下执行，uart关闭WCPU，异常恢复推迟到wifi下次open的时候执行 */
        declare_dft_trace_key_info("wlan_poweroff_cmd_fail", OAL_DFT_TRACE_FAIL);
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                             CHR_PLT_DRV_EVENT_CLOSE, CHR_PLAT_DRV_ERROR_CLOSE_WCPU);
    }
    hcc_bus_disable_state(hcc_get_bus(HCC_EP_WIFI_DEV), OAL_BUS_STATE_ALL);

    (void)mp12_board_power_off(W_SYS);

    if (wlan_pm_info != NULL) {
        wlan_pm_info->wlan_power_state = POWER_STATE_SHUTDOWN;
    }
    return SUCCESS;
}

STATIC void mp12_power_state_change(int32_t type, int32_t flag)
{
    if (flag == BOARD_POWER_ON) {
        conn_set_gpio_level(type, GPIO_LEVEL_LOW);
        mdelay(10); /* delay 10ms */
        conn_set_gpio_level(type, GPIO_LEVEL_HIGH);
        // 上电要等待efuse ready时间比较长， 需要等待60ms
        mdelay(60);
    } else if (flag == BOARD_POWER_OFF) {
        conn_set_gpio_level(type, GPIO_LEVEL_LOW);
        mdelay(10); /* delay 10ms */
    }
}

STATIC int32_t mp12_board_power_reset(uint32_t ul_subsystem)
{
    int32_t ret;
    board_sys_disable(B_SYS);
#ifndef BFGX_UART_DOWNLOAD_SUPPORT
    board_sys_disable(W_SYS);
#endif
    board_chip_power_off();

    board_chip_power_on();
#ifndef BFGX_UART_DOWNLOAD_SUPPORT
    ret = board_sys_enable(W_SYS);
#endif
    ret = board_sys_enable(B_SYS);
    return ret;
}

STATIC void board_callback_init_mp12_power(void)
{
    g_st_board_info.bd_ops.bfgx_dev_power_on = mp12_bfgx_dev_power_on;
    g_st_board_info.bd_ops.bfgx_dev_power_off = mp12_bfgx_dev_power_off;
    g_st_board_info.bd_ops.wlan_power_on = mp12_wlan_power_on;
    g_st_board_info.bd_ops.wlan_power_off = mp12_wlan_power_off;
    g_st_board_info.bd_ops.board_power_on = mp12_board_power_on;
    g_st_board_info.bd_ops.board_power_off = mp12_board_power_off;
    g_st_board_info.bd_ops.susbsys_is_shutdown = mp12_susbsys_is_shutdown;
    g_st_board_info.bd_ops.power_state_change = mp12_power_state_change;
    g_st_board_info.bd_ops.board_power_reset = mp12_board_power_reset;
    g_st_board_info.bd_ops.board_power_notify = NULL;
    g_st_board_info.bd_ops.board_error_handler = NULL;
}

STATIC void board_get_gnss_dpe_switch(plat_cust_data_mp12 *cust)
{
    int32_t cfg_value;
    int32_t ret;

    cust->cfg.gnss_dpe_switch = 0;

    /* 获取ini的配置值 */
    ret = get_cust_conf_int32(INI_MODU_GNSS, INI_GNSS_DPE_SWICH, &cfg_value);
    if (ret == INI_FAILED) {
        return;
    }

    cust->cfg.gnss_dpe_switch = (uint32_t)cfg_value;
    ps_print_info("get gnss dpe switch =%d\n", cust->cfg.gnss_dpe_switch);
}

STATIC void board_get_sensorhub_enable(plat_cust_data_mp12 *cust)
{
    int32_t cfg_value;
    int32_t ret;

    cust->cfg.sensorhub_enable = OAL_TRUE;

    /* 获取ini的配置值 */
    ret = get_cust_conf_int32(INI_MODU_PLAT, "plat_sensorhub_enable", &cfg_value);
    if (ret == INI_FAILED) {
        return;
    }

    if ((cfg_value == OAL_FALSE) || (cfg_value == OAL_TRUE)) {
        cust->cfg.sensorhub_enable = cfg_value;
    }

    ps_print_info("get plat_sensorhub_enable =%d\n", cust->cfg.sensorhub_enable);
}

STATIC void board_plat_cust_init_mp12(void)
{
    int32_t ret;
    plat_cust_data_mp12 plat_cust;

    memset_s(&plat_cust, sizeof(plat_cust), 0, sizeof(plat_cust));
    plat_cust.cfg.release_version = (hi110x_get_release_type() == MPXX_VTYPE_RELEASE) ? OAL_TRUE : OAL_FALSE;

    board_get_gnss_dpe_switch(&plat_cust);
    board_get_sensorhub_enable(&plat_cust);

    ret = set_plat_cust_buf((uint8_t *)&plat_cust, sizeof(plat_cust_data_mp12));
    if (ret != SUCC) {
        ps_print_err("set plat cust buf fail, ret = %d\n", ret);
        return;
    }
    ps_print_info("set plat cust buf succ, vtype=%d\n", hi110x_get_release_type());
}

int32_t board_info_init_mp12(void)
{
    board_callback_init_dts();
    board_callback_init_mp12_power();
    board_clk_init(DET_BY_WKUP_IO, CLK_INIT_INI);
    chip_firmware_cfg_register(g_mp12_cfg_str, oal_array_size(g_mp12_cfg_str));
    board_plat_cust_init_mp12();
    return BOARD_SUCC;
}
