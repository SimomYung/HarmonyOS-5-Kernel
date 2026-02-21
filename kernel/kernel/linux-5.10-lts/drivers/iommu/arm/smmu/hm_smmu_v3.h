/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: hisi smmu v3 driver
 * Date: 2024/12/07
 */

#ifndef __HM_SMMU_V3_H__
#define __HM_SMMU_V3_H__

#include <linux/bitmap.h>
#include "mm_smmu.h"

/* Stage-1 PTE */
#define ARM_LPAE_PTE_AP_UNPRIV		(((u64)1) << 6)
#define ARM_LPAE_PTE_AP_RDONLY		(((u64)2) << 6)
#define ARM_LPAE_PTE_ATTRINDX_SHIFT	2
#define ARM_LPAE_PTE_nG			(((u64)1) << 11)
#ifdef CONFIG_MM_IO_PGTABLE_EBIT
#define MM_LPAE_PTE_EBIT        (1UL << 42)
#endif

#define ARM_LPAE_PTE_NSTABLE		(((u64)1) << 63)
#define ARM_LPAE_PTE_XN			(((u64)3) << 53)
#define ARM_LPAE_PTE_AF			(((u64)1) << 10)
#define ARM_LPAE_PTE_SH_NS		(((u64)0) << 8)
#define ARM_LPAE_PTE_SH_OS		(((u64)2) << 8)
#define ARM_LPAE_PTE_SH_IS		(((u64)3) << 8)
#define ARM_LPAE_PTE_NS			(((u64)1) << 5)
#define ARM_LPAE_PTE_VALID		(((u64)1) << 0)


#define ARM_LPAE_MAIR_ATTR_SHIFT(n)			((n) << 3)
#define ARM_LPAE_MAIR_ATTR_MASK				0xff
#define ARM_LPAE_MAIR_ATTR_DEVICE			0x04
#define ARM_LPAE_MAIR_ATTR_NC				0x44
#define ARM_LPAE_MAIR_ATTR_INC_OWBRWA			0xf4
#define ARM_LPAE_MAIR_ATTR_WBRWA			0xff
/* Inner/Outer Write-Back Non-Read/Write-Allocate */
#define ARM_LPAE_MAIR_ATTR_IWBNRANWA_OWBNRANWA		0xcc
#define ARM_LPAE_MAIR_ATTR_IDX_NC			0
#define ARM_LPAE_MAIR_ATTR_IDX_CACHE			1
#define ARM_LPAE_MAIR_ATTR_IDX_DEV			2
#define ARM_LPAE_MAIR_ATTR_IDX_INC_OCACHE		3
/* Inner/Outer CACHE Non-Read/Write-Allocate */
#define ARM_LPAE_MAIR_ATTR_IDX_ICACHE_OCACHE_NRANWA	6

/*
 * Context descriptors.
 *
 * Linear: when less than 64 SSIDs are supported
 * 2lvl: at most 64 L1 entries,
 *       64 lazy entries per table.
 */
#define CTXDESC_SURPPOT_SPLIT		6
#define CTXDESC_CD_MAX_SSIDS		(1 << 6)

#define ARM_SMMU_ID_SIZE_8		8

#define SMMU_SSID_BITS  		0xCU
#define SMMU_MAX_STREAM_ID      64U
#define DOMAIN_ATTR_NESTING_NOT_SUPPORTED 0

#define MSI_IOVA_BASE			0x8000000
#define MSI_IOVA_LENGTH			0x100000

#define IOVA_DEFAULT_ADDS		0x100000
#define IOVA_DEFAULT_SIZE		0xc0000000
#define INVAL_TTWC_ASID			0xFF

#define LAZY_FREE_WATERLINE		10
#define WATCHDOG_CPU			0
#define WIFI_INTX_CPU			4
#define LAZY_FREE_SCHED_PRI		97

#define PAGESIZE_BITMAP		(SZ_4K | SZ_2M | SZ_1G)

#define PAGE_PRIV_OSH_SHIFT 6

enum tlb_flush_ops {
	ASID_FLUSH, /* arm smmu driver flush tlb by asid */
	SSID_FLUSH, /* arm smmu driver flush tlb by ssid */
	DEV_FLUSH,  /* master dev driver flush tlb by ssid */
};

/* SMMU private data for an IOMMU domain */
enum arm_smmu_domain_stage {
	ARM_SMMU_DOMAIN_S1 = 0,
	ARM_SMMU_DOMAIN_S2,
	ARM_SMMU_DOMAIN_NESTED,
	ARM_SMMU_DOMAIN_BYPASS,
};

enum arm_smmu_domain_type {
	ARM_SMMU_DOMAIN_DMA,
	ARM_SMMU_DOMAIN_SVM,
};

struct arm_smmu_stream {
	u32				id;
	struct arm_smmu_master		*master;
	struct rb_node			node;
};

/* SMMU private data for each master */
struct arm_smmu_master {
	struct arm_smmu_device		*smmu;
	struct arm_smmu_domain		*smmu_domain;
	struct iommu_domain_data	iova;
	struct device			*dev;
	struct arm_smmu_stream		*streams;
	bool				stall_enabled;
	u32				*sids;
	u32             num_sids;
	u32				ssid_bits;
	u32				ssid_nums;
	u32				asid_base;
	u8				tlb_flush;
};

struct arm_smmu_domain {
	u32				sid;
	struct arm_smmu_device		*smmu;
	struct mutex			init_mutex; /* Protects smmu pointer */
	bool				stall_enabled;
	struct iommu_domain		domain;
	u16				asid;
	u32				type; /* domain type:dma or svm */
	u8				tlb_flush;
	u32				ssid_nums;
	struct device			*dev; /* svm domain use */
	u64				svm_ttbr; /* svm domain use */
	u32				svm_ssid; /* svm domain use */
	bool				ssid_assign; /* svm domain use */
	DECLARE_BITMAP(ssid_map, CTXDESC_CD_MAX_SSIDS); /* svm domain use */
};

struct mm_smmu_domain {
	struct arm_smmu_domain smmu_domain;
	struct list_head list;
};

#define MAX_WHITELIST_NUM		(13U)

struct whitelist_entry {
	u32 addr;
	u32 end;
};

struct arm_smmu_device {
	struct device			*dev;
	/* IOMMU core code handle */
	struct iommu_device		iommu;
	int                             smmuid;
	struct rb_root			streams;
	struct mutex			streams_mutex;
	struct list_head		smmu_list;
#define ARM_SMMU_MAX_ASIDS		(1 << 16)
	DECLARE_BITMAP(asid_map, ARM_SMMU_MAX_ASIDS);
	struct mutex asid_mutex;
	spinlock_t			iommu_groups_lock;
	struct list_head		iommu_groups;
	struct notifier_block		tcu_pm_nb;
	u32  whitelist_num;
	struct whitelist_entry whitelist_range[MAX_WHITELIST_NUM];
};

struct arm_smmu_iommu_group {
	u32				sid;
	u32				ssid;
	struct list_head			list;
	struct iommu_group		*group;
	struct blocking_notifier_head	notifier;
	struct device   *dev;
};

struct smmu_map_paras {
	u64 paddr;
	u64 iova;
	u64 prot;
	u32 sid;
	u64 size;
};

struct smmu_unmap_paras {
	u64 iova;
	u32 sid;
	u64 size;
	u64 unmap_size;
};

#define MAX_SMMU_MAP_PAGE_BATCH_NUMS     128

struct smmu_map_sg_paras {
	u64 phys[MAX_SMMU_MAP_PAGE_BATCH_NUMS];
	u64 iova;
	u32 length[MAX_SMMU_MAP_PAGE_BATCH_NUMS];
	u64 prot[MAX_SMMU_MAP_PAGE_BATCH_NUMS];
	u32 count;
	u16 sid;
	u16 map_page_batch;
	u64 tlb_iova;
	u64 tlb_size;
};

struct smmu_unmap_inv_cache_paras {
	u64 inv_iova;
	u32 length[MAX_SMMU_MAP_PAGE_BATCH_NUMS];
	u64 unmap_iova;
	u64 size;
	u32 sid;
	u16 count;
	u16 is_unmap;
};

struct smmu_map_batch_paras {
	u64 phys[MAX_SMMU_MAP_PAGE_BATCH_NUMS];
	u64 iova[MAX_SMMU_MAP_PAGE_BATCH_NUMS];
	u32 length[MAX_SMMU_MAP_PAGE_BATCH_NUMS];
	u64 prot;
	u32 count;
	u32 sid;
	bool is_coherent;
	bool is_dcache_flush;
};
 
struct smmu_unmap_batch_paras {
	u64 iova[MAX_SMMU_MAP_PAGE_BATCH_NUMS];
	u32 length[MAX_SMMU_MAP_PAGE_BATCH_NUMS];
	u32 count;
	u32 sid;
	bool is_coherent;
	bool is_dma_2_dev;
};

struct smmu_iova_paras {
	u64 iova;
	u64 paddr;
    u32 sid;
};

struct smmu_attach_paras {
    u32 smmuid;
    u32 sid;
    u32 ssid;
    u32 tlb_flush;
    u32 domain_type;
    bool is_svm;
    u32 svm_ssid;
    u64 svm_ttbr;
    u32 ssid_nums;
    u32 asid;
};

struct smmu_detach_paras {
    u32 sid;
    u32 svm_ssid;
};

struct smmu_remove_paras {
    u32 smmuid;
};

struct domain_free_paras {
    u32 sid;
};

struct smmu_tlb_paras {
    u32 sid;
    bool is_svm;
    u32 ssid;
};

struct smmu_ttwc_paras {
    u32 sid;
};

struct smmu_tcu_power_on {
	u32 smmuid;
};

struct smmu_tcu_power_off {
	u32 smmuid;
};

struct smmu_ttbr_paras {
    u32 sid;
    u64 ttbr;
};

struct smmu_tcu_info_paras {
    u32 smmuid;
	u64 strtab_base;
	u64 strtab_base_cfg;
	u64 cmdq_q_qbase;
	u64 evtq_q_qbase;
};

#define PMCG_EVENT_MAX_CNT_NUM		16
 
enum smmu_pmu_tcu_cmd {
	SMMU_PMU_TCU_SEC_SID_INIT = 0,
	SMMU_PMU_TCU_EVT_CNT_SET,
	SMMU_PMU_TCU_ENABLE,
	SMMU_PMU_TCU_READ_PMCG_SERV,
};

struct smmu_pmu_tcu_config_paras {
	u32 cmd;
	u32 smmuid;
	u32 sid;
	u32 evt_type_size;
	u32 pmcg_srv_size;
	u8 evt_type[PMCG_EVENT_MAX_CNT_NUM];
	u32 pmcg_serv[PMCG_EVENT_MAX_CNT_NUM];
};

#define debug_print(...) ((void)0)

/* svm_private Interface */
struct arm_smmu_domain *to_smmu_domain(struct iommu_domain *dom);
int arm_smmu_poweron(struct device *dev);
int arm_smmu_poweroff(struct device *dev);
int arm_smmu_svm_get_ssid(struct iommu_domain *domain, u16 *ssid, u64 *ttbr, u64 *tcr);
void arm_smmu_svm_ssid_asign(struct iommu_domain *domain, u16 ssid);
void arm_smmu_svm_tlb_inv_context(struct mm_svm *svm);
int arm_smmu_get_reg_info(struct device *dev, char *buffer, int length);
int arm_smmu_evt_register_notify(struct device *dev, struct notifier_block *n);
int arm_smmu_evt_unregister_notify(struct device *dev, struct notifier_block *n);
#ifdef CONFIG_SMMU_SUPPORT_AOD
bool get_smmu_tcu_init_info(enum smmu_type_id smmuid, struct smmu_init_info *info);
#endif /* CONFIG_SMMU_SUPPORT_AOD */
#ifdef CONFIG_LDK_SVM
extern int hm_mem_dump_pte_with_pgd(pid_t pid, uint64_t pgd, uintptr_t va);
#endif
void hm_mem_dump_pte(u64 ttbr, u64 va);

#ifdef CONFIG_SMMU_PMU_TCU
int pmu_tcu_sec_sid_init(int smmuid, u32 sid);
int pmu_tcu_evt_cnt_set(int smmuid, u8 *evt_type, u32 evt_type_size);
int pmu_tcu_enable(int smmuid);
int pmu_tcu_read_pmcg_serv(int smmuid, u32 *pmcg_serv, u32 pmcg_srv_size);
#endif /* CONFIG_SMMU_PMU_TCU */
#endif