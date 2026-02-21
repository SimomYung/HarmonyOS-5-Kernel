/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: This is the implementation of udk char device related interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 15 15:44:53 2019
 */
#include <udk/mm.h>
#include <udk/log.h>
#include <udk/char.h>
#include <udk/iolib.h>
#include <udk/device.h>
#include <udk/module.h>

#include <hmkernel/errno.h>
#include <hongmeng/errno.h>
#include <devhost/memory.h>
#include <devhost/backend.h>
#include <libdevhost/devhost.h>
#include <libhwsecurec/securec.h>
#include <libsysif/devmgr/api.h>
#include <libhmsrv_sys/hm_select_common.h>

#include <internal/init.h>
#include <internal/poll.h>

#define MINOR_START	0U

static void native_char_context_release(struct raw_refcnt *refcnt);

int udk_alloc_chrdev_region(devnum_t *dev, unsigned baseminor, unsigned count,
			    const char *name)
{
	int ret;
	unsigned int major = 0;

	if (dev == NULL) {
		return -EINVAL;
	}

	ret = libdh_register_chrdev_region(0U, baseminor, (int)count, name, &major);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}
	*dev = MKDEV(major, baseminor);
	return 0;
}

int udk_register_chrdev_region(devnum_t from, unsigned count, const char *name)
{
	devnum_t cur = from;
	devnum_t end = from + count;

	while (cur != end) {
		int ret;
		devnum_t next = MKDEV(MAJOR(cur) + 1, MINOR_START);
		if (next > end) {
			next = end;
		}
		ret = libdh_register_chrdev_region(MAJOR(cur), MINOR(cur),
						   (int)(next - cur), name, NULL);
		if (ret < 0) {
			udk_unregister_chrdev_region(from, cur - from);
			return -hmerrno2posix(ret);
		}
		cur = next;
	}
	return 0;
}

void udk_unregister_chrdev_region(devnum_t from, unsigned count)
{
	devnum_t cur = from;
	devnum_t end = from + count;

	while (cur != end) {
		devnum_t next = MKDEV(MAJOR(cur) + 1, MINOR_START);
		if (next > end) {
			next = end;
		}
		libdh_unregister_chrdev_region(MAJOR(cur), MINOR(cur),
						     (int)(next - cur));
		cur = next;
	}
}

struct __file_ctx {
	void *ctx;
	struct udk_chrdev *chrdev;
};

void *udk_chrdev_ctx_to_filectx(const void *ctx)
{
	if (ctx == NULL) {
		return NULL;
	}

	return container_of(ctx, struct __file_ctx, ctx);
}

/* Following interfaces will be deprecated */
static struct udk_chrdev *__devinfo2chrdev(const struct device_info *info)
{
	struct udk_device *udkdev = udk_device_of(info);

	if (udkdev != NULL) {
		return container_of(udkdev, struct udk_chrdev, udkdev);
	}
	return NULL;
}

static inline struct udk_module *__chrdev_owner_of(const struct udk_chrdev *udk_chrdev)
{
	if (udk_chrdev->fops->owner == NULL) {
		return NULL;
	}
	return (struct udk_module *)(*udk_chrdev->fops->owner);
}

static int __native_chrdev_open(struct device_info *info, void **ctx, unsigned int flags)
{
	struct udk_chrdev *udk_chrdev = __devinfo2chrdev(info);
	struct __file_ctx *__file_ctx = NULL;
	int ret;

	if (udk_chrdev == NULL || udk_chrdev->fops == NULL || udk_chrdev->fops->open == NULL) {
		return -ENOSYS;
	}

	/* __file_ctx will free in __native_chrdev_close */
	__file_ctx = (struct __file_ctx *)malloc(sizeof(struct __file_ctx));
	if (__file_ctx == NULL) {
		return -ENOMEM;
	}

	ret = udk_module_open(__chrdev_owner_of(udk_chrdev));
	if (ret != 0) {
		free(__file_ctx);
		return ret;
	}
	ret = udk_device_opener_inc(&udk_chrdev->udkdev);
	if (ret != 0) {
		udk_module_close(__chrdev_owner_of(udk_chrdev));
		free(__file_ctx);
		return ret;
	}
	ret = udk_chrdev->fops->open(info, &__file_ctx->ctx, flags);
	if (ret != 0) {
		udk_device_opener_dec(&udk_chrdev->udkdev);
		udk_module_close(__chrdev_owner_of(udk_chrdev));
		free(__file_ctx);
		return ret;
	}
	/*
	 * udk_chrdev include devinfo, other ops can guarantee devinfo is valid,
	 * so __file_ctx->chrdev is valid in fops.
	 */
	__file_ctx->chrdev = udk_chrdev;
	*ctx = __file_ctx;

	return 0;
}

static int __native_chrdev_close(void *ctx)
{
	struct __file_ctx *__file_ctx = (struct __file_ctx *)ctx;
	struct udk_chrdev *udk_chrdev = NULL;
	int ret;

	if (__file_ctx == NULL) {
		return -EINVAL;
	}
	udk_chrdev = __file_ctx->chrdev;
	if (udk_chrdev == NULL || udk_chrdev->fops == NULL || udk_chrdev->fops->close == NULL) {
		return -ENOSYS;
	}

	ret = udk_chrdev->fops->close(__file_ctx->ctx);
	if (ret != 0) {
		return ret;
	}
	udk_device_opener_dec(&udk_chrdev->udkdev);
	udk_module_close(__chrdev_owner_of(udk_chrdev));
	free(ctx);

	return ret;
}

static int __native_chrdev_read(void *ctx, void *buf, unsigned long count,
			      unsigned long long off)
{
	struct __file_ctx *__file_ctx = (struct __file_ctx *)ctx;
	struct udk_chrdev *udk_chrdev = NULL;

	if (__file_ctx == NULL) {
		return -EINVAL;
	}
	udk_chrdev = __file_ctx->chrdev;
	if (udk_chrdev == NULL || udk_chrdev->fops == NULL || udk_chrdev->fops->read == NULL) {
		return -ENOSYS;
	}

	return udk_chrdev->fops->read(__file_ctx->ctx, buf, count, off);
}

static int __native_chrdev_write(void *ctx, const void *buf, unsigned long count,
			       unsigned long long off)
{
	struct __file_ctx *__file_ctx = (struct __file_ctx *)ctx;
	struct udk_chrdev *udk_chrdev = NULL;

	if (__file_ctx == NULL) {
		return -EINVAL;
	}
	udk_chrdev = __file_ctx->chrdev;
	if (udk_chrdev == NULL || udk_chrdev->fops == NULL || udk_chrdev->fops->write == NULL) {
		return -ENOSYS;
	}

	return udk_chrdev->fops->write(__file_ctx->ctx, buf, count, off);
}

static int __native_chrdev_ioctl(void *ctx, unsigned int cmd, unsigned long arg)
{
	struct __file_ctx *__file_ctx = (struct __file_ctx *)ctx;
	struct udk_chrdev *udk_chrdev = NULL;

	if (__file_ctx == NULL) {
		return -EINVAL;
	}
	udk_chrdev = __file_ctx->chrdev;
	if (udk_chrdev == NULL || udk_chrdev->fops == NULL || udk_chrdev->fops->ioctl == NULL) {
		return -ENOSYS;
	}

	return udk_chrdev->fops->ioctl(__file_ctx->ctx, cmd, arg);
}

static int __native_chrdev_mmap(void *ctx, unsigned long addr, unsigned long length,
			      unsigned long prot, unsigned long offset)
{
	struct __file_ctx *__file_ctx = (struct __file_ctx *)ctx;
	struct udk_chrdev *udk_chrdev = NULL;

	if (__file_ctx == NULL) {
		return -EINVAL;
	}
	udk_chrdev = __file_ctx->chrdev;
	if (udk_chrdev == NULL || udk_chrdev->fops == NULL || udk_chrdev->fops->mmap == NULL) {
		return -ENOSYS;
	}

	return udk_chrdev->fops->mmap(__file_ctx->ctx, addr, length, prot, offset);
}

static int __native_chrdev_poll(void *ctx, unsigned long *event_out,
			      struct devhost_pt_info *pt_info, uint64_t key)
{
	struct __file_ctx *__file_ctx = (struct __file_ctx *)ctx;
	struct udk_chrdev *udk_chrdev = NULL;

	if (__file_ctx == NULL) {
		return -EINVAL;
	}
	udk_chrdev = __file_ctx->chrdev;
	if (udk_chrdev == NULL || udk_chrdev->fops == NULL || udk_chrdev->fops->poll == NULL) {
		return -ENOSYS;
	}

	return udk_chrdev->fops->poll(__file_ctx->ctx, event_out, pt_info, key);
}

static long long __native_chrdev_llseek(void *ctx, long long offset, int whence)
{
	struct __file_ctx *__file_ctx = (struct __file_ctx *)ctx;
	struct udk_chrdev *udk_chrdev = NULL;

	if (__file_ctx == NULL) {
		return -EINVAL;
	}
	udk_chrdev = __file_ctx->chrdev;
	if (udk_chrdev == NULL || udk_chrdev->fops == NULL || udk_chrdev->fops->llseek == NULL) {
		return -ENOSYS;
	}

	return udk_chrdev->fops->llseek(__file_ctx->ctx, offset, whence);
}

static struct devhost_fops udk_chrdev_ops __read_mostly = {
	.open   = __native_chrdev_open,
	.close  = __native_chrdev_close,
	.ioctl  = __native_chrdev_ioctl,
	.read   = __native_chrdev_read,
	.write  = __native_chrdev_write,
	.mmap   = __native_chrdev_mmap,
	.poll   = __native_chrdev_poll,
	.llseek = __native_chrdev_llseek,
	.read_ex = NULL,
	.write_ex = NULL,
	.owner	= NULL,
};

static void udk_chrdev_release(struct udk_device *udkdev)
{
	struct udk_chrdev *chrdev = NULL;

	chrdev = container_of(udkdev, struct udk_chrdev, udkdev);
	if (chrdev == NULL) {
		return;
	}
	/* Free name allocated by strndup */
	if (udkdev->name != NULL) {
		free(udkdev->name);
		udkdev->name = NULL;
	}
	/* Free memory allocated for chrdev */
	free(chrdev);
}

static struct udk_chrdev *udk_chrdev_alloc(const char *name)
{
	struct udk_chrdev *chrdev = NULL;
	struct udk_device *udkdev = NULL;
	int ret;

	chrdev = (struct udk_chrdev *)malloc(sizeof(struct udk_chrdev));
	if (chrdev == NULL) {
		return NULL;
	}
	mem_zero_s(*chrdev);
	udkdev = &chrdev->udkdev;

	udkdev->name = strndup(name, DEVICE_NAME_LEN - 1);
	if (udkdev->name == NULL) {
		free(chrdev);
		return NULL;
	}

	ret = udk_device_init(udkdev);
	if (ret < 0) {
		free(udkdev->name);
		free(chrdev);
		return NULL;
	}
	udkdev->release = udk_chrdev_release;

	return chrdev;
}

static int udk_chrdev_add(struct udk_chrdev *chrdev,
			  const devnum_t devno, struct devhost_fops *fops)
{
	struct udk_device *udkdev = &chrdev->udkdev;

	udk_device_capable_devfs(udkdev, devno, (int)MODE_CHR, &udk_chrdev_ops);
	chrdev->fops = fops;
	return udk_device_add(udkdev);
}

/* for compatibility with driver api, we will rename later */
struct udk_device *udk_create_chrdev_duplicated(const char *name, const devnum_t devno,
						struct devhost_fops *fops)
{
	struct udk_chrdev *chrdev = NULL;
	int ret;

	if (name == NULL) {
		return NULL;
	}
	chrdev = udk_chrdev_alloc(name);
	if (chrdev == NULL) {
		return NULL;
	}
	ret = udk_chrdev_add(chrdev, devno, fops);
	if (ret != 0) {
		udk_chrdev_free_duplicated(chrdev);
		return NULL;
	}

	return &chrdev->udkdev;
}

void udk_chrdev_free_duplicated(struct udk_chrdev *chrdev)
{
	if (chrdev == NULL) {
		return;
	}

	udk_device_free(&chrdev->udkdev);
}

void udk_chrdev_destroy_duplicated(struct udk_chrdev *chrdev)
{
	if (chrdev == NULL) {
		return;
	}

	udk_device_destroy(&chrdev->udkdev);
}
/* Above interfaces will be deprecated */

static struct udk_char_device *devinfo2chrdev(const struct device_info *info)
{
	struct udk_device *udkdev = udk_device_of(info);

	if (udkdev != NULL) {
		return container_of(udkdev, struct udk_char_device, udkdev);
	}
	return NULL;
}

static inline struct udk_module *chrdev_owner_of(const struct udk_char_device *udk_chrdev)
{
	if (udk_chrdev->ops->owner == NULL) {
		return NULL;
	}
	return (struct udk_module *)(*udk_chrdev->ops->owner);
}

static inline bool is_invalid_ctx(struct udk_char_context *ctx)
{
	return ((ctx == NULL) ||
		(ctx->chrdev == NULL) ||
		(ctx->chrdev->ops == NULL));
}

static int native_chrdev_open(struct device_info *info, void **ctx, unsigned int flags)
{
	struct udk_char_device *udk_chrdev = devinfo2chrdev(info);
	struct udk_char_context *fctx = NULL;
	struct udk_module *owner = NULL;
	int err;

	if (udk_chrdev == NULL || udk_chrdev->ops == NULL || udk_chrdev->ops->open == NULL) {
		return -ENOSYS;
	}
	/* udk_char_context will free in native_chrdev_close */
	fctx = (struct udk_char_context *)udk_malloc(sizeof(struct udk_char_context));
	if (fctx == NULL) {
		return -ENOMEM;
	}
	mem_zero_s(*fctx);
	/* owner is allowed to be NULL */
	owner = chrdev_owner_of(udk_chrdev);
	err = udk_module_open(owner);
	if (err != 0) {
		udk_free(fctx);
		return err;
	}
	err = udk_device_opener_inc(&udk_chrdev->udkdev);
	if (err != 0) {
		udk_module_close(owner);
		udk_free(fctx);
		return err;
	}
	/*
	 * udk_chrdev include devinfo, other ops can guarantee devinfo is valid,
	 * so udk_char_context->chrdev is valid in fops.
	 */
	fctx->chrdev = udk_chrdev;
	fctx->io_flags = flags;
	err = udk_chrdev->ops->open(fctx);
	if (err != 0) {
		udk_device_opener_dec(&udk_chrdev->udkdev);
		udk_module_close(owner);
		udk_free(fctx);
		return err;
	}
	*ctx = fctx;
	raw_refcnt_init(&fctx->refcnt, 1, native_char_context_release);

	return 0;
}

static int native_chrdev_close(void *ctx)
{
	struct udk_char_context *fctx = (struct udk_char_context *)ctx;
	(void)raw_refcnt_put(&fctx->refcnt);

	return 0;
}

static int __char_context_release(struct udk_char_context *fctx)
{
	struct udk_char_device *udk_chrdev = NULL;
	int err = 0;

	if (unlikely(is_invalid_ctx(fctx))) {
		return -EINVAL;
	}
	udk_chrdev = fctx->chrdev;
	if (udk_chrdev->ops->close != NULL) {
		err = udk_chrdev->ops->close(fctx);
		if (err != 0) {
			goto out;
		}
	}

out:
	fctx->ctx = NULL;
	udk_device_opener_dec(&udk_chrdev->udkdev);
	udk_module_close(chrdev_owner_of(udk_chrdev));
	udk_free(fctx);

	return err;
}

static void native_char_context_release(struct raw_refcnt *refcnt)
{
	int err;
	struct udk_char_context *fctx = container_of(refcnt, struct udk_char_context, refcnt);

	err = __char_context_release(fctx);
	if (err < 0) {
		udk_error("failed to release contxt, ret=%s\n", strerror(-err));
	}
	return;
}

static int native_chrdev_read_ex(void *ctx, void *buf, unsigned long count,
				 unsigned long long *off, unsigned int is_pread)
{
	struct udk_char_context *fctx = (struct udk_char_context *)ctx;
	struct udk_char_device *udk_chrdev = NULL;
	unsigned long long __off = *off;
	int err;

	if (unlikely(is_invalid_ctx(fctx))) {
		return -EINVAL;
	}
	udk_chrdev = fctx->chrdev;
	if (udk_chrdev->ops->read == NULL) {
		return -ENOSYS;
	}
	err = udk_chrdev->ops->read(fctx, buf, count, &__off);
	/* pread not update offset */
	if ((err >= 0) && (is_pread == 0)) {
		*off = __off;
	}

	return err;
}

static int native_chrdev_write_ex(void *ctx, const void *buf, unsigned long count,
				  unsigned long long *off, unsigned int is_pwrite)
{
	struct udk_char_context *fctx = (struct udk_char_context *)ctx;
	struct udk_char_device *udk_chrdev = NULL;
	unsigned long long __off = *off;
	int err;

	if (unlikely(is_invalid_ctx(fctx))) {
		return -EINVAL;
	}
	udk_chrdev = fctx->chrdev;
	if (udk_chrdev->ops->write == NULL) {
		return -ENOSYS;
	}
	err = udk_chrdev->ops->write(fctx, buf, count, &__off);
	/* pread not update offset */
	if ((err >= 0) && (is_pwrite == 0)) {
		*off = __off;
	}

	return err;
}

/* read/write do not update offset, it equals to pread/pwrite */
static int native_chrdev_read(void *ctx, void *buf, unsigned long count,
			      unsigned long long off)
{
	return native_chrdev_read_ex(ctx, buf, count, &off, 1);
}

static int native_chrdev_write(void *ctx, const void *buf, unsigned long count,
			       unsigned long long off)
{
	return native_chrdev_write_ex(ctx, buf, count, &off, 1);
}

static int native_chrdev_ioctl(void *ctx, unsigned int cmd, unsigned long arg)
{
	struct udk_char_context *fctx = (struct udk_char_context *)ctx;
	struct udk_char_device *udk_chrdev = NULL;

	if (unlikely(is_invalid_ctx(fctx))) {
		return -EINVAL;
	}
	udk_chrdev = fctx->chrdev;
	if (udk_chrdev->ops->ioctl != NULL) {
		return udk_chrdev->ops->ioctl(fctx, cmd, arg);
	} else if (udk_chrdev->ops->compat_ioctl != NULL) {
		return udk_chrdev->ops->compat_ioctl(fctx, cmd, arg);
	} else {
		return -ENOSYS;
	}
}

static int native_chrdev_compat_ioctl(void *ctx, unsigned int cmd, unsigned long arg)
{
	struct udk_char_context *fctx = (struct udk_char_context *)ctx;
	struct udk_char_device *udk_chrdev = NULL;

	if (unlikely(is_invalid_ctx(fctx))) {
		return -EINVAL;
	}
	udk_chrdev = fctx->chrdev;
	if (udk_chrdev->ops->compat_ioctl == NULL) {
		return -ENOSYS;
	}

	return udk_chrdev->ops->compat_ioctl(fctx, cmd, arg);
}

static int native_chrdev_fcntl(void *ctx, int cmd, unsigned long arg)
{
	struct udk_char_context *fctx = (struct udk_char_context *)ctx;
	int ret = 0;

	if (unlikely(is_invalid_ctx(fctx))) {
		return -EINVAL;
	}

	switch (cmd) {
	case F_GETFL:
		ret = (int)(fctx->io_flags);
		break;
	case F_SETFL:
		if (arg & O_NONBLOCK) {
			fctx->io_flags |= O_NONBLOCK;
		} else {
			fctx->io_flags &= ~O_NONBLOCK;
		}
		break;
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}

static int native_chrdev_mmap_ex(void *ctx, struct devhost_vma_info *vma_info)
{
	struct udk_char_context *fctx = (struct udk_char_context *)ctx;
	struct udk_char_device *udk_chrdev = NULL;
	struct udk_vma *vma = NULL;
	int ret;

	vma_info->priv = NULL;
	if (unlikely(is_invalid_ctx(fctx))) {
		return -EINVAL;
	}
	udk_chrdev = fctx->chrdev;
	if (udk_chrdev->ops->mmap == NULL) {
		return -ENOSYS;
	}
	vma = udk_malloc(sizeof(struct udk_vma));
	if (vma == NULL) {
		return -ENOMEM;
	}
	vma->len = vma_info->len;
	vma->start = vma_info->start;
	vma->prot = vma_info->prot;
	vma->offset = vma_info->offset;
	vma->flags = vma_info->flags;
	vma->vma_id = vma_info->vma_id;
	vma->fctx = ctx;
	vma->ops = NULL;
	ret = udk_chrdev->ops->mmap(fctx, vma);
	if (ret < 0 || (vma->start != vma_info->start)) {
		/* Note:
		 * for some virtual devices (/dev/zero), its mmap process will be
		 * implemented as an anon mmap (will reset the new vma->start value),
		 * and the iomem unmapping process is not be triggered.
		 * so it needs to release "struct udk_vma" after the current mmap process.
		 */
		vma_info->start = vma->start;
		udk_free(vma);
		return ret;
	}
	vma_info->priv = vma;

	ret = raw_refcnt_get(&fctx->refcnt);
	BUG_ON(ret < 0);

	return 0;
}

static int native_chrdev_mmap(void *ctx, unsigned long addr, unsigned long length,
			      unsigned long prot, unsigned long offset)
{
	struct devhost_vma_info vma_info;

	vma_info.len = length;
	vma_info.start = addr;
	vma_info.prot = prot;
	vma_info.offset = offset;
	vma_info.flags = 0;
	vma_info.vma_id = 0;
	vma_info.priv = NULL;

	return native_chrdev_mmap_ex(ctx, &vma_info);
}

static int native_chrdev_munmap(void *data, int vs_is_releasing)
{
	struct udk_vma *vma = (struct udk_vma *)data;
	UNUSED(vs_is_releasing);
	struct udk_char_context *fctx;

	if (vma == NULL) {
		return 0;
	}
	if ((vma->ops != NULL) && (vma->ops->munmap != NULL)) {
		vma->ops->munmap(vma);
	}

	fctx = (struct udk_char_context *)vma->fctx;
	BUG_ON(fctx == NULL);
	(void)raw_refcnt_put(&fctx->refcnt);

	udk_free(vma);

	return 0;
}

/* This flags will be refactored by kernel eventpoll in the future, defined it here now */
static int native_chrdev_poll_ex(void *ctx, void *poll_info, unsigned int *revents)
{
	struct udk_char_context *fctx = (struct udk_char_context *)ctx;
	struct udk_char_device *udk_chrdev = NULL;
	struct udk_poll_wevent *event = NULL;
	unsigned int __revents = 0U;
	int ret;

	if (unlikely(is_invalid_ctx(fctx) || (revents == NULL))) {
		return -EINVAL;
	}
	udk_chrdev = fctx->chrdev;
	if (udk_chrdev->ops->poll == NULL) {
		*revents = DEFAULT_POLLMASK;
		udk_debug("Not support poll\n");
		return -EPERM;
	}
	if (poll_info != NULL) {
		ret = udk_poll_wevent_init(poll_info, &event);
		if (ret < 0) {
			return ret;
		}
	}

	ret = udk_chrdev->ops->poll(fctx, event, &__revents);
	if (ret < 0) {
		udk_poll_wevent_deinit(poll_info, &event);
	} else {
		*revents = __revents;
	}

	return ret;
}

static int native_chrdev_poll(void *ctx, unsigned long *event_out,
			      struct devhost_pt_info *pt_info, uint64_t key)
{
	unsigned int revents = 0;
	int ret;

	UNUSED(pt_info, key);
	ret = native_chrdev_poll_ex(ctx, NULL, &revents);
	if (ret == 0) {
		*event_out = (unsigned long)revents;
	}

	return ret;
}

static long long native_chrdev_llseek(void *ctx, long long offset, int whence)
{
	struct udk_char_context *fctx = (struct udk_char_context *)ctx;
	struct udk_char_device *udk_chrdev = NULL;

	if (unlikely(is_invalid_ctx(fctx))) {
		return -EINVAL;
	}
	udk_chrdev = fctx->chrdev;
	if (udk_chrdev->ops->llseek == NULL) {
		return -ENOSYS;
	}

	return udk_chrdev->ops->llseek(fctx, offset, whence);
}

static struct devhost_fops native_chrdev_ops __read_mostly = {
	.open	  = native_chrdev_open,
	.close	  = native_chrdev_close,
	.read	  = native_chrdev_read,
	.write	  = native_chrdev_write,
	.ioctl	  = native_chrdev_ioctl,
	.fcntl	  = native_chrdev_fcntl,
	.mmap	  = native_chrdev_mmap,
	.poll	  = native_chrdev_poll,
	.llseek	  = native_chrdev_llseek,
	.read_ex  = native_chrdev_read_ex,
	.write_ex = native_chrdev_write_ex,
	.owner	  = NULL,
};

static struct devhost_fops_ex native_chrdev_ops_ex __read_mostly = {
	.size		= sizeof(struct devhost_fops_ex),
	.poll		= native_chrdev_poll_ex,
	.compat_ioctl	= native_chrdev_compat_ioctl,
	.mmap_ex	= native_chrdev_mmap_ex,
	.write_iter	= NULL,
	.read_iter	= NULL,
};

static struct devhost_umap_ops udk_umap_ops __read_mostly = {
	.munmap = native_chrdev_munmap,
};


static int native_devnode_config(struct device_info *info,
				 char *node_name, unsigned int size,
				 struct devnode_req *req)
{
	struct udk_char_device *chrdev = NULL;
	struct udk_device_attr dev_attr;
	int ret = 0;

	if ((info == NULL) || (node_name == NULL) || (req == NULL)) {
		return -EINVAL;
	}
	chrdev = devinfo2chrdev(info);
	if ((chrdev == NULL) || (chrdev->ops == NULL)) {
		return -EINVAL;
	}

	if (chrdev->ops->config_device != NULL) {
		mem_zero_s(dev_attr);
		ret = chrdev->ops->config_device(chrdev, &dev_attr);
		if (ret == 0) {
			req->mode = dev_attr.mode;
			req->uid = dev_attr.uid;
			req->gid = dev_attr.gid;
			NOFAIL(strncpy_s(node_name, size, dev_attr.name, strlen(dev_attr.name)));
		}
	} else {
		NOFAIL(strncpy_s(node_name, size, info->devname, strlen(info->devname)));
	}

	return ret;
}

static void chrdev_release(struct udk_device *udkdev)
{
	struct udk_char_device *chrdev = NULL;

	chrdev = container_of(udkdev, struct udk_char_device, udkdev);
	if (chrdev == NULL) {
		return;
	}
	/* Free name allocated by strndup */
	if (udkdev->name != NULL) {
		free(udkdev->name);
		udkdev->name = NULL;
	}
	/* Free memory allocated for chrdev */
	free(chrdev);
}

static struct udk_char_device *chrdev_alloc(const char *name)
{
	struct udk_char_device *chrdev = NULL;
	struct udk_device *udkdev = NULL;
	int ret;

	chrdev = (struct udk_char_device *)malloc(sizeof(struct udk_char_device));
	if (chrdev == NULL) {
		return NULL;
	}
	mem_zero_s(*chrdev);
	udkdev = &chrdev->udkdev;

	udkdev->name = strndup(name, DEVICE_NAME_LEN - 1);
	if (udkdev->name == NULL) {
		free(chrdev);
		return NULL;
	}

	ret = udk_device_init(udkdev);
	if (ret < 0) {
		free(udkdev->name);
		free(chrdev);
		return NULL;
	}
	udkdev->release = chrdev_release;

	return chrdev;
}

static int chrdev_add(struct udk_char_device *chrdev,
		      const devnum_t devno, struct udk_char_operations *ops)
{
	struct udk_device *udkdev = &chrdev->udkdev;

	udk_device_capable_devfs_full(udkdev, devno, (int)MODE_CHR, &native_chrdev_ops, native_devnode_config);
	udk_device_capable_devfs_ex(udkdev, &native_chrdev_ops_ex);
	chrdev->ops = ops;

	return udk_device_add(udkdev);
}

struct udk_char_device *udk_chrdev_create(const char *name, const devnum_t devno,
					  struct udk_char_operations *fops)
{
	struct udk_char_device *chrdev = NULL;
	int ret;

	if ((name == NULL) || (strcmp(name, "") == 0)) {
		return NULL;
	}
	chrdev = chrdev_alloc(name);
	if (chrdev == NULL) {
		return NULL;
	}
	ret = chrdev_add(chrdev, devno, fops);
	if (ret != 0) {
		udk_chrdev_free(chrdev);
		return NULL;
	}

	return chrdev;
}

void udk_chrdev_free(struct udk_char_device *chrdev)
{
	if (chrdev == NULL) {
		return;
	}

	udk_device_free(&chrdev->udkdev);
}

void udk_chrdev_destroy(struct udk_char_device *chrdev)
{
	if (chrdev == NULL) {
		return;
	}

	udk_device_destroy(&chrdev->udkdev);
}

static int __chrdev_init(void)
{
	return devhost_backend_register_umap_ops(&udk_umap_ops);
}

int udk_chrdev_init(void)
{
	return __chrdev_init();
}

udk_init_call(__chrdev_init);
