/* SPDX-License-Identifier: GPL-2.0-only */
#include <liblinux/pal.h>
#include <linux/slab.h>
#include <securec.h>
#include <liblinux/pal.h>

#include "blackbox/rdr_inner.h"
#include "track_info.h"

#ifdef CONFIG_LIBLINUX_USE_TRACK_INFO
static int format_path(const char *dst_dir_str, unsigned int type, char *path, size_t path_size)
{
	int ret = 0;
	const char *filename = liblinux_track_info_file_name_of(type);
	if (filename == NULL) {
		pr_err("Invalid type: %u\n", type);
		return -1;
	}

	ret = sprintf_s(path, path_size, "%s/%s", dst_dir_str, filename);
	if (ret < 0)
		pr_err("Failed to sprintf_s: path: %s, type: %u\n", dst_dir_str, type);

	return ret > 0 ? 0 : -1;
}

static int write_track_info_record(const char *buf, unsigned int len, uintptr_t arg)
{
	int fd = (int)arg;
	int ret = liblinux_pal_ksys_write(fd, buf, len);
	if (ret < 0)
		pr_err("Failed to write fd %d: %d\n", fd, ret);

	return ret >= 0 ? 0 : -1;
}

static int dump_hm_track_info(void *track_info_ptr, size_t size, const char *dst_dir_str)
{
	char path[PATH_MAXLEN] = {};
	void *slot_header = NULL;
	void *region_header = NULL;
	unsigned int type_nr;
	int ret = 0;
	int err = 0;

	slot_header = liblinux_track_info_slot_header_of(track_info_ptr, size);
	if (slot_header == NULL) {
		pr_info("track_info: track_info_header is invalid.\n");
		return -1;
	}

	type_nr = liblinux_track_info_record_type_nr_of(slot_header);
	for (unsigned int type = 0; type < type_nr; type++) {
		region_header = liblinux_track_info_region_header_of(slot_header, type);
		if (region_header == NULL) {
			pr_err("track_info: region %u is invalid.\n", type);
			ret = -1;
			continue;
		}

		if (format_path(dst_dir_str, type, path, sizeof(path)) < 0) {
			ret = -1;
			continue;
		}

		int fd = liblinux_pal_ksys_open(path, O_CREAT | O_WRONLY, FILE_LIMIT);
		if (fd < 0) {
			pr_err("Failed to open track_info file. path: %s\n", path);
			ret = -1;
			continue;
		}

		pr_info("start to dump track_info %u to path: %s\n", type, path);
		err = liblinux_track_info_record_for_each(region_header,
							  write_track_info_record, fd);
		if (err < 0) {
			pr_err("Failed to dump track_info %u to path %s err: %d\n",
			       type, path, err);
			ret = -1;
		}
		liblinux_pal_ksys_fsync(fd);
		liblinux_pal_ksys_close(fd);
		pr_info("finish to dump track_info %u to path: %s\n", type, path);
	}

	return ret;
}

void liblinux_dump_hm_track_info(const char *dst_dir_str, const char *bin_file_name)
{
	u64 track_info_addr = 0ULL;
	u32 track_info_size = 0U;
	void *track_info_buf = NULL;
	u32 is_append = 1;
	int ret = 0;

	ret = liblinux_read_track_info_memory_config(&track_info_addr, &track_info_size);
	if (ret < 0) {
		pr_err("Failed to read track_info config\n");
		return;
	}

	track_info_buf = ioremap_wc(track_info_addr, track_info_size);
	if (track_info_buf == NULL) {
		pr_err("Failed to mmap track_info.\n");
		return;
	}

	ret = dump_hm_track_info(track_info_buf, track_info_size, dst_dir_str);
	if (ret < 0) {
		pr_err("Fallback to dump track_info bin.\n");
		ret = rdr_savebuf2fs(dst_dir_str, bin_file_name, track_info_buf,
				     (u32)track_info_size, is_append);
		if (ret != 0)
			pr_err("Failed to dump track_info bin ret: %d.\n", ret);
	}

	iounmap(track_info_buf);
}
#else /* !CONFIG_LIBLINUX_USE_TRACK_INFO */
void liblinux_dump_hm_track_info(const char *dst_dir_str, const char *bin_file_name) {};
#endif /* CONFIG_LIBLINUX_USE_TRACK_INFO */
