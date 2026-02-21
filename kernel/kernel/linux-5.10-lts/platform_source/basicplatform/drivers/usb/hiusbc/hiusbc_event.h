/*
 * hiusbc_event.h -- Event Header File for Hisilicon USB Controller.
 *
 * Copyright (c) 2019 Hisilicon Technologies CO., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#ifndef __HIUSBC_EVENT_H
#define __HIUSBC_EVENT_H

#include <linux/irqreturn.h>
#include "hiusbc_core.h"

/* eventen */
#define HIUSBC_DEVT_EN_CMD_SET_ADDR (1 << 10)
#define HIUSBC_DEVT_EN_CMD_ENABLE_EP (1 << 11)
#define HIUSBC_DEVT_EN_CMD_DISABLE_EP (1 << 12)
#define HIUSBC_DEVT_EN_CMD_START_XFER (1 << 13)
#define HIUSBC_DEVT_EN_CMD_END_XFER (1 << 14)
#define HIUSBC_DEVT_EN_CMD_SET_HALT (1 << 15)
#define HIUSBC_DEVT_EN_CMD_CLEAR_HALT (1 << 16)
#define HIUSBC_DEVT_EN_CMD_FORCE_HEADER	(1 << 17)

#define HIUSBC_DEVT_TYPE_DISCONNECT (1 << 0)
#define HIUSBC_DEVT_TYPE_CONNECT (1 << 1)
#define HIUSBC_DEVT_TYPE_RST (1 << 2)
#define HIUSBC_DEVT_TYPE_PLC (1 << 3)
#define HIUSBC_DEVT_TYPE_SUSPEND (1 << 4)
#define HIUSBC_DEVT_TYPE_RESUME (1 << 5)
#define HIUSBC_DEVT_TYPE_L1_SUSPEND (1 << 6)
#define HIUSBC_DEVT_TYPE_L1_RESUME (1 << 7)
#define HIUSBC_DEVT_TYPE_SOF (1 << 8)
#define HIUSBC_DEVT_TYPE_PHY_ERR (1 << 9)

/* Event TRB */
#define HIUSBC_TRB_TYPE_XFER_EVENT 5
#define HIUSBC_TRB_TYPE_CMD_EVENT 6
#define HIUSBC_TRB_TYPE_DEV_EVENT 7
#define HIUSBC_TRB_TYPE_MF_WRAP_EVENT 8
#define HIUSBC_TRB_TYPE_VENDOR_TEST_EVENT 9

#define HIUSBC_CMPLT_CODE_INVALID 0
#define HIUSBC_CMPLT_CODE_SUCCESS 1
#define HIUSBC_CMPLT_CODE_FIFO_ERR 2
#define HIUSBC_CMPLT_CODE_BABBLE_ERR 3
#define HIUSBC_CMPLT_CODE_TRANS_ERR 4
#define HIUSBC_CMPLT_CODE_TRB_ERR 5
#define HIUSBC_CMPLT_CODE_NRDY 6
#define HIUSBC_CMPLT_CODE_SHORT 7
#define HIUSBC_CMPLT_CODE_SETUP 8
#define HIUSBC_CMPLT_CODE_MISSED 9
#define HIUSBC_CMPLT_CODE_ISO_OVERRUN 10
#define HIUSBC_CMPLT_CODE_EB_FULL 11
#define HIUSBC_CMPLT_CODE_PARM_ERR 12
#define HIUSBC_CMPLT_CODE_EP_STATE_ERR 13
#define HIUSBC_CMPLT_CODE_CMD_ABORT_ERR 14
#define HIUSBC_CMPLT_CODE_EVT_LOST 15
#define HIUSBC_CMPLT_CODE_CTRL_SHORT 16
#define HIUSBC_CMPLT_CODE_CTRL_DIR_ERR 17

struct hiusbc_evt_ring *hiusbc_event_ring_alloc(struct hiusbc *hiusbc,
	unsigned int size, unsigned int index);
void hiusbc_event_ring_free(struct hiusbc_evt_ring *ring);
dma_addr_t hiusbc_evt_trb_vrt_to_dma(
			const struct hiusbc_evt_ring *event_ring,
			const union hiusbc_trb *trb);
void hiusbc_intr_handle_evt_ring(unsigned long param);
irqreturn_t hiusbc_interrupt(int irq, void *_hiusbc);

#endif /* __HIUSBC_EVENT_H */
