/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 19 09:09:00 2019
 */
#include <linux/bio.h>
#include <linux/init.h>
#include <linux/blkdev.h>
#include <linux/module.h>
#include <linux/securec.h>
#include <linux/version.h>
#include <linux/of_address.h>
#include <linux/platform_device.h>

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)) && (defined CONFIG_DAX)
#include <linux/dax.h>
#include <linux/pfn_t.h>
#endif

#define SECTOR_SHIFT		9
#define PAGE_SECTORS_SHIFT	(PAGE_SHIFT - SECTOR_SHIFT)
#define PAGE_SECTORS		(1 << PAGE_SECTORS_SHIFT)
#define MAX_ZONE		64
#define MIN_ZONE		1
/* min pramdisk size 1M, max size 4096M */
#define MIN_PRADISK_SIZE	0x100000
#define MAX_PRADISK_SIZE	0x100000000
#define PRAMDISK_DEVICE_NAME	"pramdisk"

struct pramdisk_device {
	struct request_queue *pramdisk_queue;
	struct gendisk *pramdisk_gendisk;
	struct list_head pramdisk_list;
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)) && (defined CONFIG_DAX)
	/* Kernel5.10 introduce dax_operations to replace bdev_direct_access */
	struct dax_device *dax_dev;
#endif
};

struct pramdisk_capacity {
	struct list_head cap_list;
	/* the capability of pramdisk */
	unsigned long cap;
	unsigned long pramdisk_addr_pfn;
	unsigned long pramdisk_max_pfn;
	unsigned long pramdisk_phy_addr;
	void __iomem *pramdisk_virt_addr;
	/* the mirror of dev */
	int mirror;
};

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Non-volatile RAM Block Driver");
MODULE_VERSION("1.1");

static LIST_HEAD(pramdisk_cap_list);
static LIST_HEAD(pramdisk_device_list);
static rwlock_t list_lock = __RW_LOCK_UNLOCKED(list_lock);
static int pramdisk_major = 0;
static int capacity_num = 0;
static unsigned long capacity[MAX_ZONE] = {0};

static int pramdisk_get_addr_pfn(int mirror, unsigned long *pramdisk_addr_pfn,
				 unsigned long *pramdisk_max_pfn, void **virt, unsigned long *cap);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)) && (defined CONFIG_DAX)
static long pramdisk_dax_direct_access(struct dax_device *dax_dev, pgoff_t pgoff,
				    long nr_pages, void **kaddr, pfn_t *pfn)
{
	int ret;
	long page_num;
	phys_addr_t rd_phys_addr = 0;
	void *pramdisk_virt_addr;
	unsigned long pramdisk_addr_pfn;
	unsigned long pramdisk_max_pfn;
	unsigned long pramdisk_length;
	struct pramdisk_device *pramdisk = dax_get_private(dax_dev);
	resource_size_t offset = pgoff * PAGE_SIZE;

	BUG_ON((pramdisk == NULL) || (pramdisk->pramdisk_gendisk == NULL));

	ret = pramdisk_get_addr_pfn(pramdisk->pramdisk_gendisk->first_minor, &pramdisk_addr_pfn,
				    &pramdisk_max_pfn, &pramdisk_virt_addr, &pramdisk_length);
	if (ret < 0) {
		pr_err("Fail to get addr pfn\n");
		return -ERANGE;
	}

	rd_phys_addr = PFN_PHYS(pramdisk_addr_pfn) + offset;
	if (PHYS_PFN(rd_phys_addr) > pramdisk_max_pfn) {
		return -ERANGE;
	}

	*kaddr = pramdisk_virt_addr + offset;
	*pfn = phys_to_pfn_t(rd_phys_addr, PFN_DEV | PFN_SPECIAL);

	page_num = (long)PHYS_PFN(PFN_PHYS(pramdisk_max_pfn + 1) - rd_phys_addr);
	return page_num;
}

static int pramdisk_dax_zero_page_range(struct dax_device *dax_dev, pgoff_t pgoff,
				    size_t nr_pages)
{
	long ret;
	void *kaddr;

	ret = dax_direct_access(dax_dev, pgoff, nr_pages, &kaddr, NULL);
	if (ret < 0)
		return (int)ret;
	ret = memset_s(kaddr, nr_pages << PAGE_SHIFT, 0, nr_pages << PAGE_SHIFT);
	BUG_ON(ret != 0);
	return 0;
}

static const struct dax_operations pramdisk_dax_ops = {
	.direct_access = pramdisk_dax_direct_access,
	.dax_supported = generic_fsdax_supported,
	.zero_page_range = pramdisk_dax_zero_page_range
};

static int pramdisk_init_dax(struct pramdisk_device *pramdisk)
{
	struct dax_device *dax_dev;

	if ((pramdisk == NULL) || (pramdisk->pramdisk_gendisk == NULL)) {
		return -EINVAL;
	}

	dax_dev = alloc_dax(pramdisk, pramdisk->pramdisk_gendisk->disk_name, &pramdisk_dax_ops, 0);
	if (IS_ERR(dax_dev)) {
		pr_err("pramdisk: alloc dax_dev failed\n");
		return -ENOMEM;
	}

	pramdisk->dax_dev = dax_dev;
	blk_queue_flag_set(QUEUE_FLAG_DAX, pramdisk->pramdisk_queue);
	return 0;
}

static void pramdisk_destroy_dax(struct pramdisk_device *pramdisk)
{
	if (pramdisk->dax_dev != NULL) {
		kill_dax(pramdisk->dax_dev);
	}
	return;
}
#else
static inline int pramdisk_init_dax(struct pramdisk_device *pramdisk)
{
	return 0;
}
static inline void pramdisk_destroy_dax(struct pramdisk_device *pramdisk)
{
	return;
}
#ifdef CONFIG_DAX
#warning "pramdisk DAX support only enabled for LDK >= 5.10"
#endif
#endif

/*
 * clean queue
 */
static void pramdisk_free(struct pramdisk_device *pramdisk)
{
	if (pramdisk->pramdisk_gendisk != NULL) {
		put_disk(pramdisk->pramdisk_gendisk);
	}
	if (pramdisk->pramdisk_queue != NULL) {
		blk_cleanup_queue(pramdisk->pramdisk_queue);
	}
	pramdisk_destroy_dax(pramdisk);
	kfree(pramdisk);
}

/*
 * delete gendisk
 */
static void pramdisk_del_one(struct pramdisk_device *pramdisk)
{
	list_del(&pramdisk->pramdisk_list);

	if (pramdisk->pramdisk_gendisk != NULL) {
		del_gendisk(pramdisk->pramdisk_gendisk);
	}
	pramdisk_free(pramdisk);
}

/*
 * Description	: get the addr pfn of mirror
 */
static int pramdisk_get_addr_pfn(int mirror, unsigned long *pramdisk_addr_pfn,
				 unsigned long *pramdisk_max_pfn, void **virt, unsigned long *cap)
{
	struct pramdisk_capacity *capacity_list;
	struct list_head *p;
	unsigned long tmp_cap = 0;
	unsigned long tmp_pramdisk_addr_pfn = 0;
	unsigned long tmp_pramdisk_max_pfn = 0;
	void *tmp_pramdisk_virt_addr = NULL;

	read_lock(&list_lock);
	if (list_empty(&pramdisk_cap_list)) {
		pr_err("pramdisk_cap_list should not be NULL\n");
		read_unlock(&list_lock);
		return -EINVAL;
	}
	list_for_each(p, &pramdisk_cap_list) {
		capacity_list = list_entry(p, struct pramdisk_capacity, cap_list);
		if (capacity_list->mirror == mirror) {
			tmp_cap = capacity_list->cap;
			tmp_pramdisk_addr_pfn = capacity_list->pramdisk_addr_pfn;
			tmp_pramdisk_max_pfn = capacity_list->pramdisk_max_pfn;
			tmp_pramdisk_virt_addr = capacity_list->pramdisk_virt_addr;
			break;
		}
	}
	read_unlock(&list_lock);

	if (tmp_pramdisk_addr_pfn == 0) {
		pr_err("can not get pramdisk_get_addr_pfn [%d] the number of partition must > 1\n",
			mirror);
		return -EPERM;
	}
	*pramdisk_addr_pfn = tmp_pramdisk_addr_pfn;
	*pramdisk_max_pfn = tmp_pramdisk_max_pfn;
	*virt = tmp_pramdisk_virt_addr;
	*cap = tmp_cap;

	return 0;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(5,10,0)
/*
 * Description	: xip filesystem interface
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)
static long pramdisk_direct_access(struct block_device *bdev, sector_t sector,
				   void __pmem **kaddr, unsigned long *pfn)
#else
static long pramdisk_direct_access(struct block_device *bdev, sector_t sector,
				   void **kaddr, unsigned long *pfn)
#endif
{
	int ret;
	unsigned long rd_pfn = 0;
	phys_addr_t rd_phys_addr = 0;
	void *pramdisk_virt_addr;
	unsigned long pramdisk_addr_pfn;
	unsigned long pramdisk_max_pfn;
	unsigned long pramdisk_length;
	long size;

	if (sector & (PAGE_SECTORS-1)) {
		pr_err("sector not aligned with PAGE_SECTORS\n");
		return -EINVAL;
	}

	if ((sector + PAGE_SECTORS) > get_capacity(bdev->bd_disk)) {
		pr_err("sector is too big\n");
		return -ERANGE;
	}

	ret = pramdisk_get_addr_pfn(bdev->bd_disk->first_minor, &pramdisk_addr_pfn,
				    &pramdisk_max_pfn, &pramdisk_virt_addr, &pramdisk_length);
	if (ret < 0) {
		pr_err("Fail to get addr pfn\n");
		return -ERANGE;
	}
	rd_phys_addr = (((phys_addr_t)pramdisk_addr_pfn) << PAGE_SHIFT) + (sector << SECTOR_SHIFT);
	rd_pfn = PFN_DOWN(rd_phys_addr);
	if (rd_pfn > pramdisk_max_pfn) {
		return -ERANGE;
	}
	*kaddr = pramdisk_virt_addr + (sector << SECTOR_SHIFT);
	*pfn = rd_pfn;

	size = (long)(((pramdisk_max_pfn + 1) << PAGE_SHIFT) - rd_phys_addr);
	return size;
}

static const struct block_device_operations pramdisk_ops = {
	.owner = THIS_MODULE,
	.direct_access = pramdisk_direct_access,
};
#endif

/*
 * Process a single bvec of a bio
 */
static int pramdisk_do_bvec(struct bio_vec *bio_bvec, bool is_write, unsigned long sector_addr, int mirror)
{
	int ret;
	void *mem;
	phys_addr_t rd_phys_addr;
	void *pramdisk_virt_addr;
	unsigned long pramdisk_addr_pfn;
	unsigned long pramdisk_max_pfn;
	unsigned long pramdisk_length;

	ret = pramdisk_get_addr_pfn(mirror, &pramdisk_addr_pfn, &pramdisk_max_pfn, &pramdisk_virt_addr,
				    &pramdisk_length);
	if (ret < 0) {
		pr_err("fail to get addr pfn\n");
		return -ERANGE;
	}

	rd_phys_addr = (((phys_addr_t)pramdisk_addr_pfn) << PAGE_SHIFT) + sector_addr;
	if (PFN_DOWN(rd_phys_addr) > pramdisk_max_pfn) {
		pr_err("failed: out-of-bounds access\n");
		return -ERANGE;
	}
	/* for security warning */
	if (bio_bvec->bv_offset > PAGE_SIZE ||
		bio_bvec->bv_len > PAGE_SIZE ||
		bio_bvec->bv_offset + bio_bvec->bv_len > PAGE_SIZE) {
		pr_err("memory segment must be located in one page\n");
		return -ERANGE;
	}
	/* arm64 only support normal memory access */
	mem = kmap_atomic(bio_bvec->bv_page);
	if (mem == NULL) {
		return -ENOMEM;
	}
	if (!is_write) {
		ret = memcpy_s(mem + bio_bvec->bv_offset, PAGE_SIZE - bio_bvec->bv_offset,
		      pramdisk_virt_addr + sector_addr, bio_bvec->bv_len);
	} else {
		ret = memcpy_s(pramdisk_virt_addr + sector_addr, pramdisk_length - sector_addr,
		      mem + bio_bvec->bv_offset, bio_bvec->bv_len);
	}
	BUG_ON(ret != 0);
	kunmap_atomic(mem);

	return 0;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)
static inline bool is_op_valid(struct bio *bio)
{
	int rw = bio_rw(bio);
	return (rw == READ) || (rw == WRITE) || (rw == READA);
}

static inline bool is_op_write(struct bio *bio)
{
	return bio_rw(bio) == WRITE;
}

static inline void bio_set_status(struct bio *bio, int status)
{
	bio->bi_error = status;
}

static inline struct gendisk *bio_disk_of(struct bio *bio)
{
	return bio->bi_bdev->bd_disk;
}

static inline void device_add_disk(struct device *parent, struct gendisk *disk)
{
	disk->driverfs_dev = parent;
	add_disk(disk);
}
#else
static inline bool is_op_valid(struct bio *bio)
{
	int op = bio_op(bio);
	return (op == REQ_OP_READ) || (op == REQ_OP_WRITE);
}

static inline bool is_op_write(struct bio *bio)
{
	return op_is_write(bio_op(bio));
}

static inline void bio_set_status(struct bio *bio, int status)
{
	bio->bi_status = (unsigned char)status;
}

static inline struct gendisk *bio_disk_of(struct bio *bio)
{
	return bio->bi_disk;
}
#endif

static blk_qc_t pramdisk_make_request(struct request_queue *q, struct bio *bio)
{
	int err;
	struct bvec_iter iter;
	struct bio_vec bio_bvec;
	unsigned long sector_addr;
	struct gendisk *bd_disk = bio_disk_of(bio);

	if (bio->bi_iter.bi_sector + (bio->bi_iter.bi_size >> SECTOR_SHIFT) >
	    get_capacity(bd_disk)) {
		goto fail;
	}
	if (!is_op_valid(bio)) {
		goto fail;
	}

	sector_addr = bio->bi_iter.bi_sector << SECTOR_SHIFT;
	bio_for_each_segment(bio_bvec, bio, iter) {
		unsigned int len = bio_bvec.bv_len;

		err = pramdisk_do_bvec(&bio_bvec, is_op_write(bio), sector_addr, bd_disk->first_minor);
		bio_set_status(bio, err);
		if (err) {
			goto fail;
		}

		sector_addr += len;
	}
	bio_endio(bio);
	return BLK_QC_T_NONE;

fail:
	bio_io_error(bio);
	return BLK_QC_T_NONE;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)
static blk_qc_t pramdisk_make_request_ex(struct bio *bio)
{
	return pramdisk_make_request(NULL, bio);
}

static const struct block_device_operations pramdisk_ops = {
	.owner = THIS_MODULE,
	.submit_bio = pramdisk_make_request_ex,
};
#endif

static inline struct request_queue *__pramdisk_alloc_queue(void)
{
	struct request_queue *queue = NULL;

#if LINUX_VERSION_CODE < KERNEL_VERSION(5,10,0)
	queue = blk_alloc_queue(GFP_KERNEL);
	if (queue == NULL) {
		return queue;
	}
	blk_queue_make_request(queue, pramdisk_make_request);
#else
	queue = blk_alloc_queue(NUMA_NO_NODE);
#endif
	return queue;
}

/*
 * Description	: 1.allocate a request queue
 *		: 2.bind the request queue with the request handle function
 *		: 3.init gendisk members
 *		: 4.init dax
 */
static struct pramdisk_device *pramdisk_alloc(int i, unsigned long size)
{
	int err;
	struct gendisk *disk;
	struct pramdisk_device *pramdisk_tmp;

	pramdisk_tmp = kzalloc(sizeof(*pramdisk_tmp), GFP_KERNEL);
	if (pramdisk_tmp == NULL) {
		goto out;
	}
	pramdisk_tmp->pramdisk_queue = __pramdisk_alloc_queue();
	if (pramdisk_tmp->pramdisk_queue == NULL) {
		goto out_free_dev;
	}

	disk = pramdisk_tmp->pramdisk_gendisk = alloc_disk(1);
	if (disk == NULL) {
		goto out_free_queue;
	}
	disk->major = pramdisk_major;
	disk->first_minor = i;
	disk->fops = &pramdisk_ops;
	disk->queue = pramdisk_tmp->pramdisk_queue;
	err = snprintf_s(disk->disk_name, DISK_NAME_LEN, DISK_NAME_LEN - 1U, "pramdisk%d", i);
	if (err < 0) {
		goto out_free_queue;
	}
	set_capacity(disk, size >> SECTOR_SHIFT);

	err = pramdisk_init_dax(pramdisk_tmp);
	if (err < 0) {
		goto out_free_queue;
	}

	return pramdisk_tmp;

out_free_queue:
	blk_cleanup_queue(pramdisk_tmp->pramdisk_queue);
out_free_dev:
	kfree(pramdisk_tmp);
out:
	return NULL;
}

static void free_pramdisk_cap_list(void)
{
	struct pramdisk_capacity *capacity_list;
	struct pramdisk_capacity *capacity_next;

	write_lock(&list_lock);
	list_for_each_entry_safe(capacity_list, capacity_next, &pramdisk_cap_list, cap_list) {
		list_del(&capacity_list->cap_list);
		iounmap(capacity_list->pramdisk_virt_addr);
		kfree(capacity_list);
	}
	write_unlock(&list_lock);
}

static inline void __device_add_disk(struct device *parent, struct gendisk *disk)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,10,0)
	device_add_disk(parent, disk);
#else
	device_add_disk(parent, disk, NULL);
#endif
}

/*
 * Intialize the module.The main function is register a block dev
 * and add a gendisk.
 */
static int pramdisk_init(struct platform_device *pdev)
{
	int zone;
	int major;
	int error = 0;
	struct pramdisk_device *next;
	struct pramdisk_device *pramdisk;

	major = register_blkdev(0, PRAMDISK_DEVICE_NAME);
	if (major < 0) {
		pr_err("%s: register_blkdev failed, err=%d\n", PRAMDISK_DEVICE_NAME, major);
		error = major;
		goto out_free_pramdisk_cap_list;
	}
	pramdisk_major = major;

	for (zone = 0; zone < capacity_num; zone++) {
		pramdisk = pramdisk_alloc(zone, capacity[zone]);
		if (pramdisk == NULL) {
			pr_err("pramdisk alloc failed\n");
			error = -ENOMEM;
			goto out_free_pramdisk;
		}
		__device_add_disk(&pdev->dev, pramdisk->pramdisk_gendisk);

		list_add_tail(&pramdisk->pramdisk_list, &pramdisk_device_list);
	}

	return 0;

out_free_pramdisk:
	list_for_each_entry_safe(pramdisk, next, &pramdisk_device_list, pramdisk_list) {
		list_del(&pramdisk->pramdisk_list);
		pramdisk_free(pramdisk);
	}
	unregister_blkdev(pramdisk_major, PRAMDISK_DEVICE_NAME);

out_free_pramdisk_cap_list:
	free_pramdisk_cap_list();

	return error;
}

/*
 * Unintialize the module.The main function is unregister a blcok dev
 * and delete a gendisk.
 */
static void pramdisk_exit(void)
{
	struct pramdisk_device *next;
	struct pramdisk_device *pramdisk;

	list_for_each_entry_safe(pramdisk, next, &pramdisk_device_list, pramdisk_list)
		pramdisk_del_one(pramdisk);

	free_pramdisk_cap_list();
	unregister_blkdev(pramdisk_major, PRAMDISK_DEVICE_NAME);
}

static int pramdisk_check_resource(struct resource *res)
{
	u64 size;
	u64 base;

	base = res->start;
	size = res->end - res->start + 1;
	if (size < (MIN_PRADISK_SIZE) || size > (MAX_PRADISK_SIZE) || (PAGE_ALIGNED(size) == 0)) {
		pr_err("input parameter size 0x%llx must be bigger than 1M, "
		       "and smaller than 4096M and must be aligned 4K/ONE PAGE SIZE\n", size);
		return -EINVAL;
	}
	if (!PAGE_ALIGNED(base)) {
		pr_err("memery base addr should be aligned one page\n");
		return -EINVAL;
	}
	if (base == 0) {
		pr_err("memery base addr must not be zero\n");
		return -EINVAL;
	}
	return 0;
}

static int pramdisk_add_caplist(struct resource *resource, int num)
{
	u64 size;
	void *virt;
	u64 pramdisk_length;
	u64 pramdisk_addr_tmp;
	u64 pramdisk_max_pfn_tmp;
	u64 pramdisk_addr_pfn_tmp;
	struct pramdisk_capacity *cap_tmp;

	pramdisk_addr_tmp = resource->start;
	size = resource->end - resource->start + 1;
	pramdisk_length = size;
	pramdisk_addr_pfn_tmp = PFN_UP(pramdisk_addr_tmp);
	if (pramdisk_addr_pfn_tmp == 0) {
		return -EINVAL;
	}
	pramdisk_max_pfn_tmp = pramdisk_addr_pfn_tmp + (pramdisk_length >> PAGE_SHIFT) - 1;
	cap_tmp = kzalloc(sizeof(struct pramdisk_capacity), GFP_KERNEL);
	if (cap_tmp == NULL) {
		pr_err("kzalloc pramdisk_capacity_list failed\n");
		return -ENOMEM;
	}

	virt = ioremap_wc(pramdisk_addr_tmp, pramdisk_length);
	if (virt == NULL) {
		kfree(cap_tmp);
		pr_err("cannot ioremap phy address\n");
		return -ENOMEM;
	}
	cap_tmp->cap = pramdisk_length;
	cap_tmp->pramdisk_addr_pfn = pramdisk_addr_pfn_tmp;
	cap_tmp->pramdisk_phy_addr = pramdisk_addr_tmp;
	cap_tmp->pramdisk_virt_addr = virt;
	cap_tmp->pramdisk_max_pfn = pramdisk_max_pfn_tmp;
	cap_tmp->mirror = num;

	write_lock(&list_lock);
	list_add_tail(&cap_tmp->cap_list, &pramdisk_cap_list);
	write_unlock(&list_lock);
	capacity[num] = cap_tmp->cap;

	return 0;
}

static int hm_pramdisk_probe(struct platform_device *pdev)
{
	int ret;
	int num_reg = 0;
	int pramdisk_num = 0;
	struct resource temp_res;

	INIT_LIST_HEAD(&pramdisk_cap_list);

	/*
	* parse dts configuration
	*/
	while (of_address_to_resource(pdev->dev.of_node, num_reg, &temp_res) == 0) {
		num_reg++;
		if (pramdisk_num >= MAX_ZONE) {
			pr_err("parse pramdisk failed, the pramdisk num should less than 64\n");
			ret = -EINVAL;
			goto fail;
		}
		ret = pramdisk_check_resource(&temp_res);
		if (ret) {
			goto fail;
		}
		ret = pramdisk_add_caplist(&temp_res, pramdisk_num);
		if (ret) {
			goto fail;
		}
		pramdisk_num++;
	}

	if (pramdisk_num < MIN_ZONE) {
		pr_err("there are no pramdisk areas, the pramdisk num should more than 0\n");
		ret =  -EINVAL;
		goto fail;
	}
	capacity_num = pramdisk_num;
	ret = pramdisk_init(pdev);
	if (ret) {
		pr_err("pramdisk init fail, err=%d\n", ret);
		goto fail;
	}

	return 0;
fail:
	free_pramdisk_cap_list();
	return ret;
}

static int hm_pramdisk_remove(struct platform_device *pdev)
{
	pramdisk_exit();
	return 0;
}

static struct of_device_id pramdisk_dt_ids[] = {
	{ .compatible = "hm-pramdisk"},
	{},
};


static struct platform_driver hm_pramdisk_platform_driver = {
	.probe = hm_pramdisk_probe,
	.remove = hm_pramdisk_remove,
	.driver = {
		.name = "hm_pramdisk",
		.of_match_table = pramdisk_dt_ids,
	}
};

static int __init hm_pramdisk_init(void)
{
	pr_info("HM pramdisk init\n");
	if (platform_driver_register(&hm_pramdisk_platform_driver)) {
		pr_err("platform_driver_register fail\n");
		return -1;
	}
	pr_info("HM pramdisk end\n");
	return 0;
}

static void __exit hm_pramdisk_exit(void)
{
	pr_info("HM pramdisk exit\n");
	platform_driver_unregister(&hm_pramdisk_platform_driver);
}

module_init(hm_pramdisk_init);
module_exit(hm_pramdisk_exit);
