/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include "securec.h"
#include "mdrv_diag.h"
#include "pcs_def.h"
#include "pcs_log.h"
#include "ps_log_ue_interface.h"

u32 pcs_log_level = PCS_LOG_LVL_ERR | PCS_LOG_LVL_HIGH;
module_param(pcs_log_level, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(pcs_log_level, "pcs log level");

void pcs_trans_report(u32 msgid, void *data, u32 len)
{
	struct ps_log_rpt_head rpt_head = { 0 };

	rpt_head.msg_id = msgid;
	rpt_head.mode = PS_LOG_MODE_COMM;
	rpt_head.modem_id = 0;
	rpt_head.level = PS_LOG_OPEN_LEVEL_0;
	rpt_head.gid = PS_LOG_GID_PS;
	rpt_head.msg_type = PS_LOG_TYPE_STRUCT;

	ps_log_report(&rpt_head, data, len);
}

STATIC void pcs_log_fill_prt_head(unsigned short print_level, struct ps_log_print_head *prt_head)
{
	prt_head->snd_pid = PCS_FIX_PID;
	prt_head->print_level = print_level;
	prt_head->gid = PS_LOG_GID_PS;
}

void pcs_log_print(unsigned short print_level, const char *string, ...)
{
	va_list args;
	char buffer[PCS_PRINT_MAX_LEN + 1];
	int ret;
	struct ps_log_print_head prt_head = {0};

	pcs_log_fill_prt_head(print_level, &prt_head);

	va_start(args, string);
	ret = vsnprintf_s(buffer, PCS_PRINT_MAX_LEN, PCS_PRINT_MAX_LEN - 1, string, args);
	va_end(args);

	if (ret < 0)
		ps_log_print(&prt_head, string);
	else
		ps_log_print(&prt_head, buffer);
}