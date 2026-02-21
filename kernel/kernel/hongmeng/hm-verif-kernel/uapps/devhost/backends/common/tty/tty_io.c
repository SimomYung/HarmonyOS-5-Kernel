/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Process of I/O of tty
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 08 16:32:27 2019
 */

#include "tty_io.h"
#include "tty.h"
#include "tty_core.h"
#include "tty_wrap.h"
#include "tty_flow.h"
#include "tty_ioctl.h"
#include "tty_convert.h"

#define UART_ASCII_BACK_SPACE 0x1f
#define UART_ASCII_DELETE 0x7f

static int current_process_info(pid_t *current_pid, pid_t *current_pgid,
				pid_t *current_sid)
{
	pid_t pid;
	pid_t pgid;
	pid_t sid;

	/*
	 * Make the below three IPCs into one when HM ulibs
	 * support API to query pid, pgid and sid by one call
	 */
	pid = (pid_t)tty_current_pid();
	if (pid < 0) {
		tty_warn("tty: Failed to get current_pid, err: %s\n", strerror(-pid));
		return pid;
	}

	pgid = (pid_t)tty_getpgid(pid);
	if (pgid < 0) {
		tty_warn("tty: Failed to get current_pgid, err: %s\n", strerror(-pgid));
		return pgid;
	}

	sid = (pid_t)tty_getsid(pid);
	if (sid < 0) {
		tty_warn("tty: Failed to get current_sid, err: %s\n", strerror(-sid));
		return sid;
	}
	*current_pid = pid;
	*current_pgid = pgid;
	*current_sid = sid;
	return 0;
}

static int check_foreground_process(const struct tty_struct *tty, int signum)
{
	pid_t current_pid = 0;
	pid_t current_pgid = 0;
	pid_t current_sid = 0;
	struct tty_struct *current_tty = NULL;
	int ret;

	if (tty == NULL) {
		return -EIO;
	}

	if (tty->pgrp == 0 || tty->sid == 0) {
		/*
		 * In some situation tty will be used just as one char device,
		 * so when no pgrp is set, we do not check foreground process
		 */
		return 0;
	}

	ret = current_process_info(&current_pid, &current_pgid, &current_sid);
	if (ret < 0) {
		return ret;
	}

	current_tty = find_tty_by_sid(current_sid);
	if (current_tty != tty) {
		/*
		 * Behave same as linux, allow process in different session or
		 * even belongs to session assocated with no tty (which means
		 * current_tty equals to NULL) to read the other tty, even when
		 * process is background
		 */
		tty_debug("tty: current process does not belong to current tty, "
			 "pid=%d, sid=%d\n", current_pid, current_sid);
		return 0;
	}

	if (current_pgid == tty->pgrp) {
		return 0;
	}
	tty_warn("tty: send sig %d to pgid %d which is not foregound process\n",
		 signum, -current_pgid);
	/* send signal to background process */
	tty_kill_process(-current_pgid, signum);

	return -ERESTARTSYS;
}

static unsigned int tty_ldisc_poll(const struct tty_context *ctx)
{
	unsigned int mask = 0;
	int input_status;
	int output_status;
	struct tty_struct *tty = NULL;

	if (ctx == NULL) {
		return POLLNVAL;
	}
	tty = ctx->tty;
	if (tty == NULL) {
		return POLLNVAL;
	}

	/* Note: we do not check background process in poll */
	input_status = tty_line_buf_state(tty->input_buf);
	output_status = tty_line_buf_state(tty->output_buf);

	/* if input buf is not empty, set POLLIN */
	if (input_status != (int)LINE_BUF_STATUS_EMPTY) {
		mask |= POLLIN | POLLRDNORM;
	}
	/* if output buf is not full, set POLLOUT */
	if (output_status != (int)LINE_BUF_STATUS_FULL) {
		mask |= POLLOUT | POLLWRNORM;
	}
	/* if status < 0, set POLLERR */
	if ((input_status < 0) || (output_status < 0)) {
		mask |= POLLERR;
	}
	/* if pts have been removed, set POLLHUP POLLERR */
	if ((tty->flag & TTY_FLAG_REMOVED) != 0) {
		mask |= POLLHUP | POLLERR;
	}
	return mask;
}

/* Currenlty 1 char will be converted to 2 chars at most */
#define TTY_IO_TMP_BUF_SIZE 2

static int tty_process_write_char(const struct tty_struct *tty, char *buf, int size, char c)
{
	int i = 0;

	if (size > TTY_IO_TMP_BUF_SIZE) {
		return -ERANGE;
	}

	if ((tty->tio.c_oflag & OPOST) != OPOST) {
		buf[i] = c;
		return (i + 1);
	}
	if (c == '\n' && (tty->tio.c_oflag & ONLCR)) {
		tty_convert_char(&c, CNVRT_OPT_NL2CR);
		buf[i] = c;
		i++;
		buf[i] = '\n';
	} else if ((c == '\r') && (tty->tio.c_oflag & OCRNL)) {
		tty_convert_char(&c, CNVRT_OPT_CR2NL);
		buf[i] = c;
	} else {
		buf[i] = c;
	}

	return (i + 1);
}

/*
 * tty_write_output_buf is called with helding output_mutex lock, so it must
 * return with helding the lock
 */
static ssize_t tty_write_output_buf(struct tty_struct *tty, const char *buf,
				    ssize_t count)
{
	ssize_t i = 0;
	char tmpbuf[TTY_IO_TMP_BUF_SIZE];

	while (i < count) {
		int ret;
		int size;
		int real_wt_size = 0;

		mem_zero_a(tmpbuf);
		size = tty_process_write_char(tty, tmpbuf, TTY_IO_TMP_BUF_SIZE, buf[i]);
		if (size < 0 || size > TTY_IO_TMP_BUF_SIZE) {
			tty_warn("process write char failed, size=%d\n", size);
			return -EIO;
		}
		while (real_wt_size < size) {
			ret = tty_line_buf_write(tty->output_buf, tmpbuf + real_wt_size,
						 (unsigned int)(size - real_wt_size));
			if (ret < 0) {
				tty_warn("write to output buf failed, ret: %s\n", strerror(-ret));
				return ret;
			}
			real_wt_size += ret;
			if (real_wt_size == size) {
				break;
			}
			(void)tty_mutex_unlock(&tty->output_mutex);
			ret = tty_line_buf_schedule_writer(tty->output_buf);
			(void)tty_mutex_lock(&tty->output_mutex);
			if (ret < 0) {
				tty_debug("schedule writer to output buf failed, ret: %s\n",
					  strerror(ret));
				return ((ret == -EINTR) ? -ERESTARTSYS : ret);
			}
			/* Since tty line buf may false triggered, only notify POLLOUT when space available */
			if (tty_line_buf_space_avail(tty->output_buf) > 0) {
				tty_poll_notify(&tty->wq, POLLOUT);
			}
		}
		i++;
	}
	return i;
}

static int tty_ldisc_write(const struct tty_context *ctx, const char *buf,
			   ssize_t count)
{
	int ret = 0;
	struct tty_struct *tty = NULL;

	if (ctx == NULL) {
		return -EINVAL;
	}
	tty = ctx->tty;

	if (tty == NULL || tty->bops == NULL || tty->bops->flush == NULL) {
		return -EIO;
	}
	if ((tty->tio.c_lflag & TOSTOP) == TOSTOP) {
		int err;
		err = check_foreground_process(tty, SIGTTOU);
		if (err < 0) {
			return err;
		}
	}
	if (buf == NULL) {
		return -EINVAL;
	}
	if (tty_flow_is_output_off(&tty->flow_ctl)) {
		ret = tty_flow_schedule(&tty->flow_ctl);
		if (ret < 0) {
			tty_warn("tty: flow schedule writer failed, err: %s\n", strerror(-ret));
			return ((ret == -EINTR) ? -ERESTARTSYS : ret);
		}
	}

	(void)tty_mutex_lock(&tty->output_mutex);
	ret = (int)tty_write_output_buf(tty, buf, count);
	(void)tty_mutex_unlock(&tty->output_mutex);
	/*
	 * Since linebuf wake and notify need no lock to protect, do not need
	 * to lock flush func to avoid calling syscall while holding lock
	 */
	(void)tty->bops->flush(tty->backend_drv);
	return ret;
}

#define LOOP_CONTINUE 1
#define LOOP_BREAK 0
#define CHAR_SIZE 1 /* Character type byte length */

static inline void tty_process_normal_char(char c, char *buf, int *real_rd_size_p)
{
	/* handle normal input char */
	buf[*real_rd_size_p] = c;
	*real_rd_size_p += CHAR_SIZE;
}

static void tty_process_erase_word(char *buf, int *real_rd_size_p)
{
	/* last char is whitespace */
	while ((*real_rd_size_p > 0) && (buf[*real_rd_size_p - 1] == ' ')) {
		*real_rd_size_p -= CHAR_SIZE;
	}
	/* delete one word */
	while ((*real_rd_size_p > 0) && (buf[*real_rd_size_p - 1] != ' ')) {
		buf[*real_rd_size_p] = '\0';
		*real_rd_size_p -= CHAR_SIZE;
	}
	if (*real_rd_size_p < 0) {
		*real_rd_size_p = 0;
	}
	return;
}

static int tty_process_ctrl_code(char c, char *buf, int *real_rd_size_p,
				 const struct tty_struct *tty)
{
	switch (c) {
	case (UART_ASCII_BACK_SPACE):
		/* fall-through */
	case (UART_ASCII_CTRL_H):
		/* fall-through */
	case (UART_ASCII_DELETE):
		if (tty->tio.c_lflag & ECHOE) {
			/*
			 * handle backspace/delete,
			 * regard delete same as backspace
			 */
			buf[*real_rd_size_p] = '\0';
			*real_rd_size_p -= CHAR_SIZE;
			if (*real_rd_size_p < 0) {
				*real_rd_size_p = 0;
			}
		} else {
			tty_process_normal_char(c, buf, real_rd_size_p);
		}
		return LOOP_CONTINUE;
	case (UART_ASCII_NUL):
		tty_process_normal_char('\0', buf, real_rd_size_p);
		return LOOP_BREAK;
	case (UART_ASCII_CTRL_D):
		/* When receive Ctrl-D break */
		return LOOP_BREAK;
	case (UART_ASCII_CTRL_U):
		if (tty->tio.c_lflag & ECHOK) {
			/* delete the whole line */
			buf[0] = '\0';
			*real_rd_size_p = 0;
		} else {
			tty_process_normal_char(c, buf, real_rd_size_p);
		}
		return LOOP_CONTINUE;
	case (UART_ASCII_CTRL_W):
		if (tty->tio.c_lflag & (ECHOE | IEXTEN)) {
			/* delete the whitespace-delimited word */
			tty_process_erase_word(buf, real_rd_size_p);
		} else {
			tty_process_normal_char(c, buf, real_rd_size_p);
		}
		return LOOP_CONTINUE;
	default:
		tty_process_normal_char(c, buf, real_rd_size_p);
		return LOOP_CONTINUE;
	}
}

/* process read char and return status about loop continue or not */
static int tty_process_read_char(char c, char *buf, int *real_rd_size_p,
				 const struct tty_struct *tty)
{
	if (c == '\r') {
		if (tty->tio.c_iflag & IGNCR) {
			/* IGNCR will ignore \r */
			return LOOP_CONTINUE;
		}
		if (tty->tio.c_iflag & ICRNL) {
			tty_convert_char(&c, CNVRT_OPT_CR2NL);
		}
	} else if (c == '\n' && (tty->tio.c_iflag & INLCR)) {
		tty_convert_char(&c, CNVRT_OPT_NL2CR);
	}

	if ((tty->tio.c_lflag & ICANON) == 0) {
		/* Not enable canonical mode, just return */
		tty_process_normal_char(c, buf, real_rd_size_p);
		return LOOP_CONTINUE;
	}
	if (c == '\n') {
		/* When recv \n, we just break loop read */
		tty_process_normal_char(c, buf, real_rd_size_p);
		return LOOP_BREAK;
	}

	/* ctrl code shall be handled unter canonical mode */
	return tty_process_ctrl_code(c, buf, real_rd_size_p, tty);
}

/*
 * tty_read_input_buf is called with helding lock tty->input_mutex,
 * so it must return with helding the lock
 */
static int tty_read_input_buf(struct tty_struct *tty, int max_rd_size,
			      char *buf, unsigned int flag)
{
	char c = 0;
	/* here we read one char from input buf everytime to evaluate it */
	const unsigned int c_rd_size = CHAR_SIZE;
	int real_rd_size = 0;
	int ret;

	/* read until a linebreak or real read size is enough */
	while (real_rd_size < max_rd_size) {
		/* read one char each time */
		ret = tty_line_buf_read(tty->input_buf, &c, c_rd_size);
		if (ret < 0) {
			return ret;
		}
		if (ret > 0) {
			int loop_continue;
			loop_continue = tty_process_read_char(c, buf, &real_rd_size, tty);
			if (loop_continue == LOOP_BREAK) {
				break;
			}
			continue;
		}
		/* handle ret = 0 case, which means read no data */
		if((tty->flag & TTY_FLAG_REMOVED) == TTY_FLAG_REMOVED) {
			return -EAGAIN;
		}
		if (((tty->tio.c_lflag & ICANON) != ICANON) &&
		    (tty->tio.c_cc[VMIN] == 0)) {
			break;
		}
		if (!file_block(flag)) {
			/* if non block when read no data, we just return real_rd_size */
			if (real_rd_size == 0) {
				return -EAGAIN;
			}
			break;
		}
		/* schedule thread when blocked, unlock input_mutex */
		(void)tty_mutex_unlock(&tty->input_mutex);
		ret = tty_line_buf_schedule_reader(tty->input_buf);
		(void)tty_mutex_lock(&tty->input_mutex);
		if (ret < 0) {
			tty_warn("tty: tty_schedule_input_reader failed, err: %s\n", strerror(-ret));
			return ((ret == -EINTR) ? -ERESTARTSYS : ret);
		}
	}
	return real_rd_size;
}

static void calculate_read_size(const struct tty_struct *tty, ssize_t count,
				int *max_rd_size_p, unsigned int flag)
{
	/* VMIN only works in non-canonical mode */
	if ((tty->tio.c_lflag & ICANON) != ICANON) {
		/* set max_rd_size according to vmin and count when in block mode */
		if ((tty->tio.c_cc[VMIN] > 0) && file_block(flag)) {
			*max_rd_size_p = min((int)tty->tio.c_cc[VMIN], (int)count);
		}
	}
}

static int tty_ldisc_read(const struct tty_context *ctx, char *buf,
			  ssize_t count)
{
	int ret;
	int max_rd_size = (int)count;
	struct tty_struct *tty = NULL;
	unsigned int flag;
	int input_status;
	int real_rd_size;

	if (ctx == NULL) {
		return -EINVAL;
	}
	tty = ctx->tty;
	flag = ctx->flag;

	if (tty == NULL || tty->bops == NULL) {
		return -EINVAL;
	}
	ret = check_foreground_process(tty, SIGTTIN);
	if (ret < 0) {
		return ret;
	}

	(void)tty_mutex_lock(&tty->input_mutex);
	/* before read, get input_buf state */
	input_status = tty_line_buf_state(tty->input_buf);

	calculate_read_size(tty, count, &max_rd_size, flag);

	real_rd_size = tty_read_input_buf((struct tty_struct *)tty,
					  max_rd_size, buf, flag);
	if (real_rd_size <= 0) {
		/*
		 * Since we read nothing, just return here, and does not need
		 * to clean POLLIN event.
		 */
		(void)tty_mutex_unlock(&tty->input_mutex);
		return real_rd_size;
	}

	/*
	 * Only notify POLLOUT when read linebuf from full to half-full,
	 * since when linebuf is not full, we will always return POLLOUT
	 * for poll func and we do not need to notify again.
	 */
	if (input_status == (int)LINE_BUF_STATUS_FULL && real_rd_size > 0) {
		if (tty->bops->poll_notify != NULL) {
			tty->bops->poll_notify(tty->backend_drv, POLLOUT);
		}
	}
	(void)tty_mutex_unlock(&tty->input_mutex);

	return real_rd_size;
}

static int tty_ldisc_open(struct tty_context *ctx)
{
	struct tty_struct *tty = NULL;
	if (ctx == NULL || ctx->tty == NULL) {
		tty_warn("tty: ctx or ctx->tty is NULL\n");
		return -EINVAL;
	}
	tty = ctx->tty;

	(void)tty_mutex_lock(&tty->mutex);
	tty->refcnt++;
	tty->flag |= TTY_FLAG_BE_USING;
	(void)tty_mutex_unlock(&tty->mutex);

	/* If flag do not contains O_NOCTTY, try to set as control terminal */
	if ((ctx->flag & O_NOCTTY) != O_NOCTTY) {
		/* Here we ignore the result of set control terminal */
		(void)tty_set_control_terminal(tty, TTY_TERMINAL_NOT_PREEMPTED);
	}

	return 0;
}

static void tty_ldisc_close(const struct tty_context *ctx)
{
	struct tty_struct *tty = NULL;
	if (ctx == NULL || ctx->tty == NULL) {
		tty_warn("tty: ctx or ctx->tty is NULL\n");
		return;
	}
	tty = ctx->tty;

	/* Sub refcnt of tty */
	(void)tty_mutex_lock(&tty->mutex);
	tty->refcnt--;
	if (tty->refcnt == 0) {
		tty->flag &= TTY_FLAG_NOT_USED;
		if (tty->bops->poll_notify != NULL) {
			tty->bops->poll_notify(tty->backend_drv, POLLHUP);
		}
	}
	(void)tty_mutex_unlock(&tty->mutex);
}

static struct tty_operations g_tty_ops __read_mostly = {
	.open = tty_ldisc_open,
	.close = tty_ldisc_close,
	.write = tty_ldisc_write,
	.read = tty_ldisc_read,
	.ioctl = tty_ldisc_ioctl,
	.poll = tty_ldisc_poll,
};

struct tty_operations *current_tty_operations(void)
{
	return &g_tty_ops;
}
