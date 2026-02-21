/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Scheduler internal types and methods:
 */
#include <linux/sched.h>

#include <linux/sched/autogroup.h>
#include <linux/sched/clock.h>
#include <linux/sched/coredump.h>
#include <linux/sched/cpufreq.h>
#include <linux/sched/cputime.h>
#include <linux/sched/deadline.h>
#include <linux/sched/debug.h>
#include <linux/sched/hotplug.h>
#include <linux/sched/idle.h>
#include <linux/sched/init.h>
#include <linux/sched/isolation.h>
#include <linux/sched/jobctl.h>
#include <linux/sched/loadavg.h>
#include <linux/sched/mm.h>
#include <linux/sched/nohz.h>
#include <linux/sched/numa_balancing.h>
#include <linux/sched/prio.h>
#include <linux/sched/rt.h>
#include <linux/sched/signal.h>
#include <linux/sched/smt.h>
#ifdef CONFIG_DFX_STATS
#include <linux/sched/dfx_stat.h>
#endif
#include <linux/sched/stat.h>
#include <linux/sched/sysctl.h>
#include <linux/sched/task.h>
#include <linux/sched/task_stack.h>
#include <linux/sched/topology.h>
#include <linux/sched/user.h>
#include <linux/sched/wake_q.h>
#include <linux/sched/xacct.h>

#include <uapi/linux/sched/types.h>

#include <linux/binfmts.h>
#include <linux/blkdev.h>
#include <linux/compat.h>
#include <linux/context_tracking.h>
#include <linux/cpufreq.h>
#include <linux/cpuidle.h>
#include <linux/cpuset.h>
#include <linux/ctype.h>
#include <linux/debugfs.h>
#include <linux/delayacct.h>
#include <linux/energy_model.h>
#include <linux/init_task.h>
#include <linux/kprobes.h>
#include <linux/kthread.h>
#include <linux/membarrier.h>
#include <linux/migrate.h>
#include <linux/mmu_context.h>
#include <linux/nmi.h>
#include <linux/proc_fs.h>
#include <linux/prefetch.h>
#include <linux/profile.h>
#include <linux/psi.h>
#include <linux/rcupdate_wait.h>
#include <linux/security.h>
#include <linux/stop_machine.h>
#include <linux/suspend.h>
#include <linux/swait.h>
#include <linux/syscalls.h>
#include <linux/task_work.h>
#include <linux/tsacct_kern.h>

#include <asm/tlb.h>
#include <asm-generic/vmlinux.lds.h>

#ifdef CONFIG_PARAVIRT
# include <asm/paravirt.h>
#endif
#ifdef CONFIG_HISI_EAS_SCHED
#include <linux/reciprocal_div.h>
#endif
#ifdef CONFIG_RENDER_RT
#include <linux/render_rt.h>
#endif

#include "cpupri.h"
#include "cpudeadline.h"
#ifdef CONFIG_HISI_EAS_SCHED
#include "sched_cluster.h"
#endif
#ifdef CONFIG_SCHED_RUNNING_AVG
#include "sched_avg.h"
#endif
#include "rtg/rtg.h"
#ifdef CONFIG_ED_TASK
#include "ed_task.h"
#endif
#ifdef CONFIG_SCHED_RUNNING_TASK_ROTATION
#include "rotation.h"
#endif
#ifdef CONFIG_RT_CAS
#include "rt_cas.h"
#endif
#ifdef CONFIG_SCHED_VIP
#include "vip.h"
#endif
#ifdef CONFIG_SCHED_DYNAMIC_PRIO
#include "dynamic_prio.h"
#endif
#include "plat_sched.h"
#include <linux/version.h>
#ifdef CONFIG_MIPS_LOAD_TRACKING
#include <platform_include/cee/linux/mips.h>
#endif
#if defined(CONFIG_OPT_TIMER_ENERGY_EFFICIENCY) ||\
	defined(CONFIG_HRTIMER_SOFT_EXPIRE_SYNC)
#include "timer_energy_efficiency.h"
#endif
#ifdef CONFIG_SCHED_SMT
#include "smt/smt.h"
#endif
#ifdef CONFIG_SCHED_CORE
#include <platform_include/cee/linux/core_sched_extend.h>
#endif
#include "plat_sched.h"
#include "platform/walt_coloc_grp.h"

#include <trace/events/sched.h>
#include <securec.h>

#ifdef CONFIG_SCHED_DEBUG
# define SCHED_WARN_ON(x)	WARN_ONCE(x, #x)
#else
# define SCHED_WARN_ON(x)	({ (void)(x), 0; })
#endif

enum {
	TASK_DC,
	CPU_DC,
	TOP_TASK_DC,
	DISCOUNT_MAX_NR,
};
struct rq;
struct cpuidle_state;

#ifdef CONFIG_SCHED_TOP_TASK
#define DEFAULT_TOP_TASK_HIST_SIZE		RAVG_HIST_SIZE_MAX
#define DEFAULT_TOP_TASK_STATS_POLICY		WINDOW_STATS_RECENT
#define DEFAULT_TOP_TASK_STATS_EMPTY_WINDOW	false

#define NUM_TRACKED_WINDOWS	2
#define NUM_LOAD_INDICES	64
#define SCHED_LOAD_GRANULE (sched_ravg_window / NUM_LOAD_INDICES)
#define ZERO_LOAD_INDEX (NUM_LOAD_INDICES - 1)

unsigned long top_task_util(struct rq *rq);
void top_task_exit(struct task_struct *p, struct rq *rq);
void top_task_rq_init(struct rq *rq);

struct top_task_entry {
	u8 count;
	u8 preferidle_count;
};
#else /* !CONFIG_SCHED_TOP_TASK */
static inline unsigned long top_task_util(struct rq *rq) { return 0; }
static inline void top_task_exit(struct task_struct *p, struct rq *rq) {}
#endif /* CONFIG_SCHED_TOP_TASK */
#ifdef CONFIG_SCHED_PRED_LOAD
bool use_task_pred_load(int cpu, struct task_struct *p);
unsigned long predict_util_sum(struct rq *rq);
unsigned long task_pred_util(struct task_struct *p);
unsigned long max_pred_ls(struct rq *rq);
unsigned long cpu_util_pred_min(struct rq *rq);
extern unsigned int predl_jump_load;
extern unsigned int predl_jump_boost;
extern unsigned int predl_do_predict_ctrl;
extern unsigned int predl_window_size;
extern unsigned int predl_window_size_sync_with_frame;
extern unsigned int predl_enable;
#else
static inline bool use_task_pred_load(int cpu, struct task_struct *p) { return false; }
static inline unsigned long predict_util_sum(struct rq *rq) { return 0; }
static inline unsigned long task_pred_util(struct task_struct *p) { return 0; }
static inline unsigned long max_pred_ls(struct rq *rq) { return 0; }
static inline unsigned long cpu_util_pred_min(struct rq *rq) { return 0; }
#endif
static inline unsigned long predl_max_vip_util(struct rq *rq) { return 0; }
static inline bool predl_hwvip_boost(struct task_struct *p) { return false; }
static inline bool predl_vip_accelerated_task(struct task_struct *p) { return false; }
static inline void sync_predl_window_size_with_frame(void) {}
#ifdef CONFIG_MIGRATION_NOTIFY
#define DEFAULT_FREQ_INC_NOTIFY (200 * 1000)
#define DEFAULT_FREQ_DEC_NOTIFY (200 * 1000)
#endif

#ifdef CONFIG_SCHED_WALT
extern unsigned int sched_ravg_window;
extern unsigned int walt_cpu_util_freq_divisor;

struct walt_sched_stats {
	u64 cumulative_runnable_avg_scaled;
};

struct load_subtractions {
	u64 window_start;
	u64 subs;
	u64 new_subs;
};

#define NUM_TRACKED_WINDOWS 2
#ifdef CONFIG_MULTI_MARGIN
extern int num_clusters;
#endif

extern unsigned int sched_disable_window_stats;
#endif /* CONFIG_SCHED_WALT */
/* task_struct::on_rq states: */
#define TASK_ON_RQ_QUEUED	1
#define TASK_ON_RQ_MIGRATING	2
#ifdef CONFIG_SCHED_CORE
/* rq sched core flags */
#define SCF_CHECK_REVERSE	0x1
#define SCF_HANDLING_REVERSE	0x2
#define SCF_NORMAL_FORCEIDLE	0x4
#define SCF_SKIP_UPDATE_INFO	0x8
#endif

extern __read_mostly int scheduler_running;

extern unsigned long calc_load_update;
extern atomic_long_t calc_load_tasks;

extern void calc_global_load_tick(struct rq *this_rq);
extern long calc_load_fold_active(struct rq *this_rq, long adjust);

#ifdef CONFIG_SMP
extern void init_sched_groups_capacity(int cpu, struct sched_domain *sd);
#endif

#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL_OPT
extern void update_task_ravg(struct task_struct *p, struct rq *rq, int event,
				u64 wallclock, u64 irqtime);
extern int walt_cpu_overload_irqload(int cpu);
#endif

extern void call_trace_sched_update_nr_running(struct rq *rq, int count);
/*
 * Helpers for converting nanosecond timing to jiffy resolution
 */
#define NS_TO_JIFFIES(TIME)	((unsigned long)(TIME) / (NSEC_PER_SEC / HZ))

#ifdef CONFIG_SCHED_LATENCY_NICE
/*
 * Latency nice is meant to provide scheduler hints about the relative
 * latency requirements of a task with respect to other tasks.
 * Thus a task with latency_nice == 19 can be hinted as the task with no
 * latency requirements, in contrast to the task with latency_nice == -20
 * which should be given priority in terms of lower latency.
 */
#define MAX_LATENCY_NICE	19
#define MIN_LATENCY_NICE	-20

#define LATENCY_NICE_WIDTH	\
	(MAX_LATENCY_NICE - MIN_LATENCY_NICE + 1)

/*
 * Default tasks should be treated as a task with latency_nice = 0.
 */
#define DEFAULT_LATENCY_NICE	0
#define DEFAULT_LATENCY_PRIO	(DEFAULT_LATENCY_NICE + LATENCY_NICE_WIDTH/2)

/*
 * Convert user-nice values [ -20 ... 0 ... 19 ]
 * to static latency [ 0..39 ],
 * and back.
 */
#define NICE_TO_LATENCY(nice)	((nice) + DEFAULT_LATENCY_PRIO)
#define LATENCY_TO_NICE(prio)	((prio) - DEFAULT_LATENCY_PRIO)
#define NICE_LATENCY_SHIFT	(SCHED_FIXEDPOINT_SHIFT)
#define NICE_LATENCY_WEIGHT_MAX	(1L << NICE_LATENCY_SHIFT)
#endif /* CONFIG_SCHED_LATENCY_NICE */

/*
 * Increase resolution of nice-level calculations for 64-bit architectures.
 * The extra resolution improves shares distribution and load balancing of
 * low-weight task groups (eg. nice +19 on an autogroup), deeper taskgroup
 * hierarchies, especially on larger systems. This is not a user-visible change
 * and does not change the user-interface for setting shares/weights.
 *
 * We increase resolution only if we have enough bits to allow this increased
 * resolution (i.e. 64-bit). The costs for increasing resolution when 32-bit
 * are pretty high and the returns do not justify the increased costs.
 *
 * Really only required when CONFIG_FAIR_GROUP_SCHED=y is also set, but to
 * increase coverage and consistency always enable it on 64-bit platforms.
 */
#ifdef CONFIG_64BIT
# define NICE_0_LOAD_SHIFT	(SCHED_FIXEDPOINT_SHIFT + SCHED_FIXEDPOINT_SHIFT)
# define scale_load(w)		((w) << SCHED_FIXEDPOINT_SHIFT)
# define scale_load_down(w) \
({ \
	unsigned long __w = (w); \
	if (__w) \
		__w = max(2UL, __w >> SCHED_FIXEDPOINT_SHIFT); \
	__w; \
})
#else
# define NICE_0_LOAD_SHIFT	(SCHED_FIXEDPOINT_SHIFT)
# define scale_load(w)		(w)
# define scale_load_down(w)	(w)
#endif

/*
 * Task weight (visible to users) and its load (invisible to users) have
 * independent resolution, but they should be well calibrated. We use
 * scale_load() and scale_load_down(w) to convert between them. The
 * following must be true:
 *
 *  scale_load(sched_prio_to_weight[USER_PRIO(NICE_TO_PRIO(0))]) == NICE_0_LOAD
 *
 */
#define NICE_0_LOAD		(1L << NICE_0_LOAD_SHIFT)

/*
 * Single value that decides SCHED_DEADLINE internal math precision.
 * 10 -> just above 1us
 * 9  -> just above 0.5us
 */
#define DL_SCALE		10

/*
 * Single value that denotes runtime == period, ie unlimited time.
 */
#define RUNTIME_INF		((u64)~0ULL)

static inline int idle_policy(int policy)
{
	return policy == SCHED_IDLE;
}
static inline int fair_policy(int policy)
{
	return policy == SCHED_NORMAL || policy == SCHED_BATCH;
}

static inline int rt_policy(int policy)
{
	return policy == SCHED_FIFO || policy == SCHED_RR;
}

static inline int dl_policy(int policy)
{
	return policy == SCHED_DEADLINE;
}
static inline bool valid_policy(int policy)
{
	return idle_policy(policy) || fair_policy(policy) ||
		rt_policy(policy) || dl_policy(policy);
}

static inline int task_has_idle_policy(struct task_struct *p)
{
	return idle_policy(p->policy);
}

static inline int task_has_rt_policy(struct task_struct *p)
{
	return rt_policy(p->policy);
}

static inline int task_has_dl_policy(struct task_struct *p)
{
	return dl_policy(p->policy);
}

#define cap_scale(v, s) ((v)*(s) >> SCHED_CAPACITY_SHIFT)

static inline void update_avg(u64 *avg, u64 sample)
{
	s64 diff = sample - *avg;
	*avg += diff / 8;
}

/*
 * Shifting a value by an exponent greater *or equal* to the size of said value
 * is UB; cap at size-1.
 */
#define shr_bound(val, shift)							\
	(val >> min_t(typeof(shift), shift, BITS_PER_TYPE(typeof(val)) - 1))

/*
 * !! For sched_setattr_nocheck() (kernel) only !!
 *
 * This is actually gross. :(
 *
 * It is used to make schedutil kworker(s) higher priority than SCHED_DEADLINE
 * tasks, but still be able to sleep. We need this on platforms that cannot
 * atomically change clock frequency. Remove once fast switching will be
 * available on such platforms.
 *
 * SUGOV stands for SchedUtil GOVernor.
 */
#define SCHED_FLAG_SUGOV	0x10000000

#define SCHED_DL_FLAGS (SCHED_FLAG_RECLAIM | SCHED_FLAG_DL_OVERRUN | SCHED_FLAG_SUGOV)

static inline bool dl_entity_is_special(struct sched_dl_entity *dl_se)
{
#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL
	return unlikely(dl_se->flags & SCHED_FLAG_SUGOV);
#else
	return false;
#endif
}

/*
 * Tells if entity @a should preempt entity @b.
 */
static inline bool
dl_entity_preempt(struct sched_dl_entity *a, struct sched_dl_entity *b)
{
	return dl_entity_is_special(a) ||
	       dl_time_before(a->deadline, b->deadline);
}

/*
 * This is the priority-queue data structure of the RT scheduling class:
 */
struct rt_prio_array {
	DECLARE_BITMAP(bitmap, MAX_RT_PRIO+1); /* include 1 bit for delimiter */
	struct list_head queue[MAX_RT_PRIO];
};

struct rt_bandwidth {
	/* nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_lock;
	ktime_t			rt_period;
	u64			rt_runtime;
	struct hrtimer		rt_period_timer;
	unsigned int		rt_period_active;
#ifdef CONFIG_RT_SOFT_BANDWIDTH
	u64			rt_soft_runtime;
#endif
};

void __dl_clear_params(struct task_struct *p);

struct dl_bandwidth {
	raw_spinlock_t		dl_runtime_lock;
	u64			dl_runtime;
	u64			dl_period;
};

static inline int dl_bandwidth_enabled(void)
{
	return sysctl_sched_rt_runtime >= 0;
}

/*
 * To keep the bandwidth of -deadline tasks under control
 * we need some place where:
 *  - store the maximum -deadline bandwidth of each cpu;
 *  - cache the fraction of bandwidth that is currently allocated in
 *    each root domain;
 *
 * This is all done in the data structure below. It is similar to the
 * one used for RT-throttling (rt_bandwidth), with the main difference
 * that, since here we are only interested in admission control, we
 * do not decrease any runtime while the group "executes", neither we
 * need a timer to replenish it.
 *
 * With respect to SMP, bandwidth is given on a per root domain basis,
 * meaning that:
 *  - bw (< 100%) is the deadline bandwidth of each CPU;
 *  - total_bw is the currently allocated bandwidth in each root domain;
 */
struct dl_bw {
	raw_spinlock_t		lock;
	u64			bw;
	u64			total_bw;
};

static inline void __dl_update(struct dl_bw *dl_b, s64 bw);

static inline
void __dl_sub(struct dl_bw *dl_b, u64 tsk_bw, int cpus)
{
	dl_b->total_bw -= tsk_bw;
	__dl_update(dl_b, (s32)tsk_bw / cpus);
}

static inline
void __dl_add(struct dl_bw *dl_b, u64 tsk_bw, int cpus)
{
	dl_b->total_bw += tsk_bw;
	__dl_update(dl_b, -((s32)tsk_bw / cpus));
}

static inline bool __dl_overflow(struct dl_bw *dl_b, unsigned long cap,
				 u64 old_bw, u64 new_bw)
{
	return dl_b->bw != -1 &&
	       cap_scale(dl_b->bw, cap) < dl_b->total_bw - old_bw + new_bw;
}

/*
 * Verify the fitness of task @p to run on @cpu taking into account the
 * CPU original capacity and the runtime/deadline ratio of the task.
 *
 * The function will return true if the CPU original capacity of the
 * @cpu scaled by SCHED_CAPACITY_SCALE >= runtime/deadline ratio of the
 * task and false otherwise.
 */
static inline bool dl_task_fits_capacity(struct task_struct *p, int cpu)
{
	unsigned long cap = arch_scale_cpu_capacity(cpu);

	return cap_scale(p->dl.dl_deadline, cap) >= p->dl.dl_runtime;
}

extern void init_dl_bw(struct dl_bw *dl_b);
extern int  sched_dl_global_validate(void);
extern void sched_dl_do_global(void);
extern int  sched_dl_overflow(struct task_struct *p, int policy, const struct sched_attr *attr);
extern void __setparam_dl(struct task_struct *p, const struct sched_attr *attr);
extern void __getparam_dl(struct task_struct *p, struct sched_attr *attr);
extern bool __checkparam_dl(const struct sched_attr *attr);
extern bool dl_param_changed(struct task_struct *p, const struct sched_attr *attr);
extern int  dl_task_can_attach(struct task_struct *p, const struct cpumask *cs_cpus_allowed);
extern int  dl_cpuset_cpumask_can_shrink(const struct cpumask *cur, const struct cpumask *trial);
extern bool dl_cpu_busy(unsigned int cpu);

#ifdef CONFIG_CGROUP_SCHED

#include <linux/cgroup.h>
#include <linux/psi.h>

struct cfs_rq;
struct rt_rq;

extern struct list_head task_groups;

struct cfs_bandwidth {
#ifdef CONFIG_CFS_BANDWIDTH
	raw_spinlock_t		lock;
	ktime_t			period;
	u64			quota;
	u64			runtime;
	s64			hierarchical_quota;

	u8			idle;
	u8			period_active;
	u8			slack_started;
	struct hrtimer		period_timer;
	struct hrtimer		slack_timer;
	struct list_head	throttled_cfs_rq;

	/* Statistics: */
	int			nr_periods;
	int			nr_throttled;
	u64			throttled_time;
#endif
};

/* Task group related information */
struct task_group {
	struct cgroup_subsys_state css;

#ifdef CONFIG_SCHED_CORE
	unsigned int			tagged;
#endif

#ifdef CONFIG_FAIR_GROUP_SCHED
	/* schedulable entities of this group on each CPU */
	struct sched_entity	**se;
	/* runqueue "owned" by this group on each CPU */
	struct cfs_rq		**cfs_rq;
	unsigned long		shares;

#ifdef	CONFIG_SMP
	/*
	 * load_avg can be heavily contended at clock tick time, so put
	 * it in its own cacheline separated from the fields above which
	 * will also be accessed at each tick.
	 */
	atomic_long_t		load_avg ____cacheline_aligned;
#endif
#endif

#ifdef CONFIG_RT_GROUP_SCHED
	struct sched_rt_entity	**rt_se;
	struct rt_rq		**rt_rq;

	struct rt_bandwidth	rt_bandwidth;
#endif

	struct rcu_head		rcu;
	struct list_head	list;

	struct task_group	*parent;
	struct list_head	siblings;
	struct list_head	children;

#ifdef CONFIG_SCHED_AUTOGROUP
	struct autogroup	*autogroup;
#endif

	struct cfs_bandwidth	cfs_bandwidth;

#ifdef CONFIG_UCLAMP_TASK_GROUP
	/* The two decimal precision [%] value requested from user-space */
	unsigned int		uclamp_pct[UCLAMP_CNT];
	/* Clamp values requested for a task group */
	struct uclamp_se	uclamp_req[UCLAMP_CNT];
	/* Effective clamp values used for a task group */
	struct uclamp_se	uclamp[UCLAMP_CNT];
	/* Latency-sensitive flag used for a task group */
	unsigned int		latency_sensitive;

#ifdef CONFIG_HISI_EAS_SCHED
	int			boost;
	int			placement_boost;
#endif
#ifdef CONFIG_SCHED_TOP_TASK
	unsigned int		top_task;
#endif
#endif

#ifdef CONFIG_SCHED_RTG_CGROUP
	/*
	 * Controls whether tasks of this cgroup should be colocated with each
	 * other and tasks of other cgroups that have the same flag turned on.
	 */
	bool colocate;

	/* Controls whether further updates are allowed to the colocate flag */
	bool colocate_update_disabled;
#endif
#ifdef CONFIG_SCHED_VIP_CGROUP
	unsigned int vip_prio;
#endif

#ifdef CONFIG_OPT_TIMER_ENERGY_EFFICIENCY
	unsigned int		timer_slack_pct;
	unsigned int		timer_slack_max_delay_ms;
	unsigned int		timer_slack_exempt_ms;
#endif
#ifdef CONFIG_SCHED_SMT_EXPELLING
	unsigned int		smt_expeller;
#endif
};

#ifdef CONFIG_FAIR_GROUP_SCHED
#define ROOT_TASK_GROUP_LOAD	NICE_0_LOAD

/*
 * A weight of 0 or 1 can cause arithmetics problems.
 * A weight of a cfs_rq is the sum of weights of which entities
 * are queued on this cfs_rq, so a weight of a entity should not be
 * too large, so as the shares value of a task group.
 * (The default weight is 1024 - so there's no practical
 *  limitation from this.)
 */
#define MIN_SHARES		(1UL <<  1)
#define MAX_SHARES		(1UL << 18)
#endif

typedef int (*tg_visitor)(struct task_group *, void *);

extern int walk_tg_tree_from(struct task_group *from,
			     tg_visitor down, tg_visitor up, void *data);

/*
 * Iterate the full tree, calling @down when first entering a node and @up when
 * leaving it for the final time.
 *
 * Caller must hold rcu_lock or sufficient equivalent.
 */
static inline int walk_tg_tree(tg_visitor down, tg_visitor up, void *data)
{
	return walk_tg_tree_from(&root_task_group, down, up, data);
}

extern int tg_nop(struct task_group *tg, void *data);

extern void free_fair_sched_group(struct task_group *tg);
extern int alloc_fair_sched_group(struct task_group *tg, struct task_group *parent);
extern void online_fair_sched_group(struct task_group *tg);
extern void unregister_fair_sched_group(struct task_group *tg);
extern void init_tg_cfs_entry(struct task_group *tg, struct cfs_rq *cfs_rq,
			struct sched_entity *se, int cpu,
			struct sched_entity *parent);
extern void init_cfs_bandwidth(struct cfs_bandwidth *cfs_b);

extern void __refill_cfs_bandwidth_runtime(struct cfs_bandwidth *cfs_b);
extern void start_cfs_bandwidth(struct cfs_bandwidth *cfs_b);
extern void unthrottle_cfs_rq(struct cfs_rq *cfs_rq);

extern void free_rt_sched_group(struct task_group *tg);
extern int alloc_rt_sched_group(struct task_group *tg, struct task_group *parent);
extern void init_tg_rt_entry(struct task_group *tg, struct rt_rq *rt_rq,
		struct sched_rt_entity *rt_se, int cpu,
		struct sched_rt_entity *parent);
extern int sched_group_set_rt_runtime(struct task_group *tg, long rt_runtime_us);
extern int sched_group_set_rt_period(struct task_group *tg, u64 rt_period_us);
extern long sched_group_rt_runtime(struct task_group *tg);
extern long sched_group_rt_period(struct task_group *tg);
extern int sched_rt_can_attach(struct task_group *tg, struct task_struct *tsk);

extern struct task_group *sched_create_group(struct task_group *parent);
extern void sched_online_group(struct task_group *tg,
			       struct task_group *parent);
extern void sched_destroy_group(struct task_group *tg);
extern void sched_offline_group(struct task_group *tg);

extern void sched_move_task(struct task_struct *tsk);

#ifdef CONFIG_FAIR_GROUP_SCHED
extern int sched_group_set_shares(struct task_group *tg, unsigned long shares);

#ifdef CONFIG_SMP
extern void set_task_rq_fair(struct sched_entity *se,
			     struct cfs_rq *prev, struct cfs_rq *next);
#else /* !CONFIG_SMP */
static inline void set_task_rq_fair(struct sched_entity *se,
			     struct cfs_rq *prev, struct cfs_rq *next) { }
#endif /* CONFIG_SMP */
#endif /* CONFIG_FAIR_GROUP_SCHED */

#else /* CONFIG_CGROUP_SCHED */

struct cfs_bandwidth { };

#endif	/* CONFIG_CGROUP_SCHED */

/* CFS-related fields in a runqueue */
struct cfs_rq {
	struct load_weight	load;
	unsigned int		nr_running;
	unsigned int		h_nr_running;      /* SCHED_{NORMAL,BATCH,IDLE} */
	unsigned int		idle_h_nr_running; /* SCHED_IDLE */

	u64			exec_clock;
	u64			min_vruntime;
#ifdef CONFIG_SCHED_CORE
	unsigned int		forceidle_seq;
	u64			min_vruntime_fi;
#endif

#ifndef CONFIG_64BIT
	u64			min_vruntime_copy;
#endif

	struct rb_root_cached	tasks_timeline;

	/*
	 * 'curr' points to currently running entity on this cfs_rq.
	 * It is set to NULL otherwise (i.e when none are currently running).
	 */
	struct sched_entity	*curr;
	struct sched_entity	*next;
	struct sched_entity	*last;
	struct sched_entity	*skip;

#ifdef	CONFIG_SCHED_DEBUG
	unsigned int		nr_spread_over;
#endif

#ifdef CONFIG_SMP
	/*
	 * CFS load tracking
	 */
	struct sched_avg	avg;
#ifndef CONFIG_64BIT
	u64			load_last_update_time_copy;
#endif
	struct {
		raw_spinlock_t	lock ____cacheline_aligned;
		int		nr;
		unsigned long	load_avg;
		unsigned long	util_avg;
		unsigned long	runnable_avg;
	} removed;

#ifdef CONFIG_FAIR_GROUP_SCHED
	unsigned long		tg_load_avg_contrib;
	long			propagate;
	long			prop_runnable_sum;

	/*
	 *   h_load = weight * f(tg)
	 *
	 * Where f(tg) is the recursive weight fraction assigned to
	 * this group.
	 */
	unsigned long		h_load;
	u64			last_h_load_update;
	struct sched_entity	*h_load_next;
#endif /* CONFIG_FAIR_GROUP_SCHED */
#endif /* CONFIG_SMP */

#ifdef CONFIG_FAIR_GROUP_SCHED
	struct rq		*rq;	/* CPU runqueue to which this cfs_rq is attached */

	/*
	 * leaf cfs_rqs are those that hold tasks (lowest schedulable entity in
	 * a hierarchy). Non-leaf lrqs hold other higher schedulable entities
	 * (like users, containers etc.)
	 *
	 * leaf_cfs_rq_list ties together list of leaf cfs_rq's in a CPU.
	 * This list is used during load balance.
	 */
	int			on_list;
	struct list_head	leaf_cfs_rq_list;
	struct task_group	*tg;	/* group that "owns" this runqueue */

#ifdef CONFIG_SCHED_WALT
	struct walt_sched_stats walt_stats;
#endif

#ifdef CONFIG_CFS_BANDWIDTH
	int			runtime_enabled;
	s64			runtime_remaining;

	u64			throttled_clock;
	u64			throttled_clock_task;
	u64			throttled_clock_task_time;
	int			throttled;
	int			throttle_count;
	struct list_head	throttled_list;
#ifdef CONFIG_SCHED_WALT
	u64 cumulative_runnable_avg;
#endif
#endif /* CONFIG_CFS_BANDWIDTH */
#endif /* CONFIG_FAIR_GROUP_SCHED */
};

static inline int rt_bandwidth_enabled(void)
{
	return sysctl_sched_rt_runtime >= 0;
}

/* RT IPI pull logic requires IRQ_WORK */
#if defined(CONFIG_IRQ_WORK) && defined(CONFIG_SMP)
# define HAVE_RT_PUSH_IPI
#endif

/* Real-Time classes' related field in a runqueue: */
struct rt_rq {
	struct rt_prio_array	active;
	unsigned int		rt_nr_running;
	unsigned int		rr_nr_running;
#if defined CONFIG_SMP || defined CONFIG_RT_GROUP_SCHED
	struct {
		int		curr; /* highest queued rt task prio */
#ifdef CONFIG_SMP
		int		next; /* next highest */
#endif
	} highest_prio;
#endif
#ifdef CONFIG_SMP
	unsigned long		rt_nr_migratory;
	unsigned long		rt_nr_total;
	int			overloaded;
	struct plist_head	pushable_tasks;

#endif /* CONFIG_SMP */
	int			rt_queued;

	int			rt_throttled;
	u64			rt_time;
	u64			rt_runtime;
	/* Nests inside the rq lock: */
	raw_spinlock_t		rt_runtime_lock;

#ifdef CONFIG_RT_GROUP_SCHED
	unsigned long		rt_nr_boosted;

	struct rq		*rq;
	struct task_group	*tg;
#endif
#ifdef CONFIG_RT_SOFT_BANDWIDTH
	int			rt_soft_throttled;
	int			throttle_status;
	u64			rt_soft_time;
	u64			rt_soft_runtime;

	struct list_head	soft_throttle_head;
	struct irq_work		rt_throttle_work;
	raw_spinlock_t		throttle_handle_lock;
#endif
};

static inline bool rt_rq_is_runnable(struct rt_rq *rt_rq)
{
	return rt_rq->rt_queued && rt_rq->rt_nr_running;
}

/* Deadline class' related fields in a runqueue */
struct dl_rq {
	/* runqueue is an rbtree, ordered by deadline */
	struct rb_root_cached	root;

	unsigned long		dl_nr_running;

#ifdef CONFIG_SMP
	/*
	 * Deadline values of the currently executing and the
	 * earliest ready task on this rq. Caching these facilitates
	 * the decision whether or not a ready but not running task
	 * should migrate somewhere else.
	 */
	struct {
		u64		curr;
		u64		next;
	} earliest_dl;

	unsigned long		dl_nr_migratory;
	int			overloaded;

	/*
	 * Tasks on this rq that can be pushed away. They are kept in
	 * an rb-tree, ordered by tasks' deadlines, with caching
	 * of the leftmost (earliest deadline) element.
	 */
	struct rb_root_cached	pushable_dl_tasks_root;
#else
	struct dl_bw		dl_bw;
#endif
	/*
	 * "Active utilization" for this runqueue: increased when a
	 * task wakes up (becomes TASK_RUNNING) and decreased when a
	 * task blocks
	 */
	u64			running_bw;

	/*
	 * Utilization of the tasks "assigned" to this runqueue (including
	 * the tasks that are in runqueue and the tasks that executed on this
	 * CPU and blocked). Increased when a task moves to this runqueue, and
	 * decreased when the task moves away (migrates, changes scheduling
	 * policy, or terminates).
	 * This is needed to compute the "inactive utilization" for the
	 * runqueue (inactive utilization = this_bw - running_bw).
	 */
	u64			this_bw;
	u64			extra_bw;

	/*
	 * Inverse of the fraction of CPU utilization that can be reclaimed
	 * by the GRUB algorithm.
	 */
	u64			bw_ratio;
};

#ifdef CONFIG_FAIR_GROUP_SCHED
/* An entity is a task if it doesn't "own" a runqueue */
#define entity_is_task(se)	(!se->my_q)

static inline void se_update_runnable(struct sched_entity *se)
{
	if (!entity_is_task(se))
		se->runnable_weight = se->my_q->h_nr_running;
}

static inline long se_runnable(struct sched_entity *se)
{
	if (entity_is_task(se))
		return !!se->on_rq;
	else
		return se->runnable_weight;
}

#else
#define entity_is_task(se)	1

static inline void se_update_runnable(struct sched_entity *se) {}

static inline long se_runnable(struct sched_entity *se)
{
	return !!se->on_rq;
}
#endif

#ifdef CONFIG_SMP
/*
 * XXX we want to get rid of these helpers and use the full load resolution.
 */
static inline long se_weight(struct sched_entity *se)
{
	return scale_load_down(se->load.weight);
}


static inline bool sched_asym_prefer(int a, int b)
{
	return arch_asym_cpu_priority(a) > arch_asym_cpu_priority(b);
}

struct perf_domain {
	struct em_perf_domain *em_pd;
	struct perf_domain *next;
	struct rcu_head rcu;
};

/* Scheduling group status flags */
#define SG_OVERLOAD		0x1 /* More than one runnable task on a CPU. */
#define SG_OVERUTILIZED		0x2 /* One or more CPUs are over-utilized. */
#ifdef CONFIG_SCHED_PER_SD_OVERUTILIZED
#define SG_HAS_MISFIT_TASK	0x4 /* Group has misfit task. */
#define SG_HAS_VIP		0x8 /* Group has cfs tasks preempted by vip. */
#endif

/*
 * We add the notion of a root-domain which will be used to define per-domain
 * variables. Each exclusive cpuset essentially defines an island domain by
 * fully partitioning the member CPUs from any other cpuset. Whenever a new
 * exclusive cpuset is created, we also create and attach a new root-domain
 * object.
 *
 */
struct root_domain {
	atomic_t		refcount;
	atomic_t		rto_count;
	struct rcu_head		rcu;
	cpumask_var_t		span;
	cpumask_var_t		online;

	/*
	 * Indicate pullable load on at least one CPU, e.g:
	 * - More than one runnable task
	 * - Running task is misfit
	 */
	int			overload;

	/* Indicate one or more cpus over-utilized (tipping point) */
	int			overutilized;

	/*
	 * The bit corresponding to a CPU gets set here if such CPU has more
	 * than one runnable -deadline task (as it is below for RT tasks).
	 */
	cpumask_var_t		dlo_mask;
	atomic_t		dlo_count;
	struct dl_bw		dl_bw;
	struct cpudl		cpudl;

#ifdef HAVE_RT_PUSH_IPI
	/*
	 * For IPI pull requests, loop across the rto_mask.
	 */
	struct irq_work		rto_push_work;
	raw_spinlock_t		rto_lock;
	/* These are only updated and read within rto_lock */
	int			rto_loop;
	int			rto_cpu;
	/* These atomics are updated outside of a lock */
	atomic_t		rto_loop_next;
	atomic_t		rto_loop_start;
#endif
	/*
	 * The "RT overload" flag: it gets set if a CPU has more than
	 * one runnable RT task.
	 */
	cpumask_var_t		rto_mask;
	struct cpupri		cpupri;

	unsigned long		max_cpu_capacity;

	/* First cpu with maximum and minimum original capacity */
	int max_cap_orig_cpu, min_cap_orig_cpu;

	/*
	 * NULL-terminated list of performance domains intersecting with the
	 * CPUs of the rd. Protected by RCU.
	 */
	struct perf_domain __rcu *pd;

};

extern void init_defrootdomain(void);
extern int sched_init_domains(const struct cpumask *cpu_map);
extern void rq_attach_root(struct rq *rq, struct root_domain *rd);
extern void sched_get_rd(struct root_domain *rd);
extern void sched_put_rd(struct root_domain *rd);

#ifdef HAVE_RT_PUSH_IPI
extern void rto_push_irq_work_func(struct irq_work *work);
#endif
#endif /* CONFIG_SMP */

#ifdef CONFIG_UCLAMP_TASK
/*
 * struct uclamp_bucket - Utilization clamp bucket
 * @value: utilization clamp value for tasks on this clamp bucket
 * @tasks: number of RUNNABLE tasks on this clamp bucket
 *
 * Keep track of how many tasks are RUNNABLE for a given utilization
 * clamp value.
 */
struct uclamp_bucket {
	unsigned long value : bits_per(SCHED_CAPACITY_SCALE);
	unsigned long tasks : BITS_PER_LONG - bits_per(SCHED_CAPACITY_SCALE);
};

/*
 * struct uclamp_rq - rq's utilization clamp
 * @value: currently active clamp values for a rq
 * @bucket: utilization clamp buckets affecting a rq
 *
 * Keep track of RUNNABLE tasks on a rq to aggregate their clamp values.
 * A clamp value is affecting a rq when there is at least one task RUNNABLE
 * (or actually running) with that value.
 *
 * There are up to UCLAMP_CNT possible different clamp values, currently there
 * are only two: minimum utilization and maximum utilization.
 *
 * All utilization clamping values are MAX aggregated, since:
 * - for util_min: we want to run the CPU at least at the max of the minimum
 *   utilization required by its currently RUNNABLE tasks.
 * - for util_max: we want to allow the CPU to run up to the max of the
 *   maximum utilization allowed by its currently RUNNABLE tasks.
 *
 * Since on each system we expect only a limited number of different
 * utilization clamp values (UCLAMP_BUCKETS), use a simple array to track
 * the metrics required to compute all the per-rq utilization clamp values.
 */
struct uclamp_rq {
	unsigned int value;
	struct uclamp_bucket bucket[UCLAMP_BUCKETS];
};

DECLARE_STATIC_KEY_FALSE(sched_uclamp_used);
#endif /* CONFIG_UCLAMP_TASK */

/*
 * This is the main, per-CPU runqueue data structure.
 *
 * Locking rule: those places that want to lock multiple runqueues
 * (such as the load balancing or the thread migration code), lock
 * acquire operations must be ordered by ascending &runqueue.
 */
#define NR_MIPS_WINDOW	6
struct rq {
	/* runqueue lock: */
	raw_spinlock_t		__lock;

	/*
	 * nr_running and cpu_load should be in the same cacheline because
	 * remote CPUs use both these fields when doing load calculation.
	 */
	unsigned int		nr_running;
#ifdef CONFIG_NUMA_BALANCING
	unsigned int		nr_numa_running;
	unsigned int		nr_preferred_running;
	unsigned int		numa_migrate_on;
#endif
#ifdef CONFIG_NO_HZ_COMMON
#ifdef CONFIG_SMP
	unsigned long		last_blocked_load_update_tick;
	unsigned int		has_blocked_load;
	call_single_data_t	nohz_csd;
#endif /* CONFIG_SMP */
	unsigned int		nohz_tick_stopped;
	atomic_t		nohz_flags;
#endif /* CONFIG_NO_HZ_COMMON */

#ifdef CONFIG_SMP
	unsigned int		ttwu_pending;
#endif
	u64			nr_switches;

#ifdef CONFIG_UCLAMP_TASK
	/* Utilization clamp values based on CPU's RUNNABLE tasks */
	struct uclamp_rq	uclamp[UCLAMP_CNT] ____cacheline_aligned;
	unsigned int		uclamp_flags;
#define UCLAMP_FLAG_IDLE 0x01
#endif

	struct cfs_rq		cfs;
	struct rt_rq		rt;
	struct dl_rq		dl;

#ifdef CONFIG_FAIR_GROUP_SCHED
	/* list of leaf cfs_rq on this CPU: */
	struct list_head	leaf_cfs_rq_list;
	struct list_head	*tmp_alone_branch;
#endif /* CONFIG_FAIR_GROUP_SCHED */

	/*
	 * This is part of a global counter where only the total sum
	 * over all CPUs matters. A task can increase this counter on
	 * one CPU and if it got migrated afterwards it may decrease
	 * it on another CPU. Always updated under the runqueue lock:
	 */
	unsigned long		nr_uninterruptible;

	struct task_struct __rcu	*curr;
	struct task_struct	*idle;
	struct task_struct	*stop;
	unsigned long		next_balance;
	struct mm_struct	*prev_mm;

	unsigned int		clock_update_flags;
	u64			clock;
	/* Ensure that all clocks are in the same cache line */
	u64			clock_task ____cacheline_aligned;
	u64			clock_pelt;
	unsigned long		lost_idle_time;

	atomic_t		nr_iowait;

#ifdef CONFIG_MEMBARRIER
	int membarrier_state;
#endif

#ifdef CONFIG_SMP
	struct root_domain		*rd;
	struct sched_domain __rcu	*sd;
	struct perf_domain __rcu	*pd;

	unsigned long		cpu_capacity;
	unsigned long		cpu_capacity_orig;

	struct callback_head	*balance_callback;

	unsigned char		nohz_idle_balance;
	unsigned char		idle_balance;

	unsigned long		misfit_task_load;

	/* For active balancing */
	int			active_balance;
	int			push_cpu;
#ifdef CONFIG_SCHED_EAS
	struct task_struct	*push_task;
#endif
	struct cpu_stop_work	active_balance_work;

	/* CPU of this runqueue: */
	int			cpu;
	int			online;

	struct list_head cfs_tasks;

	struct sched_avg	avg_rt;
	struct sched_avg	avg_dl;
#ifdef CONFIG_HAVE_SCHED_AVG_IRQ
	struct sched_avg	avg_irq;
#endif
#ifdef CONFIG_SCHED_THERMAL_PRESSURE
	struct sched_avg	avg_thermal;
#endif
	u64			idle_stamp;
	u64			avg_idle;

	/* This is used to determine avg_idle's max value */
	u64			max_idle_balance_cost;
#endif /* CONFIG_SMP */

#ifdef CONFIG_SCHED_WALT
	struct sched_cluster *cluster;
	struct cpumask freq_domain_cpumask;
	struct walt_sched_stats walt_stats;

	u64 window_start;
	unsigned long walt_flags;

	u64 cur_irqload;
	u64 avg_irqload;
	u64 irqload_ts;
	u64 curr_runnable_sum;
	u64 prev_runnable_sum;
	u64 nt_curr_runnable_sum;
	u64 nt_prev_runnable_sum;
	u64 cum_window_demand_scaled;
	struct load_subtractions load_subs[NUM_TRACKED_WINDOWS];
#ifdef CONFIG_SCHED_RTG
	struct group_cpu_time grp_time;
#endif

#ifdef CONFIG_SCHED_PRED_LOAD
	u64 sum_pred_load;
	u64 predl_window_start;
	u32 curr_max_predls;
	u32 prev_max_predls;
	u64 predl_curr_busy_time;
	u64 predl_prev_busy_time;
#endif

#ifdef CONFIG_SCHED_CPULOAD
	u64 ravg_sum;
#endif

#ifdef CONFIG_SCHED_TOP_TASK
	DECLARE_BITMAP_ARRAY(top_tasks_bitmap,
			NUM_TRACKED_WINDOWS, NUM_LOAD_INDICES);
	struct top_task_entry *top_tasks[NUM_TRACKED_WINDOWS];
	u8 curr_table;
	int top_task_index[NUM_TRACKED_WINDOWS];

	unsigned int top_task_hist_size;
	unsigned int top_task_stats_policy;
	bool top_task_stats_empty_window;
#endif /* CONFIG_SCHED_TOP_TASK */

#ifdef CONFIG_MIGRATION_NOTIFY
	unsigned int freq_inc_notify;
	unsigned int freq_dec_notify;
#endif

#ifdef CONFIG_ED_TASK
	unsigned int ed_task_running_duration;
	unsigned int ed_task_waiting_duration;
	unsigned int ed_new_task_running_duration;
	struct task_struct *ed_task;
#endif /* CONFIG_ED_TASK */
	raw_spinlock_t walt_update_lock;
#endif /* CONFIG_SCHED_WALT */

#ifdef CONFIG_VIP_RAISE_BINDED_KTHREAD
	struct task_struct *binded_kthread;
#endif
#ifdef CONFIG_SCHED_DYNAMIC_PRIO
	struct irq_work dyn_prio_work;
	struct task_struct *dyn_prio_work_task;
	void (*dyn_prio_work_func)(struct task_struct *p);
#endif

#ifdef CONFIG_IRQ_TIME_ACCOUNTING
	u64			prev_irq_time;
#endif
#ifdef CONFIG_PARAVIRT
	u64			prev_steal_time;
#endif
#ifdef CONFIG_PARAVIRT_TIME_ACCOUNTING
	u64			prev_steal_time_rq;
#endif

	/* calc_load related fields */
	unsigned long		calc_load_update;
	long			calc_load_active;

#ifdef CONFIG_MIPS_LOAD_TRACKING
	struct rq_mips_demand rmipsd;
	struct phase_event_pcount	pcount_nr_history[NR_MIPS_WINDOW];
	u64 mips_mem_window_start;
	u64 mips_cpu_window_start;
#endif

#ifdef CONFIG_SCHED_HRTICK
#ifdef CONFIG_SMP
	call_single_data_t	hrtick_csd;
#endif
	struct hrtimer		hrtick_timer;
	ktime_t 		hrtick_time;
#endif

#ifdef CONFIG_DFX_STATS
	struct dfx_info	 	rq_dfx_info;
#endif /* CONFIG_DFX_STATS */

#ifdef CONFIG_SCHEDSTATS
	/* latency stats */
	struct sched_info	rq_sched_info;
	unsigned long long	rq_cpu_time;
	/* could above be rq->cfs_rq.exec_clock + rq->rt_rq.rt_runtime ? */

	/* sys_sched_yield() stats */
	unsigned int		yld_count;

	/* schedule() stats */
	unsigned int		sched_count;
	unsigned int		sched_goidle;

	/* try_to_wake_up() stats */
	unsigned int		ttwu_count;
	unsigned int		ttwu_local;
#endif

#ifdef CONFIG_CPU_IDLE
	/* Must be inspected within a rcu lock section */
	struct cpuidle_state	*idle_state;
#endif

#ifdef CONFIG_CPUIDLE_STATE_NOTIFY
	int idle_state_idx;
#endif

#ifdef CONFIG_SCHED_CORE
	/* per rq */
	struct rq		*core;
	struct rq		*core_max_rq;
	struct task_struct	*core_pick;
	struct task_struct	*core_pick_max;
	unsigned int		core_enabled;
	unsigned int		core_sched_seq;
	struct rb_root		core_tree;

	/* shared state -- careful with sched_core_cpu_deactivate() */
	unsigned int		core_task_seq;
	unsigned int		core_pick_seq;
	unsigned long		core_cookie;
	unsigned int		core_forceidle_count;
	unsigned int		core_forceidle_count_sum;
	unsigned int		core_forceidle_seq;
	unsigned int		core_forceidle_occupation;
	u64			core_forceidle_start;

	u64			core_start_time;
	u64			core_forceidle_time[2]; /* 0-cookie not match, 1-smt expeller */
	unsigned int		core_flags;
	unsigned char		core_prev_mode;
	unsigned char		core_next_mode;
#ifdef CONFIG_SCHED_SMT_EXPELLING
	u8			core_smt_expeller;
#endif
#ifdef CONFIG_SCHED_CORE_PREFER
	struct sched_core_prefer *core_prefer;
#endif
#ifdef CONFIG_SCHED_CORE_LAZY_FORCEIDLE
	u64			lazy_forceidle_start;
	bool			lazy_forceidle;
#endif
#endif

#ifdef CONFIG_SCHED_SMT
	bool			is_smt;
	int			smt_sibling; /* SMT2 */
	unsigned long		smt_capacity;
	unsigned int		smt_cap_discount[DISCOUNT_MAX_NR];
#endif

#ifdef CONFIG_HISI_EAS_SCHED
	bool reserved;
	unsigned int sync_waiting;
#endif
#ifdef CONFIG_SCHED_RUNNING_AVG
	int nr_heavy_running;
#endif
#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL_OPT
	bool skip_overload_detect;
#endif
};

#ifdef CONFIG_FAIR_GROUP_SCHED

/* CPU runqueue to which this cfs_rq is attached */
static inline struct rq *rq_of(struct cfs_rq *cfs_rq)
{
	return cfs_rq->rq;
}

#else

static inline struct rq *rq_of(struct cfs_rq *cfs_rq)
{
	return container_of(cfs_rq, struct rq, cfs);
}
#endif

static inline int cpu_of(struct rq *rq)
{
#ifdef CONFIG_SMP
	return rq->cpu;
#else
	return 0;
#endif
}

struct sched_group;

#ifdef CONFIG_SCHED_SMT_EXPELLING
static inline bool sched_core_smt_expelling(struct rq *rq)
{
	return rq->core->core_smt_expeller;
}
#else
static inline bool sched_core_smt_expelling(struct rq *rq)
{
	return false;
}
#endif

#ifdef CONFIG_SCHED_CORE
static inline struct cpumask *sched_group_span(struct sched_group *sg);

DECLARE_STATIC_KEY_FALSE(__sched_core_enabled);

static inline bool sched_core_enabled(struct rq *rq)
{
#ifdef CONFIG_SMT_ENABLE_CORE_SCHED
	return rq->core_enabled && rq->is_smt;
#else
	return static_branch_unlikely(&__sched_core_enabled) && rq->core_enabled;
#endif
}

static inline bool sched_core_disabled(void)
{
#ifdef CONFIG_SMT_ENABLE_CORE_SCHED
	return false;
#else
	return !static_branch_unlikely(&__sched_core_enabled);
#endif
}

/*
 * Be careful with this function; not for general use. The return value isn't
 * stable unless you actually hold a relevant rq->__lock.
 */
static inline raw_spinlock_t *rq_lockp(struct rq *rq)
{
	if (sched_core_enabled(rq))
		return &rq->core->__lock;

	return &rq->__lock;
}

static inline raw_spinlock_t *__rq_lockp(struct rq *rq)
{
	if (rq->core_enabled)
		return &rq->core->__lock;

	return &rq->__lock;
}

bool cfs_prio_less(struct task_struct *a, struct task_struct *b, bool fi);

#ifdef CONFIG_SCHED_CORE_LAZY_FORCEIDLE
bool lazy_forceidle_can_pick(struct task_struct *p, struct rq *rq);
#endif
/*
 * Helpers to check if the CPU's core cookie matches with the task's cookie
 * when core scheduling is enabled.
 * A special case is that the task's cookie always matches with CPU's core
 * cookie if the CPU is in an idle core.
 */
static inline bool sched_cpu_cookie_match(struct rq *rq, struct task_struct *p)
{
	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	if (!sched_core_enabled(rq))
		return true;

	if (sched_core_smt_expelling(rq))
		return false;

#ifdef CONFIG_SCHED_CORE_LAZY_FORCEIDLE
	return true;
#endif

	return rq->core->core_cookie == p->core_cookie;
}

static inline bool sched_core_cookie_match(struct rq *rq, struct task_struct *p)
{
	bool idle_core = true;
	int cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	if (!sched_core_enabled(rq))
		return true;

	if (sched_core_smt_expelling(rq))
		return false;

#ifdef CONFIG_SCHED_CORE_LAZY_FORCEIDLE
	/*
	 * sched_group_cookie_match is used for select_task_rq.
	 * When LAZY_FORCEIDLE enabled:
	 * if one task no need forceidle
	 * we can select a !cookie_matched cpu and run for a while.
	 */
	if (lazy_forceidle_can_pick(p, rq))
		return true;
#endif

	for_each_cpu(cpu, cpu_smt_mask(cpu_of(rq))) {
		if (!available_idle_cpu(cpu)) {
			idle_core = false;
			break;
		}
	}

	/*
	 * A CPU in an idle core is always the best choice for tasks with
	 * cookies.
	 */
	return idle_core || rq->core->core_cookie == p->core_cookie;
}

static inline bool sched_group_cookie_match(struct rq *rq,
					    struct task_struct *p,
					    struct sched_group *group)
{
	int cpu;

	/* Ignore cookie match if core scheduler is not enabled on the CPU. */
	if (!sched_core_enabled(rq))
		return true;

	for_each_cpu_and(cpu, sched_group_span(group), p->cpus_ptr) {
		if (sched_core_cookie_match(rq, p))
			return true;
	}
	return false;
}

static inline bool sched_core_enqueued(struct task_struct *p)
{
	return !RB_EMPTY_NODE(&p->core_node);
}

extern void sched_core_enqueue(struct rq *rq, struct task_struct *p);
extern void sched_core_dequeue(struct rq *rq, struct task_struct *p, int flags);

extern void sched_core_get(void);
extern void sched_core_put(void);

static inline bool rq_forced_idle(struct rq *rq)
{
	bool core_force_idle = sched_core_smt_expelling(rq) || rq->core->core_forceidle_count;

	return core_force_idle && rq->curr == rq->idle;
}
void sched_core_push_tasks(struct rq *src_rq);
#else /* !CONFIG_SCHED_CORE */

static inline bool sched_core_enabled(struct rq *rq)
{
	return false;
}

static inline bool sched_core_disabled(void)
{
	return true;
}

static inline raw_spinlock_t *rq_lockp(struct rq *rq)
{
	return &rq->__lock;
}

static inline raw_spinlock_t *__rq_lockp(struct rq *rq)
{
	return &rq->__lock;
}

static inline bool sched_cpu_cookie_match(struct rq *rq, struct task_struct *p)
{
	return true;
}

static inline bool sched_core_cookie_match(struct rq *rq, struct task_struct *p)
{
	return true;
}

static inline bool sched_group_cookie_match(struct rq *rq,
					    struct task_struct *p,
					    struct sched_group *group)
{
	return true;
}

static inline bool rq_forced_idle(struct rq *rq)
{
	return false;
}

static inline void sched_core_push_tasks(struct rq *src_rq) {}
#endif

#ifdef CONFIG_SCHED_CORE_PREFER
extern void sched_core_prefer_enqueue(struct rq *rq, struct task_struct *p);
extern void sched_core_prefer_dequeue(struct rq *rq, struct task_struct *p);
extern int find_core_prefer_cpu(struct task_struct *p);
#else
static inline void sched_core_prefer_enqueue(struct rq *rq, struct task_struct *p) {}
static inline void sched_core_prefer_dequeue(struct rq *rq, struct task_struct *p) {}
static inline int find_core_prefer_cpu(struct task_struct *p) { return -1; }
#endif

static inline void lockdep_assert_rq_held(struct rq *rq)
{
	lockdep_assert_held(__rq_lockp(rq));
}

extern void raw_spin_rq_lock_nested(struct rq *rq, int subclass);
extern bool raw_spin_rq_trylock(struct rq *rq);
extern void raw_spin_rq_unlock(struct rq *rq);

static inline void raw_spin_rq_lock(struct rq *rq)
{
	raw_spin_rq_lock_nested(rq, 0);
}

static inline void raw_spin_rq_lock_irq(struct rq *rq)
{
	local_irq_disable();
	raw_spin_rq_lock(rq);
}

static inline void raw_spin_rq_unlock_irq(struct rq *rq)
{
	raw_spin_rq_unlock(rq);
	local_irq_enable();
}

static inline unsigned long _raw_spin_rq_lock_irqsave(struct rq *rq)
{
	unsigned long flags;
	local_irq_save(flags);
	raw_spin_rq_lock(rq);
	return flags;
}

static inline void raw_spin_rq_unlock_irqrestore(struct rq *rq, unsigned long flags)
{
	raw_spin_rq_unlock(rq);
	local_irq_restore(flags);
}

#define raw_spin_rq_lock_irqsave(rq, flags)	\
do {						\
	flags = _raw_spin_rq_lock_irqsave(rq);	\
} while (0)

#ifdef CONFIG_SCHED_SMT
extern void __update_idle_core(struct rq *rq);

static inline void update_idle_core(struct rq *rq)
{
	if (static_branch_unlikely(&sched_smt_present))
		__update_idle_core(rq);
}

#else
static inline void update_idle_core(struct rq *rq) { }
#endif

DECLARE_PER_CPU_SHARED_ALIGNED(struct rq, runqueues);

#define cpu_rq(cpu)		(&per_cpu(runqueues, (cpu)))
#define this_rq()		this_cpu_ptr(&runqueues)
#define task_rq(p)		cpu_rq(task_cpu(p))
#define cpu_curr(cpu)		(cpu_rq(cpu)->curr)
#define raw_rq()		raw_cpu_ptr(&runqueues)

#ifdef CONFIG_FAIR_GROUP_SCHED
static inline struct task_struct *task_of(struct sched_entity *se)
{
	SCHED_WARN_ON(!entity_is_task(se));
	return container_of(se, struct task_struct, se);
}

static inline struct cfs_rq *task_cfs_rq(struct task_struct *p)
{
	return p->se.cfs_rq;
}

/* runqueue on which this entity is (to be) queued */
static inline struct cfs_rq *cfs_rq_of(struct sched_entity *se)
{
	return se->cfs_rq;
}

/* runqueue "owned" by this group */
static inline struct cfs_rq *group_cfs_rq(struct sched_entity *grp)
{
	return grp->my_q;
}

#else

static inline struct task_struct *task_of(struct sched_entity *se)
{
	return container_of(se, struct task_struct, se);
}

static inline struct cfs_rq *task_cfs_rq(struct task_struct *p)
{
	return &task_rq(p)->cfs;
}

static inline struct cfs_rq *cfs_rq_of(struct sched_entity *se)
{
	struct task_struct *p = task_of(se);
	struct rq *rq = task_rq(p);

	return &rq->cfs;
}

/* runqueue "owned" by this group */
static inline struct cfs_rq *group_cfs_rq(struct sched_entity *grp)
{
	return NULL;
}
#endif

extern void update_rq_clock(struct rq *rq);

static inline u64 __rq_clock_broken(struct rq *rq)
{
	return READ_ONCE(rq->clock);
}

/*
 * rq::clock_update_flags bits
 *
 * %RQCF_REQ_SKIP - will request skipping of clock update on the next
 *  call to __schedule(). This is an optimisation to avoid
 *  neighbouring rq clock updates.
 *
 * %RQCF_ACT_SKIP - is set from inside of __schedule() when skipping is
 *  in effect and calls to update_rq_clock() are being ignored.
 *
 * %RQCF_UPDATED - is a debug flag that indicates whether a call has been
 *  made to update_rq_clock() since the last time rq::lock was pinned.
 *
 * If inside of __schedule(), clock_update_flags will have been
 * shifted left (a left shift is a cheap operation for the fast path
 * to promote %RQCF_REQ_SKIP to %RQCF_ACT_SKIP), so you must use,
 *
 *	if (rq-clock_update_flags >= RQCF_UPDATED)
 *
 * to check if %RQCF_UPADTED is set. It'll never be shifted more than
 * one position though, because the next rq_unpin_lock() will shift it
 * back.
 */
#define RQCF_REQ_SKIP		0x01
#define RQCF_ACT_SKIP		0x02
#define RQCF_UPDATED		0x04

static inline void assert_clock_updated(struct rq *rq)
{
	/*
	 * The only reason for not seeing a clock update since the
	 * last rq_pin_lock() is if we're currently skipping updates.
	 */
	SCHED_WARN_ON(rq->clock_update_flags < RQCF_ACT_SKIP);
}

static inline u64 rq_clock(struct rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	return rq->clock;
}

static inline u64 rq_clock_task(struct rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	return rq->clock_task;
}

#ifdef CONFIG_SMP
DECLARE_PER_CPU(u64, clock_task_mult);

static inline u64 rq_clock_task_mult(struct rq *rq)
{
	lockdep_assert_rq_held(rq);
	assert_clock_updated(rq);

	return per_cpu(clock_task_mult, cpu_of(rq));
}
#else
static inline u64 rq_clock_task_mult(struct rq *rq)
{
	return rq_clock_task(rq);
}
#endif

/**
 * By default the decay is the default pelt decay period.
 * The decay shift can change the decay period in
 * multiples of 32.
 *  Decay shift		Decay period(ms)
 *	0			32
 *	1			64
 *	2			128
 *	3			256
 *	4			512
 */
extern int sched_thermal_decay_shift;

static inline u64 rq_clock_thermal(struct rq *rq)
{
	return rq_clock_task(rq) >> sched_thermal_decay_shift;
}

static inline void rq_clock_skip_update(struct rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags |= RQCF_REQ_SKIP;
}

/*
 * See rt task throttling, which is the only time a skip
 * request is cancelled.
 */
static inline void rq_clock_cancel_skipupdate(struct rq *rq)
{
	lockdep_assert_rq_held(rq);
	rq->clock_update_flags &= ~RQCF_REQ_SKIP;
}

struct rq_flags {
	unsigned long flags;
	struct pin_cookie cookie;
#ifdef CONFIG_SCHED_DEBUG
	/*
	 * A copy of (rq::clock_update_flags & RQCF_UPDATED) for the
	 * current pin context is stashed here in case it needs to be
	 * restored in rq_repin_lock().
	 */
	unsigned int clock_update_flags;
#endif
};

/*
 * Lockdep annotation that avoids accidental unlocks; it's like a
 * sticky/continuous lockdep_assert_held().
 *
 * This avoids code that has access to 'struct rq *rq' (basically everything in
 * the scheduler) from accidentally unlocking the rq if they do not also have a
 * copy of the (on-stack) 'struct rq_flags rf'.
 *
 * Also see Documentation/locking/lockdep-design.rst.
 */
static inline void rq_pin_lock(struct rq *rq, struct rq_flags *rf)
{
	rf->cookie = lockdep_pin_lock(__rq_lockp(rq));

#ifdef CONFIG_SCHED_DEBUG
	rq->clock_update_flags &= (RQCF_REQ_SKIP|RQCF_ACT_SKIP);
	rf->clock_update_flags = 0;
#endif
}

static inline void rq_unpin_lock(struct rq *rq, struct rq_flags *rf)
{
#ifdef CONFIG_SCHED_DEBUG
	if (rq->clock_update_flags > RQCF_ACT_SKIP)
		rf->clock_update_flags = RQCF_UPDATED;
#endif

	lockdep_unpin_lock(__rq_lockp(rq), rf->cookie);
}

static inline void rq_repin_lock(struct rq *rq, struct rq_flags *rf)
{
	lockdep_repin_lock(__rq_lockp(rq), rf->cookie);

#ifdef CONFIG_SCHED_DEBUG
	/*
	 * Restore the value we stashed in @rf for this pin context.
	 */
	rq->clock_update_flags |= rf->clock_update_flags;
#endif
}

struct rq *__task_rq_lock(struct task_struct *p, struct rq_flags *rf)
	__acquires(rq->lock);

struct rq *task_rq_lock(struct task_struct *p, struct rq_flags *rf)
	__acquires(p->pi_lock)
	__acquires(rq->lock);

static inline void __task_rq_unlock(struct rq *rq, struct rq_flags *rf)
	__releases(rq->lock)
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

static inline void
task_rq_unlock(struct rq *rq, struct task_struct *p, struct rq_flags *rf)
	__releases(rq->lock)
	__releases(p->pi_lock)
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
	raw_spin_unlock_irqrestore(&p->pi_lock, rf->flags);
}

static inline void
rq_lock_irqsave(struct rq *rq, struct rq_flags *rf)
	__acquires(rq->lock)
{
	raw_spin_rq_lock_irqsave(rq, rf->flags);
	rq_pin_lock(rq, rf);
}

static inline void
rq_lock_irq(struct rq *rq, struct rq_flags *rf)
	__acquires(rq->lock)
{
	raw_spin_rq_lock_irq(rq);
	rq_pin_lock(rq, rf);
}

static inline void
rq_lock(struct rq *rq, struct rq_flags *rf)
	__acquires(rq->lock)
{
	raw_spin_rq_lock(rq);
	rq_pin_lock(rq, rf);
}

static inline void
rq_relock(struct rq *rq, struct rq_flags *rf)
	__acquires(rq->lock)
{
	raw_spin_rq_lock(rq);
	rq_repin_lock(rq, rf);
}

static inline void
rq_unlock_irqrestore(struct rq *rq, struct rq_flags *rf)
	__releases(rq->lock)
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock_irqrestore(rq, rf->flags);
}

static inline void
rq_unlock_irq(struct rq *rq, struct rq_flags *rf)
	__releases(rq->lock)
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock_irq(rq);
}

static inline void
rq_unlock(struct rq *rq, struct rq_flags *rf)
	__releases(rq->lock)
{
	rq_unpin_lock(rq, rf);
	raw_spin_rq_unlock(rq);
}

static inline struct rq *
this_rq_lock_irq(struct rq_flags *rf)
	__acquires(rq->lock)
{
	struct rq *rq;

	local_irq_disable();
	rq = this_rq();
	rq_lock(rq, rf);
	return rq;
}

#ifdef CONFIG_NUMA
enum numa_topology_type {
	NUMA_DIRECT,
	NUMA_GLUELESS_MESH,
	NUMA_BACKPLANE,
};
extern enum numa_topology_type sched_numa_topology_type;
extern int sched_max_numa_distance;
extern bool find_numa_distance(int distance);
extern void sched_init_numa(void);
extern void sched_domains_numa_masks_set(unsigned int cpu);
extern void sched_domains_numa_masks_clear(unsigned int cpu);
extern int sched_numa_find_closest(const struct cpumask *cpus, int cpu);
#else
static inline void sched_init_numa(void) { }
static inline void sched_domains_numa_masks_set(unsigned int cpu) { }
static inline void sched_domains_numa_masks_clear(unsigned int cpu) { }
static inline int sched_numa_find_closest(const struct cpumask *cpus, int cpu)
{
	return nr_cpu_ids;
}
#endif

#ifdef CONFIG_NUMA_BALANCING
/* The regions in numa_faults array from task_struct */
enum numa_faults_stats {
	NUMA_MEM = 0,
	NUMA_CPU,
	NUMA_MEMBUF,
	NUMA_CPUBUF
};
extern void sched_setnuma(struct task_struct *p, int node);
extern int migrate_task_to(struct task_struct *p, int cpu);
extern void init_numa_balancing(unsigned long clone_flags, struct task_struct *p);
#else
static inline void
init_numa_balancing(unsigned long clone_flags, struct task_struct *p)
{
}
#endif /* CONFIG_NUMA_BALANCING */

#ifdef CONFIG_SMP

extern int migrate_swap(struct task_struct *p, struct task_struct *t,
			int cpu, int scpu);
static inline void
queue_balance_callback(struct rq *rq,
		       struct callback_head *head,
		       void (*func)(struct rq *rq))
{
	lockdep_assert_rq_held(rq);

	if (unlikely(head->next))
		return;

	head->func = (void (*)(struct callback_head *))func;
	head->next = rq->balance_callback;
	rq->balance_callback = head;
}

#define rcu_dereference_check_sched_domain(p) \
	rcu_dereference_check((p), \
			      lockdep_is_held(&sched_domains_mutex))

/*
 * The domain tree (rq->sd) is protected by RCU's quiescent state transition.
 * See destroy_sched_domains: call_rcu for details.
 *
 * The domain tree of any CPU may only be accessed from within
 * preempt-disabled sections.
 */
#define for_each_domain(cpu, __sd) \
	for (__sd = rcu_dereference_check_sched_domain(cpu_rq(cpu)->sd); \
			__sd; __sd = __sd->parent)

/**
 * highest_flag_domain - Return highest sched_domain containing flag.
 * @cpu:	The CPU whose highest level of sched domain is to
 *		be returned.
 * @flag:	The flag to check for the highest sched_domain
 *		for the given CPU.
 *
 * Returns the highest sched_domain of a CPU which contains the given flag.
 */
static inline struct sched_domain *highest_flag_domain(int cpu, int flag)
{
	struct sched_domain *sd, *hsd = NULL;

	for_each_domain(cpu, sd) {
		if (!(sd->flags & flag))
			break;
		hsd = sd;
	}

	return hsd;
}

static inline struct sched_domain *lowest_flag_domain(int cpu, int flag)
{
	struct sched_domain *sd;

	for_each_domain(cpu, sd) {
		if (sd->flags & flag)
			break;
	}

	return sd;
}

DECLARE_PER_CPU(struct sched_domain __rcu *, sd_llc);
DECLARE_PER_CPU(int, sd_llc_size);
DECLARE_PER_CPU(int, sd_llc_id);
DECLARE_PER_CPU(struct sched_domain_shared __rcu *, sd_llc_shared);
DECLARE_PER_CPU(struct sched_domain __rcu *, sd_numa);
DECLARE_PER_CPU(struct sched_domain __rcu *, sd_asym_packing);
DECLARE_PER_CPU(struct sched_domain __rcu *, sd_asym_cpucapacity);
extern struct static_key_false sched_asym_cpucapacity;

struct sched_group_capacity {
	atomic_t		ref;
	/*
	 * CPU capacity of this group, SCHED_CAPACITY_SCALE being max capacity
	 * for a single CPU.
	 */
	unsigned long		capacity;
	unsigned long		min_capacity;		/* Min per-CPU capacity in group */
	unsigned long		max_capacity;		/* Max per-CPU capacity in group */
	unsigned long		next_update;
	int			imbalance;		/* XXX unrelated to capacity but shared group state */

#ifdef CONFIG_SCHED_DEBUG
	int			id;
#endif

	unsigned long		cpumask[];		/* Balance mask */
};

struct sched_group {
	struct sched_group	*next;			/* Must be a circular list */
	atomic_t		ref;

	unsigned int		group_weight;
	struct sched_group_capacity *sgc;
	int			asym_prefer_cpu;	/* CPU of highest priority in group */

	/*
	 * The CPUs this group covers.
	 *
	 * NOTE: this field is variable length. (Allocated dynamically
	 * by attaching extra space to the end of the structure,
	 * depending on how many CPUs the kernel has booted up with)
	 */
	unsigned long		cpumask[];
};

static inline struct cpumask *sched_group_span(struct sched_group *sg)
{
	return to_cpumask(sg->cpumask);
}

/*
 * See build_balance_mask().
 */
static inline struct cpumask *group_balance_mask(struct sched_group *sg)
{
	return to_cpumask(sg->sgc->cpumask);
}

/**
 * group_first_cpu - Returns the first CPU in the cpumask of a sched_group.
 * @group: The group whose first CPU is to be returned.
 */
static inline unsigned int group_first_cpu(struct sched_group *group)
{
	return cpumask_first(sched_group_span(group));
}

extern int group_balance_cpu(struct sched_group *sg);

#if defined(CONFIG_SCHED_DEBUG) && defined(CONFIG_SYSCTL)
void register_sched_domain_sysctl(void);
void dirty_sched_domain_sysctl(int cpu);
void unregister_sched_domain_sysctl(void);
#else
static inline void register_sched_domain_sysctl(void)
{
}
static inline void dirty_sched_domain_sysctl(int cpu)
{
}
static inline void unregister_sched_domain_sysctl(void)
{
}
#endif

extern void flush_smp_call_function_from_idle(void);

#else /* !CONFIG_SMP: */
static inline void flush_smp_call_function_from_idle(void) { }
#endif

#include "dfx_stats.h"
#include "stats.h"
#include "autogroup.h"

#if defined(CONFIG_SCHED_CORE) && defined(CONFIG_SCHEDSTATS)

extern void __sched_core_account_forceidle(struct rq *rq);

static inline void sched_core_account_forceidle(struct rq *rq)
{
	__sched_core_account_forceidle(rq);
}

extern void __sched_core_tick(struct rq *rq);

static inline void sched_core_tick(struct rq *rq)
{
	if (sched_core_enabled(rq))
		__sched_core_tick(rq);
}

#else

static inline void sched_core_account_forceidle(struct rq *rq) {}

static inline void sched_core_tick(struct rq *rq) {}

#endif /* CONFIG_SCHED_CORE && CONFIG_SCHEDSTATS */

#ifdef CONFIG_CGROUP_SCHED

/*
 * Return the group to which this tasks belongs.
 *
 * We cannot use task_css() and friends because the cgroup subsystem
 * changes that value before the cgroup_subsys::attach() method is called,
 * therefore we cannot pin it and might observe the wrong value.
 *
 * The same is true for autogroup's p->signal->autogroup->tg, the autogroup
 * core changes this before calling sched_move_task().
 *
 * Instead we use a 'copy' which is updated from sched_move_task() while
 * holding both task_struct::pi_lock and rq::lock.
 */
static inline struct task_group *task_group(struct task_struct *p)
{
	return p->sched_task_group;
}

/* Change a task's cfs_rq and parent entity if it moves across CPUs/groups */
static inline void set_task_rq(struct task_struct *p, unsigned int cpu)
{
#if defined(CONFIG_FAIR_GROUP_SCHED) || defined(CONFIG_RT_GROUP_SCHED)
	struct task_group *tg = task_group(p);
#endif

#ifdef CONFIG_FAIR_GROUP_SCHED
	set_task_rq_fair(&p->se, p->se.cfs_rq, tg->cfs_rq[cpu]);
	p->se.cfs_rq = tg->cfs_rq[cpu];
	p->se.parent = tg->se[cpu];
#endif

#ifdef CONFIG_RT_GROUP_SCHED
	p->rt.rt_rq  = tg->rt_rq[cpu];
	p->rt.parent = tg->rt_se[cpu];
#endif
}

#else /* CONFIG_CGROUP_SCHED */

static inline void set_task_rq(struct task_struct *p, unsigned int cpu) { }
static inline struct task_group *task_group(struct task_struct *p)
{
	return NULL;
}

#endif /* CONFIG_CGROUP_SCHED */

static inline void __set_task_cpu(struct task_struct *p, unsigned int cpu)
{
	set_task_rq(p, cpu);
#ifdef CONFIG_SMP
	/*
	 * After ->cpu is set up to a new value, task_rq_lock(p, ...) can be
	 * successfully executed on another CPU. We must ensure that updates of
	 * per-task data have been completed by this moment.
	 */
	smp_wmb();
#ifdef CONFIG_THREAD_INFO_IN_TASK
	WRITE_ONCE(p->cpu, cpu);
#else
	WRITE_ONCE(task_thread_info(p)->cpu, cpu);
#endif
	p->wake_cpu = cpu;
#endif
}

/*
 * Tunables that become constants when CONFIG_SCHED_DEBUG is off:
 */
#ifdef CONFIG_SCHED_DEBUG
# include <linux/static_key.h>
# define const_debug __read_mostly
#else
# define const_debug const
#endif

#define SCHED_FEAT(name, enabled)	\
	__SCHED_FEAT_##name ,

enum {
#include "features.h"
	__SCHED_FEAT_NR,
};

#undef SCHED_FEAT

#ifdef CONFIG_SCHED_DEBUG

/*
 * To support run-time toggling of sched features, all the translation units
 * (but core.c) reference the sysctl_sched_features defined in core.c.
 */
extern const_debug unsigned int sysctl_sched_features;

#ifdef CONFIG_JUMP_LABEL
#define SCHED_FEAT(name, enabled)					\
static __always_inline bool static_branch_##name(struct static_key *key) \
{									\
	return static_key_##enabled(key);				\
}

#include "features.h"
#undef SCHED_FEAT

extern struct static_key sched_feat_keys[__SCHED_FEAT_NR];
#define sched_feat(x) (static_branch_##x(&sched_feat_keys[__SCHED_FEAT_##x]))

#else /* !CONFIG_JUMP_LABEL */

#define sched_feat(x) (sysctl_sched_features & (1UL << __SCHED_FEAT_##x))

#endif /* CONFIG_JUMP_LABEL */

#else /* !SCHED_DEBUG */

/*
 * Each translation unit has its own copy of sysctl_sched_features to allow
 * constants propagation at compile time and compiler optimization based on
 * features default.
 */
#define SCHED_FEAT(name, enabled)	\
	(1UL << __SCHED_FEAT_##name) * enabled |
static const_debug __maybe_unused unsigned int sysctl_sched_features =
#include "features.h"
	0;
#undef SCHED_FEAT

#define sched_feat(x) !!(sysctl_sched_features & (1UL << __SCHED_FEAT_##x))

#endif /* SCHED_DEBUG */

extern struct static_key_false sched_numa_balancing;
extern struct static_key_false sched_schedstats;

static inline u64 global_rt_period(void)
{
	return (u64)sysctl_sched_rt_period * NSEC_PER_USEC;
}

static inline u64 global_rt_runtime(void)
{
	if (sysctl_sched_rt_runtime < 0)
		return RUNTIME_INF;

	return (u64)sysctl_sched_rt_runtime * NSEC_PER_USEC;
}

#ifdef CONFIG_RT_SOFT_BANDWIDTH
static inline u64 global_rt_soft_runtime(void)
{
	if (sysctl_sched_rt_soft_runtime < 0)
		return RUNTIME_INF;

	return (u64)sysctl_sched_rt_soft_runtime * NSEC_PER_USEC;
}
#endif

static inline int task_current(struct rq *rq, struct task_struct *p)
{
	return rq->curr == p;
}

static inline int task_running(struct rq *rq, struct task_struct *p)
{
#ifdef CONFIG_SMP
	return p->on_cpu;
#else
	return task_current(rq, p);
#endif
}

static inline int task_on_rq_queued(struct task_struct *p)
{
	return p->on_rq == TASK_ON_RQ_QUEUED;
}

static inline int task_on_rq_migrating(struct task_struct *p)
{
	return READ_ONCE(p->on_rq) == TASK_ON_RQ_MIGRATING;
}

/*
 * wake flags
 */
#define WF_SYNC			0x01		/* Waker goes to sleep after wakeup */
#define WF_FORK			0x02		/* Child wakeup after fork */
#define WF_MIGRATED		0x04		/* Internal use, task got migrated */
#define WF_ON_CPU		0x08		/* Wakee is on_cpu */

/*
 * To aid in avoiding the subversion of "niceness" due to uneven distribution
 * of tasks with abnormal "nice" values across CPUs the contribution that
 * each task makes to its run queue's load is weighted according to its
 * scheduling class and "nice" value. For SCHED_NORMAL tasks this is just a
 * scaled version of the new time slice allocation that they receive on time
 * slice expiry etc.
 */

#define WEIGHT_IDLEPRIO		3
#define WMULT_IDLEPRIO		1431655765

extern const int		sched_prio_to_weight[40];
extern const u32		sched_prio_to_wmult[40];
#ifdef CONFIG_SCHED_LATENCY_NICE
extern const int		sched_latency_to_weight[40];
#endif

/*
 * {de,en}queue flags:
 *
 * DEQUEUE_SLEEP  - task is no longer runnable
 * ENQUEUE_WAKEUP - task just became runnable
 *
 * SAVE/RESTORE - an otherwise spurious dequeue/enqueue, done to ensure tasks
 *                are in a known state which allows modification. Such pairs
 *                should preserve as much state as possible.
 *
 * MOVE - paired with SAVE/RESTORE, explicitly does not preserve the location
 *        in the runqueue.
 *
 * ENQUEUE_HEAD      - place at front of runqueue (tail if not specified)
 * ENQUEUE_REPLENISH - CBS (replenish runtime and postpone deadline)
 * ENQUEUE_MIGRATED  - the task was migrated during wakeup
 *
 */

#define DEQUEUE_SLEEP		0x01
#define DEQUEUE_SAVE		0x02 /* Matches ENQUEUE_RESTORE */
#define DEQUEUE_MOVE		0x04 /* Matches ENQUEUE_MOVE */
#define DEQUEUE_NOCLOCK		0x08 /* Matches ENQUEUE_NOCLOCK */

#define ENQUEUE_WAKEUP		0x01
#define ENQUEUE_RESTORE		0x02
#define ENQUEUE_MOVE		0x04
#define ENQUEUE_NOCLOCK		0x08

#define ENQUEUE_HEAD		0x10
#define ENQUEUE_REPLENISH	0x20
#ifdef CONFIG_SMP
#define ENQUEUE_MIGRATED	0x40
#else
#define ENQUEUE_MIGRATED	0x00
#endif

#define ENQUEUE_WAKEUP_SYNC	0x80

#define RETRY_TASK		((void *)-1UL)

struct sched_class {

#ifdef CONFIG_UCLAMP_TASK
	int uclamp_enabled;
#endif

	void (*enqueue_task) (struct rq *rq, struct task_struct *p, int flags);
	void (*dequeue_task) (struct rq *rq, struct task_struct *p, int flags);
	void (*yield_task)   (struct rq *rq);
	bool (*yield_to_task)(struct rq *rq, struct task_struct *p);

	void (*check_preempt_curr)(struct rq *rq, struct task_struct *p, int flags);

	struct task_struct *(*pick_next_task)(struct rq *rq);

	void (*put_prev_task)(struct rq *rq, struct task_struct *p);
	void (*set_next_task)(struct rq *rq, struct task_struct *p, bool first);

#ifdef CONFIG_SMP
	int (*balance)(struct rq *rq, struct task_struct *prev, struct rq_flags *rf);
	int  (*select_task_rq)(struct task_struct *p, int task_cpu, int sd_flag, int flags);

	struct task_struct * (*pick_task)(struct rq *rq);

	void (*migrate_task_rq)(struct task_struct *p, int new_cpu);

	void (*task_woken)(struct rq *this_rq, struct task_struct *task);

	void (*set_cpus_allowed)(struct task_struct *p,
				 const struct cpumask *newmask);

	void (*rq_online)(struct rq *rq);
	void (*rq_offline)(struct rq *rq);
#endif

	void (*task_tick)(struct rq *rq, struct task_struct *p, int queued);
	void (*task_fork)(struct task_struct *p);
	void (*task_dead)(struct task_struct *p);

	/*
	 * The switched_from() call is allowed to drop rq->lock, therefore we
	 * cannot assume the switched_from/switched_to pair is serliazed by
	 * rq->lock. They are however serialized by p->pi_lock.
	 */
	void (*switched_from)(struct rq *this_rq, struct task_struct *task);
	void (*switched_to)  (struct rq *this_rq, struct task_struct *task);
	void (*prio_changed) (struct rq *this_rq, struct task_struct *task,
			      int oldprio);

	unsigned int (*get_rr_interval)(struct rq *rq,
					struct task_struct *task);

	void (*update_curr)(struct rq *rq);

#define TASK_SET_GROUP		0
#define TASK_MOVE_GROUP		1

#ifdef CONFIG_FAIR_GROUP_SCHED
	void (*task_change_group)(struct task_struct *p, int type);
#endif
#ifdef CONFIG_SCHED_WALT
	void (*fixup_walt_sched_stats)(struct rq *rq, struct task_struct *p,
					u16 updated_demand_scaled);
#endif
#ifdef CONFIG_SCHED_EAS
	void  (*check_for_migration)(struct rq *rq, struct task_struct *p);
#endif
} __aligned(STRUCT_ALIGNMENT); /* STRUCT_ALIGN(), vmlinux.lds.h */

static inline void put_prev_task(struct rq *rq, struct task_struct *prev)
{
	WARN_ON_ONCE(rq->curr != prev);
	prev->sched_class->put_prev_task(rq, prev);
}

static inline void set_next_task(struct rq *rq, struct task_struct *next)
{
	next->sched_class->set_next_task(rq, next, false);
}

/* Defined in include/asm-generic/vmlinux.lds.h */
extern struct sched_class __begin_sched_classes[];
extern struct sched_class __end_sched_classes[];

#define sched_class_highest (__end_sched_classes - 1)
#define sched_class_lowest  (__begin_sched_classes - 1)

#define for_class_range(class, _from, _to) \
	for (class = (_from); class != (_to); class--)

#define for_each_class(class) \
	for_class_range(class, sched_class_highest, sched_class_lowest)

extern const struct sched_class stop_sched_class;
extern const struct sched_class dl_sched_class;
extern const struct sched_class rt_sched_class;
extern const struct sched_class fair_sched_class;
extern const struct sched_class idle_sched_class;

static inline bool sched_stop_runnable(struct rq *rq)
{
	return rq->stop && task_on_rq_queued(rq->stop);
}

static inline bool sched_dl_runnable(struct rq *rq)
{
	return rq->dl.dl_nr_running > 0;
}

static inline bool sched_rt_runnable(struct rq *rq)
{
	return rq->rt.rt_queued > 0;
}

static inline bool sched_fair_runnable(struct rq *rq)
{
	return rq->cfs.nr_running > 0;
}

extern struct task_struct *pick_next_task_fair(struct rq *rq, struct task_struct *prev, struct rq_flags *rf);
extern struct task_struct *pick_next_task_idle(struct rq *rq);

#ifdef CONFIG_SMP

extern void update_group_capacity(struct sched_domain *sd, int cpu);

extern void trigger_load_balance(struct rq *rq);

extern void set_cpus_allowed_common(struct task_struct *p, const struct cpumask *new_mask);

#endif

#ifdef CONFIG_CPU_IDLE
static inline void idle_set_state(struct rq *rq,
				  struct cpuidle_state *idle_state)
{
	rq->idle_state = idle_state;
}

static inline struct cpuidle_state *idle_get_state(struct rq *rq)
{
	SCHED_WARN_ON(!rcu_read_lock_held());

	return rq->idle_state;
}
#else
static inline void idle_set_state(struct rq *rq,
				  struct cpuidle_state *idle_state)
{
}

static inline struct cpuidle_state *idle_get_state(struct rq *rq)
{
	return NULL;
}
#endif

#ifdef CONFIG_CPUIDLE_STATE_NOTIFY
static inline int idle_get_state_idx(struct rq *rq)
{
	return rq->idle_state_idx;
}
#else
static inline int idle_get_state_idx(struct rq *rq)
{
	return -1;
}
#endif

extern void schedule_idle(void);

extern void sysrq_sched_debug_show(void);
extern void sched_init_granularity(void);
extern void update_max_interval(void);

extern void init_sched_dl_class(void);
extern void init_sched_rt_class(void);
extern void init_sched_fair_class(void);

extern void reweight_task(struct task_struct *p, int prio);

extern void resched_curr(struct rq *rq);
extern void resched_cpu(int cpu);

extern struct rt_bandwidth def_rt_bandwidth;
extern void init_rt_bandwidth(struct rt_bandwidth *rt_b, u64 period, u64 runtime);

extern struct dl_bandwidth def_dl_bandwidth;
extern void init_dl_bandwidth(struct dl_bandwidth *dl_b, u64 period, u64 runtime);
extern void init_dl_task_timer(struct sched_dl_entity *dl_se);
extern void init_dl_inactive_task_timer(struct sched_dl_entity *dl_se);

#define BW_SHIFT		20
#define BW_UNIT			(1 << BW_SHIFT)
#define RATIO_SHIFT		8
#define MAX_BW_BITS		(64 - BW_SHIFT)
#define MAX_BW			((1ULL << MAX_BW_BITS) - 1)
unsigned long to_ratio(u64 period, u64 runtime);

extern void init_entity_runnable_average(struct sched_entity *se);
extern void post_init_entity_util_avg(struct task_struct *p);

#ifdef CONFIG_NO_HZ_FULL
extern bool sched_can_stop_tick(struct rq *rq);
extern int __init sched_tick_offload_init(void);

/*
 * Tick may be needed by tasks in the runqueue depending on their policy and
 * requirements. If tick is needed, lets send the target an IPI to kick it out of
 * nohz mode if necessary.
 */
static inline void sched_update_tick_dependency(struct rq *rq)
{
	int cpu = cpu_of(rq);

	if (!tick_nohz_full_cpu(cpu))
		return;

	if (sched_can_stop_tick(rq))
		tick_nohz_dep_clear_cpu(cpu, TICK_DEP_BIT_SCHED);
	else
		tick_nohz_dep_set_cpu(cpu, TICK_DEP_BIT_SCHED);
}
#else
static inline int sched_tick_offload_init(void) { return 0; }
static inline void sched_update_tick_dependency(struct rq *rq) { }
#endif

static inline void add_nr_running(struct rq *rq, unsigned count)
{
	unsigned prev_nr = rq->nr_running;

	rq->nr_running = prev_nr + count;
	if (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, count);
	}

#ifdef CONFIG_SCHED_INDICATE_FCM_IDLE
	add_total_nr_running(count);
#endif

#ifdef CONFIG_SCHED_RUNNING_AVG
	sched_update_nr_prod(rq);
#endif
#ifdef CONFIG_SMP
	if (prev_nr < 2 && rq->nr_running >= 2) {
		if (!READ_ONCE(rq->rd->overload))
			WRITE_ONCE(rq->rd->overload, 1);
	}
#endif

	sched_update_tick_dependency(rq);
}

static inline void sub_nr_running(struct rq *rq, unsigned count)
{
	rq->nr_running -= count;
	if (trace_sched_update_nr_running_tp_enabled()) {
		call_trace_sched_update_nr_running(rq, -count);
	}

#ifdef CONFIG_SCHED_INDICATE_FCM_IDLE
	sub_total_nr_running(count);
#endif

#ifdef CONFIG_SCHED_RUNNING_AVG
	sched_update_nr_prod(rq);
#endif

	/* Check if we still need preemption */
	sched_update_tick_dependency(rq);
}

extern void activate_task(struct rq *rq, struct task_struct *p, int flags);
extern void deactivate_task(struct rq *rq, struct task_struct *p, int flags);

extern void check_preempt_curr(struct rq *rq, struct task_struct *p, int flags);

extern const_debug unsigned int sysctl_sched_nr_migrate;
extern const_debug unsigned int sysctl_sched_migration_cost;

#ifdef CONFIG_SCHED_HRTICK

/*
 * Use hrtick when:
 *  - enabled by features
 *  - hrtimer is actually high res
 */
static inline int hrtick_enabled(struct rq *rq)
{
	if (!sched_feat(HRTICK))
		return 0;
	if (!cpu_active(cpu_of(rq)))
		return 0;
	return hrtimer_is_hres_active(&rq->hrtick_timer);
}

void hrtick_start(struct rq *rq, u64 delay);

#else

static inline int hrtick_enabled(struct rq *rq)
{
	return 0;
}

#endif /* CONFIG_SCHED_HRTICK */

#ifdef CONFIG_SCHED_WALT
u64 sched_ktime_clock(void);
#else
static inline u64 sched_ktime_clock(void)
{
	return sched_clock();
}
#endif

#ifndef arch_scale_freq_tick
static __always_inline
void arch_scale_freq_tick(void)
{
}
#endif

#ifndef arch_scale_freq_capacity
/**
 * arch_scale_freq_capacity - get the frequency scale factor of a given CPU.
 * @cpu: the CPU in question.
 *
 * Return: the frequency scale factor normalized against SCHED_CAPACITY_SCALE, i.e.
 *
 *     f_curr
 *     ------ * SCHED_CAPACITY_SCALE
 *     f_max
 */
static __always_inline
unsigned long arch_scale_freq_capacity(int cpu)
{
	return SCHED_CAPACITY_SCALE;
}
#endif

unsigned long capacity_curr_of(int cpu);
unsigned long cpu_util(int cpu);
unsigned long cpu_util_without(int cpu, struct task_struct *p);

#ifdef CONFIG_SMP
#ifdef CONFIG_SCHED_WALT
extern unsigned int sysctl_sched_use_walt_cpu_util;
extern unsigned int walt_disabled;
#endif

static inline bool rq_order_less(struct rq *rq1, struct rq *rq2)
{
#ifdef CONFIG_SCHED_CORE
	/*
	 * In order to not have {0,2},{1,3} turn into into an AB-BA,
	 * order by core-id first and cpu-id second.
	 *
	 * Notably:
	 *
	 *	double_rq_lock(0,3); will take core-0, core-1 lock
	 *	double_rq_lock(1,2); will take core-1, core-0 lock
	 *
	 * when only cpu-id is considered.
	 */
	if (rq1->core->cpu < rq2->core->cpu)
		return true;
	if (rq1->core->cpu > rq2->core->cpu)
		return false;

	/*
	 * __sched_core_flip() relies on SMT having cpu-id lock order.
	 */
#endif
	return rq1->cpu < rq2->cpu;
}

extern void double_rq_lock(struct rq *rq1, struct rq *rq2);

#ifdef CONFIG_PREEMPTION

/*
 * fair double_lock_balance: Safely acquires both rq->locks in a fair
 * way at the expense of forcing extra atomic operations in all
 * invocations.  This assures that the double_lock is acquired using the
 * same underlying policy as the spinlock_t on this architecture, which
 * reduces latency compared to the unfair variant below.  However, it
 * also adds more overhead and therefore may reduce throughput.
 */
static inline int _double_lock_balance(struct rq *this_rq, struct rq *busiest)
	__releases(this_rq->lock)
	__acquires(busiest->lock)
	__acquires(this_rq->lock)
{
	raw_spin_rq_unlock(this_rq);
	double_rq_lock(this_rq, busiest);

	return 1;
}

#else
/*
 * Unfair double_lock_balance: Optimizes throughput at the expense of
 * latency by eliminating extra atomic operations when the locks are
 * already in proper order on entry.  This favors lower CPU-ids and will
 * grant the double lock to lower CPUs over higher ids under contention,
 * regardless of entry order into the function.
 */
static inline int _double_lock_balance(struct rq *this_rq, struct rq *busiest)
	__releases(this_rq->lock)
	__acquires(busiest->lock)
	__acquires(this_rq->lock)
{
	if (__rq_lockp(this_rq) == __rq_lockp(busiest))
		return 0;

	if (likely(raw_spin_rq_trylock(busiest)))
		return 0;

	if (rq_order_less(this_rq, busiest)) {
		raw_spin_rq_lock_nested(busiest, SINGLE_DEPTH_NESTING);
		return 0;
	}

	raw_spin_rq_unlock(this_rq);
	double_rq_lock(this_rq, busiest);

	return 1;
}

#endif /* CONFIG_PREEMPTION */

/*
 * double_lock_balance - lock the busiest runqueue, this_rq is locked already.
 */
static inline int double_lock_balance(struct rq *this_rq, struct rq *busiest)
{
	lockdep_assert_irqs_disabled();

	return _double_lock_balance(this_rq, busiest);
}

static inline void double_unlock_balance(struct rq *this_rq, struct rq *busiest)
	__releases(busiest->lock)
{
	if (__rq_lockp(this_rq) != __rq_lockp(busiest))
		raw_spin_rq_unlock(busiest);
	lock_set_subclass(&__rq_lockp(this_rq)->dep_map, 0, _RET_IP_);
}

static inline void double_lock(spinlock_t *l1, spinlock_t *l2)
{
	if (l1 > l2)
		swap(l1, l2);

	spin_lock(l1);
	spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

static inline void double_lock_irq(spinlock_t *l1, spinlock_t *l2)
{
	if (l1 > l2)
		swap(l1, l2);

	spin_lock_irq(l1);
	spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

static inline void double_raw_lock(raw_spinlock_t *l1, raw_spinlock_t *l2)
{
	if (l1 > l2)
		swap(l1, l2);

	raw_spin_lock(l1);
	raw_spin_lock_nested(l2, SINGLE_DEPTH_NESTING);
}

/*
 * double_rq_unlock - safely unlock two runqueues
 *
 * Note this does not restore interrupts like task_rq_unlock,
 * you need to do so manually after calling.
 */
static inline void double_rq_unlock(struct rq *rq1, struct rq *rq2)
	__releases(rq1->lock)
	__releases(rq2->lock)
{
	if (__rq_lockp(rq1) != __rq_lockp(rq2))
		raw_spin_rq_unlock(rq2);
	else
		__release(rq2->lock);
	raw_spin_rq_unlock(rq1);
}

extern void set_rq_online (struct rq *rq);
extern void set_rq_offline(struct rq *rq);
extern bool sched_smp_initialized;

#else /* CONFIG_SMP */

/*
 * double_rq_lock - safely lock two runqueues
 *
 * Note this does not disable interrupts like task_rq_lock,
 * you need to do so manually before calling.
 */
static inline void double_rq_lock(struct rq *rq1, struct rq *rq2)
	__acquires(rq1->lock)
	__acquires(rq2->lock)
{
	BUG_ON(!irqs_disabled());
	BUG_ON(rq1 != rq2);
	raw_spin_rq_lock(rq1);
	__acquire(rq2->lock);	/* Fake it out ;) */
}

/*
 * double_rq_unlock - safely unlock two runqueues
 *
 * Note this does not restore interrupts like task_rq_unlock,
 * you need to do so manually after calling.
 */
static inline void double_rq_unlock(struct rq *rq1, struct rq *rq2)
	__releases(rq1->lock)
	__releases(rq2->lock)
{
	BUG_ON(rq1 != rq2);
	raw_spin_rq_unlock(rq1);
	__release(rq2->lock);
}

#endif

extern struct sched_entity *__pick_first_entity(struct cfs_rq *cfs_rq);
extern struct sched_entity *__pick_last_entity(struct cfs_rq *cfs_rq);

#ifdef	CONFIG_SCHED_DEBUG
extern bool sched_debug_enabled;

extern void print_cfs_stats(struct seq_file *m, int cpu);
extern void print_rt_stats(struct seq_file *m, int cpu);
extern void print_dl_stats(struct seq_file *m, int cpu);
extern void print_cfs_rq(struct seq_file *m, int cpu, struct cfs_rq *cfs_rq);
extern void print_rt_rq(struct seq_file *m, int cpu, struct rt_rq *rt_rq);
extern void print_dl_rq(struct seq_file *m, int cpu, struct dl_rq *dl_rq);
#ifdef CONFIG_NUMA_BALANCING
extern void
show_numa_stats(struct task_struct *p, struct seq_file *m);
extern void
print_numa_stats(struct seq_file *m, int node, unsigned long tsf,
	unsigned long tpf, unsigned long gsf, unsigned long gpf);
#endif /* CONFIG_NUMA_BALANCING */
#endif /* CONFIG_SCHED_DEBUG */

extern void init_cfs_rq(struct cfs_rq *cfs_rq);
extern void init_rt_rq(struct rt_rq *rt_rq);
extern void init_dl_rq(struct dl_rq *dl_rq);

extern void cfs_bandwidth_usage_inc(void);
extern void cfs_bandwidth_usage_dec(void);

#ifdef CONFIG_NO_HZ_COMMON
#define NOHZ_BALANCE_KICK_BIT	0
#define NOHZ_STATS_KICK_BIT	1

#define NOHZ_BALANCE_KICK	BIT(NOHZ_BALANCE_KICK_BIT)
#define NOHZ_STATS_KICK		BIT(NOHZ_STATS_KICK_BIT)

#define NOHZ_KICK_MASK	(NOHZ_BALANCE_KICK | NOHZ_STATS_KICK)

#define nohz_flags(cpu)	(&cpu_rq(cpu)->nohz_flags)

extern void nohz_balance_exit_idle(struct rq *rq);
#else
static inline void nohz_balance_exit_idle(struct rq *rq) { }
#endif


#ifdef CONFIG_SMP
static inline
void __dl_update(struct dl_bw *dl_b, s64 bw)
{
	struct root_domain *rd = container_of(dl_b, struct root_domain, dl_bw);
	int i;

	RCU_LOCKDEP_WARN(!rcu_read_lock_sched_held(),
			 "sched RCU must be held");
	for_each_cpu_and(i, rd->span, cpu_active_mask) {
		struct rq *rq = cpu_rq(i);

		rq->dl.extra_bw += bw;
	}
}
#else
static inline
void __dl_update(struct dl_bw *dl_b, s64 bw)
{
	struct dl_rq *dl = container_of(dl_b, struct dl_rq, dl_bw);

	dl->extra_bw += bw;
}
#endif


#ifdef CONFIG_IRQ_TIME_ACCOUNTING
struct irqtime {
	u64			total;
	u64			tick_delta;
	u64			irq_start_time;
	struct u64_stats_sync	sync;
};

DECLARE_PER_CPU(struct irqtime, cpu_irqtime);

/*
 * Returns the irqtime minus the softirq time computed by ksoftirqd.
 * Otherwise ksoftirqd's sum_exec_runtime is substracted its own runtime
 * and never move forward.
 */
static inline u64 irq_time_read(int cpu)
{
	struct irqtime *irqtime = &per_cpu(cpu_irqtime, cpu);
	unsigned int seq;
	u64 total;

	do {
		seq = __u64_stats_fetch_begin(&irqtime->sync);
		total = irqtime->total;
	} while (__u64_stats_fetch_retry(&irqtime->sync, seq));

	return total;
}
#endif /* CONFIG_IRQ_TIME_ACCOUNTING */

static inline bool use_pelt(void)
{
#ifdef CONFIG_SCHED_WALT
	return (!sysctl_sched_use_walt_cpu_util || walt_disabled);
#else
	return true;
#endif
}

#ifdef CONFIG_CPU_FREQ
#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL_OPT
DECLARE_PER_CPU(struct update_util_data *, cpufreq_update_util_data);
DECLARE_PER_CPU(rwlock_t, update_util_data_lock);
#else
DECLARE_PER_CPU(struct update_util_data __rcu *, cpufreq_update_util_data);
#endif

/**
 * cpufreq_update_util - Take a note about CPU utilization changes.
 * @rq: Runqueue to carry out the update for.
 * @flags: Update reason flags.
 *
 * This function is called by the scheduler on the CPU whose utilization is
 * being updated.
 *
 * It can only be called from RCU-sched read-side critical sections.
 *
 * The way cpufreq is currently arranged requires it to evaluate the CPU
 * performance state (frequency/voltage) on a regular basis to prevent it from
 * being stuck in a completely inadequate performance level for too long.
 * That is not guaranteed to happen if the updates are only triggered from CFS
 * and DL, though, because they may not be coming in if only RT tasks are
 * active all the time (or there are RT tasks only).
 *
 * As a workaround for that issue, this function is called periodically by the
 * RT sched class to trigger extra cpufreq updates to prevent it from stalling,
 * but that really is a band-aid.  Going forward it should be replaced with
 * solutions targeted more specifically at RT tasks.
 */
static inline void cpufreq_update_util(struct rq *rq, unsigned int flags)
{
	struct update_util_data *data;
	u64 clock;
#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL_OPT
	rwlock_t *lock = &per_cpu(update_util_data_lock, cpu_of(rq));
#endif
#ifdef CONFIG_SCHED_WALT
	if (!(flags & SCHED_CPUFREQ_WALT))
		return;

	clock = sched_ktime_clock();
#else
	clock = rq_clock(rq);
#endif
#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL_OPT
	if (!read_trylock(lock))
		return;

	data = per_cpu(cpufreq_update_util_data, cpu_of(rq));
	if (data)
		data->func(data, clock, flags);

	read_unlock(lock);
#else
	data = rcu_dereference_sched(*per_cpu_ptr(&cpufreq_update_util_data,
						  cpu_of(rq)));
	if (data)
		data->func(data, clock, flags);
#endif
}
#else
static inline void cpufreq_update_util(struct rq *rq, unsigned int flags) {}
#endif /* CONFIG_CPU_FREQ */

#ifdef CONFIG_UCLAMP_TASK
unsigned long uclamp_eff_value(struct task_struct *p, enum uclamp_id clamp_id);
static inline int uclamp_task_util_boost(struct task_struct *p);

/**
 * uclamp_rq_util_with - clamp @util with @rq and @p effective uclamp values.
 * @rq:		The rq to clamp against. Must not be NULL.
 * @util:	The util value to clamp.
 * @p:		The task to clamp against. Can be NULL if you want to clamp
 *		against @rq only.
 *
 * Clamps the passed @util to the max(@rq, @p) effective uclamp values.
 *
 * If sched_uclamp_used static key is disabled, then just return the util
 * without any clamping since uclamp aggregation at the rq level in the fast
 * path is disabled, rendering this operation a NOP.
 *
 * Use uclamp_eff_value() if you don't care about uclamp values at rq level. It
 * will return the correct effective uclamp value of the task even if the
 * static key is disabled.
 */
static __always_inline
unsigned long uclamp_rq_util_with(struct rq *rq, unsigned long util,
				  struct task_struct *p)
{
	unsigned long min_util = 0;
	unsigned long max_util = 0;

	if (!static_branch_likely(&sched_uclamp_used))
		return util;

	if (p) {
		min_util = uclamp_eff_value(p, UCLAMP_MIN);
		max_util = uclamp_eff_value(p, UCLAMP_MAX);

		/*
		 * Ignore last runnable task's max clamp, as this task will
		 * reset it. Similarly, no need to read the rq's min clamp.
		 */
		if (rq->uclamp_flags & UCLAMP_FLAG_IDLE)
			goto out;
	}

	min_util = max_t(unsigned long, min_util, READ_ONCE(rq->uclamp[UCLAMP_MIN].value));
	max_util = max_t(unsigned long, max_util, READ_ONCE(rq->uclamp[UCLAMP_MAX].value));
out:
	/*
	 * Since CPU's {min,max}_util clamps are MAX aggregated considering
	 * RUNNABLE tasks with _different_ clamps, we can end up with an
	 * inversion. Fix it now when the clamps are applied.
	 */
	if (unlikely(min_util >= max_util))
		return min_util;

	return clamp(util, min_util, max_util);
}

static inline bool uclamp_boosted(struct task_struct *p)
{
	return (uclamp_eff_value(p, UCLAMP_MIN) > 0 || uclamp_task_util_boost(p) > 0);
}

/*
 * When uclamp is compiled in, the aggregation at rq level is 'turned off'
 * by default in the fast path and only gets turned on once userspace performs
 * an operation that requires it.
 *
 * Returns true if userspace opted-in to use uclamp and aggregation at rq level
 * hence is active.
 */
static inline bool uclamp_is_used(void)
{
	return static_branch_likely(&sched_uclamp_used);
}
#else /* CONFIG_UCLAMP_TASK */
static inline
unsigned long uclamp_rq_util_with(struct rq *rq, unsigned long util,
				  struct task_struct *p)
{
	return util;
}

static inline bool uclamp_boosted(struct task_struct *p)
{
	return false;
}

static inline bool uclamp_is_used(void)
{
	return false;
}
#endif /* CONFIG_UCLAMP_TASK */

#ifdef CONFIG_UCLAMP_TASK_GROUP
static inline bool uclamp_latency_sensitive(struct task_struct *p)
{
	struct cgroup_subsys_state *css = NULL;
	struct task_group *tg = NULL;
	bool ret = false;

	rcu_read_lock();
	css = task_css(p, cpu_cgrp_id);
	if (!css)
		goto unlock;

	tg = container_of(css, struct task_group, css);
	ret = tg->latency_sensitive;

unlock:
	rcu_read_unlock();
	return ret;
}

#ifdef CONFIG_HISI_EAS_SCHED
static inline int uclamp_task_util_boost(struct task_struct *p)
{
	struct cgroup_subsys_state *css = NULL;
	struct task_group *tg = NULL;
	int ret = 0;

	rcu_read_lock();
	css = task_css(p, cpu_cgrp_id);
	if (!css)
		goto unlock;

	tg = container_of(css, struct task_group, css);
	ret = tg->boost;

unlock:
	rcu_read_unlock();
	return ret;
}

/*
 * Considering there's no rmdir for cpu cgroup on our platform, which
 * means no use-after-free concern, we can visit task_css(p, cpu_cgrp_id)
 * without rcu lock and visit task_group(p) without pi or rq lock.
 * Task_group(p) is better as far as I can see.
 */
static inline int uclamp_task_placement_boost(struct task_struct *p)
{
	return task_group(p)->placement_boost;
}

extern int global_boost_enable;
extern int boot_boost;
static inline bool is_heavy_rt(struct task_struct *p);

static inline int task_placement_boosted(struct task_struct *p)
{
	bool boosted;

	boosted = uclamp_task_placement_boost(p) > 0;
	/*
	 * Keep the old logic for compatibility with perfgenius on old
	 * platforms. And we hope global_boost_enable will always be false
	 * on future platforms.
	 */
	boosted = boosted || (global_boost_enable && uclamp_boosted(p));

	if (rt_task(p))
		boosted = boosted && is_heavy_rt(p);
	else
		boosted = boosted || boot_boost;

	return boosted;
}
#endif

#ifdef CONFIG_SCHED_TOP_TASK
static inline bool uclamp_top_task(struct task_struct *p)
{
	struct cgroup_subsys_state *css = NULL;
	struct task_group *tg = NULL;
	bool ret = false;

	rcu_read_lock();
	css = task_css(p, cpu_cgrp_id);
	if (!css)
		goto unlock;

	tg = container_of(css, struct task_group, css);
	ret = tg->top_task;

unlock:
	rcu_read_unlock();
	return ret;
}
#endif

#ifdef CONFIG_SCHED_VIP_CGROUP
static inline unsigned int uclamp_vip_prio(struct task_struct *p)
{
	struct cgroup_subsys_state *css = NULL;
	struct task_group *tg = NULL;
	unsigned int ret = 0;

	rcu_read_lock();
	css = task_css(p, cpu_cgrp_id);
	if (!css)
		goto unlock;

	tg = container_of(css, struct task_group, css);
	ret = tg->vip_prio;

unlock:
	rcu_read_unlock();
	return ret;
}
#endif

#else
static inline bool uclamp_latency_sensitive(struct task_struct *p)
{
	return false;
}

static inline int uclamp_task_util_boost(struct task_struct *p)
{
	return 0;
}

static inline int uclamp_task_placement_boost(struct task_struct *p)
{
	return 0;
}

static inline int task_placement_boosted(struct task_struct *p)
{
	return 0;
}

static inline bool uclamp_top_task(struct task_struct *p)
{
	return false;
}
#endif /* CONFIG_UCLAMP_TASK_GROUP */

#ifdef arch_scale_freq_capacity
# ifndef arch_scale_freq_invariant
#  define arch_scale_freq_invariant()	true
# endif
#else
# define arch_scale_freq_invariant()	false
#endif

#define cap_scale(v, s) ((v)*(s) >> SCHED_CAPACITY_SHIFT)

#ifdef CONFIG_SMP
static inline unsigned long capacity_of(int cpu)
{
#ifdef CONFIG_SMT_MODE_GOV
	if (cpu_rq(cpu)->is_smt)
		return cpu_rq(cpu)->smt_capacity;
#endif
	return cpu_rq(cpu)->cpu_capacity;
}

static inline unsigned long capacity_orig_of(int cpu)
{
	return cpu_rq(cpu)->cpu_capacity_orig;
}

/* Returns true if CPU a has higher CPU capacity than CPU b */
static inline bool higher_cpu_capacity(int a, int b)
{
#ifdef CONFIG_SMT_MODE_GOV
	return capacity_of(a) * 100 > capacity_of(b) * HIGHER_CPU_CAP_PCT;
#else
	return capacity_orig_of(a) > capacity_orig_of(b);
#endif
}
#endif

unsigned long capacity_spare_without(int cpu, struct task_struct *p);

/**
 * enum schedutil_type - CPU utilization type
 * @FREQUENCY_UTIL:	Utilization used to select frequency
 * @ENERGY_UTIL:	Utilization used during energy calculation
 *
 * The utilization signals of all scheduling classes (CFS/RT/DL) and IRQ time
 * need to be aggregated differently depending on the usage made of them. This
 * enum is used within schedutil_freq_util() to differentiate the types of
 * utilization expected by the callers, and adjust the aggregation accordingly.
 */
enum schedutil_type {
	FREQUENCY_UTIL,
	ENERGY_UTIL,
};

#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL

unsigned long schedutil_cpu_util(int cpu, unsigned long util_cfs,
				 unsigned long max, enum schedutil_type type,
				 struct task_struct *p);

static inline unsigned long cpu_bw_dl(struct rq *rq)
{
	return (rq->dl.running_bw * SCHED_CAPACITY_SCALE) >> BW_SHIFT;
}

static inline unsigned long cpu_util_dl(struct rq *rq)
{
	return READ_ONCE(rq->avg_dl.util_avg);
}

static inline unsigned long cpu_util_cfs(struct rq *rq)
{
	unsigned long util = READ_ONCE(rq->cfs.avg.util_avg);

	if (sched_feat(UTIL_EST)) {
		util = max_t(unsigned long, util,
			     READ_ONCE(rq->cfs.avg.util_est.enqueued));
	}

	return util;
}

static inline unsigned long cpu_util_rt(struct rq *rq)
{
	return READ_ONCE(rq->avg_rt.util_avg);
}
#else /* CONFIG_CPU_FREQ_GOV_SCHEDUTIL */
static inline unsigned long schedutil_cpu_util(int cpu, unsigned long util_cfs,
				 unsigned long max, enum schedutil_type type,
				 struct task_struct *p)
{
	return 0;
}
#endif /* CONFIG_CPU_FREQ_GOV_SCHEDUTIL */

#ifdef CONFIG_HAVE_SCHED_AVG_IRQ
static inline unsigned long cpu_util_irq(struct rq *rq)
{
	return rq->avg_irq.util_avg;
}

static inline
unsigned long scale_irq_capacity(unsigned long util, unsigned long irq, unsigned long max)
{
	util *= (max - irq);
	util /= max;

	return util;

}
#else
static inline unsigned long cpu_util_irq(struct rq *rq)
{
	return 0;
}

static inline
unsigned long scale_irq_capacity(unsigned long util, unsigned long irq, unsigned long max)
{
	return util;
}
#endif

#if defined(CONFIG_ENERGY_MODEL) && defined(CONFIG_CPU_FREQ_GOV_SCHEDUTIL)

#define perf_domain_span(pd) (to_cpumask(((pd)->em_pd->cpus)))

DECLARE_STATIC_KEY_FALSE(sched_energy_present);

static inline bool sched_energy_enabled(void)
{
	return static_branch_unlikely(&sched_energy_present);
}

#else /* ! (CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL) */

#define perf_domain_span(pd) NULL
static inline bool sched_energy_enabled(void) { return false; }

#endif /* CONFIG_ENERGY_MODEL && CONFIG_CPU_FREQ_GOV_SCHEDUTIL */

#ifdef CONFIG_MEMBARRIER
/*
 * The scheduler provides memory barriers required by membarrier between:
 * - prior user-space memory accesses and store to rq->membarrier_state,
 * - store to rq->membarrier_state and following user-space memory accesses.
 * In the same way it provides those guarantees around store to rq->curr.
 */
static inline void membarrier_switch_mm(struct rq *rq,
					struct mm_struct *prev_mm,
					struct mm_struct *next_mm)
{
	int membarrier_state;

	if (prev_mm == next_mm)
		return;

	membarrier_state = atomic_read(&next_mm->membarrier_state);
	if (READ_ONCE(rq->membarrier_state) == membarrier_state)
		return;

	WRITE_ONCE(rq->membarrier_state, membarrier_state);
}
#else
static inline void membarrier_switch_mm(struct rq *rq,
					struct mm_struct *prev_mm,
					struct mm_struct *next_mm)
{
}
#endif

#ifdef CONFIG_SMP
static inline bool is_per_cpu_kthread(struct task_struct *p)
{
	if (!(p->flags & PF_KTHREAD))
		return false;

	if (p->nr_cpus_allowed != 1)
		return false;

	return true;
}
#endif

void swake_up_all_locked(struct swait_queue_head *q);
void __prepare_to_swait(struct swait_queue_head *q, struct swait_queue *wait);

#ifdef CONFIG_RT_SOFTINT_OPTIMIZATION
/* Called with rcu_read_lock() held for accessing cpu_curr. */
static inline bool cpu_may_not_preemptible(int cpu)
{
	return test_tsk_thread_flag(cpu_curr(cpu), TIF_LONG_NONPREEMPTIBLE) ||
		task_may_not_preempt(cpu_curr(cpu), cpu);
}
#else
static inline bool cpu_may_not_preemptible(int cpu)
{
	return false;
}
#endif

static inline unsigned int core_sum_cfs_h_nr(struct rq *rq)
{
	unsigned int sum_cfs_h_nr;

	sum_cfs_h_nr = rq->cfs.h_nr_running;
#ifdef CONFIG_SCHED_SMT
	if (rq->is_smt)
		sum_cfs_h_nr += sibling_rq(rq)->cfs.h_nr_running;
#endif
	return sum_cfs_h_nr;
}

#ifdef CONFIG_SCHED_RTG
extern int sched_set_group_freq(unsigned int grp_id, unsigned int freq);
extern int sched_set_group_freq_update_interval(unsigned int grp_id, unsigned int interval);
extern int sched_set_preferred_cluster(unsigned int grp_id, int sched_cluster_id);
extern int sched_set_group_util_invalid_interval(unsigned int grp_id, unsigned int interval);
int sched_set_group_load_mode(struct rtg_load_mode *mode);
int sched_set_group_ed_params(struct rtg_ed_params *params);
extern int set_task_rtg_min_freq(struct task_struct *p, unsigned int freq);
extern unsigned long capacity_spare_without(int cpu, struct task_struct *p);
extern int update_preferred_cluster(struct related_thread_group *grp,
			struct task_struct *p, u32 old_load, bool from_tick);
extern struct cpumask *find_rtg_target(struct task_struct *p);
#endif
#ifdef CONFIG_SMP
unsigned long cpu_util(int cpu);
#endif

#ifdef CONFIG_SCHED_WALT
static inline int cluster_first_cpu(struct sched_cluster *cluster)
{
	return cpumask_first(&cluster->cpus);
}

extern struct list_head cluster_head;
extern struct sched_cluster *sched_cluster[NR_CPUS];

extern struct mutex policy_mutex;
extern unsigned int sched_disable_window_stats;
extern unsigned int max_possible_freq;
extern unsigned int min_max_freq;
extern unsigned int max_possible_efficiency;
extern unsigned int min_possible_efficiency;
extern unsigned int max_capacity;
extern unsigned int min_capacity;
extern unsigned int max_load_scale_factor;
extern unsigned int max_possible_capacity;
extern unsigned int min_max_possible_capacity;
extern unsigned int max_power_cost;
extern unsigned int __read_mostly sched_init_task_load_windows;
extern unsigned int sysctl_sched_restrict_cluster_spill;
extern unsigned int sched_pred_alert_load;
extern struct sched_cluster init_cluster;

static inline void walt_fixup_cum_window_demand(struct rq *rq, s64 scaled_delta)
{
	rq->cum_window_demand_scaled += scaled_delta;
	if (unlikely((s64)rq->cum_window_demand_scaled < 0))
		rq->cum_window_demand_scaled = 0;
}

/* Is frequency of two cpus synchronized with each other? */
static inline int same_freq_domain(int src_cpu, int dst_cpu)
{
	struct rq *rq = cpu_rq(src_cpu);

	if (src_cpu == dst_cpu)
		return 1;

	return cpumask_test_cpu(dst_cpu, &rq->freq_domain_cpumask);
}

extern void reset_task_stats(struct task_struct *p);

#define CPU_RESERVED	1
static inline int is_reserved(int cpu)
{
	struct rq *rq = cpu_rq(cpu);

#ifdef CONFIG_SCHED_SMT
	if (rq->is_smt)
		return rq->reserved || cpu_rq(rq->smt_sibling)->reserved;
#endif
	return rq->reserved;
}

static inline void mark_reserved(int cpu)
{
	struct rq *rq = cpu_rq(cpu);

#ifdef CONFIG_SCHED_SMT
	if (rq->is_smt)
		cpu_rq(rq->smt_sibling)->reserved = true;
#endif
	rq->reserved = true;
}

static inline void clear_reserved(int cpu)
{
	struct rq *rq = cpu_rq(cpu);

#ifdef CONFIG_SCHED_SMT
	if (rq->is_smt)
		cpu_rq(rq->smt_sibling)->reserved = false;
#endif
	rq->reserved = false;
}

static inline unsigned int cluster_max_freq(struct sched_cluster *cluster)
{
	/*
	 * Governor and thermal driver don't know the other party's mitigation
	 * voting. So struct cluster saves both and return min() for current
	 * cluster fmax.
	 */
	return cluster->max_freq;
}

static inline unsigned long cpu_util_freq_walt(int cpu)
{
	u64 util;
	unsigned long capacity = capacity_orig_of(cpu);

	if (unlikely(walt_disabled || !sysctl_sched_use_walt_cpu_util))
		return cpu_util(cpu);

#ifdef CONFIG_SCHED_SMT
	if (cpu_rq(cpu)->is_smt)
		util = cpu_rq(cpu)->prev_runnable_sum +
				cpu_rq(cpu_rq(cpu)->smt_sibling)->prev_runnable_sum;
	else
#endif
		util = cpu_rq(cpu)->prev_runnable_sum;
	util = util << SCHED_CAPACITY_SHIFT;
	util = div_u64(util, sched_ravg_window);

	return (util >= capacity) ? capacity : util;
}

static inline bool hmp_capable(void)
{
	return max_possible_capacity != min_max_possible_capacity;
}

#define SCHED_NEW_TASK_WINDOWS 5

static inline bool is_new_task(struct task_struct *p)
{
	return p->ravg.active_windows < SCHED_NEW_TASK_WINDOWS;
}

#define SCHED_HIGH_IRQ_TIMEOUT 3
static inline u64 sched_irqload(int cpu)
{
	struct rq *rq = cpu_rq(cpu);
	s64 delta;

	delta = get_jiffies_64() - rq->irqload_ts;
	/*
	 * Current context can be preempted by irq and rq->irqload_ts can be
	 * updated by irq context so that delta can be negative.
	 * But this is okay and we can safely return as this means there
	 * was recent irq occurrence.
	 */

	if (delta < SCHED_HIGH_IRQ_TIMEOUT)
		return rq->avg_irqload;
	else
		return 0;
}

static inline int walt_cpu_high_irqload(int cpu)
{
	return sched_irqload(cpu) >= sysctl_sched_cpu_high_irqload;
}
#else /* CONFIG_SCHED_WALT */
static inline void walt_fixup_cum_window_demand(struct rq *rq,
						s64 scaled_delta) { }

static inline int same_freq_domain(int src_cpu, int dst_cpu)
{
	return 1;
}

static inline int is_reserved(int cpu)
{
	return 0;
}

static inline int mark_reserved(int cpu)
{
	return 0;
}

static inline void clear_reserved(int cpu) { }

static inline unsigned long cpu_util_freq_walt(int cpu)
{
	return 0;
}

static inline bool hmp_capable(void)
{
	return false;
}

static inline bool is_new_task(struct task_struct *p __maybe_unused)
{
	return false;
}

static inline u64 sched_irqload(int cpu)
{
	return 0;
}

#define walt_cpu_high_irqload(cpu) false
#endif /* CONFIG_SCHED_WALT */

struct sched_avg_stats {
	int nr;
	int nr_misfit;
	int nr_max;
	int nr_scaled;
};
#ifdef CONFIG_SCHED_RUNNING_AVG
extern void sched_get_nr_running_avg(struct sched_avg_stats *stats);
#else
static inline void sched_get_nr_running_avg(struct sched_avg_stats *stats) { }
#endif

#ifdef CONFIG_CPU_ISOLATION_OPT
extern int group_balance_cpu_not_isolated(struct sched_group *sg);
#else
static inline int group_balance_cpu_not_isolated(struct sched_group *sg)
{
	return group_balance_cpu(sg);
}
#endif /* CONFIG_CPU_ISOLATION_OPT */

#ifdef CONFIG_HISI_EAS_SCHED
extern unsigned int capacity_margin;
extern unsigned int sd_capacity_margin;
extern unsigned int up_migration_util_filter;
extern unsigned int task_boost_limit;
extern struct reciprocal_value spc_rdiv;

unsigned long uclamp_task_util(struct task_struct *p);
void pre_init_entity_util_avg(struct task_struct *p);
bool task_fits_max(struct task_struct *p, int cpu);
unsigned long task_util(struct task_struct *p);
unsigned int cpu_capacity_margin(int cpu);
bool task_fits(struct task_struct *p, int cpu, unsigned long capacity);
int find_energy_efficient_cpu(struct task_struct *p, int prev_cpu, int sync);
int find_max_idle_cpu(struct task_struct *p);
bool is_core_idle(int cpu);

static inline unsigned int freq_to_util(unsigned int cpu, unsigned int freq)
{
	return arch_scale_cpu_capacity(cpu) *
		(unsigned long)freq / cpu_rq(cpu)->cluster->max_freq;
}

/*
 * Note that util_to_freq(i, freq_to_util(i, *freq*)) is lower than *freq*.
 * That's ok since we use CPUFREQ_RELATION_L in __cpufreq_driver_target().
 */
static inline unsigned int util_to_freq(unsigned int cpu, unsigned int util)
{
	return cpu_rq(cpu)->cluster->max_freq *
		(unsigned long)util / arch_scale_cpu_capacity(cpu);
}

static inline bool is_ui_thread(struct task_struct *p)
{
#ifdef CONFIG_SCHED_DYNAMIC_PRIO
	return (p && (p->dyn_prio.normal_vip_prio == 5));
#else
	return false;
#endif
}
#endif

void check_runnable_rt(struct rq *rq);

#ifdef CONFIG_RT_CAS
void push_rt_tasks(struct rq *);
extern int sched_rr_exec_timeslice_ms;
#endif

#ifdef CONFIG_SCHED_DYNAMIC_PRIO
int sched_setattr_directly(struct task_struct *p, const struct sched_attr *attr);
#endif

#ifdef CONFIG_RT_ENERGY_EFFICIENT_SUPPORT
static inline bool is_heavy_rt(struct task_struct *p)
{
	return rt_task(p) && test_tsk_thread_flag(p, TIF_EXPECTED_HEAVY);
}
#else
static inline bool is_heavy_rt(struct task_struct *p) { return false; }
#endif

/* bits in struct sugov_cpu flags field */
enum {
	WALT_WINDOW_ROLLOVER            = (1 << 0),  /* 1 */
	INTER_CLUSTER_MIGRATION_SRC     = (1 << 1),  /* 2 */
	INTER_CLUSTER_MIGRATION_DST     = (1 << 2),  /* 4 */
	ADD_TOP_TASK                    = (1 << 3),  /* 8 */
	ADD_ED_TASK                     = (1 << 4),  /* 16 */
	CLEAR_ED_TASK                   = (1 << 5),  /* 32 */
	POLICY_MIN_RESTORE              = (1 << 6),  /* 64 */
	FORCE_UPDATE                    = (1 << 7),  /* 128 */
	TASK_PRED_LOAD_CHANGE           = (1 << 8),  /* 256 */
	PRED_LOAD_WINDOW_ROLLOVER       = (1 << 9),  /* 512 */
	PRED_LOAD_ENQUEUE               = (1 << 10), /* 1024 */
	SET_MIN_UTIL                    = (1 << 11), /* 2048 */
	ENQUEUE_MIN_UTIL                = (1 << 12), /* 4096 */
	FRAME_UPDATE                    = (1 << 13), /* 8192 */
#ifdef CONFIG_MIPS_LOAD_TRACKING
	MIPS_UPDATE                     = (1 << 14), /* 16384 */
	MIPS_MIGRATE                    = (1 << 15), /* 32768 */
#endif
	VIP_TASK_PRED_LOAD_CHANGE       = (1 << 16),
};

#ifdef CONFIG_CPU_FREQ_GOV_SCHEDUTIL_OPT
extern void sched_set_io_is_busy(int val);
extern void sugov_mark_util_change(int cpu, unsigned int flags);
extern void sugov_check_freq_update(int cpu);
extern unsigned long freq_policy_util(int cpu);
extern bool sugov_iowait_boost_pending(int cpu);
#else
static inline void sched_set_io_is_busy(int val) {}
static inline void sugov_mark_util_change(int cpu, unsigned int flags) {}
static inline void sugov_check_freq_update(int cpu) {}
static inline unsigned long freq_policy_util(int cpu) { return 0; }
static inline bool sugov_iowait_boost_pending(int cpu) { return false; }
#endif
#ifdef CONFIG_SCHED_RUNNING_TASK_ROTATION
unsigned int sched_get_cpus_real_big_tasks(const struct cpumask *cpus);
#endif

#ifdef CONFIG_SCHED_CFS_WORKAROUND
void detect_cfs_starving(struct rq *rq);
#endif

#define RT_HARD_EXCEEDED 1 /* hard rt bandwidth, no runtime anymore */

#ifdef CONFIG_RT_SOFT_BANDWIDTH
#define RT_SOFT_EXCEEDED 2 /* soft rt bandwidth, downgrade to cfs */

#define MAX_MISSION_NUM 10 /* mission num restriction in irq */

enum rt_rq_thrt_status {
	TASK_WAITING_FOR_DOWNGRADE = 1,
	TASK_DOWNGRADING,
	TASK_DOWNGRADED,
	TASK_WAITING_FOR_RECOVER,
	TASK_RECOVERING = 5,
	TASK_RECOVERED,
	TASK_NORMAL_RUNNING
};

extern int sched_setattr_naked(struct task_struct *p, const struct sched_attr *attr);

static inline bool in_throttle_handle_context()
{
	return test_tsk_thread_flag(current, TIF_RES_RT_SWITCHING) && in_interrupt();
}
#endif

#ifndef CONFIG_HW_VIP_THREAD
static inline bool test_task_vip(struct task_struct *p) { return false; }
#endif
