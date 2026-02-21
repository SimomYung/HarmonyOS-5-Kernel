/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Dec 26 16:38:46 2018
 */
#ifndef LIB_PERF_H
#define LIB_PERF_H

#include <stdbool.h>
#include <stdint.h>
#include <libhmsync/raw_mutex.h>

#include <hmkernel/capability.h>
#include <hmkernel/const.h>
#include <hmkernel/perf_event.h>

#include <hmasm/types.h>
#include <lib/dlist.h>

#define BIT(nr)	(1UL << (nr))

#ifdef CONFIG_AARCH64_PMUV3
#define PERF_MAX_COUNTER_NUM 32U
#else
#define PERF_MAX_COUNTER_NUM 7U
#endif

enum perf_type_id {
	PERF_TYPE_HARDWARE_IDX      = 0, /* hardware counters: e.g. for PMUv3: 1 cycle + 6 event. */
	PERF_TYPE_SOFTWARE_IDX      = 1, /* 1 software context-switch counter now. */
	PERF_TYPE_MAXTYPE_IDX       = 2,
	PERF_TYPE_CONTINUE_IDX      = 3, /* not reset counters, continue to count */
};

enum perf_type {
	PERF_TYPE_HARDWARE          = 1 << PERF_TYPE_HARDWARE_IDX,
	PERF_TYPE_SOFTWARE          = 1 << PERF_TYPE_SOFTWARE_IDX,
	PERF_TYPE_ALL               = (1 << PERF_TYPE_MAXTYPE_IDX) - 1,
	PERF_TYPE_CONTINUE          = 1 << PERF_TYPE_CONTINUE_IDX,
};

enum perf_hw_event_id {
	/* Common hardware events. */
	HM_HW_CPU_CYCLES                = 0,
	HM_HW_INSTRUCTIONS              = 1,
	HM_HW_CACHE_REFERENCES          = 2,
	HM_HW_CACHE_MISSES              = 3,
	HM_HW_BRANCH_INSTRUCTIONS       = 4,
	HM_HW_BRANCH_MISSES             = 5,
	HM_HW_BUS_CYCLES                = 6,
	HM_HW_STALLED_CYCLES_FRONTEND   = 7,
	HM_HW_STALLED_CYCLES_BACKEND    = 8,
	HM_HW_REF_CPU_CYCLES            = 9,
	HM_HW_PMU_MAX                   = 10,

	HM_HW_BREAK_POINT               = 20,
	HM_HW_DEBUG_MAX                 = 25,

	HM_HW_DSU_CYCLES                = 100,
	HM_HW_DSU_BUS_ACCESS            = 101,
	HM_HW_DSU_MEMORY_ERROR          = 102,
	HM_HW_DSU_BUS_CYCLES            = 103,
	HM_HW_DSU_L3D_CACHE_ALLOCATE    = 104,
	HM_HW_DSU_L3D_CACHE_REFILLE     = 105,
	HM_HW_DSU_L3D_CACHE             = 106,
	HM_HW_DSU_L3D_CACHE_WB          = 107,
	HM_HW_DSU_MAX,
};

enum perf_hw_event_bit {
	/* Common hardware events. */
	HM_HW_CPU_CYCLES_BIT                = __U(1) << HM_HW_CPU_CYCLES,
	HM_HW_INSTRUCTIONS_BIT              = __U(1) << HM_HW_INSTRUCTIONS,
	HM_HW_CACHE_REFERENCES_BIT          = __U(1) << HM_HW_CACHE_REFERENCES,
	HM_HW_CACHE_MISSES_BIT              = __U(1) << HM_HW_CACHE_MISSES,
	HM_HW_BRANCH_INSTRUCTIONS_BIT       = __U(1) << HM_HW_BRANCH_INSTRUCTIONS,
	HM_HW_BRANCH_MISSES_BIT             = __U(1) << HM_HW_BRANCH_MISSES,
	HM_HW_BUS_CYCLES_BIT                = __U(1) << HM_HW_BUS_CYCLES,
	HM_HW_STALLED_CYCLES_FRONTEND_BIT   = __U(1) << HM_HW_STALLED_CYCLES_FRONTEND,
	HM_HW_STALLED_CYCLES_BACKEND_BIT    = __U(1) << HM_HW_STALLED_CYCLES_BACKEND,
	HM_HW_REF_CPU_CYCLES_BIT            = __U(1) << HM_HW_REF_CPU_CYCLES,
	HM_HW_PMU_ALLEVENT_BIT              = (__U(1) << HM_HW_PMU_MAX) - __U(1),
};

static const char *perf_type_names[] __attribute__((used)) = {
	"PERF_TYPE_HARDWARE",
	"PERF_TYPE_SOFTWARE",
	"PERF_TYPE_ALL",
};

/*
 * Special "software" events provided by the kernel,
 * Only Context Switch counting supported now!
 */
enum perf_sw_event_id {
	HM_SW_CONTEXT_SWITCHES		= 0,
	HM_SW_MAX, /* non-ABI */
};

enum perf_state {
	PERF_STATE_STOPPED = 0,
	PERF_STATE_RUNNING = 1,
};

enum perf_attach_type {
	PERF_ATTACH_THREAD = 0,
	PERF_ATTACH_CPU    = 1,
	PERF_ATTACH_MAX    = 2,
};

enum perf_monitor_type {
	PERF_MONITOR_PMU = 0,
	PERF_MONITOR_DSU = 1,
	PERF_MONITOR_MAX = 2,
};

enum {
	HW_BREAKPOINT_LEN_1 = 0x1,
	HW_BREAKPOINT_LEN_2 = 0x2,
	HW_BREAKPOINT_LEN_4 = 0x4,
	HW_BREAKPOINT_LEN_8 = 0x8,
};

enum {
	HW_BREAKPOINT_DEFAULT_VAL = 0x0,
	HW_BREAKPOINT_LEN_1_VAL = 0x01,
	HW_BREAKPOINT_LEN_2_VAL = 0x03,
	HW_BREAKPOINT_LEN_4_VAL = 0x0f,
	HW_BREAKPOINT_LEN_8_VAL = 0xff,
};

enum {
	HW_BREAKPOINT_EMPTY	= 0x0,
	HW_BREAKPOINT_R		= 0x1,
	HW_BREAKPOINT_W		= 0x2,
	HW_BREAKPOINT_RW	= HW_BREAKPOINT_R | HW_BREAKPOINT_W,
};

#define PERF_EVENT_NAME_MAX_SIZE 128U

struct event_definition {
	unsigned char type;
	unsigned int config;
	const char *symbol;
	unsigned int symlen;
};

enum perf_pmu_config_cmd {
	PERF_PMU_CONFIG_INVALID = 0,
	PERF_PMU_CONFIG_USER_ENABLE,
	PERF_PMU_CONFIG_USER_DISABLE,
	PERF_PMU_CONFIG_EVTYPE_ACQUIRE,
	PERF_PMU_CONFIG_EVTYPE_RELEASE,
	PERF_PMU_CONFIG_EVTCNT_GET,
};

/*
 * PERF event definition string helper.
 */
#define chw(x) .type = ((unsigned char)PERF_TYPE_HARDWARE - (unsigned char)1), .config = (unsigned int)HM_HW_##x
#define csw(x) .type = ((unsigned char)PERF_TYPE_SOFTWARE - (unsigned char)1), .config = (unsigned int)HM_SW_##x
#define HW_OP_UNSUPPORTED		0xFFFF

static struct event_definition event_symbols[] __attribute__((used)) = {
	{ chw(CPU_CYCLES),          "cpu-cycles",          10  },
	{ chw(INSTRUCTIONS),        "instructions",        12  },
	{ chw(CACHE_REFERENCES),    "cache-references",    16  },
	{ chw(CACHE_MISSES),        "cache-misses",        12  },
	{ chw(BRANCH_INSTRUCTIONS), "branch-instructions", 19  },
	{ chw(BRANCH_MISSES),       "branch-misses",       13  },
	{ chw(BUS_CYCLES),          "bus-cycles",          10  },

	{ csw(CONTEXT_SWITCHES),    "context-switches",    16  },

};

static struct event_definition dsu_event_symbols[] __attribute__((used)) = {
	{ chw(DSU_CYCLES),          "dsu-cycles",          10  },
	{ chw(DSU_BUS_ACCESS),      "dsu-bus-access",      14  },
	{ chw(DSU_MEMORY_ERROR),    "dsu-memory-error",    16  },
	{ chw(DSU_BUS_CYCLES),      "dsu-bus-cycles",      14  },
	{ chw(DSU_L3D_CACHE_ALLOCATE), "dsu-l3d-cache-allocate",  22  },
	{ chw(DSU_L3D_CACHE_REFILLE),  "dsu-l3d-cache-refill",    20  },
	{ chw(DSU_L3D_CACHE),          "dsu-l3d-cache",           13  },
	{ chw(DSU_L3D_CACHE_WB),       "dsu-l3d-cache-wb",        16  },
};

static struct event_definition debug_event_symbols[] __attribute__((used)) = {
	{ chw(BREAK_POINT),          "mem:<addr>[/len][:access]",          25  },
};

/* usr app parameter definition. */
typedef struct perf_evset_attr {
	/* event type: software or hardware */
	unsigned int type;
	/* event select, use perf_hw_event_bit to select event to monitor */
	unsigned int eventbits;

	/* perf context attach type */
	enum perf_attach_type attach_type;

	/* perf context monitor type */
	enum perf_monitor_type monitor_type;
} perf_evset_attr_t;

/*
 * struct generic_pmu - generic performance monitoring unit
 *
 * Abstract API.
 * (1) hm_perf_start call hw/sw_pmu_init to init PMU hw counters and initialize all counters to 0.
 * (2) config_events: config hw with default attributes,
 * 	now the default measured events are:
 * 	(cycle, instruction, cache reference/miss, branch instructions/miss, bus cycles)
 * (3) pmu_start: start countering all configured events
 * (4) pmu_stop: stop countering
 * (5) read/write_counters: read/write all configured cycle and events counters' value.
 * (6) xxx_context: batch handling(sync/restore) PMU context regiters.
 * (7) reset: reset all counters to 0.
 * (8) perf_disable/enable:  temporarily disable countering but did not reset counter
 * 	values to 0, and perf_enable would continue counting beginning with previous value.
 * (9) handle_irq: reserved for future handling pmu overflow status.
 * (10) profilling_report: read out all counters' value, and print it out with a report format.
 */
struct generic_pmu {
	char *name;
	bool initialized;
	int sample_period;
	/* events bit to select event */
	unsigned int events;
	/* state control. */
	int state;
	enum perf_attach_type attach_type;
	enum perf_monitor_type monitor_type;
	struct raw_mutex mtx;

	/*
	 * Try and initialize the event for this PMU.
	 * Should return E_HM_NOOBJ when the @event doesn't match this PMU.
	 */
	int (*config_events)(const struct generic_pmu *pmu);

	/* trigger start and stop profilling. */
	void (*pmu_start)(const struct generic_pmu *pmu);
	void (*pmu_stop)(struct generic_pmu *pmu);

	/* access counters. */
	int (*read_counters)(const struct generic_pmu *pmu, unsigned int *stat_array, unsigned int array_len);
	void (*write_counters)(const struct generic_pmu *pmu);

	/* reset all PMU context and status. */
	void (*reset)(const struct generic_pmu *pmu);

	/* swevent related config. */
	void (*pmu_enable)(const struct generic_pmu *pmu); /* optional */
	void (*pmu_disable)(const struct generic_pmu *pmu); /* optional */

	/* generate profilling report without fileio */
	int (*profilling_report)(const struct generic_pmu *pmu, unsigned int *stat_array, unsigned int array_len);
};

int hm_perf_start(const struct perf_evset_attr *attr);
int hm_perf_stop(unsigned int *stat_array, unsigned int array_len);
int hm_perf_profilling_report(unsigned int *stat_array, unsigned int array_len);

int perf_start(enum perf_attach_type attach_type,
	       enum perf_monitor_type monitor_type,
	       struct __perf_event_config_s *evt_config,
	       unsigned int cpuid, cref_t *pperf_cref);
int perf_stop(enum perf_attach_type attach_type,
	      enum perf_monitor_type monitor_type,
	      cref_t perf_cref);
int perf_read(enum perf_monitor_type monitor_type,
	      cref_t perf_cref, struct __perf_event_ret_s *ret);
#ifdef CONFIG_WCET_PLUGIN
void hm_perf_enable(void);
void hm_perf_disable(void);
void hm_perf_read_counters(unsigned int *stat_array, unsigned int array_len);
#else
static inline void
hm_perf_enable(void)
{
}
static inline void
hm_perf_disable(void)
{
}
static inline void
hm_perf_read_counters(unsigned int *stat_array, unsigned int array_len)
{
}
#endif

struct perf_proc_mmap_segment {
	struct dlist_node node;
	int pid;
	uint32_t cnode_idx;

	uint32_t flags;
#define PERF_PROC_MMAP_F_READ	(1 << 0)
#define PERF_PROC_MMAP_F_WRITE	(1 << 1)
#define PERF_PROC_MMAP_F_EXE	(1 << 2)
#define PERF_PROC_MMAP_F_SHARE	(1 << 3)

	unsigned long base; /* base address */
	unsigned long size; /* mmap region size */
	unsigned long offset; /* mmap offset */
	char *fname; /* elf file path */
};

/* a collection of memory maps of all interested processes. */
struct perf_proc_maps {
	struct dlist_node mmap_list;
	int count;
};

int perf_collect_process_maps(struct perf_proc_maps *maps);
void perf_destroy_process_maps(struct perf_proc_maps *maps);

#endif
