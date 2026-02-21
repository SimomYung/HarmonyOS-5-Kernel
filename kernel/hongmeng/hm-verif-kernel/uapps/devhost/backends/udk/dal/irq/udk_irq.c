/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: This is the implementation of udk_irq related interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jul 05 09:05:36 2019
 */
#include <udk/irq.h>
#include <udk/log.h>
#include <udk/kvic.h>
#include <udk/errno.h>
#include <udk/iolib.h>
#include <udk/mutex.h>
#include <udk/securec.h>
#include <udk/irq_ext.h>
#include <udk/lib/idr.h>
#include <internal/irq.h>
#include <internal/init.h>

#include <devhost/interrupt.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libstrict/strict.h>

#define GIC_IRQ_CELLS		3U
#define ITS_IRQ_CELLS		4U
#define ITS_DEVID_SHIFT		11U
#define ITS_IRQ_START		8192U

struct kirq_entry {
	unsigned int kirq;
	const void *owner;
};

struct kirq_manager {
	struct idr kirq_idr;
	struct udk_mutex lock;
};

static struct kirq_manager kirq_mgr;

unsigned int udk_irq_create_mapping(unsigned int oirq_offset,
				    const unsigned int *args,
				    unsigned int args_count)
{
	return devhost_create_irq_mapping(oirq_offset, args, args_count);
}

static inline void kirq_mgr_lock(void)
{
	udk_mutex_lock(&kirq_mgr.lock);
}

static inline void kirq_mgr_unlock(void)
{
	udk_mutex_unlock(&kirq_mgr.lock);
}

static unsigned int kirq_id_make(unsigned int irq_cells,
				 const unsigned int *irq_args)
{
	/* This id-generate algorithm is same as the devmgr's */
	if (irq_cells == GIC_IRQ_CELLS) {
		return irq_args[0];
	} else if (irq_cells == ITS_IRQ_CELLS) {
		return (irq_args[0] << ITS_DEVID_SHIFT) + irq_args[1] + ITS_IRQ_START;
	}

	return 0;
}

static inline struct kirq_entry *kirq_entry_lookup(unsigned int kirq_id)
{
	return (struct kirq_entry *)idr_find(&kirq_mgr.kirq_idr, kirq_id);
}

static struct kirq_entry *kirq_entry_create(const void *owner, unsigned int kirq_id)
{
	struct kirq_entry *entry = NULL;
	int ret;

	entry = udk_malloc(sizeof(*entry));
	if (entry == NULL) {
		udk_warn("malloc kirq_entry failed\n");
		return NULL;
	}
	mem_zero_s(*entry);
	entry->owner = owner;
	ret = idr_set(&kirq_mgr.kirq_idr, entry, kirq_id);
	if (ret < 0) {
		udk_warn("kirq idr set kirq_id=%u failed\n", kirq_id);
		udk_free(entry);
		return NULL;
	}

	return entry;
}

static void kirq_entry_destroy(unsigned int kirq_id)
{
	struct kirq_entry *entry = NULL;

	entry = (struct kirq_entry *)idr_remove(&kirq_mgr.kirq_idr, kirq_id);
	if (entry != NULL) {
		udk_free(entry);
	}
}

int udk_irq_kirq_create(const void *owner, unsigned int irqctrl_id,
			const unsigned int *irq_args, unsigned int irq_cells)
{
	struct kirq_entry *entry = NULL;
	unsigned int kirq_id;
	unsigned int kirq;

	kirq_id = kirq_id_make(irq_cells, irq_args);
	if (kirq_id == 0) {
		udk_warn("kirq id make failed\n");
		return -EINVAL;
	}
	kirq_mgr_lock();
	entry = kirq_entry_lookup(kirq_id);
	if (entry != NULL) {
		if (entry->owner != owner) {
			udk_warn("irq has been mapped\n");
			kirq_mgr_unlock();
			return -EFAULT;
		}
		kirq = entry->kirq;
		kirq_mgr_unlock();
		return (int)kirq;
	}
	entry = kirq_entry_create(owner, kirq_id);
	if (entry == NULL) {
		udk_warn("kirq entry create failed\n");
		kirq_mgr_unlock();
		return -ENOMEM;
	}
	kirq = udk_irq_create_mapping(irqctrl_id, irq_args, irq_cells);
	if (kirq == 0) {
		udk_warn("irq create mapping failed\n");
		kirq_entry_destroy(kirq_id);
		kirq_mgr_unlock();
		return -EFAULT;
	}
	entry->kirq = kirq;
	kirq_mgr_unlock();

	return (int)kirq;
}

#ifdef CONFIG_UDK_IRQ_EXT

static int request_kvic_irq(unsigned int irq, irq_handler_t handler,
			    const char *devname, void *ctx)
{
	int err;

	err = devhost_request_threaded_irq(irq, handler,
					   1, devname, ctx);
	if (err < 0) {
		return -hmerrno2posix(err);
	}
	devhost_enable_irq(irq);

	return 0;
}

int udk_request_irq(unsigned int irq, irq_handler_t handler,
		    unsigned long irqflags, const char *devname,
		    void *ctx)
{
	int err;

	if (handler == NULL || irq == 0) {
		return -EINVAL;
	}
	if (is_kvic_irq(irq) == 1) {
		err = request_kvic_irq(irq, handler, devname, ctx);
	} else {
		err = udk_irq_ext_uirq_request(irq, handler, irqflags,
					       devname, ctx);
	}

	return err;
}

void udk_free_irq(unsigned int irq)
{
	if (is_kvic_irq(irq) == 1) {
		devhost_free_irq(irq);
	} else {
		udk_irq_ext_uirq_unrequest(irq);
	}
}

void udk_enable_irq(unsigned int irq)
{
	if (is_kvic_irq(irq) == 1) {
		devhost_enable_irq(irq);
	} else {
		udk_irq_ext_uirq_enable(irq);
	}
}

void udk_disable_irq(unsigned int irq)
{
	if (is_kvic_irq(irq) == 1) {
		devhost_disable_irq(irq);
	} else {
		udk_irq_ext_uirq_disable(irq);
	}
}

int udk_irq_setaffinity(unsigned int irq, unsigned long affinity)
{
	UNUSED(irq, affinity);
	udk_info("not support udk_irq_setaffinity when UDK_IRQ_EXT enabled\n");

	return -ENOSYS;
}

#else

int udk_request_irq(unsigned int irq, irq_handler_t handler,
		    unsigned long irqflags, const char *devname,
		    void *ctx)
{
	int err;
	UNUSED(irqflags);

	if (handler == NULL) {
		return -EINVAL;
	}
	err = devhost_request_threaded_irq(irq, handler,
					   is_kvic_irq(irq),
					   devname, ctx);
	if (err < 0) {
		return -hmerrno2posix(err);
	}
	devhost_enable_irq(irq);

	return 0;
}

void udk_free_irq(unsigned int irq)
{
	devhost_free_irq(irq);
}

void udk_enable_irq(unsigned int irq)
{
	devhost_enable_irq(irq);
}

void udk_disable_irq(unsigned int irq)
{
	devhost_disable_irq(irq);
}

int udk_irq_setaffinity(unsigned int irq, unsigned long affinity)
{
	return -hmerrno2posix(devhost_irq_setaffinity(irq, affinity));
}

#endif /* CONFIG_UDK_IRQ_EXT */

void udk_local_irq_disable(void)
{
	devhost_disable_local_irq();
}

void udk_local_irq_enable(void)
{
	devhost_enable_local_irq();
}

void udk_local_irq_restore(unsigned long flags)
{
	devhost_restore_local_irq(flags);
}

unsigned long udk_local_irq_save(void)
{
	return devhost_save_local_irq();
}

unsigned long udk_local_save_flags(void)
{
	return devhost_save_local_flags();
}

static int udk_irq_init(void)
{
	idr_init(&kirq_mgr.kirq_idr);
	udk_mutex_init(&kirq_mgr.lock);

	return 0;
}
udk_init_call(udk_irq_init);
