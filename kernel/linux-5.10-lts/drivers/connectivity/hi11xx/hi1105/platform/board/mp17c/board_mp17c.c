/**
 * Copyright (c) @CompanyNameMagicTag 2016-2023. All rights reserved.
 *
 * Description: Driver Entry Function Declaration.
 * Author: @CompanyNameTag
 */

/* 头文件包含 */
#define MPXX_LOG_MODULE_NAME     "[MP17C_BOARD]"
#define MPXX_LOG_MODULE_NAME_VAR mp17c_board_loglevel

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
#include "hisi_conn_nve.h"
#include "nve_info_interface.h"
#include "plat_cust.h"
#include "hisi_ini.h"
#include "plat_common_clk.h"
#include "gpio_mp17c.h"
#include "pcie_host.h"
#include "pcie_linux.h"
#include "chr_devs.h"
#include "oal_util.h"
#include "board_mp17c.h"

#define MP17C_GLB_CTL_G_RB_G_EXT_FLG_REG  0x4004070
#define MP17C_FIRMWARE_OK                 0xbeaf
// 由于devcie划定的存储空间限制，这里需要做判断， device预留空间为192字节
oal_static_assert(((sizeof(plat_cust_data_mp17c)) <= SHARE_MEM_PLAT_CUST_LEN), device_mem_must_big_than_host);

/* efuse 规避方案 */
#define MP17C_VBG_THRESH      440
#define MP17C_EFUSE_DET_VER   7

typedef enum {
    NVRAM_EFUSE_VER,
    NVRAM_EFUSE_VBG_TIME,
    NVRAM_EFUSE_CRC,
    NVRAM_EFUSE_BUFF
} mp17c_nv_efuse_enum;

#define MP17C_GLBA_CTL_RB_GLB_CTL_SYS_CTL_ID_REG                    0x4000000
#define MP17C_LP_CTL_GLB_RB_LP_CTL_GLB_73_REG                       0x4006170
#define LP_CTL_GLB_RB_D_BG_PMU_REF_BG_EN_MAN_SEL_MASK               0x400
#define MP17C_LP_CTL_GLB_RB_LP_CTL_GLB_75_REG                       0x4006178
#define LP_CTL_GLB_RB_D_WL_PMU_REF_BG_EN_MAN_SEL_MASK               0x200
#define MP17C_LP_CTL_GLB_RB_LP_CTL_GLB_82_REG                       0x400619c
#define LP_CTL_GLB_RB_D_BG_PMU_REF_BG_EN_MAN_MASK                   0x2
#define MP17C_LP_CTL_GLB_RB_LP_CTL_GLB_88_REG                       0x40061b4
#define LP_CTL_GLB_RB_D_WL_PMU_REF_BG_EN_MAN_MASK                   0x2

#define SSI_MASTER_CUSTOM_REGBANK_SSI_RST_A_CRG_SEL_MASK            0x200
#define SSI_MASTER_CUSTOM_REGBANK_SSI_RST_A_LGC_SEL_MASK            0x100
#define SSI_MASTER_CUSTOM_REGBANK_SSI_RST_A_CRG_N_MASK              0x200
#define SSI_MASTER_CUSTOM_REGBANK_SSI_RST_A_LGC_N_MASK              0x100

/*
 * mp17c 支撑三种时钟
 * 主路时钟
 * 次路时钟 gnss时钟
 * 三路时钟 rsmc时钟
*/

/* mp17c */
#define FIRMWARE_MP17C_PILOT_CFG_DIR "/vendor/firmware/mp17c/pilot"
#define FIRMWARE_MP17C_FPGA_CFG_DIR  "/vendor/firmware/mp17c/fpga"

STATIC void mp17c_bugfix_after_power_en(void);

typedef struct {
    uint32_t ccause;
    uint32_t mcause;
    uint32_t mtval;
    uint32_t gp;
    uint32_t mstatus;
    uint32_t pc;
    uint32_t a3;
    uint32_t a2;
    uint32_t a1;
    uint32_t a0;
    uint32_t t2;
    uint32_t t1;
    uint32_t t0;
    uint32_t lr;
    uint32_t clkpll_sts;
} chr_inner_mp17c_wifi_panic_info;


STATIC cfg_map_info g_mp17c_cfg_patch_in_vendor[] = {
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

chip_firmware_cfg_str g_mp17c_cfg_str[] = {
    {
        .chip_nr = BOARD_VERSION_MP17C,
        .is_asic = VERSION_FPGA,
        .cfg_map_size = oal_array_size(g_mp17c_cfg_patch_in_vendor),
        .cfg_map = g_mp17c_cfg_patch_in_vendor,
        .pre_directory = FIRMWARE_MP17C_FPGA_CFG_DIR
    }, {
        .chip_nr = BOARD_VERSION_MP17C,
        .is_asic = VERSION_ASIC,
        .cfg_map_size = oal_array_size(g_mp17c_cfg_patch_in_vendor),
        .cfg_map = g_mp17c_cfg_patch_in_vendor,
        .pre_directory = FIRMWARE_MP17C_PILOT_CFG_DIR
    }
};

STATIC bool g_ssi_nv_supprot = true;
STATIC enum {
    SSI_CFG_BYPASS,
    SSI_CFG_SUCC,
    SSI_CFG_TRY_SUCC,
    SSI_CFG_FAIL
} g_ssi_config_debug = SSI_CFG_BYPASS;

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

STATIC bool mp17c_susbsys_is_shutdown(uint32_t subsystem)
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

/*
 * 设置Vmin模式，下次加载firmware时随plat_cust数据下发
 * factory模式下禁止修改vmin_mode
 */
STATIC void board_vmin_mode_set_mp17c(uint32_t vmin_mode, uint32_t errid)
{
    plat_cust_data_mp17c *plat_cust = NULL;

    plat_cust = (plat_cust_data_mp17c *)get_plat_cust_addr();
    if (plat_cust == NULL) {
        ps_print_err("plat_cust buf not init, please check\n");
        return;
    }

    if (plat_cust->cfg.vmin_mode == VMIN_MODE_FACTORY) {
        ps_print_err("current vmin_mode is factory, not allow reset\n");
        return;
    }

    if (vmin_mode >= VMIN_MODE_BUTT) {
        plat_cust->cfg.vmin_mode = VMIN_MODE_NORMAL;
    } else {
        plat_cust->cfg.vmin_mode = vmin_mode;
    }
    ps_print_info("request vmin_mode = %u, set = %u\n", vmin_mode, plat_cust->cfg.vmin_mode);
    chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                         CHR_PLT_DRV_EVENT_PM, errid);
}

STATIC void mp17c_board_error_handler(uint32_t errid)
{
    switch (errid) {
        case CHR_PLAT_DRV_ERROR_VMIN_WLAN_POWER_FAIL:
        case CHR_PLAT_DRV_ERROR_VMIN_BFGX_POWER_FAIL:
        case CHR_PLAT_DRV_ERROR_VMIN_WLAN_PLL_REG_WRITE_FAIL:
        case CHR_PLAT_DRV_ERROR_VMIN_WLAN_PHY_REG_WRITE_FAIL:
        case CHR_PLAT_DRV_ERROR_VMIN_WLAN_CALI_TIMEOUT:
            board_vmin_mode_set_mp17c(VMIN_MODE_EMERGENCY, errid);
            break;
        default:
            ps_print_info("no such vmin err type\n");
    }
}

STATIC void board_baud_rate_set_mp17c(struct ps_core_s *ps_core_d)
{
    plat_cust_data_mp17c *plat_cust = NULL;
    struct ps_plat_s *ps_plat_d = NULL;

    ps_plat_d = (struct ps_plat_s *)ps_core_d->ps_plat;
    if (ps_plat_d == NULL) {
        return;
    }

    plat_cust = (plat_cust_data_mp17c *)get_plat_cust_addr();
    if (plat_cust == NULL) {
        ps_print_err("plat_cust buf not init, please check\n");
        return;
    }

    if (ps_plat_d->uart_index == BUART) {
        plat_cust->cfg.buart_rate = ps_plat_d->baud_rate;
    } else if (ps_plat_d->uart_index == GUART) {
        plat_cust->cfg.guart_rate = ps_plat_d->baud_rate;
    }

    ps_print_info("[%s]baud_rate set = %lu\n", index2name(ps_core_d->uart_index), ps_plat_d->baud_rate);
}

STATIC int32_t mp17c_board_w_power_on(void)
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

STATIC int32_t mp17c_board_b_power_on(void)
{
    if (wlan_is_shutdown() && gnss_is_shutdown()) {
        ps_print_info("bcore bootup pull up pwr_en!\n");
        board_chip_power_on();
    }

    board_sys_enable(B_SYS);
    return  SUCC;
}

STATIC int32_t mp17c_board_g_power_on(void)
{
    if (wlan_is_shutdown() && bsle_is_shutdown()) {
        ps_print_info("gnss bootup pull up pwr_en!\n");
        board_chip_power_on();
    }
    board_sys_enable(G_SYS);
    if (mpxx_is_asic() == VERSION_FPGA) {
        ssi_write_value32_test(MP17C_GLB_CTL_G_RB_G_EXT_FLG_REG, MP17C_FIRMWARE_OK);
    }
    return SUCC;
}

STATIC int32_t mp17c_board_power_on(uint32_t ul_subsystem)
{
    int32_t ret;
    ret = mp17c_gugong_subsys_power_on(ul_subsystem);
    if (ret < 0) {
        ps_print_err("subsys:%d gugong check fail\n", ul_subsystem);
        return WIFI_POWER_FAIL;
    }

    if (ul_subsystem == W_SYS) {
        return mp17c_board_w_power_on();
    } else if (ul_subsystem == B_SYS) {
        return mp17c_board_b_power_on();
    } else if (ul_subsystem == G_SYS) {
        return mp17c_board_g_power_on();
    } else {
        ps_print_err("power input system:%d error\n", ul_subsystem);
        return WIFI_POWER_FAIL;
    }
}

STATIC void mp17c_board_w_power_off(void)
{
    board_sys_disable(W_SYS);
    if (bsle_is_shutdown() && gnss_is_shutdown()) {
        ps_print_info("wifi close pull down pwr_en\n");
        board_chip_power_off();
    }
}

STATIC void mp17c_board_bsle_power_off(void)
{
    board_sys_disable(B_SYS);
    if (wlan_is_shutdown() && gnss_is_shutdown()) {
        ps_print_info("bi close pull down pwr_en\n");
        board_sys_disable(W_SYS); // 混合加载模式
        board_chip_power_off();
    }
}

STATIC void mp17c_board_gnss_power_off(void)
{
    board_sys_disable(G_SYS);
    if (wlan_is_shutdown() && bsle_is_shutdown()) {
        ps_print_info("gnss close pull down pwr_en\n");
        board_sys_disable(W_SYS); // 混合加载模式
        board_chip_power_off();
    }
}

STATIC int32_t mp17c_board_power_off(uint32_t ul_subsystem)
{
    if (ul_subsystem == W_SYS) {
        mp17c_board_w_power_off();
    } else if (ul_subsystem == B_SYS) {
        mp17c_board_bsle_power_off();
    } else if (ul_subsystem == G_SYS) {
        mp17c_board_gnss_power_off();
    } else {
        ps_print_err("power input system:%d error\n", ul_subsystem);
        return -EFAIL;
    }

    mp17c_gugong_subsys_power_off(ul_subsystem);

    return SUCC;
}

STATIC int32_t mp17c_first_power_on_com_download(void)
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
STATIC int32_t mp17c_bfgx_download_handler(struct ps_core_s *ps_core_d, uint32_t cfg, uint32_t sys)
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

    ret = mp17c_first_power_on_com_download();
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

STATIC int32_t mp17c_get_bfgx_comb_dl_cfg(struct ps_core_s *ps_core_d, uint32_t sys, uint32_t *whitch_cfg)
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

STATIC int32_t mp17c_get_bfgx_separate_cfg(struct ps_core_s *ps_core_d, uint32_t sys, uint32_t *whitch_cfg)
{
    if (oal_atomic_read(&ps_core_d->ir_only) == 1) {
        *whitch_cfg = IR_CFG;
    } else {
        *whitch_cfg = (sys == B_SYS) ? B_CFG : G_CFG;
    }
    return BFGX_POWER_SUCCESS;
}

STATIC int32_t mp17c_bfgx_download_by_wifi(struct ps_core_s *ps_core_d, uint32_t sys)
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
        ret = mp17c_get_bfgx_comb_dl_cfg(ps_core_d, sys, &whitch_cfg);
    } else {
        ret = mp17c_get_bfgx_separate_cfg(ps_core_d, sys, &whitch_cfg);
    }

    if (ret == BFGX_POWER_ALREADY_DOWNLOAD) {
        return BFGX_POWER_SUCCESS;
    }

    if (mp17c_bfgx_download_handler(ps_core_d, whitch_cfg, sys) != OAL_SUCC) {
        ps_print_info("wifi download bfgx frw fail");
        return BFGX_POWER_DOWNLOAD_FIRMWARE_FAIL;
    }

    return BFGX_POWER_SUCCESS;
}
#endif

STATIC void mp17c_judge_ir_mode(struct ps_core_s *ps_core_d)
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

STATIC int32_t mp17c_bfgx_dev_power_on(struct ps_core_s *ps_core_d, uint32_t sys, uint32_t subsys)
{
    int32_t ret;
    int32_t error = BFGX_POWER_SUCCESS;

    if ((ps_core_d == NULL) || (ps_core_d->pm_data == NULL)) {
        ps_print_err("ps_core_d is err\n");
        return BFGX_POWER_FAILED;
    }

    if (subsys == BFGX_IR) {
        mp17c_judge_ir_mode(ps_core_d);
        ps_print_info("ir mode[%d]\n", oal_atomic_read(&ps_core_d->ir_only));
    }

    board_baud_rate_set_mp17c(ps_core_d);
    ret = mp17c_board_power_on(sys);
    if (ret) {
        ps_print_err("mp17c_board_power_on bfg failed, ret=%d\n", ret);
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
    error = mp17c_bfgx_download_by_wifi(ps_core_d, sys);
    if (error != BFGX_POWER_SUCCESS) {
        goto bfgx_power_on_fail;
    }
#endif
    return BFGX_POWER_SUCCESS;

bfgx_power_on_fail:
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
    if (error != BFGX_POWER_DOWNLOAD_FIRMWARE_INTERRUPT) {
        hw_mpxx_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_MPXX_DOWNLOAD_FIRMWARE,
                                  "bcpu download firmware failed,wifi %s,ret=%d,process:%s, ssi_cfg_sts %d\n",
                                  wlan_is_shutdown() ? "off" : "on", error, current->comm,
                                  g_ssi_config_debug);
    }
#endif
    if (mpxx_firmware_download_mode() == MODE_COMBO) {
        (void)mp17c_board_power_off(B_SYS);
        (void)mp17c_board_power_off(G_SYS);
    } else {
        (void)mp17c_board_power_off(sys);
    }
    return error;
}

STATIC int32_t mp17c_bfgx_dev_power_off(struct ps_core_s *ps_core_d, uint32_t sys, uint32_t subsys)
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

    (void)mp17c_board_power_off(sys);

    if (subsys == BFGX_IR) {
        oal_atomic_set(&ps_core_d->ir_only, 0);
    }
    return BFGX_POWER_SUCCESS;
}

STATIC int32_t mp17c_wlan_firmware_download(void)
{
    int32_t ret;
    ret = mp17c_first_power_on_com_download();
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

STATIC int32_t mp17c_wlan_power_on(void)
{
    int32_t ret, error;
    struct wlan_pm_s *wlan_pm_info = wlan_pm_get_drv();

    error = mp17c_board_power_on(W_SYS);
    if (error != SUCC) {
        ps_print_err("board power on fail\n");
        goto wifi_power_fail;
    }

    hcc_bus_power_action(hcc_get_bus(HCC_EP_WIFI_DEV), HCC_BUS_POWER_PATCH_LOAD_PREPARE);

    error = mp17c_wlan_firmware_download();
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
        mp17c_board_error_handler(CHR_PLAT_DRV_ERROR_VMIN_WLAN_POWER_FAIL);
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
                                  "%s: failed to download firmware, bfgx %s, error=%d, ssi_cfg_sts %d\n",
                                  __FUNCTION__, bsle_is_shutdown() ? "off" : "on", error,
                                  g_ssi_config_debug);
    }
#endif
    return error;
}

STATIC int32_t mp17c_wlan_power_off(void)
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

    (void)mp17c_board_power_off(W_SYS);

    if (wlan_pm_info != NULL) {
        wlan_pm_info->wlan_power_state = POWER_STATE_SHUTDOWN;
    }
    return SUCCESS;
}

STATIC void mp17c_power_state_change(int32_t type, int32_t flag)
{
    if ((type == CHIP_POWR) && (flag == BOARD_POWER_ON)) {
        mp17c_bugfix_after_power_en();
        return;
    }

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

STATIC int32_t mp17c_board_power_reset(uint32_t ul_subsystem)
{
    int32_t ret;
    board_sys_disable(B_SYS);
    board_sys_disable(G_SYS);
#ifndef BFGX_UART_DOWNLOAD_SUPPORT
    board_sys_disable(W_SYS);
#endif
    board_chip_power_off();
    mp17c_tas_gpio_reset();
    board_chip_power_on();
#ifndef BFGX_UART_DOWNLOAD_SUPPORT
    ret = board_sys_enable(W_SYS);
#endif
    ret = board_sys_enable(B_SYS);
    ret = board_sys_enable(G_SYS);
    return ret;
}

/* 32bits address, 16bits value */
STATIC int32_t mp17c_ssi_write_word(uint32_t addr, uint16_t value)
{
#define WRITE_RETRY 3
    uint32_t retry = 0;
    uint32_t read_v;

    do {
        (void)ssi_write_word(addr, value);
        read_v = ssi_read_word(addr);
        if (value == (uint16_t)read_v) {
            ps_print_dbg("ssi write word: 0x%x=0x%x succ\n", addr, value);
            return BOARD_SUCC;
        }
        retry++;
    } while (retry < WRITE_RETRY);

    ps_print_err("ssi write word: 0x%x=0x%x, read=0x%x fail\n", addr, value, read_v);

    return BOARD_FAIL;
}

STATIC int32_t mp17c_board_ssi_config_ref_bg_pwr(void)
{
    int32_t ret;
    ret = mp17c_ssi_write_word(MP17C_LP_CTL_GLB_RB_LP_CTL_GLB_88_REG, LP_CTL_GLB_RB_D_WL_PMU_REF_BG_EN_MAN_MASK);
    if (ret != BOARD_SUCC) {
        ps_print_err("ssi write MP17C_LP_CTL_GLB_RB_LP_CTL_GLB_88_REG fail\n");
        return BOARD_FAIL;
    }
    ret = mp17c_ssi_write_word(MP17C_LP_CTL_GLB_RB_LP_CTL_GLB_75_REG, LP_CTL_GLB_RB_D_WL_PMU_REF_BG_EN_MAN_SEL_MASK);
    if (ret != BOARD_SUCC) {
        ps_print_err("ssi write MP17C_LP_CTL_GLB_RB_LP_CTL_GLB_75_REG fail\n");
        return BOARD_FAIL;
    }
    ret = mp17c_ssi_write_word(MP17C_LP_CTL_GLB_RB_LP_CTL_GLB_82_REG, LP_CTL_GLB_RB_D_BG_PMU_REF_BG_EN_MAN_MASK);
    if (ret != BOARD_SUCC) {
        ps_print_err("ssi write MP17C_LP_CTL_GLB_RB_LP_CTL_GLB_82_REG fail\n");
        return BOARD_FAIL;
    }
    ret = mp17c_ssi_write_word(MP17C_LP_CTL_GLB_RB_LP_CTL_GLB_73_REG, LP_CTL_GLB_RB_D_BG_PMU_REF_BG_EN_MAN_SEL_MASK);
    if (ret != BOARD_SUCC) {
        ps_print_err("ssi write MP17C_LP_CTL_GLB_RB_LP_CTL_GLB_73_REG fail\n");
        return BOARD_FAIL;
    }

    return BOARD_SUCC;
}

STATIC int32_t mp17c_ssi_config_with_32k(void)
{
    int32_t ret;

    ret = ssi_write16(gpio_ssi_reg(SSI_CTRL_AON_CRG_SEL),
        SSI_MASTER_CUSTOM_REGBANK_SSI_RST_A_CRG_SEL_MASK | SSI_MASTER_CUSTOM_REGBANK_SSI_RST_A_LGC_SEL_MASK);
    if (ret != BOARD_SUCC) {
        ps_print_err("ssi write SSI_CTRL_AON_CRG_SEL reg fail\n");
        return BOARD_FAIL;
    }
    ret = ssi_write16(gpio_ssi_reg(SSI_CTRL_AON_CRG_RST_EN_CLKEN),
        SSI_MASTER_CUSTOM_REGBANK_SSI_RST_A_CRG_N_MASK | SSI_MASTER_CUSTOM_REGBANK_SSI_RST_A_LGC_N_MASK);
    if (ret != BOARD_SUCC) {
        ps_print_err("ssi write SSI_CTRL_AON_CRG_RST_EN_CLKEN reg fail\n");
        return BOARD_FAIL;
    }
    oal_udelay(100);  // delay 100us确保aon解复位ok
    if (ssi_read_word(MP17C_GLBA_CTL_RB_GLB_CTL_SYS_CTL_ID_REG) != 0x101) {
        ps_print_err("ssi read fail, device is not alive\n");
        return BOARD_FAIL;
    }

    if (mp17c_board_ssi_config_ref_bg_pwr() != BOARD_SUCC) {
        return BOARD_FAIL;
    }

    if (ssi_read_word(MP17C_GLBA_CTL_RB_GLB_CTL_SYS_CTL_ID_REG) != 0x101) {
        ps_print_err("ssi read fail, device is not alive\n");
        return BOARD_FAIL;
    }

    return BOARD_SUCC;
}

STATIC int32_t mp17c_bugfix_by_ssi(void)
{
    int32_t ret;
    ktime_t start_time, end_time;
    unsigned long long count_time = 0;

    start_time = ktime_get();
    ret = ssi_write16(gpio_ssi_reg(SSI_AON_CLKSEL), 1);
    if (ret != BOARD_SUCC) {
        ps_print_err("ssi switch clk 32k fail\n");
        return BOARD_FAIL;
    }

    ret = mp17c_ssi_config_with_32k();
    if (ret != BOARD_SUCC) {
        ps_print_err("ssi config fail\n");
        (void)ssi_write16(gpio_ssi_reg(SSI_AON_CLKSEL), 0); // 确保失败后时钟切回
        return BOARD_FAIL;
    }

    ret = ssi_write16(gpio_ssi_reg(SSI_AON_CLKSEL), 0);
    if (ret != BOARD_SUCC) {
        ps_print_err("ssi switch to tcxo fail\n");
        return BOARD_FAIL;
    }

    end_time = ktime_get();
    count_time = (unsigned long long)ktime_to_us(ktime_sub(end_time, start_time));
    ps_print_info("ssi config succ count %llu us\n", count_time);
    return BOARD_SUCC;
}

STATIC void mp17c_ssi_config_fail_process(void)
{
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
    int32_t ret;
    char buf[DSM_CPU_INFO_SIZE] = { 0 };

    ret = snprintf_s(buf, DSM_CPU_INFO_SIZE, DSM_CPU_INFO_SIZE - 1,
                     "ssi config bg_en force on failed, try three times is also failed");
    if (ret > 0) {
        hw_mpxx_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_MPXX_HALT, "%s\n", buf);
    }
#endif
}

#ifdef HISI_NVRAM_SUPPORT
STATIC void mp17c_ssi_en_confg_by_nv_efuse(void)
{
    int32_t ret;
    uint32_t efuse_crc;
    struct external_nve_info_user info;
    uint32_t nv_data[NVRAM_EFUSE_BUFF] = {0};

    memset_s(&info, sizeof(struct external_nve_info_user), 0, sizeof(struct external_nve_info_user));

    if (strcpy_s(info.nv_name, sizeof(info.nv_name) - 1, CONN_NV_EFUSE_NAME) != EOK) {
        ps_print_err("efuse name copy fail");
        return;
    }

    info.nv_operation = NV_READ;
    info.valid_size = (uint32_t)sizeof(nv_data);
    info.nv_number = CONN_NV_EFUSE_ID;
    ret = external_nve_direct_access_interface(&info);
    if (ret != OAL_SUCC) {
        return;
    }

    ret = memcpy_s(nv_data, (uint32_t)sizeof(nv_data), info.nv_data,
                   (uint32_t)sizeof(nv_data));
    if (ret != EOK) {
        return;
    }

    ps_print_info("nv efuse data ver %u, vbg time %u, crc %u", nv_data[NVRAM_EFUSE_VER],
                  nv_data[NVRAM_EFUSE_VBG_TIME], nv_data[NVRAM_EFUSE_CRC]);
    efuse_crc = crc32c_conn_nve(CRC32C_REV_SEED, (uint8_t *)nv_data,
                                (uint32_t)(sizeof(uint32_t) + sizeof(uint32_t)));
    if (efuse_crc != nv_data[NVRAM_EFUSE_CRC]) {
        ps_print_info("nvram efuse crc check fail");
        return;
    }

    if (nv_data[NVRAM_EFUSE_VER] < MP17C_EFUSE_DET_VER) {
        return;
    }

    if (nv_data[NVRAM_EFUSE_VBG_TIME] >= MP17C_VBG_THRESH) {
        return;
    }

    ps_print_info("skip ssi config from eufse parameter\n");
    g_ssi_nv_supprot = false;
}
#endif

STATIC void mp17c_bugfix_after_power_en(void)
{
    uint32_t i;

    ps_print_info("bg_en force on config by ssi.\n");
    g_ssi_config_debug = SSI_CFG_SUCC;

    for (i = 0; ; i++) {
        conn_set_gpio_level(CHIP_POWR, GPIO_LEVEL_LOW);
        mdelay(10); /* delay 10ms */
        conn_set_gpio_level(CHIP_POWR, GPIO_LEVEL_HIGH);
        // 上电要等待efuse ready时间比较长， 需要等待60ms
        mdelay(60);

        // nv 中不需要通过ssi配置, ssi 不支持
        if ((g_ssi_nv_supprot != true) || (ssi_gpio_valid() != OAL_TRUE)) {
            g_ssi_config_debug = SSI_CFG_BYPASS;
            ps_print_info("ssi config skip, nv support %d\n", g_ssi_nv_supprot);
            return;
        }

        // 尝试3次 ssi写入都失败，直接返回
        if (i >= 3) {
            ps_print_err("ssi config fail.\n");
            mp17c_ssi_config_fail_process();
            g_ssi_config_debug = SSI_CFG_FAIL;
            return;
        }

        // 通过ssi写入
        if (mp17c_bugfix_by_ssi() == BOARD_SUCC) {
            return;
        }

        g_ssi_config_debug = SSI_CFG_TRY_SUCC;
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                             CHR_PLT_DRV_EVENT_OPEN, CHR_PLAT_DRV_ERROR_SSI_OPEN);
        ps_print_warning("ssi config bg_en force on failed, try again[%u].\n", i);
    }
}

STATIC void board_callback_init_mp17c_power(void)
{
    g_st_board_info.bd_ops.bfgx_dev_power_on = mp17c_bfgx_dev_power_on;
    g_st_board_info.bd_ops.bfgx_dev_power_off = mp17c_bfgx_dev_power_off;
    g_st_board_info.bd_ops.wlan_power_on = mp17c_wlan_power_on;
    g_st_board_info.bd_ops.wlan_power_off = mp17c_wlan_power_off;
    g_st_board_info.bd_ops.board_power_on = mp17c_board_power_on;
    g_st_board_info.bd_ops.board_power_off = mp17c_board_power_off;
    g_st_board_info.bd_ops.susbsys_is_shutdown = mp17c_susbsys_is_shutdown;
    g_st_board_info.bd_ops.power_state_change = mp17c_power_state_change;
    g_st_board_info.bd_ops.board_power_reset = mp17c_board_power_reset;
    g_st_board_info.bd_ops.board_power_notify = NULL;
    g_st_board_info.bd_ops.board_error_handler = mp17c_board_error_handler;
}

STATIC void board_get_cust_baudrate(plat_cust_data_mp17c *cust)
{
    struct device_node *np = NULL;
    uint32_t value;
    int32_t ret;

    cust->cfg.buart_rate = UART_BAUD_RATE_8M;
    cust->cfg.guart_rate = UART_BAUD_RATE_6M;

    ret = get_board_dts_node(&np, DTS_NODE_MPXX_BFGX);
    if (ret != BOARD_SUCC) {
        ps_print_err("DTS read node %s fail!!!\n", DTS_NODE_HISI_MPXX);
        return;
    }

    if (of_property_read_u32(np, DTS_PROP_MPXX_UART_RATE, &value) == 0) {
        cust->cfg.buart_rate = value;
    }

    ret = get_board_dts_node(&np, DTS_NODE_MPXX_ME);
    if (ret != BOARD_SUCC) {
        ps_print_err("DTS read node %s fail!!!\n", DTS_NODE_HISI_MPXX);
        return;
    }

    if (of_property_read_u32(np, DTS_PROP_MPXX_UART_RATE, &value) == 0) {
        cust->cfg.guart_rate = value;
    }
    ps_print_info("get uart rate, buart=%d\n", cust->cfg.buart_rate);
    ps_print_info("get uart rate, guart=%d\n", cust->cfg.guart_rate);
}


STATIC void set_slave_monitor_addr(struct device_node *np, uint32_t addr_num)
{
    int32_t ret;
    uint32_t i, addr_count, addr_h, addr_l;
    uint64_t monitor_addr;

    if (!of_property_read_bool(np, DTS_PROP_MPXX_SLV_MONITOR_ADDR)) {
        ps_print_info("DTS_PROP_MPXX_SLV_MONITOR_ADDR not exists!!!\n");
        return;
    }

    addr_count = (uint32_t)of_property_count_u32_elems(np, DTS_PROP_MPXX_SLV_MONITOR_ADDR);
    if ((addr_count < 0) | ((addr_count / 2) != addr_num)) { // 2 : 64bits addr need 2 u32
        ps_print_info("slv_monitor get addr fail");
        return;
    }

    for (i = 0; i < addr_count; i += 2) { // 2 : 64bits addr need 2 u32
        ret = of_property_read_u32_index(np, DTS_PROP_MPXX_SLV_MONITOR_ADDR, i, &addr_h);
        if (ret != BOARD_SUCC) {
            ps_print_err("of_property_read_u32_index %s %u fail!!!\n", DTS_PROP_MPXX_SLV_MONITOR_ADDR, i);
            break;
        }

        monitor_addr = (uint64_t)addr_h;
        ret = of_property_read_u32_index(np, DTS_PROP_MPXX_SLV_MONITOR_ADDR, i + 1, &addr_l);
        if (ret != BOARD_SUCC) {
            ps_print_err("of_property_read_u32_index %s %u fail!!!\n", DTS_PROP_MPXX_SLV_MONITOR_ADDR, i + 1);
            break;
        }

        monitor_addr = (monitor_addr << 32) | addr_l; // left shift 32 bit to address high bit
        ps_print_info("of_property_read_u32_index read 0x%llx", monitor_addr);
    }
}

STATIC int32_t check_is_suppor_slave_monitor(void)
{
    struct device_node *np = NULL;
    int32_t num_elems, ret;
    uint32_t addr_num = 0;
    uint32_t config;

    ret = get_board_dts_node(&np, DTS_NODE_MPXX_WIFI);
    if (ret != BOARD_SUCC) {
        ps_print_err("DTS read node %s fail!!!\n", DTS_NODE_MPXX_WIFI);
        return BOARD_FAIL;
    }

    if (!of_property_read_bool(np, DTS_PROP_MPXX_SLV_MONITOR_CONFIG)) {
        ps_print_info("DTS_PROP_MPXX_SLV_MONITOR_CONFIG not exists!!!\n");
        return BOARD_FAIL;
    }

    num_elems = of_property_count_u32_elems(np, DTS_PROP_MPXX_SLV_MONITOR_CONFIG);
    if (num_elems <= 0) {
        ps_print_info("of_property_count_u32_elems failed[%d]\n", num_elems);
        return BOARD_FAIL;
    }
    ps_print_info("slv_monitor num_elems[%d]\n", num_elems);

    ret = of_property_read_u32_index(np, DTS_PROP_MPXX_SLV_MONITOR_CONFIG, 0, &config);
    if (ret < 0) {
        ps_print_info("no slv monitor config\n");
        return BOARD_FAIL;
    }

    if (config == 0) {
        ps_print_info("slv_monitor not enable");
        return BOARD_FAIL;
    }
    ps_print_info("slv_monitor enable");

    ret = of_property_read_u32_index(np, DTS_PROP_MPXX_SLV_MONITOR_CONFIG, 1, &addr_num);
    if (ret < 0) {
        ps_print_info("no slv monitor addr\n");
        return BOARD_SUCC;
    }

    if (addr_num != 0) { // slv_monitor 地址下发
        set_slave_monitor_addr(np, addr_num);
    }

    return BOARD_SUCC;
}

STATIC void board_get_slave_monitor(plat_cust_data_mp17c *cust)
{
    int32_t cfg_value;
    int32_t ret;

    cust->cfg.slave_monitor.config = OAL_FALSE;

    ret = check_is_suppor_slave_monitor();
    if (ret != BOARD_SUCC) {
        ps_print_info("not support slave monitor\n");
        return;
    }

    /* 获取ini的配置值 */
    ret = get_cust_conf_int32(INI_MODU_PLAT, "pcie_slave_monitor_switch", &cfg_value);
    if (ret == INI_FAILED) {
        return;
    }

    cust->cfg.slave_monitor.config = (uint64_t)cfg_value;
    ps_print_info("get slave_monitor =%llu\n", cust->cfg.slave_monitor.config);
}

STATIC void board_get_pm_linkdown(plat_cust_data_mp17c *cust)
{
    int32_t cfg_value;
    int32_t ret;

    cust->cfg.pm_linkdown = OAL_FALSE;

    /* 获取ini的配置值 */
    ret = get_cust_conf_int32(INI_MODU_PLAT, "pcie_pm_linkdown_switch", &cfg_value);
    if (ret == INI_FAILED) {
        return;
    }

    cust->cfg.pm_linkdown = (uint32_t)cfg_value;
    ps_print_info("get pm_linkdown =%d\n", cust->cfg.pm_linkdown);
}

STATIC void board_plat_cust_init_mp17c(void)
{
    int32_t ret;
    plat_cust_data_mp17c plat_cust;

    memset_s(&plat_cust, sizeof(plat_cust), 0, sizeof(plat_cust));
#if defined(CONFIG_FACTORY_MODE) || defined(FACTORY_VERSION)
    plat_cust.cfg.vmin_mode = VMIN_MODE_FACTORY;
#else
    plat_cust.cfg.vmin_mode = VMIN_MODE_NORMAL;
#endif
    plat_cust.cfg.release_version = (hi110x_get_release_type() == MPXX_VTYPE_RELEASE) ? OAL_TRUE : OAL_FALSE;

    board_get_cust_baudrate(&plat_cust);
    board_get_slave_monitor(&plat_cust);
    board_get_pm_linkdown(&plat_cust);
    plat_cust.cfg.bg_tcxo_dll = 0; // bit0置1 开启b核倍频  bit1置1 开启g核倍频
    ret = set_plat_cust_buf((uint8_t *)&plat_cust, sizeof(plat_cust_data_mp17c));
    if (ret != SUCC) {
        ps_print_err("set plat cust buf fail, ret = %d\n", ret);
        return;
    }
    ps_print_info("set plat cust buf succ, vtype=%d\n", hi110x_get_release_type());
}

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
int32_t mp17c_firmware_cust(uint8_t *key, uint8_t *value)
{
    uint32_t dma_trans_switch;
    uint32_t tmp_dma_trans;
    hcc_bus *bus = hcc_get_bus(HCC_EP_WIFI_DEV);
    oal_pcie_res *pci_res = NULL;
    oal_pcie_linux_res *pst_pci_lres = NULL;

    if (key == NULL || value == NULL || bus == NULL) {
        return BFGX_POWER_FAILED;
    }

    if (g_st_board_info.wlan_download_channel == CHANNEL_SDIO) {
        return BOARD_SUCC;
    }

    dma_trans_switch = (uint32_t)simple_strtol(value, NULL, 10);  /* 将字符串转换成10进制数 */
    if (dma_trans_switch != 0 && dma_trans_switch != 1) {
        return BFGX_POWER_FAILED;
    }

    pst_pci_lres = (oal_pcie_linux_res *)bus->data;
    pci_res = pst_pci_lres->pst_pci_res;
    tmp_dma_trans = pci_res->chip_info.boot_cap.bits.dma_trans;
    // 将dma trans赋值成对应的参数
    pci_res->chip_info.boot_cap.bits.dma_trans = dma_trans_switch;

    return BOARD_SUCC;
}
#endif

#ifdef CONFIG_HI1102_PLAT_HW_CHR
static void chr_mp17c_wifi_pc_lr_reg_info(uint8_t* report, uint16_t len)
{
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
    chr_inner_mp17c_wifi_panic_info *data = NULL;
    int32_t ret;
    char buf[DSM_BUF_MAX_SIZE] = { 0 };
    if (len != sizeof(chr_inner_report_info) + sizeof(chr_inner_mp17c_wifi_panic_info)) {
        ps_print_err("%s mem overlap\n", __FUNCTION__);
        return;
    }
    data = (chr_inner_mp17c_wifi_panic_info*)(report + sizeof(chr_inner_report_info));

    ret = snprintf_s(buf, sizeof(buf), sizeof(buf) - 1, "wifi panic Ccause=0x%x Mcause=0x%x Mtval=0x%x PC=0x%x "\
        "LR=0x%x gp=0x%x mstatus=0x%x a3=0x%x a2=0x%x a1=0x%x a0=0x%x t2=0x%x t1=0x%x t0=0x%x clkpll_sts=0x%x",
        data->ccause, data->mcause, data->mtval, data->pc, data->lr, data->gp, data->mstatus,
        data->a3, data->a2, data->a1, data->a0, data->t2, data->t1, data->t0, data->clkpll_sts);
    if (ret > 0) {
        ps_print_dbg("wifi panic message %s to server\n", buf);
        hw_mpxx_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_MPXX_HALT, "%s\n", buf);
    } else {
        ps_print_err("wifi panic message to server fail, ret = %d\n", ret);
    }
#endif
}
#endif

int32_t board_info_init_mp17c(void)
{
    board_callback_init_dts();
    board_callback_init_mp17c_power();
    board_clk_init(DET_BY_WKUP_IO, CLK_INIT_INI);
    chip_firmware_cfg_register(g_mp17c_cfg_str, oal_array_size(g_mp17c_cfg_str));
    board_plat_cust_init_mp17c();
#ifdef HISI_NVRAM_SUPPORT
    mp17c_ssi_en_confg_by_nv_efuse();
#endif
#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
    set_firmware_cust(mp17c_firmware_cust);
#endif
#ifdef CONFIG_HI1102_PLAT_HW_CHR
    chr_cust_register(CHR_PLAT_INNER_ERROR_WIFI_PC_LR, chr_mp17c_wifi_pc_lr_reg_info);
#endif
    return BOARD_SUCC;
}
