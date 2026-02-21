#
# Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
# Description: the device threat detection makefile
#

obj-$(CONFIG_DEVICE_THREAT_DETECTION) += \
	src/device_threat_detection_module.o \
	src/device_threat_detection_scanner.o \
	src/utils_hash.o \
	src/detection_common.o \
	src/kcode_detection.o \
	src/rodata_detection.o \
	src/rootprocs_detection.o \
	src/sehooks_detection.o \
	src/syscall_detection.o \
	src/sysrw_detection.o

ccflags-$(CONFIG_DEVICE_THREAT_DETECTION) += \
	-I$(srctree)/security/device_threat_detection/include

flags += -Wdate-time -Wfloat-equal
