/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description:
 * Create: 2019/12/30
 */

#ifndef CRYPTO_CORE_FS_H
#define CRYPTO_CORE_FS_H

#include <crypto_core_upgrade.h>
#include <linux/syscalls.h>
#include <linux/types.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <platform_include/basicplatform/linux/fs/vendor_fs_interface.h>

#define MSPC_MAX_EMMC_COS_NUMBER     5
#define MSPC_MIN_EMMC_COS_NUMBER     0

#define MSPC_ENCOS_MAGIC_LEN         8
#define MSPC_ENCOS_START_ID          3
#define MSPC_FLASH_COS_STRING        "COS5"
#define MSPC_ENCOS_MAGIC_STRING      "encd_cos"

#define MSPC_PTN_PATH_BUF_SIZE       128
#define MSPC_ENCOS_PARTITION_NAME    "hisee_encos"
#define MSPC_FS_PARTITION_NAME       "hisee_fs"

#ifdef CONFIG_CRYPTO_CORE_DISABLE_KEY
#define MSPC_IMAGE_PARTITION_NAME    "hisee_fs"
#else
#define MSPC_IMAGE_PARTITION_NAME    "hisee_img"
#endif

#define MSPC_ENCOS_FILE_MAX          MSPC_MAX_EMMC_COS_NUMBER

#define MSPC_ENCOS_FILE_NAME_LEN       8
#define MSPC_ENCOS_FILE_LEN            MSPC_MAX_IMG_SIZE
#define MSPC_ENCOS_TOTAL_FILE_SIZE    (MSPC_ENCOS_FILE_MAX * \
	MSPC_ENCOS_FILE_LEN)

/* Default mode when creating a file or dir if user doesn't set mode */
#define MSPC_FILESYS_DEFAULT_MODE     0660

enum {
	MSPC_PARTITON_IMG   = 0x5A,
	MSPC_PARTITON_ENCOS = 0xA5,
};

enum {
	FLASH_COS_EMPTY = 0,  /* encos partition is empty */
	FLASH_COS_EXIST,  /* exist cosflash, can run */
	FLASH_COS_ERASE,  /* cosflash has been erased by kernel */
};

enum mspc_image_access_type {
	MSPC_FLAG_INFO_READ = 0,
	MSPC_UPGRADE_FLAG_READ,
	MSPC_IMAGE_HEADER_READ,
	MSPC_ENCOS_HEADER_READ,
	MSPC_ENCOS_IMAGE_READ,
	MSPC_IMAGE_READ,
	MSPC_INSTALL_RESULT_READ,
	MSPC_READ_MAX,
	MSPC_FLAG_INFO_WRITE = 0x100,
	MSPC_UPGRADE_FLAG_WRITE,
	MSPC_IMAGE_HEADER_WRITE,
	MSPC_ENCOS_HEADER_WRITE,
	MSPC_IMAGE_WRITE,
	MSPC_INSTALL_RESULT_WRITE,
	MSPC_WRITE_MAX
};
struct encos_file_info {
	char name[MSPC_ENCOS_FILE_NAME_LEN];
	uint32_t offset;
	uint32_t size;
};

struct mspc_encos_header {
	char magic[MSPC_ENCOS_MAGIC_LEN];
	uint32_t total_size;
	uint32_t file_cnt;
	struct encos_file_info file[MSPC_ENCOS_FILE_MAX];
};

struct mspc_access_info {
	int8_t fullpath[MSPC_PTN_PATH_BUF_SIZE];
	uint32_t partition_type;
	uint32_t access_type;
	int32_t flags;
	uint64_t size;
	int64_t offset;
};

int32_t mspc_access_partition(int8_t *buf, uint32_t size,
			      struct mspc_access_info *access_info);
void mspc_parse_timestamp(const int8_t *time, uint32_t len,
			  union mspc_timestamp_info *timestamp);

#ifndef CONFIG_CRYPTO_CORE_V2
int32_t mspc_get_encos_img_header(struct mspc_encos_header *header);
int32_t mspc_get_and_parse_img_header(struct mspc_img_header *header);
int32_t mspc_check_flash_cos_file(uint32_t *state);
int32_t mspc_remove_flash_cos(void);
int32_t mspc_check_hisee_fs_empty(void);
#endif
#ifdef CONFIG_DFX_DEBUG_FS
int mspc_read_input_test_file(const char *fullname, char *buffer, size_t offset,
			      size_t size);
#endif

static inline loff_t crypto_core_sys_lseek(struct file *filp, loff_t offset, unsigned int whence)
{
	return vfs_llseek(filp, offset, whence);
}

static inline struct file *crypto_core_sys_open(const char __user *filename, int flags, umode_t mode)
{
	return filp_open(filename, flags, mode);
}

static inline int crypto_core_sys_close(struct file *filp)
{
	return filp_close(filp, NULL);
}

static inline ssize_t crypto_core_sys_write(struct file *filp, const char __user *buf, size_t count, loff_t *pos)
{
	return kernel_write(filp, buf, count, pos);
}

static inline ssize_t crypto_core_sys_read(struct file *filp, char __user *buf, size_t count, loff_t *pos)
{
	return kernel_read(filp, buf, count, pos);
}

static inline long crypto_core_sys_access(const char __user *filename, int mode)
{
	return vendor_access(filename, mode);
}

#define CRYPTO_CORE_SYNC_DATA_METADATA    0
#define CRYPTO_CORE_SYNC_DATA             1
static inline long crypto_core_sys_fsync(struct file *filp, int datasync)
{
	/* if datasync is not 0(false), only data will sync, metadata will not */
	return vfs_fsync(filp, datasync);
}

static inline long crypto_core_sys_unlink(const char __user *filename)
{
	return vendor_unlink(filename);
}

static inline long crypto_core_sys_mkdir(const char __user *filename, umode_t mode)
{
	return vendor_mkdir(filename, mode);
}
#endif /* CRYPTO_CORE_FS_H */
