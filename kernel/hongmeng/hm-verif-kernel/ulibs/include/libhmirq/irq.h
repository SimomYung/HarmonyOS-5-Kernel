/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Irq lib for hongmeng irq part
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 24 10:32:19 2019
 */
#ifndef ULIBS_LIBHMIRQ_IRQ_H
#define ULIBS_LIBHMIRQ_IRQ_H

#include <stdint.h>
#include <hmkernel/cpuset.h>
#include <hmkernel/capability.h>

#define IRQ_TYPE_SHIFT			(1U)
#define IRQ_ATTR_LEVEL_TYPE		0 /* 0 << IRQ_TYPE_SHIFT */
#define IRQ_ATTR_EDGE_TYPE		(1U << IRQ_TYPE_SHIFT)

#define IRQ_SGI_NR		16U

#define HW_CONF_ITS_LENGTH		4U
#define HW_CONF_LENGTH			3U
#define HW_CONF_KVIC_LENGTH		1U

#define HW_CONF_IRQ_ID_IDX		0U
#define HW_CONF_IRQ_CONF_IDX		1U
#define HW_CONF_IRQ_TARGET_IDX		2U

#define HW_CONF_DEFAULT_PRIO		0xE0
#define HW_CONF_TARGET_CPU		0xFFFFFFFF

#define IRQ_PERCPU_NR			32U
#define IRQ_MAX_CPU_NR			32U

struct hm_irq_register_setting {
	const char *name;		/* max strlen is 31 */
	uint32_t flags;			/* 0x01 wakeup */
	/* cpus that irq will be registered to */
	__cpuset_t cpu_mask;
	cref_t irq_fastpath_ap;
	/* is fastpath bypass current process */
	unsigned int is_fp_bypass : 1;
};

struct hm_irq_attribute {
	uint32_t attr_cpu;		/* CPU bit map for irq affinirt */
	uint32_t attr_type;		/* 0b1x: edge 0b0x: level */
	uint32_t attr_affi_hint; /* CPU bit map for irq affinity hint */
};

int hm_irq_init_idx(void);
int hm_irq_conf_to_irq(int ctrl_id, const uint32_t *conf,
		       uint32_t conf_len, uint32_t cpu_id);
rref_t hm_irq_require_grant(int ctrl_id, uint32_t irq);
int hm_irq_reject_grant(rref_t irq_rref);
int hm_irq_config(rref_t irq_rref, uint32_t irq,
		  uint32_t *conf, uint32_t conf_len);
int hm_irq_register(rref_t irq_rref, uint32_t irq,
		    const struct hm_irq_register_setting *setting,
		    cref_t tcb_cref);
int hm_irq_unregister(rref_t irq_rref, uint32_t irq);
int hm_irq_generate_sgi(rref_t irq_rref, uint32_t irq,
			__cpuset_t target_set,
			void *arg, uint32_t arg_len);
int hm_irq_ioctl(rref_t irq_rref, uint32_t irq,
		 uint32_t cmd, const void *arg, uint32_t arg_len);
int hm_irq_set_attribute(rref_t irq_rref, uint32_t logic_irq,
			 struct hm_irq_attribute *hm_irq_attr,
			 unsigned int flag);

#endif
