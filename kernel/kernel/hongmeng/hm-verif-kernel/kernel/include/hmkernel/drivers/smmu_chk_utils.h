/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: The interface of util for smmu
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 22 21:40:55 2024
 */

#ifndef HMKERNEL_DRIVER_SMMU_CHK_UTILS_H
#define HMKERNEL_DRIVER_SMMU_CHK_UTILS_H

#ifdef CONFIG_SMMU_ALLOC
bool smmu_pgstr_from_pa_acquire(paddr_t pa);
bool smmu_pgstr_release(paddr_t pa);
bool smmu_pa_type_is_pt(paddr_t pte_pa);
bool smmu_pa_type_is_pgd(paddr_t pte_pa);
bool smmu_pa_type_is_iomem(paddr_t pg_pa);
bool smmu_pa_type_is_iomem_acquire(paddr_t pg_pa);
void smmu_pa_type_is_iomem_release(paddr_t pg_pa);
bool smmu_pa_type_is_iomem_2m_acquire(paddr_t pg_pa);
void smmu_pa_type_is_iomem_2m_release(paddr_t pg_pa);
#else
static inline bool smmu_pgstr_from_pa_acquire(paddr_t pa)
{
	UNUSED(pa);
	return true;
}

static inline bool smmu_pgstr_release(paddr_t pa)
{
	UNUSED(pa);
	return true;
}

static inline bool smmu_pa_type_is_pt(paddr_t pte_pa)
{
	UNUSED(pte_pa);
	return true;
}

static inline bool smmu_pa_type_is_pgd(paddr_t pte_pa)
{
	UNUSED(pte_pa);
	return true;
}

static inline bool smmu_pa_type_is_iomem(paddr_t pg_pa)
{
	UNUSED(pg_pa);
	return true;
}

static inline bool smmu_pa_type_is_iomem_acquire(paddr_t pg_pa)
{
	UNUSED(pg_pa);
	return true;
}

static inline void smmu_pa_type_is_iomem_release(paddr_t pg_pa)
{
	UNUSED(pg_pa);
}

static inline bool smmu_pa_type_is_iomem_2m_acquire(paddr_t pg_pa)
{
	UNUSED(pg_pa);
	return true;
}

static inline void smmu_pa_type_is_iomem_2m_release(paddr_t pg_pa)
{
	UNUSED(pg_pa);
}
#endif
#endif
