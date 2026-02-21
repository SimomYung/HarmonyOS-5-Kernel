/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2025. All rights reserved.
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
#include <osl_types.h>
#include <bsp_version.h>
#include <bsp_dump.h>
#include <bsp_dump_mem.h>
#include <bsp_dt.h>
#include <dump_infra.h>

/* cust_phone for dump_area.c */
int dump_areainfo_fill(struct dump_global_struct_s *dump_head)
{
    device_node_s *dev = NULL;
    device_node_s *child_node = NULL;
    u32 area_size = 0;
    u32 area_offset = 0;
    u32 area_id = 0;
    dev = bsp_dt_find_node_by_path("/modem_mntn_ddr_layout");
    if (dev == NULL) {
        dump_error("/modem_mntn_ddr_layout node no find!\n");
        return 0;
    }
    if (bsp_dt_property_read_u32_array(dev, "baseinfo_size", &area_size, 1)) {
        dump_error("get baseinfo_size failed.\n");
        return ERROR;
    }
    bsp_dt_for_each_child_of_node(dev, child_node) {
        area_offset += area_size;
        if (bsp_dt_property_read_u32_array(child_node, "id", &area_id, 1)) {
            dump_error("dump area_id not found\n");
            return ERROR;
        }
        if (bsp_dt_property_read_u32_array(child_node, "size", &area_size, 1)) {
            dump_error("dump area_size not found.\n");
            return ERROR;
        }
        if (area_id >= DUMP_AREA_BUTT) {
            dump_error("dump area_id invalid.\n");
            return ERROR;
        }
        dump_head->area_info[area_id].offset = area_offset;
        dump_head->area_info[area_id].length = area_size;
    }
    area_offset += area_size;
    if (bsp_dt_property_read_u32_array(dev, "resv_offset", &area_size, 1)) {
        dump_error("no resv_offset\n");
        area_size = 0;
    }
    dump_head->resv_offset = area_offset + area_size;

    return 0;
}

void dump_alloc_area(struct dump_global_struct_s *dump_head)
{
    dump_error("enter phone area init\n");
    (void)memset_s(dump_head, sizeof(*dump_head), 0, sizeof(*dump_head));
    if (dump_areainfo_fill(dump_head) != 0) {
        dump_error("alloc area field\n");
        return;
    }
    dump_head->top_head.area_number = DUMP_AREA_BUTT;
    dump_head->top_head.magic = DUMP_GLOBALE_TOP_HEAD_MAGIC;

    if (mdrv_ver_get_info() == NULL) {
        dump_error("get version fail\n");
    }

    if (strcpy_s((char *)dump_head->top_head.product_name, sizeof(dump_head->top_head.product_name),
        bsp_version_get_product_name())) {
        dump_error("err\n");
    }
    if (strcpy_s((char *)dump_head->top_head.product_version, sizeof(dump_head->top_head.product_version),
        bsp_version_get_release_ver())) {
        dump_error("err\n");
    }
}