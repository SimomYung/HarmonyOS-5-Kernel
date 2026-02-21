/*
 * Copyright(C) 2019-2020 Hisilicon Technologies Co., Ltd. All rights reserved.
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

#include <linux/dma-buf.h>
#include <linux/fdtable.h>
#ifdef CONFIG_H_MM_PAGE_TRACE
#include <linux/mm/mem_trace.h>
#endif
#include <linux/sched/signal.h>
#ifdef CONFIG_MM_CMA_DEBUG
#include <linux/mm/mm_cma_debug.h>
#endif
#include <linux/dmabuf/mm_dma_heap.h>

#include "mm_heap_helpers.h"
#include "mm_heap_priv.h"

unsigned long mm_dmaheap_total(void)
{
	return get_dmaheap_alloc_total_size();
}

static inline struct mm_heap_helper_buffer *get_dmaheap_buf(struct dma_buf *dbuf)
{
	return dbuf->priv;
}

void mm_dmaheap_init_dump(struct mm_heap_helper_buffer *buffer)
{
	struct task_struct *task = NULL;
	pid_t pid;

	get_task_struct(current->group_leader);
	task_lock(current->group_leader);
	pid = task_pid_nr(current->group_leader);
	/*
	 * don't bother to store task struct for kernel threads,
	 * they can't be killed anyway
	 */
	if (current->group_leader->flags & PF_KTHREAD)
		task = NULL;
	else
		task = current->group_leader;

	task_unlock(current->group_leader);
	put_task_struct(current->group_leader);

	if (!task) {
		/*
		 * record ion_alloc from kernel with
		 * name "invalid task" and pid 0;
		 */
		strncpy(buffer->task_comm, "invalid task",
			sizeof(buffer->task_comm));
		buffer->pid = 0;
	} else {
		get_task_comm(buffer->task_comm, task);
		buffer->pid = task_pid_nr(task);
	}
}

static int mm_dmaheap_debug_process_cb(const void *data,
			      struct file *f, unsigned int fd)
{
	const struct task_struct *tsk = data;
	struct dma_buf *dbuf = NULL;
	struct mm_heap_helper_buffer *hbuf = NULL;

	if (!is_dma_buf_file(f))
		return 0;

	dbuf = file_to_dma_buf(f);
	if (!dbuf)
		return 0;

	if (!is_dma_heap_dmabuf(dbuf))
		return 0;

	hbuf = get_dmaheap_buf(dbuf);
	if (!hbuf)
		return 0;

	pr_err("%-16.s%3s%-16d%-16u%-24s%-16u%-16llu\n",
	tsk->comm, "", tsk->pid, fd,
	hbuf->heap->name, dbuf->size, hbuf->magic);
	return 0;
}

int mm_dmaheap_proecss_info(void)
{
	struct task_struct *tsk = NULL;

	pr_err("Detail process info is below:\n");
	pr_err("%-16.s%3s%-16s%-16s%-24s%-16s%-16s\n",
	"taskname", "", "pid", "fd", "heap_name", "size", "magic");

	rcu_read_lock();
	for_each_process(tsk) {
		if (tsk->flags & PF_KTHREAD)
			continue;

		task_lock(tsk);
		(void)iterate_fd(tsk->files, 0,
			mm_dmaheap_debug_process_cb, (void *)tsk);
		task_unlock(tsk);
	}
	rcu_read_unlock();

	return 0;
}

static size_t mm_dmaheap_detail_cb(const void *data,
			      struct file *f, unsigned int fd)
{
	struct dma_buf *dbuf = NULL;
	struct mm_heap_helper_buffer *hbuf = NULL;

	if (!is_dma_buf_file(f))
		return 0;

	dbuf = file_to_dma_buf(f);
	if (!dbuf)
		return 0;

	if (!is_dma_heap_dmabuf(dbuf))
		return 0;

	hbuf = get_dmaheap_buf(dbuf);
	if (!hbuf)
		return 0;

	return dbuf->size;
}

static size_t dmaheap_iterate_fd(struct files_struct *files, unsigned int n,
		size_t (*f)(const void *, struct file *, unsigned),
		const void *p)
{
	struct fdtable *fdt = NULL;
	size_t res = 0;
	struct file *file = NULL;

	if (!files)
		return 0;
	spin_lock(&files->file_lock);
	fdt = files_fdtable(files);
	for (; n < fdt->max_fds; n++) {
		file = rcu_dereference_check_fdtable(files, fdt->fd[n]);
		if (!file)
			continue;
		res += f(p, file, n);
	}
	spin_unlock(&files->file_lock);
	return res;
}

#ifdef CONFIG_H_MM_PAGE_TRACE
size_t mm_get_dmaheap_detail(void *buf, size_t len)
{
	size_t cnt = 0;
	size_t size;
	struct task_struct *tsk = NULL;
	struct mm_dmaheap_detail_info *info =
		(struct mm_dmaheap_detail_info *)buf;

	if (!buf)
		return cnt;
	rcu_read_lock();
	for_each_process(tsk) {
		if (tsk->flags & PF_KTHREAD)
			continue;
		if (cnt >= len) {
			rcu_read_unlock();
			return len;
		}
		task_lock(tsk);
		size = dmaheap_iterate_fd(tsk->files, 0,
			mm_dmaheap_detail_cb, (void *)tsk);
		task_unlock(tsk);
		if (size) { /* ion fd */
			(info + cnt)->size = size;
			(info + cnt)->pid = tsk->pid;
			cnt++;
		}
	}
	rcu_read_unlock();
	return cnt;
}
#endif

static size_t mm_get_dmaheap_size_by_pid(pid_t pid)
{
	size_t size;
	struct task_struct *tsk = NULL;

	rcu_read_lock();
	tsk = find_task_by_vpid(pid);
	if (!tsk) {
		rcu_read_unlock();
		return 0;
	}
	if (tsk->flags & PF_KTHREAD) {
		rcu_read_unlock();
		return 0;
	}
	task_lock(tsk);
	size = dmaheap_iterate_fd(tsk->files, 0,
		mm_dmaheap_detail_cb, (void *)tsk);
	task_unlock(tsk);

	rcu_read_unlock();
	return size;
}

void mm_dmaheap_process_summary_info(void)
{
	struct task_struct *tsk = NULL;
	size_t tsksize;

	pr_err("Summary process info is below:\n");
	pr_err("%-16.s%3s%-16s%-16s\n", "taskname", "",
	"pid", "totalsize");

	rcu_read_lock();
	for_each_process(tsk) {
		if (tsk->flags & PF_KTHREAD)
			continue;

		task_lock(tsk);
		tsksize = dmaheap_iterate_fd(tsk->files, 0,
				mm_dmaheap_detail_cb, (void *)tsk);
		if (tsksize)
			pr_err("%-16.s%3s%-16d%-16zu\n",
			tsk->comm, "", tsk->pid, tsksize);
		task_unlock(tsk);
	}
	rcu_read_unlock();
}

int mm_dmaheap_memory_info(bool verbose)
{
	struct rb_node *n = NULL;

	pr_info("dmaheap alloc total size:%lu\n", get_dmaheap_alloc_total_size());
	if (!verbose)
		return 0;

	pr_info("orphaned allocations (info is from last known client):\n");
	mutex_lock(&dmaheap_buffers_lock);
	for (n = rb_first(&dmaheap_buffers_rb); n; n = rb_next(n)) {
		struct mm_heap_helper_buffer *buffer = rb_entry(n,
					struct mm_heap_helper_buffer, node);

		if (buffer->heap->name)
			pr_info("%16.s %16d %16zu %16s %16llu\n",
				buffer->task_comm, buffer->pid, buffer->size,
				buffer->heap->name, buffer->magic);
	}
	mutex_unlock(&dmaheap_buffers_lock);

	mm_dmaheap_process_summary_info();

	mm_dmaheap_proecss_info();
#ifdef CONFIG_MM_CMA_DEBUG
	dump_cma_mem_info();
#endif
	mm_svc_secmem_info();

	return 0;
}
