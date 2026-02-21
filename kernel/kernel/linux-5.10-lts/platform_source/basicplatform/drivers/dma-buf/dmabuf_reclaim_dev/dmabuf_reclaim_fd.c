/*
 * dmabuf_reclaim_fd.c
 *
 * Copyright (C) 2024 Hisilicon Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <liblinux/file_utils.h>
#include <linux/dma-buf.h>
#include <linux/dma-heap.h>
#include <linux/dmabuf/dmabuf_reclaim.h>
#include <linux/dmabuf/dmabuf_reclaim_manage.h>
#include <linux/dmabuf/dmabuf_swap.h>
#include <linux/dmabuf/mm_dma_heap.h>
#include <linux/err.h>
#include <linux/jiffies.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/seq_file.h>
#include <securec.h>

#define WAIT_RESUME_TIME_OUT 1000 // 1000ms

static DECLARE_WAIT_QUEUE_HEAD(resume_wait_queue);

int mm_dmabuf_reclaim_ino(int pid, unsigned long ino)
{
	int state = 0;
	int ret = 0;
	unsigned long len = 0;

	ret = add_pid_info(pid);
	if (ret) {
		pr_err("%s: add pid[%d] info failed!\n", __func__, pid);
		return ret;
	}

	if (!set_pid_reclaim_state(pid)) {
		return 0;
	}

	len = get_reclaimed_list_len(pid);
	if (len >= RECLAIM_LIST_LEN_MAX) {
		pr_err("%s: dmabuf reclaim len is %lu, which exceeds the maximum value: [%d] \n", __func__, len,
		       RECLAIM_LIST_LEN_MAX);
		return 0;
	}

	ret = mm_dmabuf_assign_reclaim(ino);
	if (ret == EOK) {
		// if other threads resume the reclaimed list of the pid, the insert operation fails
		if (!insert_ino_to_reclaimed(pid, ino)) {
			ret = mm_dmabuf_assign_resume(ino);
		}
	} else {
		pr_err("%s: dmabuf reclaim fail\n", __func__);
	}
	return ret;
}

int mm_dmabuf_resume_pid(int pid)
{
	struct mutex *resume_lock = NULL;
	struct reclaimed_dmabuf *entry, *tmp;
	struct list_head reclaimed_list;
	int ret = 0;
	int state = 0;

	INIT_LIST_HEAD(&reclaimed_list);
	state = get_reclaimed_pid_info_state(pid);
	if (state == DMABUF_STATE_RESUMED || state == DMABUF_STATE_INVALID) {
		return 0;
	}

	if (wait_resuming(pid)) {
		// wait until another thread finish resuming and just return
		wait_event_interruptible_timeout(resume_wait_queue, get_reclaimed_pid_info_state(pid) != DMABUF_STATE_RESUMING, msecs_to_jiffies(WAIT_RESUME_TIME_OUT));
		return 0;
	}

	if (!get_reclaimed_list(pid, &reclaimed_list)) {
		goto done;
	}
	list_for_each_entry_safe(entry, tmp, &reclaimed_list, list) {
		unsigned long ino = entry->ino;
		int tmp_ret = mm_dmabuf_assign_resume(ino);
		if (tmp_ret < 0) {
			ret = tmp_ret;
			pr_err("%s: pid[%d] swap in fail!!!\n", __func__, pid);
		}
	}
	free_reclaimed_list(&reclaimed_list);
done:
	set_reclaimed_pid_info_state(pid, DMABUF_STATE_RESUMED);
	wake_up_interruptible(&resume_wait_queue); // wake up other resume threads
	return ret;
}

int mm_dmabuf_clear_pid(int pid)
{
	return free_reclaimed_pid_info(pid);
}

int gpu_dmabuf_resume_fd(int fd)
{
	int ret = 0;
	struct dma_buf *dmabuf = NULL;

	if (!is_dmabuf_reclaim_enable()) {
		return 0;
	}

	dmabuf = dma_buf_get(fd);
	if (IS_ERR_OR_NULL(dmabuf)) {
		return -EFAULT;
	}

	if (!is_dma_heap_dmabuf(dmabuf)) {
		goto done;
	}

	if (!buffer_reclaim_status_get_lock(dmabuf->priv)) {
		goto done;
	}

	ret = dmabuf_resume(dmabuf);

done:
	dma_buf_put(dmabuf);
	return ret;
}

int gpu_pf_dmabuf_resume_inos(unsigned long *inos, uint32_t nums)
{
	int ret = 0;

	if (!is_dmabuf_reclaim_enable()) {
		return 0;
	}

	for (uint32_t i = 0; i < nums; i++) {
		ret = mm_dmabuf_assign_resume(inos[i]);
		if (ret) {
			pr_err("%s: gpu page fault resume ino[%lu] failed\n", __func__, inos[i]);
		}
	}
	pr_info("this gpu page fault resume ino nums:%u\n", nums);
	return ret;
}

int cpu_pf_dmabuf_resume_ino(unsigned long ino)
{
	int ret = 0;
	if (!is_dmabuf_reclaim_enable()) {
		return 0;
	}
	ret = mm_dmabuf_assign_resume(ino);
	if (ret) {
		pr_err("%s: cpu page fault resume ino:%lu failed\n", __func__, ino);
	}
	pr_info("this cpu page fault resume ino:%lu\n", ino);
	return ret;
}

int mm_dmabuf_free_ext(struct dma_buf *dmabuf)
{
	int ret = 0;
	struct mutex *lock = NULL;
	if (!is_dmabuf_reclaim_enable()) {
		return 0;
	}
	if (!dmabuf || !dmabuf->priv) {
		pr_err("%s: dmabuf or dmabuf->priv is NULL\n", __func__);
		return -EFAULT;
	}

	if (!is_dma_heap_dmabuf(dmabuf)) {
		return 0; // skip ion, is normal case
	}

	lock = buffer_lock_get(dmabuf->priv);
	if (!lock) {
		pr_err("%s: get buffer lock fail\n", __func__);
		return -EFAULT;
	}
	mutex_lock(lock);

	if (!buffer_reclaim_status_get(dmabuf->priv)) {
		mutex_unlock(lock);
		return 0;
	}

	ret = dmabuf_page_swap_free_extid(dmabuf);
	if (ret) {
		pr_err("%s: fail, ret[%d]\n", __func__, ret);
	}
	mutex_unlock(lock);

	return ret;
}

int mm_dmabuf_reclaim_fd(int pid, unsigned int buf_fd)
{
	struct file *dma_file = NULL;
	struct dma_buf *dmabuf = NULL;
	int ret = 0;

	dma_file = liblinux_ext_pidfd_fget(pid, buf_fd);
	if (!dma_file) {
		pr_err("%s: dmabuf file get by pid[%d] fd[%u] fail\n", __func__, pid, buf_fd);
		return -EFAULT;
	}

	dmabuf = file_to_dma_buf(dma_file);
	if (!dmabuf) {
		pr_err("%s: dmabuf get by pid[%d] fail\n", __func__, pid);
		fput(dma_file);
		return -EFAULT;
	}

	ret = dmabuf_reclaim(dmabuf);
	fput(dma_file);
	return ret;
}

int mm_dmabuf_resume_fd(int pid, unsigned int buf_fd)
{
	struct file *dma_file = NULL;
	struct dma_buf *dmabuf = NULL;
	int ret = 0;

	dma_file = liblinux_ext_pidfd_fget(pid, buf_fd);
	if (!dma_file) {
		pr_err("%s: dmabuf file get by pid[%d] fd[%u] fail\n", __func__, pid, buf_fd);
		return -EFAULT;
	}

	dmabuf = file_to_dma_buf(dma_file);
	if (!dmabuf) {
		pr_err("%s: dmabuf get by pid[%d] fail\n", __func__, pid);
		fput(dma_file);
		return -EFAULT;
	}

	ret = dmabuf_resume(dmabuf);
	fput(dma_file);
	return ret;
}
