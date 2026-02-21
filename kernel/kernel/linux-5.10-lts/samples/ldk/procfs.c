// SPDX-License-Identifier: GPL-2.0
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/mount.h>
#include <linux/module.h>
#include <linux/file.h>
#include <linux/pid_namespace.h>
#include <uapi/linux/mount.h>
#include <linux/init_syscalls.h>
#include <linux/proc_fs.h>
#include <securec.h>

#include "../../fs/proc/internal.h"
#include "file.h"
#include "netns/net_procfs.h"
#include "procfs.h"
#include "kernfs.h"

#include <trace/hooks/liblinux.h>

static int liblinux_pal_procfs_create_file_bypath_ex(const char *name,
				    struct liblinux_pal_kernfs_ops *ops,
				    void *ctx, unsigned int mode)
{
	return liblinux_pal_procfs_create_file_bypath(name, mode);
}

static DEFINE_MUTEX(path_lock);
static struct liblinux_procfs_create_ops liblinux_proc_create_ops_bypath = {
	.create_file = liblinux_pal_procfs_create_file_bypath_ex,
	.create_dir = liblinux_pal_procfs_create_dir,
};

/* sync from fs/procfs/liblinux.c */
struct file *alloc_file(const struct path *path, int flags,
		const struct file_operations *fop);

static struct vfsmount *proc_mnt = NULL;
static DEFINE_RWLOCK(proc_mnt_rwlock);
static DECLARE_COMPLETION(procfs_mnt_done);

static struct vfsmount *_proc_mnt_get(void)
{
	struct vfsmount *proc_mnt_ret = NULL;

	read_lock(&proc_mnt_rwlock);
	proc_mnt_ret = proc_mnt;
	read_unlock(&proc_mnt_rwlock);
	return proc_mnt_ret;
}

struct vfsmount *proc_mnt_get_locked(void)
{
	if (_proc_mnt_get() == NULL) {
		wait_for_completion(&procfs_mnt_done);
	}

	mutex_lock(&path_lock);
	return _proc_mnt_get();
}

void proc_mnt_unlock(void)
{
	mutex_unlock(&path_lock);
}

static int __liblinux_procfs_open(struct path *path, int flags, struct file **file)
{
	struct proc_dir_entry *dp = NULL;
	struct file *f = NULL;
	int ret;
	struct inode *inode = path->dentry->d_inode;

	dp = PROC_I(inode)->pde;
	if ((dp == NULL) || (dp->proc_ops == NULL)) {
		return -EINVAL;
	}

	if (fops_get(inode->i_fop) == NULL) {
		return -EINVAL;
	}

	f = alloc_file(path, flags, inode->i_fop);
	if (IS_ERR(f)) {
		fops_put(inode->i_fop);
		return PTR_ERR(f);
	}

	if (f->f_op->open != NULL) {
		ret = f->f_op->open(inode, f);
		if (ret < 0) {
			put_filp(f);
			return ret;
		}
	}

	/* the dentry does not work, disable notify for procfs */
	f->f_mode |= FMODE_NONOTIFY;
	*file = f;
	return 0;
}

static struct file *g_sensorhub_filp = NULL;

int liblinux_procfs_read(unsigned long long pos, unsigned long ubuf,
				  unsigned long size, void *ctx,
				  unsigned long *rsize /* out */)
{
	struct inode *inode = NULL;
	struct file *filep = NULL;
	int ret;
	loff_t pos_loff;
	struct proc_dir_entry *dp = NULL;
	struct dentry dentry;
	struct path path = {
		.mnt = NULL,
		.dentry = &dentry,
	};

	if ((ctx == NULL) || (pos >= LLONG_MAX)) {
		return -EINVAL;
	}

	pos_loff = (loff_t)pos;
	inode = (struct inode *)ctx;
	path.dentry->d_inode = inode;
	ret = __liblinux_procfs_open(&path, O_RDONLY, &filep);
	if (ret < 0)
		return ret;

	ret = vfs_read_full(filep, (char __user *)ubuf, size, &pos_loff);
	filep->f_path.dentry = NULL;
	__fput_sync(filep);
	if (ret >= 0) {
		*rsize = ret;
		ret = 0;
	}

	return ret;
}

typedef ssize_t (*vfs_rw_f)(struct file *file, char __user *buf, size_t count, loff_t *pos);
static int liblinux_procfs_rw_bypath(unsigned long long pos, unsigned long ubuf,
				  unsigned long size, void *filep,
				  unsigned long *rwsize /* out */, bool is_read)
{
	int ret;
	loff_t pos_loff = pos;
	vfs_rw_f rw_f = is_read ? vfs_read : vfs_write;

	if ((filep == NULL) || (pos >= LLONG_MAX)) {
		return -EINVAL;
	}

	ret = rw_f(filep, (char __user *)ubuf, size, &pos_loff);
	if (ret >= 0) {
		*rwsize = ret;
		ret = 0;
	}

	return ret;
}

static int liblinux_procfs_read_bypath(unsigned long long pos, unsigned long ubuf,
				  unsigned long size, void *filep,
				  unsigned long *rsize /* out */)
{
	return liblinux_procfs_rw_bypath(pos, ubuf, size, filep, rsize, true);
}

static int liblinux_procfs_write_bypath(unsigned long long pos, unsigned long ubuf,
				  unsigned long size, void *filep,
				  unsigned long *wsize /* out */)
{
	return liblinux_procfs_rw_bypath(pos, ubuf, size, filep, wsize, false);
}

int liblinux_procfs_write(unsigned long long pos, unsigned long ubuf,
				   unsigned long size, void *ctx,
				   unsigned long *wsize /* out */)
{
	struct inode *inode = NULL;
	struct file *filep = NULL;
	int ret;
	loff_t pos_loff;
	struct dentry dentry;
	struct path path = {
		.mnt = NULL,
		.dentry = &dentry,
	};

	if ((ctx == NULL) || (pos >= LLONG_MAX)) {
		return -EINVAL;
	}

	pos_loff = (loff_t)pos;
	inode = (struct inode *)ctx;
	path.dentry->d_inode = inode;
	ret = __liblinux_procfs_open(&path, O_WRONLY, &filep);
	if (ret < 0) {
		return ret;
	}

	ret = vfs_write_full(filep, (const char __user *)ubuf, size, &pos_loff);
	if (ret >= 0) {
		*wsize = ret;
		ret = 0;
	}

	filep->f_path.dentry = NULL;
	__fput_sync(filep);
	return ret;
}

static char *__liblinux_acquire_procfs_path(struct proc_dir_entry *dp,
					    char *buf, size_t buflen, struct proc_dir_entry **ret_dp)
{
	char *p = &buf[buflen - 1];

	if (dp == NULL) {
		return NULL;
	}

	*--p = '\0';

	do {
		if (p - buf < dp->namelen + 1) {
			buf[0] = '\0';
			p = NULL;
			break;
		}

		p -= dp->namelen;
		memcpy(p, dp->name, dp->namelen);
		*--p = '/';
		dp = dp->parent;
	} while ((dp != NULL) && (dp != dp->parent));
	*ret_dp = dp;

	return p;
}

static void *liblinux_procfs_open_bypath(void *pathname, unsigned int flags, unsigned int mode)
{
	struct file *filep = NULL;
	int ret;

	if (pathname == NULL) {
		return NULL;
	}

	filep = file_open_root_mnt(proc_mnt, pathname, flags, mode);
	if (IS_ERR(filep)) {
		filep = NULL;
	}

	return (void *)filep;
}

static void liblinux_procfs_close_bypath(void * filep)
{
	fput((struct file *)filep);
}

static struct liblinux_pal_kernfs_ops liblinux_proc_ops_bypath = {
	.read = liblinux_procfs_read_bypath,
	.write = liblinux_procfs_write_bypath,
	.open = liblinux_procfs_open_bypath,
	.close = liblinux_procfs_close_bypath,
};
static int liblinux_procfs_mnt_init(void)
{
	struct file_system_type *proc_fs_type;
	void *ptr_ret = NULL;
	bool need_unmount = false;

	proc_fs_type = get_fs_type("proc");
	if (!proc_fs_type) {
		panic("Failed to find procfs to mount\n");
		return -EINVAL;
	}

	ptr_ret = kern_mount(proc_fs_type);
	put_filesystem(proc_fs_type);
	if (IS_ERR_OR_NULL(ptr_ret)) {
		pr_warn("Failed to mount procfs, err:%d\n", PTR_ERR(ptr_ret));
		return PTR_ERR(ptr_ret);
	}

	/* When two threads enter here at the same time, only the first one proc_mnt is retained */
	write_lock(&proc_mnt_rwlock);
	if (likely(proc_mnt == NULL)) {
		proc_mnt = ptr_ret;
	} else {
		need_unmount = true;
	}
	write_unlock(&proc_mnt_rwlock);

	if (unlikely(need_unmount)) {
		kern_unmount(ptr_ret);
	}

	complete_all(&procfs_mnt_done);
	return 0;
}

void liblinux_proc_register(struct proc_dir_entry * dp)
{
	char fast_path[LDK_FAST_PATH_BUF_MAX] = { 0 };
	char *buf = fast_path;
	size_t buflen = LDK_FAST_PATH_BUF_MAX;
	char *pathname = NULL;
	char *linkname = dp->data;
	int ret;
	struct proc_dir_entry *tmp_dp = NULL;
	struct liblinux_procfs_create_ops *create_ops = &liblinux_proc_create_ops_bypath;
	struct liblinux_pal_kernfs_ops *ops = &liblinux_proc_ops_bypath;

	if (unlikely(_proc_mnt_get() == NULL) && liblinux_procfs_mnt_init() < 0) {
		return;
	}

	if(unlikely(dp->namelen + 1 > LDK_FAST_PATH_BUF_MAX - 2)){
		buf = kzalloc(PATH_MAX, GFP_KERNEL);
		if (buf == NULL) {
			pr_err("liblinux: proc kzalloc buf failed\n");
			return;
		}
		buflen = PATH_MAX;
	}
	pathname = __liblinux_acquire_procfs_path(dp, buf, buflen, &tmp_dp);
	if (pathname == NULL) {
		ret = -EINVAL;
		goto out;
	}

#ifdef CONFIG_NET_NS
	if (tmp_dp->parent != &proc_root) {
		bool need_create = true;
		trace_ldk_vh_liblinux_proc_create_net_data(tmp_dp, &create_ops, &ops, &need_create);
		if (!need_create) {
			goto out;
		}
	}
#endif

	/* skip head '/' */
	pathname++;
	if (S_ISREG(dp->mode)) {
		struct inode *inode = NULL;
		/*
		 * 1. Non-net procfs file will create just by pathname, then use pathname to Read/Write file.
		 * 2. Net procfs inode will be freed when the file is deleted in procfs_entry_destroy_specific.
		 * */
		if (tmp_dp->parent == &proc_root) {
			ret = create_ops->create_file(pathname, ops,
					NULL, (unsigned int)dp->mode);
			goto out;
		}

		pde_get(dp);
		inode = proc_get_inode(proc_mnt->mnt_sb, dp);
		if (inode == NULL) {
			ret = -ENOMEM;
		} else {
			ret = create_ops->create_file(pathname, ops,
					(void *)inode, (unsigned int)dp->mode);
			if (ret < 0) {
				iput(inode);
			}
		}
	} else if (S_ISDIR(dp->mode)) {
		ret = create_ops->create_dir(pathname, (unsigned int)dp->mode);
	} else if (S_ISLNK(dp->mode)) {
		if (linkname == NULL) {
			ret = -EINVAL;
		} else {
			ret = liblinux_pal_procfs_create_link(pathname, (unsigned int)dp->mode, linkname);
		}
	} else {
		pr_warn("liblinux: unsupported proc_dir_entry '%s/%s', mode=0x%x\n",
			dp->parent->name, dp->name, dp->mode);
		ret = 0;
	}

out:
	if (ret < 0) {
		pr_err("liblinux: create proc entry '%s/%s' failed, mode=0x%x, ret=%d\n",
			 dp->parent->name, dp->name, dp->mode, ret);
	}
	if(unlikely(buf != fast_path))
		kfree(buf);
}

void liblinux_procfs_unlink(struct proc_dir_entry *dp)
{
	char fast_path[LDK_FAST_PATH_BUF_MAX] = { 0 };
	char *buf = fast_path;
	size_t buflen = LDK_FAST_PATH_BUF_MAX;
	char *pathname = NULL;
	struct proc_dir_entry *tmp_dp = NULL;

	BUG_ON(dp == NULL);
	if(unlikely(dp->namelen + 1 > LDK_FAST_PATH_BUF_MAX - 2)){
		buf = kzalloc(PATH_MAX, GFP_KERNEL);
		if (buf == NULL) {
			pr_err("liblinux: proc kzalloc buf failed\n");
			return;
		}
		buflen = PATH_MAX;
	}

	pathname = __liblinux_acquire_procfs_path(dp, buf, buflen, &tmp_dp);
	if (pathname == NULL) {
		pr_err("liblinux: invalid proc entry '%s/%s', mode=0x%x\n",
		dp->parent->name, dp->name, dp->mode);
		goto out;
	}

#ifdef CONFIG_NET_NS
	if (tmp_dp->parent != &proc_root) {
		goto out;
	}
#endif

	/* skip head '/' */
	pathname++;

	(void)liblinux_pal_procfs_delete(pathname);
out:
	if(unlikely(buf != fast_path))
		kfree(buf);
}

/* vendor hook for procfs */
static void hook_proc_register(void *__data, int action, struct proc_dir_entry *dp)
{
	switch(action) {
	case VH_REG_PROCFS_PREPARE:
		pde_get(dp);
		break;
	case VH_REG_PROCFS:
		mutex_lock(&path_lock);
		liblinux_proc_register(dp);
		mutex_unlock(&path_lock);
		pde_put(dp);
		break;
	}
}
INIT_VENDOR_HOOK(ldk_vh_proc_register, hook_proc_register);

static void hook_remove_proc_entry(void *__data, struct proc_dir_entry *de)
{
	if (de) {
		liblinux_procfs_unlink(de);
	}
}
INIT_VENDOR_HOOK(ldk_vh_remove_proc_entry, hook_remove_proc_entry);

static void hook_remove_proc_subtree(void *__data, struct proc_dir_entry *de)
{
	liblinux_procfs_unlink(de);
}
INIT_VENDOR_HOOK(ldk_vh_remove_proc_subtree, hook_remove_proc_subtree);

static void hook_proc_root_init(void *__data, struct file_system_type *type)
{
	int ret = 0;

	ret = liblinux_pal_procfs_register_ops(&liblinux_proc_ops_bypath);
	if (ret) {
		pr_err("Failed to register procfs ops_bypath, err:%d\n", ret);
	}
	/* ensure that proc_register in proc_root_init succeed */
	register_filesystem(type);
}
INIT_VENDOR_HOOK(ldk_vh_proc_root_init, hook_proc_root_init);

static void liblinux_delete_devinet_sysctl_procfs(char *path, const char *dev_name, unsigned int nsid)
{
	char *delete_path = NULL;

	delete_path = kmalloc(PATH_MAX, GFP_KERNEL);
	if (delete_path == NULL) {
		pr_err("liblinux: proc kmalloc path failed\n");
		return;
	}

	(void)memset_s(delete_path, PATH_MAX, 0, PATH_MAX);
	(void)snprintf_s(delete_path, PATH_MAX, PATH_MAX - 1, "%s%s", path, dev_name);

#ifdef CONFIG_NET_NS
	(void)procfs_delete_net_data_specific(delete_path, nsid);
#endif
	kfree(delete_path);
}

static void hook_devinet_sysctl_unregister(void *__data, struct net *net, char *dev_name)
{
	unsigned int nsid = liblinux_get_nsid_by_ns(net);

	liblinux_delete_devinet_sysctl_procfs("sys/net/ipv4/conf/", dev_name, nsid);
	liblinux_delete_devinet_sysctl_procfs("sys/net/ipv4/neigh/", dev_name, nsid);
}
INIT_VENDOR_HOOK(ldk_vh_devinet_sysctl_unregister, hook_devinet_sysctl_unregister);

static void hook_addrconf_sysctl_unregister(void *__data, struct net *net, char *dev_name)
{
	unsigned int nsid = liblinux_get_nsid_by_ns(net);

	liblinux_delete_devinet_sysctl_procfs("sys/net/ipv6/conf/", dev_name, nsid);
	liblinux_delete_devinet_sysctl_procfs("sys/net/ipv6/neigh/", dev_name, nsid);
	liblinux_delete_devinet_sysctl_procfs("net/dev_snmp6/", dev_name, nsid);
}
INIT_VENDOR_HOOK(ldk_vh_addrconf_sysctl_unregister, hook_addrconf_sysctl_unregister);

static void hook_remove_proc_entry_with_ns(void *__data, struct net *net, char *pre_path,
					   const char *dev_name)
{
	unsigned int nsid = liblinux_get_nsid_by_ns(net);

	liblinux_delete_devinet_sysctl_procfs(pre_path, dev_name, nsid);
}
INIT_VENDOR_HOOK(ldk_vh_remove_proc_entry_with_ns, hook_remove_proc_entry_with_ns);

#ifdef CONFIG_LIBLINUX_NET_STACK
static void hook_proc_net_init(void *__data, struct pernet_operations *ops)
{
	proc_mkdir("net", NULL);
	proc_symlink("net/unix", NULL, "/proc/hmnet/unix");
}
INIT_VENDOR_HOOK(ldk_vh_proc_net_init, hook_proc_net_init);
#endif
