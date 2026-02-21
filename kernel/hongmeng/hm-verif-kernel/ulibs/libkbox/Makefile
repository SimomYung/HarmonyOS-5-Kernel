obj-y := kbox.o
obj-$(CONFIG_SNAPSHOT_COMPRESS) := kbox_snapshot.o
ifeq ($(CONFIG_AARCH64_V8_1), y)
	CFLAGS_kbox.o += -march=armv8.1-a+nolse
endif
