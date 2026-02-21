obj-$(CONFIG_MDFX_KMD) := media_dfx.o
media_dfx-objs := \
	mdfx_actor.o \
	mdfx.o \
	mdfx_dumper.o \
	mdfx_event.o \
	mdfx_file.o \
	mdfx_logger.o \
	mdfx_saver.o \
	mdfx_tracing.o \
	mdfx_utils.o \
	mdfx_visitor.o \
	mdfx_notifier.o

ifeq ($(CONFIG_MDFX_KMD_ENG_MODE),y)
media_dfx-objs += mdfx_debug.o
endif

EXTRA_CFLAGS += \
	-I$(srctree)/include/platform_include/display/linux/

clean:
	rm *.o .*cmd

