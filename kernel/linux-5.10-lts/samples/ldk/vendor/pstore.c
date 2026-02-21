// SPDX-License-Identifier: GPL-2.0
#include <linux/syscalls.h>
#include <linux/blackbox.h>
#include <linux/blackbox_common.h>
#include "blackbox/platform_ap/rdr_ap_exception_logsave.h"
#include "blackbox/rdr_inner.h"
#include <liblinux/pal.h>
#include <linux/printk.h>
#include <securec.h>

#ifdef CONFIG_LIBLINUX_USE_LASTKLOG
void __override rdr_save_console_log(const char *path)
{
	pr_info("use overwrite of rdr_save_console_log\n");
}

void __override save_pstore_info(const char *dst_dir_str)
{
	char path[PATH_MAX] = {0};
	char *filename = "dmesg-ramoops-0";
	char *lastklog_buff = NULL;
	int fd = -1;
	char *curr = NULL;
	bool klog_valid = false;
	unsigned int size = 0;
	unsigned int header_len = 0;
	int ret;
	u64 lastklog_addr = 0ULL;
	u32 lastklog_size = 0U;

	ret = sprintf_s(path, sizeof(path), "%s/%s", dst_dir_str, filename);
	if (ret < 0) {
		pr_err("Failed to sprintf_s. path: %s, name: %s\n", dst_dir_str, filename);
		return;
	}

	fd = liblinux_pal_ksys_open(path, O_CREAT | O_WRONLY, FILE_LIMIT);
	if (fd < 0) {
		pr_err("Failed to open dmesg file. path:%s\n", dst_dir_str);
		return;
	}

	ret = liblinux_read_lastklog_memory_config(&lastklog_addr, &lastklog_size);
	if (ret < 0) {
		pr_err("Failed to read lastklog addr and size: ret:%d.\n", ret);
		ksys_close(fd);
		return;
	}
	lastklog_buff = (char *)ioremap_wc(lastklog_addr, lastklog_size);
	if (lastklog_buff == NULL) {
		pr_err("Failed to mmap lastklog.\n");
		liblinux_pal_ksys_close(fd);
		return;
	}

	klog_valid = liblinux_is_lastklog_valid(lastklog_buff, lastklog_size, &header_len);
	if (klog_valid) {
		/* do not save header */
		curr = lastklog_buff + header_len;
	} else {
		curr = lastklog_buff;
	}
	while (*curr == 0) {
		curr++;
		if (curr >= lastklog_buff + lastklog_size) {
			curr = NULL;
			break;
		}
	}

	if (curr != NULL) {
		size = (lastklog_buff + lastklog_size) - curr;
	}
	pr_info("start to dump klog to dmesg file. path:%s, klog_size:0x%x, size:0x%x, header_len:%u, klog_valid:%u\n",
		path, lastklog_size, size, header_len, (unsigned int)klog_valid);
	if (size > 0) {
		ret = liblinux_pal_ksys_write(fd, (const void *)curr, size);
		if (ret < 0) {
			pr_err("Failed to write %s ret %d.\n", path, ret);
		}
	}
	/* clear lastklog after saving to disk */
	(void)memset_s(lastklog_buff, lastklog_size, 0, lastklog_size);

	liblinux_pal_ksys_fsync(fd);
	liblinux_pal_ksys_close(fd);
	iounmap(lastklog_buff);
}

#else /* !CONFIG_LIBLINUX_USE_LASTKLOG */

#define MAX_DUMP_KMSG_SIZE (512 * 1024) /* dump at max 512K */

static int open_file(const char *log_dir, const char *name)
{
	int fd = -1;
	char path[PATH_MAX] = {0};

	int ret = snprintf_s(path, sizeof(path), sizeof(path) - 1, "%s/%s", log_dir, name);
	if (ret > 0)
		fd = liblinux_pal_ksys_open(path, O_CREAT | O_WRONLY, FILE_LIMIT);

	return fd;
}

static int dump_klog(const char *log, unsigned int len, uintptr_t arg)
{
	int fd = (int)((unsigned long)arg);

	int ret = liblinux_pal_ksys_write(fd, log, len);

	return (ret >= 0) ? 0 : -1;
}

void __override rdr_save_console_log(const char *path)
{
	pr_info("use overwrite of rdr_save_console_log\n");
}

void __override save_pstore_info(const char *dst_dir_str)
{
	unsigned long max_size = MAX_DUMP_KMSG_SIZE;

	int fd = open_file(dst_dir_str, "dmesg-ramoops-0");
	if (fd < 0) {
		pr_err("Failed to open dmesg file. path:%s\n", dst_dir_str);
	} else {
		uintptr_t arg = (uintptr_t)fd;

		pr_info("start to dump klog to dmesg file. path:%s\n", dst_dir_str);
		liblinux_iter_last_klog(max_size, dump_klog, arg);
		liblinux_pal_ksys_fsync(fd);
	}

	if (fd >= 0) {
		liblinux_pal_ksys_close(fd);
	}
}
#endif /* CONFIG_LIBLINUX_USE_LASTKLOG */
