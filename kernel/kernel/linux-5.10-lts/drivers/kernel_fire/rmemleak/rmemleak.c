/*
 * Copyright(C) 2023 Huawei Technologies Co., Ltd. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/cma.h>
#include <linux/debugfs.h>
#include <linux/delay.h>
#include <linux/version.h>
#if KERNEL_VERSION(5, 10, 0) > LINUX_VERSION_CODE
#include <linux/dma-contiguous.h>
#else
#include <linux/dma-map-ops.h>
#endif
#ifndef CONFIG_DFX_OHOS
#include <linux/ion.h>
#endif
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/radix-tree.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/zsmalloc.h>
#ifdef CONFIG_DFX_LIBLINUX
#include <linux/mman.h>
#include <liblinux/pal.h>
#endif
#include "securec.h"

#define	memcheck_err(format, ...)	\
	pr_err("MemLeak[%s %d] " format, __func__, __LINE__, ##__VA_ARGS__)
#define	memcheck_info(format, ...)	\
	pr_info("MemLeak[%s %d] " format, __func__, __LINE__, ##__VA_ARGS__)

#define MAX_BUF 16
#define FREE_FREQ 2
#define GRO_MAX_HEAD (MAX_HEADER + 128)

#define LEAK_SLUB_SIZE (PAGE_SIZE / 2)
#define LEAK_LSLUB_SIZE (4 * PAGE_SIZE)
#define LEAK_ZSMALLOC_SIZE (PAGE_SIZE / 2)
#define LEAK_SKB_SIZE ((GRO_MAX_HEAD + NET_SKB_PAD + NET_IP_ALIGN) * 2)

struct zs_pool;
struct leak_item {
	int is_on;
	char *name;
	struct task_struct *task;
	void (*func)(void);
};

static struct dentry *leak_root;

static int stop_inject_thread(struct leak_item *p);
static int create_inject_thread(struct leak_item *p);

#define DEBUGFS_LEAK_TYPE(name) \
	static void memcheck_inject_##name##_leak(void); \
	static struct leak_item leak_##name##_item = { \
		.is_on = 0, \
		.task = NULL, }; \
	static int attr_##name##_show(struct seq_file *m, void *private) \
	{ \
		if (leak_##name##_item.is_on == 1) \
			seq_printf(m, "on\n"); \
		else if (leak_##name##_item.is_on == 0) \
			seq_printf(m, "off\n"); \
		else \
			seq_printf(m, "error!\n"); \
		return 0; \
	} \
	static ssize_t attr_##name##_write(struct file *file, \
					const char __user *ubuf, \
					size_t len, loff_t *offp) \
	{ \
		char buf[MAX_BUF]; \
		size_t cnt = len; \
		if (cnt > (sizeof(buf) - 1)) \
			return (-EINVAL); \
		(void)memset_s(buf, sizeof(buf), 0, sizeof(buf)); \
		if (copy_from_user(buf, ubuf, cnt)) \
			return (-EINVAL); \
		while ((cnt > 0) && ((buf[cnt - 1] == ' ') || (buf[cnt - 1] == '\n'))) \
			cnt--; \
		buf[cnt] = '\0'; \
		if (!strncmp(buf, "on", cnt)) \
			leak_##name##_item.is_on = 1; \
		else if (!strncmp(buf, "off", cnt)) \
			leak_##name##_item.is_on = 0; \
		else \
			return (-EINVAL); \
		leak_##name##_item.func = memcheck_inject_##name##_leak; \
		if (leak_##name##_item.is_on) \
			create_inject_thread(&leak_##name##_item); \
		else \
			stop_inject_thread(&leak_##name##_item); \
		return len; \
	} \
	static int attr_##name##_open(struct inode *inode, struct file *file) \
	{ \
		return single_open(file, attr_##name##_show, inode->i_private); \
	} \
	static const struct file_operations fs_##name##_fops = { \
		.owner = THIS_MODULE, \
		.open = attr_##name##_open, \
		.read = seq_read, \
		.llseek = seq_lseek, \
		.release = single_release, \
		.write = attr_##name##_write, \
	}

#define DEBUGFS_LEAK_VALUE(name, def, min, max) \
	static size_t memcheck_leak_##name##_val = (def); \
	static int attr_##name##_val_show(struct seq_file *m, void *private) \
	{ \
		seq_printf(m, "%zu\n", memcheck_leak_##name##_val); \
		return 0; \
	} \
	static ssize_t attr_##name##_val_write(struct file *file, \
						const char __user *ubuf, \
						size_t len, loff_t *off) \
	{ \
		unsigned long temp = (def); \
		char buf[MAX_BUF]; \
		size_t cnt = len; \
		if (cnt > (sizeof(buf) - 1)) \
			return (-EINVAL); \
		(void)memset_s(buf, sizeof(buf), 0, sizeof(buf)); \
		if (copy_from_user(buf, ubuf, cnt)) \
			return (-EINVAL); \
		while ((cnt > 0) && ((buf[cnt - 1] == ' ') || (buf[cnt - 1] == '\n'))) \
			cnt--; \
		buf[cnt] = '\0'; \
		if (kstrtoul(buf, 10, &temp)) \
			return (-EINVAL); \
		if ((temp < (min)) || (temp > (max))) \
			return (-EINVAL); \
		memcheck_leak_##name##_val = temp; \
		return len; \
	} \
	static int attr_##name##_val_open(struct inode *inode, \
						struct file *file) \
	{ \
		return single_open(file, attr_##name##_val_show, inode->i_private); \
	} \
	static const struct file_operations fs_##name##_fops = { \
		.owner = THIS_MODULE, \
		.open = attr_##name##_val_open, \
		.read = seq_read, \
		.llseek = seq_lseek, \
		.release = single_release, \
		.write = attr_##name##_val_write, \
	}

DEBUGFS_LEAK_VALUE(size, 1024 * 1024, 1024, 100 * 1024 * 1024);
DEBUGFS_LEAK_VALUE(interval, 1, 1, 60);

static int inject_thread_func(void *data)
{
	struct leak_item *leak_item = NULL;

	if (!data)
		return -EINVAL;

	leak_item = (struct leak_item *)data;
	while (1) {
		set_current_state(TASK_UNINTERRUPTIBLE);
		if (kthread_should_stop())
			break;
		leak_item->func();
		mdelay(memcheck_leak_interval_val * 1000); /* 1ms = 1000us */
	}

	return 0;
}

static int create_inject_thread(struct leak_item *p)
{
	int ret = 0;

	if (!p) {
		memcheck_err("p is null\n");
		return -EINVAL;
	}
	if (p->task) {
		memcheck_err("inject thread is already running\n");
		return -EFAULT;
	}
	p->task = kthread_create(inject_thread_func, p, "memleak_inject");
	if (IS_ERR(p->task)) {
		memcheck_err("kthread_create failed\n");
		ret = PTR_ERR(p->task);
		p->task = NULL;
		return ret;
	}

	wake_up_process(p->task);

	return ret;
}

static int stop_inject_thread(struct leak_item *p)
{
	if (!p) {
		memcheck_err("is null\n");
		return -EINVAL;
	}
	if (!p->task) {
		memcheck_err("inject thread is not running\n");
		return -EFAULT;
	}
	kthread_stop(p->task);
	p->task = NULL;

	return 0;
}

static void memcheck_inject_buddy_leak(void)
{
	int i;
	struct page *p = NULL;
	int num = (memcheck_leak_size_val - 1) / PAGE_SIZE + 1;

	for (i = 0; i < num; i++) {
		p = alloc_pages(GFP_KERNEL, 0);
		if (!p) {
			memcheck_err("alloc_pages failed, i=%d, num=%d\n", i, num);
			return;
		}
	}
	memcheck_info("%s success, num=%d\n", __func__, num);
}

static void memcheck_inject_slub_leak(void)
{
	int i;
	void *p = NULL;
	int num = memcheck_leak_size_val / LEAK_SLUB_SIZE + 1;

	for (i = 0; i < num * FREE_FREQ; i++) {
		p = kmalloc(LEAK_SLUB_SIZE, GFP_KERNEL);
		if (!p)
			return;
		if (i % FREE_FREQ)
			kfree(p);
	}
	memcheck_info("%s success, size=%zu, num=%d\n", __func__,
		      memcheck_leak_size_val, num);
}

static void memcheck_inject_lslub_leak(void)
{
	int i;
	int num = memcheck_leak_size_val / LEAK_LSLUB_SIZE + 1;

	for (i = 0; i < num; i++) {
		if (!kmalloc(LEAK_LSLUB_SIZE, GFP_KERNEL))
			return;
	}
	memcheck_info("%s success, size=%zu, num=%d\n", __func__,
		      memcheck_leak_size_val, num);
}

static void memcheck_inject_vmalloc_leak(void)
{
	if (!vmalloc(memcheck_leak_size_val))
		return;

	memcheck_info("%s success, size=%zu\n", __func__,
		      memcheck_leak_size_val);
}

#ifdef CONFIG_DFX_LIBLINUX
#define HM_LEAK_SLUB_SIZE 224
static void memcheck_inject_hm_slub_leak(void)
{
	int i;
	void **vaddr_array = NULL;
	unsigned long prot = PROT_READ | PROT_WRITE;
	unsigned int flag = LIBLINUX_PAL_REMAP_CACHE;
	int num = memcheck_leak_size_val / HM_LEAK_SLUB_SIZE + 1;

	vaddr_array = (void **)vmalloc(num * sizeof(void *));
	if (vaddr_array == NULL) {
		memcheck_err("vmalloc array failed!\n");
		return;
	}

	for (i = 0; i < num; i++) {
		vaddr_array[i] = liblinux_pal_vm_prepare(0, PAGE_SIZE, prot, flag);
		if (vaddr_array[i] == NULL) {
			memcheck_err("liblinux_pal_vm_prepare failed at %d!\n", i);
			goto out;
		}
	}
	vfree(vaddr_array);
	return;
out:
	for (i = i - 1; i >= 0; i--)
		(void)liblinux_pal_vm_unmap(vaddr_array[i]);
	vfree(vaddr_array);
}
#endif

static void memcheck_inject_skb_leak(void)
{
	int i;
	/* about 710B per skb */
	int num = memcheck_leak_size_val / LEAK_SKB_SIZE + 1;

	for (i = 0; i < num; i++) {
		struct napi_struct napi;
		struct sk_buff *skbuf = NULL;

		(void)memset_s(&napi, sizeof(napi), 0, sizeof(napi));
		skbuf = napi_get_frags(&napi);
		if (!skbuf) {
			memcheck_err("napi_get_frags failed, i=%d, num=%d\n",
				     i, num);
			return;
		}
	}
	memcheck_info("%s success, num=%d\n", __func__, num);
}

/*
 * This is a samle function usage flow
 * 1、zs_create_pool 2、zs_malloc 3、zs_map_object 4、zs_unmap_object
zs_unmap_objectzs_destroy_pool
 */
static void memcheck_inject_zsmalloc_leak(void)
{
	int i;
	struct zs_pool *pool = NULL;
	int num = memcheck_leak_size_val / LEAK_ZSMALLOC_SIZE + 1;

	pool = zs_create_pool("inject_zsmalloc_pool");
	if (!pool) {
		memcheck_err("zs_create_pool failed\n");
		return;
	}
	for (i = 0; i < num; i++) {
		unsigned long handle = zs_malloc(pool, LEAK_ZSMALLOC_SIZE,
						 GFP_NOIO | __GFP_HIGHMEM);
		if (!handle) {
			memcheck_err("zs_malloc failed, i=%d, num=%d\n", i, num);
			return;
		}
	}
	memcheck_info("%s success, num=%d\n", __func__, num);
}

static struct radix_tree_root radix_root;
static size_t g_index = 1;
static void memcheck_inject_radix_leak(void)
{
	int i;
	int ret;
	int num = memcheck_leak_size_val / sizeof(struct radix_tree_node) + 1;

	for (i = 0; i < num; i++) {
		g_index += 2; /* node increase 2 */
		ret = radix_tree_insert(&radix_root, g_index, (void *)g_index);
		if (ret) {
			memcheck_err("alloc radix_tree_node failed, i=%zu\n",
				     g_index);
			return;
		}
	}
	memcheck_info("%s success, size=%zu, num=%d\n", __func__,
		      memcheck_leak_size_val, num);
}

static void memcheck_inject_cma_leak(void)
{
	int i;
	struct page *page = NULL;
	int num = memcheck_leak_size_val / PAGE_SIZE + 1;

	for (i = 0; i < num; i++) {
		page  = cma_alloc(dev_get_cma_area(NULL), 1, 0, false);
		if (!page) {
			memcheck_err("alloc default cma failed, i=%d, num=%d\n",
				     i, num);
			return;
		}
	}
	memcheck_info("%s success, size=%zu, num=%d\n", __func__,
		      memcheck_leak_size_val, num);
}

DEBUGFS_LEAK_TYPE(buddy);
DEBUGFS_LEAK_TYPE(slub);
DEBUGFS_LEAK_TYPE(lslub);
DEBUGFS_LEAK_TYPE(vmalloc);
#ifdef CONFIG_DFX_LIBLINUX
DEBUGFS_LEAK_TYPE(hm_slub);
#endif
DEBUGFS_LEAK_TYPE(skb);
DEBUGFS_LEAK_TYPE(zsmalloc);
DEBUGFS_LEAK_TYPE(radix);
DEBUGFS_LEAK_TYPE(cma);

static int __init memcheck_init(void)
{
	leak_root = debugfs_create_dir("memcheck_inject", NULL);

	if (!leak_root)
		return -ENXIO;

	debugfs_create_file("buddy", 0444, leak_root, NULL, &fs_buddy_fops);
#ifdef CONFIG_DFX_LIBLINUX
	debugfs_create_file("ldk_slub", 0444, leak_root, NULL, &fs_slub_fops);
	debugfs_create_file("ldk_lslub", 0444, leak_root, NULL, &fs_lslub_fops);
	debugfs_create_file("ldk_vmalloc", 0444, leak_root, NULL, &fs_vmalloc_fops);
	debugfs_create_file("slub", 0444, leak_root, NULL, &fs_hm_slub_fops);
#else /* CONFIG_DFX_LIBLINUX */
	debugfs_create_file("slub", 0444, leak_root, NULL, &fs_slub_fops);
	debugfs_create_file("lslub", 0444, leak_root, NULL, &fs_lslub_fops);
	debugfs_create_file("vmalloc", 0444, leak_root, NULL, &fs_vmalloc_fops);
#endif /* CONFIG_DFX_LIBLINUX */
	debugfs_create_file("skb", 0444, leak_root, NULL, &fs_skb_fops);
	debugfs_create_file("zsmalloc", 0444, leak_root, NULL, &fs_zsmalloc_fops);
	debugfs_create_file("radix_tree_node", 0444, leak_root, NULL,
			    &fs_radix_fops);
	debugfs_create_file("cma", 0444, leak_root, NULL, &fs_cma_fops);

	debugfs_create_file("size", 0444, leak_root, NULL, &fs_size_fops);
	debugfs_create_file("interval", 0444, leak_root, NULL, &fs_interval_fops);

	INIT_RADIX_TREE(&radix_root, GFP_ATOMIC);
	return 0;
}

static void __exit memcheck_exit(void)
{
	debugfs_remove(leak_root);
}

module_init(memcheck_init);
module_exit(memcheck_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Huawei Memory Leak Inject debugfs");
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
#if KERNEL_VERSION(5, 4, 0) <= LINUX_VERSION_CODE
MODULE_IMPORT_NS(VFS_internal_I_am_really_a_filesystem_and_am_NOT_a_driver);
#endif
