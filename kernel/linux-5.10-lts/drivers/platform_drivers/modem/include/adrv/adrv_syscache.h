/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
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

#ifndef ADRV_SYSCACHE_H
#define ADRV_SYSCACHE_H

#include <linux/mm_types.h>

 /**
 * @brief 申请带gid的page
 *
 * @par 描述:
 * 申请带gid的page
 *
 * @param[in]  pgid:使用的pgid值
 * @param[in]  gfp_mask:内存分配方式
 * @param[in]  order:要申请的物理页数，取值为2的order次方个
 *
 * @retval page指针
 */
struct page *lb_alloc_pages(u32 pgid, gfp_t gfp_mask, u32 order);

 /**
 * @brief 释放带gid的page
 *
 * @par 描述:
 * 释放带gid的page
 *
 * @param[in]  pgid:使用的pgid值
 * @param[in]  pages:申请的page指针
 * @param[in]  order:要申请的物理页数，取值为2的order次方个
 *
 * @retval 0（成功）/非0（失败）
 */
int lb_free_pages(u32 pgid, struct page *pages, u32 order);

 /**
 * @brief cache cmo操作
 *
 * @par 描述:
 * cache cmo操作
 *
 * @param[in]  pgid:使用的pgid值
 * @param[in]  ops:执行的cmo类型，如invalid/clean等
 *
 * @retval void
 */
void lb_cmo_by_gid(u32 pgid, u32 ops);

 /**
 * @brief 清除syscache标记，并invalid syscache
 *
 * @par 描述:
 * 清除syscache标记，并invalid syscache
 *
 * @param[out]  0/-1
 * @param[in]  pid:当前page使用的pgid；pg:page指针; count:申请page数量
 *
 * @retval 0, 执行成功
 * @retval -1, 执行失败
 */
int lb_pages_detach(u32 pid, struct page *pg, size_t count);

 /**
 * @brief 获取page中保存的pgid
 *
 * @par 描述:
 * 获取page中保存的pgid
 *
 * @param[out] pgid：用户使用的pgid值
 * @param[in]  pages:page指针
 *
 * @retval pgid：用户使用的pgid值
 */
u32 lb_page_to_gid(struct page *page);

 /**
 * @brief page引用计数操作保存栈
 *
 * @par 描述:
 * page引用计数操作保存栈
 *
 * @param[out]  NA
 * @param[in]  pages:page指针；nr:page cnt值；set_flag:flag
 *
 * @retval void
 */
void cma_page_count_stack_saved(struct page *page, int nr, int set_flag);

 /**
 * @brief 获取page操作的维测标志
 *
 * @par 描述:
 * 获取page操作的维测标志
 *
 * @param[out] 0/1
 * @param[in]  NA
 *
 * @retval 0：不保存维测
 * @retval 1: 保存维测
 */
int get_himntn_cma_trace_flag(void);
#endif /* ADRV_SYSCACHE_H */
