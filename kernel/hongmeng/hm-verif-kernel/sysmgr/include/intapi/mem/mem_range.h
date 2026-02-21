/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Memory range of one vregion
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 20 03:04:00 2020
 */
#ifndef SYSMGR_MEM_MEM_RANGE_H
#define SYSMGR_MEM_MEM_RANGE_H

#include <libhmelf/symbol.h>
#include <intapi/mem/vspace.h>

#define ENABLE_MEMORY_MAX_SIZE 40

/* native_flags of mem_range_s */
#define RANGE_NATIVE_KSHARE			0x01U  /* kshare memory */
#define RANGE_NATIVE_PREH			0x02U  /* prehistoric memory */
#define RANGE_NATIVE_CONTIG			0x04U  /* specific contig PA mapping */
#define RANGE_NATIVE_NONE			0x08U  /* reserve memory */
#define RANGE_NATIVE_PERSISTED_MAP		0x20U  /* persisted mapping, will be unmapped until process exits */
#define RANGE_NATIVE_VNODES			0x40U  /* vnodes backend memory */
#define RANGE_NATIVE_VMAP			0x80U  /* vmap backend memory */

/*
 * One mem range represents one range of virtual memory of a vspace.
 * It's an interface to other modules of sysmgr for data of vregion
 * it attached. Every vregion will has one mem range for other modules
 * in sysmgr.
 */
struct mem_range_s {
	uintptr_t start;
	uintptr_t end;
	size_t guard_size;
	uint64_t flags;
	uint32_t prot;
	void *private_data;
	uint64_t offset;
	union {
		const char *anon_name;
		struct {
			unsigned long fid;
			uint32_t cnode_idx;
#ifndef CONFIG_VFS_FUSION
			rref_t vfs_rref;
#endif
		} finfo;
	} u;
	/*
	 * Used for `vregion_prepare`, indicating whether the intersected
	 * vregions are unmapped.
	 */
	bool unmapped;
	uint8_t native_flags;
};

/* struct to hold path of a file used by mem_range_file_path */
struct mem_range_file_s {
	char *path_buff;
	size_t path_len;
	uintptr_t load_start;
	uintptr_t load_end;
};

/* _guard_size is 0 when CONFIG_VR_STACK_GUARD_SUPPORT is disabled */
#define MAP_RANGE_ARGS_INIT(_start, _end, _guard_size, _flags, _prot,	\
			    _native_flags)				\
{									\
	.start =	(_start),					\
	.end =		(_end),						\
	.guard_size =	(_guard_size),					\
	.flags =	(_flags),					\
	.prot =		(_prot),					\
	.private_data =	NULL,						\
	.offset =	0ULL,						\
	.u = 		{						\
				.anon_name = NULL,			\
			},						\
	.unmapped =	false,						\
	.native_flags =	(_native_flags),				\
}

/* traverse each mem range of one vspace */
int mem_range_traverse(struct vspace_s *vspace,
		       int (*callback)(const struct mem_range_s *r, void *d),
		       void *data);

/* find mem range contains one addr */
int mem_range_find(struct vspace_s *vspace, uintptr_t va,
		   struct mem_range_s *r);
/* find the file path of the vregion where the va is located. */
int mem_range_file_path(struct vspace_s *vspace, uintptr_t va, bool is_xact_fwd, bool need_load_range,
		struct mem_range_file_s *file_range);
uintptr_t mem_range_start_of_file(struct vspace_s *vspace, const char *fname);

/* segment name for one vregion attached to mem range */
int mem_range_segname(const struct vspace_s *vspace, const struct mem_range_s *r,
		      const struct symbol_desc *sym, char *name);

/* current memory range is dumpable or not */
bool mem_range_is_dumpable(const struct mem_range_s *r);

bool mem_range_is_iomm_backend(const struct mem_range_s *r);

int memmgr_enable_rsv_mem(uint64_t pfn_start, uint64_t pfn_end);

#ifdef CONFIG_MEMLEAK_PROBER
/* Check whether there exists leaked dynamic memory in sysmgr. */
void memmgr_mleak_probe(void);
bool memmgr_acquire_mleak_lock(void);
bool mleak_is_enabled(void);
#else /* CONFIG_MEMLEAK_PROBER */
static inline bool memmgr_acquire_mleak_lock(void)
{
	return true;
}
#endif /* CONFIG_MEMLEAK_PROBER */
#endif
