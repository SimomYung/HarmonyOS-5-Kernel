/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Early console
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 29 07:23:29 2018
 */
#ifndef KLIBS_EARLY_CONSOLE_H
#define KLIBS_EARLY_CONSOLE_H

#include <hmkernel/types.h>
#include <hmkernel/io.h>

typedef enum {
	EARLY_CONSOLE_8250 = 0,
	EARLY_CONSOLE_PL011,
	EARLY_CONSOLE_STM32H7,
} early_console_dev_t;

extern void
early_console_init(void __iomem *base_addr, early_console_dev_t devtype,
		   bool uart_early_init,
		   bool kconsole_carriage_return);

extern void
early_console_put_char(int ch);

extern size_t
early_console_put_line(const char *str, size_t str_len, bool append_lf);

extern void
early_console_puts(const char *str, size_t str_len,
		   const char *prefix, size_t prefix_len);

#endif
