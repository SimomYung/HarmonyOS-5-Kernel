# SPDX-License-Identifier: GPL-2.0
obj-m += test_find_vma.o
obj-m += test_vmalloc_remap.o
obj-m += test_liblinux_exit.o
obj-m += test_signal.o
obj-m += test_vmallocinfo.o
ifdef CONFIG_KUNIT
obj-m += test_kthread.o
endif

ifeq ($(CONFIG_SAMPLE_LDK_TEST_KSYS), y)
obj-y := test_procfs.o
obj-y += test_export.o
obj-m += test_ksys.o
obj-m += test_ldk.o
test_ldk-objs += test_ldk_main.o
test_ldk-objs += test_sysfs.o
test_ldk-objs += test_kernel.o
test_ldk-objs += test_hvc.o
test_ldk-objs += test_kthread_unpark.o
endif
