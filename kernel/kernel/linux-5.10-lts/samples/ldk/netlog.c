/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Interfaces for implement procfs
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 24 14:24:43 2024
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/socket.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <linux/module.h>
#include <linux/kfifo.h>
#include <linux/time.h>
#include <linux/net.h>
#include <net/sock.h>
#include <net/net_namespace.h>
#include <trace/hooks/liblinux.h>
#include <securec.h>

#ifdef CONFIG_HISYSEVENT
#include <dfx/hiview_hisysevent.h>
#endif

MODULE_DESCRIPTION("netlog");
MODULE_LICENSE("GPL");

#define FIFO_SIZE (64 * 1024)
#define RESERVE_SIZE (FIFO_SIZE/2)
static const char procname[] = "netlog";

#define kfifo_reserve(fifo, n) \
({ \
	typeof((fifo) + 1) __tmp = (fifo); \
	struct __kfifo *__kfifo = &__tmp->kfifo; \
	__kfifo->out += n; \
})

static struct {
	struct kfifo	  fifo;
	spinlock_t	  lock;
	int reserve;
} netlogw;

static int netlog_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int can_read_full(size_t len, loff_t *ppos) {
	int fifolen = kfifo_len(&netlogw.fifo);

	if (kfifo_is_empty(&netlogw.fifo) || *ppos == fifolen)
		return 0;
	if (len < fifolen)
		return -EFBIG;

	return 1;
}

static ssize_t netlog_read(struct file *file, char __user *buf,
			      size_t len, loff_t *ppos)
{
	int err = 0, readlen;
	unsigned long flags;
	unsigned char *log;

	if (!buf)
		return -EINVAL;

	if (len == 0)
		return 0;

	log = vzalloc(len);
	if (!log)
		return -ENOMEM;

/* read displays logs only after the second vfs_read returned,
 * so we dont use wait_event here, which will lead to real-time
 * log display not supported.
 */
	spin_lock_irqsave(&netlogw.lock, flags);
	err = can_read_full(len, ppos);
	if (err <= 0) {
		spin_unlock_irqrestore(&netlogw.lock, flags);
		vfree(log);
		return err;
	}

	readlen = kfifo_out_peek(&netlogw.fifo, log, len);
	spin_unlock_irqrestore(&netlogw.lock, flags);

	err = copy_to_user(buf, log, readlen) ? -EFAULT : 0;
	*ppos += readlen;
	vfree(log);

	return err ? err : readlen;
}

static const struct proc_ops netlog_fops = {
	.proc_open	= netlog_open,
	.proc_read	= netlog_read,
	.proc_lseek	= noop_llseek,
};

static void kfifo_add_log(char *buf, int len)
{
	unsigned long flags;

	spin_lock_irqsave(&netlogw.lock, flags);
	if (kfifo_len(&netlogw.fifo) < RESERVE_SIZE)
		netlogw.reserve = kfifo_len(&netlogw.fifo);

	if (len > kfifo_avail(&netlogw.fifo)) {
		kfifo_reserve(&netlogw.fifo, netlogw.reserve);
		netlogw.reserve = kfifo_len(&netlogw.fifo);
	}

	kfifo_in(&netlogw.fifo, buf, len);
	spin_unlock_irqrestore(&netlogw.lock, flags);
}

#ifdef CONFIG_HISYSEVENT
static void hisysevent_show(const char *fmt, va_list args)
{
	struct hiview_hisysevent *event = NULL;
	char log[256];
	int ret;

	event = hisysevent_create("KERNEL_NET", "NETLOG", FAULT);
	if (event == NULL) {
		pr_err("hisysevent report process create failed: NETLOG");
		return;
	}

	vscnprintf(log, sizeof(log), fmt, args);
	hisysevent_put_string(event, "EVENT_LOG", log);
	ret = hisysevent_write(event);
	if (ret < 0)
		pr_err("hisysevent report process write failed: NETLOG");
	hisysevent_destroy(&event);
	return;
}

void pr_hisysevent(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	hisysevent_show(fmt, args);
	va_end(args);
}
#else
void pr_hisysevent(const char *fmt, ...)
{
}
#endif

static void netlog_show(const char *fmt, va_list args)
{
	char info[256], log[256];

	struct timespec64 now;
	int len;

	len = vscnprintf(info, sizeof(info), fmt, args);

	getnstimeofday64(&now);
	len = snprintf_s(log, sizeof(log), sizeof(log) - 1,
			"[%lu.%06lu] pid=%d tid=%d comm=%s %s",
			(unsigned long) now.tv_sec, (unsigned long) now.tv_nsec / NSEC_PER_USEC,
			current->tgid, current->pid, current->comm, info);
	if (len < 0)
		return;

	kfifo_add_log(log, len);
}

void pr_netlog(const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	netlog_show(fmt, args);
	va_end(args);
}

static int __init netlog_init(void)
{
	int ret = -ENOMEM;

	spin_lock_init(&netlogw.lock);
	if (kfifo_alloc(&netlogw.fifo, FIFO_SIZE, GFP_KERNEL))
		return ret;

	kfifo_reset(&netlogw.fifo);
	netlogw.reserve = 0;

	if (!proc_create(procname, 0444, init_net.proc_net, &netlog_fops)) {
		kfifo_free(&netlogw.fifo);
		return ret;
	}

	return 0;
}

static __exit void netlog_exit(void)
{
	remove_proc_entry(procname, init_net.proc_net);
	kfifo_free(&netlogw.fifo);
}

module_init(netlog_init);
module_exit(netlog_exit);
