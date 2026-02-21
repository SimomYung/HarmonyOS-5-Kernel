// SPDX-License-Identifier: GPL-2.0
#include <linux/syscalls.h>
#include <linux/blackbox.h>
#include <linux/blackbox_common.h>
#include <liblinux/pal.h>

static int create_new_dir(char *name)
{
	int ret = 0;

	ret = ksys_access(name, 0);
	if (ret != 0) {
		ret = ksys_mkdir(name, BBOX_DIR_LIMIT);
		if ((ret != 0) && (ret != -EEXIST))
			bbox_print_err("Create dir [%s] failed! ret: %d\n", name, ret);
		else
			ret = 0;
	}

	return ret;
}

int __override create_log_dir(const char *path)
{
	int ret = 0;
	char *cur_path = NULL;
	int index = 0;

	if (unlikely(!path || (*path != '/'))) {
		bbox_print_err("path is NULL or does not start with /!\n");
		return -EINVAL;
	}

	cur_path = kzalloc(PATH_MAX_LEN + 1, GFP_KERNEL);
	if (unlikely(!cur_path)) {
		bbox_print_err("kzalloc failed!\n");
		return -ENOMEM;
	}
	cur_path[index++] = *path++;
	while (*path != '\0') {
		if (*path == '/')
			ret = create_new_dir(cur_path);
		if (ret != 0)
			break;
		cur_path[index] = *path;
		path++;
		index++;
	}
	if (ret == 0)
		ret = create_new_dir(cur_path);

	kfree(cur_path);

	return ret;
}

static int get_fullpath(int fd, char *fullpath, int len)
{
	char *path = NULL;
	int ret;

	path = kzalloc(PATH_MAX_LEN , GFP_KERNEL);
	if (unlikely(!path)) {
		bbox_print_err("kzalloc failed!\n");
		return -ENOMEM;
	}

	ret = snprintf(path, PATH_MAX_LEN, "/proc/self/fd/%d", fd);
	if ((ret > 0) && (ret < PATH_MAX_LEN)) {
		ret = liblinux_pal_ksys_readlink(path, fullpath, len);
		if (ret <= 0 || ret >= len) {
			bbox_print_err("readlink %s failed:%d\n", path, ret);
			ret = -EINVAL;
		} else
			ret = 0;
	} else
		ret = -EINVAL;

	kfree(path);

	return ret;
}

int __override full_write_file(const char *pfile_path, char *buf,
		size_t buf_size, bool is_append)
{
	char *fullpath = NULL;
	int fd = -1;
	int rc = -1;
	int ret;

	if (unlikely(!pfile_path || !buf)) {
		bbox_print_err("pfile_path or buf is NULL!\n");
		return -EINVAL;
	}

	fd = liblinux_pal_ksys_open(pfile_path, O_CREAT | O_RDWR |
			(is_append ? O_APPEND : O_TRUNC), BBOX_FILE_LIMIT);
	if (fd < 0) {
		bbox_print_err("open %s failed! [%d]\n", pfile_path, fd);
		return -EBADF;
	}

	ret = liblinux_pal_ksys_write(fd, buf, buf_size);
	if (ret < 0) {
		fullpath = kzalloc(PATH_MAX_LEN , GFP_KERNEL);
		if (fullpath != NULL)
			rc = get_fullpath(fd, fullpath, PATH_MAX_LEN);
		bbox_print_err("write [%s] failed! [%d]\n", (rc == 0) ? fullpath : "", ret);
		kfree(fullpath);
	}

	(void)liblinux_pal_ksys_close(fd);

	return (ret < 0) ? ret : 0;
}

int __override file_exists(const char *name)
{
	return ksys_access(name, 0);
}
