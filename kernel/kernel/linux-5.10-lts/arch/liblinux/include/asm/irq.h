/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_IRQ_H
#define __LIBLINUX_ASM_IRQ_H

#include_next <asm/irq.h>

#ifndef __ASSEMBLER__
#include <linux/mod_devicetable.h>
int liblinux_register_irqchip_table(const struct of_device_id *table);
#endif /* !__ASSEMBLER__ */

#endif /* __LIBLINUX_ASM_IRQ_H */
