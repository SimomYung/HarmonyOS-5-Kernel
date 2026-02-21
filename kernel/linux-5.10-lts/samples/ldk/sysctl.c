// SPDX-License-Identifier: GPL-2.0-only
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/mutex.h>
#include <linux/mount.h>
#include <linux/slab.h>
#include <linux/sysctl.h>
#include "file.h"
#include "kernfs.h"
#include "procfs.h"
#include "sysctl.h"
#include "netns/net_procfs.h"
#include <trace/hooks/liblinux.h>

static long __sysctl_data;
#define LIBLINUX_UNIMPL_SYSCTL_DATA(type, name) \
	extern type name __attribute__((alias("__sysctl_data")))

LIBLINUX_UNIMPL_SYSCTL_DATA(int, aio_max_nr);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, aio_nr);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, C_A_D);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, dac_mmap_min_addr);
LIBLINUX_UNIMPL_SYSCTL_DATA(char, devkmsg_log_str[1]);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, dir_notify_enable);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, dirty_background_bytes);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, dirty_background_ratio);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, dirty_expire_interval);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, dirtytime_expire_interval);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, dmesg_restrict);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, fs_overflowuid);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, fs_overflowgid);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, lease_break_time);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, leases_enable);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, mmap_rnd_bits);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, mmap_rnd_compat_bits);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, page_cluster);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, panic_on_warn);
LIBLINUX_UNIMPL_SYSCTL_DATA(unsigned long, panic_print);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, panic_timeout);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, pipe_max_size);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, pipe_user_pages_hard);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, pipe_user_pages_soft);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, poweroff_cmd);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, print_fatal_signals);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, randomize_va_space);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, show_unhandled_signals);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_admin_reserve_kbytes);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_drop_caches);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_fault_around_bytes);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_hung_task_all_cpu_backtrace);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_hung_task_check_count);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_hung_task_check_interval_secs);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_hung_task_panic);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_hung_task_warnings);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_max_map_count);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_memory_failure_early_kill);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_memory_failure_recovery);
LIBLINUX_UNIMPL_SYSCTL_DATA(unsigned int, sysctl_nr_open);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_oom_dump_tasks);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_oom_kill_allocating_task);
LIBLINUX_UNIMPL_SYSCTL_DATA(unsigned int, sysctl_oops_all_cpu_backtrace);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_overcommit_kbytes);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_overcommit_memory);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_overcommit_ratio);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_page_lock_unfairness);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_panic_on_oom);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_sched_child_runs_first);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_sched_dl_period_max);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_sched_dl_period_min);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_sched_latency);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_sched_migration_cost);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_sched_min_granularity);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_sched_nr_migrate);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_sched_rr_timeslice);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_sched_rt_period);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_sched_rt_runtime);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_sched_tunable_scaling);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_sched_wakeup_granularity);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_shrink_memory);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, sysctl_user_reserve_kbytes);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, vm_dirty_bytes);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, vm_dirty_ratio);
LIBLINUX_UNIMPL_SYSCTL_DATA(int, vm_swappiness);
LIBLINUX_UNIMPL_SYSCTL_DATA(unsigned int, sysctl_sched_uclamp_util_min);
LIBLINUX_UNIMPL_SYSCTL_DATA(unsigned int, sysctl_sched_uclamp_util_max);
LIBLINUX_UNIMPL_SYSCTL_DATA(unsigned int, sysctl_sched_uclamp_util_min_rt_default);
LIBLINUX_UNIMPL_SYSCTL_DATA(unsigned int, sysctl_sched_energy_aware);

static long __sysctl_extra;
#define LIBLINUX_UNIMPL_SYSCTL_EXTRA(type, name) \
	extern type name __attribute__((alias("__sysctl_extra")))

LIBLINUX_UNIMPL_SYSCTL_EXTRA(int, mmap_rnd_bits_max);
LIBLINUX_UNIMPL_SYSCTL_EXTRA(int, mmap_rnd_bits_min);
LIBLINUX_UNIMPL_SYSCTL_EXTRA(int, mmap_rnd_compat_bits_max);
LIBLINUX_UNIMPL_SYSCTL_EXTRA(int, mmap_rnd_compat_bits_min);
LIBLINUX_UNIMPL_SYSCTL_EXTRA(int, pid_max_max);
LIBLINUX_UNIMPL_SYSCTL_EXTRA(int, pid_max_min);
LIBLINUX_UNIMPL_SYSCTL_EXTRA(unsigned int, sysctl_nr_open_max);
LIBLINUX_UNIMPL_SYSCTL_EXTRA(unsigned int, sysctl_nr_open_min);

static int __sysctl_handler(struct ctl_table *table, int write,
			  void *buffer, size_t *length, loff_t *ppos)
{
	return -ENOSYS;
}

#define LIBLINUX_UNIMPL_SYSCTL_HANDLER(name) \
	extern int name(struct ctl_table *table, int write, \
		void *buffer, size_t *length, loff_t *ppos) __attribute__((alias("__sysctl_handler")))

LIBLINUX_UNIMPL_SYSCTL_HANDLER(devkmsg_sysctl_set_loglvl);
LIBLINUX_UNIMPL_SYSCTL_HANDLER(drop_caches_sysctl_handler);
LIBLINUX_UNIMPL_SYSCTL_HANDLER(fault_around_bytes_handler);
LIBLINUX_UNIMPL_SYSCTL_HANDLER(mmap_min_addr_handler);
LIBLINUX_UNIMPL_SYSCTL_HANDLER(overcommit_kbytes_handler);
LIBLINUX_UNIMPL_SYSCTL_HANDLER(overcommit_policy_handler);
LIBLINUX_UNIMPL_SYSCTL_HANDLER(overcommit_ratio_handler);
LIBLINUX_UNIMPL_SYSCTL_HANDLER(sched_proc_update_handler);
LIBLINUX_UNIMPL_SYSCTL_HANDLER(sched_rr_handler);
LIBLINUX_UNIMPL_SYSCTL_HANDLER(sched_rt_handler);
LIBLINUX_UNIMPL_SYSCTL_HANDLER(sysctl_max_threads);
LIBLINUX_UNIMPL_SYSCTL_HANDLER(dirty_background_bytes_handler);
LIBLINUX_UNIMPL_SYSCTL_HANDLER(dirty_background_ratio_handler);
LIBLINUX_UNIMPL_SYSCTL_HANDLER(dirty_bytes_handler);
LIBLINUX_UNIMPL_SYSCTL_HANDLER(dirty_ratio_handler);
LIBLINUX_UNIMPL_SYSCTL_HANDLER(dirtytime_interval_handler);
LIBLINUX_UNIMPL_SYSCTL_HANDLER(dirty_writeback_centisecs_handler);
LIBLINUX_UNIMPL_SYSCTL_HANDLER(sysctl_sched_uclamp_handler);
LIBLINUX_UNIMPL_SYSCTL_HANDLER(sched_energy_aware_handler);
LIBLINUX_UNIMPL_SYSCTL_HANDLER(sysctl_shrinkmem_handler);
LIBLINUX_UNIMPL_SYSCTL_HANDLER(sysctl_schedstats);
LIBLINUX_UNIMPL_SYSCTL_HANDLER(proc_dohung_task_timeout_secs);

struct ctl_table epoll_table[] = { {}, };
struct ctl_table firmware_config_table[] = { {}, };
struct ctl_table inotify_table[] = { {}, };
struct ctl_table random_table[] = { {}, };
struct ctl_table usermodehelper_table[] = { {}, };

#define PROC_SYS_PREFIX "sys"

static struct vfsmount *proc_sys_mnt = NULL;
static DEFINE_MUTEX(path_lock);

struct liblinux_proc_sys_entry {
	char *name;
	int namelen;
	struct liblinux_proc_sys_entry *parent;
};

static struct liblinux_procfs_create_ops liblinux_proc_sys_create_ops_common = {
	.create_file = liblinux_pal_procfs_create_file,
	.create_dir = liblinux_pal_procfs_create_dir,
};
static struct liblinux_pal_kernfs_ops liblinux_proc_sys_ops_common = {
	.read = liblinux_proc_sys_read,
	.write = liblinux_proc_sys_write,
};

static int
__liblinux_proc_register(const char *path, mode_t mode,
			 struct liblinux_pal_kernfs_ops *ops, void *data,
			 struct liblinux_procfs_create_ops *create_ops)
{
	int ret;
	void *ptr_ret;

	if (!proc_sys_mnt) {
		struct file_system_type *proc_fs_type;
		proc_fs_type = get_fs_type("proc");
		if (!proc_fs_type) {
			pr_err("Failed to find procfs\n");
			return -EINVAL;
		}

		ptr_ret = kern_mount(proc_fs_type);
		put_filesystem(proc_fs_type);
		if (IS_ERR(ptr_ret)) {
			pr_err("Failed to mount procfs\n");
			return (int)PTR_ERR(ptr_ret);
		}
		proc_sys_mnt = ptr_ret;
	}

	if (S_ISREG(mode)) {
		ret = create_ops->create_file(path, ops, data, (unsigned int)mode);
	} else if (S_ISDIR(mode)) {
		ret = create_ops->create_dir(path, (unsigned int)mode);
	} else {
		pr_warn("liblinux: unsupported proc sys entry '%s', mode=0x%x\n", path, mode);
		ret = 0;
	}

	if ((ret < 0) && (ret != -EEXIST)) {
		pr_err("liblinux: create proc ct '%s' failed, mode=0x%x, ret=%d\n",
		       path, mode, ret);
	}

	return ret;
}

static char *
__liblinux_mk_proc_sys_path(struct ctl_dir *dir, const char *name,
			    int nlen, char *proc_sys_path, int plen)
{
	char *p = &proc_sys_path[plen - 1];
	int prefix_len = strlen(PROC_SYS_PREFIX);
	int len = 0;

	if (name == NULL || (nlen + prefix_len) >= plen - 1) {
		return NULL;
	}

	*--p = '\0';

	/* proc sys table */
	p -= nlen;
	memcpy(p, name, nlen);
	*--p = '/';

	/* proc sys dir */
	do {
		const char *procname = dir->header.ctl_table[0].procname;
		if (procname == NULL || strlen(procname) == 0) {
			break;
		}

		len = strlen(procname);
		if (p - proc_sys_path < len + prefix_len + 1) {
			proc_sys_path[0] = '\0';
			pr_err("proc sys path is too long: %s/.../%s/%s\n",
			       PROC_SYS_PREFIX, procname, p);
			p = NULL;
			break;
		}

		p -= len;
		memcpy(p, procname, len);
		*--p = '/';
		dir = dir->header.parent;
	} while (dir != NULL);

	/* proc sys prefix */
	if (p != NULL) {
		p -= prefix_len;
		memcpy(p, PROC_SYS_PREFIX, prefix_len);
	}

	return p;
}

static char *
__liblinux_acquire_proc_sys_path(struct rb_node *node, char *buf, int len)
{
	struct ctl_node *cnode = NULL;
	struct ctl_table *ct = NULL;
	struct ctl_table_header *head = NULL;

	cnode = rb_entry(node, struct ctl_node, node);
	head = cnode->header;
	ct = &head->ctl_table[cnode - (head)->node];

	return __liblinux_mk_proc_sys_path(head->parent, ct->procname,
					   strlen(ct->procname), buf, len);
}

int liblinux_proc_sys_read(unsigned long long pos, unsigned long ubuf,
			 unsigned long size, void *ctx,
			 unsigned long *rsize /* out */)
{
	char *buf = NULL;
	char *pathname = NULL;
	struct file *filep = NULL;
	int ret;
	loff_t pos_loff;

	if ((ctx == NULL) || (pos >= LLONG_MAX)) {
		return -EINVAL;
	}

	pos_loff = (loff_t)pos;
	buf = kmalloc(PATH_MAX, GFP_KERNEL);
	if (buf == NULL) {
		return -ENOMEM;
	}

	pathname = __liblinux_acquire_proc_sys_path((struct rb_node *)ctx, buf, PATH_MAX);
	if (pathname == NULL) {
		ret = -ENAMETOOLONG;
		goto err;
	}

	filep = file_open_root_mnt(proc_sys_mnt, pathname, O_RDONLY, 0);
	if (IS_ERR(filep)) {
		ret = (int)PTR_ERR(filep);
		goto err;
	}

	ret = vfs_read_full(filep, (char __user *)ubuf, size, &pos_loff);
	if (ret >= 0) {
		*rsize = ret;
		ret = 0;
	}

	filp_close(filep, NULL);
err:
	kfree(buf);
	return ret;
}

int liblinux_proc_sys_write(unsigned long long pos, unsigned long ubuf,
			  unsigned long size, void *ctx,
			  unsigned long *wsize /* out */)
{
	char *buf = NULL;
	char *pathname = NULL;
	struct file *filep = NULL;
	int ret;
	loff_t pos_loff;

	if ((ctx == NULL) || (pos >= LLONG_MAX)) {
		return -EINVAL;
	}

	pos_loff = (loff_t)pos;
	buf = kmalloc(PATH_MAX, GFP_KERNEL);
	if (buf == NULL) {
		return -ENOMEM;
	}

	pathname = __liblinux_acquire_proc_sys_path((struct rb_node *)ctx, buf, PATH_MAX);
	if (pathname == NULL) {
		ret = -ENAMETOOLONG;
		goto err;
	}

	filep = file_open_root_mnt(proc_sys_mnt, pathname, O_WRONLY, 0);
	if (IS_ERR(filep)) {
		ret = (int)PTR_ERR(filep);
		goto err;
	}

	ret = vfs_write_full(filep, (const char __user *)ubuf, size, &pos_loff);
	if (ret >= 0) {
		*wsize = ret;
		ret = 0;
	}

	filp_close(filep, NULL);
err:
	kfree(buf);
	return ret;
}

static int
__procfs_sys_mk_path(struct liblinux_proc_sys_entry *entry, const char *name, int pos)
{
	int ret, len;

	if (name == NULL) {
		return 0;
	}

	len = strlen(name);
	if ((pos + len) >= PATH_MAX) {
		return -ENAMETOOLONG;
	}

	ret = snprintf((entry->name + pos), (PATH_MAX - pos), "/%s", name);
	if (ret > (len + 1) || ret < 0) {
		return -EINVAL;
	}
	/* hm-fs handle the end of path is not '/' only */
	entry->namelen = pos + ((name[len - 1] == '/') ? (ret - 1) : ret);
	entry->name[entry->namelen] = '\0';

	return 0;
}

static int
__procfs_sys_register(struct liblinux_proc_sys_entry *entry,
		      struct ctl_table_header *head,
		      struct ctl_table *ct,
			  struct liblinux_pal_kernfs_ops *ops,
			  struct liblinux_procfs_create_ops *create_ops)
{
	int ret;
	int dirlen = 0;
	struct rb_node *node = NULL;
	const char *name = NULL;
	struct ctl_table *table = NULL;
	if (entry == NULL || ct == NULL) {
		return -EINVAL;
	}

	table = ct;
	dirlen = entry->namelen;
	for (; (name = table->procname) != NULL; table++) {
		ret = __procfs_sys_mk_path(entry, name, dirlen);
		if (ret < 0) {
			return ret;
		}

		node = &head->node[table - head->ctl_table].node;
		if (table->child != NULL) {
			ret = __liblinux_proc_register(entry->name, (umode_t)S_IFDIR,
						       NULL, NULL, create_ops);
			if ((ret < 0) && (ret != -EEXIST)) {
				return ret;
			}
			ret = __procfs_sys_register(entry, head, table->child, ops, create_ops);
		} else {
			ret = __liblinux_proc_register(entry->name, (umode_t)S_IFREG | table->mode,
						       ops, (void *)node, create_ops);
		}
	}

	return ret;
}

static void hook_register_sysctl_table(void *__data, const char *dir,
				       struct ctl_table_header *head, struct ctl_table *table)
{
	static char proc_sys_path[PATH_MAX] = PROC_SYS_PREFIX;
	struct liblinux_proc_sys_entry entry = {
		.name = proc_sys_path,
		.namelen = strlen(PROC_SYS_PREFIX),
		.parent = NULL,
	};
	const char *name, *nextname;
	bool need_create = false;
	int ret;
	struct liblinux_procfs_create_ops *create_ops = &liblinux_proc_sys_create_ops_common;
	struct liblinux_pal_kernfs_ops *ops = &liblinux_proc_sys_ops_common;

	if ((table == NULL) || (head == NULL)) {
		return;
	}

	mutex_lock(&path_lock);
	if (strncmp(dir, "net", strlen("net")) == 0) {
		need_create = true;
#ifdef CONFIG_NET_NS
		trace_ldk_vh_register_net_sysctl_table(head, &create_ops, &ops, &need_create);
		if (!need_create || sysctl_need_create_batch(head, dir)) {
			goto err;
		}
#endif
	}

	ret = __procfs_sys_mk_path(&entry, dir, entry.namelen);
	if (ret < 0) {
		goto err;
	}

	if (need_create) {
		char *need_path = kmalloc(PATH_MAX, GFP_KERNEL);
		if (need_path == NULL) {
			pr_err("%s: kmalloc failed\n", __func__);
			goto err;
		}

		for (name = entry.name; name; name = nextname) {
			nextname = strchr(name, '/');
			if (nextname)
				nextname++;

			if (name == entry.name)
				continue;

			memset(need_path, 0, PATH_MAX);
			memcpy(need_path, entry.name, name - entry.name - 1);

			ret = create_ops->create_dir(need_path, (unsigned int)S_IFDIR);
			if ((ret < 0) && (ret != -EEXIST))
				pr_err("liblinux: create dir '%s' failed, ret=%d\n", need_path, ret);
		}
		kfree(need_path);
	}

	/* make sure dir exist */
	ret = __liblinux_proc_register(entry.name, (umode_t)S_IFDIR, NULL, NULL, create_ops);
	if ((ret < 0) && (ret != -EEXIST)) {
		goto err;
	}
	ret = __procfs_sys_register(&entry, head, table, ops, create_ops);
	if(ret < 0)
		pr_debug("__procfs_sys_register failed, ret = %d\n", ret);
err:
	mutex_unlock(&path_lock);
}
INIT_VENDOR_HOOK(ldk_vh_register_sysctl_table, hook_register_sysctl_table);
