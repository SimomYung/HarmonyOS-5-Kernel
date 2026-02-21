/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: key file system interface
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 06 20:34:55 2023
 */

#include <stdint.h>
#include <libhmlog/hmlog.h>
#include <hongmeng/errno.h>

#include <libsysif/keymgr/api.h>
#include <libhmsrv_key/procfs.h>
#include <libhmsrv_key/key.h>

/*
 * Notice:
 * This interface only can be used by VFS. And uapp should use actvxactcapcall_**(false, true, **) to call VFS,
 * because keymgr should get uapp's DAC info.
 */
long hm_key_fscrypt_ctrl_actv(rref_t keymgr_rref, int operation, void *arg_addr, size_t arg_size)
{
	int ret = E_HM_INVAL;

	if (IS_REF_ERR(keymgr_rref)) {
		ret = REF_TO_ERR(keymgr_rref);
		hm_debug("invailed rref for keymgr, err=%s\n", hmstrerror(ret));
		return ret;
	}

	ret = actvxactcapcall_keycall_keymgr_fscrypt_ctrl(true, true, keymgr_rref,
		operation, (unsigned long)arg_addr, arg_size);
	return ret;
}

int hm_key_fsverity_ctrl_actv(rref_t keymgr_rref, int operation, void *arg_addr, size_t arg_size)
{
	int ret = E_HM_OK;

	if (IS_REF_ERR(keymgr_rref)) {
		ret = REF_TO_ERR(keymgr_rref);
		hm_debug("invailed rref for keymgr, err=%s\n", hmstrerror(ret));
		return ret;
	}

	ret = actvxactcapcall_keycall_keymgr_fsverity_ctrl(true, true, keymgr_rref,
		operation, (unsigned long)arg_addr, (unsigned long)arg_size);
	return ret;
}

static int do_procfs_read(rref_t keymgr_rref, int file_id, char *buf, size_t len)
{
	int ret = E_HM_OK;

	if (IS_REF_ERR(keymgr_rref)) {
		ret = REF_TO_ERR(keymgr_rref);
		hm_debug("invailed rref for keymgr, err=%s\n", hmstrerror(ret));
		return ret;
	}
	return actvxactcapcall_keycall_keymgr_procfs_read(true, true, keymgr_rref, file_id, buf, len);
}

static int do_procfs_write(rref_t keymgr_rref, int file_id, const char *buf, size_t len)
{
	int ret = E_HM_OK;

	if (IS_REF_ERR(keymgr_rref)) {
		ret = REF_TO_ERR(keymgr_rref);
		hm_debug("invailed rref for keymgr, err=%s\n", hmstrerror(ret));
		return ret;
	}
	return actvxactcapcall_keycall_keymgr_procfs_write(true, true, keymgr_rref, file_id, buf, len);
}

int hm_key_procfs_read(rref_t keymgr_rref, int file_id, char *buf, size_t len)
{
	return do_procfs_read(keymgr_rref, file_id, buf, len);
}

int hm_key_procfs_write(rref_t keymgr_rref, int file_id, const char *buf, size_t len)
{
	return do_procfs_write(keymgr_rref, file_id, buf, len);
}

static const char *key_procfs_map[] = {
	[KEY_PROCFS_GC_DELAY] = "gc_delay",
	[KEY_PROCFS_MAXBYTES] = "maxbytes",
	[KEY_PROCFS_MAXKEYS] = "maxkeys",
	[KEY_PROCFS_ROOT_MAXBYTES] = "root_maxbytes",
	[KEY_PROCFS_ROOT_MAXKEYS] = "root_maxkeys",
};

static int get_key_procfs_index(const char *filename)
{
	for (unsigned int i = 0; i < ARRAY_SIZE(key_procfs_map); i++) {
		if (strcmp(filename, key_procfs_map[i]) == 0) {
			return (int)i;
		}
	}
	hm_warn("/proc/sys/kernel/keys/%s has no file id map\n", filename);
	return -1;
}

int hm_key_procfs_read_by_name(rref_t keymgr_rref, const char *filename, char *buf, size_t len)
{
	int ret = E_HM_OK;
	int fileid = get_key_procfs_index(filename);
	if (fileid == -1) {
		ret = E_HM_NOENT;
	}
	if (ret == E_HM_OK) {
		ret = do_procfs_read(keymgr_rref, fileid, buf, len);
	}
	return ret;
}

int hm_key_procfs_write_by_name(rref_t keymgr_rref, const char *filename, const char *buf, size_t len)
{
	int ret = E_HM_OK;
	int fileid = get_key_procfs_index(filename);
	if (fileid == -1) {
		ret = E_HM_NOENT;
	}
	if (ret == E_HM_OK) {
		ret = do_procfs_write(keymgr_rref, fileid, buf, len);
	}
	return ret;
}
