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

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/string.h>
#include <mdrv_bmi_sysboot.h>
#include <mdrv_bmi_partition.h>
#include <mdrv_bmi_partition_name.h>
#include <bsp_partition.h>
#include <securec.h>
#include <bsp_print.h>
#include <bsp_sysboot.h>
#include <bsp_sysctrl.h>
#include <bsp_onoff.h>
#include "../power_hal.h"
#include "sysboot_dpart_policy.h"

#ifdef CONFIG_MODEM_ONOFF_PARA

#define TRANSFER_BASE 16
#define THIS_MODU mod_onoff

struct dpart_img_type_map_to_part_name {
    boot_image_type img_type;
    char part_name[PART_NAMELEN];
};

// 成员定义需要按照boot_image_type 枚举顺序定义
const struct dpart_img_type_map_to_part_name g_sysboot_img_type_map[BOOT_IMG_ID_MAX] = {
    {BOOT_IMG_XLOADER, PTABLE_XLOADER},
    {BOOT_IMG_UCE, PTABLE_UCE},
    {BOOT_IMG_HIBOOT, PTABLE_HIBOOT},
    {BOOT_IMG_PTABLE, PTABLE_PTABLE},
    {BOOT_IMG_UAPP, PTABLE_XLOADER},
    {BOOT_IMG_FW_DTS, PTABLE_DTS},
    {BOOT_IMG_DTS, PTABLE_DTS},
    {BOOT_IMG_M3IMG, PTABLE_LPMCU},
    {BOOT_IMG_KERNEL, PTABLE_KERNEL},
    {BOOT_IMG_TEEOS, PTABLE_TEEOS},
    {BOOT_IMG_NVA, PTABLE_NVA_UPGRADE},
    {BOOT_IMG_HIBOOT_EXTENTION, PTABLE_HIBOOT},
    {BOOT_IMG_BL31, PTABLE_TRUSTED_FW},
    {BOOT_AVB_PART, "avb_part"},
    {BOOT_IMG_MODEMFW, "modem_fw"},
};

struct sysboot_dpart_info g_sysboot_dpart_info = {0};

static int __init dpart_enable_parse_args(char *val)
{
    return kstrtou32(val, TRANSFER_BASE, &g_sysboot_dpart_info.runtime_info.dpart_enable);
}
early_param("dpart_enable", dpart_enable_parse_args);

static int __init dpart_policy_parse_args(char *val)
{
    return kstrtou32(val, TRANSFER_BASE, &g_sysboot_dpart_info.runtime_info.dpart_policy);
}
early_param("dpart_policy", dpart_policy_parse_args);

static int __init boot_part_parse_args(char *val)
{
    return kstrtou32(val, TRANSFER_BASE, &g_sysboot_dpart_info.runtime_info.default_boot_part);
}
early_param("boot_part", boot_part_parse_args);

static int __init image_load_failed_parse_args(char *val)
{
    return kstrtou32(val, TRANSFER_BASE, &g_sysboot_dpart_info.runtime_info.image_load_failed);
}
early_param("image_load_failed", image_load_failed_parse_args);

static int __init dpart_reg_info_parse_args(char *val)
{
    return kstrtou32(val, TRANSFER_BASE, (u32 *)(uintptr_t)&g_sysboot_dpart_info.runtime_info.reg_info.value);
}
early_param("dpart_reg_info", dpart_reg_info_parse_args);

static int __init dpart_boot_part_switched_parse_args(char *val)
{
    return kstrtou32(val, TRANSFER_BASE, (u32 *)(uintptr_t)&g_sysboot_dpart_info.runtime_info.boot_part_switched);
}
early_param("boot_part_switched", dpart_boot_part_switched_parse_args);

static int __init dpart_oeminfo_total_boot_times_parse_args(char *val)
{
    return kstrtou32(val, TRANSFER_BASE, (u32 *)(uintptr_t)&g_sysboot_dpart_info.runtime_info.oeminfo_total_boot_times);
}
early_param("oeminfo_total_boot_times", dpart_oeminfo_total_boot_times_parse_args);

static int __init dpart_boot_retry_times_parse_args(char *val)
{
    return kstrtou32(val, TRANSFER_BASE, (u32 *)(uintptr_t)&g_sysboot_dpart_info.runtime_info.boot_retry_times);
}
early_param("boot_retry_times", dpart_boot_retry_times_parse_args);

unsigned int mdrv_sysboot_get_switch_type(void)
{
    return g_sysboot_dpart_info.runtime_info.dpart_policy;
}
EXPORT_SYMBOL(mdrv_sysboot_get_switch_type);

unsigned int mdrv_sysboot_get_boot_part(void)
{
    return g_sysboot_dpart_info.runtime_info.default_boot_part;
}
EXPORT_SYMBOL(mdrv_sysboot_get_boot_part);

int bsp_sysboot_boot_is_back(void)
{
    bsp_err("boot_part is %s\n", (g_sysboot_dpart_info.runtime_info.default_boot_part == BOOT_FROM_B_PART_FLAG) ? "back" : "master");
    return (g_sysboot_dpart_info.runtime_info.default_boot_part == BOOT_FROM_B_PART_FLAG);
}

static void write_reg_info(void)
{
    unsigned int value;
    struct power_cfg *cfg = power_get_config();

    value = (unsigned int)g_sysboot_dpart_info.runtime_info.reg_info.value | (cfg->boot_protect_mask);
    writel(value, bsp_sysctrl_addr_get((void*)(uintptr_t)cfg->boot_protect_addr_base));
}

static int set_part_switch_state(unsigned int state)
{
    struct dpart_oeminfo dpart_oeminfo;
    dpart_oeminfo.dpart_magic = OEMINFO_DPART_MAGIC;
    dpart_oeminfo.total_boot_times = g_sysboot_dpart_info.runtime_info.oeminfo_total_boot_times;
    dpart_oeminfo.boot_part_switched = state;

    return bsp_oeminfo_private_rgn_write_sync(OEMINFO_PRIVATE_DPART_INFO, &dpart_oeminfo,
        sizeof(struct dpart_oeminfo));
}

static void restore_part_switch_state(void)
{
    int ret;
    if (g_sysboot_dpart_info.runtime_info.boot_part_swiched_state_cleared == 0) {
        return;
    }
    ret = set_part_switch_state(g_sysboot_dpart_info.runtime_info.boot_part_switched);
    if (ret) {
        bsp_err("[sysboot] restore part switched state for oeminfo failed, ret = 0x%x\n", ret);
    }
    bsp_err("[sysboot]: restore part witched state success\n");
}

static void clear_part_switch_state(void)
{
    int ret;

    if (g_sysboot_dpart_info.runtime_info.boot_part_switched == 0) {
        return;
    }
    ret = set_part_switch_state(0);
    if (ret) {
        bsp_err("[sysboot] clear part switched state for oeminfo failed, ret = 0x%x\n", ret);
    }
    g_sysboot_dpart_info.runtime_info.boot_part_swiched_state_cleared = 1;
    bsp_err("[sysboot]: clear part witched state success\n");
}

static void clear_escape_state(void)
{
    u32 value = g_sysboot_dpart_info.runtime_info.reg_info.member.escape_state;
    g_sysboot_dpart_info.runtime_info.reg_info.member.escape_state = 0;
    write_reg_info();
    g_sysboot_dpart_info.runtime_info.reg_info.member.escape_state = value;
    bsp_err("[sysboot]: clear escape flag\n");
}

static void set_boot_times(unsigned int boot_times)
{
    unsigned int value = g_sysboot_dpart_info.runtime_info.reg_info.member.curr_boot_retry_times;
    g_sysboot_dpart_info.runtime_info.reg_info.member.curr_boot_retry_times = boot_times;
    write_reg_info();
    g_sysboot_dpart_info.runtime_info.reg_info.member.curr_boot_retry_times = value;
    bsp_err("[sysboot]:set boot_times 0x%x\n", boot_times);
}

struct sysboot_dpart_info *sysboot_get_dpart_info(void)
{
    return &g_sysboot_dpart_info;
}

/*
 * 功能描述: 设置下载标志:flag: 1表示下载模式启动；0表示normal模式启动
 */
void sysboot_set_boot_download_mode(bool flag)
{
    g_sysboot_dpart_info.runtime_info.reg_info.member.boot_download_mode = (flag != 0 ? 1 : 0);
    write_reg_info();
    bsp_err("set download flag success.\n");
}

/*
 * 功能描述: 获取下载模式: 1表示下载模式启动；0表示normal模式启动；
 */
int sysboot_get_boot_download_mode(void)
{
    int value = (int)g_sysboot_dpart_info.runtime_info.reg_info.member.boot_download_mode;
    bsp_err("get download flag success,value = %d.\n", value);
    return value;
}

const char *sysboot_dpart_get_part_name_by_img_type(unsigned int img_type)
{
    if (img_type >= BOOT_IMG_ID_MAX) {
        bsp_err("img_type %u err\n", img_type);
        return NULL;
    }
    return g_sysboot_img_type_map[img_type].part_name;
}

int bsp_sysboot_restore_dpart_info(void)
{
    /* 需要恢复分区切换状态、逃生状态、当前启动重试次数，分区切换状态需要更新oeminfo分区，逃生状态、当前启动重试次数直接重新写寄存器就可以 */
    restore_part_switch_state();
    write_reg_info();
    return 0;
}

void bsp_sysboot_clear_dpart_info(void)
{
    clear_part_switch_state();
    clear_escape_state();
    set_boot_times(0);
}

int bsp_sysboot_dpart_is_enabled(void)
{
    return g_sysboot_dpart_info.runtime_info.dpart_enable;
}
EXPORT_SYMBOL(bsp_sysboot_dpart_is_enabled);

unsigned int bsp_sysboot_dpart_boot_part_switched(void)
{
    return g_sysboot_dpart_info.runtime_info.boot_part_switched;
}
EXPORT_SYMBOL(bsp_sysboot_dpart_boot_part_switched);

unsigned int bsp_sysboot_dpart_image_load_failed(boot_image_type image_type)
{
    return g_sysboot_dpart_info.runtime_info.image_load_failed & (0x1 << image_type);
}
EXPORT_SYMBOL(bsp_sysboot_dpart_image_load_failed);

int bsp_sysboot_get_exceptional_raw_part(char part_name_arr[BOOT_IMG_RAM_PART_ID_MAX][PART_NAMELEN], unsigned int arr_size, unsigned int *part_nums)
{
    unsigned int ret = 0;
    unsigned int i, j, nums, find_in_arr;
    char *part_name = NULL;
    unsigned int image_loader_failed = g_sysboot_dpart_info.runtime_info.image_load_failed;
    int is_back_part = bsp_sysboot_boot_is_back();

    nums = 0;
    if (g_sysboot_dpart_info.runtime_info.dpart_policy == AB_BOOT_POLICY) {
        goto out;
    }

    if (arr_size < BOOT_IMG_RAM_PART_ID_MAX) {
        bsp_err("arr size %u err\n", arr_size);
        return -1;
    }

    bsp_err("image_loader_failed:0x%x\n", image_loader_failed);

    for (i = 0; i < BOOT_IMG_RAM_PART_ID_MAX; i++) {
        find_in_arr = 0;
        if (image_loader_failed & 0x1U) {
            part_name = (char *)&(g_sysboot_img_type_map[i].part_name[0]);
            for (j = 0; j < nums; j++) {
                if (strncmp(part_name, part_name_arr[j], strlen(part_name)) == 0) {
                    find_in_arr = 1;
                    break;
                }
            }
            if (find_in_arr == 0) {
                ret = (unsigned int)strcpy_s(part_name_arr[nums], PART_NAMELEN, part_name);
                if (is_back_part) {
                    ret |= (unsigned int)strcat_s(part_name_arr[nums], PART_NAMELEN, "bk");
                }
                if (ret) {
                    bsp_err("cpy part_name %s failed, ret = 0x%x\n", part_name, ret);
                    return -1;
                }
                nums++;
            }
        }

        image_loader_failed = image_loader_failed >> 1;
    }

out:
    *part_nums = nums;
    return (int)ret;
}

int bsp_sysboot_get_restore_img_result(void)
{
    return 0;
}

#else

int bsp_sysboot_restore_dpart_info(void)
{
    return 0;
}

void bsp_sysboot_clear_dpart_info(void)
{
    return;
}

int bsp_sysboot_dpart_is_enabled(void)
{
    return 0;
}

int bsp_sysboot_get_exceptional_raw_part(char part_name_arr[BOOT_IMG_RAM_PART_ID_MAX][PART_NAMELEN],
                        unsigned int arr_size, unsigned int *part_nums)
{
    return 0;
}

#endif