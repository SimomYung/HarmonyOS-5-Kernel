/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Mem debug function implementation
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 27 09:57:53 2020
 */
#ifdef CONFIG_SLAB_DEBUG
#include "slab.h"
#include <libmem/slab_track.h>
#include <libmem/redzone.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <ctype.h>
#endif
#include <stdbool.h>
#include <libmem/debug.h>
#include <libhmsync/raw_mutex.h>
#include <libmem/utils.h>

static bool kmem_switch = false;
static bool debug_switch = false;
static struct raw_mutex mem_debug_mutex = RAW_MUTEX_INITIALIZER;

#ifdef CONFIG_SLAB_DEBUG
static uint32_t slab_debug;
static char slab_debug_ator[SLAB_NAME_LEN];

static char *slab_to_option_end(const char *opt)
{
	while (*opt != ' ' && *opt != '\0') {
		opt++;
	}

	return opt;
}

static void slab_parse_option(const char *opt, const char *opt_end)
{
	for (; *opt && opt < opt_end && *opt != ','; opt++) {
		switch (tolower(*opt)) {
#ifdef CONFIG_SLAB_DEBUG_USAGE
		case 'u':
			slab_debug |= SLAB_STORE_USER | SLAB_STORE_STACK;
			kmem_switch = true;
			break;
		case 's':
			slab_debug |= SLAB_STORE_USER;
			kmem_switch = true;
			break;
#endif
		default:
			hm_warn("unknown slab debug option '%c', skipped\n", *opt);
		}
	}

	if (*opt == ',' && ++opt < opt_end) {
		if (slab_debug == 0) {
#ifdef CONFIG_SLAB_DEBUG_USAGE
			slab_debug |= SLAB_STORE_USER | SLAB_STORE_STACK;
#endif
		}
		if (strncpy_s(slab_debug_ator, SLAB_NAME_LEN, opt, opt_end - opt) != EOK) {
			slab_debug = 0;
			slab_debug_ator[0] = '\0';
			hm_warn("strcpy name failed, disable debugs\n");
		} else {
			slab_debug_ator[opt_end - opt] = '\0';
		}

		kmem_switch = false;
	}
}

void slab_parse_cmdline_config(void)
{
	char cmdline[PAGE_SIZE];
	const char conf[] = "slab_debug=";
	const char path[] = "/chosen/bootargs";
	char *opt = NULL;
	char *opt_end = NULL;

	if (hm_sysctrl_read_dtb(path, sizeof(path), cmdline, PAGE_SIZE) < 0) {
		hm_info("no need to parse slab_debug option\n");
	} else {
		if ((opt = strstr(cmdline, conf)) != NULL) {
			opt = opt + strlen(conf);
			opt_end = slab_to_option_end(opt);
			slab_parse_option(opt, opt_end);
		}
	}
}

uint32_t slab_set_debug_flags(uint32_t s_flags, const char *name)
{
	uint32_t flags = s_flags;

	if (slab_debug && (!slab_debug_ator[0] || (name &&
	    strncmp(slab_debug_ator, name, strlen(slab_debug_ator)) == 0))) {
		flags |= slab_debug;
	}

	return flags;
}

void *slab_init_debug_zone(struct slab_ator *s_ator, void *slot)
{
#ifdef CONFIG_SLAB_DEBUG_USAGE
	slab_store_user_info(s_ator, slot);
#endif

	return slab_init_redzone(s_ator, slot);
}

uint32_t slab_calc_obj_size(uint32_t obj_size, uint32_t s_flags)
{
	uint32_t size;

#ifndef CONFIG_SLAB_DEBUG_USAGE
	UNUSED(s_flags);
#endif
	size = slab_redzone_calc_obj_size(obj_size);

#ifdef CONFIG_SLAB_DEBUG_USAGE
	size = slab_userzone_calc_obj_size(size, s_flags);
#endif

	return size;
}
#endif

bool fetch_debug_switch(void)
{
	return debug_switch;
}

void mem_debug_switch_open(void)
{
	raw_mutex_lock(&mem_debug_mutex);
	debug_switch = true;
	raw_mutex_unlock(&mem_debug_mutex);
}

void mem_debug_switch_close(void)
{
	raw_mutex_lock(&mem_debug_mutex);
	debug_switch = false;
	raw_mutex_unlock(&mem_debug_mutex);
}

bool fetch_kmem_switch(void)
{
	return kmem_switch;
}

#define NR_WORDS_DUMP 4
void dump_mem_content(uintptr_t addr, size_t size)
{
	uintptr_t va = ALIGN_UP(addr, sizeof(unsigned long));
	size_t align_size = ALIGN_DOWN(size, NR_WORDS_DUMP * sizeof(unsigned long));
	unsigned long words[NR_WORDS_DUMP];

	if (align_size > PAGE_SIZE) {
		align_size = PAGE_SIZE;
	}
	hm_error("=====dump %zx mem content size %zx, align_size %zx====\n", (size_t)addr, size, align_size);
	while (va < addr + align_size) {
		for (int i = 0; i < NR_WORDS_DUMP; i++) {
			 words[i] = ((unsigned long *)va)[i];
		}
		hm_error("0x%lx: %016lx %016lx %016lx %016lx\n",
			  vaddr_of((void *)va), words[0u], words[1u], words[2u], words[3u]);
		va += NR_WORDS_DUMP * sizeof(unsigned long);
	}
	hm_error("=====dump page content end=====\n");
}
