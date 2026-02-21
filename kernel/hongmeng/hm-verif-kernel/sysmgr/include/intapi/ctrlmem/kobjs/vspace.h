/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: kobj vspace
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 27 19:48:15 2023
 */

#ifndef INTAPI_CTRLMEM_KOBJS_VSPACE_H
#define INTAPI_CTRLMEM_KOBJS_VSPACE_H

/*
 * We will update this usage model after we move everything in vspace_cap
 * to vspace kobj, that we can create vspace kobj by ourself and put
 * everything into vspace_s at first time, instead of spawn then link.
 * Now we use this usage model because vspace_s is inited after every kobj
 * is prepared, so we are unable to use info in vspace_s for creating.
 * We must first spawn a new kobj then link it with vspace_cap.
 *
 * After we remove cap_vspace and use vspace_kobj, we should simplify this
 * usage model to 2 functions: ctrlmem_kobj_vspace_create(struct vspace_s *vs)
 * at which we create kobj use infos in vs, and
 * ctrlmem_kobj_vspace_cleanup(struct vspace_s *vs) which cleanup kobjs in
 * vs.
 */

#include <intapi/ctrlmem/kobjctnr.h>
#include <intapi/ctrlmem/pcache.h>
#include <hmkernel/ctrlmem/kobjs/vspace.h>

struct vspace_s;
struct __ctrlmem_kobj_pcache_s;
extern struct __ctrlmem_kobj_vspace_s *ctrlmem_kobj_vspace_spawn(void *pages_linear_va,
								 unsigned long flags,
								 struct __ctrlmem_kobj_pcache_s *pcache);

extern int ctrlmem_kobj_vspace_destroy(struct __ctrlmem_kobj_vspace_s *kobj);

struct page_s;
struct kobjvs_pgdbuilder_s {
	struct page_s *pages;
	unsigned int nr_pages;
	struct __ctrlmem_kobj_vspace_s *kobj;

	void *pages_linear_va;
};

#define KOBJVS_PGDBUILDER_ZERO { \
	.pages = NULL, \
	.nr_pages = 0u, \
	.kobj = NULL, \
	.pages_linear_va = NULL, \
}

struct process_s;
extern int kobjvs_pgdbuilder_init(struct kobjvs_pgdbuilder_s *pgdbuilder,
				  const struct process_s *process);

extern void kobjvs_pgdbuilder_cancel(struct kobjvs_pgdbuilder_s *pgdbuilder);

/*
 * FIXME: temporary method which keep kobj alive and kill pgd pages.
 */
extern void kobjvs_pgdbuilder_pgd_free(struct kobjvs_pgdbuilder_s *pgdbuilder);
extern void kobjvs_invoke_supervsable(struct vspace_s *vs);

#endif
