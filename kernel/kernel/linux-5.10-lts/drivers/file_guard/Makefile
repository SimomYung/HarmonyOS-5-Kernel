ifeq ($(CONFIG_LIBLINUX), y)
obj-$(CONFIG_FILEGUARD_IPCWORK) += netfilter_hm/
else
obj-$(CONFIG_FILEGUARD_NETWORK) += ipcdriver/
obj-$(CONFIG_FILEGUARD_IPCWORK) += netfilter/
obj-$(CONFIG_FILEGUARD_IPCWORK) += fsfilter/
endif
