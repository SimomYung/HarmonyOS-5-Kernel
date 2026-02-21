#
# RPMB Makefile
#

obj-$(CONFIG_VENDOR_RPMB)			+= vendor_rpmb.o
obj-$(CONFIG_RPMB_MMC)		+= rpmb_mmc.o
obj-$(CONFIG_RPMB_UFS)		+= rpmb_ufs.o
CFLAGS_rpmb_ufs.o += -I$(srctree)/drivers/scsi/ufs/

obj-$(CONFIG_RPMB_NORFLASH) += rpmb_norflash.o

obj-$(CONFIG_RPMB_STORAGE_INTERFACE)		+= rpmb_fs.o

obj-$(CONFIG_RPMB_DEBUG_FS)		+= rpmb_debugfs.o

obj-$(CONFIG_UFS_RPMB_SUPPORT_FROM_TEEOS_IN_KERNEL) += rpmb_ufs_teeos.o
