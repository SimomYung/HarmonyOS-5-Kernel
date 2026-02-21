/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Hyperhold intapi
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 17 11:19:59 2023
 */
#ifndef SYSMGR_INTAPI_MEM_ZSWAPD_PRESSURE_H
#define SYSMGR_INTAPI_MEM_ZSWAPD_PRESSURE_H

enum zswapd_pressure_level {
	LEVEL_LOW = 0,
	LEVEL_MEDIUM,
	LEVEL_CRITICAL,
	LEVEL_COUNT
};

#ifdef CONFIG_ZSWAP_GROUP
int zswapd_pressure_register(int evt_fd, unsigned int level, unsigned int cnode_idx);
void zswapd_pressure_init(void);
void zswapd_pressure_report(enum zswapd_pressure_level level);
#else /* !CONFIG_ZSWAP_GROUP */
static inline int zswapd_pressure_register(int evt_fd, unsigned int level, unsigned int cnode_idx)
{
	UNUSED(evt_fd, level, cnode_idx);
	return E_HM_OK;
}
static inline void trigger_zswapd_pressure_event(void) {}
static inline void zswapd_pressure_init(void) {}
static inline void zswapd_pressure_report(enum zswapd_pressure_level level)
{
	UNUSED(level);
}
#endif /* CONFIG_ZSWAP_GROUP */
#endif /* SYSMGR_INTAPI_MEM_ZSWAPD_PRESSURE_H */
