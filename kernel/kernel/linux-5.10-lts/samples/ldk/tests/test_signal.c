// SPDX-License-Identifier: GPL-2.0
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/mm.h>
#include <linux/namei.h>
#include <linux/file.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/completion.h>

#define CMD_SIGNAL_SEND_SIGUSR1	1U
#define CMD_TIMEOUT_MS	100

static struct task_struct *test_thread;
static DECLARE_WAIT_QUEUE_HEAD(wait);
static int flag = 0;
static DECLARE_COMPLETION(sig_complete);

static int thread_func(void *data)
{
	int sig = -1;

	wait_event_interruptible(wait, flag != 0);

	sig = kernel_dequeue_signal();
	if (sig == SIGUSR1)
		flag = 1;
	else
		return 0;

	complete(&sig_complete);
	test_thread = NULL;
	return 0;
}

static long ldk_signal_ioctl(struct file *fdev, unsigned int cmd, unsigned long args)
{
	int ret = 0;

	if (!test_thread) {
		pr_err("%s: test_thread is NULL\n", __func__);
		return -EPERM;
	}

	if (cmd == CMD_SIGNAL_SEND_SIGUSR1)
		send_sig_info(SIGUSR1, SEND_SIG_PRIV, test_thread);
	else
		return -EPERM;

	if (!wait_for_completion_timeout(&sig_complete, msecs_to_jiffies(CMD_TIMEOUT_MS))) {
		pr_err("%s: wait signal completion timeout\n", __func__);
		return -EFAULT;
	}

	if (!flag)
		ret = -EFAULT;

	return ret;
}

static const struct file_operations ldk_signal_fops = {
	.unlocked_ioctl	= ldk_signal_ioctl,
};

static struct miscdevice ldk_signal_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "ldk_signal_dev",
	.fops = &ldk_signal_fops,
};

static int __init liblinux_signal_test_init(void)
{
	int ret;

	ret = misc_register(&ldk_signal_dev);
	if (ret) {
		pr_err("%s: misc_register failed, ret=%d\n", __func__, ret);
		return ret;
	}

	test_thread = kthread_create(thread_func, NULL, "signal_test");
	if (IS_ERR_OR_NULL(test_thread)) {
		ret = PTR_ERR(test_thread);
		pr_err("%s: kthread_create failed, ret=%d\n", __func__, ret);
		misc_deregister(&ldk_signal_dev);
		return ret;
	}

	wake_up_process(test_thread);
	pr_info("liblinux_signal_test case\n");

	return ret;
}

static void __exit liblinux_signal_test_deinit(void)
{
	misc_deregister(&ldk_signal_dev);
	if (!flag) {
		flag = 1;
		wake_up_interruptible(&wait);
	}
}

module_init(liblinux_signal_test_init);
module_exit(liblinux_signal_test_deinit);

MODULE_LICENSE("GPL");
