ccflags-y += -Wextra -Wdate-time -Wfloat-equal -Wshadow -Wformat=2

PM_DIR :=$(srctree)/platform_source/cee/drivers/pm

EXTRA_CFLAGS += -I$(PM_DIR)

obj-y += ap_wake.o
obj-y += gpio_wake.o

ifeq ($(CONFIG_SR_MNTN_AP_WAKE_V100),y)
obj-y += ipc_wake_v100.o
else
obj-y += ipc_wake.o
endif
