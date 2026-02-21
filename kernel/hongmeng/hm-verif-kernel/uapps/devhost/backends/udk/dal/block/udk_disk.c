/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK disk operations
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 29 16:31:35 2019
 */

#include <lib/dlist.h>
#include <udk/block/udk_disk.h>

#include <udk/log.h>
#include <udk/device.h>
#include <internal/device.h>
#include <udk/block/udk_block.h>
#include <udk/block/udk_block_mq.h>
#include <udk/block/mas_blk/mas_blk_core_interface.h>
#include <hongmeng/errno.h>
#include <libdevhost/devhost.h>
#include <libhwsecurec/securec.h>

#include "udk_block_internal.h"

#define DISK_LIST_LEN_MAX 500
#define DISK_DEFAULT_READ_AHEAD_KB	128

static DLIST_HEAD(g_disk_list);
static struct udk_mutex g_disk_list_lock = UDK_MUTEX_INIT;

static void __disk_destroy(struct raw_refcnt *refcnt)
{
	int ret;
	struct udk_disk *disk = NULL;

	if (refcnt == NULL) {
		return;
	}

	disk = container_of(refcnt, struct udk_disk, refcnt);
	if (disk->name == NULL) {
		udk_warn("Disk is not exist\n");
		return;
	}

	ret = udk_destroy_partitions(disk);
	if (ret != E_HM_OK) {
		udk_warn("Failed to destroy partitions for disk\n");
		return;
	}
	free(disk);
}

struct udk_disk *udk_disk_alloc(int minors)
{
	struct udk_disk *disk = (struct udk_disk *)malloc(sizeof(struct udk_disk));

	if (disk == NULL) {
		udk_error("Failed to alloc udk_disk\n");
		return NULL;
	}

	mem_zero_s(*disk);

	disk->minors = minors;
	disk->part0.partno = 0;
	disk->part0.nr_sects = 0;
	disk->logical_block_size = UDK_BLKSIZE_DEFAULT;
	disk->max_discard_sectors = UDK_BLK_DEFAULT_MAX_DISCARD;
	disk->max_sectors = UDK_BLK_DEFAULT_MAX_SECTORS;
	disk->read_ahead_kb = DISK_DEFAULT_READ_AHEAD_KB;
	dlist_init(&disk->list);
	raw_refcnt_init(&disk->refcnt, 1, __disk_destroy);

	return disk;
}

void udk_disk_free(struct udk_disk *disk)
{
	udk_disk_refcnt_put(disk);
}

int udk_register_blkdev_region(unsigned int major, const char *name)
{
	unsigned int major_ret = 0;
	int ret;

	if (name == NULL) {
		return -EINVAL;
	}

	ret = libdh_register_blkdev_region(major, name, &major_ret);
	if (ret < 0) {
		udk_error("Failed to register blkdev region, name=%s, err: %s\n",
			  name, hmstrerror(ret));
		return -hmerrno2posix(ret);
	}
	if (major == 0) {
		ret = (int)major_ret;
	}

	return ret;
}

void udk_unregister_blkdev_region(unsigned int major, const char *name)
{
	libdh_unregister_blkdev_region(major, name);
}

static void disk_device_release(struct udk_device *udkdev)
{
	struct udk_disk *disk = NULL;

	if (udkdev == NULL) {
		return;
	}

	/* Free name allocated by strndup */
	if (udkdev->name != NULL) {
		free(udkdev->name);
		udkdev->name = NULL;
	}

	disk = udkdev_to_disk(udkdev);
	if (disk == NULL) {
		udk_warn("Fail to get disk from udk device\n");
		return;
	}
	udk_disk_refcnt_put(disk);
}

int udk_disk_device_add(const char *name, devnum_t devt, struct udk_device *udkdev)
{
	int ret;

	if (name == NULL || udkdev == NULL) {
		return -EINVAL;
	}

	udkdev->name = strndup(name, DISK_NAME_LEN - 1);
	if (udkdev->name == NULL) {
		return -ENOMEM;
	}

	ret = udk_device_init(udkdev);
	if (ret < 0) {
		udk_error("Failed to init udk device, name=%s\n", udkdev->name);
		free(udkdev->name);
		udkdev->name = NULL;
		return ret;
	}
	udkdev->release = disk_device_release;
	/* hold disk refcnt in part's udkdev lifecycle */
	udk_disk_refcnt_get(udkdev_to_disk(udkdev));

	udk_device_capable_devfs_full(udkdev, devt, (int)MODE_BLK,
				      &udk_blkdev_ops, udk_blkdev_config_devnode);
	udk_device_capable_blk(udkdev, &udk_blkops);
	udk_sysfs_block_create(udkdev, devt);

	ret = udk_device_add(udkdev);
	if (ret != 0) {
		udk_error("Failed to add udk device, name=%s\n", udkdev->name);
		udk_device_free(udkdev);
	}

	return ret;
}

int udk_disk_add_with_sys_devpath(struct udk_disk *disk, const char *path)
{
	int ret;

	if (disk == NULL || path == NULL) {
		udk_error("disk or path is not exist\n");
		return -EINVAL;
	}

	udk_disk_refcnt_get(disk);
	if (strncpy_s(disk->sys_devpath, DISK_SYS_DEVPATH_LEN, path,
		      strnlen(path, DISK_SYS_DEVPATH_LEN)) != 0) {
		udk_disk_refcnt_put(disk);
		return -ENOEXEC;
	}

	ret = udk_disk_add(disk);
	udk_disk_refcnt_put(disk);

	return ret;
}

/*
 * udk_disk_add - add disk information
 * @disk: per-disk information
 */
int udk_disk_add(struct udk_disk *disk)
{
	struct udk_partition *part0 = NULL;
	devnum_t devt;
	int ret;

	if (disk == NULL || disk->name == NULL) {
		udk_error("Disk is not exist\n");
		return -EINVAL;
	}

	udk_disk_refcnt_get(disk);
	part0 = &(disk->part0);
	devt = MKDEV(disk->major, disk->first_minor + part0->partno);
	disk->major = MAJOR(devt);
	disk->first_minor = MINOR(devt);
	part0->udkdisk = disk;
	part0->openers = 0;
	udk_mutex_init(&part0->lock);
	udk_disk_stat_init(part0);

	disk->iblkops = udk_internal_blkops_default();

	ret = udk_disk_device_add(disk->name, devt, &(part0->udkdev));
	if (ret != 0) {
		udk_error("Failed to add disk, name=%s\n", disk->name);
		udk_disk_refcnt_put(disk);
		return ret;
	}

#ifdef CONFIG_MAS_BLK
	if (disk->bd_queue != NULL) {
		mas_blk_queue_register(disk->bd_queue, disk);
	}
#endif
	ret = udk_scan_partitions(disk);
	if (ret != E_HM_OK) {
		udk_error("Failed to scan partitions for disk, name=%s\n", disk->name);
		udk_device_destroy(&(part0->udkdev));
		udk_disk_refcnt_put(disk);
		return -hmerrno2posix(ret);
	}
	udk_disk_refcnt_put(disk);

	udk_mutex_lock(&g_disk_list_lock);
	dlist_insert(&g_disk_list, &disk->list);
	udk_mutex_unlock(&g_disk_list_lock);

	return -hmerrno2posix(ret);
}

/* call udk_disk_destroy only after call (udk_disk_alloc + udk_disk_add) successfully */
int udk_disk_destroy(struct udk_disk *disk)
{
	struct udk_partition *part0 = NULL;

	if (disk == NULL || disk->name == NULL) {
		udk_error("Disk is not exist\n");
		return -EINVAL;
	}

	udk_mutex_lock(&g_disk_list_lock);
	dlist_delete(&disk->list);
	udk_mutex_unlock(&g_disk_list_lock);
	udk_partition_destroy_parts_device(disk);

	part0 = &disk->part0;
	udk_device_destroy(&part0->udkdev);

#ifdef CONFIG_UDK_BLOCK_MQ
	if(disk->bd_queue != NULL) {
		udk_blk_mq_free_queue(disk->bd_queue);
		disk->bd_queue = NULL;
	}
#endif
	udk_disk_free(disk);

	return 0;
}

int udk_disklist_foreach(int (* const apply)(struct udk_disk *disk, char *buf, size_t pos, size_t *rsize, size_t size,
			 size_t *sum_size), char *buf, size_t pos, size_t *rsize, size_t size, size_t *sum_size)
{
	int ret = 0;
	struct udk_disk *srv;

	udk_mutex_lock(&g_disk_list_lock);
	dlist_for_each_entry(srv, &g_disk_list, struct udk_disk, list) {
		ret = apply(srv, buf, pos, rsize, size, sum_size);
		if (ret < 0) {
			break;
		}
	}
	udk_mutex_unlock(&g_disk_list_lock);

	return ret;
}

void udk_disk_refcnt_get(struct udk_disk *disk)
{
	if (disk != NULL) {
		(void)raw_refcnt_get(&disk->refcnt);
	}
}

void udk_disk_refcnt_put(struct udk_disk *disk)
{
	if (disk != NULL) {
		(void)raw_refcnt_put(&disk->refcnt);
	}
}

int udk_device_link_disk(struct udk_disk *disk)
{
	if (disk != NULL && disk->disk_fops != NULL && disk->disk_fops->open != NULL)
		return disk->disk_fops->open(disk, FMODE_RW);

	return -EINVAL;
}

void udk_device_unlink_disk(struct udk_disk *disk)
{
	if (disk != NULL && disk->disk_fops != NULL && disk->disk_fops->release != NULL)
		disk->disk_fops->release(disk, FMODE_RW);
}

char *udk_disk_get_device_name(const char *partition_name)
{
	int i;
	char *dev_name = NULL;
	struct udk_disk *disk = NULL;
	struct udk_device *dev = NULL;

	if (partition_name == NULL) {
		return NULL;
	}

	udk_mutex_lock(&g_disk_list_lock);
	dlist_for_each_entry(disk, &g_disk_list, struct udk_disk, list) {
		for (i = 0; i < MAX_PARTITIONS; i++) {
			if ((disk->parts[i] == NULL) || (disk->parts[i]->name == NULL)) {
				continue;
			}

			if (strcmp(disk->parts[i]->name, partition_name) == 0) {
				dev = &disk->parts[i]->udkdev;
				break;
			}
		}
		if ((dev != NULL) && (dev->name != NULL)) {
			dev_name = strdup(dev->name);
			break;
		}
	}
	udk_mutex_unlock(&g_disk_list_lock);

	return dev_name;
}
