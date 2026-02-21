/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Definition for saving loadinfo of kernel and system services
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 13 16:24:46 2024
 */

#ifndef KLIBS_LOADINFO_H
#define KLIBS_LOADINFO_H

#include <hmasm/page.h>
#include <hmkernel/types.h>

#define LOADINFO_MAGIC	0x00004f464e49444c /* "LDINFO" */

#define LOADINFO_MEM_MIN_SIZE	PAGE_SIZE

#define SYSSRV_NAME_MAX_LEN	32U
#define SYSSRV_LINFO_ALIGN_SIZE	8U

#define LINFO_DESC_UNINIT_VAL	(struct loadinfo_desc_s){ .offset = (~0ULL), .size = (~0ULL) }

enum kernsrv_loadinfo_type {
	KERNSRV_LOADINFO_TYPE_KERNEL,
	KERNSRV_LOADINFO_TYPE_SYSMGR,
	KERNSRV_LOADINFO_TYPE_IDLE,
	KERNSRV_LOADINFO_TYPE_HMVIRT,
	KERNSRV_LOADINFO_TYPE_MAX
};

struct loadinfo_desc_s {
	__u64 offset;
	__u64 size;
};

struct syssrv_loadinfo_s {
	char name[SYSSRV_NAME_MAX_LEN];
	__u32 cnode_idx;

	__u32 linfo_len;

	__u32 bin_nr;
	__u32 bin_name_total_len;

	struct loadinfo_desc_s bin_linfo[];
} __attribute__((packed));

/*
 * loadinfo layout:
 * +-------------------+----------------------+-------+----------------------+
 * | loadinfo_header_s | syssrv_loadinfo_s[0] |  ...  | syssrv_loadinfo_s[n] |
 * +-------------------+----------------------+-------+----------------------+
 */
struct syssrv_loadinfo_header_s {
	__u64 linfo_paddr;
	__u64 linfo_total_len;

	__u64 linfo_used_len;
	__u64 syssrv_nr;
} __attribute__((packed));

struct loadinfo_header_s {
	__u64 magic;
	__u64 kernsrv_cnt;
	__u64 reserved; /* reserved for future use */
	struct loadinfo_desc_s linfo[KERNSRV_LOADINFO_TYPE_MAX];
	struct syssrv_loadinfo_header_s syssrv;
} __attribute__((packed));

static inline __u64 syssrv_linfo_begin_vaddr(const struct syssrv_loadinfo_header_s *syssrv)
{
	return  __ptr_to_u64(syssrv) + (__u64)sizeof(*syssrv);
}
#endif
