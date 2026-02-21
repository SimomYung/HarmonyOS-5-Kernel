/**
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved.
 *
 * Description: MP16c ssi host entry function declaration.
 * Author: @CompanyNameTag
 */

#ifndef SSI_MP17C_H
#define SSI_MP17C_H

#ifdef _PRE_CONFIG_GPIO_TO_SSI_DEBUG
int mp17c_ssi_config(unsigned long long module_set);
int mp17c_ssi_check_subsys_is_work(void);
void mp17c_ssi_dump_gpio(void);
void mp17c_ssi_show_pll_lock_status(void);
#endif /* #ifdef __PRE_CONFIG_GPIO_TO_SSI_DEBUG */
#endif /* #ifndef SSI_MP17C_H */
