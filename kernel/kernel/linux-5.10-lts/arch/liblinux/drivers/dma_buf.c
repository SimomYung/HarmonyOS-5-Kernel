// SPDX-License-Identifier: GPL-2.0-only
#include <linux/sched/clock.h>
#include <linux/sched.h>
#include <linux/spinlock.h>
#include <linux/dma-buf.h>
#include <liblinux/pal.h>
#include <securec.h>

#ifdef CONFIG_DMABUF_PROCESS_INFO
struct dma_buf_info_cache {
	pid_t pid;
	pid_t tgid;
	char exp_thread_comm[TASK_COMM_LEN]; /* Name of exporter thread */
	char exp_task_comm[TASK_COMM_LEN]; /* Name of exporter task */
	spinlock_t	cache_lock;
};

static struct dma_buf_info_cache g_dmabuf_info_cache = {
	.pid = -1,
	.cache_lock = __SPIN_LOCK_UNLOCKED(g_dmabuf_info_cache.cache_lock),
};

void __override init_dma_buf_task_info(struct dma_buf *dmabuf)
{
	int ret;

	dmabuf->pid = current->pid;
	get_task_comm(dmabuf->exp_thread_comm, current);
	spin_lock(&g_dmabuf_info_cache.cache_lock);
	if (dmabuf->pid == g_dmabuf_info_cache.pid &&
		strcmp(dmabuf->exp_thread_comm, g_dmabuf_info_cache.exp_thread_comm) == 0) {
			dmabuf->tgid = g_dmabuf_info_cache.tgid;
			(void)strncpy_s(dmabuf->exp_task_comm, TASK_COMM_LEN, g_dmabuf_info_cache.exp_task_comm, TASK_COMM_LEN - 1);
	} else {
		spin_unlock(&g_dmabuf_info_cache.cache_lock);
		ret = liblinux_pal_query_main_thread(dmabuf->pid, &dmabuf->tgid,
											 dmabuf->exp_task_comm, TASK_COMM_LEN);
		if (ret < 0) {
			dmabuf->tgid = 0;
			strlcpy(dmabuf->exp_task_comm, "[kernel task]", TASK_COMM_LEN);
		}
		spin_lock(&g_dmabuf_info_cache.cache_lock);
		g_dmabuf_info_cache.pid = dmabuf->pid;
		g_dmabuf_info_cache.tgid = dmabuf->tgid;
		(void)strncpy_s(g_dmabuf_info_cache.exp_thread_comm, TASK_COMM_LEN, dmabuf->exp_thread_comm, TASK_COMM_LEN - 1);
		(void)strncpy_s(g_dmabuf_info_cache.exp_task_comm, TASK_COMM_LEN, dmabuf->exp_task_comm, TASK_COMM_LEN - 1);
	}
	spin_unlock(&g_dmabuf_info_cache.cache_lock);
}
#endif

int __override dma_buf_fd(struct dma_buf *dmabuf, int flags)
{
	int fd;
	struct dh_fd_install_context ctx;
	struct file *file = NULL;

	if (!dmabuf)
		return -EINVAL;
#ifdef DMABUF_UDK_EN
	if (dmabuf->from_udk == true) {
		return dma_buf_call_dma_buf_fd(dmabuf, flags);
	}
#endif
	if (!dmabuf->file)
		return -EINVAL;

	file = dmabuf->file;
	if (file->f_path.dentry && file->f_path.dentry->d_name.name) {
		ctx.file_name = file->f_path.dentry->d_name.name;
		ctx.buffer_len = file->f_path.dentry->d_name.len + 1;

		fd = liblinux_pal_alloc_unused_fd(flags, file, &ctx);
	} else {
		fd = liblinux_pal_alloc_unused_fd(flags, file, NULL);
	}

	return fd;
}
