# SPDX-License-Identifier: GPL-2.0

obj-y += dma_heap.o
CFLAGS_dma_heap.o += -I$(srctree)/platform_source/basicplatform/drivers/dma-buf/heaps/ \
		     -I$(srctree)/drivers/dma-buf/heaps

obj-y += smmu/

obj-$(CONFIG_LIBLINUX_REBOOT_ON_POWERKEY) += reboot_on_pwrkey.o

obj-y += sched_vip.o
obj-$(CONFIG_CONNECTIVITY_HI1105) += hmac_auto_adjust_aspm.o
obj-$(CONFIG_CONNECTIVITY_HI1105) += hmac_auto_cpufreq.o
obj-$(CONFIG_BLACKBOX) += bbox.o
obj-$(CONFIG_BOOT_DETECTOR) += bootfail.o

obj-$(CONFIG_SCSI_UFS_INLINE_CRYPTO) += mas_blk_inline_crypt_hook.o
obj-$(CONFIG_BLK_DEV_SD_HELPER) += sd_helper.o
CFLAGS_sd_helper.o += -include $(srctree)/drivers/scsi/ufs/ufshcd.h

obj-y += sync-file.o

obj-y += vendor_fs_interface.o

obj-$(CONFIG_DFX_ZEROHUNG) += zrhung/
obj-$(CONFIG_DFX_BB) += pstore.o
CFLAGS_pstore.o += -I$(srctree)/platform_source/basicplatform/drivers/mntn
obj-$(CONFIG_DFX_BB) += rdr_blackbox.o
CFLAGS_rdr_blackbox.o += -I$(srctree)/platform_source/basicplatform/drivers/mntn
