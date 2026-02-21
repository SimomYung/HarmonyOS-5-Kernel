/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
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

/*lint --e{537,826} */
#include <product_config.h>
#include <linux/kernel.h>
#include <mdrv_memory.h>
#include <mdrv_sysboot.h>
#include "power_exchange.h"
#include "bsp_blk.h"
#include "bsp_onoff.h"
#include <bsp_print.h>
#include "power_hal.h"
#include "power_para.h"
#include <bsp_sysctrl.h>
#include "dpart/sysboot_dpart_policy.h"

#define THIS_MODU mod_onoff

#ifdef CONFIG_MODEM_ONOFF_PARA
struct onoff_shm_info {
    void *onoff_shm_virt_addr;
    phy_addr_t onoff_shm_phy_addr;
    u32 onoff_shm_size;
};

struct onoff_shm_info g_onoff_shm_info;

#define SRAM_REBOOT_ADDR ((unsigned long)(uintptr_t)g_onoff_shm_info.onoff_shm_virt_addr)

/*
 * 功能描述: 设置下载标志:flag: 1表示下载模式启动；0表示normal模式启动
 */
int bsp_sysboot_set_download_flag(bool flag)
{
    sysboot_set_boot_download_mode(flag);
    return 0;
}

/*
 * 功能描述: 获取下载模式: 1表示下载模式启动；0表示normal模式启动；
 */
int bsp_sysboot_get_download_flag(void)
{
    return sysboot_get_boot_download_mode();
}

/*
 * 功能描述: 设置重启标志
 */
void power_on_reboot_flag_set(power_off_reboot_flag_e en_flag)
{
    power_info_s *power_info = (power_info_s *)(uintptr_t)SRAM_REBOOT_ADDR;

    power_info->last_shut_reason = (unsigned int)(en_flag);

    bsp_debug("power_on_reboot_flag_set = 0x%08X \r\n", (unsigned int)en_flag);
}
/*
 * 功能描述: 获取重启原因
 */
power_off_reboot_flag_e power_on_reboot_flag_get(void)
{
    power_info_s *power_info = (power_info_s *)(uintptr_t)SRAM_REBOOT_ADDR;

    bsp_debug("power_on_reboot_flag_get = 0x%08X  \r\n", power_info->last_shut_reason);
    return (power_off_reboot_flag_e)power_info->last_shut_reason;
}
/*
 * 功能描述: 设置开机原因
 */
void power_on_start_reason_set(power_on_start_reason_e en_reason)
{
    power_info_s *power_info = (power_info_s *)(uintptr_t)SRAM_REBOOT_ADDR;

    power_info->power_on_reason = (unsigned int)(en_reason);

    bsp_debug("power_on_start_reason_set = 0x%08X  \r\n", en_reason);
}
/*
 * 功能描述: 获取开机原因
 */
power_on_start_reason_e power_on_start_reason_get(void)
{
    power_info_s *power_info = (power_info_s *)(uintptr_t)SRAM_REBOOT_ADDR;

    bsp_debug("power_on_start_reason_get = 0x%08X \r\n", power_info->power_on_reason);
    return (power_on_start_reason_e)(power_info->power_on_reason);
}
/*
 * 功能描述: 设置开机原因
 */
void power_reboot_cmd_set(power_reboot_cmd_e cmd)
{
    power_info_s *power_info = (power_info_s *)(uintptr_t)SRAM_REBOOT_ADDR;

    power_info->reboot_cmd = (unsigned int)(cmd);

    bsp_debug("power_reboot_cmd_set = 0x%08X \r\n", cmd);
}

int mdrv_sysboot_set_power_reboot_cmd(power_reboot_cmd_e cmd)
{
    power_reboot_cmd_set(cmd);
    return 0;
}

int power_on_shm_init(void)
{
    g_onoff_shm_info.onoff_shm_virt_addr = mdrv_mem_share_get("usshm_onoff", &g_onoff_shm_info.onoff_shm_phy_addr,
        &g_onoff_shm_info.onoff_shm_size, SHM_UNSEC);
    if (g_onoff_shm_info.onoff_shm_virt_addr == NULL) {
        bsp_err("usshm_onoff addr get fail\n");
        return -1;
    }
    return 0;
}

#ifdef CONFIG_LLT_MDRV
EXPORT_SYMBOL_GPL(power_on_reboot_flag_set);
EXPORT_SYMBOL_GPL(power_on_reboot_flag_get);
EXPORT_SYMBOL_GPL(power_on_start_reason_set);
EXPORT_SYMBOL_GPL(power_on_start_reason_get);
EXPORT_SYMBOL_GPL(power_reboot_cmd_set);
EXPORT_SYMBOL_GPL(bsp_sysboot_set_download_flag);
EXPORT_SYMBOL_GPL(bsp_sysboot_get_download_flag);
#endif

EXPORT_SYMBOL_GPL(mdrv_sysboot_set_power_reboot_cmd);

#endif
