/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Functions of convert-options
 * Author: Huawei OS Kernel Lab
 * Create: Sun May 19 15:52:20 2019
 */

#include "tty_convert.h"
#include "tty_wrap.h"

#define ASCII_CONTROL_CODE_RANGE 32
#define ASCII_CTRLCODE_CONVERT_MASK 0x40

bool is_control_code(unsigned char c)
{
	if (c < ASCII_CONTROL_CODE_RANGE) {
		return true;
	} else {
		return false;
	}
}

static inline void tty_convert_cr2nl(char *c)
{
	if (*c == '\r') {
		*c = '\n';
	}
}

static inline void tty_convert_nl2cr(char *c)
{
	if (*c == '\n') {
		*c = '\r';
	}
}

static inline void tty_convert_echoctl(char *c)
{
	if (is_control_code((unsigned char)*c)) {
		/* range of control code is 0-32, won't overflow */
		*c = (char)((unsigned char)*c ^ ASCII_CTRLCODE_CONVERT_MASK);
	}
}

typedef void (*tty_convert_func_t)(char *c);

static tty_convert_func_t tty_convert_fn[] = {
	[CNVRT_OPT_CR2NL]		= tty_convert_cr2nl,
	[CNVRT_OPT_NL2CR]		= tty_convert_nl2cr,
	[CNVRT_OPT_ECHOCTL]		= tty_convert_echoctl,
};

void tty_convert_char(char *ch, enum tty_convert_option idx)
{
	if (ch == NULL) {
		tty_warn("tty: convert char points to NULL\n");
		return;
	}

	if (idx > CNVRT_OPT_ECHOCTL) {
		tty_warn("tty: convert char opt out of range, idx=%d\n", (int)idx);
		return;
	}

	tty_convert_fn[idx](ch);
}
