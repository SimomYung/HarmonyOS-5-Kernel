/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Interfaces of vspace for outside modules(of sysmgr) for memmgr
 * Author: Huawei OS Kernel Lab
 * Create: Fri Dec 14 15:22:32 2018
 */
#ifndef INTAPI_MEM_VSPACE_H
#define INTAPI_MEM_VSPACE_H

#include <stdint.h>
#include <stdbool.h>
#include <libhmsync/raw_workqueue.h>
#include <hmkernel/mm/pgtable.h>
#include <hmkernel/capability.h>
#include <lib/vspace_layout.h>
#include <hongmeng/types.h>
#include <libstrict/strict.h>

/*
 * flags for direct paging area
 *
 * DPA_CODE:            direct paging for code segment
 * DPA_MAIN_STACK:      direct paging for main stack of one process
 * DPA_MMAP_ANONYMOUS:  direct paging for anonymous mmap areas
 * DPA_MMAP_file:       direct paging for file mmap areas
 */
#define DPA_CODE       		0x01U
#define DPA_MAIN_STACK 		0x02U
#define DPA_MMAP_ANONYMOUS	0x04U
#define DPA_MMAP_FILE		0x08U

struct process_s;
struct vspace_s;
struct reserved_region_s;

#ifdef CONFIG_PROCESS_KMEM_STAT
enum km_type {
	KM_SLAB,    /* kernel meta-used slab size */
	KM_SYSMGR_MMAP,   /* kernel meta-used sysmgr mmap size */
	NR_KM_TYPES,
};
#endif

int vspace_get(struct vspace_s *vspace);

int vspace_put(struct vspace_s *vspace);

struct vspace_layout *vspace_layout_of(struct vspace_s *vspace);

#ifdef CONFIG_COREDUMP_MEMORY_KDUMP
/*
 * This interface can only be called when COREDUMP_MEMORY_KDUMP is enabled,
 * calling it to read the pgd of the system service, and the system should panic
 * immediately afterwards. Otherwise, do not call this interface.
 */
uint64_t vspace_pgd_of(const struct vspace_s *vspace);
#endif

rref_t vspace_rref_of(const struct vspace_s *vspace);

cref_t vspace_cref_of(const struct vspace_s *vspace);

void vspace_set_process(struct vspace_s *vspace, struct process_s *process);

struct process_s *vspace_process_of(struct vspace_s *vspace);

#ifdef CONFIG_DEBUG_BUILD
uint32_t vspace_state_of(struct vspace_s *vspace);
#endif

void vspace_set_cnode_index(struct vspace_s *vspace, uint32_t idx);

uint32_t vspace_cnode_index_of(const struct vspace_s *vspace);

#ifdef CONFIG_SYSMGR_VSPACE_COPY_PARALLELIZE
void vspace_copy_helper_init(void);
#endif

#ifdef CONFIG_SECURITY_XPM
struct reserved_region_s *vspace_xpm_region_of(struct vspace_s *vspace);

int vspace_clone_xpm_ownerid(struct vspace_s *dst, struct vspace_s *src);
#endif

struct raw_work *vspace_raw_work_of(struct vspace_s *vspace);

struct vspace_s *vspace_from_raw_work(const struct raw_work *wk);

bool vspace_is_priv(const struct vspace_s *vspace);

void vspace_set_start(struct vspace_s *vspace, uintptr_t val);

uintptr_t vspace_start_of(const struct vspace_s *vspace);

#ifdef CONFIG_MEM_TSAN_MMAP_LIMIT
void vspace_set_tsan_mmap(struct vspace_s *vspace);
#else
static inline void vspace_set_tsan_mmap(__maybe_unused struct vspace_s *vspace)
{
	UNUSED(vspace);
	return;
}
#endif

void vspace_set_vlimit(struct vspace_s *vspace, uintptr_t val);

uintptr_t vspace_vlimit_of(const struct vspace_s *vspace);

bool vspace_sfi_enabled(const struct vspace_s *vspace);
uintptr_t vspace_slimit_of(const struct vspace_s *vspace);

void vspace_set_free_cached(struct vspace_s *vspace, uintptr_t val);

void vspace_set_direct_paging_areas(struct vspace_s *vspace, uint32_t dpa);

bool vspace_is_direct_paging_enable(const struct vspace_s *vspace, uint32_t dpa);

void vspace_set_swapless(struct vspace_s *vspace);

void vspace_set_system_service(struct vspace_s *vs);

#ifdef CONFIG_HKIP_PROTECT_EL0
bool vspace_get_hkip_attr(const struct vspace_s *vs);
void vspace_set_hkip_attr(struct vspace_s *vs);
#endif

void vspace_clear_swapless(struct vspace_s *vspace);

struct vspace_s *vspace_create_at_current(const struct process_s *process, bool is_vm, bool is_sys_srv);

void vspace_destroy(struct vspace_s *vs);

int vspace_copy(struct vspace_s *vs, struct vspace_s *new);

struct vspace_s *vspace_of_sysmgr(void);

void vspace_init_sysmgr(struct vspace_s *vs);

int vspace_release_resource(struct vspace_s *vs);

bool vspace_is_addr_valid(struct vspace_s *vspace, uintptr_t vaddr);

bool vspace_is_range_valid(struct vspace_s *vspace, uintptr_t vaddr_start,
			   uintptr_t vaddr_end);

uintptr_t vspace_get_start(uint32_t privilege_num);

uintptr_t vspace_get_limit(uint32_t privilege_num);

int vspace_reclaim_page_table(struct vspace_s *vs, uintptr_t start, size_t length);

int vspace_rwlock_tryrdlock(struct vspace_s *vspace);

void vspace_rwlock_rdlock(struct vspace_s *vspace);

void vspace_rwlock_unlock(struct vspace_s *vspace);

void vspace_rwlock_downgrade(struct vspace_s *vspace);

int vspace_rwlock_try_upgrade(struct vspace_s *vspace);

void vspace_wrlock_pair(struct vspace_s *vs1, struct vspace_s *vs2);

void vspace_unlock_pair(struct vspace_s *vs1, struct vspace_s *vs2);

#if defined(CONFIG_COMPAT) || defined(CONFIG_ILP32)
uintptr_t vspace_get_limit32(void);
bool vspace_is_compat(const struct vspace_s *vs);
#else
static inline bool vspace_is_compat(const struct vspace_s *vs)
{
	return false;
}
#endif

#ifdef CONFIG_MMAP_GAP_RANDOMIZATION
uint8_t vspace_gap_rnd_bits_of(const struct vspace_s *vspace);

void vspace_set_gap_rnd_bits(struct vspace_s *vspace, uint8_t rnd_bits);

uint8_t vspace_gap_mode_of(const struct vspace_s *vspace);

void vspace_set_gap_mode(struct vspace_s *vspace, uint8_t mode);

void vspace_set_map_gap(struct vspace_s *new, const struct vspace_s *old);
#endif
void vspace_init_sysmgr_mmap_base(void);

int memmgr_set_anon_name(struct vspace_s *vspace, uintptr_t start, size_t len, const char *name);

int vspace_set_xpm_region(uintptr_t addr_base, size_t length);

int vspace_set_xpm_ownerid(uint32_t id_type, uint64_t ownerid);

void vspace_get_codesign_info(struct vspace_s *vspace, uint32_t *id_type, uint64_t *ownerid);

static inline uint64_t vspace_rw_hint(struct vspace_s *vspace)
{
	uint64_t hint;

	if (vspace_is_priv(vspace)) {
		hint = PGTABLE_ATTR_KREAD | PGTABLE_ATTR_KWRITE |
		       PGTABLE_ATTR_KMAP | PGTABLE_ATTR_PSMAP;
	} else {
		hint = PGTABLE_ATTR_UREAD | PGTABLE_ATTR_UWRITE;
	}

	return hint;
}

uint64_t vspace_rss_of(struct vspace_s *vspace);

int vspace_max_map_count_read(void);
int vspace_max_map_count_write(int count);
int vspace_max_map_count_exceed(struct vspace_s *vs, int margin);

struct vspace_s *vspace_get_by_cnode_idx(uint32_t cnode_idx);
void vspace_put_by_cnode_idx(struct vspace_s *vspace);
void vspace_set_als_region(struct vspace_s *vs);

#ifdef CONFIG_PROCESS_KMEM_STAT
int64_t vspace_pgtable_size_of(struct vspace_s *vs);
int64_t vspace_km_size_of(struct vspace_s *vs, enum km_type stat_type);

void vspace_add_km_size(struct vspace_s *vs, enum km_type stat_type, int64_t value);
void vspace_sub_km_size(struct vspace_s *vs, enum km_type stat_type, int64_t value);

uint64_t process_kmem_size(struct process_s *process);
#endif /* CONFIG_PROCESS_KMEM_STAT */
/*
 * Records the main thread name in struct vspace_s.
 *
 * @param	[I] proc	process which provides main thread name
 */
void vspace_init_main_proc_name(struct process_s *proc);
void vspace_main_proc_name_of(struct vspace_s *vs, char *buf, size_t size);
bool is_vspace_locked(struct vspace_s *vspace);
bool is_sysmgr_vspace_locked(void);
bool is_devhost_vspace_locked(void);

#ifdef CONFIG_HARMONYOS
void vspace_proc_cnt_set_reaping(struct vspace_s *vs);
bool vspace_proc_cnt_is_reaping(struct vspace_s *vs);
void vspace_proc_cnt_lock(struct vspace_s *vs);
void vspace_proc_cnt_unlock(struct vspace_s *vs);
uint32_t vspace_proc_cnt_of(struct vspace_s *vs);
void vspace_inc_proc_cnt(struct vspace_s *vs);
void vspace_dec_proc_cnt(struct vspace_s *vs);
#else
static inline void vspace_proc_cnt_set_reaping(struct vspace_s *vs)
{
	UNUSED(vs);
}
static inline bool vspace_proc_cnt_is_reaping(struct vspace_s *vs)
{
	UNUSED(vs);
	return false;
}
static inline void vspace_proc_cnt_lock(struct vspace_s *vs)
{
	UNUSED(vs);
}
static inline void vspace_proc_cnt_unlock(struct vspace_s *vs)
{
	UNUSED(vs);
}
static inline uint32_t vspace_proc_cnt_of(struct vspace_s *vs)
{
	UNUSED(vs);
	return 1U;
}
static inline void vspace_inc_proc_cnt(struct vspace_s *vs)
{
	UNUSED(vs);
}
static inline void vspace_dec_proc_cnt(struct vspace_s *vs)
{
	UNUSED(vs);
}
#endif
#ifdef CONFIG_HYPERVISOR
void vspace_set_vm_vspace(struct vspace_s *vspace);
bool vspace_is_vm_vspace(const struct vspace_s *vspace);
#else /* !CONFIG_HYPERVISOR */
static inline void vspace_set_vm_vspace(struct vspace_s *vspace)
{
	UNUSED(vspace);
}

static inline bool vspace_is_vm_vspace(const struct vspace_s *vspace)
{
	UNUSED(vspace);
	return false;
}

static inline void vspace_clear_vm_vspace(struct vspace_s *vspace)
{
	UNUSED(vspace);
}
#endif /* CONFIG_HYPERVISOR */

unsigned long *vspace_saved_auxv_of(struct vspace_s *vs);
size_t vspace_saved_auxv_size_of(const struct vspace_s *vs);
#endif /* INTAPI_MEM_VSPACE_H */
