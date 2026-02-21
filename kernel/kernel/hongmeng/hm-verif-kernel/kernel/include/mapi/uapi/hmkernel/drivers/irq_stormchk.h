/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2022. All rights reserved.
 * Description: Definition of irq storm check driver
 * Author: Huawei OS Kernel Lab
 * Create: Sat Dec 25 17:31:10 2021
 */
#ifndef UAPI_HMKERNEL_DRIVERS_IRQ_STORMCHK_H
#define UAPI_HMKERNEL_DRIVERS_IRQ_STORMCHK_H

#include <hmkernel/types.h>

#define IRQ_STORMCHK_REGION_NAME	"irq-stormchk"
/*
 *  magic value equals to hex ascii code of "IRQSTORM".
 */
#define IRQ_STORMCHK_MAGIC		0x49525153544f524dULL
#define IRQ_NAME_LEN			32U
#define MAX_CPU_NR			32U

struct irq_stormchk_desc_s {
	__u64 magic_num;
	__u32 index;
	__u64 timestamp;
};

struct irq_info_s {
	char name[IRQ_NAME_LEN];
	__u32 logic_irq;
	__u32 hw_irq;
	__u32 affinity;
	__u32 trigger_type;
	__u32 active_cnt[MAX_CPU_NR];
	__u64 active_total_cnt;
};

#endif
