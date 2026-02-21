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

#include <securec.h>
#include <osl_module.h>
#include <bsp_print.h>
#include <bsp_avs.h>
#include "avs_base.h"
#include "avs_perchip.h"
#ifdef BSP_CONFIG_PHONE_TYPE
#include <linux/mtd/nve_ap_kernel_interface.h>
#endif

#undef THIS_MODU
#define THIS_MODU mod_avs

AVS_PERCHIP_MEASURE_S g_perchip_measure_info;
perchip_nve_info_s g_perchip_nve_info;

static int perchip_avs_read_nvme(void)
{
    int ret;
    struct opt_nve_info_user nveinfo = { 0 };
    u32 len = sizeof(g_perchip_nve_info);
    nveinfo.nv_operation = 1; /* 1, read nvme */
    nveinfo.nv_number = 566; /* 566, NVME_MABD13_ID */
    nveinfo.valid_size = 104; /* 104, NVME_MABD13_SIZE */
    ret = nve_direct_access_interface(&nveinfo);
    if (ret) {
        avs_err("read avs bd13 nve failed:%d\n", ret);
        return ret;
    }
    ret = memcpy_s(&g_perchip_nve_info, len, nveinfo.nv_data, len);
    if (ret) {
        avs_err("avs memcpy nvedata fail,ret=0x%x\n", ret);
        return ret;
    }
    return 0;
}

static void perchip_measure_info_update(void)
{
    u32 core_id, cur_profile;
    for (core_id = 0; core_id < AVS_DOMAIN_MAX; core_id++) {
        for (cur_profile = 0; cur_profile < AVS_PROFILE_MAX; cur_profile++) {
            g_perchip_measure_info.perchip_measure_info[core_id].perchip_valid_flag[cur_profile] =
                g_perchip_nve_info.perchip_measure[core_id].perchip_valid_flag[cur_profile];
            g_perchip_measure_info.perchip_measure_info[core_id].temp[cur_profile] =
                g_perchip_nve_info.perchip_measure[core_id].perchip_measure_temp[cur_profile];
            g_perchip_measure_info.perchip_measure_info[core_id].vmin[cur_profile] =
                g_perchip_nve_info.perchip_measure[core_id].perchip_measure_vmin[cur_profile];
        }
    }
}
int g_perchip_update_req = 0;
int avs_perchip_req_cb(char *data, unsigned int data_len)
{
    int ret;
    avs_err("perchip req received\n");
    if (data_len != sizeof(int)) {
        avs_err("perchip req len err %d\n", data_len);
        return -1;
    }
    g_perchip_update_req = (*(int *)data);
    avs_err("perchip update info send\n");
    ret = avs_val_notify(AVS_PERCHIP_UPDATE, (char *)&g_perchip_measure_info, sizeof(AVS_PERCHIP_MEASURE_S));
    if (ret) {
        return -1;
    }
    return 0;
}

int perchip_avs_init(void)
{
    int ret;
    ret = perchip_avs_read_nvme();
    if (ret) {
        return -1;
    }
    perchip_measure_info_update();

    avs_msg_cb_register(AVS_PERCHIP_UPDATE_REQ, avs_perchip_req_cb);
    return 0;
}