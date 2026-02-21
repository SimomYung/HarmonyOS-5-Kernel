obj-$(CONFIG_L3CACHE_SHARE)        += l3cache_share.o
obj-$(CONFIG_L3CACHE_EXTENSION)    += l3cache_extension.o
obj-$(CONFIG_L3CACHE_PARTITION_CTRL)    += l3cache_partition_ctrl.o
obj-$(CONFIG_NAMTSO_FLUSH_CMO)     += namtso_flush_cmo.o
obj-$(CONFIG_NONSHAREABLE)         += nonshareable.o
obj-$(CONFIG_L3CACHE_LP_DEBUG)     += l3cache_lp_debug.o
obj-$(CONFIG_NAMTSO_DEBUG)         += namtso_debug.o amu_test.o
obj-$(CONFIG_HYBRID_CACHE_DEBUG)   += hybrid_cache_debug.o
obj-$(CONFIG_CACHE_GM_TEST)    += cache_gm_test.o

ccflags-y += -Wextra -Wdate-time -Wfloat-equal -Wshadow
ccflags-$(CONFIG_NAMTSO_DEBUG) += -I$(srctree)/drivers/staging/android/ion
