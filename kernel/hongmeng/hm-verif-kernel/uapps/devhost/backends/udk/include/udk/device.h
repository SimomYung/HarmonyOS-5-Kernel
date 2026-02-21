/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK device model
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 06 02:59:10 2019
 */
#ifndef __UDK_DEVICE_H__
#define __UDK_DEVICE_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <vfs_device.h>
#include <udk/lib/dlist.h>
#include <devhost/device.h>
#include <libdevhost/compatible.h>
#include <libhmsrv_io/type.h>
#include <libhmsync/raw_mutex.h>

struct device;
struct device_node;
struct inode;
struct device_info;
struct udk_dt_node;

/* For OF compatibility, resources and properties are defined in devhost_api */
typedef struct hm_property_s {
	char     *name;
	size_t    nbytes;
	uint32_t  flags;
	struct dlist_node list;
	char   bytes[];
} property_t;

#define IORESOURCE_TYPE_BITS    0x00001f00
#define IORESOURCE_IO           0x00000100
#define IORESOURCE_MEM          0x00000200
#define IORESOURCE_REG          0x00000300
#define IORESOURCE_DMA          0x00000800
#define IORESOURCE_BUS          0x00001000

struct udk_resource {
	unsigned long long addr;
	size_t        size;
	unsigned long flags;
	const char    *name;
	struct dlist_node list;
};

#define MAX_NUM_RESOURCE (1UL << 16)
#define MAX_NUM_PROPERTY (1UL << 16)

#define DEVID_INVALID		((int)-1)

#define DEVICE_NAME_LEN		128U
#define DEVICE_OPEN_INVALID_FLAG	((int)-1)

struct udk_device_base;

struct udk_iommu_info;

struct udk_device {
	char *name;
	struct udk_device *parent;
	struct udk_dt_node *dt_node;
	struct udk_device_base *base;
	void *priv;
	/* this function is used to release the owner of udk_device or udk_device itself */
	void (*release)(struct udk_device *udkdev);

	int parent_devid;

	devnum_t devt;
	struct raw_mutex opener_lock;
	int nr_opener;

	struct dlist_node reslist;
	struct dlist_node proplist;
	struct udk_iommu_info *iommu;
};

struct udk_device_attr {
	char name[DEVICE_NAME_LEN];
	unsigned int mode;
	unsigned int uid;
	unsigned int gid;
};

struct udk_chrdev {
	struct udk_device udkdev;
	struct devhost_fops *fops;
};

struct udk_driver {
	hm_driver_t drv;
	struct hm_driver_ops *drv_ops;
	void *ctx;
	/*
	 * following members only used in fusion mode
	 * fusion udk needs to record compat string and match it to
	 * find correct driver. It's done by devhost if udk as a backend.
	 */
	struct compatible_string compat;
	struct dlist_node node;
};

/*
 * Core hmdev APIs. Device lifecycle APIs work in pairs:
 * udk_device_init / udk_device_reset
 * udk_device_alloc / with udk_device_destroy
 */
int udk_device_init(struct udk_device *dev);
struct udk_device *udk_device_alloc(const char *);
void udk_device_free(struct udk_device *udkdev);
void udk_device_reset(struct udk_device *udkdev);
void udk_device_destroy(struct udk_device *udkdev);
int udk_device_add(struct udk_device *);
int udk_device_opener_inc(struct udk_device *udkdev);
void udk_device_opener_dec(struct udk_device *udkdev);
struct udk_device *udk_device_get_by_name(const char *name);
struct udk_device *udk_device_get_by_devt(devnum_t devt);
void udk_device_put(struct udk_device *udkdev);

void udk_device_capable_devfs(struct udk_device *udkdev, uint32_t devt,
			      int vfs_mode, struct devhost_fops *fops);
void udk_device_capable_devfs_full(struct udk_device *udkdev, uint32_t devt,
			      int vfs_mode, struct devhost_fops *fops, devnode_fn_t devnode_fn);
void udk_device_capable_devfs_ex(struct udk_device *udkdev,
				 struct devhost_fops_ex *fops_ex);
void udk_device_capable_blk(struct udk_device *udkdev,
			    struct devhost_block_ops *blkops);
void udk_device_capable_pm(struct udk_device *udkdev,
			   struct devhost_dev_pm_ops *pmops);
void udk_device_capable_net(struct udk_device *udkdev,
			    struct devhost_net_ops *netops);

void udk_device_set_autoload(struct udk_device *udkdev);
void udk_device_set_isolate(struct udk_device *udkdev, const char *isolate_group);
void udk_device_set_probeable(struct udk_device *udkdev, const char *compatible,
			      size_t size);

struct udk_device *udk_device_of(const struct device_info *info);
#define to_udk_device(...) udk_device_of(__VA_ARGS__)

struct device_info *udk_device_devinfo_of(struct udk_device *udkdev);

/* Core hmdev property/resource APIs */
int udk_device_add_property(struct udk_device *hmdev, const char *name,
			    size_t length, const void *value);

int udk_device_add_resource(struct udk_device *udkdev, unsigned long long addr,
			    size_t size, unsigned long flags);
int udk_device_add_resource_with_name(struct udk_device *udkdev,
				      unsigned long long addr, size_t size,
				      unsigned long flags, const char *name);

static inline int udk_device_add_iomem(struct udk_device *udkdev,
				       unsigned long long addr, size_t size,
				       unsigned long flags)
{
	return udk_device_add_resource(udkdev, addr, size, flags | IORESOURCE_REG);
}

static inline int udk_device_add_iomem_with_name(struct udk_device *udkdev,
						 unsigned long long addr, size_t size,
						 unsigned long flags, const char *name)
{
	return udk_device_add_resource_with_name(udkdev, addr, size,
						 flags | IORESOURCE_REG, name);
}

struct hm_property_s *udk_device_get_prop(const struct udk_device *dev,
					  const char *name);
static inline bool udk_device_prop_read_bool(struct udk_device *dev,
					     const char *name)
{
	return udk_device_get_prop(dev, name) ? true : false;
}

int udk_device_prop_read_u32(const struct udk_device *dev,
			     const char *name,
			     uint32_t *out);
const char *udk_device_prop_read_string(const struct udk_device *dev,
					const char *name);

#define udk_for_each_prop(udkdev, pp)	\
	dlist_for_each_entry(pp, &(udkdev)->proplist, property_t, list)

#define udk_for_each_resource(udkdev, res)	\
	dlist_for_each_entry(res, &(udkdev)->reslist, struct udk_resource, list)

int udk_alloc_chrdev_region(devnum_t *dev, unsigned baseminor, unsigned count,
			    const char *name);
int udk_register_chrdev_region(devnum_t from, unsigned count, const char *name);
void udk_unregister_chrdev_region(devnum_t from, unsigned count);
struct udk_device *udk_create_chrdev_duplicated(const char *name, const devnum_t devno, struct devhost_fops *fops);
void udk_chrdev_destroy_duplicated(struct udk_chrdev *udkdev);
void udk_chrdev_free_duplicated(struct udk_chrdev *chrdev);
void *udk_chrdev_ctx_to_filectx(const void *ctx);

int udk_device_get_irq(const struct udk_device *dev, unsigned int index);
struct udk_resource *udk_device_get_resource(const struct udk_device *dev,
					     unsigned int type, unsigned int num);

static inline size_t udk_res_size(const struct udk_resource *res)
{
	return res->size;
}
static inline unsigned long udk_res_type(const struct udk_resource *res)
{
	return res->flags & IORESOURCE_TYPE_BITS;
}

/* Obsolete */
static inline void *udk_get_drvdata(const struct udk_device *pdev)
{
	return pdev->priv;
}

/* Obsolete */
static inline void udk_set_drvdata(struct udk_device *pdev, void *data)
{
	pdev->priv = data;
}

void *udk_device_priv_get(struct udk_device *udkdev);
void udk_device_priv_set(struct udk_device *udkdev, void *data);

#endif /* __UDK_DEVICE_H__ */
