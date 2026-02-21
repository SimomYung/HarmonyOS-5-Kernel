/*
 *  ISP driver, hisp_memory.c
 *
 * Copyright (c) 2013 ISP Technologies CO., Ltd.
 *
 */

#include <platform_include/isp/linux/hisp_remoteproc.h>
#include <platform_include/isp/linux/hisp_mempool.h>
#include <platform_include/isp/linux/hoisp.h>
#include "hisp_internel.h"
#include <securec.h>
#include <linux/iommu/mm_iommu.h>
#include "isp_ddr_map.h"
#include <linux/dma-buf.h>
#ifdef CONFIG_ION
#include <linux/ion.h>
#include <linux/ion/mm_ion.h>
#elif defined CONFIG_DMABUF_HEAPS
#include <linux/dma-heap.h>
#include <linux/dmabuf/mm_dma_heap.h>
#endif
#include <linux/syscalls.h>
#include <linux/types.h>

#define ISP_MAX_BYTE_BIT_NUM     (31)
#define ISP_MEM_INDEX_OFFSET     (32)
#define ISP_SECMEM_INDEX_OFFSET  (30)
#define ISP_MEM_BITMAP_NUM \
	(DIV_ROUND_UP(ISP_MEM_INDEX_MAX, 8 * sizeof(unsigned long)))

#ifdef CONFIG_MM_LB
#define IOMMU_MASK (~(IOMMU_READ | IOMMU_WRITE | IOMMU_CACHE | IOMMU_NOEXEC \
		| IOMMU_MMIO | IOMMU_DEVICE | IOMMU_EXEC | IOMMU_PORT_MASK))
#else
#define IOMMU_MASK (~(IOMMU_READ | IOMMU_WRITE | IOMMU_CACHE | IOMMU_NOEXEC \
		| IOMMU_MMIO | IOMMU_DEVICE | IOMMU_EXEC))
#endif

unsigned long tatal_cpu_size = 0;
unsigned long tatal_core_size = 0;
unsigned long tatal_oisp_size = 0;

struct hisp_mem_pool_info_list_s {
	unsigned int index;
	struct hisp_mem_type type;
	unsigned long long iova;
	unsigned int iova_size;
	unsigned long long addr;
	unsigned int size;
	unsigned long cpu_addr;
	unsigned long cpu_size;
	unsigned long core_addr;
	unsigned long core_size;
	unsigned long oisp_addr;
	unsigned long oisp_size;
	void *sg_info;
	struct sg_table *table;
	struct dma_buf *buf;
	struct dma_buf_attachment *attachment;
	struct list_head list;
	struct sg_table *table_copy;
	struct dma_buf *buf_copy;
	struct dma_buf_attachment *attachment_copy;
};

struct hisp_mem_pool_info_s {
	unsigned long long addr;
	unsigned int size;
	unsigned int prot;
	unsigned int enable;
	unsigned int mem_map_flag;
	unsigned int memmap_type;
	unsigned long long iova_start;
	unsigned long long iova_end;
	unsigned long index_bitmap[ISP_MEM_BITMAP_NUM];
	union hisp_mem_usage_vote mem_use_vote[ISP_MEM_INDEX_MAX];
	struct hisp_mem_pool_info_list_s *node;
};

struct hisp_mem_pool_s {
	unsigned int count;
	struct hisp_mem_pool_info_s iova_pool_info;
	struct mutex hisp_map_mutex;
	struct mutex hisp_iova_mutex;
	struct mutex hisp_list_mutex;
	struct mutex hisp_mem_mutex;
};

struct hisp_separate_mem_info_s {
	size_t mem_iova;
	unsigned int cpu_size;
	struct dma_buf *dmabuf;
	struct dma_buf_attachment *attach;
	struct sg_table *sgt_cpu;
	unsigned int map_size;
};

enum hisp_mem_vote_e {
	KERNEL_VOTE = 0,
	ISPCPU_VOTE,
	VOTE_MAX,
};

static struct hisp_mem_pool_s g_hisp_mem_pool_info;

static inline unsigned int hisp_mem_get_index(struct hisp_mem_info_s *mem_info)
{
	return (unsigned int)(mem_info->cpu_addr >> ISP_MEM_INDEX_OFFSET);
}

static inline bool hisp_mem_index_valid(unsigned int index)
{
	/* Note: HAL regard all zero address as illegal addr, so from start 1 */
	return index >= 1 && index <= ISP_MEM_INDEX_MAX;
}

static unsigned int hisp_mem_index_alloc(void)
{
	unsigned int index;
	struct hisp_mem_pool_s *mem_pool = &g_hisp_mem_pool_info;
	struct hisp_mem_pool_info_s *pool_info = &mem_pool->iova_pool_info;

	if (pool_info == NULL) {
		pr_err("[%s] isp mem pool didn't creat\n", __func__);
		return (unsigned int)-1;
	}

	mutex_lock(&mem_pool->hisp_iova_mutex);
	index = (unsigned int)find_first_zero_bit(pool_info->index_bitmap, ISP_MEM_INDEX_MAX);
	if (index >= ISP_MEM_INDEX_MAX) {
		pr_err("[%s] alloc index failed\n", __func__);
		goto exit;
	}
	set_bit(index, pool_info->index_bitmap);
exit:
	mutex_unlock(&mem_pool->hisp_iova_mutex);
	/* Note: HAL regard all zero address as illegal addr, so from start 1 */
	return index + 1;
}

static int hisp_mem_index_free(unsigned int index)
{
	struct hisp_mem_pool_s *mem_pool = &g_hisp_mem_pool_info;
	struct hisp_mem_pool_info_s *pool_info;

	if (!hisp_mem_index_valid(index)) {
		pr_err("[%s] index.%u not in valid range[1, %d]!\n",
		       __func__, index, ISP_MEM_INDEX_MAX);
		return -EINVAL;
	}
	pool_info = &mem_pool->iova_pool_info;
	if (pool_info == NULL) {
		pr_err("[%s] isp mem pool didn't creat\n", __func__);
		return -EINVAL;
	}

	/* Note: HAL regard all zero address as illegal addr, so from start 1 */
	index -= 1;
	mutex_lock(&mem_pool->hisp_iova_mutex);
	if (test_bit((int)index, pool_info->index_bitmap) == 0)
		pr_warn("[%s] index.%u already free\n", __func__, index);
	clear_bit(index, pool_info->index_bitmap);
	mutex_unlock(&mem_pool->hisp_iova_mutex);

	return 0;
}

static int hisp_mem_useage_vote(unsigned int index, enum hisp_mem_vote_e type)
{
	struct hisp_mem_pool_s *mem_pool = &g_hisp_mem_pool_info;
	struct hisp_mem_pool_info_s *pool_info;

	/* Note: HAL regard all zero address as illegal addr, so from start 1 */
	if (!hisp_mem_index_valid(index)) {
		pr_err("[%s] index.%u not in valid range[1, %d]!\n",
		       __func__, index, ISP_MEM_INDEX_MAX);
		return -EINVAL;
	}
	pool_info = &mem_pool->iova_pool_info;
	if (pool_info == NULL) {
		pr_err("[%s] isp mem pool didn't creat\n", __func__);
		return -EINVAL;
	}

	index -= 1;
	mutex_lock(&mem_pool->hisp_iova_mutex);
	if (type == KERNEL_VOTE) {
		if (pool_info->mem_use_vote[index].bits.kernel_use == 1)
			pr_warn("[%s] index.%u kernel already voted\n", __func__, index);
		pool_info->mem_use_vote[index].bits.kernel_use = 1;
	} else if (type == ISPCPU_VOTE) {
		if (pool_info->mem_use_vote[index].bits.ispcpu_use == 1)
			pr_warn("[%s] index.%u kernel already voted\n", __func__, index);
		pool_info->mem_use_vote[index].bits.ispcpu_use = 1;
	} else {
		pr_err("[%s] no such type.%d\n", __func__, type);
		mutex_unlock(&mem_pool->hisp_iova_mutex);
		return -EINVAL;
	}
	mutex_unlock(&mem_pool->hisp_iova_mutex);

	return 0;
}

static int hisp_mem_useage_devote(unsigned int index, enum hisp_mem_vote_e type)
{
	struct hisp_mem_pool_s *mem_pool = &g_hisp_mem_pool_info;
	struct hisp_mem_pool_info_s *pool_info;

	/* Note: HAL regard all zero address as illegal addr, so from start 1 */
	if (!hisp_mem_index_valid(index)) {
		pr_err("[%s] index.%u not in valid range[1, %d]!\n",
		       __func__, index, ISP_MEM_INDEX_MAX);
		return -EINVAL;
	}
	pool_info = &mem_pool->iova_pool_info;
	if (pool_info == NULL) {
		pr_err("[%s] isp mem pool didn't creat\n", __func__);
		return -EINVAL;
	}

	index -= 1;
	mutex_lock(&mem_pool->hisp_iova_mutex);
	if (type == KERNEL_VOTE) {
		if (pool_info->mem_use_vote[index].bits.kernel_use == 0)
			pr_warn("[%s] index.%u kernel already devoted\n", __func__, index);
		pool_info->mem_use_vote[index].bits.kernel_use = 0;
	} else if (type == ISPCPU_VOTE) {
		if (pool_info->mem_use_vote[index].bits.ispcpu_use == 0)
			pr_warn("[%s] index.%u kernel already devoted\n", __func__, index);
		pool_info->mem_use_vote[index].bits.ispcpu_use = 0;
	} else {
		pr_err("[%s] no such type.%d\n", __func__, type);
		mutex_unlock(&mem_pool->hisp_iova_mutex);
		return -EINVAL;
	}
	mutex_unlock(&mem_pool->hisp_iova_mutex);

	return 0;
}

static int hisp_mem_type_check(struct hisp_mem_type type)
{
	unsigned int master;
	unsigned int mem_type;

	master = type.master;
	if ((master & ~(MASTER_MAX - 1)) || (master == 0)) {
		pr_err("[%s] master[%u] master invalid\n", __func__, master);
		return -EINVAL;
	}

	mem_type = type.mem_type;
	if (mem_type >= ISP_MEM_TYPE_MAX || mem_type == ISP_UNKNOW_MEM) {
		pr_err("[%s] mem_type[%u] master invalid\n", __func__, mem_type);
		return -EINVAL;
	}

	return 0;
}

static bool hisp_master_has(unsigned int actual, isp_master_e expect)
{
	bool res;

	if ((actual == 0) || (actual & ~(MASTER_MAX - 1)) != 0) {
		pr_err("[%s] actual master.%u invalid\n", __func__, actual);
		return false;
	}
	res = (actual & expect) ? true : false;
	return res;
}

static bool hisp_memtype_is(unsigned int actual, isp_memtype_e expect)
{
	bool res;

	if ((actual == 0) || (actual >= ISP_MEM_TYPE_MAX)) {
		pr_err("[%s] actual memtype.%u invalid\n", __func__, actual);
		return false;
	}
	res = (actual == expect) ? true : false;
	return res;
}

static bool hisp_type_is(struct hisp_mem_type actual,
			 struct hisp_mem_type expect)
{
	bool res;

	if (hisp_mem_type_check(actual) != 0) {
		pr_err("[%s] actual type.0x%x invalid\n",
			__func__, *(unsigned int *)(uintptr_t)&actual);
		return false;
	}

	res = (actual.master == expect.master) &&
	    (actual.mem_type == expect.mem_type) ? true : false;
	return res;
}

static unsigned int hisp_mem_get_prot(unsigned int master, unsigned int prot)
{
	(void)master;
	/* Now iommu_map_sg can't support more prot, only w+r */
#ifdef CONFIG_MM_LB
	prot = prot & (IOMMU_PORT_MASK | IOMMU_READ | IOMMU_WRITE | IOMMU_CACHE);
#else
	prot = prot & (IOMMU_READ | IOMMU_WRITE | IOMMU_CACHE);
#endif
	return prot;
}

static struct scatterlist *hisp_mem_get_sgl(struct hisp_mem_pool_info_list_s
					    *list_info)
{
	struct sg_table *sgl_table = NULL;
	struct scatterlist *sgl = NULL;

	if (list_info == NULL) {
		pr_err("[%s] list_info is NULL\n", __func__);
		return NULL;
	}

	sgl_table = list_info->table;
	if (sgl_table == NULL) {
		sgl = NULL;
		return sgl;
	}

	sgl = sgl_table->sgl;
	if (sgl == NULL) {
		pr_err("[%s] sgl is NULL\n", __func__);
		return NULL;
	}
	return sgl;
}

static struct device *hisp_mem_get_subdev(isp_master_e master)
{
	struct device *subdev = NULL;

	switch (master) {
	case ISP_CPU:
	case ISP_CPU | OISP_CPU:
		subdev = hisp_get_device();
		break;
	case ISP_CORE:
		subdev = hisp_get_ispfw_device();
		break;
#ifdef ISP_DMA_SPLITTED_SID
	case ISP_DMA:
		subdev = hisp_get_ispdma_device();
		break;
#endif
	case OISP_CPU: // fall through
	default:
		pr_err("[%s] not support now\n", __func__);
		break;
	}
	return subdev;
}

static void hisp_mem_set_mem_info(struct hisp_mem_pool_info_list_s *node,
				  struct hisp_mem_info_s *mem_info)
{
	unsigned long long addr = 0;

	addr |= (unsigned long long)node->index << ISP_MEM_INDEX_OFFSET;
	mem_info->cpu_addr = addr;
	mem_info->cpu_size = node->size;
	pr_info("[%s] type.0x%x size.0x%x index.0x%x\n", __func__,
		node->type, mem_info->cpu_size, node->index);
	pr_debug("[%s] addr_info.0x%llx\n", __func__, mem_info->cpu_addr);
}

static void hisp_dump_mem_info(struct hisp_mem_info_s *mem)
{
	if (mem == NULL)
		return;

	pr_debug("[%s] cpu addr_info=0x%lx, size=0x%x\n",
		 __func__, mem->cpu_addr, mem->cpu_size);
}

static unsigned int hisp_find_first_bit(unsigned int num, unsigned int align)
{
	unsigned int index = 0;

	for (index = ISP_MAX_BYTE_BIT_NUM; (1 << index) >= align; index--) {
		if (num & (1 << index))
			return index;
	}
	return 0;
}

static struct sg_table *hisp_get_fd_sg_table(int fd,
					     struct dma_buf **dmabuf,
					     struct dma_buf_attachment **attach)
{
	struct sg_table *sgt = NULL;
	struct device *dev = NULL;

	dev = hisp_get_device();
	if (dev == NULL) {
		pr_err("[%s] platform dev.NULL\n", __func__);
		return NULL;
	}

	*dmabuf = dma_buf_get(fd);
	if (IS_ERR_OR_NULL(*dmabuf)) {
		pr_err("[%s] failed: get dmabuf\n", __func__);
		return NULL;
	}

	*attach = dma_buf_attach(*dmabuf, dev);
	if (IS_ERR_OR_NULL(*attach)) {
		pr_err("[%s] failed: dma_buf_attach\n", __func__);
		goto err_put;
	}

	sgt = dma_buf_map_attachment(*attach, DMA_BIDIRECTIONAL);
	if (IS_ERR_OR_NULL(sgt)) {
		pr_err("[%s] failed: dma_buf_map_attachment\n", __func__);
		goto err_detach;
	}

	return sgt;

err_detach:
	dma_buf_detach(*dmabuf, *attach);
err_put:
	dma_buf_put(*dmabuf);

	return NULL;
}

#define ISP_BUF_ALIGN 0x1000

static int hisp_record_memory_map(struct hisp_mem_type type,
				  unsigned int prot,
				  struct hisp_mem_pool_info_list_s *info)
{
	struct hisp_msg_req_mem_record_s *hisp_mem_record =
		kzalloc(sizeof(struct hisp_msg_req_mem_record_s), GFP_KERNEL);
	if (hisp_mem_record == NULL) {
		pr_err("[%s] hisp_mem_record struct kmalloc failed\n", __func__);
		return -EINVAL;
	}

	info->index = hisp_mem_index_alloc();
	if (!hisp_mem_index_valid(info->index)) {
		pr_err("[%s] fail to alloc index\n", __func__);
		goto exit;
	}
	pr_info("[%s] alloc index %d\n", __func__, info->index);

	// record mem info, kernel use mem vote
	hisp_mem_record->state = HISP_MEM_MAP;
	hisp_mem_record->index = info->index;
	hisp_mem_record->size = info->size;
	hisp_mem_record->mem_info = *(unsigned int *)(uintptr_t)&type;
	hisp_mem_record->cpu_addr = info->cpu_addr;
	hisp_mem_record->cpu_size = info->cpu_size;
	hisp_mem_record->core_addr = (unsigned int)info->core_addr;
	hisp_mem_record->core_size = (unsigned int)info->core_size;
	hisp_mem_record->oisp_addr = (unsigned int)info->oisp_addr;
	hisp_mem_record->oisp_size = (unsigned int)info->oisp_addr;
	hisp_mem_record->prot = prot;
	if (hisp_mem_useage_vote(info->index, KERNEL_VOTE))
		goto free_index;
	if (hisp_msg_mem_record(hisp_mem_record))
		goto free_index;
	if (hisp_mem_useage_vote(info->index, ISPCPU_VOTE))
		goto free_index;

	kfree(hisp_mem_record);
	return 0;
free_index:
	hisp_mem_index_free(info->index);
	hisp_mem_useage_devote(info->index, ISPCPU_VOTE);
	hisp_mem_useage_devote(info->index, KERNEL_VOTE);
exit:
	kfree(hisp_mem_record);
	pr_err("[%s] record memory region failed, size=0x%x\n",
		__func__, info->size);
	return -ENOKEY;
}

#define HISP_SPI_CMD_1_TRIGGER_CACHE_FLUSH 1
static void hisp_unrecord_memory_map(struct hisp_mem_type type,
				     struct hisp_mem_pool_info_list_s *info)
{
	int ret;
	struct hisp_msg_req_mem_unrecord_s *hisp_mem_record =
		kzalloc(sizeof(struct hisp_msg_req_mem_unrecord_s), GFP_KERNEL);
	if (hisp_mem_record == NULL) {
		pr_err("[%s] hisp_mem_record struct kmalloc failed\n", __func__);
		return;
	}

	if (!hisp_mem_index_valid(info->index)) {
		pr_err("[%s] index.%d overflow\n", __func__, info->index);
		goto exit;
	}
	ret = hisp_mem_index_free(info->index);
	if (ret != 0) {
		pr_err("[%s] index.%d free fail\n", __func__, info->index);
		goto exit;
	}
	pr_info("[%s] free index %d\n", __func__, info->index);

	hisp_mem_record->state = HISP_MEM_UNUSED;
	hisp_mem_record->index = info->index;
	if (hisp_mem_useage_devote(info->index, KERNEL_VOTE))
		goto exit;
	if (hisp_ispcpu_is_powerup()) {
		if (hisp_msg_mem_unrecord(type, hisp_mem_record))
			goto exit;
	} else {
		pr_warn("[%s] kernel msg not send because ispcpu power off\n",
			__func__);
	}
	if (hisp_mem_useage_devote(info->index, ISPCPU_VOTE))
		goto exit;

	kfree(hisp_mem_record);
	return;
exit:
	kfree(hisp_mem_record);
	pr_err("[%s] unrecord memory region failed, size=0x%x\n", __func__,
	       info->size);
}

static int hisp_alloc_sgt_from_fd(unsigned int master,
				  int fd,
				  struct hisp_mem_pool_info_list_s *list_info)
{
	struct device *dev = NULL;
	struct dma_buf *buf = NULL;
	struct dma_buf_attachment *attach = NULL;
	struct sg_table *table = NULL;

	dev = hisp_mem_get_subdev(master);
	if (IS_ERR_OR_NULL(dev)) {
		pr_err("[%s] platform dev is NULL!\n", __func__);
		return -EINVAL;
	}

	buf = dma_buf_get(fd);
	if (IS_ERR_OR_NULL(buf)) {
		pr_err("[%s] fail to get dma buf", __func__);
		return -EINVAL;
	}

	attach = dma_buf_attach(buf, dev);
	if (IS_ERR_OR_NULL(attach)) {
		pr_err("[%s] fail to attach dma buf", __func__);
		goto err_attach_node_buf;
	}

	table = dma_buf_map_attachment(attach, DMA_BIDIRECTIONAL);
	if (IS_ERR_OR_NULL(table)) {
		pr_err("[%s] fail to map attachment", __func__);
		goto err_map_node_buf;
	}

	list_info->buf = buf;
	list_info->attachment = attach;
	list_info->table = table;

	list_info->buf_copy = buf;
	list_info->attachment_copy = attach;
	list_info->table_copy = table;

	return 0;
err_map_node_buf:
	dma_buf_detach(buf, attach);
err_attach_node_buf:
	dma_buf_put(buf);

	return -EINVAL;
}

static void hisp_free_sgt_from_fd(struct hisp_mem_pool_info_list_s *list_info)
{
	if (list_info->attachment == NULL ||
	    list_info->table == NULL || list_info->buf == NULL) {
		pr_err("[%s] attachment or table or buf is NULL\n", __func__);
		return;
	}

	dma_buf_unmap_attachment(list_info->attachment,
				 list_info->table, DMA_BIDIRECTIONAL);
	dma_buf_detach(list_info->buf, list_info->attachment);
	dma_buf_put(list_info->buf);

	list_info->table = NULL;
	list_info->buf = NULL;
	list_info->attachment = NULL;
	list_info->table_copy = NULL;
	list_info->buf_copy = NULL;
	list_info->attachment_copy = NULL;
}

static int hisp_alloc_sgt_from_size(size_t size,
				    struct hisp_mem_pool_info_list_s *list_info)
{
	struct sg_table *table = NULL;

	if (size == 0) {
		pr_err("[%s] size equal 0, just return", __func__);
		return -EINVAL;
	}

	table = hisp_alloc_dynmem_sg_table((unsigned long)size, &list_info->sg_info);
	if (IS_ERR_OR_NULL(table)) {
		pr_err("[%s] ISP_CPU fail to alloc_dynmem_sg_table", __func__);
		return -EINVAL;
	}

	list_info->table = table;
	list_info->table_copy = table;

	return 0;
}

static void hisp_free_sgt_from_size(struct hisp_mem_pool_info_list_s *list_info)
{
	if (list_info == NULL) {
		pr_err("[%s] invalid param list_info\n", __func__);
		return;
	}
	if (list_info->table != NULL) {
		hisp_free_dynmem_sg_table(&list_info->sg_info);
		list_info->table = NULL;
		list_info->table_copy = NULL;
	}
}

#ifdef ISP_DMA_SPLITTED_SID
static int hisp_memory_map_ispdma(struct scatterlist *sgl,
	size_t addr, size_t size, unsigned long prot)
{
	struct device *subdev = NULL;
	struct iommu_domain *domain = NULL;

	subdev = hisp_mem_get_subdev(ISP_DMA);
	if (subdev == NULL) {
		pr_err("%s: ispdma subdev is NULL!\n", __func__);
		return -EINVAL;
	}

	domain = iommu_get_domain_for_dev(subdev);
	if (domain == NULL) {
		pr_err("%s: ispdma domain is NULL!\n", __func__);
		return -EINVAL;
	}

	prot &= ~(IOMMU_PRIV | IOMMU_CACHE);

	size_t phy_len = iommu_map_sg(
		domain, addr, sgl, (unsigned int)sg_nents(sgl), (int)prot);
	if (phy_len != size) {
		pr_err("%s: iommu map sg failed! phy_len.0x%lx, size.0x%lx\n",
			__func__, phy_len, size);
		return -EINVAL;
	}

	return 0;
}

static int hisp_memory_unmap_ispdma(size_t addr, size_t size)
{
	struct device *subdev = NULL;

	subdev = hisp_mem_get_subdev(ISP_DMA);
	if (unlikely(subdev == NULL)) {
		pr_err("[%s] ispdma subdev is NULL!\n", __func__);
		return -EINVAL;
	}

	size_t len = mm_iommu_unmap_fast(subdev, addr, size);
	if (len != size) {
		pr_err("%s: iommu_unmap failed: len.0x%lx\n", __func__, len);
		return -EINVAL;
	}

	return 0;
}
#endif

static int hisp_memory_map_phy(isp_master_e master,
					struct hisp_map_info_s *map_info,
					unsigned long *addr)
{
	struct hisp_mem_pool_s *dev = &g_hisp_mem_pool_info;
	struct device *subdev = NULL;
	unsigned int prot;

	subdev = hisp_mem_get_subdev(master);
	if (subdev == NULL) {
		pr_err("[%s] platform dev is NULL!\n", __func__);
		return -EINVAL;
	}

	prot = hisp_mem_get_prot(master, map_info->prot);
	pr_debug("[%s] map size.0x%lx, prot.0x%x\n",
		 __func__, map_info->size, prot);

	mutex_lock(&dev->hisp_map_mutex);
	*addr = mm_iommu_map(subdev, map_info->paddr, map_info->size, (int)prot);
	if (*addr == 0) {
		pr_err("[%s] iommu map failed! size.0x%lx\n",
			__func__, map_info->size);
		mutex_unlock(&dev->hisp_map_mutex);
		return -EINVAL;
	}
	mutex_unlock(&dev->hisp_map_mutex);

	return 0;
}

static int hisp_memory_unmap_phy(isp_master_e master,
					  unsigned long addr,
					  unsigned long size)
{
	struct hisp_mem_pool_s *dev = &g_hisp_mem_pool_info;
	struct device *subdev = NULL;
	int ret;

	subdev = hisp_mem_get_subdev(master);
	if (subdev == NULL) {
		pr_err("[%s] platform dev is NULL!\n", __func__);
		return -EINVAL;
	}

	mutex_lock(&dev->hisp_map_mutex);
	ret = mm_iommu_unmap(subdev, addr, size);
	if (ret) {
		pr_err("[%s] iommu unmap failed! size.0x%x\n",
			__func__, size);
		mutex_unlock(&dev->hisp_map_mutex);
		return -EINVAL;
	}
	mutex_unlock(&dev->hisp_map_mutex);

	return 0;
}

static int hisp_memory_map(isp_master_e master,
			   unsigned int prot,
			   unsigned int size,
			   struct hisp_mem_pool_info_list_s *list_info)
{
	int ret;
	unsigned long addr = 0;
	struct hisp_mem_pool_s *pool_info = &g_hisp_mem_pool_info;
	struct device *subdev;
	size_t map_size = 0;
	struct scatterlist *sgl = NULL;

	if ((IOMMU_MASK & prot) != 0) {
		pr_err("[%s] iommu attr error.0x%x\n", __func__, prot);
		return -EINVAL;
	}

	if (size == 0) {
		pr_err("[%s] size is zero\n", __func__);
		return -EINVAL;
	}

	sgl = hisp_mem_get_sgl(list_info);
	if (sgl == NULL) {
		pr_err("[%s] hisp_mem_get_sgl fail\n", __func__);
		return -EINVAL;
	}

	if (master != OISP_CPU) {
		subdev = hisp_mem_get_subdev(master);
		if (subdev == NULL) {
			pr_err("[%s] platform subdev is NULL!\n", __func__);
			return -EINVAL;
		}

		prot = hisp_mem_get_prot(master, prot);
		pr_info("[%s] map size.0x%x, prot.0x%x, sgl.0x%p\n",
			__func__, size, prot, sgl);

		mutex_lock(&pool_info->hisp_map_mutex);
		addr = mm_iommu_map_sg(subdev, sgl, (int)prot, &map_size);
		if (addr == 0 || size != map_size) {
			pr_err
			    ("[%s] mm_iommu_map_sg failed! map_size.0x%lx, size.0x%lx\n",
			     __func__, map_size, size);
			mutex_unlock(&pool_info->hisp_map_mutex);
			return -ENOMEM;
		}

#ifdef ISP_DMA_SPLITTED_SID
		if (master == ISP_CPU) {
			ret = hisp_memory_map_ispdma(sgl, addr, size, prot);
			if (ret != 0) {
				pr_err("[%s] hisp_memory_map_ispdma failed! size.0x%lx\n", __func__, size);
				mutex_unlock(&pool_info->hisp_map_mutex);
				return -ENOMEM;
			}
		}
#endif

		mutex_unlock(&pool_info->hisp_map_mutex);
	} else {
		mutex_lock(&pool_info->hisp_map_mutex);
		if (size == OISP_MSG_MEM_SIZE) {
			pr_info("[%s] hoisp_msg_mem_map start\n", __func__);
			ret = hoisp_msg_mem_map(list_info->table);
			if (ret) {
				pr_err("[%s] hoisp_msg_mem_map fail\n", __func__);
				mutex_unlock(&pool_info->hisp_map_mutex);
				return -EINVAL;
			}
		} else {
			pr_info("[%s] hoisp_msg_mem_map start\n", __func__);
			ret = hoisp_shared_mem_map(sgl, list_info->cpu_addr, list_info->cpu_size);
			if (ret) {
				pr_err("[%s] hoisp_shared_mem_map fail\n", __func__);
				mutex_unlock(&pool_info->hisp_map_mutex);
				return -EINVAL;
			}
		}
		mutex_unlock(&pool_info->hisp_map_mutex);
	}

	pr_info("[%s] mapped size.0x%x, master.0x%x\n",
		__func__, size, master);

	switch (master) {
	case ISP_CPU:
		list_info->cpu_addr = addr;
		list_info->cpu_size = size;
		break;
	case ISP_CORE:
		list_info->core_addr = addr;
		list_info->core_size = size;
		break;
	case OISP_CPU:
		list_info->oisp_addr = addr;
		list_info->oisp_size = size;
		break;
	default:
		break;
	}
	list_info->size = size;

	return 0;
}

static int hisp_memory_unmap(isp_master_e master,
			     unsigned long long addr,
			     unsigned long size,
			     struct hisp_mem_pool_info_list_s *list_info)
{
	struct device *subdev = NULL;
	struct hisp_mem_pool_s *pool_info = &g_hisp_mem_pool_info;
	struct scatterlist *sgl = NULL;
	int ret;

	pr_info("func---[%s],line---[%d]---master: 0x%x, unmap size: 0x%lx\n",
 		__func__, __LINE__, master, size);

	if (unlikely(list_info == NULL)) {
		pr_err("[%s] list_info is NULL\n", __func__);
		return -EINVAL;
	}

	if (master != OISP_CPU) {
		subdev = hisp_mem_get_subdev(master);
		if (unlikely(subdev == NULL)) {
			pr_err("[%s] subdev is NULL!\n", __func__);
			return -EINVAL;
		}

		sgl = hisp_mem_get_sgl(list_info);
		if (sgl == NULL) {
			pr_err("[%s] hisp_mem_get_sgl fail\n", __func__);
			return -EINVAL;
		}

		if (list_info->table_copy != list_info->table) {
			pr_err("func---[%s],line---[%d]---Failed: table pointer not equal\n", __func__, __LINE__);
		}

		if (list_info->table_copy->sgl != sgl) {
			pr_err("func---[%s],line---[%d]---Failed: sgl pointer not equal\n", __func__, __LINE__);
		}

		if (list_info->buf_copy != list_info->buf) {
			pr_err("func---[%s],line---[%d]---Failed: buf pointer not equal\n", __func__, __LINE__);
		}

		if (list_info->attachment_copy != list_info->attachment) {
			pr_err("func---[%s],line---[%d]---Failed: attachment pointer not equal\n", __func__, __LINE__);
		}

		pr_info("[%s] unmap size.0x%lx, master.0x%x\n",
			__func__, size, master);

		mutex_lock(&pool_info->hisp_map_mutex);
#ifdef ISP_DMA_SPLITTED_SID
		if (master == ISP_CPU) {
			ret = hisp_memory_unmap_ispdma(addr, size);
			if (ret != 0) {
				pr_err("[%s] hisp_mem_get_sgl fail\n", __func__);
				goto err_unmap;
			}
		}
#endif

#ifdef CONFIG_HISP_FFA_SUPPORT
		if (hisp_ispcpu_is_powerup() && (master == ISP_CPU)) {
			ret = hisp_ffa_send_spi(HISP_SPI_CMD_1_TRIGGER_CACHE_FLUSH, 0);
			if (ret != 0) {
				pr_err("[%s] hisp_ffa_send_spi trigger cache flush fail, 0x%x\n", __func__, ret);
			}
		}
#endif

		ret = mm_iommu_unmap_sg(subdev, sgl, addr);
		if (ret != 0) {
		pr_err("[%s] mm_iommu_unmap_sg failed\n", __func__);
			goto err_unmap;
		}

#ifdef CONFIG_HISP_FFA_SUPPORT
		if (hisp_ispcpu_is_powerup() && (master == ISP_CPU)) {
			ret = hisp_ffa_send_spi(HISP_SPI_CMD_1_TRIGGER_CACHE_FLUSH, 0);
			if (ret != 0) {
				pr_err("[%s] hisp_ffa_send_spi trigger cache flush fail, 0x%x\n", __func__, ret);
			}
		}
#endif

		mutex_unlock(&pool_info->hisp_map_mutex);
	} else {
		mutex_lock(&pool_info->hisp_map_mutex);
		if (size == OISP_MSG_MEM_SIZE) {
			ret = hoisp_msg_mem_unmap();
			if (ret != 0) {
				pr_err("%s: hoisp_msg_mem_unmap failed\n", __func__);
				goto err_unmap;
			}
		} else {
			ret = hoisp_shared_mem_unmap();
			if (ret != 0) {
				pr_err("%s: hoisp_shared_mem_unmap failed\n", __func__);
				goto err_unmap;
			}
		}
		mutex_unlock(&pool_info->hisp_map_mutex);
	}
	return 0;
err_unmap:
	mutex_unlock(&pool_info->hisp_map_mutex);
	return ret;
}

static int hisp_mem_param_check(struct hisp_mem_type type,
				struct hisp_map_info_s *info)
{
	int ret;

	if (info == NULL) {
		pr_err("[%s] invalid info(NULL)\n", __func__);
		return -EINVAL;
	}

	pr_debug("[%s] type.0x%x size.0x%x prot.0x%x\n", __func__,
		*(unsigned int *)(uintptr_t)&type, info->size, info->prot);

	ret = hisp_mem_type_check(type);
	if (ret != 0) {
		pr_err("[%s] type[%x] mem type check failed\n",
		       __func__, *((unsigned int *)(uintptr_t)&type));
		return -EINVAL;
	}

	if (info->size >= ISP_MAP_MAX_SIZE) {
		pr_err("[%s] type[0x%x] size[0x%x], map size over 0x%x\n",
		       __func__, *((unsigned int *)(uintptr_t)&type),
		       info->size, ISP_MAP_MAX_SIZE);
		return -EINVAL;
	}

	return 0;
}

static int hisp_memmap_share(struct hisp_mem_type type,
			     struct hisp_map_info_s *map_info,
			     struct hisp_mem_info_s *mem_info,
			     struct hisp_mem_pool_info_list_s *list_info)
{
	int ret;
	unsigned int master = type.master;
	unsigned int prot = map_info->prot;
	unsigned int size = map_info->size;

	pr_info("func---[%s],line---[%d]---master: 0x%x, prot: 0x%x, size: 0x%x\n",
 		__func__, __LINE__, master, prot, size);

	ret = hisp_alloc_sgt_from_fd(master, map_info->fd, list_info);
	if (ret != 0) {
		pr_err("[%s] type[0x%x] hisp_alloc_sgt_from_fd failed\n",
		       __func__, *((unsigned int *)(uintptr_t)&type));
		return ret;
	}

	if (hisp_master_has(master, ISP_CPU)) {
		ret = hisp_memory_map(ISP_CPU, prot, size, list_info);
		if (ret != 0) {
			pr_err("[%s] type[0x%x] ISP_CPU map failed\n",
			       __func__, *((unsigned int *)(uintptr_t)&type));
			goto err_map_fd;
		}
	}

	if (hisp_master_has(master, ISP_CORE)) {
		ret =
		    hisp_memory_map(ISP_CORE, prot, size, list_info);
		if (ret != 0) {
			pr_err("[%s] type[0x%x] ISP_CORE map failed\n",
			       __func__, *((unsigned int *)(uintptr_t)&type));
			goto err_map_iova_cpu;
		}
	}

	if (hisp_master_has(master, OISP_CPU)) {
		ret =
		    hisp_memory_map(OISP_CPU, prot, size, list_info);
		if (ret != 0) {
			pr_err("[%s] type[0x%x] OISP_CPU map failed\n",
			       __func__, *((unsigned int *)(uintptr_t)&type));
			goto err_map_iova_core;
		}
	}

	prot |= IOMMU_RSV_ISP_SHARE_MEM;
	ret = hisp_record_memory_map(type, prot, list_info);
	if (ret != 0) {
		pr_err("[%s] record memory failed\n", __func__);
		goto err_map_record;
	}

	hisp_mem_set_mem_info(list_info, mem_info);

	return 0;

err_map_record:
	if (hisp_master_has(type.master, OISP_CPU))
		hisp_memory_unmap(OISP_CPU, list_info->oisp_addr,
				  list_info->oisp_size, list_info);
err_map_iova_core:
	if (hisp_master_has(type.master, ISP_CORE))
		hisp_memory_unmap(ISP_CORE, list_info->core_addr,
				  (unsigned int)list_info->core_size, list_info);
err_map_iova_cpu:
	if (hisp_master_has(type.master, ISP_CPU))
		hisp_memory_unmap(ISP_CPU, list_info->cpu_addr,
				  (unsigned int)list_info->cpu_size, list_info);
err_map_fd:
	hisp_free_sgt_from_fd(list_info);
	return -EINVAL;
}

static void hisp_memunmap_share(struct hisp_mem_type type,
				struct hisp_mem_info_s *mem_info,
				struct hisp_mem_pool_info_list_s *list_node)
{
	struct hisp_mem_pool_s *mem_pool = &g_hisp_mem_pool_info;
	struct hisp_mem_pool_info_list_s *node = NULL;
	struct hisp_mem_pool_info_list_s *node_temp = NULL;

	unsigned int index = hisp_mem_get_index(mem_info);

	mutex_lock(&mem_pool->hisp_list_mutex);
	list_for_each_entry_safe(node, node_temp, &list_node->list, list) {
		if (node->index != index)
			continue;
		if (hisp_type_is(node->type, type)) {
			pr_info("[%s] [ISP Clean Mem] type[0x%x]\n",
				__func__, *(unsigned int *)(uintptr_t)&type);
			if (!hisp_memtype_is(type.mem_type, ISP_SHARE_MEM))
				continue;

			hisp_unrecord_memory_map(type, node);

			if (hisp_master_has(type.master, ISP_CPU)) {
				tatal_cpu_size -= node->cpu_size;
				pr_info("func---[%s],line---[%d]---total cpu size: 0x%x, cpu size: 0x%x\n",
					__func__, __LINE__, tatal_cpu_size, node->cpu_size);
				hisp_memory_unmap(ISP_CPU, node->cpu_addr,
						  (unsigned int)node->cpu_size, node);
			}
			if (hisp_master_has(type.master, ISP_CORE)) {
				tatal_core_size -= node->core_size;
				pr_info("func---[%s],line---[%d]---total core size: 0x%x, core size: 0x%x\n",
					__func__, __LINE__, tatal_core_size, node->core_size);
				hisp_memory_unmap(ISP_CORE, node->core_addr,
						  (unsigned int)node->core_size, node);
			}
			if (hisp_master_has(type.master, OISP_CPU)) {
				tatal_oisp_size -= node->oisp_size;
				pr_info("func---[%s],line---[%d]---total oisp size: 0x%x, oisp size: 0x%x\n",
					__func__, __LINE__, tatal_oisp_size, node->oisp_size);
				hisp_memory_unmap(OISP_CPU, node->oisp_addr,
						  (unsigned int)node->oisp_size, node);
			}

			hisp_free_sgt_from_fd(node);

			list_del(&node->list);
			kfree(node);
			goto exit;
		}
	}
	pr_warn("[%s] not found record mem info: type.0x%x\n",
			__func__, *((unsigned int *)(uintptr_t)&type));
exit:
	mutex_unlock(&mem_pool->hisp_list_mutex);
}

static int hisp_memmap_private(struct hisp_mem_type type,
			       struct hisp_map_info_s *map_info,
			       struct hisp_mem_info_s *mem_info,
			       struct hisp_mem_pool_info_list_s *list_info)
{
	int ret;
	unsigned int master = type.master;
	unsigned int prot = map_info->prot;
	unsigned int size = map_info->size;

	pr_info("func---[%s],line---[%d]---master: 0x%x, prot: 0x%x, size: 0x%x\n",
 		__func__, __LINE__, master, prot, size);

	ret = hisp_alloc_sgt_from_size(size, list_info);
	if (ret != 0) {
		pr_err("[%s] type[0x%x] hisp_alloc_sgt_from_size failed\n",
		       __func__, *((unsigned int *)(uintptr_t)&type));
		return -ENOMEM;
	}

	if (hisp_master_has(master, ISP_CPU)) {
		ret = hisp_memory_map(ISP_CPU, prot, map_info->size, list_info);
		if (ret != 0) {
			pr_err("[%s] type[0x%x] ISP_CPU map failed\n",
			       __func__, *((unsigned int *)(uintptr_t)&type));
			goto err_map_iova;
		}
	}

	if (hisp_master_has(master, ISP_CORE)) {
		ret =
		    hisp_memory_map(ISP_CORE, prot, map_info->size, list_info);
		if (ret != 0) {
			pr_err("[%s] type[0x%x] ISP_CORE map failed\n",
			       __func__, *((unsigned int *)(uintptr_t)&type));
			goto err_map_iova_cpu;
		}
	}

	if (hisp_master_has(master, OISP_CPU)) {
		ret =
		    hisp_memory_map(OISP_CPU, prot, map_info->size, list_info);
		if (ret != 0) {
			pr_err("[%s] type[0x%x] OISP_CPU map failed\n",
			       __func__, *((unsigned int *)(uintptr_t)&type));
			goto err_map_iova_core;
		}
	}

	prot |= IOMMU_RSV_ISP_PRIVATE_MEM;
	ret = hisp_record_memory_map(type, prot, list_info);
	if (ret != 0) {
		pr_err("[%s] record memory failed\n", __func__);
		goto err_map_record;
	}

	hisp_mem_set_mem_info(list_info, mem_info);

	return 0;

err_map_record:
	if (hisp_master_has(type.master, OISP_CPU))
		hisp_memory_unmap(OISP_CPU, list_info->oisp_addr,
				  list_info->oisp_size, list_info);
err_map_iova_core:
	if (hisp_master_has(type.master, ISP_CORE))
		hisp_memory_unmap(ISP_CORE, list_info->core_addr,
				  (unsigned int)list_info->core_size, list_info);
err_map_iova_cpu:
	if (hisp_master_has(type.master, ISP_CPU))
		hisp_memory_unmap(ISP_CPU, list_info->cpu_addr,
				  (unsigned int)list_info->cpu_size, list_info);

err_map_iova:
	hisp_free_sgt_from_size(list_info);
	return ret;
}

static void hisp_memunmap_private(struct hisp_mem_type type,
				  struct hisp_mem_info_s *mem_info,
				  struct hisp_mem_pool_info_list_s *list_node)
{
	struct hisp_mem_pool_s *mem_pool = &g_hisp_mem_pool_info;
	struct hisp_mem_pool_info_list_s *node = NULL;
	struct hisp_mem_pool_info_list_s *node_temp = NULL;

	unsigned int index = hisp_mem_get_index(mem_info);

	mutex_lock(&mem_pool->hisp_list_mutex);
	list_for_each_entry_safe(node, node_temp, &list_node->list, list) {
		if (node->index != index)
			continue;
		if (hisp_type_is(node->type, type)) {
			pr_info("[%s] [ISP Clean Mem] type[0x%x]\n",
				__func__, *((unsigned int *)(uintptr_t)&type));
			if (!hisp_memtype_is(type.mem_type, ISP_PRIVATE_MEM))
				continue;

			hisp_unrecord_memory_map(type, node);

			if (hisp_master_has(type.master, ISP_CPU)) {
				tatal_cpu_size -= node->cpu_size;
				pr_info("func---[%s],line---[%d]---total cpu size: 0x%x, cpu size: 0x%x\n",
					__func__, __LINE__, tatal_cpu_size, node->cpu_size);
				hisp_memory_unmap(ISP_CPU, node->cpu_addr,
						  (unsigned int)node->cpu_size, node);
			}
			if (hisp_master_has(type.master, ISP_CORE)) {
				tatal_core_size -= node->core_size;
				pr_info("func---[%s],line---[%d]---total core size: 0x%x, core size: 0x%x\n",
					__func__, __LINE__, tatal_core_size, node->core_size);
				hisp_memory_unmap(ISP_CORE, node->core_addr,
						  (unsigned int)node->core_size, node);
			}
			if (hisp_master_has(type.master, OISP_CPU)) {
				tatal_oisp_size -= node->oisp_size;
				pr_info("func---[%s],line---[%d]---total oisp size: 0x%x, oisp size: 0x%x\n",
					__func__, __LINE__, tatal_oisp_size, node->oisp_size);
				hisp_memory_unmap(OISP_CPU, node->oisp_addr,
						  (unsigned int)node->oisp_size, node);
			}

			hisp_free_sgt_from_size(node);

			list_del(&node->list);
			kfree(node);
			goto exit;
		}
	}
	pr_warn("[%s] not found record mem info: type.0x%x\n",
			__func__, *((unsigned int *)(uintptr_t)&type));
exit:
	mutex_unlock(&mem_pool->hisp_list_mutex);
}

static int hisp_memmap_phyaddr(struct hisp_mem_type type,
			       struct hisp_map_info_s *map_info,
			       struct hisp_mem_info_s *mem_info,
			       struct hisp_mem_pool_info_list_s *list_info)
{
	int ret;

	unsigned int master = type.master;

	if (map_info->prot != IOMMU_READ) {
		pr_err("[%s] paddr not only read attr\n", __func__);
		return -EINVAL;
	}

	if ((map_info->prot & IOMMU_MASK) != 0) {
		pr_err("([%s] iommu attr error.0x%x\n", __func__, map_info->prot);
		return -EINVAL;
	}

	if (map_info->paddr == 0) {
		pr_err("[%s] paddr is invalid!\n", __func__);
		return -EINVAL;
	}

	if (hisp_master_has(master, ISP_CPU)) {
		list_info->cpu_size = map_info->size;
		ret = hisp_memory_map_phy(ISP_CPU, map_info, &list_info->cpu_addr);
		if (ret != 0) {
			pr_err
			    ("[%s] type[0x%x] ISP_CPU hisp_memory_map_phy failed",
			     __func__, *((unsigned int *)(uintptr_t)&type));
			goto err_map_iova_cpu;
		}
	}

	if (hisp_master_has(master, ISP_CORE)) {
		list_info->core_size = map_info->size;
		ret = hisp_memory_map_phy(ISP_CORE, map_info, &list_info->core_addr);
		if (ret != 0) {
			pr_err
			    ("[%s] type[0x%x] ISP_CORE hisp_memory_map_phy failed",
			     __func__, *((unsigned int *)(uintptr_t)&type));
			goto err_map_iova_core;
		}
	}

	if (hisp_master_has(master, OISP_CPU)) {
		list_info->oisp_size = map_info->size;
		ret = hisp_memory_map_phy(OISP_CPU, map_info, &list_info->oisp_addr);
		if (ret != 0) {
			pr_err
			    ("[%s] type[0x%x] OISP_CPU hisp_memory_map_phy failed",
			     __func__, *((unsigned int *)(uintptr_t)&type));
			goto err_map_iova_oisp;
		}
	}

	list_info->size = map_info->size;

	ret = hisp_record_memory_map(type, map_info->prot, list_info);
	if (ret != 0) {
		pr_err("[%s] record memory failed\n", __func__);
		goto err_map_record;
	}
	hisp_mem_set_mem_info(list_info, mem_info);

	return 0;

err_map_record:
	if (hisp_master_has(type.master, OISP_CPU))
		hisp_memory_unmap(OISP_CPU, list_info->oisp_addr,
				  list_info->oisp_size, list_info);
err_map_iova_oisp:
	if (hisp_master_has(type.master, ISP_CORE))
		(void)hisp_memory_unmap_phy(ISP_CORE,
				list_info->core_addr, list_info->core_size);
err_map_iova_core:
	if (hisp_master_has(type.master, ISP_CPU))
		(void)hisp_memory_unmap_phy(ISP_CPU,
				list_info->cpu_addr, list_info->cpu_size);
err_map_iova_cpu:
	return -EINVAL;
}

static void hisp_memunmap_phyaddr(struct hisp_mem_type type,
				  struct hisp_mem_info_s *mem_info,
				  struct hisp_mem_pool_info_list_s *list_node)
{
	struct hisp_mem_pool_s *mem_pool = &g_hisp_mem_pool_info;
	struct hisp_mem_pool_info_list_s *node = NULL;
	struct hisp_mem_pool_info_list_s *node_temp = NULL;

	unsigned int index = hisp_mem_get_index(mem_info);

	mutex_lock(&mem_pool->hisp_list_mutex);
	list_for_each_entry_safe(node, node_temp, &list_node->list, list) {
		if (node->index != index)
			continue;
		if (hisp_type_is(node->type, type)) {
			pr_info("[%s] [ISP Mem] type[0x%x]\n", __func__,
				*((unsigned int *)(uintptr_t)&type));
			if (!hisp_memtype_is(type.mem_type, ISP_PHY_MEM))
				continue;

			hisp_unrecord_memory_map(type, node);

			if (hisp_master_has(type.master, ISP_CPU)) {
				tatal_cpu_size -= node->cpu_size;
				pr_info("func---[%s],line---[%d]---total cpu size: 0x%x, cpu size: 0x%x\n",
					__func__, __LINE__, tatal_cpu_size, node->cpu_size);
				hisp_memory_unmap_phy(ISP_CPU,
					node->cpu_addr, node->cpu_size);
			}
			if (hisp_master_has(type.master, ISP_CORE)) {
				tatal_core_size -= node->core_size;
				pr_info("func---[%s],line---[%d]---total core size: 0x%x, core size: 0x%x\n",
					__func__, __LINE__, tatal_core_size, node->core_size);
				hisp_memory_unmap_phy(ISP_CORE,
					node->core_addr, node->core_size);
			}
			if (hisp_master_has(type.master, OISP_CPU)) {
				tatal_oisp_size -= node->oisp_size;
				pr_info("func---[%s],line---[%d]---total oisp size: 0x%x, oisp size: 0x%x\n",
					__func__, __LINE__, tatal_oisp_size, node->oisp_size);
				hisp_memory_unmap_phy(OISP_CPU,
					node->oisp_addr, node->oisp_size);
			}

			list_del(&node->list);
			kfree(node);
			goto exit;
		}
	}
	pr_warn("[%s] not found record mem info: type.0x%x\n",
			__func__, *((unsigned int *)(uintptr_t)&type));
exit:
	mutex_unlock(&mem_pool->hisp_list_mutex);
}

int hisp_mem_map_addr(struct hisp_mem_type type,
		      struct hisp_map_info_s *info, struct hisp_mem_info_s *mem)
{
	struct hisp_mem_pool_s *mem_pool = &g_hisp_mem_pool_info;
	struct hisp_mem_pool_info_s *pool_info = NULL;
	struct hisp_mem_pool_info_list_s *list_node = NULL;
	struct hisp_mem_pool_info_list_s *list_info = NULL;
	int ret;
	unsigned int mem_type = type.mem_type;

	pr_info("[%s] type[0x%x] map addr start\n",
		__func__, *((unsigned int *)(uintptr_t)&type));
	ret = hisp_mem_param_check(type, info);
	if (ret != 0) {
		pr_err("[%s] invalid param\n", __func__);
		return -EINVAL;
	}

	pool_info = &mem_pool->iova_pool_info;
	if (pool_info == NULL) {
		pr_err("[%s] isp mem pool didn't creat\n", __func__);
		return -EINVAL;
	}

	list_node = pool_info->node;
	if (list_node == NULL) {
		pr_err("[%s] iova mem pool list_node is NULL\n", __func__);
		return -EINVAL;
	}

	list_info =
	    kzalloc(sizeof(struct hisp_mem_pool_info_list_s), GFP_KERNEL);
	if (!list_info) {
		pr_err("[%s] fail to alloc list_info", __func__);
		return -ENOMEM;
	}
	list_info->type = type;

	mutex_lock(&mem_pool->hisp_mem_mutex);
	switch (mem_type) {
	case ISP_SHARE_MEM: // For ispcpu
		ret = hisp_memmap_share(type, info, mem, list_info);
		if (ret != 0) {
			pr_err("[%s] mem_type.%u, hisp_memmap_share failed",
			       __func__, mem_type);
			goto MAP_ADDR_ERR;
		}
		break;
	case ISP_PRIVATE_MEM: // For ispcpu & ispcore
		ret = hisp_memmap_private(type, info, mem, list_info);
		if (ret != 0) {
			pr_err("[%s] mem_type.%u, hisp_memmap_private failed",
			       __func__, mem_type);
			goto MAP_ADDR_ERR;
		}
		break;
	case ISP_PHY_MEM: // For ispcpu
		ret = hisp_memmap_phyaddr(type, info, mem, list_info);
		if (ret != 0) {
			pr_err("[%s] mem_type.%u, hisp_memmap_phyaddr failed",
			       __func__, mem_type);
			goto MAP_ADDR_ERR;
		}
		break;
	case ISP_UNKNOW_MEM: // fall through
	default:
		ret = -EINVAL;
		pr_err("[%s] type[0x%x] unsupported", __func__,
		       *((unsigned int *)(uintptr_t)&type));
		goto MAP_ADDR_ERR;
	}

	pr_info("func---[%s],line---[%d]---map_info.fd: 0x%x, map_info.size: 0x%x, map_info.prot: 0x%x\n",
 		__func__, __LINE__, info->fd, info->size, info->prot);

	pr_info("func---[%s],line---[%d]---master: 0x%x, mem_type: 0x%x\n",
 		__func__, __LINE__, type.master, type.mem_type);

	if (hisp_master_has(type.master, ISP_CPU)) {
		tatal_cpu_size += list_info->cpu_size;
		pr_info("func---[%s],line---[%d]---total cpu size: 0x%x, cpu size: 0x%x\n",
 			__func__, __LINE__, tatal_cpu_size, list_info->cpu_size);
	}

	if (hisp_master_has(type.master, ISP_CORE)) {
		tatal_core_size += list_info->core_size;
		pr_info("func---[%s],line---[%d]---total core size: 0x%x, core size: 0x%x\n",
 			__func__, __LINE__, tatal_core_size, list_info->core_size);
	}

	if (hisp_master_has(type.master, OISP_CPU)) {
		tatal_oisp_size += list_info->oisp_size;
		pr_info("func---[%s],line---[%d]---total oisp size: 0x%x, oisp size: 0x%x\n",
 			__func__, __LINE__, tatal_oisp_size, list_info->oisp_size);
	}

	mutex_unlock(&mem_pool->hisp_mem_mutex);

	mutex_lock(&mem_pool->hisp_list_mutex);
	list_add_tail(&list_info->list, &list_node->list);
	mutex_unlock(&mem_pool->hisp_list_mutex);

	hisp_dump_mem_info(mem);
	pr_info("[%s] type[0x%x] size.0x%x map addr end\n",
		__func__, *((unsigned int *)(uintptr_t)&type), info->size);

	return 0;

MAP_ADDR_ERR:
	mutex_unlock(&mem_pool->hisp_mem_mutex);
	kfree(list_info);
	return ret;
}
EXPORT_SYMBOL(hisp_mem_map_addr);

int hisp_mem_unmap_addr(struct hisp_mem_type type,
			struct hisp_mem_info_s mem_info)
{
	struct hisp_mem_pool_s *mem_pool = &g_hisp_mem_pool_info;
	struct hisp_mem_pool_info_s *pool_info = NULL;
	struct hisp_mem_pool_info_list_s *list_node = NULL;
	int ret;

	pr_info("func---[%s],line---[%d]---master: 0x%x, mem_type: 0x%x, cpu_size: 0x%x\n",
 		__func__, __LINE__, type.master, type.mem_type, mem_info.cpu_size);

	ret = hisp_mem_type_check(type);
	if (ret != 0) {
		pr_err("[%s] param type.0x%x check failed\n",
		       __func__, *((unsigned int *)(uintptr_t)&type));
		return -EINVAL;
	}

	pr_info("[%s] type[0x%x] unmap addr start\n", __func__,
		*((unsigned int *)(uintptr_t)&type));
	pool_info = &mem_pool->iova_pool_info;
	if (pool_info == NULL) {
		pr_err("[%s] isp mem pool didn't creat\n", __func__);
		return -EINVAL;
	}

	list_node = pool_info->node;
	if (list_node == NULL) {
		pr_err("[%s] iova mem pool is NULL\n", __func__);
		return -EINVAL;
	}

	mutex_lock(&mem_pool->hisp_mem_mutex);
	switch (type.mem_type) {
	case ISP_SHARE_MEM: // For ispcpu
		hisp_memunmap_share(type, &mem_info, list_node);
		break;
	case ISP_PRIVATE_MEM: // For ispcpu & ispcore
		hisp_memunmap_private(type, &mem_info, list_node);
		break;
	case ISP_PHY_MEM: // For ispcpu
		hisp_memunmap_phyaddr(type, &mem_info, list_node);
		break;
	case ISP_UNKNOW_MEM: // fall through
	default:
		pr_err("[%s] type[0x%x] unsupported\n", __func__,
		       *((unsigned int *)(uintptr_t)&type));
		goto UNMAP_ADDR_ERR;
	}
	mutex_unlock(&mem_pool->hisp_mem_mutex);

	pr_info("[%s] type[0x%x] unmap addr end\n",
		__func__, *((unsigned int *)(uintptr_t)&type));
	return 0;
UNMAP_ADDR_ERR:
	mutex_unlock(&mem_pool->hisp_mem_mutex);
	return -ENOMEM;
}
EXPORT_SYMBOL(hisp_mem_unmap_addr);

int hisp_mem_map_addr_sec(unsigned int pool,
			  struct hisp_map_info_s *info,
			  struct hisp_mem_info_s *mem)
{
	unsigned long long addr = 0;
    	int ret = -1;

	ret = hisp_secmem_ca_map(pool, info->fd, info->size);
	if (ret != 0) {
		pr_err("%s: hisp_secmem_ca_map failed", __func__);
		return -ENOMEM;
	}
	addr |= (pool + 1ULL) << ISP_SECMEM_INDEX_OFFSET; // index from 1 to 3
	mem->cpu_addr = addr;
	mem->cpu_size = info->size;

	return 0;
}
EXPORT_SYMBOL(hisp_mem_map_addr_sec);

int hisp_mem_unmap_addr_sec(unsigned int pool)
{
	int ret = -1;

	ret = hisp_secmem_ca_unmap(pool);
	if (ret != 0)
		pr_err("%s: hisp_secmem_ca_unmap failed", __func__);

	return 0;
}
EXPORT_SYMBOL(hisp_mem_unmap_addr_sec);

void hisp_mem_list_clean(void)
{
	struct hisp_mem_pool_s *mem_pool = &g_hisp_mem_pool_info;
	struct hisp_mem_pool_info_s *pool_info = NULL;
	struct hisp_mem_pool_info_list_s *list_node = NULL;
	struct hisp_mem_pool_info_list_s *node = NULL;
	struct hisp_mem_pool_info_list_s *node_temp = NULL;

	pool_info = &mem_pool->iova_pool_info;
	if (pool_info == NULL) {
		pr_err("[%s] isp mem pool didn't creat\n", __func__);
		return;
	}

	list_node = pool_info->node;
	if (list_node == NULL) {
		pr_err("[%s] iova mem pool list_node is NULL\n", __func__);
		return;
	}

	mutex_lock(&mem_pool->hisp_mem_mutex);
	list_for_each_entry_safe(node, node_temp, &list_node->list, list) {
		struct hisp_mem_type *type = &node->type;

		hisp_unrecord_memory_map(node->type, node);

		if (hisp_memtype_is(type->mem_type, ISP_PRIVATE_MEM)) {
			pr_info("[%s][ISP Clean Mem] type.0x%x\n",
				__func__, *((int *)type));
			if (hisp_master_has(type->master, ISP_CPU))
				hisp_memory_unmap(ISP_CPU, node->cpu_addr,
						  node->cpu_size, node);
			if (hisp_master_has(type->master, ISP_CORE))
				hisp_memory_unmap(ISP_CORE, node->core_addr,
						  node->core_size, node);
			if (hisp_master_has(type->master, OISP_CPU))
				hisp_memory_unmap(OISP_CPU, node->oisp_addr,
						  node->oisp_size, node);
			hisp_free_sgt_from_size(node);
		} else if (hisp_memtype_is(type->mem_type, ISP_PHY_MEM)) {
			pr_info("[%s][ISP Clean Mem] type.0x%x\n",
				__func__, *((int *)type));
			if (hisp_master_has(type->master, ISP_CPU))
				hisp_memory_unmap_phy(ISP_CPU,
						  node->cpu_addr, node->cpu_size);
			if (hisp_master_has(type->master, ISP_CORE))
				hisp_memory_unmap_phy(ISP_CORE,
						  node->core_addr, node->core_size);
			if (hisp_master_has(type->master, OISP_CPU))
				hisp_memory_unmap_phy(OISP_CPU,
						  node->oisp_addr, node->oisp_size);
		} else if (hisp_memtype_is(type->mem_type, ISP_SHARE_MEM)) {
			pr_info("[%s][ISP Clean Mem] type.0x%x\n",
				__func__, *((int *)type));
			if (hisp_master_has(type->master, ISP_CPU))
				hisp_memory_unmap(ISP_CPU, node->cpu_addr,
						  node->cpu_size, node);
			if (hisp_master_has(type->master, ISP_CORE))
				hisp_memory_unmap(ISP_CORE, node->core_addr,
						  node->core_size, node);
			if (hisp_master_has(type->master, OISP_CPU))
				hisp_memory_unmap(OISP_CPU, node->oisp_addr,
						  node->oisp_size, node);

			hisp_free_sgt_from_fd(node);
		} else {
			pr_debug
			    ("[%s][ISP Clean Mem] type[0x%x] just unrecord map\n",
			    __func__, *((int *)type));
		}
		list_del(&node->list);
		kfree(node);
	}
	mutex_unlock(&mem_pool->hisp_mem_mutex);
}
EXPORT_SYMBOL(hisp_mem_list_clean);

static int hisp_mempool_info_init(unsigned int use_mempool,
				  unsigned long long addr, unsigned int size)
{
	struct hisp_mem_pool_s *info = &g_hisp_mem_pool_info;
	int ret = 0;

	ret = memset_s(info, sizeof(g_hisp_mem_pool_info), 0x0, sizeof(g_hisp_mem_pool_info));	/*lint !e838 */
	if (ret < 0) {
		pr_err("[%s] memset_s g_hisp_mem_pool_info to 0 fail.%d\n",
		       __func__, ret);
		return ret;
	}

	if ((use_mempool == 0) || (addr == 0) || (size == 0)) {
		pr_err("[%s] Invalid args use_mempool.%u size.0x%x\n",
		       __func__, use_mempool, size);
		return -EINVAL;
	}

	if (addr >= ISP_MAX_IOVA_MAGIC ||
	    size >= ISP_MAX_IOVA_MAGIC || addr >= ISP_MAX_IOVA_MAGIC - size) {
		pr_err("[%s] argument out mem! size.0x%x\n",
		       __func__, size);
		return -EINVAL;
	}

	return 0;
}

int hisp_mempool_init(unsigned int addr, unsigned int size)
{
	struct hisp_mem_pool_s *info = &g_hisp_mem_pool_info;
	struct hisp_mem_pool_info_s *dev = NULL;
	struct hisp_mem_pool_info_list_s *listnode = NULL;
	unsigned int mempool_size = 0;
	int ret = 0;
	unsigned int use_mempool = hisp_smmuv3_mode();

	ret = hisp_mempool_info_init(use_mempool, addr, size);
	if (ret < 0)
		pr_err("[%s] mem_pool_info_int fail.%d\n", __func__, ret);

	dev = &info->iova_pool_info;
	if (dev == NULL) {
		pr_err("[%s] isp mem pool didn't creat\n", __func__);
		return -ENXIO;
	}

	mutex_init(&info->hisp_map_mutex);
	mutex_init(&info->hisp_iova_mutex);
	mutex_init(&info->hisp_list_mutex);
	mutex_init(&info->hisp_mem_mutex);

	mempool_size = PAGE_ALIGN(size);	/*lint !e50 */

	listnode =
	    kzalloc(sizeof(struct hisp_mem_pool_info_list_s), GFP_KERNEL);
	if (listnode == NULL)
		goto gen_pool_create_fail;

	INIT_LIST_HEAD(&listnode->list);	/*lint !e613 */
	dev->node = listnode;
	dev->enable = 1;
	dev->iova_start = addr;
	dev->iova_end = addr + size;
	return 0;

gen_pool_create_fail:
	mutex_destroy(&info->hisp_map_mutex);
	mutex_destroy(&info->hisp_iova_mutex);
	mutex_destroy(&info->hisp_list_mutex);
	mutex_destroy(&info->hisp_mem_mutex);

	return -ENOMEM;
}

void hisp_mempool_exit(void)
{
	struct hisp_mem_pool_s *info = &g_hisp_mem_pool_info;
	struct hisp_mem_pool_info_s *dev = NULL;
	struct hisp_mem_pool_info_list_s *listnode = NULL;
	unsigned int use_mempool = hisp_smmuv3_mode();
	int ret = 0;

	if (use_mempool) {
		dev = &info->iova_pool_info;
		if (dev == NULL) {
			pr_err("[%s] isp mem pool didn't creat\n", __func__);
			return;
		}
		if (dev->enable == 0) {
			pr_err("[%s] iova mem pool didn't creat\n", __func__);
			return;
		}
		listnode = dev->node;
		if (listnode == NULL) {
			pr_err("[%s] isp mem pool didn't creat\n", __func__);
			return;
		}

		kfree(listnode);
		dev->node = NULL;
		dev->enable = 0;
	}
	mutex_destroy(&info->hisp_map_mutex);
	mutex_destroy(&info->hisp_iova_mutex);
	mutex_destroy(&info->hisp_list_mutex);
	mutex_destroy(&info->hisp_mem_mutex);
	ret = memset_s(info, sizeof(g_hisp_mem_pool_info),
		0x0, sizeof(g_hisp_mem_pool_info));	/*lint !e838 */
	if (ret < 0)
		pr_err("[%s] memset_s g_hisp_mem_pool_info to 0 fail.%d\n",
		       __func__, ret);
}

#ifdef HISP_FUZZ
struct hisp_sanitizer_dev {
	struct hisp_mem_pool_info_list_s *shadow_list_info;
	struct hisp_mem_pool_info_list_s *kcov_list_info;
};
static struct hisp_sanitizer_dev g_hisp_sanitizer_dev;

static struct scatterlist *isp_vmalloc_to_sg(void *virt, int nr_pages)
{
	struct scatterlist *sglist;
	struct page *pg;
	int i;

	sglist = vzalloc(nr_pages * sizeof(*sglist));
	if (sglist == NULL)
		return NULL;
	sg_init_table(sglist, nr_pages);
	for (i = 0; i < nr_pages; i++, virt += PAGE_SIZE) {
		pg = vmalloc_to_page(virt);
		if (pg == NULL)
			goto isp_vm_to_sg_err;
		BUG_ON(PageHighMem(pg));
		sg_set_page(&sglist[i], pg, PAGE_SIZE, 0);
	}
	return sglist;

isp_vm_to_sg_err:
	vfree(sglist);
	return NULL;
}

static int hisp_vmalloc_map_ispaddr(
	void *vaddr, unsigned long size, unsigned int prot,
	struct hisp_mem_pool_info_list_s *list_info)
{
	int ret = 0;
	struct device *subdev;
	unsigned long iova = 0;
	size_t map_size = 0;
	struct hisp_mem_pool_s *pool_info = &g_hisp_mem_pool_info;
	struct scatterlist *scatterlist =
		isp_vmalloc_to_sg(vaddr, size / PAGE_SIZE);
	if (scatterlist == NULL) {
		pr_err("[%s] isp_vmalloc_to_sg failed\n", __func__);
		return -EINVAL;
	}

	subdev = hisp_mem_get_subdev(ISP_CPU);
	if (subdev == NULL) {
		pr_err("[%s] platform subdev is NULL!\n", __func__);
		goto map_err;
	}

	prot = hisp_mem_get_prot(ISP_CPU, prot);
	pr_debug("[%s] map size.0x%lx, prot.0x%x, scatterlist.0x%lx\n",
		__func__, size, prot, scatterlist);

	mutex_lock(&pool_info->hisp_map_mutex);
	iova = mm_iommu_map_sg(subdev, scatterlist, (int)prot, &map_size);
	if (iova == 0 || size != map_size) {
		pr_err("[%s] mm_iommu_map_sg failed! map_size.0x%lx, size.0x%lx\n",
			__func__, map_size, size);
		mutex_unlock(&pool_info->hisp_map_mutex);
		ret = -ENOMEM;
		goto map_err;
	}
	mutex_unlock(&pool_info->hisp_map_mutex);

	list_info->cpu_addr = iova;
	list_info->cpu_size = size;
	list_info->table->sgl = scatterlist;
	pr_info("[%s] iova.0x%llx size.0x%x", __func__, iova, size);
	return 0;
map_err:
	vfree(scatterlist);
	return ret;
}

static int hisp_vmalloc_unmap_ispaddr(void *vaddr,
				      struct scatterlist *scatterlist)
{
	int ret = 0;
	struct device *subdev;
	struct hisp_mem_pool_s *pool_info = &g_hisp_mem_pool_info;
	if (scatterlist == NULL) {
		pr_err("[%s] scatterlist is NULL\n", __func__);
		return -EINVAL;
	}

	subdev = hisp_mem_get_subdev(ISP_CPU);
	if (subdev == NULL) {
		pr_err("[%s] platform subdev is NULL!\n", __func__);
		return NULL;
	}

	mutex_lock(&pool_info->hisp_map_mutex);
	ret = mm_iommu_unmap_sg(subdev, scatterlist, vaddr);
	if (unlikely(ret != 0)) {
		pr_err("[%s] mm_iommu_unmap_sg failed! addr.0x%lx\n",
			__func__, vaddr);
		mutex_unlock(&pool_info->hisp_map_mutex);
		ret = -ENOMEM;
		goto map_exit;
	}
	mutex_unlock(&pool_info->hisp_map_mutex);

map_exit:
	vfree(scatterlist);
	return ret;
}

static int hisp_kcov_shadow_mem_map(
	isp_memtype_e type, struct hisp_mem_pool_info_list_s *list_info)
{
	int ret;
	void *addr = NULL;
	unsigned long size = 0;
	unsigned int prot = 0;

	if (list_info == NULL) {
		pr_err("[%s] list_info null\n", __func__);
		return -EINVAL;
	}

	if (type == ISP_KCOV_MEM) {
		ret = hisp_kcov_get_shmem(&addr, &size);
		pr_info("[%s] kcov mem: size.0x%lx\n", __func__, size);
		prot = IOMMU_READ | IOMMU_WRITE;
	} else if (type== ISP_SHADOW_MEM) {
		ret = hisp_fuzz_get_shmem(&addr, &size);
		pr_info("[%s] shadow mem: size.0x%lx\n", __func__, size);
		prot = IOMMU_READ | IOMMU_WRITE | IOMMU_CACHE;
	} else {
		pr_err("[%s] mem_type = %d, not supported\n", __func__, type);
		return -EINVAL;
	}
	if (ret != 0) {
		pr_err("[%s] type.%d: get_shmem failed\n", __func__, type);
		return -EINVAL;
	}
	list_info->type.mem_type = type;
	list_info->table =
		(struct sg_table *)kzalloc(sizeof(struct sg_table), GFP_KERNEL);
	if (list_info->table == NULL) {
		pr_err("[%s] sg_table malloc failed\n", __func__);
		return -ENOMEM;
	}

	ret = hisp_vmalloc_map_ispaddr(addr, size, prot, list_info);
	if (ret != 0) {
		pr_err("[%s] vmalloc map ispaddr failed\n", __func__);
		(void)kfree(list_info->table);
		return -EINVAL;
	}
	pr_info("[%s] map done", __func__);
	return 0;
}

static int hisp_kcov_shadow_mem_unmap(
	struct hisp_mem_pool_info_list_s *list_info)
{
	int ret;
	void *vaddr;
	struct scatterlist *scatterlist;
	if (!list_info->table) {
		pr_err("[%s] list_info->table is NULL\n", __func__);
		return -EINVAL;
	}
 
	scatterlist = list_info->table->sgl;
	vaddr = list_info->cpu_addr;
	ret = hisp_vmalloc_unmap_ispaddr(vaddr, scatterlist);
	if (ret != 0) {
		pr_err("[%s] vmalloc unmap ispaddr failed\n", __func__);
		return -EINVAL;
	}
	if (list_info->table)
		(void)kfree(list_info->table);
	pr_info("[%s] unmap done", __func__);
	return ret;
}


int hisp_fuzz_kcov_shadow_mem_init(void)
{
	int ret = 0;
	struct hisp_mem_pool_info_list_s *list_info_kcov, *list_info_shadow;
	/* shadow memory map */
	list_info_shadow = kzalloc(
			sizeof(struct hisp_mem_pool_info_list_s), GFP_KERNEL);
	if (unlikely(!list_info_shadow)) {
		pr_err("%s: fail to alloc list_info_shadow\n", __func__);
		return -ENOMEM;
	}
	ret = hisp_kcov_shadow_mem_map(ISP_SHADOW_MEM, list_info_shadow);
	if (unlikely(ret != 0)) {
		pr_err("[%s]: kcov mem map failed\n", __func__);
		goto free_shadow_mem;
	}
	g_hisp_sanitizer_dev.shadow_list_info = list_info_shadow;
	/* kcov memory map */
	list_info_kcov = kzalloc(
			sizeof(struct hisp_mem_pool_info_list_s), GFP_KERNEL);
	if (unlikely(!list_info_kcov)) {
		pr_err("%s: fail to alloc list_info_kcov\n", __func__);
		goto unmap_shadow_mem;
	}
	ret = hisp_kcov_shadow_mem_map(ISP_KCOV_MEM, list_info_kcov);
	if (unlikely(ret != 0)) {
		pr_err("[%s]: kcov mem map failed\n", __func__);
		goto free_kcov_mem;
	}
	g_hisp_sanitizer_dev.kcov_list_info = list_info_kcov;

	return 0;
unmap_kcov_mem:
	(void)hisp_kcov_shadow_mem_unmap(list_info_kcov);
free_kcov_mem:
	if (list_info_kcov)
		kfree(list_info_kcov);
unmap_shadow_mem:
	(void)hisp_kcov_shadow_mem_unmap(list_info_shadow);
free_shadow_mem:
	if (list_info_shadow)
		kfree(list_info_shadow);
	return ret;
}

void hisp_fuzz_kcov_shadow_mem_deinit(void)
{
	int ret = 0;
	ret = hisp_kcov_shadow_mem_unmap(g_hisp_sanitizer_dev.shadow_list_info);
	if (unlikely(ret != 0))
		pr_err("[%s] unmap shadow memory failed! ret.%d", __func__, ret);

	ret = hisp_kcov_shadow_mem_unmap(g_hisp_sanitizer_dev.kcov_list_info);
	if (unlikely(ret != 0))
		pr_err("[%s] unmap kcov memory failed! ret.%d", __func__, ret);
}
#endif