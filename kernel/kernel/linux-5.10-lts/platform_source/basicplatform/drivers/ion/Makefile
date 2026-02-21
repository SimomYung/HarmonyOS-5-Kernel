ifeq ($(CONFIG_LIBLINUX),y)
obj-y += ion_mm_heap.o

subdir-ccflags-y += -I$(srctree)/drivers/staging/android/ion/ \
			-I$(srctree)/samples/ldk/
endif