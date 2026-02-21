/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2025. All rights reserved.
 * foss@huawei.com
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
 *
 */

#ifndef __DUMP_ERRNO_H__
#define __DUMP_ERRNO_H__

#include <mdrv_errno.h>

enum dump_errno {
    DUMP_ERR_END = -1024,
    DUMP_ERRNO_GET_DUMP_MEM_AREA = -19,
    DUMP_ERRNO_SUBSYS_NOT_EXISIT = -18,
    DUMP_ERRNO_GET_BASEINFO = -17,
    DUMP_ERRNO_IPC_CPNNECT = -16,
    DUMP_ERRNO_IPC_OPEN = -15,
    DUMP_ERRNO_TIMEOUT = -14,
    DUMP_ERRNO_QUEUE_WORK = -13,
    DUMP_ERRNO_CREATE_WQ = -12,
    DUMP_ERRNO_REG_RDR_MODULE_OPS = -11,
    DUMP_ERRNO_CREATE_TASK = -10,
    DUMP_ERRNO_EXISTED = -9,
    DUMP_ERRNO_MALLOC = -8,
    DUMP_ERRNO_DTS_PROP = -7,
    DUMP_ERRNO_DTS_NODE = -6,
    DUMP_ERRNO_MEM_OPS = -5,
    DUMP_ERRNO_INVA_PARAM = -4,
    DUMP_ERRNO_DUMP_FEATURE_OFF = -3,
    DUMP_ERRNO_DUMP_LEVEL_NOT_MATCH = -2,
    DUMP_ERROR = -1,
    DUMP_ERR_BEGIN = DUMP_ERROR,
    DUMP_OK = 0,
};

static inline bool dump_is_err(int dump_err_no)
{
    return (dump_err_no <= DUMP_ERR_BEGIN) && (dump_err_no >= DUMP_ERR_END);
}

#endif
