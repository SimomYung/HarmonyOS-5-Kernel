/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Declarations of Audit log
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 17 11:46:29 2020
 */
#ifndef ULIBS_LIBHMAUDIT_AUDIT_LOG_H
#define ULIBS_LIBHMAUDIT_AUDIT_LOG_H

#include <stdint.h>
#include <sys/types.h>
#include <sys/time.h>
#include <libhmaudit/audit.h>

/* size of buffer inside audit log, we support 1024 bytes long format string */
#define AUDIT_LOG_BUF_SIZE	1024

#define TYPE_AUDIT_SYSCALL	0x00000001
#define TYPE_AUDIT_MAC		0x00000002
#define TYPE_AUDIT_PID		0x00000004
#define TYPE_AUDIT_DEVINFO	0x00000008
#define TYPE_AUDIT_DAC		0x00000010
#define TYPE_AUDIT_COMMON	0x00000020
#define TYPE_AUDIT_SESSION	0x00000040
/*
 * NOTE: store formatted string in audit log data,
 * should be used after the other types, since the sequence of data
 * can NOT be guaranteed.
 */
#define TYPE_AUDIT_FORMAT	0x00000080
#define TYPE_AUDIT_SOCKADDR	0x00000100
#define TYPE_AUDIT_FD_PAIR	0x00000200
#define TYPE_AUDIT_PATH		0x00000400

#define TYPE_AUDIT_ONE_ABLTY	1
#define TYPE_AUDIT_GROUP_ABLTY	2
#define TYPE_AUDIT_MAC_ACTION	3

#define TYPE_AUDIT_MAC_GRANTED	0
#define TYPE_AUDIT_MAC_DENIED	1

#define ABILITY_GRP_NUM		2
#define AUDIT_SYSCALL_ARGC	4
#define AUDIT_NET_FD_COUNT   2

typedef struct {
	uint32_t arch;
	uint32_t syscall_num;
	uint64_t argv[AUDIT_SYSCALL_ARGC];
	uint64_t exit;
} audit_syscall_items;

typedef struct {
	pid_t ppid;
	pid_t pid;
	uint32_t cwd_len;
	uint32_t exe_len;
	uint32_t comm_len;
	uint32_t tty_len;
} audit_pid_items;

typedef struct {
	uint32_t arch;
	uint32_t hostname_len;
	uint32_t terminal_len;
	uint32_t vmname_len;
} audit_devinfo_items;

typedef struct {
	gid_t gid;
	uid_t uid;
	gid_t egid;
	uid_t euid;
	gid_t sgid;
	uid_t suid;
	gid_t fsgid;
	uid_t fsuid;
	uint32_t acct_len;
} audit_dac_items;

typedef struct {
	uint32_t field_count;
	uint32_t watch_len;
} audit_watch_items;

typedef struct {
	uid_t loginuid;
	uint32_t session_id;
} audit_session_items;

typedef struct {
	int fd[AUDIT_NET_FD_COUNT];
	uint32_t addr_len;
} audit_socket_items;

typedef struct {
	size_t avc_len;
	uint32_t comm_len;
	uint64_t subj;
	uint64_t obj;
} audit_avc_items;

struct audit_log {
	struct timespec ts;
	uint32_t audit_type;
	uint32_t total_len; /* sizeof(audit_entry) + strlen(*) + ... */
	uint32_t cnode_idx;
	uint32_t serial;
	uint64_t rule_id;
	uint32_t flag;
	audit_session_items session;
	audit_syscall_items syscall;
	audit_pid_items pid;
	audit_devinfo_items devinfo;
	audit_dac_items dac;
	audit_watch_items watch;
	audit_socket_items sock;
	audit_avc_items avc;
	// char cwd[] + char exe[] + char comm[] + char hostname[] +
	// char terminal[] + char vmname[] + char acct[]
	char data[0];
};

static inline size_t audit_log_tail_size(struct audit_log *log)
{
	return sizeof(struct audit_log) + AUDIT_LOG_BUF_SIZE - log->total_len;
}

static inline char *audit_log_tail(const struct audit_log *log)
{
	return (char *)(log->data) + log->total_len - sizeof(struct audit_log);
}

int audit_log_write(const struct audit_log *log);
struct audit_log *audit_log_gen(struct audit_ctx *ctx, uint32_t audit_type);
struct audit_log *audit_log_alloc(uint32_t audit_type, uint32_t flag);
int audit_log_fmt(struct audit_log *log, const char *fmt, ...);
void audit_log_key(struct audit_log *log, const char *key);
int audit_log_send(uint32_t audit_type, uint32_t flag, const char *fmt, ...);
int audit_log_vsend(uint32_t audit_type, uint32_t flag, const char *fmt, va_list args);

#endif /* ifndef ULIBS_LIBHMAUDIT_AUDIT_LOG_H */
