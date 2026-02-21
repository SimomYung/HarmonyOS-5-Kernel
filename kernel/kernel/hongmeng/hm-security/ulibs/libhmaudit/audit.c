/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: APIs of audit system used by system services
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 17 12:23:44 2020
 */
#include <ctype.h>
#include <stdarg.h>
#include <fcntl.h>
#include <hongmeng/types.h>
#include <hongmeng/errno.h>
#include <hongmeng/macro.h>

#include <libhmlog/hmlog.h>
#include <hmasm/types.h>
#include <hongmeng/panic.h>
#include <hmkernel/capability.h>
#include <libhmaudit/audit.h>
#include <libhmaudit/rule.h>
#include <libhmaudit/audit_log.h>
#include <libhmtrace/hmtrace.h>
#include <libhmtrace/ring_buffer/ring_buffer.h>
#include <libhmactv/actv.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libstrict/strict.h>
#include <libsysif/utils.h>
#include <libsysif/auditmgr/api.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <libhmsrv_audit/audit.h>
#include <libhmsrv_audit/audit_syscall_nums.h>
#include <libhmsync/atomic.h>
#include <libhmutils/hmutils.h>
#include <libhwsecurec/securec.h>

#include <libsec/log.h>
#include <libsec/cred.h>
#include <libsec/chk_cred.h>
#include <libsec/audit.h>

#include <libhwsecurec/securec.h>

#define PROCTITLE_HEAD		" proctitle="
#define PROCTITLE_HEAD_SIZE	(sizeof(PROCTITLE_HEAD) - 1UL)
#define PROCTITLE_QUOTE_SIZE	2
#define TTY_NONE		"(none)"

#ifdef __HOST_LLT__
#define STATIC
#define inline
#else
#define STATIC static
#endif

/* server number defined in server_audit_num.h */
__thread enum __server_audit_numbers g_server_num = __SERVER_INVAL;
__thread struct audit_ctx g_audit_ctx;
__thread struct audit_watch_del_ctx g_audit_watch_del_ctx;

STATIC int audit_ctx_fill_timestamp_and_serial(struct audit_ctx *ctx);
STATIC int audit_log_generate_prepare(struct audit_ctx *ctx);
STATIC raw_atomic_int_t g_audit_serial_num = {0};
STATIC uint32_t g_audit_serial_header = 0;

STATIC audit_get_cmdline_hook_t g_get_cmdline_hook_func = NULL;

STATIC struct hmtrace_event audit_event = {
	.event_entry = HMTRACE_EVENT_ENTRY_INIT(audit_event.event_entry),
	.name = "audit_event",
	.efmt = NULL,
};

STATIC uint32_t g_audit_type[] = {
	HM_AUDIT_SYSCALL,
	HM_AUDIT_PATH,
	HM_AUDIT_FD_PAIR,
	HM_AUDIT_SOCKADDR,
	HM_AUDIT_PROCTITLE,
	HM_AUDIT_EOE,
};

STATIC struct hmtrace_class audit_class_uev = {
	/* name must be "audit_trace" to match client */
	.name = AUDIT_TRACE_CLASS_NAME,
	.register_event = NULL,
	.unregister_event = NULL,
	.get_available_events_list = NULL,
	.class_entry = HMTRACE_CLASS_ENTRY_INIT(audit_class_uev.class_entry),
	.enable_event = NULL,
	.disable_event = NULL,
	.prepare_for_show = NULL,
	.event_table.uev_table = NULL
};

STATIC rref_t auditmgr_rref;

STATIC int auditmgr_rref_get(void)
{
	int err;
	rref_t rref;

	err = hm_path_acquire(AUDITMGR_PATH, &rref);
	if (err != E_HM_OK) {
		hm_error("failed to acquire auditmgr path, err=%s\n", hmstrerror(err));
		return err;
	}
	auditmgr_rref = rref;

	return E_HM_OK;
}

STATIC raw_atomic_int_t shm_initialized = RAW_ATOMIC_VAL_INIT(0);
STATIC raw_atomic_int_t rb_initialized = RAW_ATOMIC_VAL_INIT(0);
STATIC struct raw_mutex g_shm_lock = RAW_MUTEX_INITIALIZER;
STATIC struct raw_mutex g_rb_lock = RAW_MUTEX_INITIALIZER;
typedef int (*init_func)(void);
STATIC inline int init_done_check(int ret, raw_atomic_int_t *initialized)
{
	if (ret == E_HM_OK) {
		(void)raw_atomic_int_set(initialized, 1);
	}
	return ret;
}

STATIC inline int init_template(raw_atomic_int_t *initialized,
				struct raw_mutex *lock, const init_func func)
{
	if (raw_atomic_int_read(initialized) != 0) {
		return E_HM_OK;
	}
	RAW_MUTEX_GUARD(_, lock);
	if (raw_atomic_int_read(initialized) != 0) {
		return E_HM_OK;
	}
	return init_done_check(func(), initialized);
}

STATIC int shm_rule_init(void)
{
	return audit_shared_rule_init(false);
}

STATIC inline int init_shm(void)
{
	return init_template(&shm_initialized, &g_shm_lock, shm_rule_init);
}

STATIC int audit_init_ringbuffer(void)
{
	int pid;
	int err;
	struct __actvret_adcall_msg_register ret = {0};

	pid = hm_getpid();
	if (pid < 0) {
		hm_error("get pid failed, err=%d\n", pid);
		return pid;
	}

	hm_trace_init(pid);

	err = audit_trace_init();
	if(err != E_HM_OK) {
		hm_error("audit trace init failed\n");
		return err;
	}

	err = auditmgr_rref_get();
	if (err != E_HM_OK) {
		hm_error("get auditmgr rref failed\n");
		return err;
	}
	err = actvcapcall_adcall_msg_register(auditmgr_rref, pid, &ret);
	if (err != E_HM_OK) {
		hm_error("register self to auditmgr failed\n");
		return err;
	}

	err = audit_init_serial(ret.serial_header);
	if (err != E_HM_OK) {
		hm_error("init serial failed\n");
		return err;
	}
	return err;
}

STATIC inline int init_ringbuffer(void)
{
	return init_template(&rb_initialized, &g_rb_lock, audit_init_ringbuffer);
}

int audit_init_serial(uint32_t serial_header)
{
	int ret = E_HM_OK;
	if (g_audit_serial_header != 0U) {
		return ret;
	}
	if (serial_header <= AUDIT_SYSMGR_SERIAL_HEADER) {
		g_audit_serial_header = serial_header << AUDIT_SERIAL_HEADER_OFFSET;
		raw_atomic_int_init(&g_audit_serial_num, 0);
	} else {
		hm_error("audit serial header invalid: %u\n", serial_header);
		ret = E_HM_INVAL;
	}

	return ret;
}

/* Initialize shared ring buffer based on hmtrace
 *
 * @return	: error code
 */
int audit_trace_init(void)
{
	int ret;

	ret = hm_trace_register_class(&audit_class_uev);
	if (ret != E_HM_OK) {
		hm_error("failed to register audit class, err=%s\n", hmstrerror(ret));
		return ret;
	}
	ret = hm_trace_register_event(&audit_class_uev, &audit_event);
	if (ret != E_HM_OK) {
		hm_error("failed to register audit event, err=%s\n", hmstrerror(ret));
		return ret;
	}
	ret = hm_trace_set_event_tracing_on(&audit_event);

	return ret;
}

void audit_sysif_hook_set(void)
{
	sysif_set_entry_hook(audit_sysif_entry);
	sysif_set_exit_hook(audit_sysif_exit);
}

void audit_server_num_set(enum __server_audit_numbers num)
{
	if ((unsigned int)num == (unsigned int)__SERVER_INVAL || (unsigned int)num >= (unsigned int)__SERVER_MAX) {
		hm_warn("failed to set server number, num=%u\n", (unsigned int)num);
	}
	g_server_num = num;
}

enum __server_audit_numbers audit_server_num_get(void)
{
	return g_server_num;
}

STATIC int audit_ctx_get_cmdline_info(struct audit_ctx *ctx, struct audit_cmdline_info *info)
{
	int ret = E_HM_OK;
	struct sec_cred_audit *audit;
	char *cred_cmdline = NULL;
	cmdline_node_t *cmdline_node = NULL;

	/* shared cred may be null if cred is not properly filled */
	if (ctx->cred.cred != NULL) {
		audit = audit_cred_of_sec_cred(&(ctx->cred));
		if (audit->cmdline_idx == 0) {
			hm_debug("invalid cmdline_idx\n");
		} else {
			cmdline_node = sec_cred_idx_to_ptr(audit->cmdline_idx);
		}
	}
	if (cmdline_node != NULL && cmdline_node->size > 0) {
		cred_cmdline = sec_cred_idx_to_ptr(cmdline_node->str_idx);
		if (cred_cmdline == NULL) {
			hm_error("cred_cmdline is NULL\n");
			ret = E_HM_INVAL;
		} else {
			info->cmdline_size = cmdline_node->size - 1;
			info->cmdline = cred_cmdline;
		}
	}

	return ret;
}

STATIC void audit_ctx_fill_tty(struct audit_ctx *ctx)
{
	struct sec_cred_audit *audit;
	char *tty = TTY_NONE;
	uint32_t tty_len = sizeof(TTY_NONE) - 1;
	tty_node_t *tty_node = NULL;

	/* shared cred may be null if cred is not properly filled */
	if (ctx->cred.cred != NULL) {
		audit = audit_cred_of_sec_cred(&(ctx->cred));
		if (audit->tty_idx == 0) {
			hm_debug("invalid tty_idx\n");
		} else {
			tty_node = sec_cred_idx_to_ptr(audit->tty_idx);
		}
	}
	if (tty_node != NULL && tty_node->size > 0) {
		tty = sec_cred_idx_to_ptr(tty_node->str_idx);
		if (tty != NULL) {
			tty_len = tty_node->size;
		}
	}
	ctx->tty.tty_len = tty_len;
	ctx->tty.tty = tty;
}

void audit_ctx_fill_proctitle(struct audit_ctx *ctx)
{
	int ret = E_HM_OK;

	if (g_get_cmdline_hook_func != NULL) {
		ret = g_get_cmdline_hook_func(ctx, &ctx->proctitle);
	}

	if (ret != E_HM_OK || ctx->proctitle.cmdline_size == 0) {
		audit_ctx_fill_proctitle_nullmsg(ctx);
	}

	return;
}

void audit_proctitle_set_hook(audit_get_cmdline_hook_t func)
{
	g_get_cmdline_hook_func = func;
}

void audit_sysif_sw_hook_set(void)
{
	sysif_entry_need_sw_set(hm_sec_shconf_audit());
	if (audit_server_num_get() == __server_fs) {
		sysif_exit_need_hook_set(audit_ctx_4_fs_is_valid);
	} else {
		sysif_exit_need_hook_set(audit_ctx_is_valid);
	}
}

/*
 * Init audit module during the initializing of each system server
 *
 * @return		: error code
 */
int audit_module_init(void)
{
	int err;

	err = hm_sec_shconf_init();
	if (err != E_HM_OK) {
		hm_error("failed to initialize shconf, err=%s\n", hmstrerror(err));
		return err;
	}
	audit_sysif_sw_hook_set();

	if (audit_sw_is_disabled()) {
		hm_info("audit switch is disable, init exit\n");
		return E_HM_OK;
	}

	audit_sysif_hook_set();
	sec_set_deny_log_hook(audit_log_vsend);
	audit_proctitle_set_hook(audit_ctx_get_cmdline_info);

	hm_info("init audit module successfully\n");
	return E_HM_OK;
}
int audit_field_cmp_signed(long left, uint32_t op, long right)
{
	switch (op) {
	case HM_AUDIT_ENUM_GT:
		return (left > right);
	case HM_AUDIT_ENUM_LT:
		return (left < right);
	case HM_AUDIT_ENUM_LE:
		return (left <= right);
	case HM_AUDIT_ENUM_GE:
		return (left >= right);
	case HM_AUDIT_ENUM_NOT_EQUAL:
		return (left != right);
	case HM_AUDIT_ENUM_EQUAL:
		return (left == right);
	case HM_AUDIT_ENUM_BITTEST:
		return (((unsigned long)left & (unsigned long)right) == (unsigned long)right);
	case HM_AUDIT_ENUM_BITMASK:
		return (((unsigned long)left & (unsigned long)right) != 0U);
	default:
		BUG("audit_field_cmp_signed operation is not valid");
		return 0;
	}
}

int audit_field_cmp(unsigned long left, uint32_t op, unsigned long right)
{
	switch (op) {
	case HM_AUDIT_ENUM_LT:
		return (left < right);
	case HM_AUDIT_ENUM_GT:
		return (left > right);
	case HM_AUDIT_ENUM_LE:
		return (left <= right);
	case HM_AUDIT_ENUM_GE:
		return (left >= right);
	case HM_AUDIT_ENUM_EQUAL:
		return (left == right);
	case HM_AUDIT_ENUM_NOT_EQUAL:
		return (left != right);
	case HM_AUDIT_ENUM_BITTEST:
		return ((left & right) == right);
	case HM_AUDIT_ENUM_BITMASK:
		return ((left & right) != 0UL);
	default:
		BUG("audit_field_cmp operation is not valid");
		return 0;
	}
}

STATIC int audit_field_match_top_half(const struct audit_field *field, const struct audit_match *match)
{
	switch (field->type) {
	case HM_AUDIT_PID:
		return audit_field_cmp_signed(match->ctx->pid, field->op, (int)field->val);
	case HM_AUDIT_PPID:
		return audit_field_cmp_signed(match->ctx->ppid, field->op, (int)field->val);
	case HM_AUDIT_UID:
		return audit_field_cmp(match->ctx->uid, field->op, field->val);
	case HM_AUDIT_EUID:
		return audit_field_cmp(match->ctx->euid, field->op, field->val);
	case HM_AUDIT_SUID:
		return audit_field_cmp(match->ctx->suid, field->op, field->val);
	case HM_AUDIT_FSUID:
		return audit_field_cmp(match->ctx->fsuid, field->op, field->val);
	case HM_AUDIT_GID:
		return audit_field_cmp(match->ctx->gid, field->op, field->val);
	case HM_AUDIT_EGID:
		return audit_field_cmp(match->ctx->egid, field->op, field->val);
	case HM_AUDIT_SGID:
		return audit_field_cmp(match->ctx->sgid, field->op, field->val);
	case HM_AUDIT_FSGID:
		return audit_field_cmp(match->ctx->fsgid, field->op, field->val);
	case HM_AUDIT_LOGINUID:
		return audit_field_cmp(match->ctx->login_uid, field->op, field->val);
	case HM_AUDIT_SESSIONID:
		return audit_field_cmp(match->ctx->session_id, field->op, field->val);
	default:
		hm_error("rule field type isn't supported\n");
		return 0;
	}
}

STATIC int audit_field_match_bottom_half(const struct audit_field *field, const struct audit_match *match)
{
	switch (field->type) {
	case HM_AUDIT_ARG0:
	case HM_AUDIT_ARG1:
	case HM_AUDIT_ARG2:
	case HM_AUDIT_ARG3:
		return audit_field_cmp(match->ctx->argv[field->type - HM_AUDIT_ARG0],
					field->op, field->val);
	case HM_AUDIT_EXIT:
		if (match->ctx->ret_valid != 0) {
			return audit_field_cmp_signed(match->ctx->ret_code, field->op, (int)field->val);
		}
		return 0;
	case HM_AUDIT_SUCCESS:
		if (match->ctx->ret_valid != 0) {
			return audit_field_cmp_signed(match->ctx->ret_valid, field->op,
						(field->val != 0U) ? HM_AUDIT_SYSIF_SUCCESS
						: HM_AUDIT_SYSIF_FAILURE);
		}
		return 0;
	case HM_AUDIT_PERM:
		return (((field->val) & ((uint32_t)(match->ctx->perm))) != 0U);
	default:
		hm_error("rule field type(%u) isn't supported\n", field->type);
		return 0;
	}
}

int audit_field_match(const struct audit_field *field, const struct audit_match *match)
{
	if (field->type <= HM_AUDIT_SESSIONID) {
		return audit_field_match_top_half(field, match);
	} else {
		return audit_field_match_bottom_half(field, match);
	}
}

int audit_cred_field_match(const struct audit_field *field, struct audit_ctx *ctx)
{
	int ret = MATCH_RES_NOT_MATCHED;
	struct audit_match match = {
		.msgtype = 0,
		.ctx = ctx,
	};

	if (field->type <= HM_AUDIT_SESSIONID) {
		ret = audit_field_match_top_half(field, &match);
		if (ret != 0) {
			ret = MATCH_RES_MATCHED;
		}
	}
	return ret;
}

STATIC void audit_init_match_from_ctx(struct audit_ctx *ctx, int msgtype,
				      struct audit_match *match)
{
	match->msgtype = msgtype;
	match->ctx = ctx;
}

STATIC int audit_rule_filter_syscall(struct audit_match *match)
{
	return audit_shared_rule_iterate(HM_AUDIT_FILTER_EXIT, match);
}

int audit_rule_filter(int msgtype, unsigned int listtype, struct audit_ctx *ctx)
{
	int result = 0;
	struct audit_match match;
	audit_init_match_from_ctx(ctx, msgtype, &match);
	if (listtype == HM_AUDIT_FILTER_EXIT) {
		result = audit_rule_filter_syscall(&match);
	}
	return result;
}

STATIC int __audit_ctx_fill_dac(struct audit_ctx *ctx, const struct sec_chk_cred *cred)
{
	int ret = E_HM_OK;
	struct sec_cred_dac *cred_dac = NULL;

	cred_dac = dac_cred_of_sec_cred(cred);
	ctx->uid = cred_dac->ruid;
	ctx->euid = cred_dac->euid;
#ifdef CONFIG_AUDIT
	ctx->suid = cred_dac->suid;
	ctx->sgid = cred_dac->sgid;
#endif
	ctx->fsuid = cred_dac->fsuid;
	ctx->gid = cred_dac->rgid;
	ctx->egid = cred_dac->egid;
	ctx->fsgid = cred_dac->fsgid;

	return ret;
}

STATIC int audit_ctx_fill_cred_info(struct audit_ctx *ctx)
{
	struct sec_chk_cred *cred = &ctx->cred;
	struct sec_cred_basic *cred_basic = &ctx->basic;
	int ret = E_HM_OK;

	ctx->cnode_idx = cred->cnode_idx;
	ctx->pid = cred_basic->pid;
#ifdef CONFIG_AUDIT
	ctx->ppid = cred_basic->ppid;
	struct sec_cred_audit *cred_audit;
	cred_audit = audit_cred_of_sec_cred(cred);
	ctx->login_uid = cred_audit->login_uid;
	ctx->session_id = cred_audit->session_id;
#else
	ctx->login_uid = 0;
	ctx->session_id = 0;
#endif

	ret = __audit_ctx_fill_dac(ctx, cred);
	if (ret != E_HM_OK) {
		hm_error("failed to fill in dac information into audit context, err=%s\n",
			 hmstrerror(ret));
		return ret;
	}

	return ret;
}

/* fill in the common fields of audit context */
STATIC int audit_ctx_fill_common(struct audit_ctx *ctx, long ret_code,
				 int callnum, int argc, va_list args)
{
	int success = ((ret_code >= 0) || (ret_code < E_HM_USER_MAX)) ?
			HM_AUDIT_SYSIF_SUCCESS : HM_AUDIT_SYSIF_FAILURE;

	ctx->syscall_num = (uint32_t)callnum;
	ctx->ret_code = ret_code;
	ctx->ret_valid = success;
	for (int i = 0; i < argc && i < AUDIT_SYSCALL_ARGC; ++i) {
		ctx->argv[i] = va_arg(args, unsigned long);
	}
#ifdef CONFIG_AARCH64
		/* should check if the client is compat in the future */
		ctx->arch = HM_AUDIT_ARCH_AARCH64;
#else
		ctx->arch = HM_AUDIT_ARCH_ARM;
#endif

	return audit_ctx_fill_cred_info(ctx);
}

STATIC void audit_watch_del_ctx_reset(struct audit_watch_del_ctx *ctx)
{
	unsigned int i;
	for (i = 0; i < ctx->del_dir_count; i++) {
		free(ctx->del_dir[i].full_path);
		ctx->del_dir[i].full_path = NULL;
		ctx->del_dir[i].me_idx = AUDIT_INVALID_ME_IDX;
	}
	ctx->del_dir_count = 0;
}

void audit_ctx_reset(struct audit_ctx *ctx)
{
	unsigned int i;
	for (i = 0; i < ctx->watch.field_count; i++) {
		if (ctx->watch.field[i]->full_path != NULL) {
			free(ctx->watch.field[i]->full_path);
			ctx->watch.field[i]->full_path = NULL;
		}
		free(ctx->watch.field[i]);
		ctx->watch.field[i] = NULL;
	}
	if (ctx->avc_info.avc_str != NULL) {
		free(ctx->avc_info.avc_str);
		ctx->avc_info.avc_str = NULL;
	}
	mem_zero_s(*ctx);
	ctx->record_state = AUDIT_BUILD;
	ctx->sock.fd[0] = -1;
	audit_ctx_set_invalid();
}

STATIC int __audit_log_write(const struct audit_log *log)
{
	return hm_trace_write_event_unblock(&audit_event, log, log->total_len);
}

int audit_log_write(const struct audit_log *log)
{
	int ret = init_ringbuffer();
	if (ret != E_HM_OK) {
		hm_error("failed to initialize ringbuffer, err=%s\n", hmstrerror(ret));
	} else {
		ret = __audit_log_write(log);
	}
	return ret;
}

/*
 * for user msg type, we only set serial and timestamp to audit log
 * without impacting audit ctx
 */
STATIC void audit_log_common(const struct sec_chk_cred *cred, uint32_t audit_type,
			     struct audit_log *log)
{
	log->cnode_idx = cred->cnode_idx;
	switch (audit_type) {
	case HM_AUDIT_USER:
	case HM_AUDIT_FIRST_USER_MSG ... HM_AUDIT_LAST_USER_MSG:
	case HM_AUDIT_FIRST_USER_MSG2 ... HM_AUDIT_LAST_USER_MSG2:
	case HM_AUDIT_CONFIG_CHANGE:
	case HM_AUDIT_NETFILTER_PKT:
		log->serial = audit_get_serial();
		NOFAIL(clock_gettime(CLOCK_REALTIME, &(log->ts)));
		break;
	default:
		NOFAIL(audit_ctx_fill_timestamp_and_serial(&g_audit_ctx));
		log->serial = g_audit_ctx.serial;
		log->ts = g_audit_ctx.timestamp;
		break;
	}
}

STATIC inline void audit_log_pid(const struct sec_chk_cred *cred, struct audit_log *log)
{
	struct sec_cred_basic *cred_basic;

	log->flag |= TYPE_AUDIT_PID;
	cred_basic = basic_cred_of_sec_cred(cred);
	log->pid.ppid = cred_basic->ppid;
	log->pid.pid = cred_basic->pid;
}

STATIC inline void audit_log_session(const struct sec_chk_cred *cred, struct audit_log *log)
{
	struct sec_cred_audit *cred_audit;

	log->flag |= TYPE_AUDIT_SESSION;
	cred_audit = audit_cred_of_sec_cred(cred);
	log->session.session_id = cred_audit->session_id;
	log->session.loginuid = cred_audit->login_uid;
}

STATIC void audit_log_dac(const struct sec_chk_cred *cred, struct audit_log *log)
{
	struct sec_cred_dac *cred_dac;

	log->flag |= TYPE_AUDIT_DAC;
	cred_dac = dac_cred_of_sec_cred(cred);
	log->dac.uid = cred_dac->ruid;
	log->dac.euid = cred_dac->euid;
	log->dac.suid = cred_dac->suid;
	log->dac.sgid = cred_dac->sgid;
	log->dac.fsuid = cred_dac->fsuid;
	log->dac.gid = cred_dac->rgid;
	log->dac.egid = cred_dac->egid;
	log->dac.fsgid = cred_dac->fsgid;
}

STATIC void audit_watch_log_gen(struct audit_log *log, const struct audit_ctx *ctx)
{
	struct audit_watch_field *field = NULL;
	char *path = NULL;
	uint32_t watch_len = ((uint32_t)sizeof(struct audit_watch_field)) * (ctx->watch.field_count);
	char *pos = log->data;

	field = (struct audit_watch_field *)pos;
	path = pos + sizeof(struct audit_watch_field) * ctx->watch.field_count;
	for (unsigned int i = 0; i < ctx->watch.field_count; i++) {
		field[i].dev_id = ctx->watch.field[i]->dev_id;
		field[i].index = ctx->watch.field[i]->index;
		field[i].devno = ctx->watch.field[i]->devno;
		field[i].type = ctx->watch.field[i]->type;
		if (ctx->watch.field[i]->path != NULL) {
			size_t len = strlen(ctx->watch.field[i]->path) + 1;
			NOFAIL(strcpy_s(path, len, ctx->watch.field[i]->path));
			path += len;
			watch_len += (uint32_t)len;
		}
	}
	log->watch.field_count = ctx->watch.field_count;
	log->watch.watch_len = watch_len;
	log->flag |= TYPE_AUDIT_PATH;
}

STATIC bool audit_string_need_trans_hex(char *buffer, size_t buf_len)
{
	char *tmp_char = NULL;
	size_t i = 0;
	for (i = 0; i < buf_len; i++) {
		tmp_char = buffer + i;
		if (*tmp_char == '"' || isgraph((int)*tmp_char) == 0) {
			return true;
		}
	}

	return false;
}

STATIC uint32_t audit_log_exe_comm_len(struct audit_ctx *ctx)
{
	uint32_t len;

	if (ctx->proctitle.cmdline == NULL) {
		audit_ctx_fill_proctitle(ctx);
	}

	/* exe len */
	len = (uint32_t)ctx->proctitle.cmdline_size + 1U;
	/* comm len */
	len += (uint32_t)ctx->proctitle.cmdline_size + 1U;
	/* tty len */
	audit_ctx_fill_tty(ctx);
	len += (uint32_t)ctx->tty.tty_len + 1U;
	return len;
}

STATIC uint32_t audit_log_avc_len(struct audit_ctx *ctx)
{
	uint32_t len = 0;
	if (ctx->avc_info.avc_size == 0) {
		return len;
	}

	len = (uint32_t)(ctx->avc_info.avc_size) + 1U;

	if (ctx->proctitle.cmdline == NULL) {
		audit_ctx_fill_proctitle(ctx);
	}

	/* comm len */
	len += (uint32_t)ctx->proctitle.cmdline_size + 1U;
	return len;
}

STATIC uint32_t audit_log_total_len(struct audit_ctx *ctx, uint32_t audit_type)
{
	uint32_t total_len = (uint32_t)sizeof(struct audit_log);
	uint32_t watch_len = 0U;
	uint32_t avc_len = 0U;

	switch(audit_type) {
	case HM_AUDIT_SYSCALL:
		total_len += audit_log_exe_comm_len(ctx);
		break;
	case HM_AUDIT_PATH:
		watch_len = ((uint32_t)sizeof(struct audit_watch_field)) * (ctx->watch.field_count);
		for (unsigned int i = 0; i < ctx->watch.field_count && ctx->watch.field != NULL; i++) {
			watch_len += (uint32_t)strlen(ctx->watch.field[i]->path) + 1;
		}
		total_len = (watch_len > 0U) ? (total_len + watch_len) : 0U;
		break;
	case HM_AUDIT_FD_PAIR:
		if (ctx->sock.fd[0] < 0) {
			total_len = 0U;
		}
		break;
	case HM_AUDIT_SOCKADDR:
		total_len = (ctx->sock.addr_len > 0U) ? (total_len + ctx->sock.addr_len) : 0U;
		break;
	case HM_AUDIT_PROCTITLE:
		if (ctx->proctitle.cmdline == NULL) {
			audit_ctx_fill_proctitle(ctx);
		}

		if (audit_string_need_trans_hex(ctx->proctitle.cmdline, ctx->proctitle.cmdline_size)) {
			total_len += (uint32_t)(PROCTITLE_HEAD_SIZE + (ctx->proctitle.cmdline_size << 1) + 1);
		} else {
			total_len += (uint32_t)(PROCTITLE_HEAD_SIZE + PROCTITLE_QUOTE_SIZE + ctx->proctitle.cmdline_size + 1);
		}
		break;
	case HM_AUDIT_EOE:
		break;
	case HM_AUDIT_AVC:
		/*
		 * When avc_len is 0, it means that there is no avc deny message and
		 * no avc type log is required to be generated.
		 */
		avc_len = audit_log_avc_len(ctx);
		total_len = (avc_len > 0U) ? (total_len + avc_len) : 0U;
		break;
	default:
		total_len = 0U;
		hm_error("not support the type=%u\n", audit_type);
		break;
	}

	return total_len;
}

STATIC void audit_syscall_exe_comm_log_gen(struct audit_log *log, const struct audit_ctx *ctx)
{
	char *exe = NULL;
	char *comm = NULL;
	char *saveptr = NULL;
	char *tmp = NULL;
	char *buff = log->data;
	size_t pos_len = log->total_len - sizeof(struct audit_log);

	log->pid.exe_len = 0;
	log->pid.comm_len = 0;
	log->pid.tty_len = 0;

	tmp = malloc(ctx->proctitle.cmdline_size + 1);
	if (tmp == NULL) {
		return;
	}
	NOFAIL(memcpy_s(tmp, ctx->proctitle.cmdline_size,
			ctx->proctitle.cmdline, ctx->proctitle.cmdline_size));
	*(tmp + ctx->proctitle.cmdline_size) = '\0';

	exe = strtok_r(tmp, " ", &saveptr);
	if (exe == NULL) {
		exe = tmp;
	}

	log->pid.exe_len = (uint32_t)strlen(tmp);
	NOFAIL(memcpy_s(buff, pos_len, tmp, log->pid.exe_len));
	buff += log->pid.exe_len;
	*buff = '\0';
	buff++;
	log->pid.exe_len++;
	pos_len -= log->pid.exe_len;

	comm = strrchr(exe, '/');
	if (comm == NULL) {
		comm = exe;
	} else {
		comm += 1;
	}
	log->pid.comm_len = (uint32_t)strlen(comm);
	NOFAIL(memcpy_s(buff, pos_len, comm, log->pid.comm_len));
	buff += log->pid.comm_len;
	*buff = '\0';
	log->pid.comm_len++;
	buff++;
	pos_len -= log->pid.comm_len;
	NOFAIL(memcpy_s(buff, pos_len, ctx->tty.tty, ctx->tty.tty_len));
	log->pid.tty_len = ctx->tty.tty_len + 1;
	buff += ctx->tty.tty_len;
	*buff = '\0';

	free(tmp);
	return;
}

STATIC void audit_syscall_log_gen(struct audit_log *log, const struct audit_ctx *ctx)
{
	/* syscall info */
	log->flag |= TYPE_AUDIT_SYSCALL;
	log->syscall.arch = ctx->arch;
	log->syscall.syscall_num = ctx->syscall_num;
	/* copy the first four arg */
	log->syscall.argv[0] = ctx->argv[0];
	log->syscall.argv[1] = ctx->argv[1];
	log->syscall.argv[2] = ctx->argv[2];
	log->syscall.argv[3] = ctx->argv[3];
	log->syscall.exit = (uint64_t)ctx->ret_code;
	/* pid info */
	log->flag |= TYPE_AUDIT_PID;
	log->pid.ppid = ctx->ppid;
	log->pid.pid = ctx->pid;
	audit_syscall_exe_comm_log_gen(log, ctx);
	/* session info */
	log->flag |= TYPE_AUDIT_SESSION;
	log->session.loginuid = ctx->login_uid;
	log->session.session_id = ctx->session_id;
	/* dac info */
	log->flag |= TYPE_AUDIT_DAC;
	log->dac.uid = ctx->uid;
	log->dac.euid = ctx->euid;
	log->dac.suid = ctx->suid;
	log->dac.fsuid = ctx->fsuid;
	log->dac.gid = ctx->gid;
	log->dac.egid = ctx->egid;
	log->dac.sgid = ctx->sgid;
	log->dac.fsgid = ctx->fsgid;
	/* serial */
	log->serial = ctx->serial;
}

STATIC void audit_fd_pair_gen(struct audit_log *log, const struct audit_ctx *ctx)
{
	log->sock.fd[0] = ctx->sock.fd[0];
	log->sock.fd[1] = ctx->sock.fd[1];
	log->flag |= TYPE_AUDIT_FD_PAIR;
}

STATIC void audit_sock_log_gen(struct audit_log *log, const struct audit_ctx *ctx)
{
	log->sock.addr_len = ctx->sock.addr_len;
	NOFAIL(memcpy_s(log->data, ctx->sock.addr_len, ctx->sock.addr, ctx->sock.addr_len));
	log->flag |= TYPE_AUDIT_SOCKADDR;
}

STATIC int audit_string_trans_to_hex(const char *in_buffer, size_t in_len, char *out_buffer, size_t out_total_len)
{
	int ret = E_HM_OK;
	size_t i = 0;
	char *buff_ptr = NULL;

	if (in_buffer == NULL || in_len == 0 || out_buffer == NULL) {
		hm_error("invalid parameter, in_buffer or out_buffer is NULL or in_len=0\n");
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK && (in_len << 1) > out_total_len) {
		hm_error("out_total_len is not large enough\n");
		ret =  E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		buff_ptr = out_buffer;
		for (i = 0; i < in_len; i++) {
			buff_ptr = trans_to_hex_upper(buff_ptr, *(in_buffer + i));
		}
	}

	return ret;
}

STATIC void audit_proctitle_log_gen(struct audit_log *log, struct audit_ctx *ctx)
{
	int ret;
	char *buff = log->data;
	size_t pos_len = log->total_len - sizeof(struct audit_log);

	BUG_ON(buff == NULL);
	NOFAIL(memcpy_s(buff, pos_len, PROCTITLE_HEAD, PROCTITLE_HEAD_SIZE));

	buff += PROCTITLE_HEAD_SIZE;
	pos_len -= PROCTITLE_HEAD_SIZE;

	if (audit_string_need_trans_hex(ctx->proctitle.cmdline, ctx->proctitle.cmdline_size)) {
		ret = audit_string_trans_to_hex(ctx->proctitle.cmdline,
						ctx->proctitle.cmdline_size,
						buff, pos_len);
		if (ret != E_HM_OK) {
			hm_error("failed to translate audit string to hex, err=%s\n", hmstrerror(ret));
		}
	} else {
		/* reserve double_quote size and terminator size */
		pos_len -= PROCTITLE_QUOTE_SIZE + 1;
		*buff = '"';
		buff++;

		NOFAIL(memcpy_s(buff, pos_len, ctx->proctitle.cmdline,
			 ctx->proctitle.cmdline_size));
		buff += ctx->proctitle.cmdline_size;

		*buff = '"';
		buff++;
		*buff = '\0';
	}

	log->flag = TYPE_AUDIT_FORMAT;
	return;
}

STATIC void audit_eoe_log_gen(struct audit_log *log)
{
	log->flag = 0;
	return;
}

STATIC uint32_t audit_avc_comm_log_gen(char *pos, size_t pos_len, const struct audit_ctx *ctx)
{
	char *exe = NULL;
	char *comm = NULL;
	char *saveptr = NULL;
	char *tmp = NULL;
	uint32_t len = 0;

	tmp = malloc(ctx->proctitle.cmdline_size + 1);
	if (tmp == NULL) {
		hm_error("malloc failed\n");
		return len;
	}
	NOFAIL(memcpy_s(tmp, ctx->proctitle.cmdline_size,
			ctx->proctitle.cmdline, ctx->proctitle.cmdline_size));
	*(tmp + ctx->proctitle.cmdline_size) = '\0';

	exe = strtok_r(tmp, " ", &saveptr);
	if (exe == NULL) {
		exe = tmp;
	}

	comm = strrchr(exe, '/');
	if (comm == NULL) {
		comm = exe;
	} else {
		comm += 1;
	}
	len = (uint32_t)strlen(comm);
	NOFAIL(memcpy_s(pos, pos_len, comm, len));
	pos[len] = '\0';
	len++;

	free(tmp);
	return len;
}

// This method is used to write deny logs of CPERMISSION and CRIGHT from kernel,
// The sec deny logs from system services are wrriten by audit_log_vsend.
// In the future, they should be merged.
STATIC void audit_avc_log_gen(struct audit_log *log, struct audit_ctx *ctx)
{
	int ret = E_HM_OK;

	if (log->total_len == 0U) {
		return; // empty avc log
	}
	char *pos = log->data;
	size_t pos_len = log->total_len - sizeof(struct audit_log);

	log->flag = TYPE_AUDIT_MAC;
	log->pid.pid = ctx->pid;

	// copy avc: denied { CPERMISSION | CRIGHT } field
	ret = sprintf_s(pos, pos_len, "%s", ctx->avc_info.avc_str);
	if (ret < 0) {
		hm_error("failed to generate field of audit avc log\n");
		return;
	} else {
		log->avc.avc_len = (uint32_t)ctx->avc_info.avc_size;
		pos += ctx->avc_info.avc_size; // avc_size has included terminator null
		pos_len -= ctx->avc_info.avc_size;
	}

	log->avc.subj = ctx->avc_info.subj;
	log->avc.obj = ctx->avc_info.obj;

	/* fill comm in avc log */
	log->avc.comm_len = audit_avc_comm_log_gen(pos, pos_len, ctx);
	if (log->avc.comm_len == 0) {
		hm_error("failed to generate command field of audit avc log\n");
	}

	return;
}

struct audit_log *audit_log_gen(struct audit_ctx *ctx, uint32_t audit_type)
{
	struct audit_log *log = NULL;
	uint32_t total_len = audit_log_total_len(ctx, audit_type);
	if (total_len == 0U) {
		return NULL;
	}

	log = malloc(total_len);
	if (log == NULL) {
		hm_error("failed to allocate memory for audit_log\n");
		return NULL;
	}
	mem_zero_b(log, total_len);

	/* common info */
	log->rule_id = ctx->rule_id;
	log->audit_type = audit_type;
	log->cnode_idx = ctx->cnode_idx;
	log->ts = ctx->timestamp;
	log->total_len = total_len;
	log->serial = ctx->serial;

	switch(audit_type) {
	case HM_AUDIT_SYSCALL:
		audit_syscall_log_gen(log, ctx);
		break;
	case HM_AUDIT_PATH:
		audit_watch_log_gen(log, ctx);
		break;
	case HM_AUDIT_FD_PAIR:
		audit_fd_pair_gen(log, ctx);
		break;
	case HM_AUDIT_SOCKADDR:
		audit_sock_log_gen(log, ctx);
		break;
	case HM_AUDIT_PROCTITLE:
		audit_proctitle_log_gen(log, ctx);
		break;
	case HM_AUDIT_EOE:
		audit_eoe_log_gen(log);
		break;
	case HM_AUDIT_AVC:
		audit_avc_log_gen(log, ctx);
		break;
	default:
		hm_debug("audit type is not supported, type=%u\n", audit_type);
		free(log);
		log = NULL;
		break;
	}
	return log;
}

/*
 * Allocate a mem for audit_log with a AUDIT_LOG_BUF_SIZE long data, in which
 * format string are filled.
 *
 * @param audit_type [I] message type of audit log, refer to <libhmsrv_audit/audit.h>
 * @param flag [I] specified field type, refer to <libhmaudit/audit_log.h>
 *
 * @return The pointer to allocated audit_log
 *
 */
struct audit_log *audit_log_alloc(uint32_t audit_type, uint32_t flag)
{
	struct audit_log *log = NULL;
	struct sec_chk_cred cred = {0};
	uint32_t len = sizeof(struct audit_log) + AUDIT_LOG_BUF_SIZE;

	log = malloc(len);
	if (log == NULL) {
		hm_debug("failed to allocate memory for audit log\n");
	} else {
		mem_zero_b(log, len);
		log->audit_type = audit_type;
		log->total_len = sizeof(struct audit_log);
		log->flag = flag;
		NOFAIL(sec_chk_cred_init_requester(&cred));
		if ((flag & TYPE_AUDIT_COMMON) != 0) {
			audit_log_common(&cred, audit_type, log);
		}
		if ((flag & TYPE_AUDIT_PID) != 0) {
			audit_log_pid(&cred, log);
		}
		if ((flag & TYPE_AUDIT_SESSION) != 0) {
			audit_log_session(&cred, log);
		}
		if ((flag & TYPE_AUDIT_DAC) != 0) {
			audit_log_dac(&cred, log);
		}
	}

	return log;
}

/* Returns the number of characters formatted or an error code if an error occurred. */
STATIC inline int __audit_log_fmt(struct audit_log *log, const char *fmt,
				  va_list args)
{
	log->flag |= TYPE_AUDIT_FORMAT;
	int ret = vsprintf_s(audit_log_tail(log), audit_log_tail_size(log), fmt, args);
	if (ret < 0) {
		hm_debug("failed to format audit log\n");
		ret = E_HM_INVAL;
	} else {
		log->total_len += (uint32_t)ret;
		ret = E_HM_OK;
	}

	return ret;
}

/*
 * Fill format string into audit log, can be invoked multiple times but only after
 * all structural fields.
 *
 * @param log [I] pointer to audit_log
 * @param fmt [I] format string to use
 * @param ... [I] variables for the format string
 *
 * @note   The total maximum length of string is now 1024.
 *         Be aware that, the method should only be invoked after all structural
 *         fields(pid, session, etc.) are filled to avoid messing up the
 *         sequence of logs.
 * @return error code
 *
 */
int audit_log_fmt(struct audit_log *log, const char *fmt, ...)
{
	int ret = E_HM_OK;
	va_list args;

	if (log == NULL) {
		ret = E_HM_INVAL;
	}
	if (ret == E_HM_OK) {
		va_start(args, fmt);
		ret = __audit_log_fmt(log, fmt, args);
		va_end(args);
	}

	return ret;
}

/*
 * Generates an audit log and sends it out.
 *
 * @param log [I] pointer to audit_log
 * @param fmt [I] format string to use
 * @param va_list [I] variables for the format string
 * @return Error code
 *
 */
int audit_log_vsend(uint32_t audit_type, uint32_t flag, const char *fmt, va_list args)
{
	int rc = E_HM_OK;
	struct audit_log *log = NULL;

	if (audit_is_disabled()) {
		return E_HM_OK;
	}

	log = audit_log_alloc(audit_type, flag);
	if (log == NULL) {
		rc = E_HM_NOMEM;
	}
	if (rc == E_HM_OK) {
		rc = __audit_log_fmt(log, fmt, args);
		if (rc == E_HM_OK) {
			rc = audit_log_write(log);
		}
		free(log);
	}

	return rc;
}

/*
 * Generates an audit log and sends it out.
 * This is a wrapper of audit_log_alloc, audit_log_vsprintf and audit_log_write.
 *
 * @param log [I] pointer to audit_log
 * @param fmt [I] format string to use
 * @param ... [I] variables for the format string
 * @return Error code
 *
 */
int audit_log_send(uint32_t audit_type, uint32_t flag, const char *fmt, ...)
{
	int rc;
	va_list args;

	va_start(args, fmt);
	rc = audit_log_vsend(audit_type, flag, fmt, args);
	va_end(args);

	return rc;
}

void audit_log_key(struct audit_log *log, const char *key)
{
	if (key) {
		(void)audit_log_fmt(log, " key=\"%s\"", key);
	} else {
		(void)audit_log_fmt(log, " key=(null)");
	}
}

STATIC int rb_log_gen(int rule_filter, struct audit_ctx *ctx)
{
	int ret = E_HM_OK;

	if (rule_filter == MATCH_RES_MATCHED && ctx->record_state == AUDIT_RECORD) {
		ret = audit_log_generate_prepare(ctx);
		for (unsigned int i = 0U; i < ARRAY_SIZE(g_audit_type) && (ret == E_HM_OK); i++) {
			struct audit_log *log = audit_log_gen(ctx, g_audit_type[i]);
			if (log != NULL) {
				ret = __audit_log_write(log);
				free(log);
			}
		}
	}
	return ret;
}

STATIC int audit_rule_callnum(unsigned int callnum)
{
	if (callnum <= __ACTV_HIGHTABLE_START) {
		return E_HM_INVAL;
	}
	int audit_callnum = ((__hm_audit_sysif_start[(unsigned int)g_server_num - 1] +
			     (int)callnum) - __ACTV_HIGHTABLE_START) - 1;

	return audit_callnum;
}

int audit_lazy_init(void)
{
	return init_shm();
}

STATIC void audit_ctx_set_invoke_type(bool type)
{
	struct audit_ctx *ctx = audit_ctx_get();
	ctx->invoked_by_sys = type;
	return;
}

STATIC bool is_invoked_by_sys_service_or_auditd(void)
{
	int ret;
	struct audit_ctx *ctx = audit_ctx_get();
	struct sec_chk_cred *cred = &ctx->cred;
	struct sec_cred_basic *basic = &ctx->basic;

	ret = sec_chk_cred_init_requester(cred);
	if (ret != E_HM_OK) {
		hm_error("failed to initialize credential and requester, err=%s\n",
			 hmstrerror(ret));
		return false;
	}
	basic = basic_cred_of_sec_cred(cred);
	if (is_auditd((uint32_t)basic->pid)) {
		return true;
	}
	if ((basic->flags & ((1U << SEC_BASIC_SYS_SRVC))) == 0) {
		return false;
	}
	audit_ctx_set_invoke_type(true);
	return true;
}

STATIC int audit_sysif_log_gen(long ret_code, int argc, va_list args)
{
	int ret = E_HM_OK;
	int audit_callnum;
	unsigned int callnum;
	struct arch_actv_local *local = NULL;

	local = hm_actv_current_arch_actv_local();
	BUG_ON(local == NULL);

	callnum = (unsigned int)(__RPC_INFO_DECODE_CALLNO(local->rpcinfo));
	audit_callnum = audit_rule_callnum(callnum);
	if (audit_callnum < 0) {
		hm_error("failed to find the matched sysif callnum, callnum=0x%08x\n", callnum);
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ret = audit_ctx_fill_common(&g_audit_ctx, ret_code, audit_callnum, argc, args);
		if (ret != E_HM_OK) {
			hm_error("failed to fill audit context info, err=%s\n", hmstrerror(ret));
		}
	}

	if (ret == E_HM_OK) {
		ret = audit_rule_filter(HM_AUDIT_SYSCALL, HM_AUDIT_FILTER_EXIT, &g_audit_ctx);
		ret = rb_log_gen(ret, &g_audit_ctx);
		if (ret != E_HM_OK) {
			hm_error("failed to write audit log, err=%s\n", hmstrerror(ret));
		}
	}

	return ret;
}

STATIC void audit_sysif_release_ctx(void)
{
	audit_ctx_reset(&g_audit_ctx);
	return;
}

STATIC void audit_sysif_release_del_dir_ctx(void)
{
	if (g_audit_watch_del_ctx.del_dir_count > 0) {
		audit_watch_dir_rule_refresh(&g_audit_watch_del_ctx);
		audit_watch_del_ctx_reset(&g_audit_watch_del_ctx);
	}
}

STATIC void audit_watch_rule_reset(void)
{
	const struct audit_watch_field *field = g_audit_ctx.watch.del_field;
	if (field != NULL) {
		audit_update_rule(field->path, 0, 0, 1);
	}
}
/*
 * This method is the main entry of audit for sysif call.
 * It is called before sysif exit, creates audit context, filter matched rules
 * and send out audit log if needed.
 *
 * @param	long ret_code:		return value of sysif call
 * @param	int argc:		count of sysif call args
 * @param	...:			sysif call args
 *
 * @return	error code
 *
 */
int audit_sysif_exit(long ret_code, int argc, ...)
{
	int ret = E_HM_OK;
	va_list args;

	if (audit_ctx_is_valid()) {
		va_start(args, argc);
		ret = audit_sysif_log_gen(ret_code, argc, args);
		va_end(args);

		audit_watch_rule_reset();
		audit_sysif_release_ctx();
	}
	audit_sysif_release_del_dir_ctx();

	return ret;
}

bool audit_ctx_watch_field_exist(const struct audit_watch_field *new)
{
	struct audit_watch_field *field = NULL;

	for (unsigned int i = 0; i < g_audit_ctx.watch.field_count; ++i) {
		field = g_audit_ctx.watch.field[i];
		if (field->dev_id == new->dev_id &&
		    field->index == new->index &&
		    field->type == new->type &&
		    strcmp(field->path, new->path) == 0) {
			return true;
		}
	}

	return false;
}

int audit_ctx_watch_field_add(struct audit_watch_field *field, uint32_t *field_index)
{
	uint32_t cnt = g_audit_ctx.watch.field_count;
	if (cnt >= HM_AUDIT_WATCH_FIELD_MAX_NUM) {
		hm_error("watch fields is full\n");
		return E_HM_NOMEM;
	}
	g_audit_ctx.watch.field[cnt] = field;
	*field_index = cnt;
	g_audit_ctx.watch.field_count ++;
	return E_HM_OK;
}

void audit_ctx_perm_fill(int perm)
{
	if (g_audit_ctx.perm == 0) {
		g_audit_ctx.perm = perm;
	}
}

void audit_ctx_del_dir_fill(char *full_path, int me_idx, int pos)
{
	uint32_t i;
	if (g_audit_watch_del_ctx.del_dir_count >= HM_AUDIT_WATCH_FIELD_MAX_NUM) {
		hm_error("me array is too small\n");
		return;
	}

	i = g_audit_watch_del_ctx.del_dir_count;
	g_audit_watch_del_ctx.del_dir[i].full_path = full_path;
	g_audit_watch_del_ctx.del_dir[i].path = full_path + pos;
	g_audit_watch_del_ctx.del_dir[i].me_idx = me_idx;
	g_audit_watch_del_ctx.del_dir_count ++;
}

void audit_fill_del_field(const struct audit_watch_field *field)
{
	if (g_audit_ctx.watch.del_field == NULL) {
		g_audit_ctx.watch.del_field = field;
	}
}

void audit_ctx_meinfo_fill(int me_idx, uint32_t field_index)
{
	unsigned int i;

	if (g_audit_ctx.watch.dir_field_count >= HM_AUDIT_WATCH_FIELD_MAX_NUM) {
		hm_error("me array is too small\n");
		return;
	}
	for (i = 0; i < g_audit_ctx.watch.dir_field_count; ++i) {
		if (g_audit_ctx.watch.dir_field[i].me_idx == me_idx) {
			return;
		}
	}
	g_audit_ctx.watch.dir_field[i].me_idx = me_idx;
	g_audit_ctx.watch.dir_field[i].field_index = field_index;
	g_audit_ctx.watch.dir_field_count++;
}

struct audit_ctx *audit_ctx_get(void)
{
	return &g_audit_ctx;
}

struct audit_watch_del_ctx *audit_watch_del_ctx_get(void)
{
	return &g_audit_watch_del_ctx;
}

uint32_t audit_get_serial(void)
{
	uint32_t serial_value = 0;
	serial_value = (uint32_t)raw_atomic_int_inc(&g_audit_serial_num);
	serial_value &= ((1U << AUDIT_SERIAL_HEADER_OFFSET) - 1);
	return (serial_value | g_audit_serial_header);
}

void audit_ctx_set_serial(struct audit_ctx *ctx)
{
	if (ctx->serial == 0) {
		ctx->serial = audit_get_serial();
	}
	return;
}

int audit_ctx_fill_timestamp(struct audit_ctx *ctx)
{
	int ret = E_HM_OK;

	if (ctx->timestamp.tv_sec == 0 && ctx->timestamp.tv_nsec == 0) {
		/*
		 * In order to optimize performance, replace hm_clock_gettime with clock_gettime,
		 * clock_gettime is implemented with vdso, because sysmgr does not support vdso
		 * of clock_gettime, sysmgr override clock_gettime with hm_clock_gettime, and the
		 * performance in sysmgr is not improved.
		 */
		ret = clock_gettime(CLOCK_REALTIME, &(ctx->timestamp));
		if (ret < 0) {
			hm_error("get clock realtime failed, err = %s\n", hmstrerror(ret));
		}
	}

	return ret;
}

STATIC int audit_ctx_fill_timestamp_and_serial(struct audit_ctx *ctx)
{
	audit_ctx_set_serial(ctx);
	return audit_ctx_fill_timestamp(ctx);
}

STATIC int audit_log_generate_prepare(struct audit_ctx *ctx)
{
	int ret;
	ret = init_ringbuffer();
	if (ret != 0) {
		hm_error("init ringbuffer failed\n");
	} else {
		ret = audit_ctx_fill_timestamp_and_serial(ctx);
	}
	return ret;
}

STATIC bool audit_sysif_need_skip(void)
{
	/* We should not audit the sysifs added by dfx tools dynamically while g_server_num is not initialized */
	return ((unsigned int)g_server_num == (unsigned int)__SERVER_INVAL ||
		audit_is_disabled() || audit_rule_empty() ||
		is_invoked_by_sys_service_or_auditd());
}

int audit_sysif_entry(long ret_code, int argc, ...)
{
	(void)ret_code;
	(void)argc;

	int ret = init_shm();
	if (ret != E_HM_OK) {
		hm_error("failed to initialize audit, ret=%s\n", hmstrerror(ret));
		return E_HM_OK;
	}
	if (!audit_sysif_need_skip()) {
		BUG_ON(((unsigned int)g_server_num == (unsigned int)__SERVER_INVAL ||
			(unsigned int)g_server_num >= (unsigned int)__SERVER_MAX));
		audit_ctx_set_valid();
	}
	return E_HM_OK;
}
