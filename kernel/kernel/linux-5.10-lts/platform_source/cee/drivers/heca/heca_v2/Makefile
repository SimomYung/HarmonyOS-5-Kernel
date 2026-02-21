HECA_ROOT := $(srctree)/platform_source/cee/drivers/heca
HECA_INC_PATH := $(HECA_ROOT)

ccflags-y += -I$(HECA_INC_PATH)
obj-$(CONFIG_HECA_V2_LTS) += heca_v2_lts.o
obj-$(CONFIG_HECA_GPU_LTS) += heca_gpu_lts.o