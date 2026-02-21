# SPDX-License-Identifier: GPL-2.0-only

obj-y += init.o cache.o ioremap.o vmemmap.o

obj-y += pageattr.o vmalloc.o swap.o filemap.o

gfp-y := page_alloc.o
CFLAGS_page_alloc.o += -I$(srctree)/mm

# simple gfp interface wrapper
gfp-$(CONFIG_LIBLINUX_SIMPLE_GFP) += simple_gfp.o

# native buddy helper APIs
gfp-$(CONFIG_LIBLINUX_NATIVE_BUDDY) += native_gfp.o
CFLAGS_native_gfp.o += -I$(srctree)/mm

obj-y += gfp.o
obj-y += memory.o mmap.o
obj-y += shrinker.o
CFLAGS_shrinker.o += -I$(srctree)/mm

obj-y += unimpl.o truncate.o oom_kill.o

obj-$(CONFIG_CMA) += cma.o
obj-$(CONFIG_KASAN) += kasan_init.o
obj-$(CONFIG_ARM64) += dcache.o
