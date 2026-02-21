/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Irq intr for hongmeng irq part
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 26 10:32:19 2019
 */
#ifndef ULIBS_LIBHMIRQ_INTR_H
#define ULIBS_LIBHMIRQ_INTR_H

#include <libhmactv/actv.h>
#include <libbunch/bunch.h>
#include <hmkernel/capability.h>

#define IRQ_ACTV_NAME			"irq-actv"
#define IRQ_ACTV_STACK_ORDER		2UL 	/* 16K */
#define IRQ_ACTV_STACK_SIZE		(1UL << (IRQ_ACTV_STACK_ORDER + PAGE_SHIFT))
#define IRQ_ACTV_TSD_SIZE		128U
#define IRQ_ACTV_POOLID			7U

struct hm_irq_version {
	uint16_t major;
	uint16_t minor;
	uint16_t patch;
	uint16_t build;
};

/* type of irqctrl */
enum hm_irq_ctrl_type {
	HM_IRQ_CTRL_TYPE_PHYSICAL	= 0,
	HM_IRQ_CTRL_TYPE_SMC	= 1,
	HM_NR_IRQ_CTRL_TYPE,
};

struct hm_irq_request_config {
	uint32_t *conf;		/* config of this irq */
	uint32_t conf_len;	/* config length */
	uint32_t flags;
	const char *name;
	cref_t tcb_cref;
};

void hm_irq_version_get(struct hm_irq_version *version);
int hm_irq_version_check(const struct hm_irq_version *expected);
int hm_irq_default_ctrl_id(enum hm_irq_ctrl_type type);
int hm_irq_request_current_cpu(int ctrl_id,
			       struct hm_irq_request_config *config,
			       rref_t *irq_rref);
int hm_irq_free(rref_t irq_rref);
int hm_irq_enable(rref_t irq_rref, uint32_t irq);
int hm_irq_disable(rref_t irq_rref, uint32_t irq);

/* if flag equal IRQCTRL_SET_WAKE, the irq can wakeup system from suspend */
int hm_irq_set_wake(rref_t irq_rref, uint32_t irq, uint32_t flag);
int hm_irq_stat(const char *name, struct bunch *bunch);
int hm_irq_wait_for_irq(void);

struct irq_fastpath_res_s {
	cref_t irq_actv_pool;
};

int hm_irq_create_fastpath(struct actvpool_configs *irq_ap_cfg,
			   struct irq_fastpath_res_s *irq_fp_res,
			   int (*irq_fastpath_init)(void));

void hm_irq_destroy_fastpath(struct actvpool_configs *irq_ap_cfg,
			     struct irq_fastpath_res_s *irq_fp_res);
#endif
