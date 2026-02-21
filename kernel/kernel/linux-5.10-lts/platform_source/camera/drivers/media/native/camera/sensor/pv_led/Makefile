#
# Makefile for camera pvled driver
#
EXTRA_CFLAGS += -fno-pic

ifeq ($(TARGET_VERSION_MODE), factory)
EXTRA_CFLAGS += -DPVLED_FACTORY_MODE
endif

obj-y += pv_led.o