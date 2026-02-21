/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Keyboard shorcut
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 13 15:31:39 2019
 */

#include "tty_keyboard.h"
#include "tty.h"
#include "tty_wrap.h"
#include "tty_flow.h"
#include "tty_workqueue.h"

static void do_flow_on(struct tty_struct *tty)
{
	tty_flow_set_output_on(&tty->flow_ctl);
	tty_flow_wake_up(&tty->flow_ctl);
}

static void do_flow_off(struct tty_struct *tty)
{
	tty_flow_set_output_off(&tty->flow_ctl);
}

struct proc_signal_data {
	pid_t sid;
	int sig;
};

static void __do_send_proc_signal_handler(void *data)
{
	struct proc_signal_data *signal_data = (struct proc_signal_data *)data;

	if (signal_data == NULL) {
		tty_warn("tty: Invalid arg for proc send signal handler !\n");
		return;
	}
	tty_kill_process_group(signal_data->sid, signal_data->sig);
	tty_debug("tty: foreground processes were sent with signal %d\n", signal_data->sig);
	tty_mem_free(signal_data);
}

static int do_send_proc_signal(pid_t sid, int sig, const struct tty_struct *tty,
			       int ret)
{
	struct proc_signal_data *data = NULL;

	if ((tty->tio.c_lflag & ISIG) == 0) {
		return ret;
	}

	data = (struct proc_signal_data *)tty_mem_zalloc(sizeof(struct proc_signal_data));
	if (data == NULL) {
		tty_err("tty: malloc failed for struct proc_signal_data\n");
		return ret;
	}
	data->sid = sid;
	data->sig = sig;
	/*
	 * Since do_send_proc_signal will be called inside interrupt handler
	 * which is disabling preemption, to avoid calling activation call
	 * with disabling preemption, we put the send signal work into wq.
	 */
	tty_async_schedule(__do_send_proc_signal_handler, data);

	return SHORTCUT_KEY_ONLY_ECHO;
}

static int process_flow_ctrl(char shortcuts_key, struct tty_struct *tty,
			     int ret)
{
	/* If no IXON flag set, do no flow control */
	if ((tty->tio.c_iflag & IXON) != IXON) {
		return ret;
	}
	if (shortcuts_key == tty->tio.c_cc[VSTART]) {
		do_flow_on(tty);
		return SHORTCUT_KEY_DISCARD;
	}
	if (shortcuts_key == tty->tio.c_cc[VSTOP]) {
		do_flow_off(tty);
		return SHORTCUT_KEY_DISCARD;
	}
	/* break signal shall be allowed to break flow off and start the output */
	if (shortcuts_key == tty->tio.c_cc[VINTR] ||
	    shortcuts_key == tty->tio.c_cc[VQUIT] ||
	    shortcuts_key == tty->tio.c_cc[VSUSP]) {
		do_flow_on(tty);
		return ret;
	}
	if (tty->tio.c_iflag & IXANY) {
		do_flow_on(tty);
	}
	return ret;
}

static int process_signal_char(char shortcuts_key, const struct tty_struct *tty,
			       int ret)
{
	if (shortcuts_key == tty->tio.c_cc[VINTR]) {
		return do_send_proc_signal(tty->sid, SIGINT, tty, ret);
	}
	if (shortcuts_key == tty->tio.c_cc[VQUIT]) {
		return do_send_proc_signal(tty->sid, SIGQUIT, tty, ret);
	}
	if (shortcuts_key == tty->tio.c_cc[VSUSP]) {
		return do_send_proc_signal(tty->sid, SIGTSTP, tty, ret);
	}
	return ret;
}

static int process_debug_char(char shortcuts_key, const struct tty_struct *tty,
			      int ret)
{
	if ((tty->options & TTY_OPTION_HANDLE_DEBUG_SHORTCUT) != TTY_OPTION_HANDLE_DEBUG_SHORTCUT) {
		return ret;
	}

	/*
	 * If no shortcut key is enabled, the parameter shortcuts_key should be
	 * declared UNUSED. At the same time, this declaration does not affect
	 * the subsequent use of this parameter.
	 */
	UNUSED(shortcuts_key);
#ifdef CONFIG_SERIAL_KSHORTCUT_SYS_PAUSE
	if (shortcuts_key == UART_ASCII_SYS_PAUSE) {
		tty_info("receive ctrl-x, will shutdown system\n");
		(void)tty_shutdown_hm();
		return SHORTCUT_KEY_ONLY_ECHO;
	}
#endif
#ifdef CONFIG_SERIAL_KSHORTCUT_TRIGGER_SNAPSHOT
	if (shortcuts_key == UART_ASCII_SNAPSHOT) {
		tty_info("receive ctrl-t, will trigger snapshot\n");
		(void)tty_snapshot();
		return SHORTCUT_KEY_ONLY_ECHO;
	}
#endif
	return ret;
}

int tty_handle_shortcuts_key(char shortcuts_key, struct tty_struct *tty)
{
	int ret = SHORTCUT_KEY_NORMAL;

	if (tty == NULL) {
		tty_err("tty: Invalid arg for tty\n");
		return SHORTCUT_KEY_DISCARD;
	}

	ret = process_debug_char(shortcuts_key, tty, ret);
	ret = process_flow_ctrl(shortcuts_key, tty, ret);
	ret = process_signal_char(shortcuts_key, tty, ret);

	return ret;
}
tty_export_symbol(tty_handle_shortcuts_key);
