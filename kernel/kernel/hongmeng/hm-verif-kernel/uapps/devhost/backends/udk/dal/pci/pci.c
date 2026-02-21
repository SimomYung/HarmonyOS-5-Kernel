/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: UDK PCI framework
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 22 17:22:14 2021
 */

#include <udk/pci.h>

#include <udk/log.h>
#include <udk/errno.h>
#include <udk/iolib.h>
#include <udk/bitops.h>
#include <udk/securec.h>
#include <udk/lib/idr.h>
#include <udk/lib/dlist.h>
#include <udk/lib/stack.h>
#include <hongmeng/errno.h>

#include "pci_memory.h"

#define PCI_BUS_ID_MAX		0xffU
#define PCI_DEV_ID_MAX		0x20U

/* common */
#define PCI_VENDOR_ID		0x00U
#define PCI_DEVICE_ID		0x02U
#define PCI_STATUS		0x06U
#define PCI_HEADER_TYPE		0x0eU
#define PCI_CAP_LIST		0x34U
#define PCI_BASE_ADDRESS_0	0x10U
/* header type 1 */
#define PCI_PRIMARY_BUS		0x18U
#define PCI_IO_BASE		0x1cU
#define PCI_MEM_BASE		0x20U
#define PCI_IO_BASE_UPPER	0x30U

#define PCI_STATUS_CAP_LIST	0x10U

#define PCI_CONFIG_HEADER_SIZE	0x40U
#define PCI_CAP_MAX		48

#define PCI_HEADER_TYPE_NORMAL	0x0U
#define PCI_HEADER_TYPE_BRIDGE	0x1U

#define VENDOR_ID_MIN		0x0U
#define VENDOR_ID_MAX		0xffffU
#define DEVICE_ID_MIN		0x0U
#define DEVICE_ID_MAX		0xffffU

#define BUS_NUM_MASK		0xffU
#define PRIMARY_BUS_SHIFT	0x0U
#define SECONDARY_BUS_SHIFT	0x8U
#define SUBORDINATE_BUS_SHIFT	0x10U

#define PCI_CAP_ID_EXP		0x10U

#define EXP_CAP_FLAGS		0x2U
#define EXP_CAP_TYPE_MASK	0xfU
#define EXP_CAP_TYPE_SHIFT	0x4U
#define EXP_CAP_TYPE_ROOT_PORT	0x4U
#define EXP_CAP_TYPE_UPSTREAM	0x5U
#define EXP_CAP_TYPE_DOWNSTREAM	0x6U
#define EXP_CAP_TYPE_BRIDGE	0x8U

#define BRIDGE_BAR_NR		0x2U
#define DEVICE_BAR_NR		0x6U
#define PCI_BAR_WIDTH		0x20U
#define PCI_BAR_SIZE		0x4U
#define PCI_BAR_INIT		0xffffffff

#define BAR_MEM_MASK		(~0xf)
#define BAR_IO_MASK		(~0x3)

#define BAR_SPACE_MASK		0x1
#define BAR_TYPE_MASK		0x6
#define BAR_PREFETCHABLE_MASK	0x8

#define BRIDGE_IO_ALIGN_MIN	0x1000U
#define BRIDGE_MEM_ALIGN_MIN	0x100000U

struct pci_domain_manager {
	struct idr domain_idr;
};

struct pci_domain {
	unsigned int id;
	unsigned int bus_index;
	struct udk_pci_host *host;
	struct dlist_node bridge_list;
};

struct pci_cap {
	uint8_t id;
	uint8_t offset;
	struct dlist_node node;
};

struct pci_mem_region {
	uint32_t flags;
	uint64_t size;
	uint64_t align;
	uint64_t cpu_addr;
	uint64_t pci_addr;
	struct udk_pci_device *owner;
};

struct pci_mem_region_node {
	struct pci_mem_region *mem;
	struct dlist_node node;
};

struct pci_bar {
	unsigned int index;
	struct pci_mem_region mem;
	struct dlist_node node;
};

struct udk_pci_device {
	uint16_t vendor_id;
	uint16_t device_id;
	uint16_t exp_flag;
	uint8_t type;
	struct udk_pci_slot slot;
	struct pci_bus *bus;
	struct dlist_node cap_list;
	struct dlist_node bar_list;
	struct dlist_node node;
};

struct pci_bus {
	struct pci_domain *domain;
	struct udk_pci_ctx ctx;
	unsigned char dev_nr;
	struct dlist_node devices;
};

struct pci_bridge {
	unsigned char primary;
	unsigned char secondary;
	unsigned char subordinate;
	struct pci_bus bus;
	struct pci_bridge *parent;
	struct udk_pci_device *self;
	struct pci_mem_region child_mem[(int)UDK_PCI_MEM_TYPE_NR];
	struct dlist_node node;
};

static struct pci_domain_manager pci_domains = {
	.domain_idr = IDR_INIT(domain_idr),
};

#define PCI_CONFIG_READ_DEFINE(size, type)				\
static int pci_config_read##size(const struct pci_bus *bus,		\
				 const struct udk_pci_slot *slot,	\
				 unsigned int offset, type *val)	\
{									\
	const struct udk_pci_ctx *ctx = &bus->ctx;			\
	unsigned int len = sizeof(type);				\
	unsigned int data = 0;						\
	int ret;							\
	if (val == NULL) {						\
		return -EINVAL;						\
	}								\
	ret = ctx->host->ops.read(ctx, slot, offset, len, &data);	\
	*val = (type)data;						\
	return ret;							\
}

#define PCI_CONFIG_WRITE_DEFINE(size, type)				\
static int pci_config_write##size(const struct pci_bus *bus,		\
				  const struct udk_pci_slot *slot,	\
				  unsigned int offset, type val)	\
{									\
	const struct udk_pci_ctx *ctx = &bus->ctx;			\
	unsigned int len = sizeof(type);				\
	unsigned int data = (unsigned int)val;				\
	int ret;							\
	ret = ctx->host->ops.write(ctx, slot, offset, len, data);	\
	return ret;							\
}

PCI_CONFIG_READ_DEFINE(8, uint8_t)
PCI_CONFIG_READ_DEFINE(16, uint16_t)
PCI_CONFIG_READ_DEFINE(32, uint32_t)
PCI_CONFIG_WRITE_DEFINE(16, uint16_t)
PCI_CONFIG_WRITE_DEFINE(32, uint32_t)

#define BRIDGE_MEM_RANGE_DEFINE(size, type)				\
static inline type bridge_mem_range##size(uint64_t start, uint64_t end)	\
{									\
	type base, limit, ret;						\
	unsigned int shift = (unsigned int)((size) / 2U);		\
	unsigned int mask = UDK_MAKE_MASK(shift);			\
	base = (type)udk_get_bits(start, mask, shift);			\
	limit = (type)udk_get_bits(end, mask, shift);			\
	ret = (type)(UDK_BIT_LS(limit, shift) | base);			\
	return ret;							\
}									\

BRIDGE_MEM_RANGE_DEFINE(16, uint16_t)
BRIDGE_MEM_RANGE_DEFINE(32, uint32_t)

static inline unsigned int pci_bar_off(unsigned int index)
{
	return PCI_BASE_ADDRESS_0 + (index * PCI_BAR_SIZE);
}

static inline int pci_device_read_bar(const struct udk_pci_device *dev,
				      unsigned int index, uint32_t *bar)
{
	return pci_config_read32(dev->bus, &dev->slot, pci_bar_off(index), bar);
}

static inline int pci_device_write_bar(const struct udk_pci_device *dev,
				       unsigned int index, uint32_t bar)
{
	return pci_config_write32(dev->bus, &dev->slot, pci_bar_off(index), bar);
}

static inline struct pci_bridge *pci_bridge_of(const struct pci_bus *bus)
{
	return container_of(bus, struct pci_bridge, bus);
}

static void pci_bridge_child_mem_init(struct pci_bridge *bridge)
{
	unsigned int type;

	for (type = 0; type < (unsigned int)UDK_PCI_MEM_TYPE_NR; type++) {
		bridge->child_mem[type].owner = bridge->self;
		bridge->child_mem[type].flags |= mem_type_to_flag(type);
		bridge->child_mem[type].flags |= MEM_FLAG_CHILD_MEM;
		if (type == (unsigned int)UDK_PCI_MEM_IO) {
			bridge->child_mem[type].align = BRIDGE_IO_ALIGN_MIN;
		} else {
			bridge->child_mem[type].align = BRIDGE_MEM_ALIGN_MIN;
		}
	}
}

static int pci_add_bridge(struct pci_domain *domain, struct pci_bridge *parent,
			  struct udk_pci_device *device, struct udk_stack *stack)
{
	int err;
	unsigned char bus_id = (unsigned char)domain->bus_index;
	struct pci_bridge *bridge = NULL;

	if (bus_id >= PCI_BUS_ID_MAX) {
		udk_error("pci bus number exceeds upper limit\n");
		return -EINVAL;
	}
	bridge = (struct pci_bridge *)udk_malloc(sizeof(*bridge));
	if (bridge == NULL) {
		udk_error("pci bridge malloc failed\n");
		return -ENOMEM;
	}
	mem_zero_s(*bridge);
	bridge->parent = parent;
	bridge->self = device;
	bridge->bus.ctx.bus_id = bus_id;
	bridge->bus.ctx.host = domain->host;
	bridge->bus.domain = domain;
	dlist_init(&bridge->bus.devices);
	pci_bridge_child_mem_init(bridge);
	domain->bus_index++;
	if (bridge->parent != NULL) {
		bridge->primary = parent->bus.ctx.bus_id;
		bridge->secondary = bus_id;
		bridge->subordinate = PCI_BUS_ID_MAX;
	}
	err = udk_stack_push(stack, bridge);
	if (err < 0) {
		udk_error("bridge stack push failed: %s\n", strerror(-err));
		udk_free(bridge);
		return err;
	}
	dlist_insert(&domain->bridge_list, &bridge->node);

	return 0;
}

static int pci_check_device(const struct pci_bus *bus, const struct udk_pci_slot *slot,
			    uint16_t *vendor_id, uint16_t *device_id)
{
	int err;
	uint16_t vid, did;

	err = pci_config_read16(bus, slot, PCI_VENDOR_ID, &vid);
	if (err < 0) {
		return err;
	}
	err = pci_config_read16(bus, slot, PCI_DEVICE_ID, &did);
	if (err < 0) {
		return err;
	}
	if (vid == VENDOR_ID_MIN || vid == VENDOR_ID_MAX ||
	    did == DEVICE_ID_MIN || did == DEVICE_ID_MAX) {
		return -EINVAL;
	}
	*vendor_id = vid;
	*device_id = did;

	return 0;
}

static struct pci_cap *
pci_device_cap_find(const struct udk_pci_device *device, uint8_t id)
{
	struct pci_cap *pos = NULL;

	dlist_for_each_entry(pos, &device->cap_list,
			     struct pci_cap, node) {
		if (pos->id == id) {
			return pos;
		}
	}

	return NULL;
}

static inline bool is_pcie_device(const struct udk_pci_device *device)
{
	struct pci_cap *exp_cap = NULL;

	exp_cap = pci_device_cap_find(device, PCI_CAP_ID_EXP);
	if (exp_cap == NULL) {
		return false;
	}

	return true;
}

static inline uint16_t exp_cap_type_of(uint16_t exp_flag)
{
	return (uint16_t)udk_get_bits(exp_flag, EXP_CAP_TYPE_MASK, EXP_CAP_TYPE_SHIFT);
}

static inline void exp_cap_type_set(struct udk_pci_device *device, uint16_t type)
{
	uint16_t flag = device->exp_flag;

	flag = udk_set_bits(flag, type, EXP_CAP_TYPE_MASK, EXP_CAP_TYPE_SHIFT);
	device->exp_flag = flag;
}

static inline bool is_downstream_port(uint16_t exp_flag)
{
	uint16_t type = exp_cap_type_of(exp_flag);

	return type == EXP_CAP_TYPE_ROOT_PORT ||
	       type == EXP_CAP_TYPE_DOWNSTREAM ||
	       type == EXP_CAP_TYPE_BRIDGE;
}

static inline bool is_upstream_port(uint16_t exp_flag)
{
	uint16_t type = exp_cap_type_of(exp_flag);

	return type == EXP_CAP_TYPE_UPSTREAM;
}

static int pcie_port_type_init(struct udk_pci_device *device,
			       const struct pci_cap *exp_cap)
{
	struct udk_pci_slot *slot = &device->slot;
	struct pci_bus *bus = device->bus;
	struct pci_bridge *parent = NULL;
	uint16_t exp_flag, type;
	int err;

	err = pci_config_read16(bus, slot, exp_cap->offset + EXP_CAP_FLAGS,
				&exp_flag);
	if (err < 0) {
		return err;
	}
	device->exp_flag = exp_flag;
	parent = pci_bridge_of(bus);
	if (parent->self == NULL) {
		return 0;
	}
	type = exp_cap_type_of(exp_flag);
	/*
	 * Since some pcie bridge devices can not decide its own port type
	 * correctly, the pci subsystem needs to check and correct their
	 * port types according to the rules:
	 *	1. The downstream port's children must be upstream ports.
	 *	2. The upstream port's children must be downstream ports.
	 */
	if (type == EXP_CAP_TYPE_DOWNSTREAM) {
		if (is_downstream_port(parent->self->exp_flag)) {
			exp_cap_type_set(device, EXP_CAP_TYPE_UPSTREAM);
		}
	} else if (type == EXP_CAP_TYPE_UPSTREAM) {
		if (is_upstream_port(parent->self->exp_flag)) {
			exp_cap_type_set(device, EXP_CAP_TYPE_DOWNSTREAM);
		}
	}

	return 0;
}

static void pci_device_caps_cleanup(const struct udk_pci_device *device)
{
	struct pci_cap *pos = NULL;
	struct pci_cap *next = NULL;

	dlist_for_each_entry_safe(pos, next, &device->cap_list,
				  struct pci_cap, node) {
		dlist_delete(&pos->node);
		udk_free(pos);
	}
}

static int pci_device_cap_create(struct udk_pci_device *device, uint8_t id,
				 uint8_t cap_off)
{
	struct pci_cap *cap = NULL;

	cap = (struct pci_cap *)udk_malloc(sizeof(*cap));
	if (cap == NULL) {
		udk_error("pci cap malloc failed\n");
		return -ENOMEM;
	}
	mem_zero_s(*cap);
	cap->id = id;
	cap->offset = cap_off;
	dlist_insert_tail(&device->cap_list, &cap->node);

	return 0;
}

static int pci_device_caps_probe(struct udk_pci_device *device)
{
	uint16_t status;
	int err, cap_count;
	uint8_t id, offset;
	struct pci_bus *bus = device->bus;
	struct udk_pci_slot *slot = &device->slot;

	err = pci_config_read16(bus, slot, PCI_STATUS, &status);
	if (err < 0) {
		return err;
	}
	if ((status & PCI_STATUS_CAP_LIST) == 0) {
		return 0;
	}
	err = pci_config_read8(bus, slot, PCI_CAP_LIST, &offset);
	if (err < 0) {
		return err;
	}
	cap_count = PCI_CAP_MAX;
	while ((--cap_count > 0) && (offset >= PCI_CONFIG_HEADER_SIZE)) {
		err = pci_config_read8(bus, slot, offset, &id);
		if (err < 0) {
			return err;
		}
		if (id == 0) {
			/* Since 0 is not a valid id, terminate the traverse. */
			break;
		}
		err = pci_device_cap_create(device, id, offset);
		if (err < 0) {
			udk_error("cap create failed\n");
			return err;
		}
		err = pci_config_read8(bus, slot, offset + 1, &offset);
		if (err < 0) {
			return err;
		}
	}

	return 0;
}

static int pci_bridge_init(struct udk_pci_device *device, struct udk_stack *stack)
{
	int err;
	struct pci_cap *exp_cap = NULL;
	struct pci_bus *bus = device->bus;
	struct pci_bridge *parent = pci_bridge_of(bus);

	exp_cap = pci_device_cap_find(device, PCI_CAP_ID_EXP);
	if (exp_cap != NULL) {
		err = pcie_port_type_init(device, exp_cap);
		if (err < 0) {
			udk_error("pcie port type init failed: %s\n", strerror(-err));
			return err;
		}
	}
	err = pci_add_bridge(bus->domain, parent, device, stack);
	if (err < 0) {
		udk_error("pci bridge init failed: %s\n", strerror(-err));
		return err;
	}

	return 0;
}

static void pci_device_bars_cleanup(const struct udk_pci_device *device)
{
	struct pci_bar *pos = NULL;
	struct pci_bar *next = NULL;

	dlist_for_each_entry_safe(pos, next, &device->bar_list,
				  struct pci_bar, node) {
		dlist_delete(&pos->node);
		udk_free(pos);
	}
}

static int do_bar_init(const struct udk_pci_device *device, unsigned int index,
		       uint32_t *psize)
{
	int err;
	uint32_t sz;

	err = pci_device_write_bar(device, index, PCI_BAR_INIT);
	if (err < 0) {
		return err;
	}
	err = pci_device_read_bar(device, index, &sz);
	if (err < 0) {
		return err;
	}
	*psize = sz;

	return 0;
}

static unsigned int bar_decode(unsigned int bar)
{
	unsigned int flags = 0U;

	if ((bar & BAR_SPACE_MASK) != 0U) {
		flags |= BAR_SPACE_IO;
	} else {
		if ((bar & BAR_TYPE_MASK) == BAR_TYPE_32BIT) {
			flags |= BAR_TYPE_32BIT;
		} else {
			flags |= BAR_TYPE_64BIT;
		}
		if ((bar & BAR_PREFETCHABLE_MASK) != 0U) {
			flags |= BAR_PREFETCHABLE;
		}
	}

	return flags;
}

static void parent_mem_update(struct udk_pci_device *device,
			      const struct pci_mem_region *mem)
{
	unsigned int type;
	struct pci_bus *bus = device->bus;
	struct pci_bridge *parent = pci_bridge_of(bus);
	struct pci_mem_region *mem_sum = NULL;

	type = (unsigned int)mem_flag_to_type(mem->flags);
	mem_sum = &parent->child_mem[type];
	mem_sum->size += mem->size;
	if (mem_sum->align < mem->align) {
		mem_sum->align = mem->align;
	}
}

static int bar_insert(struct udk_pci_device *device, unsigned int index,
		      uint64_t size, unsigned int flags)
{
	struct pci_bar *bar = NULL;

	bar = udk_malloc(sizeof(*bar));
	if (bar == NULL) {
		return -ENOMEM;
	}
	mem_zero_s(*bar);
	bar->index = index;
	bar->mem.flags = flags;
	bar->mem.owner = device;
	bar->mem.size = size;
	bar->mem.align = size;
	dlist_insert(&device->bar_list, &bar->node);
	parent_mem_update(device, &bar->mem);

	return 0;
}

static int bar_init(struct udk_pci_device *device, unsigned int index)
{
	unsigned int flags;
	uint32_t bar, size;
	uint64_t sz64;
	int err;

	err = pci_device_read_bar(device, index, &bar);
	if (err < 0) {
		return err;
	}
	err = do_bar_init(device, index, &size);
	if (err < 0) {
		return err;
	}
	if (size == PCI_BAR_INIT || size == 0U) {
		return 0;
	}
	flags = bar_decode(bar);
	if ((flags & BAR_SPACE_IO) == 0U) {
		size &= BAR_MEM_MASK;
	} else {
		size &= BAR_IO_MASK;
	}
	if ((flags & BAR_TYPE_64BIT) != 0U) {
		sz64 = (uint64_t)size;
		err = do_bar_init(device, index + 1, &size);
		if (err < 0) {
			return err;
		}
		sz64 |= ((uint64_t)size << PCI_BAR_WIDTH);
		sz64 = ~sz64 + 1;
	} else {
		sz64 = (uint64_t) ((~size) + 1);
	}
	err = bar_insert(device, index, sz64, flags);
	if (err < 0) {
		return err;
	}

	return (flags & BAR_TYPE_64BIT) != 0U ? 1 : 0;
}

static int pci_device_bars_init(struct udk_pci_device *device)
{
	int err;
	unsigned int bar_nr;
	unsigned int i = 0;
	uint8_t type = device->type;

	switch (type) {
	case PCI_HEADER_TYPE_NORMAL:
		bar_nr = DEVICE_BAR_NR;
		break;
	case PCI_HEADER_TYPE_BRIDGE:
		bar_nr = BRIDGE_BAR_NR;
		break;
	default:
		return 0;
	}
	while (i < bar_nr) {
		err = bar_init(device, i);
		if (err < 0) {
			return err;
		} else if (err == 1) {
			i += 2U;
		} else {
			i++;
		}
	}

	return 0;
}

static int pci_device_setup(struct udk_pci_device *device, struct udk_stack *stack)
{
	int err;
	uint8_t type = device->type;

	err = pci_device_caps_probe(device);
	if (err < 0) {
		udk_error("pci probe caps failed: %s\n", strerror(-err));
		return err;
	}
	err = pci_device_bars_init(device);
	if (err < 0) {
		udk_error("pci init bars failed: %s\n", strerror(-err));
		return err;
	}
	switch (type) {
	case PCI_HEADER_TYPE_NORMAL:
		break;
	case PCI_HEADER_TYPE_BRIDGE:
		err = pci_bridge_init(device, stack);
		if (err < 0) {
			return err;
		}
		break;
	default:
		udk_error("unknown pci device type: %u\n", (unsigned int)type);
		return -EINVAL;
	}

	return 0;
}

static void pci_device_destroy(struct udk_pci_device *device)
{
	pci_device_caps_cleanup(device);
	pci_device_bars_cleanup(device);
	udk_free(device);
}

static struct udk_pci_device *
pci_device_create(struct pci_bus *bus, const struct udk_pci_slot *slot,
		  uint16_t vendor_id, uint16_t device_id)
{
	struct udk_pci_device *device = NULL;
	uint8_t type;
	int err;

	err = pci_config_read8(bus, slot, PCI_HEADER_TYPE, &type);
	if (err < 0) {
		return NULL;
	}
	device = (struct udk_pci_device *)udk_malloc(sizeof(*device));
	if (device == NULL) {
		udk_error("pci device malloc failed\n");
		return NULL;
	}
	mem_zero_s(*device);
	device->vendor_id = vendor_id;
	device->device_id = device_id;
	device->type = type;
	device->slot = *slot;
	device->bus = bus;
	dlist_init(&device->cap_list);
	dlist_init(&device->bar_list);
	udk_info("[%04X:%02X:%02X.%01X] %04X:%04X\n", bus->domain->id,
		 (unsigned int)bus->ctx.bus_id, (unsigned int)slot->id,
		 (unsigned int)slot->func, (unsigned int)vendor_id,
		 (unsigned int)device_id);

	return device;
}

static int pci_bus_scan_device(struct pci_bus *bus, const struct udk_pci_slot *slot,
			       struct udk_stack *stack)
{
	int err;
	uint16_t vendor_id, device_id;
	struct udk_pci_device *device = NULL;

	err = pci_check_device(bus, slot, &vendor_id, &device_id);
	if (err < 0) {
		udk_debug("pci [%04x:%02x:%02x] no device\n",
			  bus->domain->id, (unsigned int)bus->ctx.bus_id,
			  (unsigned int)slot->id);
		return 0;
	}
	device = pci_device_create(bus, slot, vendor_id, device_id);
	if (device == NULL) {
		udk_error("pci device create failed\n");
		return -ENOMEM;
	}
	err = pci_device_setup(device, stack);
	if (err < 0) {
		udk_error("pci device setup failed: %s\n", strerror(-err));
		pci_device_destroy(device);
		return err;
	}
	dlist_insert(&bus->devices, &device->node);

	return 0;
}

static bool is_single_device(struct pci_bridge *bridge)
{
	struct udk_pci_device *device = bridge->self;

	if (device == NULL) {
		return false;
	}
	if (is_pcie_device(device) && is_downstream_port(device->exp_flag)) {
		return true;
	}

	return false;
}

static int pci_bridge_route_config(struct pci_bridge *bridge)
{
	int err;
	uint32_t val;
	unsigned char max_bus;
	struct pci_bus *dev_bus = NULL;
	struct pci_bus *bus = &bridge->bus;
	struct udk_pci_device *device = bridge->self;

	if (device == NULL) {
		return 0;
	}
	dev_bus = device->bus;
	max_bus = (unsigned char)(bus->domain->bus_index - 1);
	if (bridge->secondary < max_bus) {
		bridge->subordinate = max_bus;
	}
	err = pci_config_read32(dev_bus, &device->slot, PCI_PRIMARY_BUS, &val);
	if (err < 0) {
		return err;
	}
	val = udk_set_bits(val, bridge->subordinate, BUS_NUM_MASK, SUBORDINATE_BUS_SHIFT);
	val = udk_set_bits(val, bridge->secondary, BUS_NUM_MASK, SECONDARY_BUS_SHIFT);
	val = udk_set_bits(val, bridge->primary, BUS_NUM_MASK, PRIMARY_BUS_SHIFT);
	err = pci_config_write32(dev_bus, &device->slot, PCI_PRIMARY_BUS, val);
	if (err < 0) {
		return err;
	}

	return 0;
}

static void pci_bridge_parent_mem_update(struct pci_bridge *bridge)
{
	unsigned int type;

	if (bridge->parent == NULL) {
		return;
	}
	for (type = 0; type < (unsigned int)UDK_PCI_MEM_TYPE_NR; type++) {
		parent_mem_update(bridge->self, &bridge->child_mem[type]);
	}
}

static int pci_host_bridge_init(struct pci_domain *domain,
				struct udk_stack *stack)
{
	int err;
	struct pci_bridge *bridge = NULL;
	struct pci_bus *bus = NULL;
	struct udk_pci_slot slot;
	unsigned char dev_max;

	/* no parent bridge and device for host bridge */
	err = pci_add_bridge(domain, NULL, NULL, stack);
	if (err < 0) {
		udk_error("add pci host bridge failed: %s\n", strerror(-err));
		return err;
	}
	while (!udk_stack_empty(stack)) {
		bridge = udk_stack_top(stack);
		if (is_single_device(bridge)) {
			dev_max = 1;
		} else {
			dev_max = PCI_DEV_ID_MAX;
		}
		bus = &bridge->bus;
		if (bus->dev_nr != dev_max) {
			slot.id = bus->dev_nr;
			slot.func = 0U;
			err = pci_bus_scan_device(bus, &slot, stack);
			if (err < 0) {
				return err;
			}
			bus->dev_nr++;
		} else {
			err = pci_bridge_route_config(bridge);
			if (err < 0) {
				return err;
			}
			pci_bridge_parent_mem_update(bridge);
			(void)udk_stack_pop(stack);
		}
	}

	return 0;
}

static int pci_root_complex_init(struct pci_domain *domain)
{
	int err;
	struct udk_stack *stack = NULL;

	dlist_init(&domain->bridge_list);
	stack = udk_stack_create();
	if (stack == NULL) {
		udk_error("pci bridge stack create failed\n");
		return -ENOMEM;
	}
	err = pci_host_bridge_init(domain, stack);
	if (err < 0) {
		udk_error("initialize pci host bridge failed: %s\n",
			  strerror(-err));
		udk_stack_destroy(stack);
		return err;
	}
	udk_stack_destroy(stack);

	return 0;
}

static uint64_t pci_addr_to_cpu_addr(const struct udk_pci_host *host,
				     uint64_t pci_addr)
{
	const struct udk_pci_mem *space = NULL;
	uint64_t cpu_addr;
	uint32_t type;

	for (type = 0; type < (uint32_t)UDK_PCI_MEM_TYPE_NR; type++) {
		space = &host->mem_space[type];
		if (pci_addr >= space->pci_addr &&
		    pci_addr < space->pci_addr + space->size) {
			cpu_addr = space->cpu_addr + (pci_addr - space->pci_addr);
			return cpu_addr;
		}
	}

	return 0;
}

static int pci_host_mem_assign(const struct udk_pci_host *host,
			       struct pci_bridge *bridge)
{
	struct pci_allocator *allocator = NULL;
	const struct udk_pci_mem *mem = NULL;
	struct pci_mem_region *sum = NULL;
	unsigned int type;
	uint64_t addr;
	int err;

	allocator = pci_allocator_alloc();
	if (allocator == NULL) {
		udk_error("alloc pci allocator failed\n");
		return -ENOMEM;
	}
	for (type = 0; type < (uint32_t)UDK_PCI_MEM_TYPE_NR; type++) {
		mem = &host->mem_space[type];
		if (mem->pci_addr == 0) {
			continue;
		}
		err = pci_allocator_insert(allocator, mem->pci_addr, mem->size, type);
		if (err < 0) {
			udk_error("insert mem space failed\n");
			pci_allocator_destroy(allocator);
			return err;
		}
	}
	for (type = 0; type < (uint32_t)UDK_PCI_MEM_TYPE_NR; type++) {
		sum = &bridge->child_mem[type];
		if (sum->size == 0) {
			continue;
		}
		addr = pci_allocator_mem_alloc(allocator, sum->size, sum->align, type);
		if (addr == 0) {
			udk_error("mem space type: %u size is not large enough\n", type);
			pci_allocator_destroy(allocator);
			return -ENOMEM;
		}
		sum->pci_addr = addr;
		sum->cpu_addr = pci_addr_to_cpu_addr(host, addr);
	}
	pci_allocator_destroy(allocator);

	return 0;
}

/*
 * The BAR requested sizes are power of 2, and the allocation policy has to
 * meet the alignment requirememts. So, the memory regions are inserted into
 * the list in the order of alignment size to make full use of the source space.
 */
static int mem_region_insert(struct dlist_node *mem_head, struct pci_mem_region *mem)
{
	struct pci_mem_region_node *node = NULL;
	struct pci_mem_region_node *pos = NULL;
	struct pci_mem_region_node *next = NULL;
	struct pci_mem_region_node *prev = NULL;
	struct pci_mem_region *pos_mem = NULL;

	node = udk_malloc(sizeof(*node));
	if (node == NULL) {
		udk_error("malloc mem region node failed\n");
		return -ENOMEM;
	}
	mem_zero_s(*node);
	node->mem = mem;
	dlist_for_each_entry_safe(pos, next, mem_head, struct pci_mem_region_node, node) {
		pos_mem = pos->mem;
		if (mem->align > pos_mem->align) {
			if (prev != NULL) {
				dlist_insert(&prev->node, &node->node);
				return 0;
			}
			break;
		}
		prev = pos;
	}
	dlist_insert(mem_head, &node->node);

	return 0;
}

static inline void child_mem_list_cleanup(const struct dlist_node *mem_head)
{
	struct pci_mem_region_node *pos;
	struct pci_mem_region_node *next;

	dlist_for_each_entry_safe(pos, next, mem_head, struct pci_mem_region_node, node) {
		dlist_delete(&pos->node);
		udk_free(pos);
	}
}

static int child_mem_collect(const struct pci_bridge *bridge,
			    struct dlist_node *mem_head, uint32_t type)
{
	const struct pci_bus *bus = &bridge->bus;
	struct pci_domain *domain = bus->domain;
	struct udk_pci_device *device = NULL;
	struct pci_bridge *child = NULL;
	struct pci_bar *bar = NULL;
	int err;

	dlist_for_each_entry(device, &bus->devices, struct udk_pci_device, node) {
		dlist_for_each_entry(bar, &device->bar_list, struct pci_bar, node) {
			if (!is_mem_type_flag_match(type, bar->mem.flags)) {
				continue;
			}
			err = mem_region_insert(mem_head, &bar->mem);
			if (err < 0) {
				child_mem_list_cleanup(mem_head);
				return err;
			}
		}
	}
	dlist_for_each_entry(child, &domain->bridge_list, struct pci_bridge, node) {
		if (child->parent == bridge) {
			err = mem_region_insert(mem_head, &child->child_mem[type]);
			if (err < 0) {
				child_mem_list_cleanup(mem_head);
				return err;
			}
		}
	}

	return 0;
}

static int pci_device_bar_config(const struct pci_mem_region *mem)
{
	struct udk_pci_device *dev = mem->owner;
	struct pci_bar *bar = NULL;
	uint32_t low, high;
	int err;

	bar = container_of(mem, struct pci_bar, mem);
	low = (uint32_t)(mem->pci_addr | mem->flags);
	high = (uint32_t)(mem->pci_addr >> PCI_BAR_WIDTH);
	if (mem->flags & BAR_TYPE_32BIT) {
		if (mem->pci_addr > UINT_MAX) {
			return -EINVAL;
		}
	}
	err = pci_device_write_bar(dev, bar->index, low);
	if (err < 0) {
		return err;
	}
	if (mem->flags & BAR_TYPE_64BIT) {
		err = pci_device_write_bar(dev, bar->index + 1, high);
		if (err < 0) {
			return err;
		}
	}

	return 0;
}

static int pci_bridge_io_range_config(const struct pci_mem_region *mem)
{
	struct udk_pci_device *dev = mem->owner;
	struct pci_bus *bus = dev->bus;
	uint16_t io_low;
	uint32_t io_high;
	int err;

	io_low = bridge_mem_range16(mem->pci_addr, mem->pci_addr + mem->size - 1);
	err = pci_config_write16(bus, &dev->slot, PCI_IO_BASE, io_low);
	if (err < 0) {
		return err;
	}
	if (mem->pci_addr > USHRT_MAX) {
		io_high = bridge_mem_range32(mem->pci_addr, mem->pci_addr + mem->size - 1);
		err = pci_config_write32(bus, &dev->slot, PCI_IO_BASE_UPPER, io_high);
		if (err < 0) {
			return err;
		}
	}

	return 0;
}

static int pci_bridge_normal_range_config(const struct pci_mem_region *mem)
{
	struct udk_pci_device *dev = mem->owner;
	struct pci_bus *bus = dev->bus;
	uint32_t mem_range;
	int err;

	if ((mem->flags & BAR_PREFETCHABLE) != 0) {
		udk_error("prefetchable memory does not support now.\n");
		return -EINVAL;
	}
	if (mem->pci_addr > UINT_MAX) {
		udk_error("pci addr out of range.\n");
		return -EINVAL;
	}
	mem_range = bridge_mem_range32(mem->pci_addr, mem->pci_addr + mem->size - 1);
	err = pci_config_write32(bus, &dev->slot, PCI_MEM_BASE, mem_range);
	if (err < 0) {
		return err;
	}

	return 0;
}

static int pci_bridge_mem_config(const struct pci_mem_region *mem)
{
	if ((mem->flags & BAR_SPACE_IO) != 0) {
		return pci_bridge_io_range_config(mem);
	} else {
		return pci_bridge_normal_range_config(mem);
	}
}

static int pci_mem_config(const struct pci_mem_region *mem)
{
	if ((mem->flags & MEM_FLAG_CHILD_MEM) != 0) {
		return pci_bridge_mem_config(mem);
	} else {
		return pci_device_bar_config(mem);
	}
}

static int child_mem_assign(const struct pci_mem_region *src,
			    const struct dlist_node *mem_head)
{
	struct pci_mem_region_node *pos = NULL;
	struct pci_mem_region *mem = NULL;
	uint64_t pci_addr = src->pci_addr;
	uint64_t cpu_addr = src->cpu_addr;
	int err;

	dlist_for_each_entry(pos, mem_head, struct pci_mem_region_node, node) {
		mem = pos->mem;
		mem->pci_addr = pci_addr;
		mem->cpu_addr = cpu_addr;
		pci_addr += ALIGN_UP(mem->size, mem->align);
		cpu_addr += ALIGN_UP(mem->size, mem->align);
		err = pci_mem_config(mem);
		if (err < 0) {
			return err;
		}
	}

	return 0;
}

static int pci_bridge_mem_assign(struct pci_bridge *bridge)
{
	int err;
	uint32_t type;
	struct dlist_node mem_head;
	struct pci_mem_region *sum = NULL;

	for (type = 0; type < (uint32_t)UDK_PCI_MEM_TYPE_NR; type++) {
		sum = &bridge->child_mem[type];
		if (sum->size == 0) {
			continue;
		}
		dlist_init(&mem_head);
		err = child_mem_collect(bridge, &mem_head, type);
		if (err < 0) {
			return err;
		}
		err = child_mem_assign(sum, &mem_head);
		if (err < 0) {
			child_mem_list_cleanup(&mem_head);
			return err;
		}
		child_mem_list_cleanup(&mem_head);
	}

	return 0;
}

static int pci_domain_mem_assign(struct pci_domain *domain)
{
	struct udk_pci_host *host = domain->host;
	struct pci_bridge *host_bridge = NULL;
	struct pci_bridge *pos = NULL;
	int err;

	host_bridge = dlist_last_entry(&domain->bridge_list, struct pci_bridge, node);
	err = pci_host_mem_assign(host, host_bridge);
	if (err < 0) {
		return err;
	}
	/*
	 * The bridges are inserted into the head of the list when they are probed
	 * in the depth-first order, so the list should be traversed reversely to
	 * ensure the parent bridges to be assigned before the children devices.
	 */
	dlist_for_each_entry_reverse(pos, &domain->bridge_list, struct pci_bridge, node) {
		err = pci_bridge_mem_assign(pos);
		if (err < 0) {
			return err;
		}
	}

	return 0;
}

static struct pci_domain *pci_domain_find(const struct udk_pci_host *host)
{
	struct idr *domain_idr = &pci_domains.domain_idr;
	struct pci_domain *domain = NULL;
	unsigned int id = 0;

	do {
		domain = (struct pci_domain *)idr_get_next(domain_idr, &id);
		id++;
	} while (domain != NULL && domain->host != host);

	return domain;
}

static void pci_bridge_destroy(struct pci_bridge *bridge)
{
	struct udk_pci_device *pos = NULL;
	struct udk_pci_device *next = NULL;
	struct pci_bus *bus = &bridge->bus;

	dlist_for_each_entry_safe(pos, next, &bus->devices,
				  struct udk_pci_device, node) {
		dlist_delete(&pos->node);
		pci_device_destroy(pos);
	}
	dlist_delete(&bridge->node);
	udk_free(bridge);
}

static void pci_domain_destroy(struct pci_domain *domain)
{
	struct pci_bridge *pos = NULL;
	struct pci_bridge *next = NULL;

	dlist_for_each_entry_safe(pos, next, &domain->bridge_list,
				  struct pci_bridge, node) {
		pci_bridge_destroy(pos);
	}
	(void)idr_remove(&pci_domains.domain_idr, domain->id);
	udk_free(domain);
}

static int pci_domain_create(struct udk_pci_host *host)
{
	int err;
	struct pci_domain *domain = NULL;

	domain = (struct pci_domain *)udk_malloc(sizeof(*domain));
	if (domain == NULL) {
		udk_error("pci domain malloc failed\n");
		return -ENOMEM;
	}
	mem_zero_s(*domain);
	domain->host = host;
	domain->bus_index = host->init_bus_id;
	err = idr_alloc(&pci_domains.domain_idr, (void *)domain);
	if (err < 0) {
		udk_error("pci domain alloc idr failed\n");
		udk_free(domain);
		return -hmerrno2posix(err);
	}
	domain->id = (unsigned int)err;
	err = pci_root_complex_init(domain);
	if (err < 0) {
		udk_error("initialize pci root complex failed: %s\n",
			  strerror(-err));
		pci_domain_destroy(domain);
		return err;
	}
	err = pci_domain_mem_assign(domain);
	if (err < 0) {
		udk_error("pci domian assign bars failed: %s\n", strerror(-err));
		pci_domain_destroy(domain);
		return err;
	}

	return 0;
}

int udk_pci_host_register(struct udk_pci_host *host)
{
	int err;

	if (host == NULL || host->ops.read == NULL || host->ops.write == NULL) {
		return -EINVAL;
	}
	err = pci_domain_create(host);
	if (err < 0) {
		udk_error("create pci domain failed: %s\n", strerror(-err));
		return err;
	}

	return 0;
}

int udk_pci_host_unregister(const struct udk_pci_host *host)
{
	struct pci_domain *domain = NULL;

	if (host == NULL) {
		return -EINVAL;
	}
	domain = pci_domain_find(host);
	if (domain == NULL) {
		udk_warn("pci domain not found\n");
		return -EINVAL;
	}
	pci_domain_destroy(domain);

	return 0;
}
