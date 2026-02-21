/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2019-2020. All rights reserved.
 * Description: soc_blpwm_interface.h
 * Author: Hisilicon
 * Create: 2019-10-29
 */

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/

#ifndef __SOC_BLPWM_INTERFACE_H__
#define __SOC_BLPWM_INTERFACE_H__

#ifdef __cplusplus
    #if __cplusplus
        extern "C" {
    #endif
#endif



/*****************************************************************************
  2 宏定义
*****************************************************************************/

/****************************************************************************
                     (1/1) register_define
 ****************************************************************************/

#define SOC_BLPWM_IN_CTRL_ADDR(base)                          ((base) + (0x000UL))
#define SOC_BLPWM_IN_DIV_ADDR(base)                           ((base) + (0x004UL))
#define SOC_BLPWM_OUT_CTRL_ADDR(base)                         ((base) + (0x100UL))
#define SOC_BLPWM_OUT_DIV_ADDR(base)                          ((base) + (0x104UL))
#define SOC_BLPWM_OUT_CFG_ADDR(base)                          ((base) + (0x108UL))

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of soc_blpwm_interface.h */
