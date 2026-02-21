/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: PCI mem type interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 30 14:44:54 2021
 */
#ifndef __UDK_PCI_MEMORY_H__
#define __UDK_PCI_MEMORY_H__

#include <udk/types.h>
#include <stdbool.h>

#define BAR_SPACE_IO		0x1
#define BAR_TYPE_32BIT		0x2
#define BAR_TYPE_64BIT		0x4
#define BAR_PREFETCHABLE	0x8
#define MEM_FLAG_TYPE_MASK	0xfU

#define MEM_FLAG_CHILD_MEM	0x10U

int mem_flag_to_type(uint32_t flag);
uint32_t mem_type_to_flag(uint32_t type);
bool is_mem_type_flag_match(uint32_t type, uint32_t flag);

struct pci_allocator;

struct pci_allocator *pci_allocator_alloc(void);
int pci_allocator_insert(struct pci_allocator *allocator, uint64_t addr,
			 uint64_t size, uint32_t type);
uint64_t pci_allocator_mem_alloc(struct pci_allocator *allocator,
				 uint64_t size, uint64_t align,
				 uint32_t type);
void pci_allocator_destroy(struct pci_allocator *allocator);

#endif /* __UDK_PCI_MEMORY_H__ */
