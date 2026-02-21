/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: devmgr mgmt helper APIs
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 16 16:46:07 2021
 */

#include <libgen.h>
#include <vfs.h>
#include <lib/utils.h>
#include <libhmsrv_io/mgmt.h>
#include <libmem/utils.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libbunch/bunch_ipc.h>
#include <libhmlog/hmlog.h>

#include "internal.h"

static char *__get_basename(const char *str)
{
	const char *slash = NULL;

	if ((str == NULL) || (str[0] == '\0')) {
		return NULL;
	}

	slash = strrchr(str, '/');
	if (slash == NULL) {
		slash = str;
	} else {
		slash++;
	}

	return strdup(slash);
}


static int __acquire_module(const char *pathname, size_t *rsize)
{
	int fd = -1;
	int ret = E_HM_OK;
	struct stat stat;

	/* open file */
	fd = vfs_open(pathname, O_RDONLY, 0);
	if (fd < 0) {
		ret = fd;
		goto err_open;
	}

	ret = vfs_fstat(fd, &stat);
	if (ret < 0) {
		goto err_stat;
	}
	*rsize = (size_t)stat.st_size;
	if (*rsize == 0U) {
		ret = E_HM_INVAL;
	}

err_stat:
	if (ret < 0) {
		if (vfs_close(fd) < 0) {
			hm_warn("failed to close %s\n", pathname);
		}
	}
err_open:
	return (ret < 0) ? ret : fd;
}

static int __read_modules(int fd, void *buf, size_t size)
{
	size_t left = size;
	ssize_t num = 0;
	int ret = E_HM_OK;

	while (left > 0) {
		num = vfs_read(fd, buf, left);
		if (num <= 0) {
			ret = (int)num;
			break;
		}

		buf = ptr_add(buf, num);
		left -= (size_t)(unsigned int)num;
	}
	return ret;
}

static int __load_modules_data(const char *pathname, char **mod_buf_out, size_t *mod_size_out)
{
	int ret = E_HM_OK;
	int fd = -1;
	size_t size;
	char *mod_buf = NULL;

	/* stat to get size */
	fd = __acquire_module(pathname, &size);
	if (fd < 0) {
		ret = fd;
		goto err_open;
	}

	if (size == 0) {
		ret = E_HM_INVAL;
		goto err_alloc_buf;
	}

	mod_buf = malloc(size);
	if (mod_buf == NULL) {
		ret = E_HM_NOMEM;
		goto err_alloc_buf;
	}

	ret = __read_modules(fd, mod_buf, size);
	if (ret != E_HM_OK) {
		free(mod_buf);
		goto err_alloc_buf;
	}

	*mod_buf_out = mod_buf;
	*mod_size_out = size;

err_alloc_buf:
	if (vfs_close(fd) < 0) {
		hm_warn("failed to close %s\n", pathname);
	}
err_open:
	return ret;
}

static int __do_devmgr_external_insmod(struct devmgr_mgmt_target target, const char *name, const char *args,
				       char* mod_buf, const size_t size)
{
	int ret;
	struct hm_actv_buf modname;
	struct hm_actv_buf modargs;
	struct load_module_arg load_args= {0};

	hm_actv_buf_init(&modname, name, strlen(name) + 1U);
	hm_actv_buf_init(&modargs, args,
				(args == NULL) ? 0U : strlen(args) + 1U);

	load_args.mod_buf = mod_buf;
	load_args.mod_size = size;

	ret = actvxactcapcall_drvcall_devmgr_mgmt_load_module(false, true,
					g_devmgr_ap_rref, (int)DM_MGMT_EXTERNAL_MODULE,
					modname, modargs, target,
					&load_args, sizeof(load_args));

	return ret;
}

static int __load_external_module(struct devmgr_mgmt_target target, const char *pathname, const char *args)
{
	int ret = E_HM_INVAL;
	size_t modsize = 0;
	char *name = NULL;
	char *module_buf = NULL;

	/* resolve driver name */
	if ((name = __get_basename(pathname)) == NULL) {
		return E_HM_NOMEM;
	}
	if (!is_valid_module_name(name)) {
		ret = E_HM_INVAL;
		goto err_out;
	}

	/* load driver data */
	ret = __load_modules_data(pathname, &module_buf, &modsize);
	if (ret < 0) {
		hm_debug("load module data failed, err=%s path=%s\n", hmstrerror(ret),
			 pathname);
		goto err_out;
	}

	ret = __do_devmgr_external_insmod(target, name, args, module_buf, modsize);
	if (ret < 0) {
		hm_debug("load module failed, err=%s path=%s\n", hmstrerror(ret),
			 pathname);
	}
	free(module_buf);

err_out:
	free(name);
	return ret;
}

int hmsrv_io_mgmt_load_module(struct devmgr_mgmt_target target,
			      const char *name, const char *args)
{
	struct hm_actv_buf modname;
	struct hm_actv_buf modargs;

	if (!is_valid_module_name(name)) {
		return E_HM_INVAL;
	}

	hm_actv_buf_init(&modname, name, strlen(name) + 1U);
	hm_actv_buf_init(&modargs, args,
			 (args == NULL) ? 0U : strlen(args) + 1U);

	return actvxactcapcall_drvcall_devmgr_mgmt_load_module(false, true, g_devmgr_ap_rref,
			(int)DM_MGMT_INTERNAL_MODULE, modname, modargs, target, NULL, 0);
}

int hmsrv_io_mgmt_load_external_module(struct devmgr_mgmt_target target,
				       const char *pathname, const char *args)
{
	if (pathname == NULL || pathname[0] == '\0') {
		return E_HM_INVAL;
	}

	return __load_external_module(target, pathname, args);
}

int hmsrv_io_mgmt_unload_module(struct devmgr_mgmt_target target,
				const char *name)
{
	struct hm_actv_buf modname;

	if (!is_valid_module_name(name)) {
		return E_HM_INVAL;
	}

	hm_actv_buf_init(&modname, name, strlen(name) + 1U);

	return actvxactcapcall_drvcall_devmgr_mgmt_unload_module(false, true,
			g_devmgr_ap_rref, modname, target);
}

int hmsrv_io_mgmt_sync(void)
{
	return actvcapcall_drvcall_devmgr_mgmt_sync(g_devmgr_ap_rref);
}

static int mgmt_dump_ipc(struct bunch_ipc_attr *attr, void *data)
{
	struct __actvret_drvcall_devmgr_mgmt_dump *dump_ret = NULL;

	dump_ret = ptr_from_void(data, struct __actvret_drvcall_devmgr_mgmt_dump);
	return actvxactcapcall_drvcall_devmgr_mgmt_dump(false, true,
							g_devmgr_ap_rref,
							*attr, dump_ret);
}

int hmsrv_io_mgmt_dump(const hmsrv_io_mgmt_dump_callback_fn fn, void *ctx)
{
	struct __actvret_drvcall_devmgr_mgmt_dump dump_ret;
	struct bunch *bunch = NULL;
	struct bunch_ipc_reader_ctx bunch_ctx;
	struct sysarg_cnode_statrref statrref;
	const char *str = NULL;
	int ret;

	mem_zero_s(dump_ret);
	mem_zero_s(statrref);

	/* query devmgr cnode_idx by rref */
	ret = sysfast_cnode_stat_rref(&statrref, g_devmgr_ap_rref,
				CNODE_STATRREF_SRC_CNODE_IDX);
	if (ret < 0) {
		hm_debug("query devmgr cnode_idx failed:%s\n", hmstrerror(ret));
	}

	if (ret == E_HM_OK) {
		bunch = bunch_ipc_create(0);
		if (bunch == NULL) {
			ret = E_HM_RESFAULT;
			hm_debug("bunch_ipc_create failed\n");
		}
	}

	if (ret == E_HM_OK) {
		bunch_ctx.ipc = mgmt_dump_ipc;
		bunch_ctx.private_data = ptr_to_void(&dump_ret);
		bunch_ctx.target_info = (uint64_t)g_devmgr_ap_rref;
		ret = bunch_ipc_request(bunch, &bunch_ctx);
		if (ret < 0) {
			hm_debug("bunch ipc bunch request data with shm failed: %s\n",
				 hmstrerror(ret));
		}
	}

	if (ret == E_HM_OK) {
		/* do callback fn for valid string */
		while (bunch_get_position(bunch) < dump_ret.pos_end) {
			str = bunch_read_string(bunch);
			if (str == NULL) {
				continue;
			}
			ret = (*fn)(ctx, str);
			if (ret < 0) {
				break;
			}
		}
	}

	/* free bunch ipc, allow bunch is NULL */
	bunch_ipc_destroy(bunch);

	return ret;
}
