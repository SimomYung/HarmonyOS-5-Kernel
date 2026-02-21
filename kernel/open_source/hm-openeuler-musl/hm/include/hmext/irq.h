/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: header file of irq
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 24 17:25:32 2022
 */
#ifndef HM_INCLUDE_HMEXT_IRQ_H
#define HM_INCLUDE_HMEXT_IRQ_H

#include <stdlib.h>
#include <stdint.h>
#include <hmext/ext.h>

/* for ext module use */
#define HM_EXT_IRQ_CMD_ENABLE           0x101UL
#define HM_EXT_IRQ_CMD_DISABLE          0x102UL
#define HM_EXT_IRQ_CMD_AFFINITY_SET     0x103UL
#define HM_EXT_IRQ_CMD_AFFINITY_GET     0x104UL
#define HM_EXT_IRQ_CMD_TRIGGER_SET      0x105UL
#define HM_EXT_IRQ_CMD_TRIGGER_GET      0x106UL
#define HM_EXT_IRQ_CMD_SET_PENDING      0x107UL
#define HM_EXT_IRQ_CMD_CLEAR_PENDING    0x108UL
#define HM_EXT_IRQ_CMD_GET_PENDING      0x109UL
#define HM_EXT_IRQ_CMD_SGI              0x10AUL
#define HM_EXT_IRQ_CMD_EOI              0x10BUL
#define HM_EXT_IRQ_REGISTER		0x10CUL
#define HM_EXT_IRQ_CMD_SET_SERVER       0x10DUL

#define __TRIGGER_WAIT_DONE	1UL

static inline int __hm_ext_syscall(uint32_t user_cmd,
				 void *cfg, uint32_t cfg_len,
				 void *rlt, uint32_t rlt_len)
{
	int ret = 0;

	ret = hm_ext_syscall(user_cmd, cfg, cfg_len, rlt, rlt_len);
	if (ret < 0) {
		errno = -ret;
		ret = -1;
	}

	return ret;
}

static inline int __hm_ext_wait(unsigned long flags, const struct __hmsigmask *sigmask)
{
	int ret = 0;

	ret = hm_ext_wait(flags, sigmask);
	if (ret < 0) {
		errno = -ret;
		ret = -1;
	}

	return ret;
}

static inline int hm_ipi_trigger(unsigned int ipi, unsigned int cpumask)
{
	return ENOSYS;
}

static inline int hm_uirq_nton_source(unsigned int irq, unsigned int source_select)
{
	return ENOSYS;
}

static inline int hm_uirq_affinity_set(unsigned int irq, unsigned int cpumask)
{
	return __hm_ext_syscall(HM_EXT_CMD_IRQ_OP,
			      &cpumask, irq, NULL, HM_EXT_IRQ_CMD_AFFINITY_SET);
}

static inline int hm_uirq_affinity_get(unsigned int irq, unsigned int *cpumask)
{
	return __hm_ext_syscall(HM_EXT_CMD_IRQ_OP,
			      cpumask, irq, NULL, HM_EXT_IRQ_CMD_AFFINITY_GET);
}

static inline int hm_uirq_trigger_set(unsigned int irq, unsigned int trigger_type)
{
	return __hm_ext_syscall(HM_EXT_CMD_IRQ_OP,
			      &trigger_type, irq, NULL, HM_EXT_IRQ_CMD_TRIGGER_SET);
}

static inline int hm_uirq_trigger_get(unsigned int irq, unsigned int *trigger_type)
{
	return __hm_ext_syscall(HM_EXT_CMD_IRQ_OP,
			      trigger_type, irq, NULL, HM_EXT_IRQ_CMD_TRIGGER_GET);
}

static inline int hm_uirq_pending_clear(unsigned int irq)
{
	return ENOSYS;
}

static inline int hm_uirq_enable(unsigned int irq)
{
	return __hm_ext_syscall(HM_EXT_CMD_IRQ_OP,
			      NULL, irq, NULL, HM_EXT_IRQ_CMD_ENABLE);
}

static inline int hm_uirq_disable(unsigned int irq)
{
	return __hm_ext_syscall(HM_EXT_CMD_IRQ_OP,
			      NULL, irq, NULL, HM_EXT_IRQ_CMD_DISABLE);
}

static inline int hm_uirq_pending_get(unsigned int irq, unsigned int *pending)
{
	return ENOSYS;
}

static inline int hm_uirq_active_clear(unsigned int irq)
{
	return ENOSYS;
}

static inline int hm_uirq_active_get(unsigned int irq, unsigned int *active)
{
	return ENOSYS;
}

static inline int hm_uirq_pri_get(unsigned int irq, unsigned int *pri)
{
	return ENOSYS;
}

static inline int hm_uirq_pri_set(unsigned int irq, unsigned int pri)
{
	return ENOSYS;
}

static inline int hm_uirq_trigger(unsigned int irq, unsigned int cpumask)
{
	return ENOSYS;
}

static inline int hm_uirq_register(unsigned int irq, const uint64_t *server)
{
	return __hm_ext_syscall(HM_EXT_CMD_IRQ_OP,
			      server, irq, NULL, HM_EXT_IRQ_REGISTER);
}

static inline int hm_uirq_wait(void)
{
	return __hm_ext_wait(__TRIGGER_WAIT_DONE, NULL);
}

static inline int hm_uirq_set_server(unsigned int irq, const uint64_t *new_server, const uint64_t *old_server)
{
	return __hm_ext_syscall(HM_EXT_CMD_IRQ_OP,
			      new_server, irq, old_server, HM_EXT_IRQ_CMD_SET_SERVER);
}

static inline int hm_uirq_eoi(unsigned int irq)
{
	return ENOSYS;
}

static inline int hm_uirq_mbinfo_clr(unsigned int irq)
{
	return ENOSYS;
}
#endif
