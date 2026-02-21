/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef BSP_AVS_H
#define BSP_AVS_H

#ifdef __cplusplus /* __cplusplus */
extern "C"
#endif /* __cplusplus */

typedef enum {
    AVS_DOMAIN_PERI,
    AVS_DOMAIN_MAX,
} avs_core_e;

typedef enum {
    AGING_COMP = 0, /* 老化补偿 */
    TEMP_COMP,      /* 温度补偿 */
    USER_COMP,      /* 商用补偿 */
    VEHICLE_COMP,   /* 车载补偿 */
} avs_function_e;

/**
 * @brief PERI_AVS 动态开关子补偿接口
 *
 * @param[in]  core_id，电源域选择 取值范围参见avs_core_e。
 * @param[in]  function_id，子补偿功能，取值范围参见avs_function_e枚举。
 * @param[in]  persis_status，命令持久状态0 表示单次生效; 1表示写入NV并持续生效，重启不丢失。
 * @param[in]  ena，开关状态 0:关闭; 1:打开。
 *
 * @retval 0, 表示操作成功。
 * @retval 非0, 表示操作失败。
 *
 * @attention
 * <ul><li> 使用约束 </li></ul>
 * <ol>
 * <li> 依赖avs 初始化之后调用。 </li>
 * <li> 当persis_status设置为1时，会同步写NV，不能在中断或者锁中断上下文等阻塞场景调用。 </li>
 * <li> 支持多次调用。 </li>
 * <li> 无并发保护，并发调用可能造成前一笔修改不生效。 </li>
 * </ol>
 */
int bsp_avs_function_onoff(avs_core_e core_id, avs_function_e function_id, unsigned int persis_status, unsigned int ena);


/**
 * @brief PERI_AVS 动态开关接口
 *
 * @param[in]  core_id，电源域选择 取值范围参见avs_core_e。
 * @param[in]  persis_status，命令持久状态0 表示单次生效; 1表示写入NV并持续生效，重启不丢失。
 * @param[in]  ena，开关状态 0:关闭; 1:打开。
 *
 * @retval 0, 表示操作成功。
 * @retval 非0, 表示操作失败。
 *
 * @attention
 * <ul><li> 使用约束 </li></ul>
 * <ol>
 * <li> 依赖avs 初始化之后调用。 </li>
 * <li> 当persis_status设置为1时，会同步写NV，不能在中断或者锁中断上下文等阻塞场景调用。 </li>
 * <li> 支持多次调用。 </li>
 * <li> 无并发保护，并发调用可能造成前一笔修改不生效。 </li>
 *
 * </ol>
 */
int bsp_avs_onoff(avs_core_e core_id, unsigned int persis_status, unsigned int ena);


/**
 * @brief PERI_AVS 打开/关闭档位子开关
 *
 * @param[in]  core_id，电源域选择 取值范围参见avs_core_e。
 * @param[in]  persis_status，命令持久状态0 表示单次生效; 1表示写入NV并持续生效，重启不丢失。
 * @param[in]  pro_mask（所有pro档位开关状态）每1bit代表1个pro档位; 0:关闭; 1:打开; BIT0代表pro0，BIT9代表pro9。
 *
 * @retval 0, 表示操作成功。
 * @retval 非0, 表示操作失败。
 *
 * @attention
 * <ul><li> 使用约束 </li></ul>
 * <ol>
 * <li> 依赖avs 初始化之后调用。 </li>
 * <li> 当persis_status设置为1时，会同步写NV，不能在中断或者锁中断上下文等阻塞场景调用。 </li>
 * <li> 支持多次调用。 </li>
 * <li> 无并发保护，并发调用可能造成前一笔修改不生效。 </li>
 * </ol>
 */
int bsp_avs_pros_switch_onoff(avs_core_e core_id, unsigned int persis_status, unsigned int pro_mask);


/**
 * @brief PERI_AVS 设置某一档位拉偏值
 *
 * @param[in]  core_id，电源域选择 取值范围参见avs_core_e。
 * @param[in]  persis_status，命令持久状态0 表示单次生效; 1表示写入NV并持续生效，重启不丢失。
 * @param[in]  pro_idx，需要修改的profile 档位，0-9为合法值。
 * @param[in]  voltage_bias，pro_idx档位对应的电压拉偏值,单位100uv，如果拉偏值为负数，串口调用时请传入十六进制数。
 *
 * @retval 0, 表示操作成功。
 * @retval 非0, 表示操作失败。
 *
 * @attention
 * <ul><li> 使用约束 </li></ul>
 * <ol>
 * <li> 依赖avs 初始化之后调用。 </li>
 * <li> 当persis_status设置为1时，会同步写NV，不能在中断或者锁中断上下文等阻塞场景调用。 </li>
 * <li> 支持多次调用,多次调用刷新上次调用配置值。 </li>
 * <li> 无并发保护，并发调用可能造成前一笔修改不生效。 </li>
 * </ol>
 */
int bsp_avs_set_pro_volt_bias(avs_core_e core_id, unsigned int persis_status, unsigned int pro_idx, int voltage_bias);


/**
 * @brief PERI_AVS 设置后5个档位（5档位~9档位）拉偏值
 *
 * @param[in]  persis_status，命令持久状态0 表示单次生效; 1表示写入NV并持续生效，重启不丢失。
 * @param[in]  bias0，5档位对应的电压拉偏值,单位100uv，若该档位不拉偏或不存在，请传0，如果拉偏值为负数，串口调用时请传入十六进制数。
 * @param[in]  bias1，6档位对应的电压拉偏值,单位100uv，若该档位不拉偏或不存在，请传0，如果拉偏值为负数，串口调用时请传入十六进制数。
 * @param[in]  bias2，7档位对应的电压拉偏值,单位100uv，若该档位不拉偏或不存在，请传0，如果拉偏值为负数，串口调用时请传入十六进制数。
 * @param[in]  bias3，8档位对应的电压拉偏值,单位100uv，若该档位不拉偏或不存在，请传0，如果拉偏值为负数，串口调用时请传入十六进制数。
 * @param[in]  bias4，9档位对应的电压拉偏值,单位100uv，若该档位不拉偏或不存在，请传0，如果拉偏值为负数，串口调用时请传入十六进制数。
 *
 * @retval 0, 表示操作成功。
 * @retval 非0, 表示操作失败。
 *
 * @attention
 * <ul><li> 使用约束 </li></ul>
 * <ol>
 * <li> 依赖avs 初始化之后调用。 </li>
 * <li> 当persis_status设置为1时，会同步写NV，不能在中断或者锁中断上下文等阻塞场景调用。 </li>
 * <li> 支持多次调用,多次调用刷新上次调用配置值。 </li>
 * <li> 无并发保护，并发调用可能造成前一笔修改不生效。 </li>
 * <li> 与bsp_avs_set_all_pro_bias接口配合使用。 </li>
 * </ol>
 */
int bsp_avs_set_all_pro_bias_con(unsigned int persis_status, int bias0, int bias1, int bias2, int bias3, int bias4);


/**
 * @brief PERI_AVS 设置所有档位拉偏值,默认支持5个档位（0档位~4档位）。
 *
 * @param[in]  persis_status，命令持久状态0 表示单次生效; 1表示写入NV并持续生效，重启不丢失。
 * @param[in]  bias0，0档位对应的电压拉偏值,单位100uv，若该档位不拉偏，请传0，如果拉偏值为负数，串口调用时请传入十六进制数。
 * @param[in]  bias1，1档位对应的电压拉偏值,单位100uv，若该档位不拉偏或不存在，请传0，如果拉偏值为负数，串口调用时请传入十六进制数。
 * @param[in]  bias2，2档位对应的电压拉偏值,单位100uv，若该档位不拉偏或不存在，请传0，如果拉偏值为负数，串口调用时请传入十六进制数。
 * @param[in]  bias3，3档位对应的电压拉偏值,单位100uv，若该档位不拉偏或不存在，请传0，如果拉偏值为负数，串口调用时请传入十六进制数。
 * @param[in]  bias4，4档位对应的电压拉偏值,单位100uv，若该档位不拉偏或不存在，请传0，如果拉偏值为负数，串口调用时请传入十六进制数。
 *
 * @retval 0, 表示操作成功。
 * @retval 非0, 表示操作失败。
 *
 * @attention
 * <ul><li> 使用约束 </li></ul>
 * <ol>
 * <li> 依赖avs 初始化之后调用。 </li>
 * <li> 当persis_status设置为1时，会同步写NV，不能在中断或者锁中断上下文等阻塞场景调用。 </li>
 * <li> 支持多次调用,多次调用刷新上次调用配置值。 </li>
 * <li> 无并发保护，并发调用可能造成前一笔修改不生效。 </li>
 * <li> 如果拉偏档位大于5,请继续调用接口bsp_avs_set_all_pro_bias_con </li>
 * </ol>
 */
int bsp_avs_set_all_pro_bias(unsigned int persis_status, int bias0, int bias1, int bias2, int bias3, int bias4);


/**
 * @brief PERI_AVS 更新AVS电压上下限
 *
 * @param[in]  core_id，电源域选择 取值范围参见avs_core_e。
 * @param[in]  persis_status，命令持久状态0 表示单次生效; 1表示写入NV并持续生效，重启不丢失。
 * @param[in]  volt_limit_sw 动态修改电压上下限开关, 1 表示使用动态电压上下限，修改公式中的默认配置；0 使用公式中的默认配置。
 * @param[in]  maximum, 电源域所有档位的电压上限,单位100uv。
 * @param[in]  minimum, 电源域所有档位的电压下限,单位100uv。
 *
 * @retval 0, 表示操作成功。
 * @retval 非0, 表示操作失败。
 *
 * @attention
 * <ul><li> 使用约束 </li></ul>
 * <ol>
 * <li> 依赖avs 初始化之后调用。 </li>
 * <li> 当persis_status设置为1时，会同步写NV，不能在中断或者锁中断上下文等阻塞场景调用。 </li>
 * <li> 支持多次调用。 </li>
 * <li> 无并发保护，并发调用可能造成前一笔修改不生效。 </li>
 * </ol>
 */
int bsp_avs_set_volt_limit(avs_core_e core_id, unsigned int persis_status, unsigned int volt_limit_sw, unsigned int maximum, unsigned int minimum);
#ifdef __cplusplus /* __cplusplus */
}
#endif /* __cplusplus */

#endif
