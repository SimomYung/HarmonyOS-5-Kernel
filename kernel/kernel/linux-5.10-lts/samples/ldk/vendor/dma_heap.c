// SPDX-License-Identifier: GPL-2.0-only

#include <linux/compiler.h>
#include <linux/dma-buf.h>
#include <linux/dma-heap.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <securec.h>
#include <platform_include/basicplatform/linux/dmabuf/mm_dma_heap.h>

#include "mm_heap_priv.h"
#include "mm_heap_helpers.h"
#include "dma_buf_info.h"

#ifdef CONFIG_DMABUF_MM
static bool is_mm_heap_helper_buffer(struct dma_buf *dmabuf)
{
	return dmabuf->ops == &mm_heap_helper_ops;
}

int dmaheap_debug_dma_buf_info(struct file *file, struct dma_buf_info *file_info)
{
	struct dma_buf *dbuf = NULL;
	struct mm_heap_helper_buffer *hbuf = NULL;
	int ret;

	if (file == NULL || file_info == NULL)
		return -EINVAL;

	if (!is_dma_buf_file(file))
		return -EINVAL;

	dbuf = file_to_dma_buf(file);
	if (!dbuf)
		return -EINVAL;

	if (!is_mm_heap_helper_buffer(dbuf))
		return -EINVAL;

	hbuf = dbuf->priv;
	if (!hbuf)
		return -EINVAL;

	file_info->size = dbuf->size;
	file_info->magic = hbuf->magic;
	file_info->allocator_pid = hbuf->pid;
	ret = strncpy_s(file_info->allocator_comm, TASK_COMM_LEN, hbuf->task_comm, TASK_COMM_LEN - 1);
	if (ret < 0) {
		pr_err("file_info->allocator_comm copy failed, err=%d\n", ret);
	}
	return 0;
}
#else /* CONFIG_DMABUF_MM */
int dmaheap_debug_dma_buf_info(struct file *file, struct dma_buf_info *file_info)
{
	return -EPERM;
}
#endif /* CONFIG_DMABUF_MM */
EXPORT_SYMBOL(dmaheap_debug_dma_buf_info);

#if defined(CONFIG_DFX_MEMCHECK) || defined(CONFIG_DMABUF_PROCESS_INFO)
int memcheck_debug_ion_info(struct file *file, struct ion_info *file_info)
{
	struct dma_buf *dbuf = NULL;
	int ret;

	if (file == NULL || file_info == NULL)
		return -EINVAL;

	if (!is_dma_buf_file(file))
		return -EPERM;

	dbuf = file_to_dma_buf(file);
	if (!dbuf)
		return -EPERM;

	file_info->size = dbuf->size;
	file_info->i_ino = file_inode(dbuf->file)->i_ino;
	file_info->allocator_tgid = dbuf->tgid;
	file_info->allocator_pid = dbuf->pid;
	file_info->reclaimable = is_direct_reclaim_dmabuf(dbuf);
	if (file_info->reclaimable == false) {
		file_info->is_reclaimed = false;
	} else {
		file_info->is_reclaimed = buffer_reclaim_status_get_lock(dbuf->priv);
	}

	ret = strncpy_s(file_info->exp_task_comm, TASK_COMM_LEN, dbuf->exp_task_comm, TASK_COMM_LEN - 1);
	if (ret < 0) {
		pr_err("file_info->exp_task_comm copy failed, err=%d\n", ret);
	}
	ret = strncpy_s(file_info->exp_thread_comm, TASK_COMM_LEN, dbuf->exp_thread_comm, TASK_COMM_LEN - 1);
	if (ret < 0) {
		pr_err("file_info->exp_thread_comm copy failed, err=%d\n", ret);
	}

	spin_lock(&dbuf->name_lock);
	if (dbuf->name)
		ret = strncpy_s(file_info->name, DMA_BUF_NAME_LEN, dbuf->name, DMA_BUF_NAME_LEN - 1);
	else
		ret = strncpy_s(file_info->name, DMA_BUF_NAME_LEN, "NULL", DMA_BUF_NAME_LEN - 1);
	spin_unlock(&dbuf->name_lock);
	if (ret < 0) {
		pr_err("file_info->name copy failed, err=%d\n", ret);
	}

	if (dbuf->exp_name)
		ret = strncpy_s(file_info->exp_name, DMA_BUF_NAME_LEN, dbuf->exp_name, DMA_BUF_NAME_LEN - 1);
	else
		ret = strncpy_s(file_info->exp_name, DMA_BUF_NAME_LEN, "NULL", DMA_BUF_NAME_LEN - 1);
	if (ret < 0) {
		pr_err("file_info->exp_name copy failed, err=%d\n", ret);
	}

	spin_lock(&dbuf->name_lock);
	if (dbuf->type)
		ret = strncpy_s(file_info->type, DMA_BUF_TYPE_LEN, dbuf->type, DMA_BUF_TYPE_LEN - 1);
	else
		ret = strncpy_s(file_info->type, DMA_BUF_TYPE_LEN, "NULL", DMA_BUF_TYPE_LEN - 1);
	spin_unlock(&dbuf->name_lock);
	if (ret < 0) {
		pr_err("file_info->type copy failed, err=%d\n", ret);
	}

	return 0;
}
#else /* CONFIG_DFX_MEMCHECK || CONFIG_DMABUF_PROCESS_INFO */
int memcheck_debug_ion_info(struct file *file, struct ion_info *file_info)
{
	return -EPERM;
}
#endif /* CONFIG_DFX_MEMCHECK || CONFIG_DMABUF_PROCESS_INFO */
EXPORT_SYMBOL(memcheck_debug_ion_info);

#ifdef CONFIG_DMABUF_MM
void __override mm_dmaheap_init_dump(struct mm_heap_helper_buffer *buffer)
{
	struct dma_buf *dmabuf = buffer->dmabuf;
	if (dmabuf == NULL) {
		strlcpy(buffer->task_comm, "invalid task",
				sizeof(buffer->task_comm));
		buffer->pid = 0;
	} else {
		(void)strncpy_s(buffer->task_comm, TASK_COMM_LEN, dmabuf->exp_task_comm,
				TASK_COMM_LEN - 1);
		buffer->pid = dmabuf->tgid;
	}
}
EXPORT_SYMBOL(mm_dmaheap_init_dump);
#endif
