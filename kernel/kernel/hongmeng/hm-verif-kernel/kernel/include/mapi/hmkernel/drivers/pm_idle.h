/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Header of pm idle management
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 10 21:05:23 2021
 */

#ifndef MAPI_HMKERNEL_DRIVERS_PM_IDLE_H
#define MAPI_HMKERNEL_DRIVERS_PM_IDLE_H

#include <hmkernel/errno.h>
#include <hmkernel/drivers/idle_state.h>

/*
 * if these irqs intercept idle, the idle_sys_stat's is_intercept will return false.
 */
ENUM(3, idle_nointercept_ppi_type, IDLE_NOINTERCEPT_PPI_TYPE_,
	TIMER,
	IPI_CALL_FUNC,
)

ENUM(2, idle_nointercept_spi_type, IDLE_NOINTERCEPT_SPI_TYPE_,
	BROADCAST_TIMER,
)

#ifdef CONFIG_DEEP_IDLE
#define IDLE_LE_LATENCY_FACTOR		1U
/*
 * Set IDLE_GE_LATENCY_FACTOR to 2 for making it easier to
 * allow higher cluster to run task.
 */
#define IDLE_GE_LATENCY_FACTOR		2U

bool all_cpu_in_idle(void);
unsigned int idle_state_nr_of(unsigned int cpu);
unsigned int idle_latency_of_cpu(unsigned int cpu_id);
int pm_idle_register_state(const struct idle_state *state);
bool all_cpu_in_deep_idle(void);
void idle_nointercept_ppi_register(unsigned int cpu,
				   enum idle_nointercept_ppi_type type,
				   unsigned int ppi);
void idle_nointercept_spi_register(enum idle_nointercept_spi_type type,
				   unsigned int spi);
cpuset_t deep_idle_cpuset_read(void);
#else

static inline unsigned int idle_state_nr_of(unsigned int cpu)
{
	UNUSED(cpu);
	return 0;
}

static inline int pm_idle_register_state(const struct idle_state *state)
{
	UNUSED(state);
	return E_HM_NOSYS;
}

static inline bool all_cpu_in_deep_idle(void)
{
	return false;
}

static inline void idle_nointercept_ppi_register(unsigned int cpu,
						 enum idle_nointercept_ppi_type type,
						 unsigned int ppi)
{
	UNUSED(cpu, type, ppi);
}

static inline void idle_nointercept_spi_register(enum idle_nointercept_spi_type type,
						 unsigned int spi)
{
	UNUSED(type, spi);
}

static inline cpuset_t deep_idle_cpuset_read(void)
{
	return cpuset_empty;
}

#endif

#endif
