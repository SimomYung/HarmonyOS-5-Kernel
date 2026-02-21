/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Definition of cap_cnode for module
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 28 11:25:19 CST 2020
 */

#ifndef MAPI_HMKERNEL_CAP_CNODE_H
#define MAPI_HMKERNEL_CAP_CNODE_H

#include <uapi/hmkernel/capability.h>
#include <hmkernel/types.h>
#include <hmkernel/compiler.h>

struct capability_s;
struct CNode_s;

struct capability_s *
cnode_lookup_type(struct CNode_s *cnode, cref_t cref, enum hmobj cap_type);

struct capability_s *
cnode_lookup_and_get(struct CNode_s *cnode, cref_t cref,
		     enum hmobj cap_type);
struct capability_s *
cnode_lookup_and_get_nolock(struct CNode_s *cnode, cref_t cref,
		     enum hmobj cap_type);

int cap_refcnt_inc(struct capability_s *pcap);
int cap_refcnt_dec(struct capability_s *pcap);

#endif /* end of MAPI_HMKERNEL_CAP_CNODE_H */
