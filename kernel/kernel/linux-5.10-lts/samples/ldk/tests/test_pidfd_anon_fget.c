#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/proc_fs.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <liblinux/file_utils.h>

#define TEST_PIDFD_FGET_PROCNAME "test_pidfd_fget"

static struct proc_dir_entry *root;

static int test_pidfd_fget_show(struct seq_file *m, void *v)
{
	seq_printf(m, "input format: pid-fd\n");
	return 0;
}

static int test_pidfd_fget_open(struct inode *inode, struct file *file)
{
	return single_open(file, test_pidfd_fget_show, PDE_DATA(inode));
}

static int test_liblinux_pidfd_fget(int pid, int fd)
{
    struct file *file = liblinux_ext_pidfd_fget(pid, fd);
    if (file == NULL) {
        return -EINVAL;
    }
    fput(file);
    return 0;
}

ssize_t test_pidfd_fget_write(struct file *file, const char __user *buffer, size_t len, loff_t *loff)
{
    int pid, fd, ret;
    char *kbuf, *pid_str, *fd_str;

	kbuf = memdup_user(buffer, len);
	if (IS_ERR(kbuf)) {
        return PTR_ERR(kbuf);
    }

    pid_str = kbuf;
    fd_str = strstr(kbuf, "-");
    fd_str[0] = '\0';
    fd_str++;

    ret = kstrtoint(pid_str, 10, &pid);
    if (ret < 0) {
        goto out;
    }

    ret = kstrtoint(fd_str, 10, &fd);
    if (ret < 0) {
        goto out;
    }

    ret = test_liblinux_pidfd_fget(pid, fd);
    if (ret < 0) {
        goto out;
    }

    kfree(kbuf);
    return len;

out:
    kfree(kbuf);
    return ret;
}

static const struct proc_ops test_pidfd_fget_fops = {
	.proc_open = test_pidfd_fget_open,
	.proc_write = test_pidfd_fget_write,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};

static int test_pidfd_fget_create_procfs(void)
{
    root = proc_create_data(TEST_PIDFD_FGET_PROCNAME, 0666, NULL, &test_pidfd_fget_fops, NULL);
    if (!root) {
        pr_err("create %s proc file failed.\n", TEST_PIDFD_FGET_PROCNAME);
        return -ENOENT;
    }
    return 0;
}

static int __init test_pidfd_fget_init(void)
{
	int ret = test_pidfd_fget_create_procfs();
	if (ret < 0)
		return ret;
	return 0;
}

static void __exit test_pidfd_fget_exit(void)
{
	proc_remove(root);
}

module_init(test_pidfd_fget_init);
module_exit(test_pidfd_fget_exit);
MODULE_LICENSE("GPL v2");
