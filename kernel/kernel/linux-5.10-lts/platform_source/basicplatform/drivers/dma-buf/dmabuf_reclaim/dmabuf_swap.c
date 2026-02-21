/*
 * dmabuf_swap.c
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
#include <linux/err.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/slab.h>
#include <linux/scatterlist.h>
#include <linux/dma-buf.h>
#include <linux/dma-heap.h>
#include <linux/dmabuf/mm_dma_heap.h>
#include <linux/dmabuf/dmabuf_swap.h>

#include <liblinux/pal.h>

/* swap out/in max 64MB every time */
#define GPU_SWAP_MAX_PAGE		(0x4000)
/* swap space max 4GB */
#define GPU_SWAP_SPACE_MAX_SIZE		(0x100000000)

/* swap space number */
enum {
	GPU_SWAP_ID = 0,
	DMABUF_SWAP_ID = 1,
	MAX_SWAP_ID = 2,
};

struct dmabuf_swap_space {
	unsigned long total_size;
	unsigned int ext_size;
	struct mutex lock;
	struct mutex extid_lock;
	bool init_status;
};
static struct dmabuf_swap_space db_swap_space[MAX_SWAP_ID];

static struct mutex* dmabuf_get_swap_extid_lock(int swap_id)
{
	if (swap_id >= MAX_SWAP_ID)
		return NULL;

	return &db_swap_space[swap_id].extid_lock;
}

static bool mm_swap_space_init_status(int swap_id)
{
	bool init_status = false;

	mutex_lock(&db_swap_space[swap_id].lock);
	init_status = db_swap_space[swap_id].init_status;
	mutex_unlock(&db_swap_space[swap_id].lock);

	return init_status;
}

int mm_gpu_page_array_out(struct page **page, int num, int *ext_ids)
{
	unsigned long *pa_array = NULL;
	int ret_page = 0;
	int i, j, ret = 0;

	if (!mm_swap_space_init_status(GPU_SWAP_ID)) {
		pr_err("%s, gpu swap space not init\n", __func__);
		return -EINVAL;
	}

	if (!page || !ext_ids) {
		pr_err("%s, paras err\n", __func__);
		return -EINVAL;	
	}

	if ((num <= 0) || (num > GPU_SWAP_MAX_PAGE)) {
		pr_err("%s, num[%d] err, max page num[%d]\n",
				__func__, num, GPU_SWAP_MAX_PAGE);
		return -EINVAL;
	}

	pa_array = kvzalloc(num * sizeof(unsigned long), GFP_KERNEL);
	if (!pa_array) {
		pr_err("%s, alloc page_array nums[%d] fail\n", __func__, num);
		return -ENOMEM;
	}

	for (i = 0; i < num; i++) {
		ret = liblinux_pal_mem_swap_alloc_extentid(GPU_SWAP_ID,
							&ext_ids[i]);
		if (ret) {
			pr_err("%s, alloc extentid fail\n", __func__);
			goto err;
		}

		pa_array[i] = page_to_phys(page[i]);
	}

	ret = liblinux_pal_mem_swap_out_range(GPU_SWAP_ID, pa_array,
						ext_ids, num);
	if (ret) {
		pr_err("%s, swap out fail\n", __func__);
		goto err;
	}

	kvfree(pa_array);

	return ret;
err:
	for (j = 0; j < i; j++)
		liblinux_pal_mem_swap_free_extentid(GPU_SWAP_ID, ext_ids[j]);

	pr_err("%s, fail\n", __func__);
	kvfree(pa_array);
	return ret;
}

int mm_gpu_page_array_in(struct page **page, int num, int *ext_ids)
{
	unsigned long *pa_array = NULL;
	int i, ret = 0;

	if (!mm_swap_space_init_status(GPU_SWAP_ID)) {
		pr_err("%s, gpu swap space not init\n", __func__);
		return -EINVAL;
	}

	if (!page || !ext_ids) {
		pr_err("%s, paras err\n", __func__);
		return -EINVAL;	
	}

	if ((num <= 0) || (num > GPU_SWAP_MAX_PAGE)) {
		pr_err("%s, num[%d] err, max page num[%d]\n",
				__func__, num, GPU_SWAP_MAX_PAGE);
		return -EINVAL;
	}

	pa_array = kvzalloc(num * sizeof(unsigned long), GFP_KERNEL);
	if (!pa_array) {
		pr_err("%s, alloc page_array nums[%d] fail\n", __func__, num);
		return -ENOMEM;
	}

	for (i = 0; i < num; i++)
		pa_array[i] = page_to_phys(page[i]);

	ret = liblinux_pal_mem_swap_in_range(GPU_SWAP_ID, pa_array,
						ext_ids, num);
	if (ret) {
		pr_err("%s, swap in fail\n", __func__);
		goto done;
	}

	for (i = 0; i < num; i++)
		liblinux_pal_mem_swap_free_extentid(GPU_SWAP_ID, ext_ids[i]);

done:
	kvfree(pa_array);

	return ret;
}

void mm_gpu_page_array_free_extid(int num, int *ext_ids)
{
	int  i;

	if (!mm_swap_space_init_status(GPU_SWAP_ID)) {
		pr_err("%s, gpu swap space not init\n", __func__);
		return;
	}

	if (!ext_ids) {
		pr_err("%s, paras err\n", __func__);
		return;	
	}

	if ((num <= 0) || (num > GPU_SWAP_MAX_PAGE)) {
		pr_err("%s, num[%d] err, max page num[%d]\n",
				__func__, num, GPU_SWAP_MAX_PAGE);
		return;
	}

	for (i = 0; i < num; i++)
		liblinux_pal_mem_swap_free_extentid(GPU_SWAP_ID, ext_ids[i]);
}

static int mm_page_swap_space_init(int swap_id, unsigned long total_size,
					unsigned int ext_size)
{
	int ret = 0;

	if ((swap_id < 0) || (swap_id >= MAX_SWAP_ID)) {
		pr_err("%s, swap[id = %d] err\n", __func__, swap_id);
		return -EINVAL;
	}

	mutex_lock(&db_swap_space[swap_id].lock);
	/* for init operation, do nothing if swap space exists */
	if (total_size > 0 && db_swap_space[swap_id].init_status) {
		pr_err("%s, swap[id = %d] already init\n", __func__, swap_id);
		mutex_unlock(&db_swap_space[swap_id].lock);
		return 0;	
	}
	/*
	 * total_size == 0 means deinit operation,
	 * for deinit operation, do nothing if swap space NOT exists
	 */
	if (total_size == 0 && !db_swap_space[swap_id].init_status) {
		pr_err("%s, swap[id = %d] not initialized\n", __func__, swap_id);
		mutex_unlock(&db_swap_space[swap_id].lock);
		return 0;	
	}

	ret = liblinux_pal_mem_swap_space_init(swap_id, total_size, ext_size);
	if (ret) {
		pr_err("%s, swap_id[%d], total_size[0x%lx] init or deinit fail\n",
				__func__, swap_id, total_size);
		mutex_unlock(&db_swap_space[swap_id].lock);
		return ret;	
	}

	db_swap_space[swap_id].total_size = total_size;
	db_swap_space[swap_id].ext_size = ext_size;
	/* for deinit operation, init_status = false */
	db_swap_space[swap_id].init_status = total_size == 0 ? false : true;
	mutex_unlock(&db_swap_space[swap_id].lock);

	pr_info("%s, swap_id[%d], total_size[0x%lx], ext_size[0x%x] init\n",
				__func__, swap_id, total_size, ext_size);
	return ret;
}

int mm_gpu_page_swap_space_init(unsigned long total_size)
{
	if (total_size > GPU_SWAP_SPACE_MAX_SIZE) {
		pr_err("%s, size[0x%lx] toot large, max size[0x%lx]\n",
				__func__, total_size, GPU_SWAP_SPACE_MAX_SIZE);
		return -EINVAL;	
	}

	return mm_page_swap_space_init(GPU_SWAP_ID, total_size, PAGE_SIZE);
}

int dmabuf_page_swap_alloc_extid(struct dma_buf *dmabuf)
{
	int *extid_table = NULL;
	int extid_num = 0;
	int i, j, ret = 0;
	struct mutex* alloc_extid_lock = dmabuf_get_swap_extid_lock(DMABUF_SWAP_ID);

	if (alloc_extid_lock == NULL) {
		pr_err("%s, extid_lock get failed\n", __func__);
		return -EINVAL;
	}

	if (!mm_swap_space_init_status(DMABUF_SWAP_ID)) {
		pr_err("%s, dmabuf swap space not init\n", __func__);
		return -EINVAL;
	}

	extid_table = buffer_extid_table_get(dmabuf->priv);
	if (!extid_table) {
		pr_err("%s, dmabuf get extid table err\n", __func__);
		return -EINVAL;
	}

	extid_num = buffer_extid_num_get(dmabuf->priv);
	if (extid_num == 0) {
		pr_err("%s, dmabuf get extid num value 0\n", __func__);
		return -EINVAL;
	}
	mutex_lock(alloc_extid_lock);
	for (i = 0; i < extid_num; i++) {
		ret = liblinux_pal_mem_swap_alloc_extentid(DMABUF_SWAP_ID, &extid_table[i]);
		if (ret) {
			pr_err("%s, alloc extentid fail: %d/%d\n", __func__, i , extid_num);
			mutex_unlock(alloc_extid_lock);
			goto err;
		}
	}
	mutex_unlock(alloc_extid_lock);

	return ret;
err:
	for (j = 0; j < i; j++) {
		liblinux_pal_mem_swap_free_extentid(DMABUF_SWAP_ID, extid_table[j]);
		extid_table[j] = 0;
	}

	return ret;
}

int dmabuf_page_swap_out(struct dma_buf *dmabuf)
{
	unsigned long *pa_array;
	struct sg_table *sgtable;
	struct scatterlist *sg;
	int *extid_table = NULL;
	int extid_num = 0, cur_num = 0;
	int i, j, ret = 0;

	if (!mm_swap_space_init_status(DMABUF_SWAP_ID)) {
		pr_err("%s, dmabuf swap space not init\n", __func__);
		return -EINVAL;
	}

	extid_table = buffer_extid_table_get(dmabuf->priv);
	if (!extid_table) {
		pr_err("%s, dmabuf get extid table err\n", __func__);
		return -EINVAL;
	}

	extid_num = buffer_extid_num_get(dmabuf->priv);
	if (extid_num == 0) {
		pr_err("%s, dmabuf get extid num value 0\n", __func__);
		return -EINVAL;
	}

	sgtable = buffer_sgtable_get(dmabuf->priv);
	if (!sgtable) {
		pr_err("%s, dmabuf get sgtable err\n", __func__);
		return -EINVAL;
	}

	pa_array = kvzalloc(extid_num * sizeof(unsigned long), GFP_KERNEL);
	if (!pa_array) {
		pr_err("%s, alloc page_array nums[%d] fail\n", __func__, extid_num);
		return -ENOMEM;
	}

	for_each_sg(sgtable->sgl, sg, sgtable->nents, i) {
		unsigned long sg_base_phy = sg_phys(sg);
		int page_num = sg->length >> PAGE_SHIFT;

		for (j = 0; j < page_num; j++, cur_num++) {
			pa_array[cur_num] = sg_base_phy;
			sg_base_phy += PAGE_SIZE;
		}
	}

	ret = liblinux_pal_mem_swap_out_range(DMABUF_SWAP_ID, pa_array, extid_table, extid_num);
	if (ret)
		pr_err("%s, swap out fail\n", __func__);

	kvfree(pa_array);

	return ret;
}

int dmabuf_page_swap_in(struct dma_buf *dmabuf)
{
	unsigned long *pa_array = NULL;
	struct sg_table *sgtable = NULL;
	struct scatterlist *sg = NULL;
	int *extid_table = NULL;
	int extid_num = 0, cur_num = 0;
	int i, j, ret = 0;

	if (!mm_swap_space_init_status(DMABUF_SWAP_ID)) {
		pr_err("%s, dmabuf swap space not init\n", __func__);
		return -EINVAL;
	}

	extid_table = buffer_extid_table_get(dmabuf->priv);
	if (!extid_table) {
		pr_err("%s, dmabuf get extid table err\n", __func__);
		return -EINVAL;
	}

	extid_num = buffer_extid_num_get(dmabuf->priv);
	if (extid_num == 0) {
		pr_err("%s, dmabuf get extid num value 0\n", __func__);
		return -EINVAL;
	}

	sgtable = buffer_sgtable_get(dmabuf->priv);
	if (!sgtable) {
		pr_err("%s, dmabuf get sgtable err\n", __func__);
		return -EINVAL;
	}

	pa_array = kvzalloc(extid_num * sizeof(unsigned long), GFP_KERNEL);
	if (!pa_array) {
		pr_err("%s, alloc page_array nums[%d] fail\n", __func__, extid_num);
		return -ENOMEM;
	}

	for_each_sg(sgtable->sgl, sg, sgtable->nents, i) {
		unsigned long sg_base_phy = sg_phys(sg);
		int page_num = sg->length >> PAGE_SHIFT;

		for (j = 0; j < page_num; j++, cur_num++) {
			pa_array[cur_num] = sg_base_phy;
			sg_base_phy += PAGE_SIZE;
		}
	}

	ret = liblinux_pal_mem_swap_in_range(DMABUF_SWAP_ID, pa_array, extid_table, extid_num);
	if (ret)
		pr_err("%s, swap in fail\n", __func__);

	kvfree(pa_array);

	return ret;
}

int dmabuf_page_swap_free_extid(struct dma_buf *dmabuf)
{
	int *extid_table = NULL;
	int extid_num = 0;
	int i;

	if (!mm_swap_space_init_status(DMABUF_SWAP_ID)) {
		pr_err("%s, dmabuf swap space not init\n", __func__);
		return -EINVAL;
	}

	extid_table = buffer_extid_table_get(dmabuf->priv);
	if (!extid_table) {
		pr_err("%s, dmabuf get extid table err\n", __func__);
		return -EINVAL;
	}

	extid_num = buffer_extid_num_get(dmabuf->priv);
	if (extid_num == 0) {
		pr_err("%s, dmabuf get extid num value 0\n", __func__);
		return -EINVAL;
	}

	for (i = 0 ; i < extid_num; i++) {
		liblinux_pal_mem_swap_free_extentid(DMABUF_SWAP_ID, extid_table[i]);
		extid_table[i] = 0;
	}

	return 0;
}

int dmabuf_page_swap_space_init(unsigned long total_size)
{
	return mm_page_swap_space_init(DMABUF_SWAP_ID, total_size, PAGE_SIZE);
}

static int __init dmabuf_swap_init(void)
{
	int i;

	for (i = 0; i < MAX_SWAP_ID; i++) {
		db_swap_space[i].total_size = 0;
		db_swap_space[i].ext_size = 0;
		db_swap_space[i].init_status = false;
		mutex_init(&db_swap_space[i].lock);
		mutex_init(&db_swap_space[i].extid_lock);
	}

	pr_err("%s: init succ!\n", __func__);
	return 0;
}

subsys_initcall(dmabuf_swap_init);
