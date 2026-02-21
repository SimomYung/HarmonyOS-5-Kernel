/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Devhost operations in UDK block
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 12 15:00:00 2019
 */
#include <udk/block/udk_disk.h>

#include <inttypes.h>
#include <devhost/device.h>
#include <udk/mm.h>
#include <udk/log.h>
#include <udk/module.h>
#include <udk/api.h>
#include <udk/block/udk_block.h>
#include <udk/block/udk_block_mq.h>
#include <sys/mount.h>
#include <hongmeng/errno.h>
#include <libsysif/devhost/api.h>
#include <libstrict/strict.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_io/type.h>

#ifdef CONFIG_MAS_BLK
#include <udk/block/mas_blk/mas_blk.h>
#endif

#include "udk_block_internal.h"

#define bio_ctx(bio)			((bio)->bi_ctx)

#define BLK_IOCTL_DISCARD_DATA_SIZE	2


static struct udk_module *udk_disk_owner_of(const struct udk_disk *disk)
{
	if ((disk == NULL) || (disk->disk_fops == NULL) || (disk->disk_fops->owner == NULL)) {
		return NULL;
	}

	return (struct udk_module *)(*disk->disk_fops->owner);
}

/*
 * udk_get_disk - notify drivers to get disk info through @part
 * Every partitions is discoveried to part0
 *
 * @part: the partition which user want to get info from
 */
int udk_get_disk(struct udk_partition *part)
{
	struct udk_disk *disk = NULL;
	int ret = E_HM_OK;

	disk = part->udkdisk;
	if (disk == NULL) {
		udk_error("Fail to get disk from Part, name=%s\n", part->name);
		return E_HM_INVAL;
	}

	udk_mutex_lock(&part->lock);
	if (part->openers == DEVICE_OPEN_INVALID_FLAG) {
		udk_mutex_unlock(&part->lock);
		udk_warn("unable to get disk, it's being destroyed\n");
		return E_HM_INVAL;
	}

	/* this partition has already be opened, just increase the openers by one */
	if (part->openers > 0) {
		part->openers++;
		udk_mutex_unlock(&part->lock);
		return ret;
	}

	if (part->partno == 0) {
		/* Just do that when part0 is not opened */
		if (disk->disk_fops == NULL || disk->disk_fops->open == NULL) {
			udk_mutex_unlock(&part->lock);
			return E_HM_NOSYS;
		}

		ret = disk->disk_fops->open(disk, FMODE_RW);
		ret = posix2hmerrno(-ret);
		if (ret != E_HM_OK) {
			udk_mutex_unlock(&part->lock);
			return ret;
		}
	} else {
		/*
		 * This partitoion is not part0, So recursive to open through part0.
		 * Should be careful of the nested mutex lock!!!
		 */
		ret = udk_get_disk(&disk->part0);
		if (ret != E_HM_OK) {
			udk_mutex_unlock(&part->lock);
			return ret;
		}
	}

	part->openers++;
	udk_mutex_unlock(&part->lock);
	return ret;
}

static int udk_blkdev_fops_open(struct device_info *dev, void **ctx, unsigned int flags)
{
	struct blkdev_context *blkdev_ctx = NULL;
	struct udk_partition *part = NULL;
	struct udk_device *udkdev = NULL;
	struct udk_disk *disk = NULL;
	int ret;

	UNUSED(flags);
	if (dev == NULL || ctx == NULL) {
		return -EINVAL;
	}

	udkdev = udk_device_of(dev);
	if (udkdev == NULL) {
		return -EINVAL;
	}

	disk = udkdev_to_disk(udkdev);
	if (disk == NULL) {
		udk_error("Fail to get disk from udk device, name=%s\n", udkdev->name);
		return -EINVAL;
	}

	ret = udk_module_open(udk_disk_owner_of(disk));
	if (ret != 0) {
		udk_warn("udk module open failed, name=%s, %s\n",
			 dev->devname, strerror(-ret));
		return ret;
	}

	ret = udk_device_opener_inc(udkdev);
	if (ret != 0) {
		udk_error("udk_device_opener_inc failed, name=%s\n", dev->devname);
		udk_module_close(udk_disk_owner_of(disk));
		return ret;
	}

	blkdev_ctx = (struct blkdev_context *)malloc(sizeof(*blkdev_ctx));
	if (blkdev_ctx == NULL) {
		udk_device_opener_dec(udkdev);
		udk_module_close(udk_disk_owner_of(disk));
		return -ENOMEM;
	}

	part = udkdev_to_part(udkdev);
	ret = udk_get_disk(part);
	ret = -hmerrno2posix(ret);
	if (ret == 0) {
		blkdev_ctx->udkdev = udkdev;
		blkdev_ctx->priv = NULL;
		*ctx = blkdev_ctx;
	} else {
		free(blkdev_ctx);
		udk_device_opener_dec(udkdev);
		udk_module_close(udk_disk_owner_of(disk));
	}

	return ret;
}

/*
 * udk_put_disk - notify drivers to put disk info through @part
 * @part: the partition which user want to put info from
 */
int udk_put_disk(struct udk_partition *part)
{
	struct udk_disk *disk = NULL;
	int ret = E_HM_OK;

	disk = part->udkdisk;
	if (disk == NULL) {
		udk_error("Fail to get disk from Part, name=%s\n", part->name);
		return E_HM_INVAL;
	}

	udk_mutex_lock(&part->lock);
	if (part->openers <= 0) {
		udk_warn("Can't put the disk again, name=%s\n", disk->name);
		udk_mutex_unlock(&part->lock);
		return E_HM_RESFAULT;
	}
	part->openers--;
	/* Also open in other place, just return */
	if (part->openers > 0) {
		udk_mutex_unlock(&part->lock);
		return ret;
	}

	if (part->partno == 0) {
		if (disk->disk_fops == NULL || disk->disk_fops->release == NULL) {
			udk_mutex_unlock(&part->lock);
			return ret;
		}
		disk->disk_fops->release(disk, FMODE_RW);
	} else {
		/*
		 * this partition is not part0, So recursive to close through part0.
		 */
		ret = udk_put_disk(&disk->part0);
	}

	udk_mutex_unlock(&part->lock);
	return ret;
}

static int udk_blkdev_fops_close(void *ctx)
{
	struct blkdev_context *blkdev_ctx = NULL;
	struct udk_device *udkdev = NULL;
	struct udk_partition *part = NULL;
	struct udk_disk *disk = NULL;
	int ret;

	if (ctx == NULL) {
		return -EINVAL;
	}
	blkdev_ctx = (struct blkdev_context *)ctx;
	udkdev = blkdev_ctx->udkdev;
	if (udkdev == NULL) {
		return -EINVAL;
	}
	part = udkdev_to_part(udkdev);
	ret = udk_put_disk(part);
	ret = -hmerrno2posix(ret);
	if (ret == 0) {
		free(blkdev_ctx);
	}
	udk_device_opener_dec(udkdev);
	disk = udkdev_to_disk(udkdev);
	udk_module_close(udk_disk_owner_of(disk));

	return ret;
}

static int
udk_copy_blksize_to_user(struct udk_device *udkdev, void *argp, unsigned int request)
{
	uint64_t blksize;
	uint64_t nr_sects;
	uint32_t blksize_32;
	uint32_t sector_size = 1U; /* default get block size in 512-byte sectors */
	struct udk_partition *udkpart = NULL;

	udkpart = udkdev_to_part(udkdev);
	if (udkpart == NULL) {
		udk_error("Fail to get part info from udk device, name=%s\n",
			  udkdev->name);
		return -EINVAL;
	}
	nr_sects = udkpart->nr_sects;
	/* get block size in bytes (sector_num * 512) */
	if (request == BLKGETSIZE64) {
		sector_size = UDK_BLKSIZE_DEFAULT;
	}

	if (udk_is_compat_client()) {
		if ((nr_sects > UINT_MAX) || (nr_sects * sector_size > UINT_MAX)) {
			udk_warn("The size of block is overflow, sectors=%"PRIu64"\n", nr_sects);
		}
		blksize_32 = (uint32_t)nr_sects * sector_size;
		return udk_copy_to_user(argp, (void *)&blksize_32, sizeof(uint32_t));
	}

#if __SIZEOF_LONG__ == 4
	if ((nr_sects > ULONG_MAX) ||  (nr_sects * sector_size > ULONG_MAX)) {
		udk_warn("The size of block is overflow, sectors=%"PRIu64"\n", nr_sects);
	}
#endif
	blksize = nr_sects * sector_size;
	if ((nr_sects != 0UL) && (blksize / nr_sects) != sector_size) {
		udk_warn("The size of block is overflow, sectors=%"PRIu64"\n", nr_sects);
	}

	return udk_put_user_ulong(argp, (unsigned long)blksize);
}

#define min_sects(a, b) ((a) < (b) ? (a) : (b))
static int blk_ioctl_discard(struct udk_device *udkdev, void *argp)
{
	int ret;
	uint64_t max_discard_sects;
	uint64_t start;
	uint64_t nr_sects;
	uint64_t max_sects;
	uint64_t left_sects;
	uint64_t data[BLK_IOCTL_DISCARD_DATA_SIZE];
	struct udk_partition *udkpart = NULL;

	udkpart = udkdev_to_part(udkdev);
	if (udkpart == NULL) {
		udk_error("Fail to get part info from udk device, name=%s\n",
			  udkdev->name);
		return -EINVAL;
	}
	max_sects = udkpart->nr_sects;
	max_discard_sects = min_sects(UINT_MAX >> UDK_BLKSHIFT_DEFAULT,
				      udk_disk_obtain_max_discard_sectors(udkdev_to_disk(udkdev)));

	ret = udk_copy_from_user(data, argp, sizeof(data));
	if (ret != 0) {
		return -EINVAL;
	}
	if ((data[0] & (UDK_BLKSIZE_DEFAULT - 1)) != 0 ||
	    (data[1] & (UDK_BLKSIZE_DEFAULT - 1)) != 0) {
		return -EINVAL;
	}

	start = data[0] >> UDK_BLKSHIFT_DEFAULT;
	left_sects = data[1] >> UDK_BLKSHIFT_DEFAULT;

	if ((start + left_sects) > max_sects) {
		return -EOVERFLOW;
	}

	/* Discard I/Os with a large size need split */
	while (left_sects > 0) {
		nr_sects = min_sects(left_sects, max_discard_sects);
		ret = udk_blk_submit_cmd_sync(udkdev, UDK_REQ_DISCARD, start, nr_sects);
		if (ret < 0) {
			return ret;
		}
		left_sects -= nr_sects;
		start  += nr_sects;
	}

	return 0;
}

static int blk_ioctl_max_sectors(struct udk_disk *disk, void *argp)
{
	uint32_t max_sectors = udk_disk_obtain_max_sectors(disk);

	if (argp == NULL)
		return -EINVAL;

	return udk_copy_to_user(argp, (void *)(&max_sectors), sizeof(uint32_t));
}

static int udk_blkdev_fops_ioctl(void *ctx, unsigned int request,
				 unsigned long arg)
{
	struct blkdev_context *blkdev_ctx = NULL;
	struct udk_device *udkdev = NULL;
	void *argp = (void *)(uintptr_t)arg;
	struct udk_disk *disk = NULL;

	if (ctx == NULL) {
		return -EINVAL;
	}
	blkdev_ctx = (struct blkdev_context *)ctx;
	udkdev = blkdev_ctx->udkdev;
	if (udkdev == NULL) {
		return -EINVAL;
	}

	disk = udkdev_to_disk(udkdev);
	if (disk == NULL) {
		udk_error("Fail to get disk from udk device, name=%s\n", udkdev->name);
		return -EINVAL;
	}

	switch (request) {
	case BLKSSZGET: /* get block device logical block size */
		return udk_put_user_int(argp, (int)disk->logical_block_size);
	case BLKGETSIZE: /* get block size in 512-byte sectors */
	case BLKGETSIZE64: /* get device size in bytes */
		return udk_copy_blksize_to_user(udkdev, argp, request);
	case BLKDISCARD:
		return blk_ioctl_discard(udkdev, argp);
	case BLKSECTGET:
		return blk_ioctl_max_sectors(disk, argp);
	default:
		if (disk->disk_fops == NULL || disk->disk_fops->ioctl == NULL) {
			return -ENOSYS;
		}
		return disk->disk_fops->ioctl(disk, FMODE_RW, request, arg);
	}
}

struct devhost_fops udk_blkdev_ops __read_mostly = {
	.open   = udk_blkdev_fops_open,
	.ioctl  = udk_blkdev_fops_ioctl,
	.close  = udk_blkdev_fops_close,
	.read   = NULL,
	.write  = NULL,
	.mmap   = NULL,
	.poll   = NULL,
	.llseek = NULL,
	.read_ex = NULL,
	.write_ex = NULL,
	.owner	= NULL,
};

int udk_blkdev_config_devnode(struct device_info *info,
			      char *node_name, unsigned int size,
			      struct devnode_req *req)
{
	struct udk_device *udkdev = NULL;
	struct udk_disk *disk = NULL;
	struct udk_device_attr dev_attr;
	int ret = 0;

	if ((info == NULL) || (node_name == NULL) || (req == NULL)) {
		return -EINVAL;
	}

	udkdev = udk_device_of(info);
	disk = udkdev_to_disk(udkdev);
	if(disk->disk_fops == NULL) {
		return -EINVAL;
	}

	if (disk->disk_fops->config_device != NULL) {
		mem_zero_s(dev_attr);
		ret = disk->disk_fops->config_device(disk, &dev_attr);
		if (ret == 0) {
			req->mode = dev_attr.mode;
			req->uid = dev_attr.uid;
			req->gid = dev_attr.gid;
			NOFAIL(strncpy_s(node_name, size, dev_attr.name, strlen(dev_attr.name)));
		}
	} else {
		NOFAIL(strncpy_s(node_name, size, info->devname, strlen(info->devname)));
	}

	return ret;
}

static unsigned long udk_req_flags(unsigned long flag, bool is_empty_data)
{
	unsigned long udk_flag = 0;

	/* set udk request operation flags */
	if ((flag & REQ_DISCARD_SYSIF) != 0) {
		udk_flag |= UDK_REQ_DISCARD;
	} else if ((flag & REQ_GC_SYSIF) != 0) {
		udk_flag |= UDK_REQ_GC;
	} else if ((flag & REQ_PREFLUSH_SYSIF) != 0 && is_empty_data) {
		udk_flag |= UDK_REQ_FLUSH;
	} else if ((flag & REQ_WRITE_SYSIF) != 0) {
		udk_flag |= UDK_REQ_WRITE;
	} else {
		udk_flag |= UDK_REQ_READ;
	}

	/* set udk request other flags */
	if ((flag & REQ_FUA_SYSIF) != 0) {
		udk_flag |= UDK_REQ_FLAGS_FUA;
	}
	if ((flag & REQ_PREFLUSH_SYSIF) != 0) {
		udk_flag |= UDK_REQ_FLAGS_PREFLUSH;
	}
	if ((flag & UDK_FS_REQ_SYNC) != 0) {
		udk_flag |= UDK_REQ_FLAGS_SYNC;
	}
	if ((flag & UDK_FS_REQ_FG) != 0) {
		udk_flag |= UDK_REQ_FLAGS_FG;
	}
	if ((flag & UDK_FS_REQ_META) != 0) {
		udk_flag |= UDK_REQ_FLAGS_META;
	}
	if ((flag & UDK_FS_REQ_PRIO) != 0) {
		udk_flag |= UDK_REQ_FLAGS_PRIO;
	}
	if ((flag & UDK_FS_REQ_DIO) != 0) {
		udk_flag |= UDK_REQ_FLAGS_DIO;
	}
	return udk_flag;
}

static void init_bio_ic_data(const struct bd_io *bd_io, struct udk_bio *bio)
{
#if defined(CONFIG_HISI_INLINE_ENCRYPTION) || defined(CONFIG_UDK_FALLBACK_ENCRYPTION)
	struct hisi_inline_crypto_key *hisi_ic_key;

	if (bd_io->bi_hisi_crypt_context.ci_key_len == 0) {
		return;
	}

	hisi_ic_key = (struct hisi_inline_crypto_key *)malloc(sizeof(struct hisi_inline_crypto_key));
	if (hisi_ic_key == NULL) {
		return;
	}

	/* pass pointers only in fusion condition to reduce copy spends */
	NOFAIL(memcpy_s((uint8_t *)&hisi_ic_key->ci_key, sizeof(hisi_ic_key->ci_key),
			bd_io->bi_hisi_crypt_context.ci_key, sizeof(hisi_ic_key->ci_key)));
	NOFAIL(memcpy_s((uint8_t *)&hisi_ic_key->ci_metadata, sizeof(hisi_ic_key->ci_metadata),
			bd_io->bi_hisi_crypt_context.ci_metadata, sizeof(hisi_ic_key->ci_metadata)));

	hisi_ic_key->ci_key_len = bd_io->bi_hisi_crypt_context.ci_key_len;
	hisi_ic_key->ci_key_index = bd_io->bi_hisi_crypt_context.ci_key_index;
	hisi_ic_key->index = bd_io->bi_hisi_crypt_context.index;

	bio->crypto_key = hisi_ic_key;
#else
	UNUSED(bd_io, bio);
#endif
}

static int udk_bio_check_flags(struct udk_bio *bio, const struct bd_io *bd_io,
			       struct bd_context *bd_ctx)
{
	unsigned int bv_len = bd_io->bv_len;
	unsigned long rw_flag = (unsigned long)(unsigned int)bd_io->rw;
	unsigned long io_flag = (unsigned long)bd_io->io_flags & (~BD_FEATURE_MAS_BLK);
	bool non_data_io = false;

	if (bv_len == 0U) {
		if (bd_ctx->direct_map_paddr == 1U) {
			non_data_io = (bd_ctx->vec_cnt == 0U) ? true : false;
		} else {
			non_data_io = true;
		}
	}

	rw_flag = udk_req_flags(rw_flag | io_flag, non_data_io);
	if ((rw_flag & UDK_REQ_DISCARD) != 0) {
		if (bv_len == 0) {
			udk_error("Bio should not be zero when discarding\n");
			return -EINVAL;
		}
		bio->bi_size = bv_len;
	}

	if ((rw_flag & UDK_REQ_GC) != 0) {
		if (bv_len != 0) {
			udk_error("DEVHOST: native_bio_alloc bv_len err, bv_len=%u\n",
				  bv_len);
			return -EINVAL;
		}
		bio->bi_size = bv_len;
	}

	bio->bi_rw = rw_flag;
	return 0;
}

static void udk_blkops_bio_polling(void *filep, int rw)
{
	struct blkdev_context *blkdev_ctx = (struct blkdev_context *)filep;
	struct udk_disk *disk;

	disk = udkdev_to_disk(blkdev_ctx->udkdev);
	if (disk->bd_queue && disk->bd_queue->mq_ops->polling) {
		disk->bd_queue->mq_ops->polling(disk->bd_queue, rw);
	}
}

static bool udk_blkdev_support_polling(struct blkdev_context *blkdev)
{
	struct udk_disk *disk = udkdev_to_disk(blkdev->udkdev);

	return disk->bd_queue && disk->bd_queue->mq_ops->polling;
}

static void *udk_blkops_bio_alloc(void *filep, const struct bd_io *bd_io,
				  void *private, unsigned int bd_io_idx)
{
	struct blkdev_context *blkdev_ctx = NULL;
	struct udk_bio *bio = NULL;
	struct bd_context *bd_ctx = (struct bd_context *)private;
	bool async_io = ((bd_io->flags & REQ_SIO) == 0);
	int ret;

	if (bd_ctx == NULL || filep == NULL) {
		udk_error("bd_ctx or filep is NULL\n");
		return NULL;
	}

	bio = udk_bio_alloc(bd_ctx->vec_cnt, bd_ctx->direct_map_paddr != 0);
	if (bio == NULL) {
		udk_error("udk_bio_alloc bio failed\n");
		return NULL;
	}

	if ((bd_io->io_flags & BD_FEATURE_MAS_BLK) != 0) {
		bio->async_flush = true;
	}

	ret = udk_bio_check_flags(bio, bd_io, bd_ctx);
	if (ret < 0) {
		udk_bio_free(bio);
		return NULL;
	}

	blkdev_ctx = (struct blkdev_context *)filep;
	if (!udk_blkdev_support_polling(blkdev_ctx)) {
		bio->bi_rw &= ~UDK_REQ_FLAGS_DIO;
	}
	bio->bi_ioprio = (unsigned short)bd_io->io_priority;
	bio->bi_sector = bd_io->block_offset;
	bio->bi_dev = blkdev_ctx->udkdev;
	bio->bd_io_idx = bd_io_idx;
	bio->bi_ctx.direct_map_paddr = bd_ctx->direct_map_paddr == 1 ? true : false;
	init_bio_ic_data(bd_io, bio);
#ifdef CONFIG_MAS_BLK
	bio->dump_fs = bd_io->dump_fs;
#endif
	if (async_io) {
		bio->bi_ctx.async = true;
		set_udk_bio_context(bio, bd_ctx->private);
		bio->bi_ctx.bio_idx = bd_io_idx;
	}

	return (void *)bio;
}

int udk_blkops_submit_bio(void *bio, unsigned int flags);
int udk_blkops_submit_bio(void *bio, unsigned int flags)
{
	int ret;

	if (bio == NULL) {
		return -EINVAL;
	}

	ret = udk_blk_submit_bio((struct udk_bio *)bio, flags);
	if (ret != E_HM_OK) {
		udk_warn("Failed to submit udk_bio, err: %s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	return 0;
}

static void udk_blkops_bio_free(void *bio)
{
	struct udk_bio *udk_bio = (struct udk_bio *)bio;

	udk_bio_free(udk_bio);
}

static int udk_blkops_bio_add_data(void *bio, uintptr_t vaddr,
				   unsigned int bv_len)
{
	int ret;
	struct udk_bio *udk_bio = NULL;

	if (bio == NULL || bv_len == 0) {
		return -EINVAL;
	}

	udk_bio = (struct udk_bio *)bio;
	ret = udk_bio_add_data(udk_bio, (void *)vaddr, bv_len);
	if (ret != E_HM_OK) {
		udk_warn("udk_bio: add data failed, bv_len=%u, err: %s\n",
			 bv_len, hmstrerror(ret));
		return -EFAULT;
	}
	return 0;
}

static int udk_blkops_dev_check_feature(void *filep, int idx,
					uint64_t features,
					uint64_t *supported_features)
{
	struct blkdev_context *blkdev_ctx = NULL;
	struct udk_disk *disk = NULL;
	uint64_t sup_features = 0;

	UNUSED(idx);
	blkdev_ctx = (struct blkdev_context *)filep;
	if (blkdev_ctx == NULL || blkdev_ctx->udkdev == NULL) {
		udk_error("filep or udkdev is NULL\n");
		return -EINVAL;
	}
	disk = udkdev_to_disk(blkdev_ctx->udkdev);
	if (((features & REQ_DISCARD_SYSIF) != 0) &&
	    udk_disk_check_flag(disk, UDK_DISK_FLAG_DISCARD)) {
		sup_features |= REQ_DISCARD_SYSIF;
	}
	if (((features & REQ_SIO) != 0) &&
	    udk_disk_check_flag(disk, UDK_DISK_FLAG_SYNC_IO)) {
		sup_features |= REQ_SIO;
	}

	if (((features & REQ_GC_SYSIF) != 0) &&
	    udk_disk_check_flag(disk, UDK_DISK_FLAG_GC)) {
		sup_features |= REQ_GC_SYSIF;
	}
	if (((features & BD_FEATURE_DAS) != 0) &&
	    udk_disk_check_flag(disk, UDK_DISK_FLAG_DAS)) {
		sup_features |= BD_FEATURE_DAS;
	}

	if (((features & BD_FEATURE_DIRECTMAP) != 0) &&
	    udk_disk_check_flag(disk, UDK_DISK_FLAG_DIRECTMAP)) {
		sup_features |= BD_FEATURE_DIRECTMAP;
	}

	if (((features & BD_FEATURE_RODEV) != 0) &&
	    udk_disk_check_flag(disk, UDK_DISK_FLAG_RODEV)) {
		sup_features |= BD_FEATURE_RODEV;
	}

#ifdef CONFIG_MAS_BLK
	sup_features |= BD_FEATURE_MAS_BLK;
	if (mas_blk_support_feature(disk->bd_queue,
				    BLK_LLD_FLUSH_REDUCE_SUPPORT)) {
		sup_features |= BD_FEATURE_FLUSH_REDUCE;
	}
#endif

	*supported_features = sup_features;
	return 0;
}

static int
udk_blkops_direct_access(void *filep,
			 unsigned long offset, unsigned long size,
			 unsigned long long *paddr, unsigned long *ret_size)
{
	struct blkdev_context *blkdev_ctx = NULL;
	struct udk_disk *disk = NULL;

	blkdev_ctx = (struct blkdev_context *)filep;
	if (blkdev_ctx == NULL || blkdev_ctx->udkdev == NULL) {
		udk_error("filep or udkdev is NULL\n");
		return -EINVAL;
	}
	disk = udkdev_to_disk(blkdev_ctx->udkdev);
	if (!udk_disk_check_flag(disk, UDK_DISK_FLAG_DAS)) {
		udk_error("Direct access not supported, name=%s\n", disk->name);
		return -ENOTSUP;
	}
	if (disk->disk_fops == NULL || disk->disk_fops->direct_access == NULL) {
		udk_error("No direct access function, for name=%s\n", disk->name);
		return -EINVAL;
	}
	return disk->disk_fops->direct_access(disk, offset, size, paddr, ret_size);
}

static int native_bio_add_pfn(void *_bio, unsigned long long pfn,
			      unsigned int len, unsigned int offset)
{
	struct udk_bio *bio = (struct udk_bio *)_bio;
	struct udk_bio_pvec *bpv = NULL;

	if (bio == NULL) {
		udk_error("bio is NULL\n");
		return -EINVAL;
	}

	if (len == 0) {
		udk_error("len is 0\n");
		return -EINVAL;
	}

	if (bio->bi_vec_num >= bio->bi_vec_size) {
		return -EIO;
	}

	bpv = &bio->bio_io_pvec[bio->bi_vec_num];
	bpv->pfn = pfn;
	bpv->offset = offset;
	bpv->len = len;
	bio->bi_vec_num++;
	bio->bi_size += len;

	return 0;
}

static void native_bio_set_ctx(void *_bio, devhost_end_bio_fn_t devhost_end_bio_fn)
{
	struct udk_bio *bio = (struct udk_bio *)_bio;
	struct udk_bio_context *bio_ctx = NULL;

	if (bio == NULL) {
		udk_error("bio is NULL\n");
		return;
	}
	bio_ctx = &bio->bi_ctx;

	if (bio_ctx->async) {
		bio_ctx->devhost_end_bio_fn = devhost_end_bio_fn;
	}
}

struct devhost_block_ops udk_blkops __read_mostly = {
	.bio_alloc = udk_blkops_bio_alloc,
	.bio_free = udk_blkops_bio_free,
	.submit_bio = udk_blkops_submit_bio,
	.bio_add_data = udk_blkops_bio_add_data,
	.dev_check_feature = udk_blkops_dev_check_feature,
	.direct_access = udk_blkops_direct_access,
	.bio_add_page_by_pfn = native_bio_add_pfn,
	.bio_set_ctx = native_bio_set_ctx,
	.bio_polling = udk_blkops_bio_polling,
};

static struct udk_internal_blkops __udk_internal_blkops __read_mostly = {
	.submit_bio = udk_blkops_submit_bio,
	.bio_add_data = udk_blkops_bio_add_data,
};

struct udk_internal_blkops *udk_internal_blkops_default(void)
{
	return &__udk_internal_blkops;
}
