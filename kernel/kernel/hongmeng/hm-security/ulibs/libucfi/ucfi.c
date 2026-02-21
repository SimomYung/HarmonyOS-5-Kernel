/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Init and manage bitmap for ucfi
 * Author: Huawei OS Kernel Lab
 * Create: Web Mar 30 17:41:11 2022
 */

#include <dlfcn.h>
#include <link.h>
#include <sys/resource.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhwsecurec/securec.h>
#include <libmem/utils.h>

#include "ucfi.h"

#ifdef __HOST_LLT__
#define STATIC
#define INLINE
#else
#define STATIC static
#define INLINE inline
#endif

#define UCFI_BITMAP_SCALE_SIZE 8

struct ucfi_bitmap_info {
	struct rlimit rlim;
	size_t sz;
	uint8_t *bitmap;
};

STATIC bool ucfi_inited = false;
STATIC struct ucfi_bitmap_info ucfi_bitmap;
STATIC struct raw_mutex g_bitmap_lock = RAW_MUTEX_INITIALIZER;

#define ucfi_addr_2_index(addr) \
	((addr) / ((PAGE_SIZE) * UCFI_BITMAP_SCALE_SIZE))

#define ucfi_addr_2_offset(addr) \
	(((addr) % ((PAGE_SIZE) * UCFI_BITMAP_SCALE_SIZE)) / (PAGE_SIZE))

STATIC INLINE void ucfi_bitmap_byte_set(uint8_t *bitmap, size_t idx, bool is_set)
{
	bitmap[idx] = is_set ? 0xff : 0x0;
}

STATIC INLINE void ucfi_bitmap_bit_set(uint8_t *bitmap, size_t idx, size_t bit_off, bool is_set)
{
	if (is_set) {
		bitmap[idx] |= (1U << bit_off);
	} else {
		bitmap[idx] &= ~(1U << bit_off);
	}
}

STATIC INLINE bool ucfi_bitmap_test_bit(const uint8_t *bitmap, size_t idx, size_t bit_off)
{
	return ((bitmap[idx] & (1U << bit_off)) != 0U);
}

STATIC INLINE int ucfi_bitmap_change_prot(size_t start, size_t end, uint32_t prot)
{
	size_t offset = PAGE_ALIGN_DOWN(start);
	size_t len = PAGE_ALIGN_UP(end) - offset;
	BUG_ON(len == 0UL);
	return hm_mem_mprotect(ucfi_bitmap.bitmap + offset, len, prot);
}

STATIC void ucfi_bitmap_proc(size_t start, size_t end, bool is_set)
{
	size_t i;
	size_t start_idx = ucfi_addr_2_index(start);
	size_t start_off = ucfi_addr_2_offset(start);
	size_t end_idx = ucfi_addr_2_index(end);
	size_t end_off = ucfi_addr_2_offset(end);

	hm_debug("ucfi refresh bitmap, start: 0x%zx, end: 0x%zx, is_set: %d\n", start, end, is_set);
	RAW_MUTEX_GUARD(_, &g_bitmap_lock);
	if (ucfi_bitmap_change_prot(start_idx, end_idx + ((end_off == 0) ? 0 : 1),
				    __U(PROT_READ) | __U(PROT_WRITE)) != E_HM_OK) {
		hm_error("failed to change ucfi bitmap prot\n");
		return;
	}

	if (start_idx != end_idx) {
		size_t byte_idx = start_idx + ((start_off == 0) ? 0 : 1);
		start_off = (start_off == 0) ? UCFI_BITMAP_SCALE_SIZE : start_off;
		for (i = start_off; i < UCFI_BITMAP_SCALE_SIZE; i++) {
			ucfi_bitmap_bit_set(ucfi_bitmap.bitmap, start_idx, i, is_set);
		}
		for (i = byte_idx; i < end_idx; i++) {
			ucfi_bitmap_byte_set(ucfi_bitmap.bitmap, i, is_set);
		}
		for (i = 0; i < end_off; i++) {
			ucfi_bitmap_bit_set(ucfi_bitmap.bitmap, end_idx, i, is_set);
		}
	} else {
		for (i = start_off; i < end_off; i++) {
			ucfi_bitmap_bit_set(ucfi_bitmap.bitmap, start_idx, i, is_set);
		}
	}

	if (ucfi_bitmap_change_prot(start_idx, end_idx + ((end_off == 0) ? 0 : 1),
				    __U(PROT_READ)) != E_HM_OK) {
		hm_error("failed to change ucfi bitmap prot\n");
		return;
	}
}

/*
 * If the UCFI function is disabled for the DSO, set the bitmap to 1.
 */
STATIC void ucfi_bitmap_refresh(const struct dl_phdr_info *info, bool is_set)
{
	int i;
	size_t start;
	size_t end;
	const ElfW(Phdr) *phdr = NULL;

	phdr = info->dlpi_phdr;
	for (i = info->dlpi_phnum; i > 0; i--, phdr++) {
		if ((phdr->p_type == PT_LOAD) && ((phdr->p_flags & PF_X) != 0)) {
			if (phdr->p_memsz == 0) {
				hm_warn("memory size is 0 in dso, name=%s\n", info->dlpi_name);
				continue;
			}
			start = PAGE_ALIGN_DOWN(info->dlpi_addr + phdr->p_vaddr);
			end = info->dlpi_addr + phdr->p_vaddr + phdr->p_memsz;
			end = PAGE_ALIGN_UP(end);

			ucfi_bitmap_proc(start, end, is_set);
		}
	}
}

STATIC size_t count_syms(uint32_t *gnuhashtab)
{
	size_t nsym, i;
	uint32_t *buckets = gnuhashtab + 4 + (gnuhashtab[2]*sizeof(size_t)/4);
	uint32_t *hashval;
	for (i = nsym = 0; i < gnuhashtab[0]; i++) {
		if (buckets[i] > nsym) {
			nsym = buckets[i];
		}
	}
	if (nsym != 0) {
		hashval = buckets + gnuhashtab[0] + (nsym - gnuhashtab[1]);
		nsym++;
		while ((*hashval & 1) == 0) {
			nsym++;
			hashval++;
		}
	}
	return nsym;
}

STATIC bool ucfi_sym_exist(const ElfW(Sym*) sym, ElfW(Word) sym_cnt, char *strtab)
{
	char* sym_name = NULL;
	size_t len = 0;

	if (sym == NULL || strtab == NULL) {
		return false;
	}

	for (ElfW(Word) sym_index = 0; sym_index < sym_cnt; sym_index++) {
		sym_name = &strtab[sym[sym_index].st_name];
		len = strlen(sym_name);
		if (len != 0 && strncmp(sym_name, "ucfi_fail", strlen(sym_name)) == 0) {
			return true;
		}
	}
	return false;
}

STATIC bool is_ucfi_enable(ElfW(Addr) base, ElfW(Dyn) *dyn)
{
	ElfW(Dyn) *tmp = dyn;
	ElfW(Word) sym_cnt = 0;
	ElfW(Sym*) sym = NULL;
	char* strtab = NULL;
	while (tmp->d_tag != DT_NULL) {
		if (tmp->d_tag == DT_HASH) {
			ElfW(Word*) hash = (ElfW(Word*))(base + tmp->d_un.d_ptr);
			sym_cnt = hash[1];
		} else if (tmp->d_tag == DT_GNU_HASH && sym_cnt == 0) {
			sym_cnt = (ElfW(Word))count_syms((uint32_t *)(base + tmp->d_un.d_ptr));
		} else if (tmp->d_tag == DT_STRTAB) {
			strtab = (char*)(base + tmp->d_un.d_ptr);
		} else if (strtab != NULL && tmp->d_tag == DT_SYMTAB) {
			sym = (ElfW(Sym*))(base + tmp->d_un.d_ptr);
		}

		tmp++;
	}

	return ucfi_sym_exist(sym, sym_cnt, strtab);
}

STATIC bool ucfi_dso_compat_checker(struct dl_phdr_info *info)
{
	bool ucfi_enabled = false;
	const ElfW(Phdr) *phdr = info->dlpi_phdr;

	for (int i = 0; i < info->dlpi_phnum; i++, phdr++) {
		if (phdr->p_type == PT_DYNAMIC) {
			ucfi_enabled = is_ucfi_enable(info->dlpi_addr, (ElfW(Dyn) *)(info->dlpi_addr + phdr->p_vaddr));
			break;
		}
	}

	return ucfi_enabled;
}

struct ucfi_updata_info {
	bool is_open;
	struct link_map *link_map;
};

STATIC int ucfi_bitmap_update(struct dl_phdr_info *info, size_t size, void *data)
{
	bool ucfi_enabled = false;
	struct ucfi_updata_info *up_info = NULL;

	UNUSED(size);
	if (data == NULL) {
		return E_HM_INVAL;
	}

	up_info = (struct ucfi_updata_info *)data;
	if (info->dlpi_addr != up_info->link_map->l_addr) {
		return E_HM_OK;
	}

	if (up_info->is_open) {
		ucfi_enabled = is_ucfi_enable(up_info->link_map->l_addr, up_info->link_map->l_ld);
	}

	ucfi_bitmap_refresh(info, !ucfi_enabled);
	return E_HM_OK;
}

void ucfi_update(void *dso, bool is_open)
{
	int rc;
	struct ucfi_updata_info up_info = {0};

	if (!ucfi_inited || dso == NULL) {
		return;
	}

	up_info.is_open = is_open;
	rc = dlinfo(dso, RTLD_DI_LINKMAP, &up_info.link_map);
	if (rc != 0) {
		hm_debug("dlinfo failed, %d\n", rc);
		return;
	}

	rc = dl_iterate_phdr(ucfi_bitmap_update, (void *)&up_info);
	if (rc != 0) {
		hm_error("failed to iterate dso phdr, err=%d\n", rc);
	}

	return;
}

STATIC int ucfi_bitmap_init(struct dl_phdr_info *info, size_t size, void *data)
{
	bool ucfi_enabled = false;

	UNUSED(size);
	UNUSED(data);
	if (info == NULL) {
		return E_HM_INVAL;
	}

	ucfi_enabled = ucfi_dso_compat_checker(info);
	if (!ucfi_enabled) {
		ucfi_bitmap_refresh(info, !ucfi_enabled);
	}

	return E_HM_OK;
}

int ucfi_init(void)
{
	int rc = 0;
	size_t vs_limit = 0;

	if (ucfi_inited) {
		return E_HM_OK;
	}

	rc = getrlimit(RLIMIT_AS, &ucfi_bitmap.rlim);
	if (rc != 0) {
		hm_error("failed to get rlimit, err=%d\n", rc);
		return posix2hmerrno(rc);
	}

	rc = hm_sysctrl_read_conf(__SYSCONF_VSPACE_LIMIT, &vs_limit, sizeof(vs_limit));
	if (rc < 0) {
		hm_error("failed to get vspace limit, err=%d\n", rc);
		return rc;
	}

	ucfi_bitmap.sz = ((size_t)ucfi_bitmap.rlim.rlim_cur > vs_limit) ?
			 ucfi_addr_2_index(vs_limit) : ucfi_addr_2_index((size_t)ucfi_bitmap.rlim.rlim_cur);

	/* mmap ucfi bitmap */
	ucfi_bitmap.bitmap = hm_mem_mmap(NULL, ucfi_bitmap.sz, PROT_NONE,
					 __U(MAP_PRIVATE) | __U(MAP_ANON), -1, 0);
	if (ucfi_bitmap.bitmap == MAP_FAILED) {
		hm_error("failed to mmap ucfi bitmap\n");
		return E_HM_NOMEM;
	}

	rc = dl_iterate_phdr(ucfi_bitmap_init, NULL);
	if (rc < 0) {
		int err = hm_mem_munmap(ucfi_bitmap.bitmap, ucfi_bitmap.sz);
		if (err != E_HM_OK) {
			hm_error("failed to unmap ucfi bitmap, err=%s\n", hmstrerror(err));
		}
		return rc;
	}
	ucfi_inited = true;

	return E_HM_OK;
}

int ucfi_fail(unsigned long addr)
{
	size_t idx = ucfi_addr_2_index(addr);
	size_t bit_off = ucfi_addr_2_offset(addr);

	return ucfi_bitmap_test_bit(ucfi_bitmap.bitmap, idx, bit_off) ? 0 : 1;
}
