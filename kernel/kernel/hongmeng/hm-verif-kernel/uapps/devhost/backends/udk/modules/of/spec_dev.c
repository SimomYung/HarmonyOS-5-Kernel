/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Special devices zero & null
 * Author: Huawei OS Kernel Lab
 * Create: Thur May 18 15:50:54 2023
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <hmasm/page.h>
#include <libstrict/strict.h>
#include <libmem/flags.h>

#include <udk/char.h>
#include <udk/device.h>
#include <udk/errno.h>
#include <udk/iolib.h>
#include <udk/log.h>
#include <udk/mm.h>
#include <udk/module.h>
#include <udk/securec.h>
#include <udk/sync.h>
#include <udk/vfs.h>
#include <vfs.h>

#include "of_dev.h"

static struct udk_char_device *g_null_dev = NULL;
static const struct udk_char_device *g_zero_dev = NULL;

#define SPECIAL_MAJOR	1
#define NULL_MINOR	3
#define ZERO_MINOR	5
#define FULL_MINOR	7
#define RANDOM_MINOR	8
#define URANDOM_MINOR	9
#define NULL_DEV_NAME	"null"
#define ZERO_DEV_NAME	"zero"
#define FULL_DEV_NAME	"full"
#define RANDOM_DEV_NAME	"random"
#define URANDOM_DEV_NAME	"urandom"

static bool is_random_dev(devnum_t devt)
{
	return (MINOR(devt) == RANDOM_MINOR);
}

devnum_t udk_zerodev_devt(void)
{
	return  MKDEV(SPECIAL_MAJOR, ZERO_MINOR);
}

/* null, zero and full mode=666 */
#define SPEC_DEV_MODE 0666

static int spec_config_device(struct udk_char_device *chrdev, struct udk_device_attr *attr)
{
	UNUSED(chrdev);
	NOFAIL(strncpy_s(attr->name, DEVICE_NAME_LEN, chrdev->udkdev.name, strlen(chrdev->udkdev.name)));
	attr->mode = SPEC_DEV_MODE;

	return 0;
}

static int null_open(struct udk_char_context *ctx)
{
	UNUSED(ctx);
	return 0;
}
static int null_close(struct udk_char_context *ctx)
{
	UNUSED(ctx);
	return 0;
}

static int null_read(struct udk_char_context *ctx, void *buf,
		     unsigned long count, unsigned long long *off)
{
	UNUSED(ctx, buf, count, off);
	return 0;
}

static int null_write(struct udk_char_context *ctx, const void *buf,
		      unsigned long count, unsigned long long *off)
{
	UNUSED(ctx, buf);
	*off += (unsigned long long)count;
	return (int)count;
}

static long long null_llseek(struct udk_char_context *ctx, long long offset, int whence)
{
	UNUSED(ctx, offset, whence);
	return 0;
}

static struct udk_char_operations nullops = {
	.open = null_open,
	.close = null_close,
	.read = null_read,
	.write = null_write,
	.ioctl = NULL,
	.compat_ioctl = NULL,
	.mmap = NULL,
	.poll = NULL,
	.llseek = null_llseek,
	.config_device = spec_config_device,
	.__rsv = {0},
	.owner	= UDK_THIS_MODULE,
};

/* Only create /dev/null for background process output redirection */
static int udk_null_device_create(void)
{
	devnum_t devt = MKDEV(SPECIAL_MAJOR, NULL_MINOR);
	int err;

	err = udk_register_chrdev_region(devt, 1, NULL_DEV_NAME);
	if (err < 0) {
		return err;
	}
	g_null_dev = udk_chrdev_create(NULL_DEV_NAME, devt, &nullops);
	if (g_null_dev == NULL) {
		udk_unregister_chrdev_region(devt, 1);
		return -EINVAL;
	}

	return 0;
}
static int zero_open(struct udk_char_context *ctx)
{
	UNUSED(ctx);

	return 0;
}
static int zero_close(struct udk_char_context *ctx)
{
	UNUSED(ctx);

	return 0;
}

#ifdef UDK_FUSION
static inline char *zeropg_ptr_of(void)
{
	return ptr_to_type(gzeropg_ptr(), char *);
}
#else
static const char __zero_page[PAGE_SIZE];
static inline char *zeropg_ptr_of(void)
{
	return (char *)(&__zero_page[0]);
}
#endif

static int zero_read(struct udk_char_context *ctx, void *buf,
		     unsigned long count, unsigned long long *off)
{
	int err;
	unsigned long pos = 0;
	unsigned long cnt = count;

	UNUSED(ctx, off);

	if (cnt == 0) {
		return 0;
	}
	if (buf == NULL) {
		return -EINVAL;
	}

	while (cnt > 0) {
		unsigned long copy_size = cnt < PAGE_SIZE ? cnt : PAGE_SIZE;
		err = udk_copy_to_user(ptr_add(buf, pos),
				       ptr_to_type(zeropg_ptr_of(), void *), copy_size);
		if (err != 0) {
			return -EIO;
		}
		cnt -= copy_size;
		pos += copy_size;
	}

	*off += (unsigned long long)pos;

	return (int)pos;
}

static int zero_write(struct udk_char_context *ctx, const void *buf,
		      unsigned long count, unsigned long long *off)
{
	UNUSED(ctx, buf, off);

	*off += (unsigned long long)count;
	return (int)count;
}

static long long zero_llseek(struct udk_char_context *ctx, long long offset, int whence)
{
	UNUSED(ctx, offset, whence);
	return 0;
}

#ifdef UDK_FUSION
static int zero_mmap(struct udk_char_context *uctx, struct udk_vma *vma)
{
	int err;
	uintptr_t rvaddr;
	uint32_t flags = MAP_ANONYMOUS;

	UNUSED(uctx);

	if (vma == NULL) {
		return -EINVAL;
	}

	/* a simple way to resolve flags conflicts(MAP_ANONYMOUS / MAP_IOMM) */
	if ((vma->flags | MAP_PRIVATE) != 0) {
		flags = flags | MAP_PRIVATE;
	} else if ((vma->flags | MAP_SHARED) != 0) {
		flags = flags | MAP_SHARED;
	} else if ((vma->flags | MAP_FIXED) != 0) {
		udk_warn("not support MAP_FIXED flags\n");
		return -EINVAL;
	}

	err = udk_mmap(vma->start, vma->len, vma->prot, flags, -1, vma->offset, &rvaddr);
	if (err < 0) {
		return err;
	}
	vma->start = (unsigned long)rvaddr;

	return err;
}
#endif

static struct udk_char_operations zeroops = {
	.open = zero_open,
	.close = zero_close,
	.read = zero_read,
	.write = zero_write,
	.ioctl = NULL,
	.compat_ioctl = NULL,
#ifdef UDK_FUSION
	.mmap = zero_mmap,
#else
	.mmap = NULL,
#endif
	.poll = NULL,
	.llseek = zero_llseek,
	.config_device = spec_config_device,
	.__rsv = {0},
	.owner	= UDK_THIS_MODULE,
};

/* Only create /dev/zero for background process output redirection */
static int udk_zero_device_create(void)
{
	devnum_t devt = MKDEV(SPECIAL_MAJOR, ZERO_MINOR);
	int err;

	err = udk_register_chrdev_region(devt, 1, ZERO_DEV_NAME);
	if (err < 0) {
		return err;
	}
	g_zero_dev = udk_chrdev_create(ZERO_DEV_NAME, devt, &zeroops);
	if (g_zero_dev == NULL) {
		udk_unregister_chrdev_region(devt, 1);
		return -EINVAL;
	}

	return 0;
}

static int full_open(struct udk_char_context *ctx)
{
	UNUSED(ctx);
	return 0;
}
static int full_close(struct udk_char_context *ctx)
{
	UNUSED(ctx);
	return 0;
}

static int full_read(struct udk_char_context *ctx, void *buf,
		     unsigned long count, unsigned long long *off)
{
	return zero_read(ctx, buf, count, off);
}

static int full_write(struct udk_char_context *ctx, const void *buf,
		      unsigned long count, unsigned long long *off)
{
	UNUSED(ctx, buf, count, off);
	return -ENOSPC;
}

static long long full_llseek(struct udk_char_context *ctx, long long offset, int whence)
{
	UNUSED(ctx, offset, whence);
	return 0;
}

static struct udk_char_operations fullops = {
	.open = full_open,
	.close = full_close,
	.read = full_read,
	.write = full_write,
	.ioctl = NULL,
	.compat_ioctl = NULL,
	.mmap = NULL,
	.poll = NULL,
	.llseek = full_llseek,
	.config_device = spec_config_device,
	.__rsv = {0},
	.owner	= UDK_THIS_MODULE,
};

/* Only create /dev/full for background process output redirection */
static int udk_full_device_create(void)
{
	int err;
	struct udk_char_device *full_dev = NULL;
	devnum_t devt = MKDEV(SPECIAL_MAJOR, FULL_MINOR);

	err = udk_register_chrdev_region(devt, 1, FULL_DEV_NAME);
	if (err < 0) {
		return err;
	}
	full_dev = udk_chrdev_create(FULL_DEV_NAME, devt, &fullops);
	if (full_dev == NULL) {
		udk_unregister_chrdev_region(devt, 1);
		return -EINVAL;
	}

	return 0;
}

static volatile int random_fd = -1;
static volatile int random_fd_nonblock = -1;
static volatile int urandom_fd = -1;
static struct udk_mutex g_random_fd_lock = UDK_MUTEX_INIT;

static int do_random_open(void)
{
	if (random_fd < 0) {
		udk_mutex_lock(&g_random_fd_lock);
		if (random_fd < 0) {
			random_fd = udk_vfs_open("/dev/random-crypto", O_RDWR, S_IRUSR | S_IWUSR);
		}
		udk_mutex_unlock(&g_random_fd_lock);
	}
	return random_fd < 0 ? random_fd : 0;
}

static int do_random_open_with_nonblock(void)
{
	if (random_fd_nonblock < 0) {
		udk_mutex_lock(&g_random_fd_lock);
		if (random_fd_nonblock < 0) {
			random_fd_nonblock = udk_vfs_open("/dev/random-crypto", O_RDWR | O_NONBLOCK, S_IRUSR | S_IWUSR);
		}
		udk_mutex_unlock(&g_random_fd_lock);
	}
	return random_fd_nonblock < 0 ? random_fd_nonblock : 0;
}

static int do_urandom_open(void)
{
	if (urandom_fd < 0) {
		udk_mutex_lock(&g_random_fd_lock);
		if (urandom_fd < 0) {
			urandom_fd = udk_vfs_open("/dev/urandom-crypto", O_RDWR, S_IRUSR | S_IWUSR);
		}
		udk_mutex_unlock(&g_random_fd_lock);
	}
	return urandom_fd < 0 ? urandom_fd : 0;
}

static inline bool is_flag_nonblock(struct udk_char_context *ctx)
{
	return ctx->io_flags & O_NONBLOCK;
}

static int random_open(struct udk_char_context *ctx)
{
	int ret = 0;

	if (is_random_dev(ctx->chrdev->udkdev.devt)) {
		if (is_flag_nonblock(ctx)) {
			ret = do_random_open_with_nonblock();
		} else {
			ret = do_random_open();
		}
	} else {
		ret = do_urandom_open();
	}

	return ret;
}

static int random_close(struct udk_char_context *ctx)
{
	UNUSED(ctx);
	return 0;
}

static int random_read(struct udk_char_context *ctx, void *buf,
		     unsigned long count, unsigned long long *off)
{
	int rsize;

	if (is_random_dev(ctx->chrdev->udkdev.devt)) {
		if (is_flag_nonblock(ctx)) {
			rsize = udk_vfs_pread(random_fd_nonblock, *off, buf, count);
		} else {
			rsize = udk_vfs_pread(random_fd, *off, buf, count);
		}
	} else {
		rsize = udk_vfs_pread(urandom_fd, *off, buf, count);
	}
	*off = *off + rsize;
	return rsize;
}

static int random_write(struct udk_char_context *ctx, const void *buf,
		      unsigned long count, unsigned long long *off)
{
	int rsize;

	if (is_random_dev(ctx->chrdev->udkdev.devt)) {
		if (is_flag_nonblock(ctx)) {
			rsize = udk_vfs_pwrite(random_fd_nonblock, *off, buf, count);
		} else {
			rsize = udk_vfs_pwrite(random_fd, *off, buf, count);
		}
		rsize = udk_vfs_pwrite(random_fd, *off, buf, count);
	} else {
		rsize = udk_vfs_pwrite(urandom_fd, *off, buf, count);
	}
	*off = *off + rsize;
	return rsize;
}

static int random_poll(struct udk_char_context *ctx,
		    struct udk_poll_wevent *pevent, unsigned int *revent)
{
	UNUSED(ctx, pevent);
	*revent = EPOLLIN;

	return 0;
}

static int random_ioctl(struct udk_char_context *ctx, unsigned int cmd, unsigned long arg)
{
	if (is_random_dev(ctx->chrdev->udkdev.devt)) {
		/* arg_sz is unused */
		return udk_vfs_ioctl(random_fd, cmd, arg, 0);
	} else {
		/* arg_sz is unused */
		return udk_vfs_ioctl(urandom_fd, cmd, arg, 0);
	}
	return 0;
}

static long long random_llseek(struct udk_char_context *ctx, long long offset, int whence)
{
	UNUSED(ctx, offset, whence);
	return 0;
}

static struct udk_char_operations randomops = {
	.open = random_open,
	.close = random_close,
	.read = random_read,
	.write = random_write,
	.ioctl = random_ioctl,
	.compat_ioctl = NULL,
	.mmap = NULL,
	.poll = random_poll,
	.llseek = random_llseek,
	.config_device = spec_config_device,
	.__rsv = {0},
	.owner	= UDK_THIS_MODULE,
};

/* Only create /dev/random for background process output redirection */
static int udk_random_device_create(const char *devname, int minor)
{
	int err;
	struct udk_char_device *random_dev = NULL;
	devnum_t devt = MKDEV(SPECIAL_MAJOR, minor);

	err = udk_register_chrdev_region(devt, 1, devname);
	if (err < 0) {
		return err;
	}
	random_dev = udk_chrdev_create(devname, devt, &randomops);
	if (random_dev == NULL) {
		udk_unregister_chrdev_region(devt, 1);
		return -EINVAL;
	}

	return 0;
}

int udk_special_device_create(void)
{
	int ret;

	ret = udk_null_device_create();
	if (ret < 0) {
		udk_error("Failed to create udk null device\n");
		return ret;
	}

	ret = udk_zero_device_create();
	if (ret < 0) {
		udk_error("Failed to create udk zero device\n");
		return ret;
	}

	ret = udk_full_device_create();
	if (ret < 0) {
		udk_error("Failed to create udk zero device\n");
		return ret;
	}
	ret = udk_random_device_create(RANDOM_DEV_NAME, RANDOM_MINOR);
	if (ret < 0) {
		udk_error("Failed to create udk random device\n");
		return ret;
	}
	ret = udk_random_device_create(URANDOM_DEV_NAME, URANDOM_MINOR);
	if (ret < 0) {
		udk_error("Failed to create udk urandom device\n");
		return ret;
	}

	return 0;
}
