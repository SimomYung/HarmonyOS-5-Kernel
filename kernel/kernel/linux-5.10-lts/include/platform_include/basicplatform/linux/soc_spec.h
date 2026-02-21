/*
 *
 * cmdline parse head file
 *
 * Copyright (c) 2019-2020 Huawei Technologies Co., Ltd.
 *
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
#ifndef __SOC_SPEC_H
#define __SOC_SPEC_H

#include "soc_spec_info.h"
#include "soc_spec_common.h"

#if defined(CONFIG_SOC_PG_INFO)
void get_soc_pg_info_by_index(enum soc_component_pg_info_index pg_info_index, struct soc_component_pg_info *pg_info);
#endif

#endif
