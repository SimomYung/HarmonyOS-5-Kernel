// SPDX-License-Identifier: GPL-2.0-only

#include <linux/device.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/firmware.h>
#include <linux/shrinker.h>
#include <liblinux/pal.h>
#include <liblinux/sched.h>
#include <trace/hooks/liblinux.h>
#include <linux/fcntl.h>

#include "firmware.h"
#include "iofast.h"

#ifndef CONFIG_LIBLINUX_UDK_UFS
static int fw_read_file_contents(struct fw_priv *fw_priv, const char *path,
				 unsigned int size)
{
	unsigned int rsize;
	char *buf = NULL;
	int rc;

	if (size == 0)
		return -EINVAL;

	buf = vmalloc(size);
	if (!buf)
		return -ENOMEM;

	rc = liblinux_pal_fw_acquire_contents(path, buf, size, &rsize);
	if (rc)
		goto fail;

	if (rsize != size) {
		rc = -EIO;
		goto fail;
	}

	fw_priv->data = buf;
	fw_priv->size = size;
	return 0;
fail:
	vfree(buf);
	return rc;
}

static void
hook_fw_get_filesystem_firmware(void *__data, struct device *device,
				struct fw_priv *fw_priv, const char *suffix,
				int (*decompress)(struct device *dev,
						  struct fw_priv *fw_priv,
						  size_t in_size,
						  const void *in_buffer),
				const char * const *fw_path,
				size_t fw_path_size, int *ret)
{
	int i, len;
	int rc = -ENOENT;
	char *path;
	unsigned int rsize;

	if (decompress != NULL) {
		*ret = -EOPNOTSUPP;
		return;
	}
	path = vmalloc(PATH_MAX);
	if (!path) {
		*ret = -ENOMEM;
		return;
	}

	for (i = 0; i < fw_path_size; i++) {
		/* skip the unset customized path */
		if (!fw_path[i][0])
			continue;

		len = snprintf(path, PATH_MAX, "%s/%s%s",
			       fw_path[i], fw_priv->fw_name, suffix);
		if (len >= PATH_MAX) {
			rc = -ENAMETOOLONG;
			break;
		}

		rc = liblinux_pal_fw_acquire_contents(path, NULL, 0, &rsize);
		if (rc)
			continue;

		rc = fw_read_file_contents(fw_priv, path, rsize);
		if (rc)
			dev_dbg(device, "firmware, attempted to load %s, but failed\n", path);
		else
			break;
	}
	vfree(path);

	if (!rc) {
		fw_state_done(fw_priv);
	}

	*ret = rc;
}
#else
static bool __firmware_cache_is_allowed(const char *name);
static int fw_read_file_contents(struct fw_priv *fw_priv, int fd,
				 unsigned int size)
{
	char *buf = NULL;
	int rc;
	struct page *pages;
	bool is_cache = __firmware_cache_is_allowed(fw_priv->fw_name);

	if (size == 0)
		return -EINVAL;

	if (is_cache) {
		pages = liblinux_iofast_alloc_page(GFP_KERNEL | __GFP_ZERO, get_order(size), LIBLINUX_IOFAST_PAGE_FW_CACHE);
		if (!pages)
			return -ENOMEM;
		buf = page_to_virt(pages);
	} else {
		buf = vmalloc(size);
		if (!buf)
			return -ENOMEM;
	}

	rc = liblinux_vfs_read(fd, buf, size);
	if (rc < 0)
		goto fail;

	if (rc != size) {
		rc = -EIO;
		goto fail;
	}

	fw_priv->data = buf;
	fw_priv->size = size;
	return 0;
fail:
	if (is_cache) {
		liblinux_iofast_free_pages(pages, get_order(size), LIBLINUX_IOFAST_PAGE_FW_CACHE);
	} else {
		vfree(buf);
	}
	return rc;
}

static void
hook_fw_get_filesystem_firmware(void *__data, struct device *device,
				struct fw_priv *fw_priv, const char *suffix,
				int (*decompress)(struct device *dev,
						  struct fw_priv *fw_priv,
						  size_t in_size,
						  const void *in_buffer),
				const char * const *fw_path,
				size_t fw_path_size, int *ret)
{
	int i, len, fw_fd;
	int rc = -ENOENT;
	char *path;
	struct stat fstat;

	if (decompress != NULL) {
		*ret = -EOPNOTSUPP;
		return;
	}
	path = vmalloc(PATH_MAX);
	if (!path) {
		*ret = -ENOMEM;
		return;
	}

	for (i = 0; i < fw_path_size; i++) {
		int rc_close = 0;
		/* skip the unset customized path */
		if (!fw_path[i][0])
			continue;

		len = snprintf(path, PATH_MAX, "%s/%s%s",
			       fw_path[i], fw_priv->fw_name, suffix);
		if (len >= PATH_MAX) {
			rc = -ENAMETOOLONG;
			break;
		}

		fw_fd = liblinux_vfs_open(path, O_RDONLY, 0);
		if (fw_fd < 0) {
			dev_warn(device, "attempted to open fw %s, but failed %d\n", path, fw_fd);
			continue;
		}
		rc = liblinux_vfs_fstat(fw_fd, &fstat);
		if (rc < 0) {
			dev_warn(device, "firmware, attempted to fstat %s, but failed %d\n", path, rc);
			rc_close = liblinux_vfs_close(fw_fd);
			if (rc_close)
				dev_warn(device, "firmware, attempted to close %s, but failed %d\n", path, rc_close);
			continue;
		}
		rc = fw_read_file_contents(fw_priv, fw_fd, fstat.st_size);
		rc_close = liblinux_vfs_close(fw_fd);
		if (rc_close)
			dev_warn(device, "[%s:%d]firmware, attempted to close %s, but failed %d\n", __func__, __LINE__, path, rc_close);
		if (rc < 0)
			dev_warn(device, "firmware, attempted to load %s, but failed %d\n", path, rc);
		else
			break;
	}
	vfree(path);

	if (!rc)
		fw_state_done(fw_priv);

	*ret = rc;
}
#endif
INIT_VENDOR_HOOK(ldk_vh_fw_get_filesystem_firmware, hook_fw_get_filesystem_firmware);

static const char * const firmware_cache_whitelist[] = {
	"isp_fw.elf", // The whitelist sypports firmware that alloc mem upon startup
};

static bool __firmware_cache_is_allowed(const char *name)
{
	bool allow = false;

	for (int i = 0; i < ARRAY_SIZE(firmware_cache_whitelist); i++) {
		if (strcmp(firmware_cache_whitelist[i], name) == 0) {
			allow = true;
			break;
		}
	}

	return allow;
}

typedef void (*free_fw_priv_func_t)(struct fw_priv *fw_priv);
static struct firmware_cache_s {
	struct list_head info_list;
	struct mutex list_lock;
	bool enable_cache;
	free_fw_priv_func_t free_fw_priv_func;
	struct shrinker shrinker;
} firmware_cache = {0};

static void __firmware_cache_add(struct fw_priv *fw_priv)
{
	bool allow_cache = false;
	struct fw_priv *tmp = NULL;

	allow_cache = __firmware_cache_is_allowed(fw_priv->fw_name);
	if (allow_cache && (firmware_cache.enable_cache == true) &&
	    (fw_priv->data != NULL) && (fw_priv->size != 0)) {
		mutex_lock(&firmware_cache.list_lock);
		if (list_empty(&fw_priv->white_list)) {
			kref_get(&fw_priv->ref);
			list_add_tail(&fw_priv->white_list, &firmware_cache.info_list);
		}
		mutex_unlock(&firmware_cache.list_lock);
	}
}

static void
hook_request_firmware(void *__data, struct fw_priv *fw_priv,
		      free_fw_priv_func_t free_fw_priv_func)
{
	if (fw_priv == NULL)
	    return;

	if (!(fw_priv->opt_flags & (FW_OPT_NOCACHE | FW_OPT_PARTIAL))) {
		__firmware_cache_add(fw_priv);
		if (!firmware_cache.free_fw_priv_func)
			firmware_cache.free_fw_priv_func = free_fw_priv_func;
	}
}
INIT_VENDOR_HOOK(ldk_rvh_request_firmware, hook_request_firmware);

static unsigned long firmware_cache_shrinker_count(struct shrinker *shrinker,
						   struct shrink_control *sc)
{
	unsigned long size_sum = 0;
	struct fw_priv *fw_priv = NULL;

	mutex_lock(&firmware_cache.list_lock);
	list_for_each_entry(fw_priv, &firmware_cache.info_list, white_list) {
		size_sum += fw_priv->size;
	}
	mutex_unlock(&firmware_cache.list_lock);

	return size_sum ? (size_sum >> PAGE_SHIFT) : 0;
}

static unsigned long firmware_cache_shrinker_scan(struct shrinker *shrinker,
						  struct shrink_control *sc)
{
	unsigned long size_sum = 0;
	struct fw_priv *pos, *tmp;
	LIST_HEAD(fw_shrinker_list);

	mutex_lock(&firmware_cache.list_lock);
	list_for_each_entry_safe(pos, tmp, &firmware_cache.info_list, white_list) {
		list_del(&pos->white_list);
		size_sum += pos->size;
		list_add_tail(&pos->white_list, &fw_shrinker_list);
		if ((size_sum >> PAGE_SHIFT) >= sc->nr_to_scan)
			break;
	}
	mutex_unlock(&firmware_cache.list_lock);

	list_for_each_entry_safe(pos, tmp, &fw_shrinker_list, white_list) {
		list_del_init(&pos->white_list);
		if (firmware_cache.free_fw_priv_func)
			firmware_cache.free_fw_priv_func(pos);
	}

	return size_sum ? (size_sum >> PAGE_SHIFT) : SHRINK_STOP;
}

static void firmware_register_shrinker(void)
{
	firmware_cache.shrinker.scan_objects = firmware_cache_shrinker_scan;
	firmware_cache.shrinker.count_objects = firmware_cache_shrinker_count;
	firmware_cache.shrinker.batch = 0;
	firmware_cache.shrinker.seeks = DEFAULT_SEEKS;
	register_shrinker(&firmware_cache.shrinker);
}

static void firmware_unregister_shrinker(void)
{
	struct shrink_control sc = {0};

	unregister_shrinker(&firmware_cache.shrinker);
	sc.nr_to_scan = ~0UL;
	(void)firmware_cache_shrinker_scan(NULL, &sc);
}

static int __init firmware_cache_init(void)
{
	INIT_LIST_HEAD(&firmware_cache.info_list);
	mutex_init(&firmware_cache.list_lock);
	firmware_cache.enable_cache = true;
	firmware_cache.free_fw_priv_func = NULL;

	return 0;
}

static void __exit firmware_cache_exit(void)
{
	firmware_cache.enable_cache = false;
	firmware_cache.free_fw_priv_func = NULL;
}

postcore_initcall(firmware_cache_init);
module_exit(firmware_cache_exit);
