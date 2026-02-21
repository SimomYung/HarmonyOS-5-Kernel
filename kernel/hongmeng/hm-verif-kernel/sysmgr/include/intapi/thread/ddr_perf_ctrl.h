/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: ddr_perf_ctrl.h
 * Author: Huawei
 * Create: Fri Jan 4 21:49:13 2024
 */

#ifndef DDR_PERF_CTRL_H
#define DDR_PERF_CTRL_H

#if defined(CONFIG_DDR_CRATER)
#define PERF_CTRL_DDR_MAX_CH    8
#elif defined(CONFIG_DDR_PISCES)
#define PERF_CTRL_DDR_MAX_CH    2
#else
#define PERF_CTRL_DDR_MAX_CH    4
#endif

#define ATF_SUB_RESERVED_BL31_SHARE_MEM_PHYMEM_BASE  0x10941000
#define DDR_PERF_DATA0_ADDR_OFFSET       0x1d00
#define DDR_PERF_DATA1_ADDR_OFFSET       0x1000
#define DDR_PERF_VIRT_DATA_ADDR_OFFSET   0xd00
#define SYSMGR_SMC_RESULT_SIZE           4
#define DDR_PERF_DATA_SIZE               2

#define PERF_CTRL_DDR_FLUX_MAX_CNT UINT_MAX
#define ACCESS_REGISTER_FN_MAIN_ID	0xc500aa01UL
#define ACCESS_REGISTER_FN_SUB_ID_MASTER_SECURITY_CONFIG    0x55bbcce7UL
#define ACCESS_REGISTER_FN_SUB_ID_DDR_MODEM_SEC             0x55bbcce9UL
#define ACCESS_REGISTER_FN_SUB_ID_DDR_KERNEL_CODE_PROTECT   0x55bbccedUL
#define ACCESS_REGISTER_FN_SUB_ID_SECS_POWER_CTRL           0x55bbccf0UL
#define ACCESS_REGISTER_FN_SUB_ID_DDR_PERF_CTRL             0x55bbccf3UL

struct ddr_scene {
	unsigned long long app_scene;
};

int perf_ctrl_get_ddr_flux(void *uarg);
#ifdef CONFIG_DDR_SCENE
int perf_ctrl_set_ddr_scene(void *uarg);
#else
static inline int perf_ctrl_set_ddr_scene(void *uarg)
{
	return 0;
}
#endif

struct sysmgr_smc_result;
struct sysmgr_smc_result *sysmgr_smc_result_alloc(void);
void sysmgr_smc_result_destroy(struct sysmgr_smc_result *result);
int sysmgr_smc_result_read(const struct sysmgr_smc_result *result, unsigned int idx, unsigned long *out);
int sysmgr_smc_call(unsigned long func_id, unsigned long arg0, unsigned long arg1, unsigned long arg2,
	unsigned long arg3, unsigned long arg4, unsigned long arg5, unsigned long arg6,
	struct sysmgr_smc_result *result);
int ddr_perf_map_memory(struct vspace_s *vspace, uint64_t paddr, size_t size, uintptr_t *vaddr);
#endif
