/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Early console for elfloader kernel
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 25 15:18:36 2021
 */

#ifndef LIB_EDRIVERS_ECONSOLE_H
#define LIB_EDRIVERS_ECONSOLE_H

#include <hmkernel/types.h>
#include <hmkernel/io.h>
#include <hmasm/memory.h>

#define ECONSOLE_DEV_FLAGS_CARRIAGE_RETURN	0x1u

struct econsole_dev {
	/*
	 * let early early kernel (including elfloader and lowvisor)
	 * ioreg_base is unknown to klibs, should be filled by
	 * driver in early kernel.
	 * Both read only. inited by init().
	 */
	paddr_t ioreg_base_phy;
	unsigned long ioreg_len;

	/*
	 * for econsole framework.
	 * flags: readonly
	 * is_line_empty: used by the framework
	 */
	unsigned long flags;
	bool is_line_empty;

	/* Readonly. Set it using set_vaddr. */
	void __iomem *ioreg_base_virt;

	void (*touch_watchdog)(void);

	/*
	 * Early kernel set virtual device address after mapping.
	 */
	void (*set_vaddr_enable)(struct econsole_dev *edev,
				 void __iomem *base_addr_virt);
	void (*put_char)(struct econsole_dev *edev, int ch);

	void (*reset)(struct econsole_dev *edev);

	void *_private;
};

extern void
econsole_puts(struct econsole_dev *edev, const char *str,
	      size_t str_len, const char *prefix,
	      size_t prefix_len);

extern void
econsole_put_char(struct econsole_dev *edev, int ch);

/* helpers */
extern void
econsole_dev_init_common(struct econsole_dev *edev, paddr_t base_addr_phy,
			 unsigned long ioreg_len, unsigned long flags,
			 void (*put_char)(struct econsole_dev *, int));

static inline void
econsole_dev_set_vaddr_enable(struct econsole_dev *edev, void __iomem *base_addr_virt)
{
	if (edev->set_vaddr_enable != NULL) {
		edev->set_vaddr_enable(edev, base_addr_virt);
	}
	edev->ioreg_base_virt = base_addr_virt;
}

static inline void
econsole_dev_reset(struct econsole_dev *edev)
{
	if (edev->reset != NULL) {
		edev->reset(edev);
	}
	edev->is_line_empty = true;
}

#endif
