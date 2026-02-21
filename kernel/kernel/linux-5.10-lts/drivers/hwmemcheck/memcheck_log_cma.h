/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2024. All rights reserved.
 * Get CMA memory info function
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef _MEMCHECK_LOG_CMA_H
#define _MEMCHECK_LOG_CMA_H

int memcheck_cma_createfs(void);
void memcheck_cma_report(char *name, unsigned long total, unsigned long req);
void memcheck_cma_info_show(void);

#endif /* _MEMCHECK_LOG_ION_H */
