/*
 * Copyright (c) 2019-2020 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef VCODEC_VDEC_IRQ_HANDLER_H
#define VCODEC_VDEC_IRQ_HANDLER_H
#include <linux/interrupt.h>

irqreturn_t vdec_scd_isr(int irq, void *id);
irqreturn_t vdec_bsp_isr(int irq, void *id);
irqreturn_t vdec_pxp_isr(int irq, void *id);
#ifdef VDEC_MCORE_ENABLE
irqreturn_t vdec_ipc_ack_isr(int irq, void *id);
irqreturn_t vdec_ipc_mbx0_isr(int irq, void *id);
#endif
irqreturn_t vdec_watch_dog_isr(int irq, void *id);
irqreturn_t vdec_others_isr(int irq, void *id);
#endif
