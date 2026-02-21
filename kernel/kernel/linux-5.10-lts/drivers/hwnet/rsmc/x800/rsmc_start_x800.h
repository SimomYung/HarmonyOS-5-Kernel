/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: This file is the interface file of other modules.
 * Author: linlixin2@huawei.com
 * Create: 2020-10-19
 */

#ifndef RSMC_START_X800_H
#define RSMC_START_X800_H

// X800 driver coupling with GENI spi driver, so X800 driver ctrl by GENI
#ifdef CONFIG_SPI_MSM_GENI_MODULE
int rsmc_start_x800(void);
void rsmc_stop_x800(void);
#else
// Non GENI platform ctrl by independent macro switch
#ifdef CONFIG_HW_RSMC_X800
int rsmc_start_x800(void);
void rsmc_stop_x800(void);
// X800 driver platform implement by virtual function
#else
static inline int rsmc_start_x800(void)
{
	return 0;
}
static inline void rsmc_stop_x800(void)
{
	return;
}
#endif
#endif

#endif /* RSMC_START_X800_H */

