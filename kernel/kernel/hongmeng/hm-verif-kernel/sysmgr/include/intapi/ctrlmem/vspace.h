/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Ctrlmem vspace in sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 13 17:13:43 2022
 */

#ifndef INTAPI_CTRLMEM_VSPACE_H
#define INTAPI_CTRLMEM_VSPACE_H

#include <hongmeng/errno.h>
#include <hmkernel/ctrlmem.h>
#include <hmkernel/ctrlmem/vspace.h>

void ctrlmem_vspace_init(const __u64 *meta);

void ctrlmem_vspace_dump(void);

struct process_s;
int ctrlmem_vspace_vsfault(const struct process_s *process, cref_t fault_vspace_cref);

struct __ctrlmem_vspace_slot_s *ctrlmem_vspace_preassign_slot(const struct process_s *process);

/*
 * At very beginning use idx to fetch slot. Now slot is represent vspace kernel
 * object. Currently we only use idx == 0 to maintain sysmgr's vspace. In next step
 * we should bind busy idx to its vspace structure. However, it is not as easy as
 * one may think, because real binding is done in kernel and there is no lock to
 * protect this assignment. It is possible that multiple cores try to assign an idx
 * to one vspace but only one winner, and the value written into that vspace structure
 * is not ensure to be the one accepted by kernel.
 *
 * Two possible solusion:
 * 1. Use a lockless ring buffer or a per-cpu array to save different assignment.
 *    When required, test these slots one-by-one;
 * 2. If not found, scan all slots.
 *
 * Finally we should bind busy idx to PGD (PGTBL_FL), make PGD to represent a vspace
 * kernel object. At that time kernel controls both PGD and vspace array. For kernel
 * it would be much easier to make them consistant with each other.
 */
struct __ctrlmem_vspace_slot_s *ctrlmem_vspace_slot_of(unsigned int idx);

const struct __ctrlmem_vspace_slot_s *ctrlmem_vspace_slot_of_sysmgr(void);

#endif
