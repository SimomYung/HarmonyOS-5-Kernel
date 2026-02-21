// SPDX-License-Identifier: GPL-2.0
/*
 * DMA-BUF: dmabuf ext interface.
 *
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 */

#include <linux/err.h>
#include <linux/module.h>
#include <linux/dma-buf.h>
#include <linux/io.h>
#include <linux/file.h>
#include <linux/slab.h>
#include <uapi/linux/uio.h>

#include <liblinux/pal.h>
#include <linux/dmabuf/mm_dma_heap.h>

#include <sd_proxy.h>

#define dma_buf_ext_err(fmt, ...)	pr_err("dma buf ext: " fmt, ##__VA_ARGS__)
#define dma_buf_ext_info(fmt, ...)	pr_info("dma buf ext: " fmt, ##__VA_ARGS__)
#ifdef DMA_BUF_EXT_DBG
#define dma_buf_ext_dbg(fmt, ...)	pr_info("dma buf ext: " fmt, ##__VA_ARGS__)
#else
#define dma_buf_ext_dbg(fmt, ...)
#endif

enum dma_buf_ext_cmd {
	DMA_BUF_EXT_CMD_GET,
	DMA_BUF_EXT_CMD_PUT,
	DMA_BUF_EXT_CMD_GET_SG_COLLECT,
	DMA_BUF_EXT_CMD_MAX,
};

struct ldk_sg_collect {
	unsigned int nents;
	struct scatterlist *collect[];
};

struct ldk_dma_buf_msg {
	void *ptr;
	unsigned int sg_page_count;
};

static int ldk_flush_cache(struct dma_buf *buf)
{
	int ret;
 
	ret = dma_buf_begin_cpu_access(buf, DMA_BIDIRECTIONAL);
	if (ret) {
		dma_buf_ext_err("failed to begin cpu access %d\n", ret);
		return ret;
	}
 
	ret = dma_buf_end_cpu_access(buf, DMA_BIDIRECTIONAL);
	if (ret) {
		dma_buf_ext_err("failed to end cpu access %d\n", ret);
		return ret;
	}
 
	return ret;
}

static void sg_print(struct sg_table *sgt)
{
	struct scatterlist* sg_list;
	int s_num;
	int i;

	dma_buf_ext_dbg("sg print\n");
 
	s_num = (1 << PAGE_SHIFT) / sizeof(struct scatterlist) - 1;
	sg_list = sgt->sgl;

	dma_buf_ext_dbg("sg page pa 0x%lx va 0x%lx\n", virt_to_phys(sg_list), sg_list);
 
	for (i = 0; i < sgt->nents;) {
		dma_buf_ext_dbg("ldk dma buf pa:0x%lx dma len:%d\n",
			sg_list->dma_address, sg_list->dma_length);

		sg_list++;
		if ((++i % s_num) != 0) {
			continue;
		}

		/* depart multi 4k block */
		sg_list = sg_chain_ptr(sg_list);
		dma_buf_ext_dbg("sg page pa 0x%lx va 0x%lx\n", virt_to_phys(sg_list), sg_list);
	}
}

static inline int sg_page_count(struct sg_table *sgt)
{
	struct scatterlist *sgl;
	int count;

	count = 1;
	for (sgl = sgt->sgl; !sg_is_last(sgl);) {
		++sgl;
		if (!sg_is_chain(sgl))
			continue;
		sgl = sg_chain_ptr(sgl);
		++count;
	}

	return count;
}
 
static int dma_buf_ext_get(void *rd, size_t rd_size, void *wr, size_t wr_size)
{
	struct ldk_dma_buf_msg msg;
	struct dma_buf *buf;
	struct sg_table *sgt;
	int fd, ret;
 
	if (!rd || rd_size < sizeof(fd) ||
		!wr || wr_size < sizeof(buf)) {
		dma_buf_ext_err("get invalid param 0x%lx %d 0x%lx %d\n",
			rd, rd_size, wr, wr_size);
		return -EFAULT;
	}
	
	ret = liblinux_pal_copy_from_caller(&fd, rd, sizeof(fd));
	if (ret) {
		dma_buf_ext_err("failed to copy fd %d\n", ret);
		return ret;
	}
 
	buf = dma_buf_get(fd);
	if (IS_ERR(buf)) {
		ret = PTR_ERR(buf);
		dma_buf_ext_err("failed to get dma buf %d\n", ret);
		return ret;
	}

	sgt = dmabuf_heap_sg_table_get(buf);
	if (!sgt) {
		ret = -EFAULT;
		dma_buf_ext_err("failed to get sg table %d\n", ret);
		dma_buf_put(buf);
		return ret;
	}

	msg.ptr = buf;
	msg.sg_page_count = sg_page_count(sgt);

	return liblinux_pal_copy_to_caller(wr, &msg, sizeof(msg));
}
 
static int dma_buf_ext_put(void *rd, size_t rd_size, void *wr, size_t wr_size)
{
	struct dma_buf *buf;
	int ret;

	if (!rd || rd_size < sizeof(buf)) {
		dma_buf_ext_err("put invalid param 0x%lx %d\n", rd, rd_size);
		return -EFAULT;
	}
 
	ret = liblinux_pal_copy_from_caller(&buf, rd, sizeof(buf));
	if (ret) {
		dma_buf_ext_err("failed to copy dma buf ptr\n");
		return ret;
	}
 
	dma_buf_put(buf);
	return 0;
}

static struct ldk_sg_collect *sgt_collect(struct sg_table *sgt, int *collect_size)
{
	struct scatterlist *sgl;
	struct ldk_sg_collect *collect;
	int sgl_page_count;
	int idx = 0;

	sgl_page_count = sg_page_count(sgt);

	*collect_size = sizeof(struct ldk_sg_collect) + sizeof(struct scatterlist *) * sgl_page_count;
	collect = vmalloc(*collect_size);
	if (!collect) {
		dma_buf_ext_err("failed to alloc sg collect\n");
		return NULL;
	}

#ifdef DMA_BUF_EXT_DBG
	sg_print(sgt);
#endif

	sgl = sgt->sgl;
	collect->collect[idx++] = virt_to_phys(sgl);
	dma_buf_ext_dbg("get sg pa 0x%lx va 0x%lx\n", collect->collect[idx - 1], sgl);

	while(!sg_is_last(sgl)) {
		sgl++;
		if (!sg_is_chain(sgl)) {
			continue;
		}

		sgl = sg_chain_ptr(sgl);
		collect->collect[idx++] = virt_to_phys(sgl);
		dma_buf_ext_dbg("get sg pa 0x%lx va 0x%lx\n", collect->collect[idx - 1], sgl);
	}

	collect->nents = sgt->nents;
	return collect;
}

static int dma_buf_ext_get_sg_collect(void *rd, size_t rd_size, void *wr, size_t wr_size)
{
	struct dma_buf *buf;
	struct sg_table *sgt;
	
	struct ldk_sg_collect *collect;
	int collect_size;
	int ret;

	if (!rd || rd_size < sizeof(buf)) {
		dma_buf_ext_err("get iovec invalid param 0x%lx %d 0x%lx %d\n",
			rd, rd_size);
		return -EFAULT;
	}

	ret = liblinux_pal_copy_from_caller(&buf, rd, sizeof(buf));
	if (ret) {
		dma_buf_ext_err("failed to copy dma buf ptr %d\n", ret);
		return ret;
	}

	sgt = dmabuf_heap_sg_table_get(buf);
	if (!sgt) {
		dma_buf_ext_err("failed to get sg table\n");
		return -EINVAL;
	}

	ret = ldk_flush_cache(buf);
	if (ret) {
		dma_buf_ext_err("failed to flush cache %d\n", ret);
		return ret;
	}

	collect = sgt_collect(sgt, &collect_size);
	if (!collect) {
		return -ENOMEM;
	}

	if (!wr || wr_size < collect_size) {
		dma_buf_ext_err("invalid collect fill param 0x%lx %d\n", wr, wr_size);
		vfree(collect);
		return -EINVAL;
	}

	ret = liblinux_pal_copy_to_caller(wr, collect, collect_size);
	vfree(collect);

	return ret;
}

static int dma_buf_ext_handler(unsigned int cmd, void *rd, size_t rd_size, void *wr, size_t wr_size)
{
	int ret = -EINVAL;
 
	dma_buf_ext_dbg("handler %d 0x%lx %d 0x%lx %d\n", cmd, rd, rd_size, wr, wr_size);
 
	switch (PAL_PROXY_MINOR_CMD_OF(cmd)) {
	case DMA_BUF_EXT_CMD_GET:
		ret = dma_buf_ext_get(rd, rd_size, wr, wr_size);
		break;
	case DMA_BUF_EXT_CMD_PUT:
		ret = dma_buf_ext_put(rd, rd_size, wr, wr_size);
		break;
	case DMA_BUF_EXT_CMD_GET_SG_COLLECT:
		ret = dma_buf_ext_get_sg_collect(rd, rd_size, wr, wr_size);
		break;
	default:
		break;
	}
 
	return ret;
}
 
static int __init dma_buf_ext_init(void)
{
	return liblinux_pal_proxy_register(PAL_PROXY_MAJOR(DMA_BUF_EXT_MAJOR_CMD), dma_buf_ext_handler);
}
 
module_init(dma_buf_ext_init);