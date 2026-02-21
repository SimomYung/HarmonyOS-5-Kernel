/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Early watchdog for elfloader and kernel
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 27 16:30:55 2021
 */

#ifndef LIB_EDRIVERS_EWATCHDOG_H
#define LIB_EDRIVERS_EWATCHDOG_H

#include <hmkernel/types.h>
#include <hmasm/memory.h>

#define EWDG_CYCLES_PER_US 1000U

struct ewatchdog_dev {
	/*
	 * similar to econsole, record physical address,
	 * length of io registers and ioreg_base_virt which
	 * can be used by this driver.
	 */
	paddr_t ioreg_base_phy;
	unsigned long ioreg_len;
	void __iomem *ioreg_base_virt;

	/*
	 * Early kernel set virtual device address after mapping.
	 */
	void (*set_vaddr_enable)(struct ewatchdog_dev *edev,
				 void __iomem *base_addr_virt);

	void (*touch)(struct ewatchdog_dev *edev);
};

static inline void
ewatchdog_touch(struct ewatchdog_dev *edev)
{
	if ((edev->touch != NULL) && (edev->ioreg_base_virt != NULL)) {
		edev->touch(edev);
	}
}

void ewatchdog_dev_init_common(struct ewatchdog_dev *edev, paddr_t base_addr_phy,
			       unsigned long ioreg_len,
			       void (*touch)(struct ewatchdog_dev *));

static inline void
ewatchdog_dev_set_vaddr_enable(struct ewatchdog_dev *edev,
			       void __iomem *base_addr_virt)
{
	if (edev->set_vaddr_enable != NULL) {
		edev->set_vaddr_enable(edev, base_addr_virt);
	}
	edev->ioreg_base_virt = base_addr_virt;
}

#endif
