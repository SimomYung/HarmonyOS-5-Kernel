/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBLINUX_ASM_BOOTPARAM_H
#define __LIBLINUX_ASM_BOOTPARAM_H

struct boot_param {
	int is_early;
	char *name;
	char *val;
	int should_panic;
};

void liblinux_set_param(void);

#endif /* __LIBLINUX_ASM_BOOTPARAM_H */
