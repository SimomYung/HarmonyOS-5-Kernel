/*
 * vcodec_vdec_irq.c
 *
 * This is irq implement used in formal version
 *
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
#include "vcodec_vdec_irq.h"
#include "vcodec_vdec_irq_handler.h"
#include "vcodec_osal.h"
#include "dbg.h"

struct vcodec_vdec_irq_info {
	uint32_t irq_num;
	uint32_t irq_handle;
	uint8_t *irq_name;
	irqreturn_t (*irq_callback) (int, void *);
};

struct vcodec_vdec_irq_ctx {
	int32_t irq_count;
	struct vcodec_vdec_irq_info irq_tbl[MAX_VDEC_NORM_IRQ];
};

static struct vcodec_vdec_irq_ctx* irq_ctx(void)
{
	static struct vcodec_vdec_irq_ctx ctx = {
		MAX_VDEC_NORM_IRQ,
		{
#ifdef VDEC_MCORE_ENABLE
			{0, 0, "vdec_ipc_int0_irq", vdec_ipc_ack_isr},
			{0, 0, "vdec_ipc_mbx0_irq", vdec_ipc_mbx0_isr},
#else
			{0, 0, "vdec_ipc_int0_irq", NULL},
			{0, 0, "vdec_ipc_mbx0_irq", NULL},
#endif
			{0, 0, "vdec_watch_dog_irq",vdec_watch_dog_isr},
			{0, 0, "vdec_bsp_norm_irq", vdec_bsp_isr},
			{0, 0, "vdec_pxpc_norm_irq",vdec_pxp_isr},
			{0, 0, "vdec_scd_norm_irq", vdec_scd_isr},
			{0, 0, "vdec_norm_irq", vdec_others_isr},
			{0, 0, "vdec_hts_irq", NULL}
		}
	};
	return &ctx;
}

void vcodec_vdec_enable_irq(vdec_norm_irq_num irq_num)
{
	struct vcodec_vdec_irq_ctx* ctx = irq_ctx();
	if (irq_num >= MAX_VDEC_NORM_IRQ) {
		dprint(PRN_ERROR, "error irq enum %d", irq_num);
		return;
	}
	vcodec_osal_enable_irq(ctx->irq_tbl[irq_num].irq_num);
	dprint(PRN_ALWS, "irq enum %d", irq_num);
}

void vcodec_vdec_disable_irq(vdec_norm_irq_num irq_num)
{
	struct vcodec_vdec_irq_ctx* ctx = irq_ctx();
	if (irq_num >= MAX_VDEC_NORM_IRQ) {
		dprint(PRN_ERROR, "error irq enum %d", irq_num);
		return;
	}
	vcodec_osal_disable_irq(ctx->irq_tbl[irq_num].irq_num);
	dprint(PRN_ALWS, "irq enum %d", irq_num);
}

struct vcodec_vdec_irq_ctx* vcodec_vdec_irq_request_ctx(
	vdec_dts *dts_info, uint8_t *reg_base)
{
	int32_t i = 0;
	int32_t ret = 0;
	struct vcodec_vdec_irq_ctx* ctx = irq_ctx();

	if (unlikely(!reg_base) || unlikely(!dts_info))
		return NULL;

	for (i = 0; i < ctx->irq_count; i++) {
		if (!ctx->irq_tbl[i].irq_callback)
			continue;

		ret = vcodec_osal_request_irq(dts_info->irq_norm[i],
			(vfmw_irq_handler_t)ctx->irq_tbl[i].irq_callback,
			IRQF_SHARED, ctx->irq_tbl[i].irq_name,
			(void *)&ctx->irq_tbl[i].irq_handle);
		vfmw_assert_goto_prnt(ret == 0,
			request_irq_error, "irq request %ld failed\n", i);
		ctx->irq_tbl[i].irq_num = dts_info->irq_norm[i];
	}
	/* default disable bsp/pxp irq */
	vcodec_vdec_disable_irq(BSP_NORM_IRQ);
	vcodec_vdec_disable_irq(PXPC_NORM_IRQ);
	return ctx;

request_irq_error:
	vcodec_vdec_irq_free_ctx(ctx);
	return NULL;
}

void vcodec_vdec_irq_free_ctx(struct vcodec_vdec_irq_ctx* ctx)
{
	int32_t i;

	if (unlikely(ctx != irq_ctx()))
		return;

	for (i = 0; i < ctx->irq_count; i++) {
		if (ctx->irq_tbl[i].irq_num) {
			vcodec_osal_free_irq(ctx->irq_tbl[i].irq_num,
				(void *)&ctx->irq_tbl[i].irq_handle);
			ctx->irq_tbl[i].irq_num = 0;
			ctx->irq_tbl[i].irq_handle = 0;
		}
	}
}

