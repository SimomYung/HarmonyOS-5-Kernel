KERNEL_DIR :=$(srctree)

EXTRA_CFLAGS += -I$(KERNEL_DIR)/drivers/platform_drivers/tzdriver
EXTRA_CFLAGS += -I$(KERNEL_DIR)/drivers/platform_drivers/tzdriver/core
EXTRA_CFLAGS += -I$(KERNEL_DIR)/drivers/platform_drivers/tzdriver/src_adaptor
EXTRA_CFLAGS += -I$(KERNEL_DIR)/drivers/platform_drivers/tzdriver/tlogger
EXTRA_CFLAGS += -I$(KERNEL_DIR)/drivers/platform_drivers/tzdriver/include
EXTRA_CFLAGS += -I$(KERNEL_DIR)/security/selinux/include
EXTRA_CFLAGS += -I$(KERNEL_DIR)/security/selinux
ifeq ($(CONFIG_LIBLINUX), y)
EXTRA_CFLAGS += -I$(KERNEL_DIR)/samples/ldk/selinux
endif
EXTRA_CFLAGS += -DCONFIG_SELINUX_AUTH_ENBALE
EXTRA_CFLAGS += -DSELINUX_TEECD_LABEL=\"u:r:teecd:s0\"
EXTRA_CFLAGS += -DSELINUX_CONTAINER_TEECD_LABEL=\"u:r:aoco_tee:s0\"

ifeq ($(CONFIG_CADAEMON_AUTH),y)
EXTRA_CFLAGS += -DCADAEMON_PATH_UID_AUTH_CTX=\"/system/bin/sa_main:6668\"
EXTRA_CFLAGS += -DSELINUX_CADAEMON_LABEL=\"u:r:cadaemon:s0\"
endif
ifeq ($(CONFIG_TZDRIVER_OHOS),y)
EXTRA_CFLAGS += -DTEECD_PATH_UID_AUTH_CTX=\"/vendor/bin/teecd:6668\"
EXTRA_CFLAGS += -DCONTAINER_TEECD_PATH_UID_AUTH_CTX=\"/vendor/bin/teecd:0\"
else
EXTRA_CFLAGS += -DTEECD_PATH_UID_AUTH_CTX=\"/vendor/bin/teecd:0\"
endif
ifeq ($(CONFIG_TZDRIVER_INTERNAL),y)
	include $(KERNEL_DIR)/drivers/platform_drivers/tzdriver/tzdriver_internal/internal.mk
endif

obj-$(CONFIG_CLIENT_AUTH) += client_hash_auth.o

ifeq ($(findstring y, $(CONFIG_TEECD_AUTH) $(CONFIG_CLIENT_AUTH)), y)
	obj-y += auth_base_impl.o auth_path_impl.o auth_selinux_label.o
endif
