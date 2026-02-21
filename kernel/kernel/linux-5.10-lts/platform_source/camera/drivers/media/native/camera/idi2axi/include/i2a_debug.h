/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022.
 * Description: idi2axi debug interface
 *
 * This file is released under the GPLv2.
 */
#ifndef I2A_DEBUG_H
#define I2A_DEBUG_H

void sleep_here_wait(void);
bool use_kmalloc_mem(void);
bool log_reg_rw(void);
bool log_i2c_rw(void);
bool log_i2a_status(void);
bool csi_int_enable(void);
bool sengen_enabled(void);
bool single_vc(void);
uint16_t get_color_bar_pat(void);
uint64_t get_i2a_timeout(void);
int i2a_debugfs_init(void);

#endif /* end of include guard: I2A_DEBUG_H */
