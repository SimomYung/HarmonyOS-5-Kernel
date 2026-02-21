/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Core functions of tty-APIs for low level drivers
 * Author: Huawei OS Kernel Lab
 * Create: Sun Mar 10 18:33:04 2019
 */
#include "tty_core.h"
#include "tty_api.h"
#include "tty_wrap.h"
#include "tty_flow.h"
#include "tty_chrdev.h"

struct tty_struct *find_tty_by_devt(dev_t devt)
{
	struct tty_driver *tty_drv = NULL;
	struct tty_struct *tty = NULL;

	tty_drv = current_tty_driver();
	if (tty_drv == NULL) {
		return NULL;
	}

	(void)tty_mutex_lock(&tty_drv->mutex);
	/* Go through the tty list to find corresponding tty by devt */
	tty_list_for_each_entry(tty, &tty_drv->tty_list, struct tty_struct, node) {
		if (tty->dev == NULL) {
			continue;
		}
		if (tty->dev->dev_no == devt) {
			(void)tty_mutex_unlock(&tty_drv->mutex);
			return tty;
		}
	}

	(void)tty_mutex_unlock(&tty_drv->mutex);
	return NULL;
}

struct tty_struct *find_tty_by_sid(pid_t sid)
{
	struct tty_driver *tty_drv = NULL;
	struct tty_struct *tty = NULL;

	tty_drv = current_tty_driver();
	if (tty_drv == NULL) {
		return NULL;
	}

	(void)tty_mutex_lock(&tty_drv->mutex);
	tty_list_for_each_entry(tty, &tty_drv->tty_list, struct tty_struct, node) {
		if (tty->sid == sid) {
			(void)tty_mutex_unlock(&tty_drv->mutex);
			return tty;
		}
	}

	(void)tty_mutex_unlock(&tty_drv->mutex);
	return NULL;
}

static void tty_struct_attr_reset(struct tty_struct *tty)
{
	/* Reset the attribution of tty */
	tty->options = tty->default_options;
	tty->sid = 0;

	/* Reset the struct of line_buf */
	tty_line_buf_reset(tty->input_buf);
	tty_line_buf_reset(tty->output_buf);

	/* Reset the default value of tty termios */
	mem_zero_s(tty->tio);
	tty->tio.c_cc[VMIN] = 1;
	tty->tio.c_cc[VEOL] = UART_ASCII_NUL;
	tty->tio.c_cc[VEOF] = UART_ASCII_CTRL_D;
	tty->tio.c_cc[VINTR] = UART_ASCII_CTRL_C;
	tty->tio.c_cc[VSUSP] = UART_ASCII_CTRL_Z;
	tty->tio.c_cc[VQUIT] = UART_ASCII_CTRL_BACKSLASH;
	tty->tio.c_cc[VERASE] = UART_ASCII_CTRL_H;
	tty->tio.c_cc[VKILL] = UART_ASCII_CTRL_U;
	tty->tio.c_cc[VWERASE] = UART_ASCII_CTRL_W;
	tty->tio.c_cc[VSTART] = UART_ASCII_CTRL_Q;
	tty->tio.c_cc[VSTOP] = UART_ASCII_CTRL_S;

	tty->tio.c_iflag = ICRNL | IXON;
	tty->tio.c_oflag = OPOST | ONLCR;
	tty->tio.c_lflag = ISIG | ICANON | ECHO | ECHOK | ECHOCTL |
			   ECHOE | IEXTEN;
	tty->tio.c_cflag = CREAD;
}

static void tty_struct_attr_init(struct tty_struct *tty)
{
	/* reset some attribute */
	tty_struct_attr_reset(tty);

	/* init the mutex of tty */
	tty_mutex_init(&tty->mutex);

	/* init the flow control struct of tty */
	tty_flow_ctl_init(&tty->flow_ctl);

	/* Init the struct of line_buf */
	tty_line_buf_init(tty->input_buf);
	tty_line_buf_init(tty->output_buf);
	tty_mutex_init(&tty->input_mutex);
	tty_mutex_init(&tty->output_mutex);

	/* init refcnt and flag */
	tty->refcnt = 0;
	tty->flag = TTY_FLAG_DEFAULT;

	/* init poll wait queue */
	tty_poll_wq_init(&tty->wq);
}

static int tty_struct_init(struct tty_struct *tty, const char *tty_name)
{
	int ret;
	struct tty_chrdrv *chrdrv = NULL;
	struct tty_chrdev *chrdev = NULL;

	if (tty == NULL || tty->dev == NULL || current_tty_driver() == NULL) {
		tty_warn("tty: invalid tty is NULL\n");
		return -EIO;
	}
	/* bind tty with tty_driver */
	tty->drv = current_tty_driver();
	chrdrv = &(tty->drv->chrdrv);
	chrdev = tty->dev;

	/* initialize the struct of tty */
	tty_struct_attr_init(tty);

	/* Add tty to drv->tty_list */
	(void)tty_mutex_lock(&tty->drv->mutex);
	tty->id = tty->drv->num;
	if (tty->id >= tty->drv->max_num) {
		tty_warn("tty: tty num %u overflow\n", tty->id);
		(void)tty_mutex_unlock(&tty->drv->mutex);
		return -ENOSPC;
	}
	tty->drv->num++;
	(void)tty_mutex_unlock(&tty->drv->mutex);

	chrdev->dev_no = MKDEV(chrdrv->major, chrdrv->minor_start) + tty->id;
	/* If not set tty_name, defaultly set name as ttyX */
	if (tty_name == NULL) {
		ret = snprintf_s(chrdev->name, CHRDEV_MAX_NAME_LEN,
				 CHRDEV_MAX_NAME_LEN - 1, "tty%u", tty->id);
	} else {
		ret = snprintf_s(chrdev->name, CHRDEV_MAX_NAME_LEN,
				 CHRDEV_MAX_NAME_LEN - 1, "%s", tty_name);
	}
	if (ret < 0) {
		return ret;
	}

	ret = tty_chrdev_create(chrdev, chrdrv, tty->drv->max_num);
	if (ret < 0) {
		tty_warn("tty: chrdev_register failed, err: %s\n", strerror(-ret));
		return ret;
	}

	(void)tty_mutex_lock(&tty->drv->mutex);
	tty_list_insert_tail(&tty->drv->tty_list, &tty->node);
	(void)tty_mutex_unlock(&tty->drv->mutex);
	return 0;
}

static void tty_struct_exit(struct tty_struct *tty)
{
	if (tty == NULL || tty->drv == NULL) {
		tty_warn("tty: try to delete NULL tty\n");
		return;
	}
	tty_chrdev_destroy(tty->dev);
	(void)tty_mutex_lock(&tty->drv->mutex);
	tty_list_delete(&tty->node);
	(void)tty_mutex_unlock(&tty->drv->mutex);
	tty->drv = NULL;
}

static int tty_struct_alloc(struct tty_struct **ttyp)
{
	struct tty_chrdev *chrdev = NULL;
	struct tty_struct *tty = NULL;
	struct tty_line_buf *input_buf = NULL;
	struct tty_line_buf *output_buf = NULL;

	input_buf = tty_mem_zalloc(sizeof(struct tty_line_buf));
	if (input_buf == NULL) {
		return -ENOMEM;
	}
	output_buf = tty_mem_zalloc(sizeof(struct tty_line_buf));
	if (output_buf == NULL) {
		tty_mem_free(input_buf);
		return -ENOMEM;
	}

	/* alloc memory for tty_device, currently
	 * we only support one tty_device for one tty
	 */
	chrdev = tty_mem_zalloc(sizeof(struct tty_chrdev));
	if (chrdev == NULL) {
		tty_mem_free(input_buf);
		tty_mem_free(output_buf);
		return -ENOMEM;
	}
	tty = tty_mem_zalloc(sizeof(struct tty_struct));
	if (tty == NULL) {
		tty_mem_free(input_buf);
		tty_mem_free(output_buf);
		tty_mem_free(chrdev);
		return -ENOMEM;
	}

	/* bind tty_drv and chrdev */
	tty->dev = chrdev;
	tty->input_buf = input_buf;
	tty->output_buf = output_buf;
	tty->sid = 0;

	*ttyp = tty;

	return 0;
}

static void tty_struct_free(struct tty_struct *tty)
{
	if (tty == NULL) {
		return;
	}
	if (tty->dev != NULL) {
		tty_mem_free(tty->dev);
	}
	if (tty->input_buf != NULL) {
		tty_line_buf_destroy(tty->input_buf);
	}
	if (tty->output_buf != NULL) {
		tty_line_buf_destroy(tty->output_buf);
	}
	tty_mem_free(tty);
}

struct tty_struct *tty_struct_register(void *backend_drv,
				       const struct tty_backend_operations *bops,
				       unsigned int tty_opt,
				       const char *tty_name)
{
	int err;
	struct tty_struct *tty = NULL;

	/* NOTE: We allow backend_drv and bops being NULL as parameters.
	 * In that case, we return a pointer to a valid tty_struct. */

	/* alloc tty */
	err = tty_struct_alloc(&tty);
	if (err < 0) {
		tty_warn("tty: serial fail to alloc tty, err: %s\n", strerror(-err));
		return NULL;
	}

	/* init tty */
	err = tty_struct_init(tty, tty_name);
	if (err < 0) {
		tty_warn("tty: serial fail to init tty, err: %s\n", strerror(-err));
		tty_struct_free(tty);
		return NULL;
	}
	tty->bops = bops;
	tty->options = tty_opt;
	tty->default_options = tty_opt;

	/* bind tty with backend drv */
	tty->backend_drv = backend_drv;

	return tty;
}
tty_export_symbol(tty_struct_register);

/*
 * Since module remove is not fully supported in devhost
 * we can not guarantee the resource releasing of tty_struct_unregister
 */
void tty_struct_unregister(struct tty_struct *tty)
{
	if (tty == NULL) {
		tty_warn("tty: tty_struct_unregister arg is NULL!\n");
		return;
	}
	tty_struct_exit(tty);
	tty_struct_free(tty);
}
tty_export_symbol(tty_struct_unregister);

/* reset tty before recycling */
void tty_struct_reset(struct tty_struct *tty)
{
	/*
	 * When bash on serial ends due to an unexpected situation, the init
	 * process will try to automatically recreate a new bash process and
	 * reset the tty. In order to ensure the consistency of the input and
	 * output status of the serial device before and after resetting, the
	 * flow status of the and some status of mutex and sem in tty is not
	 * reset and remains the same as before.
	 */
	if (tty == NULL) {
		return;
	}
	tty_struct_attr_reset(tty);
	tty_flow_reset(&tty->flow_ctl);
}
tty_export_symbol(tty_struct_reset);

/* checking tty is using or not. If not, recycle using it */
bool tty_struct_try_recycle(struct tty_struct *tty)
{
	int err;

	if (tty == NULL) {
		tty_warn("tty: tty is NULL, invalid\n");
		return false;
	}
	/* TTY can be recycled only when not used and removed */
	(void)tty_mutex_lock(&tty->mutex);
	if (((tty->flag & TTY_FLAG_BE_USING) != TTY_FLAG_BE_USING) &&
	    ((tty->flag & TTY_FLAG_REMOVED) == TTY_FLAG_REMOVED)) {
		err = tty_struct_device_reinit(tty);
		if (err < 0) {
			tty_warn("tty: reinit tty device  %s failed: %s\n",
				 tty->dev->name, strerror(-err));
			/* Set tty as using for not recycling it */
			tty->flag &= TTY_FLAG_BE_USING;
			(void)tty_mutex_unlock(&tty->mutex);
			return false;
		}
		/* Recycle tty and set its flag not TTY_FLAG_REMOVED and TTY_FLAG_BE_USING */
		tty->flag &= ~(TTY_FLAG_REMOVED);
		tty->flag |= TTY_FLAG_BE_USING;
		tty_struct_reset(tty);
		(void)tty_mutex_unlock(&tty->mutex);
		return true;
	}
	(void)tty_mutex_unlock(&tty->mutex);

	return false;
}
tty_export_symbol(tty_struct_try_recycle);

static void tty_struct_hang_up(struct tty_struct *tty, bool tty_is_using)
{
	unsigned int mask;

	/* Notify poll event to tty process */
	mask = POLLHUP | POLLERR;
	tty_poll_notify(&tty->wq, mask);
	/* wake up the potential pending reader and writer */
	tty_line_buf_wake_up_reader(tty->input_buf);
	tty_line_buf_wake_up_writer(tty->output_buf);
	/*
	 * We do not lock for sending signal because we can tolerate
	 * killing sig to non-existent process.
	 * Note: Sometimes bash will exit but close tty after tty is hung up
	 * by ptmx, so we might send signal to non-existent bash process.
	 */
	if (tty_is_using && tty->sid != 0) {
		tty_kill_process(tty->sid, SIGHUP);
		tty_kill_process(tty->sid, SIGCONT);
	}
}

void tty_struct_set_removed(struct tty_struct *tty)
{
	bool tty_is_using = true;

	if (tty == NULL) {
		tty_warn("tty: tty is NULL, invalid\n");
		return;
	}
	(void)tty_mutex_lock(&tty->mutex);
	/* Update tty->flag by refcnt before hung up */
	if (tty->refcnt > 0) {
		tty_is_using = true;
		tty->flag |= TTY_FLAG_BE_USING;
	} else {
		tty_is_using = false;
		tty->flag &= TTY_FLAG_NOT_USED;
	}
	tty->flag |= TTY_FLAG_REMOVED;
	(void)tty_mutex_unlock(&tty->mutex);
	tty_struct_hang_up(tty, tty_is_using);

	(void)tty_mutex_lock(&tty->mutex);
	/* When tty is recycled, we need to reset it */
	tty_struct_reset(tty);
	tty_chrdev_destroy(tty->dev);
	(void)tty_mutex_unlock(&tty->mutex);
}
tty_export_symbol(tty_struct_set_removed);

int tty_struct_device_reinit(struct tty_struct *tty)
{
	struct tty_chrdev *chrdev = NULL;
	struct tty_chrdrv *chrdrv = NULL;

	if (tty == NULL || tty->drv == NULL || tty->dev == NULL) {
		tty_warn("tty: tty is NULL, invalid\n");
		return -EINVAL;
	}
	chrdrv = &(tty->drv->chrdrv);
	chrdev = tty->dev;

	return tty_chrdev_create(chrdev, chrdrv, tty->drv->max_num);
}
tty_export_symbol(tty_struct_device_reinit);

/* checking tty is using or not. */
bool tty_struct_is_using(struct tty_struct *tty)
{
	bool is_using = false;

	if (tty == NULL) {
		tty_warn("tty: tty is NULL, invalid\n");
		return is_using;
	}
	(void)tty_mutex_lock(&tty->mutex);
	/* No need to update tty's flag since this func is only used in poll ops */
	is_using = ((tty->flag & TTY_FLAG_BE_USING) == TTY_FLAG_BE_USING);
	(void)tty_mutex_unlock(&tty->mutex);

	return is_using;
}
tty_export_symbol(tty_struct_is_using);
