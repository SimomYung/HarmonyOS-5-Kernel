/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony head file for classmapping define
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */

#ifndef HMSEL_CLASSMAPPING_H
#define HMSEL_CLASSMAPPING_H

#ifdef CONFIG_SEHARMONY_POLICY_SPLIT
#include <libsec/service_management.h>
#endif
#include <libhmseharmony/services.h>
#include <sys/types.h>

#include "services.h"

#define CLASS_PERM_MAX 8

struct hmsel_str_mapping {
	const char *name;
	const char *perms[sizeof(uint32_t) * CLASS_PERM_MAX + 1];
};

int hmsel_set_mapping(struct hmsel_policydb *policydb, struct hmsel_map *class_map);
const char *hmsel_class_name(hmsel_class_t class);
void hmsel_perms(hmsel_class_t class, hmsel_av_t *requested, char *buff, size_t size);
#ifdef CONFIG_SEHARMONY_AC_CACHE_STAT
size_t hmsel_class_num(void);
#endif
#ifdef CONFIG_SEHARMONY_POLICY_SPLIT
void class_bitmap_init(unsigned int service, struct  hmsel_policydb *db);
void class_bitmap_fini(void);
int class_split_setup(unsigned char *map_policy, size_t len);
#endif

#define HM_COMMON_FILE_SOCK_PERMS "ioctl", "read", "write", "create", \
	"getattr", "setattr", "lock", "relabelfrom", "relabelto", "append", "map"

#define HM_COMMON_FILE_PERMS HM_COMMON_FILE_SOCK_PERMS, "unlink", "link", \
	"rename", "execute", "quotaon", "mounton", "audit_access", \
	"open", "execmod", "watch", "watch_mount", "watch_sb", \
	"watch_with_perm", "watch_reads"

#define HM_COMMON_SOCK_PERMS HM_COMMON_FILE_SOCK_PERMS, "bind", "connect", \
	"listen", "accept", "getopt", "setopt", "shutdown", "recvfrom",  \
	"sendto", "name_bind"

#define HM_COMMON_IPC_PERMS "create", "destroy", "getattr", "setattr", "read", \
	"write", "associate", "unix_read", "unix_write"

#define HM_COMMON_CAP_PERMS  "chown", "dac_override", "dac_read_search", \
	"fowner", "fsetid", "kill", "setgid", "setuid", "setpcap", \
	"linux_immutable", "net_bind_service", "net_broadcast", "net_admin", \
	"net_raw", "ipc_lock", "ipc_owner", "sys_module", \
	"sys_rawio", "sys_chroot", "sys_ptrace", "sys_pacct", \
	"sys_admin", "sys_boot", "sys_nice", "sys_resource", "sys_time", \
	"sys_tty_config", "mknod", "lease", "audit_write", \
	"audit_control", "setfcap"

#define HM_COMMON_CAP2_PERMS  "mac_override", "mac_admin", "syslog", \
	"wake_alarm", "block_suspend", "audit_read", "perfmon", "bpf", \
	"checkpoint_restore"

static inline void hmsel_free_mapping(struct hmsel_map *class_map)
{
	if (class_map != NULL) {
#ifdef CONFIG_HKIP_SEHARMONY_PROT
		hmsel_prmem_free(class_map->mapping);
#else
		free(class_map->mapping);
#endif
		class_map->mapping = NULL;
		class_map->size = 0;
	}
}

#endif
