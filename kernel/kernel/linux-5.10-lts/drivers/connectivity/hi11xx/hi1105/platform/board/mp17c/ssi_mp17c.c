/**
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved.
 *
 * Description: Implementation of the mp17c ssi host function.
 * Author: @CompanyNameTag
 */

#ifdef _PRE_CONFIG_GPIO_TO_SSI_DEBUG
#include "ssi_mp17c.h"

#include "plat_debug.h"
#include "plat_pm.h"
#include "ssi_common.h"
#include "plat_parse_changid.h"
#include "ssi_spmi.h"
#include "oal_util.h"
#include "oam_dsm.h"
#include "oam_ext_if.h"
#include "bfgx_dev.h"
#include "chr_user.h"
#include "board_mp17c.h"


#undef THIS_FILE_ID
#define THIS_FILE_ID  OAM_FILE_ID_SSI_MP17C_C

uint32_t g_dump_dtcm = 0;
oal_debug_module_param(g_dump_dtcm, uint, S_IRUGO | S_IWUSR);

#define W_DUMP_DTCM_MSK (1 << 0)
#define B_DUMP_DTCM_MSK (1 << 1)
#define G_DUMP_DTCM_MSK (1 << 2)
#define C_DUMP_DTCM_MSK (1 << 3)

#define BOARD_TCXO_38P4         0
#define BOARD_TCXO_76P8         0x10
#define TCXO_32K_DET_VALUE      10
#define TCXO_LIMIT_THRESHOLD    5
#define TCXO_38P4_NOMAL_CKL     38400000
#define TCXO_76P8_NOMAL_CKL     76800000
#define RTC_32K_NOMAL_CKL       32768
#define SPMI_BASE_ADDR          0x4021000

#define SSI_DUMP_LIMIT_TIMEOUT_USER   4200000 // 超时时间定为4.2s 最低设置为 1s 商用导ssi时间长会导致死机
#define SSI_DUMP_LIMIT_TIMEOUT_DEGUG  15000000 // debug 模式允许导ssi为15 s

/* hi6506 reg, reg_offset, default_value */
#define CHIP_VERSION_0                 0x0
#define CHIP_VERSION_0_DEFAULT_VALUE   0x36
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

/* 以下寄存器是mp17c device定义 */
#define GLBA_CTL_RB_BASE                          0x4000000
#define GLBA_CTL_RB_B_SYS_STS_REG                 (GLBA_CTL_RB_BASE + 0x150)
#define GLBA_CTL_RB_COM_SYS_STS_REG               (GLBA_CTL_RB_BASE + 0x154)
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
#define GLB_CTL_W_RB_GLB_WCPU_SP_H_REG            (GLB_CTL_W_RB_BASE + 0x38)    /* WCPU_SP高16bit */
#define GLB_CTL_W_RB_HOST_CRG_SOFT_RST_W_REG      (GLB_CTL_W_RB_BASE + 0x118)   /* HOST_CRG_SOFT_RST_W */

#define GLB_CTL_W_RB_SOFT_RST_MASK                0x1F


#define GLB_CTL_B_RB_BASE    	                  0x4003000
#define GLB_CTL_B_RB_GLB_BCPU_LOAD_REG            (GLB_CTL_B_RB_BASE + 0x20)    /* BCPU_LOAD */
#define GLB_CTL_B_RB_GLB_BCPU_PC_L_REG            (GLB_CTL_B_RB_BASE + 0x24)    /* BCPU_PC低16bit */
#define GLB_CTL_B_RB_GLB_BCPU_PC_H_REG            (GLB_CTL_B_RB_BASE + 0x28)    /* BCPU_PC高16bit */
#define GLB_CTL_B_RB_GLB_BCPU_LR_L_REG            (GLB_CTL_B_RB_BASE + 0x2C)    /* BCPU_LR低16bit */
#define GLB_CTL_B_RB_GLB_BCPU_LR_H_REG            (GLB_CTL_B_RB_BASE + 0x30)    /* BCPU_LR高16bit */
#define GLB_CTL_B_RB_GLB_BCPU_SP_L_REG            (GLB_CTL_B_RB_BASE + 0x34)    /* BCPU_SP低16bit */
#define GLB_CTL_B_RB_GLB_BCPU_SP_H_REG            (GLB_CTL_B_RB_BASE + 0x38)    /* BCPU_SP高16bit */

#define GLB_CTL_G_RB_BASE    	                  0x4004000
#define GLB_CTL_G_RB_GLB_GCPU_LOAD_REG            (GLB_CTL_G_RB_BASE + 0x20)    /* GCPU_LOAD */
#define GLB_CTL_G_RB_GLB_GCPU_PC_L_REG            (GLB_CTL_G_RB_BASE + 0x24)    /* GCPU_PC低16bit */
#define GLB_CTL_G_RB_GLB_GCPU_PC_H_REG            (GLB_CTL_G_RB_BASE + 0x28)    /* GCPU_PC高16bit */
#define GLB_CTL_G_RB_GLB_GCPU_LR_L_REG            (GLB_CTL_G_RB_BASE + 0x2C)    /* GCPU_LR低16bit */
#define GLB_CTL_G_RB_GLB_GCPU_LR_H_REG            (GLB_CTL_G_RB_BASE + 0x30)    /* GCPU_LR高16bit */
#define GLB_CTL_G_RB_GLB_GCPU_SP_L_REG            (GLB_CTL_G_RB_BASE + 0x34)    /* GCPU_SP低16bit */
#define GLB_CTL_G_RB_GLB_GCPU_SP_H_REG            (GLB_CTL_G_RB_BASE + 0x38)    /* GCPU_SP高16bit */

#define GLB_CTL_COM_RB_BASE                       0x4005000
#define GLB_CTL_COM_RB_GLB_COMCPU_LOAD_REG        (GLB_CTL_COM_RB_BASE + 0x20)    /* COMCPU_LOAD */
#define GLB_CTL_COM_RB_GLB_COMCPU_PC_L_REG        (GLB_CTL_COM_RB_BASE + 0x24)    /* COMCPU_PC低16bit */
#define GLB_CTL_COM_RB_GLB_COMCPU_PC_H_REG        (GLB_CTL_COM_RB_BASE + 0x28)    /* COMCPU_PC高16bit */
#define GLB_CTL_COM_RB_GLB_COMCPU_LR_L_REG        (GLB_CTL_COM_RB_BASE + 0x2C)    /* COMCPU_LR低16bit */
#define GLB_CTL_COM_RB_GLB_COMCPU_LR_H_REG        (GLB_CTL_COM_RB_BASE + 0x30)    /* COMCPU_LR高16bit */
#define GLB_CTL_COM_RB_GLB_COMCPU_SP_L_REG        (GLB_CTL_COM_RB_BASE + 0x34)    /* COMCPU_SP低16bit */
#define GLB_CTL_COM_RB_GLB_COMCPU_SP_H_REG        (GLB_CTL_COM_RB_BASE + 0x38)    /* COMCPU_SP高16bit */

#define PINMUX_RB_BASE    	                      0x4029400
#define PINMUX_RB_CFG_SPMI_CLK_MODE_REG           (PINMUX_RB_BASE + 0xB0)    /* SPMI_CLK */
#define PINMUX_RB_CFG_SPMI_DATA_MODE_REG          (PINMUX_RB_BASE + 0xB4)    /* SPMI_DATA */

#define AON_CTL_RB_BASE    	                      0x4000000
#define AON_CTL_SYS_TICK_CFG                      (AON_CTL_RB_BASE + 0x1290)
#define AON_CTL_SYS_TICK_VALUE                    (AON_CTL_RB_BASE + 0x1294)

#define GLBA_CTL_RB_CUR_STS_MASK                    0x7
#define LP_CTL_GLB_RB_IRQ_FROM_PMIC_GRM_MASK        0x1
#define LP_CTL_GLB_RB_IRQ_FROM_PMIC_GRM_1STICK_MASK 0x1
#define LP_CTL_GLB_RB_XO_DET_STS_MASK               0x1
#define LP_CTL_GLB_RB_XO_DET_VAL_MASK               0x1
#define GLBA_CTL_RB_FREQ_DET_MASK                   0x10

#define AON_CTL_SYS_TICK_STZ_MASK                   0x2

#define CTL_WCPU_PC_LR_LOAD_EN                      0x81
#define CTL_WCPU_PC_LR_CLR_EN                       0x40
#define CTL_BCPU_PC_LR_LOAD_EN                      0x101
#define CTL_BCPU_PC_LR_CLR_EN                       0x80
#define CTL_GCPU_PC_LR_LOAD_EN                      0x41
#define CTL_GCPU_PC_LR_CLR_EN                       0x20
#define CTL_COMSUB_PC_LR_LOAD_EN                    0x81
#define CTL_COMSUB_PC_LR_CLR_EN                     0x40

#define WIFI_TRACE_PC_RAM_SEL                      (0x04194000 + 0x10C)
#define BT_TRACE_PC_RAM_SEL                        (0x04224000 + 0x10C)
#define COMSUB_TRACE_PC_RAM_SEL                    (0x04805000 + 0x10C)
#define GNSS_TRACE_PC_RAM_SEL                      (0x04324000 + 0x10C)

#define W_CTL_RB_W_CTL_SYS_CTL_ID_REG 0x41B5000
#define B_CTL_RB_B_CTL_ID_REG         0x4200000
#define G_CTL_RB_G_CTL_ID_REG         0x4300000
#define C_CTL_RB_C_CTL_ID_REG         0x4800000

#define GLBA_CTL_RB_AON_CRG_SOFT_RST_W_0_REG 0x40000A0
#define GLBA_CTL_RB_AON_CRG_SOFT_RST_B_0_REG 0x400008C
#define GLBA_CTL_RB_AON_CRG_SOFT_RST_G_0_REG 0x4000098
#define GLBA_CTL_RB_AON_CRG_SOFT_RST_C_0_REG 0x4000090

#define W_M7_CPU_TRACE_CTL_RB_CFG_CPU_MONITOR_CLOCK_REG 0x4194108
#define B_M7_CPU_TRACE_CTL_RB_CFG_CPU_MONITOR_CLOCK_REG 0x4224108
#define G_M7_CPU_TRACE_CTL_RB_CFG_CPU_MONITOR_CLOCK_REG 0x4324108
#define C_M7_CPU_TRACE_CTL_RB_CFG_CPU_MONITOR_CLOCK_REG 0x4805108

#define GLB_CTL_W_RB_GLB_CTL_W_GP_REG3_REG              0x400201c
#define GLB_CTL_B_RB_GLB_CTL_B_GP_REG3_REG              0x400301c
#define GLB_CTL_G_RB_GLB_CTL_G_GP_REG3_REG              0x400401c
#define GLB_CTL_COM_RB_GLB_CTL_COM_GP_REG3_REG          0x400501c
#define FLAG_PM_CLKPLL_ENABLE_MASK                      0x4

#define LP_CTL_G_RB_LP_CTL_G_GP_REG2_REG                0x4009008
#define TRACE_GFPLL_START                               0x5a14
#define TRACE_GFPLL_LOCK_DETECT                         0x5a15

#define PCIE_DBI_DEV_ID_VEND_ID_REG         0x4940000
#define PCIE_DEV_ID_VEND_ID_VAL_07C         0xa00219e7

#define PCIE_DBI_LTSSM_TRACER_OUTPUT_1      0x4970084
#define PCIE_DBI_LTSSM_TRACER_LAST_ADDR     0x4970088
#define PCIE_DBI_LTSSM_TRACERADDR           0x49702f8
#define PCIE_TRACER_LAST_ADDR_ROLLBACK      0x40
#define PCIE_TRACER_LAST_WADDR_MASK         0x3f
#define PCIE_TRACER_LTSSM_MASK              0x3f

/* devcie soft irq 3 相关配置 */
#define MP17C_GLBA_CTL_RB_B_SOFT_INT_SET_REG            0x4000144
#define MP17C_GLBA_CTL_RB_B_SOFT_INT3_SET_MASK          0x8
#define MP17C_GLBA_CTL_RB_B_SOFT_INT_STS_REG            0x400014C
#define MP17C_GLBA_CTL_RB_B_SOFT_INT3_STS_MASK          0x8
#define MP17C_GLBA_CTL_RB_G_SOFT_INT_SET_REG            0x4000164
#define MP17C_GLBA_CTL_RB_G_SOFT_INT3_SET_AON_MASK      0x8
#define MP17C_GLBA_CTL_RB_G_SOFT_INT_STS_AON_REG        0x400016C
#define MP17C_GLBA_CTL_RB_G_SOFT_INT3_STS_AON_MASK      0x8

#define LP_CTL_GLB_RB_LP_CTL_GLB_126_REG                0x4006290
#define GF_RF_ABB_CTL_RB_GF_RF_ABB_CTL_24_REG           0x4316080

enum cpu {
    WCPU,
    BCPU,
    GCPU,
    COMSUB,
    CPU_BUFF
};

enum cpu_addr {
    TRACE_PC,
    TRACE_LR,
    TRACE_SP,
    TRACE_BUFF
};

enum cpu_status {
    STATUS_OFF,
    STATUS_BOOTING,
    STATUS_SLEEPING,
    STATUS_WORK,
    STATUS_SAVING,
    STATUS_PROTECT,
    STATUS_SLEEP,
    STATUS_PROTECTING,
    STATUS_BUFF
};

/* dump 寄存器定义 */
static ssi_reg_info g_ssi_master_reg_full = { 0x40, 0x0, 0x30, SSI_RW_SSI_MOD };
static ssi_reg_info g_glba_ctrl = { 0x4000000, 0x0, 0x10c8, SSI_RW_WORD_MOD };
static ssi_reg_info g_glba_ctrl_extend1 = { 0x4001200, 0x0, 0x98, SSI_RW_WORD_MOD };
static ssi_reg_info g_glba_ctrl_extend2 = { 0x4001400, 0x0, 0x24, SSI_RW_WORD_MOD };
static ssi_reg_info g_glba_ctrl_extend3 = { 0x4001c00, 0x0, 0x50, SSI_RW_WORD_MOD };
static ssi_reg_info g_glba_ctrl_extend4 = { 0x4001d00, 0x0, 0x58, SSI_RW_WORD_MOD };
static ssi_reg_info g_glb_w_ctrl = { 0x4002000, 0x0, 0x79c,   SSI_RW_WORD_MOD };
static ssi_reg_info g_glb_b_ctrl = { 0x4003000, 0x0, 0x258,   SSI_RW_WORD_MOD };
static ssi_reg_info g_glb_g_ctrl = { 0x4004000, 0x0, 0x240,   SSI_RW_WORD_MOD };
static ssi_reg_info g_glb_g_ctrl_extend1 = { 0x4004500, 0x0, 0x50,   SSI_RW_WORD_MOD };
static ssi_reg_info g_glb_g_ctrl_extend2 = { 0x4004700, 0x0, 0x148,   SSI_RW_WORD_MOD };
static ssi_reg_info g_glb_com_ctrl = { 0x4005000, 0x0, 0x240,   SSI_RW_WORD_MOD };
static ssi_reg_info g_lp_ctrl = { 0x4006000, 0x0, 0x3a4,  SSI_RW_WORD_MOD };
static ssi_reg_info g_lp_w_ctrl = { 0x4007000, 0x0, 0x1e4,  SSI_RW_WORD_MOD };
static ssi_reg_info g_lp_b_ctrl = { 0x4008000, 0x0, 0x1ac,  SSI_RW_WORD_MOD };
static ssi_reg_info g_lp_g_ctrl = { 0x4009000, 0x0, 0x1e4,  SSI_RW_WORD_MOD };
static ssi_reg_info g_lp_com_ctrl = { 0x400a000, 0x0, 0x184,  SSI_RW_WORD_MOD };
static ssi_reg_info g_ipc_ctrl = { 0x4014000, 0x0, 0x200,  SSI_RW_WORD_MOD };
static ssi_reg_info g_pmu2_cmu_ctrl_part1 = { 0x401a000, 0x0, 0x890,  SSI_RW_WORD_MOD };
static ssi_reg_info g_pmu2_cmu_ctrl_part2 = { 0x401af00, 0x0, 0x2c,  SSI_RW_WORD_MOD };
static ssi_reg_info g_coex_ctl = { 0x4019000, 0x0, 0x1000,  SSI_RW_WORD_MOD };
#ifdef MP17C_SSI_SUPPORT_XOADC
static ssi_reg_info g_xoadc_ctrl_part1 = { 0x4020000, 0x0, 0x20,  SSI_RW_WORD_MOD };
static ssi_reg_info g_xoadc_ctrl_part2 = { 0x4020200, 0x0, 0x20,  SSI_RW_WORD_MOD };
static ssi_reg_info g_xoadc_ctrl_part3 = { 0x4020600, 0x0, 0x68,  SSI_RW_WORD_MOD };
static ssi_reg_info g_xoadc_ctrl_part4 = { 0x4020860, 0x0, 0x8,  SSI_RW_WORD_MOD };
static ssi_reg_info g_xoadc_ctrl_part5 = { 0x4020a80, 0x0, 0x2c,  SSI_RW_WORD_MOD };
#endif
static ssi_reg_info g_mipi_ctrl = { 0x4025000, 0x0, 0xa0,  SSI_RW_WORD_MOD };
static ssi_reg_info g_gpio0_full = {0x400e000, 0x0, 0x208, SSI_RW_DWORD_MOD};
static ssi_reg_info g_gpio1_full = {0x400f000, 0x0, 0x208, SSI_RW_DWORD_MOD};

static ssi_reg_info g_sharemem_fix_pos = { 0x04040000, 0x0, 0xd80,  SSI_RW_DWORD_MOD };

static ssi_reg_info g_b_uart_header_mem    = { 0x1403724, 0x0, 0x18, SSI_RW_DWORD_MOD};
static ssi_reg_info g_b_uart_payload_mem   = { 0x14025a4, 0x0, 0x10, SSI_RW_DWORD_MOD};
static ssi_reg_info g_gf_uart_header_mem    = { 0x2403264, 0x0, 0x18, SSI_RW_DWORD_MOD};
static ssi_reg_info g_gf_uart_payload_mem   = { 0x24020fc, 0x0, 0x10, SSI_RW_DWORD_MOD};

// 分别存储着四个核的平台相关的data段和bss段的起始地址、结束地址
static ssi_reg_info g_w_plat_ram_info = { 0x4040720, 0x0, 0x10,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_b_plat_ram_info = { 0x4040730, 0x0, 0x10,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_g_plat_ram_info = { 0x4040740, 0x0, 0x10,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_c_plat_ram_info = { 0x4040750, 0x0, 0x10,  SSI_RW_DWORD_MOD };

#define SSI_ADDR_WCPU_OFFSET 0x0000000
#define SSI_ADDR_BCPU_OFFSET 0x1000000
#define SSI_ADDR_GCPU_OFFSET 0x2000000
#define SSI_ADDR_CCPU_OFFSET 0x3000000

static ssi_reg_info g_wcpu_plat_rom_data = { 0x40476c + SSI_ADDR_WCPU_OFFSET, 0x0, 0x778,  SSI_RW_DWORD_MOD };
// 真实地址0x4088fa, 长度2292, 4字节对齐, 从0x4088f8开始读
static ssi_reg_info g_wcpu_plat_rom_bss = { 0x4088f8 + SSI_ADDR_WCPU_OFFSET, 0x0, 0x2294,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_wcpu_plat_ram_data = { 0x0, 0x0, 0x0,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_wcpu_plat_ram_bss = { 0x0, 0x0, 0x0,  SSI_RW_DWORD_MOD };

static ssi_reg_info g_bcpu_plat_rom_data = { 0x401e34 + SSI_ADDR_BCPU_OFFSET, 0x0, 0x3a8,  SSI_RW_DWORD_MOD };
// ssi地址0x4025a3, 长度5b11, 4字节对齐, 从0x4025a0开始读
static ssi_reg_info g_bcpu_plat_rom_bss = { 0x4025a0 + SSI_ADDR_BCPU_OFFSET, 0x0, 0x5b14,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_bcpu_plat_ram_data = { 0x0, 0x0, 0x0,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_bcpu_plat_ram_bss = { 0x0, 0x0, 0x0,  SSI_RW_DWORD_MOD };

static ssi_reg_info g_gcpu_plat_rom_data = { 0x401d6c + SSI_ADDR_GCPU_OFFSET, 0x0, 0x390,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_gcpu_plat_rom_bss = { 0x4020fc + SSI_ADDR_GCPU_OFFSET, 0x0, 0x4A28,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_gcpu_plat_ram_data = { 0x0, 0x0, 0x0,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_gcpu_plat_ram_bss = { 0x0, 0x0, 0x0,  SSI_RW_DWORD_MOD };

static ssi_reg_info g_comsub_plat_ram_data = { 0x0, 0x0, 0x0,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_comsub_plat_ram_bss = { 0x0, 0x0, 0x0,  SSI_RW_DWORD_MOD };

static ssi_reg_info g_wlan_exception_mesg = { 0x808de00, 0x0, 0xB4,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_bcpu_exception_mesg = { 0x140b824, 0x0, 0x90,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_gcpu_exception_mesg = { 0x24083a8, 0x0, 0x90,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_comsub_exception_mesg = { 0x32001ac, 0x0, 0x90,  SSI_RW_DWORD_MOD };

static ssi_reg_info g_w_trace_mem   = { 0x4196000, 0x0, 0x2000, SSI_RW_DWORD_MOD};
static ssi_reg_info g_b_trace_mem   = { 0x4226000, 0x0, 0x2000, SSI_RW_DWORD_MOD};
static ssi_reg_info g_gnss_trace_mem  = { 0x4326000, 0x0, 0x2000, SSI_RW_DWORD_MOD};
static ssi_reg_info g_comsub_trace_mem = { 0x4807000, 0x0, 0x800, SSI_RW_DWORD_MOD};

static ssi_reg_info g_w_ctrl_part1 = { 0x41b5000, 0x0, 0x1258,  SSI_RW_WORD_MOD };
static ssi_reg_info g_b_ctrl_full = { 0x4200000, 0x0, 0x400,  SSI_RW_WORD_MOD };
static ssi_reg_info g_b_timer1_full = { 0x4203300, 0x0, 0x20,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_gf_ctrl_full = { 0x4300000, 0x0, 0x44c,  SSI_RW_WORD_MOD };
static ssi_reg_info g_comsub_ctrl_full = { 0x4800000, 0x0, 0x3c8,  SSI_RW_WORD_MOD };
static ssi_reg_info g_pcie_ctrl_full = { 0x4980000, 0x0, 0x7a8,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_pcie_ctrl_full2 = { 0x4982c0c, 0x0, 0x4,  SSI_RW_DWORD_MOD }; // 误码率统计
static ssi_reg_info g_host_ctrl_full = { 0x4985000, 0x0, 0xf80,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_pcie_linkdown_debug = { 0x4040420, 0x0, 0x20,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_ringbuf_res_debug = { 0x408e70, 0x0, 0x160,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_pcie_stats_debug = { 0x409060, 0x0, 0xe4,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_pcie_msg_buf_debug = { 0x409150, 0x0, 0x100,  SSI_RW_DWORD_MOD };

static ssi_reg_info g_pcie_mac_ap_part0 = { 0x4908000, 0x0, 0x258,  SSI_RW_DWORD_MOD };   // AP_GLOBAL_REG0
static ssi_reg_info g_pcie_mac_ap_part1 = { 0x4908000, 0x3f8, 0x28,  SSI_RW_DWORD_MOD };   // AP_GLOBAL_REG1
static ssi_reg_info g_pcie_mac_ap_part2 = { 0x4908000, 0x7fc, 0x30,  SSI_RW_DWORD_MOD };   // AP_GLOBAL_REG2
static ssi_reg_info g_pcie_mac_part1 = { 0x4940000, 0x0, 0x100,  SSI_RW_DWORD_MOD };   // EPF_CFGSPACE
static ssi_reg_info g_pcie_mac_part2 = { 0x495e000, 0x400, 0x200,  SSI_RW_DWORD_MOD }; // CORE_GLOBAL_CTRL_REG
static ssi_reg_info g_pcie_mac_part3 = { 0x4970000, 0x0, 0x5fc,  SSI_RW_DWORD_MOD };   // MAC_REG
static ssi_reg_info g_pcie_mac_part4 = { 0x4900000, 0x800, 0x100,  SSI_RW_DWORD_MOD };   // AP_IOB_TX_REG
static ssi_reg_info g_pcie_mac_part5 = { 0x4904000, 0x1700, 0x20,  SSI_RW_DWORD_MOD };   // AP_IOB_RX_REG
static ssi_reg_info g_pcie_mac_part6 = { 0x4920000, 0x56C, 0x44,  SSI_RW_DWORD_MOD };   // TL_REG
static ssi_reg_info g_pcie_mac_part7 = { 0x4960000, 0x0, 0x10c,  SSI_RW_DWORD_MOD };   // DL_REG

static ssi_reg_info g_clkpll_w_debug = { 0x41c8000, 0x0, 0xc8,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_clkpll_b_debug = { 0x421c000, 0x0, 0xc8,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_clkpll_gf_debug = { 0x431a000, 0x0, 0xc8,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_clkpll_com_debug = { 0x4812000, 0x0, 0xc8,  SSI_RW_DWORD_MOD };

static ssi_reg_info g_w_boot_dbg_full = { 0x401828, 0x0, 0x5AC,  SSI_RW_DWORD_MOD };

static ssi_reg_info g_uart_b_part0 = { 0x4204000, 0x8, 0x14,  SSI_RW_WORD_MOD };
static ssi_reg_info g_uart_b_part1 = { 0x4204000, 0x24, 0x10,  SSI_RW_WORD_MOD };
static ssi_reg_info g_uart_b_part2 = { 0x4204000, 0x44, 0x34,  SSI_RW_WORD_MOD };
static ssi_reg_info g_uart_gf_part0 = { 0x4305000, 0x8, 0x14,  SSI_RW_WORD_MOD };
static ssi_reg_info g_uart_gf_part1 = { 0x4305000, 0x24, 0x10,  SSI_RW_WORD_MOD };
static ssi_reg_info g_uart_gf_part2 = { 0x4305000, 0x44, 0x34,  SSI_RW_WORD_MOD };

static ssi_reg_info g_dma_b_reg = { 0x4230000, 0x0, 0x1f8,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_dma_gf_reg = { 0x4328000, 0x0, 0x138,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_dma_com_reg = { 0x4802000, 0x0, 0x138,  SSI_RW_DWORD_MOD };

static ssi_reg_info g_wcpu_ram_log_mesg = { 0x8060000, 0x0, 0x1220,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_bcpu_ram_log_mesg = { 0x1400000, 0x4f08, 0x23c0,  SSI_RW_DWORD_MOD };
static ssi_reg_info g_gcpu_ram_log_mesg = { 0x2400000, 0x397c, 0x23c0,  SSI_RW_DWORD_MOD };

static ssi_reg_info g_gf_rf_abb_ctl_1 = { 0x4316000, 0x0, 0xc4, SSI_RW_WORD_MOD };
static ssi_reg_info g_gf_rf_abb_ctl_2 = { 0x4316000, 0x200, 0x144, SSI_RW_WORD_MOD };
static ssi_reg_info g_gnss_trx_dreg = { 0x4313000, 0x0, 0x3c8, SSI_RW_WORD_MOD };
static ssi_reg_info g_gnss_pll_dig_l1 = { 0x4314000, 0x0, 0xcc, SSI_RW_WORD_MOD };
static ssi_reg_info g_gnss_pll_dig_l5 = { 0x4315000, 0x0, 0xcc, SSI_RW_WORD_MOD };

static ssi_reg_info *g_wcpu_pcie_debug_full[] = {
    &g_ringbuf_res_debug,
    &g_pcie_stats_debug,
    &g_pcie_msg_buf_debug,
    &g_pcie_linkdown_debug,
};

static ssi_reg_info *g_wcpu_reg_full[] = {
    &g_w_ctrl_part1,
    &g_w_boot_dbg_full,
};

static ssi_reg_info *g_bcpu_reg_full[] = {
    &g_b_ctrl_full,
    &g_b_timer1_full,
    &g_b_uart_header_mem,
    &g_b_uart_payload_mem,
    &g_dma_b_reg,
    &g_uart_b_part0,
    &g_uart_b_part1,
    &g_uart_b_part2
};

static ssi_reg_info *g_gfcpu_reg_full[] = {
    &g_gf_ctrl_full,
    &g_gf_uart_header_mem,
    &g_gf_uart_payload_mem,
    &g_dma_gf_reg,
    &g_uart_gf_part0,
    &g_uart_gf_part1,
    &g_uart_gf_part2
};

static ssi_reg_info *g_comsub_reg_full[] = {
    &g_comsub_ctrl_full,
    &g_dma_com_reg
};

static ssi_reg_info *g_clkpll_reg_full[CPU_BUFF] = {
    &g_clkpll_w_debug,
    &g_clkpll_b_debug,
    &g_clkpll_gf_debug,
    &g_clkpll_com_debug
};

static ssi_reg_info *g_gfpll_reg_full[] = {
    &g_gf_rf_abb_ctl_1,
    &g_gf_rf_abb_ctl_2,
    &g_gnss_trx_dreg,
    &g_gnss_pll_dig_l1,
    &g_gnss_pll_dig_l5
};

/* 默认dump 所有寄存器 */
static ssi_reg_info *g_aon_reg_full[] = {
    &g_ssi_master_reg_full,
    &g_glba_ctrl,
    &g_glba_ctrl_extend1,
    &g_glba_ctrl_extend2,
    &g_glba_ctrl_extend3,
    &g_glba_ctrl_extend4,
    &g_glb_w_ctrl,
    &g_glb_b_ctrl,
    &g_glb_g_ctrl,
    &g_glb_g_ctrl_extend1,
    &g_glb_g_ctrl_extend2,
    &g_glb_com_ctrl,
    &g_lp_ctrl,
    &g_lp_w_ctrl,
    &g_lp_b_ctrl,
    &g_lp_g_ctrl,
    &g_lp_com_ctrl,
    &g_ipc_ctrl,
    &g_pmu2_cmu_ctrl_part1,
    &g_pmu2_cmu_ctrl_part2,
    &g_coex_ctl,
#ifdef MP17C_SSI_SUPPORT_XOADC
    &g_xoadc_ctrl_part1,
    &g_xoadc_ctrl_part2,
    &g_xoadc_ctrl_part3,
    &g_xoadc_ctrl_part4,
    &g_xoadc_ctrl_part5,
#endif
    &g_mipi_ctrl
};

static ssi_reg_info *g_pcie_mac_ap_full[] = {
    &g_pcie_mac_ap_part0,
    &g_pcie_mac_ap_part1,
    &g_pcie_mac_ap_part2,
};

static ssi_reg_info *g_pcie_mac_full[] = {
    &g_pcie_mac_part1,
    &g_pcie_mac_part2,
    &g_pcie_mac_part3,
    &g_pcie_mac_part4,
    &g_pcie_mac_part5,
    &g_pcie_mac_part6,
    &g_pcie_mac_part7,
};

static uint32_t g_irq_from_pmu;

static uint32_t pmu_regs_arr[][2] = {
    /* 先读NP_EVENT寄存器, 下电不丢失 */
    {SCP0_NP_EVENT_REG, 0x0},
    {OCP0_NP_EVENT_REG, 0x0},
    {OCP1_NP_EVENT_REG, 0x0},
    {IRQ0_NP_EVENT_REG, 0x0},
    {OCP2_NP_EVENT_REG, 0x0},
    /* 后读IRQ寄存器, 下电丢失 */
    {SCP0_IRQ_REG, 0x0},
    {OCP0_IRQ_REG, 0x0},
    {OCP1_IRQ_REG, 0x0},
    {IRQ0_IRQ_REG, 0x0},
    {OCP2_IRQ_REG, 0x0},
    /* 读BUCK0/SYSLDO/CLDO2寄存器, 下电丢失 */
    {WLAN_W_BUCK0_VSET_REG, 0x0},
    {WLAN_W_SYSLDO_VSET_REG, 0x0},
    {WLAN_W_CLDO2_VSET_REG, 0x0},
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
        "bcpu",
        0x0,
        GLB_CTL_B_RB_GLB_BCPU_LOAD_REG,
        { 0x0, 0x0 },
        {
            {
                "pc",
                GLB_CTL_B_RB_GLB_BCPU_PC_L_REG,
                GLB_CTL_B_RB_GLB_BCPU_PC_H_REG,
                { 0x0, 0x0 }
            },
            {
                "lr",
                GLB_CTL_B_RB_GLB_BCPU_LR_L_REG,
                GLB_CTL_B_RB_GLB_BCPU_LR_H_REG,
                { 0x0, 0x0 }
            },
            {
                "sp",
                GLB_CTL_B_RB_GLB_BCPU_SP_L_REG,
                GLB_CTL_B_RB_GLB_BCPU_SP_H_REG,
                { 0x0, 0x0 }
            }
        }
    },
    {
        "gcpu",
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
    {
        "comsub",
        0x0,
        GLB_CTL_COM_RB_GLB_COMCPU_LOAD_REG,
        { 0x0, 0x0 },
        {
            {
                "pc",
                GLB_CTL_COM_RB_GLB_COMCPU_PC_L_REG,
                GLB_CTL_COM_RB_GLB_COMCPU_PC_H_REG,
                { 0x0, 0x0 }
            },
            {
                "lr",
                GLB_CTL_COM_RB_GLB_COMCPU_LR_L_REG,
                GLB_CTL_COM_RB_GLB_COMCPU_LR_H_REG,
                { 0x0, 0x0 }
            },
            {
                "sp",
                GLB_CTL_COM_RB_GLB_COMCPU_SP_L_REG,
                GLB_CTL_COM_RB_GLB_COMCPU_SP_H_REG,
                { 0x0, 0x0 }
            }
        }
    }
};

enum {
    PCIE_S_INIT             = 0x0,
    PCIE_S_RESET_PIPE_AFIFO = 0x1,
    PCIE_S_DETECT_QUIET     = 0x2,
    PCIE_S_DETECT_ACTIVE    = 0x3,
    PCIE_S_DETECT_WAIT      = 0x4,
    PCIE_S_DETECT_PWR_P0    = 0x5,
    PCIE_S_POLL_ACTIVE      = 0x6,
    PCIE_S_POLL_CONFIG      = 0x7,
    PCIE_S_POLL_COMP        = 0x8,
    PCIE_S_CONFIG_LW_STR    = 0x9,
    PCIE_S_CONFIG_LW_ACC    = 0xA,
    PCIE_S_CONFIG_LN_WAIT   = 0xB,
    PCIE_S_CONFIG_LN_ACC    = 0xC,
    PCIE_S_CONFIG_CONPLETE  = 0xD,
    PCIE_S_CONFIG_IDLE1     = 0xE,
    PCIE_S_CONFIG_IDLE2     = 0xF,
    PCIE_S_L0               = 0x10,
    PCIE_S_RX_L0S           = 0x11,
    PCIE_S_TX_L0S_ENTRY     = 0x14,
    PCIE_S_TX_L0S_IDLE      = 0x15,
    PCIE_S_TX_L0S_FTS       = 0x16,
    PCIE_S_L1               = 0x17,
    PCIE_S_L2               = 0x18,
    PCIE_S_TX_BEACON_BEGIN  = 0x19,
    PCIE_S_TX_BEACON_END    = 0x1A,
    PCIE_S_RECOVERY_RCVLCK  = 0x20,
    PCIE_S_RECOVERY_RCVCFG  = 0x21,
    PCIE_S_RECOVERY_SPEED   = 0x22,
    PCIE_S_RECOVERY_IDLE1   = 0x23,
    PCIE_S_RECOVERY_IDLE2   = 0x24,
    PCIE_S_RECOVERY_EQ_P0   = 0x25,
    PCIE_S_RECOVERY_EQ_P1   = 0x26,
    PCIE_S_RECOVERY_EQ_P2   = 0x27,
    PCIE_S_RECOVERY_EQ_P3   = 0x28,
    PCIE_S_TX_EIOS_ST       = 0x30,
    PCIE_S_CHG_RATE_GEN1    = 0x31,
    PCIE_S_CHANGE_POWER     = 0x32,
    PCIE_S_HOT_RESET        = 0x33,
    PCIE_S_DISABLE_P1       = 0x34,
    PCIE_S_DISABLE_P2       = 0x35,
    PCIE_S_LOOPBACK_ENTRY   = 0x38,
    PCIE_S_LOOPBACK_ACTIVE  = 0x39,
    PCIE_S_LOOPBACK_EXIT    = 0x3A
};

typedef struct {
    uint32_t state;
    char *desc;
} pcie_ltssm_t;

STATIC pcie_ltssm_t pcie_ltssm[] = {
    {PCIE_S_INIT, "LTSSM_INIT"},
    {PCIE_S_RESET_PIPE_AFIFO, "LTSSM_RESET_PIPE_AFIFO"},
    {PCIE_S_DETECT_QUIET, "LTSSM_DETEC_QUIET"},
    {PCIE_S_DETECT_ACTIVE, "LTSSM_DETECT_ACTIVE"},
    {PCIE_S_DETECT_WAIT, "LTSSM_DETECT_WAIT"},
    {PCIE_S_DETECT_PWR_P0, "LTSSM_DETECT_PWR_P0"},
    {PCIE_S_POLL_ACTIVE, "LTSSM_POLL_ACTIVE"},
    {PCIE_S_POLL_CONFIG, "LTSSM_POLL_CONFIG"},
    {PCIE_S_POLL_COMP, "LTSSM_POLL_COMP"},
    {PCIE_S_CONFIG_LW_STR, "LTSSM_CONFIG_LW_STR"},
    {PCIE_S_CONFIG_LW_ACC, "LTSSM_CONFIG_LW_ACC"},
    {PCIE_S_CONFIG_LN_WAIT, "LTSSM_CONFIG_LN_WAIT"},
    {PCIE_S_CONFIG_LN_ACC, "LTSSM_CONFIG_LN_ACC"},
    {PCIE_S_CONFIG_CONPLETE, "LTSSM_CONFIG_CONPLETE"},
    {PCIE_S_CONFIG_IDLE1, "LTSSM_CONFIG_IDLE1"},
    {PCIE_S_CONFIG_IDLE2, "LTSSM_CONFIG_IDLE2"},
    {PCIE_S_L0, "LTSSM_L0"},
    {PCIE_S_RX_L0S, "LTSSM_RX_L0s"},
    {PCIE_S_TX_L0S_ENTRY, "LTSSM_TX_L0S_ENTRY"},
    {PCIE_S_TX_L0S_IDLE, "LTSSM_TX_L0S_IDLE"},
    {PCIE_S_TX_L0S_FTS, "LTSSM_TX_L0S_FTS"},
    {PCIE_S_L1, "LTSSM_L1"},
    {PCIE_S_L2, "LTSSM_L2"},
    {PCIE_S_TX_BEACON_BEGIN, "LTSSM_TX_BEACON_BEGIN"},
    {PCIE_S_TX_BEACON_END, "LTSSM_TX_BEACON_END"},
    {PCIE_S_RECOVERY_RCVLCK, "LTSSM_RECOVERY_RCVLCK"},
    {PCIE_S_RECOVERY_RCVCFG, "LTSSM_RECOVERY_RCVCFG"},
    {PCIE_S_RECOVERY_SPEED, "LTSSM_RECOVERY_SPEED"},
    {PCIE_S_RECOVERY_IDLE1, "LTSSM_RECOVERY_IDLE1"},
    {PCIE_S_RECOVERY_IDLE2, "LTSSM_RECOVERY_IDLE2"},
    {PCIE_S_RECOVERY_EQ_P0, "LTSSM_RECOVERY_EQ_P0"},
    {PCIE_S_RECOVERY_EQ_P1, "LTSSM_RECOVERY_EQ_P1"},
    {PCIE_S_RECOVERY_EQ_P2, "LTSSM_RECOVERY_EQ_P2"},
    {PCIE_S_RECOVERY_EQ_P3, "LTSSM_RECOVERY_EQ_P3"},
    {PCIE_S_TX_EIOS_ST, "LTSSM_TX_EIOS_ST"},
    {PCIE_S_CHG_RATE_GEN1, "LTSSM_CHG_RATE_GEN1"},
    {PCIE_S_CHANGE_POWER, "LTSSM_CHANGE_POWER"},
    {PCIE_S_HOT_RESET, "LTSSM_HOT_RESET"},
    {PCIE_S_DISABLE_P1, "LTSSM_DISABLE_P1"},
    {PCIE_S_DISABLE_P2, "LTSSM_DISABLE_P2"},
    {PCIE_S_LOOPBACK_ENTRY, "LTSSM_LOOPBACK_ENTRY"},
    {PCIE_S_LOOPBACK_ACTIVE, "LTSSM_LOOPBACK_ACTIVE"},
    {PCIE_S_LOOPBACK_EXIT, "LTSSM_LOOPBACK_EXIT"},
};

static char *pcie_ctrl_get_ltssm_desc(uint32_t ltssm)
{
    int i;
    for (i = 0; i < sizeof(pcie_ltssm) / sizeof(pcie_ltssm[0]); i++) {
        if (pcie_ltssm[i].state == ltssm) {
            return pcie_ltssm[i].desc;
        }
    }
    return "unknown";
}

uint32_t mp17c_ssi_spmi_read(uint32_t addr)
{
    int32_t ret;
    uint32_t data;

    /* first, reinit spmi_base_addr */
    reinit_spmi_base_addr(SPMI_BASE_ADDR);

    ret = ssi_check_device_isalive();
    if (ret) {
        ps_print_err("please check wifi/bt/gnss is turned on?\n");
        return 0xFFFFFFFF;
    }

    ret = ssi_spmi_read(addr, SPMI_CHANNEL_GNSS, mp17c_pmu_ver_get(), &data);
    if (ret != OAL_SUCC) {
        ps_print_err("Spmi read data through ssi fail\n");
        return 0xFFFFFFFF;
    }

    return data;
}

void mp17c_ssi_spmi_write(uint32_t addr, uint32_t data)
{
    int32_t ret;

    /* first, reinit spmi_base_addr */
    reinit_spmi_base_addr(SPMI_BASE_ADDR);

    ret = ssi_check_device_isalive();
    if (ret) {
        ps_print_err("please check wifi/bt/gnss is turned on?\n");
        return;
    }

    ret = ssi_spmi_write(addr, SPMI_CHANNEL_GNSS, SPMI_SLAVEID_HI6506, data);
    if (ret != OAL_SUCC) {
        ps_print_err("Spmi write data through ssi fail\n");
    }
}

int mp17c_ssi_check_subsys_is_work(void)
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

static void dsm_cpu_info_dump(unsigned long long module_set)
{
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
    int32_t ret;
    char buf[DSM_CPU_INFO_SIZE] = { 0 };
    /* dsm cpu信息上报 */
    if (module_set & SSI_MODULE_MASK_OFF_FIND_LOG) {
        return;
    }

    if (g_halt_det_cnt || g_mpxx_kernel_crash) {
        ps_print_info("g_halt_det_cnt=%u g_mpxx_kernel_crash=%d dsm_cpu_info_dump return\n",
                      g_halt_det_cnt, g_mpxx_kernel_crash);
        return;
    }

    ret = snprintf_s(buf, DSM_CPU_INFO_SIZE, DSM_CPU_INFO_SIZE - 1,
        "wcpu:0x%08x 0x%08x 0x%08x 0x%08x s:%1d bcpu:0x%08x 0x%08x 0x%08x 0x%08x s:%1d "\
        "gcpu:0x%08x 0x%08x 0x%08x 0x%08x s:%1d sle:0x%08x 0x%08x 0x%08x 0x%08x s:%1d pmu:%1d",
        (uint32_t)g_ssi_cpu_trace[WCPU].trace_part[TRACE_PC].value[0],
        (uint32_t)g_ssi_cpu_trace[WCPU].trace_part[TRACE_LR].value[0],
        (uint32_t)g_ssi_cpu_trace[WCPU].trace_part[TRACE_PC].value[1],
        (uint32_t)g_ssi_cpu_trace[WCPU].trace_part[TRACE_LR].value[1],
        (uint8_t)g_ssi_cpu_trace[WCPU].cpu_state,
        (uint32_t)g_ssi_cpu_trace[BCPU].trace_part[TRACE_PC].value[0],
        (uint32_t)g_ssi_cpu_trace[BCPU].trace_part[TRACE_LR].value[0],
        (uint32_t)g_ssi_cpu_trace[BCPU].trace_part[TRACE_PC].value[1],
        (uint32_t)g_ssi_cpu_trace[BCPU].trace_part[TRACE_LR].value[1],
        (uint8_t)g_ssi_cpu_trace[BCPU].cpu_state,
        (uint32_t)g_ssi_cpu_trace[GCPU].trace_part[TRACE_PC].value[0],
        (uint32_t)g_ssi_cpu_trace[GCPU].trace_part[TRACE_LR].value[0],
        (uint32_t)g_ssi_cpu_trace[GCPU].trace_part[TRACE_PC].value[1],
        (uint32_t)g_ssi_cpu_trace[GCPU].trace_part[TRACE_LR].value[1],
        (uint8_t)g_ssi_cpu_trace[GCPU].cpu_state,
        (uint32_t)g_ssi_cpu_trace[COMSUB].trace_part[TRACE_PC].value[0],
        (uint32_t)g_ssi_cpu_trace[COMSUB].trace_part[TRACE_LR].value[0],
        (uint32_t)g_ssi_cpu_trace[COMSUB].trace_part[TRACE_PC].value[1],
        (uint32_t)g_ssi_cpu_trace[COMSUB].trace_part[TRACE_LR].value[1],
        (uint8_t)g_ssi_cpu_trace[COMSUB].cpu_state,
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

static int get_cpu_status(int sys)
{
    uint32_t reg, sts;
    uint32_t cpu_sts[][2] = {
        { WCPU,   GLBA_CTL_RB_W_SYS_STS_REG },
        { BCPU,   GLBA_CTL_RB_B_SYS_STS_REG },
        { GCPU,   GLBA_CTL_RB_G_SYS_STS_REG },
        { COMSUB, GLBA_CTL_RB_COM_SYS_STS_REG }
    };

    reg = (uint32_t)ssi_read32(cpu_sts[sys][1]);
    ps_print_info("cpu state=0x%x\n", reg);
    sts = reg & GLBA_CTL_RB_CUR_STS_MASK;
    g_ssi_cpu_trace[sys].cpu_state = sts;
    ps_print_info("%s is %s\n", g_ssi_cpu_trace[sys].name, g_ssi_cpu_st_str[sts]);

    return sts;
}

static int ssi_check_cpu_is_working(int sys)
{
    return (get_cpu_status(sys) == STATUS_WORK);
}

static int ssi_check_pcie_status(void)
{
    uint32_t ret;
    ret = (uint32_t)ssi_read32(GLB_CTL_W_RB_HOST_CRG_SOFT_RST_W_REG);
    if ((ret & GLB_CTL_W_RB_SOFT_RST_MASK) == GLB_CTL_W_RB_SOFT_RST_MASK) {
        return OAL_SUCC;
    }
    return OAL_FAIL;
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

int mp17c_ssi_read_pc_lr_sp(void)
{
    int i;
    uint32_t load;
    uint32_t ctl_cpu_pc_lrload[][3] = { /* Number of columns in the array is 3 */
        { WCPU,   CTL_WCPU_PC_LR_LOAD_EN, CTL_WCPU_PC_LR_CLR_EN },
        { BCPU,   CTL_BCPU_PC_LR_LOAD_EN, CTL_BCPU_PC_LR_CLR_EN },
        { GCPU,   CTL_GCPU_PC_LR_LOAD_EN, CTL_GCPU_PC_LR_CLR_EN },
        { COMSUB, CTL_COMSUB_PC_LR_LOAD_EN,  CTL_COMSUB_PC_LR_CLR_EN }
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

void mp17c_ssi_show_pll_lock_status(void)
{
    if (ssi_check_cpu_is_working(BCPU)) {
        ps_print_warning("BCPU clkpll lock status = [0x%x]\n", ssi_read_dword(LP_CTL_GLB_RB_LP_CTL_GLB_126_REG));
    }

    if (ssi_check_cpu_is_working(GCPU)) {
        ps_print_warning("gnss gfpll lock status = [0x%x]\n", ssi_read_dword(GF_RF_ABB_CTL_RB_GF_RF_ABB_CTL_24_REG));
    }
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

void mp17c_ssi_dump_gpio(void)
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
        ret = mp17c_ssi_read_pc_lr_sp();
        if (ret) {
            goto ssi_fail;
        }
    }

    return 0;
ssi_fail:
    return ret;
}

static int32_t check_ssi_spmi_is_ok(void)
{
    uint32_t data = mp17c_ssi_spmi_read(CHIP_VERSION_0);
    if (data != CHIP_VERSION_0_DEFAULT_VALUE) {
        ps_print_info("ssi_spmi interface is not normal\n");
        return -OAL_EFAIL;
    }

    return OAL_SUCC;
}

static void pmu_status_error_dsm_report(void)
{
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
    int32_t i;
    int32_t idx = 0;
    int32_t nread = 0;
    int8_t dsm_buf[DSM_BUF_MAX_SIZE] = { 0 };

    for (i = 0; i < oal_array_size(pmu_regs_arr); i++) {
        if (idx < DSM_BUF_MAX_SIZE - 1) {
            nread = snprintf_s(dsm_buf + idx, DSM_BUF_MAX_SIZE - idx, DSM_BUF_MAX_SIZE - idx - 1,
                "0x%x,", pmu_regs_arr[i][1]);
        } else {
            nread = 0;
        }

        if (nread > 0) {
            idx += nread;
        }
    }
    hw_mpxx_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_PMU_ERROR, "get pmu reg:%s", dsm_buf);
#else
    ps_print_info("dsm not support");
#endif
}

/* 检测6506电源异常，读取pmu寄存器 */
static int32_t ssi_dump_power_status_regs(void)
{
    int32_t ret, i;
    uint32_t data;

    /* config spmi pinmux */
    ssi_write_dword(PINMUX_RB_CFG_SPMI_CLK_MODE_REG, 0x1);
    ps_print_err("6506 NP_EVENT IRQ_STS IRQ_MASK and BUCK0_VSET DUMP\n");
    ret = check_ssi_spmi_is_ok();
    if (ret != OAL_SUCC) {
        return -OAL_EFAIL;
    }

    for (i = 0; i < oal_array_size(pmu_regs_arr); i++) {
        data = mp17c_ssi_spmi_read(pmu_regs_arr[i][0]);
        if (data == 0xFFFFFFFF) {
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
            hw_mpxx_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_PMU_ERROR, "get pmu reg fail");
#endif
            return -OAL_EFAIL;
        } else {
            ps_print_info("reg%d: 0x%x = 0x%x\n", i, pmu_regs_arr[i][0], data);
            pmu_regs_arr[i][1] = data;
        }
    }

    return OAL_SUCC;
}

static void ssi_dump_pmu_regs(unsigned long long module_set)
{
    int32_t ret;
    int32_t pmu1_status_grm;
    int32_t pmu1_status_grm_stick;

    g_irq_from_pmu = 0;
    /* try to read pmu regs */
    if ((module_set & SSI_MODULE_MASK_PMU) == 0x0) {
        return;
    }

    pmu1_status_grm = ssi_read32(LP_CTL_GLB_RB_LP_CTL_GLB_93_REG);
    if (pmu1_status_grm < 0) {
        return;
    }

    pmu1_status_grm_stick = ssi_read32(LP_CTL_GLB_RB_LP_CTL_GLB_94_REG);
    if (pmu1_status_grm_stick < 0) {
        return;
    }

    if ((((uint32_t)pmu1_status_grm & LP_CTL_GLB_RB_IRQ_FROM_PMIC_GRM_MASK) ==
        LP_CTL_GLB_RB_IRQ_FROM_PMIC_GRM_MASK) ||
        (((uint32_t)pmu1_status_grm_stick & LP_CTL_GLB_RB_IRQ_FROM_PMIC_GRM_1STICK_MASK) ==
         LP_CTL_GLB_RB_IRQ_FROM_PMIC_GRM_1STICK_MASK)) {
        g_irq_from_pmu = 1;
    }

    ret = ssi_dump_power_status_regs();
    if (ret != OAL_SUCC) {
        ps_print_err("ssi_dump_pmu_regs read np_event failed\n");
        return;
    }

    /* 有pmu error中断时，DSM上报大数据 */
    if (g_irq_from_pmu == 1) {
        pmu_status_error_dsm_report();
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

static uint16_t ssi_read_systick(void)
{
    uint16_t sys_tick_old;
    sys_tick_old = (uint16_t)ssi_read32(AON_CTL_SYS_TICK_VALUE);
    return sys_tick_old;
}

static int ssi_detect_32k_handle(uint16_t sys_tick_old, uint64_t cost_time_s, uint32_t *clock_32k)
{
    uint32_t base_32k_clock = RTC_32K_NOMAL_CKL;
    uint16_t sys_tick_new;

    sys_tick_new = ssi_read_systick();
    if (sys_tick_new == sys_tick_old) {
        ps_print_err("32k sys_tick don't change after detect, 32k maybe abnormal, sys_tick=0x%x\n", sys_tick_new);
        return -OAL_EFAIL;
    } else {
        cost_time_s += 1446; /* 经验值,误差1446us */
        if (sys_tick_new > sys_tick_old) {
            /* 1000 is mean hz to KHz */
            (*clock_32k) = ((sys_tick_new - sys_tick_old) * 1000) / (uint32_t)cost_time_s;
            ps_print_info("32k runtime:%llu us, sys_tick:%u\n", cost_time_s, sys_tick_new - sys_tick_old);
        } else {
            /* 1000 is mean hz to KHz */
            (*clock_32k) = ((sys_tick_new + (0xffff - sys_tick_old)) * 1000) / (uint32_t)cost_time_s;
            ps_print_info("32k runtime:%llu us, sys_tick:%u\n", cost_time_s, sys_tick_new + (0xffff - sys_tick_old));
        }

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

static int ssi_detect_tcxo_is_normal(void)
{
    /*
     * tcxo detect依赖tcxo和32k时钟
     * 如果在启动后tcxo异常, 那么tcxo_det_res为旧值
     * 如果在启动后32k异常, 那么sys_tick为旧值
     */
    int ret;
    uint16_t sys_tick_old;
    uint32_t tcxo_det_sts, tcxo_det_val, clock_32k;
    uint32_t tcxo_det_res_old;
    uint32_t tcxo_det_value_target = TCXO_32K_DET_VALUE;
    uint64_t us_to_s;

    declare_time_cost_stru(cost);
    if (mp17c_ssi_check_subsys_is_work() == BOARD_FAIL) {
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
    sys_tick_old = ssi_read_systick();

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
    uint32_t gpio_type[CPU_BUFF] = {W_POWER, BFGX_POWER, G_POWER, CHIP_POWR};

    if (!(module_set & SSI_MODULE_MASK_ARM_REG)) {
        return;
    }
    // 不检查小核状态，否则关机找回通过soft_en拉低小核，必现下次开机上报dmd打点
    for (i = WCPU; i < COMSUB; i++) {
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
    ssi_dump_sharemem_fix();

    return OAL_SUCC;
}

static int ssi_dump_device_clkpll_regs(void)
{
    int ret;
    int i;
    uint32_t subsys_flag[CPU_BUFF] = {
        GLB_CTL_W_RB_GLB_CTL_W_GP_REG3_REG,
        GLB_CTL_B_RB_GLB_CTL_B_GP_REG3_REG,
        GLB_CTL_G_RB_GLB_CTL_G_GP_REG3_REG,
        GLB_CTL_COM_RB_GLB_CTL_COM_GP_REG3_REG
    };
    for (i = 0; i < CPU_BUFF; i++) {
        /* flag标志在，CPU肯定是work的 */
        if ((ssi_read_dword(subsys_flag[i]) & FLAG_PM_CLKPLL_ENABLE_MASK) == FLAG_PM_CLKPLL_ENABLE_MASK) {
            ps_print_info("cpu[%u] dump clkpll reg start\n", i);
            ret = ssi_read_reg_info(g_clkpll_reg_full[i], NULL, 0, g_ssi_is_logfile);
            if (ret != 0) {
                ps_print_info("dump cpu[%u] clkpll reg failed\n", i);
                return ret;
            }
            /* clkpll所有cpu配置的是同一套寄存器，有任意一个子系统dump就行 */
            break;
        }
    }
    if (i == CPU_BUFF) {
        ps_print_info("All cpu can not dump clkpll reg\n");
    }
    return 0;
}

static int ssi_dump_device_gfpll_regs(void)
{
    int ret;
    uint32_t reg_value;

    if (ssi_check_cpu_is_working(GCPU)) {
        reg_value = ssi_read_dword(LP_CTL_G_RB_LP_CTL_G_GP_REG2_REG);
        if ((reg_value == TRACE_GFPLL_START) || (reg_value == TRACE_GFPLL_LOCK_DETECT)) {
            ps_print_info("gcpu dump gfpll reg\n");
            ret = ssi_read_reg_info_arry(g_gfpll_reg_full, sizeof(g_gfpll_reg_full) /
                                         sizeof(ssi_reg_info *), g_ssi_is_logfile);
            if (ret) {
                return ret;
            }
        }
    } else {
        ps_print_info("don't need dump gfpll reg\n");
    }

    return 0;
}

typedef struct {
    enum cpu cpu_type;
    ssi_reg_info *ram_info;
    ssi_reg_info *ram_data;
    ssi_reg_info *ram_bss;
    ssi_reg_info *rom_data;
    ssi_reg_info *rom_bss;
    uint32_t offset;
} dtcm_info;

dtcm_info g_dtcm_info[] = {
    {BCPU, &g_b_plat_ram_info, &g_bcpu_plat_ram_data, &g_bcpu_plat_ram_bss,
     &g_bcpu_plat_rom_data, &g_bcpu_plat_rom_bss, SSI_ADDR_BCPU_OFFSET},
    {WCPU, &g_w_plat_ram_info, &g_wcpu_plat_ram_data, &g_wcpu_plat_ram_bss,
     &g_wcpu_plat_rom_data, &g_wcpu_plat_rom_bss, SSI_ADDR_WCPU_OFFSET},
    {GCPU, &g_g_plat_ram_info, &g_gcpu_plat_ram_data, &g_gcpu_plat_ram_bss,
     &g_gcpu_plat_rom_data, &g_gcpu_plat_rom_bss, SSI_ADDR_GCPU_OFFSET},
    {COMSUB, &g_c_plat_ram_info, &g_comsub_plat_ram_data, &g_comsub_plat_ram_bss,
     NULL, NULL, SSI_ADDR_CCPU_OFFSET},
};

void ssi_dump_device_rom_info(int sys)
{
    ssi_reg_info *rom_data = NULL;
    ssi_reg_info *rom_bss = NULL;
    int ret, i;

    if (ssi_check_cpu_is_working(sys) == OAL_FALSE) {
        ps_print_info("cpu %d is not working\n", sys);
        return;
    }

    for (i = 0; i < sizeof(g_dtcm_info) / sizeof(dtcm_info); i++) {
        if (sys == g_dtcm_info[i].cpu_type) {
            rom_data = g_dtcm_info[i].rom_data;
            rom_bss = g_dtcm_info[i].rom_bss;
            break;
        }
    }

    if (rom_data == NULL || rom_bss == NULL) {
        ps_print_info("cpu is %d. rom_info can't get, subsys type is not match\n", sys);
        return;
    }

    ps_print_info("cpu is %d. ssi dump rom data of dtcm start\n", sys);
    ret = ssi_read_reg_info(rom_data, NULL, 0, g_ssi_is_logfile);
    if (ret) {
        ps_print_info("cpu is %d rom data read fail\n", sys);
        return;
    }
    ps_print_info("cpu is %d. ssi dump rom bss of dtcm start\n", sys);
    ret = ssi_read_reg_info(rom_bss, NULL, 0, g_ssi_is_logfile);
    if (ret) {
        ps_print_info("cpu is %d rom bss read fail\n", sys);
        return;
    }
    ps_print_info("cpu is %d. ssi dump rom of dtcm end\n", sys);
}

bool is_get_ram_info_succ(int sys)
{
#define RAM_DATA_NUM 4 // 每个系统的sharemem都会记录4个U32
    uint32_t i, addr, real_times, offset;
    size_t len = sizeof(uint32_t);
    uint32_t addr_info[RAM_DATA_NUM];
    ssi_reg_info *ram_info = NULL;
    ssi_reg_info *ram_data = NULL;
    ssi_reg_info *ram_bss = NULL;

    for (i = 0; i < sizeof(g_dtcm_info) / sizeof(dtcm_info); i++) {
        if (sys == g_dtcm_info[i].cpu_type) {
            ram_info = g_dtcm_info[i].ram_info;
            ram_data = g_dtcm_info[i].ram_data;
            ram_bss = g_dtcm_info[i].ram_bss;
            offset = g_dtcm_info[i].offset;
            break;
        }
    }

    if (ram_info == NULL || ram_data == NULL || ram_bss == NULL) {
        ps_print_info("cpu is %d. ram address can't get, subsys type is error\n", sys);
        return false;
    }

    addr = ram_info->base_addr;
    real_times = ram_info->len / len;

    if (real_times != RAM_DATA_NUM) {
        ps_print_info("cpu is %d. sharemem len is not equal addr_info array len\n", sys);
        return false;
    }
    for (i = 0; i < RAM_DATA_NUM; i++) {
        addr_info[i] = ssi_read_dword(addr + i * len);
        if (addr_info[i] == 0xffffffff) {
            ps_print_info("cpu is %d. data reading error\n", sys);
            return false;
        }
    }
    ram_bss->base_addr = addr_info[0] + offset; // 获取ssi读dtcm bss段时的首地址
    ram_bss->len = addr_info[1] - addr_info[0]; // 获取ssi读dtcm bss段时的长度
    ram_data->base_addr = addr_info[2] + offset; // addr_info[2]对应data段首地址, 获取ssi读dtcm data段时的首地址
    ram_data->len = addr_info[3] - addr_info[2]; // addr_info[3]对应data段尾地址, 获取ssi读dtcm data段时的长度
    ps_print_info("cpu is %d. data 0x%x, len 0x%x, bss 0x%x, len 0x%x\n", sys, ram_data->base_addr - offset,
                  ram_data->len, ram_bss->base_addr - offset, ram_bss->len);
    return true;
}

void ssi_dump_device_ram_info(int sys)
{
    ssi_reg_info *ram_data = NULL;
    ssi_reg_info *ram_bss = NULL;
    int ret, i;

    if (ssi_check_cpu_is_working(sys) == OAL_FALSE) {
        ps_print_info("cpu %d is not working\n", sys);
        return;
    }

    if (is_get_ram_info_succ(sys) == OAL_FALSE) {
        return;
    }

    for (i = 0; i < sizeof(g_dtcm_info) / sizeof(dtcm_info); i++) {
        if (sys == g_dtcm_info[i].cpu_type) {
            ram_data = g_dtcm_info[i].ram_data;
            ram_bss = g_dtcm_info[i].ram_bss;
            break;
        }
    }

    if (ram_data == NULL || ram_bss == NULL) {
        ps_print_info("cpu is %d. ram_info can't get, subsys type is not match\n", sys);
        return;
    }

    ps_print_info("cpu is %d. ssi dump ram data of dtcm start\n", sys);
    ret = ssi_read_reg_info(ram_data, NULL, 0, g_ssi_is_logfile);
    if (ret) {
        ps_print_info("cpu is %d ram data read fail\n", sys);
        return;
    }
    ps_print_info("cpu is %d. ssi dump ram bss of dtcm start\n", sys);
    ret = ssi_read_reg_info(ram_bss, NULL, 0, g_ssi_is_logfile);
    if (ret) {
        ps_print_info("cpu is %d ram bss read fail\n", sys);
        return;
    }
    ps_print_info("cpu is %d. ssi dump ram of dtcm end\n", sys);
}

/*
 * Prototype    : ssi_dump_device_dtcm_info
 * Description  : dump dtcm for debug
 * input        : @g_dump_dtcm:  dump dtcm by mask switch, can be control by manual :
 *                  echo xxxx > /sys/module/plat_1105/parameters/g_dump_dtcm
 *                  mask value：
 *                      W_DUMP_DTCM_MSK = 0x1
 *                      B_DUMP_DTCM_MSK = 0x2
 *                      G_DUMP_DTCM_MSK = 0x4
 *                      C_DUMP_DTCM_MSK = 0x8
 */
void ssi_dump_device_dtcm_info(void)
{
    if (g_dump_dtcm & W_DUMP_DTCM_MSK) {
        ssi_dump_device_rom_info(WCPU);
        ssi_dump_device_ram_info(WCPU);
    }

    if (g_dump_dtcm & B_DUMP_DTCM_MSK) {
        ssi_dump_device_rom_info(BCPU);
        ssi_dump_device_ram_info(BCPU);
    }

    if (g_dump_dtcm & G_DUMP_DTCM_MSK) {
        ssi_dump_device_rom_info(GCPU);
        ssi_dump_device_ram_info(GCPU);
    }

    if (g_dump_dtcm & C_DUMP_DTCM_MSK) {
        ssi_dump_device_ram_info(COMSUB); // 小核不存在rom，只需要导出ram
    }
}

static int ssi_dump_device_wctrl_regs(unsigned long long module_set)
{
    int ret;

    if (module_set & SSI_MODULE_MASK_WCTRL) {
        if (ssi_check_cpu_is_working(WCPU)) {
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
    if (ssi_check_cpu_is_working(WCPU) == OAL_FALSE) {
        ps_print_info("pcie_ctrl can't dump, wcpu down\n");
        return 0;
    }

    if (module_set & SSI_MODULE_MASK_PCIE_CFG) {
        ret = ssi_read_reg_info_arry(g_wcpu_pcie_debug_full, sizeof(g_wcpu_pcie_debug_full) /
                                     sizeof(ssi_reg_info *), g_ssi_is_logfile);
        if (ret) {
            return ret;
        }

        if (ssi_check_pcie_status() == OAL_SUCC) {
            ret = ssi_read_reg_info(&g_pcie_ctrl_full, NULL, 0, g_ssi_is_logfile);
            if (ret) {
                return ret;
            }
            ret = ssi_read_reg_info(&g_pcie_ctrl_full2, NULL, 0, g_ssi_is_logfile);
            if (ret) {
                return ret;
            }
            ret = ssi_read_reg_info(&g_host_ctrl_full, NULL, 0, g_ssi_is_logfile);
            if (ret) {
                return ret;
            }

            ret = ssi_read_reg_info_arry(g_pcie_mac_ap_full, sizeof(g_pcie_mac_ap_full) /
                                        sizeof(ssi_reg_info *), g_ssi_is_logfile);
            if (ret) {
                return ret;
            }
        } else {
            ps_print_info("pcie regs can't dump, pcie is reset\n");
        }
    }
    return 0;
}

static void ssi_dump_pcie_ltssm(void)
{
    uint32_t last_addr;
    uint32_t idx;
    uint32_t val;
    last_addr = ssi_read_dword(PCIE_DBI_LTSSM_TRACER_LAST_ADDR);
    ps_print_info("SSI_DUMP_PCIE_LTSSM\n");
    ps_print_info("last addr is: 0x%x\n", (last_addr & PCIE_TRACER_LAST_WADDR_MASK));

    if ((last_addr & PCIE_TRACER_LAST_ADDR_ROLLBACK) != 0) {
        last_addr = PCIE_TRACER_LAST_ADDR_ROLLBACK;
    } else {
        last_addr &= PCIE_TRACER_LAST_WADDR_MASK;
    }

    for (idx = 0; idx < last_addr; idx++) {
        ssi_write_dword(PCIE_DBI_LTSSM_TRACERADDR, (idx & PCIE_TRACER_LAST_WADDR_MASK));
        val = ssi_read_dword(PCIE_DBI_LTSSM_TRACER_OUTPUT_1);
        ps_print_info("ltssm:%s(0x%x)\n", pcie_ctrl_get_ltssm_desc(val & PCIE_TRACER_LTSSM_MASK), val);
    }
}

static int ssi_dump_device_pcie_dbi_regs(unsigned long long module_set)
{
    int ret;
    if (ssi_check_cpu_is_working(WCPU) == OAL_FALSE) {
        ps_print_info("pcie_dbi can't dump, wcpu down\n");
        return 0;
    }

    if (module_set & SSI_MODULE_MASK_PCIE_CFG) {
        if (ssi_check_pcie_status() != OAL_SUCC) {
            ps_print_info("pcie dbi regs can't dump, pcie is reset\n");
            return 0;
        }

        if (ssi_read_dword(PCIE_DBI_DEV_ID_VEND_ID_REG) == PCIE_DEV_ID_VEND_ID_VAL_07C) {
            ssi_dump_pcie_ltssm();
            ret = ssi_read_reg_info_arry(g_pcie_mac_full, sizeof(g_pcie_mac_full) /
                                        sizeof(ssi_reg_info *), g_ssi_is_logfile);
            if (ret) {
                return ret;
            }
        } else {
            ps_print_info("pcie dbi regs can't dump\n");
        }
    }
    return 0;
}

static int ssi_dump_device_bctrl_regs(unsigned long long module_set)
{
    int ret;

    if (module_set & SSI_MODULE_MASK_BCTRL) {
        if (ssi_check_cpu_is_working(BCPU)) {
            ret = ssi_read_reg_info_arry(g_bcpu_reg_full, sizeof(g_bcpu_reg_full) /
                                         sizeof(ssi_reg_info *), g_ssi_is_logfile);
            if (ret) {
                return ret;
            }
        } else {
            ps_print_info("bctrl can't dump, bcpu down\n");
        }
    }
    return 0;
}

static int ssi_dump_device_gfctrl_regs(unsigned long long module_set)
{
    int ret;

    if (module_set & SSI_MODULE_MASK_GFCTRL) {
        if (ssi_check_cpu_is_working(GCPU)) {
            ret = ssi_read_reg_info_arry(g_gfcpu_reg_full, sizeof(g_gfcpu_reg_full) /
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

static int ssi_dump_device_comsubctrl_regs(unsigned long long module_set)
{
    int ret;

    if (module_set & SSI_MODULE_MASK_SLECTRL) {
        if (ssi_check_cpu_is_working(COMSUB)) {
            ret = ssi_read_reg_info_arry(g_comsub_reg_full, sizeof(g_comsub_reg_full) /
                                         sizeof(ssi_reg_info *), g_ssi_is_logfile);
            if (ret) {
                return ret;
            }
        } else {
            ps_print_info("slectrl can't dump, slecpu down\n");
        }
    }
    return 0;
}

static int ssi_dump_device_reg(unsigned long long module_set)
{
    int ret;

    /* detect tcxo clock is normal, trigger */
    ssi_dump_device_tcxo_regs(module_set);

    ret = ssi_dump_device_wctrl_regs(module_set);
    if (ret != OAL_SUCC) {
        return ret;
    }

    ret = ssi_dump_device_pciectrl_regs(module_set);
    if (ret != OAL_SUCC) {
        return ret;
    }

    ret = ssi_dump_device_bctrl_regs(module_set);
    if (ret != OAL_SUCC) {
        return ret;
    }

    ret = ssi_dump_device_gfctrl_regs(module_set);
    if (ret != OAL_SUCC) {
        return ret;
    }

    ret = ssi_dump_device_comsubctrl_regs(module_set);
    if (ret != OAL_SUCC) {
        return ret;
    }

    if (ssi_dump_device_clkpll_regs() != OAL_SUCC) {
        ps_print_info("dump clkpll fail!!!\n");
    }

    ret = ssi_dump_device_gfpll_regs();
    if (ret != OAL_SUCC) {
        return ret;
    }

    return ret;
}

static int32_t mp17c_ssi_check_valid(void)
{
    int32_t ret;

    ret = ssi_check_device_isalive();
    if (ret) {
        /* try to reset aon */
        /* 先复位再解复位AON，注意寄存器配置顺序 */
        (void)ssi_write16(gpio_ssi_reg(SSI_CTRL_AON_CRG_SEL), 0x300);
        (void)ssi_write16(gpio_ssi_reg(SSI_CTRL_AON_CRG_RST_EN_CLKEN), 0x0);
        oal_udelay(100); // 延时100 确保复位完成
        (void)ssi_write16(gpio_ssi_reg(SSI_CTRL_AON_CRG_RST_EN_CLKEN), 0x300);
        oal_udelay(100); // 延时100 确保复位完成
        ssi_switch_clk(SSI_AON_CLKSEL_SSI);
        if (ssi_check_device_isalive() < 0) {
            ps_print_info("after reset aon, ssi still can't work\n");
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
            hw_mpxx_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_PMU_ERROR, "aon access fail after reset");
#endif
            return OAL_FAIL;
        } else {
            ps_print_info("after reset aon, ssi ok\n");
        }
    }

    return OAL_SUCC;
}

static void ssi_dump_device_wcpu_exception_mem(void)
{
    int ret;
    ps_print_info("wcpu exception dump mem start\n");
    if (ssi_check_cpu_is_working(WCPU)) {
        ret = ssi_read_reg_info(&g_wlan_exception_mesg, NULL, 0, g_ssi_is_logfile);
        if (ret != 0) {
            ps_print_info("wcpu exception mem read failed\n");
        }
    } else {
        ps_print_info("wcpu exception mem can't dump, wcpu down\n");
    }
}

static void ssi_dump_device_bcpu_exception_mem(void)
{
    int ret;
    ps_print_info("bcpu exception dump mem start\n");
    if (ssi_check_cpu_is_working(BCPU)) {
        ret = ssi_read_reg_info(&g_bcpu_exception_mesg, NULL, 0, g_ssi_is_logfile);
        if (ret != 0) {
            ps_print_info("bcpu exception mem read failed\n");
        }
    } else {
        ps_print_info("bcpu exception mem can't dump, bcpu down\n");
    }
}

static void ssi_dump_device_gcpu_exception_mem(void)
{
    int ret;
    ps_print_info("gcpu exception dump mem start\n");
    if (ssi_check_cpu_is_working(GCPU)) {
        ret = ssi_read_reg_info(&g_gcpu_exception_mesg, NULL, 0, g_ssi_is_logfile);
        if (ret != 0) {
            ps_print_info("gcpu exception mem read failed\n");
        }
    } else {
        ps_print_info("gcpu exception mem can't dump, gcpu down\n");
    }
}

static void ssi_dump_device_comsub_exception_mem(void)
{
    int ret;
    ps_print_info("comsub exception dump mem start\n");
    if (ssi_check_cpu_is_working(COMSUB)) {
        ret = ssi_read_reg_info(&g_comsub_exception_mesg, NULL, 0, g_ssi_is_logfile);
        if (ret != 0) {
            ps_print_info("comsub exception mem read failed\n");
        }
    } else {
        ps_print_info("comsub exception mem can't dump, comsub down\n");
    }
}

static void set_bcpu_not_sleep(void)
{
    uint32_t allow_sleep_addr = 0x400804c;
    uint32_t soft_wakeup_addr = 0x4000144;

    if (get_cpu_status(BCPU) == STATUS_OFF) {
        return;
    }

    ssi_switch_clk(SSI_AON_CLKSEL_SSI);
    ssi_write_dword(allow_sleep_addr, 0);
    oal_mdelay(10); // 等待10ms生效
    ssi_write_dword(soft_wakeup_addr, 0x4); // 软中断int3唤醒
    ssi_switch_clk(SSI_AON_CLKSEL_TCXO);
}

void ssi_dump_bcpu_ram_log_mem(void)
{
    int ret;
    uint32_t base_addr = 0x1400000;
    uint32_t head_offset = 0x72cc;
    uint32_t tail_offset = 0x72d0;

    if (ssi_check_cpu_is_working(BCPU)) {
        uint32_t head, tail;
        head = (uint32_t)ssi_read32((base_addr + head_offset));
        tail = (uint32_t)ssi_read32((base_addr + tail_offset));
        ps_print_info("logbuf head: 0x%x, tail: 0x%x\n", head, tail);
        ps_print_info("bcpu ram log dump mem start\n");
        ret = ssi_read_reg_info(&g_bcpu_ram_log_mesg, NULL, 0, g_ssi_is_logfile);
        if (ret != 0) {
            ps_print_info("bcpu ram log mem read failed\n");
        }
        ps_print_info("bcpu ram log dump mem end\n");
    } else {
        ps_print_info("bcpu ram log mem can't dump, bcpu down\n");
    }
}

void ssi_dump_gcpu_ram_log_mem(void)
{
    int ret;
    uint32_t base_addr = 0x2400000;
    uint32_t head_offset = 0x5040;
    uint32_t tail_offset = 0x5044;

    if (ssi_check_cpu_is_working(GCPU)) {
        uint32_t head, tail;
        head = (uint32_t)ssi_read32((base_addr + head_offset));
        tail = (uint32_t)ssi_read32((base_addr + tail_offset));
        ps_print_info("logbuf head: 0x%x, tail: 0x%x\n", head, tail);
        ps_print_info("gcpu ram log dump mem start\n");
        ret = ssi_read_reg_info(&g_gcpu_ram_log_mesg, NULL, 0, g_ssi_is_logfile);
        if (ret != 0) {
            ps_print_info("gcpu ram log mem read failed\n");
        }
        ps_print_info("gcpu ram log dump mem end\n");
    } else {
        ps_print_info("gcpu ram log mem can't dump, gcpu down\n");
    }
}

void ssi_dump_wcpu_ram_log_mem(void)
{
    int ret;

    if (ssi_check_cpu_is_working(WCPU)) {
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
    if (module_set & SSI_MODULE_MASK_WCPU_DEVICE_LOG) {
        ssi_dump_wcpu_ram_log_mem();
    }

    if (module_set & (SSI_MODULE_MASK_BCPU_DEVICE_LOG | SSI_MODULE_MASK_OFF_FIND_LOG)) {
        ssi_dump_bcpu_ram_log_mem();
    }

    if (module_set & SSI_MODULE_MASK_GCPU_DEVICE_LOG) {
        ssi_dump_gcpu_ram_log_mem();
    }
}

static void ssi_dump_arm_exception_msg(unsigned long long module_set)
{
    ssi_dump_device_wcpu_exception_mem();
    ssi_dump_device_bcpu_exception_mem();
    ssi_dump_device_comsub_exception_mem();
    ssi_dump_device_gcpu_exception_mem();
    ssi_dump_device_oam_log(module_set);
}

static void ssi_dump_device_bcpu_trace_mem(void)
{
    int ret;
    ps_print_info("bcpu trace mem read start\n");
    ssi_write32(BT_TRACE_PC_RAM_SEL, 0x1);
    ret = ssi_read_reg_info(&g_b_trace_mem, NULL, 0, g_ssi_is_logfile);
    if (ret) {
        ps_print_info("bcpu trace mem read failed\n");
    }
}

static void ssi_dump_device_gnss_trace_mem(void)
{
    int ret;
    ps_print_info("gcpu trace mem read start\n");
    ssi_write32(GNSS_TRACE_PC_RAM_SEL, 0x1);
    ret = ssi_read_reg_info(&g_gnss_trace_mem, NULL, 0, g_ssi_is_logfile);
    if (ret) {
        ps_print_info("gfcpu trace mem read failed\n");
    }
}

static void ssi_dump_device_comsub_trace_mem(void)
{
    int ret;
    ps_print_info("comsub trace mem read start\n");
    ssi_write32(COMSUB_TRACE_PC_RAM_SEL, 0x1);
    ret = ssi_read_reg_info(&g_comsub_trace_mem, NULL, 0, g_ssi_is_logfile);
    if (ret) {
        ps_print_info("sle trace mem read failed, continue try aon\n");
    }
}

static void ssi_dump_device_wcpu_trace_mem(void)
{
    int ret;
    ps_print_info("wcpu trace mem read start\n");
    ssi_write32(WIFI_TRACE_PC_RAM_SEL, 0x1);
    ret = ssi_read_reg_info(&g_w_trace_mem, NULL, 0, g_ssi_is_logfile);
    if (ret != 0) {
        ps_print_info("wcpu trace mem read failed\n");
    }
}

static void ssi_dump_trace_memory(unsigned long long module_set, int32_t cnt)
{
    int32_t total_cnt = cnt;
    /* 仅debug版本, 通过ssi获取cpu_trace结果 */
    if (is_hi110x_debug_type() != OAL_TRUE || total_cnt <= 0) {
        ps_print_info("user mode or maybe beta user or cnt %d zero\n", total_cnt);
        return;
    }

    if ((total_cnt > 0) && ((module_set & SSI_MODULE_MASK_BTRACE) != 0x0) &&
        (ssi_check_cpu_is_working(BCPU) == OAL_TRUE)) {
        ssi_dump_device_bcpu_trace_mem();
        total_cnt--;
    }

    if ((total_cnt > 0) && ((module_set & SSI_MODULE_MASK_GTRACE) != 0x0) &&
        (ssi_check_cpu_is_working(GCPU) == OAL_TRUE)) {
        ssi_dump_device_gnss_trace_mem();
        total_cnt--;
    }

    if ((total_cnt > 0) && ((module_set & (SSI_MODULE_MASK_GTRACE |
        SSI_MODULE_MASK_BTRACE | SSI_MODULE_MASK_WTRACE)) != 0x0) &&
        (ssi_check_cpu_is_working(COMSUB) == OAL_TRUE)) {
        ssi_dump_device_comsub_trace_mem();
        total_cnt--;
    }

    if ((total_cnt > 0) && ((module_set & SSI_MODULE_MASK_WTRACE) != 0x0) &&
        (ssi_check_cpu_is_working(WCPU) == OAL_TRUE)) {
        ssi_dump_device_wcpu_trace_mem();
        total_cnt--;
    }
}

static void ssi_dump_device_cpu_trace_mem(int sys)
{
    if (sys == WCPU) {
        ssi_dump_device_wcpu_exception_mem();
        ssi_dump_device_wcpu_trace_mem();
    } else if (sys == BCPU) {
        ssi_dump_device_bcpu_exception_mem();
        ssi_dump_device_bcpu_trace_mem();
    } else if (sys == GCPU) {
        ssi_dump_device_gcpu_exception_mem();
        ssi_dump_device_gnss_trace_mem();
    } else if (sys == COMSUB) {
        ssi_dump_device_comsub_exception_mem();
        ssi_dump_device_comsub_trace_mem();
    }
}

static int mp17c_ssi_check_device_status(unsigned long long module_set)
{
    int sys, ret = OAL_SUCC;
    uint32_t device_ctl_id[][2] = {
        { W_CTL_RB_W_CTL_SYS_CTL_ID_REG, 0x10D },
        { B_CTL_RB_B_CTL_ID_REG,         0x710 },
        { G_CTL_RB_G_CTL_ID_REG,         0xE000 },
        { C_CTL_RB_C_CTL_ID_REG,         0x0 }
    };

    uint32_t soft_rst[][4] = {
        { GLBA_CTL_RB_AON_CRG_SOFT_RST_W_0_REG, 0x1ff, 0x5ff, 0x7ff },
        { GLBA_CTL_RB_AON_CRG_SOFT_RST_B_0_REG, 0x3,   0xb,   0xf },
        { GLBA_CTL_RB_AON_CRG_SOFT_RST_G_0_REG, 0xe3,  0xeb,  0xef },
        { GLBA_CTL_RB_AON_CRG_SOFT_RST_C_0_REG, 0x0,   0x2,    0x3 }
    };

    uint32_t trace_mem_clken[] = {
        W_M7_CPU_TRACE_CTL_RB_CFG_CPU_MONITOR_CLOCK_REG,
        B_M7_CPU_TRACE_CTL_RB_CFG_CPU_MONITOR_CLOCK_REG,
        G_M7_CPU_TRACE_CTL_RB_CFG_CPU_MONITOR_CLOCK_REG,
        C_M7_CPU_TRACE_CTL_RB_CFG_CPU_MONITOR_CLOCK_REG
    };

    for (sys = WCPU; sys < CPU_BUFF; sys++) {
        if (ssi_check_cpu_is_working(sys)) {
            if (ssi_read_dword(device_ctl_id[sys][0]) == device_ctl_id[sys][1]) {
                continue;
            }

            // user版本，并且未使能远程诊断功能，不复位aon
            if ((is_hi110x_debug_type() == OAL_FALSE) && (ps_is_device_log_enable() == false)) {
                return OAL_FAIL;
            }

            ps_print_err("sys[%d] bus maybe error, now reset cpu for cputrace dump\n", sys);
            // 总线异常ssi访问device_ctl，导致ssi挂掉，复位aon以配置以下soft_rst寄存器
            mp17c_ssi_check_valid();
            // 1:复位cpu/crg/lgc
            ssi_write32(soft_rst[sys][0], soft_rst[sys][1]);
            oal_udelay(10); // 10：delay10us再解复位
            // 2:解复位crg
            ssi_write32(soft_rst[sys][0], soft_rst[sys][2]);
            oal_udelay(10); // 10:soc上电时序，先解复位crg
            // 3:解复位lgc
            ssi_write32(soft_rst[sys][0], soft_rst[sys][3]);
            oal_udelay(100); // 100us 需要两拍32k时间
            ssi_write32(trace_mem_clken[sys], 0x3);
            oal_udelay(100); // 100us 等待clken稳定
            ssi_dump_device_cpu_trace_mem(sys);
            // crg/lgc复位之后，子系统寄存器恢复默认值，因此返回err不再继续dump device寄存器
            ret = OAL_FAIL;
        }
    }

    return ret;
}

int mp17c_ssi_device_regs_dump(unsigned long long module_set)
{
    int ret;
    unsigned long long reg_set = module_set;
    uint64_t dump_trace_cnt;
    uint64_t dump_total_time = SSI_DUMP_LIMIT_TIMEOUT_USER;
    declare_time_cost_stru(ssi_timestamp);

    ssi_read16(gpio_ssi_reg(SSI_SSI_CTRL));
    ssi_read16(gpio_ssi_reg(SSI_SEL_CTRL));

    ssi_check_tcxo_is_available();
    ssi_switch_clk(SSI_AON_CLKSEL_SSI);
    oal_get_time_cost_start(ssi_timestamp);
    ret = mp17c_ssi_check_valid();
    if (ret != OAL_SUCC) {
        goto ssi_fail;
    }

    if (module_set == SSI_MODULE_MASK_OFF_FIND) {
        set_bcpu_not_sleep();
    }

    ret = regs_dump(reg_set);
    if (ret != OAL_SUCC) {
        goto ssi_fail;
    }

    ret = mp17c_ssi_check_device_status(reg_set);
    if (ret != OAL_SUCC) {
        goto ssi_fail;
    }

    ret = ssi_dump_device_reg(reg_set);
    if (ret != OAL_SUCC) {
        goto ssi_fail;
    }
    ssi_dump_arm_exception_msg(module_set);

    oal_get_time_cost_end(ssi_timestamp);
    oal_calc_time_cost_sub(ssi_timestamp);
    ps_print_info("ssi dump cost %llu us\n", time_cost_var_sub(ssi_timestamp));
    oam_warning_log1(0, OAM_SF_DFR, "mp17c_ssi_device_regs_dump: before cpu trace dump, cost %d us",
                     time_cost_var_sub(ssi_timestamp));
    if (is_hi110x_debug_type() == OAL_TRUE) {
        dump_total_time = SSI_DUMP_LIMIT_TIMEOUT_DEGUG;
    }
    if ((time_cost_var_sub(ssi_timestamp)) > (dump_total_time - USEC_PER_SEC)) {
        ps_print_info("ssi dump too long skip cpu trace");
    } else {
            dump_trace_cnt = dump_total_time -(time_cost_var_sub(ssi_timestamp));
            do_div(dump_trace_cnt, USEC_PER_SEC);
            ps_print_info("ssi dump trace num %llu\n", dump_trace_cnt);
            ssi_dump_trace_memory(reg_set, (int32_t)dump_trace_cnt);
    }
    oal_get_time_cost_end(ssi_timestamp);
    oal_calc_time_cost_sub(ssi_timestamp);
    oam_warning_log1(0, OAM_SF_DFR, "mp17c_ssi_device_regs_dump: after cpu trace dump, cost %d us",
                     time_cost_var_sub(ssi_timestamp));

    /* try to read pmu regs */
    ssi_dump_pmu_regs(reg_set);
    /* pcie mac, 防止影响其他维测，故mac放在最后 */
    ssi_dump_device_pcie_dbi_regs(reg_set);

    ssi_dump_device_dtcm_info();

ssi_fail:
    ssi_switch_clk(SSI_AON_CLKSEL_TCXO);
    dsm_cpu_info_dump(module_set);
    return ret;
}

STATIC int32_t mp17c_ssi_commerial_check_panic_info(void)
{
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
    int32_t ret = 0;
    int32_t offset = 0;
    char buf[DSM_BUF_MAX_SIZE] = {0};
#endif
    int32_t i;
    uint32_t ccause, mcause, mtval, pc, lr, magic_tag;
    uint32_t sharemem_panic_info[][2] = {
        { WCPU,   0x4040440 },  // sharemem w_panic_info base addr
        { BCPU,   0x40404F8 },  // sharemem b_panic_info base addr
        { GCPU,   0x40405B0 },  // sharemem g_panic_info base addr
        { COMSUB, 0x4040668 }   // sharemem c_panic_info base addr
    };

    for (i = WCPU; i < CPU_BUFF; i++) {
        magic_tag = ssi_read_dword(sharemem_panic_info[i][1] + 0x90);   // lr: 144 Byte offset
        if (magic_tag != 0xadad) {
            continue;
        }
        if (i != WCPU) {
            ssi_write_dword((sharemem_panic_info[i][1] + 0x90), 0x0);
        }
        ccause = ssi_read_dword(sharemem_panic_info[i][1]);
        mcause = ssi_read_dword(sharemem_panic_info[i][1] + 0x4);   // mcause:  4 Byte offset
        mtval = ssi_read_dword(sharemem_panic_info[i][1] + 0x8);    // mtval:   8 Byte offset
        pc = ssi_read_dword(sharemem_panic_info[i][1] + 0x14);      // pc:      20 Byte offset
        lr = ssi_read_dword(sharemem_panic_info[i][1] + 0x8c);      // lr:      140 Byte offset

        ps_print_info("cpu%u Ccause=0x%x Mcause=0x%x Mtval=0x%x PC=0x%x LR=0x%x\n",
                      sharemem_panic_info[i][0], ccause, mcause, mtval, pc, lr);
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
        ret = snprintf_s(buf + offset, sizeof(buf) - offset, sizeof(buf) - offset - 1,
                         "cpu%u Ccause=0x%x Mcause=0x%x Mtval=0x%x PC=0x%x LR=0x%x\n",
                         sharemem_panic_info[i][0], ccause, mcause, mtval, pc, lr);
        if (ret < 0) {
            hw_mpxx_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_MPXX_HALT, "ssi check panic info fail[%d]\n", offset);
            return -EINVAL;
        }
        offset += ret;
#endif
    }
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
    if (offset != 0) {
        hw_mpxx_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_MPXX_HALT, "%s\n", buf);
    }
#endif
    return OAL_SUCC;
}

STATIC int mp17c_commerial_ssi_dump(unsigned long long module_set)
{
    int ret;

    ssi_read16(gpio_ssi_reg(SSI_SSI_CTRL));
    ssi_read16(gpio_ssi_reg(SSI_SEL_CTRL));

    // 检查tcxo在位状态
    ssi_check_tcxo_is_available();
    ssi_switch_clk(SSI_AON_CLKSEL_SSI);
    ret = mp17c_ssi_check_valid();
    if (ret != OAL_SUCC) {
        goto commerial_ssi_fail;
    }

    ret = mp17c_ssi_commerial_check_panic_info();
    if (ret != OAL_SUCC) {
        goto commerial_ssi_fail;
    }

    /* try to read pc&lr&sp regs */
    ret = ssi_dump_device_arm_regs(module_set);
    if (ret != OAL_SUCC) {
        goto commerial_ssi_fail;
    }

    msleep(50); // 延时50ms 等待device pc 运行一段时间
    ret = ssi_check_device_isalive();
    if (ret != OAL_SUCC) {
        goto commerial_ssi_fail;
    }

    /* try to read pc&lr&sp regs again */
    ret = ssi_dump_device_arm_regs(module_set);
    if (ret != OAL_SUCC) {
        goto commerial_ssi_fail;
    }

    // 检查gpio en状态
    ssi_check_en_state(module_set);

    // 检查 tcxo 频率
    ssi_dump_device_tcxo_regs(module_set);

commerial_ssi_fail:
    ssi_switch_clk(SSI_AON_CLKSEL_TCXO);
    dsm_cpu_info_dump(module_set);
    return ret;
}

STATIC int32_t mp17c_ssi_triger_panic(unsigned long long module_set)
{
    uint32_t ret;

    if (ssi_check_device_isalive() < 0) {
        return -OAL_EFAIL;
    }

    if ((module_set & SSI_MODULE_MASK_TRG_B_PANIC) == SSI_MODULE_MASK_TRG_B_PANIC) {
        ret = ssi_write_word(MP17C_GLBA_CTL_RB_B_SOFT_INT_SET_REG,
                             MP17C_GLBA_CTL_RB_B_SOFT_INT3_SET_MASK);
        if (ret == 0xffffffff) {
            ps_print_err("ssi triger b soft intr fail\n");
            return -OAL_EFAIL;
        }

        oal_udelay(50); // 50us 等待device 中断响应完成，后面可以check 是否响应完中断
        ps_print_info("read b soft intr sts 0x%x\n", ssi_read_word(MP17C_GLBA_CTL_RB_B_SOFT_INT_STS_REG));
        return OAL_SUCC;
    } else if ((module_set & SSI_MODULE_MASK_TRG_G_PANIC) == SSI_MODULE_MASK_TRG_G_PANIC) {
        ret = ssi_write_word(MP17C_GLBA_CTL_RB_G_SOFT_INT_SET_REG,
                             MP17C_GLBA_CTL_RB_G_SOFT_INT3_SET_AON_MASK);
        if (ret == 0xffffffff) {
            ps_print_err("ssi triger g soft intr fail\n");
            return -OAL_EFAIL;
        }

        oal_udelay(50); // 50us 等待device 中断响应完成，后面可以check 是否响应完中断
        ps_print_info("read g soft intr sts 0x%x\n", ssi_read_word(MP17C_GLBA_CTL_RB_G_SOFT_INT_STS_AON_REG));
        return OAL_SUCC;
    }

    return -OAL_EFAIL;
}

int mp17c_ssi_config(unsigned long long module_set)
{
    unsigned long long temp_module_set;

    temp_module_set = module_set & (SSI_MODULE_MASK_TRG_B_PANIC | SSI_MODULE_MASK_TRG_G_PANIC);
    if (temp_module_set != 0) {
        ps_print_info("triger device panic\n");
        return mp17c_ssi_triger_panic(temp_module_set);
    }

    // 商用版本限制性上报
    if (is_hi110x_debug_type() == OAL_FALSE) {
        return mp17c_commerial_ssi_dump(module_set);
    }

    return mp17c_ssi_device_regs_dump(module_set);
}

#endif /* #ifdef _PRE_CONFIG_GPIO_TO_SSI_DEBUG */
