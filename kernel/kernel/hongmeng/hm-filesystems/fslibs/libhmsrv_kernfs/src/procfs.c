/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Procfs functions implementations
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 20 15:23:11 2020
 */

#include <fs_procfs.h>

#include <vfs.h>
#include <unistd.h>
#include <inttypes.h>

#include <libhmlog/hmlog.h>
#include <hongmeng/errno.h>
#include <hongmeng/compiler.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsync/atomic.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_path.h>
#include <libhmsrv_sys/hm_procmgr.h>

#include "kernfslib_common.h"

#define PROCFS_PATH_ACTV "fs-procfs-actv"

static rref_t procfs_rref;

static int get_procfs_rref(rref_t *rref)
{
	int ret;

	if (procfs_rref != 0) {
		*rref = procfs_rref;
		return E_HM_OK;
	}

	ret = kernfslib_get_rref(PROCFS_PATH_ACTV, &procfs_rref);
	if (ret != E_HM_OK) {
		return ret;
	}

	*rref = procfs_rref;
	return E_HM_OK;
}

static int __procfs_create_data(rref_t rref, const char *pathname,
				mode_t mode, const struct procfs_ops_cb *ops,
				const char *linkdata)
{
	int err;

	if (S_ISDIR(mode)) {
		struct procfs_ops_cb para_ops;
		mem_zero_s(para_ops);

		err = actvcapcall_fscall_procfs_create_data(rref, pathname, mode,
							    para_ops, NULL, 0);
		if (err != E_HM_OK) {
			hm_debug("fscall procfs_create dir failed, err=%s\n", hmstrerror(err));
			return err;
		}
	} else if (S_ISREG(mode)) {
		err = actvcapcall_fscall_procfs_create_data(rref, pathname, mode,
							    *ops, NULL, 0);
		if (err != E_HM_OK) {
			hm_debug("fscall procfs_create file failed, err=%s\n", hmstrerror(err));
			return err;
		}
	} else if (S_ISLNK(mode)) {
		char *tmplink = NULL;
		struct procfs_ops_cb para_ops;
		mem_zero_s(para_ops);

		if (strlen(linkdata) + 1 > PATH_MAX) {
			hm_debug("invalid linkdata\n");
			return E_HM_INVAL;
		}

		tmplink = (char *)calloc(PATH_MAX, sizeof(char));
		if (tmplink == NULL) {
			hm_warn("calloc tmplink failed\n");
			return E_HM_NOMEM;
		}
		NOFAIL(memcpy_s(tmplink, PATH_MAX, linkdata, strlen(linkdata) + 1));

		err = actvcapcall_fscall_procfs_create_data(rref, pathname, mode,
							    para_ops, tmplink, strlen(tmplink) + 1);
		free(tmplink);
		if (err != E_HM_OK) {
			hm_debug("fscall procfs_create link failed, err=%s\n", hmstrerror(err));
			return err;
		}
	} else {
		return E_HM_NOSYS;
	}

	return E_HM_OK;
}

int procfs_create_data(const char *name, mode_t mode,
		       const struct procfs_ops_cb *ops, const char *linkdata)
{
	rref_t rref = 0;
	char pathname[PROCFS_PATH_NAME_MAX] = {0};
	int err;

	if (name == NULL || (strlen(name) + 1) > PROCFS_PATH_NAME_MAX) {
		hm_debug("invalid name\n");
		return E_HM_INVAL;
	}

	/* If the mode is not set, S_IFREG is used by default. */
	if ((mode & S_IFMT) == 0) {
		mode |= S_IFREG;
	}

	/*
	 * If the mode is not set, the default mode of DIR will be set to
	 * S_IRUGO | S_IXUGO, the default mode of FILE and LNK will be set
	 * to S_IRUGO
	 */
	if ((mode & S_IALLUGO) == 0) {
		if (S_ISDIR(mode)) {
			mode |= S_IRUGO | S_IXUGO;
		} else {
			mode |= S_IRUGO;
		}
	}

	if (S_ISLNK(mode) && (linkdata == NULL)) {
		hm_debug("miss linkdata\n");
		return E_HM_INVAL;
	}
	if (S_ISREG(mode) && (ops == NULL)) {
		hm_debug("missing file ops\n");
		return E_HM_INVAL;
	}

	err = get_procfs_rref(&rref);
	if (err != E_HM_OK) {
		hm_debug("get_procfs_rref failed, err=%s\n", hmstrerror(err));
		return err;
	}

	NOFAIL(memcpy_s(pathname, sizeof(pathname), name, strlen(name) + 1));

	return __procfs_create_data(rref, pathname, mode, ops, linkdata);
}

static int __procfs_create_data_net(rref_t rref, unsigned int nsid,
				const char *pathname, mode_t mode, const struct procfs_ops_cb *ops)
{
	int err;

	if (S_ISDIR(mode)) {
		struct procfs_ops_cb para_ops;
		mem_zero_s(para_ops);
		err = actvcapcall_fscall_procfs_create_net_data(rref, pathname, mode,
							    para_ops, nsid, NULL, 0);
		if (err != E_HM_OK) {
			hm_debug("fscall procfs_create net dir failed, err=%s\n", hmstrerror(err));
			return err;
		}
	} else if (S_ISREG(mode)) {
		err = actvcapcall_fscall_procfs_create_net_data(rref, pathname, mode,
							    *ops, nsid, NULL, 0);
		if (err != E_HM_OK) {
			hm_debug("fscall procfs_create net file failed, err=%s\n", hmstrerror(err));
			return err;
		}
	} else {
		return E_HM_NOSYS;
	}

	return E_HM_OK;
}

int procfs_create_data_net(unsigned int nsid, const char *name,
				    mode_t mode, const struct procfs_ops_cb *ops)
{
	rref_t rref = 0;
	char pathname[PROCFS_PATH_NAME_MAX] = {0};
	int err;

	if (name == NULL || (strlen(name) + 1) > PROCFS_PATH_NAME_MAX) {
		hm_debug("invalid name\n");
		return E_HM_INVAL;
	}

	/* If the mode is not set, S_IFREG is used by default. */
	if ((mode & S_IFMT) == 0) {
		mode |= S_IFREG;
	}

	/*
	 * If the mode is not set, the default mode of DIR will be set to
	 * S_IRUGO | S_IXUGO, the default mode of FILE and LNK will be set
	 * to S_IRUGO
	 */
	if ((mode & S_IALLUGO) == 0) {
		if (S_ISDIR(mode)) {
			mode |= S_IRUGO | S_IXUGO;
		} else {
			mode |= S_IRUGO;
		}
	}

	if (S_ISREG(mode) && (ops == NULL)) {
		hm_debug("missing file ops\n");
		return E_HM_INVAL;
	}

	err = get_procfs_rref(&rref);
	if (err != E_HM_OK) {
		hm_debug("get_procfs_rref failed, err=%s\n", hmstrerror(err));
		return err;
	}

	NOFAIL(memcpy_s(pathname, sizeof(pathname), name, strlen(name) + 1));

	return __procfs_create_data_net(rref, nsid, pathname, mode, ops);
}

int procfs_delete_data_net_specific(unsigned int nsid, const char *pathname)
{
	int err = E_HM_OK;
	rref_t rref = 0;

	err = get_procfs_rref(&rref);
	if (err != E_HM_OK) {
		hm_debug("get_procfs_rref failed, err=%s\n", hmstrerror(err));
		return err;
	}

	err = actvcapcall_fscall_procfs_delete_net_data_specific(rref, nsid, pathname, NULL, 0);
	if (err != E_HM_OK) {
		hm_debug("fscall delete net procfs data failed, err=%s\n", hmstrerror(err));
		return err;
	}

	return err;
}

int procfs_delete_data(const char *name)
{
	rref_t rref = 0;
	char pathname[PROCFS_PATH_NAME_MAX] = {0};
	int err;

	if (name == NULL || (strlen(name) + 1) > PROCFS_PATH_NAME_MAX) {
		hm_debug("invalid name\n");
		return E_HM_INVAL;
	}

	NOFAIL(memcpy_s(pathname, sizeof(pathname), name, strlen(name) + 1));

	err = get_procfs_rref(&rref);
	if (err != E_HM_OK) {
		hm_debug("get_procfs_rref failed, err=%s\n", hmstrerror(err));
		return err;
	}

	err = actvcapcall_fscall_procfs_delete_data(rref, pathname);
	if (err != E_HM_OK) {
		hm_debug("fscall procfs_delete_data failed, err=%s\n", hmstrerror(err));
		return err;
	}

	return E_HM_OK;
}

int procfs_create_net_proc_entries(struct net_procfs_node *proc_entry_list, unsigned int cnt)
{
	rref_t rref = 0;
	int err;

	if (proc_entry_list == NULL) {
		hm_debug("invalid name\n");
		return E_HM_INVAL;
	}

	err = get_procfs_rref(&rref);
	if (err != E_HM_OK) {
		hm_debug("get_procfs_rref failed, err=%s\n", hmstrerror(err));
		return err;
	}

	err = actvcapcall_fscall_procfs_create_net_entries(rref, (void *)proc_entry_list, cnt, NULL, 0);
	if (err != E_HM_OK) {
		return err;
	}

	return E_HM_OK;
}
