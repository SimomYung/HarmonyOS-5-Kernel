/* SPDX-License-Identifier: GPL-2.0-only */

#include <liblinux/pal.h>
#include <libkbox/kbox.h>
#include <libkbox/kbox_snapshot.h>
#include <libhmlog/hmlog.h>
#include <fcntl.h>
#include <errno.h>
#include "xz_dec.h"

static int write_decompressed_data(const void *log, unsigned int len, uintptr_t dump_arg)
{
	int fd = (int)dump_arg;
	return liblinux_pal_ksys_write(fd, log, len);
}

static int do_decompress_kbox_snapshot(const char *out_path, const void *buff, size_t size)
{
	int ret = 0;
	int fd = -1;

	fd = liblinux_pal_ksys_open(out_path, O_CREAT | O_WRONLY,
				    S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
	if (fd < 0) {
		hm_error("open %s failed, ret=%s\n", out_path, hmstrerror(fd));
		ret = fd;
	} else {
		ret = kbox_decompress_snapshot(buff, size, liblinux_xz_decompress, write_decompressed_data, (uintptr_t)fd);
		if (ret < 0) {
			hm_error("dump snapshot data to %s failed, ret=%s\n", out_path, hmstrerror(ret));
		}

		(void)liblinux_pal_ksys_close(fd);
	}

	return ret;
}

int liblinux_decompress_kbox_snapshot(const char *out_path)
{
	int ret = 0;
	int region_id = -1;
	ssize_t region_size = 0;
	void *buff = NULL;
	ssize_t rsize = 0;

	region_id = kbox_open("snapshot");
	if (region_id < 0) {
		hm_error("open kbox snapshot region failed, ret=%s\n", hmstrerror(region_id));
		return -hmerrno2posix(region_id);
	}

	region_size = kbox_region_size(region_id);
	if (region_size < 0) {
		hm_error("read snapshot region data size failed, ret=%s\n", hmstrerror(region_size));
		ret = -hmerrno2posix(region_size);
		goto out;
	} else if (region_size == 0) {
		hm_info("snapshot region size is 0\n");
		ret = 0;
		goto out;
	}

	buff = malloc((size_t)region_size);
	if (buff == NULL) {
		hm_error("malloc buff failed\n");
		ret = -ENOMEM;
		goto out;
	}

	rsize = kbox_read(region_id, (char *)buff, (size_t)region_size);
	if (rsize < 0) {
		hm_error("read snapshot region data failed, ret=%s\n", hmstrerror(rsize));
		ret = -hmerrno2posix(rsize);
	} else if (rsize > 0) {
		ret = do_decompress_kbox_snapshot(out_path, (const void *)buff, (size_t)rsize);
		if (ret < 0) {
			ret = -EFAULT;
		}
	} else {
		hm_info("no snapshot data\n");
	}

out:
	kbox_close(region_id);
	free(buff);

	return ret;
}