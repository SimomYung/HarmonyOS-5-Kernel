// SPDX-License-Identifier: GPL-2.0-only

#include <platform_include/basicplatform/linux/hck/mntn/hck_dfx_bbox.h>
#include <platform_include/basicplatform/linux/hck/mntn/hck_dfx_poweroff.h>
#ifdef CONFIG_SHUT_DETECTOR
#include <hwbootfail/core/boot_detector.h>
#include <hwbootfail/chipsets/common/bootfail_timer.h>
#endif

/*
 * Function pointers to optional machine specific functions
 */
void (*pm_power_off)(void);
EXPORT_SYMBOL_GPL(pm_power_off);

void machine_power_off(void)
{
	if (pm_power_off) {
		pm_power_off();
	}
}

void machine_restart(char *cmd)
{
	/* Now call the architecture specific reboot code. */
	int ret = false;
	CALL_HCK_VH(hck_dfx_poweroff_arm_pm_hrestart, &ret, cmd);
	if (ret) {
		pr_info("machine_restart\n");
	}
}

extern int devhost_shutdown(void);
void kernel_power_off(void)
{
#ifdef CONFIG_SHUT_DETECTOR
	int ret = set_shut_stage(SHUT_STAGE_SUCC);
	pr_info("shutfail set_shut_stage: SHUT_STAGE_SUCC ret:%d\n", ret);
	stop_boot_timer();
#endif
	pr_info("kernel power off, hm_shut_down will take over\n");
	(void)devhost_shutdown();
}
EXPORT_SYMBOL_GPL(kernel_power_off);

void kernel_restart(char *cmd)
{
#ifdef CONFIG_SHUT_DETECTOR
	int ret = set_shut_stage(SHUT_STAGE_SUCC);
	pr_info("shutfail set_shut_stage: SHUT_STAGE_SUCC ret:%d\n", ret);
	stop_boot_timer();
#endif
	pr_info("kernel restart, hm_reboot will take over\n");
	(void)liblinux_reboot(cmd, false);
}
EXPORT_SYMBOL_GPL(kernel_restart);
