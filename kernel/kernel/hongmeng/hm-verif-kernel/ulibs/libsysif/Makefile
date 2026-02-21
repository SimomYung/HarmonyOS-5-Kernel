obj-y := sysmgr_client.o \
	 devmgr_client.o \
	 devhost_client.o \
	 fs_client.o \
	 pwrmgr_client.o \
	 freqmgr_client.o \
	 utils_ldso.o \
	 utils.o \
	 utils_devmgr.o \
	 srvmgr_client.o \
	 hguard_client.o \
	 crypto_client.o

obj-y += auditmgr_client.o
obj-y += keymgr_client.o
obj-$(CONFIG_HYPERVISOR) += uvmm_client.o

CFLAGS_sysmgr_server.o += $(call cc-option, -Wno-address-of-packed-member,)

# "-Wno-strict-prototypes" to solve compilation warning when string.h
# is refernced under armeb.
CFLAGS_utils_devmgr.o  += -D_GNU_SOURCE -Wno-strict-prototypes
