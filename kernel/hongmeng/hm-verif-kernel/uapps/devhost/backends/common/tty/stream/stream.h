/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Structures and defines of stream device
 * Author: Huawei OS Kernel Lab
 * Create: Sat Mar 02 20:52:48 2019
 */

#ifndef STREAM_H
#define STREAM_H

#include "../tty.h"
#include "../tty_chrdev.h"

/* pair of devices, including tty and pty */
struct stream_device_pair {
	unsigned int			slave_id;
	/* chrdev points to ptmx devices */
	struct stream_control_device	*ptmx;
	/* tty point to pts device */
	struct tty_struct		*tty;
	struct tty_line_buf		*master_input;
	struct tty_line_buf		*master_output;
	tty_poll_wq_t			wq;
	/* the lock to protect the data consistency of reading/writing ptmx */
	struct tty_mutex_t		*m_input_mutex;
	struct tty_mutex_t		*m_output_mutex;
	/* In ptmx, one context means one pair */
	struct stream_context		*ctx;
};

struct stream_control_device {
	unsigned int			max_pair;
	/* mutex to protect the pair_map */
	struct tty_mutex_t		mutex;
	const struct tty_backend_operations	*tty_bops;
	struct tty_chrdev		*chrdev;
	/* total map about all the used and freed pairs */
	struct idr			pair_map;
};

struct stream_context {
	struct stream_device_pair *pair;
	unsigned int flag;
	void *file;
};

#define MAX_DEVICE_COUNT 64
#define DEFAULT_PTMX_MAJOR 5
#define DEFAULT_PTMX_MINOR 2
#define STREAM_PTMX_DEV_NAME "ptmx"
#define STREAM_PTS_BASE_NAME "pts"

#endif
