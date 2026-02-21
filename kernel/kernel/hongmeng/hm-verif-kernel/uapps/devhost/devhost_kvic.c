/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: KVIC interrupt support
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 13 14:39:12 2019
 */
#include "devhost_irq.h"

#include <hmkernel/interrupt/kvic.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <libhmlog/hmlog.h>
#include <libhwsecurec/securec.h>

static struct kvic_input {
	char input_buffer[KVIC_UART_DATA_MAX];
	unsigned int input_len;
} *kvic_input;

/* fetch input to input_buffer */
void devhost_kvic_getinput(const struct __kvic_uart_read_data *pdata)
{
	/* no valid input handler, return directly */
	if (kvic_input == NULL) {
		return;
	}
	/* we should cooperate with kernel: kernel will ensure that frame->data
	 * won't be changed until next interrupt */
	NOFAIL(memcpy_s(&kvic_input->input_buffer[0], sizeof(kvic_input->input_buffer),
			pdata->data, pdata->datalen));
	kvic_input->input_len = pdata->datalen;
}

int __devhost_kvic_prepare_irq(unsigned int kvic_irq_type,
			     void *arg, unsigned int arg_len)
{
	if (arg == NULL) {
		return -EINVAL;
	}
	/* current we only has input kvic, don't check for simplicity, and need
	 * input_buffer_size same with kvic */
	if (arg_len != KVIC_UART_DATA_MAX) {
		hm_error("devhost: kvic need KVIC_UART_DATA_MAX length of input\n");
		return -EINVAL;
	}

	kvic_input = (struct kvic_input *)arg;

	hm_info("devhost: kvic prepare irq successfully, irq_type=%u\n",
		kvic_irq_type);
	return 0;
}
