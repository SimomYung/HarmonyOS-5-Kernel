/**
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved.
 *
 * Description: Implementation of the mp12 ssi host function.
 * Author: @CompanyNameTag
 */

#ifdef _PRE_CONFIG_GPIO_TO_SSI_DEBUG
#include "ssi_mp12.h"

#include "plat_debug.h"
#include "plat_pm.h"
#include "ssi_common.h"
#include "plat_parse_changid.h"
#include "ssi_spmi.h"
#include "oal_util.h"
#include "oam_dsm.h"
#include "oam_ext_if.h"
#include "chr_user.h"


#define BOARD_TCXO_38P4         0
#define BOARD_TCXO_76P8         0x10
#define TCXO_32K_DET_VALUE      10
#define TCXO_LIMIT_THRESHOLD    5
#define TCXO_38P4_NOMAL_CKL     38400000
#define TCXO_76P8_NOMAL_CKL     76800000
#define RTC_32K_NOMAL_CKL       32768
#define SPMI_BASE_ADDR          0x4021000

#define SSI_DUMP_LIMIT_TIMEOUT  4200000 // 超时时间定为4.2s 最低设置为 1s

/* hi6506 reg, reg_offset, default_value */
#define CHIP_VERSION_2                 0x2
#define CHIP_VERSION_2_DEFAULT_VALUE   0x30
/* IRQ MASK */
#define SCP0_MASK_REG                  0x5000
#define OCP0_MASK_REG                  0x5001
#define OCP1_MASK_REG                  0x5002
#define IRQ0_MASK_REG                  0x5003
#define OCP2_MASK_REG                  0X5004
/* IRQ */
#define SCP0_IRQ_REG                   0x6000
#define OCP0_IRQ_REG                   0x6001
#define OCP1_IRQ_REG                   0x6002
#define IRQ0_IRQ_REG                   0x6003
#define OCP2_IRQ_REG                   0X6004
/* NP_EVENT */
#define SCP0_NP_EVENT_REG              0x7000
#define OCP0_NP_EVENT_REG              0x7001
#define OCP1_NP_EVENT_REG              0x7002
#define IRQ0_NP_EVENT_REG              0x7003
#define OCP2_NP_EVENT_REG              0x7004
/* BUCK0/SYSLDO/CLDO2 VSET */
#define WLAN_W_BUCK0_VSET_REG          0x1040
#define WLAN_W_SYSLDO_VSET_REG         0x1042
#define WLAN_W_CLDO2_VSET_REG          0x1044

/* 以下寄存器是mp12 device定义 */
#define GLBA_CTL_RB_BASE                          0x4000000
#define GLBA_CTL_RB_G_SYS_STS_REG                 (GLBA_CTL_RB_BASE + 0x158)
#define GLBA_CTL_RB_W_SYS_STS_REG                 (GLBA_CTL_RB_BASE + 0x15C)
#define GLBA_CTL_RB_TCXO_DET_CTL_REG              (GLBA_CTL_RB_BASE + 0x550)    /* TCXO时钟检测控制寄存器 */
#define GLBA_CTL_RB_TCXO_32K_DET_CNT_REG          (GLBA_CTL_RB_BASE + 0x554)    /* TCXO时钟检测控制寄存器 */
#define GLBA_CTL_RB_TCXO_32K_DET_RESULT_REG       (GLBA_CTL_RB_BASE + 0x558)    /* TCXO时钟检测控制寄存器 */
#define GLBA_CTL_RB_PAD_DET_MODE_STS_REG          (GLBA_CTL_RB_BASE + 0x974)    /* 管脚检测数值 */
#define GLBA_CTL_RB_RST_STATUS_2_REG              (GLBA_CTL_RB_BASE + 0x1D24)

#define LP_CTL_GLB_RB_BASE    	                  0x4006000
#define LP_CTL_GLB_RB_LP_CTL_GLB_93_REG           (LP_CTL_GLB_RB_BASE + 0x1E8)    /* LP_CTL_GLB_93 */
#define LP_CTL_GLB_RB_LP_CTL_GLB_94_REG           (LP_CTL_GLB_RB_BASE + 0x1EC)    /* LP_CTL_GLB_94 */
#define LP_CTL_GLB_RB_LP_CTL_GLB_123_REG          (LP_CTL_GLB_RB_BASE + 0x260)  /* LP_CTL_GLB_123 */
#define LP_CTL_GLB_RB_LP_CTL_GLB_178_REG          (LP_CTL_GLB_RB_BASE + 0x364)  /* LP_CTL_GLB_178 */


#define GLB_CTL_W_RB_BASE    	                  0x4002000
#define GLB_CTL_W_RB_GLB_WCPU_LOAD_REG            (GLB_CTL_W_RB_BASE + 0x20)    /* WCPU_LOAD */
#define GLB_CTL_W_RB_GLB_WCPU_PC_L_REG            (GLB_CTL_W_RB_BASE + 0x24)    /* WCPU_PC低16bit */
#define GLB_CTL_W_RB_GLB_WCPU_PC_H_REG            (GLB_CTL_W_RB_BASE + 0x28)    /* WCPU_PC高16bit */
#define GLB_CTL_W_RB_GLB_WCPU_LR_L_REG            (GLB_CTL_W_RB_BASE + 0x2C)    /* WCPU_LR低16bit */
#define GLB_CTL_W_RB_GLB_WCPU_LR_H_REG            (GLB_CTL_W_RB_BASE + 0x30)    /* WCPU_LR高16bit */
#define GLB_CTL_W_RB_GLB_WCPU_SP_L_REG            (GLB_CTL_W_RB_BASE + 0x34)    /* WCPU_SP低16bit */
#define GLB_CTL_W_RB_GLB_WCPU_SP_H_REG            (GLB_CTL_W_RB_BASE + 0x38)    /* WCPU_SP高16bit */

#define GLB_CTL_G_RB_BASE    	                  0x4003000
#define GLB_CTL_G_RB_GLB_GCPU_LOAD_REG            (GLB_CTL_G_RB_BASE + 0x20)    /* GCPU_LOAD */
#define GLB_CTL_G_RB_GLB_GCPU_PC_L_REG            (GLB_CTL_G_RB_BASE + 0x24)    /* GCPU_PC低16bit */
#define GLB_CTL_G_RB_GLB_GCPU_PC_H_REG            (GLB_CTL_G_RB_BASE + 0x28)    /* GCPU_PC高16bit */
#define GLB_CTL_G_RB_GLB_GCPU_LR_L_REG            (GLB_CTL_G_RB_BASE + 0x2C)    /* GCPU_LR低16bit */
#define GLB_CTL_G_RB_GLB_GCPU_LR_H_REG            (GLB_CTL_G_RB_BASE + 0x30)    /* GCPU_LR高16bit */
#define GLB_CTL_G_RB_GLB_GCPU_SP_L_REG            (GLB_CTL_G_RB_BASE + 0x34)    /* GCPU_SP低16bit */
#define GLB_CTL_G_RB_GLB_GCPU_SP_H_REG            (GLB_CTL_G_RB_BASE + 0x38)    /* GCPU_SP高16bit */

#define PINMUX_RB_BASE    	                      0x4029400
#define PINMUX_RB_CFG_SPMI_CLK_MODE_REG           (PINMUX_RB_BASE + 0xB0)    /* SPMI_CLK */
#define PINMUX_RB_CFG_SPMI_DATA_MODE_REG          (PINMUX_RB_BASE + 0xB4)    /* SPMI_DATA */

#define W_CTL_RB_BASE    	                      0x41B5000
#define W_CTL_RB_W_CTL_SYS_TICK_CFG_REG           (W_CTL_RB_BASE + 0x614)
#define W_CTL_RB_W_CTL_SYS_TICK_VALUE_0_REG       (W_CTL_RB_BASE + 0x618)

#define G_CTL_RB_BASE                             0x4300000
#define G_CTL_RB_G_SYSTICK_CNT_CFG_REG            (G_CTL_RB_BASE + 0x320)
#define G_CTL_RB_G_SYSTICK_VALUE_0_REG            (G_CTL_RB_BASE + 0x324)

#define GLBA_CTL_RB_CUR_STS_MASK                    0x7
#define LP_CTL_GLB_RB_IRQ_FROM_PMIC_GRM_MASK        0x1
#define LP_CTL_GLB_RB_IRQ_FROM_PMIC_GRM_1STICK_MASK 0x1
#define LP_CTL_GLB_RB_XO_DET_STS_MASK               0x1
#define LP_CTL_GLB_RB_XO_DET_VAL_MASK               0x1
#define GLBA_CTL_RB_FREQ_DET_MASK                   0x10

#define W_CTL_RB_W_SYS_TICK_STZ_MASK                0x2
#define G_CTL_RB_G_SYS_TICK_STZ_MASK                0x2

#define CTL_WCPU_PC_LR_LOAD_EN                      0x81
#define CTL_WCPU_PC_LR_CLR_EN                       0x40
#define CTL_GCPU_PC_LR_LOAD_EN                      0x81
#define CTL_GCPU_PC_LR_CLR_EN                       0x40

#define CPU_TRACE_NUM 2
#define GLB_CTL_W_RB_CFG_CPU_TRACE_ENABLE_W_REG 0x40026C0
#define LP_CTL_W_RB_LP_CTL_W_CFG_REG 0x4007568
#define GLB_CTL_G_RB_CFG_CPU_TRACE_ENABLE_BG_REG 0x4004928
#define LP_CTL_BG_RB_LP_CTL_BG_CFG_REG 0x4009568
#define SAMPLE_EN_MASK 0xFFFD
#define DIAG_EN_MASK 0xFFFE
#define DEF_MASK 0x2

/* dump 寄存器定义 */
static ssi_reg_info g_ssi_master_reg_full = { 0x40, 0x0, 0x30, SSI_RW_SSI_MOD };

static ssi_reg_info g_glba_ctrl = { 0x4000000, 0x0, 0x1044, SSI_RW_WORD_MOD };
static ssi_reg_info g_glba_ctrl_extend1 = { 0x4001204, 0x0, 0x94, SSI_RW_WORD_MOD };
static ssi_reg_info g_glba_ctrl_extend2 = { 0x4001400, 0x0, 0x38, SSI_RW_WORD_MOD };
static ssi_reg_info g_glba_ctrl_extend3 = { 0x40015FC, 0x0, 0x94, SSI_RW_WORD_MOD };
static ssi_reg_info g_glba_ctrl_extend4 = { 0x4001A04, 0x0, 0x24C, SSI_RW_WORD_MOD };
static ssi_reg_info g_glba_ctrl_extend5 = { 0x4001D00, 0x0, 0x58, SSI_RW_WORD_MOD };
static ssi_reg_info g_glba_ctrl_extend6 = { 0x4001FB0, 0x0, 0x4C, SSI_RW_WORD_MOD };

static ssi_reg_info g_glb_w_ctrl = { 0x4002000, 0x0, 0x748,   SSI_RW_WORD_MOD };

static ssi_reg_info g_glb_g_ctrl = { 0x4003000, 0x0, 0x240,   SSI_RW_WORD_MOD };
static ssi_reg_info g_glb_g_ctrl_extend1 = { 0x4003514, 0x0, 0x3C,   SSI_RW_WORD_MOD };
static ssi_reg_info g_glb_g_ctrl_extend2 = { 0x40037BC, 0x0, 0x8C,   SSI_RW_WORD_MOD };
static ssi_reg_info g_glb_g_ctrl_extend3 = { 0x4003900, 0x0, 0x2C,   SSI_RW_WORD_MOD };
static ssi_reg_info g_glb_g_ctrl_extend4 = { 0x4003A00, 0x0, 0x84,   SSI_RW_WORD_MOD };

static ssi_reg_info g_lp_ctrl = { 0x4006000, 0x0, 0xA74,  SSI_RW_WORD_MOD };
static ssi_reg_info g_lp_w_ctrl = { 0x4007000, 0x0, 0x3C4,  SSI_RW_WORD_MOD };
static ssi_reg_info g_lp_w_ctrl_extend1 = { 0x4007568, 0x0, 0x134,  SSI_RW_WORD_MOD };

static ssi_reg_info g_lp_g_ctrl = { 0x4009000, 0x0, 0x3C4,  SSI_RW_WORD_MOD };
static ssi_reg_info g_lp_g_ctrl_extend1 = { 0x4009500, 0x0, 0x188,  SSI_RW_WORD_MOD };

static ssi_reg_info g_lp_com_ctrl = { 0x400a000, 0x0, 0x328,  SSI_RW_WORD_MOD };
static ssi_reg_info g_lp_com_ctrl_extend1 = { 0x400a644, 0x0, 0x14,  SSI_RW_WORD_MOD };

static ssi_reg_info g_pmu2_cmu_ctrl_part1 = { 0x401A000, 0x0, 0x918,  SSI_RW_WORD_MOD };
static ssi_reg_info g_pmu2_cmu_ctrl_part2 = { 0x401AC00, 0x0, 0x1AC,  SSI_RW_WORD_MOD };
static ssi_reg_info g_pmu2_cmu_ctrl_part3 = { 0x401AF00, 0x0, 0x2C,  SSI_RW_WORD_MOD };

static ssi_reg_info g_coex_ctl = { 0x4019000, 0x0, 0x9D8,  SSI_RW_WORD_MOD };
static ssi_reg_info g_xoadc_ctrl_part1 = { 0x4020000, 0x0, 0x20,  SSI_RW_WORD_MOD };
static ssi_reg_info g_xoadc_ctrl_part2 = { 0x4020200, 0x0, 0x20,  SSI_RW_WORD_MOD };
static ssi_reg_info g_xoadc_ctrl_part3 = { 0x4020600, 0x0, 0x68,  SSI_RW_WORD_MOD };
static ssi_reg_info g_xoadc_ctrl_part4 = { 0x4020860, 0x0, 0x8,  SSI_RW_WORD_MOD };
static ssi_reg_info g_xoadc_ctrl_part5 = { 0x4020a78, 0x0, 0x2c,  SSI_RW_WORD_MOD };
static ssi_reg_info g_mipi_ctrl = { 0x4025000, 0x0, 0xa0,  SSI_RW_WORD_MOD };
static ssi_reg_info g_gpio0_full = { 0x400e000, 0x0, 0x208, SSI_RW_DWORD_MOD };
static ssi_reg_info g_gpio1_full = { 0x400f000, 0x0, 0x208, SSI_RW_DWORD_MOD };
// 目前sharmem的有效长度 g_fix_sharemem_end - g_fix_sharemem_start
static ssi_reg_info g_sharemem_fix_pos = { 0x04040000, 0x0, 0x210, SSI_RW_DWORD_MOD };

static ssi_reg_info g_w_ctrl_part1 = { 0x41B5000, 0x0, 0x125C,  SSI_RW_WORD_MOD };
static ssi_reg_info g_bgi_ctrl_full = { 0x4300000, 0x0, 0x620,  SSI_RW_WORD_MOD };
static ssi_reg_info g_pcie_ctrl_full = { 0x04903000, 0x0, 0x7a8,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_host_ctrl_full = { 0x04905000, 0x0, 0xf80,  SSI_RW_DWORD_MOD };

static ssi_reg_info g_w_boot_dbg_full = { 0x4015d0, 0x0, 0x2C,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_bgi_boot_dbg_full = { 0x24af568, 0x0, 0x2C,  SSI_RW_DWORD_MOD };

static ssi_reg_info g_w_cpu_trace = { 0x04196000, 0x0, 0x2000, SSI_RW_DWORD_MOD };
static ssi_reg_info g_bgi_cpu_trace = { 0x04326000, 0x0, 0x2000, SSI_RW_DWORD_MOD };

static ssi_reg_info g_wlan_exception_mesg = { 0x805de00, 0x0, 0xB4,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_bgicpu_exception_mesg = { 0x2411150, 0x0, 0x90,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_bgicpu_os_dfx_mesg = { 0x24111e0, 0x0, 0x408,  SSI_RW_DWORD_MOD };

static ssi_reg_info g_wcpu_ram_log_mesg = { 0x8001000, 0x0, 0x1220,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_bgicpu_ram_log_mesg = { 0x2400000, 0x4680, 0x23c0,  SSI_RW_DWORD_MOD };

static ssi_reg_info g_wcpu_stack_mesg = { 0x400000, 0x0, 0x3000, SSI_RW_DWORD_MOD };
static ssi_reg_info g_bgicpu_stack_mesg = { 0x2400000, 0x0, 0x1800, SSI_RW_DWORD_MOD };

static ssi_reg_info *g_wcpu_reg_full[] = {
    &g_w_ctrl_part1,
    &g_w_boot_dbg_full
};

static ssi_reg_info *g_bgicpu_reg_full[] = {
    &g_bgi_ctrl_full,
    &g_bgi_boot_dbg_full
};

/* 默认dump 所有寄存器 */
static ssi_reg_info *g_aon_reg_full[] = {
    &g_ssi_master_reg_full,
    &g_glba_ctrl,
    &g_glba_ctrl_extend1,
    &g_glba_ctrl_extend2,
    &g_glba_ctrl_extend3,
    &g_glba_ctrl_extend4,
    &g_glba_ctrl_extend5,
    &g_glba_ctrl_extend6,
    &g_glb_w_ctrl,
    &g_glb_g_ctrl,
    &g_glb_g_ctrl_extend1,
    &g_glb_g_ctrl_extend2,
    &g_glb_g_ctrl_extend3,
    &g_glb_g_ctrl_extend4,
    &g_lp_ctrl,
    &g_lp_w_ctrl,
    &g_lp_w_ctrl_extend1,
    &g_lp_g_ctrl,
    &g_lp_g_ctrl_extend1,
    &g_lp_com_ctrl,
    &g_lp_com_ctrl_extend1,
    &g_pmu2_cmu_ctrl_part1,
    &g_pmu2_cmu_ctrl_part2,
    &g_pmu2_cmu_ctrl_part3,
    &g_coex_ctl,
    &g_xoadc_ctrl_part1,
    &g_xoadc_ctrl_part2,
    &g_xoadc_ctrl_part3,
    &g_xoadc_ctrl_part4,
    &g_xoadc_ctrl_part5,
    &g_mipi_ctrl
};

static ssi_reg_info *g_cpu_trace_full[] = {
    &g_w_cpu_trace,
    &g_bgi_cpu_trace
};

static uint32_t g_irq_from_pmu;

enum cpu {
    WCPU,
    BGICPU,
    CPU_BUFF
};

enum cpu_addr {
    TRACE_PC,
    TRACE_LR,
    TRACE_SP,
    TRACE_BUFF
};

struct trace_part_str {
    char *part;
    uint32_t addr_low;
    uint32_t addr_high;
    uint32_t value[SSI_CPU_ARM_REG_DUMP_CNT];
};

typedef struct {
    char *name;
    uint32_t cpu_state;
    uint32_t cpu_load_addr;
    uint32_t reg_flag[SSI_CPU_ARM_REG_DUMP_CNT];
    struct  trace_part_str trace_part[TRACE_BUFF];
} ssi_cpu_trace_struct;

typedef struct {
    uint32_t cpu_trace_en_addr;
    uint32_t cpu_trace_en_value[CPU_TRACE_NUM];
    uint32_t cpu_trace_clk_addr;
    uint32_t cpu_trace_clk_value;
} ssi_cpu_trace_info;

STATIC ssi_cpu_trace_struct g_ssi_cpu_trace[CPU_BUFF] = {
    {
        "wcpu",
        0x0,
        GLB_CTL_W_RB_GLB_WCPU_LOAD_REG,
        { 0x0, 0x0 },
        {
            {
                "pc",
                GLB_CTL_W_RB_GLB_WCPU_PC_L_REG,
                GLB_CTL_W_RB_GLB_WCPU_PC_H_REG,
                { 0x0, 0x0 }
            },
            {
                "lr",
                GLB_CTL_W_RB_GLB_WCPU_LR_L_REG,
                GLB_CTL_W_RB_GLB_WCPU_LR_H_REG,
                { 0x0, 0x0 }
            },
            {
                "sp",
                GLB_CTL_W_RB_GLB_WCPU_SP_L_REG,
                GLB_CTL_W_RB_GLB_WCPU_SP_H_REG,
                { 0x0, 0x0 }
            }
        }
    },
    {
        "bgicpu",
        0x0,
        GLB_CTL_G_RB_GLB_GCPU_LOAD_REG,
        { 0x0, 0x0 },
        {
            {
                "pc",
                GLB_CTL_G_RB_GLB_GCPU_PC_L_REG,
                GLB_CTL_G_RB_GLB_GCPU_PC_H_REG,
                { 0x0, 0x0 }
            },
            {
                "lr",
                GLB_CTL_G_RB_GLB_GCPU_LR_L_REG,
                GLB_CTL_G_RB_GLB_GCPU_LR_H_REG,
                { 0x0, 0x0 }
            },
            {
                "sp",
                GLB_CTL_G_RB_GLB_GCPU_SP_L_REG,
                GLB_CTL_G_RB_GLB_GCPU_SP_H_REG,
                { 0x0, 0x0 }
            }
        }
    },
};

STATIC ssi_cpu_trace_info g_cpu_trace[CPU_BUFF] = {
    { GLB_CTL_W_RB_CFG_CPU_TRACE_ENABLE_W_REG,
      { SAMPLE_EN_MASK, DIAG_EN_MASK },
      LP_CTL_W_RB_LP_CTL_W_CFG_REG,
      DEF_MASK,
    },
    { GLB_CTL_G_RB_CFG_CPU_TRACE_ENABLE_BG_REG,
      { SAMPLE_EN_MASK, DIAG_EN_MASK },
      LP_CTL_BG_RB_LP_CTL_BG_CFG_REG,
      DEF_MASK,
    },
};

int mp12_ssi_check_subsys_is_work(void)
{
    uint16_t w_cur_sts = ssi_read16(gpio_ssi_reg(SSI_SSI_RPT_STS_31));
    uint16_t b_cur_sts = ssi_read16(gpio_ssi_reg(SSI_SSI_RPT_STS_32));
    uint16_t g_cur_sts = ssi_read16(gpio_ssi_reg(SSI_SSI_RPT_STS_33));
    uint16_t com_cur_sts = ssi_read16(gpio_ssi_reg(SSI_SSI_RPT_STS_33));

    ssi_read_master_regs(&g_ssi_master_reg_full, NULL, 0, g_ssi_is_logfile);

    /* w_cur_sts/b_cur_sts/g_cur_sts中的bit[12:10]有一个为0x3, 表示TCXO在位 */
    if (((w_cur_sts & 0x1C00) == 0xC00) || ((b_cur_sts & 0x70) == 0x30) ||
        ((g_cur_sts & 0x7) == 0x3) || ((com_cur_sts & 0x1C00) == 0xC00)) {
        ps_print_info("ssi get work sys: w_cur_sts = 0x%x, b_cur_sts = 0x%x, g_cur_sts = 0x%x, com_cur_sts = 0x%x\n",
                      w_cur_sts, b_cur_sts, g_cur_sts, com_cur_sts);
        return BOARD_SUCC;
    }

    ps_print_info("ssi check all subsys is not working\n");
    return BOARD_FAIL;
}

static void dsm_cpu_info_dump(void)
{
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
    int32_t ret;
    char buf[DSM_CPU_INFO_SIZE] = { 0 };
    /* dsm cpu信息上报 */
    if (g_halt_det_cnt || g_mpxx_kernel_crash) {
        ps_print_info("g_halt_det_cnt=%u g_mpxx_kernel_crash=%d dsm_cpu_info_dump return\n",
                      g_halt_det_cnt, g_mpxx_kernel_crash);
        return;
    }

    ret = snprintf_s(buf, DSM_CPU_INFO_SIZE, DSM_CPU_INFO_SIZE - 1,
        "wcpu:0x%08x 0x%08x 0x%08x 0x%08x s:%1d bgicpu:0x%08x 0x%08x 0x%08x 0x%08x s:%1d "\
        "bgicpu:0x%08x 0x%08x 0x%08x 0x%08x s:%1d sle:0x%08x 0x%08x 0x%08x 0x%08x s:%1d pmu:%1d",
        (uint32_t)g_ssi_cpu_trace[WCPU].trace_part[TRACE_PC].value[0],
        (uint32_t)g_ssi_cpu_trace[WCPU].trace_part[TRACE_LR].value[0],
        (uint32_t)g_ssi_cpu_trace[WCPU].trace_part[TRACE_PC].value[1],
        (uint32_t)g_ssi_cpu_trace[WCPU].trace_part[TRACE_LR].value[1],
        (uint8_t)g_ssi_cpu_trace[WCPU].cpu_state,

        (uint32_t)g_ssi_cpu_trace[BGICPU].trace_part[TRACE_PC].value[0],
        (uint32_t)g_ssi_cpu_trace[BGICPU].trace_part[TRACE_LR].value[0],
        (uint32_t)g_ssi_cpu_trace[BGICPU].trace_part[TRACE_PC].value[1],
        (uint32_t)g_ssi_cpu_trace[BGICPU].trace_part[TRACE_LR].value[1],
        (uint8_t)g_ssi_cpu_trace[BGICPU].cpu_state,
        (uint8_t)g_irq_from_pmu);
    if (ret > 0) {
        hw_mpxx_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_MPXX_HALT, "%s\n", buf);
    } else {
        hw_mpxx_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_MPXX_HALT, "device panic, ssi dump fail[%d]\n", ret);
    }
#endif
}

#define W_EN_HISTORY_LOW (BIT8 | BIT4)
#define B_EN_HISTORY_LOW (BIT9 | BIT5)
#define G_EN_HISTORY_LOW (BIT10 | BIT6)
#define C_EN_HISTORY_LOW (BIT11 | BIT7)
static void ssi_check_subsys_power_en_status(void)
{
    uint32_t reg;
    reg = (uint32_t)ssi_read32(GLBA_CTL_RB_RST_STATUS_2_REG);
    if (((reg & W_EN_HISTORY_LOW) == W_EN_HISTORY_LOW) || ((reg & B_EN_HISTORY_LOW) == B_EN_HISTORY_LOW) ||
        ((reg & G_EN_HISTORY_LOW) == G_EN_HISTORY_LOW) || ((reg & C_EN_HISTORY_LOW) == C_EN_HISTORY_LOW)) {
            chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                                 CHR_PLT_DRV_EVENT_GPIO, CHR_PLAT_DRV_ERROR_SUBSYS_POWER_GPIO);
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
            hw_mpxx_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_MPXX_HALT,
                                      "power_en_status[0x%x]: w_en [%s], b_en [%s], g_en [%s], c_en[%s]", reg,
                                      ((reg & W_EN_HISTORY_LOW) ==  W_EN_HISTORY_LOW) ? "abnormal" : "normal",
                                      ((reg & B_EN_HISTORY_LOW) ==  B_EN_HISTORY_LOW) ? "abnormal" : "normal",
                                      ((reg & G_EN_HISTORY_LOW) ==  G_EN_HISTORY_LOW) ? "abnormal" : "normal",
                                      ((reg & C_EN_HISTORY_LOW) ==  C_EN_HISTORY_LOW) ? "abnormal" : "normal");
#endif
    }

    ps_print_info("power_en_status[0x%x]: w_en [%s], b_en [%s], g_en [%s], c_en[%s]", reg,
                  ((reg & W_EN_HISTORY_LOW) ==  W_EN_HISTORY_LOW) ? "abnormal" : "normal",
                  ((reg & B_EN_HISTORY_LOW) ==  B_EN_HISTORY_LOW) ? "abnormal" : "normal",
                  ((reg & G_EN_HISTORY_LOW) ==  G_EN_HISTORY_LOW) ? "abnormal" : "normal",
                  ((reg & C_EN_HISTORY_LOW) ==  C_EN_HISTORY_LOW) ? "abnormal" : "normal");
}

static int ssi_check_cpu_is_working(int sys)
{
    int i;
    uint32_t reg;
    uint32_t mask = 0;
    uint32_t cpu_status[][2] = {
        { WCPU,   GLBA_CTL_RB_W_SYS_STS_REG },
        { BGICPU,   GLBA_CTL_RB_G_SYS_STS_REG },
    };
    reg = (uint32_t)ssi_read32(cpu_status[sys][1]);
    ps_print_info("cpu state=0x%x\n", reg);
    for (i = WCPU; i < CPU_BUFF; i++) {
        if (i == sys) {
            mask = reg & GLBA_CTL_RB_CUR_STS_MASK; // 2 is bit offset
            g_ssi_cpu_trace[i].cpu_state = mask;
            ps_print_info("%s is %s\n", g_ssi_cpu_trace[i].name, g_ssi_cpu_st_str[mask]);
        }
    }

    return (mask == 0x3); /* 0x3:cpu is working */
}

static void ssi_read_pc_lr_sp(int sys)
{
    int i, j;
    uint32_t reg_low, reg_high;
    for (i = 0; i < SSI_CPU_ARM_REG_DUMP_CNT; i++) {
        for (j = 0; j < TRACE_BUFF; j++) {
            if ((g_ssi_cpu_trace[sys].trace_part[j].addr_low & 0xFFFFFFFF) != 0) {
                reg_low = (uint32_t)ssi_read32(g_ssi_cpu_trace[sys].trace_part[j].addr_low);
            } else {
                reg_low = 0;
            }
            if ((g_ssi_cpu_trace[sys].trace_part[j].addr_high & 0xFFFFFFFF) != 0) {
                reg_high = (uint32_t)ssi_read32(g_ssi_cpu_trace[sys].trace_part[j].addr_high);
            } else {
                reg_high = 0;
            }
            g_ssi_cpu_trace[sys].trace_part[j].value[i] = reg_low | (reg_high << 16); // offset 16bit
        }

        ps_print_info("gpio-ssi:load_sts:read %s[%d], pc:0x%x, lr:0x%x, sp:0x%x \n", g_ssi_cpu_trace[sys].name, i,
                      g_ssi_cpu_trace[sys].trace_part[TRACE_PC].value[i],
                      g_ssi_cpu_trace[sys].trace_part[TRACE_LR].value[i],
                      g_ssi_cpu_trace[sys].trace_part[TRACE_SP].value[i]);

        if (g_ssi_cpu_trace[sys].trace_part[TRACE_PC].value[i] == 0) {
            ps_print_info("%s pc all zero\n", g_ssi_cpu_trace[sys].name);
        } else {
            g_ssi_cpu_trace[sys].reg_flag[i] = 1;
        }
        oal_mdelay(10); /* delay 10 ms */
    }
}

int mp12_ssi_read_pc_lr_sp(void)
{
    int i;
    uint32_t load;
    uint32_t ctl_cpu_pc_lrload[][3] = { /* Number of columns in the array is 3 */
        { WCPU,   CTL_WCPU_PC_LR_LOAD_EN, CTL_WCPU_PC_LR_CLR_EN },
        { BGICPU,   CTL_GCPU_PC_LR_LOAD_EN, CTL_GCPU_PC_LR_CLR_EN },
    };

    for (i = WCPU; i < CPU_BUFF; i++) {
        if (ssi_check_cpu_is_working(i) == OAL_FALSE) {
            continue;
        }

        ssi_write32(g_ssi_cpu_trace[i].cpu_load_addr, ctl_cpu_pc_lrload[i][1]);
        oal_mdelay(2); /* delay 2 ms */

        load = (uint32_t)ssi_read32(g_ssi_cpu_trace[i].cpu_load_addr);
        ps_print_info("gpio-ssi:load_sts:load[0x%x] \n", load);

        ssi_read_pc_lr_sp(i);

        // pc锁存 清除使能位:bit1
        ssi_write32(g_ssi_cpu_trace[i].cpu_load_addr, ctl_cpu_pc_lrload[i][2]); // 2 is trce_en
        oal_mdelay(1);
    }
    return 0;
}

static void ssi_dump_gpio_regs(void)
{
    int ret;

    ps_print_info("wifi wkup host gpio level: %d\n", conn_get_gpio_level(W_WKUP_HOST));
    ps_print_info("bt wkup host gpio level: %d\n", conn_get_gpio_level(BFGX_WKUP_HOST));
    ps_print_info("sle wkup host gpio level: %d\n", conn_get_gpio_level(G_WKUP_HOST));
    ret = ssi_read_reg_info(&g_gpio0_full, NULL, 0, g_ssi_is_logfile);
    if (ret) {
        ps_print_err("ssi dump gpio0 failed, ret = %d\n", ret);
    }
    ret = ssi_read_reg_info(&g_gpio1_full, NULL, 0, g_ssi_is_logfile);
    if (ret) {
        ps_print_err("ssi dump gpio1 failed, ret = %d\n", ret);
    }
}

void mp12_ssi_dump_gpio(void)
{
    ssi_switch_clk(SSI_AON_CLKSEL_SSI);
    ssi_dump_gpio_regs();
    ssi_switch_clk(SSI_AON_CLKSEL_TCXO);
}

static int ssi_dump_device_aon_regs(unsigned long long module_set)
{
    int ret;
    if (module_set & SSI_MODULE_MASK_AON) {
        ret = ssi_read_reg_info_arry(g_aon_reg_full, sizeof(g_aon_reg_full) / sizeof(ssi_reg_info *),
                                     g_ssi_is_logfile);
        if (ret) {
            return -OAL_EFAIL;
        }
    }

    ssi_dump_gpio_regs();

    return OAL_SUCC;
}

static int ssi_dump_device_arm_regs(unsigned long long module_set)
{
    int ret;
    if (module_set & SSI_MODULE_MASK_ARM_REG) {
        ret = mp12_ssi_read_pc_lr_sp();
        if (ret) {
            goto ssi_fail;
        }
    }

    return 0;
ssi_fail:
    return ret;
}

static void ssi_dump_pmu_regs(unsigned long long module_set)
{
    uint32_t pmu1_status_grm;
    uint32_t pmu1_status_grm_stick;

    g_irq_from_pmu = 0;
    /* try to read pmu regs */
    if ((module_set & SSI_MODULE_MASK_PMU) == 0x0) {
        return;
    }

    pmu1_status_grm = (uint32_t)ssi_read32(LP_CTL_GLB_RB_LP_CTL_GLB_93_REG);
    if (pmu1_status_grm < 0) {
        return;
    }

    pmu1_status_grm_stick = (uint32_t)ssi_read32(LP_CTL_GLB_RB_LP_CTL_GLB_94_REG);
    if (pmu1_status_grm_stick < 0) {
        return;
    }

    if (((pmu1_status_grm & LP_CTL_GLB_RB_IRQ_FROM_PMIC_GRM_MASK) ==
        LP_CTL_GLB_RB_IRQ_FROM_PMIC_GRM_MASK) ||
        ((pmu1_status_grm_stick & LP_CTL_GLB_RB_IRQ_FROM_PMIC_GRM_1STICK_MASK) ==
         LP_CTL_GLB_RB_IRQ_FROM_PMIC_GRM_1STICK_MASK)) {
        g_irq_from_pmu = 1;
    }
}

static void ssi_trigger_tcxo_detect(uint32_t *tcxo_det_value_target)
{
    if ((*tcxo_det_value_target) == 0) {
        ps_print_err("tcxo_det_value_target is zero, trigger failed\n");
        return;
    }

    /* 刚做过detect,改变det_value+2,观测值是否改变 */
    if ((*tcxo_det_value_target) == ssi_read32(GLBA_CTL_RB_TCXO_32K_DET_CNT_REG)) {
        (*tcxo_det_value_target) = (*tcxo_det_value_target) << 1;
    }

    ssi_write32(GLBA_CTL_RB_TCXO_32K_DET_CNT_REG, (*tcxo_det_value_target)); /* 设置计数周期 */
    ssi_write32(GLBA_CTL_RB_TCXO_DET_CTL_REG, 0x0);                          /* tcxo_det_en disable */

    /* to tcxo */
    ssi_switch_clk(SSI_AON_CLKSEL_TCXO);
    /* delay 150 us */
    oal_udelay(150);
    /* to ssi */
    ssi_switch_clk(SSI_AON_CLKSEL_SSI);

    ssi_write32(GLBA_CTL_RB_TCXO_DET_CTL_REG, 0x1); /* tcxo_det_en enable */

    /* to tcxo */
    ssi_switch_clk(SSI_AON_CLKSEL_TCXO);
    /* delay 31 * 2 * tcxo_det_value_taget us.wait detect done,根据设置的计数周期数 */
    oal_udelay(31 * (*tcxo_det_value_target) * 2);
    /* to ssi */
    ssi_switch_clk(SSI_AON_CLKSEL_SSI);
}

static int ssi_detect_32k_handle(uint32_t sys_tick_old, uint64_t cost_time_s, uint32_t *clock_32k)
{
    uint32_t base_32k_clock = RTC_32K_NOMAL_CKL;
    uint32_t sys_tick_new;

    if (ssi_check_cpu_is_working(WCPU) == OAL_TRUE) {
        sys_tick_new = (uint32_t)ssi_read32(W_CTL_RB_W_CTL_SYS_TICK_VALUE_0_REG);
    } else {
        sys_tick_new = (uint32_t)ssi_read32(G_CTL_RB_G_SYSTICK_VALUE_0_REG);
    }

    if (sys_tick_new == sys_tick_old) {
        ps_print_err("32k sys_tick don't change after detect, 32k maybe abnormal, sys_tick=0x%x\n", sys_tick_new);
        return -OAL_EFAIL;
    } else {
        cost_time_s += 1446; /* 经验值,误差1446us */
        (*clock_32k) = (sys_tick_new * 1000) / (uint32_t)cost_time_s; /* 1000 is mean hz to KHz */
        ps_print_info("32k runtime:%llu us, sys_tick:%u\n", cost_time_s, sys_tick_new);
        ps_print_info("32k real= %u Khz[base=%u]\n", (*clock_32k), base_32k_clock);
    }
    return 0;
}

static uint16_t tcxo_freq_det(void)
{
    if ((ssi_read32(GLBA_CTL_RB_PAD_DET_MODE_STS_REG) & GLBA_CTL_RB_FREQ_DET_MASK) == BOARD_TCXO_76P8) {
        ps_print_info("board tcxo freq is 76.8M\n");
        return BOARD_TCXO_76P8;
    }
    ps_print_info("board tcxo freq is 38.4M\n");
    return BOARD_TCXO_38P4;
}

static int ssi_detect_tcxo_handle(uint32_t tcxo_det_res_old, uint32_t tcxo_det_value_target, uint32_t clock_32k)
{
    uint64_t base_tcxo_clock = TCXO_38P4_NOMAL_CKL;
    uint32_t base_32k_clock = RTC_32K_NOMAL_CKL;
    uint64_t clock_tcxo, div_clock;
    uint64_t tcxo_limit_low, tcxo_limit_high, tcxo_tmp;
    uint32_t tcxo_det_res_new = (uint32_t)ssi_read32(GLBA_CTL_RB_TCXO_32K_DET_RESULT_REG);
    if (tcxo_det_res_new == tcxo_det_res_old) {
        ps_print_err("tcxo don't change after detect, tcxo or 32k maybe abnormal, tcxo_count=0x%x, 32k_clock=%u\n",
                     tcxo_det_res_new, clock_32k);
        return -OAL_EFAIL;
    }

    if (tcxo_det_value_target == 0) {
        ps_print_err("tcxo_det_value_target is zero\n");
        return -OAL_EFAIL;
    }

    if (tcxo_freq_det() == BOARD_TCXO_76P8) {
        base_tcxo_clock = TCXO_76P8_NOMAL_CKL;
        tcxo_det_res_new *= 2;         /* 2：板级晶振如果是76.8M。soc会做2分频处理，因此将soc统计tcxo_cnt乘2，计算板级晶振频率 */
    }
    /* 计算TCXO时钟误差范围 */
    tcxo_tmp = div_u64(base_tcxo_clock, 100); /* 100 is Percentage */
    tcxo_limit_low = (tcxo_tmp * (100 - TCXO_LIMIT_THRESHOLD)); /* TCXO Lower threshold 100 - 5 */
    tcxo_limit_high = (tcxo_tmp * (100 + TCXO_LIMIT_THRESHOLD)); /* TCXO Upper threshold 100 + 5 */

    /* 计算tcxo实际时钟 */
    clock_tcxo = (uint64_t)((tcxo_det_res_new * base_32k_clock) / (tcxo_det_value_target));
    div_clock = clock_tcxo;
    div_clock = div_u64(div_clock, 1000000); /* 1000000 is unit conversion hz to Mhz */
    if ((clock_tcxo < tcxo_limit_low) || (clock_tcxo > tcxo_limit_high)) {
        /* 时钟误差超过阈值 */
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
        hw_mpxx_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_MPXX_TCXO_ERROR,
            "tcxo real=%llu hz,%llu Mhz[base=%llu][limit:%llu~%llu]\n",
            clock_tcxo, div_clock, base_tcxo_clock, tcxo_limit_low, tcxo_limit_high);
#endif
        ps_print_err("tcxo real=%llu hz,%llu Mhz[base=%llu][limit:%llu~%llu]\n",
                     clock_tcxo, div_clock, base_tcxo_clock, tcxo_limit_low, tcxo_limit_high);
        return -OAL_EFAIL;
    }

    ps_print_info("tcxo real=%llu hz,%llu Mhz[base=%llu][limit:%llu~%llu]\n",
                  clock_tcxo, div_clock, base_tcxo_clock, tcxo_limit_low, tcxo_limit_high);
    return 0;
}

static uint32_t ssi_clr_systick(void)
{
    uint32_t sys_tick_old;
    if (ssi_check_cpu_is_working(WCPU) == OAL_TRUE) {
        sys_tick_old = (uint32_t)ssi_read32(W_CTL_RB_W_CTL_SYS_TICK_VALUE_0_REG);
        ssi_write32(W_CTL_RB_W_CTL_SYS_TICK_CFG_REG, W_CTL_RB_W_SYS_TICK_STZ_MASK); /* 清零w systick */
    } else {
        sys_tick_old = (uint32_t)ssi_read32(G_CTL_RB_G_SYSTICK_VALUE_0_REG);
        ssi_write32(G_CTL_RB_G_SYSTICK_CNT_CFG_REG, G_CTL_RB_G_SYS_TICK_STZ_MASK); /* 清零gf systick */
    }
    return sys_tick_old;
}

static int ssi_detect_tcxo_is_normal(void)
{
    /*
     * tcxo detect依赖tcxo和32k时钟
     * 如果在启动后tcxo异常, 那么tcxo_det_res为旧值
     * 如果在启动后32k异常, 那么sys_tick为旧值
     */
    int ret;
    uint32_t tcxo_det_sts, tcxo_det_val, clock_32k;
    uint32_t sys_tick_old, tcxo_det_res_old;
    uint32_t tcxo_det_value_target = TCXO_32K_DET_VALUE;
    uint64_t us_to_s;

    declare_time_cost_stru(cost);
    if (mp12_ssi_check_subsys_is_work() == BOARD_FAIL) {
        ps_print_err("All subsys are not work!\n");
        return -OAL_EFAIL;
    }
    /* 检测TCXO时钟是否在位 */
    tcxo_det_val = (uint32_t)ssi_read32(LP_CTL_GLB_RB_LP_CTL_GLB_178_REG);
    tcxo_det_sts = (uint32_t)ssi_read32(LP_CTL_GLB_RB_LP_CTL_GLB_123_REG);
    if ((tcxo_det_val & LP_CTL_GLB_RB_XO_DET_VAL_MASK) == LP_CTL_GLB_RB_XO_DET_VAL_MASK) {
        if ((tcxo_det_sts & LP_CTL_GLB_RB_XO_DET_STS_MASK) == LP_CTL_GLB_RB_XO_DET_STS_MASK) {
            ps_print_info("tcxo is present after detect\n");
        } else {
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
            hw_mpxx_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_MPXX_TCXO_ERROR,
                "tcxo sts:0x%x abnormal, not present after detect\n", tcxo_det_sts);
#endif
            ps_print_err("tcxo sts:0x%x abnormal, not present after detect\n", tcxo_det_sts);
            return -OAL_EFAIL;
        }
    } else {
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
            hw_mpxx_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_MPXX_TCXO_ERROR,
                "tcxo val:0x%x abnormal, invalid detect result\n", tcxo_det_val);
#endif
        ps_print_err("tcxo val:0x%x abnormal, invalid detect result\n", tcxo_det_val);
        return -OAL_EFAIL;
    }

    /* 检测TCXO时钟精度是否准确 */
    tcxo_det_res_old = (uint32_t)ssi_read32(GLBA_CTL_RB_TCXO_32K_DET_RESULT_REG);
    sys_tick_old = ssi_clr_systick();

    oal_get_time_cost_start(cost);

    /* 使能TCXO时钟精度检测 */
    ssi_trigger_tcxo_detect(&tcxo_det_value_target);

    oal_udelay(1000); /* delay 1000 us,wait 32k count more */
    oal_get_time_cost_end(cost);
    oal_calc_time_cost_sub(cost);

    /* 32k detect by system clock */
    us_to_s = time_cost_var_sub(cost);
    ret = ssi_detect_32k_handle(sys_tick_old, us_to_s, &clock_32k);
    if (ret) {
        return ret;
    }

    /* tcxo detect by 32k clock */
    return ssi_detect_tcxo_handle(tcxo_det_res_old, tcxo_det_value_target, clock_32k);
}

static void ssi_dump_device_tcxo_regs(unsigned long long module_set)
{
    int ret;
    if (module_set & (SSI_MODULE_MASK_AON | SSI_MODULE_MASK_AON_CUT)) {
        ret = ssi_detect_tcxo_is_normal();
        if (ret) {
            ps_print_info("tcxo detect failed, continue dump\n");
        }
    }
}

static void ssi_check_en_state(unsigned long long module_set)
{
    int i;
    uint32_t mask;
    uint32_t gpio_type[CPU_BUFF] = {W_POWER, BFGX_POWER};

    if (!(module_set & SSI_MODULE_MASK_ARM_REG)) {
        return;
    }
    for (i = WCPU; i < CPU_BUFF; i++) {
        mask = g_ssi_cpu_trace[i].cpu_state;
        ps_print_info("gpio_type[%d] is %d, cpu_state is %d\n", gpio_type[i], conn_get_gpio_level(gpio_type[i]), mask);
        // 超级隐私模块断开en信号通路，导致g_en拉高无效，当host侧拉高en，cpu仍然处于off状态上报异常
        if (conn_get_gpio_level(gpio_type[i]) == GPIO_HIGHLEVEL && mask == 0x0) {
            chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                                 CHR_PLT_DRV_EVENT_GPIO, CHR_PLAT_DRV_ERROR_SUBSYS_POWER_GPIO);
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
            hw_mpxx_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_MPXX_HALT,
                                      "host set gpio_type[%d] is high, but %s is OFF", gpio_type[i],
                                      g_ssi_cpu_trace[i].name);
#endif
        }
    }
    ssi_check_subsys_power_en_status();
}

static void ssi_dump_sharemem_fix(void)
{
    int ret;
    /* 仅debug版本, 通过ssi获取share mem结果 */
    if (is_hi110x_debug_type() != OAL_TRUE) {
        return;
    }

    if (ssi_check_device_isalive() != 0) {
        ps_print_info("device is not alive\n");
        return;
    }

    ps_print_info("sharemem fix mem read start\n");
    ret = ssi_read_reg_info(&g_sharemem_fix_pos, NULL, 0, g_ssi_is_logfile);
    if (ret != 0) {
        ps_print_info("sharemem fix mem read failed\n");
        return;
    }
    ps_print_info("sharemem fix trace mem read end\n");
}

static void ssi_dump_cpu_trace(unsigned long long module_set)
{
    int i;
    int ret;
    uint32_t load;
    /* 仅debug版本, 通过ssi获取cpu trace结果 */
    if (is_hi110x_debug_type() != OAL_TRUE) {
        return;
    }

    if (ssi_check_device_isalive() != 0) {
        ps_print_info("device is not alive\n");
        return;
    }

    ps_print_info("cpu trace read start\n");
    for (i = WCPU; i < CPU_BUFF; i++) {
        ps_print_info("dump %s start.\n", g_ssi_cpu_trace[i].name);
        if (ssi_check_cpu_is_working(i) == OAL_FALSE) {
            ps_print_info("%s is not work.\n", g_ssi_cpu_trace[i].name);
            continue;
        }

        // cpu trace stop
        load = (uint32_t)ssi_read32(g_cpu_trace[i].cpu_trace_en_addr);
        ssi_write32(g_cpu_trace[i].cpu_trace_en_addr, (uint16_t)(load & g_cpu_trace[i].cpu_trace_en_value[1]));
        ssi_write32(g_cpu_trace[i].cpu_trace_en_addr, (uint16_t)(load & g_cpu_trace[i].cpu_trace_en_value[0]));
        load = (uint32_t)ssi_read32(g_cpu_trace[i].cpu_trace_clk_addr);
        ssi_write32(g_cpu_trace[i].cpu_trace_clk_addr, (uint16_t)(load | g_cpu_trace[i].cpu_trace_clk_value));

        // dump cpu trace
        ret = ssi_read_reg_info(g_cpu_trace_full[i], NULL, 0, g_ssi_is_logfile);
        if (ret != 0) {
            ps_print_info("cpu trace read failed\n");
            return;
        }
        ps_print_info("dump %s end.\n", g_ssi_cpu_trace[i].name);
    }
    ps_print_info("cpu trace read read end\n");
}

static int regs_dump(unsigned long long module_set)
{
    int ret;

    /* try to read pc&lr&sp regs */
    ret = ssi_dump_device_arm_regs(module_set);
    if (ret) {
        return ret;
    }

    ret = ssi_check_device_isalive();
    if (ret) {
        return ret;
    }

    ssi_dump_cpu_trace(module_set);

    /* try to read all aon regs */
    ret = ssi_dump_device_aon_regs(module_set);
    if (ret != OAL_SUCC) {
        return ret;
    }

    /* try to read pc&lr&sp regs again */
    ret = ssi_dump_device_arm_regs(module_set);
    if (ret) {
        return ret;
    }
    ssi_check_en_state(module_set);

    /* detect tcxo clock is normal, trigger */
    ssi_dump_device_tcxo_regs(module_set);

    ssi_dump_sharemem_fix();
    return OAL_SUCC;
}

static int ssi_dump_device_wctrl_regs(unsigned long long module_set)
{
    int ret;

    if (module_set & SSI_MODULE_MASK_WCTRL) {
        if (ssi_check_cpu_is_working(WCPU) == OAL_TRUE) {
            ret = ssi_read_reg_info_arry(g_wcpu_reg_full, sizeof(g_wcpu_reg_full) /
                                         sizeof(ssi_reg_info *), g_ssi_is_logfile);
            if (ret) {
                return ret;
            }
        } else {
            ps_print_info("wctrl can't dump, wcpu down\n");
        }
    }
    return 0;
}

static int ssi_dump_device_pciectrl_regs(unsigned long long module_set)
{
    int ret;
    if (ssi_check_cpu_is_working(WCPU) == OAL_TRUE) {
        if (module_set & SSI_MODULE_MASK_PCIE_CFG) {
            ret = ssi_read_reg_info(&g_pcie_ctrl_full, NULL, 0, g_ssi_is_logfile);
            if (ret) {
                return ret;
            }
            ret = ssi_read_reg_info(&g_host_ctrl_full, NULL, 0, g_ssi_is_logfile);
            if (ret) {
                return ret;
            }
        }
    } else {
        ps_print_info("pcie_ctrl can't dump, wcpu down\n");
    }

    return 0;
}

static int ssi_dump_device_bgictrl_regs(unsigned long long module_set)
{
    int ret;

    if (module_set & SSI_MODULE_MASK_GFCTRL) {
        if (ssi_check_cpu_is_working(BGICPU) == OAL_TRUE) {
            ret = ssi_read_reg_info_arry(g_bgicpu_reg_full, sizeof(g_bgicpu_reg_full) /
                                         sizeof(ssi_reg_info *), g_ssi_is_logfile);
            if (ret) {
                return ret;
            }
        } else {
            ps_print_info("gfctrl can't dump, gfcpu down\n");
        }
    }
    return 0;
}

static int ssi_dump_device_reg(unsigned long long module_set)
{
    int ret;

    ret = ssi_dump_device_wctrl_regs(module_set);
    if (ret != OAL_SUCC) {
        return ret;
    }

    ret = ssi_dump_device_pciectrl_regs(module_set);
    if (ret != OAL_SUCC) {
        return ret;
    }

    ret = ssi_dump_device_bgictrl_regs(module_set);
    if (ret != OAL_SUCC) {
        return ret;
    }

    return ret;
}

static int32_t ssi_check_valid(unsigned long long *reg_set)
{
    int32_t ret;

    ret = ssi_check_device_isalive();
    if (ret) {
        /* try to reset aon */
        ssi_force_reset_reg();
        ssi_switch_clk(SSI_AON_CLKSEL_SSI);
        if (ssi_check_device_isalive() < 0) {
            ps_print_info("after reset aon, ssi still can't work\n");
            return OAL_FAIL;
        } else {
            ps_print_info("after reset aon, ssi ok\n");
            *reg_set = SSI_MODULE_MASK_COMM;
        }
    }

    return OAL_SUCC;
}

static void ssi_dump_device_wcpu_exception_mem(void)
{
    int ret;
    ps_print_info("wcpu exception dump mem start\n");
    if (ssi_check_cpu_is_working(WCPU) == OAL_TRUE) {
        ret = ssi_read_reg_info(&g_wlan_exception_mesg, NULL, 0, g_ssi_is_logfile);
        if (ret != 0) {
            ps_print_info("wcpu exception mem read failed\n");
        }
    } else {
        ps_print_info("wcpu exception mem can't dump, wcpu down\n");
    }
}

static void ssi_dump_device_wcpu_stack_mem(void)
{
    int ret;
    ps_print_info("wcpu stack dump mem start\n");
    if (ssi_check_cpu_is_working(WCPU) == OAL_TRUE) {
        ret = ssi_read_reg_info(&g_wcpu_stack_mesg, NULL, 0, g_ssi_is_logfile);
        if (ret != 0) {
            ps_print_info("wcpu stack mem read failed\n");
        }
    } else {
        ps_print_info("wcpu stack mem can't dump, wcpu down\n");
    }
}

static void ssi_dump_device_bgicpu_exception_mem(void)
{
    int ret;
    ps_print_info("bgicpu exception dump mem start\n");
    if (ssi_check_cpu_is_working(BGICPU) == OAL_TRUE) {
        ret = ssi_read_reg_info(&g_bgicpu_exception_mesg, NULL, 0, g_ssi_is_logfile);
        if (ret != 0) {
            ps_print_info("bgicpu exception mem read failed\n");
        }
    } else {
        ps_print_info("bgicpu exception mem can't dump, bgicpu down\n");
    }
}

static void ssi_dump_device_bgicpu_os_dfx_mem(void)
{
    int ret;
    ps_print_info("bgicpu os_dfx dump mem start\n");
    if (ssi_check_cpu_is_working(BGICPU) == OAL_TRUE) {
        ret = ssi_read_reg_info(&g_bgicpu_os_dfx_mesg, NULL, 0, g_ssi_is_logfile);
        if (ret != 0) {
            ps_print_info("bgicpu os_dfx mem read failed\n");
        }
    } else {
        ps_print_info("bgicpu os_dfx mem can't dump, bgicpu down\n");
    }
}

static void ssi_dump_device_bgicpu_stack_mem(void)
{
    int ret;
    ps_print_info("bgicpu stack dump mem start\n");
    if (ssi_check_cpu_is_working(BGICPU) == OAL_TRUE) {
        ret = ssi_read_reg_info(&g_bgicpu_stack_mesg, NULL, 0, g_ssi_is_logfile);
        if (ret != 0) {
            ps_print_info("bgicpu stack mem read failed\n");
        }
    } else {
        ps_print_info("bgicpu stack mem can't dump, wcpu down\n");
    }
}

static void ssi_dump_device_stack_mem(unsigned long long module_set)
{
    if (module_set & SSI_MODULE_MASK_WCPU_DEVICE_LOG) {
        ssi_dump_device_wcpu_stack_mem();
    }

    if (module_set & (SSI_MODULE_MASK_BCPU_DEVICE_LOG | SSI_MODULE_MASK_OFF_FIND_LOG)) {
        ssi_dump_device_bgicpu_stack_mem();
    }
}

static void ssi_dump_bgicpu_ram_log_mem(void)
{
    int ret;
    uint32_t base_addr = 0x2400000;
    uint32_t head_offset = 0x6a44;
    uint32_t tail_offset = 0x6a48;

    // 关机找回日志
    uint32_t allow_sleep_addr = 0x400917c;
    uint32_t soft_wakeup_addr = 0x4000164;

    ssi_switch_clk(SSI_AON_CLKSEL_SSI);
    ssi_write_dword(allow_sleep_addr, 0);
    oal_mdelay(10); // 等待10ms生效
    ssi_write_dword(soft_wakeup_addr, 0x8); // 软中断int3唤醒
    ssi_switch_clk(SSI_AON_CLKSEL_TCXO);

    if (ssi_check_cpu_is_working(BGICPU) == OAL_TRUE) {
        uint32_t head, tail;
        head = (uint32_t)ssi_read32((base_addr + head_offset));
        tail = (uint32_t)ssi_read32((base_addr + tail_offset));
        ps_print_info("logbuf head: 0x%x, tail: 0x%x\n", head, tail);
        ps_print_info("bgicpu ram log dump mem start\n");
        ret = ssi_read_reg_info(&g_bgicpu_ram_log_mesg, NULL, 0, g_ssi_is_logfile);
        if (ret != 0) {
            ps_print_info("bgicpu ram log mem read failed\n");
        }
        ps_print_info("bgicpu ram log dump mem end\n");
    } else {
        ps_print_info("bgicpu ram log mem can't dump, bgicpu down\n");
    }
}

static void ssi_dump_wcpu_ram_log_mem(void)
{
    int ret;

    if (ssi_check_cpu_is_working(WCPU) == OAL_TRUE) {
        ps_print_info("wcpu ram log dump mem start\n");
        ret = ssi_read_reg_info(&g_wcpu_ram_log_mesg, NULL, 0, g_ssi_is_logfile);
        if (ret != 0) {
            ps_print_info("wcpu ram log mem read failed\n");
        }
        ps_print_info("wcpu ram log dump mem end\n");
    } else {
        ps_print_info("wcpu ram log mem can't dump, wcpu down\n");
    }
}

static void ssi_dump_device_oam_log(unsigned long long module_set)
{
    if ((bool)(module_set & SSI_MODULE_MASK_WCPU_DEVICE_LOG)) {
        ssi_dump_wcpu_ram_log_mem();
    }

    if ((bool)(module_set & (SSI_MODULE_MASK_BCPU_DEVICE_LOG | SSI_MODULE_MASK_OFF_FIND_LOG))) {
        ssi_dump_bgicpu_ram_log_mem();
    }
}

static void ssi_dump_riscv_exception_msg(unsigned long long module_set)
{
    ssi_dump_device_wcpu_exception_mem();
    ssi_dump_device_bgicpu_exception_mem();
    ssi_dump_device_bgicpu_os_dfx_mem();
    ssi_dump_device_stack_mem(module_set);
    ssi_dump_device_oam_log(module_set);
}

static int mp12_ssi_device_regs_dump(unsigned long long module_set)
{
    int ret;
    unsigned long long reg_set = module_set;
    declare_time_cost_stru(ssi_timestamp);

    ssi_read16(gpio_ssi_reg(SSI_SSI_CTRL));
    ssi_read16(gpio_ssi_reg(SSI_SEL_CTRL));

    ssi_check_tcxo_is_available();

    oal_get_time_cost_start(ssi_timestamp);
    ret = ssi_check_valid(&reg_set);
    if (ret != OAL_SUCC) {
        goto ssi_fail;
    }

    ret = regs_dump(reg_set);
    if (ret != OAL_SUCC) {
        goto ssi_fail;
    }

    ret = ssi_dump_device_reg(reg_set);
    if (ret != OAL_SUCC) {
        goto ssi_fail;
    }
    ssi_dump_riscv_exception_msg(module_set);
    oal_get_time_cost_end(ssi_timestamp);
    oal_calc_time_cost_sub(ssi_timestamp);
    ps_print_info("ssi dump cost %llu us\n", time_cost_var_sub(ssi_timestamp));

    oal_get_time_cost_end(ssi_timestamp);
    oal_calc_time_cost_sub(ssi_timestamp);

    /* try to read pmu regs */
    ssi_dump_pmu_regs(reg_set);

ssi_fail:
    ssi_switch_clk(SSI_AON_CLKSEL_TCXO);
    dsm_cpu_info_dump();
    return ret;
}

int mp12_ssi_config(unsigned long long module_set)
{
    unsigned long long temp_module_set;

    // 不支持ssi导出 panic动作
    temp_module_set = module_set & (SSI_MODULE_MASK_TRG_B_PANIC | SSI_MODULE_MASK_TRG_G_PANIC);
    if (temp_module_set != 0) {
        temp_module_set = (module_set & (~temp_module_set));

        if (temp_module_set == 0) {
            return -OAL_EFAIL;
        }

        ps_print_err("ssi config %llu mix with panic mask\n", module_set);
    }

    return mp12_ssi_device_regs_dump(module_set);
}
#endif /* #ifdef _PRE_CONFIG_GPIO_TO_SSI_DEBUG */
