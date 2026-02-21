/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Helpers for sysfs support
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 03 08:53:16 2020
 */
#include <libsysif/devhost/server.h>

#include <fs_sysfs.h>
#include <devhost/log.h>
#include <devhost/plugin.h>
#include <libhmactv/actv.h>
#include <libdevhost/sysfs.h>
#include <libdevhost/kernfs.h>

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_sysfs_open, dh_sysfs_open_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, tagid, unsigned int, flags, unsigned int, mode, uintptr_t, ctx)
{
	struct __actvret_drvcall_devhost_sysfs_open *pret =
		actvhdlr_drvhandler_devhost_sysfs_open_prepare_ret(sender, credential);
	BUG_ON_D(pret == NULL);
	unsigned long long fd;
	int ret = E_HM_NOSYS;

	if (is_devhost_remote_tag(tagid)) {
		/* only ldk sysfs node support open now */
		struct libdh_kernfs_ops_simple *ops = libdh_get_kernfs_ops(tagid, ctx);
		if (ops != NULL) {
			ret = ops->open(ulong_to_ptr(local_tag_of(tagid), void), &fd, flags, mode);
		}
	} else {
		fd = FAKE_FD;
		ret = E_HM_OK;
	}

	if (ret == E_HM_OK) {
		pret->fd = fd;
	}
	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_sysfs_close, dh_sysfs_close_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, tagid, unsigned long long, fd, uintptr_t, ctx)
{
	int ret = E_HM_NOSYS;
	UNUSED(sender, credential);

	if (is_devhost_remote_tag(tagid)) {
		/* only ldk sysfs node support close now */
		struct libdh_kernfs_ops_simple *ops = libdh_get_kernfs_ops(tagid, ctx);
		if (ops != NULL) {
			ret = ops->close(fd);
		}
	} else {
		ret = E_HM_OK;
	}

	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_sysfs_read, dh_sysfs_read_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, tagid, unsigned long long, fd, unsigned long long, pos,
			 struct hm_actv_buf, ubuf, uintptr_t, ctx)
{
	struct __actvret_drvcall_devhost_sysfs_read *pret =
		actvhdlr_drvhandler_devhost_sysfs_read_prepare_ret(sender, credential);
	BUG_ON_D(pret == NULL);
	size_t rsize = 0;
	int ret = E_HM_NOSYS;

	if (is_devhost_remote_tag(tagid)) {
		struct libdh_kernfs_ops_simple *ops = libdh_get_kernfs_ops(tagid, ctx);
		if (ops != NULL) {
			ret = ops->read(fd, pos, (unsigned long)ubuf.buf,
					(unsigned long)ubuf.size, &rsize);
		}
	} else {
		ret = libdh_sysfs_read(tagid, pos, &ubuf, ctx, &rsize);
	}

	pret->rsize = (unsigned long long)rsize;
	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_sysfs_write, dh_sysfs_write_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, tagid, unsigned long long, fd, unsigned long long, pos,
			 struct hm_actv_buf, ubuf, uintptr_t, ctx)
{
	struct __actvret_drvcall_devhost_sysfs_write *pret =
		actvhdlr_drvhandler_devhost_sysfs_write_prepare_ret(sender, credential);
	BUG_ON_D(pret == NULL);
	size_t wsize = 0;
	int ret = E_HM_NOSYS;

	if (is_devhost_remote_tag(tagid)) {
		struct libdh_kernfs_ops_simple *ops = libdh_get_kernfs_ops(tagid, ctx);
		if (ops != NULL) {
			ret = ops->write(fd, pos, (unsigned long)ubuf.buf,
					 (unsigned long)ubuf.size, &wsize);
		}
	} else {
		ret = libdh_sysfs_write(tagid, pos, &ubuf, ctx, &wsize);
	}

	pret->wsize = (unsigned long long)wsize;
	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_sysfs_poll, dh_sysfs_poll_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, tagid, uintptr_t, ctx, bool, pollable)
{
	int ret = E_HM_NOSYS;

	UNUSED(sender, credential);
	if (is_devhost_remote_tag(tagid)) {
		struct libdh_kernfs_ops_simple *ops = libdh_get_kernfs_ops(tagid, ctx);
		if (ops != NULL) {
			ret = ops->poll(local_tag_of(tagid), pollable);
		}
	} else {
		ret = libdh_sysfs_poll(tagid, ctx, pollable);
	}

	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_fops_sysfs_read, dh_fops_sysfs_read_ext,
			 unsigned long long, sender, unsigned long, credential)
{
	struct actvpool_recv recv;

	recv = hm_actvpool_curr_recv_data(sender, credential);
	if (recv.size != sizeof(actvcall_sysfs_read_args) || recv.data == NULL) {
		return E_HM_INVAL;
	}

	return sysfs_read_dispatch((actvcall_sysfs_read_args *)recv.data);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_fops_sysfs_write,
			 dh_fops_sysfs_write_ext, unsigned long long, sender,
			 unsigned long, credential)
{
	struct actvpool_recv recv;

	recv = hm_actvpool_curr_recv_data(sender, credential);
	if (recv.size <= sizeof(actvcall_sysfs_write_args) || recv.data == NULL) {
		return E_HM_INVAL;
	}

	return sysfs_write_dispatch((actvcall_sysfs_write_args *)recv.data,
				    recv.size);
}

/*
 * sysfs sysif module, support sysfs read and write
 *
 * extension point: NULL
 *
 * extension:
 *  - target sysif-ep: `drvhandler_devhost_sysfs_read`
 *  - target sysif-ep: `drvhandler_devhost_sysfs_write`
 *  - target sysif-ep: `drvhandler_devhost_fops_sysfs_read`
 *  - target sysif-ep: `drvhandler_devhost_fops_sysfs_write`
 *  - target sysif-ep: `drvhandler_devhost_sysfs_open`
 *  - target sysif-ep: `drvhandler_devhost_sysfs_close`
 *
 * export symbol: NULL
 *
 * dependence: NULL
 */
DEFINE_BUILTIN_PLUGIN(sysfs, NULL,
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_sysfs_open, dh_sysfs_open_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_sysfs_close, dh_sysfs_close_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_sysfs_read, dh_sysfs_read_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_sysfs_write, dh_sysfs_write_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_sysfs_poll, dh_sysfs_poll_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_fops_sysfs_read, dh_fops_sysfs_read_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_fops_sysfs_write, dh_fops_sysfs_write_ext)
);
