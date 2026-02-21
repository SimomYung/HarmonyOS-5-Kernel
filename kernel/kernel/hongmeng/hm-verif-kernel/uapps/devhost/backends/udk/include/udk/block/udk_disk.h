/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK disk definitions
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 29 16:31:35 2019
 */
#ifndef __UDK_DISK_H__
#define __UDK_DISK_H__

#include <hmkernel/errno.h>
#include <libhmsync/raw_scopedptr.h>

#include <udk/block/udk_block.h>
#include <udk/block/udk_partition.h>

#ifdef CONFIG_UDK_UEFI
#define MAX_PARTITIONS	128 /* use 128 for UEFI Partition */
#else
#define MAX_PARTITIONS	5
#endif
#define DISK_NAME_LEN	32
#define DISK_SYS_DEVPATH_LEN	128

struct udk_internal_blkops;
struct udk_request_queue;

struct udk_disk {
	uint32_t major;
	char name[DISK_NAME_LEN];
	uint32_t first_minor;
	int minors;
	struct udk_partition part0; /* the partition pointed to the root disk */
	struct udk_partition *parts[MAX_PARTITIONS];

	const struct udk_disk_operations *disk_fops;
	unsigned int flags;
	unsigned int logical_block_size;
	unsigned int max_discard_sectors;
	unsigned int max_sectors;

	struct udk_request_queue *bd_queue;
	void *private_data;

	struct udk_internal_blkops *iblkops;
	char sys_devpath[DISK_SYS_DEVPATH_LEN];
	struct dlist_node list;
	unsigned int read_ahead_kb;
	struct raw_refcnt refcnt;
};

struct udk_disk_operations {
	int (*open) (struct udk_disk *, unsigned);
	void (*release) (struct udk_disk *, unsigned);
	int (*ioctl) (struct udk_disk *, unsigned, unsigned, unsigned long);
	int (*submit_bio)(struct udk_disk *, struct udk_bio *);
	int (*direct_access)(struct udk_disk *disk,
			     unsigned long offset, unsigned long size,
			     unsigned long long *paddr, unsigned long *ret_size);

	/* used to configure devfs node attribute for special devices */
	int (*config_device)(struct udk_disk *disk, struct udk_device_attr *attr);
	void **owner;
};

/* Only can be used after udk_disk_add() */
static inline struct udk_disk *udkdev_to_disk(struct udk_device *udk_dev)
{
	return udkdev_to_part(udk_dev)->udkdisk;
}

/*
 * Currently, get_capacity() and set_capacity() update the nr_sects directly,
 * In the future, If we support extend the partitions, they should be
 * converted to be protected.
 */
static inline uint64_t get_capacity(struct udk_disk *disk)
{
	return disk->part0.nr_sects;
}

static inline void set_capacity(struct udk_disk *disk, uint64_t size)
{
	disk->part0.nr_sects = size;
}

struct udk_disk *udk_disk_alloc(int minors);
void udk_disk_free(struct udk_disk *disk);
int udk_register_blkdev_region(unsigned int major, const char *name);
void udk_unregister_blkdev_region(unsigned int major, const char *name);
int udk_disk_add_with_sys_devpath(struct udk_disk *disk, const char *path);
int udk_disk_add(struct udk_disk *ud);
int udk_disk_destroy(struct udk_disk *ud);
int udk_disklist_foreach(int (* const apply)(struct udk_disk *disk, char *buf, size_t pos, size_t *rsize, size_t size,
			 size_t *sum_size), char *buf, size_t pos, size_t *rsize, size_t size, size_t *sum_size);

int udk_get_disk(struct udk_partition *part);
int udk_put_disk(struct udk_partition *part);

void udk_disk_set_logical_block_size(struct udk_disk *disk, unsigned int size);
void udk_disk_set_max_discard_sectors(struct udk_disk *disk,
				      unsigned int sectors);
unsigned int udk_disk_obtain_max_discard_sectors(const struct udk_disk *disk);
void udk_disk_set_max_sectors(struct udk_disk *disk, unsigned int sectors);
unsigned int udk_disk_obtain_max_sectors(const struct udk_disk *disk);

/* disk flags */
#define UDK_DISK_FLAG_DISCARD 0
#define UDK_DISK_FLAG_SYNC_IO 1
#define UDK_DISK_FLAG_GC      2
#define UDK_DISK_FLAG_DAS     3
#define UDK_DISK_FLAG_DIRECTMAP 4
/* record bio stats on the disk */
#define UDK_DISK_FLAG_BIO_STATS 5
#define UDK_DISK_FLAG_RODEV 6

static inline void udk_disk_clear_flag(struct udk_disk *disk, unsigned int bit)
{
	disk->flags &= ~(1UL << bit);
}

static inline void udk_disk_set_flag(struct udk_disk *disk, unsigned int bit)
{
	disk->flags |= (1UL << bit);
}

static inline bool udk_disk_check_flag(const struct udk_disk *disk,
				       unsigned int bit)
{
	return ((disk->flags) & (1UL << bit));
}

int udk_device_link_disk(struct udk_disk *disk);
void udk_device_unlink_disk(struct udk_disk *disk);
char *udk_disk_get_device_name(const char *partition_name);

#endif /* __UDK_DISK_H__ */
