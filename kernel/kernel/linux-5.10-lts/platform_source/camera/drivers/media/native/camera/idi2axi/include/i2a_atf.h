/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: idi2axi atf interfaces
 *
 * This file is released under the GPLv2.
 */
#ifndef I2A_ATF_H
#define I2A_ATF_H
#include <linux/types.h>

// id: 0/1 which idi2axi instance
int atf_idi2axi_enable(uint32_t id);
int atf_idi2axi_disable(uint32_t id);
#endif /* end of include guard: I2A_ATF_H */
