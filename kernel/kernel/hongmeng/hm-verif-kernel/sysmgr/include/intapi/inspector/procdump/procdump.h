/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Provide procdump interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 3 10:29:55 2020
 */
#ifndef SYSMGR_INTAPI_INSPECTOR_PROCDUMP_PROCDUMP_H
#define SYSMGR_INTAPI_INSPECTOR_PROCDUMP_PROCDUMP_H

#include <stddef.h>
#include <intapi/thread/thread_ops.h>
#include <intapi/process/process.h>
#include <libhmelf/symbol.h>
#include <hongmeng/errno.h>
#include <aapi/hmsysmgr/mgrs_stat.h>
#include <aapi/hmsysmgr/dumpstack.h>
#include <generated/sysmgr_head.h>
#include <hongmeng/version.h>
#include <fs/version.h>
#ifdef CONFIG_LOG_ENCRYPT
#include <security/elog.h>
#include <libhmelog/hmelog.h>
#endif

#define PROCDUMP_TAG_EXCEPTION		"ED"
#define PROCDUMP_TAG_HGUARD		"GD"
#define PROCDUMP_TAG_SYSRQ		"RQ"
#define PROCDUMP_TAG_BACKTRACE		"BT"
#define PROCDUMP_TAG_SNAPSHOT		"SS"
#define PROCDUMP_TAG_ABORT		"AB"
#define PROCDUMP_TAG_SIGNAL		"SD" /* dump on signals */

#define PROCDUMP_MAX_PROCESS_NAME_LEN 64

#define DUMP_REG_MEM_REGION_MAX_SIZE	40
#define DUMPSTACK_LOG_PREFIX_LEN	12

#define PROCDUMP_FLAGS_ON_EXCEPTION 1U
#define PROCDUMP_FLAGS_IN_EMERGENCY (1 << 1U)
/* do not dump base actv for non system processes */
#define PROCDUMP_FLAGS_NO_BASE_ACTV (1 << 2U)
/* stack backtrace droppable when stack is not enough */
#define PROCDUMP_FLAGS_BACKTRACE_DROPPABLE (1 << 3U)

typedef int (*dump_output_t)(void *aux_arg, const char *_log, size_t len);

struct dump_impl {
	dump_output_t output;
	struct symbol_list_s *base_process_symbol_list;
	struct process_symbol_pool_s *system_symbol_pool;
	/* allocated in procdump_process_name_get and will be freed in dump_impl_fini  */
	char *process_name_cache;
	void *aux_arg;
	char prefix[DUMPSTACK_LOG_PREFIX_LEN];
	unsigned int flags;
#ifdef CONFIG_LOG_ENCRYPT
	bool encrypt;
	struct elog_ctx *elog;
#endif
};

/*
 * arguments:
 *   @encrypt: encrpyt log msg or not, only work on CONFIG_LOG_ENCRYPT enabled.
 *   @output: log msg output function. use default function if keep NULL.
 */
void dump_impl_init(struct dump_impl *impl, bool encrypt, dump_output_t output, void* aux_arg);
void dump_impl_fini(struct dump_impl *impl);

static inline bool procdump_in_emergency(const struct dump_impl *impl)
{
	bool ret = false;
	if (impl != NULL) {
		ret = ((impl->flags & PROCDUMP_FLAGS_IN_EMERGENCY) != 0);
	}
	return ret;
}

static inline bool procdump_no_base_actv(const struct dump_impl *impl)
{
	bool ret = false;
	if (impl != NULL) {
		ret = ((impl->flags & PROCDUMP_FLAGS_NO_BASE_ACTV) != 0);
	}
	return ret;
}

static inline int procdump_dump_to_klog(void *aux_arg, const char *_log, size_t len)
{
	UNUSED(aux_arg);
	BUG_ON(_log == NULL);
	return sysfast_syslog((unsigned int)HMLOG_INFO, _log, len, NULL);
}

int procdump_check_dump_impl(const struct dump_impl *impl);

/* add some space on each line of header */
#define HEADER_INDENT_PADDING	""
/* add some space on each line of contents */
#define CONTENT_INDENT_PADDING	" "

void __attribute__((format(printf, 2, 3)))
__dp_printf_noenc(const struct dump_impl *impl, const char *fmt, ...);

#ifdef CONFIG_LOG_ENCRYPT

void __attribute__((format(printf, 2, 3)))
__dp_printf_enc(const struct dump_impl *impl, const char *fmt, ...);

#define __dp_printf __dp_printf_enc
#else
#define __dp_printf __dp_printf_noenc
#endif

#define dp_printf(impl, fmt, ...)				\
	__dp_printf((impl), "%s" fmt, &(impl)->prefix[0], ##__VA_ARGS__)

#define dp_hd_printf(impl, fmt, ...)					\
	__dp_printf((impl), "%s" HEADER_INDENT_PADDING fmt,	\
		    &(impl)->prefix[0], ##__VA_ARGS__)

#define dp_ctx_printf(impl, fmt, ...)					\
	__dp_printf((impl), "%s" CONTENT_INDENT_PADDING fmt,	\
		    &(impl)->prefix[0], ##__VA_ARGS__)

#define dp_printf_noenc(impl, fmt, ...)				\
	__dp_printf_noenc((impl), "%s" fmt, &(impl)->prefix[0], ##__VA_ARGS__)

#define dp_hd_printf_noenc(impl, fmt, ...)					\
	__dp_printf_noenc((impl), "%s" HEADER_INDENT_PADDING fmt,	\
		    &(impl)->prefix[0], ##__VA_ARGS__)

#define dp_ctx_printf_noenc(impl, fmt, ...)					\
	__dp_printf_noenc((impl), "%s" CONTENT_INDENT_PADDING fmt,	\
		    &(impl)->prefix[0], ##__VA_ARGS__)

#define dump_impl_output(__impl, __text) \
	(__impl)->output((__impl)->aux_arg, __text, \
	__builtin_constant_p(__text) ? (sizeof(__text) - 1) : strlen(__text))

static inline void dump_system_version(struct dump_impl *impl)
{
	dp_printf(impl, "Release: %s  ulibs version: %d fs version: %s  actv version: %d\n\
		  mem version: %d  affinity version: %d  actv signal version: %d\n",
		  HMKERNEL_RELEASE, VERSION_ULIBS, fs_version(), VERSION_ACTV, VERSION_MEM,
		  AFFINITY_VERSION, ACTV_SIGNAL_VERSION);
}

struct thread_s;

struct thread_lock_info_s {
	uintptr_t lock_addr;
	union {
		struct {
			uint8_t lock_owner_valid	:1;
			uint8_t lock_in_ldk		:1;
			uint8_t lock_xdep		:1;
			uint8_t lock_read_list		:1;
			uint8_t lock_ldk_flags		:3;
			uint8_t reserved_flags		:1;
		} flags;
		uint8_t v;
	};
};

struct actv_node_info {
	struct thread_s *thread;
	struct arch_regs *regs;
	struct symbol_list_s *symbol_list;
	struct __arch_fpsimd_state *fpsimd;
	struct process_s *process;
	struct dump_impl *dump_impl;
	struct thread_lock_info_s *lock_info;
	unsigned int flags;
	int actv_seq;
	/* if is_base_actv is true, it will record the tcb cref;
	 otherwise, it will record actv cref */
	bool is_base_actv;
	union {
		cref_t actv_cref;
		cref_t tcb_cref;
	};
};

int procdump_dumpstack(struct actv_node_info *node);

/*
 * procdump_actvchain_flags determines the dump actions for nodes in activition chain.
 *
 * Assume an actv call chain is as follows:
 *   base_actv => actv1 => actv2 => actv3
 * then:
 *  'base' flags is for base_actv; 'current' flags is for actv3;
 *  'other' flags is for actv1 and actv2.
 * When there is only one node in actv chain, that means the base actv
 * is also current actv, then 'base' flags would be used first.
 */
struct procdump_actvchain_flags {
	unsigned int base;
	unsigned int current;
	unsigned int other;
};

/*
 * Note: currently parameter 'proc_flags' only support following flags:
 *  PROC_FLAGS_WITH_THREAD / PROC_FLAGS_WITH_TIME / PROC_FLAGS_WITH_MEM / PROC_FLAGS_WITH_THREAD_ID
 */
void procdump_dump_process_stat(pid_t pid, unsigned int flags,
				const struct dump_impl *dump_impl);

void procdump_dump_thread_info(struct thread_s *thread,
			       const struct dump_impl *impl,
			       struct thread_lock_info_s *lock_info,
			       const struct procdump_actvchain_flags *chain_flags);

#ifdef CONFIG_DUMP_EXCEPTION_ON_SIGNAL
void dump_single_process(struct process_s *process, unsigned int flags,
			 struct dump_impl *impl);
#else
static inline void dump_single_process(struct process_s *process, unsigned int flags,
				       struct dump_impl *impl)
{
	UNUSED(process, flags, impl);
}
#endif

void dump_one_process(struct process_s *process, struct dump_impl *impl,
		      bool (*thread_filter)(const struct thread_s *thread),
		      unsigned int flags, unsigned int mini_flags);

int procdump_dump_thread(struct thread_s *thread,
			 const struct procdump_actvchain_flags *chain_flags,
			 struct dump_impl *impl,
			 bool dumpcallchain);

int procdump_dump_each_process(unsigned int flags, struct dump_impl *impl,
			       bool (*filter)(const struct process_s *proc, uintptr_t arg),
			       uintptr_t filter_arg);

int procdump_dump_each_process_filter_threads(unsigned int flags, struct dump_impl *impl,
					      bool (*filter)(const struct process_s *proc, uintptr_t arg),
					      bool (*thread_filter)(const struct thread_s *thread),
					      uintptr_t filter_arg);

int procdump_dump_each_process_filter_threads_slim(unsigned int flags, struct dump_impl *impl,
					      bool (*filter)(const struct process_s *proc, uintptr_t arg),
					      bool (*thread_filter)(const struct thread_s *thread),
					      uintptr_t filter_arg);

int procdump_dump_all_processes(unsigned int flags, struct dump_impl *impl);

/*
 * Dump actv chain info of caller
 *
 * Actv chain info about how the caller runs into some actvhandler is very
 * useful on debugging. For ease of use, this interface is encapsulated with
 * no parameters and defaultly dump info as klog.
 *
 * @param   NA
 *
 * @return  Returns E_HM_OK on success.
 *
 * @note    1. Logs are saved as klog with INFO level, so printing level of
 *             kev or uart should be correctly setted;
 *          2. Only register info and call stack info of each actv node would
 *             be dumped, just for ease of use;
 *          3. This interface depends on the memory or file system function,
 *             user should avoid nested invoking;
 *          4. It is recommended that this interface only be called on entry of
 *             some actv handler.
 */
int procdump_dump_selfchain(void);
void procdump_fpsimd_mcache_init(void);

void procdump_set_regs_mem_region(long num_left, long num_right);
int procdump_regs_mem_region_of(long *num_left, long *num_right);

struct thread_procdump_ops_s {
	void (*dump_append_info)(struct thread_s *thread, const struct dump_impl *impl);
	int (*dump_actvchain)(struct thread_s *thread, const struct procdump_actvchain_flags *chain_flags,
			      struct dump_impl *impl, struct thread_lock_info_s *lock_info, bool dumpcallchain);
	void (*dump_abort)(struct thread_s *thread);
};

extern const thread_procdump_ops_t klt_procdump_ops;
void procdump_ops_dump_abort(struct thread_s *thread);
int procdump_ops_dump_actvchain(struct thread_s *thread,
				const struct procdump_actvchain_flags *chain_flags,
				struct dump_impl *impl,
				struct thread_lock_info_s *lock_info,
				bool dumpcallchain);
void procdump_dump_when_oom(struct process_s *process, unsigned int flags,
			    struct dump_impl *impl);

char *procdump_process_name_get(struct dump_impl *impl, struct process_s *process);
void procdump_process_name_put(struct dump_impl *impl, struct process_s *process, char *pname);
struct process_s *procdump_acquire_process(const struct dump_impl *impl, uint32_t cnode_idx);
void procdump_put_process(const struct dump_impl *impl, struct process_s *process);
struct vspace_s *procdump_acquire_vspace(const struct dump_impl *impl, struct process_s *process);
void procdump_put_vspace(const struct dump_impl *impl, struct vspace_s *vspace);
ssize_t procdump_read_ulong_from_vspace(const struct dump_impl *impl, struct vspace_s *src_vspace,
		uintptr_t dst_addr, uintptr_t src_addr, unsigned int addr_model, bool force_check);
ssize_t procdump_read_from_vspace(const struct dump_impl *impl, struct vspace_s *src_vspace,
		uintptr_t dst_addr, size_t dst_size,
		uintptr_t src_addr, size_t src_size);
#endif
