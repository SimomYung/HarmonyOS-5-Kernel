/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */

#include <sys/ioctl.h>

#include <udk/module.h>
#include <udk/device.h>
#include <udk/iolib.h>
#include <udk/panic.h>
#include <udk/errno.h>
#include <udk/log.h>
#include <libstrict/strict.h>

#include "udktests.h"

#define TESTIO 0xEA

#define UDKTEST_DEVICE 1
#define UDKTEST_DRIVER 2
#define UDKTEST_IOCTL  3
#define UDKTEST_SETDHPID  4
#define UDKTEST_SHMLOCK 5
#define UDKTEST_BASEMINOR 0
#define UDKTEST_COUNT 255

#define UDKTEST_IOC_DEVICE           _IO(TESTIO, UDKTEST_DEVICE)
#define UDKTEST_IOC_DRIVER           _IO(TESTIO, UDKTEST_DRIVER)
#define UDKTEST_IOC_IOCTL            _IO(TESTIO, UDKTEST_IOCTL)
#define UDKTEST_IOC_SETDHPID         _IO(TESTIO, UDKTEST_SETDHPID)
#define UDKTEST_IOC_SHMLOCK          _IO(TESTIO, UDKTEST_SHMLOCK)

static int udktest_open(struct device_info *devinfo, void **ctx, unsigned int flags)
{
	UNUSED(flags);
	return udktest__fops_open(devinfo, ctx);
}

static int udktest_release(void *ctx)
{
	return udktest__fops_release(ctx);
}

static int udktest_read(void *ctx, void *buf, unsigned long count,
			unsigned long long off)
{
	return udktest__fops_read(ctx, buf, count, off);
}

static int udktest_write(void *ctx, const void *buf, unsigned long count,
			 unsigned long long off)
{
	return udktest__fops_write(ctx, buf, count, off);
}

static long long udktest_llseek(void *ctx, long long offset, int whence)
{
	return udktest__fops_llseek(ctx, offset, whence);
}

static int udktest_ioctl(void *ctx, unsigned int cmd, unsigned long arg)
{
	int ret = 0;

	switch (cmd) {
	case UDKTEST_IOC_DEVICE:
		ret = udktest__device();
		break;
	case UDKTEST_IOC_DRIVER:
		ret = udktest__driver();
		break;
	case UDKTEST_IOC_IOCTL:
		ret = udktest__fops_ioctl(ctx, cmd, arg);
		break;
	case UDKTEST_IOC_SETDHPID:
		ret = udktest_setpid(ctx, cmd, arg);
		break;
	case UDKTEST_IOC_SHMLOCK:
		ret = udktest__shmlock(ctx, cmd, arg);
		break;
	default:
		break;
	}

	return (ret == 0) ? 0 : -EINVAL;
}

static int udktest_mmap(void *ctx,
			unsigned long addr, unsigned long length,
			unsigned long prot, unsigned long offset)
{
	return udktest__fops_mmap(ctx, addr, length, prot, offset);
}

static struct devhost_fops udktest_fops = {
	.open = udktest_open,
	.read = udktest_read,
	.write = udktest_write,
	.close = udktest_release,
	.ioctl = udktest_ioctl,
	.llseek = udktest_llseek,
	.mmap = udktest_mmap,
	.poll = NULL,
	.owner = UDK_THIS_MODULE,
};

static struct udk_device *g_dev = NULL;
static unsigned int major = 0;
static unsigned int minor = 0;

/*
 * If the init call fails, the whole test fails.
 * Make sure this will not happen.
 */
static int udktest_init(void)
{
	struct udk_device *dev = NULL;
	devnum_t devno = 0;
	int ret;

	ret = udk_alloc_chrdev_region(&devno, UDKTEST_BASEMINOR,
				      UDKTEST_COUNT, "udktest");
	if (ret)
		return ret;
	major = MAJOR(devno);
	minor = MINOR(devno);

	dev = udk_create_chrdev_duplicated("udktest", devno, &udktest_fops);
	if (dev == NULL) {
		/* Module fails to init if returning negative values */
		return -EINVAL;
	}
	g_dev = dev;

	ret = udktest__mm();
	if (ret < 0) {
		udk_error("udk mm test failed with ret: %s\n", strerror(-ret));
		return ret;
	}

	ret = udktest__mempool();
	if (ret < 0) {
		udk_error("udk mempool test failed with ret: %s\n", strerror(-ret));
		return ret;
	}

	ret = udktest__thread();
	if (ret < 0) {
		udk_error("udk thread test failed with ret: %s\n", strerror(-ret));
		return ret;
	}

	return 0;
}

static int udktest_exit(void)
{
	devnum_t devno;

	udk_device_destroy(g_dev);
	devno = MKDEV(major, minor);
	udk_unregister_chrdev_region(devno, UDKTEST_COUNT);

	return 0;
}

udk_module_init(udktest_init);
udk_module_exit(udktest_exit);
