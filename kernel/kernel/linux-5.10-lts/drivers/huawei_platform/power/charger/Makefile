obj-$(CONFIG_HUAWEI_CHARGER_AP) += huawei_charger_common.o
obj-$(CONFIG_HUAWEI_CHARGER_AP) += huawei_charger_uevent.o

obj-$(CONFIG_HUAWEI_CHARGER_AP) += charger_ap/
obj-$(CONFIG_HUAWEI_WATCH_CHARGER) += watch_charger/

ifeq ($(TARGET_BOARD_PLATFORM), atherton)
obj-$(CONFIG_HUAWEI_WATCH_CHARGER) += regulator_ship_mode.o rtc_to_ship_mode.o
obj-y += watch_usb_vbus.o
endif