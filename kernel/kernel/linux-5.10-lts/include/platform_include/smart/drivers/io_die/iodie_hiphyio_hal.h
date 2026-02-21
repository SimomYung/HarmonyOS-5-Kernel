/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Contexthub 9030 driver.
 * Create: 2024-11-30
 */
#ifndef __IODIE__HIPHYIO__HAL__
#define __IODIE__HIPHYIO__HAL__

#include "iodie_hiphyio_def.h"

/**
 * @brief write 9030 register.
 *
 * @param module  : the module number needs to write.
 * @param addr    : the address within the module..
 * @param val     : writel value.
 */
inline void iodie_writel(enum iodie_module module, unsigned int addr, unsigned int val) ;

/**
 * @brief read 9030 register.
 *
 * @param module  : the module number needs to read.
 * @param addr    : the address within the module.
 * @return unsigned int : read value.
 */
inline unsigned int iodie_readl(enum iodie_module module, unsigned int addr);

/**
 * @brief cast a vote to change iodie status.
 *
 * @param user    : the voter.
 * @param status  : target status.
 * @return int    : 0 success, other fail.
 */
int iodie_normal_request(enum iodie_user user, enum iodie_runtime_status status);

/**
 * @brief cancel the vote which changed iodie status.
 *
 * @param user    : the voter.
 * @param status  : target status.
 * @return int    : 0 success, other fail.
 */
int iodie_normal_release(enum iodie_user user, enum iodie_runtime_status status);

/**
 * @brief open the 9030 side hiphyio fnpll.
 *
 * @param user    : the user.
 * @return int    : 0 success, other fail.
 */
int iodie_fnpll_request(enum iodie_user user);

/**
 * @brief close the 9030 side hiphyio fnpll.
 *
 * @param user    : the user.
 * @return int    : 0 success, other fail.
 */
int iodie_fnpll_release(enum iodie_user user);

/**
 * @brief open clock gate of the input module.
 *
 * @param module  : the various IPs on 9030.
 */
void iodie_clk_en(enum iodie_module module);

/**
 * @brief close clock gate of the input module.
 *
 * @param module  : the various IPs on 9030.
 */
void iodie_clk_dis(enum iodie_module module);

/**
 * @brief Register a notification function to the iodie driver, which will be called when initialization completion
 *        or an exception occurs.
 *
 * @param user    : the user.
 * @param func    : notification function.
 * @return int    : 0 success, other fail.
 */
int iodie_notify_register(enum iodie_user user, iodie_user_func func);

/**
 * @brief get the system status of the iodie driver.
 *
 * @return enum iodie_sys_status   : iodie driver status.
 */
enum iodie_sys_status get_iodie_status(void);

/**
 * @brief get the runtime status of the iodie driver.
 *
 * @return enum iodie_runtime_status   : iodie driver runtime status.
 */
enum iodie_runtime_status get_iodie_rt_status(void);

typedef void (*iodie_intr_hub_func)(void *data);

/**
 * @brief Register a notification function to the iodie intrrupt hub(L2 intrrupt).
 *
 * @param l2_id   : the L2 intrrupt index.
 * @param l2_bit  : the bit in the L2 interrupt register.
 * @param handler : interrupt handling function.
 * @param arg     : input parameters of the interrupt handling function.
 * @return int    : 0 success, other fail.
 */
int iodie_intr_hub_irq_register(unsigned int l2_id, unsigned int l2_bit, iodie_intr_hub_func handler, void *arg);

/**
 * @brief unregister the notification function to the iodie intrrupt hub(L2 intrrupt).
 *
 * @param l2_id   : the L2 intrrupt index.
 * @param l2_bit  : the bit in the L2 interrupt register.
 * @return int    : 0 success, other fail.
 */
int iodie_intr_hub_irq_unregister(unsigned int l2_id, unsigned int l2_bit);

int iodie_clk_out_sys(enum iodie_clk_out_op op);
int iodie_clk_out_sys_freq(enum iodie_clk_out_op op, unsigned int freq);

#endif

