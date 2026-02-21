/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: UDK block operations to adapt sysfs
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 21 17:00:00 2023
 */
#include <hmnet/hm_socket.h>
#include <hmasm/lsyscall.h>
#include <hmnet/hm_netlink.h>
#include <udk/mm.h>
#include <udk/log.h>
#include <udk/iolib.h>
#include <udk/sysfs.h>
#include <udk/delay.h>
#include <udk/block/udk_block_mq.h>
#include <udk/block/udk_disk.h>
#include <internal/sysfs.h>

#ifdef CONFIG_MAS_BLK
#include <udk/block/mas_blk/mas_blk.h>
#endif

#include "udk_block_internal.h"

#define NVME_PART0_NAME_LEN	7
#define NVME_NAME_LEN	4
#define NVME_PREFIX_NAME_LEN	5
#define NVME_COMMON_PREFIX_NAME	"nvme"
#define SD_COMMON_PREFIX_NAME	"sd"
#define SD_PREFIX_NAME_LEN	2
#define SD_PART0_NAME_LEN	3
#define SD_PREFIX_PATH_FORMAT	"platform/%s/block"
#define CLASS_BLOCK_FORMAT	"class/block/%s"

#define SUBSYSTEM_BLOCK		"block"
#define DEVTYPE_PARTITION	"partition"
#define DEVTYPE_DISK		"disk"

#define SD_UEVENT_BUF_LEN	128
#define SD_PART_UEVENT_BUF_FORMAT	"MAJOR=%u\nMINOR=%u\nDEVNAME=%s\nDEVTYPE=%s\nPARTN=%u\nPARTNAME=%s\n"
#define SD_DISK_UEVENT_BUF_FORMAT	"MAJOR=%u\nMINOR=%u\nDEVNAME=%s\nDEVTYPE=%s\n"

#define UEVENT_WRITE_BUF_LEN	16
#define SD_PART_DEVPATH_FORMAT		"/devices/platform/%s/block/sd%c/%s%c"
#define SD_DISK_DEVPATH_FORMAT		"/devices/platform/%s/block/sd%c%c"
#define SD_PART_NETLINK_BUF_FORMAT	\
"ACTION=%s%cDEVPATH=%s%cSUBSYSTEM=%s%cMAJOR=%u%cMINOR=%u%cDEVNAME=%s%cDEVTYPE=%s%cPARTN=%u%cPARTNAME=%s%c"
#define SD_DISK_NETLINK_BUF_FORMAT	\
"ACTION=%s%cDEVPATH=%s%cSUBSYSTEM=%s%cMAJOR=%u%cMINOR=%u%cDEVNAME=%s%cDEVTYPE=%s%c"

/* Need to traverse udk_disk list later */
#define SDD_MAJOR		12
#define SDD_MINOR		48
#define DISKSTATS_BUF_LEN	512
#define DISKSTATS_BUF_FORMAT	"%4d %7d %s %lu %lu %lu %u %lu %lu %lu %u %u %u %u %lu %lu %lu %u %lu %u\n"
#define DISKSTATS_EXTRA_BUF_FORMAT	"%4d %7d %s %lu %lu %lu %lu %lu %lu\n"

#define SD_QOS_BUF_LEN		16
static struct udk_mutex diskstats_lock = UDK_MUTEX_INIT;

static int diskstats_print(struct udk_partition part, char *buf, size_t pos, size_t *rsize, size_t size,
			   size_t *sum_size)
{
	int ret;
	size_t buf_size;
	char src[DISKSTATS_BUF_LEN] = {0};
	size_t offset = 0;

	ret = snprintf_s(src, DISKSTATS_BUF_LEN, DISKSTATS_BUF_LEN - 1, DISKSTATS_BUF_FORMAT,
			 MAJOR(part.udkdev.devt), MINOR(part.udkdev.devt), part.udkdev.name,
			 (unsigned long)stat_atomic32_read(&part.diskstats.ios[STAT_READ]),
			 (unsigned long)stat_atomic32_read(&part.diskstats.merges[STAT_READ]),
			 (unsigned long)stat_atomic32_read(&part.diskstats.sectors[STAT_READ]),
			 (unsigned int)(stat_atomic64_read(&part.diskstats.nsecs[STAT_READ]) / __NSEC_PER_MSEC),
			 (unsigned long)stat_atomic32_read(&part.diskstats.ios[STAT_WRITE]),
			 (unsigned long)stat_atomic32_read(&part.diskstats.merges[STAT_WRITE]),
			 (unsigned long)stat_atomic32_read(&part.diskstats.sectors[STAT_WRITE]),
			 (unsigned int)(stat_atomic64_read(&part.diskstats.nsecs[STAT_WRITE]) / __NSEC_PER_MSEC),
			 (unsigned int)stat_atomic32_read(&part.diskstats.inflight),
			 (unsigned int)(stat_atomic64_read(&part.diskstats.io_ticks) / __NSEC_PER_MSEC),
			 (unsigned int)((stat_atomic64_read(&part.diskstats.nsecs[STAT_READ]) +
			 stat_atomic64_read(&part.diskstats.nsecs[STAT_WRITE]) +
			 stat_atomic64_read(&part.diskstats.nsecs[STAT_FLUSH]) +
			 stat_atomic64_read(&part.diskstats.nsecs[STAT_DISCARD])) / __NSEC_PER_MSEC),
			 (unsigned long)stat_atomic32_read(&part.diskstats.ios[STAT_DISCARD]),
			 (unsigned long)stat_atomic32_read(&part.diskstats.merges[STAT_DISCARD]),
			 (unsigned long)stat_atomic32_read(&part.diskstats.sectors[STAT_DISCARD]),
			 (unsigned int)(stat_atomic64_read(&part.diskstats.nsecs[STAT_DISCARD]) / __NSEC_PER_MSEC),
			 (unsigned long)stat_atomic32_read(&part.diskstats.ios[STAT_FLUSH]),
			 (unsigned int)(stat_atomic64_read(&part.diskstats.nsecs[STAT_FLUSH]) / __NSEC_PER_MSEC));
	if (ret < 0) {
		udk_warn("format diskstats temp_buf failed, ret=%d\n", ret);
		return ret;
	}
	buf_size = (size_t)(unsigned int)ret;

	udk_mutex_lock(&diskstats_lock);
	/* If pos is exist, do not copy the buf to src until reach the pos. */
	if (pos > 0 && pos > *sum_size) {
		offset = pos - *sum_size;
		if (offset >= buf_size) {
			*sum_size += buf_size;
			udk_mutex_unlock(&diskstats_lock);
			return 0;
		} else {
			*sum_size += offset;
			buf_size -= offset;
		}
	}
	udk_mutex_unlock(&diskstats_lock);

	/* Return when size is not enough. */
	if (size < (*rsize + buf_size)) {
		buf_size = size - *rsize;
	}

	ret = udk_copy_to_user((void *)(buf + *rsize), src + offset, buf_size);
	if (ret != 0) {
		udk_warn("copy_to_user failed, ret=%d\n", ret);
		return ret;
	}
	*rsize += buf_size;
	*sum_size += buf_size;
	if (size <= *rsize) {
		return -EOVERFLOW;
	}

	return ret;
}

static int diskstats_extra_print(struct udk_partition part, char *buf, size_t pos, size_t *rsize,
	size_t size, size_t *sum_size)
{
	int ret;
	size_t buf_size;
	char src[DISKSTATS_BUF_LEN] = {0};
	size_t offset = 0;

	ret = snprintf_s(src, DISKSTATS_BUF_LEN, DISKSTATS_BUF_LEN - 1, DISKSTATS_EXTRA_BUF_FORMAT,
			 MAJOR(part.udkdev.devt), MINOR(part.udkdev.devt), part.udkdev.name,
			 (unsigned long)((stat_atomic64_read(&part.diskstats.nsecs[STAT_READ]) -
				 stat_atomic64_read(&part.diskstats.nsecs_margin[STAT_READ])) / __NSEC_PER_USEC /
				 (stat_atomic32_read(&part.diskstats.ios[STAT_READ]) -
				 stat_atomic32_read(&part.diskstats.ios_margin[STAT_READ]))),
			 (unsigned long)((stat_atomic64_read(&part.diskstats.nsecs[STAT_WRITE]) -
				 stat_atomic64_read(&part.diskstats.nsecs_margin[STAT_WRITE])) / __NSEC_PER_USEC /
				 (stat_atomic32_read(&part.diskstats.ios[STAT_WRITE]) -
				 stat_atomic32_read(&part.diskstats.ios_margin[STAT_WRITE]))),
			 (unsigned long)((stat_atomic64_read(&part.diskstats.nsecs[STAT_DISCARD]) -
				 stat_atomic64_read(&part.diskstats.nsecs_margin[STAT_DISCARD])) / __NSEC_PER_USEC /
				 (stat_atomic32_read(&part.diskstats.ios[STAT_DISCARD]) -
				 stat_atomic32_read(&part.diskstats.ios_margin[STAT_DISCARD]))),
			 (unsigned long)stat_atomic64_read(&part.diskstats.max_nsecs[STAT_READ]) / __NSEC_PER_USEC,
			 (unsigned long)stat_atomic64_read(&part.diskstats.max_nsecs[STAT_WRITE]) / __NSEC_PER_USEC,
			 (unsigned long)stat_atomic64_read(&part.diskstats.max_nsecs[STAT_DISCARD]) / __NSEC_PER_USEC);
	if (ret < 0) {
		udk_warn("format diskstats temp_buf failed, ret=%d\n", ret);
		return ret;
	}
	buf_size = (size_t)(unsigned int)ret;

	udk_mutex_lock(&diskstats_lock);
	/* If pos is exist, do not copy the buf to src until reach the pos. */
	if (pos > 0 && pos > *sum_size) {
		offset = pos - *sum_size;
		if (offset >= buf_size) {
			*sum_size += buf_size;
			udk_mutex_unlock(&diskstats_lock);
			return 0;
		} else {
			*sum_size += offset;
			buf_size -= offset;
		}
	}
	udk_mutex_unlock(&diskstats_lock);

	/* Return when size is not enough. */
	if (size < (*rsize + buf_size)) {
		buf_size = size - *rsize;
	}

	ret = udk_copy_to_user((void *)(buf + *rsize), src + offset, buf_size);
	if (ret != 0) {
		udk_warn("copy_to_user failed, ret=%d\n", ret);
		return ret;
	}
	*rsize += buf_size;
	*sum_size += buf_size;
	if (size <= *rsize) {
		return -EOVERFLOW;
	}

	return ret;
}

static int sysfs_diskstats_print(struct udk_disk *disk, char *buf, size_t pos, size_t *rsize,
				 size_t size, size_t *sum_size)
{
	int ret;

	ret = diskstats_print(disk->part0, buf, pos, rsize, size, sum_size);
	if (ret == 0) {
		for (int i = 0; i < MAX_PARTITIONS; i++) {
			if (disk->parts[i] != NULL) {
				ret = diskstats_print(*disk->parts[i], buf, pos, rsize, size, sum_size);
				if (ret < 0) {
					break;
				}
			}
		}
	}

	return ret;
}

static int sysfs_diskstats_read(void *ctx, void *dst, size_t pos, size_t size, size_t *rsize)
{
	UNUSED(ctx);
	int ret = 0;
	size_t sum_size = 0;

	ret = udk_disklist_foreach(sysfs_diskstats_print, (char *)dst, pos, rsize, size, &sum_size);

	return ret == -EOVERFLOW ? 0 : ret;
}

static struct udk_sysfs_fops diskstats_fops = {
	.read = sysfs_diskstats_read,
	.write = NULL,
};

static int sysfs_diskstats_extra_print(struct udk_disk *disk, char *buf, size_t pos, size_t *rsize,
	size_t size, size_t *sum_size)
{
	int ret;

	ret = diskstats_extra_print(disk->part0, buf, pos, rsize, size, sum_size);
	if (ret == 0) {
		for (int i = 0; i < MAX_PARTITIONS; i++) {
			if (disk->parts[i] != NULL) {
				ret = diskstats_extra_print(*disk->parts[i], buf, pos, rsize, size, sum_size);
			}
		}
	}

	return ret;
}

static int sysfs_diskstats_extra_read(void *ctx, void *dst, size_t pos, size_t size, size_t *rsize)
{
	UNUSED(ctx);
	size_t sum_size = 0;
	int ret = 0;

	ret = udk_disklist_foreach(sysfs_diskstats_extra_print, (char *)dst, pos, rsize, size, &sum_size);

	return ret == -EOVERFLOW ? 0 : ret;
}

static void record_reset_info(struct udk_partition *part)
{
	if (part == NULL) {
		return;
	}

	for (int group = 0; group < NR_STAT_GROUPS; group++) {
		vatomic32_init(&part->diskstats.ios_margin[group],
			stat_atomic32_read(&part->diskstats.ios[group]));
		vatomic64_init(&part->diskstats.nsecs_margin[group],
			stat_atomic64_read(&part->diskstats.nsecs[group]));
		vatomic64_init(&part->diskstats.max_nsecs[group], 0);
	}
}

static int sysfs_diskstats_reset(struct udk_disk *disk, char *buf, size_t pos, size_t *rsize,
	size_t size, size_t *sum_size)
{
	UNUSED(buf);
	UNUSED(pos);
	UNUSED(*rsize);
	UNUSED(size);
	UNUSED(*sum_size);

	if (disk == NULL) {
		return -EINVAL;
	}

	record_reset_info(&(disk->part0));
	for (int i = 0; i < MAX_PARTITIONS; i++) {
		if (disk->parts[i] != NULL) {
			record_reset_info(disk->parts[i]);
		}
	}

	return 0;
}

static int sysfs_diskstats_extra_write(void *ctx, void *dst, size_t pos, size_t size, size_t *rsize)
{
	UNUSED(pos);
	UNUSED(ctx);
	size_t len;
	char write_buf[64] = {0};
	int reset_num = 0;
	size_t sum_size = 0;
	int ret;

	if (dst == NULL || size == 0) {
		return -EINVAL;
	}

	len = (size >= (size_t)UEVENT_WRITE_BUF_LEN) ? (size_t)UEVENT_WRITE_BUF_LEN : size;
	ret = udk_copy_from_user(write_buf, dst, len);
	if (ret != 0) {
		udk_warn("copy_from_user failed, ret=%d\n", ret);
		return ret;
	}

	if (write_buf[len - 1] == '\n') {
		write_buf[len - 1] = '\0';
	}
	write_buf[len] = '\0';

	reset_num = strtol(write_buf, NULL, 0);
	if (reset_num != 1) {
		*rsize = len;
		return ret;
	}

	ret = udk_disklist_foreach(sysfs_diskstats_reset, (char *)dst, pos, rsize, size, &sum_size);
	*rsize = len;

	return ret;
}

static struct udk_sysfs_fops diskstats_extra_fops = {
	.read = sysfs_diskstats_extra_read,
	.write = sysfs_diskstats_extra_write,
};

static int sysfs_uevent_read(void *ctx, void *dst, size_t pos, size_t size, size_t *rsize)
{
	struct udk_partition *part;
	struct udk_device *udkdev;
	char buf[SD_UEVENT_BUF_LEN] = {0};
	int ret;
	size_t buf_size;

	if (ctx == NULL) {
		return 0;
	}

	part = (struct udk_partition *)ctx;
	udkdev = &part->udkdev;
	if (dst == NULL || size == 0 || udkdev == NULL) {
		return -EINVAL;
	}

	if (pos > SD_UEVENT_BUF_LEN) {
		*rsize = 0;
		return 0;
	}

	udk_info("read dev uevent, dev_name=%s\n", udkdev->name);

	/* format uevent output buf */
	if (strlen(udkdev->name) == SD_PART0_NAME_LEN) {
		ret = snprintf_s(buf, SD_UEVENT_BUF_LEN, SD_UEVENT_BUF_LEN - 1,
				 SD_DISK_UEVENT_BUF_FORMAT, MAJOR(udkdev->devt),
				 MINOR(udkdev->devt), udkdev->name, DEVTYPE_DISK);
	} else {
		ret = snprintf_s(buf, SD_UEVENT_BUF_LEN, SD_UEVENT_BUF_LEN - 1,
				 SD_PART_UEVENT_BUF_FORMAT, MAJOR(udkdev->devt),
				 MINOR(udkdev->devt), udkdev->name, DEVTYPE_PARTITION,
				 part->partno, part->name);
	}
	if (ret < 0) {
		udk_warn("format sd uevent buf failed, ret=%d\n", ret);
		return ret;
	}

	/* copy output buf to dst addr */
	buf_size = (size <= (size_t)(unsigned int)ret) ? size : (size_t)(unsigned int)ret;
	if (pos >= buf_size) {
		*rsize = 0;
		return 0;
	}
	ret = udk_copy_to_user(dst, buf + pos, buf_size - pos);
	if (ret != 0) {
		udk_warn("copy_to_user failed, ret=%d\n", ret);
		return ret;
	}

	*rsize = buf_size - pos;
	return 0;
}

static int format_netlink_msg(struct udk_partition *part, struct udk_device *udkdev,
			      struct uevent_msg *msg, char *write_buf, size_t write_buf_len)
{
	UNUSED(write_buf_len);
	devnum_t devt;
	char dev_idx;
	int ret;
	char temp_prefix[NVME_PREFIX_NAME_LEN + 1];
	char temp_part[NVME_PART0_NAME_LEN + 1];

	/* format devpath */
	dev_idx = udkdev->name[SD_PREFIX_NAME_LEN];
	devt = udkdev->devt;

	ret = snprintf_s(msg->action, UEVENT_ACTION_LEN, UEVENT_ACTION_LEN - 1,
			"%s%c", write_buf, '\0');
	if (ret <= 0) {
		return ret;
	}

	if ((strlen(udkdev->name) > NVME_NAME_LEN) &&
		strncmp(udkdev->name, NVME_COMMON_PREFIX_NAME, NVME_NAME_LEN) == 0) {
		/* cpy nvme0 */
		NOFAIL(strncpy_s(temp_prefix, sizeof(temp_prefix), udkdev->name, NVME_PREFIX_NAME_LEN));
		temp_prefix[NVME_PREFIX_NAME_LEN] = '\0';
		/* cpy nvme0n1 */
		NOFAIL(strncpy_s(temp_part, sizeof(temp_part), udkdev->name, NVME_PART0_NAME_LEN));
		temp_part[NVME_PART0_NAME_LEN] = '\0';
		if (strlen(udkdev->name) == NVME_PART0_NAME_LEN) {
			/* /devices/platform/b0000000.hi_pcie/pci0000:00/0000:00:00.0/0000:01:00.0/nvme/nvme0/nvme0n1 */
			ret = snprintf_s(msg->devpath, UEVENT_DEVPATH_LEN, UEVENT_DEVPATH_LEN - 1,
					"/devices/platform/%s/%s/%s", part->udkdisk->sys_devpath, temp_prefix, temp_part);
		} else {
			/* /devices/platform/b0000000.hi_pcie/pci0000:00/0000:00:00.0/0000:01:00.0/nvme/nvme0/nvme0n1/nvme0n1p1 */
			ret = snprintf_s(msg->devpath, UEVENT_DEVPATH_LEN, UEVENT_DEVPATH_LEN - 1,
					"/devices/platform/%s/%s/%s/%s", part->udkdisk->sys_devpath, temp_prefix, temp_part, udkdev->name);
		}
	} else {
		if (strlen(udkdev->name) == SD_PART0_NAME_LEN) {
			ret = snprintf_s(msg->devpath, UEVENT_DEVPATH_LEN, UEVENT_DEVPATH_LEN - 1,
					SD_DISK_DEVPATH_FORMAT, part->udkdisk->sys_devpath,
					dev_idx, '\0');
		} else {
			ret = snprintf_s(msg->devpath, UEVENT_DEVPATH_LEN, UEVENT_DEVPATH_LEN - 1,
					SD_PART_DEVPATH_FORMAT, part->udkdisk->sys_devpath,
					dev_idx, udkdev->name, '\0');
		}
	}
	if (ret <= 0) {
		return ret;
	}

	/* format netlink msg */
	if ((strlen(udkdev->name) == SD_PART0_NAME_LEN) || (strlen(udkdev->name) == NVME_PART0_NAME_LEN)) {
		ret = snprintf_s(msg->buf, UEVENT_BUF_LEN, UEVENT_BUF_LEN - 1,
				 SD_DISK_NETLINK_BUF_FORMAT, write_buf, '\0', msg->devpath, '\0',
				 SUBSYSTEM_BLOCK, '\0', MAJOR(devt), '\0', MINOR(devt), '\0',
				 udkdev->name, '\0', DEVTYPE_DISK, '\0');
	} else {
		ret = snprintf_s(msg->buf, UEVENT_BUF_LEN, UEVENT_BUF_LEN - 1,
				 SD_PART_NETLINK_BUF_FORMAT, write_buf, '\0', msg->devpath, '\0',
				 SUBSYSTEM_BLOCK, '\0', MAJOR(devt), '\0', MINOR(devt), '\0',
				 udkdev->name, '\0', DEVTYPE_PARTITION, '\0', part->partno, '\0',
				 part->name, '\0');
	}

	return ret;
}

static int sd_uevent_msg_send(struct udk_partition *part, struct udk_device *udkdev,
			      char *write_buf, size_t buf_len)
{
	int ret;
	struct uevent_msg *msg = NULL;

	msg = (struct uevent_msg *)udk_malloc(sizeof(struct uevent_msg));
	if (msg == NULL) {
		return -ENOMEM;
	}

	mem_zero_s(*msg);
	ret = format_netlink_msg(part, udkdev, msg, write_buf, buf_len);
	if (ret <= 0) {
		udk_free(msg);
		return (ret == 0) ? -EINVAL : ret;
	}

	msg->len = ret;

	ret = hm_net_dev_ioctl(SIOCUEVENTBROAD, (void *)msg);

	udk_free(msg);

	return ret;
}

static int sysfs_uevent_write(void *ctx, void *dst, size_t pos, size_t size, size_t *rsize)
{
	UNUSED(pos);
	struct udk_partition *part;
	struct udk_device *udkdev;
	size_t len;
	char write_buf[64] = {0};
	int ret;

	if (ctx == NULL) {
		return 0;
	}

	part = (struct udk_partition *)ctx;
	udkdev = &part->udkdev;
	if (dst == NULL || size == 0 || udkdev == NULL) {
		return -EINVAL;
	}

	len = (size >= (size_t)UEVENT_WRITE_BUF_LEN) ? (size_t)UEVENT_WRITE_BUF_LEN : size;
	ret = udk_copy_from_user(write_buf, dst, len);
	if (ret != 0) {
		udk_warn("copy_from_user failed, ret=%d\n", ret);
		return ret;
	}

	/*
	 * user usually write string like "add\n" to uevent node,
	 * need to remove '\n' at the end of buf, otherwise server will parse error.
	 */
	if (write_buf[len - 1] == '\n') {
		write_buf[len - 1] = '\0';
	}

	udk_info("write dev uevent, dev_name=%s, action=%s\n", udkdev->name, write_buf);

	ret = sd_uevent_msg_send(part, udkdev, write_buf, len);
	if (ret != E_HM_OK) {
		return ret;
	}

	*rsize = len;
	return 0;
}

static struct udk_sysfs_fops uevent_fops = {
	.read = sysfs_uevent_read,
	.write = sysfs_uevent_write,
};

static int sysfs_size_read(void *ctx, void *dst, size_t pos, size_t size, size_t *rsize)
{
	struct udk_disk *disk;
	char buf[SD_UEVENT_BUF_LEN] = {0};
	int ret = 0;
	size_t buf_size;

	if (ctx == NULL) {
		return 0;
	}

	disk = (struct udk_disk *)ctx;
	if (dst == NULL || size == 0 || disk == NULL) {
		return -EINVAL;
	}

	if (pos > SD_UEVENT_BUF_LEN) {
		*rsize = 0;
		return 0;
	}

	udk_info("read disk size, disk_name=%s\n", disk->name);

	/* format uevent output buf */
	ret = snprintf_s(buf, SD_UEVENT_BUF_LEN, SD_UEVENT_BUF_LEN - 1,
			 "%llu\n", disk->part0.nr_sects);
	if (ret < 0) {
		udk_warn("format sd uevent buf failed, ret=%d\n", ret);
		return ret;
	}

	/* copy output buf to dst addr */
	buf_size = (size <= (size_t)(unsigned int)ret) ? size : (size_t)(unsigned int)ret;
	if (pos >= buf_size) {
		*rsize = 0;
		return 0;
	}
	ret = udk_copy_to_user(dst, buf + pos, buf_size - pos);
	if (ret != E_HM_OK) {
		udk_warn("copy_to_user failed, ret=%d\n", ret);
		return ret;
	}

	*rsize = buf_size - pos;
	return 0;
}

static struct udk_sysfs_fops size_fops = {
	.read = sysfs_size_read,
	.write = NULL,
};

static int sysfs_queue_qos_read(void *ctx, void *dst, size_t pos, size_t size, size_t *rsize)
{
	int ret = 0;
	size_t len;
	struct udk_disk *disk;
	char buf[SD_QOS_BUF_LEN];
	bool qos_on;

	disk = (struct udk_disk *)ctx;
	if (dst == NULL || size == 0 || disk == NULL || disk->bd_queue == NULL) {
		return -EINVAL;
	}

	mem_zero_a(buf);
	qos_on = udk_blk_queue_flag_is_set(disk->bd_queue, UDK_BLK_QUEUE_FLAG_QOS);
	if (qos_on) {
		ret = snprintf_s(buf, SD_QOS_BUF_LEN, SD_QOS_BUF_LEN - 1, "%u\n", 1);
	} else {
		ret = snprintf_s(buf, SD_QOS_BUF_LEN, SD_QOS_BUF_LEN - 1, "%u\n", 0);
	}
	if (ret < 0) {
		return ret;
	}
	len = strlen(buf);
	len = (len < size ? len : size);
	if (pos >= len) {
		*rsize = 0;
		return 0;
	}

	ret = udk_copy_to_user(dst, &buf[0] + pos, len - pos);
	if (ret != E_HM_OK) {
		udk_warn("copy_to_user failed, ret=%d\n", ret);
		return ret;
	}
	*rsize = len - pos;

	return 0;
}

static int sysfs_queue_qos_write(void *ctx, void *dst, size_t pos, size_t size, size_t *rsize)
{
	int ret;
	int qos;
	size_t len;
	struct udk_disk *disk;
	char buf[SD_QOS_BUF_LEN];

	UNUSED(pos);
	disk = (struct udk_disk *)ctx;
	if ((disk == NULL) || (disk->bd_queue == NULL) || (dst == NULL) || (size == 0) || (rsize == NULL)) {
		return -EINVAL;
	}

	mem_zero_a(buf);
	len = (size > (sizeof(buf) - 1)) ? (sizeof(buf) - 1) : size;
	ret = udk_copy_from_user(buf, dst, len);
	if (ret < 0) {
		udk_warn("udk copy from user len = %zu failed, %s\n", len, strerror(-ret));
		return ret;
	}
	if (buf[len - 1] == '\n') {
		buf[len - 1] = '\0';
	}
	*rsize = len;

	qos = atoi(buf);
	if (qos != 0) {
		udk_blk_queue_flag_set(disk->bd_queue, UDK_BLK_QUEUE_FLAG_QOS);
	} else {
		udk_blk_queue_flag_clear(disk->bd_queue, UDK_BLK_QUEUE_FLAG_QOS);
	}

	return 0;
}

static struct udk_sysfs_fops qos_fops = {
	.read = sysfs_queue_qos_read,
	.write = sysfs_queue_qos_write,
};

static void create_sd_sysfs_block_queue_node(const char *prefix, struct udk_device *udkdev,
					     struct udk_disk *udisk)
{
	int ret;
	char path[UDK_SYSFS_PATH_NAME_MAX];

	ret = udk_sysfs_create_blk_queue_dir(udkdev, prefix, path, UDK_SYSFS_PATH_NAME_MAX);
	if (ret < 0) {
		udk_warn("create sd block queue sysfs dir failed ret=%d\n", ret);
		return;
	}

	ret = udk_sysfs_create_blk_queue_qos(path, &qos_fops, udisk);
	if (ret < 0) {
		udk_warn("create sd block queue qos sysfs failed ret=%d\n", ret);
	}

#ifdef CONFIG_MAS_BLK
	mas_blk_sysfs_block_init(path, udisk);
#endif
}

static void create_sd_sysfs_node(struct udk_device *udkdev, int name_len)
{
	char prefix[UDK_SYSFS_PATH_NAME_MAX];
	char path[UDK_SYSFS_PATH_NAME_MAX];
	char temp[SD_PART0_NAME_LEN + 1];
	char clspath[UDK_SYSFS_PATH_NAME_MAX];
	int ret;
	unsigned int retry_times = 0;
	struct udk_partition *part = container_of(udkdev, struct udk_partition, udkdev);
	struct udk_disk *udisk = part->udkdisk;

#define MAX_RETRY_TIMES		1000
#define WAIT_MSEC		30
	if (name_len == SD_PART0_NAME_LEN) {
		/* handle dev part0 whose name like "sdd" */
		ret = snprintf_s(prefix, UDK_SYSFS_PATH_NAME_MAX, UDK_SYSFS_PATH_NAME_MAX - 1,
				 SD_PREFIX_PATH_FORMAT, udisk->sys_devpath);
		if (ret <= 0) {
			udk_warn("format sd prefix path failed, ret=%d\n", ret);
			return;
		}
		while (((ret = udk_sysfs_create_simple_dir(prefix)) < 0) && (retry_times < MAX_RETRY_TIMES)) {
			/* parent path may not exist, delay max 30s here to wait ldk create */
			udk_mdelay(WAIT_MSEC);
			retry_times++;
		}
		if (ret < 0) {
			udk_warn("create simple dir failed, ret=%d\n", ret);
			return;
		}
		udk_sysfs_create_dev_dir(prefix, udkdev);
		udk_sysfs_create_block_symlink(prefix, udkdev);
		udk_sysfs_create_uevent(prefix, udkdev, &uevent_fops, part);
		create_sd_sysfs_block_queue_node(prefix, udkdev, udisk);

		ret = snprintf_s(clspath, UDK_SYSFS_PATH_NAME_MAX, UDK_SYSFS_PATH_NAME_MAX - 1,
				 CLASS_BLOCK_FORMAT, udkdev->name);
		if (ret <= 0) {
			udk_warn("format class path failed, ret=%d\n", ret);
			return;
		}
		udk_sysfs_create_dir_only(clspath);
		udk_sysfs_create_size_node(clspath, &size_fops, udisk);
	} else if (name_len > SD_PART0_NAME_LEN) {
		/* handle dev partx whose name like "sdd1" */
		NOFAIL(strncpy_s(temp, sizeof(temp) / sizeof(char), udkdev->name, SD_PART0_NAME_LEN));
		temp[SD_PART0_NAME_LEN] = '\0';
		ret = snprintf_s(path, UDK_SYSFS_PATH_NAME_MAX, UDK_SYSFS_PATH_NAME_MAX - 1,
				 SD_PREFIX_PATH_FORMAT"/%s", udisk->sys_devpath, temp);
		if (ret <= 0) {
			udk_warn("format sd dir path failed, ret=%d\n", ret);
			return;
		}
		udk_sysfs_create_dev_dir(path, udkdev);
		udk_sysfs_create_uevent(path, udkdev, &uevent_fops, part);
	}
}

static void create_nvme_sysfs_node(struct udk_device *udkdev, int name_len)
{
	char prefix[UDK_SYSFS_PATH_NAME_MAX];
	char path[UDK_SYSFS_PATH_NAME_MAX];
	char temp_prefix[NVME_PREFIX_NAME_LEN + 1];
	char temp[NVME_PART0_NAME_LEN + 1];
	char clspath[UDK_SYSFS_PATH_NAME_MAX];
	int ret;
	unsigned int retry_times = 0;
	struct udk_partition *part = container_of(udkdev, struct udk_partition, udkdev);
	struct udk_disk *udisk = part->udkdisk;

#define MAX_RETRY_TIMES		1000
#define WAIT_MSEC		30
	if (name_len == NVME_PART0_NAME_LEN) {
		/* handle dev part0 whose name like "nvme0n1" */
		/* /devices/platform/b0000000.hi_pcie/pci0000:00/0000:00:00.0/0000:01:00.0/nvme/ */
		ret = snprintf_s(prefix, UDK_SYSFS_PATH_NAME_MAX, UDK_SYSFS_PATH_NAME_MAX - 1,
				 "platform/%s", udisk->sys_devpath);
		if (ret <= 0) {
			udk_warn("format sd prefix path failed, ret=%d\n", ret);
			return;
		}
		ret = udk_sysfs_create_simple_dir(prefix);
		if (ret < 0) {
			udk_warn("create simple dir %s failed, ret=%d\n", prefix, ret);
			return;
		}
		NOFAIL(strncpy_s(temp_prefix, sizeof(temp_prefix), udkdev->name, NVME_PREFIX_NAME_LEN));
		temp_prefix[NVME_PREFIX_NAME_LEN] = '\0';
		/* /devices/platform/b0000000.hi_pcie/pci0000:00/0000:00:00.0/0000:01:00.0/nvme/nvme0 */
		ret = snprintf_s(prefix, UDK_SYSFS_PATH_NAME_MAX, UDK_SYSFS_PATH_NAME_MAX - 1,
				 "platform/%s/%s", udisk->sys_devpath, temp_prefix);
		if (ret <= 0) {
			udk_warn("format sd prefix %s path failed, ret=%d\n", ret, prefix);
			return;
		}
		while (((ret = udk_sysfs_create_simple_dir(prefix)) < 0) && (retry_times < MAX_RETRY_TIMES)) {
			/* parent path may not exist, delay max 30s here to wait ldk create */
			udk_mdelay(WAIT_MSEC);
			retry_times++;
		}
		if (ret < 0) {
			udk_warn("create simple dir failed, ret=%d\n", ret);
			return;
		}
		udk_sysfs_create_dev_dir(prefix, udkdev);
		udk_sysfs_create_block_symlink(prefix, udkdev);
		udk_sysfs_create_uevent(prefix, udkdev, &uevent_fops, part);
		create_sd_sysfs_block_queue_node(prefix, udkdev, udisk);
		ret = snprintf_s(clspath, UDK_SYSFS_PATH_NAME_MAX, UDK_SYSFS_PATH_NAME_MAX - 1,
				 CLASS_BLOCK_FORMAT, udkdev->name);
		if (ret <= 0) {
			udk_warn("format class path failed, ret=%d\n", ret);
			return;
		}
		udk_sysfs_create_dir_only(clspath);
		udk_sysfs_create_size_node(clspath, &size_fops, udisk);
	} else if (name_len > NVME_PART0_NAME_LEN) {
		/* handle dev partx whose name like "nvme0n1p1" */
		NOFAIL(strncpy_s(temp, sizeof(temp) / sizeof(char), udkdev->name, NVME_PART0_NAME_LEN));
		temp[NVME_PART0_NAME_LEN] = '\0';
		NOFAIL(strncpy_s(temp_prefix, sizeof(temp_prefix), udkdev->name, NVME_PREFIX_NAME_LEN));
		temp_prefix[NVME_PREFIX_NAME_LEN] = '\0';
		/* /devices/platform/b0000000.hi_pcie/pci0000:00/0000:00:00.0/0000:01:00.0/nvme/nvme0/nvme0n1/ */
		ret = snprintf_s(path, UDK_SYSFS_PATH_NAME_MAX, UDK_SYSFS_PATH_NAME_MAX - 1,
				 "platform/%s/%s/%s", udisk->sys_devpath, temp_prefix, temp);
		if (ret <= 0) {
			udk_warn("format sd dir path failed, ret=%d\n", ret);
			return;
		}
		udk_sysfs_create_dev_dir(path, udkdev);
		udk_sysfs_create_uevent(path, udkdev, &uevent_fops, part);
	}
}

void udk_sysfs_block_create(struct udk_device *udkdev, devnum_t devt)
{
	int name_len;

	if (udkdev->name == NULL || devt == 0) {
		udk_warn("create sysfs node failed, dev_name is NULL or devt is 0\n");
		return;
	}

	name_len = strlen(udkdev->name);
	if (name_len > SD_PREFIX_NAME_LEN &&
	    strncmp(udkdev->name, SD_COMMON_PREFIX_NAME, SD_PREFIX_NAME_LEN) == 0) {
		/* create sdx device nodes */
		create_sd_sysfs_node(udkdev, name_len);
	} else if (name_len > NVME_NAME_LEN &&
		strncmp(udkdev->name, NVME_COMMON_PREFIX_NAME, NVME_NAME_LEN) == 0) {
		/* create nvme0/nvme0n1 device nodes */
		create_nvme_sysfs_node(udkdev, name_len);
	}
}

int udk_sysfs_block_init(void)
{
	int ret = E_HM_OK;

	ret = udk_sysfs_create_block_node("diskstats", &diskstats_fops, NULL);
	if (ret != E_HM_OK) {
		return ret;
	}

	return udk_sysfs_create_block_node("diskstats_extra", &diskstats_extra_fops, NULL);
}
