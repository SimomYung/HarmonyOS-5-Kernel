/*
 * dfa_test.h
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

#ifndef _DFA_TEST_H_
#define _DFA_TEST_H_

#include "dfa_mcq.h"

typedef enum {
    COMMU_CMD_HTS_DFA_SELF_LOOP,
    COMMU_CMD_NUM_MAX
} commu_cmd_handler_type_t;

int dfa_wop_write_reg(void *data, uint32_t base_addr, uint32_t addr_offset, uint32_t val);
int dfa_write_reg(void *data, uint32_t addr, uint32_t val);
int dfa_read_reg(void *data, uint32_t addr, uint32_t* res);

int send_dfa_commu_cmd(void *data, uint32_t cmd_num, uint32_t data0, uint32_t data1);
int check_commu_cmd_finish_sign_reg(void *data);

int dfa_test_case_entry(void *data, u64 attr_id);

#endif /* _DFA_TEST_H_ */
