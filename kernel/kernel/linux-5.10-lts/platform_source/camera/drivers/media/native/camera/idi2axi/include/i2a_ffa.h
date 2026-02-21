/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: idi2axi tzsp interfaces
 *
 * This file is released under the GPLv2.
 */
#ifndef I2A_FFA_H
#define I2A_FFA_H
#include <linux/types.h>

int ffa_idi2axi_enable(uint32_t id);
int ffa_idi2axi_disable(uint32_t id);
#endif /* end of include guard: I2A_FFA_H */