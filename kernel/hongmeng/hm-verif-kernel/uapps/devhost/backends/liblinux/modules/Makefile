obj-y += devhost/

obj-y += of/ genuio/ pramdisk/
obj-$(CONFIG_DEVHOST_TTY) += tty/

obj-$(CONFIG_DEVHOST_FS_TRANSFS) += transfs/

obj-$(CONFIG_UBIFS_CTRL) += ubifs_ctrl/

subdir-ccflags-y := ${DEVHOST_COMMON_CFLAGS}
subdir-ccflags-y += -I$(EXTLIB_SRC)/../include/
subdir-ccflags-y += -I$(EXTLIB_SRC)/devhost/
subdir-ccflags-y += -I$(EXTLIB_SRC)/
ifeq (${CONFIG_MDC_GODEL_HACK}, y)
subdir-ccflags-y += -DCONFIG_MDC_GODEL_HACK
endif
ifeq (${CONFIG_DEVHOST_NETLINK}, y)
subdir-ccflags-y += -DCONFIG_DEVHOST_NETLINK
endif
ifeq (${CONFIG_DEVHOST_PM_PLUGIN}, y)
subdir-ccflags-y += -DCONFIG_DEVHOST_PM_PLUGIN
endif
ifeq (${CONFIG_SERIAL_KSHORTCUT_SYS_PAUSE}, y)
subdir-ccflags-y += -DCONFIG_SERIAL_KSHORTCUT_SYS_PAUSE
endif
ifeq (${CONFIG_SERIAL_KSHORTCUT_TRIGGER_SNAPSHOT}, y)
subdir-ccflags-y += -DCONFIG_SERIAL_KSHORTCUT_TRIGGER_SNAPSHOT
endif
