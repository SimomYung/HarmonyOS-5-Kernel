/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Early llc for elfloader
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 27 19:45:24 2021
 */

#ifndef LIB_EDRIVERS_ELLC_H
#define LIB_EDRIVERS_ELLC_H

#include <hmasm/memory.h>

struct ellc_dev {
	/*
	 * Record physical address, length of io registers and ioreg_base_virt
	 * which can be used by this driver.
	 */
	paddr_t ioreg_base_phy;
	unsigned long ioreg_len;
	void __iomem *ioreg_base_virt;

	/*
	 * Elfloader or early kernel set virtual device address after mapping.
	 */
	void (*set_vaddr_enable)(struct ellc_dev *edev,
				 void __iomem *base_addr_virt);

	/*
	 * Methods to maintain the entire llc, they are always invoked after
	 * maintain the caches integrated into the processor, the whole process
	 * (maintain integrated caches + maintain llc) should not load or store
	 * memory, so the input parameter should not be struct ellc_dev *,
	 * otherwise the methods must get base_addr_virt from memory.
	 */
	struct {
		void (*invalidate_all)(void __iomem *base_addr_virt);
		void (*clean_all)(void __iomem *base_addr_virt);
		void (*flush_all)(void __iomem *base_addr_virt);
	} all_ops;

	/*
	 * Methods to maintain llc by pa range.
	 */
	struct {
		void (*invalidate_range)(struct ellc_dev *edev,
					 paddr_t start_pa, paddr_t end_pa);
		void (*clean_range)(struct ellc_dev *edev,
				    paddr_t start_pa, paddr_t end_pa);
		void (*flush_range)(struct ellc_dev *edev,
				    paddr_t start_pa, paddr_t end_pa);
	} range_ops;
};

void ellc_dev_init_common(struct ellc_dev *edev, paddr_t base_addr_phy,
			  unsigned long ioreg_len);

void ellc_dev_init_all_ops(struct ellc_dev *edev,
			   void (*invalidate_all)(void __iomem *),
			   void (*clean_all)(void __iomem *),
			   void (*flush_all)(void __iomem *));

void ellc_dev_init_range_ops(struct ellc_dev *edev,
			     void (*invalidate_range)(struct ellc_dev *, paddr_t, paddr_t),
			     void (*clean_range)(struct ellc_dev *, paddr_t, paddr_t),
			     void (*flush_range)(struct ellc_dev *, paddr_t, paddr_t));

static inline void
ellc_dev_set_vaddr_enable(struct ellc_dev *edev,
			  void __iomem *base_addr_virt)
{
	if (edev->set_vaddr_enable != NULL) {
		edev->set_vaddr_enable(edev, base_addr_virt);
	}
	edev->ioreg_base_virt = base_addr_virt;
}

#endif
