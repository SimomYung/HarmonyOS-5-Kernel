/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2024. All rights reserved.
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
#include "pmctrl_bias.h"
#include <securec.h>
#include <linux/module.h>
#include <linux/init.h>
#include <product_config.h>
#include <bsp_sec_call.h>
#include <linux/mtd/nve_ap_kernel_interface.h>

#define THIS_MODU mod_pmctrl

int pmctrl_bias_read_nvme(unsigned int power_index, u8 *buf)
{
    int ret;
    struct opt_nve_info_user nveinfo = { 0 };

    nveinfo.nv_operation = PMCTRL_NVME_MAMSW_READ;
    nveinfo.nv_number = PMCTRL_NVME_MAMSW_ID + power_index;
    nveinfo.valid_size = PMCTRL_NVME_MAMSW_SIZE;
    ret = nve_direct_access_interface(&nveinfo);
    if (ret) {
        pmctrl_err("read pmctrl nve failed:%d\n", ret);
        return ret;
    }

    ret = memcpy_s(buf, PMCTRL_NVME_MAMSW_SIZE, nveinfo.nv_data, nveinfo.valid_size);
    if (ret) {
        pmctrl_err("memcpy fail,ret=0x%x\n", ret);
        return ret;
    }
    return 0;
}

/*lint -save -e429*/
int pmctrl_bias_init(void)
{
    int ret;
    unsigned int power_idx;
    char pmctrl_bias_volt[PMCTRL_NVME_MAMSW_SIZE] = { 0 };

    for (power_idx = 0; power_idx < POWER_NUM_MAX; power_idx++) {
        ret = pmctrl_bias_read_nvme(power_idx, pmctrl_bias_volt);
        if (ret) {
            return ret;
        }
        ret = bsp_sec_call_ext(FUNC_MDRV_PMCTRL_VBIAS, power_idx, pmctrl_bias_volt, PMCTRL_NVME_MAMSW_SIZE);
        if (ret) {
            pmctrl_err("pmctrl sec_call fail,ret=0x%x\n", ret);
            return ret;
        }
    }

    pmctrl_err("pmctrl bias init ok!\n");
    return ret;
}
EXPORT_SYMBOL(pmctrl_bias_init);

#if !IS_ENABLED(CONFIG_MODEM_DRIVER)
/*lint -restore +e429*/
module_init(pmctrl_bias_init);
#endif
