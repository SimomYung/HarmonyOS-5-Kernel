/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
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

#ifndef __NVA_ID_H__
#define __NVA_ID_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 * NVA ID 的添加按从小到大排列
 */
#define NVA_ID_BASE 7000

typedef enum {
    NVA_ID_DRV_EXAMPLE = NVA_ID_BASE,            // EXAMPLE_STRU
    NVA_ID_DRV_CCORE_RESET = NVA_ID_BASE + 2,
    NVA_ID_DRV_UART_SHELL_FLAG = NVA_ID_BASE + 3,
    NVA_ID_DRV_ACORE_PM = NVA_ID_BASE + 4,
    NVA_ID_DRV_DEV_TEST = NVA_ID_BASE + 5,
    NVA_ID_DRV_AMON_AP = NVA_ID_BASE + 9,
    NVA_ID_DRV_CORESIGHT = NVA_ID_BASE + 10,
    NVA_ID_DRV_WATCHPOINT = NVA_ID_BASE + 11,
    NVA_ID_DRV_ACORE_PM_DEBUG = NVA_ID_BASE + 12,
    NVA_ID_DRV_LED_CONTROL = NVA_ID_BASE + 13,
    NVA_ID_DRV_DDR_SWITCH = NVA_ID_BASE + 14,
    NVA_ID_DRV_MODULE_SUPPORT = NVA_ID_BASE + 15,
    NVA_ID_ADC_CHANNEL_MAP = NVA_ID_BASE + 16,
    NVA_ID_DRV_ADC_XO_CONVERT_TABLE = NVA_ID_BASE + 17,
    NVA_ID_DRV_ADC_PA_CONVERT_TABLE = NVA_ID_BASE + 18,
    NVA_ID_DRV_TSENSOR_TEMP_PROTECT_CONTROL = NVA_ID_BASE + 19,
    NVA_ID_DRV_PFA_FEATURE = NVA_ID_BASE + 20,
    NVA_ID_DRV_DUMP = NVA_ID_BASE + 21,
    NVA_ID_DMSS_KENERL = NVA_ID_BASE + 22,
    NVA_ID_DRV_DUMP_FILE = NVA_ID_BASE + 23,
    NVA_ID_DRV_DIAG_POWER_LOG = NVA_ID_BASE + 24,
    NVA_ID_DRV_HKADC_CAL_VERSION = NVA_ID_BASE + 25,
    NVA_ID_DRV_PM = NVA_ID_BASE + 26,
    NVA_ID_DRV_DRX_DELAY = NVA_ID_BASE + 27,
    NVA_ID_DRV_NR_PWR = NVA_ID_BASE + 28,
    NVA_ID_DRV_USB_FEATURE = NVA_ID_BASE + 29,
    NVA_ID_DRV_GMAC_FEATURE = NVA_ID_BASE + 30,
    NVA_ID_DRV_LPMCU_IRQ_CFG = NVA_ID_BASE + 31,
    NVA_ID_DRV_DDR_TRAIN_DATA = NVA_ID_BASE + 32,
    NVA_ID_DRV_HIBURN = NVA_ID_BASE + 33,
    NVA_ID_DRV_BUS_PROBE = NVA_ID_BASE + 34,
    NVA_ID_DRV_PINCTRL = NVA_ID_BASE + 35,
    NVA_ID_LPMCU_BASE = NVA_ID_BASE + 36,
    NVA_ID_DRV_SYSCTRL = NVA_ID_BASE + 37,
    NVA_ID_DRV_TRACE = NVA_ID_BASE + 38,
    NVA_ID_DRV_MEMORY_MAP_INFO = NVA_ID_BASE + 39,
    NVA_ID_DRV_USB_GNET_FEATURE = NVA_ID_BASE + 40,
    NVA_ID_DRV_DIALUP_UART_FEATURE = NVA_ID_BASE + 41,
    NVA_ID_DRV_ACORE_DFS_LOAD = NVA_ID_BASE + 42,
    NVA_ID_DRV_USB_VENDOR_INFO = NVA_ID_BASE + 43,
    NVA_ID_DRV_UART_2L_WAKEUP = NVA_ID_BASE + 44,
    NVA_ID_DRV_PCIE_NIC_NET_FEATURE = NVA_ID_BASE + 45,
    NVA_ID_DRV_VER_PRODUCT_INFO = NVA_ID_BASE + 46,
    NVA_ID_DRV_MCI_CHN_CUST_FEATURE = NVA_ID_BASE + 47,
    NVA_ID_DRV_MCI_FEATURE = NVA_ID_BASE + 48,

    NVA_ID_MAX = NVA_ID_BASE + 999,
} nva_id_drv_e;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
