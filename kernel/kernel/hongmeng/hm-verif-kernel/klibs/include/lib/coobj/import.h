/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: A Multi-data-structure generic co-object and its allocator
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 16 12:47:33 2022 +0800
 */
#ifndef KLIBS_COOBJ_IMPORT_H
#define KLIBS_COOBJ_IMPORT_H

#include <lib/coobj/utils.h>
#include <lib/coobj/__list.h>

struct ____coobj_chunkid_s {
	__relpos_t relpos;
	coobj_typeid_t coobj_typeid : ____COOBJ_TYPE_SIZE_BITS;
	__relpos_t level : ____COOBJ_RELPOS_BITS - ____COOBJ_TYPE_SIZE_BITS;
};
static inline
struct ____coobj_chunkid_s ____coobj_chunkid(
	coobj_typeid_t coobj_typeid, __relpos_t level, __relpos_t relpos)
{
	return (struct ____coobj_chunkid_s) {
		.relpos = relpos,
		.coobj_typeid = coobj_typeid,
		.level = level,
	};
}

struct ____coobj_resv_vm_info_s {
	__uptr_t resv_vm_start;
	__uptr_t resv_vm_end;
	__uptr_t resv_vm_cur;
};
/**************************************************
 * user
 **************************************************/
struct ____coobj_base_user_import_s {
	struct {
		/* ask mapper to map */
		int (*uholder_map)(void *memctx, struct ____coobj_chunkid_s chunkid, __relpos_t size, __bool large_obj);
		/* ask mapper to unmap */
		int (*uholder_unmap)(void *memctx, struct ____coobj_chunkid_s chunkid, __bool large_obj);
		/* tell mapper that the pointer is not referenced */
		int (*urenter_notify_noref)(void *memctx, struct ____coobj_chunkid_s chunkid);
	} mem;
	struct {
		/* Initialize the syncctx */
		void (*init)(void *syncctx);
		/* Lock by the syncctx */
		void (*lock)(void *syncctx);
		/* Unlock by the syncctx */
		void (*unlock)(void *syncctx);
	} sync;
};
struct ____coobj_base_user_context_s {
	void *memctx;
	/*
	 * Generic pointer to a lock. Note that we don't know the size of the
	 * lock. For example, the spin lock's implementation in kernel and user mode
	 * are different. And the mutex lock's implementation is also different. So
	 * we let the holder's runtime to pass the lock, instead of directly embedded
	 * the lock into the holder, so that the implementation is generic to any module.
	 */
	void *syncctx;
};

struct ____coobj_base_user_s {
	struct ____coobj_base_user_import_s *import;
	struct ____coobj_base_user_context_s *context;
};

static inline
int ____coobj_base_user_uholder_map(
	struct ____coobj_base_user_s *base_user,
	struct ____coobj_chunkid_s chunkid, __relpos_t size, __bool large_obj)
{
	return base_user->import->mem.uholder_map(base_user->context->memctx, chunkid, size, large_obj);
}

static inline
int ____coobj_base_user_uholder_unmap(
	struct ____coobj_base_user_s *base_user,
	struct ____coobj_chunkid_s chunkid, __bool large_obj)
{
	return base_user->import->mem.uholder_unmap(base_user->context->memctx, chunkid, large_obj);
}

static inline
int ____coobj_base_user_urenter_notify_noref(
	struct ____coobj_base_user_s *base_user,
	struct ____coobj_chunkid_s chunkid)
{
	return base_user->import->mem.urenter_notify_noref(base_user->context->memctx, chunkid);
}

static inline
void ____coobj_base_user_init(
	struct ____coobj_base_user_s *base_user)
{
	base_user->import->sync.init(base_user->context->syncctx);
}

static inline
void ____coobj_base_user_lock(
	struct ____coobj_base_user_s *base_user)
{
	base_user->import->sync.lock(base_user->context->syncctx);
}

static inline
void ____coobj_base_user_unlock(
	struct ____coobj_base_user_s *base_user)
{
	base_user->import->sync.unlock(base_user->context->syncctx);
}

/**************************************************
 * mapper
 **************************************************/
struct coobj_vspace_s;

enum {
	COOBJ_MAPPER_PERM_RO,
	COOBJ_MAPPER_PERM_RW,
	COOBJ_MAPPER_PERM_HKIP_ROWM,
};

struct ____coobj_base_mapper_import_s {
	struct {
		int (*init)(
			void *memctx,
			__uptr_t page_order);

		void *(*alloc)(
			void *memctx,
			__uptr_t datalen);
		void (*free)(
			void *memctx,
			void *ptr);

		/* datalen may be not aligned */
		int (*init_zero)(
			void *memctx,
			struct coobj_vspace_s *shmvs);
		int (*map_zero)(
			void *memctx,
			struct coobj_vspace_s *shmvs, __uptr_t datalen, __uptr_t page_uva);
		int (*unmap_zero)(
			void *memctx,
			struct coobj_vspace_s *shmvs, __uptr_t datalen,	__uptr_t page_uva);

		int (*map_notaddr)(
			void *memctx,
			struct coobj_vspace_s *shmvs, __uptr_t datalen, __u32 perm,
			____coobj_shmid_t *p_shmid, __uptr_t *p_page_uva, __bool islast);
		/*
		 * map_fixaddr is only used for specific obj types whose holder/renter reserves memory for it.
		 */
		int (*map_fixaddr)(
			void *memctx,
			struct coobj_vspace_s *shmvs, __uptr_t datalen, __u32 perm,
			____coobj_shmid_t *p_shmid, struct ____coobj_resv_vm_info_s *resv_vm_info,
			__uptr_t *p_page_uva, __bool islast);

		int (*map_notaddr_exist)(
			void *memctx,
			struct coobj_vspace_s *shmvs, __uptr_t datalen, __u32 perm,
			struct coobj_vspace_s *src_shmvs, ____coobj_shmid_t src_shmid,
			____coobj_shmid_t *p_shmid, __uptr_t *p_page_uva, __bool islast);

		/* fixaddr_exist is only used in renter. Since lnode and dnode in renter
		 * are respectively shared from mapper and holder, mapping to renter
		 * is ALWAYS with source shmvs and shmid.
		 */
		int (*map_fixaddr_exist)(
			void *memctx,
			struct coobj_vspace_s *shmvs, __uptr_t datalen, __u32 perm,
			struct coobj_vspace_s *src_shmvs, ____coobj_shmid_t src_shmid,
			____coobj_shmid_t *p_shmid, __uptr_t page_uva, __bool islast);
		int (*map_fixaddr_exist_update)(
			void *memctx,
			struct coobj_vspace_s *shmvs, __uptr_t datalen, __u32 perm,
			struct coobj_vspace_s *src_shmvs, ____coobj_shmid_t src_shmid,
			____coobj_shmid_t *p_shmid, struct ____coobj_resv_vm_info_s *resv_vm_info,
			__uptr_t *p_page_uva, __bool islast);
		/* unmap_fixaddr is used to unmap ashm when users invoke it directly or the mapper
		 * reverts a failed ashm mapping. page_uva indicates the starting va to unmap and
		 * p_page_resv_uva indicates the pointer of the current position of the reserved
		 * virtual memory area. To unmap mapper's page_uva, p_page_resv_uva should be NULL.
		 */
		int (*unmap_fixaddr)(
			void *memctx,
			struct coobj_vspace_s *shmvs, __uptr_t datalen,
			____coobj_shmid_t shmid, struct ____coobj_resv_vm_info_s *resv_vm_info, __uptr_t page_uva, __bool islast);
	} mem;

	struct {
		/* Initialize the syncctx */
		void (*init)(void *syncctx);
		/* Lock by the syncctx */
		void (*lock)(void *syncctx);
		/* Unlock by the syncctx */
		void (*unlock)(void *syncctx);
	} sync;
};

struct ____coobj_base_mapper_context_s {
	void *memctx;
	/*
	 * Generic pointer to a lock. Note that we don't know the size of the
	 * lock. For example, the spin lock's implementation in kernel and user mode
	 * are different. And the mutex lock's implementation is also different. So
	 * we let the holder's runtime to pass the lock, instead of directly embedded
	 * the lock into the holder, so that the implementation is generic to any module.
	 */
	void *syncctx;
};

struct ____coobj_base_mapper_s {
	struct ____coobj_base_mapper_import_s *import;
	struct ____coobj_base_mapper_context_s *context;
};

static inline
int ____coobj_base_mapper_init(struct ____coobj_base_mapper_s *base_mapper, __uptr_t page_order)
{
	return base_mapper->import->mem.init(
		base_mapper->context->memctx, page_order);
}
static inline
void *____coobj_base_mapper_alloc(struct ____coobj_base_mapper_s *base_mapper, __uptr_t datalen)
{
	return base_mapper->import->mem.alloc(
		base_mapper->context->memctx, datalen);
}
static inline
void ____coobj_base_mapper_free(struct ____coobj_base_mapper_s *base_mapper, void *ptr)
{
	return base_mapper->import->mem.free(
		base_mapper->context->memctx, ptr);
}

static inline
int ____coobj_base_mapper_init_zero(
	struct ____coobj_base_mapper_s *base_mapper, struct coobj_vspace_s *shmvs)
{
	return base_mapper->import->mem.init_zero(
		base_mapper->context->memctx, shmvs);
}
static inline
int ____coobj_base_mapper_map_zero(
	struct ____coobj_base_mapper_s *base_mapper, struct coobj_vspace_s *shmvs, __uptr_t datalen,
	__uptr_t page_uva)
{
	return base_mapper->import->mem.map_zero(
		base_mapper->context->memctx, shmvs, datalen, page_uva);
}
static inline
int ____coobj_base_mapper_unmap_zero(
	struct ____coobj_base_mapper_s *base_mapper, struct coobj_vspace_s *shmvs, __uptr_t datalen,
	__uptr_t page_uva)
{
	return base_mapper->import->mem.unmap_zero(
		base_mapper->context->memctx, shmvs, datalen, page_uva);
}

static inline
int ____coobj_base_mapper_map_notaddr(
	struct ____coobj_base_mapper_s *base_mapper, struct coobj_vspace_s *shmvs, __uptr_t datalen, __u32 perm,
	____coobj_shmid_t *p_shmid, __uptr_t *p_page_uva, __bool islast)
{
	return base_mapper->import->mem.map_notaddr(
		base_mapper->context->memctx, shmvs, datalen, perm, p_shmid, p_page_uva, islast);
}
static inline
int ____coobj_base_mapper_map_notaddr_exist(
	struct ____coobj_base_mapper_s *base_mapper, struct coobj_vspace_s *shmvs, __uptr_t datalen, __u32 perm,
	struct coobj_vspace_s *src_shmvs, ____coobj_shmid_t src_shmid,
	____coobj_shmid_t *p_shmid, __uptr_t *p_page_uva, __bool islast)
{
	return base_mapper->import->mem.map_notaddr_exist(
		base_mapper->context->memctx, shmvs, datalen, perm, src_shmvs, src_shmid, p_shmid, p_page_uva, islast);
}
static inline
int ____coobj_base_mapper_map_fixaddr(
	struct ____coobj_base_mapper_s *base_mapper, struct coobj_vspace_s *shmvs, __uptr_t datalen, __u32 perm,
	____coobj_shmid_t *p_shmid, struct ____coobj_resv_vm_info_s *resv_vm_info, __uptr_t *p_page_uva, __bool islast)
{
	return base_mapper->import->mem.map_fixaddr(
		base_mapper->context->memctx, shmvs, datalen, perm, p_shmid, resv_vm_info, p_page_uva, islast);
}
static inline
int ____coobj_base_mapper_map_fixaddr_exist(
	struct ____coobj_base_mapper_s *base_mapper, struct coobj_vspace_s *shmvs, __uptr_t datalen, __u32 perm,
	struct coobj_vspace_s *src_shmvs, ____coobj_shmid_t src_shmid,
	____coobj_shmid_t *p_shmid, __uptr_t page_uva, __bool islast)
{
	return base_mapper->import->mem.map_fixaddr_exist(
		base_mapper->context->memctx, shmvs, datalen, perm, src_shmvs, src_shmid, p_shmid, page_uva, islast);
}
static inline
int ____coobj_base_mapper_map_fixaddr_exist_update(
	struct ____coobj_base_mapper_s *base_mapper, struct coobj_vspace_s *shmvs, __uptr_t datalen, __u32 perm,
	struct coobj_vspace_s *src_shmvs, ____coobj_shmid_t src_shmid,
	____coobj_shmid_t *p_shmid, struct ____coobj_resv_vm_info_s *resv_vm_info, __uptr_t *p_page_uva, __bool islast)
{
	return base_mapper->import->mem.map_fixaddr_exist_update(
		base_mapper->context->memctx, shmvs, datalen, perm, src_shmvs, src_shmid,
		p_shmid, resv_vm_info, p_page_uva, islast);
}
static inline
int ____coobj_base_mapper_unmap_fixaddr(
	struct ____coobj_base_mapper_s *base_mapper, struct coobj_vspace_s *shmvs, __uptr_t datalen,
	____coobj_shmid_t shmid, struct ____coobj_resv_vm_info_s *resv_vm_info, __uptr_t page_uva, __bool islast)
{
	return base_mapper->import->mem.unmap_fixaddr(
		base_mapper->context->memctx, shmvs, datalen, shmid, resv_vm_info, page_uva, islast);
}

static inline
void ____coobj_base_mapper_sync_init(
	struct ____coobj_base_mapper_s *base_mapper)
{
	base_mapper->import->sync.init(base_mapper->context->syncctx);
}

static inline
void ____coobj_base_mapper_sync_lock(
	struct ____coobj_base_mapper_s *base_mapper)
{
	base_mapper->import->sync.lock(base_mapper->context->syncctx);
}

static inline
void ____coobj_base_mapper_sync_unlock(
	struct ____coobj_base_mapper_s *base_mapper)
{
	base_mapper->import->sync.unlock(base_mapper->context->syncctx);
}

#endif
