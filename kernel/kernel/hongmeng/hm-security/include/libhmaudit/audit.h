/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Declarations of libhmaudit APIs, used by internal system services.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 17 11:46:29 2020
 */
#ifndef ULIBS_LIBHMAUDIT_AUDIT_H
#define ULIBS_LIBHMAUDIT_AUDIT_H
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <lib/dlist.h>
#include <libsec/cred.h>
#include <libhmsrv_audit/audit.h>
#include <libhmaudit/rule.h>
#include <libsysif/base/macro.h>
#include <libhmsrv_audit/server_audit_num.h>
#include <libhmsrv_sys/hm_security.h>
#include <libhmsync/raw_scopedptr.h>
#include <libsec/cred.h>
#include <hmkernel/const.h>
#include <sys/socket.h>

#define AUDIT_INVALID_RULE_ID 0u
#define AUDIT_SYSCALL_ARGC    4
#define AUDIT_TRACE_CLASS_NAME "audit_trace"
#define AUDIT_NET_FD_COUNT   2

#define HM_AUDIT_FS_WATCH_FILE   0x01
#define HM_AUDIT_FS_WATCH_DIR    0x02
#define HM_AUDIT_FS_WATCH_APPEND 0x04
#define HM_AUDIT_FS_WATCH_NEW    0x08
#define HM_AUDIT_FS_WATCH_DEL    0x10
#define HM_AUDIT_FS_DIR_ROOT     0x20
#define HM_AUDIT_FS_IS_FWD       0x40

#define HM_AUDIT_RULE_STATUS_NOMAL		1
#define HM_AUDIT_RULE_STATUS_WAIT_FREE		2

#define HM_AUDIT_WATCH_FIELD_MAX_NUM		8

#define AUDIT_SERIAL_HEADER_OFFSET	20
#define AUDIT_KERNEL_SERIAL_HEADER	0xFFF
#define AUDIT_SYSMGR_SERIAL_HEADER	0xFFE
#define AUDIT_SERIAL_HEADER_MAX		0xFFD

#define AUDIT_CONTEXT_STATUS_VALID	0x1
#define AUDIT_INVALID_ME_IDX	        (-1)

#define AUDIT_PROCTITLE_NULL_MSG	"(null)"
#define AUDIT_PROCTITLE_NULL_LEN	(sizeof("(null)") - 1)

#define AUDIT_VFS_SERVER_NAME_MAX_LEN	32
#define AUDIT_SOCK_ADDR_LEN	(sizeof(struct sockaddr_storage))

enum AUDIT_RECORD_STATE {
	AUDIT_BUILD    = 0,
	AUDIT_RECORD   = 1,
	AUDIT_DISABLED = 2
};

extern struct dlist_node audit_filter_list[HM_AUDIT_NR_FILTERS];

struct audit_watch_field {
	dev_t dev_id;
	uint32_t index;
	dev_t devno;
	int type;
	char *path;
	char *full_path;
	size_t path_len;
	char srv_name[AUDIT_VFS_SERVER_NAME_MAX_LEN];
};

struct audit_watch_dir_field {
	int me_idx;
	uint32_t field_index;
};

struct audit_watch_del_dir {
	int me_idx;
	char *path;
	char *full_path;
};

struct audit_watch_del_ctx {
	struct audit_watch_del_dir del_dir[HM_AUDIT_WATCH_FIELD_MAX_NUM];
	uint32_t del_dir_count;
};

struct audit_watch_ctx {
	struct audit_watch_field *field[HM_AUDIT_WATCH_FIELD_MAX_NUM];
	uint32_t field_count;
	struct audit_watch_dir_field dir_field[HM_AUDIT_WATCH_FIELD_MAX_NUM];
	uint32_t dir_field_count;
	rref_t rref;
	const struct audit_watch_field *del_field;
};

struct audit_socket_ctx {
	int fd[AUDIT_NET_FD_COUNT];
	uint32_t addr_len;
	char addr[AUDIT_SOCK_ADDR_LEN];
};

struct audit_cmdline_info {
	char *cmdline;
	size_t cmdline_size;
};

struct audit_avc_info {
	char *avc_str;
	size_t avc_size;
	unsigned int avc_type;
	uint64_t subj;
	uint64_t obj;
	union {
		struct {
			unsigned int hmobj_type;
			unsigned char req_cright;
		} cright;
		unsigned long long req_cpermission;
	};
};

struct audit_tty_info {
	char *tty;
	uint32_t tty_len;
};

struct audit_ctx {
	uint32_t ctx_flag;
	bool invoked_by_sys; /* is invoked by sys service */
	enum AUDIT_RECORD_STATE record_state;
	uint64_t rule_id;
	unsigned int syscall_num;
	struct timespec timestamp; /* time of syscall entry */
	uint32_t arch;
	uint32_t cnode_idx;
	uint32_t serial;
	unsigned long argv[AUDIT_SYSCALL_ARGC]; /* syscall arguments */
	long ret_code; /* syscall return code */
	int ret_valid; /* return code is valid */
	pid_t pid, ppid;
	uid_t uid, euid, suid, fsuid;
	uid_t login_uid, session_id;
	gid_t gid, egid, sgid, fsgid;
	int perm;
	struct audit_watch_ctx watch;
	struct audit_socket_ctx sock;
	struct audit_cmdline_info proctitle;
	struct audit_avc_info avc_info;
	struct sec_chk_cred cred;
	struct sec_cred_basic basic;
	struct audit_tty_info tty;
};

struct audit_field {
	uint32_t type;
	uint32_t val;
	uint32_t op;
};

struct audit_watch {
	uint32_t type;
	size_t len;
	const char *path;
	struct raw_refcnt_nr refcnt;
	struct dlist_node node;
	struct dlist_node rlist; /* list of rules that reference the watch */
	struct shl_list_node *shared_watch;
};

struct audit_rule_common {
	uint64_t id;
	uint32_t flags;
	uint32_t list_num;
	uint32_t action;
	uint32_t mask[HM_AUDIT_BITMASK_SIZE]; /* syscall num */
	uint32_t field_count;
	char *filterkey; /* ties events to rules */
	struct audit_field *fields;
	struct shl_list_node *shared_rule;
	struct dlist_node dlist; /* entry in audit_rule_list */
	struct dlist_node hlist; /* entry in audit watch list */
	uint32_t status;
};

struct audit_watch_file {
	struct audit_watch aw;
	dev_t dev_id;
	uint32_t index;
};

struct audit_mount_entry {
	struct dlist_node rel_list;
};

struct audit_watch_dir {
	struct audit_watch aw;
	dev_t dev_id;
	uint32_t index;
	struct dlist_node rel_list;
};

struct audit_watch_dir_rel {
	int me_id;
	bool is_root_node;
	struct audit_watch_dir *dir;
	struct audit_mount_entry *ame;
	struct dlist_node dir_node;  /* entry in audit_watch_dir.rel_list */
	struct dlist_node ame_node;  /* entry in audit_mount_entry.rel_list */
	struct raw_refcnt_nr refcnt;
};

struct audit_fs_rule {
	struct audit_rule_common common;
	union {
		struct audit_watch_file *watch_file;
		struct audit_watch_dir *watch_dir;
	};
	struct dlist_node watch_node; /* entry in audit watch aw.rlist */
};

struct audit_rule {
	struct audit_rule_common common;
	struct audit_watch *watch;
};

struct audit_match {
	int msgtype;
	struct audit_ctx *ctx;
};

#define AUDIT_CRED_FIELD 0
#define AUDIT_OTHE_FIELD 1

struct audit_fs_node_keys {
	dev_t dev_id;
	uint32_t index;
	dev_t devno;
	int me_id;
	const char *path;
};

typedef int (*audit_get_cmdline_hook_t)(struct audit_ctx *ctx, struct audit_cmdline_info *info);

static inline bool audit_is_disabled(void)
{
	return *hm_sec_shconf_audit() == HM_AUDIT_OFF;
}

static inline bool audit_is_enabled(void)
{
	return !audit_is_disabled();
}

static inline bool is_auditd(uint32_t pid)
{
	return *hm_sec_shconf_audit_pid() == pid;
}

static inline bool audit_sw_is_disabled(void)
{
	return *hm_sec_shconf_audit_sw() == HM_SEC_SHCONF_AUDIT_SW_DISABLE;
}

static inline bool audit_feature_is_set(uint32_t feature)
{
	return ((*hm_sec_shconf_audit_features() & HM_AUDIT_FEATURE_TO_MASK(feature)) != 0);
}

/* initialize shared ring buffer */
int audit_trace_init(void);

/* initialize share memory cfg info */
int audit_lazy_init(void);
/* set g_server_num for each actv */
void audit_server_num_set(enum __server_audit_numbers num);
/* initialize audit module and ring buffer together */
int audit_module_init(void);
/* set hook method in sysif exit */
void audit_sysif_hook_set(void);
void audit_sysif_sw_hook_set(void);

int audit_rule_filter(int msgtype, unsigned int listtype, struct audit_ctx *ctx);
int audit_field_match(const struct audit_field *field, const struct audit_match *match);
struct audit_shared_rule;
int audit_watch_info_match(const struct audit_shared_rule *rule,
			   struct audit_match *match);
void audit_ctx_reset(struct audit_ctx *ctx);

#define MATCH_RES_NOT_MATCHED 0
#define MATCH_RES_MATCHED 1
#define MATCH_RES_END 2

#define HM_AUDIT_SYSIF_INVALID 0
#define HM_AUDIT_SYSIF_SUCCESS 1
#define HM_AUDIT_SYSIF_FAILURE 2

int audit_watch_insert_rule(uint16_t rule_id, uint16_t rule_num, uint32_t flags, struct audit_fs_node_keys *node_key);
int audit_watch_delete_rule(uint64_t rule_id, uint32_t flags, struct audit_fs_node_keys *vfs_node_key);
/* used before sysif exit, filter matched rule and send out audit log */
int audit_sysif_exit(long ret_code, int argc, ...);
int audit_sysif_entry(long ret_code, int argc, ...);
enum __server_audit_numbers audit_server_num_get(void);

void audit_update_rule(const char *path, dev_t dev_id, uint32_t index, bool del);

struct audit_filter_rule {
	uint64_t rule_id;
	uint32_t action;
	uint32_t res;
	uint32_t mask[HM_AUDIT_BITMASK_SIZE]; /* syscall num */
	uint32_t pre_count;
	uint32_t post_count;
	struct audit_field *prefilter;
	struct audit_field *postfilter;
	char *filterkey;
	struct dlist_node node;
};

int audit_cred_field_match(const struct audit_field *field, struct audit_ctx *ctx);

int audit_watch_dir_init(uint32_t max_me_num);

int audit_watch_insert_rule_rel(uint16_t rule_id, uint16_t rule_num, int me_id);
int audit_watch_delete_rule_rel(uint64_t rule_id, int me_id);
void audit_mount_entry_del(int me_id);

bool audit_ctx_watch_field_exist(const struct audit_watch_field *new);
int audit_ctx_watch_field_add(struct audit_watch_field *field, uint32_t *field_index);
void audit_ctx_perm_fill(int perm);
struct audit_watch_field *audit_watch_field_new(void);
int audit_watch_dir_lookup(int me_id, dev_t dev_id, uint32_t index, bool is_mp);
void audit_ctx_del_dir_fill(char *path, int me_idx, int pos);
void audit_ctx_meinfo_fill(int me_idx, uint32_t field_index);

int audit_watch_file_match(uint64_t rule_id, const struct audit_watch_field *awf);
int audit_watch_dir_match(uint64_t rule_id, int me_id);
void audit_watch_dir_rule_refresh(struct audit_watch_del_ctx *w_ctx);

struct audit_ctx *audit_ctx_get(void);
struct audit_watch_del_ctx *audit_watch_del_ctx_get(void);
uint32_t audit_get_serial(void);
void audit_ctx_set_serial(struct audit_ctx *ctx);
int audit_ctx_fill_timestamp(struct audit_ctx *ctx);
int audit_init_serial(uint32_t serial_header);
void audit_proctitle_set_hook(audit_get_cmdline_hook_t func);
void audit_ctx_fill_proctitle(struct audit_ctx *ctx);

static inline void audit_ctx_set_valid(void)
{
	struct audit_ctx *ctx = audit_ctx_get();
	ctx->ctx_flag |= AUDIT_CONTEXT_STATUS_VALID;
	return;
}

static inline void audit_ctx_set_invalid(void)
{
	struct audit_ctx *ctx = audit_ctx_get();
	ctx->ctx_flag &= ~AUDIT_CONTEXT_STATUS_VALID;
	return;
}

static inline bool audit_ctx_is_valid(void)
{
	struct audit_ctx *ctx = audit_ctx_get();
	return (bool)((ctx->ctx_flag & (uint32_t)AUDIT_CONTEXT_STATUS_VALID) != 0);
}

static inline bool audit_ctx_4_fs_is_valid(void)
{
	return audit_ctx_is_valid() || (audit_watch_del_ctx_get()->del_dir_count > 0);
}

static inline bool audit_ctx_is_invoked_by_sys(void)
{
	struct audit_ctx *ctx = audit_ctx_get();
	return ctx->invoked_by_sys;
}

bool audit_is_no_fs_dir_rule(void);
bool audit_is_no_fs_rule(void);
static inline void audit_ctx_fill_proctitle_nullmsg(struct audit_ctx *ctx)
{
	ctx->proctitle.cmdline = AUDIT_PROCTITLE_NULL_MSG;
	ctx->proctitle.cmdline_size = AUDIT_PROCTITLE_NULL_LEN;
	return;
}

bool audit_rule_empty(void);
int audit_syscall_match(const uint32_t *mask,
			unsigned int syscall_num);
int audit_watch_rule_iterate(const struct audit_watch_field *awf, struct audit_match *match);

int audit_shared_watch_field_init(bool create, uint32_t cnode_idx, struct audit_shm **new_shm);
int audit_shared_wf_default_init(bool create, uint32_t cnode_idx);
void audit_watch_point_start(void);
void audit_watch_point_end(bool is_exec, const char *path, const char *srv_name);
void audit_fill_field_2_shm_field(int me_idx, int perm,
				  const struct audit_watch_field *field,
				  struct audit_shm *wf_shm);
void audit_fill_del_field(const struct audit_watch_field *field);
int audit_field_cmp_signed(long left, uint32_t op, long right);
int audit_field_cmp(unsigned long left, uint32_t op, unsigned long right);
#endif /* ifndef ULIBS_LIBHMAUDIT_AUDIT_H */
