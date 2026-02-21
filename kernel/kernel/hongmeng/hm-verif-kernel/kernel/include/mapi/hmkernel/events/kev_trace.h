/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: kev trace
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 24 17:47:25 2023
 */
#ifndef MAPI_HMKERNEL_EVENTS_KEV_TRACE_H
#define MAPI_HMKERNEL_EVENTS_KEV_TRACE_H

#include <hmkernel/audit.h>
#include <hmkernel/compiler.h>
#include <hmkernel/macro.h>
#include <hmkernel/tcb.h>
#include <hmkernel/sched/sched.h>
#include <lib/printf.h>
#include <hmkernel/timer/vtimekeep.h>
#include <hmkernel/strict.h>
#include <hmkernel/kernel.h>
#include <hmkernel/events/kev.h>
#include <hmkernel/events/kev_fiq_texts.h>
#include <hmkernel/events/kev_trace_struct.h>
#include <hmkernel/events/kev_trace_configurable.h>
#include <mapi/hmkernel/events/kev_trace.h>
#include <uapi/hmkernel/perf_event.h>

#include <hmasm/types.h>

struct kev_tcb_state_s {
	char name[HMCAP_THREAD_NAME_SIZE];
	enum tcb_state_e state;
	int prio;
	cref_t cref;
	int pid;
	int tid;
};

struct kev_extra_info_s {
#ifdef CONFIG_KEV_KLOG_TCB_INFO
	struct kev_tcb_info_s tcb_info;
#endif
	size_t fixed_data_size;
};
#define KEV_RB_OFFSET_MAX 0xffffffffffffffffULL
#ifdef CONFIG_DEBUG_BUILD
# define KEV_MAX_STACK_DEPTH 400
#else
# define KEV_MAX_STACK_DEPTH 50
#endif

/* Enter Kernel Platform */
#define KEV_ARCH_ARM        32
#define KEV_ARCH_AARCH64    64

/* Enter Kernel Exception Symbol */
#define KEV_KERN_DATA_ABORT  0x1
#define KEV_USER_DATA_ABORT  0X2
#define KEV_KERN_UNDEF_INST  0x3
#define KEV_USER_UNDEF_INST  0x4
#define KEV_KERN_UNIMP_EXCEP 0x5
#define KEV_USER_UNIMP_EXCEP 0x6
#define KEV_USER_INST_ABORT  0x7

struct kev_stack_entry_s {
	u64 nr;
	u64 ip[KEV_MAX_STACK_DEPTH];
};
void user_stack_unwind(struct kev_stack_entry_s *stack_entry);
void kernel_stack_unwind(struct kev_stack_entry_s *stack_entry, const struct arch_regs *regs);
void actv_stack_unwind(struct actv_s *actv, struct kev_stack_entry_s *stack_entry);
bool vaddr_accessible(struct ctrlmem_kobj_vspace_s *kobjvs, uptr_t va);

#define KEV_MACRO(...) __VA_ARGS__
#define KEV_ARG(...) __VA_ARGS__

#define KEV_STRUCT(...) __VA_ARGS__
#define KEV_ASSIGN(...)
#define KEV_PRINT(...)
#define __KEV_STRING(name, size)    name[size]
#define __KEV_ARRAY(name, size)     name[size]
#define KEV_TIMESTAMP_GEN_NEED_TIMESTAMP    __u64 timestamp
#define KEV_TIMESTAMP_GEN_NO_TIMESTAMP
#define KEV_STRUCT_GEN(name, need_timestamp, n, ...)            \
	struct kev_##name##_s {             \
		struct kev_header_s header;     \
		__MAPSTAT_DEFVAR(n, __SC_DEF, __VA_ARGS__);\
		KEV_TIMESTAMP_GEN_##need_timestamp; \
		char data[]; 	\
	} __aligned(4);

#define KEV_PROTO_GEN(n, ...) __PROTO_MAP(n, __SC_DEF, __VA_ARGS__)
#define KEV_CALL_GEN(n, ...) __MAP(n, __SC_CALL, __VA_ARGS__)

/* generate real trace functions and structures of the events */
#define DEFINE_KEV(NAME, name, args, str, ...)      \
	__DEFINE_KEV(NAME, name, NO_TIMESTAMP, KEV_MACRO(args), KEV_MACRO(str), ...)
#define __DEFINE_KEV(NAME, name, need_timestamp, args, str, ...)        \
	KEV_STRUCT_GEN(name, need_timestamp, str)           \
	extern void __kev_trace_##name(KEV_PROTO_GEN(args));
#include <hmkernel/events/kev_list.h>
#undef KEV_STRUCT
#undef KEV_ASSIGN
#undef KEV_PRINT
#undef __KEV_STRING
#undef __KEV_ARRAY
#undef KEV_TIMESTAMP_GEN_NEED_TIMESTAMP
#undef KEV_TIMESTAMP_GEN_NO_TIMESTAMP
#undef KEV_STRUCT_GEN
#undef DEFINE_KEV
#undef __DEFINE_KEV

union __kev_union_event {
#define DEFINE_KEV(NAME, name, ...) \
	__DEFINE_KEV(NAME, name, ...)
#define __DEFINE_KEV(NAME, name, ...) struct kev_##name##_s name;
#include <hmkernel/events/kev_list.h>
#undef DEFINE_KEV
#undef __DEFINE_KEV
};

#ifndef __HOST_LLT__
# define RB_SIZE ((u64)(1ULL << 15))
#else
# define RB_SIZE ((u64)(1ULL << 12))
#endif

#ifdef CONFIG_KEV_TRACING

#define DEFINE_KEV(NAME, name, args, ...)   \
	__DEFINE_KEV(NAME, name, NO_TIMESTAMP, KEV_MACRO(args), ...)
#define __DEFINE_KEV(NAME, name, need_timestamp, args, ...)     \
static inline void                          \
kev_trace_##name(KEV_PROTO_GEN(args))                   \
{                                   \
	if (IS_KEV_ENABLED(NAME)) {                 \
		__kev_trace_##name(KEV_CALL_GEN(args));         \
	}                               \
}

#include <hmkernel/events/kev_list.h>

#undef DEFINE_KEV
#undef __DEFINE_KEV

extern struct kev_stack_entry_s *kev_stack_entry_of(unsigned int cpuid);
extern int kev_set_water(unsigned int index, unsigned long long water);
extern int kev_set_blockinfo(unsigned int type, cref_t vspace_cref, void *uaddr);
extern int kev_user_read(struct CNode_s *caller_cnode, const void __user *args, size_t args_size,
			 void __user *ret, size_t ret_size);
extern void __kev_set_output_enable(enum kev_type type, unsigned int enable);
extern int kev_set_output_enable(const char *event_name, unsigned int enable);
extern int kev_set_event_filter(const char *event_name, const char *filter);
extern int kev_read_event_number(unsigned int __user *event_number);
extern int kev_read_event_conf(unsigned int event_type,
			 struct __sysctrl_kev_config __user *ret_event_config);
extern int kev_clear_ringbuffer(struct CNode_s *caller_cnode, unsigned int kev_type);

extern struct ring_buffer_s *ring_buffer_of_event_type(enum kev_type type);
extern void kev_for_each(struct ring_buffer_s *rb, u64 start_offset, u64 *ret_last_read_offset,
			 int (*op)(struct kev_s *, void *arg), void *arg);
extern bool is_output_enable(enum kev_type type);
extern unsigned int kev_get_klog_level(void);
extern int kev_copy_raw_data(enum kev_type type, void *buff, u64 start_offset, u32 len);

static inline void kev_unblock_set_waker(const char *name)
{
	arch_current_cpu_local()->waker = name;
}

static inline const char *kev_unblock_curr_waker(void)
{
	return arch_current_cpu_local()->waker;
}

static inline const char *kev_unblock_waker_push(const char *new_waker)
{
	const char *backup = NULL;

	if (KCONFIG_IS_DEFINED(CONFIG_KEV_UNBLOCK_RECORD_WAKER)) {
		backup = kev_unblock_curr_waker();
		kev_unblock_set_waker(new_waker);
	}

	return backup;
}

static inline void kev_unblock_waker_pop(const char *backup)
{
	if (KCONFIG_IS_DEFINED(CONFIG_KEV_UNBLOCK_RECORD_WAKER)) {
		kev_unblock_set_waker(backup);
	}
}

struct kev_unblock_waker_scope_ctx {
	const char *backup;
	bool enter_once;
};

#define kev_unblock_waker_scope(__new_waker)                    \
	for (struct kev_unblock_waker_scope_ctx __waker_scope_ctx = {       \
			.backup = kev_unblock_waker_push(__new_waker),      \
			.enter_once = true                  \
			};                          \
		__waker_scope_ctx.enter_once;                  \
		kev_unblock_waker_pop(__waker_scope_ctx.backup),           \
		__waker_scope_ctx.enter_once = false)

extern vatomic32_t *kev_sequence_ptr_of(void);

#else

#define DEFINE_KEV(NAME, name, args, ...)   \
	__DEFINE_KEV(NAME, name, NO_TIMESTAMP, KEV_MACRO(args), ...)
#define __DEFINE_KEV(NAME, name, need_timestamp, args, ...)     \
static inline void                  \
kev_trace_##name(KEV_PROTO_GEN(args))           \
{                           \
	UNUSED(KEV_CALL_GEN(args));         \
}

#include <hmkernel/events/kev_list.h>

#undef KEV_ARG
#undef KEV_MACRO

#undef DEFINE_KEV
#undef __DEFINE_KEV

static inline struct kev_stack_entry_s *kev_stack_entry_of(unsigned int cpuid)
{
	UNUSED(cpuid);
	return NULL;
}

static inline int kev_set_blockinfo(unsigned int type, cref_t vspace_cref, void *uaddr)
{
	UNUSED(type, vspace_cref, uaddr);
	return E_HM_NOSYS;
}

static inline int kev_set_water(unsigned int index, unsigned long long water)
{
	UNUSED(index, water);
	return E_HM_NOSYS;
}

static inline int kev_user_read(struct CNode_s *caller_cnode,
				const void __user *args, size_t args_size,void __user *ret, size_t ret_size)
{
	UNUSED(caller_cnode, args, args_size, ret, ret_size);
	return E_HM_NOSYS;
}

static inline int kev_clear_ringbuffer(struct CNode_s *caller_cnode, unsigned int kev_type)
{
	UNUSED(caller_cnode, kev_type);
	return E_HM_NOSYS;
}

static inline int kev_set_output_enable(const char *event_name, unsigned int enable)
{
	UNUSED(event_name, enable);
	return E_HM_NOSYS;
}

static inline int kev_set_event_filter(const char *event_name, const char *filter)
{
	UNUSED(event_name, filter);
	return E_HM_NOSYS;
}

static inline int kev_read_event_number(unsigned int __user *event_number)
{
	UNUSED(event_number);
	return E_HM_NOSYS;
}

static inline int kev_read_event_conf(unsigned int event_type,
					 struct __sysctrl_kev_config __user *ret_event_config)
{
	UNUSED(event_type, ret_event_config);
	return E_HM_NOSYS;
}

static inline const char *kev_unblock_curr_waker(void)
{
	return NULL;
}

static inline void kev_unblock_set_waker(const char *waker)
{
	UNUSED(waker);
}

static inline const char *kev_unblock_waker_push(const char *new_waker)
{
	return NULL;
}

static inline void kev_unblock_waker_pop(const char *backup)
{
	UNUSED(backup);
}

static inline bool is_output_enable(enum kev_type type)
{
	UNUSED(type);
	return false;
}

#define kev_unblock_waker_scope(unused)                     \
	for (bool __enter_once = true; __enter_once; __enter_once = false)

static inline vatomic32_t *kev_sequence_ptr_of(void)
{
	return NULL;
}

#endif /* CONFIG_KEV_TRACING */

#ifdef __HOST_LLT__
extern void kev_last_event_of(unsigned int buf_idx, struct kev_s *event, size_t read_buf_sz);
extern void kev_iterate(int (*op)(struct kev_s *, void *arg), void *arg);
extern int llt_kev_init(void);
extern void llt_kev_reset(void);
extern void llt_kev_clean(void);
extern int llt_kev_set_resv_mem_magic(unsigned int id, u32 magic);
extern int llt_kev_set_resv_mem_ring_count(unsigned int id, u32 ring_count);
extern int llt_kev_set_resv_mem_ring_sizes(unsigned int id, u32 ring_count, u64 ring_sizes[]);
extern int llt_kev_set_resv_mem_lock_state(unsigned int rb_idx, u32 state);
extern int llt_kev_read_event_conf_check(unsigned int event_type,
					 const struct __sysctrl_kev_config *config_entry);
extern int llt_is_output_enable(unsigned int type);
extern u64 llt_kev_trace_set_water(unsigned int rb_idx, unsigned long long water);
extern void llt_kev_set_rbuffer_size(unsigned int rb_idx, u64 size);
extern void llt_kev_set_rbuffer_mode(unsigned int rb_idx, u32 mode);
void llt_kev_dump_rb_klog_info(unsigned int rb_idx);
char *llt_kev_klog_dump_area(void);
extern void llt_kev_read_all_data(unsigned int rb_idx);
extern void llt_kev_set_fiq_sharing(bool val);
#endif /* __HOST_LLT__ */

#endif
