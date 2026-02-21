/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Mem debug function declaration
 * Author: Huawei OS Kernel Lab
 * Create: Sat Feb 22 17:09:21 2020
 */
#ifndef ULIBS_INCLUDE_LIBMEM_DEBUG_H
#define ULIBS_INCLUDE_LIBMEM_DEBUG_H

#include <stdio.h>
#include <libhmlog/hmlog.h>

#ifndef __hmbuild__
# ifdef CONFIG_DEBUG_BUILD
#  define __hmfeature_debug__
# else
#  define __hmfeature_no_debug__
# endif
#endif

void mem_debug_switch_open(void);
void mem_debug_switch_close(void);

bool fetch_debug_switch(void);
bool fetch_kmem_switch(void);

void dump_mem_content(uintptr_t addr, size_t size);

#ifdef CONFIG_SLAB_DEBUG
void slab_parse_cmdline_config(void);
uint32_t slab_set_debug_flags(uint32_t s_flags, const char *name);
void *slab_init_debug_zone(struct slab_ator *s_ator, void *slot);
uint32_t slab_calc_obj_size(uint32_t obj_size, uint32_t s_flags);
#endif

#ifdef __hmfeature_debug__
#define hmmem_debug(fmt...) \
	do { \
		if (fetch_debug_switch()) { \
			__hmlog_printf(HMLOG_DEBUG, __FILE__, __func__, __LINE__, fmt); \
		} \
	} while (1 != 1)
#else
#define hmmem_debug(fmt...) \
	do { \
		if (fetch_debug_switch()) { \
			__no_hmlog(fmt); \
		} \
	} while (1 != 1)
#endif

#ifdef __hmfeature_debug__
#define hmmem_info(fmt...) \
	do { \
		if (fetch_debug_switch()) { \
			__hmlog_printf(HMLOG_INFO, __FILE__, __func__, __LINE__, fmt); \
		} \
	} while (1 != 1)
#else
#define hmmem_info(fmt...) \
	do { \
		if (fetch_debug_switch()) { \
			__hmlog_printf(HMLOG_INFO, NULL, __func__, __LINE__, fmt); \
		} \
	} while (1 != 1)
#endif
#endif
