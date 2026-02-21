/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: This file is the interface file of other modules.
 * Author: linlixin2@huawei.com
 * Create: 2020-10-19
 */

#ifndef RSMC_START_X801_H
#define RSMC_START_X801_H

// GENI platform X801 driver
#ifdef CONFIG_SPI_MSM_GENI_MODULE
int rsmc_start_x801(void);
void rsmc_stop_x801(void);
#else
// Non GENI platform x801 driver
#ifdef CONFIG_HW_RSMC_X801
int rsmc_start_x801(void);
void rsmc_stop_x801(void);
// No X801 platform implement by virtual function
#else
static inline int rsmc_start_x801(void)
{
	return 0;
}
static inline void rsmc_stop_x801(void)
{
	return;
}
#endif
#endif

#endif /* RSMC_START_X801_H */

