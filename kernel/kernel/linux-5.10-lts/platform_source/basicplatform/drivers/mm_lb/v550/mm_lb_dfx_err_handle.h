/*
 * Copyright (c) 2023-2023 Huawei Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __MM_LB_DFX_ERR_HANDLE_H
#define __MM_LB_DFX_ERR_HANDLE_H

void post_process_pgid_hit_cgid_mismatch(u32 *err_info, u32 info_num);
void lb_dfx_buffer_alloc_err_handle (u32 *err_info, u32 info_num);
void lb_dfx_dmem_err_handle (u32 *err_info, u32 info_num);
void lb_dfx_multi_dirty_err_handle (u32 *err_info, u32 info_num);
#endif
