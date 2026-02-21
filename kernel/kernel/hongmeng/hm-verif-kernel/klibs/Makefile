lib-y := vsnprintf.o \
	 klog.o \
	 div.o \
	 cpio.o \
	 errno.o \
	 strsyscall.o \
	 random.o \
	 jitter.o \
	 boot_helper.o \
	 klogport.o \
	 vspace_layout.o \
	 atomic.o \
	 scq.o \
	 ekbox_rb.o

ifeq ($(CONFIG_CC_USE_CLANG),y)
subdir-asflags-y += -Wno-unused-command-line-argument
endif

subdir-asflags-y += ${KERNEL_CFLAGS_PIE}
subdir-ccflags-y += ${KERNEL_CFLAGS_PIE}

UART_OBJS-y += early_output.o
UART_OBJS-y += pl011_early_output.o
UART_OBJS-y += 8250_early_output.o
UART_OBJS-y += stm32h7_early_output.o
lib-y += $(addprefix uart/,$(UART_OBJS-y))

WDT_OBJS-y += hisi_hi1210_wdt.o
WDT_OBJS-y += hisi_sd6219e_wdt.o
lib-y += $(addprefix wdt/,$(WDT_OBJS-y))

# Next generation of early devices and drivers
lib-y += edrivers/econsole.o
lib-y += edrivers/econsole/8250.o
lib-y += edrivers/econsole/pl011.o
lib-y += edrivers/econsole/stm32h7.o
lib-y += edrivers/econsole/hisi_uart.o

lib-y += edrivers/ewatchdog.o
lib-y += edrivers/ewatchdog/hi121x.o
lib-y += edrivers/ewatchdog/sd6219e.o
lib-y += edrivers/ewatchdog/sd5151t.o
lib-y += edrivers/ewatchdog/sp805.o
lib-y += edrivers/ewatchdog/ipq6028.o

lib-$(CONFIG_KLIBS_PGTBL) += pgtbl/map.o

lib-y += edrivers/ellc.o
lib-$(CONFIG_HISI_SD7601_LLC) += edrivers/ellc/sd7601.o
lib-$(CONFIG_HMPERF_NG) += perf_rb.o
lib-$(CONFIG_HMPERF_NG) += perf_user.o

ifdef CONFIG_AARCH64
GIC_OBJS-y += arm_early_gicv3.o
endif
GIC_OBJS-y += arm_early_gicv2.o
lib-y += $(addprefix gic/,$(GIC_OBJS-y))

CFLAGS_klog.o += -Wno-type-limits

# libverified settings
VERIFIED_OBJS += priomap.o
lib-y += $(addprefix verified/,$(VERIFIED_OBJS))
lib-$(CONFIG_AARCH64) += edrivers/epmucounter/aarch64_pmucounter.o
lib-$(CONFIG_ARM) += edrivers/epmucounter/arm_pmucounter.o

# klibc settings
KLIBC_OBJS += memcpy.o \
	      memset.o \
	      memcmp.o \
	      memmove.o \
	      memchr.o \
	      strchr.o \
	      strrchr.o \
	      strcmp.o \
	      strncmp.o \
	      strncpy.o \
	      strlen.o \
	      strnlen.o \
	      strtol.o \
	      bzero.o

KLIBC_CFLAGS += -I$(srctree)/klibs/include/lib/stdinc/
ifdef CONFIG_AARCH64
KLIBC_CFLAGS += -I$(srctree)/arch/aarch64/include/
else
KLIBC_CFLAGS += -I$(srctree)/arch/arm/include/
endif

CFLAGS_memset.o    += -Wno-implicit-fallthrough
CFLAGS_memcpy.o    += $(KLIBC_CFLAGS)
CFLAGS_memset.o    += $(KLIBC_CFLAGS)
CFLAGS_memcmp.o    += $(KLIBC_CFLAGS)
CFLAGS_strcmp.o    += $(KLIBC_CFLAGS)
CFLAGS_strncmp.o   += $(KLIBC_CFLAGS)
CFLAGS_strncpy.o   += $(KLIBC_CFLAGS)
CFLAGS_strlen.o    += $(KLIBC_CFLAGS)
CFLAGS_strnlen.o   += $(KLIBC_CFLAGS)
CFLAGS_strtol.o    += $(KLIBC_CFLAGS)
CFLAGS_memmove.o   += $(KLIBC_CFLAGS)
CFLAGS_memchr.o    += $(KLIBC_CFLAGS)
CFLAGS_strchr.o    += $(KLIBC_CFLAGS)
CFLAGS_strrchr.o   += $(KLIBC_CFLAGS)

lib-y += $(addprefix klibc/,$(KLIBC_OBJS))

# Different kbuild system use different rule for per-file compiler flag
# in directory. See commit message of Linux kernel commit
# 54b8ae66ae1a: "kbuild: change *FLAGS_<basetarget>.o to take the path relative to $(obj)".
# Should switch to relative path after we finish switching to morden kbuild system.
CFLAGS_klibc/memset.o    += -Wno-implicit-fallthrough
CFLAGS_klibc/memcpy.o    += $(KLIBC_CFLAGS)
CFLAGS_klibc/memset.o    += $(KLIBC_CFLAGS)
CFLAGS_klibc/memcmp.o    += $(KLIBC_CFLAGS)
CFLAGS_klibc/strcmp.o    += $(KLIBC_CFLAGS)
CFLAGS_klibc/strncmp.o   += $(KLIBC_CFLAGS)
CFLAGS_klibc/strncpy.o   += $(KLIBC_CFLAGS)
CFLAGS_klibc/strlen.o    += $(KLIBC_CFLAGS)
CFLAGS_klibc/strnlen.o   += $(KLIBC_CFLAGS)
CFLAGS_klibc/strtol.o    += $(KLIBC_CFLAGS)
CFLAGS_klibc/memmove.o   += $(KLIBC_CFLAGS)
CFLAGS_klibc/memchr.o    += $(KLIBC_CFLAGS)
CFLAGS_klibc/strchr.o    += $(KLIBC_CFLAGS)
CFLAGS_klibc/strrchr.o   += $(KLIBC_CFLAGS)
