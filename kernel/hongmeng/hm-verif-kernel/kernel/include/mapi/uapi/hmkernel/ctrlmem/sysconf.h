/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: ctrlmem sysconf base definition
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 04 18:29:20 2023
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_SYSCONF_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_SYSCONF_H

#include <hmasm/page.h>
#include <hmkernel/types.h>

#define __CTRLMEM_SYSCONF_VERSION	0

/*
 * 16 bytes
 */
struct __ctrlmem_sysconf_version_s {
	__u64 meta;
	__u64 version;
};

struct __ctrlmem_sysconf_spinhdlr_ptlwfe_state_s {
	union {
		struct {
			union {
				void *rply_buf;
				__u64 padding1;
			};
			union {
				unsigned long rply_buf_sz;
				__u64 padding2;
			};
		} u;
		__u64 padding[6];
	};
};

struct __ctrlmem_sysconf_spincalls_s {
	__u64 ctrlmem_write_v32;
	__u64 ctrlmem_write_v64;
	__u64 ctrlmem_cas_v32;
	__u64 ctrlmem_cas_v64;

	__u64 spinhdlr_ptlwfe_check;
	__u64 spinhdlr_ptlwfe_commit;
	__u64 spinhdlr_dfrwfe;
	__u64 spinhdlr_fallback;

	__u64 spinhdlr_ptlwfe_exphdlr;
	__u64 spinhdlr_ptlwfe_lsyscall;
	__u64 spinhdlr_ptlwfe_lsyscall_getppid;
	__u64 spinhdlr_ptlwfe_lsyscall_fcntl;
	__u64 spinhdlr_ptlwfe_lsyscall_mprotect;
	__u64 spinhdlr_ptlwfe_lsyscall_madvise;

	__u64 spinhdlr_trace_shm_set_addr;
	__u64 spinhdlr_trace_shm_set_futex_addr;
	__u64 spinhdlr_trace_shm_set_snapshot_addr;
	__u64 spinhdlr_trace_shm_set_saved_taskinfo_addr;
	__u64 spinhdlr_set_uentry;

	__u64 spinhdlr_sched_trans;
	__u64 spinhdlr_secmem_change_klinear;
	__u64 spinhdlr_pa_to_linmap_pte;

	__u64 ctrlmem_map_pg_fast;
	__u64 ctrlmem_map_batch_fast;
	__u64 ctrlmem_cas_map_pg;
	__u64 ctrlmem_unmap_range_fast;

	__u64 spinhdlr_resume_thread;
	__u64 spinhdlr_wake_thread;

	__u64 ctrlmem_clean_set_kobjvs;
	__u64 ctrlmem_remap_pgva_kobjvs;
	__u64 ctrlmem_tlbgather_unmap;
	__u64 ctrlmem_clear_abit;
	__u64 ctrlmem_channelpg_unlock;

	__u64 spinhdlr_fdtable_lsyscall_fd;
	__u64 spinhdlr_fdtable_install_fd;
	__u64 spinhdlr_fdtable_close_fd;
	__u64 spinhdlr_fdtable_alloc_fd;
	__u64 spinhdlr_fdtable_dup_fd;
	__u64 spinhdlr_fdtable_free_fd;
	__u64 spinhdlr_fdtable_alloc_install_fd;

	__u64 spinhdlr_thread_sched_setattr;
	__u64 spinhdlr_thread_stat;
	__u64 spinhdlr_thread_lt_set_conf;

	__u64 spinhdlr_set_caller_compat_state;
};

struct __ctrlmem_sysconf_global_s {
	__u32 is_ctrlmem_pgfault_enabled;
	__u32 is_ctrlmem_spincall_cocall_enabled;
	__u32 is_ctrlmem_pte_access_flag_enabled;
	__u32 is_pgtbl_fragile_enabled;
};

struct __ctrlmem_sysconf_hkip_s {
	__u64 ctrlmem_sec_region_start_addr;
	__u64 ctrlmem_sec_region_end_addr;
	__u32 is_ctrlmem_hkip_enabled;
	__u32 ctrlmem_hkip_protect;
	__u32 ctrlmem_hkip_rowm_cpy;
	__u32 ctrlmem_hkip_rowm_set;
	__u32 ctrlmem_sec_region_protect;
	__u32 padding;
};

struct __ctrlmem_sysconf_sysproc_s {
	__u32 sysproc_initrd_size;
};

/* Max number of static registering cma range: 16 */
#define __SYSCONF_CONTIGUOUS_NR 32
struct __ctrlmem_sysconf_phymem_s {
	__u64 contig_paddr_pairs[__SYSCONF_CONTIGUOUS_NR];
};

struct __ctrlmem_sysconf_klayout_s {
	__u64 k_stext_addr;
	__u64 k_etext_addr;
	__u64 k_sdata_addr;
	__u64 k_edata_addr;
	__u64 k_linear_l_va_start;
	__u64 k_linear_l_va_end;
	__u64 k_linear_l_pv_offset;
	__u64 k_linear_h_va_start;
	__u64 k_linear_h_va_end;
	__u64 k_linear_h_pv_offset;
	__u64 k_linear_low_va_end;
};

/*
 * The struct of emergency domain contains emergency stack and
 * several emergency functions, such as ufs_lush.
 * 1) emrg_stk is used for running emergency functions by kernel
 *    before system reboot emergency.
 * 2) Currently only emrg_ufs_flush, which will be registered
 *    by priviledged process, is supported.
 * 3) In future, we may support un priviledged process to register
 *    emrg domain fuctions, for which, the corresponding vspace
 *    should be record at the same time.
 */
struct __ctrlmem_sysconf_emrg_domain_s {
	__u64 emrg_stk;
	__u64 emrg_ufs_flush;
	__u64 sysmgr_load_start;
};

struct __ctrlmem_sysconf_perf_s {
	__u32 perf_cpu_time_max_percent;
	__u32 perf_event_max_sample_rate;
};

struct __ctrlmem_sysconf_bootup_keypoint_s {
	__u32 bootup_stage;
};

enum __ctrlmem_sysconf_reserv_mem_type {
	__CTRLMEM_SYSCONF_RESERV_MEM_TRACKINFO,
	__CTRLMEM_SYSCONF_RESERV_MEM_LASTKLOG,
	__CTRLMEM_SYSCONF_RESERV_MEM_TYPE_MAX
};

struct __ctrlmem_sysconf_reserv_mem_config_s {
	__u64 addr;
	__u32 size;
	__u32 padding;
};

struct __ctrlmem_sysconf_cpu_support_compat_num_s {
	__s32 cpu_support_compat_num;
};

#define __CTRLMEM_SYSCONF_CPUINFO_MAX_CPU_CNT		32
#define __CTRLMEM_SYSCONF_CPUINFO_MAX_REG_CNT		10

struct __ctrlmem_sysconf_register_pcp_s {
	__u64 id_registers[__CTRLMEM_SYSCONF_CPUINFO_MAX_REG_CNT];
	__bool inited;
};

struct __ctrlmem_sysconf_topo_pcp_s {
	__u32 core;
	__u32 cluster;
	__u32 capacity_level;
};

struct __ctrlmem_sysconf_cpuinfo_s {
	struct __ctrlmem_sysconf_register_pcp_s cpus[__CTRLMEM_SYSCONF_CPUINFO_MAX_CPU_CNT];
	struct __ctrlmem_sysconf_topo_pcp_s topos[__CTRLMEM_SYSCONF_CPUINFO_MAX_CPU_CNT];
};

#define __CTRLMEM_SYSCONF_MAX_PAGE_CNT (2)
#define __CTRLMEM_SYSCONF_MAX_SIZE (__CTRLMEM_SYSCONF_MAX_PAGE_CNT * __PAGE_SIZE)

/*
 * Static assert to make sure sizeof(struct __ctrlmem_sysconf_page_s) == 4096
 */
struct __ctrlmem_sysconf_page_s {
	/* u64 */
	struct __ctrlmem_sysconf_version_s	version;
	struct __ctrlmem_sysconf_spincalls_s	spincalls;
	struct __ctrlmem_sysconf_phymem_s	phymem;
	struct __ctrlmem_sysconf_emrg_domain_s	emrg;
	struct __ctrlmem_sysconf_klayout_s	klayout;
	struct __ctrlmem_sysconf_cpuinfo_s	cpuinfo;
	struct __ctrlmem_sysconf_hkip_s		hkip;

	struct __ctrlmem_sysconf_reserv_mem_config_s	reserv_mem_configs[__CTRLMEM_SYSCONF_RESERV_MEM_TYPE_MAX];
	/* u32 */
	struct __ctrlmem_sysconf_cpu_support_compat_num_s cpu_support_compat_num;
	struct __ctrlmem_sysconf_global_s	global;
	struct __ctrlmem_sysconf_sysproc_s	sysproc;
	struct __ctrlmem_sysconf_perf_s		perf;
	struct __ctrlmem_sysconf_bootup_keypoint_s	keypoint;

	__u8 padding[__CTRLMEM_SYSCONF_MAX_SIZE - sizeof(struct __ctrlmem_sysconf_version_s)
			     - sizeof(struct __ctrlmem_sysconf_spincalls_s)
			     - sizeof(struct __ctrlmem_sysconf_phymem_s)
			     - sizeof(struct __ctrlmem_sysconf_emrg_domain_s)
			     - sizeof(struct __ctrlmem_sysconf_klayout_s)
			     - sizeof(struct __ctrlmem_sysconf_cpuinfo_s)
			     - sizeof(struct __ctrlmem_sysconf_global_s)
			     - sizeof(struct __ctrlmem_sysconf_sysproc_s)
			     - sizeof(struct __ctrlmem_sysconf_hkip_s)
			     - sizeof(struct __ctrlmem_sysconf_perf_s)
			     - sizeof(struct __ctrlmem_sysconf_bootup_keypoint_s)
				 - sizeof(struct __ctrlmem_sysconf_reserv_mem_config_s) * __CTRLMEM_SYSCONF_RESERV_MEM_TYPE_MAX
			     - sizeof(struct __ctrlmem_sysconf_cpu_support_compat_num_s)];
} __attribute__((packed));

#endif
