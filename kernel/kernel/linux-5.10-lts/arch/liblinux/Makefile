# SPDX-License-Identifier: GPL-2.0-only
#
# This file is included by the global makefile to enable liblinux flags
# and dependencies.

# Compile kernel objects by -fPIC for dynamic link usage
KBUILD_CFLAGS := $(filter-out -mcmodel=%, $(KBUILD_CFLAGS))
KBUILD_CFLAGS_KERNEL += -fPIC
KBUILD_AFLAGS_KERNEL += -fPIC
KBUILD_CFLAGS += -nostdlib

#
# hostprogs may need additional processing for liblinux
#  - kallsym: place syms to .data.rel.ro instead of .rodata
KBUILD_HOSTCFLAGS += -DCONFIG_LIBLINUX=1

# generate asm-generic for liblinux
PHONY += liblinux-asm-generic
asm-generic: liblinux-asm-generic
liblinux-asm-generic:
	$(Q)$(MAKE) SRCARCH=um $(asm-generic)=arch/liblinux/include/generated/asm \
	generic=include/asm-generic

# rebuild vmlinux if override rules changed
vmlinux: $(srctree)/arch/liblinux/scripts/Makefile.override

# Prepend to override include search path
LINUXINCLUDE := -I$(srctree)/arch/liblinux/include \
		-I$(objtree)/arch/liblinux/include/generated \
		-I$(srctree)/arch/liblinux/include/uapi \
		$(LINUXINCLUDE) \
		-include liblinux/pal.h

# flags to link vmlinux as shared library
LDFLAGS_vmlinux := $(filter-out --no-undefined \
				$(call ld-option, --no-apply-dynamic-relocs), \
				$(LDFLAGS_vmlinux)) \
		   -z relro -z now -z noexecstack -z nodefaultlib

# extra object filters
LIBLINUX_EXTRA_FILTER_OBJS ?=
export __LIBLINUX_EXTRA_FILTER_OBJS := $(LIBLINUX_EXTRA_FILTER_OBJS)

# include liblinux's Kbuild
core-y += arch/liblinux/

# add soname to shared library
ifeq ($(CONFIG_SAMPLE_LDK_EXTRA_HEAD),y)
SONAME = libdh-linux.so
SOVERSION = $(VERSION)$(if $(PATCHLEVEL),.$(PATCHLEVEL))
LDFLAGS_vmlinux += -soname=$(SONAME).$(SOVERSION) \
                   --version-script=$(srctree)/samples/ldk/libdh_linux.map
include samples/ldk/Makefile.override
endif

LDFLAGS_vmlinux += -L$(objtree) -ldh-lnxbase

vmlinux: libdh-lnxbase

libdh-lnxbase: FORCE
	rm -f $(objtree)/dh-lnxbase.c $(objtree)/dh-lnxbase.o $(objtree)/libdh-lnxbase.so.1.0 $(objtree)/libdh-lnxbase.so.1 $(objtree)/libdh-lnxbase.so
	echo "" > $(objtree)/dh-lnxbase.c
	$(CC) -c $(objtree)/dh-lnxbase.c -o $(objtree)/dh-lnxbase.o
	$(CC) -fPIC -shared -o $(objtree)/libdh-lnxbase.so.1.0 -Wl,-soname,libdh-lnxbase.so.1 $(objtree)/dh-lnxbase.o
	ln -s libdh-lnxbase.so.1.0 $(objtree)/libdh-lnxbase.so.1
	ln -s libdh-lnxbase.so.1 $(objtree)/libdh-lnxbase.so

# filter vmlinux dependent libraries for modpost
DEPLIBS_vmlinux := $(foreach f,$(patsubst -l%,lib%.so,$(filter -l%,$(LDFLAGS_vmlinux))), \
			     $(shell $(patsubst --target=%,,$(CC)) --print-file-name $(f)))
export DEPLIBS_vmlinux

# link extralib if required
ifneq ($(CONFIG_SAMPLE_LDK_EXTRALIBS),"")
$(eval HMSDKSYSROOTPATH ?= $(shell $(CC) -print-sysroot))
extralibs := $(addprefix ${HMSDKSYSROOTPATH}/lib/modules/linux-$(KERNELVERSION)${CONFIG_LOCALVERSION}/lib/, \
			 $(subst ",, $(CONFIG_SAMPLE_LDK_EXTRALIBS)))
LDFLAGS_vmlinux += --whole-archive $(extralibs) --no-whole-archive
endif

# default target when executing plain make
all: dynlibs
dynlibs: vmlinux
# output for LDK extra build
ifeq ($(CONFIG_SAMPLE_LDK_EXTRA_HEAD),y)
	$(Q)$(STRIP) --strip-debug $< -o $(objtree)/samples/ldk/$(SONAME).$(KERNELRELEASE)
endif
PHONY += dynlibs
