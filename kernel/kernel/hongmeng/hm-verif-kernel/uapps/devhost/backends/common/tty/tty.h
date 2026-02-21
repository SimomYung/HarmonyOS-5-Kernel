/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Define structures and features of tty
 * Author: Huawei OS Kernel Lab
 * Create: Sun Mar 10 18:42:56 2019
 */

#ifndef TTY_H
#define TTY_H

#include "tty_chrdev.h"
#include "tty_wrap.h"
#include "tty_line_buf.h"

struct tty_struct;
struct tty_context;

/* global tty driver struct which represent the whole tty */
struct tty_driver {
	struct tty_chrdrv		chrdrv;
	unsigned int			num; /* total tty dev number of the whole driver */
	unsigned int			max_num; /* max available tty dev number of the whole driver */
	const char			*name;
	const struct tty_operations	*ops;
	struct tty_mutex_t		mutex; /* the mutex lock of tty driver */
	struct tty_list_node		tty_list;
};

struct tty_flow_ctl {
	unsigned int			state; /* the flow stop flag of tty */
	struct tty_mutex_t		mutex; /* the mutex lock to protect flow state chane of tty */
	struct tty_cond_t		cond; /* the conditional variable of flow_ctl */
};

/* Stream support at most 64 device and plus one serial device to 65 */
#define TTY_MAX_DEV_NUM 65

#define TTY_DEFAULT_MAJOR 4
#define TTY_DEFAULT_BASE_MINOR 0
#define TTY_DEV_NAME "tty"

/* basic tty struct which represent one tty device */
struct tty_struct {
	struct tty_driver		*drv;
	struct tty_chrdev		*dev;
	unsigned int			id; /* the tty number of current tty */
	pid_t				sid; /* session id */
	pid_t				pgrp; /* foreground process group id */
	struct tty_line_buf		*input_buf;
	struct tty_line_buf		*output_buf;
	tty_poll_wq_t			wq;
	/* the lock to protect the data consistency of reading/writing to tty */
	struct tty_mutex_t		input_mutex;
	struct tty_mutex_t		output_mutex;

	const struct tty_backend_operations	*bops; /* pointer to the backend half driver's common ops */
	void				*backend_drv; /* point to the backend half driver of tty */
	unsigned int			options; /* the extern option flag of tty */
	struct termios 			tio; /* the termios attribution of tty */
	struct winsize			winsz; /* window size */
	unsigned int			default_options; /* the default option for tty to reset itself */
	struct tty_list_node		node;
	unsigned int			refcnt;
	unsigned int			flag; /* tty state flag */
	struct tty_mutex_t		mutex; /* the mutex to protect refcnt and flag of tty */
	struct tty_flow_ctl		flow_ctl;
};

struct tty_operations {
	int	(*open)(struct tty_context *ctx);
	void	(*close)(const struct tty_context *ctx);
	int	(*write)(const struct tty_context *ctx, const char *buf, ssize_t count);
	int	(*read)(const struct tty_context *ctx, char *buf, ssize_t count);
	int	(*ioctl)(struct tty_context *ctx, unsigned int cmd, unsigned long arg);
	unsigned int	(*poll)(const struct tty_context *ctx);
};

/*
 * To fit liblinux, we have to redefine these cmds again.
 * We can include libtty/tty.h in udk
 */
#define TTY_RESET_SETTING 0x1003
#define TTY_SERIAL_CONTROL 0x1004
#define TTY_SERIAL_OOFF 0x0
#define TTY_SERIAL_OON 0x1
#define TTY_SERIAL_IOFF 0x2
#define TTY_SERIAL_ION 0x3
#define TTY_STAT_INFO 0x1005

struct tty_info_s {
	pid_t sid;
	pid_t pgrp;
	unsigned int flag;
	struct tty_line_buf_info_s i_line_buf_info;
	struct tty_line_buf_info_s o_line_buf_info;
};

#define file_nonblock(flag) (((flag) & O_NONBLOCK) == O_NONBLOCK)
#define file_block(flag) (((flag) & O_NONBLOCK) != O_NONBLOCK)

/* store current tty_struct in file->private */
struct tty_context {
	struct tty_struct *tty;
	unsigned int flag;
};

struct tty_backend_operations {
	int	(*putc)(void *backend, char ch);
	int	(*flush)(void *backend);
	int	(*set_speed)(void *backend, unsigned int speed);
	int	(*get_speed)(void *backend, unsigned int *speed);
	void	(*stop_tx)(void *backend);
	void	(*start_tx)(void *backend);
	void	(*stop_rx)(void *backend);
	void	(*start_rx)(void *backend);
	int	(*ioctl)(void *backend, unsigned int cmd, unsigned long arg);
	int	(*set_termios)(void *backend, const struct termios *tio);
	int	(*get_termios)(void *backend, struct termios *tio);
	int	(*poll_notify)(void *backend, unsigned int event);
	int	(*get_icount)(void *backend, struct serial_icounter_struct *ic);
	int	(*break_ctl)(void *backend, int state);
};

#define TTY_FLAG_BE_USING 0x1U
#define TTY_FLAG_NOT_USED ~(0x1U)
#define TTY_FLAG_REMOVED (0x1U << 1)
#define TTY_FLAG_DEFAULT 0x1U /* TTY_FLAG_BE_USING & ~(TTY_FLAG_REMOVED) */

#define UART_ASCII_NUL 0x0 /* ASCII value of NULL */
#define UART_ASCII_CTRL_C 0x3 /* ASCII value of Ctrl-C */
#define UART_ASCII_CTRL_D 0x4 /* ASCII value of Ctrl-D */
#define UART_ASCII_CTRL_H 0x8 /* ASCII value of Ctrl-H */
#define UART_ASCII_TAB 0x9 /* ASCII value of TAB */
#define UART_ASCII_NL 0xA /* ASCII value of New Line(\n) */
#define UART_ASCII_CR 0xD /* ASCII value of Carriage Return(Enter,\r) */
#define UART_ASCII_CTRL_Q 0x11 /* ASCII value of Ctrl-Q */
#define UART_ASCII_CTRL_S 0x13 /* ASCII value of Ctrl-S */
#define UART_ASCII_CTRL_U 0x15 /* ASCII value of Ctrl-U */
#define UART_ASCII_CTRL_W 0x17 /* ASCII value of Ctrl-W */
#ifdef CONFIG_SERIAL_KSHORTCUT_SYS_PAUSE
#define UART_ASCII_CTRL_X 0x18 /* ASCII value of Ctrl-X */
#endif
#ifdef CONFIG_SERIAL_KSHORTCUT_TRIGGER_SNAPSHOT
#define UART_ASCII_CTRL_T 0x14 /* ASCII value of Ctrl-T */
#endif
#define UART_ASCII_CTRL_Z 0x1A /* ASCII value of Ctrl-Z */
#define UART_ASCII_CTRL_BACKSLASH 0x1C /* ASCII value of Ctrl-\ */

#ifdef CONFIG_SERIAL_KSHORTCUT_SYS_PAUSE
#define UART_ASCII_SYS_PAUSE UART_ASCII_CTRL_X
#endif
#ifdef CONFIG_SERIAL_KSHORTCUT_TRIGGER_SNAPSHOT
#define UART_ASCII_SNAPSHOT UART_ASCII_CTRL_T
#endif

#define TTY_OPTION_EMPTY 0x0U /* the empty value of tty option setting */
#define TTY_OPTION_DEFAULT TTY_OPTION_EMPTY /* the default value of tty option */
#define TTY_OPTION_HANDLE_DEBUG_SHORTCUT 0x1 /* the last bit means handle debug shortcuts like ctrlX */

/*
 * Reserved upgrade space here, greater than zero value can be used to represent
 * the break signal duration, so TTY_BREAK_CTL_ENABLE is defined as -1.
 */
#define TTY_BREAK_CTL_ENABLE (-1)
#define TTY_BREAK_CTL_DISABLE 0

/*
 * We better ensure that TTY_MAX_RW_BUF_SIZE is less than MAX_LINE_BUF_SIZE of
 * tty linebuf, otherwise a single write operation may exceed the buf size.
 */
#define TTY_MAX_RW_BUF_SIZE 1024U

#endif
