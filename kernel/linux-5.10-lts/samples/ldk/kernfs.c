// SPDX-License-Identifier: GPL-2.0
#include <linux/fs.h>
#include <linux/idr.h>
#include <linux/file.h>
#include <linux/stat.h>
#include <linux/mount.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <uapi/linux/mount.h>
#include <linux/init_syscalls.h>
#include <net/net_namespace.h>

#include "../../fs/kernfs/kernfs-internal.h"
#include "../../fs/sysfs/sysfs.h"
#include "file.h"
#include "sysfs_batch.h"
#include "kernfs.h"
#include "procfs.h"

#include <trace/hooks/liblinux.h>

static struct vfsmount *sysfs_mnt = NULL;
static struct kernfs_root *sysfs_root = NULL;
static DECLARE_COMPLETION(sysfs_mnt_done);
static DEFINE_IDR(sysfs_idr);
static DEFINE_IDR(kernfs_idr);
static int liblinux_fs_tag_min = INT_MAX;
static int liblinux_fs_tag_max = INT_MIN;
struct mutex g_liblinux_lock;
struct file *alloc_file(const struct path *path, int flags,
						const struct file_operations *fop);

static struct inode *__liblinux_kernfs_iget(struct kernfs_node *kn)
{
	struct kernfs_root *root = kernfs_root(kn);

	if (sysfs_mnt == NULL || root == NULL) {
		return NULL;
	}
	if (root->kn != sysfs_root_kn) { /* only support sysfs now */
		return NULL;
	}
	return kernfs_get_inode(sysfs_mnt->mnt_sb, kn);
}

static int __liblinux_kernfs_open(struct dentry *dentry, int mode,
				  struct file **file)
{
	struct path path = {
		.mnt = NULL,
		.dentry = dentry,
	};
	struct file *f = NULL;
	struct inode *inode = dentry->d_inode;
	int ret = -EINVAL;

	if (fops_get(inode->i_fop) == NULL) {
		return -EINVAL;
	}

	f = alloc_file(&path, mode, inode->i_fop);
	if (IS_ERR(f)) {
		fops_put(inode->i_fop);
		return PTR_ERR(f);
	}

	if (f->f_op->open != NULL) {
		ret = f->f_op->open(inode, f);
	}

	if (ret < 0) {
		put_filp(f);
		return ret;
	}

	f->f_path.dentry = NULL;
	/* the dentry does not work, disable notify for kernfs */
	f->f_mode |= FMODE_NONOTIFY;
	*file = f;
	return 0;
}

static int __liblinux_kernfs_read(unsigned long long pos, unsigned long ubuf,
				  unsigned long size, void *ctx,
				  unsigned long *rsize /* out */)
{
	struct dentry dentry;
	struct kernfs_node *kn = NULL;
	struct file *f = NULL;
	int ret = -EINVAL;
	loff_t pos_loff;

	mutex_lock(&g_liblinux_lock);
	kn = idr_find(&sysfs_idr, *(int *)ctx);
	if ((kn == NULL) || (size > SIZE_MAX) || (pos >= LLONG_MAX)) {
		mutex_unlock(&g_liblinux_lock);
		return -EINVAL;
	}

	kernfs_get(kn);
	mutex_unlock(&g_liblinux_lock);

	pos_loff = (loff_t)pos;
	memset(&dentry, 0, sizeof(struct dentry));
	dentry.d_fsdata = (void *)kn;
	dentry.d_inode = __liblinux_kernfs_iget(kn);
	if (dentry.d_inode == NULL) {
		goto err_iget;
	}

	/* not support !KERNFS_FILE open */
	BUG_ON(!(kernfs_type(kn) == KERNFS_FILE));
	ret = __liblinux_kernfs_open(&dentry, O_RDONLY, &f);
	if (ret < 0) {
		goto err_open;
	}

	ret = vfs_read_full(f, (char __user *)(uintptr_t)ubuf, (size_t)size, &pos_loff);
	if (ret >= 0) {
		*rsize = ret;
		ret = 0;
	}

	fput_ex(f);
err_open:
	iput(dentry.d_inode);
err_iget:
	kernfs_put(kn);
	return ret;
}

static int __liblinux_kernfs_write(unsigned long long pos, unsigned long ubuf,
				   unsigned long size, void *ctx,
				   unsigned long *wsize /* out */)
{
	struct dentry dentry;
	struct kernfs_node *kn;
	struct file *f = NULL;
	int ret = -EINVAL;
	loff_t pos_loff;

	mutex_lock(&g_liblinux_lock);
	kn = idr_find(&sysfs_idr, *(int *)ctx);
	if ((kn == NULL) || (size > SIZE_MAX) || (pos >= LLONG_MAX)) {
		mutex_unlock(&g_liblinux_lock);
		return ret;
	}
	kernfs_get(kn);
	mutex_unlock(&g_liblinux_lock);

	pos_loff = (loff_t)pos;
	memset(&dentry, 0, sizeof(struct dentry));
	dentry.d_fsdata = (void *)kn;
	dentry.d_inode = __liblinux_kernfs_iget(kn);
	if (dentry.d_inode == NULL) {
		goto err_iget;
	}

	/* not support !KERNFS_FILE open */
	BUG_ON(!(kernfs_type(kn) == KERNFS_FILE));
	ret = __liblinux_kernfs_open(&dentry, O_WRONLY, &f);
	if (ret < 0) {
		goto err_open;
	}

	ret = vfs_write_full(f, (const char __user *)(uintptr_t)ubuf, (size_t)size, &pos_loff);
	if (ret >= 0) {
		*wsize = ret;
		ret = 0;
	}

	fput_ex(f);
err_open:
	iput(dentry.d_inode);
err_iget:
	kernfs_put(kn);
	return ret;
}

static int __liblinux_kernfs_poll(int (*callback)(void *_ctx, int pollable), int id, int pollable)
{
	struct kernfs_node *kn = NULL;
	int ret;

	mutex_lock(&g_liblinux_lock);
	kn = (struct kernfs_node *)idr_find(&sysfs_idr, id);
	if (kn == NULL) {
		mutex_unlock(&g_liblinux_lock);
		return -EINVAL;
	}
	kernfs_get(kn);
	mutex_unlock(&g_liblinux_lock);

	ret = callback((void *)kn, pollable);
	kernfs_put(kn);
	return ret;
}

#ifdef CONFIG_LIBLINUX_ASYNC_SYSFS
static void liblinux_kernfs_flush(void)
{
	flush_sysfs_wq();
}
#endif

static struct liblinux_pal_kernfs_ops liblinux_kernfs_fops_ns = {
	.read = __liblinux_kernfs_read,
	.write = __liblinux_kernfs_write,
	.poll = __liblinux_kernfs_poll,
	.open = NULL,
	.close = NULL,
#ifdef CONFIG_LIBLINUX_ASYNC_SYSFS
	.flush = liblinux_kernfs_flush,
#else
	.flush = NULL,
#endif
};

static int liblinux_kernfs_read(unsigned long long pos, unsigned long ubuf,
				  unsigned long size, void *ctx,
				  unsigned long *rsize /* out */)
{
	int ret;
	loff_t pos_loff = 0;
	struct file *f = (struct file*)ctx;

	if (f == NULL || ubuf == NULL || (size > SIZE_MAX) || (pos > (unsigned long long)LLONG_MAX)) {
		return -EINVAL;
	}

	pos_loff = (loff_t)pos;
	ret = vfs_read(f, (char __user *)(uintptr_t)ubuf, (size_t)size, &pos_loff);
	if (ret >= 0) {
		*rsize = ret;
		ret = 0;
	}

	return ret;
}

static int liblinux_kernfs_write(unsigned long long pos, unsigned long ubuf,
				   unsigned long size, void *ctx,
				   unsigned long *wsize /* out */)
{
	int ret = -EINVAL;
	loff_t pos_loff;
	struct file *f = (struct file*)ctx;

	if (f == NULL) {
		return -EINVAL;
	}

	ret = vfs_write_full(f, (const char __user *)(uintptr_t)ubuf, (size_t)size, &pos_loff);
	if (ret >= 0) {
		*wsize = ret;
		ret = 0;
	}

	return ret;
}

static int liblinux_kernfs_poll(int (*callback)(void *_ctx, int pollable), int id, int pollable)
{
	void *kn = NULL;

	kn = idr_find(&sysfs_root->ino_idr, (u32)id);
	return callback(kn, pollable);
}

static char *liblinux_acquire_kernfs_path(struct kernfs_node *kn)
{
	char *ret = NULL;
	int error;

	ret = kzalloc(PATH_MAX, GFP_KERNEL | __GFP_NOFAIL);
	BUG_ON(!ret);

	error = kernfs_path(kn, ret, PATH_MAX);
	if (error < 0) {
		kfree(ret);
		ret = ERR_PTR(error);
		goto err;
	}

err:
	return ret;
}

static void liblinux_free_kernfs_path(char *path)
{
	kfree(path);
}

static void *liblinux_kernfs_open(void *ctx, unsigned int flags, unsigned int mode)
{
	struct kernfs_node *kn = NULL;
	struct file *filep = NULL;
	char fast_path[LDK_FAST_PATH_BUF_MAX] = { 0 };
	char *path = fast_path;
	int ret;

	kn = kernfs_find_and_get_node_by_id(sysfs_root, (unsigned long long)(uintptr_t)ctx);
	if (kn == NULL) {
		return NULL;
	}

	ret = kernfs_path(kn, fast_path, LDK_FAST_PATH_BUF_MAX);
	if (unlikely(ret < 0)) {
		kernfs_put(kn);
		return NULL;
	} else if (unlikely(ret >= LDK_FAST_PATH_BUF_MAX)) {
		/* retry, maybe it's a longer path */
		path = liblinux_acquire_kernfs_path(kn);
		if (IS_ERR(path)) {
			kernfs_put(kn);
			return NULL;
		}
	}
	kernfs_put(kn);

	if (unlikely(sysfs_mnt == NULL))
		wait_for_completion(&sysfs_mnt_done);

	filep = file_open_root_mnt(sysfs_mnt, path, flags, mode);
	if (IS_ERR(filep)) {
		filep = NULL;
	} else {
		struct inode *inode = filep->f_inode;
		d_mark_dontcache(inode);
	}

	if (unlikely(path != fast_path))
		liblinux_free_kernfs_path(path);

	return (void *)filep;
}

static void liblinux_kernfs_close(void * filep)
{
	fput((struct file *)filep);
}

static struct liblinux_pal_kernfs_ops liblinux_kernfs_fops = {
	.read = liblinux_kernfs_read,
	.write = liblinux_kernfs_write,
	.poll = liblinux_kernfs_poll,
	.open = liblinux_kernfs_open,
	.close = liblinux_kernfs_close,
#ifdef CONFIG_LIBLINUX_ASYNC_SYSFS
	.flush = liblinux_kernfs_flush,
#else
	.flush = NULL,
#endif
};

static int liblinux_create_symlink(const char *path,
				   struct kernfs_node *kn, unsigned int nsid)
{
	int ret;
	char *targetpath = NULL;
	struct kernfs_node *target = kn->symlink.target_kn;

	targetpath = kzalloc(PATH_MAX, GFP_KERNEL);
	if (targetpath == NULL) {
		return -ENOMEM;
	}

	ret = kernfs_get_target_path(kn->parent, target, targetpath);
	if (ret < 0) {
		goto err;
	}

	liblinux_sysfs_create_link(path, kn->mode, targetpath, nsid);
err:
	kfree(targetpath);
	return ret;
}

static int liblinux_create_file(const char *path, struct kernfs_node *kn)
{
	u32 id = (u32)kernfs_ino(kn);

	return liblinux_sysfs_create_file(path, kn->mode, kn->attr.size, NULL,
					  (void *)(uintptr_t)id, KERNFS_COMMON_NSID);
}

#define FAKE_PTR PAGE_OFFSET
/* The caller needs to hold the g_liblinux_lock */
int liblinux_sysfs_get_file_id(struct kernfs_node *kn, int *id)
{
	int kn_id;
	void *fake_ptr;
	int tmp_id;
	u32 ino = (u32)kernfs_ino(kn);

	tmp_id = idr_alloc_cyclic(&sysfs_idr, kn, liblinux_fs_tag_min, liblinux_fs_tag_max, GFP_NOWAIT);
	if (tmp_id < 0) {
		pr_err("liblinux sysfs idr alloc failed: %d\n", tmp_id);
		return tmp_id;
	}

	/* Construct a false pointer to avoid idr_find from falling into an infinite loop. */
	fake_ptr = (void *)((unsigned long)FAKE_PTR | (unsigned long)tmp_id);
	kn_id = idr_alloc_cyclic(&kernfs_idr, fake_ptr, ino, ino + 1, GFP_NOWAIT);
	if (kn_id < 0 || kn_id != ino) {
		pr_err("liblinux kernfs idr alloc failed, kn->id=%d, kn_id=%d\n", ino, kn_id);
		idr_remove(&sysfs_idr, tmp_id);
		return -ENOSPC;
	}

	*id = tmp_id;
	return 0;
}

static int liblinux_sysfs_create_file_ns(const char *path, struct kernfs_node *kn, unsigned int nsid)
{
	int ret, id;

	mutex_lock(&g_liblinux_lock);
	ret = liblinux_sysfs_get_file_id(kn, &id);
	if (ret != 0) {
		mutex_unlock(&g_liblinux_lock);
		return ret;
	}

	ret = liblinux_sysfs_create_file(path, kn->mode, kn->attr.size, NULL, (void *)(uintptr_t)id, nsid);
	if (ret < 0) {
		idr_remove(&sysfs_idr, id);
		idr_remove(&kernfs_idr, (u32)kernfs_ino(kn));
	}
	mutex_unlock(&g_liblinux_lock);

	return ret;
}

static int liblinux_sysfs_create_data_ns(const char *name, struct kernfs_node *kn, const void *ns)
{
	int ret = -EINVAL;
	unsigned int nsid = liblinux_get_nsid_by_ns(ns);

	switch (kernfs_type(kn)) {
		case KERNFS_DIR:
			ret = liblinux_sysfs_create_dir(name, kn->mode, nsid);
			break;
		case KERNFS_FILE:
			ret = liblinux_sysfs_create_file_ns(name, kn, nsid);
			break;
		case KERNFS_LINK:
			ret = liblinux_create_symlink(name, kn, nsid);
			break;
		default:
			break;
	}

	return ret;
}

const void *liblinux_get_ns_info_by_kn(struct kernfs_node *kn)
{
	struct kernfs_node *tmp = kn;

	while (tmp && tmp->ns == NULL) {
		if (tmp->parent == NULL)
			break;
		tmp = tmp->parent;
	}

	return tmp ? tmp->ns : NULL;
}

static bool devnode_need_create_batch(struct kernfs_node *kn)
{
	struct kernfs_node *tmp = kn;
	struct kobject *kobj = NULL;

	if (kernfs_type(kn) == KERNFS_LINK) {
		bool parent_flag, target_flag;
		struct kobject *parent = kn->parent->priv;
 
		parent_flag = parent ? (!!(parent->batch_create)) : false;
		kobj = tmp->symlink.target_kn->priv;
		target_flag = kobj ? (!!(kobj->batch_create)) : false;
		return (parent_flag || target_flag);
	}

	while (tmp) {
		if (tmp->ns != NULL) {
			kobj = tmp->priv;
			return kobj ? (!!(kobj->batch_create)) : false;
		}
		tmp = tmp->parent;
	}

	return false;
}

int liblinux_kernfs_create(struct kernfs_node *kn)
{
	int ret;
	char fast_path[LDK_FAST_PATH_BUF_MAX] = { 0 };
	char *buf = fast_path;
	char *path = NULL;
#ifdef CONFIG_NET_NS
	const void *ns = NULL;
#endif

	BUG_ON(kn == NULL);
	ret = kernfs_path(kn, buf, LDK_FAST_PATH_BUF_MAX);
	if (unlikely(ret < 0)) {
		return ret;
	} else if (unlikely(ret >= LDK_FAST_PATH_BUF_MAX)) {
		/* retry, maybe it's a longer path */
		buf = kzalloc(PATH_MAX, GFP_KERNEL);
		if (buf == NULL)
			return -ENOMEM;

		ret = kernfs_path(kn, buf, PATH_MAX);
		if (ret < 0) {
			kfree(buf);
			return ret;
		}
	}

	/* cann't handle path begin with '/' */
	path = buf + 1;

	ret = -EINVAL;
#ifdef CONFIG_NET_NS
	ns = liblinux_get_ns_info_by_kn(kn);
	if (ns != NULL) {
		if (is_need_create_in_batch(ns) || devnode_need_create_batch(kn)) {
			ret = 0;
			goto out;
		}
		ret = liblinux_sysfs_create_data_ns(path, kn, ns);
		if (ret != 0) {
			pr_warn("create sysfs failed, path: %s\n", path);
		}

		goto out;
	}
#endif
	switch (kernfs_type(kn)) {
		case KERNFS_DIR:
			ret = liblinux_sysfs_create_dir(path, kn->mode, KERNFS_COMMON_NSID);
			break;
		case KERNFS_FILE:
			ret = liblinux_create_file(path, kn);
			break;
		case KERNFS_LINK:
			ret = liblinux_create_symlink(path, kn, KERNFS_COMMON_NSID);
			break;
		default:
			break;
	}

	if (ret)
		pr_err("create path %s failed with %d\n", path, ret);

out:
	if (unlikely(buf != fast_path))
		kfree(buf);

	return ret;
}

unsigned int liblinux_get_nsid_by_ns(const struct net *ns)
{
	unsigned int nsid = KERNFS_COMMON_NSID;

	if (ns != NULL) {
		nsid = (unsigned int)atomic64_read(&ns->net_cookie);
		nsid &= NET_NSID_MASK;
	}

	return nsid;
}

int liblinux_kernfs_unlink(struct kernfs_node *kn)
{
	int ret, id;
	char *buf = NULL;
	char *path = NULL;
	void *ptr = NULL;
	const void *ns = NULL;
	unsigned int nsid;

	if (!kn) {
		return -EINVAL;
	}

	ns = liblinux_get_ns_info_by_kn(kn);
	if (ns) {
		mutex_lock(&g_liblinux_lock);

		ptr = idr_find(&kernfs_idr, (u32)kernfs_ino(kn));
		id = (int)((unsigned long)ptr & (~FAKE_PTR));
		if (id > 0) {
			idr_remove(&sysfs_idr, id);
			idr_remove(&kernfs_idr, (u32)kernfs_ino(kn));
		}

		mutex_unlock(&g_liblinux_lock);
	}

	buf = liblinux_acquire_kernfs_path(kn);
	if (IS_ERR(buf)) {
		return PTR_ERR(buf);
	}

	/* cann't handle path begin with '/' */
	path = buf + 1;

	nsid = liblinux_get_nsid_by_ns(ns);
	ret = liblinux_sysfs_delete(path, nsid);
	if (ret < 0)
		pr_debug("liblinux_sysfs_delete failed ret = %d\n", ret);
	liblinux_free_kernfs_path(buf);
	/* ignore hm sysfs unlink failed, continue to run kernfs_put */
	return 0;
}

int liblinux_kernfs_notify(struct kernfs_node *kn)
{
	int ret;
	char fast_path[LDK_FAST_PATH_BUF_MAX] = { 0 };
	char *buf = fast_path;
	char *path = NULL;

	BUG_ON(kn == NULL);
	ret = kernfs_path(kn, buf, LDK_FAST_PATH_BUF_MAX);
	if (unlikely(ret < 0)) {
		return ret;
	} else if (unlikely(ret >= LDK_FAST_PATH_BUF_MAX)) {
		/* retry, maybe it's a longer path */
		buf = kzalloc(PATH_MAX, GFP_KERNEL);
		if (buf == NULL)
			return -ENOMEM;
 
		ret = kernfs_path(kn, buf, PATH_MAX);
		if (ret < 0) {
			kfree(buf);
			return ret;
		}
	}

	/* cann't handle path begin with '/' */
	path = buf + 1;
	ret = liblinux_sysfs_notify(kn, path);
	if (ret)
		pr_err("notify path %s failed with %d\n", path, ret);
err:
	if (unlikely(buf != fast_path))
		kfree(buf);
	return ret;
}

int liblinux_kernfs_rename_ns(struct kernfs_node *kn,
			      struct kernfs_node *new_parent, const char *new_name, const void *new_ns)
{
	int ret;
	char *buf = NULL;
	char *old_path = NULL;
	char *new_path = NULL;
	const void *old_ns = NULL;
	unsigned int old_nsid, new_nsid;

	buf = kmalloc(PATH_MAX + PATH_MAX, GFP_KERNEL);
	if (buf == NULL)
		return -ENOMEM;

	ret = kernfs_path(new_parent, buf, PATH_MAX);
	if (ret < 0)
		goto err;

	/* concat new_parent with new_name */
	ret = snprintf(buf + ret, PATH_MAX - ret, "/%s", new_name);
	if (ret != (strlen(new_name) + 1))
		goto err;

	ret = kernfs_path(kn, buf + PATH_MAX, PATH_MAX);
	if (ret < 0)
		goto err;

	/* cann't handle path begin with '/' */
	new_path = buf + 1;
	old_path = buf + PATH_MAX + 1;
	old_ns = liblinux_get_ns_info_by_kn(kn);
	old_nsid = liblinux_get_nsid_by_ns(old_ns);
	new_nsid = liblinux_get_nsid_by_ns(new_ns);

	ret = liblinux_sysfs_rename_link(old_path, new_path, old_nsid, new_nsid);
	if (ret)
		pr_err("rename path %s -> %s failed with %d\n", old_path, new_path, ret);
err:
	if (buf != NULL)
		kfree(buf);
	return ret;
}

int liblinux_sysfs_init(struct kernfs_root *root)
{
	int err;

	sysfs_root = root;

	mutex_init(&g_liblinux_lock);

	err = liblinux_pal_require_frange(LIBLINUX_SYSFS, &liblinux_fs_tag_min, &liblinux_fs_tag_max);
	if (err < 0) {
		pr_warn("require frange failed ret=%d\n", err);
		return err;
	}
	err = liblinux_pal_sysfs_register_ops_ex_ns(&liblinux_kernfs_fops_ns);
	if (err < 0) {
		return err;
	}

	return liblinux_pal_sysfs_register_ops_ex(&liblinux_kernfs_fops);
}

static void hook_kernfs_add_one(void *__data, struct kernfs_node *kn,
				void (*remove)(struct kernfs_node *), int *ret)
{
	*ret = liblinux_kernfs_create(kn);
	if (*ret < 0) {
		/*
		 * kernfs_get() do count + 1. then it will not clean up
		 * kernfs_node in __kernfs_remove(), when kernfs create failed.
		 */
		kernfs_get(kn);
		remove(kn);
	}
}
INIT_VENDOR_HOOK(ldk_vh_kernfs_add_one, hook_kernfs_add_one);

static void hook_kernfs_put(void *__data, struct kernfs_node *kn, int *ret)
{
	*ret = liblinux_kernfs_unlink(kn);
}
INIT_VENDOR_HOOK(ldk_vh_kernfs_put, hook_kernfs_put);

static void hook_sysfs_init(void *__data, struct kernfs_root *root, int *ret)
{
	*ret = liblinux_sysfs_init(root);
	if (*ret < 0) {
		pr_err("liblinux: sysfs init failed\n");
	}
}
INIT_VENDOR_HOOK(ldk_vh_sysfs_init, hook_sysfs_init);

static void hook_kernfs_notify(void *__data, struct kernfs_node *kn)
{
	liblinux_kernfs_notify(kn);
}
INIT_VENDOR_HOOK(ldk_vh_kernfs_notify, hook_kernfs_notify);

static void hook_kernfs_rename_ns(void *__data, struct kernfs_node *kn,
				  struct kernfs_node *new_parent,
				  const char *new_name, const void *new_ns, int *ret)
{
	/* only rename parent */
	if (new_name == NULL) {
		new_name = kn->name;
		BUG_ON(new_parent != kn->parent);
	}

	*ret = liblinux_kernfs_rename_ns(kn, new_parent, new_name, new_ns);
	if (*ret < 0) {
		pr_err("liblinux: kernfs rename%s%s to %s failed with %d\n",
		       (kn->name == new_name) ? "parent path of " : " ",
		       kn->name, new_name, *ret);
	}
}
INIT_VENDOR_HOOK(ldk_rvh_kernfs_rename_ns, hook_kernfs_rename_ns);

static int liblinux_sysfs_mnt_init(void)
{
	struct file_system_type *sys_fs_type;

	sys_fs_type = get_fs_type("sysfs");
	if (!sys_fs_type) {
		pr_err("Failed to find sysfs\n");
		return -EINVAL;
	}
	sysfs_mnt = kern_mount(sys_fs_type);
	put_filesystem(sys_fs_type);
	if (IS_ERR(sysfs_mnt)) {
		panic("Failed to mount sysfs\n");
		return -EINVAL;
	}

	complete_all(&sysfs_mnt_done);
	return 0;
}
late_initcall(liblinux_sysfs_mnt_init);
