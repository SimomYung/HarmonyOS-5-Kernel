# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the phy drivers.
#

ccflags-y += -Wdate-time -Wextra -Wfloat-equal -Wformat=2 -Wshadow
obj-y += hisilicon/
obj-$(CONFIG_PHY_HISI_USB2_T5) += chip-usb2-phy-t5.o
obj-$(CONFIG_PHY_HISI_USB3_LITE) += chip_usb3_lite_phy.o
obj-$(CONFIG_PHY_USB2_HI6502) += chip_usb2_hi6502_phy.o
obj-$(CONFIG_PHY_USB2_EUSB) += chip_usb2_eusb_phy.o
obj-$(CONFIG_PHY_USB4_COMBOPHY) += phy_usb4_combophy.o
ifeq ($(CONFIG_PHY_USB4_COMBOPHY), y)
	obj-$(CONFIG_DFX_DEBUG_FS)      += phy_usb4_combophy_dbgfs.o
endif
ifeq ($(CONFIG_PHY_USB2_EUSB), y)
        obj-$(CONFIG_DFX_DEBUG_FS)      += chip_usb2_eusb_phy_dbgfs.o
endif
