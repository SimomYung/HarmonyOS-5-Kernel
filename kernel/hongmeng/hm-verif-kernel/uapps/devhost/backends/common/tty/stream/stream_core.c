/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Stream driver functions
 * Author: Huawei OS Kernel Lab
 * Create: Sat Mar 02 21:15:22 2019
 */

#include "stream_core.h"
#include "../tty_wrap.h"
#include "../tty_api.h"

#define STREAM_DEFAULT_TTY_OPTION TTY_OPTION_DEFAULT

/*
 * alloc one tty driver and init it,
 * then stream_driver bind with it
 */
int stream_register_tty(struct stream_device_pair *pair)
{
	int err;
	char pts_name[CHRDEV_MAX_NAME_LEN] = {0};

	tty_info("tty: stream try to register as tty\n");
	if (pair == NULL || pair->ptmx == NULL || pair->ptmx->tty_bops == NULL) {
		return -EINVAL;
	}
	err = snprintf_s(pts_name, CHRDEV_MAX_NAME_LEN,
			 CHRDEV_MAX_NAME_LEN - 1, "pts/%u", pair->slave_id);
	if (err < 0) {
		tty_warn("tty: stream: snprintf_s failed to generate pts name\n");
		return err;
	}

	/* register stream as one tty */
	pair->tty = tty_struct_register((void *)pair,
					pair->ptmx->tty_bops,
					STREAM_DEFAULT_TTY_OPTION,
					pts_name);
	if (pair->tty == NULL) {
		tty_warn("tty: stream_register_tty failed, ret NULL\n");
		return -EIO;
	}
	/* master input/output points to tty's output/input buf */
	pair->master_input = pair->tty->output_buf;
	pair->master_output = pair->tty->input_buf;
	/* m_input/output_mutex points to tty's output/input_mutex */
	pair->m_input_mutex = &pair->tty->output_mutex;
	pair->m_output_mutex = &pair->tty->input_mutex;

	return 0;
}

/* free tty and unbind with stream_driver */
void stream_unregister_tty(struct stream_device_pair *pair)
{
	if (pair == NULL || pair->tty == NULL) {
		return;
	}
	tty_struct_unregister(pair->tty);
	pair->tty = NULL;
}
