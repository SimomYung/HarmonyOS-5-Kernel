ccflags-y += -Wextra -Wdate-time -Wfloat-equal -Wshadow -Wformat=2

PM_DIR :=$(srctree)/platform_source/cee/drivers/pm

EXTRA_CFLAGS += -I$(PM_DIR)

ifeq ($(CONFIG_LIBLINUX), y)
obj-$(CONFIG_SR) += suspend_hm.o
else
obj-$(CONFIG_SR) += suspend.o
obj-$(CONFIG_SYS_PM_ULSR) += ulsr.o
endif
obj-$(CONFIG_SR) += sr_mntn.o
obj-$(CONFIG_SR_SYNC) += sys_sync.o
obj-$(CONFIG_PM_LPCTRL) += lowpm_ctrl.o
obj-$(CONFIG_SYS_PM_DOZE) += doze.o
obj-$(CONFIG_SR_HIBERNATION_SUSPEND_TIME_OPP) += suspend_time_opp.o
obj-$(CONFIG_SR_HIBERNATION_WATCHDOG) += hibernate_watchdog.o
obj-$(CONFIG_SR_STAT_VOTE_IN_KERNEL) += lpm_suspend_state_vote.o
obj-$(CONFIG_SYS_PM_STR) += str.o

