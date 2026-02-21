/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: UDK block internal definitions
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 21 20:10:40 2020
 */
#ifndef __UDK_BLOCK_INTERNAL_H__
#define __UDK_BLOCK_INTERNAL_H__

#include <udk/block/udk_block.h>
#include <udk/block/udk_disk.h>
#include <udk/mutex.h>
#include <libstrict/strict.h>
#include <stdint.h>

extern struct devhost_block_ops udk_blkops;
extern struct devhost_fops udk_blkdev_ops;

#define MSDOS_MAGIC_SIZE	2
#define MSDOS_LABEL_MAGIC1	0x55
#define MSDOS_LABEL_MAGIC2	0xAA
#define MSDOS_PART_OFFSET	0x1be
#define MSDOS_PART_UNUSED	8
#define MBR_PARTITION_NUM	4

#define EFI_PMBR_OSTYPE_EFI	0xEF
#define EFI_PMBR_OSTYPE_EFI_GPT	0xEE
#define EFI_GUID_LENGTH		16
#define EFI_PARTITION_NAME_LENGTH	72
#define GPT_PRIMARY_PARTITION_TABLE_LBA	1
#define OS_TYPE_BYTE_IDX	4

#define PARSED_PART_NAME_LENGTH		37

struct blkdev_context {
	struct udk_device *udkdev;
	void *priv;
};

struct udk_internal_blkops {
	int (*submit_bio)(void *bio, unsigned int flags);
	int (*bio_add_data)(void *bio, uintptr_t vaddr, unsigned int bv_len);
};

struct udk_internal_blkops *udk_internal_blkops_default(void);

int udk_blkdev_config_devnode(struct device_info *info,
			      char *node_name, unsigned int size,
			      struct devnode_req *req);

typedef struct {
	uint8_t byte[EFI_GUID_LENGTH];
} guid_t;

struct efi_header {
	uint64_t signature;
	uint32_t version;
	uint32_t header_size;
	uint32_t header_crc32;
	uint32_t reserved1;
	uint64_t my_lba;
	uint64_t alternate_lba;
	uint64_t first_usable_lba;
	uint64_t last_usable_lba;
	guid_t disk_guid;
	uint64_t partition_entry_lba;
	uint32_t num_partition_entries;
	uint32_t sizeof_partition_entry;
	uint32_t partition_entry_array_crc32;
}__attribute__((packed));

struct efi_partition {
	guid_t partition_type;
	guid_t unique_id;
	uint64_t starting_lba;
	uint64_t ending_lba;
	uint64_t attributes;
	uint16_t partition_name[EFI_PARTITION_NAME_LENGTH / sizeof(uint16_t)];
}__attribute__((packed));

/* partition info read from disk, the format is fixed. */
struct msdos_partition {
	unsigned char unused[MSDOS_PART_UNUSED];
	/* starting sector counting from 0 */
	uint32_t start_sect;
	/* nr of sectors in partition */
	uint32_t nr_sects;
} __attribute__((packed));

struct msdos_header {
	unsigned char unused[MSDOS_PART_OFFSET];
	struct msdos_partition parts[MBR_PARTITION_NUM];
	unsigned char magic[MSDOS_MAGIC_SIZE];
} __attribute__((packed));

int udk_partition_set_flag(struct udk_partition *part);
void udk_partition_reset_flag(struct udk_partition *part);
int udk_scan_partitions(struct udk_disk *disk);
void udk_partition_destroy_parts_device(struct udk_disk *disk);
int udk_destroy_partitions(struct udk_disk *disk);
void udk_sysfs_block_create(struct udk_device *udkdev, devnum_t devt);
void udk_disk_stat_init(struct udk_partition *part);
int udk_disk_device_add(const char *name, devnum_t devt, struct udk_device *udkdev);
void udk_disk_refcnt_get(struct udk_disk *disk);
void udk_disk_refcnt_put(struct udk_disk *disk);

struct bio_params {
	struct udk_disk *disk;
	struct udk_mutex *bio_lock;
	devhost_end_bio_fn_t cb;
	uint64_t start_sector;
};

/*
 * udk_parsed_partitions saves partitions details.
 */
struct udk_parsed_partitions {
	char name[PARTITION_NAME_LEN];
	struct {
		uint64_t from;
		uint64_t size;
		char name[PARSED_PART_NAME_LENGTH];
	} *parts;
	uint32_t limit;
};

#define UDK_BIO_UNALLOCATED_IDX		(-1)
void udk_bio_info_init(struct udk_bio *bio);
void udk_bio_info_reset(struct udk_bio *bio);

#endif /* __UDK_BLOCK_INTERNAL_H__ */
