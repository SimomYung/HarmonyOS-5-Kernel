/*
 * dfa_test_null.c
 *
 * Copyright (c) 2024 Huawei Technologies Co., Ltd.
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

#include "dfa_test.h"

#define unused(x) ((void)(x))

static inline int return_invald_call(void *data)
{
    struct ufs_hba *hba = data;
    if (hba)
        dev_err(hba->dev, "%s: unsupport func on this platform!\n", __func__);
    return -EINVAL;
}

int dfa_wop_write_reg(void *data, uint32_t base_addr, uint32_t addr_offset, uint32_t val)
{
    unused(base_addr);
    unused(addr_offset);
    unused(val);
    return return_invald_call(data);
}

int dfa_write_reg(void *data, uint32_t addr, uint32_t val)
{
    unused(addr);
    unused(val);
    return return_invald_call(data);
}

int dfa_read_reg(void *data, uint32_t addr, uint32_t* res)
{
    unused(addr);
    unused(res);
    return return_invald_call(data);
}

int send_dfa_commu_cmd(void *data, uint32_t cmd_num, uint32_t data0, uint32_t data1)
{
    unused(cmd_num);
    unused(data0);
    unused(data1);
    return return_invald_call(data);
}

int check_commu_cmd_finish_sign_reg(void *data)
{
    return return_invald_call(data);
}

int dfa_test_case_entry(void *data, u64 attr_id) {
    unused(attr_id);
    return return_invald_call(data);
}