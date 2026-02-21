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

#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <product_config_drv_ap.h>
#include <bsp_efuse.h>
#include <bsp_print.h>
#include <bsp_sec_call.h>
#include <securec.h>
#include <mdrv_bmi_efuse.h>
#include "efuse_plat.h"
#include "efuse_driver.h"
#include "efuse_layout.h"
#include "efuse_error.h"

#define THIS_MODU mod_efuse

#ifndef CONFIG_EFUSE_IOT
#include <adrv_efuse.h>
#endif

#ifdef CONFIG_EFUSE

/* ************************************************
 函 数 名   : DRV_GET_DIEID
 功能描述   : DRV_GET_DIEID
 输入参数   : buf: data buffer
              len: length of the group
 输出参数   :
 返 回 值   : OK                (0)
              BUF_ERROR         (-55)
              LEN_ERROR         (-56)
              READ_EFUSE_ERROR  (-57)

************************************************ */
int mdrv_efuse_get_dieid(unsigned char *buf, int length)
{
#ifndef CONFIG_EFUSE_IOT
    return get_efuse_dieid_value(buf, (unsigned int)length, 0);
#else
    u32 i = 0;
    int ret;
    efuse_layout_s layout_info = {0};

    if (buf == NULL) {
        efuse_print_error("die id buf is error.\n");
        return EFUSE_ERROR_ARGS;
    }

    ret = bsp_efuse_get_layout_info(EFUSE_DRV_DIE_ID, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    if (length < GROUP_LEN(layout_info.bit_len)) {
        efuse_print_error("die id lenth is error.\n");
        return EFUSE_ERROR_ARGS;
    }

    for (i = 0; i < GROUP_LEN(layout_info.bit_len); i++) {
        buf[i] = 0;
    }

    efuse_print_info("efuse read start group %d length %d.\n", GROUP_INDEX(layout_info.bit), GROUP_LEN(layout_info.bit_len));

    ret = bsp_efuse_read((u32 *)buf, GROUP_INDEX(layout_info.bit), GROUP_LEN(layout_info.bit_len));
    if (ret != EFUSE_OK) {
        efuse_print_error("die id read efuse error.\n");
        return ret;
    }

    efuse_print_info("efuse read end ret %d\n", ret);

    return EFUSE_OK;
#endif
}
EXPORT_SYMBOL(mdrv_efuse_get_dieid);

/* ************************************************
 函 数 名   : DRV_GET_CHIPID
 功能描述   : DRV_GET_CHIPID
 输入参数   : buf: data buffer
              len: length of the group
 输出参数   :
 返 回 值   : OK                (0)
              BUF_ERROR         (-55)
              LEN_ERROR         (-56)
              READ_EFUSE_ERROR  (-57)

************************************************ */
#ifndef CONFIG_EFUSE_IOT
int mdrv_efuse_get_chipid(unsigned char *buf, int length)
{
    return get_efuse_chipid_value(buf, (unsigned int)length, 0);
}
int mdrv_efuse_ioctl(int cmd, int arg, unsigned char *buf, int len)
{
    efuse_print_error("It's only a stub");

    return EFUSE_OK;
}
#else
int mdrv_efuse_get_chipid(unsigned char *buf, int length)
{
    return EFUSE_OK;
}

int bsp_efuse_ioctl_get_socid(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    if ((buf == NULL) || (len < SOC_ID_BYTE_LEN / EFUSE_GROUP_SIZE)) {
        efuse_print_error("get socid fail, args err\n");
        return EFUSE_ERROR_ARGS;
    }

    ret = bsp_sec_call_ext(FUNC_EFUSE_READ, GET_SOC_ID, buf, SOC_ID_BYTE_LEN);
    if (ret) {
        efuse_print_error("efuse ioctl get socid fail.\n");
        return ret;
    }

    return 0;
}

int bsp_efuse_ioctl_set_kce(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    if (buf == NULL) {
        efuse_print_error("bsp_efuse_ioctl_set_kce fail. error args.\n");
        return EFUSE_ERROR_ARGS;
    }

    ret = bsp_efuse_write_prepare();
    if (ret) {
        return ret;
    }

    ret = bsp_sec_call_ext(FUNC_EFUSE_WRITE, SET_SECKCE, buf, len * sizeof(int));
    if (ret) {
        efuse_print_error("efuse ioctl set kce fail.\n");
        bsp_efuse_write_complete();
        return ret;
    }

    bsp_efuse_write_complete();
    return 0;
}


int bsp_efuse_ioctl_get_securestate(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 state = 0;

    ret = bsp_sec_call_ext(FUNC_EFUSE_READ, GET_SECURE_STATE, &state, sizeof(state));
    if (ret) {
        efuse_print_error("efuse ioctl get securestate fail.\n");
        return ret;
    }

    return (int) state;
}

int bsp_efuse_ioctl_set_securestate(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    int value = 0;
    u32 boot_sel = 0;
    efuse_layout_s layout_info = {0};

    if (arg < 0) {
        efuse_print_error("set secure state fail. error args.\n");
        return EFUSE_ERROR_ARGS;
    }

    ret = bsp_efuse_write_prepare();
    if (ret) {
        return ret;
    }

    ret = bsp_sec_call_ext(FUNC_EFUSE_WRITE, SET_SECURE_STATE, &arg, sizeof(arg));
    if (ret != 0 && ret != EFUSE_ERROR_REPEAT_WRITE) {
        efuse_print_error("efuse ioctl set securestate fail.\n");
        bsp_efuse_write_complete();
        return ret;
    }

    bsp_efuse_write_complete();

    if (arg == EFUSE_SECURESTATE_SECURE) {
        ret = bsp_efuse_get_layout_info(EFUSE_BOOT_SEL, &layout_info);
        if (ret) {
            return EFUSE_ERROR_GET_LAYOUT_INFO;
        }

        ret = bsp_efuse_read(&value, GROUP_INDEX(layout_info.bit), 1);
        if (ret) {
            return ret;
        }
        if (((value >> BIT_INDEX_IN_GROUP(layout_info.bit)) & 0x1) == 0x1) {
            efuse_print_error("bool sel repeat write.\n");
            return 0;
        }

        boot_sel |= (0x1U << BIT_INDEX_IN_GROUP(layout_info.bit));
        ret = bsp_efuse_write(&boot_sel, GROUP_INDEX(layout_info.bit), 1);
        if (ret) {
            efuse_print_error("write EFUSE_LAYOUT_BOOT_SEL_BIT_OFFSET fail.\n");
            return ret;
        }
    }

    return ret;
}

int bsp_efuse_ioctl_get_securedebug(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value = 0;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_ARM_DBG_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_read(&value, GROUP_INDEX(layout_info.bit), 1);
    if (ret) {
        return ret;
    }

    ret = (int)((value >> BIT_INDEX_IN_GROUP(layout_info.bit)) & 0x3U);
    return ret;
}

int bsp_efuse_ioctl_set_securedebug(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_ARM_DBG_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_ioctl_get_securedebug(arg, buf, len);
    if (ret < 0) {
        return ret;
    }

    if (ret > 0) {
        return 1;
    }

    value = ((u32)arg & 0x3) << BIT_INDEX_IN_GROUP(layout_info.bit);

    ret = bsp_efuse_write(&value, GROUP_INDEX(layout_info.bit), 1);

    return ret;
}

int bsp_efuse_ioctl_get_dbgen(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value = 0;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_ARM_DBG_CTRL_DBGEN, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_read(&value, GROUP_INDEX(layout_info.bit), 1);
    if (ret) {
        return ret;
    }

    ret = (int)((value >> BIT_INDEX_IN_GROUP(layout_info.bit)) & 0x1U);
    return ret;
}

int bsp_efuse_ioctl_set_dbgen(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_ARM_DBG_CTRL_DBGEN, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_ioctl_get_dbgen(arg, buf, len);
    if (ret < 0) {
        return ret;
    }

    if (ret > 0) {
        return 1;
    }

    value = ((u32)arg & 0x1) << BIT_INDEX_IN_GROUP(layout_info.bit);

    ret = bsp_efuse_write(&value, GROUP_INDEX(layout_info.bit), 1);

    return ret;
}

int bsp_efuse_ioctl_get_niden(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value = 0;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_ARM_DBG_CTRL_NIDEN, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_read(&value, GROUP_INDEX(layout_info.bit), 1);
    if (ret) {
        return ret;
    }

    ret = (int)((value >> BIT_INDEX_IN_GROUP(layout_info.bit)) & 0x1U);
    return ret;
}

int bsp_efuse_ioctl_set_niden(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_ARM_DBG_CTRL_NIDEN, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_ioctl_get_niden(arg, buf, len);
    if (ret < 0) {
        return ret;
    }

    if (ret > 0) {
        return 1;
    }

    value = ((u32)arg & 0x1) << BIT_INDEX_IN_GROUP(layout_info.bit);

    ret = bsp_efuse_write(&value, GROUP_INDEX(layout_info.bit), 1);

    return ret;
}

int bsp_efuse_ioctl_get_spiden(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value = 0;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_ARM_DBG_CTRL_SPIDEN, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_read(&value, GROUP_INDEX(layout_info.bit), 1);
    if (ret) {
        return ret;
    }

    ret = (int)((value >> BIT_INDEX_IN_GROUP(layout_info.bit)) & 0x1U);
    return ret;
}

int bsp_efuse_ioctl_set_spiden(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_ARM_DBG_CTRL_SPIDEN, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_ioctl_get_spiden(arg, buf, len);
    if (ret < 0) {
        return ret;
    }

    if (ret > 0) {
        return 1;
    }

    value = ((u32)arg & 0x1) << BIT_INDEX_IN_GROUP(layout_info.bit);

    ret = bsp_efuse_write(&value, GROUP_INDEX(layout_info.bit), 1);

    return ret;
}

int bsp_efuse_ioctl_get_spniden(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value = 0;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_ARM_DBG_CTRL_SPNIDEN, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_read(&value, GROUP_INDEX(layout_info.bit), 1);
    if (ret) {
        return ret;
    }

    ret = (int)((value >> BIT_INDEX_IN_GROUP(layout_info.bit)) & 0x1U);
    return ret;
}

int bsp_efuse_ioctl_set_spniden(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_ARM_DBG_CTRL_SPNIDEN, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_ioctl_get_spniden(arg, buf, len);
    if (ret < 0) {
        return ret;
    }

    if (ret > 0) {
        return 1;
    }

    value = ((u32)arg & 0x1) << BIT_INDEX_IN_GROUP(layout_info.bit);

    ret = bsp_efuse_write(&value, GROUP_INDEX(layout_info.bit), 1);

    return ret;
}

int bsp_efuse_ioctl_get_hifidbgen(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value = 0;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_HIFI_DBG_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_read(&value, GROUP_INDEX(layout_info.bit), 1);
    if (ret) {
        return ret;
    }

    ret = (int)((value >> BIT_INDEX_IN_GROUP(layout_info.bit)) & 0x1U);
    return ret;
}

int bsp_efuse_ioctl_set_hifidbgen(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_HIFI_DBG_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_ioctl_get_hifidbgen(arg, buf, len);
    if (ret < 0) {
        return ret;
    }

    if (ret > 0) {
        return 1;
    }

    value = ((u32)arg & 0x1) << BIT_INDEX_IN_GROUP(layout_info.bit);

    ret = bsp_efuse_write(&value, GROUP_INDEX(layout_info.bit), 1);

    return ret;
}

int bsp_efuse_ioctl_get_bbe16dbgen(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
#ifdef EFUSE_LAYOUT_BBE16_DBG_CTRL_BIT_OFFSET
    int ret;
    u32 value = 0;

    ret = bsp_efuse_read(&value, GROUP_INDEX(EFUSE_LAYOUT_BBE16_DBG_CTRL_BIT_OFFSET), 1);
    if (ret) {
        return ret;
    }

    ret = (int)((value >> BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_BBE16_DBG_CTRL_BIT_OFFSET)) & 0x1U);
    return ret;
#else
    return 0;
#endif
}

int bsp_efuse_ioctl_set_bbe16dbgen(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
#ifdef EFUSE_LAYOUT_BBE16_DBG_CTRL_BIT_OFFSET
    int ret;
    u32 value;

    ret = bsp_efuse_ioctl_get_bbe16dbgen(arg);
    if (ret < 0) {
        return ret;
    }

    if (ret > 0) {
        return 1;
    }

    value = ((u32)arg & 0x1) << BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_BBE16_DBG_CTRL_BIT_OFFSET);

    ret = bsp_efuse_write(&value, GROUP_INDEX(EFUSE_LAYOUT_BBE16_DBG_CTRL_BIT_OFFSET), 1);

    return ret;
#else
    return 0;
#endif
}

int bsp_efuse_ioctl_get_csdeviceen(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value = 0;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_CS_DEVICE_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_read(&value, GROUP_INDEX(layout_info.bit), 1);
    if (ret) {
        return ret;
    }

    ret = (int)((value >> BIT_INDEX_IN_GROUP(layout_info.bit)) & 0x1U);
    return ret;
}

int bsp_efuse_ioctl_set_csdeviceen(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_CS_DEVICE_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_ioctl_get_csdeviceen(arg, buf, len);
    if (ret < 0) {
        return ret;
    }

    if (ret > 0) {
        return 1;
    }

    value = ((u32)arg & 0x1) << BIT_INDEX_IN_GROUP(layout_info.bit);

    ret = bsp_efuse_write(&value, GROUP_INDEX(layout_info.bit), 1);

    return ret;
}

int bsp_efuse_ioctl_get_jtagen(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value = 0;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_JTAGEN_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_read(&value, GROUP_INDEX(layout_info.bit), 1);
    if (ret) {
        return ret;
    }

    ret = (int)((value >> BIT_INDEX_IN_GROUP(layout_info.bit)) & 0x1U);
    return ret;
}

int bsp_efuse_ioctl_set_jtagen(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_JTAGEN_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_ioctl_get_jtagen(arg, buf, len);
    if (ret < 0) {
        return ret;
    }

    if (ret > 0) {
        return 1;
    }

    value = ((u32)arg & 0x1) << BIT_INDEX_IN_GROUP(layout_info.bit);

    ret = bsp_efuse_write(&value, GROUP_INDEX(layout_info.bit), 1);

    return ret;
}

int bsp_efuse_ioctl_get_secdbgreset(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value = 0;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_SEC_DBG_RST_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_read(&value, GROUP_INDEX(layout_info.bit), 1);
    if (ret) {
        return ret;
    }

    ret = (int)((value >> BIT_INDEX_IN_GROUP(layout_info.bit)) & 0x1U);
    return ret;
}

int bsp_efuse_ioctl_set_secdbgreset(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_SEC_DBG_RST_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_ioctl_get_secdbgreset(arg, buf, len);
    if (ret < 0) {
        return ret;
    }

    if (ret > 0) {
        return 1;
    }

    value = ((u32)arg & 0x1) << BIT_INDEX_IN_GROUP(layout_info.bit);

    ret = bsp_efuse_write(&value, GROUP_INDEX(layout_info.bit), 1);

    return ret;
}

int bsp_efuse_ioctl_get_csreset(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value = 0;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_CORESIGHT_RST_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_read(&value, GROUP_INDEX(layout_info.bit), 1);
    if (ret) {
        return ret;
    }

    ret = (int)((value >> BIT_INDEX_IN_GROUP(layout_info.bit)) & 0x1U);
    return ret;
}

int bsp_efuse_ioctl_set_csreset(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_CORESIGHT_RST_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_ioctl_get_csreset(arg, buf, len);
    if (ret < 0) {
        return ret;
    }

    if (ret > 0) {
        return 1;
    }

    value = ((u32)arg & 0x1) << BIT_INDEX_IN_GROUP(layout_info.bit);

    ret = bsp_efuse_write(&value, GROUP_INDEX(layout_info.bit), 1);

    return ret;
}

int bsp_efuse_ioctl_get_dftsel(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value = 0;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_DFT_DISABLE_SEL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_read(&value, GROUP_INDEX(layout_info.bit), 1);
    if (ret) {
        return ret;
    }

    ret = (int)((value >> BIT_INDEX_IN_GROUP(layout_info.bit)) & 0x3U);
    return ret;
}

int bsp_efuse_ioctl_set_dftsel(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_DFT_DISABLE_SEL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_ioctl_get_dftsel(arg, buf, len);
    if (ret < 0) {
        return ret;
    }

    if (ret > 0) {
        return 1;
    }

    value = ((u32)arg & 0x3) << BIT_INDEX_IN_GROUP(layout_info.bit);

    ret = bsp_efuse_write(&value, GROUP_INDEX(layout_info.bit), 1);

    return ret;
}

int bsp_efuse_ioctl_get_authkey(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_DFT_AUTH_KEY, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    if ((!buf) || (len < GROUP_LEN(layout_info.bit_len))) {
        efuse_print_error("bsp_efuse_ioctl_get_authkey fail. error args.\n");
        return EFUSE_ERROR_ARGS;
    }

    ret = bsp_efuse_read((u32 *)buf, GROUP_INDEX(layout_info.bit), GROUP_LEN(layout_info.bit_len));
    if (ret) {
        return ret;
    }

    return ret;
}

int bsp_efuse_ioctl_set_authkey(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    u32 i = 0;
    int ret;
    efuse_layout_s layout_info = {0};
    u32 *value;

    ret = bsp_efuse_get_layout_info(EFUSE_DFT_AUTH_KEY, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    value = kzalloc(sizeof(u32) * (GROUP_LEN(layout_info.bit_len)), GFP_KERNEL);
    if (IS_ERR_OR_NULL(value)) {
        efuse_print_error("malloc buffer fail\n");
        return EFUSE_ERROR_BUFFER_EMPTY;
    }

    if ((!buf) || (len < GROUP_LEN(layout_info.bit_len))) {
        efuse_print_error("bsp_efuse_ioctl_set_authkey fail. error args.\n");
        return EFUSE_ERROR_ARGS;
    }

    ret = bsp_efuse_ioctl_get_authkey(arg, (unsigned char *)value, len);
    if (ret < 0) {
        return ret;
    }

    for (i = 0; i < GROUP_LEN(layout_info.bit_len); i++) {
        if (value[i]) {
            return 1;
        }
    }

    ret = bsp_efuse_write((u32 *)buf, GROUP_INDEX(layout_info.bit), GROUP_LEN(layout_info.bit_len));

    return ret;
}

int bsp_efuse_ioctl_get_authkeyrd(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value = 0;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_DFT_AUTH_KEY_RD_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_read(&value, GROUP_INDEX(layout_info.bit), 1);
    if (ret) {
        return ret;
    }

    ret = (int)((value >> BIT_INDEX_IN_GROUP(layout_info.bit)) & 0x1U);
    return ret;
}

int bsp_efuse_ioctl_set_authkeyrd(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_DFT_AUTH_KEY_RD_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_ioctl_get_authkeyrd(arg, buf, len);
    if (ret < 0) {
        return ret;
    }

    if (ret > 0) {
        return 1;
    }

    value = ((u32)arg & 0x1U) << BIT_INDEX_IN_GROUP(layout_info.bit);

    ret = bsp_efuse_write(&value, GROUP_INDEX(layout_info.bit), 1);

    return ret;
}

int bsp_efuse_ioctl_get_nsiverify(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
#ifdef EFUSE_LAYOUT_NS_VERIFY_BIT_OFFSET
    int ret;
    u32 value = 0;

    ret = bsp_efuse_read(&value, GROUP_INDEX(EFUSE_LAYOUT_NS_VERIFY_BIT_OFFSET), 1);
    if (ret) {
        return ret;
    }

    ret = (int)((value >> BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_NS_VERIFY_BIT_OFFSET)) & 0x3U);
    return ret;
#else
    return 0;
#endif
}

int bsp_efuse_ioctl_set_nsiverify(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
#ifdef EFUSE_LAYOUT_NS_VERIFY_BIT_OFFSET
    int ret;
    u32 value;

    ret = bsp_efuse_ioctl_get_nsiverify(arg);
    if (ret < 0) {
        return ret;
    }

    if (ret > 0) {
        return 1;
    }

    value = ((u32)arg & 0x3) << BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_NS_VERIFY_BIT_OFFSET);

    ret = bsp_efuse_write(&value, GROUP_INDEX(EFUSE_LAYOUT_NS_VERIFY_BIT_OFFSET), 1);

    return ret;
#else
    return 0;
#endif
}

int bsp_efuse_ioctl_get_uartdbgen(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value = 0;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_UART_DBG_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_read(&value, GROUP_INDEX(layout_info.bit), 1);
    if (ret) {
        return ret;
    }

    ret = (int)((value >> BIT_INDEX_IN_GROUP(layout_info.bit)) & 0x1U);

    return ret;
}

int bsp_efuse_ioctl_set_uartdbgen(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_UART_DBG_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_ioctl_get_uartdbgen(arg, buf, len);
    if (ret < 0) {
        return ret;
    }

    if (ret > 0) {
        return 1;
    }

    value = ((u32)arg & 0x1) << BIT_INDEX_IN_GROUP(layout_info.bit);

    ret = bsp_efuse_write(&value, GROUP_INDEX(layout_info.bit), 1);

    return ret;
}

int bsp_efuse_ioctl_get_nxbbe32dbgen(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
#ifdef EFUSE_LAYOUT_NXBBE32_DBG_CTRL_BIT_OFFSET
    int ret;
    u32 value = 0;

    ret = bsp_efuse_read(&value, GROUP_INDEX(EFUSE_LAYOUT_NXBBE32_DBG_CTRL_BIT_OFFSET), 1);
    if (ret) {
        return ret;
    }

    ret = (int)((value >> BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_NXBBE32_DBG_CTRL_BIT_OFFSET)) & 0x1U);

    return ret;
#else
    return 0;
#endif
}

int bsp_efuse_ioctl_set_nxbbe32dbgen(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
#ifdef EFUSE_LAYOUT_NXBBE32_DBG_CTRL_BIT_OFFSET
    int ret;
    u32 value;

    ret = bsp_efuse_ioctl_get_nxbbe32dbgen(arg);
    if (ret < 0) {
        return ret;
    }

    if (ret > 0) {
        return 1;
    }

    value = ((u32)arg & 0x1) << BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_NXBBE32_DBG_CTRL_BIT_OFFSET);

    ret = bsp_efuse_write(&value, GROUP_INDEX(EFUSE_LAYOUT_NXBBE32_DBG_CTRL_BIT_OFFSET), 1);

    return ret;
#else
    return 0;
#endif
}

int bsp_efuse_ioctl_get_pdedbgen(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value = 0;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_PDE_DBG_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_read(&value, GROUP_INDEX(layout_info.bit), 1);
    if (ret) {
        return ret;
    }

    ret = (int)((value >> BIT_INDEX_IN_GROUP(layout_info.bit)) & 0x1U);

    return ret;
}

int bsp_efuse_ioctl_set_pdedbgen(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_PDE_DBG_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_ioctl_get_pdedbgen(arg, buf, len);
    if (ret < 0) {
        return ret;
    }

    if (ret > 0) {
        return 1;
    }

    value = ((u32)arg & 0x1) << BIT_INDEX_IN_GROUP(layout_info.bit);

    ret = bsp_efuse_write(&value, GROUP_INDEX(layout_info.bit), 1);

    return ret;
}

int bsp_efuse_ioctl_get_acpudbgen(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value = 0;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_ACPU_NIDEN_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_read(&value, GROUP_INDEX(layout_info.bit), 1);
    if (ret) {
        return ret;
    }

    ret = (int)((value >> BIT_INDEX_IN_GROUP(layout_info.bit)) & 0x1U);

    return ret;
}

int bsp_efuse_ioctl_set_acpudbgen(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_ACPU_NIDEN_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_ioctl_get_acpudbgen(arg, buf, len);
    if (ret < 0) {
        return ret;
    }

    if (ret > 0) {
        return 1;
    }

    value = ((u32)arg & 0x1) << BIT_INDEX_IN_GROUP(layout_info.bit);

    ret = bsp_efuse_write(&value, GROUP_INDEX(layout_info.bit), 1);

    return ret;
}

int bsp_efuse_ioctl_get_txpdbgen(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value = 0;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_TXP_NIDEN_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_read(&value, GROUP_INDEX(layout_info.bit), 1);
    if (ret) {
        return ret;
    }

    ret = (int)((value >> BIT_INDEX_IN_GROUP(layout_info.bit)) & 0x1U);

    return ret;
}

int bsp_efuse_ioctl_set_txpdbgen(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_TXP_NIDEN_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_ioctl_get_txpdbgen(arg, buf, len);
    if (ret < 0) {
        return ret;
    }

    if (ret > 0) {
        return 1;
    }

    value = ((u32)arg & 0x1) << BIT_INDEX_IN_GROUP(layout_info.bit);

    ret = bsp_efuse_write(&value, GROUP_INDEX(layout_info.bit), 1);

    return ret;
}

int bsp_efuse_ioctl_get_usbbootdis(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value = 0;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_USB_BOOT_DIS_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_read(&value, GROUP_INDEX(layout_info.bit), 1);
    if (ret) {
        return ret;
    }

    ret = (int)((value >> BIT_INDEX_IN_GROUP(layout_info.bit)) & 0x1U);

    return ret;
}

int bsp_efuse_ioctl_set_usbbootdis(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_USB_BOOT_DIS_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_ioctl_get_usbbootdis(arg, buf, len);
    if (ret < 0) {
        return ret;
    }

    if (ret > 0) {
        return 1;
    }

    value = ((u32)arg & 0x1) << BIT_INDEX_IN_GROUP(layout_info.bit);

    ret = bsp_efuse_write(&value, GROUP_INDEX(layout_info.bit), 1);

    return ret;
}

int bsp_efuse_ioctl_get_gmecckeyrddis(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value = 0;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_GMECC_KEY_RD_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_read(&value, GROUP_INDEX(layout_info.bit), 1);
    if (ret) {
        return ret;
    }

    ret = (int)((value >> BIT_INDEX_IN_GROUP(layout_info.bit)) & 0x1U);

    return ret;
}

int bsp_efuse_ioctl_set_gmecckeyrddis(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    u32 value;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_GMECC_KEY_RD_CTRL, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    ret = bsp_efuse_ioctl_get_gmecckeyrddis(arg, buf, len);
    if (ret < 0) {
        return ret;
    }

    if (ret > 0) {
        return 1;
    }

    value = ((u32)arg & 0x1) << BIT_INDEX_IN_GROUP(layout_info.bit);

    ret = bsp_efuse_write(&value, GROUP_INDEX(layout_info.bit), 1);

    return ret;
}

int bsp_efuse_ioctl_get_product_region(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    efuse_layout_s layout_info = {0};
    struct efuse_desc_t *efuse_desc = (struct efuse_desc_t *)(uintptr_t)buf;

    ret = bsp_efuse_get_layout_info(EFUSE_MDRV_PRODUCT_REGIONS, &layout_info);
    if (ret) {
        return EFUSE_ERROR;
    }

    if ((efuse_desc == NULL) ||
        (len != sizeof(struct efuse_desc_t)) ||
        (efuse_desc->bit_len == 0) ||
        (efuse_desc->bit_start >= layout_info.bit_len) ||
        (efuse_desc->bit_len > layout_info.bit_len)) {
        return EFUSE_ERROR_ARGS;
    }

    ret = bsp_efuse_read_by_bit(efuse_desc->buf, efuse_desc->bit_start + layout_info.bit, efuse_desc->bit_len);
    if (ret) {
        efuse_print_error("efuse ioctl get product region fail.\n");
        return ret;
    }

    return 0;
}

int bsp_efuse_ioctl_set_product_region(int arg __attribute__((unused)), unsigned char *buf __attribute__((unused)), int len __attribute__((unused)))
{
    int ret;
    efuse_layout_s layout_info = {0};
    u32 group_len = 0;
    u32 group_value[EFUSE_MAX_PACK_SIZE] = {0};
    u32 cur_value = 0, cur_group = 0, cur_bit = 0;
    u32 i;
    struct efuse_desc_t *efuse_desc = (struct efuse_desc_t *)(uintptr_t)buf;
    struct efuse_desc_t read_efuse_desc = {
        .buf = {0},
        .bit_start = efuse_desc->bit_start,
        .bit_len = efuse_desc->bit_len,
    };

    ret = bsp_efuse_get_layout_info(EFUSE_MDRV_PRODUCT_REGIONS, &layout_info);
    if (ret) {
        return EFUSE_ERROR;
    }

    if ((efuse_desc == NULL) || (len != sizeof(struct efuse_desc_t)) ||
        (efuse_desc->bit_len == 0) || (efuse_desc->bit_start >= layout_info.bit_len) ||
        (efuse_desc->bit_len > layout_info.bit_len)) {
        return EFUSE_ERROR_ARGS;
    }

    group_len = efuse_calc_group_len(layout_info.bit, layout_info.bit_len);

    ret = bsp_efuse_ioctl_get_product_region(0, (unsigned char *)(uintptr_t)&read_efuse_desc, sizeof(struct efuse_desc_t));
    if (ret) {
        return ret;
    }
    for (i = 0; i < EFUSE_MAX_PACK_SIZE; i++) {
        if (group_value[i]) {
            return EFUSE_ERROR_REPEAT_WRITE;
        }
    }

    for (i = 0; i < efuse_desc->bit_len; i++) {
        cur_bit = BIT_INDEX_IN_GROUP(efuse_desc->bit_start + layout_info.bit + i);
        cur_group = GROUP_INDEX(efuse_desc->bit_start + layout_info.bit + i) - GROUP_INDEX(layout_info.bit);
        cur_value = get_value_by_bit(efuse_desc->buf[i / EFUSE_GROUP_BIT_NUM], i % EFUSE_GROUP_BIT_NUM, i % EFUSE_GROUP_BIT_NUM);

        set_value_by_bit(&group_value[cur_group], cur_bit, cur_bit, cur_value);
    }

    ret = bsp_efuse_write(group_value, GROUP_INDEX(layout_info.bit), group_len);
    if (ret) {
        efuse_print_error("efuse ioctl set product region fail.\n");
        return ret;
    }

    return 0;
}

int bsp_efuse_ioctl_get_usb_vendor_device_id(int arg __attribute__((unused)), unsigned char *buf, int len)
{
    int ret;
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_DRV_USB_VENDOR_DEVICE_ID, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    if ((buf == NULL) || (len < GROUP_LEN(layout_info.bit_len))) {
        efuse_print_error("get_usb_device_id fail. error args.\n");
        return EFUSE_ERROR_ARGS;
    }

    ret = bsp_efuse_read((u32 *)buf, GROUP_INDEX(layout_info.bit), GROUP_LEN(layout_info.bit_len));
    if (ret) {
        return ret;
    }

    return ret;
}

int bsp_efuse_ioctl_set_usb_vendor_device_id(int arg __attribute__((unused)), unsigned char *buf, int len)
{
    int ret;
    int i;
    unsigned char value[4] = {0};
    efuse_layout_s layout_info = {0};

    ret = bsp_efuse_get_layout_info(EFUSE_DRV_USB_VENDOR_DEVICE_ID, &layout_info);
    if (ret) {
        return EFUSE_ERROR_GET_LAYOUT_INFO;
    }

    if ((buf == NULL) || (len < GROUP_LEN(layout_info.bit_len))) {
        efuse_print_error("set_usb_device_id fail. error args.\n");
        return EFUSE_ERROR_ARGS;
    }

    ret = bsp_efuse_ioctl_get_usb_vendor_device_id(0, value, sizeof(value));
    if (ret < 0) {
        return ret;
    }

    for (i = 0; i < GROUP_LEN(layout_info.bit_len) * EFUSE_GROUP_SIZE; i++) {
        if (value[i]) {
            efuse_print_error("usb id repeat write.\n");
            return 1;
        }
    }

    ret = bsp_efuse_write((u32 *)buf, GROUP_INDEX(layout_info.bit), GROUP_LEN(layout_info.bit_len));

    return ret;
}

struct efuse_ioctrl_op {
    int op_code;
    int (*func_ioctrl)(int, unsigned char *, int);
};

struct efuse_ioctrl_op g_efuse_ioctrl_op[] = {
    { MDRV_EFUSE_IOCTL_CMD_GET_SOCID, bsp_efuse_ioctl_get_socid },
    { MDRV_EFUSE_IOCTL_CMD_SET_KCE, bsp_efuse_ioctl_set_kce },
    { MDRV_EFUSE_IOCTL_CMD_SET_AUTHKEY, bsp_efuse_ioctl_set_authkey },
    { MDRV_EFUSE_IOCTL_CMD_GET_AUTHKEY, bsp_efuse_ioctl_get_authkey },
    { MDRV_EFUSE_IOCTL_CMD_SET_SECURESTATE, bsp_efuse_ioctl_set_securestate },
    { MDRV_EFUSE_IOCTL_CMD_GET_SECURESTATE, bsp_efuse_ioctl_get_securestate },
    { MDRV_EFUSE_IOCTL_CMD_SET_SECUREDEBUG, bsp_efuse_ioctl_set_securedebug },
    { MDRV_EFUSE_IOCTL_CMD_GET_SECUREDEBUG, bsp_efuse_ioctl_get_securedebug },
    { MDRV_EFUSE_IOCTL_CMD_SET_DBGEN, bsp_efuse_ioctl_set_dbgen },
    { MDRV_EFUSE_IOCTL_CMD_GET_DBGEN, bsp_efuse_ioctl_get_dbgen },
    { MDRV_EFUSE_IOCTL_CMD_SET_NIDEN, bsp_efuse_ioctl_set_niden },
    { MDRV_EFUSE_IOCTL_CMD_GET_NIDEN, bsp_efuse_ioctl_get_niden },
    { MDRV_EFUSE_IOCTL_CMD_SET_SPIDEN, bsp_efuse_ioctl_set_spiden },
    { MDRV_EFUSE_IOCTL_CMD_GET_SPIDEN, bsp_efuse_ioctl_get_spiden },
    { MDRV_EFUSE_IOCTL_CMD_SET_SPNIDEN, bsp_efuse_ioctl_set_spniden },
    { MDRV_EFUSE_IOCTL_CMD_GET_SPNIDEN, bsp_efuse_ioctl_get_spniden },
    { MDRV_EFUSE_IOCTL_CMD_SET_HIFIDBGEN, bsp_efuse_ioctl_set_hifidbgen },
    { MDRV_EFUSE_IOCTL_CMD_GET_HIFIDBGEN, bsp_efuse_ioctl_get_hifidbgen },
    { MDRV_EFUSE_IOCTL_CMD_SET_BBE16DBGEN, bsp_efuse_ioctl_set_bbe16dbgen },
    { MDRV_EFUSE_IOCTL_CMD_GET_BBE16DBGEN, bsp_efuse_ioctl_get_bbe16dbgen },
    { MDRV_EFUSE_IOCTL_CMD_SET_CSDEVICEEN, bsp_efuse_ioctl_set_csdeviceen },
    { MDRV_EFUSE_IOCTL_CMD_GET_CSDEVICEEN, bsp_efuse_ioctl_get_csdeviceen },
    { MDRV_EFUSE_IOCTL_CMD_SET_JTAGEN, bsp_efuse_ioctl_set_jtagen },
    { MDRV_EFUSE_IOCTL_CMD_GET_JTAGEN, bsp_efuse_ioctl_get_jtagen },
    { MDRV_EFUSE_IOCTL_CMD_SET_SECDBGRESET, bsp_efuse_ioctl_set_secdbgreset },
    { MDRV_EFUSE_IOCTL_CMD_GET_SECDBGRESET, bsp_efuse_ioctl_get_secdbgreset },
    { MDRV_EFUSE_IOCTL_CMD_SET_CSRESET, bsp_efuse_ioctl_set_csreset },
    { MDRV_EFUSE_IOCTL_CMD_GET_CSRESET, bsp_efuse_ioctl_get_csreset },
    { MDRV_EFUSE_IOCTL_CMD_SET_DFTSEL, bsp_efuse_ioctl_set_dftsel },
    { MDRV_EFUSE_IOCTL_CMD_GET_DFTSEL, bsp_efuse_ioctl_get_dftsel },
    { MDRV_EFUSE_IOCTL_CMD_SET_AUTHKEYRD, bsp_efuse_ioctl_set_authkeyrd },
    { MDRV_EFUSE_IOCTL_CMD_GET_AUTHKEYRD, bsp_efuse_ioctl_get_authkeyrd },
    { MDRV_EFUSE_IOCTL_CMD_SET_NSIVERIFY, bsp_efuse_ioctl_set_nsiverify },
    { MDRV_EFUSE_IOCTL_CMD_GET_NSIVERIFY, bsp_efuse_ioctl_get_nsiverify },
    { MDRV_EFUSE_IOCTL_CMD_SET_UARTDBGEN, bsp_efuse_ioctl_set_uartdbgen },
    { MDRV_EFUSE_IOCTL_CMD_GET_UARTDBGEN, bsp_efuse_ioctl_get_uartdbgen },
    { MDRV_EFUSE_IOCTL_CMD_SET_NXBBE32DBGEN, bsp_efuse_ioctl_set_nxbbe32dbgen },
    { MDRV_EFUSE_IOCTL_CMD_GET_NXBBE32DBGEN, bsp_efuse_ioctl_get_nxbbe32dbgen },
    { MDRV_EFUSE_IOCTL_CMD_SET_PDEDBGEN, bsp_efuse_ioctl_set_pdedbgen },
    { MDRV_EFUSE_IOCTL_CMD_GET_PDEDBGEN, bsp_efuse_ioctl_get_pdedbgen },
    { MDRV_EFUSE_IOCTL_CMD_SET_CCPUNIDEN, bsp_efuse_ioctl_set_txpdbgen },
    { MDRV_EFUSE_IOCTL_CMD_GET_CCPUNIDEN, bsp_efuse_ioctl_get_txpdbgen },
    { MDRV_EFUSE_IOCTL_CMD_SET_ACPUNIDEN, bsp_efuse_ioctl_set_acpudbgen },
    { MDRV_EFUSE_IOCTL_CMD_GET_ACPUNIDEN, bsp_efuse_ioctl_get_acpudbgen },
    { MDRV_EFUSE_IOCTL_CMD_SET_USBBOOTDIS, bsp_efuse_ioctl_set_usbbootdis },
    { MDRV_EFUSE_IOCTL_CMD_GET_USBBOOTDIS, bsp_efuse_ioctl_get_usbbootdis },
    { MDRV_EFUSE_IOCTL_CMD_SET_GMECCKEYRDDIS, bsp_efuse_ioctl_set_gmecckeyrddis },
    { MDRV_EFUSE_IOCTL_CMD_GET_GMECCKEYRDDIS, bsp_efuse_ioctl_get_gmecckeyrddis },
    { MDRV_EFUSE_IOCTL_CMD_GET_PRODUCT_REGION, bsp_efuse_ioctl_get_product_region},
    { MDRV_EFUSE_IOCTL_CMD_SET_PRODUCT_REGION, bsp_efuse_ioctl_set_product_region},
    { MDRV_EFUSE_IOCTL_CMD_GET_USB_VENDOR_DEVICE_ID, bsp_efuse_ioctl_get_usb_vendor_device_id},
    { MDRV_EFUSE_IOCTL_CMD_SET_USB_VENDOR_DEVICE_ID, bsp_efuse_ioctl_set_usb_vendor_device_id},
};

int mdrv_efuse_ioctl(int cmd, int arg, unsigned char *buf, int len)
{
    u32 i;
    int ret = 0;

    for (i = 0; i < sizeof(g_efuse_ioctrl_op) / sizeof(g_efuse_ioctrl_op[0]); i++) {
        if (cmd == g_efuse_ioctrl_op[i].op_code) {
            ret = g_efuse_ioctrl_op[i].func_ioctrl(arg, buf, len);
            return ret;
        }
    }

    efuse_print_error("unsupported command, cmd = %d\n", cmd);
    return EFUSE_ERROR_ARGS;
}
#endif


#else /* CONFIG_EFUSE */

int mdrv_efuse_write_huk(char *buf, unsigned int len)
{
    return EFUSE_OK;
}

int mdrv_efuse_check_huk_valid(void)
{
    return EFUSE_OK;
}

int mdrv_efuse_get_dieid(unsigned char *buf, int length)
{
    return EFUSE_OK;
}

int mdrv_efuse_get_chipid(unsigned char *buf, int length)
{
    return EFUSE_OK;
}

int mdrv_efuse_ioctl(int cmd, int arg, unsigned char *buf, int len)
{
    return EFUSE_OK;
}

#endif

EXPORT_SYMBOL(mdrv_efuse_ioctl);
EXPORT_SYMBOL(mdrv_efuse_get_chipid);
