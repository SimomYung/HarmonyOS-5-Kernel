/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: smmu module header
 * Create: 2024/12/12
 */

#ifndef UAPI_HMKERNEL_SMMU_MODULE_H
#define UAPI_HMKERNEL_SMMU_MODULE_H

#include <hmkernel/types.h>

enum __smmu_cmd {
	__SMMU_MAP = 0,
	__SMMU_UNMAP,
	__SMMU_MAP_SG,
	__SMMU_MAP_BATCH,
	__SMMU_UNMAP_BATCH,
	__SMMU_IOVA_TO_PHYS,
	__SMMU_ATTACH_DEV,
	__SMMU_ATTACH_NEW_DEV,
	__SMMU_SVM_DETACH_DEV,
	__SMMU_SMMU_DEVICE_REMOVE,
	__SMMU_SMMU_DOMAIN_FREE,
	__SMMU_TLB_INV_CONTEXT,
	__SMMU_TLB_INV_CONTEXT_DEV,
	__SMMU_FLUSH_TTWC,
	__SMMU_TCU_POWER_ON,
	__SMMU_TCU_POWER_OFF,
	__SMMU_GET_TTBR,
	__SMMU_GET_TCU_INFO,
	__SMMU_PMU_TCU_CONFIG,
	__SMMU_UNMAP_INV_CACHE,
	__SMMU_CMD_MAX,
};

struct smmu_tcu_power_on_paras {
    __u32 smmuid;
};
 
struct smmu_tcu_power_off_paras {
    __u32 smmuid;
};

struct smmu_map_paras {
	__u64 paddr;
	__u64 iova;
	__u64 prot;
	__u32 sid;
	__u64 size;
	__u64 mapped_size;
};

struct smmu_unmap_paras {
	__u64 iova;
	__u32 sid;
	__u64 size;
	__u64 unmap_size;
};

#define MAX_SMMU_MAP_PAGE_BATCH_NUMS     128

struct smmu_map_sg_paras {
	__u64 phys[MAX_SMMU_MAP_PAGE_BATCH_NUMS];
	__u64 iova;
	__u32 length[MAX_SMMU_MAP_PAGE_BATCH_NUMS];
	__u64 prot[MAX_SMMU_MAP_PAGE_BATCH_NUMS];
	__u32 count;
	__u16 sid;
	__u16 map_page_batch;
	__u64 tlb_iova;
	__u64 tlb_size;
};

struct smmu_unmap_inv_cache_paras {
	__u64 inv_iova;
	__u32 length[MAX_SMMU_MAP_PAGE_BATCH_NUMS];
	__u64 unmap_iova;
	__u64 size;
	__u32 sid;
	__u16 count;
	__u16 is_unmap;
};

struct smmu_map_batch_paras {
	__u64 phys[MAX_SMMU_MAP_PAGE_BATCH_NUMS];
	__u64 iova[MAX_SMMU_MAP_PAGE_BATCH_NUMS];
	__u32 length[MAX_SMMU_MAP_PAGE_BATCH_NUMS];
	__u64 prot;
	__u32 count;
	__u32 sid;
	__bool is_coherent;
	__bool is_dcache_flush;
};
 
struct smmu_unmap_batch_paras {
	__u64 iova[MAX_SMMU_MAP_PAGE_BATCH_NUMS];
	__u32 length[MAX_SMMU_MAP_PAGE_BATCH_NUMS];
	__u32 count;
	__u32 sid;
	__bool is_coherent;
	__bool is_dma_2_dev;
};

struct smmu_iova_paras {
	__u64 iova;
	__u64 paddr;
    __u32 sid;
};

struct smmu_attach_paras {
    __u32 smmuid;
    __u32 sid;
    __u32 ssid;
    __u32 tlb_flush;
    __u32 domain_type;
    bool is_svm;
    __u32 svm_ssid;
    __u64 svm_ttbr;
    __u32 ssid_nums;
    __u32 asid;
};

struct smmu_detach_paras {
    __u32 sid;
    __u32 svm_ssid;
};

struct smmu_remove_paras {
    __u32 smmuid;
};

struct domain_free_paras {
    __u32 sid;
};

struct smmu_tlb_paras {
    __u32 sid;
    bool is_svm;
    __u32 ssid;
};

struct smmu_ttwc_paras {
    __u32 sid;
};

struct smmu_ttbr_paras {
    __u32 sid;
    __u64 ttbr;
};

struct smmu_tcu_info_paras {
    __u32 smmuid;
	__u64 strtab_base;
	__u64 strtab_base_cfg;
	__u64 cmdq_q_qbase;
	__u64 evtq_q_qbase;
};

#define PMCG_EVENT_MAX_CNT_NUM		16

struct smmu_pmu_tcu_config_paras {
	__u32 cmd;
	__u32 smmuid;
	__u32 sid;
	__u32 evt_type_size;
	__u32 pmcg_srv_size;
	__u8 evt_type[PMCG_EVENT_MAX_CNT_NUM];
	__u32 pmcg_serv[PMCG_EVENT_MAX_CNT_NUM];
};

#endif
