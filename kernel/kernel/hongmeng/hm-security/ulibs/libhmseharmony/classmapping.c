/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony source file for class mapping
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */

#include <stdlib.h>

#include <hongmeng/compiler.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmseharmony/services.h>
#include <libhmseharmony/flask.h>
#include <libhwsecurec/securec.h>

#include "services.h"
#include "policydb.h"
#include "classmapping.h"

#ifdef __HOST_LLT__
#define STATIC
#else
#define STATIC static
#endif

STATIC const struct hmsel_str_mapping secclass_map[] = {
	{ "file",
	  { HM_COMMON_FILE_PERMS,
	    "execute_no_trans", "entrypoint", NULL } },
	{ "dir",
	  { HM_COMMON_FILE_PERMS, "add_name", "remove_name",
	    "reparent", "search", "rmdir", NULL } },
	{ "chr_file",
	  { HM_COMMON_FILE_PERMS, NULL } },
	{ "fifo_file",
	  { HM_COMMON_FILE_PERMS, NULL } },
	{ "process",
	  { "fork", "transition", "sigchld", "sigkill",
	    "sigstop", "signull", "signal", "ptrace", "getsched", "setsched",
	    "getsession", "getpgid", "setpgid", "getcap", "setcap", "share",
	    "getattr", "setexec", "setfscreate", "noatsecure", "siginh",
	    "setrlimit", "rlimitinh", "dyntransition", "setcurrent",
	    "execmem", "execstack", "execheap", "setkeycreate",
	    "setsockcreate", "getrlimit", NULL } },
	{ "process2",
	  { "nnp_transition", "nosuid_transition", NULL } },
	{ "system",
	  { "ipc_info", "syslog_read", "syslog_mod",
	    "syslog_console", "module_request", "module_load", NULL } },
	{ "capability",
	  { HM_COMMON_CAP_PERMS, NULL } },
	{ "filesystem",
	  { "mount", "remount", "unmount", "getattr",
	    "relabelfrom", "relabelto", "associate", "quotamod",
	    "quotaget", "watch", NULL } },
	{ "fd", { "use", NULL } },
	{ "lnk_file",
	  { HM_COMMON_FILE_PERMS, NULL } },
	{ "blk_file",
	  { HM_COMMON_FILE_PERMS, NULL } },
	{ "sock_file",
	  { HM_COMMON_FILE_PERMS, NULL } },
	{ "security",
	  { "compute_av", "compute_create", "compute_member",
	    "check_context", "load_policy", "compute_relabel",
	    "compute_user", "setenforce", "setbool", "setsecparam",
	    "setcheckreqprot", "read_policy", "validate_trans", NULL } },
	{ "socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "tcp_socket",
	  { HM_COMMON_SOCK_PERMS,
	    "node_bind", "name_connect",
	    NULL } },
	{ "udp_socket",
	  { HM_COMMON_SOCK_PERMS,
	    "node_bind", NULL } },
	{ "rawip_socket",
	  { HM_COMMON_SOCK_PERMS,
	    "node_bind", NULL } },
	{ "node",
	  { "recvfrom", "sendto", NULL } },
	{ "netif",
	  { "ingress", "egress", NULL } },
	{ "netlink_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "packet_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "key_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "unix_stream_socket",
	  { HM_COMMON_SOCK_PERMS, "connectto", NULL } },
	{ "unix_dgram_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "sem",
	  { HM_COMMON_IPC_PERMS, NULL } },
	{ "msg", { "send", "receive", NULL } },
	{ "msgq",
	  { HM_COMMON_IPC_PERMS, "enqueue", NULL } },
	{ "shm",
	  { HM_COMMON_IPC_PERMS, "lock", NULL } },
	{ "ipc",
	  { HM_COMMON_IPC_PERMS, NULL } },
	{ "netlink_route_socket",
	  { HM_COMMON_SOCK_PERMS,
	    "nlmsg_read", "nlmsg_write", "nlmsg_readpriv", NULL } },
	{ "netlink_tcpdiag_socket",
	  { HM_COMMON_SOCK_PERMS,
	    "nlmsg_read", "nlmsg_write", NULL } },
	{ "netlink_nflog_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "netlink_xfrm_socket",
	  { HM_COMMON_SOCK_PERMS,
	    "nlmsg_read", "nlmsg_write", NULL } },
	{ "netlink_selinux_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "netlink_iscsi_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "netlink_audit_socket",
	  { HM_COMMON_SOCK_PERMS,
	    "nlmsg_read", "nlmsg_write", "nlmsg_relay", "nlmsg_readpriv",
	    "nlmsg_tty_audit", NULL } },
	{ "netlink_fib_lookup_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "netlink_connector_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "netlink_netfilter_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "netlink_dnrt_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "association",
	  { "sendto", "recvfrom", "setcontext", "polmatch", NULL } },
	{ "netlink_kobject_uevent_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "netlink_generic_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "netlink_scsitransport_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "netlink_rdma_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "netlink_crypto_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "appletalk_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "packet",
	  { "send", "recv", "relabelto", "forward_in", "forward_out", NULL } },
	{ "key",
	  { "view", "read", "write", "search", "link", "setattr", "create",
	    NULL } },
	{ "dccp_socket",
	  { HM_COMMON_SOCK_PERMS,
	    "node_bind", "name_connect", NULL } },
	{ "memprotect", { "mmap_zero", NULL } },
	{ "peer", { "recv", NULL } },
	{ "capability2",
	  { HM_COMMON_CAP2_PERMS, NULL } },
	{ "kernel_service", { "use_as_override", "create_files_as", NULL } },
	{ "tun_socket",
	  { HM_COMMON_SOCK_PERMS, "attach_queue", NULL } },
	{ "binder", { "impersonate", "call", "set_context_mgr", "transfer",
		      "actv_binder_service", "actv_binder_call", NULL } },
	{ "cap_userns",
	  { HM_COMMON_CAP_PERMS, NULL } },
	{ "cap2_userns",
	  { HM_COMMON_CAP2_PERMS, NULL } },
	{ "sctp_socket",
	  { HM_COMMON_SOCK_PERMS,
	    "node_bind", "name_connect", "association", NULL } },
	{ "icmp_socket",
	  { HM_COMMON_SOCK_PERMS,
	    "node_bind", NULL } },
	{ "ax25_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "ipx_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "netrom_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "atmpvc_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "x25_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "rose_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "decnet_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "atmsvc_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "rds_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "irda_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "pppox_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "llc_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "can_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "tipc_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "bluetooth_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "iucv_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "rxrpc_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "isdn_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "phonet_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "ieee802154_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "caif_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "alg_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "nfc_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "vsock_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "kcm_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "qipcrtr_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "smc_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "infiniband_pkey",
	  { "access", NULL } },
	{ "infiniband_endport",
	  { "manage_subnet", NULL } },
	{ "bpf",
	  { "map_create", "map_read", "map_write", "prog_load", "prog_run",
	    NULL } },
	{ "xdp_socket",
	  { HM_COMMON_SOCK_PERMS, NULL } },
	{ "perf_event",
	  { "open", "cpu", "kernel", "tracepoint", "read", "write", NULL } },
	{ "lockdown",
	  { "integrity", "confidentiality", NULL } },
	{ "xpm",
	  { "exec_no_sign", "exec_anon_mem", "exec_in_jitfort", "exec_allow_debug_id", "exec_allow_sa_plugin", NULL } },
	{ "hmpsf",
	  { "map_create", "map_read", "map_write", "module_load", "module_run",
	    NULL } },
	{ "code_sign",
	  {"add_cert_chain", "remove_cert_chain", NULL }},
	{ "hmprobe",
	  { "event_open", "event_attach", "NULL"} },
	{ "hmcap",
	  {"supervsable", "pid_mem_read", "pid_mem_write", "exec_non_lsyscall", NULL } },
	{ "vsock_host_socket",
	  {HM_COMMON_SOCK_PERMS, NULL } },
	{ "dmaheap",
	  { "iris_heap", NULL } },
	{ NULL }
};

raw_static_assert((HM_SECCLASS_NUM) > 1, secclass_macro_num_valid);
raw_static_assert((HM_SECCLASS_NUM) == (sizeof(secclass_map) / sizeof(secclass_map[0])), secclass_num_valid);

static int mapping_input_check(const struct hmsel_policydb *db, const struct hmsel_map *map)
{
	int rc = E_HM_OK;
	if (db == NULL || map == NULL) {
		rc = E_HM_INVAL;
		hm_error("failed to check hmsel class mapping input\n");
	}
	return rc;
}

static int mapping_alloc(struct hmsel_map *class_map)
{
	int rc = E_HM_OK;
	int rc1;
	size_t map_size = sizeof(secclass_map) / sizeof(secclass_map[0]);
	size_t total_size = sizeof(struct hmsel_mapping) * map_size;
#ifdef CONFIG_HKIP_SEHARMONY_PROT
	class_map->mapping = (struct hmsel_mapping *)hmsel_prmem_malloc(total_size);
#else
	class_map->mapping = (struct hmsel_mapping *)malloc(total_size);
#endif
	if (class_map->mapping == NULL) {
		rc = E_HM_NOMEM;
		hm_error("failed to alloc class map\n");
	} else {
		rc1 = memset_s(class_map->mapping, total_size, 0, total_size);
		if (rc1 != EOK) {
			hm_error("failed to initialize class mapping data\n");
#ifdef CONFIG_HKIP_SEHARMONY_PROT
			hmsel_prmem_free(class_map->mapping);
#else
			free(class_map->mapping);
#endif
			class_map->mapping = NULL;
			rc = E_HM_INVAL;
		} else {
			class_map->size = map_size;
		}
	}
	return rc;
}

static int mapping_class_entry_setup(struct hmsel_policydb *policydb,
				const struct hmsel_str_mapping *str_map, struct hmsel_mapping *val_map)
{
	int rc = E_HM_OK;
	val_map->value = hmsel_str_to_class(policydb, str_map->name);
	if(val_map->value == 0) {
		hm_warn("class not defined in seharmony policy, class=%s\n", str_map->name);
		if (hmsel_policydb_reject_unknown(policydb) == 1) {
			rc = E_HM_INVAL;
		} else {
			val_map->num_perms = 0;
		}
	} else {
		hm_debug("class %s set to %"PRIu16"\n", str_map->name, val_map->value);
	}
	return rc;
}

static int mapping_perm_setup(struct hmsel_policydb *policydb,
			const struct hmsel_str_mapping *str_map, struct hmsel_mapping *val_map)
{
	int rc = E_HM_OK;
	unsigned int perm_num = 0;
	while (str_map->perms[perm_num] != NULL) {
		if (strcmp(str_map->perms[perm_num], "") == 0) {
			perm_num++;
			continue;
		}
		val_map->perms[perm_num] = hmsel_str_to_perm(policydb, val_map->value, str_map->perms[perm_num]);
		if (val_map->perms[perm_num] == 0) {
			hm_warn("perm %s for class %s not defined in seharmony policy\n",
				str_map->perms[perm_num], str_map->name);
			if (hmsel_policydb_reject_unknown(policydb) == 1) {
				rc = E_HM_INVAL;
				break;
			}
		} else {
			hm_debug("perm %s of class %s set to %"PRIu32", idx %u\n",
				 str_map->perms[perm_num], str_map->name, val_map->perms[perm_num], perm_num);
		}
		perm_num++;
	}
	val_map->num_perms = perm_num;
	return rc;
}

static int mapping_do_setup(struct hmsel_policydb *policydb, struct hmsel_map *class_map)
{
	int rc = E_HM_OK;
	uint32_t class_num = 0;
	const struct hmsel_str_mapping *str_map = &secclass_map[0];
	size_t map_size = sizeof(secclass_map) / sizeof(secclass_map[0]);
	while (class_num < map_size - 1) {
		if (str_map->name == NULL) {
			hm_error("illegal class define %u\n", class_num);
			rc = E_HM_INVAL;
			break;
		}
		class_num++;
		struct hmsel_mapping *val_map = class_map->mapping + class_num;
		if (strcmp(str_map->name, "") == 0) {
			val_map->num_perms = 0;
			str_map++;
			continue;
		}
		rc = mapping_class_entry_setup(policydb, str_map, val_map);
		if (rc != E_HM_OK) {
			break;
		}
		if (val_map->value != 0) {
			rc = mapping_perm_setup(policydb, str_map, val_map);
			if (rc != E_HM_OK) {
				break;
			}
		}
		str_map++;
	}
	return rc;
}

int hmsel_set_mapping(struct hmsel_policydb *policydb, struct hmsel_map *class_map)
{
	int rc = mapping_input_check(policydb, class_map);
	if (rc == E_HM_OK) {
		rc = mapping_alloc(class_map);
	}
	if (rc == E_HM_OK) {
		rc = mapping_do_setup(policydb, class_map);
		if (rc != E_HM_OK) {
			hmsel_free_mapping(class_map);
		}
	}
	return rc;
}

const char *hmsel_class_name(hmsel_class_t tclass)
{
	if (tclass > HM_SECCLASS_NULL &&
	    tclass < (sizeof(secclass_map) / sizeof(secclass_map[0]))) {
		return secclass_map[tclass - 1].name;
	}
	return "unknown";
}

#ifdef CONFIG_SEHARMONY_AC_CACHE_STAT
size_t hmsel_class_num(void)
{
	return sizeof(secclass_map) / sizeof(secclass_map[0]);
}
#endif

#define BITS_WIDE (8u)
static void do_perms_log(hmsel_class_t class, hmsel_av_t *requested, char *buff, size_t size)
{
	unsigned int i = 0u;
	hmsel_av_t tmp = 1;
	size_t psize = 0;
	int count = 0;
	int ret;
	while (*requested != 0 && (i < sizeof(*requested) * BITS_WIDE)) {
		if ((*requested & tmp) && (secclass_map[class - 1].perms[i] != NULL)) {
			count++;
			if (count == 1) {
				ret = snprintf_s(buff + psize, size - psize, (size - psize) - 1,
						 "%s", secclass_map[class - 1].perms[i]);
			} else {
				ret = snprintf_s(buff + psize, size - psize, (size - psize) - 1,
						 " %s", secclass_map[class - 1].perms[i]);
			}
			if (ret < 0) {
				hm_error("failed to write sel log\n");
				return;
			}
			psize += (size_t)(unsigned int)ret;
			*requested &= ~tmp;
		}
		i++;
		tmp <<= 1;
	}
}

void hmsel_perms(hmsel_class_t class, hmsel_av_t *requested, char *buff, size_t size)
{
	if (class > HM_SECCLASS_NULL &&
	    class < (sizeof(secclass_map) / sizeof(secclass_map[0]))) {
		do_perms_log(class, requested, buff, size);
	} else {
		(void)snprintf_s(buff, size, size - 1, "%s", "unknown class perm");
	}
}

#ifdef CONFIG_SEHARMONY_POLICY_SPLIT

#ifndef BITS_PER_BYTE
#define BITS_PER_BYTE (8u)
#endif
#define CLASS_MAPSIZE ((HM_SECCLASS_NUM / BITS_PER_BYTE) + 1u)

static unsigned char class_bitmap[CLASS_MAPSIZE];

static void bitmap_set_bit(unsigned char *map, hmsel_class_t class)
{
	unsigned int byte_off = (unsigned int)(class / BITS_PER_BYTE);
	unsigned int bit_off = (unsigned int)(class % BITS_PER_BYTE);
	map[byte_off] |= (1u << bit_off);
}

static void class_bitmap_set_bit(hmsel_class_t class)
{
	bitmap_set_bit(&class_bitmap[0], class);
}

static bool bitmap_test_bit(unsigned char *map, hmsel_class_t class)
{
	unsigned int byte_off = (unsigned int)(class / BITS_PER_BYTE);
	unsigned int bit_off = (unsigned int)(class % BITS_PER_BYTE);
	return ((map[byte_off] & (1u << bit_off)) != 0);
}

static bool class_bitmap_test_bit(hmsel_class_t class)
{
	return bitmap_test_bit(&class_bitmap[0], class);
}

#define CLASS_SET(class) class_bitmap_set_bit(class)

static void class_bitmap_init_all(void)
{
	/* HM_SECCLASS_NUM bit set to diff load full and split */
	class_bitmap_set_bit(HM_SECCLASS_NUM);
}

static void class_bitmap_init_sysmgr(void)
{
#include "class_bitmap_sysmgr.h"
}

static void class_bitmap_init_vfs(void)
{
#include "class_bitmap_vfs.h"
}

static void class_bitmap_init_network(void)
{
#include "class_bitmap_network.h"
}

static void class_bitmap_init_devmgr(void)
{
#include "class_bitmap_devmgr.h"
}

static void class_bitmap_init_devhost(void)
{
#include "class_bitmap_devhost.h"
}

static void class_bitmap_init_pwrmgr(void)
{
#include "class_bitmap_pwrmgr.h"
}

static void class_bitmap_init_keymgr(void)
{
#include "class_bitmap_keymgr.h"
}

static const struct hmsel_policydb *map_db = NULL;
void class_bitmap_init(unsigned int service, const struct hmsel_policydb *db)
{
	(void)memset_s(&class_bitmap[0], sizeof(class_bitmap), 0, sizeof(class_bitmap));
	if ((service & SEC_SERVICE_ALL) != 0u) {
		class_bitmap_init_all();
	} else {
		if ((service & SEC_SERVICE_SYSMGR) != 0u) {
			class_bitmap_init_sysmgr();
		}
		if ((service & SEC_SERVICE_VFS) != 0u) {
			class_bitmap_init_vfs();
		}
		if ((service & SEC_SERVICE_NETWORK) != 0u) {
			class_bitmap_init_network();
		}
		if ((service & SEC_SERVICE_DEVMGR) != 0u) {
			class_bitmap_init_devmgr();
		}
		if ((service & SEC_SERVICE_DEVHOST) != 0u) {
			class_bitmap_init_devhost();
		}
		if ((service & SEC_SERVICE_PWRMGR) != 0u) {
			class_bitmap_init_pwrmgr();
		}
		if ((service & SEC_SERVICE_KEYMGR) != 0u) {
			class_bitmap_init_keymgr();
		}
	}
	map_db = db;
}

void class_bitmap_fini(void)
{
	(void)memset_s(&class_bitmap[0], sizeof(class_bitmap), 0, sizeof(class_bitmap));
	map_db = NULL;
}

static int class_bitmap_split_setup(unsigned char *map_policy, size_t nprim)
{
	if (map_db == NULL || map_policy == NULL) {
		hm_error("class split setup input invalid\n");
		return E_HM_INVAL;
	}
	int rc = E_HM_OK;
	hmsel_class_t class_policy;
	hmsel_class_t class_sys = 1u;
	const struct hmsel_str_mapping *str_map = &secclass_map[0];
	size_t map_size = sizeof(secclass_map) / sizeof(secclass_map[0]);
	while (class_sys < map_size) {
		if (str_map->name == NULL) {
			hm_error("illegal class define %u\n", (unsigned int)class_sys);
			rc = E_HM_INVAL;
			break;
		}
		if (class_bitmap_test_bit(class_sys)) {
			if (strcmp(str_map->name, "") == 0) {
				hm_error("class %u in bitmap and policy not match\n", (unsigned int)class_sys);
				rc = E_HM_INVAL;
				break;
			}
			class_policy = hmsel_str_to_class(map_db, str_map->name);
			if (class_policy != 0 && class_policy <= nprim) {
				bitmap_set_bit(map_policy, class_policy);
				hm_debug("seharmony class %s to register in bitmap, sys %u, policy %u\n",
					 str_map->name, (unsigned int)class_sys, (unsigned int)class_policy);
			}
		}
		str_map++;
		class_sys++;
	}
	return rc;
}

static int class_bitmap_full_setup(unsigned char *map_policy, size_t nprim)
{
	if (map_policy == NULL) {
		hm_error("class full setup input invalid\n");
		return E_HM_INVAL;
	}

	hmsel_class_t class = 1u;
	while (class <= nprim) {
		bitmap_set_bit(map_policy, class);
		hm_debug("seharmony class %u to register in bitmap", (unsigned int)class);
		class++;
	}
	return E_HM_OK;
}

int class_split_setup(unsigned char *map_policy, size_t nprim)
{
	if (class_bitmap_test_bit(HM_SECCLASS_NUM)) {
		return class_bitmap_full_setup(map_policy, nprim);
	}
	return class_bitmap_split_setup(map_policy, nprim);
}
#endif
