obj-m += of.o

# of proxy driver need early_fdt and of_dynamic support
ifeq ($(CONFIG_OF_EARLY_FLATTREE)$(CONFIG_OF_DYNAMIC),yy)
obj-m += of.proxy.o
endif

ccflags-y += ${DEVHOST_API_CFLAGS}
ccflags-y += -nostdlib
