/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Helpers for procfs support
 * Author: Huawei OS Kernel Lab
 * Create: Wed Feb 19 08:53:16 2020
 */
#include <libsysif/devhost/server.h>

#include <fs_procfs.h>
#include <devhost/plugin.h>
#include <libhmactv/actv.h>
#include <libdevhost/procfs.h>
#include <libdevhost/sysfs.h>
#include <libdevhost/devhost.h>
#include <libsysif/devmgr/api.h>

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_procfs_read, dh_procfs_read_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, tagid, unsigned long long, fd, unsigned long long, pos,
			 struct hm_actv_buf, ubuf, uintptr_t, ctx)
{
	struct __actvret_drvcall_devhost_procfs_read *pret =
		actvhdlr_drvhandler_devhost_procfs_read_prepare_ret(sender, credential);
	BUG_ON_D(pret == NULL);
	size_t rsize = 0;
	int ret = 0;

	/* fd == FAKE_FD means open method not supported, for example UDK case */
	if (fd != FAKE_FD && is_devhost_remote_tag(tagid)) {
		/* only ldk sysfs node support open now */
		struct libdh_kernfs_ops_simple *ops = libdh_get_kernfs_ops(tagid, ctx);
		if (ops != NULL) {
			ret = ops->read(fd, pos, (unsigned long)ubuf.buf,
					(unsigned long)ubuf.size, &rsize);
		}
	} else {
		ret = libdh_procfs_read(tagid, pos, &ubuf, ctx, &rsize);
	}
	pret->rsize = (unsigned long long)rsize;
	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_procfs_write, dh_procfs_write_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, tagid, unsigned long long, fd, unsigned long long, pos,
			 struct hm_actv_buf, ubuf, uintptr_t, ctx)
{
	struct __actvret_drvcall_devhost_procfs_write *pret =
		actvhdlr_drvhandler_devhost_procfs_write_prepare_ret(sender, credential);
	BUG_ON_D(pret == NULL);
	size_t wsize = 0;
	int ret = 0;

	/* fd == FAKE_FD means open method not supported, for example UDK case */
	if (fd != FAKE_FD && is_devhost_remote_tag(tagid)) {
		/* only ldk sysfs node support open now */
		struct libdh_kernfs_ops_simple *ops = libdh_get_kernfs_ops(tagid, ctx);
		if (ops != NULL) {
			ret = ops->write(fd, pos, (unsigned long)ubuf.buf,
					(unsigned long)ubuf.size, &wsize);
		}
	} else {
		ret = libdh_procfs_write(tagid, pos, &ubuf, ctx, &wsize);
	}

	pret->wsize = (unsigned long long)wsize;
	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_procfs_open, dh_procfs_open_ext,
			 unsigned long long, sender, unsigned long, credential,
			 struct hm_actv_buf, actv_buf)
{
	struct __actvret_drvcall_devhost_procfs_open *pret =
		actvhdlr_drvhandler_devhost_procfs_open_prepare_ret(sender, credential);
	int ret = 0;
	actvcall_procfs_open_args args = {0};
	unsigned long long fd = FAKE_FD;

	ret = hm_actv_read_caller_buf((void *)&args, sizeof(actvcall_procfs_open_args), &actv_buf);
	if (ret != E_HM_OK) {
		return ret;
	}

	ret = libdh_procfs_open_simple(args.tagid, args.flags, args.mode, args.ctx, &fd);
	if (ret == E_HM_OK) {
		pret->fd = fd;
	}

	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_procfs_close, dh_procfs_close_ext,
			 unsigned long long, sender, unsigned long, credential,
			 struct hm_actv_buf, actv_buf)
{
	int ret = 0;
	UNUSED(sender, credential);
	actvcall_procfs_close_args args = {0};

	ret = hm_actv_read_caller_buf((void *)&args, sizeof(actvcall_procfs_close_args), &actv_buf);
	if (ret != E_HM_OK) {
		return ret;
	}

	ret = libdh_procfs_close_simple(args.tagid, args.fd, args.ctx);
	return ret;
}
static int __procfs_create(const char *name, mode_t mode,
			   const struct procfs_ops_cb *__proc_ops,
			   const char *linkdata)
{
	struct hm_actv_buf path;
	struct hm_actv_buf ops;
	struct hm_actv_buf linkpath;
	struct procfs_ops_cb *proc_ops = (struct procfs_ops_cb *)__proc_ops;

	BUG_ON(name == NULL);

	hm_actv_buf_init(&path, name, strlen(name) + 1U);
	hm_actv_buf_init(&ops, proc_ops, (proc_ops == NULL) ? 0 : sizeof(*proc_ops));
	hm_actv_buf_init(&linkpath, linkdata, (linkdata == NULL) ? 0 : (strlen(linkdata) + 1U));

	return actvcapcall_drvcall_devmgr_procfs_create_data(
			libdh_get_devmgr_ap(), path, mode, ops, linkpath);
}

static int __procfs_unlink(const char *name)
{
	struct hm_actv_buf path;

	BUG_ON(name == NULL);

	hm_actv_buf_init(&path, name, strlen(name) + 1U);

	return actvcapcall_drvcall_devmgr_procfs_unlink(
				libdh_get_devmgr_ap(), path);
}

static int dh_procfs_plugin_init(const struct libdh_builtin_plugin *plugin)
{
	__u32 self_cnode_idx;
	int ret;
	UNUSED(plugin);

	/* query self cnode_idx */
	self_cnode_idx = hm_ucap_self_cnode_idx();
	if (is_cnode_idx_err(self_cnode_idx)) {
		return cnode_idx_to_err(self_cnode_idx);
	}

	/* init procfs, "/proc/devhost/<cnode_idx>" */
	ret = libdh_procfs_init(true, DEVHOST_PROCFS_PREFIX_FMT, self_cnode_idx);
	if (ret < 0) {
		hm_error("procfs init failed, err=%s\n", hmstrerror(ret));
		return ret;
	}

	NOFAIL(libdh_procfs_set_remote_ops(__procfs_create, __procfs_unlink));

	return E_HM_OK;
}

/*
 * define devhost builtin procfs plugin. This is procfs frontend for uapp.
 *
 * extension point: NULL
 *
 * extension:
 *  - target sysif-ep: `drvhandler_devhost_procfs_read`
 *  - target sysif-ep: `drvhandler_devhost_procfs_write`
 *
 * export symbol: NULL
 *
 * dependence: NULL
 */
DEFINE_BUILTIN_PLUGIN(procfs, dh_procfs_plugin_init,
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_procfs_open, dh_procfs_open_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_procfs_close, dh_procfs_close_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_procfs_read, dh_procfs_read_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_procfs_write, dh_procfs_write_ext)
);
