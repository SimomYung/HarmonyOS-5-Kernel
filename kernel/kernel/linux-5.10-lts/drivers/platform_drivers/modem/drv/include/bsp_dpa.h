/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2020. All rights reserved.
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

#ifndef BSP_DPA_H
#define BSP_DPA_H

#include <product_config.h>
#include "bsp_msg.h"

typedef enum {
    MODEM_REST_BEFORE_REQ = 1,
    MODEM_REST_BEFORE_FAIL,
    MODEM_REST_BEFORE_DONE,
    MODEM_REST_AFTER_REQ,
    MODEM_REST_AFTER_FAIL,
    MODEM_REST_AFTER_DONE,
    MODEM_REST_BEGIN,
    MODEM_REST_END,
    STATUS_BUTT
} mdm_rst_status_e;

typedef enum dpa_clk_grade {
    DPA_CLK_GRADE_200M,
    DPA_CLK_GRADE_400M,
    DPA_CLK_GRADE_800M,
    DPA_CLK_GRADE_BOTTOM
} dpa_clk_grade_e;

typedef enum peri_volt_trend {
    PERI_VOLT_NO_CHANGE,
    PERI_VOLT_DOWN,
    PERI_VOLT_UP,
    PERI_VOLT_TREND_BOTTOM
} peri_volt_trend_e;

typedef enum {
    DPA_REGS_ACCESS_DPA_SYSCTRL_CRG = 0,
    DPA_REGS_ACCESS_TSP_MPSS,
    DPA_REGS_ACCESS_TSP_DSS0,
    DPA_REGS_ACCESS_BUTT,
} dpa_regs_access_state_e;

typedef enum {
    DPA_RPM_EICC,
    DPA_RPM_WDT,
    DPA_RPM_FTRACE,
    DPA_RPM_SLT,
    RPM_TYPE_MAX,
} rpm_type_e;

struct dpa_rpm_ops {
    void (*rpm_suspend)(void);
    void (*rpm_resume)(void);
};

/**
 * @brief device rpm 回调注册
 *
 * @par 描述:
 * device rpm ops注册。
 *
 * @param[in]  rpm_type，rpm_type_e注册枚举。
 *
 * @retval 0，表示操作成功。
 * @retval 非0，表示rpm_type不合法，或者ops为NULL，操作失败。
 *
 * @attention
 * <ul>
 * <li>suspend失败，不支持回滚, 不影响其他模块继续执行。</li>
 * <li>resume流程失败, 不影响其他模块。</li>
 * </ul>
 *
 * @par 依赖:
 * <ul><li>bsp_pm.h：该接口声明所在的头文件。</li></ul>
 *
 * @see
 */
int bsp_dpa_rpm_register(rpm_type_e rpm_type, struct dpa_rpm_ops *dpa_rpm_ops);

typedef enum {
    AO_SYSCTRL_BASE,
    PERI_CRG_BASE,
    PERI_SYSCTRL_BASE,
    PERI_FIX0_SYSCTRL_BASE,
    PERI_FIX1_SYSCTRL_BASE,
    DPA_SYSCTRL_BASE,
    DPA_CRG_BASE,
    DPA_TSP_BASE,
    DPA_ITCM_GLOCAL,
    DPA_DTCM_GLOCAL,
    DPA_ITCM_LOCAL,

    ACTRL_BASE,
    SCTRL_BASE,
    PCTRL_BASE,
    BISR_CTRL_BASE,
    MDMPERI_CRG_REG_BASE,
    PERIPH_CRG_BASE,
    MDMPERI_SYSCTRL_BASE,
    REG_BASE_MAX,
} reg_ctrl_e;

struct dpa_reg_info {
    void *virt_addr;
    unsigned int phy_addr;
    unsigned int size;
};

/**
 * @brief device rpm 回调注册
 *
 * @par 描述:
 * device rpm ops注册。
 *
 * @param[in]  rpm_type，rpm_type_e注册枚举。
 *
 * @retval 0，表示操作成功。
 * @retval 非0，表示rpm_type不合法，或者ops为NULL，操作失败。
 *
 * @attention
 * <ul>
 * <li>suspend失败，不支持回滚, 不影响其他模块继续执行。</li>
 * <li>resume流程失败, 不影响其他模块。</li>
 * </ul>
 *
 * @par 依赖:
 * <ul><li>bsp_pm.h：该接口声明所在的头文件。</li></ul>
 *
 * @see
 */
void bsp_dpa_reg_info_get(reg_ctrl_e id, struct dpa_reg_info *info);

/**
 * @brief 获取tcm的备份信息
 *
 * @par 描述:
 * 获取tcm的备份信息。
 *
 * @param[in]  dtcm_info, itcm_info
 *
 * @retval 无
 *
 * @attention
 * <ul>
 * <li>suspend失败，不支持回滚, 不影响其他模块继续执行。</li>
 * <li>resume流程失败, 不影响其他模块。</li>
 * </ul>
 *
 * @par 依赖:
 * <ul><li>bsp_pm.h：该接口声明所在的头文件。</li></ul>
 *
 * @see
 */
void bsp_dpa_tcm_bak_info_get(struct dpa_reg_info *dtcm_info, struct dpa_reg_info *itcm_info);

typedef enum {
    DPA_UNRESET_HOOK_OCD,
    DPA_UNRESET_HOOK_QIC,
    DPA_UNRESET_HOOK_WDT,
    DPA_UNRESET_HOOK_FIPC,
    DPA_UNRESET_HOOK_DFS,
    DPA_UNRESET_HOOK_TYPE_MAX,
} unreset_type_e;

typedef void (*dpa_unreset_hook_ops)(void);

typedef enum {
    DPA_POWERON_HOOK_MSG,
    DPA_POWERON_HOOK_TYPE_MAX,
} powerup_type_e;
typedef void (*dpa_poweron_hook_ops)(void);

typedef enum {
    DPA_DEBUGFS_GMAC,
    DPA_DEBUGFS_XGMAC,
    DPA_DEBUGFS_USB,
    DPA_DEBUGFS_PFA,
    DPA_DEBUGFS_TYPE_MAX,
} dpa_debugfs_type_e;

typedef void (*dpa_debugfs_ops)(unsigned int time, unsigned int *speed);

/**
 * @brief dpa首次上电解复位事件hook
 *
 * @par 描述:
 * dpa首次上电解复位事件hook
 *
 * @param[in]  type，unreset_type_e注册枚举。
 *
 * @retval 0，表示操作成功。
 * @retval 非0，表示type不合法，或者ops为NULL，操作失败。
 *
 * @attention
 * @par 依赖:
 * <ul><li>bsp_dpa.h：该接口声明所在的头文件。</li></ul>
 *
 * @see
 */
#ifdef CONFIG_DPA
int bsp_dpa_unreset_hook_register(unreset_type_e type, dpa_unreset_hook_ops ops);
int bsp_dpa_powerup_hook_register(powerup_type_e type, dpa_unreset_hook_ops ops);
int bsp_dpa_powerup_hook_unregister(powerup_type_e type);
void bsp_dpa_wakeup_lock(void);
int bsp_dpa_debugfs_register(dpa_debugfs_type_e type, dpa_debugfs_ops ops);
#else
static inline int bsp_dpa_unreset_hook_register(unreset_type_e type, dpa_unreset_hook_ops ops) {return 0;}
static inline int bsp_dpa_powerup_hook_register(powerup_type_e type, dpa_unreset_hook_ops ops) {return 0;}
static inline int bsp_dpa_powerup_hook_unregister(powerup_type_e type) {return 0;}
static inline void bsp_dpa_wakeup_lock(void) {return;}
static inline int bsp_dpa_debugfs_register(dpa_debugfs_type_e type, dpa_debugfs_ops ops) { return 0;}
#endif


/**
 * @brief 获取dpa run time device
 *
 * @par 描述:
 * 获取dpa run time device。
 *
 * @param[in]  NA。
 *
 * @retval NULL，表示获取失败。
 * @retval 非NULL，run time device指针。
 *
 * @attention
 *
 * @par 依赖:
 * <ul><li>bsp_dpa.h：该接口声明所在的头文件。</li></ul>
 *
 * @see
 */
#ifdef CONFIG_DPA
struct device *bsp_dpa_get_runtime_dev(void);
int bsp_dpapm_get_dpa_access_state(dpa_regs_access_state_e dpa_regs_region);
int bsp_dpa_wakeup(unsigned int wakelock);
/**
 * @brief msg lite的核间消息发送接口,特定给DPA发送消息接口
 *
 * @par 描述:
 * 将msg信息发送到DPA
 *
 * @attention
 * <ul><li>本接口上下文不能禁止中断</li></ul>
 *
 * @param[in]  chn_hdl, 通道成功打开后返回的句柄
 * @param[in]  dst, 目的端信息，包含目的core id，以及dts chn信息
 * @param[in]  msg, 待发送的消息地址
 * @param[in]  msg, 待发送的消息长度
 *
 * @retval 0,注册成功
 * @retval 其他, 执行失败
 */
int bsp_msg_sendto_dpa(msg_chn_t chn_hdl, const struct msg_addr *dst, void *msg, unsigned len);

int bsp_dpa_agent_send_msg_to_dpa(unsigned int status);
#else
static inline struct device *bsp_dpa_get_runtime_dev(void)
{
    return NULL;
}
static inline int bsp_dpapm_get_dpa_access_state(dpa_regs_access_state_e dpa_regs_region)
{
    return -1;
}
static inline int bsp_dpa_wakeup(unsigned int wakelock)
{
    return 0;
}
static inline int bsp_msg_sendto_dpa(msg_chn_t chn_hdl, const struct msg_addr *dst, void *msg, unsigned len)
{
    return 0;
}
static inline int bsp_dpa_agent_send_msg_to_dpa(unsigned int status)
{
    return 0;
}
#endif

/**
 * @brief：GIC_MSI的用户id
 * @attention 需要使用GIC_MSI中断的话，请直接添加枚举，并在DPA中注册中断
 */
enum gic_msi_id {
    GIC_MSI_WALN = 0,
    GIC_MIS_RESERVED1 = 1,

    GIC_MSI_MAX = 32,
};

#ifdef CONFIG_GIC_MSI
/**
 * @brief 触发DPA的GIC-MSI中断，并发送gic-msi msg消息
 *
 * @par 描述:触发DPA的GIC-MSI中断，并发送gic-msi msg消息
 *
 *
 * @param[in]  id, GIC_MSI的用户id
 * @param[in]  msg, 待发送的消息地址，只使用中断时，不使用msg，请将msg置NULL，并将len置0
 * @param[in]  len, 待发送的消息长度
 *
 * @retval 0,发送成功
 * @retval 其他, 发送失败
 */
int bsp_gic_msi_sendto_dpa(enum gic_msi_id id, void *msg, unsigned int len);
#endif

int bsp_dpa_load_init(void);
void bsp_dpa_load_exit(void);
void* bsp_dpa_pm_get_ctrl(void);
void bsp_dpa_pm_cfg_clk_grade(peri_volt_trend_e peri_volt_trend, dpa_clk_grade_e clk_grade);
#endif
