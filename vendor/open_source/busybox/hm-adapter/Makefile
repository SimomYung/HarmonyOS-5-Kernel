# User can pass the following parameters to make:
#  LINK_MODE=$(mode): $(mode) can be static or dynamic, default is static
#  BASE_CONFIG=$(defconfig): supported $(defconfig)s are in hm-adapter/configs/,
#  default is ash_defconfig.

LINK_MODE ?= static
BASE_CONFIG ?= ash_defconfig
srcdir = .

# CONFIG_MODE=$(config_mode): $(config_mode) can be oldconfig or olddefconfig
# olddefconfig is only for local sdk build which will auto use default config
# if some CONFIG_XXX is not specified in .config file
CONFIG_MODE ?= oldconfig

srcdir := $(shell dirname $(abspath $(lastword $(MAKEFILE_LIST))))

# use to change orignal config for scripts
HM_BUILD_SPEC = enable
export HM_BUILD_SPEC

ifeq ("$(origin O)", "command line")
KBUILD_OUTPUT :=$(O)
endif

KBUILD_OUTPUT := $(shell mkdir -p $(KBUILD_OUTPUT) && cd $(KBUILD_OUTPUT) && pwd)
INSTALL := install
ARCH ?= aarch64

CFLAGS += -g

ifeq ($(ARCH),aarch64)
ifeq ($(SUBARCH),be)
CROSS_COMPILE ?= aarch64_be-linux-gnu-
CFLAGS += -mbig-endian
EXTRA_TRYLINK_LDFLAGS += -Wl,-EB
else
CROSS_COMPILE ?= aarch64-linux-gnu-
endif
endif

ifeq ($(ARCH),arm)
CFLAGS += -fno-omit-frame-pointer
ifeq ($(SUBARCH),be)
CROSS_COMPILE ?= armeb-eabi-
CFLAGS += -mbig-endian
EXTRA_TRYLINK_LDFLAGS += -Wl,-EB -Wl,--be8
else
CROSS_COMPILE ?= arm-eabi-
CFLAGS += -mlittle-endian
LDFLAGS += -Wl,-EL
endif
endif

CC ?= $(CROSS_COMPILE)gcc

$(KBUILD_OUTPUT)/busyboxhm.elf: $(KBUILD_OUTPUT)/_install/bin/busybox
	cp $< $@
	cp $(KBUILD_OUTPUT)/busybox_unstripped $(KBUILD_OUTPUT)/busyboxhm_unstripped.elf

$(KBUILD_OUTPUT)/_install/bin/busybox: $(KBUILD_OUTPUT)/.config
	rm -rf $@
	+make -C ../ O=$(KBUILD_OUTPUT) CROSS_COMPILE=$(CROSS_COMPILE) CC="${CC}" \
	EXTRA_TRYLINK_LDFLAGS="$(EXTRA_TRYLINK_LDFLAGS)" V=$(V)
	+make -C ../ O=$(KBUILD_OUTPUT) CROSS_COMPILE=$(CROSS_COMPILE) CC="${CC}" \
	EXTRA_TRYLINK_LDFLAGS="$(EXTRA_TRYLINK_LDFLAGS)" V=$(V) install

INC = -I$(HMSDKINCLUDE)
INC = -I$(HMSDKINCLUDE)/generated/sysif_client/sysmgr
INC += -I$(srcdir)/include

CFLAGS += -D__hongmeng

export KBUILD_OUTPUT CC CFLAGS INC

ifeq ($(LINK_MODE), dynamic)
EXTRA_LDFLAGS += -Wl,--dynamic-linker,/lib/hmld.so.elf -Wl,--hash-style=gnu
ifeq (,$(findstring clang,$(CC)))
EXTRA_LDFLAGS += -Wl,-export-dynamic
endif
EXTRA_LDFLAGS += -Wl,-z,relro,-z,now
endif

$(KBUILD_OUTPUT)/.config: $(srcdir)/configs/$(BASE_CONFIG)
	cp $< $@
	echo 'CONFIG_EXTRA_CFLAGS="$(EXTRA_CFLAGS) $(INC)"' >> $@
	echo 'CONFIG_EXTRA_LDFLAGS="$(EXTRA_LDFLAGS)"' >> $@
	echo 'CONFIG_EXTRA_LDLIBS="$(EXTRA_LDLIBS)"' >> $@
	sed   -i 's/$$(LDFLAGS) $$(EXTRA_LDFLAGS)/$$(LDFLAGS) $$(EXTRA_LDFLAGS) $$(EXTRA_TRYLINK_LDFLAGS)/' ../Makefile
	sed   -i 's/$$(filter-out -Wl$$(comma)%,$$(LDFLAGS) $$(EXTRA_LDFLAGS))/$$(LDFLAGS) $$(EXTRA_LDFLAGS)/' ../scripts/Makefile.lib
	if test "x$(LINK_MODE)" = "xdynamic";then \
		sed -i 's/^.*CONFIG_STATIC=y.*$$/# CONFIG_STATIC is not set/' $@; \
	elif test "x$(LINK_MODE)" = "xstatic";then \
		sed -i 's/^.*CONFIG_STATIC is not set.*$$/CONFIG_STATIC=y/' $@; \
	fi
	if test "x$(ENABLE_TELNETD)" = "xyes";then \
		sed -i 's/^.*CONFIG_TELNETD.*$$/CONFIG_TELNETD=y/' $@; \
		sed -i 's/^.*CONFIG_FEATURE_TELNETD_STANDALONE.*$$/CONFIG_FEATURE_TELNETD_STANDALONE=y/' $@; \
		sed -i 's/^.*CONFIG_HM_TELNETD_WATCHCHILD.*$$/CONFIG_HM_TELNETD_WATCHCHILD=y/' $@; \
		sed -i 's/^.*CONFIG_HM_TELNETD_NONSUPPORT_LOGIN.*$$/CONFIG_HM_TELNETD_NONSUPPORT_LOGIN=y/' $@; \
		sed -i 's/^.*CONFIG_FEATURE_DEVPTS.*$$/CONFIG_FEATURE_DEVPTS=y/' $@; \
	fi
	cat $@
	+make -C ../ O=$(KBUILD_OUTPUT) CROSS_COMPILE=$(CROSS_COMPILE) CC="${CC}" V=$(V) $(CONFIG_MODE)

install:
	$(INSTALL) -d -m 0755 $(BINDIR)
	$(INSTALL) -m 0755 $(KBUILD_OUTPUT)/busyboxhm.elf $(BINDIR)
	$(INSTALL) -m 0755 $(KBUILD_OUTPUT)/busyboxhm_unstripped.elf $(BINDIR)
	$(INSTALL) -m 0755 $(KBUILD_OUTPUT)/busybox.links $(BINDIR)
	$(INSTALL) -m 0755 $(srcdir)/scripts/udhcpc.script $(BINDIR)

clean:
	rm -rf $(KBUILD_OUTPUT)
	make -C busybox clean

.PHONY: clean $(KBUILD_OUTPUT)/busyboxhm.elf $(KBUILD_OUTPUT)/_install/bin/busybox $(KBUILD_OUTPUT)/.config
