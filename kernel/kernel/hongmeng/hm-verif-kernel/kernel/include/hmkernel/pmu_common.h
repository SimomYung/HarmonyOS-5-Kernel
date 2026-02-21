/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Common layer for arm pmu
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 16 09:12:00 2019
 */
#ifndef KERNEL_DRIVERS_ARM_PMU_COMMON_H
#define KERNEL_DRIVERS_ARM_PMU_COMMON_H

#include <hmkernel/perfctx.h>
#include <hmkernel/devicetree.h>
#include <hmkernel/interrupt/irqctrl.h>
#include <hmkernel/interrupt/interrupt.h>
#include <hmkernel/drivers/pmu.h>
#ifdef CONFIG_KERNEL_PM_SUSPEND
#include <hmkernel/drivers/pm_suspend.h>
#endif

#define PMCR_LP(x)	(((x) & 1U) << 7) /* enable long event counter */
#define PMCR_LC(x)	(((x) & 1U) << 6) /* enable long cycle counter */
#define PMCR_DP(x)	(((x) & 1U) << 5) /* disable pmccntr counts when event counting is prohibited */
#define PMCR_X(x)	(((x) & 1U) << 4) /* export events where not prohibited */
#define PMCR_D(x)	(((x) & 1U) << 3) /* pmccntr counts once every 64 clock cycles */
#define PMCR_C(x)	(((x) & 1U) << 2) /* reset pmccntr to zero */
#define PMCR_P(x)	(((x) & 1U) << 1) /* reset all event counters */
#define PMCR_E(x)	(((x) & 1U))      /* enable all counters */

#define PMUSERENR_EN(x)		((x) & 1u)
#define PMCNTEN_C(x)		(((x) & 1u) << 31)

#define PMCR_N_extract(x)	(((x) >> 11) & 0x1fu)
#define PMCR_E_extract(x)	((x) & 1u)

#define PMCCFILTR_NSH(x) (((x) & 1U) << 27)
#define PMCCFILTR_U(x) (((x) & 1U) << 30)
#define PMCCFILTR_P(x) (((x) & 1U) << 31)

#define ARM_PMU_CYCLE_COUNTER_IDX 31u
#define ARM_DSU_MAX_SUPPORT_EVENTS 31u

/* mask for counter writable bits */
#define ARM_PMU_32_BIT_COUNTER_MASK 0xFFFFFFFFULL
#define ARM_PMU_64_BIT_COUNTER_MASK 0xFFFFFFFFFFFFFFFFULL
#define ARM_PMU_OVERFLOWED_MASK     0xFFFFFFFFU

#define INVALID_CNODE_INDEX		0xFFFFFFFFU
#define INVALID_COUNTER_NUM		0xFFFFFFFFU

#define ARM_PMU_V3P1           0b0100
#define ARM_PMU_V3P4           0b0101
#define ARM_PMU_V3P5           0b0110
#define ARM_PMU_V3P7           0b0111
#define ARM_PMU_V3P8           0b1000

#define IRQ_KEV_TRACE_SLOW_BEGIN	0U
#define IRQ_KEV_TRACE_END		10U
#define IRQ_TRACE_END_SUCCESS		1

ENUM(3, pmu_reg_state_e, PMU_REG_STATE_,
	FREE,
	CPU_USED,
	TCB_USED);

struct pmu_state_s {
	unsigned int reg_taken_by[__PERF_CONFIGURABLE_EVENT_COUNTER_NUM];
	unsigned int reg_refcnt[__PERF_CONFIGURABLE_EVENT_COUNTER_NUM];

	/*
	 * This is the logic counter idx for physical counter of owned cnode.
	 * We assume one cnode only have one __perf_event_config_s instance.
	 *
	 * This field will be deleted after comming refactoring.
	 */
	int logic_idx[__PERF_CONFIGURABLE_EVENT_COUNTER_NUM];

	unsigned int reg_refcnt_percpu[CONFIG_NR_CPUS][__PERF_CONFIGURABLE_EVENT_COUNTER_NUM];
	enum pmu_reg_state_e reg_state[CONFIG_NR_CPUS][__PERF_CONFIGURABLE_EVENT_COUNTER_NUM];
};

struct dsu_pmu_state_s {
	unsigned int reg_taken_by[__PERF_CONFIGURABLE_EVENT_COUNTER_NUM];
	int evt_logic_ids[__PERF_CONFIGURABLE_EVENT_COUNTER_NUM];
};

#ifdef CONFIG_HMPERF_NG
static inline int
__logic_index_of(const struct perf_ctx_s *ctx, int cpu, unsigned int phy_idx)
{
	int logic_idx;

	if (ctx->hmperf_ng) {
		BUG_ON(cpu < 0 || cpu >= CONFIG_NR_CPUS);
		logic_idx = ctx->percpu_logic_idx[cpu][phy_idx];
	} else {
		logic_idx = ctx->logic_idx[phy_idx];
	}
	return logic_idx;
}
#else
static inline int
__logic_index_of(const struct perf_ctx_s *ctx, int cpu, unsigned int phy_idx)
{
	int logic_idx;
	UNUSED(cpu);
	logic_idx = ctx->logic_idx[phy_idx];
	return logic_idx;
}
#endif

static inline int
logic_index_of(const struct perf_ctx_s *ctx, int cpu, unsigned int phy_idx)
{
	int logic_idx = __logic_index_of(ctx, cpu, phy_idx);
	BUG_ON(logic_idx < 0);
	BUG_ON(logic_idx >= (int)ARRAY_SIZE(ctx->evts));
	return logic_idx;
}

struct pmu_irq_s {
	struct irqctrl_s *irqctrl;
	unsigned int interrupt_config[IRQ_HWCONF_CELLS];
};

/* Should be called by driver */
void arm_pmu_common_percpu_init(unsigned int cpuid);
void arm_pmu_common_enable_pmu(u32 enabled_bit, u32 disabled_bit);
void arm_pmu_common_disable_pmu(void);

/* Operate common PMU ctx */
void
arm_pmu_ctx_prepare_online(void);
void
arm_pmu_ctx_online(const struct perf_ctx_s *ctx);

void
arm_pmu_ctx_offline(struct perf_ctx_s *ctx);

unsigned int arm_pmu_taken_num(void);
unsigned int arm_pmu_common_ncounters_of(unsigned int cpuid);

/*
 * to reduce kernel complexity, new only raw perf events are supported,
 * after the heterogeneous architecture is introduced,
 * hardware perf event support can be added
 */
int arm_pmu_event_map(const struct perf_event_s *event, u32 *ret);
int arm_dsu_event_map(const struct __perf_event_attr_s *evt_attr, u32 *ret);

void arm_pmu_init_state(void);
u64 arm_pmu_counter_mask(unsigned int idx);
u64 arm_dsu_pmu_counter_mask(unsigned int idx);
int dsu_pmu_physical2logic_bit(unsigned int index);
bool arm_pmu_sample_enabled(void);

static inline bool is_sampled_dsu_event(const struct __perf_event_attr_s *evt_attr)
{
	return (evt_attr->period != 0ULL) ? true : false;
}

u32 arm_pmu_overflow_flag(const struct perf_ctx_s *ctx);

void arm_pmu_save_pmevtr(struct perf_ctx_s *ctx, unsigned int idx,
			 unsigned int logic_idx, u64 raw_count, u64 mask,
			 u32 flag);
void arm_dsu_pmu_save_pmevtr(struct dsu_pmu_common_ctx_s *ctx, unsigned int idx,
			     unsigned int logic_idx, u64 raw_count, u64 mask, u32 flag);

int arm_pmu_common_acquire(struct perf_ctx_s *ctx,
			   struct __perf_event_config_s *config,
			   unsigned int evts_num, bool cycle_enabled,
			   unsigned int cnode_idx);
void arm_pmu_common_attach(struct perf_ctx_s *ctx);
void arm_pmu_common_detach(struct perf_ctx_s *ctx);
void arm_pmu_common_release(struct perf_ctx_s *ctx);
void arm_pmu_update_evt_counter(struct perf_ctx_s *ctx,
				unsigned int idx);
void pmu_update_events(unsigned int cpuid, struct tcb_s *tcb,
		       unsigned int flags, bool *need_time);
void arm_pmu_stop_counters(void);
void arm_pmu_start_counters(void);
unsigned int enabled_physical_event_bits_of(const struct perf_ctx_s *ctx, unsigned int cpu);
u64 arm_pmu_read_evt_counter(unsigned int idx);
void arm_pmu_reg_taken_of(unsigned int cnode_index,
				unsigned int *reg_idx, unsigned int nums);
#ifdef CONFIG_HMPERF_NG
int arm_pmu_common_acquire_single(struct perf_ctx_s *ctx,
				  int logic_idx);
void arm_pmu_common_release_single(struct perf_ctx_s *ctx, int logic_idx);
void arm_pmu_stop(struct perf_event_s *event);
void arm_pmu_start(struct perf_event_s *event);
#endif
#ifdef CONFIG_ARM_PMU_OVERFLOW
/* PMU overflow interrupt config function in kernel */
void arm_pmu_irq_init(const struct dtb_node_s *node, unsigned int cpuid);
void arm_pmu_percpu_irq_init(unsigned int cpuid);
void pmu_cpu_hotplug_register_notif(void);
void arm_pmu_update_events_maybe_sample(struct perf_ctx_s *ctx,
					struct tcb_s *tcb, unsigned int i);
#else
static inline void arm_pmu_irq_init(const struct dtb_node_s *node,
				    unsigned int cpuid)
{
	UNUSED(node, cpuid);
}

static inline void arm_pmu_percpu_irq_init(unsigned int cpuid)
{
	UNUSED(cpuid);
}

static inline void pmu_cpu_hotplug_register_notif(void)
{
	/* Nothing special to do */
}
static inline void arm_pmu_update_events_maybe_sample(struct perf_ctx_s *ctx,
						      struct tcb_s *tcb,
						      unsigned int i)
{
	UNUSED(ctx, tcb, i);
}
#endif

#ifdef CONFIG_AARCH64_DSU
/* DSU PMU overflow interrupt config function in kernel */
void arm_dsu_pmu_irq_init(const struct dtb_node_s *node);
void arm_dsu_pmu_irq_register(unsigned int cpuid);
void arm_dsu_pmu_set_affinity(cpuset_t cpumask);
#endif

#ifdef CONFIG_KERNEL_PM_SUSPEND
void pmu_pm_ctxs_init(struct perf_ctx_s *pmctxs, unsigned int size);
void suspend_save_evt_configs(struct perf_ctx_s *ctx, unsigned int cpuid);
void resume_restore_evt_config(struct perf_ctx_s *ctx, unsigned int cpuid);
int pmu_pm_suspend(struct pm_dev_s *pm_dev);
void pmu_pm_resume(struct pm_dev_s *pm_dev);
#endif

#ifdef __HOST_LLT__
void pmu_sample_guest(const struct hyp_vcpu_s *vcpu);
void pmu_sample_kernel(void);
unsigned long pmu_handler_irq(struct irqctrl_s *ctrl, unsigned int logic_irq,
			      union irqconf_data *data, struct tcb_s *preempted_tcb);
void pmu_finish_irq(struct irqctrl_s *ctrl, unsigned int logic_irq,
		    union irqconf_data *data, struct tcb_s *preempted_tcb);
#ifdef CONFIG_AARCH64_PMUV3
int llt_arm_pmuv3_init(void);
#endif

#ifdef CONFIG_ARM_PMUV1
int llt_arm_pmuv1_init(void);
#endif

#ifdef CONFIG_AARCH64_DSU
int llt_arm_dsu_pmu_init(void);
unsigned long arm_dsu_pmu_handler_irq(struct irqctrl_s *ctrl, unsigned int logic_irq,
				      union irqconf_data *data, struct tcb_s *preempted_tcb);
#endif

#ifdef CONFIG_KERNEL_PM_SUSPEND
#ifdef CONFIG_AARCH64_DSU
int dsu_pmu_pm_suspend(struct pm_dev_s *pm_dev);
void dsu_pmu_pm_resume(struct pm_dev_s *pm_dev);
#endif
#endif
#endif
#endif
