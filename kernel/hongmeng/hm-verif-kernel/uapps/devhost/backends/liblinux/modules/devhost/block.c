/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 18 15:55:48 2019
 */
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/bio.h>
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/kernel.h>
#include <linux/highmem.h>
#include <linux/version.h>
#include <linux/securec.h>
#include <linux/blk_types.h>
#ifdef CONFIG_LIBLINUX_ASYNC_END_IO
#include <linux/workqueue.h>
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)
#include <linux/dax.h>
#include <linux/pfn_t.h>
#endif

#include <devhost/bd_types.h> /* `libsysif/devhost/bd_types.h` */

#ifdef CONFIG_BLK_INLINE_ENCRYPTION
#include <linux/blk-crypto.h>
#endif

#include "devhost.h"

#define bio_ctx(bio)			((bio)->bi_ctx)
#define set_bio_ctx(bio, v)		((bio)->bi_ctx = ((void *)((uintptr_t)(v))))

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)
#define bio_status(bio) ((bio)->bi_error)
#else
#define bio_status(bio) ((bio)->bi_status)
#endif

#define SECTOR_SHIFT 9
#define SECTOR_SIZE (1 << SECTOR_SHIFT)

#define BD_FEATURE_RODEV (1ULL << 28)
#define BD_FEATURE_MAS_BLK (1ULL << 27)
#define BD_FEATURE_DIRECTMAP (1ULL << 29)
#define BD_FEATURE_DAS (1ULL << 30)
#define BD_REQ_DISCARD	(1ULL << 7)

#ifdef CONFIG_HISI_FALLBACK_ENCRYPTION
#define NVME_CRYPTO_DATA_UNIT_SIZE 4096
#define NVME_CRYPTO_DUN_BYTES      16
#define NVME_CRYPTO_CI_KEY_IDX_MAX 31
#define NVME_CRYPTO_CI_KEY_IDX_MASK 0xff
#endif

struct bio_context {
	void *block_ctx;
	devhost_end_bio_fn_t devhost_end_bio_fn;
	unsigned int bio_idx;
	bool direct_map_page;
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)
	int bi_status;
#else
	blk_status_t bi_status;
#endif
#ifdef CONFIG_LIBLINUX_ASYNC_END_IO
	struct work_struct work;
#endif
};

static inline struct block_device *
block_device_of_filep(void *filep)
{
	struct inode *inode = ((struct file *)filep)->f_mapping->host;

	return I_BDEV(inode);
}

static inline void __end_bio(struct bio_context *bio_ctx)
{
	int ret = bio_ctx->devhost_end_bio_fn(bio_ctx->block_ctx, bio_ctx->bi_status,
					  bio_ctx->bio_idx);
	if (ret != 0) {
		pr_warn("block end io failed, err=%d\n", ret);
	}

	kfree(bio_ctx);
}

#ifdef CONFIG_LIBLINUX_ASYNC_END_IO
static void end_bio_handle(struct work_struct *work)
{
	struct bio_context *bio_ctx = container_of(work, struct bio_context, work);
	BUG_ON(bio_ctx == NULL);
	__end_bio(bio_ctx);
}
#endif

static void end_bio(struct bio *bio)
{
	struct bio_vec *bv = NULL;
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,10,0)
	unsigned short i = 0;
#else
	struct bvec_iter_all i;
#endif
	int ret;
	struct bio_context *bio_ctx = bio_ctx(bio);

	BUG_ON(bio_ctx == NULL || bio_ctx->devhost_end_bio_fn == NULL);
	if (bio_status(bio) != 0) {
		pr_warn("bio callback called, err=%d\n", (int)bio_status(bio));
	}

	if (bio_ctx->direct_map_page) {
		goto skip_copy;
	}
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)
	if (bio->bi_rw & BD_REQ_DISCARD) {
#else
	if ((bio->bi_opf & REQ_OP_MASK) == REQ_OP_DISCARD) {
#endif
		goto skip_copy;
	}

	bio_for_each_segment_all(bv, bio, i) {
		struct page *page = bv->bv_page;
		/* copy data back to bio buffer */
		ret = memcpy_s((void *)page->private, bv->bv_len, page_address(page), bv->bv_len);
		BUG_ON(ret != 0);
		/* free this page */
		__free_page(page);
	}

skip_copy:
	bio_ctx->bi_status = bio_status(bio);
#ifdef CONFIG_LIBLINUX_ASYNC_END_IO
	queue_work(system_highpri_wq, &bio_ctx->work);
#else
	__end_bio(bio_ctx);
#endif
	bio_put(bio);
}

static int native_submit_bio(void *bio, unsigned int flags)
{
	bio_get((struct bio *)bio);

	/* linux don't care the ret of submit_bio */
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)
	submit_bio(((struct bio *)bio)->bi_rw, (struct bio *)bio);
#else
	submit_bio((struct bio *)bio);
#endif
	bio_put(bio);
	return 0;
}

static void native_bio_free(void *_bio)
{
	struct bio *bio = (struct bio *)_bio;
	struct bio_vec bv;
	struct bvec_iter iter;
	struct bio_context *bio_ctx = NULL;

	BUG_ON(bio == NULL);
	bio_ctx = bio_ctx(bio);

	if (bio_ctx != NULL) {
		if (!bio_ctx->direct_map_page) {
			bio_for_each_segment(bv, bio, iter) {
				struct page *page = bv.bv_page;
					/* free this page */
					__free_page(page);
			}
		}

		kfree(bio_ctx(bio));
	}
	bio_put(bio);
}

static int native_bio_discard_support(unsigned int *rw, unsigned int bv_len,
				      struct block_device *bdev, struct bio *bio)
{
	char b[BDEVNAME_SIZE];
	struct request_queue *q = NULL;
	unsigned short sector_size = 0;
	unsigned int rw_flag = *rw;

	if (rw_flag & BD_REQ_DISCARD) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)
		rw_flag |= REQ_WRITE;
#else
		rw_flag = REQ_OP_DISCARD;
#endif
		*rw = (int)rw_flag;
		q = bdev_get_queue(bdev);
		if (unlikely(q == NULL)) {
			pr_err("block-device %s do not support discard\n",
					bdevname(bdev, b));
			return -EINVAL;
		}
		sector_size = queue_logical_block_size(q);
		BUG_ON(sector_size == 0);

		if ((bv_len == 0 || bv_len % sector_size != 0)) {
			pr_err("native bio bv_len(%u) err\n", bv_len);
			return -EINVAL;
		}
		bio->bi_iter.bi_size = bv_len;
	}

	return 0;
}

#ifdef CONFIG_HISI_FALLBACK_ENCRYPTION
static int bio_nvme_crypto_set_ctx(struct bio *bio, const struct bd_io *bd_io)
{
	uint8_t *ci_metadata = NULL;
	struct bio_crypt_ctx *bc = NULL;
	struct blk_crypto_key *crypto_key = NULL;
	const struct bd_io_hisi_crypt_ctx *bd_io_crypt_ctx = &bd_io->bi_hisi_crypt_context;
	int ret = 0;

	if (bd_io_crypt_ctx->ci_key_len == 0)
		return ret;

	bio->ci_key_index = (int)((uint32_t)(bd_io_crypt_ctx->ci_key_index) & NVME_CRYPTO_CI_KEY_IDX_MASK);
	if ((bio->ci_key_index < 0) || (bio->ci_key_index > NVME_CRYPTO_CI_KEY_IDX_MAX)) {
		pr_err("ci_key_index is error, bd_io_key_index = %d, bio_key_index = %d\n", 
			bd_io_crypt_ctx->ci_key_index, bio->ci_key_index);
		return -EINVAL;
	}

	ci_metadata = (uint8_t *)kmalloc(BD_CRYPTO_METADATA_SIZE, GFP_KERNEL);
	if (ci_metadata == NULL) {
		pr_err("metadata buffer alloc fail\n");
		ret = -ENOMEM;
		goto out_ret;
	}

	ret = memcpy_s(ci_metadata, BD_CRYPTO_METADATA_SIZE, bd_io_crypt_ctx->ci_metadata,
		BD_CRYPTO_METADATA_SIZE);
	if (ret != 0)
		goto out_metadata;

	bio->ci_metadata = ci_metadata;

	bc = (struct bio_crypt_ctx *)kmalloc(sizeof(*bc), GFP_KERNEL);
	if (bc == NULL) {
		pr_err("crypto context buffer alloc failed\n");
		ret = -ENOMEM;
		goto out_metadata;
	}

	bc->bc_dun[0] = bd_io_crypt_ctx->index;
	bc->bc_dun[1] = 0; // padding the upper 8 bytes with 0

	crypto_key = (struct blk_crypto_key *)kmalloc(sizeof(*crypto_key), GFP_KERNEL);
	if (crypto_key == NULL) {
		pr_err("crypto key buffer alloc failed\n");
		ret = -ENOMEM;
		goto out_crypto_key;
	}

	crypto_key->crypto_cfg.data_unit_size = NVME_CRYPTO_DATA_UNIT_SIZE;
	crypto_key->crypto_cfg.dun_bytes = NVME_CRYPTO_DUN_BYTES;

	bc->bc_key = crypto_key;

	bio->bi_crypt_context = bc;
	return ret;

out_crypto_key:
	kfree(bc);
out_metadata:
	kfree(ci_metadata);
out_ret:
	return ret;
}
#endif

#ifdef CONFIG_HISI_INLINE_ENCRYPTION
static int bio_hw_crypt_set_ctx(struct bio *bio, const struct bd_io *bd_io)
{
	int ret;
	uint8_t *ci_key = NULL, *ci_metadata = NULL;
	uint8_t *bd_ci_key = NULL, *bd_ci_metadata = NULL;
	const struct bd_io_hisi_crypt_ctx *bd_io_crypt_ctx = &bd_io->bi_hisi_crypt_context;

	if (bd_io_crypt_ctx->ci_key_len == 0)
		return 0;

	ci_key = (uint8_t *)kmalloc(BD_CRYPTO_CI_KEY_LEN, GFP_KERNEL);
	if (ci_key == NULL)
		return -ENOMEM;

	ci_metadata = (uint8_t *)kmalloc(BD_CRYPTO_METADATA_SIZE, GFP_KERNEL);
	if (ci_metadata == NULL) {
		kfree(ci_key);
		return -ENOMEM;
	}

	bd_ci_key = (uint8_t *)bd_io_crypt_ctx->ci_key;
	bd_ci_metadata = (uint8_t *)bd_io_crypt_ctx->ci_metadata;

	ret = memcpy_s(ci_key, BD_CRYPTO_CI_KEY_LEN, bd_ci_key, BD_CRYPTO_CI_KEY_LEN);
	if (ret != 0) {
		kfree(ci_metadata);
		kfree(ci_key);
		return ret;
	}

	ret = memcpy_s(ci_metadata, BD_CRYPTO_METADATA_SIZE, bd_ci_metadata, BD_CRYPTO_METADATA_SIZE);
	if (ret != 0) {
		kfree(ci_metadata);
		kfree(ci_key);
		return ret;
	}

	bio->ci_key = ci_key;
	bio->ci_metadata = ci_metadata;
	bio->ci_key_len = bd_io_crypt_ctx->ci_key_len;
	bio->ci_key_index = bd_io_crypt_ctx->ci_key_index;
	bio->index = bd_io_crypt_ctx->index;

	return 0;

out:
	kfree(ci_key);
	kfree(ci_metadata);

	return -EFAULT;
}
#endif

static void *native_bio_alloc(void *filep,
			      const struct bd_io *bd_io,
			      void *private,
			      unsigned int bd_io_idx)
{
	struct block_device *bdev = NULL;
	struct bio *bio = NULL;
	struct bio_context *bio_ctx = NULL;
	struct bd_context *bd_ctx = (struct bd_context *)private;
	unsigned int rw = (unsigned int)bd_io->rw;

	BUG_ON(bd_ctx == NULL);
	bio = bio_alloc(GFP_KERNEL, 0xFF);
	if (bio == NULL) {
		return NULL;
	}
	bio_ctx = kmalloc(sizeof(*bio_ctx),  GFP_KERNEL);
	if (bio_ctx == NULL) {
		bio_put(bio);
		return NULL;
	}
	bio_ctx->block_ctx = bd_ctx->private;
	bio_ctx->bio_idx = bd_io_idx;
	bio_ctx->direct_map_page = false;
#ifdef CONFIG_LIBLINUX_ASYNC_END_IO
	INIT_WORK(&bio_ctx->work, end_bio_handle);
#endif

	set_bio_ctx(bio, bio_ctx);
	bdev = block_device_of_filep(filep);
	if(native_bio_discard_support(&rw, bd_io->bv_len, bdev, bio) < 0) {
		kfree(bio_ctx);
		set_bio_ctx(bio, NULL);
		bio_put(bio);
		return NULL;
	}

	bio->bi_iter.bi_sector = bd_io->block_offset;
	bio->bi_end_io = end_bio;
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)
	bio->bi_bdev = bdev;
	bio->bi_rw = rw;
#else
	/* ldk is not support MAS_BLK */
	bio_set_op_attrs(bio, rw, bd_io->io_flags & (~BD_FEATURE_MAS_BLK));
	bio_set_dev(bio, bdev);
#endif
#ifdef CONFIG_HISI_FALLBACK_ENCRYPTION
	if (bio_nvme_crypto_set_ctx(bio, bd_io)) {
		kfree(bio_ctx);
		set_bio_ctx(bio, NULL);
		bio_put(bio);
		return NULL;
	}
#endif
#ifdef CONFIG_HISI_INLINE_ENCRYPTION
	if (bio_hw_crypt_set_ctx(bio, bd_io)) {
		kfree(bio_ctx);
		set_bio_ctx(bio, NULL);
		bio_put(bio);
		return NULL;
	}
#endif
	return (void *)bio;
}

static bool __is_direct_access(struct block_device *bdev)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,10,0)
	if (bdev->bd_disk->fops->direct_access != NULL) {
		return true;
	}
#else
	struct request_queue *q = bdev_get_queue(bdev);
	if (blk_queue_dax(q)) {
		return true;
	}
#endif

	return false;
}

static int native_dev_check_feature(void *filep, int idx,
				    uint64_t features, uint64_t *supported_features)
{
	struct block_device *bdev = NULL;
	struct request_queue *q = NULL;
	char b[BDEVNAME_SIZE];
	uint64_t sup_features = 0;

	bdev = block_device_of_filep(filep);
	if (unlikely(bdev == NULL)) {
		pr_err("trying to access nonexistent block-device %s\n",
		       bdevname(bdev, b));
		return -ENODEV;
	}

	q = bdev_get_queue(bdev);
	if (unlikely(q == NULL)) {
		pr_err("trying to access nonexistent queue in block-device %s\n",
		       bdevname(bdev, b));
		return -EINVAL;
	}

	if ((features & BD_REQ_DISCARD) &&
	    blk_queue_discard(q)) {
		sup_features |= BD_REQ_DISCARD;
	}

	if ((features & BD_FEATURE_DAS) && __is_direct_access(bdev)) {
		sup_features |= BD_FEATURE_DAS;
	}

	if ((features & BD_FEATURE_RODEV) && bdev_read_only(bdev) != 0) {
		sup_features |= BD_FEATURE_RODEV;
	}

#ifdef CONFIG_HIGHMEM
	sup_features |= BD_FEATURE_DIRECTMAP;
#endif

	*supported_features = sup_features;
	return 0;
}

static struct page *prepare_page(unsigned int pagenum, uintptr_t vaddr)
{
	void *ptr = NULL;
	struct page *pages = NULL;
	unsigned int i;
	int ret;

	/* alloc pages for bio data */
	ptr = alloc_pages_exact(pagenum * PAGE_SIZE, GFP_KERNEL);
	if (ptr == NULL) {
		pr_err("prepare_page failed: ENOMEM\n");
		return NULL;
	}
	pages = virt_to_page(ptr);

	/* copy bio data to allocated pages */
	ret = memcpy_s(ptr, pagenum * PAGE_SIZE, (void *)vaddr, pagenum * PAGE_SIZE);
	BUG_ON(ret != 0);

	/* set origin data ptr to page->private */
	for (i = 0; i < pagenum; i++) {
		pages[i].private = vaddr + i * PAGE_SIZE;
	}

	return pages;
}

static int native_bio_add_data(void *_bio, uintptr_t vaddr, unsigned int bv_len)
{
	unsigned int i;
	struct bio *bio = (struct bio *)_bio;
	struct page *pages = NULL;
	struct page *page = NULL;
	unsigned int offset = bv_len % PAGE_SIZE;
	unsigned int pagenum = bv_len / PAGE_SIZE;

	if (bv_len == 0) {
		return -EINVAL;
	}
	if (offset) {
		pagenum += 1;
	}
	pages = prepare_page(pagenum, vaddr);
	if (pages == NULL) {
		return -ENOMEM;
	}
	page = pages;
	for (i = 0; i < pagenum; i++) {
		unsigned int len;
		unsigned int ret;

		if (offset && i == pagenum - 1) {
			len = offset;
		} else {
			len = PAGE_SIZE;
		}
		ret = (unsigned int)bio_add_page((struct bio *)bio, page, len, 0);
		if (ret != len) {
			pr_err("native_bio_add_page add page failed, "
			       "pagenum=%u, bv_len=%u, ret=%u\n",
			       pagenum, bv_len, ret);
			free_pages_exact(page_address(pages), pagenum * PAGE_SIZE);

			return -EFAULT;
		}
		page++;
	}
	return 0;
}

static int __direct_access(struct block_device *bdev, unsigned long offset,
			   unsigned long size, unsigned long long *paddr,
			   unsigned long *ret_size);
static int
native_direct_access(void *filep, unsigned long offset, unsigned long size,
		     unsigned long long *paddr, unsigned long *ret_size)
{
	struct block_device *bdev = NULL;
	char b[BDEVNAME_SIZE];

	if ((size & (SECTOR_SIZE - 1)) != 0 ||
		(offset & (SECTOR_SIZE - 1)) != 0) {
		pr_err("direct access request not aligned with SECTOR_SIZE\n");
		return -EINVAL;
	}
	bdev = block_device_of_filep(filep);
	if (unlikely(bdev == NULL)) {
		pr_err("trying to access nonexistent block-device %s\n",
		       bdevname(bdev, b));
		return -ENODEV;
	}

	return __direct_access(bdev, offset, size, paddr, ret_size);
}

static int __direct_access(struct block_device *bdev, unsigned long offset,
			   unsigned long size, unsigned long long *paddr,
			   unsigned long *ret_size)
{
	char b[BDEVNAME_SIZE];
	long ret;
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)
	void __pmem *kaddr = NULL;
#else
	void *kaddr = NULL;
#endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,10,0)
	unsigned long pfn = 0;

	if (bdev->bd_disk->fops->direct_access == NULL) {
		pr_err("direct access not supported by %s\n", bdevname(bdev, b));
		return -EOPNOTSUPP;
	}

	ret = bdev->bd_disk->fops->direct_access(bdev, offset >> SECTOR_SHIFT, &kaddr, &pfn);
	if (ret < 0) {
		return ret;
	}
	*paddr = PFN_PHYS(pfn);
#else
	struct dax_device *dax_dev = NULL;
	pgoff_t pgoff;
	pfn_t pfn;
	int id;

	if (!bdev_dax_supported(bdev, block_size(bdev))) {
		pr_err("dax not supported by %s\n", bdevname(bdev, b));
		return -EOPNOTSUPP;
	}

	ret = bdev_dax_pgoff(bdev, offset >> SECTOR_SHIFT, ALIGN(size, PAGE_SIZE), &pgoff);
	if (ret < 0) {
		return ret;
	}

	dax_dev = dax_get_by_host(bdev->bd_disk->disk_name);
	id = dax_read_lock();
	ret = dax_direct_access(dax_dev, pgoff, PHYS_PFN(ALIGN(size, PAGE_SIZE)), &kaddr, &pfn);
	dax_read_unlock(id);
	put_dax(dax_dev);
	if (ret < 0) {
		return ret;
	}

	*paddr = pfn_t_to_phys(pfn);
	ret = PFN_PHYS(ret);
#endif
	if ((unsigned long)ret > size) {
		*ret_size = size;
	} else {
		*ret_size = (unsigned long)ret;
	}
	return 0;
}

static int native_bio_add_page_by_pfn(void *_bio, unsigned long long pfn,
				      unsigned int len, unsigned int offset)
{
	struct bio *bio = (struct bio *)_bio;
	struct bio_context *bio_ctx = NULL;
	struct page *page = (struct page *)pfn_to_page(pfn);

	BUG_ON(bio == NULL);
	bio_ctx = bio_ctx(bio);
	BUG_ON(bio_ctx == NULL);

	bio_ctx->direct_map_page = true;

	return bio_add_page((struct bio *)_bio, page, len, offset);
}

static void native_bio_set_ctx(void *_bio, devhost_end_bio_fn_t devhost_end_bio_fn)
{
	struct bio *bio = (struct bio *)_bio;
	struct bio_context *bio_ctx = NULL;

	BUG_ON(bio == NULL);
	bio_ctx = bio_ctx(bio);
	BUG_ON(bio_ctx == NULL);

	bio_ctx->devhost_end_bio_fn = devhost_end_bio_fn;
}

struct devhost_block_ops blkops __read_mostly = {
	.bio_alloc = native_bio_alloc,
	.dev_check_feature = native_dev_check_feature,
	.bio_free = native_bio_free,
	.submit_bio = native_submit_bio,
	.bio_add_data = native_bio_add_data,
	.direct_access = native_direct_access,
	.bio_add_page_by_pfn = native_bio_add_page_by_pfn,
	.bio_set_ctx = native_bio_set_ctx,
};
