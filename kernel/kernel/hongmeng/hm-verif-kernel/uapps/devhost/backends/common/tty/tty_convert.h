/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Prototypes of convert-options
 * Author: Huawei OS Kernel Lab
 * Create: Sun May 19 17:56:50 2019
 */
#ifndef TTY_CONVERT_H
#define TTY_CONVERT_H

#include "tty_wrap.h"

enum tty_convert_option {
	CNVRT_OPT_CR2NL = 0,
	CNVRT_OPT_NL2CR,
	CNVRT_OPT_ECHOCTL
};

bool is_control_code(unsigned char c);
void tty_convert_char(char *ch, enum tty_convert_option idx);

#endif
