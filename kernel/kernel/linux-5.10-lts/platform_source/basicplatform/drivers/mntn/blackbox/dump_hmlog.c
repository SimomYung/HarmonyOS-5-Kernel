// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: hongmeng log dump function moudle.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 6 09:46:43 2023
 */

#include <linux/module.h>
#include <linux/io.h>
#include <asm/page.h>
#include <linux/types.h>
#include <linux/syscalls.h>
#include <securec.h>
#include <platform_include/basicplatform/linux/util.h>

#include "rdr_inner.h"
#include "rdr_print.h"

#define KERNEL_LOGBUF_BIN_FILE "kernel_logbuff.bin"
#define HM_KLOG_TXT_FILE       "hm_klog.txt"
#define HM_SNAPSHOT_TXT_FILE   "hm_snapshot.txt"

#ifdef CONFIG_DUMP_DEVHOST_CORE_ON_EXCEPTION
#include <global_ddr_map.h>

#define COREDUMP_IMAGE_MAGIC "MCOR"
#define MAGIC_SIZE 4U

struct coredump_image_header {
	char magic[MAGIC_SIZE];
	/* The number of process coredumps that are stored to the memory. */
	uint8_t nr_procdump;
	/* Backup of nr_procdump */
	uint8_t last_nr_procdump;
	/* Flags used to indicate the storage status, for example, whether to compress.
	 * bit0: is coredump data compressed, 0:not, 1:yes.
	 */
	uint16_t flags;
	uint32_t total_size;
	uint32_t used_size;
};

static int create_memorydump_path(char *memorydump_path, int path_len, const char *raw_path)
{
	int ret = 0;
	const char *time_stamp = NULL;

	/*
	 * PATH_ROOT example: /data/hisi_logs/
	 * raw path example: /data/hisi_logs/20231226080151-00000011
	 */
	if (strncmp(raw_path, PATH_ROOT, strlen(PATH_ROOT)) == 0) {
		/* extract timestamp from raw path */
		time_stamp = raw_path + strlen(PATH_ROOT);
	} else {
		BB_PRINT_ERR("[%s] extract timestamp from %s fail!\n", __func__, raw_path);
		return -1;
	}

	ret = snprintf_s(memorydump_path, path_len, path_len - 1,
			 "%s%s/%s/", PATH_ROOT, PATH_MEMDUMP, time_stamp);
	if (unlikely(ret < 0)) {
		BB_PRINT_ERR("[%s] snprintf_s memorydump path ret %d!\n", __func__, ret);
		return ret;
	}

	ret = rdr_create_dir(memorydump_path);
	if (unlikely(ret != 0)) {
		BB_PRINT_ERR("[%s] create memorydump path fail\n", __func__);
		return ret;
	}

	return ret;
}

static bool has_devhost_coredump(void *buff_addr, size_t size)
{
	/* return true when devhost core's data is useful. */

	struct coredump_image_header *img_header = NULL;

	if (size <= sizeof(struct coredump_image_header))
		return false;

	img_header = (struct coredump_image_header *)(buff_addr);
	if (strncmp(img_header->magic, COREDUMP_IMAGE_MAGIC, MAGIC_SIZE) ||
	    (img_header->last_nr_procdump == 0))
		return false;

	return true;
}

static void dump_devhost_coredump(const char *path_raw)
{
	int err;
	unsigned long long paddr = (unsigned long long)RESERVED_COREDUMP_PHYMEM_BASE;
	size_t buff_size = (size_t)RESERVED_COREDUMP_PHYMEM_SIZE;
	void *buff_addr = NULL;
	char memorydump_path[PATH_MAXLEN] = {0};

	paddr &= PAGE_MASK;
	buff_size = PAGE_UP(buff_size);
	buff_addr = ioremap_wc(paddr, buff_size);
	if (buff_addr == NULL) {
		BB_PRINT_ERR("[%s] map coredump be memory fail\n", __func__);
		return;
	}

	if (!has_devhost_coredump(buff_addr, buff_size)) {
		iounmap((void __iomem *)buff_addr);
		return;
	}

	err = create_memorydump_path(memorydump_path, PATH_MAXLEN, path_raw);
	if (err) {
		iounmap((void __iomem *)buff_addr);
		return;
	}

	err = rdr_savebuf2fs(memorydump_path, "hm_coredump.bin", buff_addr, (u32)buff_size, 0);
	if (err)
		BB_PRINT_ERR("[%s] save devhost coredump failed\n", __func__);

	iounmap((void __iomem *)buff_addr);
}
#else
static void dump_devhost_coredump(const char *path_raw)
{
	unused(path_raw);
}
#endif

static int klog_dump_func(const char *log, unsigned int len, uintptr_t arg)
{
	int fd = (int)((unsigned long)arg);

	int ret = ksys_write(fd, log, (size_t)len);

	return (ret >= 0) ? 0 : -1;
}

static int dump_hm_klog_txt(const char *path, const char *klog_buf, size_t klog_buflen)
{
	int ret = 0;
	char dump_path[PATH_MAXLEN] = {};
	int fd = -1;

	if ((path == NULL) || (klog_buf == NULL) || (klog_buflen == 0)) {
		BB_PRINT_ERR("[%s] invalid arguments\n", __func__);
		return -1;
	}

	ret = snprintf_s(dump_path, PATH_MAXLEN, PATH_MAXLEN - 1,
			 "%s/"HM_KLOG_TXT_FILE, path);
	if (unlikely(ret < 0)) {
		BB_PRINT_ERR("[%s] failed to snprintf path:%s\n", __func__, path);
		return -1;
	}

	fd = ksys_open(dump_path, O_CREAT | O_WRONLY | O_APPEND, FILE_LIMIT);
	if (fd < 0) {
		BB_PRINT_ERR("[%s] failed to open dump_file, ret=%d, file path:%s\n",
			     __func__, fd, dump_path);
		return -1;
	}

	BB_PRINT_PN("[%s] start to dump klog. path:%s, buflen:0x%zx\n", __func__, dump_path, klog_buflen);
	ret = liblinux_iter_on_klog_data(klog_buf, klog_buflen, klog_dump_func, (uintptr_t)fd);
	if (ret < 0)
		BB_PRINT_ERR("[%s] failed to dump klog from buffer, ret=%d\n", __func__, ret);
	else
		BB_PRINT_PN("[%s] save klog successed. path:%s, buflen:0x%zx, ret:%d\n",
				__func__, dump_path, klog_buflen, ret);

	(void)ksys_close(fd);

	return ret;
}

static int map_klog(void **ret_addr, u32 *ret_size)
{
	int err;
	u64 klog_paddr = 0ULL;
	u32 klog_size = 0;
	void *klog_buff = NULL;

	err = liblinux_read_klog_config(&klog_paddr, &klog_size);
	if (err) {
		BB_PRINT_ERR("[%s] Read klog config fail, ret=%d\n", __func__, err);
		return -EINVAL;
	}

	klog_paddr &= PAGE_MASK;
	klog_size = PAGE_UP(klog_size);
	klog_buff = ioremap_wc(klog_paddr, klog_size);
	if (klog_buff == NULL) {
		BB_PRINT_ERR("[%s] map klog memory fail\n", __func__);
		return -ENOMEM;
	}

	*ret_addr = klog_buff;
	*ret_size = klog_size;

	return 0;
}

static bool need_save_log(void)
{
	bool ret = true;
	u32 reboot_type = rdr_get_reboot_type();
	if (reboot_type < REBOOT_REASON_LABEL1 || (reboot_type >= REBOOT_REASON_LABEL4 &&
	    reboot_type < REBOOT_REASON_LABEL5)) {
		ret = false;
	}

	BB_PRINT_PN("[%s] Test need save log, reboot_type=[0x%x], ret=%d\n", __func__, reboot_type, ret);

	return ret;
}

int rdr_hmlog_save_klog(char **ret_buf, size_t *ret_buflen)
{
	int err = 0;
	u64 klog_paddr = 0ULL;
	u32 klog_size = 0;
	char *buffer = NULL;
	size_t read_len;

	if (!need_save_log())
		return 0;

	if (ret_buf == NULL || ret_buflen == NULL) {
		return -EINVAL;
	}

	err = liblinux_read_klog_config(&klog_paddr, &klog_size);
	if (err) {
		BB_PRINT_ERR("[%s] Read klog config fail, ret=%d\n", __func__, err);
		return -EINVAL;
	}
	buffer = (char *)vmalloc(klog_size);
	if (buffer == NULL) {
		BB_PRINT_PN("[%s] Failed to alloc klog buffer\n", __func__);
		return -ENOMEM;
	}

	/* set offset to -1 to read from last reboot */
	err = liblinux_read_klog_data(buffer, klog_size, -1, &read_len);
	if (err != 0) {
		BB_PRINT_PN("[%s] Failed to read klog data, err=%d\n", __func__, err);
		vfree(buffer);
		return -1;
	}
	*ret_buf = buffer;
	*ret_buflen = read_len;

	return err;
}

static void clear_kbox_snapshot(void)
{
	BB_PRINT_PN("Erase kbox snapshot\n");
	int err = liblinux_erase_kbox_region("snapshot");
	if (err < 0)
		BB_PRINT_ERR("[%s] failed to erase snapshot, err=%d\n", __func__, err);
}

static void clear_ekbox(void)
{
	int ret = 0;
	unsigned int ekbox_size = 0;
	u64 ekbox_paddr = 0;
	ret = liblinux_read_ekbox_memory_config(&ekbox_paddr, &ekbox_size);
	if (ret != 0) {
		BB_PRINT_ERR("[%s] Failed to read ekbox memory, ret:%d\n", __func__, ret);
		return;
	}

	void *ekbox_buf = ioremap_wc(ekbox_paddr, ekbox_size);
	if (ekbox_buf == NULL) {
		BB_PRINT_ERR("[%s] Failed to mmap ekbox memory\n", __func__);
		return;
	}
	BB_PRINT_PN("Clear ekbox memory.\n");
	ret = memset_s(ekbox_buf, ekbox_size, 0, ekbox_size);
	if (ret != 0) {
		BB_PRINT_ERR("[%s] Failed to clear ekbox memory, ret:%d\n", __func__, ret);
	}
	iounmap(ekbox_buf);
}

void rdr_hmlog_dump_post(const char *klog_buf)
{
	clear_kbox_snapshot();
	clear_ekbox();
	/* free klog buffer allocated from rdr_hmlog_save_klog */
	if (klog_buf != NULL)
		vfree(klog_buf);
}

static void unmap_klog(void *addr) {
	if (addr != NULL) {
		iounmap((void __iomem *)addr);
	}
}

static void dump_hm_klog_bin(const char *path)
{
	int err;
	void *buff_addr = NULL;
	u32 buff_size = 0;
	u32 is_append = 0U;

	err = map_klog(&buff_addr, &buff_size);
	if (err)
		return;

	err = rdr_savebuf2fs(path, KERNEL_LOGBUF_BIN_FILE, buff_addr, (u32)buff_size, is_append);
	if (err)
		BB_PRINT_ERR("[%s] save hm klog to %s"KERNEL_LOGBUF_BIN_FILE" fail\n", __func__, path);

	unmap_klog(buff_addr);
}

static void dump_hm_klog(const char *path, const char *klog_buf, size_t klog_buflen)
{
	int err = dump_hm_klog_txt(path, klog_buf, klog_buflen);
	if (err < 0)
		dump_hm_klog_bin(path);
}

static void dump_hm_klog_for_sg(const char *path, const char *klog_buf, size_t klog_buflen)
{
    char dump_path[PATH_MAXLEN] = {};
    int ret = 0;
    
	if ((path == NULL) || (klog_buf == NULL) || (klog_buflen == 0)) {
		BB_PRINT_ERR("[%s] invalid arguments\n", __func__);
		return;
	}
	dump_hm_klog_txt(path, klog_buf, klog_buflen);
	ret = snprintf_s(dump_path, PATH_MAXLEN, PATH_MAXLEN - 1,
			 "%s/"HM_KLOG_TXT_FILE, path);
	if (unlikely(ret < 0)) {
		BB_PRINT_ERR("[%s] failed to snprintf path:%s\n", __func__, path);
		return;
	}

	ret = (int)SYS_CHOWN((const char __user *)dump_path, 3520, 3520);
	if (ret)
		BB_PRINT_ERR("chown %s uid [3520] gid [3520] failed err [%d]!\n", dump_path, ret);
}

#ifndef CONFIG_LIBLINUX_CDC_RDA
static int map_kbox(void **ret_addr, unsigned long *ret_size)
{
	int err;
	unsigned long long kbox_paddr = 0ULL;
	unsigned long kbox_size = 0UL;
	void *kbox_buff = NULL;

	err = liblinux_read_kbox_config(&kbox_paddr, &kbox_size);
	if (err) {
		BB_PRINT_ERR("[%s] read kbox config fail, ret=%d\n", __func__, err);
		return -EINVAL;
	}

	kbox_paddr &= PAGE_MASK;
	kbox_size = PAGE_UP(kbox_size);

	kbox_buff = ioremap_wc(kbox_paddr, kbox_size);
	if (kbox_buff == NULL) {
		BB_PRINT_ERR("[%s] map kbox memory fail\n", __func__);
		return -ENOMEM;
	}

	*ret_addr = kbox_buff;
	*ret_size = kbox_size;
	return 0;
}

static void unmap_kbox(void *addr)
{
	if (addr != NULL) {
		iounmap((void __iomem *)addr);
	}
}

static void dump_hm_kbox_bin(const char *path)
{
	int err;
	void *buff_addr = NULL;
	unsigned long buff_size = 0UL;
	u32 is_append = 1U;

	err = map_kbox(&buff_addr, &buff_size);
	if (err)
		return;

	err = rdr_savebuf2fs(path, "kernel_logbuff.bin", buff_addr, (u32)buff_size, is_append);
	if (err)
		BB_PRINT_ERR("[%s] save hm klog to %skernel_logbuff.bin fail\n", __func__, path);

	unmap_kbox(buff_addr);
}
#else
static void dump_hm_kbox_bin(const char *path)
{
}
#endif

#ifdef CONFIG_DUMP_SNAPSHOT_PLAINTEXT
static int dump_hm_snapshot_txt(const char *path)
{
	char dump_path[PATH_MAXLEN] = {};
	int ret = 0;

	ret = snprintf_s(dump_path, PATH_MAXLEN, PATH_MAXLEN - 1,
			 "%s/"HM_SNAPSHOT_TXT_FILE, path);
	if (unlikely(ret < 0)) {
		BB_PRINT_ERR("[%s] failed to snprintf path:%s\n", __func__, path);
	} else {
		ret = liblinux_decompress_kbox_snapshot(dump_path);
		if (ret < 0) {
			BB_PRINT_ERR("[%s] failed to decompress snapshot, ret=%d\n",
				     __func__, ret);
		}
	}

	return ret;
}
#else
static int dump_hm_snapshot_txt(const char *path)
{
	unused(path);
	return -1;
}
#endif

static void dump_hm_kbox_snapshot(const char *path)
{
	int err = dump_hm_snapshot_txt(path);
	if (err < 0)
		dump_hm_kbox_bin(path);
}

static int concat_path(char *buffer, unsigned int buf_size, const char *path, const char *name)
{
	int ret = 0;
	unsigned int plen = 0;
	if (path != NULL) {
		plen = (unsigned int)strnlen(path, buf_size);
	}
	if (plen == 0) {
		ret = snprintf_s(buffer, buf_size, buf_size - 1, "%s", name);
	} else if (path[plen - 1] == '/') {
		ret = snprintf_s(buffer, buf_size, buf_size - 1, "%s%s", path, name);
	} else {
		ret = snprintf_s(buffer, buf_size, buf_size - 1, "%s/%s", path, name);
	}
	return ret;
}

static int dump_ekbox_txt(void *buffer, unsigned int size, const char *path, const char *name)
{
	int ret = 0;
	int fd = -1;
	char dump_path[PATH_MAXLEN] = {};
	ret = concat_path(dump_path, sizeof(dump_path), path, name);
	if (unlikely(ret < 0)) {
		BB_PRINT_ERR("[%s] failed to snprintf path:%s, ret:%d\n", __func__, path, ret);
		return -1;
	}

	fd = ksys_open(dump_path, O_CREAT | O_WRONLY, FILE_LIMIT);
	if (fd < 0) {
		BB_PRINT_ERR("[%s] failed to open ekbox dump_file, fd:%d, file path:%s\n",
			     __func__, fd, dump_path);
		return -1;
	}
	ret = liblinux_dump_ekbox(buffer, size, klog_dump_func, (uintptr_t)fd);
	if (ret < 0) {
		BB_PRINT_ERR("[%s] read ekbox data failed, ret:%d\n", __func__, ret);
	}
	(void)ksys_close(fd);
	return ret;
}

int rdr_hmlog_dump_ekbox(const char* dump_path, const char *file_name)
{
	int ret = 0;
	unsigned int ekbox_size = 0;
	u64 ekbox_paddr = 0;
	ret = liblinux_read_ekbox_memory_config(&ekbox_paddr, &ekbox_size);
	if (ret != 0) {
		BB_PRINT_ERR("[%s] Failed to read ekbox memory, ret:%d\n", __func__, ret);
		return -EINVAL;
	}

	void *ekbox_buf = ioremap_wc(ekbox_paddr, ekbox_size);
	if (ekbox_buf == NULL) {
		BB_PRINT_ERR("[%s] Failed to mmap ekbox memory.\n", __func__);
		return -ENOMEM;
	}
	ret = dump_ekbox_txt(ekbox_buf, ekbox_size, dump_path, file_name);
	if (ret < 0) {
		BB_PRINT_ERR("[%s] Failed to dump ekbox data, ret:%d\n", __func__, ret);
	} else {
		BB_PRINT_PN("[%s] save ekbox log successed. path:%s, buflen:0x%x, ret:%d\n",
				__func__, dump_path, ekbox_size, ret);
	}
	iounmap(ekbox_buf);
	return ret;
}

void rdr_hmlog_dump_on_exception(const char *path, const char *klog_buf, size_t klog_buflen)
{
	char dump_path[PATH_MAXLEN] = {};
	int ret = 0;
	if (path == NULL || !need_save_log())
		return;

	ret = snprintf_s(dump_path, PATH_MAXLEN, PATH_MAXLEN - 1, "%s/ap_log", path);
	if (unlikely(ret < 0)) {
		BB_PRINT_ERR("[%s] snprintf_s dump_path ret %d!\n", __func__, ret);
	} else {
		rdr_hmlog_dump_ekbox(dump_path, HM_KLOG_TXT_FILE);
		dump_hm_klog(dump_path, klog_buf, klog_buflen);
		dump_hm_klog_for_sg("/data/log/security_guard/panic_log", klog_buf, klog_buflen);
		dump_hm_kbox_snapshot(dump_path);
		if (check_mntn_switch(MNTN_KERNEL_DUMP_ENABLE))
			liblinux_dump_hm_track_info(dump_path, KERNEL_LOGBUF_BIN_FILE, false);
	}

	dump_devhost_coredump(path);
}
