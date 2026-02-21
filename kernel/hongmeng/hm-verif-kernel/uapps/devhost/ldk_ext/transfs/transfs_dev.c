/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Transfs device informations
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 21 16:00:39 2021
 */

#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libsysif/devhost/bd_types.h>
#include <libalgo/id_generator.h>
#include <libhmsync/raw_rwlock.h>
#include <vfs.h>
#include <devhost/transfs.h>

#include "table.h"
#include "transfs_dev.h"

#define TRANSFS_BIO_MAX_PAGES (128)

struct transfs_bio {
	int rw;
	unsigned int sb_index;
	unsigned int v_index;
	unsigned int buf_len;
	char *buf_addr;
	unsigned long long offset;
	unsigned long long pfn[TRANSFS_BIO_MAX_PAGES];
	void *block_ctx;
	unsigned int pfn_cnt;
	unsigned int bio_idx;
	unsigned int t_flags;
	bool direct_map_page;
};

#define TRANS_IO_READ 0
#define TRANS_IO_WRITE 1

#ifndef SECTOR_SHIFT
#ifdef UDK_BLKSHIFT_DEFAULT
#define SECTOR_SHIFT UDK_BLKSHIFT_DEFAULT
#else
#define SECTOR_SHIFT (9)
#endif
#endif
#define sec2blk(ba, block_shift) ((ba) >> ((block_shift) - SECTOR_SHIFT))

#define trans_io_sector_mask(bits) ((1ULL << (bits)) - 1)
#define trans_io_sector(sec, bits) ((sec) & trans_io_sector_mask(bits))
#define trans_io_v_index(sec, bits) ((sec) >> (bits))

#define TRANSFS_BIO_DIRECT_MAP

static void *transfs_bio_alloc(void *filep,
			       const struct bd_io *bd_io,
			       void *private,
			       unsigned int bd_io_idx)
{
	struct transfs_bio *t_bio = NULL;
	struct trans_dev_file *trans_file = NULL;
	unsigned long long trans_io_sector;
	struct bd_context *bd_ctx = (struct bd_context *)private;
	uint64_t block_offset;

	BUG_ON(bd_ctx == NULL);
	if (bd_io->rw != TRANS_IO_READ && bd_io->rw != TRANS_IO_WRITE) {
		hm_warn("Unknown transfs io type:%d\n", bd_io->rw);
		return NULL;
	}

	t_bio = malloc(sizeof(struct transfs_bio));
	if (t_bio == NULL) {
		return NULL;
	}
	mem_zero_s(*t_bio);

	t_bio->block_ctx = bd_ctx->private;
	t_bio->bio_idx = bd_io_idx;
	trans_file = (struct trans_dev_file *)filep;
	t_bio->sb_index = trans_file->sb_index;
	block_offset = sec2blk(bd_io->block_offset, trans_file->block_size_shift);
	t_bio->v_index = (unsigned int)trans_io_v_index(block_offset,
							trans_file->sector_node_bits);
	trans_io_sector = trans_io_sector(block_offset,
					  trans_file->sector_node_bits);
	t_bio->offset = trans_io_sector << trans_file->block_size_shift;
	t_bio->rw = bd_io->rw;
	t_bio->t_flags = bd_io->flags;

#ifdef TRANSFS_BIO_DIRECT_MAP
	t_bio->direct_map_page = true;
#else
	t_bio->direct_map_page = false;
#endif

	return t_bio;
}

static void transfs_bio_free(void *_bio)
{
	BUG_ON(_bio == NULL);
	struct transfs_bio *t_bio = (struct transfs_bio *)_bio;
	free(t_bio);
}

static int transfs_do_submit_bio(struct transfs_bio *t_bio)
{
	int err;
	struct vfsmount *vmnt = NULL;
	struct inode *inode = NULL;
	struct devhost_transfs_ops *trans_ops = NULL;
	char *bio_data_addr = NULL;

	if (t_bio->rw != TRANS_IO_READ && t_bio->rw != TRANS_IO_WRITE) {
		hm_warn("Unknown transfs io type:%d\n", t_bio->rw);
		return -hmerrno2posix(E_HM_IO);
	}

	vmnt = get_vmnt_by_index(t_bio->sb_index);
	if (vmnt == NULL) {
		return -hmerrno2posix(E_HM_INVAL);
	}

	inode = get_inode_by_index(t_bio->sb_index, t_bio->v_index);
	if (inode == NULL) {
		return -hmerrno2posix(E_HM_INVAL);
	}

	trans_ops = get_ops_by_index(t_bio->sb_index);
	if (trans_ops == NULL) {
		return -hmerrno2posix(E_HM_INVAL);
	}

	bio_data_addr = (t_bio->direct_map_page ? (char *)t_bio->pfn : t_bio->buf_addr);
	if (t_bio->rw == TRANS_IO_READ) {
		err = trans_ops->io_read(vmnt, inode, bio_data_addr,
					 t_bio->buf_len, t_bio->offset,
					 t_bio->direct_map_page);
	} else {
		err = trans_ops->io_write(vmnt, inode, bio_data_addr,
					  t_bio->buf_len, t_bio->offset,
					  t_bio->direct_map_page);
		if ((unsigned int)err != t_bio->buf_len) {
			err = -hmerrno2posix(E_HM_IO);
		}
	}
	if ((t_bio->t_flags & REQ_SIO) == 0) {
		err = devhost_block_queue_end_io(t_bio->block_ctx, err >= 0 ? 0 : err,
					   t_bio->bio_idx);
		if (err != 0) {
			hm_warn("trans end io failed, err:%d\n", err);
		}
	} else if (err < 0) {
		return err;
	}

	transfs_bio_free(t_bio);

	return 0;
}

static int transfs_submit_bio(void *bio, unsigned int flags)
{
	BUG_ON(bio == NULL);
	UNUSED(flags);
	struct transfs_bio *t_bio = (struct transfs_bio *)bio;

	return transfs_do_submit_bio(t_bio);
}

static int transfs_bio_add_data(void *_bio, uintptr_t vaddr, unsigned int bv_len)
{
	BUG_ON(_bio == NULL);
	struct transfs_bio *t_bio = (struct transfs_bio *)_bio;

	t_bio->buf_addr = (char *)vaddr;
	t_bio->buf_len = bv_len;

	return 0;
}

static int transfs_bio_add_page_by_pfn(void *_bio, unsigned long long pfn,
				       unsigned int len, unsigned int offset)
{
	BUG_ON(_bio == NULL);
	struct transfs_bio *t_bio = (struct transfs_bio *)_bio;

	BUG_ON(!t_bio->direct_map_page);
	/*
	 * no need to check offset and len, just add page to
	 * mapping is enough.
	 */
	UNUSED(offset);

	if (t_bio->pfn_cnt >= TRANSFS_BIO_MAX_PAGES) {
		return E_HM_INVAL;
	}

	t_bio->pfn[t_bio->pfn_cnt] = pfn;
	t_bio->pfn_cnt++;
	t_bio->buf_len += len;

	return 0;
}

static int transfs_dev_check_feature(void *filep, int idx,
				     uint64_t features,
				     uint64_t *supported_features)
{
	UNUSED(filep, idx, features);
	*supported_features = 0;
	return 0;
}

static struct devhost_block_ops transfs_blkops __read_mostly = {
	.bio_alloc = transfs_bio_alloc,
	.dev_check_feature = transfs_dev_check_feature,
	.bio_free = transfs_bio_free,
	.submit_bio = transfs_submit_bio,
	.bio_add_data = transfs_bio_add_data,
	.direct_access = NULL,
#ifdef TRANSFS_BIO_DIRECT_MAP
	.bio_add_page_by_pfn = transfs_bio_add_page_by_pfn,
#else
	.bio_add_page_by_pfn = NULL,
#endif
	.bio_set_ctx = NULL,
};

static int transfs_dev_open(struct device_info *dev, void **ctx, unsigned int flags)
{
	struct trans_dev_file *trans_file = NULL;

	UNUSED(dev, flags);

	trans_file = malloc(sizeof(struct trans_dev_file));
	if (trans_file == NULL) {
		return -hmerrno2posix(E_HM_NOMEM);
	}

	*ctx = trans_file;

	return 0;
}

static int transfs_dev_close(void *ctx)
{
	struct trans_dev_file *trans_file = NULL;

	if (ctx == NULL) {
		return -hmerrno2posix(E_HM_INVAL);
	}
	trans_file = (struct trans_dev_file *)ctx;
	free(trans_file);

	return 0;
}

static struct devhost_fops transfs_fops = {
	.open = transfs_dev_open,
	.close = transfs_dev_close,
	.read = NULL,
	.write = NULL,
	.ioctl = NULL,
	.fcntl = NULL,
	.mmap = NULL,
	.poll = NULL,
	.llseek = NULL,
	.owner = NULL,
	.read_ex = NULL,
	.write_ex = NULL,
};

#define TRANSFS_NAME "transfs"
#define TRANSFS_DEVICE_NAME_LEN 16
#define TRANSFS_DEVICE_MAX_NUM 512

static struct device_info *g_transfs_parent_devinfo = NULL;
static struct id_generator *g_trans_id_gen = NULL;

struct transfs_devinfo {
	struct device_info devinfo;
	unsigned int devnum;
	char devname[];
};

static inline void transfs_put_devnum(unsigned int devnum)
{
	id_generator_free(g_trans_id_gen, devnum);
}

static void transfs_devinfo_destroy(struct device_info *devinfo)
{
	struct transfs_devinfo *t_devinfo = NULL;

	t_devinfo = container_of(devinfo, struct transfs_devinfo, devinfo);
	BUG_ON(t_devinfo == NULL);
	if (t_devinfo->devnum != 0) {
		transfs_put_devnum(t_devinfo->devnum);
	}
	free(t_devinfo);
}

int transfs_parent_device_init(void)
{
	int ret;
	struct id_generator_attr attr;
	struct transfs_devinfo *t_devinfo = NULL;

	/* alloc an id generator */
	mem_zero_s(attr);
	attr.min_index = 1;
	attr.max_index = TRANSFS_DEVICE_MAX_NUM;
	ret = id_generator_create(&attr, &g_trans_id_gen);
	if (ret != E_HM_OK) {
		hm_warn("transfs: alloc device id generator failed:%s",
			hmstrerror(ret));
		return ret;
	}

	t_devinfo = calloc(1, sizeof(struct transfs_devinfo) + TRANSFS_DEVICE_NAME_LEN);
	if (t_devinfo == NULL) {
		ret = E_HM_NOMEM;
		goto err_free_id_gec;
	}

	ret = snprintf_s(t_devinfo->devname, TRANSFS_DEVICE_NAME_LEN,
			 TRANSFS_DEVICE_NAME_LEN - 1, "%s", TRANSFS_NAME);
	BUG_ON(ret < 0);

	t_devinfo->devinfo.devname = t_devinfo->devname;
	t_devinfo->devinfo.devid = -1;
	ret = devhost_device_init(&t_devinfo->devinfo);
	if (ret != 0) {
		ret = posix2hmerrno(-ret);
		goto err_free_t_dev;
	}
	t_devinfo->devinfo.release = transfs_devinfo_destroy;

	ret = devhost_register_device(&t_devinfo->devinfo);
	if (ret != 0) {
		devhost_device_destroy(&t_devinfo->devinfo);
		ret = posix2hmerrno(-ret);
		goto err_free_id_gec;
	}

	g_transfs_parent_devinfo = &t_devinfo->devinfo;

	return 0;

err_free_t_dev:
	free(t_devinfo);
err_free_id_gec:
	id_generator_destroy(g_trans_id_gen);

	return ret;
}

static int transfs_get_devnum(unsigned int *devnum_ret)
{
	unsigned int devnum;
	int ret;

	ret = id_generator_alloc(g_trans_id_gen, &devnum);
	if (ret != E_HM_OK) {
		return ret;
	}

	*devnum_ret = devnum;
	return ret;
}

int transfs_device_init(struct device_info **devinfo_ret, uint32_t *dev_id_ret)
{
	struct transfs_devinfo *t_devinfo = NULL;
	unsigned int devnum;
	uint32_t dev_id;
	int ret;

	t_devinfo = calloc(1, sizeof(struct transfs_devinfo) + TRANSFS_DEVICE_NAME_LEN);
	if (t_devinfo == NULL) {
		return E_HM_NOMEM;
	}

	BUG_ON(g_transfs_parent_devinfo == NULL);
	ret = transfs_get_devnum(&devnum);
	if (ret != E_HM_OK) {
		free(t_devinfo);
		return ret;
	}
	t_devinfo->devnum = devnum;
	ret = snprintf_s(t_devinfo->devname, TRANSFS_DEVICE_NAME_LEN,
			 TRANSFS_DEVICE_NAME_LEN - 1, "%s_%u", TRANSFS_NAME, devnum);
	BUG_ON(ret < 0);
	t_devinfo->devinfo.parent = g_transfs_parent_devinfo->devid;
	dev_id = MKDEV(0, devnum);

	t_devinfo->devinfo.devname = t_devinfo->devname;
	t_devinfo->devinfo.devid = -1;
	ret = devhost_device_init(&t_devinfo->devinfo);
	if (ret != 0) {
		free(t_devinfo);
		return posix2hmerrno(-ret);
	}
	t_devinfo->devinfo.release = transfs_devinfo_destroy;

	devhost_device_capable_devfs(&t_devinfo->devinfo, dev_id,
				     (int)MODE_BLK, NULL, &transfs_fops);
	devhost_device_capable_blk(&t_devinfo->devinfo, &transfs_blkops);
#ifdef TRANSFS_BIO_DIRECT_MAP
	devhost_device_capable_directmap_shm(&t_devinfo->devinfo);
#endif

	ret = devhost_register_device(&t_devinfo->devinfo);
	if (ret != 0) {
		devhost_device_destroy(&t_devinfo->devinfo);
		return posix2hmerrno(-ret);
	}

	*dev_id_ret = dev_id;
	*devinfo_ret = &t_devinfo->devinfo;

	return E_HM_OK;
}

int transfs_device_destroy(struct device_info *devinfo)
{
	int ret;

	ret = devhost_unregister_device(devinfo);
	if (ret != 0) {
		return posix2hmerrno(-ret);
	}

	devhost_device_destroy(devinfo);

	return E_HM_OK;
}
