/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: ULWT utils function header file
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jun 14 14:53:46 2024
 */
#ifndef MAPI_HMKERNEL_ULWT_UTILS_H
#define MAPI_HMKERNEL_ULWT_UTILS_H

#ifdef CONFIG_TSM_COMMON

#include <hmkernel/types.h>

#include <hmkernel/attributes/hmattributes.h>
__hmattr_tag(SILK);

struct CNode_s;
struct tcb_s;
struct ctrlmem_kobj_ulwt_s;
struct __ctrlmem_kobj_ulwt_s;
struct ctrlmem_kobj_process_s;

/* The following functions are used by all ulwt features. */
struct ctrlmem_kobj_process_s *ulwt_utils_cnode_kobj_process_of(struct CNode_s *cnode);
struct ctrlmem_kobj_thread_s *ulwt_utils_tcb_kobj_thread_of(struct tcb_s *tcb);
int ulwt_utils_kshm_init(unsigned long kshm_page_uva, struct pgstr_s **pgstr_ptr,
			 void **kva_ptr, __u32 pgstr_type);
void ulwt_utils_kshm_destroy(struct pgstr_s **pgstr_ptr, void **kva_ptr);
int ulwt_utils_kobj_ulwt_init(struct __ctrlmem_kobj_ulwt_s *ukobjulwt,
			      struct ctrlmem_kobj_ulwt_s **kobj_ulwt);
void ulwt_utils_kobj_ulwt_destroy(struct ctrlmem_kobj_ulwt_s **kobj_ulwt);
int ulwt_utils_tcb_read_tls_slot(struct tcb_s *tcb, unsigned int slot_num, unsigned long *value_ptr);
int ulwt_utils_tcb_write_tls_slot(struct tcb_s *tcb, unsigned int slot_num, unsigned long value);

/* The following functions are only used by ulwt monitor. */
#ifdef CONFIG_ULWT
void ulwt_utils_tcb_set_ulwt_exclusive(struct tcb_s *tcb);
void ulwt_utils_tcb_unset_ulwt_exclusive(struct tcb_s *tcb);
#endif /* CONFIG_ULWT */

#endif /* CONFIG_TSM_COMMON */

#endif /* MAPI_HMKERNEL_ULWT_MONITOR_H */
