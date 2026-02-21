/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: smmu syscall
 * Create: 2024/12/12
 */

#include <hmkernel/types.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <libsysif/utils.h>
#include <libstrict/strict.h>
#include <libsysif/devmgr/api.h>
#include <libhmucap/ucap.h>
#include <libhmsrv_sys/hm_module.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libhwsecurec/securec.h>
#include <devhost/log.h>
#include <hmkernel/smmu_syscall.h>

int liblinux_smmu_tcu_power_on(void *para)
{
	struct smmu_tcu_power_on_paras *input = (struct smmu_tcu_power_on *)para;

	return sysfast_smmu_syscall(__SMMU_TCU_POWER_ON,
		input, sizeof(*input), input, sizeof(*input));
}
 
int liblinux_smmu_tcu_power_off(void *para)
{
	struct smmu_tcu_power_off_paras *input = (struct smmu_tcu_power_off *)para;

	return sysfast_smmu_syscall(__SMMU_TCU_POWER_OFF,
		input, sizeof(*input), input, sizeof(*input));
}

int liblinux_smmu_get_ttbr(void *para)
{
	struct smmu_ttbr_paras *input = (struct smmu_ttbr_paras *)para;

	return sysfast_smmu_syscall(__SMMU_GET_TTBR,
		input, sizeof(*input), input, sizeof(*input));
}

int liblinux_smmu_map(void *para)
{
	struct smmu_map_paras *input = (struct smmu_map_paras *)para;

	return sysfast_smmu_syscall(__SMMU_MAP,
		input, sizeof(*input), input, sizeof(*input));
}

int liblinux_smmu_unmap(void *para)
{
	struct smmu_unmap_paras *input = (struct smmu_unmap_paras *)para;

	return sysfast_smmu_syscall(__SMMU_UNMAP,
		input, sizeof(*input), input, sizeof(*input));
}

int liblinux_smmu_map_sg(void *para)
{
	struct smmu_map_sg_paras *input = (struct smmu_map_sg_paras *)para;

	return sysfast_smmu_syscall(__SMMU_MAP_SG,
		input, sizeof(*input), NULL, 0);
}

int liblinux_smmu_iova_to_phys(void *para)
{
	struct smmu_iova_paras *input = (struct smmu_iova_paras *)para;

	return sysfast_smmu_syscall(__SMMU_IOVA_TO_PHYS,
		input, sizeof(*input), input, sizeof(*input));
}

int liblinux_smmu_attach_device(void *para)
{
	struct smmu_attach_paras *input = (struct smmu_attach_paras *)para;

	return sysfast_smmu_syscall(__SMMU_ATTACH_DEV,
		input, sizeof(*input), input, sizeof(*input));
}

int liblinux_smmu_domain_attach_new_dev(void *para)
{
	struct smmu_attach_paras *input = (struct smmu_attach_paras *)para;

	return sysfast_smmu_syscall(__SMMU_ATTACH_NEW_DEV,
		input, sizeof(*input), input, sizeof(*input));
}

int liblinux_smmu_svm_detach_device(void *para)
{
	struct smmu_detach_paras *input = (struct smmu_detach_paras *)para;

	return sysfast_smmu_syscall(__SMMU_SVM_DETACH_DEV,
		input, sizeof(*input), input, sizeof(*input));
}

int liblinux_smmu_device_remove(void *para)
{
	struct smmu_remove_paras *input = (struct smmu_remove_paras *)para;

	return sysfast_smmu_syscall(__SMMU_SMMU_DEVICE_REMOVE,
		input, sizeof(*input), input, sizeof(*input));
}

int liblinux_smmu_domain_free(void *para)
{
	struct domain_free_paras *input = (struct domain_free_paras *)para;

	return sysfast_smmu_syscall(__SMMU_SMMU_DOMAIN_FREE,
		input, sizeof(*input), input, sizeof(*input));
}

int liblinux_smmu_tlb_inv_context(void *para)
{
	struct smmu_tlb_paras *input = (struct smmu_tlb_paras *)para;

	return sysfast_smmu_syscall(__SMMU_TLB_INV_CONTEXT,
		input, sizeof(*input), input, sizeof(*input));
}

int liblinux_smmu_tlb_inv_context_dev(void *para)
{
	struct smmu_tlb_paras *input = (struct smmu_tlb_paras *)para;

	return sysfast_smmu_syscall(__SMMU_TLB_INV_CONTEXT_DEV,
		input, sizeof(*input), input, sizeof(*input));
}

int liblinux_smmu_ttwc_inv_context(void *para)
{
	struct smmu_ttwc_paras *input = (struct smmu_ttwc_paras *)para;

	return sysfast_smmu_syscall(__SMMU_FLUSH_TTWC,
		input, sizeof(*input), input, sizeof(*input));
}

int liblinux_smmu_map_page_batch(void *para)
{
	struct smmu_map_batch_paras *input = (struct smmu_map_batch_paras *)para;
 
	if (sysfast_smmu_syscall(__SMMU_MAP_BATCH, input, sizeof(*input),
		NULL, 0) != E_HM_OK) {
		return 0;
	}
	return (int)input->count;
}
 
int liblinux_smmu_unmap_page_batch(void *para)
{
	struct smmu_unmap_batch_paras *input = (struct smmu_unmap_batch_paras *)para;
	if(sysfast_smmu_syscall(__SMMU_UNMAP_BATCH, input, sizeof(*input),
		NULL, 0) != E_HM_OK) {
		return 0;
	}
	return (int)input->count;
}

int liblinux_smmu_tcu_info(void *para)
{
	struct smmu_tcu_info_paras *input = (struct smmu_tcu_info_paras *)para;

	return sysfast_smmu_syscall(__SMMU_GET_TCU_INFO,
		input, sizeof(*input), input, sizeof(*input));
}

int liblinux_smmu_pmu_tcu_config(void *para)
{
	struct smmu_pmu_tcu_config_paras *input = (struct smmu_pmu_tcu_config_paras *)para;

	return sysfast_smmu_syscall(__SMMU_PMU_TCU_CONFIG,
		input, sizeof(*input), input, sizeof(*input));
}