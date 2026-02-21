// SPDX-License-Identifier: GPL-2.0-only
#include <linux/compiler.h>
#include <linux/sync_file.h>
#include <linux/dma-fence.h>
#include <linux/dma-fence-array.h>
#include <linux/fs.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <securec.h>
#include "dma_buf_info.h"

#ifdef CONFIG_DFX_MEMCHECK
int is_sync_file(struct file *f);

int memcheck_debug_fence_info(struct file *file, unsigned int offset, unsigned int max_fences,
			      struct dma_fence_info *file_info, struct fences_info *fences_array)
{
	struct sync_file *sync_file = NULL;
	struct dma_fence **fences = NULL;
	unsigned int total_num_fences, i;
	int ret;
	char *name = NULL;

	if (!is_sync_file(file))
		return -EPERM;

	sync_file = (struct sync_file *)file->private_data;
	if (dma_fence_is_array(sync_file->fence)) {
		struct dma_fence_array *array = to_dma_fence_array(sync_file->fence);
		total_num_fences = array->num_fences;
		fences = array->fences;
	} else {
		total_num_fences = 1;
		fences = &sync_file->fence;
	}

	if (strlen(sync_file->user_name))
		name = sync_file->user_name;
	else
		name = "NULL";

	ret = strncpy_s(file_info->fence_name, FENCE_NAME_LEN, name, FENCE_NAME_LEN - 1);
	if (ret < 0) {
		pr_err("file_info->fence_name copy failed, err=%d\n", ret);
	}

	file_info->i_ino = file_inode(file)->i_ino;
	file_info->total_num_fences = total_num_fences;
	if (offset < total_num_fences)
		file_info->num_fences = min(total_num_fences - offset, max_fences);
	else
		file_info->num_fences = 0;
	for (i = 0; i < file_info->num_fences; i++) {
		ret = strncpy_s(fences_array[i].timeline_name, FENCE_NAME_LEN,
				fences[offset + i]->ops->get_timeline_name(fences[offset + i]),
				FENCE_NAME_LEN - 1);
		if (ret < 0) {
			pr_err("fences_array[%d].timeline_name copy failed!\n", offset + i);
		}

		ret = strncpy_s(fences_array[i].driver_name, FENCE_NAME_LEN,
				fences[offset + i]->ops->get_driver_name(fences[offset + i]),
				FENCE_NAME_LEN - 1);
		if (ret < 0) {
			pr_err("fences_array[%d].driver_name copy failed!\n", offset + i);
		}

		fences_array[i].status = dma_fence_get_status(fences[offset + i]);
		if (ktime_to_ns(fences[offset + i]->timestamp) >= 0)
			fences_array[i].timestamp = (size_t)ktime_to_ns(fences[offset + i]->timestamp);
		else
			fences_array[i].timestamp = 0;
	}
	return 0;
}
#else /* CONFIG_DFX_MEMCHECK */
int memcheck_debug_fence_info(struct file *file, unsigned int offset, unsigned int max_fences,
			      struct dma_fence_info *file_info, struct fences_info *fences_array)
{
	return -EPERM;
}
#endif /* CONFIG_DFX_MEMCHECK */
EXPORT_SYMBOL(memcheck_debug_fence_info);
