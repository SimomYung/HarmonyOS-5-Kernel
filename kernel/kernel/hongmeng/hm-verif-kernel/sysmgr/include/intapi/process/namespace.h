/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition of namespace data structure
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 04 16:13:28 2019
 */

#ifndef SYSMGR_INTAPI_PROCESS_NAMESPACE_H
#define SYSMGR_INTAPI_PROCESS_NAMESPACE_H

#include <stdint.h>
#include <libhmsync/raw_scopedptr.h>

enum ns_type {
	NS_TYPE_MNT,
	NS_TYPE_UTS,
	NS_TYPE_NUM,
};

struct process_ns_set {
	struct raw_refcnt refcnt;
	uint32_t ns_ids[NS_TYPE_NUM];
};

#endif
