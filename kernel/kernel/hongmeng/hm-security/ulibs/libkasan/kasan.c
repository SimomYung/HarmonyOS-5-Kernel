/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Init and manage shadow range for kasan
 * Author: Huawei OS Kernel Lab
 * Create: Sun Sep 29 11:11:11 2019
 */
#include "macro.h"
/* libc and sys */
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <hmasm/lsyscall.h>
#ifdef ADAPT_FOR_LIBC_KASAN
#include <kasanhook.h>
#endif

/* hongmeng ulibs */
#include <libmem/utils.h>
#include <libhmlog/hmlog.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <libhmsrv_sys/hm_security.h>

/* hm-security */
#include <libkasan/kasan.h>
#include <libkasan/mutex.h>
#include <libkasan/kasan_init.h>
#include "shadow.h"
#include "wrapper_hm.h"
#include "asan_env.h"
#include "wrapper_libc_decl.h"
#include "wrapper_hm_decl.h"
#include "wrapper_ldk_decl.h"

#define KASAN_MPOOL_NPAGE 4

#define hex_trans_to_val(input, base) ((unsigned int)((int)(input) - (int)(base)))

static bool kasan_inited = false;
static uintptr_t g_heap_start = 0;
static uintptr_t g_heap_end = 0;

/* avoid tls access */
static kasan_mutex_t kasan_shadow_ref_mutex;

struct kasan_shm_info {
	int32_t id;
	size_t len;
	void *vaddr;
};

/* use in libkasan and kasan_sysmgr */
bool kasan_get_inited(void)
{
	return kasan_inited;
}

void kasan_set_inited(bool val)
{
	kasan_inited = val;
}

void kasan_init_shadow_ref_mutex(void)
{
	kasan_mutex_init(&kasan_shadow_ref_mutex);
}

void kasan_lock_shadow_ref_mutex(void)
{
	kasan_mutex_lock(&kasan_shadow_ref_mutex);
}

void kasan_unlock_shadow_ref_mutex(void)
{
	kasan_mutex_unlock(&kasan_shadow_ref_mutex);
}

/* just use in libkasan */
uintptr_t kasan_get_heap_start(void)
{
	return g_heap_start;
}

uintptr_t kasan_get_heap_end(void)
{
	return g_heap_end;
}

void kasan_set_heap_start(uintptr_t new_heap_start)
{
	g_heap_start = new_heap_start;
}

void kasan_set_heap_end(uintptr_t new_heap_end)
{
	g_heap_end = new_heap_end;
}

static int madvise_procmaps(const uintptr_t maps, size_t sz)
{
	int rc;
	rc = (int)lsyscall_syscall3(__NR_madvise, (long)maps, (long)ALIGN_UP(sz, __PAGE_SIZE), MADV_DONTNEED);
	return rc;
}
/*
 * populate range using correct prot , caller should guarantee
 * that start and end are page aligned
 */
static int populate_range(uintptr_t start, size_t sz)
{
	const uint32_t flag = MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS;
	const uint32_t prot = PROT_READ | PROT_WRITE;
	void *raddr = NULL;
	raddr = hm_mem_mmap((void *)start, sz, prot, flag, 0, 0);
	if (raddr == MAP_FAILED) {
		hm_error("failed to remap when populating range\n");
		return E_HM_NOMEM;
	}
	return E_HM_OK;
}

void kasan_map_calculate_range(uintptr_t start, uintptr_t end,
			       uintptr_t *map_start, uintptr_t *map_end)
{
	uintptr_t start_up;
	uintptr_t end_down;

	start_up = ALIGN_UP(start, REF_RANGE);
	end_down = ALIGN_DOWN(end, REF_RANGE);
	if (start_up <= end_down) {
		if (start < start_up) {
			if (get_ref(start) == 0) {
				*map_start = shadow_page(start);
			} else {
				/* unpoison remaped range */
				kasan_poison_shadow(start, start_up - start, 0);
				*map_start = shadow_page(start_up);
			}
		} else {
			*map_start = shadow_page(start_up);
		}
		if (end_down < end) {
			if (get_ref(end) == 0) {
				*map_end = shadow_page(end) + __PAGE_SIZE;
			} else {
				kasan_poison_shadow(end_down, end - end_down, 0);
				*map_end = shadow_page(end_down);
			}
		} else {
			*map_end = shadow_page(end_down);
		}
	} else {
		if (get_ref(start) == 0) {
			*map_start = shadow_page(start);
			*map_end = *map_start + __PAGE_SIZE;
		} else {
			*map_start = 0;
			*map_end = 0;
			kasan_poison_shadow(start, end - start, 0);
		}
	}

	return;
}

/* remap shadow range using correct prot */
static int map_shadow_for_range(uintptr_t start, uintptr_t end)
{
	uintptr_t map_start, map_end;

	kasan_map_calculate_range(start, end, &map_start, &map_end);

	if (map_start < map_end) {
		int ret = hm_mem_madvise((void *)map_start, (size_t)(map_end - map_start), MADV_FLAG_WILLNEED);
		if (ret !=  E_HM_OK) {
			hm_error("failed to map the shadow region, ret=%s\n", hmstrerror(ret));
			return ret;
		}
	}

	return E_HM_OK;
}

static void check_overcommit_policy(void)
{
	int ret;
	struct __actvret_hmcall_procfs_simple_read read_val;
	struct hm_actv_buf name_buf;
	char overcommit_policy;

	mem_zero_s(read_val);
	hm_actv_buf_init(&name_buf, "overcommit_policy", sizeof("overcommit_policy"));
	ret = actvcall_hmcall_procfs_simple_read(name_buf, &read_val);
	if (ret < 0 || read_val.result.size == 0) {
		hm_debug("simple_read overcommit_policy failed\n");
	} else {
		overcommit_policy = read_val.result.data[0];
		if (overcommit_policy != '1') {
			hm_error("please set CONFIG_OVERCOMMIT_MEMORY to OVERCOMMIT_ALWAYS\n");
		}
	}

	return;
}

static int reserve_shadow_range(void)
{
	int ret = E_HM_OK;
	const uint32_t flag = MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS;
	const uint32_t prot = PROT_NONE;
	void *raddr = NULL;
	uintptr_t shadow_beg;
	kasan_init_shadow_beg();
	shadow_beg = KASAN_SHADOW_BEG;
	raddr = hm_mem_mmap((void *)shadow_beg, (size_t)(KASAN_ADDR_LIMIT - shadow_beg),
			    prot, flag, 0, 0);
	if (raddr == MAP_FAILED) {
		hm_error("failed to map the reserved shadow region\n");
		check_overcommit_policy();
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK) {
		ret = hm_mem_mprotect(raddr, (size_t)(KASAN_ADDR_LIMIT - shadow_beg), PROT_READ | PROT_WRITE);
		if (ret != E_HM_OK) {
			hm_error("failed to mprotect shadow, ret=%s\n", hmstrerror(ret));
		}
	}

	return ret;
}

static uintptr_t parse_hex(char *cur, char **out)
{
	char c;
	uintptr_t ret = 0;
	char *tmp = cur;
	/*
	 * Loop termination condition:
	 * Following loop terminate when c is not 0-9 or a-f or A-F, Since c traversed the content
	 * pointed to by tmp in loop body, it has a bound.
	 * Loop condition is constant and reasonable: tmp is point to a virtual address according
	 * to init_shadow_ranges, so the loop condition is reasonable.
	 */
	do {
		c = *tmp;
		if (c >= '0' && c <= '9') {
			ret = ret * KASAN_HEX_SCALE + hex_trans_to_val(c, '0');
		} else if (c >= 'a' && c <= 'f') {
			ret = ret * KASAN_HEX_SCALE + hex_trans_to_val(c, 'a') + KASAN_DEC_SCALE;
		} else if (c >= 'A' && c <= 'F') {
			ret = ret * KASAN_HEX_SCALE + hex_trans_to_val(c, 'A') + KASAN_DEC_SCALE;
		} else {
			if (out != NULL) {
				*out = tmp;
			}
			break;
		}
		tmp++;
	} while (true);
	return ret;
}

static int get_heap_start(void)
{
	long ret;
	ret = hm_mem_brk(0);
	if (ret == E_HM_NOSYS) {
		hm_debug("hm_mem_brk is not supported\n");
		ret = E_HM_OK;
	}
	return (int)ret;
}

static void update_heap_stat(uintptr_t start, uintptr_t end)
{
	if ((start == g_heap_start) || (start == g_heap_end)) {
		g_heap_end = end;
	}
	return;
}

static int parse_map(char *cur)
{
	char *tmp = cur;
	uintptr_t start = parse_hex(tmp, &tmp);
	uintptr_t end;
	if (*tmp != '-') {
		hm_error("the format of proc/maps is not correct\n");
		return E_HM_INVAL;
	}
	tmp++;
	end = parse_hex(tmp, NULL);
	update_heap_stat(start, end);
	return kasan_map_shadow_range(start, end);
}

static char *next_line(char *cur, const char *last)
{
	char *line = NULL;
	char *tmp = cur;
	/*
	 * Loop termination condition:
	 * Following loop will terminate when tmp reach last. Since tmp is monotonic, keep increasing
	 * in loop body, it has an upper bound.
	 * Loop condition is constant and reasonable: last is equal to cur plus map_size according to init_shadow_ref,
	 * so loop condition is reasonable.
	 */
	for (; tmp < last; tmp++) {
		if (*tmp == '\n') {
			line = tmp + 1;
			break;
		}
	}
	if (line == last) {
		return NULL;
	}
	if (line != NULL && *line == '\0') {
		return NULL;
	}
	return line;
}

static int init_shadow_ref(void *proc_maps, size_t maps_size)
{
	char *last = (char *)proc_maps + maps_size;
	char *cur = (char *)proc_maps;
	int ret;
	/*
	 * Loop termination condition:
	 * Following loop terminate when cur is NULL, Since the address pointed to by cur will
	 * increase in loop body, it has a bound.
	 * Loop condition is constant and reasonable: when cur increase to last, it will be set to NULL,
	 * maps_size(different between cur and last) is the return value according to init_shadow_ranges,
	 * it has a bound, so the loop condition is reasonable.
	 */
	while (cur != NULL) {
		ret = parse_map(cur);
		if (ret != E_HM_OK) {
			return ret;
		}
		cur = next_line(cur, last);
	}
	return E_HM_OK;
}

static inline bool kasan_need_map_shadow(uintptr_t start, uintptr_t end)
{
	bool ret = true;

	if (!kasan_inited || start >= end || end >= KASAN_SHADOW_BEG) {
		ret = false;
	}

	return ret;
}

int kasan_map_shadow_range(uintptr_t start, uintptr_t end)
{
	int ret = E_HM_OK;

	if (kasan_need_map_shadow(start, end)) {
		kasan_mutex_lock(&kasan_shadow_ref_mutex);
		ret = map_shadow_for_range(start, end);
		if (ret == E_HM_OK) {
			kasan_add_ref(start, end);
		} else {
			hm_error("failed to map the shadow of map ranges, ret=%s\n", hmstrerror(ret));
		}
		kasan_mutex_unlock(&kasan_shadow_ref_mutex);
	}

	return ret;
}

void kasan_unmap_shadow_range(uintptr_t start, uintptr_t end)
{
	if (!kasan_inited) {
		return;
	}
#ifdef CONFIG_KASAN_UNMAP_SHADOW
	kasan_mutex_lock(&kasan_shadow_ref_mutex);
	kasan_remove_ref(start, end);
	kasan_mutex_unlock(&kasan_shadow_ref_mutex);
#else
	kasan_poison_shadow(start, end - start, UNMAP_POISON_VALUE);
#endif
	return;
}

int kasan_calc_shadowref_len(uintptr_t maps, uintptr_t shadow_beg,
			    uintptr_t shadow_pgref, size_t *ref_len)
{
	int ret = E_HM_OK;
	uintptr_t tmp;
	size_t tmp_ref_len;

	if (maps >= KASAN_ADDR_LIMIT || maps < shadow_pgref) {
		hm_error("maps address is invalid\n");
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		tmp = ALIGN_UP(shadow_beg, KASAN_SHADOW_SCALE_SIZE * __PAGE_SIZE);
		tmp_ref_len = (size_t)((tmp - KASAN_ADDR_BASE) / (KASAN_SHADOW_SCALE_SIZE * __PAGE_SIZE)) * sizeof(uint16_t);
		if (maps - shadow_pgref < tmp_ref_len) {
			hm_error("address space range is not properly configured\n");
			ret = E_HM_INVAL;
		} else {
			*ref_len = PAGE_ALIGN_UP(tmp_ref_len);
		}
	}

	return ret;
}

static int prepare_init_shadow_ranges(uintptr_t maps)
{
	int ret = E_HM_OK;
	size_t ref_len;
	uintptr_t shadow_pgref = (uintptr_t)KASAN_SHADOW_PGREF;

	ret = kasan_calc_shadowref_len(maps, KASAN_SHADOW_BEG, shadow_pgref, &ref_len);
	if (ret == E_HM_OK) {
		ret = populate_range(shadow_pgref, ref_len);
	}

	return ret;
}

static int free_kasan_shm(struct kasan_shm_info *shm_info)
{
	int rc = E_HM_OK;

	if (shm_info == NULL) {
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		if (shm_info->vaddr != ulong_to_ptr(MAP_FAILED, void)) {
			rc = hm_mem_munmap(shm_info->vaddr, shm_info->len);
			if (rc < 0) {
				hm_warn("failed to unmap memory, err=%s\n", hmstrerror(rc));
			}
		}
	}
	if (rc == E_HM_OK) {
		rc = hm_ashm_close(shm_info->id);
		if (rc < 0) {
			hm_warn("failed to close shared memory, err=%s\n", hmstrerror(rc));
		}
	}
	if (rc == E_HM_OK) {
		rc = hm_ashm_unlink(shm_info->id);
		if (rc < 0) {
			hm_warn("failed to unlink shared memory, err=%s\n", hmstrerror(rc));
		}
	}

	return rc;
}

static int init_get_pid_maps(struct kasan_shm_info *shm)
{
	unsigned int mpool_npage = KASAN_MPOOL_NPAGE;
	pid_t cur_pid = hm_getpid();
	int ret;
	struct hm_actv_buf name_buf;

	/*
	 * Loop termination condition:
	 * Following loop terminate when the return value of actvcall_hmcall_procfs_pid_read is
	 * not E_HM_FBIG or open/mmap shm failed. Since shm->len keep increasing in loop body,
	 * it has an upper bound.
	 * Loop condition is constant and reasonable: if shm->len is larger than *olen the loop will
	 * terminate according to mem_maps_desc, shm->len increase fast, so the loop condition
	 * is reasonable.
	 */
	while (true) {
		ret = hm_ashm_open_auth(0, 0U, (size_t)mpool_npage << (__PAGE_SHIFT),
				   SHM_O_CREAT | SHM_O_EXCL | SHM_O_RDWR, &(shm->len));
		if (ret < 0) {
			hm_error("failed to open the shared memory, err=%s\n", hmstrerror(ret));
			return ret;
		}
		shm->id = ret;
		shm->vaddr = hm_mem_mmap_posix(NULL, shm->len, MPROT_READ | MPROT_WRITE,
					       MFLAG_MAP_ANONYMOUS | MFLAG_MAP_SHARED | MFLAG_MAP_POPULATE,
					       shm->id, 0, &ret);
		if (ret != E_HM_OK) {
			hm_error("failed to map the shared memory, err=%s\n", hmstrerror(ret));
			(void)free_kasan_shm(shm);
			return ret;
		}

		hm_actv_buf_init(&name_buf, "maps", sizeof("maps"));
		ret = actvcall_hmcall_procfs_pid_read(cur_pid, CALLER_PIDNSID, 0,
						      (unsigned long)(unsigned int)shm->id,
						      shm->len, name_buf);
		if (ret == E_HM_FBIG) {
			if (free_kasan_shm(shm) != E_HM_OK) {
				hm_warn("failed to free kasan share memory\n");
			}
			mpool_npage = mpool_npage << 1;
		} else {
			break;
		}
	}

	if (ret < 0) {
		hm_error("failed to read from sysmgr, err=%s\n", hmstrerror(ret));
		if (free_kasan_shm(shm) != E_HM_OK) {
			hm_warn("failed to free kasan share memory\n");
		}
	}

	return ret;
}

static int init_shadow_ranges(void)
{
	int ret;
	uintptr_t maps;
	char *data = NULL;
	size_t rsize;
	struct kasan_shm_info info;
	uintptr_t shadow_beg = KASAN_SHADOW_BEG;

	mem_zero_s(info);
	maps = PAGE_ALIGN_UP(shadow_beg + (shadow_beg - KASAN_ADDR_BASE) / KASAN_SHADOW_SCALE_SIZE +
			     (shadow_beg - KASAN_ADDR_BASE) / KASAN_SHADOW_BEG_SCALE);
	ret = prepare_init_shadow_ranges(maps);
	if (ret != E_HM_OK) {
		return ret;
	}
	if (g_heap_start == 0) {
		ret = get_heap_start();
		if (ret != E_HM_OK) {
			hm_error("failed to get heap start, err=%s\n", hmstrerror(ret));
			return ret;
		}
	}

	ret = init_get_pid_maps(&info);
	if (ret < 0) {
		return ret;
	}

	data = info.vaddr;
	rsize = (size_t)(unsigned int)ret;

	/* we set kasan_inited just before we parse pid_maps */
	kasan_set_inited(true);
	ret = init_shadow_ref(data, rsize);
	if (ret != E_HM_OK) {
		hm_error("failed to initialize shadow ref, err=%s\n", hmstrerror(ret));
	}

	if (free_kasan_shm(&info) != E_HM_OK) {
		hm_warn("failed to free kasan share memory\n");
	}

	if (madvise_procmaps(maps, MAPS_SIZE) != E_HM_OK) {
		hm_warn("failed to madvise kasan maps\n");
	}
	return ret;
}

#ifdef ADAPT_FOR_LIBC_KASAN
#define ASAN_LOADSTORE_ASSIGN(size)				\
	.__asan_load##size = __asan_load##size,			\
	.__asan_load##size##_noabort = __asan_load##size,	\
	.__asan_store##size = __asan_store##size,		\
	.__asan_store##size##_noabort = __asan_store##size,

static int kasan_callback_init(void)
{
	struct asan_callback s_ac = {
		ASAN_LOADSTORE_ASSIGN(1)
		ASAN_LOADSTORE_ASSIGN(2)
		ASAN_LOADSTORE_ASSIGN(4)
		ASAN_LOADSTORE_ASSIGN(8)
		ASAN_LOADSTORE_ASSIGN(16)
		ASAN_LOADSTORE_ASSIGN(N)
		.__asan_register_globals = __asan_register_globals,
		.__asan_unregister_globals = __asan_unregister_globals,
		.__asan_handle_no_return = __asan_handle_no_return
	};
	return set_asan_callback(&s_ac);
}
#endif

#ifdef KASAN_SHARED
static void kasan_reg_ori_function(void)
{
#define KASAN_WRAPPER_DEFINE
#define KASAN_WRAPPER(ret_type, name, ARG) kasan_reg_##name();
#define KASAN_WRAPPER_VOID(name, ARG) kasan_reg_##name();
#include "wrapper_libc_api.h"
#include "wrapper_hm_api.h"
#include "wrapper_ldk_api.h"
#undef KASAN_WRAPPER_VOID
#undef KASAN_WRAPPER
#undef KASAN_WRAPPER_DEFINE
	return;
}
#endif

int kasan_init(void)
{
	int rc = E_HM_OK;

#ifdef KASAN_SHARED
	kasan_reg_ori_function();
#endif
	kasan_init_map_ref();
	kasan_init_shadow_ref_mutex();
	kasan_heap_mutex_init();

	rc = reserve_shadow_range();
	if (rc != 0) {
		hm_error("failed to reserve shadow range\n");
		return rc;
	}

	rc = init_shadow_ranges();
	if (rc != 0) {
		hm_error("failed to init shadow range\n");
		return rc;
	}

#ifdef ADAPT_FOR_LIBC_KASAN
	rc = kasan_callback_init();
	if (rc != 0) {
		hm_error("failed to init kasan callback\n");
		return E_HM_INVAL;
	}
#endif
	rc = asan_env_init();
	if (rc != 0) {
		hm_error("failed to init asan options\n");
		return rc;
	}

	hm_sec_set_kasan_state(true);
	hm_sec_set_shadow_pgref_va((uintptr_t)KASAN_SHADOW_PGREF);
	hm_debug("kasan_init success\n");
	return rc;
}

static int kasan_mprotect_map_range(uintptr_t start, uintptr_t end)
{
	int ret = E_HM_OK;
	uintptr_t start_up = ALIGN_UP(start, REF_RANGE);
	uintptr_t start_down = ALIGN_DOWN(start, REF_RANGE);
	uintptr_t end_up = ALIGN_UP(end, REF_RANGE);
	uintptr_t end_down = ALIGN_DOWN(end, REF_RANGE);
	uintptr_t tmp = 0;

	kasan_mutex_lock(&kasan_shadow_ref_mutex);
	if (start > start_down) {
		if (get_ref(start_down) == 0) {
			ret = map_shadow_for_range(start_down, start_up);
		}
		if (ret == E_HM_OK) {
			kasan_add_ref(start, start_up);
		}
	}

	if (ret == E_HM_OK) {
		/* if start is equal to start_down, start is equal to start_up. */
		for (tmp = start_up; tmp < end_down; tmp += REF_RANGE) {
			if (get_ref(tmp) == 0) {
				ret = map_shadow_for_range(tmp, tmp + REF_RANGE);
			}
			if (ret == E_HM_OK) {
				kasan_add_ref(tmp, tmp + REF_RANGE);
			}
		}
	}

	if (ret == E_HM_OK) {
		if (end > end_down) {
			if (get_ref(end_down) == 0) {
				ret = map_shadow_for_range(end_down, end_up);
			}
			if (ret == E_HM_OK) {
				kasan_add_ref(end_down, end);
			}
		}
	}
	kasan_mutex_unlock(&kasan_shadow_ref_mutex);

	return ret;
}

int kasan_mprotect_shadow_range(uintptr_t start, uintptr_t end)
{
	int ret = E_HM_OK;

	if (kasan_need_map_shadow(start, end)) {
		ret = kasan_mprotect_map_range(start, end);
		if (ret != E_HM_OK) {
			hm_warn("failed to mprotect map shadow range\n");
		}
	}

	return ret;
}
