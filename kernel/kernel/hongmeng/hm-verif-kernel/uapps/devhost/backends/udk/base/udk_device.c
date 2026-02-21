/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK device model
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 06 02:23:10 2019
 */
#include <udk/device.h>
#include <devhost/device.h>
#include <internal/device.h>

#include <endian.h>
#include <udk/log.h>
#include <udk/module.h>
#include <libalgo/idr.h>
#include <internal/irq.h>
#include <hongmeng/errno.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhwsecurec/securec.h>
#include <libstrict/strict.h>

static DEFINE_IDR(g_udk_device_idr);

struct device_info *udk_device_devinfo_of(struct udk_device *udkdev)
{
	if (udk_device_sanity_check(udkdev) == E_HM_OK) {
		return __udk_device_devinfo_of(udkdev);
	}
	return NULL;
}

static inline int udk_device_devid_of(struct udk_device *udkdev)
{
	return __udk_device_devinfo_of(udkdev)->devid;
}

static inline bool udk_device_init_done(struct udk_device *udkdev)
{
	return (__udk_device_devinfo_of(udkdev)->p != NULL);
}

void udk_device_set_autoload(struct udk_device *udkdev)
{
	if (udk_device_sanity_check(udkdev) == E_HM_OK) {
		devhost_device_set_autoload(__udk_device_devinfo_of(udkdev));
	}
}

void udk_device_set_isolate(struct udk_device *udkdev, const char *isolate_group)
{
	if (udk_device_sanity_check(udkdev) == E_HM_OK) {
		devhost_device_set_isolate(__udk_device_devinfo_of(udkdev),
					   isolate_group);
	}
}

void udk_device_set_probeable(struct udk_device *udkdev, const char *compatible,
			      size_t size)
{
	if (udk_device_sanity_check(udkdev) == E_HM_OK) {
		(void)devhost_device_set_probeable(__udk_device_devinfo_of(udkdev),
						   compatible, size);
	}
}

struct udk_device *udk_device_of(const struct device_info *info)
{
	struct udk_device_base *base = NULL;
	struct udk_device *udkdev = NULL;

	if (info != NULL) {
		base = container_of(info, struct udk_device_base, info);
		if (base != NULL && base->udkdev != NULL &&
		    base->udkdev->base == base) {
			udkdev = base->udkdev;
		}
	}
	return udkdev;
}

int udk_device_opener_inc(struct udk_device *udkdev)
{
	if (udkdev == NULL) {
		return -EINVAL;
	}

	raw_mutex_lock(&udkdev->opener_lock);
	if (udkdev->nr_opener == DEVICE_OPEN_INVALID_FLAG) {
		udk_warn("unable to open udk_device, it`s destroying, name=%s\n", udkdev->name);
		raw_mutex_unlock(&udkdev->opener_lock);
		return -EINVAL;
	}
	udkdev->nr_opener++;
	raw_mutex_unlock(&udkdev->opener_lock);

	return 0;
}

void udk_device_opener_dec(struct udk_device *udkdev)
{
	if (udkdev == NULL) {
		return;
	}

	raw_mutex_lock(&udkdev->opener_lock);
	if (udkdev->nr_opener == 0) {
		raw_mutex_unlock(&udkdev->opener_lock);
		return;
	}
	udkdev->nr_opener--;
	raw_mutex_unlock(&udkdev->opener_lock);
}

void udk_device_capable_devfs_full(struct udk_device *udkdev, uint32_t devt,
				   int vfs_mode, struct devhost_fops *fops,
				   devnode_fn_t devnode_fn)
{
	if (udk_device_sanity_check(udkdev) == E_HM_OK) {
		devhost_device_capable_devfs(__udk_device_devinfo_of(udkdev),
					     devt, vfs_mode, devnode_fn, fops);
		/* Remember devt for some drivers (e.g., tty) */
		udkdev->devt = devt;
	}
}

void udk_device_capable_devfs(struct udk_device *udkdev, uint32_t devt,
			      int vfs_mode, struct devhost_fops *fops)
{
	udk_device_capable_devfs_full(udkdev, devt, vfs_mode, fops, NULL);
}

void udk_device_capable_devfs_ex(struct udk_device *udkdev,
				 struct devhost_fops_ex *fops_ex)
{
	if (udk_device_sanity_check(udkdev) == E_HM_OK) {
		devhost_device_capable_devfs_ex(__udk_device_devinfo_of(udkdev), fops_ex);
	}
}

void udk_device_capable_blk(struct udk_device *udkdev,
			    struct devhost_block_ops *blkops)
{
	if (udk_device_sanity_check(udkdev) == E_HM_OK) {
		devhost_device_capable_blk(__udk_device_devinfo_of(udkdev), blkops);
	}
}

void udk_device_capable_pm(struct udk_device *udkdev,
			   struct devhost_dev_pm_ops *pmops)
{
	if (udk_device_sanity_check(udkdev) == E_HM_OK) {
		devhost_device_capable_pm(__udk_device_devinfo_of(udkdev), pmops);
	}
}

void udk_device_capable_net(struct udk_device *udkdev,
			    struct devhost_net_ops *netops)
{
	if (udk_device_sanity_check(udkdev) == E_HM_OK) {
		devhost_device_capable_net(__udk_device_devinfo_of(udkdev), netops);
	}
}

static void udk_device_res_prop_free(struct udk_device *udkdev)
{
	struct udk_resource *res = NULL, *n = NULL;
	struct hm_property_s *pp = NULL, *npp = NULL;

	if (udkdev != NULL) {
		dlist_for_each_entry_safe(res, n, &udkdev->reslist,
					  struct udk_resource, list) {
			dlist_delete(&res->list);
			free(res);
		}
		dlist_for_each_entry_safe(pp, npp, &udkdev->proplist,
					  struct hm_property_s, list) {
			dlist_delete(&pp->list);
			free(pp);
		}
	}
}

static void udk_device_base_free(struct udk_device *udkdev)
{
	free(udkdev->base);
	udkdev->base = NULL;
}

static void udk_device_base_release(struct device_info *info)
{
	struct udk_device *udkdev = NULL;
	if (info == NULL) {
		return;
	}

	udkdev = udk_device_of(info);
	if (udkdev == NULL) {
		return;
	}
	udk_device_base_free(udkdev);
	udk_device_res_prop_free(udkdev);
	if (udkdev->release != NULL) {
		udkdev->release(udkdev);
	}
}

static int udk_device_base_alloc(struct udk_device *udkdev)
{
	struct udk_device_base *base = NULL;
	int ret = 0;

	base = (struct udk_device_base *)malloc(sizeof(*base));
	if (base == NULL) {
		udk_warn("Failed to allocate udk_device_base\n");
		ret = -ENOMEM;
	}

	if (ret == 0) {
		mem_zero_b(base, sizeof(*base));
		ret = devhost_device_init(&base->info);
		base->info.release = udk_device_base_release;
		if (ret != 0) {
			udk_warn("Failed to init devinfo in devhost, name=%s\n",
				 udkdev->name);
			free(base);
		} else {
			base->info.devname = udkdev->name;
			base->info.devid = DEVID_INVALID;

			base->udkdev = udkdev;
			udkdev->base = base;
		}
	}

	return ret;
}

int udk_device_init(struct udk_device *udkdev)
{
	int ret;

	/* Each udk_device/device_info should have a name */
	if (udkdev == NULL || udkdev->name == NULL) {
		return -EINVAL;
	}

	if (udkdev->base != NULL) {
		/* Repeated initialization */
		return -EINVAL;
	}

	ret = udk_device_base_alloc(udkdev);
	if (ret != 0) {
		return ret;
	}

	dlist_init(&udkdev->reslist);
	dlist_init(&udkdev->proplist);
	raw_mutex_init(&udkdev->opener_lock);
	udkdev->release = NULL;
	udkdev->nr_opener = 0;

	return 0;
}

static void udk_device_release(struct udk_device *udkdev)
{
	if (udkdev == NULL) {
		return;
	}

	/* Free name allocated by strndup */
	if (udkdev->name != NULL) {
		free(udkdev->name);
	}
	/* Free memory allocated by udk_device_alloc */
	free(udkdev);
}

/*
 * udk_device_alloc - Allocate a native device
 *
 * Return a udkdev on success.
 * Return NULL if the name is NULL or memory allocations fail.
 * Use udk_device_add to add the device to the device hierarchy
 * and notify devmgr.
 */
/* should allow linux parent */
struct udk_device *udk_device_alloc(const char *name)
{
	struct udk_device *udkdev = NULL;
	int ret;

	if (name == NULL) {
		return NULL;
	}

	udkdev = (struct udk_device *)malloc(sizeof(*udkdev));
	if (udkdev == NULL) {
		return NULL;
	}

	mem_zero_s(*udkdev);

	udkdev->name = strndup(name, DEVICE_NAME_LEN - 1);
	if (udkdev->name == NULL) {
		free(udkdev);
		return NULL;
	}

	ret = udk_device_init(udkdev);
	udkdev->release = udk_device_release;
	if (ret < 0) {
		free(udkdev->name);
		free(udkdev);
		return NULL;
	}

	return udkdev;
}

void *udk_device_priv_get(struct udk_device *udkdev)
{
	return (udkdev == NULL) ? NULL : udkdev->priv;
}

void udk_device_priv_set(struct udk_device *udkdev, void *data)
{
	if (udkdev != NULL) {
		udkdev->priv = data;
	}
}

struct hm_property_s *udk_device_get_prop(const struct udk_device *dev,
					  const char *name)
{
	struct hm_property_s *pp = NULL;

	if (dev == NULL || name == NULL) {
		return NULL;
	}

	udk_for_each_prop(dev, pp) {
		if (strcmp(name, pp->name) == 0) {
			return pp;
		}
	}

	return NULL;
}

int udk_device_prop_read_u32(const struct udk_device *dev,
			     const char *name,
			     uint32_t *out)
{
	struct hm_property_s *pp = NULL;

	if (out == NULL) {
		return -EINVAL;
	}
	pp = udk_device_get_prop(dev, name);
	if (pp == NULL) {
		return -EINVAL;
	}
	if (pp->nbytes < sizeof(uint32_t)) {
		return -EINVAL;
	}
	*out = be32toh(*((uint32_t *)(uintptr_t)(pp->bytes)));
	return 0;
}

const char *udk_device_prop_read_string(const struct udk_device *dev,
					const char *name)
{
	struct hm_property_s *pp = udk_device_get_prop(dev, name);
	if (pp == NULL) {
		return NULL;
	}
	return pp->bytes;
}

struct udk_dev_idr_search_args {
	const char *name;
	devnum_t devt;
	struct udk_device *ret;
};

static int udk_device_match_name_callback(unsigned int id, void *ptr, void *data)
{
	UNUSED(id);

	struct udk_device *dev = ptr;
	struct udk_dev_idr_search_args *arg = data;
	if (strcmp(dev->name, arg->name) == 0) {
		if (udk_device_opener_inc(dev) < 0) {
			return 0; /* destroying, ignore */
		} else {
			arg->ret = dev;
			return 1;
		}
	}
	return 0;
}

struct udk_device *udk_device_get_by_name(const char *name)
{
	struct udk_dev_idr_search_args args = {
		.name = name,
		.devt = 0U,
		.ret = NULL,
	};

	if (name == NULL) {
		return NULL;
	} else {
		(void)idr_for_each(&g_udk_device_idr, udk_device_match_name_callback, &args);
		return args.ret;
	}
}

static int udk_device_match_devt_callback(void *ptr, bool flag)
{
	UNUSED(flag);

	struct udk_device *dev = ptr;
	if (udk_device_opener_inc(dev) < 0) {
		return E_HM_INVAL; /* destroying, ignore */
	}
	return E_HM_OK;
}

struct udk_device *udk_device_get_by_devt(devnum_t devt)
{
	return idr_find_ex(&g_udk_device_idr, devt, false, udk_device_match_devt_callback);
}

void udk_device_put(struct udk_device *udkdev)
{
	if (udkdev != NULL) {
		udk_device_opener_dec(udkdev);
	}
}

static int udk_device_cache(struct udk_device *udkdev)
{
	int ret = idr_set(&g_udk_device_idr, udkdev, udkdev->devt);
	if (ret >= 0 || ret == E_HM_OBJEXIST) {
		ret = 0;
	} else {
		udk_warn("failed to cache udkdev devt=%u, err=%s\n",
			 udkdev->devt, hmstrerror(ret));
		ret = -hmerrno2posix(ret);
	}
	return ret;
}

static void udk_device_uncache(struct udk_device *udkdev)
{
	(void)idr_remove(&g_udk_device_idr, udkdev->devt);
}

int udk_device_add(struct udk_device *udkdev)
{
	struct device_info *info = NULL;
	int ret = udk_device_sanity_check(udkdev);
	if (ret != E_HM_OK) {
		return -EINVAL;
	}

	info = __udk_device_devinfo_of(udkdev);
	if (!udk_device_init_done(udkdev)) {
		udk_error("call udk_device_init FIRST\n");
		return -EINVAL;
	}

	/* For UDK, NO device with assigned devid exist. */
	if (udk_device_devid_of(udkdev) != DEVID_INVALID) {
		udk_error("set obsolete devinfo.devid\n");
		return -EINVAL;
	}

	/* Set parent */
	if (udkdev->parent != NULL) {
		info->parent = __udk_device_devinfo_of(udkdev->parent)->devid;
	} else {
		info->parent = 0;
	}

	/* Cache device in global idr */
	ret = udk_device_cache(udkdev);
	if (ret < 0) {
		udk_error("failed to cache udk device, name=%s, err=%s\n",
			  udkdev->name, hmstrerror(ret));
		return ret;
	}

	/* Register device at devmgr and retrieve the devid */
	ret = devhost_register_device(info);
	if (ret < 0) {
		udk_error("Failed to register device in devmgr, name=%s\n",
			 udkdev->name);
		udk_device_uncache(udkdev);
		return ret;
	}

	return 0;
}

void udk_device_free(struct udk_device *udkdev)
{
	if (udk_device_sanity_check(udkdev) == E_HM_OK) {
		devhost_device_destroy(__udk_device_devinfo_of(udkdev));
	}
}

/*
 * Deinitialize a device which has been initialized with udk_device_init
 * rather than udk_device_alloc.
 */
void udk_device_reset(struct udk_device *udkdev)
{
	if (udk_device_sanity_check(udkdev) == E_HM_OK) {
		/* OK when devid is DEVID_INVALID, which is -1 or 0xffffffff */
		(void)devhost_unregister_device(__udk_device_devinfo_of(udkdev));
		udk_device_free(udkdev);
	}
}

/*
 * If the name is manually set by drivers, this routine should not be called.
 */
void udk_device_destroy(struct udk_device *udkdev)
{
	raw_mutex_lock(&udkdev->opener_lock);
	if (udkdev->nr_opener > 0) {
		udk_warn("device has opener, continue to destroy, name=%s\n",
			 udkdev->name);
	}
	udkdev->nr_opener = DEVICE_OPEN_INVALID_FLAG;
	raw_mutex_unlock(&udkdev->opener_lock);
	udk_device_uncache(udkdev); /* uncache before udk_device_release in udk_device_reset */
	udk_device_reset(udkdev);
}

int udk_device_add_resource_with_name(struct udk_device *udkdev,
				      unsigned long long addr, size_t size,
				      unsigned long flags, const char *name)
{
	struct udk_resource *res = NULL;

	if (udkdev == NULL) {
		return -EINVAL;
	}
	/* name with empty string is invalid */
	if (name != NULL && name[0] == '\0') {
		return -EINVAL;
	}
	res = (struct udk_resource *)malloc(sizeof(*res));
	if (res == NULL) {
		return -ENOMEM;
	}
	res->addr = addr;
	res->size = size;
	res->flags = flags;
	res->name = name;
	dlist_insert_tail(&udkdev->reslist, &res->list);

	return 0;
}

int udk_device_add_resource(struct udk_device *udkdev, unsigned long long addr,
			    size_t size, unsigned long flags)
{
	return udk_device_add_resource_with_name(udkdev, addr, size, flags, NULL);
}

#define PROPERTY_NAME_MAXLEN 256
#define PROPERTY_MAXLEN	0x4000
int udk_device_add_property(struct udk_device *udkdev,
			    const char *name,
			    size_t length,
			    const void *value)
{
	property_t *pp = NULL;
	size_t pplen;
	int err;

	if (udkdev == NULL || name == NULL ||
	    strnlen(name, PROPERTY_NAME_MAXLEN) == PROPERTY_NAME_MAXLEN) {
		return -EINVAL;
	}

	if (value == NULL && length != 0) {
		return -EINVAL;
	}

	pplen = sizeof(*pp) + strlen(name) + 1 + length;
	if (pplen >= PROPERTY_MAXLEN) {
		return -EINVAL;
	}
	pp = (property_t *)malloc(pplen);
	if (pp == NULL) {
		return -ENOMEM;
	}
	mem_zero_b(pp, pplen);

	if (length != 0) {
		err = memcpy_s(pp->bytes, length, value, length);
		if (err != 0) {
			free(pp);
			return -EINVAL;
		}
	}

	pp->nbytes = length;
	pp->name = (char *)ptr_add(pp->bytes, length);
	err = strncpy_s(pp->name, (pplen - length) - sizeof(*pp),
			name, strlen(name));
	if (err != 0) {
		free(pp);
		return -EINVAL;
	}

	dlist_insert_tail(&udkdev->proplist, &pp->list);

	return 0;
}

struct udk_resource *udk_device_get_resource(const struct udk_device *dev,
					     unsigned int type, unsigned int num)
{
	struct udk_resource *res = NULL;

	if (dev == NULL) {
		return NULL;
	}

	dlist_for_each_entry(res, &dev->reslist,
			     struct udk_resource, list) {
		if (type == udk_res_type(res)) {
			if (num == 0) {
				return res;
			}
			num--;
		}
	}

	return NULL;
}

int udk_device_get_irq(const struct udk_device *dev, unsigned int index)
{
	if (dev == NULL) {
		return -EINVAL;
	}

	return udk_irq_of_uirq(dev->dt_node, index);
}
