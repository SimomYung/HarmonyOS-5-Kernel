/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Udk uio framework
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 3 09:51:16 2021
 */
#include <udk/uio.h>

#include <poll.h>
#include <udk/of.h>
#include <udk/mm.h>
#include <udk/irq.h>
#include <udk/log.h>
#include <udk/char.h>
#include <udk/poll.h>
#include <hmasm/page.h>
#include <udk/iolib.h>
#include <udk/errno.h>
#include <udk/mutex.h>
#include <udk/atomic.h>
#include <udk/device.h>
#include <udk/lib/idr.h>
#include <internal/init.h>
#include <udk/lib/string.h>
#include <udk/thread_cond.h>
#include <libstrict/strict.h>
#include <libhmsync/raw_mutex.h>
#include <libhwsecurec/securec.h>

#include "uio_sysfs.h"

#define UIO_NAME		"uio"
#define UIO_MAX_DEVICES		(1U << MINORBITS)

struct uio_event {
	struct udk_atomic_int *count;
	struct udk_mutex mutex;
	struct udk_thread_cond thread_cond;
};

struct uio_device {
	unsigned int minor;
	struct udk_uio_info *info;
	struct udk_device *dev;
	struct uio_event *event;
	struct udk_mutex mutex;
	struct udk_poll_wqueue wqueue;
};

struct udk_uio_manager {
	struct udk_mutex mutex;
	unsigned int major;
	struct idr devices;
};

struct uio_context {
	struct uio_device *uiodev;
	int event_count;
};

static struct udk_uio_manager uio_manager;

static inline void uio_manager_lock(void)
{
	udk_mutex_lock(&uio_manager.mutex);
}

static inline void uio_manager_unlock(void)
{
	udk_mutex_unlock(&uio_manager.mutex);
}

static inline void uio_device_lock(struct uio_device *uiodev)
{
	udk_mutex_lock(&uiodev->mutex);
}

static inline void uio_device_unlock(struct uio_device *uiodev)
{
	udk_mutex_unlock(&uiodev->mutex);
}

static inline devnum_t uio_get_devno(unsigned int minor)
{
	return MKDEV(uio_manager.major, minor);
}

static inline struct uio_device *ctx_device_of(const struct uio_context *ctx)
{
	return (ctx == NULL) ? NULL : ctx->uiodev;
}

static inline bool is_event_invalid(const struct uio_event *event)
{
	return (event == NULL || event->count == NULL);
}

static inline void uio_event_wakeup(struct uio_event *event)
{
	(void)udk_atomic_int_inc(event->count);
	(void)udk_thread_cond_broadcast(&event->thread_cond);
}

static inline void uio_event_wait(struct uio_event *event)
{
	udk_mutex_lock(&event->mutex);
	(void)udk_thread_cond_wait(&event->thread_cond, &event->mutex);
	udk_mutex_unlock(&event->mutex);
}

static int uio_open(struct udk_char_context *uctx)
{
	struct uio_context *uioctx = NULL;
	struct uio_device *uiodev = NULL;
	struct udk_uio_info *info = NULL;
	struct uio_event *event = NULL;
	int err;

	if (uctx == NULL) {
		return -EINVAL;
	}
	uiodev = (struct uio_device *)udk_device_priv_get(&(uctx->chrdev->udkdev));
	if (uiodev == NULL) {
		return -EINVAL;
	}
	uioctx = udk_malloc(sizeof(struct uio_context));
	if (uioctx == NULL) {
		return -ENOMEM;
	}
	mem_zero_s(*uioctx);
	info = uiodev->info;
	if (info->ops != NULL && info->ops->open != NULL) {
		uio_device_lock(uiodev);
		err = info->ops->open(info);
		uio_device_unlock(uiodev);
		if (err < 0) {
			udk_free(uioctx);
			return err;
		}
	}
	if (info->irq > 0) {
		event = uiodev->event;
		uioctx->event_count = udk_atomic_int_read(event->count);
	}
	uioctx->uiodev = uiodev;
	uctx->ctx = uioctx;

	return 0;
}

static int uio_close(struct udk_char_context *uctx)
{
	struct uio_context *uioctx = NULL;
	struct uio_device *uiodev = NULL;
	struct udk_uio_info *info = NULL;
	int err;

	if (uctx == NULL || uctx->ctx == NULL) {
		return -EINVAL;
	}
	uioctx = uctx->ctx;
	uiodev = ctx_device_of((struct uio_context *)uctx->ctx);
	if ((uiodev == NULL) || (uiodev->info == NULL)) {
		return -EINVAL;
	}
	info = uiodev->info;
	if (info->ops != NULL && info->ops->close != NULL) {
		uio_device_lock(uiodev);
		err = info->ops->close(info);
		uio_device_unlock(uiodev);
		if (err < 0) {
			return err;
		}
	}
	udk_free(uioctx);

	return 0;
}

static int uio_read(struct udk_char_context *uctx, void __user *buf,
		    unsigned long count, unsigned long long *off)
{
	struct uio_context *uioctx = NULL;
	struct uio_device *uiodev = NULL;
	struct uio_event *event = NULL;
	struct udk_uio_info *info = NULL;
	int event_count = 0;
	int err;

	if (count != sizeof(int) || buf == NULL ||
	    uctx == NULL || uctx->ctx == NULL) {
		return -EINVAL;
	}
	uioctx = uctx->ctx;
	uiodev = ctx_device_of((struct uio_context *)uctx->ctx);
	if ((uiodev == NULL) || (uiodev->info == NULL)) {
		return -EINVAL;
	}
	info = uiodev->info;
	if (info->irq <= 0) {
		return -EIO;
	}
	if (is_event_invalid(uiodev->event)) {
		return -EINVAL;
	}
	event = uiodev->event;
	event_count = udk_atomic_int_read(event->count);
	while (uioctx->event_count == event_count) {
		uio_event_wait(event);
		event_count = udk_atomic_int_read(event->count);
	}
	err = udk_copy_to_user(buf, &event_count, count);
	if (err < 0) {
		return err;
	}
	uioctx->event_count = event_count;
	*off += count;

	return (int)count;
}

static int uio_write(struct udk_char_context *uctx, const void __user *buf,
		     unsigned long count, unsigned long long *off)
{
	struct uio_device *uiodev = NULL;
	struct udk_uio_info *info = NULL;
	int flags, err;

	if (count != sizeof(int) || buf == NULL ||
	    uctx == NULL || uctx->ctx == NULL) {
		return -EINVAL;
	}
	uiodev = ctx_device_of((struct uio_context *)uctx->ctx);
	if (uiodev == NULL || uiodev->info == NULL) {
		return -EINVAL;
	}
	info = uiodev->info;
	if (info->irq <= 0) {
		return -EIO;
	}
	if (info->ops == NULL || info->ops->irq_control == NULL) {
		return -ENOSYS;
	}
	err = udk_copy_from_user(&flags, buf, count);
	if (err < 0) {
		return err;
	}
	uio_device_lock(uiodev);
	err = info->ops->irq_control(info, flags);
	uio_device_unlock(uiodev);
	if (err < 0) {
		return err;
	}
	*off += count;

	return (int)count;
}

static inline unsigned int uio_mem_offset(unsigned long offset)
{
	return (unsigned int)offset >> PAGE_SHIFT;
}

static int uio_mmap(struct udk_char_context *uctx, struct udk_vma *vma)
{
	struct uio_device *uiodev = NULL;
	struct udk_uio_info *info = NULL;
	struct udk_uio_mem *mem = NULL;
	int err;

	if ((uctx == NULL) || (uctx->ctx == NULL) || (vma == NULL)) {
		return -EINVAL;
	}
	uiodev = ctx_device_of((struct uio_context *)uctx->ctx);
	if (uiodev == NULL || uiodev->info == NULL) {
		return -EINVAL;
	}
	info = uiodev->info;
	mem = (struct udk_uio_mem *)idr_find(&info->mem,
					     uio_mem_offset(vma->offset));
	if (mem == NULL) {
		udk_error("iomem slot not found, slot=%u\n", uio_mem_offset(vma->offset));
		return -ENOENT;
	}
	if (vma->len > mem->size) {
		udk_error("uio_mmap doesn't has enough physical memory: "
			  "vsize=0x%lx, psize=0x%lx\n",
			  vma->len, mem->size);
		return -ENOMEM;
	}
	uio_device_lock(uiodev);
	if (info->ops != NULL && info->ops->mmap != NULL) {
		err = info->ops->mmap(info, vma);
	} else {
		err = udk_iomem_mmap(vma->start, mem->addr, vma->len, vma->prot, info->mem_flags);
	}
	uio_device_unlock(uiodev);

	return err;
}

static int uio_poll(struct udk_char_context *uctx, struct udk_poll_wevent *pevent, unsigned int *revent)
{
	struct uio_context *uioctx = NULL;
	struct uio_device *uiodev = NULL;
	struct uio_event *event = NULL;
	struct udk_uio_info *info = NULL;
	int ret;

	if ((uctx == NULL) || (uctx->ctx == NULL) || (revent == NULL)) {
		return -EINVAL;
	}
	uioctx = (struct uio_context *)uctx->ctx;
	uiodev = ctx_device_of(uioctx);
	if (uiodev == NULL || uiodev->info == NULL) {
		return -EINVAL;
	}
	info = uiodev->info;
	if (info->irq <= 0) {
		return -EIO;
	}
	event = uiodev->event;
	if (is_event_invalid(event)) {
		return -EINVAL;
	}
	ret = udk_poll_wqueue_add(&uiodev->wqueue, pevent);
	if (ret < 0) {
		udk_warn("uio add poll wait queue failed, err=%s\n", strerror(ret));
		return ret;
	}
	if (udk_atomic_int_read(event->count) != uioctx->event_count) {
		*revent = POLLIN | POLLRDNORM;
	}

	return 0;
}

static struct udk_char_operations uio_fops = {
	.open	= uio_open,
	.close	= uio_close,
	.read	= uio_read,
	.write	= uio_write,
	.ioctl	= NULL,
	.compat_ioctl = NULL,
	.mmap	= uio_mmap,
	.poll	= uio_poll,
	.llseek	= NULL,
	.__rsv	= {0},
	.owner	= NULL,
};

static struct uio_event *uio_event_create(void)
{
	struct uio_event *event = NULL;
	struct udk_atomic_int *count = NULL;

	event = (struct uio_event *)udk_malloc(sizeof(struct uio_event));
	if (event == NULL) {
		return NULL;
	}
	mem_zero_s(*event);
	count = udk_atomic_int_alloc(0);
	if (count == NULL) {
		udk_free(event);
		return NULL;
	}
	event->count = count;
	udk_mutex_init(&event->mutex);
	(void)udk_thread_cond_init(&event->thread_cond, NULL);

	return event;
}

static void uio_event_destroy(struct uio_event *event)
{
	if (event != NULL) {
		udk_atomic_int_destroy(event->count);
		udk_free(event);
	}
}

static int uio_irq_handler(int irq, void *dev_id)
{
	struct uio_device *uiodev = (struct uio_device *)dev_id;
	struct udk_uio_info *info = NULL;
	struct uio_event *event = NULL;
	int err;

	if (uiodev == NULL || uiodev->info == NULL ||
	    uiodev->dev == NULL) {
		return -EINVAL;
	}
	info = uiodev->info;
	if (info->ops == NULL || info->ops->irq_handler == NULL) {
		return -ENOSYS;
	}
	err = info->ops->irq_handler(info, irq);
	if (err < 0) {
		return err;
	}
	event = uiodev->event;
	if (is_event_invalid(event)) {
		return -EINVAL;
	}
	uio_event_wakeup(event);
	udk_poll_wqueue_wakeup(&uiodev->wqueue, POLLIN | POLLRDNORM);

	return 0;
}

static int uio_irq_init(struct uio_device *uiodev, const char *name,
			int irq)
{
	struct uio_event *event = NULL;
	int err;

	if (irq <= 0) {
		return 0;
	}
	err = udk_request_irq((unsigned int)irq, uio_irq_handler,
			      0, name, (void *)uiodev);
	if (err < 0) {
		udk_error("request irq for %s failed, irq=%d, err: %s",
			  name, irq, strerror(-err));
		return err;
	}
	event = uio_event_create();
	if (event == NULL) {
		udk_error("create uio event failed\n");
		udk_free_irq((unsigned int)irq);
		return -ENOMEM;
	}
	uiodev->event = event;

	return 0;
}

static void uio_irq_free(struct uio_device *uiodev)
{
	struct udk_uio_info *info = uiodev->info;
	int irq = info->irq;

	if (irq > 0) {
		udk_free_irq((unsigned int)irq);
		uio_event_destroy(uiodev->event);
	}
}

static int uio_device_add(struct uio_device *uiodev)
{
	int minor;

	minor = idr_alloc(&uio_manager.devices, (void *)uiodev);
	if (minor < 0) {
		udk_error("alloc minor failed, err=%s\n", strerror(-minor));
		return minor;
	}
	if ((unsigned int)minor >= UIO_MAX_DEVICES) {
		udk_error("no more uio devices\n");
		(void)idr_remove(&uio_manager.devices, (unsigned int)minor);
		return -ENOMEM;
	}
	uiodev->minor = (unsigned int)minor;

	return 0;
}

static void uio_device_remove(unsigned int id)
{
	(void)idr_remove(&uio_manager.devices, id);
}

static inline struct uio_device *uio_device_find(const struct udk_uio_info *info)
{
	struct uio_device *uiodev = NULL;
	unsigned int id = 0;

	do {
		uiodev = (struct uio_device *)idr_get_next(&uio_manager.devices, &id);
		id++;
	} while(uiodev != NULL && uiodev->info != info);

	return uiodev;
}

static struct udk_device *uio_chrdev_create(unsigned int minor, const char *name)
{
	char name_buf[DEVICE_NAME_LEN];
	const char *uio_dev_name = NULL;
	struct udk_char_device *chrdev = NULL;
	int err;

	if (name == NULL) {
		mem_zero_a(name_buf);
		err = snprintf_s(name_buf, DEVICE_NAME_LEN, DEVICE_NAME_LEN - 1,
				 "%s%u", UIO_NAME, minor);
		if (err < 0) {
			udk_error("generate uio device name failed, err=%s\n",
				  strerror(-err));
			return NULL;
		}
		uio_dev_name = name_buf;
	} else {
		uio_dev_name = name;
	}
	chrdev = udk_chrdev_create(uio_dev_name, uio_get_devno(minor),
				   &uio_fops);
	if (chrdev == NULL) {
		udk_error("alloc char device failed\n");
		return NULL;
	}

	return &(chrdev->udkdev);
}

static int uio_device_init(struct uio_device *uiodev, struct udk_uio_info *info)
{
	struct udk_device *dev = NULL;
	int err;

	dev = uio_chrdev_create(uiodev->minor, info->name);
	if (dev == NULL) {
		udk_error("create char device for uio%u failed\n",
			  uiodev->minor);
		return -EINVAL;
	}
	err = uio_sysfs_create(dev, info);
	if (err < 0) {
		udk_error("create device sysfs failed, err=%s\n", strerror(-err));
		udk_device_destroy(dev);
		return err;
	}
	err = uio_irq_init(uiodev, dev->name, info->irq);
	if (err < 0) {
		udk_error("init uio irq failed, err: %s\n", strerror(-err));
		(void)uio_sysfs_delete(dev, info);
		udk_device_destroy(dev);
		return err;
	}
	uiodev->dev = dev;
	udk_device_priv_set(dev, uiodev);
	uiodev->info = info;
	udk_mutex_init(&uiodev->mutex);
	udk_poll_wqueue_init(&uiodev->wqueue);

	return 0;
}

static int uio_device_set_opener(struct udk_device *dev, int flag)
{
	raw_mutex_lock(&dev->opener_lock);
	if (dev->nr_opener > 0) {
		raw_mutex_unlock(&dev->opener_lock);
		return -EBUSY;
	}
	dev->nr_opener = flag;
	raw_mutex_unlock(&dev->opener_lock);

	return 0;
}

static int uio_device_destroy(struct uio_device *uiodev)
{
	int err;

	if (uiodev->dev != NULL) {
		err = uio_device_set_opener(uiodev->dev,
					    DEVICE_OPEN_INVALID_FLAG);
		if (err < 0) {
			return err;
		}
		err = uio_sysfs_delete(uiodev->dev, uiodev->info);
		if (err < 0) {
			udk_error("%s sysfs delete failed, err=%s\n",
				  uiodev->dev->name, strerror(-err));
			(void)uio_device_set_opener(uiodev->dev, 0);
			return err;
		}
		udk_device_destroy(uiodev->dev);
	}
	uio_irq_free(uiodev);
	uio_device_remove(uiodev->minor);
	udk_free(uiodev);

	return 0;
}

static int uio_device_create(struct udk_uio_info *info)
{
	struct uio_device *uiodev = NULL;
	int err;

	uiodev = (struct uio_device *)udk_malloc(sizeof(struct uio_device));
	if (uiodev == NULL) {
		udk_error("alloc uio device failed\n");
		return -ENOMEM;
	}
	mem_zero_s(*uiodev);
	err = uio_device_add(uiodev);
	if (err < 0) {
		udk_error("add uio device failed, err=%s\n", strerror(-err));
		udk_free(uiodev);
		return err;
	}
	err = uio_device_init(uiodev, info);
	if (err < 0) {
		udk_error("init uio device failed, err=%s\n", strerror(-err));
		uio_device_remove(uiodev->minor);
		udk_free(uiodev);
		return err;
	}

	return 0;
}

static int uio_mem_destroy(unsigned int id, void *item, void *data)
{
	UNUSED(id, data);
	udk_free((struct udk_uio_mem *)item);

	return 0;
}

int udk_uio_info_destroy(struct udk_uio_info *info)
{
	if (info == NULL) {
		return -EINVAL;
	}
	(void)idr_for_each(&info->mem, uio_mem_destroy, (void *)&info->mem);
	idr_free(&info->mem);
	udk_free(info);

	return 0;
}

static int uio_info_init(struct udk_uio_info *info, struct udk_device *udkdev)
{
	struct udk_uio_mem *mem = NULL;
	struct udk_resource *res = NULL;
	unsigned int i;
	int err;

	idr_init(&info->mem);
	for (i = 0; i < MAX_NUM_RESOURCE; i++) {
		res = udk_device_get_resource(udkdev, IORESOURCE_REG, i);
		if (res == NULL) {
			break;
		}
		mem = (struct udk_uio_mem *)udk_malloc(sizeof(struct udk_uio_mem));
		if (mem == NULL) {
			return -ENOMEM;
		}
		mem->name = res->name;
		mem->addr = res->addr;
		mem->size = res->size;
		err = idr_set(&info->mem, (void *)mem, i);
		if (err < 0) {
			udk_free(mem);
			(void)idr_for_each(&info->mem, uio_mem_destroy, (void *)&info->mem);
			return err;
		}
	}
	if (udk_device_prop_read_bool(udkdev, "normal")) {
		info->mem_flags |= UDK_IOMEM_NORMAL;
	} else {
		info->mem_flags |= UDK_IOMEM_DEVICE;
	}
	info->name = udk_of_get_node_name(udkdev->dt_node);
	info->irq = udk_device_get_irq(udkdev, 0);

	return 0;
}

struct udk_uio_info *udk_uio_info_create(struct udk_device *udkdev)
{
	struct udk_uio_info *info = NULL;
	int err;

	if (udkdev == NULL) {
		udk_error("invalid input\n");
		return NULL;
	}
	info = (struct udk_uio_info *)udk_malloc(sizeof(struct udk_uio_info));
	if (info  == NULL) {
		udk_error("alloc info failed\n");
		return NULL;
	}
	mem_zero_s(*info);
	err = uio_info_init(info, udkdev);
	if (err < 0) {
		udk_error("init uio info failed, err=%s\n", strerror(-err));
		(void)udk_uio_info_destroy(info);
		return NULL;
	}

	return info;
}

int udk_uio_register(struct udk_uio_info *info)
{
	int err;

	if (info == NULL) {
		return -EINVAL;
	}
	err = uio_device_create(info);
	if (err < 0) {
		udk_error("create uio device failed, err=%s\n", strerror(-err));
		return err;
	}

	return 0;
}

int udk_uio_unregister(const struct udk_uio_info *info)
{
	struct uio_device *uiodev = NULL;
	int err;

	if (info == NULL) {
		return -EINVAL;
	}
	uio_manager_lock();
	uiodev = uio_device_find(info);
	if (uiodev == NULL) {
		udk_warn("can not find uio device\n");
		uio_manager_unlock();
		return -ENOENT;
	}
	err = uio_device_destroy(uiodev);
	if (err < 0) {
		udk_error("destroy uio_device failed, err=%s\n", strerror(-err));
		uio_manager_unlock();
		return err;
	}
	uio_manager_unlock();

	return 0;
}

static int uio_major_init(void)
{
	devnum_t devno = 0;
	int err;

	err = udk_alloc_chrdev_region(&devno, 0, UIO_MAX_DEVICES, UIO_NAME);
	if (err < 0) {
		udk_error("alloc major for uio chrdev failed, err=%s\n",
			  strerror(-err));
		return err;
	}
	uio_manager.major = MAJOR(devno);

	return 0;
}

static int udk_uio_init(void)
{
	idr_init(&uio_manager.devices);
	udk_mutex_init(&uio_manager.mutex);

	return uio_major_init();
}

udk_init_call(udk_uio_init);
