/*
 * Huawei spark Driver
 *
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#include <securec.h>
#include <asm/ioctls.h>
#include <linux/module.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/vmalloc.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <linux/completion.h>
#include "sparklink.h"

#define SPARKLINK_UART_STATE_EMPTY 0
#define SPARKLINK_UART_STATE_FULL 1

#define SPARKLINK_UART_OPEN 0
#define SPARKLINK_UART_CLOSE 1

#define SPARKLINK_CLEAR_RECV 0
#define SPARKLINK_SET_RECV 1

/* record uart write state when call api of sparklink_uart_write;
   and distinguish uart read/write process */
enum SPARKLINK_UART_WRITE_STATE {
	SPARKLINK_WRITE_DATA_START = 0,
	SPARKLINK_WRITE_DATA_END,
	SPARKLINK_WRITE_DATA_DEFAULT
};
#define SPARKLINK_CLEAR_SEND 0
#define SPARKLINK_SET_SEND 1

#define SPARKLINK_CTR_NO_INIT 0
#define SPARKLINK_CTR_INIT 1
#define PUBLIC_BUF_MAX (8 * 1024)
#define SPARKLINK_UART_WRITE_TIMEOUT 1000

#define SPARKLINK_UART_SEND_MAX 4000

static int g_sparklink_log_mask = SPARKLINK_LOG_INFO;
static u32 g_uart_desc = SPARKLINK_UART;

int sparklink_log_level(void)
{
	return g_sparklink_log_mask;
}

struct _sparklink_uart_recv {
	u8 buf[SPARKLINK_UART_BUFF_LEN];
	int len;
	int buf_state;
	int recv_state;
	int send_state;
	int write_complete;
	int controll_init;
	int open_flag;
	int read_buf_len;
	int read_callback_flag;
	uart_read_callback func_find_mode;
	spinlock_t uart_lock;
	struct completion recv_complete;
	struct completion send_complete;
};

static struct _sparklink_uart_recv g_uart_controll = {
	.buf = {0},
	.buf_state = SPARKLINK_UART_STATE_EMPTY,
	.recv_state = SPARKLINK_CLEAR_RECV,
	.send_state = SPARKLINK_CLEAR_SEND,
	.write_complete = SPARKLINK_WRITE_DATA_DEFAULT,
	.controll_init = SPARKLINK_CTR_NO_INIT,
	.open_flag = SPARKLINK_UART_CLOSE,
};
static struct tty_struct *g_sparklink_tty = NULL;
static spinlock_t *g_uart_lock = &g_uart_controll.uart_lock;
static struct completion *g_recv_complete = &g_uart_controll.recv_complete;
static struct completion *g_send_complete = &g_uart_controll.send_complete;

static int sparklink_tty_open(struct tty_struct *tty)
{
	if (!tty)
		return -EINVAL;

	/* don't do an wakeup for now */
	clear_bit(TTY_DO_WRITE_WAKEUP, &tty->flags);

	/* set mem already allocated */
	tty->receive_room = PUBLIC_BUF_MAX;
	/* Flush any pending characters in the driver and discipline. */
	tty_ldisc_flush(tty);
	tty_driver_flush_buffer(tty);
	sparklink_infomsg("tty: open\n");
	return 0;
}

static void sparklink_tty_close(struct tty_struct *tty)
{
	sparklink_infomsg("tty: close\n");
	return;
}

static void sparklink_tty_receive(struct tty_struct *tty,
	const unsigned char *cp, char *fp, int count)
{
	int ret;
	int receive_time = 0;

	if (!cp || count > SPARKLINK_UART_BUFF_LEN || count < 0)
		return;

	spin_lock_bh(g_uart_lock);
	if (g_uart_controll.read_callback_flag == 1 && g_uart_controll.func_find_mode != NULL) {
		g_uart_controll.func_find_mode(cp, count);
		spin_unlock_bh(g_uart_lock);
		return;
	}
	spin_unlock_bh(g_uart_lock);

	while(receive_time < SPARKLINK_UART_RECEIVE_TIME) {
		spin_lock_bh(g_uart_lock);
		if (g_uart_controll.recv_state == SPARKLINK_SET_RECV) {
			if (receive_time > 0)
				sparklink_errmsg("receive_time retry %d\n", receive_time);
			break;
		}
		spin_unlock_bh(g_uart_lock);
		/* to avoid execute many times msleep
		when there is no read or write operation and trigger tty_receive. */
		if (g_uart_controll.write_complete == SPARKLINK_WRITE_DATA_START)
			msleep(SPARKLINK_UART_RECEIVE_INTERVAL);

		if (++receive_time >= SPARKLINK_UART_RECEIVE_TIME)
			return;
	}

	mb();
	/* optimize the read buff size */
	if (g_uart_controll.len + count > SPARKLINK_UART_BUFF_LEN)
		count = SPARKLINK_UART_BUFF_LEN - g_uart_controll.len;

	ret = memcpy_s(g_uart_controll.buf + g_uart_controll.len,
		SPARKLINK_UART_BUFF_LEN - g_uart_controll.len, cp, count);
	if (ret != EOK) {
		sparklink_errmsg("memcpy_s err\n");
		spin_unlock_bh(g_uart_lock);
		return;
	}
	g_uart_controll.len += count;
	if (g_uart_controll.len >= g_uart_controll.read_buf_len) {
		g_uart_controll.buf_state = SPARKLINK_UART_STATE_FULL;
		g_uart_controll.recv_state = SPARKLINK_CLEAR_RECV;
		complete(g_recv_complete);
	}
	spin_unlock_bh(g_uart_lock);

	return;
}

static void sparklink_tty_wakeup(struct tty_struct *tty)
{
	if (!tty)
		return;

	/* clear wakeup flage for now */
	clear_bit(TTY_DO_WRITE_WAKEUP, &tty->flags);

	spin_lock_bh(g_uart_lock);
	if (g_uart_controll.send_state != SPARKLINK_SET_SEND) {
		spin_unlock_bh(g_uart_lock);
		return;
	}
	g_uart_controll.send_state = SPARKLINK_CLEAR_SEND;
	spin_unlock_bh(g_uart_lock);

	complete(g_send_complete);
	return;
}

static void sparklink_tty_flush_buffer(struct tty_struct *tty)
{
	sparklink_infomsg("tty: flush\n");
	return;
}

static struct tty_ldisc_ops sparklink_ldisc_ops = {
	.magic = TTY_LDISC_MAGIC,
	.name = "sparklink_tty",
	.open = sparklink_tty_open,
	.close = sparklink_tty_close,
	.receive_buf = sparklink_tty_receive,
	.write_wakeup = sparklink_tty_wakeup,
	.flush_buffer = sparklink_tty_flush_buffer,
	.owner = THIS_MODULE
};

static void sparklink_ktty_set_termios(struct tty_struct *tty, long baud_rate)
{
	struct ktermios ktermios;

	if (!tty)
		return;

	ktermios = tty->termios;
	/* close soft flowctrl */
	ktermios.c_iflag = 0;
	/* set uart cts/rts flowctrl */
	ktermios.c_cflag &= ~CRTSCTS;
	/* set csize */
	ktermios.c_cflag &= ~(CSIZE);
	ktermios.c_cflag |= CS8;
	/* set uart baudrate */
	ktermios.c_cflag &= ~CBAUD;
	ktermios.c_cflag |= BOTHER;
	tty_termios_encode_baud_rate(&ktermios, baud_rate, baud_rate);
	tty_set_termios(tty, &ktermios);
	sparklink_errmsg("set baud_rate=%d, except=%d\n",
		(int)tty_termios_baud_rate(&tty->termios), (int)baud_rate);
}

static int sparklink_uart_open(const char *uart_port, long baud, dev_t *dev_no)
{
	int ret;

	if (!uart_port || !dev_no)
		return -EINVAL;

	/* open tty */
	g_sparklink_tty = tty_kopen(*dev_no);
	if (IS_ERR_OR_NULL(g_sparklink_tty)) {
		sparklink_errmsg("open tty %s failed\n", uart_port);
		goto SPARKLINK_KOPEN_ERROR;
	}

	if (g_sparklink_tty->ops && g_sparklink_tty->ops->open) {
		ret = g_sparklink_tty->ops->open(g_sparklink_tty, NULL);
	} else {
		sparklink_errmsg("tty->ops->open is NULL\n");
		ret = -ENODEV;
	}

	if (ret) {
		tty_unlock(g_sparklink_tty);
		goto SPARKLINK_OPEN_ERROR;
	}

	sparklink_ktty_set_termios(g_sparklink_tty, baud);
	tty_unlock(g_sparklink_tty);
	ret = tty_set_ldisc(g_sparklink_tty, g_uart_desc);
	sparklink_infomsg("g_uart_desc value is  %d\n", g_uart_desc);
	if (ret) {
		sparklink_errmsg("tty_set_ldisc error\n");
		goto SPARKLINK_LDISC_ERROR;
	}
	g_uart_controll.open_flag = SPARKLINK_UART_OPEN;

	return 0;
SPARKLINK_LDISC_ERROR:
	if (g_sparklink_tty->ops->close)
		g_sparklink_tty->ops->close(g_sparklink_tty, NULL);
SPARKLINK_OPEN_ERROR:
	tty_kclose(g_sparklink_tty);
SPARKLINK_KOPEN_ERROR:
	tty_unregister_ldisc(g_uart_desc);
	g_sparklink_tty = NULL;
	return -1;
}

static int sparklink_uart_init(const char *uart_port, long baud)
{
	int ret;
	dev_t dev_no;

	if (!uart_port)
		return -EINVAL;

	if (g_uart_controll.open_flag == SPARKLINK_UART_OPEN) {
		sparklink_errmsg("init have done\n");
		return 0;
	}
	g_uart_desc = hi116x_get_uart_desc();
	ret = tty_register_ldisc(g_uart_desc, &sparklink_ldisc_ops);
	if (ret != 0)
		sparklink_errmsg("tty_register_ldisc error ret=%d\n", ret);

	ret = tty_dev_name_to_number(uart_port, &dev_no);
	if (ret != 0) {
		sparklink_errmsg("dev to number tty:%s ret=%d\n", uart_port, ret);
		tty_unregister_ldisc(g_uart_desc);
		return -1;
	}

	spin_lock_bh(g_uart_lock);
	g_uart_controll.read_callback_flag = 0;
	g_uart_controll.func_find_mode = NULL;
	spin_unlock_bh(g_uart_lock);

	ret = sparklink_uart_open(uart_port, baud, &dev_no);
	if (ret < 0)
		return -1;

	sparklink_infomsg("%s end\n", uart_port);

	return 0;
}

static void sparklink_uart_clear_state(void)
{
	if (!g_sparklink_tty)
		return;

	tty_ldisc_flush(g_sparklink_tty);
	tty_driver_flush_buffer(g_sparklink_tty);

	spin_lock_bh(g_uart_lock);
	g_uart_controll.buf_state = SPARKLINK_UART_STATE_EMPTY;
	g_uart_controll.recv_state = SPARKLINK_CLEAR_RECV;
	g_uart_controll.len = 0;
	spin_unlock_bh(g_uart_lock);
	return;
}

static void sparklink_reconfig_baudrate(long baud)
{
	if (!g_sparklink_tty)
		return;

	sparklink_ktty_set_termios(g_sparklink_tty, baud);
	return;
}

static int sparklink_uart_read(u8 *buf, size_t *len, int time_out, int want_len)
{
	int ret;

	if (!buf || !len)
		return -EINVAL;

	if (!g_sparklink_tty) {
		*len = 0;
		return -1;
	}

	spin_lock_bh(g_uart_lock);
	g_uart_controll.buf_state = SPARKLINK_UART_STATE_EMPTY;
	g_uart_controll.recv_state = SPARKLINK_SET_RECV;
	g_uart_controll.len = 0;
	g_uart_controll.read_buf_len = want_len;
	spin_unlock_bh(g_uart_lock);

	if (!wait_for_completion_timeout(g_recv_complete, msecs_to_jiffies(time_out))) {
		*len = 0;
		spin_lock_bh(g_uart_lock);
		g_uart_controll.recv_state = SPARKLINK_CLEAR_RECV;
		spin_unlock_bh(g_uart_lock);
		sparklink_errmsg("sparklink_uart_read timeout\n");
		return -EIO;
	}

	spin_lock_bh(g_uart_lock);
	*len = g_uart_controll.len;
	ret = memcpy_s(buf, SPARKLINK_UART_BUFF_LEN, g_uart_controll.buf, g_uart_controll.len);
	if (ret != EOK) {
		sparklink_errmsg("memcpy_s err\n");
		spin_unlock_bh(g_uart_lock);
		return -1;
	}
	g_uart_controll.buf_state = SPARKLINK_UART_STATE_EMPTY;
	g_uart_controll.len = 0;
	spin_unlock_bh(g_uart_lock);

	return 0;
}

static int sparklink_uart_write(u8 *buf, size_t len)
{
	int remain = len;
	int size_send, addr;
	int ret;

	if (!buf || !g_sparklink_tty)
		return -EINVAL;

	addr = 0;
	while (remain > 0) {
		size_send = SPARKLINK_UART_SEND_MAX > remain ? remain : SPARKLINK_UART_SEND_MAX;
		set_bit(TTY_DO_WRITE_WAKEUP, &g_sparklink_tty->flags);

		spin_lock_bh(g_uart_lock);
		g_uart_controll.send_state = SPARKLINK_SET_SEND;
		g_uart_controll.write_complete = SPARKLINK_WRITE_DATA_START;
		spin_unlock_bh(g_uart_lock);

		if (g_sparklink_tty->ops && g_sparklink_tty->ops->write) {
			ret = g_sparklink_tty->ops->write(g_sparklink_tty, buf + addr, size_send);
		} else {
			sparklink_errmsg("tty->ops->write is NULL\n");
			ret = -ENODEV;
		}
		if (ret < 0)
			return -EIO;
		if (!wait_for_completion_timeout(g_send_complete,
				msecs_to_jiffies(SPARKLINK_UART_WRITE_TIMEOUT))) {
			spin_lock_bh(g_uart_lock);
			g_uart_controll.send_state = SPARKLINK_CLEAR_SEND;
			g_uart_controll.write_complete = SPARKLINK_WRITE_DATA_END;
			spin_unlock_bh(g_uart_lock);
			sparklink_errmsg("sparklink_uart_write timeout\n");
			return -EIO;
		}
		remain -= size_send;
		addr += size_send;
	}
	spin_lock_bh(g_uart_lock);
	g_uart_controll.write_complete = SPARKLINK_WRITE_DATA_END;
	spin_unlock_bh(g_uart_lock);
	return 0;
}

static int sparklink_uart_deinit(void)
{
	if (g_uart_controll.open_flag != SPARKLINK_UART_OPEN)
		return 0;

	if (!g_sparklink_tty || !g_sparklink_tty->ops || !g_sparklink_tty->ops->close)
		return -1;

	tty_ldisc_release(g_sparklink_tty);
	g_sparklink_tty->ops->close(g_sparklink_tty, NULL);
	tty_kclose(g_sparklink_tty);
	tty_unregister_ldisc(g_uart_desc);
	g_sparklink_tty = NULL;

	spin_lock_bh(g_uart_lock);
	g_uart_controll.send_state = SPARKLINK_CLEAR_SEND;
	g_uart_controll.recv_state = SPARKLINK_CLEAR_RECV;
	g_uart_controll.buf_state = SPARKLINK_UART_STATE_EMPTY;
	g_uart_controll.open_flag = SPARKLINK_UART_CLOSE;
	g_uart_controll.read_callback_flag = 0;
	g_uart_controll.func_find_mode = NULL;
	spin_unlock_bh(g_uart_lock);

	return 0;
}

static int sparklink_uart_read_start(uart_read_callback func)
{
	if (!func)
		return -EINVAL;

	spin_lock_bh(g_uart_lock);
	g_uart_controll.read_callback_flag = 1;
	g_uart_controll.func_find_mode = func;
	g_uart_controll.buf_state = SPARKLINK_UART_STATE_EMPTY;
	g_uart_controll.len = 0;
	spin_unlock_bh(g_uart_lock);
	return 0;
}

static void sparklink_uart_read_end(void)
{
	spin_lock_bh(g_uart_lock);
	g_uart_controll.read_callback_flag = 0;
	g_uart_controll.func_find_mode = NULL;
	g_uart_controll.buf_state = SPARKLINK_UART_STATE_FULL;
	g_uart_controll.len = 0;
	spin_unlock_bh(g_uart_lock);
	return;
}

static struct sparklink_uart_ops uart_ops = {
	.uart_init = sparklink_uart_init,
	.uart_clear = sparklink_uart_clear_state,
	.reconfig_baudrate = sparklink_reconfig_baudrate,
	.uart_read = sparklink_uart_read,
	.uart_read_start = sparklink_uart_read_start,
	.uart_read_end = sparklink_uart_read_end,
	.uart_write = sparklink_uart_write,
	.uart_deinit = sparklink_uart_deinit,
};

static int __init sparklink_uart_module_init(void)
{
	sparklink_infomsg("sparklink_uart_module_init enter\n");
	if (g_uart_controll.controll_init == SPARKLINK_CTR_NO_INIT) {
		spin_lock_init(g_uart_lock);
		init_completion(g_recv_complete);
		init_completion(g_send_complete);
		g_uart_controll.controll_init = SPARKLINK_CTR_INIT;
	}
	return sparklink_uart_ops_register(&uart_ops);
}

static void __exit sparklink_uart_module_exit(void)
{
	sparklink_infomsg("sparklink_uart_module_exit\n");
	return;
}

module_init(sparklink_uart_module_init);
module_exit(sparklink_uart_module_exit);
MODULE_AUTHOR("Huawei Technologies Co., Ltd.");
MODULE_DESCRIPTION("Huawei sparklink uart Driver");
MODULE_LICENSE("GPL v2");
