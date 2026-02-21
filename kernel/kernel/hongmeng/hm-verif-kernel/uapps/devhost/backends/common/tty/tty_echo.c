/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Function of echo characters
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 13 15:31:39 2019
 */

#include "tty.h"
#include "tty_api.h"
#include "tty_wrap.h"
#include "tty_flow.h"
#include "tty_convert.h"

static bool is_non_printable_ctrl_code(char c)
{
	unsigned int i;
	const char printable[] = {UART_ASCII_CTRL_D, UART_ASCII_TAB,
				  UART_ASCII_NL, UART_ASCII_CR,
				  UART_ASCII_CTRL_Q, UART_ASCII_CTRL_S};

	if (!is_control_code((unsigned char)c)) {
		return false;
	}
	for (i = 0; i < ARRAY_SIZE(printable); i++) {
		if (c == printable[i]) {
			return false;
		}
	}
	/*
	 * Only when the current char is a control char(0-0x1F) and cannot be
	 * printed directly
	 */
	return true;
}

static int tty_process_echos(struct tty_struct *tty, char c)
{
	if (c == '\r' && (tty->tio.c_iflag & ICRNL)) {
		tty_convert_char(&c, CNVRT_OPT_CR2NL);
	}
	/*
	 * convert control char(other than NL, TAB, START, STOP) to X, where
	 * X is the char with ASCII code 0x40 greater than the special char-
	 * acter, echo "^X". Refer to the definition of termios ECHOCTL
	 */
	if (is_non_printable_ctrl_code(c) && ((tty->tio.c_lflag & ECHOCTL) != 0U)) {
		tty->bops->putc(tty->backend_drv, '^');
		tty_convert_char(&c, CNVRT_OPT_ECHOCTL);
	}

	/* Map NL to CR-NL on output if enable ONLCR, first put CR */
	if ((c == '\n') && (tty->tio.c_oflag & ONLCR)) {
		(void)tty->bops->putc(tty->backend_drv, '\r');
	}

	return tty->bops->putc(tty->backend_drv, c);
}

static int tty_echo_char(struct tty_struct *tty, char c)
{
	if (tty->tio.c_lflag & ECHO) {
		return tty_process_echos(tty, c);
	}

	/* echo the NL char when ECHONL is set */
	if ((tty->tio.c_lflag & (ECHONL | ICANON)) && (c == '\n')) {
		return tty_process_echos(tty, c);
	}

	/* tty won't echo chars, return */
	return 0;
}

int tty_echo(struct tty_struct *tty, const char *echo_buf, int size)
{
	int i;
	if (tty == NULL || tty->bops == NULL || tty->bops->putc == NULL ||
	    echo_buf == NULL) {
		return -EINVAL;
	}
	/* when tty outpot flow is off, no echo */
	if (tty_flow_is_output_off(&tty->flow_ctl)) {
		return 0;
	}

	for (i = 0; i < size; i++) {
		int ret;
		ret = tty_echo_char(tty, echo_buf[i]);
		if (ret < 0) {
			return ret;
		}
	}
	return 0;
}
tty_export_symbol(tty_echo);
