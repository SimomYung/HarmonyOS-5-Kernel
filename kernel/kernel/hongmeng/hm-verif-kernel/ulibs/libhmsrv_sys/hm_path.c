/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: HongMeng native api implementation for pathmgr
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 17 12:59:34 2019
 */
#include <hongmeng/errno.h>
#include <libhmucap/ucap.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_path.h>
#include <libhwsecurec/securec.h>
#include <libsysif/sysmgr/api.h>

static bool check_path_validity(const char *path)
{
	bool ret_value = false;
	size_t len;

	if (path != NULL) {
		len = strnlen(path, PATHMGR_PATH_NAME_MAX);
		if (len > 0 && len < PATHMGR_PATH_NAME_MAX) {
			ret_value = true;
		}
	}

	return ret_value;
}

/*
 * Deal with client's add path request and create path with DAC rights by mode
 *
 * @params cref [I] Client process cnode rref
 *
 * @params path [I] Path name acquired
 *
 * @params grant_info_addr [I] Grant info addr to pass kernel
 *
 * @params mode [I] Provide the mode of path for changeing DAC rights with the effective flags as follow:
 * 		S_ISYS_SRVC flag suggests that objects only can be accessed by system service
 * 		S_IRUSR flag suggests that objects can be accessed by process has same uid with path node
 * 		S_IRGRP flag suggests that objects can be accessed by process has same gid with path node
 * 		or satisfies the condition of additional groups.
 * 		S_IROTH flag suggests that objects can be accessed by any process
 *
 * @return Returns E_HM_OK on success. On any error, returns an errno.
 */
int hm_path_add_with_mode(cref_t cref, unsigned long grant_info, const char *path, mode_t mode)
{
	int err = E_HM_OK;
	struct pathmgr_name name;

	if (IS_REF_VALID(cref) == __false || !check_path_validity(path)) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		NOFAIL(strncpy_s(name.path, sizeof(name.path),
				 path, PATHMGR_PATH_NAME_MAX - 1));
		err = actvcall_hmcall_path_add_path(cref, grant_info, name, mode);
		if (err != E_HM_OK) {
			hm_debug("add path %s failed, ret = %s\n",
				path, hmstrerror(err));
		}
	}

	return err;
}

int hm_path_add(cref_t cref, unsigned long grant_info, const char *path)
{
	return hm_path_add_with_mode(cref, grant_info, path, __U(S_IRUSR) | __U(S_IRGRP) | __U(S_IROTH));
}

/*
 * Deal with client's grant path request, only
 * the service process can grant
 *
 * @params path [I] Path name acquired
 *
 * @return Returns E_HM_OK on success. On any error, returns an errno.
 */
int hm_path_grant(const char *path)
{
	int err = E_HM_OK;
	struct pathmgr_name name;

	if (!check_path_validity(path)) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		NOFAIL(strncpy_s(name.path, sizeof(name.path),
				 path, PATHMGR_PATH_NAME_MAX - 1));
		err = actvcall_hmcall_path_grant_path(name);
		if (err != E_HM_OK) {
			hm_debug("grant path %s failed, ret = %s\n",
				path, hmstrerror(err));
		}
	}

	return err;
}

int hm_path_delete(const char *path)
{
	int err = E_HM_OK;
	struct pathmgr_name name;

	if (!check_path_validity(path)) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		NOFAIL(strncpy_s(name.path, sizeof(name.path),
				 path, PATHMGR_PATH_NAME_MAX - 1));
		err = actvcall_hmcall_path_del_path(name);
		if (err != E_HM_OK) {
			hm_debug("delete path %s failed, ret = %s\n",
				path, hmstrerror(err));
		}
	}

	return err;
}

int hm_path_acquire(const char *path, rref_t *rref)
{
	int err = E_HM_OK;
	struct pathmgr_name name;
	struct __actvret_hmcall_path_acquire_path path_ret;

	if (rref == NULL || !check_path_validity(path)) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		NOFAIL(strncpy_s(name.path, sizeof(name.path),
				 path, PATHMGR_PATH_NAME_MAX - 1));
		mem_zero_s(path_ret);
		err = actvcall_hmcall_path_acquire_path(name, &path_ret);
		if (err != E_HM_OK) {
			hm_debug("acquire path %s failed, ret = %s\n",
				 path, hmstrerror(err));
		} else {
			*rref = path_ret.rref;
		}
	}

	return err;
}

int hm_path_release(const char *path, rref_t rref)
{
	int err = E_HM_OK;
	struct pathmgr_name name;

	if (!check_path_validity(path)) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		NOFAIL(strncpy_s(name.path, sizeof(name.path),
				 path, PATHMGR_PATH_NAME_MAX - 1));
		err = actvcall_hmcall_path_release_path(name, rref);
		if (err != E_HM_OK) {
			hm_debug("release path %s failed, ret = %s\n",
				 path, hmstrerror(err));
		}
	}

	return err;
}

static int path_stat_ipc(struct bunch_ipc_attr *attr, void *path_name)
{
	struct pathmgr_name arg;
	char *name = void_to_ptr(path_name, char);

	NOFAIL(strncpy_s(arg.path, sizeof(arg.path), name, PATHMGR_PATH_NAME_MAX - 1));
	return actvcall_hmcall_path_stat(arg, *attr);
}

int hm_path_stat(const char *path, struct bunch *bunch)
{
	int err = E_HM_OK;
	struct pathmgr_name name;
	struct bunch_ipc_reader_ctx ctx;
	uint32_t cnode_idx;

	if (bunch == NULL || !check_path_validity(path)) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		NOFAIL(strncpy_s(name.path, sizeof(name.path),
				 path, PATHMGR_PATH_NAME_MAX - 1));
	}
	if (err == E_HM_OK) {
		cnode_idx = hmsrv_sysmgr_cnode_idx();
		ctx.ipc = path_stat_ipc;
		ctx.private_data = ptr_to_void(name.path);
		ctx.target_info = (uint64_t)cnode_idx;

		err = bunch_ipc_request(bunch, &ctx);
		if (err < 0) {
			hm_debug("bunch ipc try request data with shm failed: %s\n",
				hmstrerror(err));
		}
	}

	return err;
}

int hm_path_chmod(const char *path, mode_t mode)
{
	int err = E_HM_OK;
	struct pathmgr_name name;

	if (!check_path_validity(path)) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		NOFAIL(strncpy_s(name.path, sizeof(name.path),
				 path, PATHMGR_PATH_NAME_MAX - 1));
		err = actvcall_hmcall_path_chmod(name, mode);
		if (err != E_HM_OK) {
			hm_debug("chmod path %s failed, ret = %s\n",
				path, hmstrerror(err));
		}
	}

	return err;
}
