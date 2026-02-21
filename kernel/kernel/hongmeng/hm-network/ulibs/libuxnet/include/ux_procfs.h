/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description:  Header file for network procfs
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jan 11 13:34:09 2020
 */

#ifndef __LIBUXNET_UX_PROCFS_
#define __LIBUXNET_UX_PROCFS_

#include <sys/types.h>
#include <fs_procfs.h>

#define PROCFS_MSG_MAX_LEN 4096
#define SOCKET_INFO_STR_MAX (HMCAP_THREAD_NAME_SIZE + 32) /* the printf lenth of pid fd not more than 32 */

enum procfs_net {
	PROCFS_ALL_LOG_MARTIANS,
	PROCFS_DEFAULT_LOG_MARTIANS,
	PROCFS_ALL_ACCEPT_REDIRECTS,
	PROCFS_DEFAULT_ACCEPT_REDIRECTS,
	PROCFS_ALL_SECURE_REDIRECTS,
	PROCFS_DEFAULT_SECURE_REDIRECTS,
	PROCFS_ALL_ACCEPT_SOURCE_ROUTE,
	PROCFS_DEFAULT_ACCEPT_SOURCE_ROUTE,
	PROCFS_NETSTAT_TCP,
	PROCFS_NETSTAT_UDP,
	PROCFS_NETSTAT_RAW,
	PROCFS_NETSTAT_UNIX,
	PROCFS_GET_ROUTE_INFO,
	PROCFS_TCP_SYNCOOKIES,
	PROCFS_TCP_FIN_TIMEOUT,
	PROCFS_TCP_SYNBACKLOG,
	PROCFS_GET_NETSTAT,
	PROCFS_ICMP_ECHO_IGNORE_ALL,
	PROCFS_TCP_TIMESTAMPS,
	PROCFS_ICMP_ECHO_IGNORE_BROADCASTS,
	PROCFS_ICMP_IGNORE_BOGUS_ERROR_RESPONSES,
	PROCFS_ALL_ARP_PROXY,
	PROCFS_DEFAULT_ARP_PROXY,
	PROCFS_IP_FORWARD,
	PROCFS_DEFAULT_RP_FILTER,
	PROCFS_ALL_RP_FILTER,
	PROCFS_ALL_SEND_REDIRECTS,
	PROCFS_DEFAULT_SEND_REDIRECTS,
	PROCFS_DUMP_ARPTABLE,
	PROCFS_NETSTAT_VSOCK,
};

struct common_procfs_entry {
	int (*read)(struct common_procfs_entry *entry, void *buf, size_t count);
	int (*write)(struct common_procfs_entry *entry, const void *buf, size_t count);
	char *name;
	unsigned int nsid;
	struct raw_refcnt ref;
};

struct procfs_msg {
	void *buf;
	size_t count;
	int type;
	uint32_t offset;
};

int net_procfs_dispatch_write(int tagid, const void *buf, size_t size);
int net_procfs_dispatch_read(int tagid, void *buf, size_t size);
void procfs_entry_destroy_specific(unsigned int nsid, const char *pathname);
int procfs_alloc_tagid(struct common_procfs_entry *net_entry);
void procfs_free_tagid(int tagid);
void procfs_init_callback(int tagid, struct procfs_ops_cb *cb_ops);

int procfs_create_net_file(unsigned int nsid, const char *path, mode_t mode,
			   int (*read)(struct common_procfs_entry *entry, void *buf, size_t count),
			   int (*write)(struct common_procfs_entry *entry, const void *buf, size_t count));
int procfs_create_net_dir(unsigned int nsid, const char *path);
int procfs_delete_net_file(unsigned int nsid, const char *path);

void procfs_entry_release_func(struct raw_refcnt *ref);
void procfs_entry_get(struct common_procfs_entry *entry);
void procfs_entry_put(struct common_procfs_entry *entry);
int net_do_get_socket_process_info(void *buf, size_t count, int type);

enum procfs_special_callnum {
	PROCFS_SP_SOCKFS_UNIX_NETSTAT,
	PROCFS_SP_SOCKFS_UNIX_MAX_DGRAM_QLEN,
	PROCFS_SP_MAX
};

typedef int (*special_read_fn)(void *buf, size_t count, int type);
typedef int (*special_write_fn)(const void *buf, size_t count, int type);

int ux_procfs_dispatch_special_read(enum procfs_special_callnum callnum, void *buf, size_t count, int type);
int ux_procfs_dispatch_special_write(enum procfs_special_callnum callnum, const void *buf, size_t count, int type);
void ux_procfs_register_special_func(enum procfs_special_callnum callnum,
				     special_read_fn read_fn, special_write_fn write_fn);

typedef struct ux_procfs_ops {
	int (*get_port_range)(uint16_t *start_port, uint16_t *end_port);
	int (*set_port_range)(uint16_t start_port, uint16_t end_port);
	int (*get_netstat)(char **msg, int *msg_len, int type);
	void (*release_netstat)(char *msg, int type);
	int (*get_configure)(int *val, int type);
	int (*set_configure)(const int val, int type);
} ux_procfs_ops_t;

ux_procfs_ops_t *get_procfs_ops(void);

#endif
