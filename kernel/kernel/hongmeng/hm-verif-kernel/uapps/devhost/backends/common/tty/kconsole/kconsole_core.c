/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Kconsole backend core implementation
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 05 02:36:15 2020
 */

#include "kconsole_core.h"
#include "../tty_wrap.h"
#include "kconsole.h"
#include "kconsole_kvic.h"
#include "../tty_api.h"
#include "../tty_line_buf.h"

#define KCONSOLE_DEFAULT_TTY_OPTION TTY_OPTION_DEFAULT
#define KCONSOLE_MAX_PRINT_LEN 512 /* Len limited by kconsole print syscall */

static struct kconsole_device *g_kconsole_dev = NULL;

static int kconsole_flush(void *backend)
{
	int ret;
	int len = 0;
	char tx_buf[KCONSOLE_MAX_PRINT_LEN];
	struct kconsole_device *kconsole = (struct kconsole_device *)backend;

	if (kconsole == NULL) {
		return -EINVAL;
	}
	mem_zero_a(tx_buf);

	do {
		ret = tty_line_buf_read(kconsole->output_buf, tx_buf, KCONSOLE_MAX_PRINT_LEN);
		if (ret <= 0) {
			break;
		}
		if (kconsole->status == KCONSOLE_DEVICE_ENABLED) {
			/* Using syscall to send data to kconsole */
			(void)tty_kconsole_print(tx_buf, (size_t)(unsigned int)ret);
		}
		kconsole->icount.tx += ret;
		len += ret;
	} while (ret == KCONSOLE_MAX_PRINT_LEN);

	if (ret < 0) {
		return ret;
	}
	/* Wake up potential writing activation blocking on output_buf */
	if (len > 0) {
		tty_line_buf_wake_up_writer(kconsole->output_buf);
	}
	return len;
}

static int kconsole_putc(void *backend, char ch)
{
	int ret;
	struct kconsole_device *kconsole = (struct kconsole_device *)backend;
	if (kconsole == NULL) {
		return -EINVAL;
	}
	if (kconsole->status == KCONSOLE_DEVICE_ENABLED) {
		ret = tty_kconsole_print(&ch, sizeof(ch));
	} else {
		/* When kconsole is disabled, do not output */
		ret = (int)(sizeof(ch));
	}
	kconsole->icount.tx++;
	return ret;
}

static int kconsole_get_icount(void *backend,
			       struct serial_icounter_struct *icount)
{
	struct kconsole_device *kconsole = (struct kconsole_device *)backend;
	int ret;

	if (backend == NULL || icount == NULL) {
		return -EINVAL;
	}
	/* Read data from kconsole->icount, we do not lock here */
	ret = memcpy_s(icount, sizeof(struct serial_icounter_struct),
		       &kconsole->icount, sizeof(struct serial_icounter_struct));
	if (ret != EOK) {
		return -EIO;
	}
	return 0;
}

static void kconsole_enable(void *backend)
{
	int ret;
	struct kconsole_device *kconsole = (struct kconsole_device *)backend;

	if (kconsole == NULL) {
		tty_warn("tty: enable kconsole but it's NULL\n");
		return;
	}
	ret = tty_kconsole_enable();
	if (ret < 0) {
		tty_warn("tty: enable kconsole failed, ret=%d\n", ret);
		return;
	}
	kconsole->status = KCONSOLE_DEVICE_ENABLED;
}

static void kconsole_disable(void *backend)
{
	int ret;
	struct kconsole_device *kconsole = (struct kconsole_device *)backend;

	if (kconsole == NULL) {
		tty_warn("tty: enable kconsole but it's NULL\n");
		return;
	}
	ret = tty_kconsole_disable();
	if (ret < 0) {
		tty_warn("tty: disable kconsole failed, ret=%d\n", ret);
		return;
	}
	kconsole->status = KCONSOLE_DEVICE_DISABLED;
}

/* Currently kconsole do not support changing its spped and outher settings */
static const struct tty_backend_operations g_kconsole_bops = {
	.putc = kconsole_putc,
	.flush = kconsole_flush,
	.set_speed = NULL,
	.get_speed = NULL,
	/*
	 * Since kconsole only support the enabling and disabling of the whole
	 * device instead of individual input or output, the stop_tx/rx will
	 * point to one func kconsole_disable, so as start_tx/rx.
	 */
	.stop_tx = kconsole_disable,
	.start_tx = kconsole_enable,
	.stop_rx = kconsole_disable,
	.start_rx = kconsole_enable,
	.set_termios = NULL,
	.get_termios = NULL,
	.ioctl = NULL,
	.poll_notify = NULL,
	.get_icount = kconsole_get_icount,
	.break_ctl = NULL,
};

static int kconsole_register_tty(struct kconsole_device *kconsole)
{
	tty_info("tty: kconsole try to register as tty\n");
	kconsole->tty = tty_struct_register((void *)kconsole, kconsole->tty_bops,
					    KCONSOLE_DEFAULT_TTY_OPTION, NULL);
	if (kconsole->tty == NULL) {
		tty_err("tty: kconsole_register_tty failed, ret NULL\n");
		return -EFAULT;
	}
	/* kconsole's input buf points to tty's input buf */
	kconsole->input_buf = kconsole->tty->input_buf;
	kconsole->output_buf = kconsole->tty->output_buf;
	return 0;
}

/* free tty and unbind with kconsole_device */
static void kconsole_unregister_tty(struct kconsole_device *kconsole)
{
	if (kconsole == NULL) {
		tty_warn("tty: unregister tty but kconsole is NULL\n");
		return;
	}
	tty_struct_unregister(kconsole->tty);
	kconsole->tty = NULL;
}

int do_kconsole_init(void)
{
	int err;
	struct kconsole_device *kconsole = NULL;
	tty_info("tty: start to init module of backend kconsole\n");

	kconsole = (struct kconsole_device *)
		   tty_mem_zalloc(sizeof(struct kconsole_device));
	if (kconsole == NULL) {
		return -ENOMEM;
	}
	kconsole->tty_bops = &g_kconsole_bops;
	kconsole->status = KCONSOLE_DEVICE_ENABLED;

	err = kconsole_register_tty(kconsole);
	if (err < 0) {
		tty_err("kconsole_init_kvic failed, err: %s\n", strerror(-err));
		tty_mem_free(kconsole);
		return err;
	}

	/* do kvic preprae and request irq */
	err = kconsole_init_kvic(kconsole);
	if (err < 0) {
		tty_err("kconsole_init_kvic failed, err: %s\n", strerror(-err));
		kconsole_unregister_tty(kconsole);
		tty_mem_free(kconsole);
		return err;
	}
	g_kconsole_dev = kconsole;
	tty_info("tty: end of initing module of backend kconsole\n");
	return 0;
}

int do_kconsole_exit(void)
{
	if (g_kconsole_dev == NULL) {
		return -EINVAL;
	}
	tty_info("tty: module exit of kconsole backend\n");
	kconsole_unregister_tty(g_kconsole_dev);
	kconsole_uninit_kvic(g_kconsole_dev);
	tty_mem_free(g_kconsole_dev);
	g_kconsole_dev = NULL;
	return 0;
}
