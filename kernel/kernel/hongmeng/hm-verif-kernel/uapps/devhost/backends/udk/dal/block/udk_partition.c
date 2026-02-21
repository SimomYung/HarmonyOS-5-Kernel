/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK partition table parser
 * Author: Huawei OS Kernel Lab
 * Create: Sat Aug 24 16:31:35 2019
 */

#include <ctype.h>
#include <endian.h>
#include <inttypes.h>

#include <udk/log.h>
#include <udk/device.h>
#include <udk/block/udk_disk.h>
#include <udk/block/udk_block.h>
#include <udk/block/udk_disk_stats.h>
#include <udk/lib/idr.h>
#include <hongmeng/errno.h>
#include <libsysif/devhost/api.h>
#include <libhwsecurec/securec.h>

#ifdef CONFIG_FSCK_BOOST
#include <intapi/vfs/fsck_boost.h>
#endif
#ifdef CONFIG_HISI_UDK_MAS_BLK
#include <basicplatform/udk/block/mas_blk/mas_blk_core_interface.h>
#endif

#include "udk_block_internal.h"

#define UDK_PART_EXT_MAJOR 260

static DEFINE_IDR(udk_part_ext_minor_idr);

static void partition_parse_msdos_part(struct udk_parsed_partitions *p,
					struct msdos_header *header)
{
	struct msdos_partition *part = header->parts;
	uint32_t slot;

	for (slot = 0; slot < p->limit; slot++, part++) {
		uint64_t from = le32toh(part->start_sect);
		uint64_t size = le32toh(part->nr_sects);
		if (size == 0) {
			continue;
		}

		p->parts[slot].from = from;
		p->parts[slot].size = size;
		udk_info("partition=%u, from=%"PRIu64", size=%"PRIu64"\n",
			 slot + 1, from, size);
	}
	return;
}

static void uint16_to_str(char *out, int length, uint16_t *in)
{
	int count = 0;
	while (count < length) {
		char c = in[count] & 0xff;
		out[count++] = c;
	}
	out[count] = '\0';
}

static int partition_parse_efi_part(struct udk_parsed_partitions *p,
				    struct efi_partition *pte, uint64_t last_lba,
				    unsigned int logical_size)
{
	uint32_t slot;
	uint64_t pre_lba_end = 0ULL;

	for (slot = 0; slot < p->limit; slot++, pte++) {
		/* check whether pte guid is valid */
		uint64_t from = le64toh(pte->starting_lba);
		uint64_t end = le64toh(pte->ending_lba);
		if (from > last_lba || end > last_lba) {
			continue;
		}
		uint64_t size = end - from + 1;
		if (size <= 1) {
			continue;
		}

		p->parts[slot].from = from * (logical_size / UDK_BLKSIZE_DEFAULT);
		p->parts[slot].size = size * (logical_size / UDK_BLKSIZE_DEFAULT);

		if (pre_lba_end != 0ULL && pre_lba_end > p->parts[slot].from) {
			udk_error("invalid slot[%u]:pre_lba_end=%"PRIu64" from=%"PRIu64" size=%"PRIu64"\n",
				  slot, pre_lba_end, p->parts[slot].from, p->parts[slot].size);
			return E_HM_INVAL;
		}
		pre_lba_end = p->parts[slot].from + p->parts[slot].size;

		uint16_to_str(&(p->parts[slot].name[0]),
			      EFI_PARTITION_NAME_LENGTH / sizeof(uint16_t), pte->partition_name);
		udk_info("efi: partition=%u, from=%"PRIu64", size=%"PRIu64", name=%s\n",
			 slot + 1, p->parts[slot].from, p->parts[slot].size, p->parts[slot].name);
	}
	return E_HM_OK;
}

static int partition_end_bio(void *private, int bi_error, unsigned int rq_idx)
{
	struct udk_mutex *bio_lock = NULL;
	UNUSED(bi_error, rq_idx);

	bio_lock = (struct udk_mutex *)private;
	if (bio_lock == NULL) {
		udk_error("bio_lock does not exist!\n");
		return E_HM_INVAL;
	}
	/*
	 * check_part_msdos() will lock twice to wait I/O completed,
	 * So here if the I/O has end, unlock one to make it continue.
	 */
	udk_mutex_unlock(bio_lock);

	return E_HM_OK;
}

static inline bool msdos_magic_present(const struct msdos_header *header)
{
	return (header->magic[0] == MSDOS_LABEL_MAGIC1 &&
		header->magic[1] == MSDOS_LABEL_MAGIC2);
}

static struct udk_bio* prepare_bio(struct bio_params params,
				   const unsigned char *data,
				   unsigned int bv_len)
{
	struct udk_disk *disk = params.disk;
	struct udk_mutex *bio_lock = params.bio_lock;
	devhost_end_bio_fn_t cb = params.cb;
	uint64_t start_sector = params.start_sector;
	struct udk_bio *bio = NULL;

	bio = udk_bio_alloc(1, false);
	if (bio == NULL) {
		return NULL;
	}

	bio->bi_rw = 0;
	bio->bi_sector = start_sector;
	/* Internal bio implies part0 is used */
	bio->bi_dev = &(disk->part0.udkdev);
	disk->iblkops->bio_add_data(bio, (uintptr_t)data, bv_len);
	set_udk_bio_context(bio, bio_lock);
	bio->bi_ctx.devhost_end_bio_fn = cb;
	return bio;
}

static int submit_bio(bool async_io, struct udk_mutex *bio_lock,
		      struct udk_disk *disk, struct udk_bio *bio)
{
	unsigned int flags;
	int ret;

	if (async_io) {
		bio->bi_ctx.async = true;
		udk_mutex_lock(bio_lock);
	}
	flags = async_io ? 0 : REQ_SIO;
	/* submit_bio will call bio_free automatically */
	ret = disk->iblkops->submit_bio(bio, flags);
	ret = posix2hmerrno(-ret);
	if (ret == E_HM_OK) {
		/*
		 * Lock again for waiting the partition_end_bio() has been called
		 * to make sure the I/O has completed.
		 */
		if (async_io) {
			udk_mutex_lock(bio_lock);
		}
	} else {
		/* Don't do bio completion callback */
		bio->bi_ctx.devhost_end_bio_fn = NULL;
		udk_bio_free(bio);
	}

	if (async_io) {
		udk_mutex_unlock(bio_lock);
		free(bio_lock);
	}

	return ret;
}

static int partition_read(struct udk_disk *disk, unsigned char *data,
			  unsigned int bv_len, uint64_t start_sector)
{
	struct udk_mutex *bio_lock = NULL;
	struct udk_bio *bio = NULL;
	devhost_end_bio_fn_t cb = NULL;
	bool async_io = true;
	int ret;

	if (disk->iblkops == NULL || disk->iblkops->bio_add_data == NULL ||
	    disk->iblkops->submit_bio == NULL) {
		return E_HM_INVAL;
	}

	async_io = !udk_disk_check_flag(disk, UDK_DISK_FLAG_SYNC_IO);
	if (async_io) {
		bio_lock = (struct udk_mutex *)malloc(sizeof(struct udk_mutex));
		if (bio_lock == NULL) {
			udk_error("Failed to malloc udk_mutex\n");
			return E_HM_NOMEM;
		}
		udk_mutex_init(bio_lock);
		cb = partition_end_bio;
	}

	struct bio_params params = {
		.disk		= disk,
		.bio_lock	= bio_lock,
		.cb		= cb,
		.start_sector	= start_sector,
	};
	bio = prepare_bio(params, data, bv_len);
	if (bio == NULL) {
		if (async_io) {
			free(bio_lock);
		}
		return E_HM_NOMEM;
	}

	ret = submit_bio(async_io, bio_lock, disk, bio);
	return ret;
}

static int partition_read_pte(struct udk_disk *disk, unsigned int logical_size,
			      struct efi_partition **pte_handle, struct efi_header *gpt)
{
	unsigned int bv_len;
	struct efi_partition *pte = NULL;
	int ret;

	/* read pte */
	bv_len = gpt->num_partition_entries * gpt->sizeof_partition_entry;
	bv_len = ALIGN_UP(bv_len, logical_size);
	pte = (struct efi_partition *)malloc(bv_len);
	if (pte == NULL) {
		return E_HM_NOMEM;
	}
	*pte_handle = pte;

	ret = partition_read(disk, (unsigned char *)pte, bv_len,
			     gpt->partition_entry_lba *
			     (logical_size / UDK_BLKSIZE_DEFAULT));
	return ret;
}

static int partition_read_gpt(struct udk_disk *disk, unsigned int logical_size,
			      struct efi_header **gpt_handle)
{
	unsigned int bv_len;
	struct efi_header *gpt = NULL;
	int ret;

	/* read gpt */
	bv_len = ALIGN_UP(sizeof(struct efi_header), logical_size);
	gpt = (struct efi_header *)malloc(bv_len);
	if (gpt == NULL) {
		return E_HM_NOMEM;
	}
	*gpt_handle = gpt;

	ret = partition_read(disk, (unsigned char *)gpt, bv_len,
			     GPT_PRIMARY_PARTITION_TABLE_LBA *
			     (logical_size / UDK_BLKSIZE_DEFAULT));
	/* It may be desired to check the validity of the gpt data in the future,
	   which is currently not implemented yet */
	return ret;
}

static int partition_parse_efi(struct udk_parsed_partitions *parsed_parts,
			       struct udk_disk *disk, unsigned int logical_size)
{
	int ret;
	struct efi_header *gpt = NULL;
	struct efi_partition *pte = NULL;

	ret = partition_read_gpt(disk, logical_size, &gpt);
	if (ret != E_HM_OK) {
		goto err;
	}

	ret = partition_read_pte(disk, logical_size, &pte, gpt);
	if ((ret == E_HM_OK) && (gpt != NULL) && (pte != NULL)) {
		ret = partition_parse_efi_part(parsed_parts, pte, gpt->last_usable_lba,
					       logical_size);
	}

err:
	free(gpt);
	free(pte);
	return ret;
}

static int partition_check_header_flag(struct msdos_header *header)
{
	unsigned int idx;
	bool is_efi = false;

	/* check header's os_type_byte whether EFI flag */
	for (idx = 0; idx < MBR_PARTITION_NUM; idx++) {
		struct msdos_partition parts = header->parts[idx];
		if (parts.unused[OS_TYPE_BYTE_IDX] == EFI_PMBR_OSTYPE_EFI_GPT ||
		    parts.unused[OS_TYPE_BYTE_IDX] == EFI_PMBR_OSTYPE_EFI) {
			is_efi = true;
			break;
		}
	}

	return is_efi ? E_HM_OK : E_HM_INVAL;
}

static int partition_check_efi(struct udk_parsed_partitions *parsed_parts,
			       struct msdos_header *header,
			       struct udk_disk *disk, unsigned int logical_size)
{
	if (partition_check_header_flag(header) != E_HM_OK) {
		return E_HM_INVAL;
	}

	return partition_parse_efi(parsed_parts, disk, logical_size);
}

static int partition_check_msdos(struct udk_parsed_partitions *parsed_parts,
				 struct msdos_header *header)
{
	if (msdos_magic_present(header)) {
		partition_parse_msdos_part(parsed_parts, header);
	}
	return E_HM_OK;
}

static bool parts_name_valid(char *name)
{
	const char *dangerous_ch = "./\\";
	char *c = name;

	while (*c != '\0') {
		if (strchr(dangerous_ch, *c)) {
			udk_error("invalid name %s\n", name);
			return false;
		}
		c++;
	}

	return true;
}

static int partition_check_name(struct udk_parsed_partitions *parsed_parts)
{
	if (!parts_name_valid(&parsed_parts->name[0])) {
		return E_HM_INVAL;
	}

	for (uint32_t i = 0; i < parsed_parts->limit; i++) {
		if (!parts_name_valid(&parsed_parts->parts[i].name[0])) {
			return E_HM_INVAL;
		}
	}

	return E_HM_OK;
}

static int partition_check(struct udk_parsed_partitions *parsed_parts,
			   struct udk_disk *disk)
{
	struct msdos_header *header = NULL;
	unsigned int bv_len = sizeof(struct msdos_header);
	unsigned int logical_size;
	int ret;

	if (disk->logical_block_size != 0) {
		logical_size = disk->logical_block_size;
	} else {
		logical_size = UDK_BLKSIZE_DEFAULT;
	}
	bv_len = ALIGN_UP(bv_len, logical_size);
	header = (struct msdos_header *)malloc(bv_len);
	if (header == NULL) {
		udk_warn("Failed to malloc header for read\n");
		return E_HM_NOMEM;
	}

	/* read first MBR */
	ret = partition_read(disk, (unsigned char *)header, bv_len, 0);
	if (ret != E_HM_OK) {
		free(header);
		return ret;
	}

	/* check partition type: efi must before msdos */
	ret = partition_check_efi(parsed_parts, header, disk, logical_size);
	if (ret != E_HM_OK) {
		ret = partition_check_msdos(parsed_parts, header);
	}

	if (ret == E_HM_OK) {
		ret = partition_check_name(parsed_parts);
	}

	free(header);
	return ret;
}

static int partition_set_name(struct udk_disk *disk, uint32_t partno, char *buf)
{
	int ret = 0;
	unsigned long dname_len = strnlen(disk->name, DISK_NAME_LEN);
	if (dname_len >= DISK_NAME_LEN) {
		ret = -EINVAL;
	} else if (partno == 0) {
		ret = snprintf_s(buf, PARTITION_NAME_LEN, PARTITION_NAME_LEN - 1,
				 "%s", disk->name);
	} else if (dname_len > 0 &&
		   isdigit((unsigned char)(disk->name[dname_len - 1]))) {
		ret = snprintf_s(buf, PARTITION_NAME_LEN, PARTITION_NAME_LEN - 1,
				 "%sp%u", disk->name, partno);
	} else {
		ret = snprintf_s(buf, PARTITION_NAME_LEN, PARTITION_NAME_LEN - 1,
				 "%s%u", disk->name, partno);
	}

	return ret;
}

static void partition_init_one(struct udk_partition *part,
			       struct udk_disk *disk, uint32_t partno,
			       uint64_t start, uint64_t len)
{
	mem_zero_s(*part);
	part->start_sect = start;
	part->nr_sects = len;
	part->partno = partno;
	part->udkdisk = disk;
	part->openers = 0;
	udk_mutex_init(&part->lock);
	udk_disk_stat_init(part);
}

static void partition_free_devt(devnum_t devt)
{
	if (MAJOR(devt) == UDK_PART_EXT_MAJOR) {
		idr_remove(&udk_part_ext_minor_idr, MINOR(devt));
	}
}

static int partition_add_device(struct udk_disk *disk, uint32_t partno, char *name,
								struct udk_partition *part, struct udk_device *udkdev)
{
	int ret;
	int major;
	int minor;
	devnum_t devt;

	if (partno < (uint32_t)disk->minors) {
		major = disk->major;
		minor = disk->first_minor + partno;
	} else {
		major = UDK_PART_EXT_MAJOR;
		minor = idr_alloc(&udk_part_ext_minor_idr, part);
		if (minor < 0) {
			return -ENOMEM;
		}
	}
	devt = MKDEV(major, minor);
	ret = udk_disk_device_add(name, devt, udkdev);
	if (ret != E_HM_OK) {
		partition_free_devt(devt);
	}

	return ret;
}

static struct udk_partition *partition_add_one(struct udk_disk *disk,
					       uint32_t partno, uint64_t start,
					       uint64_t len, char *part_name)
{
	struct udk_partition *part = NULL;
	struct udk_device *udkdev = NULL;
	char *name = NULL;
	int ret;

	if (partno >= MAX_PARTITIONS) {
		udk_warn("partno[%u] out of limit: %d\n", partno,
			 MAX_PARTITIONS - 1);
		return NULL;
	}
	if (disk->parts[partno] != NULL) {
		return NULL;
	}
	part = (struct udk_partition *)malloc(sizeof(*part));
	if (part == NULL) {
		return NULL;
	}
	partition_init_one(part, disk, partno, start, len);
	NOFAIL(strcpy_s(part->name, sizeof(part->name), part_name));
	udkdev = &part->udkdev;
	name = (char *)malloc(PARTITION_NAME_LEN);
	if (name == NULL) {
		free(part);
		return NULL;
	}
	ret = partition_set_name(disk, partno, name);
	if (ret < 0) {
		udk_error("Failed to set partition name\n");
		free(name);
		free(part);
		return NULL;
	}
	udkdev->parent = &disk->part0.udkdev;
	disk->bd_queue = disk->part0.udkdisk->bd_queue;

	ret = partition_add_device(disk, partno, name, part, udkdev);
	if (ret != E_HM_OK) {
		free(name);
		free(part);
		return NULL;
	}

#ifdef CONFIG_FSCK_BOOST
	fsck_boost_start(udkdev->devt, part->name, len);
#endif

#ifdef CONFIG_HISI_UDK_MAS_BLK
	mas_blk_check_partition_done(disk, true);
#endif
	free(name);
	return part;
}

static void partition_add_parts(struct udk_disk *disk,
				const struct udk_parsed_partitions *parsed_parts)
{
	struct udk_partition *part = NULL;
	uint64_t size, from;
	uint32_t i;

	for (i = 0; i < parsed_parts->limit; i++) {
		size = parsed_parts->parts[i].size;
		if (size == 0) {
			continue;
		}
		from = parsed_parts->parts[i].from;
		part = partition_add_one(disk, i + 1, from, size,
					 parsed_parts->parts[i].name);
		if (part == NULL) {
			udk_warn("Skip bad data in parsing part, part=%u\n", i + 1);
		} else {
			disk->parts[i + 1] = part;
		}
	}
}

void udk_partition_destroy_parts_device(struct udk_disk *disk)
{
	int i;

	for (i = 0; i < MAX_PARTITIONS; i++) {
		if (disk->parts[i] != NULL) {
			partition_free_devt(disk->parts[i]->udkdev.devt);
			udk_device_destroy(&disk->parts[i]->udkdev);
		}
	}
}

static void partition_destroy_parts(struct udk_disk *disk)
{
	int i;

	for (i = 0; i < MAX_PARTITIONS; i++) {
		if (disk->parts[i] != NULL) {
			free(disk->parts[i]);
			disk->parts[i] = NULL;
		}
	}
}

static void partition_init_parts(struct udk_disk *disk)
{
	int i;

	for (i = 0; i < MAX_PARTITIONS; i++) {
		if (disk->parts[i]) {
			free(disk->parts[i]);
			disk->parts[i] = NULL;
		}
	}
}

int udk_partition_set_flag(struct udk_partition *part)
{
	udk_mutex_lock(&part->lock);
	if (part->openers > 0) {
		udk_mutex_unlock(&part->lock);
		udk_error("Partition has opener, please close it first\n");
		return E_HM_BUSY;
	}
	part->openers = DEVICE_OPEN_INVALID_FLAG;
	udk_mutex_unlock(&part->lock);
	return E_HM_OK;
}

void udk_partition_reset_flag(struct udk_partition *part)
{
	udk_mutex_lock(&part->lock);
	part->openers = 0;
	udk_mutex_unlock(&part->lock);
}

int udk_scan_partitions(struct udk_disk *disk)
{
	struct udk_parsed_partitions *parsed_parts =  NULL;
	uint32_t nr_parts;
	int ret;

	if (disk == NULL || disk->minors < 0) {
		return E_HM_INVAL;
	}

	if (disk->minors <= 1) {
		udk_info("No other partitions, skip scanning\n");
		return E_HM_OK;
	}

	partition_init_parts(disk);

	if (disk->part0.nr_sects == 0) {
		return E_HM_OK;
	}

	parsed_parts = (struct udk_parsed_partitions *)malloc(sizeof(struct udk_parsed_partitions));
	if (parsed_parts == NULL) {
		return E_HM_NOMEM;
	}

	nr_parts = MAX_PARTITIONS - 1;
	parsed_parts->parts = malloc(sizeof(parsed_parts->parts[0]) * nr_parts);
	if (parsed_parts->parts == NULL) {
		free(parsed_parts);
		return E_HM_NOMEM;
	}

	parsed_parts->limit = nr_parts;
	mem_zero_b(parsed_parts->parts,
		   parsed_parts->limit * sizeof(parsed_parts->parts[0]));

	ret = partition_set_name(disk, 0, parsed_parts->name);
	if (ret < 0) {
		udk_error("Failed to set partition name\n");
		free(parsed_parts->parts);
		free(parsed_parts);
		return ret;
	}

	ret = partition_check(parsed_parts, disk);
	if (ret != E_HM_OK) {
		udk_error("Check msdos or efi failed, %s\n", hmstrerror(ret));
	} else {
		partition_add_parts(disk, parsed_parts);
	}

	free(parsed_parts->parts);
	free(parsed_parts);
	return ret;
}

int udk_destroy_partitions(struct udk_disk *disk)
{
	if (disk == NULL || disk->minors < 0) {
		return E_HM_INVAL;
	}

	if (disk->minors <= 1) {
		udk_info("No other partitions need to destroy\n");
		return E_HM_OK;
	}

	if (disk->part0.nr_sects == 0) {
		return E_HM_OK;
	}

	partition_destroy_parts(disk);

	return E_HM_OK;
}

int udk_block_ext_region_register(void)
{
	return udk_register_blkdev_region(UDK_PART_EXT_MAJOR, "udk-block-ext-region");
}
