OUTPUT = .
ifneq ($(EXTLIB_OUTPUT),)
OUTPUT = $(EXTLIB_OUTPUT)
endif
objdir = $(OUTPUT)/obj

-include $(KCONFIG_CONFIG)

includedir = /usr/include

objdir := $(abspath $(objdir))
OUTPUT := $(abspath $(OUTPUT))

DESTDIR ?= $(HMSDKSYSROOTPATH)
LIBDIR ?= $(DESTDIR)/usr/lib
DOCDIR ?= $(DESTDIR)/usr/share/doc

# DEFAULT_MALLOC can be himalloc/heapmgr/musl
# otherwise himalloc will be used
DEFAULT_MALLOC := himalloc

TARGET = libc.a
TARGET-fpic = libc-fpic.a
TARGET-shared = libc.so
TARGET-shared-unstripped = libc.unstripped.so
TARGET-sys = libc-sys.a
TARGET-sys-shared = libc-sys.so
TARGET-sys-shared-unstripped = libc-sys.unstripped.so
MEMTRACE = libmemleak.a
MEMTRACE-shared = libmemleak.so.0.1
MEMTRACE-shared-unstripped = libmemleak.unstripped.so

TARGET-kasan-shared = libc_kasan.so
TARGET-kasan-shared-unstripped = libc_kasan.unstripped.so
MEM-kasan-shared = libmem_kasan.so.0.1
MEM-kasan-shared-unstripped = libmem_kasan.unstripped.so

EMPTY_LIB_NAMES = m rt pthread crypt util xnet resolv dl
EMPTY_LIB = $(EMPTY_LIB_NAMES:%=$(OUTPUT)/lib%.a)

ifeq ("$(origin V)", "command line")
  HM_LIBC_VERBOSE = $(V)
endif
ifndef HM_LIBC_VERBOSE
  HM_LIBC_VERBOSE = 0
endif

ifeq ($(HM_LIBC_VERBOSE),1)
  Q =
else
  Q = @
endif

ARCH = $(SRCARCH)

GENH = $(objdir)/include/bits/alltypes.h $(objdir)/include/bits/syscall.h $(objdir)/include/hmsyscall.h
GENH_INT = $(objdir)/include/version.h

INCLUDE_PATHS  = $(srcdir)/hm/arch/$(ARCH)/
INCLUDE_PATHS += $(srcdir)/arch/$(ARCH)
INCLUDE_PATHS += $(srcdir)/arch/generic
INCLUDE_PATHS += $(srcdir)/hm/src/internal
INCLUDE_PATHS += $(srcdir)/src/internal
INCLUDE_PATHS += $(srcdir)/src/include
INCLUDE_PATHS += $(srcdir)/hm/include
INCLUDE_PATHS += $(objdir)/include
INCLUDE_PATHS += $(srcdir)/include
INCLUDE_PATHS += $(srcdir)/src/passwd
INCLUDE_PATHS += $(HMSDKINCLUDE)

INCLUDE_PATH = $(INCLUDE_PATHS:%=-I%)

CFLAGS_AUTO = -pipe -fno-unwind-tables -fno-asynchronous-unwind-tables -ffunction-sections -fdata-sections -Werror=implicit-function-declaration -Werror=implicit-int -Werror=pointer-sign -Werror=pointer-arith -Wall -Wno-parentheses -Wno-uninitialized -Wno-missing-braces -Wno-unused-value -Wno-unused-but-set-variable -Wno-unknown-pragmas -Wno-pointer-to-int-cast -g -fstack-protector-strong -s

CFLAGS_AUTO += -fno-omit-frame-pointer -mapcs

LDFLAGS = -Wl,-z,noexecstack -Wl,-z,relro,-z,now -Wl,--sort-section,alignment -Wl,--sort-common -Wl,--gc-sections -Wl,--hash-style=both -Wl,--no-undefined -Wl,--exclude-libs=ALL -Wl,--dynamic-list=$(srcdir)/dynamic.list
LDFLAGS_ALL = $(LDFLAGS_AUTO) $(LDFLAGS)

ifdef CONFIG_CC_OPTIMIZE_FOR_SIZE
CFLAGS_AUTO	+= -Os
endif
ifdef CONFIG_CC_OPTIMIZE_O0
CFLAGS_AUTO	+= -O0
endif
ifdef CONFIG_CC_OPTIMIZE_O1
CFLAGS_AUTO	+= -O1
endif
ifdef CONFIG_CC_OPTIMIZE_O2
CFLAGS_AUTO	+= -O2
endif
ifdef CONFIG_CC_OPTIMIZE_O3
CFLAGS_AUTO	+= -O3
endif

CFLAGS_C99FSE = -std=gnu99 -ffreestanding -nostdinc -fexcess-precision=standard -frounding-math -Wa,--noexecstack
CFLAGS_MEMOPS = -fno-tree-loop-distribute-patterns
CFLAGS_NOSSP = -fno-stack-protector
CFLAGS_A32   = -march=armv7-a

CFLAGS_ALL = $(CFLAGS_C99FSE)
CFLAGS_ALL += -D_XOPEN_SOURCE=700 -DSYSLIBDIR='"$(syslibdir)"' -DLIBDIR='"$(libdir)"' $(INCLUDE_PATH)
CFLAGS_ALL += $(CFLAGS_AUTO) $(CFLAGS) $(CFLAGS_USR)
ifeq ($(DEFAULT_MALLOC),musl)
  CFLAGS_ALL += -D_DEFAULT_MALLOC
endif
ifeq ($(DEFAULT_MALLOC),himalloc)
CFLAGS_ALL += -D_MUSL_SOURCE -DOS_HONGMENG
endif
CFLAGS_ALL += -DPTHREAD_SIMPLE_CHECK

ifeq ($(ARCH),arm)
  CFLAGS_ALL += $(CFLAGS_A32)
endif

-include config.mak

# reset srcdir if there is no config.mak
srcdir ?= $(shell dirname $(shell pwd))
hmsrcdir := $(srcdir)/hm

ifeq ($(ARCH),aarch64)
ifeq ($(SUBARCH),be)
  CROSS_COMPILE ?= aarch64_be-euler-elf-
  CONFIG_BIG_ENDIAN = y
else
  CROSS_COMPILE ?= aarch64-euler-elf-
endif
endif
ifeq ($(ARCH),arm)
ifeq ($(SUBARCH),be)
  CROSS_COMPILE ?= armeb-euler-eabi-
  CONFIG_BIG_ENDIAN = y
else
  CROSS_COMPILE ?= arm-euler-eabi-
endif
endif

libsubdir =
ifeq ($(DATAMODE),ilp32)
  libsubdir = "ilp32/"
endif

export srcdir ARCH CROSS_COMPILE CONFIG_BIG_ENDIAN

include $(hmsrcdir)/scripts/Makefile.sysmgr
ifneq ($(PLAT),)
  include $(hmsrcdir)/scripts/Makefile.$(PLAT)
else
  include $(hmsrcdir)/scripts/Makefile.common
endif
include $(hmsrcdir)/scripts/Makefile.hmsdk

# add LIBC_SYS_VERSION when compile libc-sys
$(LDSO_OBJS-sys): CFLAGS_ALL += -DLIBC_SYS_VERSION
$(LNOFP_OBJS-sys): CFLAGS_ALL += -DLIBC_SYS_VERSION
ifeq ($(ARCH),aarch64)
$(LNOFP_OBJS-sys): CFLAGS_ALL += -mgeneral-regs-only
$(ANOFP_OBJS-sys): CFLAGS_ALL += -mgeneral-regs-only
endif
# dymanic-link add '-fPIC' option
$(LOBJS) $(LDSO_OBJS) $(LOBJS-sys) $(LDSO_OBJS-sys) $(MEMTRACE_OBJS) $(LNOFP_OBJS-sys): CFLAGS_ALL += -fPIC -DCONFIG_DYN_LINK
CFLAGS_MEMTRACE = -fno-omit-frame-pointer -g -O0 -rdynamic -funwind-tables -fasynchronous-unwind-tables -fstack-protector-strong

ifneq ($(USE_CLANG), yes)
AS      = $(CROSS_COMPILE)as
CC      = $(CROSS_COMPILE)gcc
AR      = $(CROSS_COMPILE)ar
RANLIB  = $(CROSS_COMPILE)ranlib
STRIP = $(CROSS_COMPILE)strip
endif
INSTALL = $(srcdir)/tools/install.sh

LLVM_RT = $(realpath $(call shell,$(CC) --sysroot=$(HMSDKSYSROOTPATH) -print-file-name=libclang_rt.builtins.a))
ifeq ($(USE_CLANG), yes)
USE_LLVM_RT = y
else
USE_LLVM_RT = $(call shell,sh -c "if test -f $(LLVM_RT) && file $(LLVM_RT) | grep -q archive; then echo y; else echo n; fi")
endif

ifeq ($(USE_LLVM_RT),n)
COMPILER-LIBS = $(call shell,$(CC) --sysroot=$(HMSDKSYSROOTPATH) -print-file-name=$(libsubdir)libgcc.a)
COMPILER-LIBS += $(call shell,$(CC) --sysroot=$(HMSDKSYSROOTPATH) -print-file-name=$(libsubdir)libgcc_eh.a)
else
COMPILER-LIBS = $(call shell,$(CC) --sysroot=$(HMSDKSYSROOTPATH) -print-file-name=libclang_rt.builtins.a)
COMPILER-LIBS += $(call shell,$(CC) --sysroot=$(HMSDKSYSROOTPATH) -print-file-name=libunwind.a)
endif

MUSL_INCLUDES = $(wildcard $(srcdir)/include/*.h $(srcdir)/include/*/*.h)
GENERIC_INCLUDES = $(wildcard $(srcdir)/arch/generic/bits/*.h)
ARCH_INCLUDES = $(wildcard $(srcdir)/arch/$(ARCH)/bits/*.h)
HM_INCLUDES = $(wildcard $(srcdir)/hm/include/*.h) $(srcdir)/hm/include/hmlibc/version.h $(wildcard $(srcdir)/hm/include/hmct/*.h) $(wildcard $(srcdir)/hm/include/hmext/*.h)
ALL_INCLUDES = $(sort $(HM_INCLUDES:$(srcdir)/hm/%=%) $(GENH:$(objdir)/%=%) $(ARCH_INCLUDES:$(srcdir)/arch/$(ARCH)/%=include/%) $(GENERIC_INCLUDES:$(srcdir)/arch/generic/%=include/%) $(MUSL_INCLUDES:$(srcdir)/%=%))

ALL_HEADER  = $(ALL_INCLUDES:include/%=$(OUTPUT)$(includedir)/%)
ALL_HEADER += $(PRIVATE_HEADERS:src/internal/%=$(OUTPUT)$(includedir)/private/%)
ALL_HEADER += $(PRIVATE_HEADERS:src/include/%=$(OUTPUT)$(includedir)/private/%)
ALL_HEADER += $(PRIVATE_HEADERS:arch/$(ARCH)/%=$(OUTPUT)$(includedir)/private/%)
ALL_HEADERS = $(filter-out $(PRIVATE_HEADERS) %/malloc.h, $(ALL_HEADER))

LOC_SRC_DIR := $(srcdir)/hm/localedata/zh_CN
LOC_SRC_DATA := $(wildcard $(LOC_SRC_DIR)/*.po)
LOC_OUT_DIR := $(OUTPUT)/locale/zh_CN/LC_MESSAGES
LOC_OUT_DATA := $(patsubst $(LOC_SRC_DIR)/%.po, $(LOC_OUT_DIR)/%.mo, $(LOC_SRC_DATA))
LOC_INSTALL_DIR := $(DESTDIR)/usr/share/locale/zh_CN/LC_MESSAGES
LOC_INSTALL_DATA := $(patsubst $(LOC_OUT_DIR)/%, $(LOC_INSTALL_DIR)/%, $(LOC_OUT_DATA))

ifeq ($(ARCH),)

all:
	@echo "Please set ARCH (eg. make ARCH=aarch64)."
	@exit 1

install-headers:
	@echo "Please set ARCH (eg. make ARCH=aarch64 install-headers)."
	@exit 1

install:
	@echo "Please set ARCH (eg. make ARCH=aarch64 install)."
	@exit 1
else

ALL_TARGETS := $(OUTPUT)/$(TARGET-sys) $(OUTPUT)/$(TARGET-sys-shared) \
	$(OUTPUT)/$(TARGET) $(OUTPUT)/$(TARGET-shared) \
	$(OUTPUT)/$(TARGET-fpic) \
	$(CRT_OBJS) $(EMPTY_LIB) $(OUTPUT)/$(MEMTRACE) \
	$(OUTPUT)/$(MEMTRACE-shared) $(LOC_OUT_DATA)

# filter kasan files, avoid impact on the normal library
LOBJS := $(filter-out ${objdir}/hm/src/kasan/kasan.lo, $(LOBJS))
LOBJS-sys := $(filter-out ${objdir}/hm/src/kasan/kasan.lo, $(LOBJS-sys))
AOBJS := $(filter-out ${objdir}/hm/src/kasan/kasan.o, $(AOBJS))
AOBJS-sys := $(filter-out ${objdir}/hm/src/kasan/kasan.o ${objdir}/hm/src/debug/poll_chk.o  ${objdir}/hm/src/debug/ppoll_chk.o, $(AOBJS-sys))

ifeq ($(LIBC_ENABLE_TSAN), y)
TSAN_EXCLUDE_SRCS = $(wildcard $(srcdir)/src/env/*.c)
TSAN_EXCLUDE_SRCS += $(wildcard $(srcdir)/src/thread/*.c)
TSAN_EXCLUDE_SRCS += $(wildcard $(srcdir)/src/stdio/*.c)
TSAN_EXCLUDE_SRCS += $(wildcard $(srcdir)/crt/*.c)
TSAN_EXCLUDE_SRCS += $(wildcard $(srcdir)/ldso/*.c)
TSAN_EXCLUDE_SRCS += $(wildcard $(srcdir)/hm/src/debug/*.c)
TSAN_EXCLUDE_SRCS += $(wildcard $(srcdir)/hm/src/himalloc/*.c)
TSAN_EXCLUDE_SRCS += $(srcdir)/hm/src/tsan/tsan.c \
	$(srcdir)/src/stat/fstat.c \
	$(srcdir)/src/stat/fstatat.c \
	$(srcdir)/src/fcntl/open.c \
	$(srcdir)/src/string/strspn.c \
	$(srcdir)/src/string/strcspn.c \
	$(srcdir)/src/string/wmemcmp.c \
	$(srcdir)/src/termios/tcsendbreak.c \
	$(srcdir)/src/conf/sysconf.c \
	$(srcdir)/hm/src/malloc/alloc_fast.c \
	$(srcdir)/hm/src/malloc/alloc_slow.c

TSAN_ALL_LOBJS = $(LDSO_OBJS) $(LOBJS)
TSAN_EXCLUDE_LOBJS = $(patsubst $(srcdir)/%.c, $(objdir)/%.lo, $(TSAN_EXCLUDE_SRCS))
TSAN_LOBJS = $(filter-out $(TSAN_EXCLUDE_LOBJS), $(TSAN_ALL_LOBJS))
$(TSAN_LOBJS): CFLAGS_ALL += $(TSAN_CFLAGS)

TSAN_ALL_LOBJS-sys = $(LDSO_OBJS-sys) $(LOBJS-sys) $(LNOFP_OBJS-sys)
TSAN_EXCLUDE_LOBJS-sys = $(patsubst $(srcdir)/%.c, $(objdir)/%.lo, $(TSAN_EXCLUDE_SRCS))
TSAN_LOBJS-sys = $(filter-out $(TSAN_EXCLUDE_LOBJS-sys), $(TSAN_ALL_LOBJS-sys))
$(TSAN_LOBJS-sys): CFLAGS_ALL += $(TSAN_CFLAGS)

TSAN_ALL_AOBJS = ${AOBJS}
TSAN_EXCLUDE_AOBJS = $(patsubst $(srcdir)/%.c, $(objdir)/%.o, $(TSAN_EXCLUDE_SRCS))
TSAN_AOBJS = $(filter-out $(TSAN_EXCLUDE_AOBJS), $(TSAN_ALL_AOBJS))
$(TSAN_AOBJS): CFLAGS_ALL += $(TSAN_CFLAGS)

TSAN_ALL_AOBJS-sys = ${AOBJS-sys} $(ANOFP_OBJS-sys)
TSAN_EXCLUDE_AOBJS-sys = $(patsubst $(srcdir)/%.c, $(objdir)/%.o, $(TSAN_EXCLUDE_SRCS))
TSAN_AOBJS-sys = $(filter-out $(TSAN_EXCLUDE_AOBJS-sys), $(TSAN_ALL_AOBJS-sys))
$(TSAN_AOBJS-sys): CFLAGS_ALL += $(TSAN_CFLAGS)

LOBJS-sys += ${objdir}/hm/src/tsan/tsan.lo
AOBJS-sys += ${objdir}/hm/src/tsan/tsan.o

else
LOBJS := $(filter-out ${objdir}/hm/src/tsan/tsan.lo, $(LOBJS))
LOBJS-sys := $(filter-out ${objdir}/hm/src/tsan/tsan.lo, $(LOBJS-sys))
AOBJS := $(filter-out ${objdir}/hm/src/tsan/tsan.o, $(AOBJS))
AOBJS-sys := $(filter-out ${objdir}/hm/src/tsan/tsan.o, $(AOBJS-sys))
endif

# Compile libc_kasan.so
ifeq ($(LIBC_ENABLE_KASAN), y)
KASAN_ALL_LOBJS = $(LDSO_OBJS) $(LOBJS) $(objdir)/hm/src/kasan/kasan.lo

# These files will be used before entering the main program.
# To avoid some problems caused by kasan, these files will disable
# kasan.
KASAN_EXCLUDE_SRCS = $(wildcard $(srcdir)/src/env/*.c)
KASAN_EXCLUDE_SRCS += $(wildcard $(srcdir)/ldso/*.c)
KASAN_EXCLUDE_SRCS += $(wildcard $(srcdir)/hm/src/himalloc/*.c)
KASAN_EXCLUDE_SRCS += $(srcdir)/hm/src/kasan/kasan.c \
	$(srcdir)/src/stat/fstat.c \
	$(srcdir)/src/stat/fstatat.c \
	$(srcdir)/src/fcntl/open.c \
	$(srcdir)/src/stdio/fopen.c \
	$(srcdir)/src/stdio/fclose.c \
	$(srcdir)/src/stdio/snprintf.c \
	$(srcdir)/src/stdio/vsprintf.c \
	$(srcdir)/src/stdio/vfprintf.c \
	$(srcdir)/src/stdio/vsnprintf.c \
	$(srcdir)/src/stdio/__uflow.c \
	$(srcdir)/src/stdio/__fdopen.c \
	$(srcdir)/src/stdio/__stdio_read.c \
	$(srcdir)/src/string/strspn.c \
	$(srcdir)/src/string/strcspn.c \
	$(srcdir)/src/string/strncmp.c \
	$(srcdir)/src/string/wmemcmp.c \
	$(srcdir)/src/thread/pthread_cond_timedwait.c \
	$(srcdir)/src/thread/pthread_mutexattr_init.c \
	$(srcdir)/src/thread/pthread_mutexattr_settype.c \
	$(srcdir)/src/thread/pthread_mutex_init.c \
	$(srcdir)/src/thread/pthread_mutexattr_destroy.c \
	$(srcdir)/src/thread/pthread_mutex_trylock.c \
	$(srcdir)/src/thread/pthread_mutex_lock.c \
	$(srcdir)/src/thread/pthread_mutex_unlock.c \
	$(srcdir)/src/thread/pthread_getspecific.c \
	$(srcdir)/src/thread/pthread_setspecific.c \
	$(srcdir)/src/thread/pthread_key_create.c \
	$(srcdir)/src/termios/tcsendbreak.c \
	$(srcdir)/src/conf/sysconf.c \
	$(srcdir)/hm/src/malloc/alloc_fast.c \
	$(srcdir)/hm/src/malloc/alloc_slow.c

KASAN_EXCLUDE_OBJS = $(patsubst $(srcdir)/%.c, $(objdir)/%.lo, $(KASAN_EXCLUDE_SRCS))
KASAN_LOBJS = $(filter-out $(KASAN_EXCLUDE_OBJS), $(KASAN_ALL_LOBJS))

$(KASAN_ALL_LOBJS): CFLAGS_ALL += -DLIBC_ENABLE_KASAN -O0 -fPIC -DCONFIG_DYN_LINK
$(KASAN_LOBJS): CFLAGS_ALL += $(KASAN_CFLAGS)

$(MEMTRACE_OBJS): CFLAGS_ALL += $(KASAN_CFLAGS)
# After adding kasan cflags for memtrace objects
MEMTRACE_OBJS += ${objdir}/hm/src/kasan/kasan.lo

ALL_TARGETS = $(OUTPUT)/$(TARGET-kasan-shared) \
	$(OUTPUT)/$(TARGET-kasan-shared-unstripped) \
	$(OUTPUT)/$(MEM-kasan-shared) \
	$(OUTPUT)/$(MEM-kasan-shared-unstripped)
endif

all: $(ALL_TARGETS)

OBJ_DIRS = $(sort $(patsubst %/,%,$(dir $(AOBJS) $(GENH) $(GENH_INT) $(LDSO_OBJS) $(CRT_OBJS) $(MEMTRACE_OBJS))) $(objdir)/include)

$(LOC_OUT_DIR):
	mkdir -p $@

$(LOC_OUT_DATA): | $(LOC_OUT_DIR)
$(LOC_OUT_DIR)/%.mo: $(LOC_SRC_DIR)/%.po
	msgfmt $< -o $@

$(LOC_INSTALL_DIR)/%: $(LOC_OUT_DIR)/%
	$(INSTALL) -D -m 644 $< $@

$(OBJ_DIRS):
	mkdir -p $@

$(AOBJS) $(GENH) $(GENH_INT) $(LDSO_OBJS) $(CRT_OBJS) $(MEMTRACE_OBJS): | $(OBJ_DIRS)

$(objdir)/include/bits/alltypes.h: $(srcdir)/arch/$(ARCH)/bits/alltypes.h.in $(srcdir)/include/alltypes.h.in $(srcdir)/tools/mkalltypes.sed
	sed -f $(srcdir)/tools/mkalltypes.sed $(srcdir)/arch/$(ARCH)/bits/alltypes.h.in $(srcdir)/include/alltypes.h.in > $@

$(objdir)/include/bits/syscall.h: $(srcdir)/arch/$(ARCH)/bits/syscall.h.in
	cp $< $@
	sed -n -e s/__NR_/SYS_/p < $< >> $@

$(objdir)/include/hmsyscall.h: $(srcdir)/hm/arch/$(ARCH)/hmsyscall.h.in
	cp $< $@

$(objdir)/include/version.h: $(wildcard $(srcdir)/VERSION $(srcdir)/.git)
	printf '#define VERSION "%s"\n' "$$(cd $(srcdir); sh tools/version.sh)" > $@

$(objdir)/src/internal/version.o $(objdir)/src/internal/version.lo: $(objdir)/include/version.h

MEMOPS_SRCS = src/string/memcpy.c src/string/memmove.c src/string/memcmp.c src/string/memset.c
$(MEMOPS_SRCS:%.c=$(objdir)/%.o): CFLAGS_ALL += $(CFLAGS_MEMOPS)

$(HM_OBJS): CFLAGS_ALL += -Werror
$(HM_OBJS-sys): CFLAGS_ALL += -Werror

NOSSP_SRCS = $(patsubst $(srcidr)/%, %, $(wildcard $(srcdir)/crt/*.c)) ldso/%.c \
	src/env/__libc_start_main.c src/env/__init_tls.c \
	src/env/__stack_chk_fail.c \
	src/thread/__set_thread_area.c src/thread/$(ARCH)/__set_thread_area.c \
	src/string/memset.c src/string/$(ARCH)/memset.c \
	src/string/memcpy.c src/string/$(ARCH)/memcpy.c \

$(NOSSP_SRCS:%.c=$(objdir)/%.o) $(NOSSP_SRCS:%.c=$(objdir)/%.lo): CFLAGS_ALL += $(CFLAGS_NOSSP)

CC_CMD = mkdir -p `dirname $@` && $(CC) $(CFLAGS_ALL) -c -o $@ $<
AS_CMD = $(CC_CMD)

ifeq ($(DEFAULT_MALLOC),heapmgr)
  $(objdir)/hm/src/malloc/alloc_fast.lo: CFLAGS_ALL += -std=gnu11 -Wall -Wextra
  $(objdir)/hm/src/malloc/alloc_slow.lo: CFLAGS_ALL += -std=gnu11 -Wall -Wextra
  $(objdir)/hm/src/malloc/backend_musl.lo: CFLAGS_ALL += -std=gnu11 -Wall -Wextra -D_GNU_SOURCE
  $(objdir)/hm/src/malloc/backend_musl.o: CFLAGS_ALL += -std=gnu11 -Wall -Wextra -D_GNU_SOURCE
  $(objdir)/hm/src/malloc/mallinfo.lo: CFLAGS_ALL += -std=gnu11 -Wall -Wextra
  $(objdir)/hm/src/malloc/mallinfo.o: CFLAGS_ALL += -std=gnu11 -Wall -Wextra
  $(objdir)/hm/src/malloc/mallopt.lo: CFLAGS_ALL += -D_GNU_SOURCE
  $(objdir)/hm/src/malloc/mallopt.o: CFLAGS_ALL += -D_GNU_SOURCE
endif

ifeq ($(DEFAULT_MALLOC),heapmgr)
  $(objdir)/hm/src/malloc/alloc_fast.o: CFLAGS_ALL += -DSTC_MEMTRACE
  $(objdir)/hm/src/malloc/alloc_slow.o: CFLAGS_ALL += -DSTC_MEMTRACE
  $(objdir)/hm/src/malloc/memtrace.o: CFLAGS_ALL += -DSTC_MEMTRACE
  $(objdir)/hm/src/malloc/memtrace.lo: CFLAGS_ALL += -DDYN_MEMTRACE
  $(objdir)/hm/memtrace/malloc.lo: CFLAGS_ALL += -DDYN_MEMTRACE
else ifeq ($(DEFAULT_MALLOC),musl)
  $(objdir)/src/malloc/malloc.o: CFLAGS_ALL += -DSTC_MEMTRACE
  $(objdir)/hm/src/malloc/memtrace.o: CFLAGS_ALL += -DSTC_MEMTRACE
  $(objdir)/hm/src/malloc/memtrace.lo: CFLAGS_ALL += -DDYN_MEMTRACE
  $(objdir)/hm/memtrace/malloc.lo: CFLAGS_ALL += -DDYN_MEMTRACE
else ifeq ($(DEFAULT_MALLOC),himalloc)
  #memleak detection has been embedded in himalloc's impl, do not need MEMTRACE macros on
  $(objdir)/hm/memtrace/memtrace.o: CFLAGS_ALL += -DHIMALLOC_MEMTRACE
  $(objdir)/hm/memtrace/memtrace.lo: CFLAGS_ALL += -DHIMALLOC_MEMTRACE
  $(objdir)/hm/src/malloc/memtrace.o: CFLAGS_ALL += -DSTC_MEMTRACE
  $(objdir)/hm/src/malloc/memtrace.lo: CFLAGS_ALL += -DDYN_MEMTRACE
endif

ifeq ($(ARCH),arm)
$(objdir)/hm/memtrace/memleak.o: CFLAGS_ALL += -DMEMTRACE_ARM
$(objdir)/hm/memtrace/memleak.lo: CFLAGS_ALL += -DMEMTRACE_ARM
$(objdir)/hm/memtrace/dump.o: CFLAGS_ALL += -DMEMTRACE_ARM
$(objdir)/hm/memtrace/dump.lo: CFLAGS_ALL += -DMEMTRACE_ARM
endif

# add _GNU_SOURCE flag
$(MEMTRACE_AOBJS): CFLAGS_ALL += -D_GNU_SOURCE
$(MEMTRACE_OBJS): CFLAGS_ALL += -D_GNU_SOURCE
ifeq ($(DEFAULT_MALLOC),himalloc)
  $(HIMALLOC_AOBJS): CFLAGS_ALL += -D_GNU_SOURCE
  $(HIMALLOC_OBJS): CFLAGS_ALL += -D_GNU_SOURCE
endif

$(objdir)/hm/src/malloc/alloc_fast.o: CFLAGS_ALL += -std=gnu11 -Wall -Wextra
$(objdir)/hm/src/malloc/alloc_slow.o: CFLAGS_ALL += -std=gnu11 -Wall -Wextra
$(objdir)/ldso/dynlink.lo: CFLAGS_ALL += -Wno-misleading-indentation
$(objdir)/hm/memtrace/dump.lo: CFLAGS_ALL += -DCOMPILE_SHARE
$(objdir)/hm/memtrace/malloc.lo: CFLAGS_ALL += -DCOMPILE_SHARE

$(CRT_OBJS): CFLAGS_ALL += -DCRT
$(CRT_OBJS): $(GENH) $(GENH_INT)

$(objdir)/lib/crt/Scrt1.o: CFLAGS_ALL += -fPIC
$(objdir)/lib/crt/rcrt1.o: CFLAGS_ALL += -fPIC

UNWIND_CFLAGS = -funwind-tables
OLD_UNWIND_PATH = $(dir $(call shell,find $(HMSDKSYSROOTPATH)/.. -name unwind.h))
ifneq ($(OLD_UNWIND_PATH),)
	UNWIND_CFLAGS += -I$(OLD_UNWIND_PATH)
endif
UNWIND_PATH = $(dir $(call shell,find $(_HMNATIVESDKSYSROOTPATH) -name unwind.h))
ifneq ($(UNWIND_PATH),)
	UNWIND_CFLAGS += -I$(UNWIND_PATH)
endif
$(objdir)/hm/src/debug/backtrace.o: CFLAGS_ALL += $(UNWIND_CFLAGS)
$(objdir)/hm/src/debug/backtrace.lo: CFLAGS_ALL += $(UNWIND_CFLAGS)
$(objdir)/hm/src/debug/backtracesyms.o: CFLAGS_ALL += -D_GNU_SOURCE
$(objdir)/hm/src/debug/backtracesyms.lo: CFLAGS_ALL += -D_GNU_SOURCE
$(objdir)/hm/src/debug/backtracesymsfd.o: CFLAGS_ALL += -D_GNU_SOURCE
$(objdir)/hm/src/debug/backtracesymsfd.lo: CFLAGS_ALL += -D_GNU_SOURCE

ifeq ($(CONFIG_NOFP), y)
$(objdir)/src/stdio/%.o: CFLAGS_ALL += -mgeneral-regs-only
$(objdir)/src/stdio/%.lo: CFLAGS_ALL += -mgeneral-regs-only
$(AOBJS) $(LOBJS): CFLAGS_ALL += -DCONFIG_NOFP=1
endif

$(objdir)/lib/%.o: $(srcdir)/%.s
	$(Q)$(AS_CMD)

$(objdir)/lib/%.o: $(srcdir)/%.c
	$(Q)$(CC_CMD)

$(objdir)/%.o: $(srcdir)/%.s
	$(Q)$(AS_CMD)

$(objdir)/%.o: $(srcdir)/%.S
	$(Q)$(CC_CMD)

$(objdir)/%.o: $(srcdir)/%.c $(GENH) $(GENH_INT)
	$(Q)$(CC_CMD)

$(objdir)/%.lo: $(srcdir)/%.s
	$(Q)$(AS_CMD)

$(objdir)/%.lo: $(srcdir)/%.S
	$(Q)$(CC_CMD)

$(objdir)/%.lo: $(srcdir)/%.c $(GENH) $(GENH_INT)
	$(Q)$(CC_CMD)

$(objdir)/ldso/%-sys.lo: $(srcdir)/ldso/%.c $(GENH) $(GENH_INT)
	$(Q)$(CC_CMD)

$(objdir)/%-sys.lo: $(srcdir)/%.c $(GENH) $(GENH_INT)
	$(Q)$(CC_CMD)

$(objdir)/%-sys.o: $(srcdir)/%.c $(GENH) $(GENH_INT)
	$(Q)$(CC_CMD)

# we need to keep the same md5 sum for different build.
# don't use RANLIB to generate random id for libhmc.a
$(OUTPUT)/$(TARGET): $(AOBJS)
	@rm -f $@
	$(Q)$(AR) rcsD $@ $(AOBJS)
	#$(Q)$(RANLIB) $@

ifeq ($(LIBC_ENABLE_KASAN_SYS), y)
$(AOBJS-sys): CFLAGS_ALL += $(KASAN_CFLAGS) -DLIBC_ENABLE_KASAN_SYS
endif
$(OUTPUT)/$(TARGET-sys): $(AOBJS-sys) $(ANOFP_OBJS-sys)
	@rm -f $@
	$(Q)$(AR) rcsD $@ $(AOBJS-sys) $(ANOFP_OBJS-sys)
	$(Q)$(RANLIB) $@

$(OUTPUT)/$(TARGET-fpic): $(LOBJS)
	@rm -f $@
	$(Q)$(AR) rcsD $@ $(LOBJS)

$(OUTPUT)/$(TARGET-shared): $(OUTPUT)/$(TARGET-shared-unstripped)
	$(Q) cp $< $@
	$(Q)$(STRIP) $(CONFIG_STRIP_OPTION) $@

$(OUTPUT)/$(TARGET-shared-unstripped): $(LOBJS) $(LDSO_OBJS)
	@rm -f $@
	$(Q)$(CC) $(CFLAGS_ALL) $(LDFLAGS_ALL) -nostdlib -shared \
		-Wl,-e,_dlstart -Wl,-soname,libc.so -o $@ $(LOBJS) $(LDSO_OBJS) $(COMPILER-LIBS)

$(OUTPUT)/$(TARGET-sys-shared): $(OUTPUT)/$(TARGET-sys-shared-unstripped)
	$(Q) cp $< $@
	$(Q)$(STRIP) $(CONFIG_STRIP_OPTION) $@

$(OUTPUT)/$(TARGET-sys-shared-unstripped): $(LOBJS-sys) $(LDSO_OBJS-sys) $(LNOFP_OBJS-sys)
	@rm -f $@
	$(Q)$(CC) $(CFLAGS_ALL) $(LDFLAGS_ALL) -nostdlib -shared \
		-Wl,-e,_dlstart -Wl,-soname,libc-sys.so -o $@ $(LOBJS-sys) $(LNOFP_OBJS-sys) $(LDSO_OBJS-sys) $(COMPILER-LIBS)

$(OUTPUT)/$(TARGET-kasan-shared): $(OUTPUT)/$(TARGET-kasan-shared-unstripped)
	$(Q) cp $< $@
	$(Q)$(STRIP) $(CONFIG_STRIP_OPTION) $@

$(OUTPUT)/$(TARGET-kasan-shared-unstripped): $(KASAN_ALL_LOBJS)
	@rm -f $@
	$(Q)$(CC) $(CFLAGS_ALL) $(LDFLAGS_ALL) -nostdlib -shared \
		-Wl,-e,_dlstart -Wl,-soname,libc_kasan.so -o $@ $(KASAN_ALL_LOBJS) $(COMPILER-LIBS)

$(OUTPUT)/$(MEMTRACE): $(MEMTRACE_AOBJS)
	@rm -f $@
	$(Q)$(AR) rcsD $@ $(MEMTRACE_AOBJS)

$(OUTPUT)/$(MEMTRACE-shared): $(OUTPUT)/$(MEMTRACE-shared-unstripped)
	$(Q) cp $< $@
	$(Q)$(STRIP) $(CONFIG_STRIP_OPTION) $@

$(OUTPUT)/$(MEMTRACE-shared-unstripped): $(MEMTRACE_OBJS) $(OUTPUT)/$(TARGET-shared)
	@rm -f $@
	$(Q)$(CC) $(CFLAGS_ALL) $(CFLAGS_MEMTRACE) $(LDFLAGS_ALL) -nostdlib -shared \
		-Wl,-soname,libmemleak.so.0 -o $@ $^ -lc $(COMPILER-LIBS) -L $(OUTPUT)

$(OUTPUT)/$(MEM-kasan-shared): $(OUTPUT)/$(MEM-kasan-shared-unstripped)
	$(Q) cp $< $@
	$(Q)$(STRIP) $(CONFIG_STRIP_OPTION) $@

$(OUTPUT)/$(MEM-kasan-shared-unstripped): $(MEMTRACE_OBJS) $(OUTPUT)/$(TARGET-kasan-shared)
	@rm -f $@
	$(Q)$(CC) $(CFLAGS_ALL) $(CFLAGS_MEMTRACE) $(LDFLAGS_ALL) -nostdlib -shared \
		-Wl,-soname,libmem.so.0 -o $@ $< -lc_kasan $(COMPILER-LIBS) -L $(OUTPUT)

$(EMPTY_LIB):
	@rm -f $@
	$(Q)$(AR) rcD $@

$(OUTPUT)$(includedir)/%: $(srcdir)/hm/include/%
	$(INSTALL) -D -m 644 $< $@

$(OUTPUT)$(includedir)/hmsyscall.h: $(objdir)/include/hmsyscall.h
	$(INSTALL) -D -m 644 $< $@

$(OUTPUT)$(includedir)/bits/%: $(objdir)/include/bits/%
	$(INSTALL) -D -m 644 $< $@

$(OUTPUT)$(includedir)/bits/%: $(srcdir)/arch/$(ARCH)/bits/%
	$(INSTALL) -D -m 644 $< $@

$(OUTPUT)$(includedir)/bits/%: $(srcdir)/arch/generic/bits/%
	$(INSTALL) -D -m 644 $< $@

$(OUTPUT)$(includedir)/private/%: $(srcdir)/src/internal/%
	$(INSTALL) -D -m 644 $< $@

$(OUTPUT)$(includedir)/private/%: $(srcdir)/src/include/%
	$(INSTALL) -D -m 644 $< $@

$(OUTPUT)$(includedir)/private/%: $(srcdir)/arch/$(ARCH)/%
	$(INSTALL) -D -m 644 $< $@

$(OUTPUT)$(includedir)/%: $(srcdir)/include/%
	$(INSTALL) -D -m 644 $< $@

# select header to be malloc.h
MALLOC_HEADERS = $(srcdir)/include/malloc.h

ifeq ($(SHADOW_STACK), yes)
SHADOW_STACK_OFFSET_HEADER = $(objdir)/include/shadow_stack_offset.h
else
SHADOW_STACK_OFFSET_HEADER =
endif

# now we don't need atomic.h, just export it as empty files
install-headers: $(ALL_HEADERS) $(SHADOW_STACK_OFFSET_HEADER)
	@$(INSTALL) -D -m 644 $(MALLOC_HEADERS) $(OUTPUT)$(includedir)/malloc.h
	@touch $(OUTPUT)$(includedir)/private/tmp
	@$(INSTALL) -D -m 644 $(OUTPUT)$(includedir)/private/tmp $(OUTPUT)$(includedir)/private/atomic.h
	@rm $(OUTPUT)$(includedir)/private/tmp
ifeq ($(SHADOW_STACK), yes)
	@$(INSTALL) -D -m 644 $(SHADOW_STACK_OFFSET_HEADER) $(OUTPUT)$(includedir)/shadow_stack_offset.h
endif

# in dynlink.c of musl, musl only recognize 'libc.so' as libc.
# so we need to setup a symbol link named 'libc.so' to 'libhmc.so'.
install: $(LOC_INSTALL_DATA)
	$(INSTALL) -D -m 644 $(hmsrcdir)/doc/api.md $(DOCDIR)/hm-libc/api.md
	$(INSTALL) -D -m 644 $(OUTPUT)/$(TARGET) $(LIBDIR)/$(TARGET)
	$(INSTALL) -D -m 644 $(OUTPUT)/$(TARGET-fpic) $(LIBDIR)/$(TARGET-fpic)
	$(INSTALL) -D -m 644 $(OUTPUT)/$(TARGET-shared) $(LIBDIR)/$(TARGET-shared)
	$(INSTALL) -D -m 644 $(OUTPUT)/$(TARGET-shared-unstripped) $(LIBDIR)/$(TARGET-shared-unstripped)
	$(INSTALL) -D -m 644 $(OUTPUT)/$(TARGET-sys) $(LIBDIR)/$(TARGET-sys)
	$(INSTALL) -D -m 644 $(OUTPUT)/$(TARGET-sys-shared) $(LIBDIR)/$(TARGET-sys-shared)
	$(INSTALL) -D -m 644 $(OUTPUT)/$(TARGET-sys-shared-unstripped) $(LIBDIR)/$(TARGET-sys-shared-unstripped)
	$(INSTALL) -D -m 644 $(OUTPUT)/$(MEMTRACE) $(LIBDIR)/$(MEMTRACE)
	$(INSTALL) -D -m 644 $(OUTPUT)/$(MEMTRACE-shared) $(LIBDIR)/$(MEMTRACE-shared)
	$(INSTALL) -D -m 644 $(OUTPUT)/$(MEMTRACE-shared-unstripped) $(LIBDIR)/$(MEMTRACE-shared-unstripped)
	$(INSTALL) -D -l libmemleak.so.0.1 $(LIBDIR)/libmemleak.so.0
	$(INSTALL) -D -l libmemleak.so.0 $(LIBDIR)/libmemleak.so
	$(INSTALL) -D -m 644 $(OUTPUT)/libm.a $(LIBDIR)/libm.a
	$(INSTALL) -D -m 644 $(OUTPUT)/librt.a $(LIBDIR)/librt.a
	$(INSTALL) -D -m 644 $(OUTPUT)/libpthread.a $(LIBDIR)/libpthread.a
	$(INSTALL) -D -m 644 $(OUTPUT)/libcrypt.a $(LIBDIR)/libcrypt.a
	$(INSTALL) -D -m 644 $(OUTPUT)/libutil.a $(LIBDIR)/libutil.a
	$(INSTALL) -D -m 644 $(OUTPUT)/libxnet.a $(LIBDIR)/libxnet.a
	$(INSTALL) -D -m 644 $(OUTPUT)/libresolv.a $(LIBDIR)/libresolv.a
	$(INSTALL) -D -m 644 $(OUTPUT)/libdl.a $(LIBDIR)/libdl.a
	$(INSTALL) -D -m 644 $(objdir)/lib/crt/crt1.o $(LIBDIR)/crt1.o
	$(INSTALL) -D -m 644 $(objdir)/lib/crt/rcrt1.o $(LIBDIR)/rcrt1.o
	$(INSTALL) -D -m 644 $(objdir)/lib/crt/Scrt1.o $(LIBDIR)/Scrt1.o
	$(INSTALL) -D -m 644 $(objdir)/lib/crt/$(ARCH)/crti.o $(LIBDIR)/crti.o
	$(INSTALL) -D -m 644 $(objdir)/lib/crt/$(ARCH)/crtn.o $(LIBDIR)/crtn.o
	ln -sf libc.so $(LIBDIR)/libhmc.so
	ln -sf libc.a  $(LIBDIR)/libhmc.a

# install libc.kasan.so
kasan_install:
	$(INSTALL) -D -m 644 $(OUTPUT)/$(TARGET-kasan-shared) $(LIBDIR)/$(TARGET-kasan-shared)
	$(INSTALL) -D -m 644 $(OUTPUT)/$(TARGET-kasan-shared-unstripped) $(LIBDIR)/$(TARGET-kasan-shared-unstripped)
	$(INSTALL) -D -m 644 $(OUTPUT)/$(MEM-kasan-shared) $(LIBDIR)/$(MEM-kasan-shared)
	$(INSTALL) -D -m 644 $(OUTPUT)/$(MEM-kasan-shared-unstripped) $(LIBDIR)/$(MEM-kasan-shared-unstripped)
	ln -sf libc_kasan.so $(LIBDIR)/libhmc_kasan.so
	$(INSTALL) -D -l libmem_kasan.so.0.1 $(LIBDIR)/libmem_kasan.so.0
	$(INSTALL) -D -l libmem_kasan.so.0 $(LIBDIR)/libmem_kasan.so

endif

check:
	@./scripts/check_cpright.sh
	@./scripts/check_header_format.sh

asm_offset: $(objdir)/include/bits/alltypes.h $(objdir)/include/bits/syscall.h
	$(CC) $(CFLAGS_ALL) -S $(srcdir)/hm/src/shadow_stack/offsets.c

ifeq ($(SHADOW_STACK), yes)
$(SHADOW_STACK_OFFSET_HEADER): asm_offset
	sed -ne 's:^[[:space:]]*offset\.ascii[[:space:]]*"##\(.*\)#\(.*\)":#define \1 /* \2 */:p;' offsets.s > $@

$(objdir)/src/setjmp/$(ARCH)/setjmp_shadow_stack.o: $(SHADOW_STACK_OFFSET_HEADER)
$(objdir)/src/setjmp/$(ARCH)/longjmp_shadow_stack.o: $(SHADOW_STACK_OFFSET_HEADER)
$(objdir)/src/setjmp/$(ARCH)/setjmp_shadow_stack.lo: $(SHADOW_STACK_OFFSET_HEADER)
$(objdir)/src/setjmp/$(ARCH)/longjmp_shadow_stack.lo: $(SHADOW_STACK_OFFSET_HEADER)
endif

clean:
	rm -rf $(objdir) $(OUTPUT)/$(TARGET) $(OUTPUT)/$(TARGET-shared) \
		$(OUTPUT)/$(TARGET-shared-unstripped) \
		$(OUTPUT)/$(TARGET-sys) \
		$(OUTPUT)/$(TARGET-sys-shared) \
		$(OUTPUT)/$(TARGET-sys-shared-unstripped) \
		$(OUTPUT)/$(TARGET-fpic) \
		$(OUTPUT)/usr $(EMPTY_LIB) \
		$(OUTPUT)/$(MEMTRACE) $(OUTPUT)/$(MEMTRACE-shared) \
		$(OUTPUT)/$(MEMTRACE-shared-unstripped) $(OUTPUT)/locale

.PHONY: all clean install-headers install check
