KBUILD_CFLAGS += -I$(srctree)/modules/include \
		 -I$(srctree)/modules/include/mapi \
		 -D__MODULE__

obj-y += watchdog/ futex/
obj-$(CONFIG_DEBUG_BUILD) += test/
obj-$(CONFIG_MODULE_TIMER) += timer/
obj-$(CONFIG_MODULE_MISC) += misc/
obj-$(CONFIG_MODULE_ATFD_SMC) += atfd/
obj-$(CONFIG_MODULE_FAULT_INJECT) += fault_inject/
obj-$(CONFIG_RT_FUTEX) += rtfutex/
obj-$(CONFIG_AUDIT) += audit/
subdir-asflags-y := $(KERNEL_CFLAGS_PIE)
subdir-ccflags-y := $(KERNEL_CFLAGS_PIE)
subdir-ccflags-$(CONFIG_MODULE_UTSAN) += -fsanitize=thread
