/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
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

#ifndef __MDRV_PARTITION_NAME_H__
#define __MDRV_PARTITION_NAME_H__

/* partition name define, must be less than 16 characters */
#define PTABLE_XLOADER               "xloader"
#define PTABLE_XLOADER_BK            "xloaderbk"
#define PTABLE_PTABLE                "ptable"
#define PTABLE_PTABLE_BK             "ptablebk"
#define PTABLE_HIBOOT                "hiboot"
#define PTABLE_HIBOOT_BK             "hibootbk"
#define PTABLE_UCE                   "uce"
#define PTABLE_UCE_BK                "ucebk"
#define PTABLE_DTS                   "dts"
#define PTABLE_DTS_BK                "dtsbk"
#define PTABLE_LPMCU                 "lpmcu"
#define PTABLE_LPMCU_BK              "lpmcubk"
#define PTABLE_TEEOS                 "teeos"
#define PTABLE_TEEOS_BK              "teeosbk"
#define PTABLE_TRUSTED_FW            "trusted_fw"
#define PTABLE_TRUSTED_FW_BK         "trusted_fwbk"
#define PTABLE_KERNEL                "kernel"
#define PTABLE_KERNEL_BK             "kernelbk"
#define PTABLE_HACLOAD               "hacload"
#define PTABLE_HACLOAD_BK            "hacloadbk"
#define PTABLE_HIFI                  "hifi"
#define PTABLE_HIFI_BK               "hifibk"
#define PTABLE_OEMINFO               "oeminfo"
#define PTABLE_DEBUG_CERT            "debug_cert"
#define PTABLE_END                   "end"

#define PTABLE_NV_DEFAULT            "nv_default"
#define PTABLE_NV_BACKUP             "nv_backup"
#define PTABLE_NV_BACKUP_BK          "nv_backupbk"
#define PTABLE_NV_IMG                "nv_img"
#define PTABLE_NV_IMG_BK             "nv_imgbk"
#define PTABLE_NV_MODEM              "nv_modem"
#define PTABLE_NV_MODEM_BK           "nv_modembk"
#define PTABLE_NV_CUST               "nv_cust"
#define PTABLE_NV_CUST_BK            "nv_custbk"

#define PTABLE_NVA_DEFAULT           "nva_default"
#define PTABLE_NVA_UPGRADE           "nva_upgrade"
#define PTABLE_NVA_UPGRADE_BK        "nva_upgradebk"
#define PTABLE_NVA_IMG               "nva_img"
#define PTABLE_NVA_IMG_BK            "nva_imgbk"
#define PTABLE_NVA_BACKUP            "nva_backup"
#define PTABLE_NVA_BACKUP_BK         "nva_backupbk"

#define PTABLE_SYSTEM                 "system"
#define PTABLE_SYSTEM_BK              "systembk"
#define PTABLE_MODEM_FW               "modem_fw"
#define PTABLE_MODEM_FW_BK            "modem_fwbk"
#define PTABLE_MODEM_SECURE           "modem_secure"
#define PTABLE_MODEM_SECURE_BK        "modem_securebk"
#define PTABLE_SEC_STORAGE            "sec_storage"
#define PTABLE_SEC_STORAGE_BK         "sec_storagebk"
#define PTABLE_USERDATA               "userdata"
#define PTABLE_USERDATA_BK            "userdatabk"
#define PTABLE_ONLINE                 "online"

#endif
