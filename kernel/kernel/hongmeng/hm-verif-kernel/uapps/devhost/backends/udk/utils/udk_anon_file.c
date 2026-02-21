/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: This is the implementation of udk vfs interface
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 27 11:24:17 2024
 */
#include <fd.h>
#include <vfs.h>

#include <devhost/file.h>
#include <devhost/device.h>
#include <libsysif/fs/api.h>
#include <libdevhost/devhost.h>

#include <udk/log.h>
#include <udk/vfs.h>
#include <udk/char.h>
#include <udk/iolib.h>

#include <internal/init.h>
#include <internal/device.h>
#include <libhmsync/raw_scoped_coptr.h>

/* Cache devfs rref for further actvcalls */
#define DEVFS_PATH 		"/dev"
static rref_t g_devfs_rref = 0ULL;
static uint32_t g_devfs_cnode_idx = 0U;
static struct device_info *p_anon_devinfo = NULL;
static struct udk_char_device *p_anon_chrdev = NULL;

/* anon device fops */
static int anon_fops_no_open(struct udk_char_context *ctx)
{
	UNUSED(ctx);
	return 0;
}

static int anon_fops_ioctl(struct udk_char_context *ctx, unsigned int request, unsigned long arg)
{
	struct anon_file *f = (struct anon_file *)ctx;

	if ((f == NULL) || (f->fops == NULL) || (f->fops->ioctl == NULL)) {
		return -ENOSYS;
	}
	return f->fops->ioctl(ctx, request, arg);
}

static long long anon_fops_llseek(struct udk_char_context *ctx, long long offset, int whence)
{
	struct anon_file *f = (struct anon_file *)ctx;

	if ((f == NULL) || (f->fops == NULL) || (f->fops->llseek == NULL)) {
		return -ENOSYS;
	}
	return f->fops->llseek(ctx, offset, whence);
}

static int anon_fops_read(struct udk_char_context *ctx, void *buf, unsigned long count, unsigned long long *off)
{
	struct anon_file *f = (struct anon_file *)ctx;

	if ((f == NULL) || (f->fops == NULL) || (f->fops->read == NULL)) {
		return -ENOSYS;
	}

	return f->fops->read(ctx, buf, count, off);
}

static int anon_fops_write(struct udk_char_context *ctx, const void *buf, unsigned long count, unsigned long long *off)
{
	struct anon_file *f = (struct anon_file *)ctx;

	if ((f == NULL) || (f->fops == NULL) || (f->fops->write == NULL)) {
		return -ENOSYS;
	}
	return f->fops->write(ctx, buf, count, off);
}

static int anon_fops_mmap(struct udk_char_context *ctx, struct udk_vma *vma)
{
	struct anon_file *f = (struct anon_file *)ctx;

	if ((f == NULL) || (f->fops == NULL) || (f->fops->mmap == NULL)) {
		return -ENOSYS;
	}
	return f->fops->mmap(ctx, vma);
}

static int anon_fops_close(struct udk_char_context *ctx)
{
	struct anon_file *f = (struct anon_file *)ctx;

	if ((f == NULL) || (f->fops == NULL) || (f->fops->close == NULL)) {
		return -ENOSYS;
	}
	return f->fops->close(ctx);
}

static int anon_fops_poll(struct udk_char_context *ctx, struct udk_poll_wevent *pevent, unsigned int *revent)
{
	struct anon_file *f = (struct anon_file *)ctx;

	if ((f == NULL) || (f->fops == NULL) || (f->fops->poll == NULL)) {
		return -ENOSYS;
	}
	return f->fops->poll(ctx, pevent, revent);
}

/* for anonymous device */
static struct udk_char_operations udk_anon_fops = {
	.open = anon_fops_no_open,
	.ioctl = anon_fops_ioctl,
	.llseek = anon_fops_llseek,
	.read = anon_fops_read,
	.write = anon_fops_write,
	.mmap = anon_fops_mmap,
	.close = anon_fops_close,
	.poll = anon_fops_poll,
};

static int devfs_info_init(void)
{
	int srv_fd;
	int err;
	struct server_info *si = NULL;

	srv_fd = vfs_open(DEVFS_PATH, O_PATH, 0);
	if (srv_fd < 0) {
		udk_warn("get server name failed: %s\n", hmstrerror(srv_fd));
		return srv_fd;
	}

	err = get_fsmgr_si(&si);
	if (err != E_HM_OK) {
		udk_warn("get server fd failed\n");
		(void)vfs_close(srv_fd);
		return E_HM_BADF;
	}

	g_devfs_rref = si->actv_rref;
	g_devfs_cnode_idx = si->cnode_idx;

	put_server_info(si);
	(void)vfs_close(srv_fd);

	return E_HM_OK;
}

static void devfs_info_clear(void)
{
	g_devfs_rref = 0;
	g_devfs_cnode_idx = 0;
}

static int anon_register_devhost(void)
{
	int ret;
	uref_t dh_ap_uref;

	/* export udk ref to fs/sysmgr for interaction */
	dh_ap_uref = libdh_ap_export(g_devfs_cnode_idx);
	if (IS_REF_ERR(dh_ap_uref)) {
		udk_error("export actv pool to sysmgr failed, err=%s\n",
			  hmstrerror(REF_TO_ERR(dh_ap_uref)));
		return REF_TO_ERR(dh_ap_uref);
	}

	ret = actvcapcall_fscall_vfs_anon_register_devhost(g_devfs_rref,
							   dh_ap_uref);
	if (ret != E_HM_OK) {
		(void)hm_ucap_revoke(0, dh_ap_uref);
		udk_error("register devhost to fs failed: %s\n", hmstrerror(ret));
	}

	return ret;
}

/* Failures are not allowed int the anon_dev initialization phase */
static int anon_devinfo_init(void)
{
	int ret;

	if (p_anon_devinfo != NULL) {
		udk_warn("anon_devinfo already initialized\n");
		return -EEXIST;
	}

	ret = devfs_info_init();
	if (ret != E_HM_OK) {
		udk_error("get devfs rref failed, ret=%d, %s\n", ret, hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	ret = anon_register_devhost();
	if (ret != E_HM_OK) {
		udk_error("register devhost to devfs failed, ret=%d, %s\n", ret, hmstrerror(ret));
		devfs_info_clear();
		return -hmerrno2posix(ret);
	}

	p_anon_chrdev = udk_chrdev_create("anon_file", 0U, &udk_anon_fops);
	if (p_anon_chrdev == NULL) {
		udk_warn("failed to create anon chrdev\n");
		devfs_info_clear();
		return -ENOMEM;
	}
	p_anon_devinfo = __udk_device_devinfo_of(&p_anon_chrdev->udkdev);

	return 0;
}

static int udk_anon_file_init(void)
{
	return anon_devinfo_init();
}

int udk_anon_unused_fd_get(unsigned int flags)
{
	int ret;

	ret = actvxactcapcall_fscall_vfs_get_unused_fd(true, true, g_devfs_rref, flags);
	if (ret < 0) {
		udk_warn("get unused fd failed: %s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	return ret;
}

void udk_anon_unused_fd_put(int fd)
{
	int ret = 0;

	/* free fd and file in fs */
	ret = actvxactcapcall_fscall_vfs_put_unused_fd(true, true, g_devfs_rref, fd);
	if (ret < 0) {
		udk_error("put unused fd failed, %s\n", hmstrerror(ret));
	}
}

void udk_anon_fd_install(unsigned int fd, struct anon_file *file)
{
	int ret = 0;
	struct filp_node *node = NULL;
	unsigned int dev_fops_flags = 0;

	if (file == NULL) {
		udk_warn("input file is NULL\n");
		return;
	}
	node = file->filp_node;
	dev_fops_flags = devhost_device_get_fops_flags(node->devinfo, node->filp);
	ret = actvxactcapcall_fscall_vfs_fd_info_update(true, true, g_devfs_rref,
							(int)fd, ptr_to_u64(node), NULL, 0, dev_fops_flags);
	if (ret < 0) {
		udk_warn("update fd info failed, %s\n", hmstrerror(ret));
	}
}

struct anon_file *udk_anon_file_alloc(const char *name, struct udk_char_operations *fops,
				      void *priv, unsigned long flags)
{
	struct anon_file *file = NULL;
	struct filp_node *node = NULL;

	UNUSED(name);
	if (fops == NULL) {
		udk_warn("file ops cannot be null\n");
		return NULL;
	}

	file = (struct anon_file *)udk_malloc(sizeof(struct anon_file));
	if (file == NULL) {
		udk_warn("cannot allocate file, no memory\n");
		return NULL;
	}

	node = devhost_alloc_filp_node_fast(p_anon_devinfo);
	if (node == NULL) {
		udk_free(file);
		udk_warn("failed to allocate file_node\n");
		return NULL;
	}

	mem_zero_s(*file);
	/* node->filp is udk_chr_context, ctx is equals to anon_file */
	node->filp = (void *)file;
	file->fops = fops;
	file->filp_node = node;

	(void)udk_chrctx_init(p_anon_chrdev, &file->ctx, priv,
			      flags);

	return file;
}

struct anon_file *udk_anon_fget(int fd)
{
	struct filp_node *node;
	int ret;
	struct __actvret_fscall_vfs_query_info fscall_ret = {0};

	/* Use flag = 1 to make vfs to do file_get operation to ensure the validity of anon_file. */
	ret = actvxactcapcall_fscall_vfs_query_info(true, true,
						    g_devfs_rref, fd, 0,
						    IO_DEV_ALL_GET | IO_DEV_CALLER_CHECK, &fscall_ret);
	if (ret != E_HM_OK || fscall_ret.file_idx == ~0ULL) {
		udk_error("udk query vfs info failed, cnode_idx:%u, fd:%d, %s\n",
			  sysif_actv_fwd_cnode_idx(), fd, hmstrerror(ret));
		return NULL;
	}
	node = u64_to_ptr(fscall_ret.file_idx, struct filp_node);
	BUG_ON(node == NULL);

	if (p_anon_devinfo != NULL && p_anon_devinfo != node->devinfo) {
		udk_error("udk not use anon file fd: %d\n", fd);
		return NULL;
	}

	/* register_file & query_info hold node refcnt */
	ret = raw_compounded_auto_refcnt_counter(&node->refcnt);

	BUG_ON(ret <= 1);

	return (struct anon_file *)node->filp;
}

void udk_anon_fput(struct anon_file *file)
{
	if (file == NULL) {
		return;
	}
	devhost_put_file(file->filp_node);
}

udk_init_call(udk_anon_file_init);
