/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Define ioctl of tty
 * Author: Huawei OS Kernel Lab
 * Create: Sun Mar 10 19:14:01 2019
 */

#include "tty_ioctl.h"
#include "tty.h"
#include "tty_api.h"
#include "tty_core.h"
#include "tty_wrap.h"
#include "tty_flow.h"

struct str_baud {
	tcflag_t baud_bit;
	speed_t baud_speed;
};

static const struct str_baud baud_table[] = {
	{B0,		0},
	{B50,		50},
	{B300,		300},
	{B1200,		1200},
	{B2400,		2400},
	{B4800,		4800},
	{B9600,		9600},
	{B19200,	19200},
	{B38400,	38400},
	{B57600,	57600},
	{B115200,	115200},
	{B460800,	460800},
};

static int achieve_baud_speed(tcflag_t bit)
{
	int i;
	int n_baud_table = (int)ARRAY_SIZE(baud_table);

	for (i = 0; i < n_baud_table; i++) {
		if (baud_table[i].baud_bit == bit) {
			return (int)baud_table[i].baud_speed;
		}
	}
	return -EINVAL;
}

#define BAUDRATE_FAULT_TOLERANCE_BASE		50
static int achieve_baud_bit(speed_t speed)
{
	int i;
	int n_baud_table = (int)ARRAY_SIZE(baud_table);
	/* Calculation method refer to the implementation of linux */
	speed_t tolerance = speed / BAUDRATE_FAULT_TOLERANCE_BASE;

	for (i = 0; i < n_baud_table; i++) {
		if (speed - tolerance <= baud_table[i].baud_speed &&
		    baud_table[i].baud_speed <= speed + tolerance) {
			return (int)baud_table[i].baud_bit;
		}
	}
	return -EINVAL;
}

static int tiocgpgrp(struct tty_struct *tty, unsigned long __user _arg, bool is_master)
{
	int ret;
	pid_t pgrp, remote_pgrp;
	void *arg = (void *)_arg;

	if (arg == NULL) {
		tty_debug("tty: Invalid ioctl arg, get tcpgrp failed\n");
		return -EINVAL;
	}

	pgrp = tty->pgrp;
	/* do not check remote pgrp for ptmx */
	if (!is_master) {
		remote_pgrp = tty_getfgpgrp(tty->sid);
		if (remote_pgrp < 0) {
			tty_warn("tty: getpgrp from sysmgr failed, err: %s\n", strerror(-remote_pgrp));
			return remote_pgrp;
		}
		if (remote_pgrp != pgrp) {
			tty_warn("tty: pgrp from sysmgr not consist with tty local pgrp, remote=%d, local=%d\n",
				 remote_pgrp, pgrp);
			return -EIO;
		}
	}

	ret = (int)copy_to_user(arg, &pgrp, sizeof(int));
	if (ret != 0) {
		tty_warn("tty: copy_to_user failed, err: %s\n", strerror(-ret));
		return -EFAULT;
	}
	tty_debug("tty: get pgrp from tty, pgrp=%d, tty->id=%u\n", pgrp, tty->id);
	return 0;
}

static int tiocspgrp(struct tty_struct *tty, unsigned long __user _arg, bool is_master)
{
	int ret;
	int pgrp = 0;
	void *arg = (void *)_arg;

	UNUSED(is_master);
	if (arg == NULL) {
		tty_debug("tty: invalid ioctl arg, set tcpgrp failed\n");
		return -EFAULT;
	}

	ret = (int)copy_from_user(&pgrp, arg, sizeof(int));
	if (ret != 0) {
		tty_warn("tty: copy_from_user failed, err: %s\n", strerror(-ret));
		return -EFAULT;
	}
	if (pgrp < 0) {
		return -EINVAL;
	}

	ret = tty_setfgpgrp(tty->sid, pgrp);
	if (ret != 0) {
		tty_warn("tty: cannot set pgrp, err: %s\n", strerror(-ret));
		return ret;
	}
	/* To reduce IPC to sysmgr, tty contains one copy of pgrp */
	tty->pgrp = pgrp;
	return ret;
}

static int tiocgsid(struct tty_struct *tty, unsigned long __user _arg, bool is_master)
{
	int ret;
	void *arg = (void *)_arg;

	UNUSED(is_master);
	if (arg == NULL) {
		tty_debug("tty: invalid ioctl arg, get sid failed\n");
		return -EFAULT;
	}
	ret = (int)copy_to_user(arg, &tty->sid, sizeof(int));
	if (ret != 0) {
		tty_warn("tty: copy_to_user failed, return %s\n", strerror(-ret));
		return -EFAULT;
	}
	return 0;
}

/* Check pid and sid of caller process and return sid */
static int current_caller_process_pre_check(void)
{
	pid_t pid;
	pid_t sid;

	sid = tty_mysid();
	if (sid < 0) {
		tty_warn("tty: acquire sid failed, err: %s\n", strerror(-sid));
		return sid;
	}

	pid = (pid_t)tty_current_pid();
	/* not session leader, no permission to control terminal */
	if (pid != sid) {
		return -EPERM;
	}
	return sid;
}

/* Check if tty has already been controled and will be preempted by root process */
static int control_terminal_preempt_check(const struct tty_struct *tty, int arg)
{
	/* Current tty has not been controled */
	if (tty->sid == 0) {
		return 0;
	}
	/*
	 * If tty has already been controled, only root process can preempt
	 * it when arg equals to 1
	 */
	if ((arg == TTY_TERMINAL_PREEMPTED_BY_ROOT)) {
		if (tty_caller_has_ab_and_cap(TTY_ABILITY, TTY_CAP_ADMIN) == 0) {
			return 0;
		}
	}
	tty_warn("tty: the tty has already been controled by other process, and "
		 "caller is not root, other sid = %d, arg = %d\n", tty->sid, arg);
	return -EPERM;
}

int tty_set_control_terminal(struct tty_struct *tty, int arg)
{
	int ret;
	pid_t pid;
	pid_t sid;
	struct tty_struct *sid_tty = NULL;

	if (tty == NULL || tty->dev == NULL) {
		return -EINVAL;
	}
	sid = current_caller_process_pre_check();
	if (sid < 0) {
		return sid;
	}
	pid = sid;

	/*
	 * caller process is leader of current session
	 * and it set this terminal as it's controling terminal already.
	 */
	if (sid == tty->sid) {
		return 0;
	}
	/*
	 * The calling process must be a session leader and
	 * it don't have a controlling terminal.
	 */
	sid_tty = find_tty_by_sid(sid);
	if (sid_tty != NULL) {
		tty_warn("tty: sid already point to tty, sid=%d, tty->id=%u\n",
			 sid, sid_tty->id);
		return -EPERM;
	}
	ret = control_terminal_preempt_check(tty, arg);
	if (ret < 0) {
		return ret;
	}

	/* set pgrp in sysmgr */
	ret = tty_setfgpgrp(sid, pid);
	if (ret != 0) {
		tty_warn("tty: cannot set pgrp, err: %s\n", strerror(-ret));
		return -EINVAL;
	}
	ret = tty_session_set_ttyinfo(sid, tty->dev->name, tty->dev->dev_no);
	if (ret < 0) {
		tty_warn("tty: set session tty name failed, sid: %d, err: %s\n",
			 sid, strerror(-ret));
		return ret;
	}
	tty->sid = sid;
	/* To reduce IPC to sysmgr, tty contains one copy of pgrp */
	tty->pgrp = pid;
	return ret;
}

static int tiocsctty(struct tty_struct *tty, unsigned long __user arg, bool is_master)
{
	UNUSED(is_master);
	return tty_set_control_terminal(tty, (int)(uintptr_t)arg);
}

static int tiocswinsz(struct tty_struct *tty, unsigned long __user _arg, bool is_master)
{
	int ret;
	struct winsize ws;
	void *arg = (void *)_arg;

	UNUSED(is_master);
	if (tty == NULL || arg == NULL) {
		tty_debug("tty: Invalid ioctl arg, set winsize failed\n");
		return -EFAULT;
	}

	ret = (int)copy_from_user(&ws, arg, sizeof(struct winsize));
	if (ret != 0) {
		tty_warn("tty: copy_from_user failed, err: %s\n", strerror(-ret));
		return -EFAULT;
	}

	/* Do not send signal SIGWINCH if winsz not changed */
	if (memcmp(&tty->winsz, &ws, sizeof(struct winsize)) == 0) {
		return 0;
	}

	tty->winsz = ws;
	tty_debug("tty: Resize tty winsz, send sig SIGWINCH\n");
	tty_kill_process_group(tty->sid, SIGWINCH);
	return 0;
}

static int tiocgwinsz(struct tty_struct *tty, unsigned long __user _arg, bool is_master)
{
	int ret;
	void *arg = (void *)_arg;

	UNUSED(is_master);
	if (tty == NULL || arg == NULL) {
		tty_debug("tty: Invalid ioctl arg, get winsize failed\n");
		return -EFAULT;
	}
	ret = (int)copy_to_user(arg, &tty->winsz, sizeof(struct winsize));
	if (ret != 0) {
		tty_warn("tty: copy_to_user failed, err: %s\n", strerror(-ret));
		return -EFAULT;
	}
	return 0;
}

static int tiocsptlck(struct tty_struct *tty, unsigned long arg, bool is_master)
{
	UNUSED(tty);
	UNUSED(arg);
	UNUSED(is_master);
	/* We do not need to support pts lock */
	return 0;
}

static int tty_perform_flush(struct tty_struct *tty, unsigned long arg, bool is_master)
{
	int ret;

	UNUSED(is_master);
	switch (arg) {
	case TCIFLUSH:
		ret = tty_line_buf_flush(tty->input_buf);
		break;

	case TCIOFLUSH:
		ret = tty_line_buf_flush(tty->input_buf);
		if (ret == 0) {
			ret = tty_line_buf_flush(tty->output_buf);
		}
		break;

	case TCOFLUSH:
		ret = tty_line_buf_flush(tty->output_buf);
		break;

	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}

static int tty_get_speed(const struct tty_struct *tty,
			 unsigned int *speed)
{
	if (tty == NULL || tty->bops == NULL) {
		return -EFAULT;
	}
	if (tty->bops->get_speed == NULL) {
		return -ENOSYS;
	}
	if (speed == NULL) {
		return -EINVAL;
	}
	return tty->bops->get_speed(tty->backend_drv, speed);
}

static int tty_set_speed(struct tty_struct *tty, unsigned int speed)
{
	if (tty == NULL || tty->bops == NULL) {
		return -EFAULT;
	}
	if (tty->bops->set_speed == NULL) {
		return -ENOSYS;
	}
	return tty->bops->set_speed(tty->backend_drv, speed);
}

static int tcsets(struct tty_struct *tty, unsigned long __user _arg, bool is_master)
{
	unsigned int bit;
	unsigned int old_bit;
	int ret;
	struct termios tio;
	void *arg = (void *)_arg;
	unsigned int speed = 0U;

	UNUSED(is_master);
	if (arg == NULL) {
		tty_debug("tty: Invalid ioctl arg\n");
		return -EFAULT;
	}
	ret = (int)copy_from_user(&tio, arg, sizeof(struct termios));
	if (ret != 0) {
		tty_warn("tty: copy_from_user failed, err: %s\n", strerror(-ret));
		return -EFAULT;
	}

	bit = tio.c_cflag & CBAUD;
	old_bit = tty->tio.c_cflag & CBAUD;
	if (bit != 0 && bit != old_bit) {
		ret = achieve_baud_speed(bit);
		if (ret < 0) {
			tty_warn("tty: not support this baudrate, baudrate=%u\n", bit);
			return -EINVAL;
		}
		speed = (unsigned int)ret;
		ret = tty_set_speed(tty, speed);
		if (ret == -ENOSYS) {
			/*
			 * Since not all platform support set/get baudrate,
			 * here we omit the error about ENOSYS
			 */
		} else if (ret != 0) {
			tty_warn("set baudrate err, speed=%u, ret: %s\n", speed, strerror(-ret));
			return ret;
		}
	}
	/* If bops support set_termios, set tio to backend driver */
	if (tty->bops != NULL && tty->bops->set_termios != NULL) {
		ret = tty->bops->set_termios(tty->backend_drv, &tio);
		if (ret < 0) {
			tty_warn("tty: set_termios failed, err: %s\n", strerror(-ret));
			return ret;
		}
	}

	ret = memcpy_s(&tty->tio, sizeof(tty->tio), &tio, sizeof(tio));
	if (ret != 0) {
		return -EFAULT;
	}

	return 0;
}

static int tcgets(struct tty_struct *tty, unsigned long __user _arg, bool is_master)
{
	int ret;
	int bit = 0;
	unsigned int speed = 0;
	void *arg = (void *)_arg;

	UNUSED(is_master);
	if (arg == NULL) {
		tty_debug("tty: Invalid ioctl arg\n");
		return -EFAULT;
	}
	/* If bops support get_termios, get tio from backend driver */
	if (tty->bops != NULL && tty->bops->get_termios != NULL) {
		ret = tty->bops->get_termios(tty->backend_drv, &tty->tio);
		if (ret < 0) {
			tty_warn("tty: get_termios failed, err: %s\n", strerror(-ret));
			return ret;
		}
	}

	/* input and output baudrate can be different when they are used
	 * in different terminals, but this case is not support now.
	 */
	ret = tty_get_speed(tty, &speed);
	if (ret == 0) {
		bit = achieve_baud_bit(speed);
		if (bit >= 0) {
			tty->tio.c_cflag &= ~CBAUD;
			tty->tio.c_cflag |= (speed_t)bit;
		} else {
			tty_info("tty: achieved baudrate err, baudrate=%u\n", speed);
			return -ENODATA;
		}
	} else if (ret == -ENOSYS) {
		/*
		 * Since not all platform support set/get baudrate,
		 * here we omit the error about ENOSYS
		 */
	} else {
		tty_warn("tty: achieve baudrate err\n");
		return ret;
	}
	ret = (int)copy_to_user(arg, &tty->tio, sizeof(struct termios));
	if (ret != 0) {
		tty_warn("tty: copy_to_user failed, err: %s\n", strerror(-ret));
		return -EFAULT;
	}
	return 0;
}

static int __tty_send_xchar(struct tty_struct *tty, char ch)
{
	int ret;

	if (tty->bops == NULL || tty->bops->putc == NULL) {
		return -EINVAL;
	}
	ret = tty->bops->putc(tty->backend_drv, ch);
	if (ret < 0) {
		tty_warn("tty: send xchar failed, ret: %s\n", strerror(-ret));
		return ret;
	}

	return 0;
}

static int tty_flow_state(struct tty_struct *tty, unsigned long arg, bool is_master)
{
	UNUSED(is_master);
	switch (arg) {
	/* stop tty output flow */
	case TCOOFF:
		tty_flow_set_output_off(&tty->flow_ctl);
		return 0;
	/* start tty output flow */
	case TCOON:
		tty_flow_set_output_on(&tty->flow_ctl);
		tty_flow_wake_up(&tty->flow_ctl);
		return 0;
	/* tty send stop char */
	case TCIOFF:
		return __tty_send_xchar(tty, UART_ASCII_CTRL_S);
	/* tty send start char */
	case TCION:
		return __tty_send_xchar(tty, UART_ASCII_CTRL_Q);
	default:
		return -EINVAL;
	};
}

static int tiocgicount(struct tty_struct *tty, unsigned long __user _arg, bool is_master)
{
	int ret;
	void *arg = (void *)_arg;
	struct serial_icounter_struct icount;

	UNUSED(is_master);
	if (arg == NULL) {
		tty_debug("tty: Invalid ioctl arg\n");
		return -EFAULT;
	}
	if (tty == NULL || tty->bops == NULL) {
		return -EFAULT;
	}
	if (tty->bops->get_icount == NULL) {
		return -ENOSYS;
	}

	mem_zero_s(icount);
	ret = tty->bops->get_icount(tty->backend_drv, &icount);
	if (ret < 0) {
		return ret;
	}
	ret = (int)copy_to_user(arg, &icount, sizeof(struct serial_icounter_struct));
	if (ret != 0) {
		tty_warn("tty: copy_to_user failed, err: %s\n", strerror(-ret));
		return -EFAULT;
	}
	return 0;
}

static int tty_stat_info(struct tty_struct *tty, unsigned long __user _arg, bool is_master)
{
	int ret;
	void *arg = (void *)_arg;
	struct tty_info_s tinfo;

	UNUSED(is_master);
	if (arg == NULL) {
		tty_debug("tty: Invalid ioctl arg\n");
		return -EFAULT;
	}
	if (tty == NULL || tty->input_buf == NULL || tty->output_buf == NULL) {
		return -EFAULT;
	}

	mem_zero_s(tinfo);

	tinfo.sid = tty->sid;
	tinfo.pgrp = tty->pgrp;
	tinfo.flag = tty->flag;
	(void)tty_line_buf_info(tty->input_buf, &tinfo.i_line_buf_info);
	(void)tty_line_buf_info(tty->output_buf, &tinfo.o_line_buf_info);

	ret = (int)copy_to_user(arg, &tinfo, sizeof(struct tty_info_s));
	if (ret != 0) {
		tty_warn("tty: copy_to_user failed, err: %s\n", strerror(-ret));
		return -EFAULT;
	}
	return 0;
}

static int tty_reset_setting(struct tty_struct *tty, unsigned long arg, bool is_master)
{
	UNUSED(arg);
	UNUSED(is_master);
	/*
	 * Temporary solutions, due to the problem of process resource
	 * recycling, currently we can only reset tty through ioctl. Normally,
	 * tty should be reset when tty device is released.
	 */
	tty_struct_reset(tty);
	return 0;
}

static int tty_serial_control(struct tty_struct *tty, unsigned long arg, bool is_master)
{
	int err;

	UNUSED(is_master);
	err = tty_caller_has_ab_and_cap(TTY_ABILITY_SERIAL_TOGGLE,
					TTY_CAP_CONFIG);
	if (err < 0) {
		tty_warn("tty: permission check failed, err: %s", strerror(-err));
		return err;
	}
	if (tty->bops == NULL) {
		tty_warn("tty: tty backend ops is NULL\n");
		return -EINVAL;
	}
	switch (arg) {
	case TTY_SERIAL_OOFF:
		if (tty->bops->stop_tx == NULL) {
			return -ENOSYS;
		}
		tty->bops->stop_tx(tty->backend_drv);
		return 0;
	case TTY_SERIAL_OON:
		if (tty->bops->start_tx == NULL) {
			return -ENOSYS;
		}
		tty->bops->start_tx(tty->backend_drv);
		return 0;
	case TTY_SERIAL_IOFF:
		if (tty->bops->stop_rx == NULL) {
			return -ENOSYS;
		}
		tty->bops->stop_rx(tty->backend_drv);
		return 0;
	case TTY_SERIAL_ION:
		if (tty->bops->start_rx == NULL) {
			return -ENOSYS;
		}
		tty->bops->start_rx(tty->backend_drv);
		return 0;
	default:
		return -EINVAL;
	};
}

static int tty_send_break(struct tty_struct *tty, unsigned int duration)
{
	if (tty->bops == NULL) {
		tty_warn("tty: tty backend ops is NULL\n");
		return -EINVAL;
	}
	if (tty->bops->break_ctl == NULL) {
		return 0;
	}
	(void)tty->bops->break_ctl(tty->backend_drv, TTY_BREAK_CTL_ENABLE);
	tty_msleep(duration);
	(void)tty->bops->break_ctl(tty->backend_drv, TTY_BREAK_CTL_DISABLE);
	return 0;
}

#define TCDRAIN_ARG 1UL
#define DEFAULT_BREAK_DURATION_MS 250U
#define BREAK_TIME_MULTIPLE 100U /* follow linux implementation */
static int tcsbrk(struct tty_struct *tty, unsigned long arg, bool is_master)
{
	int ret;
	unsigned int duration;

	UNUSED(is_master);
	if (tty->bops == NULL || tty->bops->flush == NULL) {
		return -EINVAL;
	}
	/*
	 * In hm-libc, the ioctl cmd used by tcdrain and tcsendbreak is both
	 * TCSBRK. The difference is that the tcdrain arg is 1 and the
	 * tcsendbreak arg is 0. Also, for the purpose of specifying duration
	 * for later tcsendbreak support, if arg does not equal 1,
	 * it is considered a tcsendbreak call.
	 */
	if (arg == TCDRAIN_ARG) { /* for tcdrain */
		(void)tty_mutex_lock(&tty->output_mutex);
		ret = tty->bops->flush(tty->backend_drv);
		(void)tty_mutex_unlock(&tty->output_mutex);
		return ret;
	} else {
		duration = (unsigned int)(arg > 0 ? arg * BREAK_TIME_MULTIPLE : DEFAULT_BREAK_DURATION_MS);
		return tty_send_break(tty, duration);
	}
}

static struct tty_ioctl_table g_ioctl_table[] = {
	{ TIOCGPGRP, tiocgpgrp },
	{ TIOCSPGRP, tiocspgrp },
	{ TIOCSCTTY, tiocsctty },
	{ TIOCGSID, tiocgsid },
	{ TIOCGWINSZ, tiocgwinsz },
	{ TIOCSWINSZ, tiocswinsz },
	{ TIOCSPTLCK, tiocsptlck },
	{ TIOCGICOUNT, tiocgicount },
	{ TCSETSF, tcsets },
	{ TCSETSW, tcsets },
	{ TCSETS, tcsets },
	{ TCGETS, tcgets },
	{ TCFLSH, tty_perform_flush },
	{ TCXONC, tty_flow_state },
	{ TTY_RESET_SETTING, tty_reset_setting },
	{ TTY_SERIAL_CONTROL, tty_serial_control },
	{ TTY_STAT_INFO, tty_stat_info },
	{ TCSBRK, tcsbrk },
	{ TCSBRKP, tcsbrk },
};

static tty_ioctl_handler_t get_ioctl_handler(unsigned int cmd)
{
	unsigned int i;
	unsigned int ioctl_cmd_num;

	ioctl_cmd_num = sizeof(g_ioctl_table) / sizeof(g_ioctl_table[0]);
	for (i = 0; i < ioctl_cmd_num; i++) {
		if (cmd == g_ioctl_table[i].cmd) {
			return g_ioctl_table[i].handler;
		}
	}

	return NULL;
}

/*
 * tty_ioctl will convert to uart_ioctl through tty_driver structure.
 */
int tty_ldisc_ioctl(struct tty_context *ctx, unsigned int cmd,
		    unsigned long arg)
{
	struct tty_struct *tty = NULL;
	tty_ioctl_handler_t ioctl_handler = NULL;

	if (ctx == NULL || ctx->tty == NULL) {
		return -EINVAL;
	}
	tty = ctx->tty;

	ioctl_handler = get_ioctl_handler(cmd);
	if (ioctl_handler != NULL) {
		return ioctl_handler(tty, arg, false);
	}

	if (tty->bops == NULL || tty->bops->ioctl == NULL) {
		tty_debug("tty: unsupport ioctl cmd, cmd=0x%x\n", cmd);
		return -EINVAL;
	}
	return tty->bops->ioctl(tty->backend_drv, cmd, arg);
}

int tty_stream_master_ioctl(struct tty_struct *tty, unsigned int cmd, unsigned long arg)
{
	tty_ioctl_handler_t ioctl_handler = NULL;

	if (tty == NULL) {
		return -EINVAL;
	}

	ioctl_handler = get_ioctl_handler(cmd);
	if (ioctl_handler == NULL) {
		tty_warn("tty: stream master ioctl not support cmd 0x%x\n", cmd);
		return -EOPNOTSUPP;
	}

	return ioctl_handler(tty, arg, true);
}
tty_export_symbol(tty_stream_master_ioctl);
