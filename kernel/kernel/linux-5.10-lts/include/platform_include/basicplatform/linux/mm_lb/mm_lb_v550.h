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

#ifndef __MM_LB_V550_H
#define __MM_LB_V550_H
#include <linux/scatterlist.h>

/* it need same with dts */
enum lb_pid {
	PID_BYPASS = 0,         // CPU ISPCPU
	PID_ISP = 1,            // ISP
	PID_OISP = 1,           // OISP
	PID_AUDIO = 2,          // DSP
	PID_SMARTISP_H = 3,     // NPU
	PID_GPU_PAGETABLE = 4,  // GPU
	PID_AIRAW = 5,          // NPU
	PID_NPU = 6,            // NPU
	PID_FRAMEBUFFER = 7,    // GPU->DSS
	PID_SWING = 8,          // sensorhub
	PID_AOD = 9,            // sensorhub
	PID_NPU_LOWPRIO = 10,   // NPU
	PID_SMARTISP_L = PID_NPU_LOWPRIO,    // NPU
	PID_PROJECTION = 13,    // GPU->VENC
	PID_GPU_VERTEX = 14,    // GPU
	PID_AIC = 16,           // NPU
	PID_DSS = 17,           // DSS
	PID_MINI_ISP = 18,      // Mini isp
	PID_VOICE = 19,         // DSP
	PID_HIFI = 19,          // DSP
	PID_MODEM_START = 20,   // MODEM
	PID_MODEM_END = 23,     // MODEM
	PID_VDEC_START = 24,    // VDEC->DSS
	PID_VDEC_END = 39,      // VDEC->DSS
	PID_VENC = 40,          // VENC
	PID_VDEC = 41,          // VDEC
	PID_GPU_DESC = 42,      // GPU
	PID_GPU_HEBC = 43,      // GPU
	PID_GPU_RSV0 = 44,      // GPU
	PID_GPU_RSV1 = 45,      // GPU
	PID_GPU_TXT = 46,       // GPU
	PID_GPU_FBO = 47,       // GPU
	PID_GPU_FBO_START = 48, // GPU
	PID_GPU_FBO_END = 63,   // GPU
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

struct lb_flow_ctrl {
	u32 pid;
	u32 ctrl0;
	u32 ctrl1;
};

enum policy_type {
	MASTER_GID,
	MASTER_QUOTA,
	MASTER_PLC,
};

void __lb_assert_page(struct page *pg);
void __lb_assert_phys(phys_addr_t phys);
void __lb_assert_pte(pte_t pte);
void *lb_page_to_virt(struct page *page);
u32 lb_page_to_gid(struct page *page);
int lb_request_quota(u32 pid);
int lb_release_quota(u32 policy_id);
struct page *lb_alloc_pages(u32 pid, gfp_t gfp_mask, u32 order);
struct page *lb_alloc_pages_osh(u32 pgid, gfp_t gfp_mask, u32 order);
struct page *lb_alloc_pages_bigbw(u32 pgid, gfp_t gfp_mask, u32 order);
struct page *lb_alloc_pages_osh_bigbw(u32 pgid, gfp_t gfp_mask, u32 order);
int lb_free_pages(u32 pid, struct page *pages, u32 order);
u64 lb_pte_attr(phys_addr_t phy_addr);
u64 lb_pte_attr_gpu(phys_addr_t phy_addr);
int lb_prot_build(struct page *pages, pgprot_t *pageprot);
int lb_pid_prot_build(u32 pid, pgprot_t *pageprot);
int lb_pages_attach(u32 pid, struct page *pg, size_t count);
int lb_sg_attach(u32 pid, struct scatterlist *sgl, u32 nents);
int lb_pages_detach(u32 pid, struct page *pg, size_t count);
int lb_sg_detach(u32 pid, struct scatterlist *sgl, u32 nents);
u64 lb_pid_to_gidphys(u32 pid);
u32 is_lb_available(void);
u32 lb_get_available_size(void);
u32 lb_get_master_policy(u32 pid, enum policy_type type);
int lb_down_policy_prio(u32 pid);
int lb_up_policy_prio(u32 pid);
int lb_gid_enable(u32 pid);
int lb_gid_bypass(u32 pid);
int perf_ctrl_lb_set_policy(void __user *uarg);
int perf_ctrl_lb_reset_policy(void __user *uarg);
int perf_ctrl_lb_set_quota(void __user *uarg);
int perf_ctrl_lb_reset_quota(void __user *uarg);
int perf_ctrl_lb_lite_info(void __user *uarg);
int get_flow_stat(struct lb_policy_flow *flow);
u32 get_sc_size(void);
u32 lb_set_flow(struct lb_flow_ctrl *flow, u32 statis_num);
u32 lb_get_flow_in(u32 statis_num);
u32 lb_get_flow_out(u32 statis_num);
int lb_mpam_way_set(u32 mpam_id, u32 way);
int lb_mpam_way_reset(u32 mpam_id);
int lb_mpam_set_replace_policy(u32 mpam_id, u32 revisit_age);
u32 lb_mpam_get_replace_policy(u32 mpam_id);
int lb_mpam_set_evict_en(u32 mpam_id, u32 enable);
u32 lb_mpam_get_evict_en(void);
int lb_release_syncbuf(void);
int lb_alloc_syncbuf(u32 size);

u32 lb_get_page_osh_flag(struct page *page);
void lb_sg_set_osh(struct scatterlist *sgl, u32 nents, u32 osh);
void lb_pages_set_osh(struct page *pg, size_t count, u32 osh);
void pgid_validate(u32 pgid_idx);
u32 lb_get_pid_by_userpid(u32 user_pid);

#ifndef declare_sc_property
#define declare_sc_property(type_name, property) \
	u32 get_##type_name##_##property(const u32 gid); \
	void set_##type_name##_##property(const u32 gid, const u32 val, const u32 direct);
#endif

declare_sc_property(pgid, disable)
declare_sc_property(pgid, drop)
declare_sc_property(pgid, self_replaceable)
declare_sc_property(pgid, priority)
declare_sc_property(pgid, rd_policy)
declare_sc_property(pgid, wr_policy)

#ifdef CONFIG_MM_LB_L3_EXTENSION
void lb_ci_cache_exclusive(void);
#endif

#ifdef CONFIG_DMABUF_UDK_EN
int lb_pages_attach_from_udk(u32 pid, struct page *pg, size_t count);
int lb_sg_attach_from_udk(u32 pid, struct scatterlist *sgl, u32 nents);
int lb_pages_detach_from_udk(u32 pid, struct page *pg, size_t count);
int lb_sg_detach_from_udk(u32 pid, struct scatterlist *sgl, u32 nents);
extern int get_cpu_pgid(u32 pgid_idx);
#else
static inline int lb_pages_attach_from_udk(u32 pid, struct page *pg, size_t count) {return 0;}
static inline int lb_sg_attach_from_udk(u32 pid, struct scatterlist *sgl, u32 nents) {return 0;}
static inline int lb_pages_detach_from_udk(u32 pid, struct page *pg, size_t count) {return 0;}
static inline int lb_sg_detach_from_udk(u32 pid, struct scatterlist *sgl, u32 nents) {return 0;}
#endif /* CONFIG_DMABUF_UDK_EN */

#endif