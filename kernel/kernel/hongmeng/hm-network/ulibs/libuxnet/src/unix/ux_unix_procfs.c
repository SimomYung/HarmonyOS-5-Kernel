/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Source file unix procfs
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct  5 11:02:56 2023
 */

#include "ux_netlog.h"
#include "ux_procfs.h"
#include "ux_socket.h"
#include "ux_namespace.h"
#include "ux_unix_procfs.h"
#include "osal.h"

#define	DGRAM_QLEN_THRD_VALUE	4000
static int max_dgram_qlen = 60000;    /* 60000 is the default max queue len */

int ux_max_dgram_qlen(void)
{
	return max_dgram_qlen + DGRAM_QLEN_THRD_VALUE;
}

#ifdef CONFIG_NET_NS
#ifdef CONFIG_NETFS_FUSION_PROCFS
static int sockfs_do_set_max_dgram_qlen(const void *buf, size_t count, int type)
{
	int ret;
	struct server_info *si = NULL;

	ret = get_server_info("fsmgr", &si);
	if (ret != E_HM_OK) {
		return ret;
	}

	ret = actvcapcall_fscall_sockfs_set_max_dgram_qlen(si->actv_rref,
		PROCFS_SP_SOCKFS_UNIX_MAX_DGRAM_QLEN, buf, count, type);

	put_server_info(si);

	return ret;
}

static int sockfs_do_get_max_dgram_qlen(void *buf, size_t count, int type)
{
	int ret;
	struct server_info *si = NULL;

	ret = get_server_info("fsmgr", &si);
	if (ret != E_HM_OK) {
		return ret;
	}

	ret = actvcapcall_fscall_sockfs_get_max_dgram_qlen(si->actv_rref,
		PROCFS_SP_SOCKFS_UNIX_MAX_DGRAM_QLEN, buf, count, type);

	put_server_info(si);

	return ret;
}
#endif

static int net_do_set_max_dgram_qlen(const void *buf, size_t count, bool is_ipc)
{
	void *tmp_buf = NULL;
	int tmp = 0;
	int ret;

	if (is_ipc) {
		tmp_buf = calloc(1, count + 1);
		if (tmp_buf == NULL) {
			return E_HM_NOMEM;
		}

		ret = osal_copy_from_user(tmp_buf, buf, count);
		if (ret != E_HM_OK) {
			free(tmp_buf);
			return E_HM_POSIX_FAULT;
		}

		ret = sscanf_s(tmp_buf, "%d", &tmp);
		free(tmp_buf);
	} else {
		ret = sscanf_s(buf, "%d", &tmp);
	}

	if (ret <= 0) {
		return E_HM_INVAL;
	}

	max_dgram_qlen = tmp;

	return (int)(count);
}

static int net_do_get_max_dgram_qlen(void *buf, size_t count, bool is_ipc)
{
	int ret;
	void *tmp_buf = NULL;

	if (is_ipc) {
		tmp_buf = calloc(1, count);
		if (tmp_buf == NULL) {
			return E_HM_NOMEM;
		}

		ret = snprintf_s(tmp_buf, count, count - 1, "%d\n", max_dgram_qlen);
		if (ret > 0) {
			if (osal_copy_to_user(buf, tmp_buf, count) != E_HM_OK) {
				ret = E_HM_INVAL;
			}
		}

		free(tmp_buf);
	} else {
		ret = snprintf_s(buf, count, count - 1, "%d\n", max_dgram_qlen);
	}

	return ret < 0 ? E_HM_INVAL : ret;
}

static int procfs_set_max_dgram_qlen(struct common_procfs_entry *entry, const void *buf, size_t count)
{
	UNUSED(entry);

#ifdef CONFIG_NETFS_FUSION_PROCFS
	return sockfs_do_set_max_dgram_qlen(buf, count, 0);
#else
	return net_do_set_max_dgram_qlen(buf, count, false);
#endif
}

static int procfs_get_max_dgram_qlen(struct common_procfs_entry *entry, void *buf, size_t count)
{
	UNUSED(entry);

#ifdef CONFIG_NETFS_FUSION_PROCFS
	return sockfs_do_get_max_dgram_qlen(buf, count, 0);
#else
	return net_do_get_max_dgram_qlen(buf, count, false);
#endif
}

static int __procfs_set_max_dgram_qlen(const void *buf, size_t count, int type)
{
	UNUSED(type);
	return net_do_set_max_dgram_qlen(buf, count, true);
}

static int __procfs_get_max_dgram_qlen(void *buf, size_t count, int type)
{
	UNUSED(type);
	return net_do_get_max_dgram_qlen(buf, count, true);
}

static int get_dump_msg(void *buf, size_t count, int type, bool is_ipc,
			int (*dump)(char **msg, int *msg_len, int type),
			void (*release)(char *msg, int type))
{
	char *msg = NULL;
	int msg_len = 0;
	int ret = 0;

	ret = dump(&msg, &msg_len, type);
	if (ret != 0) {
		net_error(HMNET_CORE, "get net statistic info failed\n");
		return E_HM_INVAL;
	}

	if ((int)count <= msg_len) {
		net_debug(HMNET_CORE, "msg too long\n");
		ret = E_HM_FBIG;
	}

	if (ret == E_HM_OK) {
		if (is_ipc) {
			ret = osal_copy_to_user(buf, msg, msg_len);
		} else {
			ret = memcpy_s(buf, count, msg, msg_len);
		}

		if (ret != E_HM_OK) {
			net_error(HMNET_CORE, "copy msg to rvaddr failed\n");
			ret = E_HM_POSIX_FAULT;
		}
	}

	release(msg, type);

	return ret == E_HM_OK ? msg_len : ret;
}

static int net_do_get_unix_netstat(void * buf, size_t count, int type, bool is_ipc)
{
	return get_dump_msg(buf, count, type, is_ipc, get_netstat_unix, release_netstat_unix);
}

#ifdef CONFIG_NETFS_FUSION_PROCFS
static int sockfs_do_get_unix_netstat(void *buf, size_t count, int type)
{
	int ret;
	struct server_info *si = NULL;

	ret = get_server_info("fsmgr", &si);
	if (ret != E_HM_OK) {
		return ret;
	}

	ret = actvcapcall_fscall_sockfs_get_netstat(si->actv_rref,
		PROCFS_SP_SOCKFS_UNIX_NETSTAT, buf, count, type);

	put_server_info(si);

	return ret;
}
#endif

static int __get_netstat_unix(struct common_procfs_entry *entry, void *buf, size_t count)
{
	UNUSED(entry);

#ifdef CONFIG_NETFS_FUSION_PROCFS
	return sockfs_do_get_unix_netstat(buf, count, 0);
#else
	return net_do_get_unix_netstat(buf, count, 0, false);
#endif
}

static int __sockfs_get_netstat_unix(void *buf, size_t count, int type)
{
	return net_do_get_unix_netstat(buf, count, type, true);
}

static int unix_ns_create(ux_namespace_t *ns)
{
#ifdef CONFIG_NETFS_FUSION
	return E_HM_OK;
#else
	int rc;

	rc = procfs_create_net_dir(ns->nsid, "net");
	if (rc < 0 && rc != E_HM_EXIST) {
		net_error(HMNET_CORE, "create net procfs failed rc = %s\n", hmstrerror(rc));
		return rc;
	}

	rc = procfs_create_net_dir(ns->nsid, "sys/net");
	if (rc < 0 && rc != E_HM_EXIST) {
		net_error(HMNET_CORE, "create sys/net procfs failed rc = %s\n", hmstrerror(rc));
		return rc;
	}

	rc = procfs_create_net_dir(ns->nsid, "sys/net/unix");
	if (rc < 0 && rc != E_HM_EXIST) {
		net_error(HMNET_CORE, "create sys/net/unix procfs failed rc = %s\n", hmstrerror(rc));
		return rc;
	}

	rc = procfs_create_net_file(ns->nsid, "net/unix", S_IFREG | S_IRUGO, __get_netstat_unix, NULL);
	if (rc < 0 && rc != E_HM_EXIST) {
		net_error(HMNET_CORE, "create net/unix procfs failed rc = %s\n", hmstrerror(rc));
		return rc;
	}

	rc = procfs_create_net_file(ns->nsid, "sys/net/unix/max_dgram_qlen", S_IFREG | S_IRUGO | S_IWUSR,
				    procfs_get_max_dgram_qlen, procfs_set_max_dgram_qlen);
	if (rc < 0 && rc != E_HM_EXIST) {
		net_error(HMNET_CORE, "create sys/net/unix/max_dgram_qlen procfs failed rc = %s\n", hmstrerror(rc));
		return rc;
	}

	return rc;
#endif
}

static int unix_ns_delete(ux_namespace_t *ns)
{
	procfs_delete_net_file(ns->nsid, "");
	return E_HM_OK;
}

static ux_namespace_ops_t unix_procfs_ns_ops = {
	.node = DLIST_HEAD_INIT(unix_procfs_ns_ops.node),
	.ns_create = unix_ns_create,
	.ns_destroy = unix_ns_delete
};
#endif

void ux_unix_procfs_init(void)
{
#ifdef CONFIG_NET_NS
	ux_register_namespace_ops(&unix_procfs_ns_ops);
	ux_procfs_register_special_func(PROCFS_SP_SOCKFS_UNIX_NETSTAT, __sockfs_get_netstat_unix, NULL);
	ux_procfs_register_special_func(PROCFS_SP_SOCKFS_UNIX_MAX_DGRAM_QLEN,
					__procfs_get_max_dgram_qlen, __procfs_set_max_dgram_qlen);
#endif
}
