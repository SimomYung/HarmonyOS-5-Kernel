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

#define MP12_GLB_CTL_G_RB_G_EXT_FLG_REG  0x4004070
#define MP12_GLB_CTL_B_RB_B_EXT_FLG_REG  0x4003070
#define MP12_FIRMWARE_OK                 0xbeaf

/* mp12 */
#define FIRMWARE_MP12_PILOT_CFG_DIR "/vendor/firmware/mp12/pilot"
#define FIRMWARE_MP12_FPGA_CFG_DIR  "/vendor/firmware/mp12/fpga"

STATIC cfg_map_info g_mp12_cfg_patch_in_vendor[] = {
    {
        .idx = BFGX_AND_WIFI_CFG,
        .cfg_path = BFGX_AND_WIFI_CFG_FILE
    }, {
        .idx = WIFI_CFG,
        .cfg_path = WIFI_CFG_FILE
    }, {
        .idx = RAM_REG_TEST_CFG,
        .cfg_path = RAM_WIFI_CHECK_FILE
    }, {
        .idx = G_CFG,
        .cfg_path = GNSS_CFG_FILE
    }, {
        .idx = B_CFG,
        .cfg_path = BSLE_CFG_FILE
    }, {
        .idx = IR_CFG,
        .cfg_path = IR_CFG_FILE
    }, {
        .idx = B_COMB_CFG,
        .cfg_path = BSLE_COMB_CFG_FILE
    }, {
        .idx = G_COMB_CFG,
        .cfg_path = GNSS_COMB_CFG_FILE
    }, {
        .idx = COMC_CFG,
        .cfg_path = COMC_CFG_FILE,
    }, {
        .idx = WIFI_COMB_CFG,
        .cfg_path = WIFI_COMB_CFG_FILE,
    }, {
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

/* 必须和芯片定义的DTCM_PLAT_CUST_LEN长度相同 */
#define DTCM_PLAT_CUST_LEN       0x10

typedef union {
    struct {
        uint8_t release_version;
    } cfg;
    uint8_t data[DTCM_PLAT_CUST_LEN];
} plat_cust_data_mp12;

STATIC bool bsle_is_shutdown(void)
{
    struct ps_core_s *ps_core_d = ps_get_core_reference(BUART);
    if (ps_core_d == NULL) {
        ps_print_info("bfgx not init\n");
        return true;
    }

    return ps_core_chk_bfgx_active(ps_core_d) ? false : true;
}

STATIC bool gnss_is_shutdown(void)
{
    struct ps_core_s *ps_core_d = ps_get_core_reference(GUART);
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
        is_open = bsle_is_shutdown();
    } else if (subsystem == G_SYS) {
        is_open = gnss_is_shutdown();
    } else {
        ps_print_err("invalid system:%d error\n", subsystem);
    }
    return is_open;
}

STATIC int32_t mp12_board_w_power_on(void)
{
    if (bsle_is_shutdown() && gnss_is_shutdown()) {
        ps_print_info("wifi bootup pull up pwr_en\n");
        board_chip_power_on();
    }

    if (board_sys_enable(W_SYS)) {
        ps_print_info("wlan sys enable fail");
        return WIFI_POWER_BFGX_OFF_PULL_WLEN_FAIL;
    }

    if (mpxx_firmware_download_mode() == MODE_COMBO) {
        if (bsle_is_shutdown() && gnss_is_shutdown()) {
            board_sys_enable(B_SYS);
            board_sys_enable(G_SYS);
        }
    }

    return OAL_SUCC;
}

STATIC int32_t mp12_board_b_power_on(void)
{
    if (wlan_is_shutdown() && gnss_is_shutdown()) {
        ps_print_info("bcore bootup pull up pwr_en!\n");
        board_chip_power_on();
    }

    board_sys_enable(B_SYS);
    if (mpxx_is_asic() == VERSION_FPGA) {
        ssi_write_value32_test(MP12_GLB_CTL_B_RB_B_EXT_FLG_REG, MP12_FIRMWARE_OK);
    }
    return  SUCC;
}

STATIC int32_t mp12_board_g_power_on(void)
{
    if (wlan_is_shutdown() && bsle_is_shutdown()) {
        ps_print_info("gnss bootup pull up pwr_en!\n");
        board_chip_power_on();
    }
    board_sys_enable(G_SYS);
    if (mpxx_is_asic() == VERSION_FPGA) {
        ssi_write_value32_test(MP12_GLB_CTL_G_RB_G_EXT_FLG_REG, MP12_FIRMWARE_OK);
    }
    return SUCC;
}

STATIC int32_t mp12_board_power_on(uint32_t ul_subsystem)
{
    if (ul_subsystem == W_SYS) {
        return mp12_board_w_power_on();
    } else if (ul_subsystem == B_SYS) {
        return mp12_board_b_power_on();
    } else if (ul_subsystem == G_SYS) {
        return mp12_board_g_power_on();
    } else {
        ps_print_err("power input system:%d error\n", ul_subsystem);
        return WIFI_POWER_FAIL;
    }
}

STATIC void mp12_board_w_power_off(void)
{
    board_sys_disable(W_SYS);
    if (bsle_is_shutdown() && gnss_is_shutdown()) {
        ps_print_info("wifi close pull down pwr_en\n");
        board_chip_power_off();
    }
}

STATIC void mp12_board_bsle_power_off(void)
{
    board_sys_disable(B_SYS);
    if (wlan_is_shutdown() && gnss_is_shutdown()) {
        ps_print_info("bi close pull down pwr_en\n");
        board_sys_disable(W_SYS); // 混合加载模式
        board_chip_power_off();
    }
}

STATIC void mp12_board_gnss_power_off(void)
{
    board_sys_disable(G_SYS);
    if (wlan_is_shutdown() && bsle_is_shutdown()) {
        ps_print_info("gnss close pull down pwr_en\n");
        board_sys_disable(W_SYS); // 混合加载模式
        board_chip_power_off();
    }
}

STATIC int32_t mp12_board_power_off(uint32_t ul_subsystem)
{
    if (ul_subsystem == W_SYS) {
        mp12_board_w_power_off();
    } else if (ul_subsystem == B_SYS) {
        mp12_board_bsle_power_off();
    } else if (ul_subsystem == G_SYS) {
        mp12_board_gnss_power_off();
    } else {
        ps_print_err("power input system:%d error\n", ul_subsystem);
        return -EFAIL;
    }

    return SUCC;
}

STATIC int32_t mp12_first_power_on_com_download(void)
{
    int32_t ret = BFGX_POWER_SUCCESS;

    if (mpxx_firmware_download_mode() == MODE_COMBO) {
        return BFGX_POWER_SUCCESS;
    }

    if ((gnss_is_shutdown() == true) && (bsle_is_shutdown() == true) && (wlan_is_shutdown() == true)) {
        ret =  firmware_download_function(COMC_CFG, hcc_get_bus(HCC_EP_WIFI_DEV));
    }

    return ret;
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
        if (gnss_is_shutdown() == true) {
            board_sys_enable(G_SYS);
        }
        if (bsle_is_shutdown() == true) {
            board_sys_enable(B_SYS);
        }
    }

    ret = mp12_first_power_on_com_download();
    if (ret != (int32_t)BFGX_POWER_SUCCESS) {
        ps_print_err("comcpu download failed!\n");
        return ret;
    }

    ret = firmware_download_function(cfg, hcc_get_bus(HCC_EP_WIFI_DEV));
    if (ret != (int32_t)BFGX_POWER_SUCCESS) {
        hcc_bus_disable_state(pm_top_data->wlan_pm_info->pst_bus, OAL_BUS_STATE_ALL);
        ps_print_err("bfgx download firmware fail!\n");
        return (ret == -OAL_EINTR) ? BFGX_POWER_DOWNLOAD_FIRMWARE_INTERRUPT : BFGX_POWER_DOWNLOAD_FIRMWARE_FAIL;
    }

    if (mpxx_firmware_download_mode() == MODE_COMBO &&
        oal_atomic_read(&ps_core_d->ir_only) == 0) {
        if (sys == B_SYS) {
            board_sys_disable(G_SYS);
        } else {
            board_sys_disable(B_SYS);
        }
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
        if (gnss_is_shutdown() == false || bsle_is_shutdown() == false || wlan_is_shutdown() == false) {
            ps_print_info("board already power on");
            return BFGX_POWER_ALREADY_DOWNLOAD;
        }
        *whitch_cfg = (sys == B_SYS) ? B_COMB_CFG : G_COMB_CFG;
    }
    return BFGX_POWER_SUCCESS;
}

STATIC int32_t mp12_get_bfgx_separate_cfg(struct ps_core_s *ps_core_d, uint32_t sys, uint32_t *whitch_cfg)
{
    if (oal_atomic_read(&ps_core_d->ir_only) == 1) {
        *whitch_cfg = IR_CFG;
    } else {
        *whitch_cfg = (sys == B_SYS) ? B_CFG : G_CFG;
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
#endif

STATIC void mp12_judge_ir_mode(struct ps_core_s *ps_core_d)
{
    int32_t download_mode;

    download_mode = mpxx_firmware_download_mode();
    if (download_mode == MODE_COMBO) {
        if (wlan_is_shutdown() && gnss_is_shutdown() &&
            bsle_is_shutdown()) {
            oal_atomic_set(&ps_core_d->ir_only, 1);
        }
    } else if (download_mode == MODE_SEPARATE) {
        if (bsle_is_shutdown()) {
            oal_atomic_set(&ps_core_d->ir_only, 1);
        }
    }
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
    ret = mp12_first_power_on_com_download();
    if (ret != BFGX_POWER_SUCCESS) {
        ps_print_err("comcpu download failed!\n");
        return ret;
    }

    if ((mpxx_firmware_download_mode() == MODE_COMBO)) {
        if (bsle_is_shutdown() && gnss_is_shutdown()) {
            ret = firmware_download_function(BFGX_AND_WIFI_CFG, hcc_get_bus(HCC_EP_WIFI_DEV));
            board_sys_disable(B_SYS);
            board_sys_disable(G_SYS);
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
                                  __FUNCTION__, bsle_is_shutdown() ? "off" : "on", error);
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
    board_sys_disable(G_SYS);
#ifndef BFGX_UART_DOWNLOAD_SUPPORT
    board_sys_disable(W_SYS);
#endif
    board_chip_power_off();

    board_chip_power_on();
#ifndef BFGX_UART_DOWNLOAD_SUPPORT
    ret = board_sys_enable(W_SYS);
#endif
    ret = board_sys_enable(B_SYS);
    ret = board_sys_enable(G_SYS);
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

STATIC void board_plat_cust_init_mp12(void)
{
    int32_t ret;
    plat_cust_data_mp12 plat_cust;

    memset_s(&plat_cust, sizeof(plat_cust), 0, sizeof(plat_cust));
    plat_cust.cfg.release_version = (hi110x_get_release_type() == MPXX_VTYPE_RELEASE) ? OAL_TRUE : OAL_FALSE;

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
