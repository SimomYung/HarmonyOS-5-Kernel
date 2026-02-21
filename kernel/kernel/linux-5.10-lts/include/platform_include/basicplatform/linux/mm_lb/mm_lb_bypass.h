/*
 * Copyright (c) 2018-2019 Huawei Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __MM_LB_BYPASS_H
#define __MM_LB_BYPASS_H
#include <linux/scatterlist.h>

/* it need same with dts */
enum lb_pid {
	PID_BY_PASS = 0,

	PID_CAMAIP = 0,
	PID_AUDIO   = 0,
	PID_GPUFBO  = 0,
	PID_GPUTXT  = 0,
	PID_IDISPLAY = 0,
	PID_NPU     = 0,
	PID_VIDEO   = 0,
	PID_CAMTOF = 0,
	PID_TINY    = 0,
	PID_ISPNN   = 0,
	PID_ISPPIPE = 0,
	PID_VENC    = 0,
	PID_DSS     = 0,
	PID_VOICE   = 0,
	PID_HIFI    = 0,
	PID_VDEC    = 0,

	PID_MAX,
};

struct lb_policy_config {
	u32 pid;
	u32 quota;
	u32 prio;
	u32 available;
};

struct lb_policy_flow {
	u32 pid;
	u32 in;
	u32 out;
};

enum policy_type {
	MASTER_GID,
	MASTER_QUOTA,
	MASTER_PLC,
};
static inline void __lb_assert_page(struct page *pg) {}
static inline void __lb_assert_phys(phys_addr_t phys) {}
static inline void __lb_assert_pte(pte_t pte) {}
static inline void *lb_page_to_virt(struct page *page) { return 0; }
static inline u32 lb_page_to_gid(struct page *page) { return 0; }
static inline int lb_request_quota(u32 pid) { return 0; }
static inline int lb_release_quota(u32 policy_id) { return 0; }
static inline struct page *lb_alloc_pages(u32 pid, gfp_t gfp_mask, u32 order) { return 0; }
static inline int lb_free_pages(u32 pid, struct page *pages, u32 order) { return 0; }
static inline u64 lb_pte_attr(phys_addr_t phy_addr) { return 0; }
static inline u64 lb_pte_attr_gpu(phys_addr_t phy_addr) { return 0; }
static inline int lb_prot_build(struct page *pages, pgprot_t *pageprot) { return 0; }
static inline int lb_pid_prot_build(struct page *pages, pgprot_t *pageprot) { return 0; }
static inline int lb_sg_attach(u32 pid, struct scatterlist *sgl, u32 nents) { return 0; }
static inline int lb_sg_detach(u32 pid, struct scatterlist *sgl, u32 nents) { return 0; }
static inline int lb_pages_attach(u32 pid, struct page *pg, size_t count) { return 0; }
static inline int lb_pages_detach(u32 pid, struct page *pg, size_t count) { return 0; }
static inline u64 lb_pid_to_gidphys(u32 pid) { return 0; }
static inline u32 is_lb_available(void) { return 0; }
static inline u32 lb_get_available_size(void) { return 0; }
static inline u32 lb_get_master_policy(u32 pid, enum policy_type type) { return 0; }
static inline int lb_down_policy_prio(u32 pid) { return 0; }
static inline int lb_up_policy_prio(u32 pid) { return 0; }
static inline int lb_gid_enable(u32 pid) { return 0; }
static inline int lb_gid_bypass(u32 pid) { return 0; }
static inline int perf_ctrl_lb_set_policy(void __user *uarg) { return 0; }
static inline int perf_ctrl_lb_reset_policy(void __user *uarg) { return 0; }
static inline int perf_ctrl_lb_set_quota(void __user *uarg) { return 0; }
static inline int perf_ctrl_lb_reset_quota(void __user *uarg) { return 0; }
static inline int perf_ctrl_lb_lite_info(void __user *uarg) { return 0; }
static inline int get_flow_stat(struct lb_policy_flow *flow) { return 0; }
static inline u32 get_sc_size(void) {return 0;}
static inline int lb_mpam_alloc(u32 mpam_id, u32 waybitmap) { return 0; }
static inline int lb_mpam_free(u32 mpam_id) { return 0; }

static inline void lb_ci_cache_exclusive(void) { return; }
static inline u32 lb_get_pid_by_userpid(u32 user_pid) { return user_pid; }

#ifdef CONFIG_DMABUF_UDK_EN
static inline int lb_pages_attach_from_udk(u32 pid, struct page *pg, size_t count) {return 0;}
static inline int lb_sg_attach_from_udk(u32 pid, struct scatterlist *sgl, u32 nents) {return 0;}
static inline int lb_pages_detach_from_udk(u32 pid, struct page *pg, size_t count) {return 0;}
static inline int lb_sg_detach_from_udk(u32 pid, struct scatterlist *sgl, u32 nents) {return 0;}
#endif /* CONFIG_DMABUF_UDK_EN */

#endif
