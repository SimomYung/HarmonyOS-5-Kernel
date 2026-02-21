/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: pm_def.h
 * Create: 2019-10-29
 */
#ifndef PM_DEF_H
#define PM_DEF_H

#include <soc_cpu_pdc_interface.h>
#include <pmic_interface.h>
#include <m3_interrupts.h>
#ifdef CONFIG_PM_DEL_PLATFORM_ADDR
#include <lpm_kernel_map.h>
#else
#include <soc_sctrl_interface.h>
#include <m3_sram_map.h>
#endif

/*
 * NOTE!
 * must be configed together!
 * ap wakeup sys irq source
 * timer40: sr test
 * timer50: FLP
 * timer61: only for modem test
 */
#define AP_WAKE_INT_MASK (BIT(SOC_SCTRL_SCINT_MASK_intr_fer2_pmu_gic_START) | \
			BIT(SOC_SCTRL_SCINT_MASK_rtc0_int_START) | \
			BIT(SOC_SCTRL_SCINT_MASK_rtc1_int_START) | \
			BIT(SOC_SCTRL_SCINT_MASK_timer00_int_START) | \
			BIT(SOC_SCTRL_SCINT_MASK_timer50_int_START) | \
			BIT(SOC_SCTRL_SCINT_MASK_timer61_int_START))

/*
 * for SCINT_MASK1
 * IRQ_MAD
 * IRQ_GPIO1_SE_LPMCU
 * AO_IOC_COMB, no irq in lpmcu
 */
#define AP_WAKE_INT_MASK1 (BIT(SOC_SCTRL_SCINT_MASK1_mad_int_START) | \
			   BIT(SOC_SCTRL_SCINT_MASK1_se_gpio1_START) | \
			   BIT(SOC_SCTRL_SCINT_MASK1_ao_ioc_START))

#define AP_WAKE_INT_MASK3 BIT(SOC_SCTRL_SCINT_MASK3_vad_int_START)

#define AP_WAKE_INT_MASK5 BIT(SOC_SCTRL_SCINT_MASK5_intr_usb_sum_gic_lpmcu_iomcu_START)

/* for AO IPC SCINT_MASK1 */
#define AO_IPC_WAKE_INT_MASK1 (BIT(SOC_SCTRL_SCINT_MASK1_ao_ipc_mdm_START) | \
			       BIT(SOC_SCTRL_SCINT_MASK1_ao_ipc_gic_START) | \
			       BIT(SOC_SCTRL_SCINT_MASK1_ao_ipc_lpmcu_START) | \
			       BIT(SOC_SCTRL_SCINT_MASK1_ao_ipc_asp_START) | \
			       BIT(SOC_SCTRL_SCINT_MASK1_ao_ipc_iomcu_START))

/*
 * iomcu wakeup sys irq source mask
 * IRQ_IOMCU_WDT
 * IRQ_INTR_WAKEUP_IOM3
 */
#define IOMCU_WAKE_INT_MASK (BIT(SOC_SCTRL_SCINT_MASK_intr_iomcu_wdog_START) | \
			     BIT(SOC_SCTRL_SCINT_MASK_intr_wakeup_iomcu_START))

/*
 * SCINT_STAT
 * bit 9:timer01_int
 * bit 10:timer10_int
 * bit 12:timer20_int
 * bit 13:timer21_int
 * bit 14:timer30_int
 * bit 15:timer31_int
*/
#define MODEM_INT_MASK (BIT(SOC_SCTRL_SCINT_MASK_timer01_int_START) | \
			BIT(SOC_SCTRL_SCINT_MASK_timer10_int_START) | \
			BIT(SOC_SCTRL_SCINT_MASK_timer20_int_START) | \
			BIT(SOC_SCTRL_SCINT_MASK_timer21_int_START) | \
			BIT(SOC_SCTRL_SCINT_MASK_timer30_int_START) | \
			BIT(SOC_SCTRL_SCINT_MASK_timer31_int_START) | \
			BIT(SOC_SCTRL_SCINT_MASK_timer70_int_START))

#define MODEM_INT_MASK4 (BIT(SOC_SCTRL_SCINT_STAT4_lte_drx_arm_wakeup_int_START) | \
			 BIT(SOC_SCTRL_SCINT_STAT4_nrv_drx_arm_wakeup_int_START) | \
			 BIT(SOC_SCTRL_SCINT_MASK4_g1_int_gbbp_to_cpu_on_START) | \
			 BIT(SOC_SCTRL_SCINT_MASK4_g2_int_gbbp_to_cpu_on_START) | \
			 BIT(SOC_SCTRL_SCINT_MASK4_w_cpu_int02_START) | \
			 BIT(SOC_SCTRL_SCINT_MASK4_intr_sdrx_wakeup_START) | \
			 BIT(SOC_SCTRL_SCINT_MASK4_g3_int_gbbp_to_cpu_on_START) | \
			 BIT(SOC_SCTRL_SCINT_MASK4_w_cpu_int02_2_START) | \
			 BIT(SOC_SCTRL_SCINT_STAT4_lte2_drx_arm_wakeup_int_START) | \
			 BIT(SOC_SCTRL_SCINT_MASK4_ltev_drx_arm_wakeup_int_START))

#define MODEM_DRX_INT_MASK	0
/* hifi wakeup sys irq source mask */
#define HIFI_WAKE_INT_MASK (BIT(SOC_SCTRL_SCINT_MASK_intr_ipc_arm_START) | \
			    BIT(SOC_SCTRL_SCINT_MASK_intr_asp_watchdog_START))
#define MODEM_NR_INT_MASK4 (BIT(SOC_SCTRL_SCINT_STAT4_nr_drx_arm_wakeup_int_START) | \
			    BIT(SOC_SCTRL_SCINT_STAT4_nr2_drx_arm_wakeup_int_START))

#define MODEM_NR_INT_MASK5 (BIT(SOC_SCTRL_SCINT_STAT5_intr_timer140_START) | \
			    BIT(SOC_SCTRL_SCINT_STAT5_intr_timer141_START) | \
			    BIT(SOC_SCTRL_SCINT_STAT5_intr_timer150_START) | \
			    BIT(SOC_SCTRL_SCINT_STAT5_intr_timer151_START))

#define LPMCU_TICKMARK_FEATURE
/* TICK_MODEM_EARLY_INIT_ENTRY point should add if define this */
#define LPMCU_TICKMARK_MODEM_DRX_PROTECT

enum tickmark_point
{
	TICK_SYS_WAKEUP			= 6,
	TICK_SYS_NORMAL			= 8,
	TICK_DDR_AVAILABLE		= 9,
	TICK_SRAM_AVAILABLE		= 10,
	TICK_MODEM_SRAM_START		= 11,
	TICK_SYS_MAIN_IN		= 12,
	TICK_MODEM_SRAM_END		= 13,
	TICK_MODEM_UP_START		= 2,
	TICK_MODEM_UP_END		= 3,
	TICK_MODEM_DOWN_START		= 0,
	TICK_MODEM_DOWN_END		= 1,
	TICK_SLEEPFLOW_ENTER		= 14,
	TICK_MODEM_UNAVAILABLE		= 15,
	TICK_PERI_UNAVAILABLE		= 16,
	TICK_DDR_UNAVAILABLE		= 17,
	TICK_SYS_UNAVAILABLE		= 18,
	TICK_DEEPSLEEP_WFI_ENTER	= 5,
	TICK_SYS_WAKEUP_END		= 7,
	TICK_SR_DDR_OP			= 19,
	TICK_REM_DDR_OP			= 20,
	TICK_AP_WAKEUP_IRQ		= 21,
	TICK_AP_WAKEUP_RESUME		= 22,
	TICK_AP_WAKEUP_UP_BEFORE	= 23,
	TICK_AP_WAKEUP_UP_AFTER		= 24,
	TICK_USBPHY_BEFORE		= 25,
	TICK_USBPHY_AFTER		= 26,

	TICK_SYS_SUSPEND_ENTRY		= 4,
	TICK_LATSTAT_STOP_ENTRY		= 28,
	TICK_LATSTAT_STOP_EXIT		= 29,
	TICK_MODEM_SUSPEND_ENTRY	= 30,
	TICK_MODEM_SUSPEND_EXIT		= 31,
	TICK_DDR_DPM_SUSPEND_ENTRY	= 32,
	TICK_DDR_DPM_SUSPEND_EXIT	= 33,
	TICK_DDR_SUSPEND_ENTRY		= 34,
	TICK_DDR_SUSPEND_EXIT		= 35,
	TICK_PERI_FIX_DOWN_ENTRY	= 36,
	TICK_PERI_FIX_DOWN_EXIT		= 37,
	TICK_AOBUS_SUSPEND_ENTRY	= 38,
	TICK_AOBUS_SUSPEND_EXIT		= 39,
	TICK_MODEM_RESUME_PRE_ENTRY	= 40,
	TICK_MODEM_RESUME_PRE_EXIT	= 41,
	TICK_PERI_FIX_UP_ENTRY		= 42,
	TICK_PERI_FIX_UP_EXIT		= 43,
	TICK_DDR_RESUME_ENTRY		= 44,
	TICK_DDR_RESUME_EXIT		= 45,
	TICK_MODEM_RESUME_ENTRY		= 46,
	TICK_MODEM_RESUME_EXIT		= 47,
	TICK_MODEM_RESUME_AFTER_ENTRY	= 48,
	TICK_MODEM_RESUME_AFTER_EXIT	= 49,
	TICK_DDR_DPM_RESUME_ENTRY	= 50,
	TICK_DDR_DPM_RESUME_EXIT	= 51,
	TICK_AP_SUSPEND_ENTRY		= 52,
	TICK_AP_SUSPEND_EXIT		= 53,
	TICK_AP_WAKEUP_RESUME_END	= 54,
	TICK_MODEM_EARLY_INIT_ENTRY	= 55,
	TICK_32K_ENTER_ENTRY	= 56,
	TICK_32K_ENTER_CXPU_ENTRY	= 57,
	TICK_32K_ENTER_CXPU_EXIT	= 58,
	TICK_32K_ENTER_DDR_ENTRY	= 59,
	TICK_32K_ENTER_DDR_EXIT	= 60,
	TICK_32K_ENTER_SYSBUS_ENTRY	= 61,
	TICK_32K_ENTER_SYSBUS_EXIT	= 62,
	TICK_32K_ENTER_SLOW_ENTRY	= 63,
	TICK_32K_ENTER_END	= 64,
	TICK_32K_EXIT_ENTRY	= 65,
	TICK_32K_EXIT_CHECK_END	= 66,
	TICK_32K_EXIT_SLOW_END	= 67,
	TICK_32K_EXIT_WAIT_DDRPLL_STABLE_ENTRY	= 68,
	TICK_32K_EXIT_WAIT_DDRPLL_STABLE_END	= 69,
	TICK_32K_EXIT_SYSBUS_END	= 70,
	TICK_32K_EXIT_DDR_ENTRY	= 71,
	TICK_32K_EXIT_DDR_END	= 72,
	TICK_32K_EXIT_CXPU_ENTRY	= 73,
	TICK_32K_EXIT_CXPU_END	= 74,
	TICK_32K_EXIT_END	= 75,
	TICK_PRE_CHECK                      = 76,
	TICK_PRE_CHECK_EXIT                 = 77,
	TICK_WAKEUP_SET_CASE                = 78,
	TICK_DDR_SUSPEND                    = 79,
	TICK_DDR_DPM_RESUME_BEGIN           = 80,
	TICK_PERI_BACKUP_SYSPEND            = 81,
	TICK_PMU_SOC_SUSPEND                = 82,
	TICK_CLOCK_SUSPEND                  = 83,
	TICK_PMU_SUSPEND                    = 84,
	TICK_SPMI_SUSPEND                   = 85,
	TICK_WATCHDOG_DISABLE               = 86,
	TICK_CHECK_FER_ERROR                = 87,
	TICK_CHECK_BUS_ACK_ERR              = 88,
	TICK_MEMORY_DUAL_RAIL_OUT           = 89,
	TICK_LPM3_CLKINIT                   = 90,
	TICK_AOBUS_RESUME                   = 91,
	TICK_SWITCH_NORMAL                  = 92,
	TICK_CHECK_QICE_CLK_SW              = 93,
	TICK_PERI_FIX_UP                    = 94,
	TICK_RESTORE_RESUME                 = 95,
	TICK_PMU_SOC_RESUME                 = 96,
	TICK_CPU_SUBCHIP_POWER_ON           = 97,
	TICK_GIC_CLK_INIT                   = 98,
	TICK_CLOCK_RESUME                   = 99,
	TICK_TSENSOR_RESUME                 = 100,
	TICK_PERI_DVS_RESUME                = 101,
	TICK_NON_AO_LPRAM_COPY_ENTRY        = 102,
	TICK_NON_AO_LPRAM_COPY_EXIT         = 103,
	TICK_DS_RESUME_CFG_EXIT             = 104,
	TICK_SYS_RESUME                     = 105,
	TICK_SYS_RESUME_EXIT                = 106,
	TICK_MODEM_RESUME                   = 107,
	TICK_DS_RESUME_ENTRY_END            = 108,
	TICK_DDR_RESUME                     = 109,
	TICK_CLOSE_SYS_CACHE                = 110,
	TICK_IOC_RESUME                     = 111,
	TICK_GPIO_RESUME                    = 112,
	TICK_HSDT_IO_TZSP_RESUME            = 113,
	TICK_USB_RESUME                     = 114,
	TICK_UFS_RESUME                     = 115,
	SYS_DDR_SEC_REGION_SUSPEND          = 116,
	SYS_DDR_SEC_REGION_RESUME           = 117,
	TICK_REGULATOR_RESUME               = 118,
	TICK_SYS_MAIN_IN_START              = 119,
	TICK_SYS_MAIN_IN_END                = 170,
	TICK_UART_WAIT_IDLE                 = 171,
	TICK_HSDT_POWERUP                   = 172,
	TICK_SEC_REGION_SUSPEND_MODEM       = 173,
	TICK_USED_MAX,
	TICK_MARK_END_FLAG		            = TICK_USED_MAX,
	/* must be last */
	TICK_MAX = LPMCU_TELE_MNTN_DATA_TICKMARK_SIZE / 4,
};

enum sr_tick_pos {
	NO_SR = 0, /* 0 */
	KERNEL_SUSPEND_PREPARE, /* 1 */
	KERNEL_SUSPEND_IN, /* 2 */
	KERNEL_SUSPEND_SETFLAG, /* 3 */
	BL31_SUSPEND_IPC, /* 4 */
	LPMCU_AP_SUSPEND_IN, /* 5 */
	LPMCU_AP_SUSPEND_CPU, /* 6 */
	LPMCU_AP_SUSPEND_IOMCU, /* 7 */
	LPMCU_AP_SUSPEND_ASP, /* 8 */
	LPMCU_AP_SUSPEND_END, /* 9 */
	LPMCU_SYS_SUSPEND_IN, /* 10 */
	LPMCU_SYS_SUSPEND_DDRDFS, /* 11 */
	LPMCU_SYS_SUSPEND_IO, /* 12 */
	LPMCU_SYS_SUSPEND_CLK, /* 13 */
	LPMCU_SYS_SUSPEND_DDR, /* 14 */
	LPMCU_SYS_SUSPEND_UART, /* 15 */
	LPMCU_SYS_SUSPEND_AUTOFS, /* 16 */
	LPMCU_SYS_RESUME_IO, /* 17 */
	LPMCU_SYS_RESUME_AUTOFS, /* 18 */
	LPMCU_SYS_MAIN_IN, /* 19 */
	LPMCU_SYS_MAIN_END, /* 20 */
	LPMCU_AP_RESUME_IN, /* 21 */
	LPMCU_AP_RESUME_ASP, /* 22 */
	LPMCU_AP_RESUME_IOMCU, /* 23 */
	LPMCU_AP_RESUME_CPU, /* 24 */
	BL31_RESUME_IN, /* 25 */
	KERNEL_RESUME, /* 26 */
	KERNEL_RESUME_OUT, /* 27 */
	SR_ABNORMAL, /* 28 */
	SR_TICK_MAX, /* 29 */
};

#define PMUOFFSET_SR_TICK PMIC_HRST_REG11_ADDR(0)
/* gpio debug */
#define BYTES_PER_GPIO_POS 4
/* total gpio num in this plat */
#define GPIO_NUM_MAX    214
/* gpio info for each gpio (iomg_addr iomg ...) defined in pm_gpio_io_save */
#define GPIO_PER_LINE   10
/* total gpio info show in kernel */
#define GPIO_INFO_SHOW_ADDR_NUM   (GPIO_NUM_MAX * GPIO_PER_LINE)
/* ao ipc processor max */
#define AO_IPC_PROCESSOR_MAX	3

/* ao ipc mailbox max */
#define AO_NSIPC_MAILBOX_MAX	3

#define AP_IPC_PROC_BIT	1

/* no comb ipc */
#define IRQ_COMB_GIC_IPC IRQ_IPC0
#define GIC_IRQ_GROUP_START 1
#define GIC_IRQ_GROUP_MAX 22

#endif
