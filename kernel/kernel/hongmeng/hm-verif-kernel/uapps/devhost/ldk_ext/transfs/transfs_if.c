/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Handler for transfs operation sysif APIs
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 28 16:05:44 2021
 */

#include <libsysif/devhost/server.h>

#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libstrict/strict.h>
#include <libhmsrv_io/transfs.h>
#include <devhost/file.h>
#include <devhost/plugin.h>
#include <devhost/memory.h>
#include <devhost/backend.h>
#include <devhost/transfs.h>
#include <libstrict/strict.h>
#include <vfs.h>
#include <lnxbase/lnxbase.h>
#include <lnxbase/page_pool.h>
#include <libhmsrv_crypt/entropy_register.h>

#include "table.h"
#include "transfs_dev.h"

#define MODULE_ARG_BUF_LEN	256

static transfs_mounted_t transfs_mount_if = NULL;
static const struct devhost_transfs_file_ops *trans_file_ops = NULL;
static ubi_major2num transfs_ubi_major2num = NULL;
static transfs_dget_t transfs_dget = NULL;
struct devhost_transfs_mounted_xactv_data {
	char *fs_name;
	char *dev_name;
	char *mount_opts;
	struct hmsrv_io_transfs_mount_recv *recv;
};

PUBLIC_SYMBOL
int devhost_register_transfs(transfs_mounted_t mnt_func,
			     const struct devhost_transfs_file_ops *fops,
			     ubi_major2num ubi_major2num_func,
			     transfs_dget_t dget_func)
{
	int ret;

	if (transfs_mount_if != NULL || trans_file_ops != NULL) {
		return -EEXIST;
	}

	ret = transfs_parent_device_init();
	if (ret != 0) {
		return ret;
	}

	transfs_mount_if = mnt_func;
	trans_file_ops = fops;
	transfs_ubi_major2num = ubi_major2num_func;
	transfs_dget = dget_func;

	return 0;
}

static int devhost_transfs_init_dev_path(char *dev_path, const char *fs_name,
					 const char *dev_name,
					 unsigned int dev_id, int dev_mode)
{
	int ret;

	if (dev_mode == (int)MODE_CHR && strcmp(fs_name, "ubifs") == 0) {
		int ubi_num = transfs_ubi_major2num(MAJOR(dev_id));
		if (ubi_num < 0) {
			return posix2hmerrno(-ubi_num);
		}
		/*
		 * Using ubiX_Y (mount UBI device number X, volume Y) method to
		 * mount ubifs.
		 */
		ret = sprintf_s(dev_path, PATH_MAX, "ubi%d_%u", ubi_num, MINOR(dev_id) - 1);
	} else if (dev_mode == (int)MODE_BLK) {
		/*
		 * <major>:<minor> major and minor number of the device used for
		 * block device mount process.
		 */
		ret = sprintf_s(dev_path, PATH_MAX, "%u:%u", MAJOR(dev_id),
				MINOR(dev_id));
	} else if (dev_mode == (int)MODE_NODEV) {
		if (strcmp(fs_name, "functionfs") == 0) {
			ret = sprintf_s(dev_path, PATH_MAX, dev_name);
		} else {
			ret = sprintf_s(dev_path, PATH_MAX, "nodev");
		}
	} else {
		hm_warn("invalid transfs fs_type %s for dev_mode %d\n", fs_name, dev_mode);
		return E_HM_OPNOTSUPP;
	}

	return (ret < 0 ? E_HM_POSIX_FAULT : E_HM_OK);
}

PUBLIC_SYMBOL
void *devhost_transfs_get_dentry(const char *pathname)
{
	unsigned int sb_index = 0;
	unsigned int d_index = 0;
	struct dentry *dentry = NULL;
	int err;

	if (pathname == NULL) {
		hm_error("pathname is NULL\n");
		return NULL;
	}

	err = vfs_transfs_query_dinfo(pathname, &sb_index, &d_index);
	if (err < 0) {
		hm_error("get file the sb index and dentry index of file %s error:%s\n",
			 pathname, hmstrerror(err));
		return NULL;
	}

	dentry = get_dentry_by_index_ref(sb_index, d_index, transfs_dget);
	if (dentry == NULL) {
		hm_warn("get the dentry of file %s failed\n", pathname);
	}

	return dentry;
}

PUBLIC_SYMBOL
void devhost_transfs_put_dentry(void *sb, void *dentry)
{
	unsigned int sb_index = 0;
	unsigned int d_index = 0;
	int ret;

	ret = get_sb_index_and_d_index(sb, &sb_index, dentry, &d_index);
	if (ret < 0) {
		hm_error("get sb_index and d_index error, ret:%s\n", hmstrerror(ret));
		return;
	}
	ret = vfs_transfs_put_dentry(sb_index, d_index);
	if (ret < 0) {
		hm_error("put dentry error:%s\n", hmstrerror(ret));
	}
}

PUBLIC_SYMBOL
int devhost_transfs_inode_permission(const char *pathname, int mask)
{
	int err;

	if (pathname == NULL) {
		hm_error("pathname is NULL\n");
		return E_HM_INVAL;
	}

	err = vfs_transfs_inode_permission(pathname, mask);
	if (err < 0) {
		hm_error("inode permission failed, pathname %s, mask:%d, err:%s\n", pathname, mask, hmstrerror(err));
		return err;
	}

	return 0;
}

PUBLIC_SYMBOL
void *devhost_transfs_fget(int fd, void (*fget)(void*))
{
	unsigned int sb_index = 0;
	unsigned int f_index = 0;
	unsigned int me_index = 0;
	unsigned long long hm_filp = 0;
	struct file *file = NULL;

	UNUSED(fget);
	if (vfs_transfs_fget(fd, &sb_index, &f_index, &me_index, &hm_filp) < 0) {
		return NULL;
	}
	file = get_file_by_index(sb_index, f_index);
	if (file != NULL) {
		int err = trans_file_ops->set_file_info(file, me_index, hm_filp);
		if (err < 0) {
			err = vfs_transfs_fput(me_index, hm_filp);
			if (err < 0) {
				hm_error("vfs transfs fput failed %s\n", hmstrerror(err));
			}
			return NULL;
		}
	}

	return file;
}

PUBLIC_SYMBOL
void devhost_transfs_fput(void *file)
{
	int err;
	unsigned int me_idx = 0;
	unsigned long long hm_filp = 0;

	err = trans_file_ops->get_file_info(file, &me_idx, &hm_filp);
	if (err != 0) {
		return;
	}
	if (vfs_transfs_fput(me_idx, hm_filp) < 0) {
		hm_error("vfs transfs fput failed\n");
	}
}

static int transfs_mounted_alloc_to_table(struct transfs_m_info_t *m_info)
{
	int ret = 0;

	/* alloc slot of table for sb, inode, dentry */
	ret = alloc_sb_to_table();
	if (ret < 0) {
		hm_error("alloc_sb_to_table failed\n");
		return ret;
	}
	m_info->sb_index = (unsigned int)ret;

	ret = alloc_inode_to_table(m_info->sb_index);
	if (ret < 0) {
		hm_error("alloc_inode_to_table failed\n");
		goto release_sb;
	}
	m_info->root_v_index = (unsigned int)ret;

	ret = alloc_dentry_to_table(m_info->sb_index);
	if (ret < 0) {
		hm_error("alloc_dentry_to_table failed\n");
		goto release_inode;
	}
	m_info->root_d_index = (unsigned int)ret;

	return ret;

release_inode:
	release_inode_to_table(m_info->sb_index, m_info->root_v_index);
release_sb:
	release_sb_to_table(m_info->sb_index);

	return ret;
}

static inline void transfs_set_tables(const struct transfs_m_info_t *m_info,
				      const struct transfs_mounted_ret *mount_ret)
{
	/* put sb, inode, dentry to target slot */
	set_sb_to_table(m_info->sb_index, mount_ret, m_info->per_write_size, m_info->s_reverse_query);
	set_inode_to_table(m_info->sb_index, m_info->root_v_index,
			   mount_ret->inode);
	set_dentry_to_table(m_info->sb_index, m_info->root_d_index,
			    mount_ret->dentry);
}

static void __transfs_mounted_free_xactv_data(struct devhost_transfs_mounted_xactv_data *data)
{
	if (data->fs_name != NULL) {
		free(data->fs_name);
		data->fs_name = NULL;
	}
	if (data->dev_name != NULL) {
		free(data->dev_name);
		data->dev_name = NULL;
	}
	if (data->mount_opts != NULL) {
		free(data->mount_opts);
		data->mount_opts = NULL;
	}
}

static void __transfs_mounted_release_data(const struct transfs_m_info_t *m_info,
				 char **dev_path,
				 struct devhost_transfs_mounted_xactv_data *xactv_data)
{
	release_dentry_to_table(m_info->sb_index, m_info->root_d_index);
	release_inode_to_table(m_info->sb_index, m_info->root_v_index);
	release_sb_to_table(m_info->sb_index);

	__transfs_mounted_free_xactv_data(xactv_data);

	if (*dev_path != NULL) {
		free(*dev_path);
		*dev_path = NULL;
	}
}

static int transfs_get_xactv_data(const struct hm_actv_buf *udata, char **data,
				  size_t max_len, bool is_usr)
{
	char *__data = NULL;
	int ret;

	if (udata->size == 0) {
		return E_HM_OK;
	}
	if (data == NULL) {
		return E_HM_INVAL;
	}
	if ((size_t)udata->size >= max_len) {
		return E_HM_INVAL;
	}

	/* Reserve one byte for '\0' */
	__data = malloc((size_t)udata->size + 1);
	if (__data == NULL) {
		return E_HM_NOMEM;
	}

	if (is_usr) {
		ret = hm_actv_read_xact_vm(__data, ulong_to_ptr(udata->buf, void), (size_t)udata->size);
	} else {
		ret = hm_actv_read_caller_buf(__data, (size_t)udata->size, udata);
	}
	if (ret < 0) {
		hm_error("get data from xactv is_usr:%d failed, %s\n", (int)is_usr, hmstrerror(ret));
		free(__data);
		return E_HM_POSIX_FAULT;
	}

	__data[udata->size] = '\0';

	*data = __data;
	return E_HM_OK;
}

static int __transfs_mounted_get_xactv_data(
			unsigned long long sender, unsigned long credential,
			struct devhost_transfs_mounted_xactv_data *xactv_data,
			struct hm_actv_buf *fs_name,
			struct hm_actv_buf *dev_name,
			struct hm_actv_buf *mount_opts)
{
	int ret;
	struct actvpool_recv recv = hm_actvpool_curr_recv_data(sender, credential);
	if ((recv.size != sizeof(*(xactv_data->recv))) || (recv.data == NULL)) {
		return E_HM_INVAL;
	}
	xactv_data->recv = (struct hmsrv_io_transfs_mount_recv *)(recv.data);

	ret = transfs_get_xactv_data(fs_name, &xactv_data->fs_name, MAX_FS_NAME_LEN, false);
	if (ret != E_HM_OK) {
		return ret;
	}
	ret = transfs_get_xactv_data(dev_name, &xactv_data->dev_name, MAX_FQSN_LEN, false);
	if (ret != E_HM_OK) {
		goto err_free_fs_name;
	}
	ret = transfs_get_xactv_data(mount_opts, &xactv_data->mount_opts, MAX_MNTOPTS_LEN, false);
	if (ret != E_HM_OK) {
		goto err_free_dev_name;
	}

	return E_HM_OK;
err_free_dev_name:
	free(xactv_data->dev_name);
	xactv_data->dev_name = NULL;
err_free_fs_name:
	free(xactv_data->fs_name);
	xactv_data->fs_name = NULL;

	return ret;
}

static int transfs_get_dev_path(const char *fs_name, const char *dev_name,
				dev_t dev_id, int dev_mode,
				char **dev_path_ret)
{
	char *dev_path = NULL;
	int ret;

	dev_path = calloc(PATH_MAX, sizeof(char));
	if (dev_path == NULL) {
		return E_HM_NOMEM;
	}

	ret = devhost_transfs_init_dev_path(dev_path, fs_name, dev_name,
					    (unsigned int)dev_id, dev_mode);
	if (ret < 0) {
		free(dev_path);
		return ret;
	}

	*dev_path_ret = dev_path;
	return E_HM_OK;
}

static int transfs_unregister_device(unsigned long long sender, unsigned long long file_id)
{
	int ret;
	__u32 cnode_idx;
	struct filp_node *node = NULL;
	struct device_info *devinfo = NULL;

	cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	acquire_devhost_filp_node(node, (uint32_t)cnode_idx,
				  0, (unsigned int)file_id);
	if (node == NULL || node->devinfo == NULL) {
		return E_HM_INVAL;
	}
	devinfo = node->devinfo;

	ret = devhost_unregister_file((uint64_t)cnode_idx, (unsigned int)file_id);
	if (ret != E_HM_OK) {
		hm_error("file_id is invalid\n");
		return ret;
	}

	ret = transfs_device_destroy(devinfo);
	if (ret != E_HM_OK) {
		hm_error("do trans device destroy err:%s\n", hmstrerror(ret));
	}

	return ret;
}

static inline int log_2(size_t _num)
{
	int nr = 0;
	size_t num = _num;

	if ((num & (num - 1)) != 0) {
		return -1;
	}
	while ((num >>= 1) != 0) {
		nr++;
	}
	return nr;
}

static void transfs_dev_file_init(unsigned long long sender,
				  unsigned int file_id,
				  unsigned int sector_node_bits,
				  const struct transfs_m_info_t *m_info,
				  const struct transfs_mounted_ret *mount_ret)
{
	int ret;
	__u32 cnode_idx;
	struct filp_node *node = NULL;
	struct trans_dev_file *trans_file = NULL;

	cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	acquire_devhost_filp_node(node, (uint32_t)cnode_idx, 0, file_id);
	BUG_ON(node == NULL);
	trans_file = (struct trans_dev_file *)node->filp;

	trans_file->sb_index = m_info->sb_index;
	ret = log_2(mount_ret->block_size);
	BUG_ON(ret < 0, "error ret:%d", ret);
	trans_file->block_size_shift = (unsigned int)ret;
	trans_file->sector_node_bits = sector_node_bits;
}

static int transfs_register_device(unsigned long long sender,
				   struct transfs_m_info_t *m_info,
				   unsigned int *file_id_ret)
{
	int ret;
	__u32 cnode_idx;
	struct filp_node *node = NULL;
	unsigned int file_id = 0;
	uint32_t trans_dev_id = 0;
	struct device_info *devinfo = NULL;

	ret = transfs_device_init(&devinfo, &trans_dev_id);
	if (ret != E_HM_OK) {
		hm_error("transfs get devinfo failed\n");
		return ret;
	}

	cnode_idx = __RPC_SENDER_DECODE_SRC_CIDX(sender);
	node = devhost_alloc_filp_node(devinfo, cnode_idx);
	if (node == NULL) {
		(void)transfs_device_destroy(devinfo);
		return E_HM_NOMEM;
	}

	ret = devhost_device_open(devinfo, &node->filp, 0);
	if (ret != 0) {
		ret = posix2hmerrno(-ret);
		goto out;
	}

	ret = devhost_register_file(node, &file_id);
	if (ret != E_HM_OK) {
		hm_error("failed to register file_node, err = %d, %s\n",
			 ret, hmstrerror(ret));
		goto out;
	}
	m_info->trans_dev_id = trans_dev_id;

	*file_id_ret = file_id;

	devhost_put_file(node);

	return E_HM_OK;

out:
	devhost_put_file(node);
	devinfo_put(devinfo);
	(void)transfs_device_destroy(devinfo);
	return ret;
}

static void transfs_fill_mounted_out(const struct transfs_mounted_ret *mount_ret,
				     const struct transfs_m_info_t *m_info,
				     unsigned int file_id,
				     struct transfs_m_info_t *m_info_out)
{
	m_info_out->root_lnkcnt = mount_ret->root_lnkcnt;
	m_info_out->block_size = mount_ret->block_size;
	m_info_out->s_time_gran = mount_ret->s_time_gran;
	m_info_out->s_max_time = mount_ret->s_max_time;
	m_info_out->s_min_time = mount_ret->s_min_time;

	m_info_out->root_v_index = m_info->root_v_index;
	m_info_out->root_d_index = m_info->root_d_index;
	m_info_out->sb_index = m_info->sb_index;
	m_info_out->per_write_size = m_info->per_write_size;
	m_info_out->trans_dev_id = m_info->trans_dev_id;

	m_info_out->filp = (unsigned long long)file_id;
}

static inline int transfs_mounted_table_init(struct transfs_m_info_t *m_info)
{
	int ret;

	/* init each table of sb */
	ret = transfs_sb_table_init();
	if (ret != E_HM_OK) {
		return ret;
	}

	return transfs_mounted_alloc_to_table(m_info);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_mounted, dh_transfs_mounted_ext,
			 unsigned long long, sender, unsigned long, credential,
			 struct hm_actv_buf, fs_name,
			 struct hm_actv_buf, dev_name,
			 struct hm_actv_buf, mount_opts)
{
	int ret;
	struct transfs_mounted_ret mount_ret;
	struct transfs_m_info_t m_info;
	struct devhost_transfs_mounted_xactv_data xactv_data = {0};
	char *trans_dev_path = NULL;
	unsigned int file_id = 0;

	struct __actvret_drvcall_devhost_transfs_mounted *__ret =
		actvhdlr_drvhandler_devhost_transfs_mounted_prepare_ret(sender, credential);

	BUG_ON_D(__ret == NULL);
	ret = __transfs_mounted_get_xactv_data(sender, credential, &xactv_data,
					       &fs_name, &dev_name, &mount_opts);
	if (ret != E_HM_OK) {
		return ret;
	}

	mem_zero_s(m_info);
	mem_zero_s(mount_ret);

	/* init each table of sb */
	ret = transfs_mounted_table_init(&m_info);
	if (ret != E_HM_OK) {
		return ret;
	}

	m_info.per_write_size = xactv_data.recv->per_write_size;

	ret = transfs_get_dev_path(xactv_data.fs_name, xactv_data.dev_name,
				   xactv_data.recv->dev_id, xactv_data.recv->dev_mode,
				   &trans_dev_path);
	if (ret != E_HM_OK) {
		goto err_out;
	}

	ret = transfs_register_device(sender, &m_info, &file_id);
	if (ret != E_HM_OK) {
		goto err_out;
	}

	if (strcmp(xactv_data.fs_name, "functionfs") == 0 ||
	    strcmp(xactv_data.fs_name, "configfs") == 0 ||
		strcmp(xactv_data.fs_name, "debugfs") == 0) {
		m_info.s_reverse_query = true;
	}

	ret = transfs_mount_if(xactv_data.fs_name, trans_dev_path, xactv_data.mount_opts,
			       xactv_data.recv->mount_flags, &mount_ret);
	if (ret < 0) {
		(void)transfs_unregister_device(sender, (unsigned long long)file_id);
		ret = posix2hmerrno(-ret);
		goto err_out;
	}

	transfs_dev_file_init(sender, file_id, xactv_data.recv->sector_node_bits, &m_info, &mount_ret);
	transfs_set_tables(&m_info, &mount_ret);
	/* return table index of sb, inode, dentry */
	transfs_fill_mounted_out(&mount_ret, &m_info, file_id, &__ret->m_info);

	free(trans_dev_path);
	__transfs_mounted_free_xactv_data(&xactv_data);

	return E_HM_OK;

err_out:
	__transfs_mounted_release_data(&m_info, &trans_dev_path, &xactv_data);

	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_unmounted, dh_transfs_unmounted_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, int, flags, unsigned long long, file_id)
{
	int ret;
	struct super_block *sb = NULL;
	struct vfsmount *vmnt = NULL;
	struct devhost_transfs_ops *trans_ops = NULL;
	void *data = NULL;

	UNUSED(credential);

	/* get sb, vmnt from table */
	sb = get_sb_by_index(s_index);
	if (sb == NULL) {
		return E_HM_INVAL;
	}
	data = get_data_by_sb_index(s_index);

	vmnt = get_vmnt_by_index(s_index);
	if (vmnt == NULL) {
		return E_HM_INVAL;
	}

	trans_ops = get_ops_by_index(s_index);
	if (trans_ops == NULL) {
		return E_HM_INVAL;
	}

	ret = transfs_unregister_device(sender, file_id);
	if (ret != E_HM_OK) {
		hm_error("do transfs dev destroy err:%s\n", hmstrerror(ret));
		return ret;
	}
	/* delete sb, mnt from table */
	ret = trans_del_sb_by_index(s_index, sb, vmnt, trans_ops);
	if (ret < 0) {
		hm_error("trans_del_sb_by_index failed\n");
		return ret;
	}

	trans_ops->unmounted(vmnt, flags, data);

	return E_HM_OK;
}

static int alloc_dentry_inode_index(unsigned int s_index,
				    struct transfs_dn_info_t *dn_info)
{
	int ret;

	if (dn_info == NULL) {
		return E_HM_POSIX_FAULT;
	}
	/* alloc slot of table for inode, dentry */
	ret = alloc_inode_to_table(s_index);
	if (ret < 0) {
		hm_error("alloc_inode_to_table failed\n");
		return ret;
	}
	dn_info->v_index = (unsigned int)ret;

	ret = alloc_dentry_to_table(s_index);
	if (ret < 0) {
		hm_error("alloc_dentry_to_table failed\n");
		release_inode_to_table(s_index, dn_info->v_index);
		return ret;
	}
	dn_info->d_index = (unsigned int)ret;

	return E_HM_OK;
}

static int do_prepare_trans(unsigned int s_index, unsigned int pd_index,
			    struct dentry **pdentry,
			    struct devhost_transfs_ops **trans_ops,
			    struct transfs_dn_info_t *dn_info)
{
	int ret;

	mem_zero_s(*dn_info);

	*pdentry = get_dentry_by_index(s_index, pd_index);
	if (*pdentry == NULL) {
		return E_HM_INVAL;
	}

	*trans_ops = get_ops_by_index(s_index);
	if (*trans_ops == NULL) {
		return E_HM_INVAL;
	}

	ret = alloc_dentry_inode_index(s_index, dn_info);
	if (ret < 0) {
		hm_error("alloc_inode_to_table failed\n");
		return ret;
	}

	return E_HM_OK;
}

/* put inode, dentry to the target slot */
static inline void set_dn_info(unsigned int s_index,
			       const struct transfs_dn_info_t *dn_info,
			       struct transfs_dn_meta_info_t *dn_meta_out)
{
	set_inode_to_table(s_index, dn_info->v_index, dn_meta_out->inode);
	set_dentry_to_table(s_index, dn_info->d_index, dn_meta_out->dentry);
}

/* release inode, dentry from the target slot */
static inline void release_dn_info(unsigned int s_index,
				   const struct transfs_dn_info_t *dn_info)
{
	release_dentry_to_table(s_index, dn_info->d_index);
	release_inode_to_table(s_index, dn_info->v_index);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_mkdir, dh_transfs_mkdir_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, unsigned int, pd_index, mode_t, mode,
			 struct hm_actv_buf, actv_dname)
{
	int ret;
	struct dentry *pdentry = NULL;
	struct transfs_dn_meta_info_t dn_meta_out;
	struct devhost_transfs_ops *trans_ops = NULL;
	struct transfs_dn_info_t dn_info;
	char *dname = NULL;
	struct __actvret_drvcall_devhost_transfs_mkdir *mkdir_ret =
		actvhdlr_drvhandler_devhost_transfs_mkdir_prepare_ret(sender, credential);

	BUG_ON_D(mkdir_ret == NULL);
	mem_zero_s(dn_meta_out);
	ret = do_prepare_trans(s_index, pd_index, &pdentry, &trans_ops, &dn_info);
	if (ret != E_HM_OK) {
		return ret;
	}

	ret = transfs_get_xactv_data(&actv_dname, &dname, PATH_MAX, false);
	if (ret != E_HM_OK) {
		goto err_out;
	}

	ret = trans_ops->mkdir(pdentry, mode, dname, &dn_meta_out);
	free(dname);
	if (ret < 0) {
		ret = posix2hmerrno(-ret);
		goto err_out;
	}

	set_dn_info(s_index, &dn_info, &dn_meta_out);

	/* return the index of dentry and inode */
	mkdir_ret->dn_info_out.d_index = dn_info.d_index;
	mkdir_ret->dn_info_out.v_index = dn_info.v_index;
	mkdir_ret->dn_info_out.ino = dn_meta_out.ino;

	return E_HM_OK;

err_out:
	release_dn_info(s_index, &dn_info);

	return ret;
}

#define TRANS_UNLINK 0
#define TRANS_RMDIR 1

static int do_unlink(unsigned int s_index, unsigned int pd_index,
		     unsigned int d_index, unsigned int type)
{
	int ret;
	struct dentry *pdentry = NULL;
	struct dentry *dentry = NULL;
	struct devhost_transfs_ops *trans_ops = NULL;

	trans_ops = get_ops_by_index(s_index);
	if (trans_ops == NULL) {
		return E_HM_INVAL;
	}

	/* get dentry, inode from table */
	pdentry = get_dentry_by_index(s_index, pd_index);
	if (pdentry == NULL) {
		return E_HM_INVAL;
	}

	dentry = get_dentry_by_index(s_index, d_index);
	if (dentry == NULL) {
		return E_HM_INVAL;
	}

	if (type == TRANS_UNLINK) {
		ret = trans_ops->unlink(pdentry, dentry);
	} else if (type == TRANS_RMDIR) {
		ret = trans_ops->rmdir(pdentry, dentry);
	} else {
		return E_HM_INVAL;
	}
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_rmdir, dh_transfs_rmdir_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, unsigned int, pd_index,
			 unsigned int, d_index)
{
	UNUSED(sender, credential);
	return do_unlink(s_index, pd_index, d_index, TRANS_RMDIR);
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_create, dh_transfs_create_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, unsigned int, pd_index,
			 mode_t, mode, struct hm_actv_buf, actv_dname)
{
	int ret;
	char *dname = NULL;
	struct dentry *pdentry = NULL;
	struct transfs_dn_meta_info_t dn_meta_out;
	struct devhost_transfs_ops *trans_ops = NULL;
	struct transfs_dn_info_t dn_info;
	struct __actvret_drvcall_devhost_transfs_create *create_ret =
		actvhdlr_drvhandler_devhost_transfs_create_prepare_ret(sender, credential);

	BUG_ON_D(create_ret == NULL);
	mem_zero_s(dn_meta_out);
	ret = do_prepare_trans(s_index, pd_index, &pdentry, &trans_ops, &dn_info);
	if (ret != E_HM_OK) {
		return ret;
	}

	ret = transfs_get_xactv_data(&actv_dname, &dname, PATH_MAX, false);
	if (ret != E_HM_OK) {
		goto err_out;
	}

	ret = trans_ops->create(pdentry, mode, dname, &dn_meta_out);
	free(dname);
	if (ret < 0) {
		ret = posix2hmerrno(-ret);
		goto err_out;
	}

	set_dn_info(s_index, &dn_info, &dn_meta_out);

	create_ret->dn_info_out.d_index = dn_info.d_index;
	create_ret->dn_info_out.v_index = dn_info.v_index;
	create_ret->dn_info_out.ino = dn_meta_out.ino;

	return E_HM_OK;

err_out:
	release_dn_info(s_index, &dn_info);

	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_unlink, dh_transfs_unlink_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, unsigned int, pd_index,
			 unsigned int, d_index)
{
	UNUSED(sender, credential);
	return do_unlink(s_index, pd_index, d_index, TRANS_UNLINK);
}

typedef struct {
	uint64_t pos;
	const void *src;
	size_t len;
	size_t per_write_size;
} transfs_segment_write_context_t;

static int do_prepare_trans_file(unsigned int s_index, unsigned int f_index,
				 struct file **file, struct devhost_transfs_ops **trans_ops)
{
	*file = get_file_by_index(s_index, f_index);
	if (*file == NULL) {
		return E_HM_INVAL;
	}

	*trans_ops = get_ops_by_index(s_index);
	if (*trans_ops == NULL) {
		return E_HM_INVAL;
	}

	return E_HM_OK;
}

static ssize_t transfs_segment_write(transfs_segment_write_context_t segment_write_context,
				     struct file *file,
				     long long (*write)(struct file *file,
							uint64_t pos,
							const void *src,
							size_t len))
{
	size_t write_offset = 0;
	size_t remain_size = segment_write_context.len;
	size_t per_write_size = segment_write_context.per_write_size;
	const void *src = segment_write_context.src;
	size_t ppos = (size_t)segment_write_context.pos;
	ssize_t per_ret = 0;
	ssize_t ret = 0;

	while (remain_size > 0) {
		if (per_write_size > remain_size) {
			per_write_size = remain_size;
		}
		per_ret = (ssize_t)write(file, (uint64_t)ppos,
					 (const void *)((const char *)src + write_offset),
					 per_write_size);
		if (per_ret <= 0) {
			return ret > 0 ? ret : per_ret;
		}
		write_offset += (size_t)per_ret;
		ret += per_ret;
		ppos += (size_t)per_ret;
		remain_size -= (size_t)per_ret;
		if (remain_size > segment_write_context.len) {
			hm_error("write error, remain size: %zu, write_context.len: %zu\n", remain_size, segment_write_context.len);
			return E_HM_INVAL;
		}
	}

	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_write, dh_transfs_write_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, unsigned int, f_index,
			 uint64_t, pos, const void *, src, size_t, len)
{
	ssize_t ret = 0;
	struct file *file = NULL;
	struct devhost_transfs_ops *trans_ops = NULL;
	struct __actvret_drvcall_devhost_transfs_write *__ret =
		actvhdlr_drvhandler_devhost_transfs_write_prepare_ret(sender, credential);

	BUG_ON_D(__ret == NULL);
	ret = (ssize_t)do_prepare_trans_file(s_index, f_index, &file, &trans_ops);
	if (ret < 0) {
		return (int)ret;
	}

	transfs_segment_write_context_t segment_write_context = {
		.pos = pos,
		.src = src,
		.len = len,
		.per_write_size = get_per_write_size_by_index(s_index),
	};

	ret = transfs_segment_write(segment_write_context, file, trans_ops->write);
	if (ret < 0) {
		return posix2hmerrno(-(int)ret);
	}

	__ret->nwritten = ret;

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_read, dh_transfs_read_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, unsigned int, f_index,
			 uint64_t, pos, const void *, src, size_t, len)
{
	ssize_t ret = 0;
	struct file *file = NULL;
	struct devhost_transfs_ops *trans_ops = NULL;
	struct __actvret_drvcall_devhost_transfs_read *__ret =
		actvhdlr_drvhandler_devhost_transfs_read_prepare_ret(sender, credential);

	BUG_ON_D(__ret == NULL);
	ret = (ssize_t)do_prepare_trans_file(s_index, f_index, &file, &trans_ops);
	if (ret < 0) {
		return (int)ret;
	}

	ret = (ssize_t)trans_ops->read(file, pos, src, len);
	if (ret < 0) {
		return posix2hmerrno(-(int)ret);
	}

	__ret->nread = ret;

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_write_iter, dh_transfs_write_iter_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, unsigned int, f_index, uint64_t, pos,
			 const void *, iov, int, nr_segs, size_t, count)
{
	ssize_t ret;
	struct file *file = NULL;
	struct iovec *_iov = NULL;
	struct devhost_transfs_ops *trans_ops = NULL;
	struct __actvret_drvcall_devhost_transfs_write_iter *__ret =
		actvhdlr_drvhandler_devhost_transfs_write_iter_prepare_ret(sender, credential);

	BUG_ON_D(__ret == NULL);
	ret = (ssize_t)do_prepare_trans_file(s_index, f_index, &file, &trans_ops);
	if (ret < 0) {
		return (int)ret;
	}

	BUG_ON(file == NULL || trans_ops == NULL);

	_iov = (struct iovec *)calloc((size_t)(unsigned int)nr_segs, sizeof(struct iovec));
	if (_iov == NULL) {
		return E_HM_NOMEM;
	}

	ret = (int)hm_actv_read_caller_vm(_iov, iov, (size_t)(unsigned int)nr_segs * sizeof(struct iovec));
	if (ret < 0) {
		free(_iov);
		return (int)ret;
	}

	ret = trans_ops->write_iter(file, pos, (struct iovec *)_iov, nr_segs, count);
	if (ret < 0) {
		free(_iov);
		return posix2hmerrno(-(int)ret);
	}

	free(_iov);
	__ret->nwritten = ret;

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_fsync, dh_transfs_fsync_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, unsigned int, f_index)
{
	int ret;
	struct devhost_transfs_ops *trans_ops = NULL;
	struct file *file = NULL;

	UNUSED(sender, credential);

	ret = do_prepare_trans_file(s_index, f_index, &file, &trans_ops);
	if (ret < 0) {
		return ret;
	}

	ret = trans_ops->fsync(file);
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_mknod, dh_transfs_mknod_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, unsigned int, pd_index, unsigned int, mode,
			 unsigned long long, devno, struct hm_actv_buf, actv_dname)
{
	int ret;
	struct dentry *pdentry = NULL;
	struct transfs_dn_meta_info_t dn_meta_out;
	struct devhost_transfs_ops *trans_ops = NULL;
	struct transfs_dn_info_t dn_info;
	char *dname = NULL;
	struct __actvret_drvcall_devhost_transfs_mknod *mknod_ret =
		actvhdlr_drvhandler_devhost_transfs_mknod_prepare_ret(sender, credential);

	BUG_ON_D(mknod_ret == NULL);
	mem_zero_s(dn_meta_out);
	ret = do_prepare_trans(s_index, pd_index, &pdentry, &trans_ops, &dn_info);
	if (ret != E_HM_OK) {
		return ret;
	}

	ret = transfs_get_xactv_data(&actv_dname, &dname, PATH_MAX, false);
	if (ret != E_HM_OK) {
		goto err_out;
	}

	ret = trans_ops->mknod(pdentry, mode, devno, dname, &dn_meta_out);
	free(dname);
	if (ret < 0) {
		ret = posix2hmerrno(-ret);
		goto err_out;
	}

	set_dn_info(s_index, &dn_info, &dn_meta_out);

	/* return the index of dentry, inode */
	mknod_ret->dn_info_out.d_index = dn_info.d_index;
	mknod_ret->dn_info_out.v_index = dn_info.v_index;
	mknod_ret->dn_info_out.ino = dn_meta_out.ino;

	return E_HM_OK;

err_out:
	release_dn_info(s_index, &dn_info);

	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_link, dh_transfs_link_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, unsigned int, old_d_index,
			 unsigned int, new_pd_index, struct hm_actv_buf, actv_dname)
{
	int ret;
	unsigned int new_d_index = 0;
	struct dentry *new_pdentry = NULL;
	struct dentry *old_dentry = NULL;
	struct dentry *new_dentry = NULL;
	char *dname = NULL;
	struct devhost_transfs_ops *trans_ops = NULL;
	struct __actvret_drvcall_devhost_transfs_link *__ret =
		actvhdlr_drvhandler_devhost_transfs_link_prepare_ret(sender, credential);

	BUG_ON_D(__ret == NULL);
	/* get dentry from table */
	new_pdentry = get_dentry_by_index(s_index, new_pd_index);
	if (new_pdentry == NULL) {
		return E_HM_INVAL;
	}

	old_dentry = get_dentry_by_index(s_index, old_d_index);
	if (old_dentry == NULL) {
		return E_HM_INVAL;
	}

	trans_ops = get_ops_by_index(s_index);
	if (trans_ops == NULL) {
		return E_HM_INVAL;
	}

	/* alloc slot of table for dentry */
	ret = alloc_dentry_to_table(s_index);
	if (ret < 0) {
		hm_error("alloc_dentry_to_table failed\n");
		return ret;
	}
	new_d_index = (unsigned int)ret;

	ret = transfs_get_xactv_data(&actv_dname, &dname, PATH_MAX, false);
	if (ret != E_HM_OK) {
		release_dentry_to_table(s_index, new_d_index);
		return ret;
	}

	ret = trans_ops->link(old_dentry, new_pdentry, dname, &new_dentry);
	free(dname);
	if (ret < 0) {
		release_dentry_to_table(s_index, new_d_index);
		return posix2hmerrno(-ret);
	}

	/* put dentry to the target slot */
	set_dentry_to_table(s_index, new_d_index, new_dentry);

	/* return the index of dentry */
	__ret->dn_info_out.d_index = new_d_index;

	return E_HM_OK;
}

static int transfs_symlink_get_dname_path(const struct hm_actv_buf *actv_dname,
					  const struct hm_actv_buf *actv_path,
					  char **dname, char **path)
{
	int ret;

	ret = transfs_get_xactv_data(actv_dname, dname, PATH_MAX, false);
	if (ret != E_HM_OK) {
		return ret;
	}

	ret = transfs_get_xactv_data(actv_path, path, PATH_MAX, true);
	if (ret != E_HM_OK) {
		free(*dname);
		*dname = NULL;
	}

	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_symlink, dh_transfs_symlink_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, unsigned int, pd_index,
			 struct hm_actv_buf, actv_dname, struct hm_actv_buf, actv_path)
{
	int ret;
	struct dentry *pdentry = NULL;
	struct transfs_dn_meta_info_t dn_meta_out;
	struct devhost_transfs_ops *trans_ops = NULL;
	struct transfs_dn_info_t dn_info;
	char *dname = NULL;
	char *path = NULL;
	struct __actvret_drvcall_devhost_transfs_symlink *__ret =
		actvhdlr_drvhandler_devhost_transfs_symlink_prepare_ret(sender, credential);

	BUG_ON_D(__ret == NULL);
	mem_zero_s(dn_meta_out);
	ret = do_prepare_trans(s_index, pd_index, &pdentry, &trans_ops, &dn_info);
	if (ret != E_HM_OK) {
		return ret;
	}
	ret = transfs_symlink_get_dname_path(&actv_dname, &actv_path, &dname, &path);
	if (ret != E_HM_OK) {
		goto out;
	}

	ret = trans_ops->symlink(pdentry, dname, path, &dn_meta_out);
	free(dname);
	free(path);
	if (ret < 0) {
		ret = posix2hmerrno(-ret);
		goto out;
	}

	/* put inode, dentry to the target slot */
	set_inode_to_table(s_index, dn_info.v_index, dn_meta_out.inode);
	set_dentry_to_table(s_index, dn_info.d_index, dn_meta_out.dentry);

	/* return the index of dentry and inode */
	__ret->dn_info_out.d_index = dn_info.d_index;
	__ret->dn_info_out.v_index = dn_info.v_index;
	__ret->dn_info_out.ino = dn_meta_out.ino;

	return E_HM_OK;

out:
	release_dentry_to_table(s_index, dn_info.d_index);
	release_inode_to_table(s_index, dn_info.v_index);

	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_statfs, dh_transfs_statfs_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, unsigned int, d_index, void *, buf)
{
	int ret;
	struct dentry *dentry = NULL;
	struct transfs_statfs transfs_st;
	struct statfs st;
	struct vfsmount *vmnt = NULL;
	struct devhost_transfs_ops *trans_ops = NULL;

	UNUSED(sender, credential);

	mem_zero_s(transfs_st);
	mem_zero_s(st);

	/* get dentry from table */
	dentry = get_dentry_by_index(s_index, d_index);
	if (dentry == NULL) {
		return E_HM_INVAL;
	}
	vmnt = get_vmnt_by_index(s_index);
	if (vmnt == NULL) {
		return E_HM_INVAL;
	}

	trans_ops = get_ops_by_index(s_index);
	if (trans_ops == NULL) {
		return E_HM_INVAL;
	}

	ret = trans_ops->statfs(vmnt, dentry, &transfs_st);
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	st.f_type = transfs_st.f_type;
	st.f_bsize = transfs_st.f_bsize;
	st.f_blocks = transfs_st.f_blocks;
	st.f_bfree = transfs_st.f_bfree;
	st.f_bavail = transfs_st.f_bavail;
	st.f_files = transfs_st.f_files;
	st.f_ffree = transfs_st.f_ffree;
	st.f_fsid.__val[0] = transfs_st.f_fsid.val[0];
	st.f_fsid.__val[1] = transfs_st.f_fsid.val[1];
	st.f_namelen = transfs_st.f_namelen;
	st.f_frsize = transfs_st.f_frsize;
	st.f_flags = transfs_st.f_flags;
	ret = memset_s(st.f_spare, sizeof(st.f_spare), 0, sizeof(st.f_spare));
	if (ret != 0) {
		return E_HM_POSIX_FAULT;
	}
	ret = hm_actv_write_xact_vm(buf, &st, sizeof(struct statfs));

	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_rename, dh_transfs_rename_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, sb_index, unsigned int, old_pd_index,
			 unsigned int, old_d_index, unsigned int, new_pd_index,
			 unsigned int, new_d_index, struct hm_actv_buf, actv_dname)
{
	int ret;
	bool need_new = (new_d_index == UINT_MAX) ? true : false;
	struct dentry *old_pdentry = NULL;
	struct dentry *old_dentry = NULL;
	struct dentry *new_pdentry = NULL;
	struct dentry *new_dentry = NULL;
	char *dname = NULL;
	struct devhost_transfs_ops *trans_ops = NULL;

	UNUSED(sender, credential);

	/* get dentry from table */
	old_pdentry = get_dentry_by_index(sb_index, old_pd_index);
	if (old_pdentry == NULL) {
		return E_HM_INVAL;
	}

	old_dentry = get_dentry_by_index(sb_index, old_d_index);
	if (old_dentry == NULL) {
		return E_HM_INVAL;
	}

	new_pdentry = get_dentry_by_index(sb_index, new_pd_index);
	if (new_pdentry == NULL) {
		return E_HM_INVAL;
	}

	trans_ops = get_ops_by_index(sb_index);
	if (trans_ops == NULL) {
		return E_HM_INVAL;
	}

	if (!need_new) {
		new_dentry = get_dentry_by_index(sb_index, new_d_index);
		if (new_dentry == NULL) {
			return E_HM_INVAL;
		}
	}

	ret = transfs_get_xactv_data(&actv_dname, &dname, PATH_MAX, false);
	if (ret != E_HM_OK) {
		return ret;
	}

	ret = trans_ops->rename(old_pdentry, old_dentry, new_pdentry, new_dentry, dname);
	free(dname);
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_readdir, dh_transfs_readdir_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, sb_index, unsigned int, f_index,
			 uint64_t, pos, void *, buf, size_t, size)
{
	int ret;
	struct file *file = NULL;
	uint64_t fpos = pos;
	struct devhost_transfs_ops *trans_ops = NULL;
	struct __actvret_drvcall_devhost_transfs_readdir *__ret =
		actvhdlr_drvhandler_devhost_transfs_readdir_prepare_ret(sender, credential);

	BUG_ON_D(__ret == NULL);
	ret = do_prepare_trans_file(sb_index, f_index, &file, &trans_ops);
	if (ret < 0) {
		return ret;
	}

	ret = trans_ops->readdir(file, &fpos, buf, size);
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	__ret->fpos = (int64_t)fpos;

	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_lookup, dh_transfs_lookup_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, sb_index, unsigned int, pd_index,
			 struct hm_actv_buf, actv_dname, void *, meta)
{
	int ret;
	struct dentry *pdentry = NULL;
	struct transfs_dn_meta_info_t dn_meta_out;
	struct devhost_transfs_ops *trans_ops = NULL;
	struct transfs_dn_info_t dn_info;
	char *dname = NULL;
	struct __actvret_drvcall_devhost_transfs_lookup *__ret =
		actvhdlr_drvhandler_devhost_transfs_lookup_prepare_ret(sender, credential);

	BUG_ON_D(__ret == NULL);
	mem_zero_s(dn_info);
	mem_zero_s(dn_meta_out);

	/* get dentry from table */
	pdentry = get_dentry_by_index(sb_index, pd_index);
	if (pdentry == NULL) {
		return E_HM_INVAL;
	}

	trans_ops = get_ops_by_index(sb_index);
	if (trans_ops == NULL) {
		return E_HM_INVAL;
	}

	ret = transfs_get_xactv_data(&actv_dname, &dname, PATH_MAX, false);
	if (ret != E_HM_OK) {
		return ret;
	}
	BUG_ON(dname == NULL, "get dname from actv-stack failed\n");

	ret = trans_ops->lookup(pdentry, dname, meta, &dn_meta_out);
	free(dname);
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	ret = alloc_dentry_inode_index(sb_index, &dn_info);
	if (ret < 0) {
		hm_error("alloc_inode_to_table failed\n");
		return ret;
	}

	/* put inode, dentry to the target slot */
	set_inode_to_table(sb_index, dn_info.v_index, dn_meta_out.inode);
	set_dentry_to_table(sb_index, dn_info.d_index, dn_meta_out.dentry);

	/* return the index of dentry and inode */
	__ret->dn_info_out.d_index = dn_info.d_index;
	__ret->dn_info_out.v_index = dn_info.v_index;

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_setattr, dh_transfs_setattr_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, sb_index, unsigned int, d_index,
			 const void *, attr, uint64_t, size)
{
	int ret;
	struct transfs_vattr vattr;
	struct dentry *dentry = NULL;
	struct devhost_transfs_ops *trans_ops = NULL;

	UNUSED(sender, credential);

	mem_zero_s(vattr);

	/* get dentry from table */
	dentry = get_dentry_by_index(sb_index, d_index);
	if (dentry == NULL) {
		return E_HM_INVAL;
	}

	trans_ops = get_ops_by_index(sb_index);
	if (trans_ops == NULL) {
		return E_HM_INVAL;
	}

	ret = hm_actv_read_xact_vm(&vattr, attr, sizeof(struct transfs_vattr));
	if (ret != 0) {
		return ret;
	}

	ret = trans_ops->setattr(dentry, &vattr, size);
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_readlink, dh_transfs_readlink_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, sb_index, unsigned int, d_index, char *, buf,
			 size_t, bufsize)
{
	int ret;
	struct dentry *dentry = NULL;
	struct devhost_transfs_ops *trans_ops = NULL;

	UNUSED(sender, credential);

	/* get dentry from table */
	dentry = get_dentry_by_index(sb_index, d_index);
	if (dentry == NULL) {
		return E_HM_INVAL;
	}

	trans_ops = get_ops_by_index(sb_index);
	if (trans_ops == NULL) {
		return E_HM_INVAL;
	}

	ret = trans_ops->readlink(dentry, buf, bufsize);
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_open, dh_transfs_open_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, sb_index, unsigned int, d_index,
			 unsigned int, f_modes, unsigned int, f_flags)
{
	int ret;
	unsigned int f_index = 0;
	struct dentry *dentry = NULL;
	struct vfsmount *vmnt = NULL;
	struct file *file = NULL;
	struct devhost_transfs_ops *trans_ops = NULL;
	struct __actvret_drvcall_devhost_transfs_open *__ret =
		actvhdlr_drvhandler_devhost_transfs_open_prepare_ret(sender, credential);

	BUG_ON_D(__ret == NULL);
	/* get vmnt, dentry from table */
	vmnt = get_vmnt_by_index(sb_index);
	if (vmnt == NULL) {
		return E_HM_INVAL;
	}

	dentry = get_dentry_by_index(sb_index, d_index);
	if (dentry == NULL) {
		return E_HM_INVAL;
	}

	trans_ops = get_ops_by_index(sb_index);
	if (trans_ops == NULL) {
		return E_HM_INVAL;
	}

	/* alloc slot of table for file */
	ret = alloc_file_to_table(sb_index);
	if (ret < 0) {
		hm_error("alloc file to table failed\n");
		return ret;
	}
	f_index = (unsigned int)ret;

	ret = trans_ops->open(vmnt, dentry, f_modes, f_flags, &file);
	if (ret < 0) {
		release_file_to_table(sb_index, f_index);
		return posix2hmerrno(-ret);
	}

	/* put file to the target slot */
	set_file_to_table(sb_index, f_index, file);

	/* return the index of file */
	__ret->f_index = f_index;

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_fallocate, dh_transfs_fallocate_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, sb_index, unsigned int, f_index,
			 unsigned int, mode, uint64_t, pos, uint64_t, len)
{
	int ret;
	struct file *file = NULL;
	struct devhost_transfs_ops *trans_ops = NULL;

	UNUSED(sender, credential);

	ret = do_prepare_trans_file(sb_index, f_index, &file, &trans_ops);
	if (ret < 0) {
		return ret;
	}

	ret = trans_ops->fallocate(file, mode, pos, len);
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_getattr, dh_transfs_getattr_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, sb_index, unsigned int, d_index, void *, meta)
{
	int ret;
	struct dentry *dentry = NULL;
	struct vfsmount *vmnt = NULL;
	struct devhost_transfs_ops *trans_ops = NULL;

	UNUSED(sender, credential);

	/* get dentry, vmnt from table */
	dentry = get_dentry_by_index(sb_index, d_index);
	if (dentry == NULL) {
		return E_HM_INVAL;
	}

	vmnt = get_vmnt_by_index(sb_index);
	if (vmnt == NULL) {
		return E_HM_INVAL;
	}

	trans_ops = get_ops_by_index(sb_index);
	if (trans_ops == NULL) {
		return E_HM_INVAL;
	}

	ret = trans_ops->getattr(vmnt, dentry, meta);
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_listxattr, dh_transfs_listxattr_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, unsigned int, d_index, char *, buffer,
			 size_t, size)
{
	ssize_t ret;
	struct dentry *dentry = NULL;
	struct devhost_transfs_ops *trans_ops = NULL;
	struct __actvret_drvcall_devhost_transfs_listxattr *__ret =
		actvhdlr_drvhandler_devhost_transfs_listxattr_prepare_ret(sender, credential);

	BUG_ON_D(__ret == NULL);
	/* get dentry from table */
	dentry = get_dentry_by_index(s_index, d_index);
	if (dentry == NULL) {
		return E_HM_INVAL;
	}

	trans_ops = get_ops_by_index(s_index);
	if (trans_ops == NULL) {
		return E_HM_INVAL;
	}

	ret = (ssize_t)trans_ops->listxattr(dentry, buffer, size);
	if (ret < 0) {
		return posix2hmerrno(-(int)ret);
	}

	__ret->written = ret;

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_getxattr, dh_transfs_getxattr_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, unsigned int, d_index, int, prefix,
			 struct hm_actv_buf, actv_name, char *, value,
			 size_t, size)
{
	ssize_t ret;
	struct dentry *dentry = NULL;
	char *name = NULL;
	struct devhost_transfs_ops *trans_ops = NULL;
	struct __actvret_drvcall_devhost_transfs_getxattr *__ret =
		actvhdlr_drvhandler_devhost_transfs_getxattr_prepare_ret(sender, credential);

	BUG_ON_D(__ret == NULL);
	/* get dentry from table */
	dentry = get_dentry_by_index(s_index, d_index);
	if (dentry == NULL) {
		return E_HM_INVAL;
	}

	trans_ops = get_ops_by_index(s_index);
	if (trans_ops == NULL) {
		return E_HM_INVAL;
	}

	ret = transfs_get_xactv_data(&actv_name, &name, (size_t)actv_name.size + 1, false);
	if (ret != E_HM_OK) {
		return (int)ret;
	}
	ret = (ssize_t)trans_ops->getxattr(dentry, prefix, name, value, size);
	free(name);
	if (ret < 0) {
		return posix2hmerrno(-(int)ret);
	}

	__ret->nbytes = ret;

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_setxattr, dh_transfs_setxattr_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, unsigned int, d_index, int, prefix,
			 struct hm_actv_buf, actv_name, const char *, value,
			 size_t, size, int, flags)
{
	ssize_t ret;
	struct dentry *dentry = NULL;
	char *name = NULL;
	char *value_buf = NULL;
	struct devhost_transfs_ops *trans_ops = NULL;

	UNUSED(sender, credential);

	/* get dentry from table */
	dentry = get_dentry_by_index(s_index, d_index);
	if (dentry == NULL) {
		return E_HM_INVAL;
	}

	trans_ops = get_ops_by_index(s_index);
	if (trans_ops == NULL) {
		hm_warn("get ops failed\n");
		return E_HM_INVAL;
	}

	ret = transfs_get_xactv_data(&actv_name, &name, (size_t)actv_name.size + 1, false);
	if (ret != E_HM_OK) {
		return (int)ret;
	}

	if (size > 0) {
		value_buf = malloc(size);
		if (value_buf == NULL) {
			free(name);
			return E_HM_NOMEM;
		}
		ret = hm_actv_read_xact_vm(value_buf, value, size);
		if (ret != E_HM_OK) {
			free(name);
			free(value_buf);
			return (int)ret;
		}
	} else if (value != NULL) {
		/* empty EA, do not remove */
		value_buf = "";
	}
	ret = (ssize_t)trans_ops->setxattr(dentry, prefix, name, value_buf,
					   size, flags);
	free(name);
	if (size > 0) {
		free(value_buf);
	}
	if (ret < 0) {
		return posix2hmerrno(-(int)ret);
	}

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_tmpfile, dh_transfs_tmpfile_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, unsigned int, pd_index,
			 mode_t, mode, struct hm_actv_buf, actv_dname)
{
	int ret;
	struct dentry *pdentry = NULL;
	struct transfs_dn_meta_info_t dn_meta_out;
	struct devhost_transfs_ops *trans_ops = NULL;
	struct transfs_dn_info_t dn_info;
	char *dname = NULL;
	struct __actvret_drvcall_devhost_transfs_tmpfile *tmpfile_ret =
		actvhdlr_drvhandler_devhost_transfs_tmpfile_prepare_ret(sender, credential);

	BUG_ON_D(tmpfile_ret == NULL);
	mem_zero_s(dn_meta_out);
	ret = do_prepare_trans(s_index, pd_index, &pdentry, &trans_ops, &dn_info);
	if (ret != E_HM_OK) {
		return ret;
	}

	ret = transfs_get_xactv_data(&actv_dname, &dname, PATH_MAX, false);
	if (ret != E_HM_OK) {
		goto err_out;
	}

	ret = trans_ops->tmpfile(pdentry, mode, dname, &dn_meta_out);
	free(dname);
	if (ret < 0) {
		ret = posix2hmerrno(-ret);
		goto err_out;
	}

	set_dn_info(s_index, &dn_info, &dn_meta_out);

	/* return the index of inode and dentry */
	tmpfile_ret->dn_info_out.d_index = dn_info.d_index;
	tmpfile_ret->dn_info_out.v_index = dn_info.v_index;
	tmpfile_ret->dn_info_out.ino = dn_meta_out.ino;

	return E_HM_OK;

err_out:
	release_dn_info(s_index, &dn_info);

	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_sync, dh_transfs_sync_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index)
{
	int ret;
	struct super_block *sb = NULL;
	struct devhost_transfs_ops *trans_ops = NULL;

	UNUSED(sender, credential);

	/* get inode from table */
	sb = get_sb_by_index(s_index);
	if (sb == NULL) {
		return E_HM_INVAL;
	}

	trans_ops = get_ops_by_index(s_index);
	if (trans_ops == NULL) {
		return E_HM_INVAL;
	}

	ret = trans_ops->sync(sb);
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_drop_caches, dh_transfs_drop_caches_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, unsigned int, mode, int, max_nr)
{
	int ret;
	struct devhost_transfs_ops *trans_ops = NULL;
	struct super_block *sb = NULL;

	UNUSED(sender, credential);

	trans_ops = get_ops_by_index(s_index);
	if (trans_ops == NULL) {
		return E_HM_INVAL;
	}

	sb = get_sb_by_index(s_index);
	if (sb == NULL) {
		return E_HM_INVAL;
	}

	ret = trans_ops->drop_caches(sb, mode, max_nr);
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	return ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_close, dh_transfs_close_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, unsigned int, f_index)
{
	int ret;
	struct file *file = NULL;
	struct devhost_transfs_ops *trans_ops = NULL;

	UNUSED(sender, credential);

	ret = do_prepare_trans_file(s_index, f_index, &file, &trans_ops);
	if (ret < 0) {
		return ret;
	}

	/* delete file from table */
	ret = del_file_by_index(s_index, f_index);
	if (ret < 0) {
		hm_error("del_file_by_index failed\n");
		return ret;
	}

	/* release the slot of table */
	release_file_to_table(s_index, f_index);

	trans_ops->close(file);

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_ioctl, dh_transfs_ioctl_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, unsigned int, f_index,
			 unsigned int, cmd, unsigned long, arg)
{
	int ret;
	struct file *file = NULL;
	struct devhost_transfs_ops *trans_ops = NULL;

	UNUSED(sender, credential);

	ret = do_prepare_trans_file(s_index, f_index, &file, &trans_ops);
	if (ret < 0) {
		return ret;
	}

	ret = trans_ops->ioctl(file, cmd, arg);
	if (ret < 0) {
		return posix2hmerrno(-ret);
	}

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_direct_IO, dh_transfs_direct_IO_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, unsigned int, f_index, uint64_t, offset,
			 void *, buf, size_t, len, unsigned int, type)
{
	ssize_t ret;
	struct file *file = NULL;
	struct devhost_transfs_ops *trans_ops = NULL;
	struct __actvret_drvcall_devhost_transfs_direct_IO *__ret =
		actvhdlr_drvhandler_devhost_transfs_direct_IO_prepare_ret(sender, credential);

	BUG_ON_D(__ret == NULL);
	ret = (ssize_t)do_prepare_trans_file(s_index, f_index, &file, &trans_ops);
	if (ret < 0) {
		return (int)ret;
	}

	ret = (ssize_t)trans_ops->direct_IO(file, offset, buf, len, type);
	if (ret < 0) {
		return posix2hmerrno(-(int)ret);
	}

	__ret->nbytes = ret;

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_kill_dentry, dh_transfs_kill_dentry_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, unsigned int, d_index)
{
	int ret;
	struct dentry *dentry = NULL;
	struct devhost_transfs_ops *trans_ops = NULL;

	UNUSED(sender, credential);

	/* get dentry from table */
	dentry = get_dentry_by_index(s_index, d_index);
	if (dentry == NULL) {
		return E_HM_INVAL;
	}

	trans_ops = get_ops_by_index(s_index);
	if (trans_ops == NULL) {
		return E_HM_INVAL;
	}

	/* delete dentry from table */
	ret = del_dentry_by_index(s_index, d_index);
	if (ret < 0) {
		hm_error("del_dentry_by_index failed\n");
		return ret;
	}

	/* release the slot for dentry */
	release_dentry_to_table(s_index, d_index);

	trans_ops->kill_dentry(dentry);

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_node_put, dh_transfs_node_put_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, unsigned int, v_index)
{
	struct inode *inode = NULL;
	struct devhost_transfs_ops *trans_ops = NULL;

	UNUSED(sender, credential);

	/* get inode from table */
	inode = get_inode_by_index(s_index, v_index);
	if (inode == NULL) {
		return E_HM_INVAL;
	}

	trans_ops = get_ops_by_index(s_index);
	if (trans_ops == NULL) {
		return E_HM_INVAL;
	}

	/* release the slot for inode */
	release_inode_to_table(s_index, v_index);

	trans_ops->node_put(inode);

	return E_HM_OK;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_update_opts, dh_transfs_update_opts_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, char *, ubuf, size_t, buflen)
{
	UNUSED(sender, credential);
	ssize_t ret;
	struct devhost_transfs_ops *trans_ops = NULL;
	struct super_block *sb = NULL;
	char *buf = NULL;

	if (buflen == 0) {
		return E_HM_INVAL;
	}
	trans_ops = get_ops_by_index(s_index);
	if (trans_ops == NULL) {
		return E_HM_INVAL;
	}
	sb = get_sb_by_index(s_index);
	if (sb == NULL) {
		return E_HM_INVAL;
	}
	buf = malloc(buflen);
	if (buf == NULL) {
		hm_warn("malloc failed, buflen=%lu\n", (unsigned long)buflen);
		return E_HM_NOMEM;
	}

	ret = trans_ops->update_opts(sb, buf, buflen);
	if (ret >= 0) {
		ret = hm_actv_write_caller_vm((void *)ubuf, (void *)buf,
					      (size_t)ret);
	} else {
		ret = posix2hmerrno(-(int)ret);
	}
	free(buf);

	return (int)ret;
}

DEFINE_SYSIF_EXT_HANDLER(drvhandler_devhost_transfs_iommap, dh_transfs_iommap_ext,
			 unsigned long long, sender, unsigned long, credential,
			 unsigned int, s_index, unsigned int, f_index,
			 unsigned long, addr,
			 unsigned long long, vspace_rref,
			 unsigned long, length,
			 unsigned long, prot,
			 unsigned int, flags,
			 unsigned long, offset)
{
	int ret;
	struct file *file = NULL;
	struct devhost_transfs_ops *trans_ops = NULL;
	struct __actvret_drvcall_devhost_transfs_iommap *__ret =
		actvhdlr_drvhandler_devhost_transfs_iommap_prepare_ret(sender, credential);
	struct devhost_vma_info *vma_info = NULL;

	BUG_ON_D(__ret == NULL);
	UNUSED(vspace_rref);
	ret = do_prepare_trans_file(s_index, f_index, &file, &trans_ops);
	if (ret < 0) {
		return ret;
	}

	ret = devhost_pal_usermap_prepare(addr, length, prot, flags, true /* prepare va ahead */, &vma_info);
	if (ret < 0) {
		return ret;
	}
	vma_info->offset = offset;
	ret = trans_ops->iommap(file, vma_info);
	if (ret < 0) {
		devhost_pal_usermap_finish(vma_info, true /* prepare va ahead */, 0 /* reclaim */, 0);
		return posix2hmerrno(-ret);
	}
	devhost_pal_usermap_finish(vma_info, true /* prepare va ahead */, 1 /* success */, 0);
	__ret->addr = (unsigned long long)vma_info->start;
	__ret->iomem_id = vma_info->vma_id;
	return E_HM_OK;
}

static int __transfs_parse_args(char *args, size_t size)
{
	int ret;
	const char *clean_wm = NULL;
	const char *sync_wm = NULL;

	clean_wm = lnxbase_backend_args_find("transfs_page_clean_watermark");
	if (clean_wm != NULL) {
		ret = strcat_s(args, size, "transfs_page_clean_watermark=");
		if (unlikely(ret != EOK)) {
			hm_error("transfs_if: strcat failed for clean, %s\n",
				 hmstrerror(posix2hmerrno(-ret)));
			return posix2hmerrno(-ret);
		}
		ret = strcat_s(args, size, clean_wm);
		if (unlikely(ret != EOK)) {
			hm_error("transfs_if: strcat failed for clean value, %s\n",
				 hmstrerror(posix2hmerrno(-ret)));
			return posix2hmerrno(-ret);
		}
	}

	sync_wm = lnxbase_backend_args_find("transfs_page_sync_watermark");
	if (sync_wm != NULL) {
		ret = strcat_s(args, size, " transfs_page_sync_watermark=");
		if (unlikely(ret != EOK)) {
			hm_error("transfs_if: strcat failed for sync, %s\n",
				 hmstrerror(posix2hmerrno(-ret)));
			return posix2hmerrno(-ret);
		}
		ret = strcat_s(args, size, sync_wm);
		if (unlikely(ret != EOK)) {
			hm_error("transfs_if: strcat failed for sync value, %s\n",
				 hmstrerror(posix2hmerrno(-ret)));
			return posix2hmerrno(-ret);
		}
	}

	return E_HM_OK;
}

/* load transfs module */
static int transfs_load_module(const struct libdh_dso_plugin *plugin)
{
	UNUSED(plugin);

	char args[MODULE_ARG_BUF_LEN] = {0};
	struct page_pool *pgcache_pool = NULL;
	int ret;

	/* parse transfs args from backend */
	ret = __transfs_parse_args(args, sizeof(args));
	if (ret < 0) {
		return ret;
	}

	/* setup pagecache pool if configured */
	pgcache_pool = lnxbase_page_pool_get("pagecache");
	if (pgcache_pool != NULL) {
		ret = lnxbase_page_pool_setup_default(pgcache_pool);
		if (ret < 0) {
			hm_error("setup pagecache pool failed, ret=%s\n",
				 hmstrerror(ret));
			return ret;
		}
	}

	/* load support module */
	ret = devhost_pal_request_module_args("transfs.ko", args);
	if (ret < 0) {
		hm_error("lnxbase: load transfs.ko failed, %s\n", hmstrerror(ret));
		return ret;
	}

	return 0;
}

DEFINE_DSO_PLUGIN(transfs, transfs_load_module,
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_mounted, dh_transfs_mounted_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_unmounted, dh_transfs_unmounted_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_mkdir, dh_transfs_mkdir_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_rmdir, dh_transfs_rmdir_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_create, dh_transfs_create_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_unlink, dh_transfs_unlink_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_write, dh_transfs_write_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_read, dh_transfs_read_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_write_iter, dh_transfs_write_iter_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_fsync, dh_transfs_fsync_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_mknod, dh_transfs_mknod_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_link, dh_transfs_link_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_symlink, dh_transfs_symlink_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_statfs, dh_transfs_statfs_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_rename, dh_transfs_rename_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_readdir, dh_transfs_readdir_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_lookup, dh_transfs_lookup_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_setattr, dh_transfs_setattr_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_readlink, dh_transfs_readlink_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_open, dh_transfs_open_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_fallocate, dh_transfs_fallocate_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_getattr, dh_transfs_getattr_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_listxattr, dh_transfs_listxattr_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_getxattr, dh_transfs_getxattr_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_setxattr, dh_transfs_setxattr_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_tmpfile, dh_transfs_tmpfile_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_sync, dh_transfs_sync_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_drop_caches, dh_transfs_drop_caches_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_close, dh_transfs_close_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_ioctl, dh_transfs_ioctl_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_direct_IO, dh_transfs_direct_IO_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_kill_dentry, dh_transfs_kill_dentry_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_node_put, dh_transfs_node_put_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_update_opts, dh_transfs_update_opts_ext),
	IMPORT_SYSIF_EXTENSION(drvhandler_devhost_transfs_iommap, dh_transfs_iommap_ext)
);
