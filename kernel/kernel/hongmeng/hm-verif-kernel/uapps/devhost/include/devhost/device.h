/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Prototypes and type definitions for device APIs
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#ifndef __DEVHOST_API_DEVICE_H__
#define __DEVHOST_API_DEVICE_H__

#if defined(MODULE)
#include <linux/types.h>
#elif defined(UDK_MODULE) || defined(UDK_FRAMEWORK)
#include <udk/stdint.h>
#include <stddef.h>
#include <stdbool.h>
#else
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#endif

/* device APIs */
struct device_private;
struct devhost_pt_info;
struct device_info {
	struct device_private *p;
	const char *devname;
	int devid;
	int parent;
	/* this function is used to release the owner of device_info or device_info itself */
	void (*release)(struct device_info *devinfo);
};

int devhost_register_device(struct device_info *info);
int devhost_unregister_device(struct device_info *info);
int devhost_acquire_device(int devid, struct device_info **pinfo);
void devhost_devinfo_put(struct device_info *info);
struct device_info *devinfo_get(struct device_info *devinfo);
void devinfo_put(struct device_info *devinfo);

int devhost_device_init(struct device_info *devinfo);
void devhost_device_destroy(struct device_info *devinfo);
void devhost_device_set_autoload(struct device_info *devinfo);
void devhost_device_set_isolate(struct device_info *devinfo, const char *isolate_group);
int devhost_device_set_probeable(struct device_info *devinfo,
				const char *compatible, size_t size);
void devhost_device_set_anonymous(struct device_info *devinfo);
int devhost_device_is_anonymous(const struct device_info *devinfo);

/* same as /libhmsrv_io/type.h HMSRV_IO_CTX_DEVFOPS_NO_*_FLAGS */
#define DH_IO_CTX_DEVFOPS_NO_FLUSH_FLAG		0x00000001
#define DH_IO_CTX_DEV_ZERO_FLAG			0x00000004
unsigned int devhost_device_get_fops_flags(struct device_info *devinfo, void *filp);

/* devfs capable */
struct devhost_fops {
	int (*open)(struct device_info *dev, void **ctx, unsigned int flags);
	int (*close)(void *ctx);
	int (*read)(void *ctx, void *buf, unsigned long count,
		    unsigned long long off);
	int (*write)(void *ctx, const void *buf, unsigned long count,
		     unsigned long long off);
	int (*ioctl)(void *ctx, unsigned int cmd, unsigned long arg);
	int (*fcntl)(void *ctx, int cmd, unsigned long arg);
	int (*mmap)(void *ctx,
		    unsigned long addr, unsigned long length, unsigned long prot,
		    unsigned long offset);
	int (*poll)(void *ctx, unsigned long *event_out,
		    struct devhost_pt_info *pt_info, uint64_t key);
	long long (*llseek)(void *ctx, long long offset, int whence);
	void **owner;
	int (*read_ex)(void *ctx, void *buf, unsigned long count,
		       unsigned long long *off, unsigned int is_pread);
	int (*write_ex)(void *ctx, const void *buf, unsigned long count,
			unsigned long long *off, unsigned int is_pwrite);
};

int devhost_device_open(struct device_info *dev, void **ctx, unsigned int flags);

struct devhost_vma_info;
/*
 * devfs capable extensible
 */
struct devhost_fops_ex {
	size_t size;      /* size of the structure */
	int (*open)(struct device_info *info, void **ctx, unsigned int flags, void *kobj_ctx);
	int (*poll)(void *ctx, void *poll_info, unsigned int *revents);
	int (*compat_ioctl)(void *ctx, unsigned int cmd, unsigned long arg);
	int (*mmap_ex)(void *ctx, struct devhost_vma_info *vma_info);
	int (*write_iter)(void *ctx, unsigned long long *pos, void *iov,
			  int nr_segs, size_t count, unsigned int flags);
	int (*read_iter)(void *ctx, unsigned long long *pos, void *iov,
			 int nr_segs, size_t count, unsigned int flags);
	int (*flush)(void *ctx);
	long (*get_unmapped_area)(void *ctx, unsigned long addr, unsigned long len,
				  unsigned long offset, unsigned long flags);
	unsigned int (*get_fops_flags)(void *ctx); /* get_fops_flags needs to be added based on the site requirements */
};

struct devnode_req {
	unsigned int mode;
	unsigned int uid;
	unsigned int gid;
};
/*
 * devnode_fn_t gets the information needed to create
 * the device node, currently only the node name is
 * supported, and subsequent extensions support
 * mode/uid/gid information
 */
typedef int (*devnode_fn_t)(struct device_info *devinfo,
			    char *devnode_name, unsigned int size,
			    struct devnode_req *req);
void devhost_device_capable_devfs(struct device_info *devinfo,
				  uint32_t devt, int vfs_mode,
				  devnode_fn_t devnode_fn,
				  struct devhost_fops *fops);

void devhost_device_capable_devfs_ex(struct device_info *devinfo,
				     struct devhost_fops_ex *ex_fops);

struct bd_io;

struct bd_context {
	void *private;
	unsigned int vec_cnt;
	unsigned int direct_map_paddr;
	unsigned int io_priority;
};

/* block capable */
typedef int (*devhost_end_bio_fn_t)(void *private, int bi_error, unsigned int io_idx);
struct devhost_block_ops {
	void* (*bio_alloc)(void *filep,
			   const struct bd_io *bd_io,
			   void *private, unsigned int bd_io_idx);
	void (*bio_free)(void *bio);
	int (*submit_bio)(void *bio, unsigned int flags);
	int (*bio_add_data)(void *bio, uintptr_t vaddr, unsigned int bv_len);
	int (*dev_check_feature)(void *filep, int idx, uint64_t feature,
				 uint64_t *supported_features);
	int (*direct_access)(void *filep,
			     unsigned long offset, unsigned long size,
			     unsigned long long *paddr, unsigned long *ret_size);
	int (*bio_add_page_by_pfn)(void *bio, unsigned long long pfn,
				   unsigned int len, unsigned int offset);
	void (*bio_set_ctx)(void *bio, devhost_end_bio_fn_t devhost_end_bio_fn);
	void (*bio_polling)(void *filep, int rw);
};
/* requirement:
 *   - `flags == !DEVICE_CAN_PROBE`
 *   - `devt != 0`
 *   - vfs_mode == MODE_BLK
 */
void devhost_device_capable_blk(struct device_info *devinfo,
				struct devhost_block_ops *blkops);

void devhost_device_capable_directmap_shm(struct device_info *devinfo);

int devhost_block_queue_end_io(void *private, int bi_error, unsigned int bd_io_idx);
int devhost_block_end_io(void *private, int bi_error, unsigned int rq_idx);

/* pm capable */
struct devhost_dev_pm_ops {
	int (*shutdown)(struct device_info *info);
	int (*pre_suspend)(struct device_info *info);
	int (*suspend)(struct device_info *info);
	int (*post_suspend)(struct device_info *info);
	int (*noirq_suspend)(struct device_info *info);
	int (*noirq_resume)(struct device_info *info);
	int (*pre_resume)(struct device_info *info);
	int (*resume)(struct device_info *info);
	int (*post_resume)(struct device_info *info);
};

void devhost_device_capable_pm(struct device_info *devinfo,
			       struct devhost_dev_pm_ops *pmops);

/* net capable */
struct netdev_status;
struct devhost_net_ops {
	int (*net_tx)(unsigned long flags, unsigned int payload_len, unsigned int size,
		      unsigned int offset, struct device_info *info);
	int (*show_status)(struct device_info *info, struct netdev_status *p);
	int (*driver_ioctl)(struct device_info *info, int request, unsigned long args);
};

/* requirement: `flags == !DEVICE_CAN_PROBE` */
void devhost_device_capable_net(struct device_info *devinfo,
				struct devhost_net_ops *netops);

/* posix-clock capable */
struct timex;
struct timespec;
struct devhost_posix_clock_ops {
	int (*clock_adjtime)(unsigned long long file_id, struct timex *tx);
	int (*clock_gettime)(unsigned long long file_id, struct timespec *tp);
	int (*clock_getres) (unsigned long long file_id, struct timespec *tp);
	int (*clock_settime)(unsigned long long file_id, const struct timespec *tp);
};

/* requirement:
 *  - flags == !DEVICE_CAN_PROBE
 *  - vfs_mode == MODE_CHR
 *  - devt != 0
 */
void devhost_device_capable_pclk(struct device_info *devinfo,
				 struct devhost_posix_clock_ops *clkops);

/* register rtc ops from rtc.ko */
typedef int (*set_power_on_alarm_t)(long, bool);
int devhost_register_rtc_ops(set_power_on_alarm_t set_power_on_alarm);

struct driver_private;
typedef struct hm_driver {
	struct driver_private *p;
	char *libname;
	/* this function is used to release the owner of hm_driver or hm_driver itself */
	void (*release)(struct hm_driver *drv);
} hm_driver_t;

/* driver APIs */
struct device_create_args {
	int parent_devid;
	const void *resource;
	size_t resource_size;
};
typedef struct hm_driver_ops {
	int (*bind)(void *ctx, struct device_info *dev);
	int (*create)(void *ctx, const char *name,
		      const struct device_create_args *args,
		      struct device_info **info_out);
	void **owner;
} hm_driver_ops_t;

int devhost_driver_init(hm_driver_t *drv, const struct hm_driver_ops *ops, void *ctx);
void devhost_driver_destroy(hm_driver_t *drv);
int devhost_driver_register(hm_driver_t *drv);
int devhost_driver_add_compat(hm_driver_t *drv, const char *compat, size_t compat_len);

int devhost_register_driver(const char *libname, const struct hm_driver_ops *ops, void *ctx);
void devhost_drv_put(struct hm_driver *drv);

/* this function will increase drv`s refcnt */
int devhost_acquire_driver(const char *compat, hm_driver_t **pdrv);

#endif /* __DEVHOST_API_DEVICE_H__ */
