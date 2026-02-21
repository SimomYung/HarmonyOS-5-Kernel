/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK partition definitions
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 29 16:31:35 2019
 */
#ifndef __UDK_PARTITION_H__
#define __UDK_PARTITION_H__

#include <stdbool.h>

#include <udk/sync.h>
#include <udk/device.h>
#include <udk/block/udk_disk_stats.h>

#define PARTITION_NAME_LEN 64
struct udk_disk;

#define PARTITION_META_INFO_VOLNAMELTH	64
/*
 * Enough for the string representation of any kind of UUID plus NULL.
 * EFI UUID is 36 characters. MSDOS UUID is 11 characters.
 */
#define PARTITION_META_INFO_UUIDLTH	37

struct udk_partition_meta {
	char		uuid[PARTITION_META_INFO_UUIDLTH];
	unsigned char	volname[PARTITION_META_INFO_VOLNAMELTH];
};

/*
 * udk_partition - a generic partition in generic hard disk
 *
 * @udkdisk - the generic hard disk which this partition belongs to
 */
struct udk_partition {
	uint64_t			start_sect;
	/*
	 * The nr_sects might be updated while partition
	 * be extended, If IO is happening at the same time,
	 * it's non-atomic on 32-bit machines.
	 *
	 * Now, One can't extend a partition in block layer,
	 * So, this nr_sects no need to be protected.
	 */
	uint64_t			nr_sects;
	struct udk_device		udkdev;
	struct udk_disk			*udkdisk;
	uint32_t			partno;
	int				openers;
	char				name[PARTITION_NAME_LEN];
	struct udk_partition_meta	meta;
	struct udk_mutex		lock;
	struct udk_diskstats		diskstats;
};

static inline struct udk_partition *udkdev_to_part(struct udk_device *udk_dev)
{
	return container_of((udk_dev), struct udk_partition, udkdev);
}
#endif /* __UDK_PARTITION_H__ */
