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
#include <linux/sched/debug.h>
#include <osl_malloc.h>
#include <mdrv_memory_layout.h>
#include <bsp_dump.h>
#include <bsp_dump_mem.h>
#include <bsp_adump.h>
#include <dump_frame.h>
#include <dump_infra.h>
#include <dump_config.h>
#include <dump_subsys_mgr.h>
#include <dump_teeos_service.h>
#include <dump_log_strategy.h>
#include "dump_lastkmsg.h"
#include "dump_ko.h"

/******************************* 内部dump流程总需要保存的log相关接口 ***************************/
/*
 * 功能描述: 打印调用栈，用于非arm异常
 */
void dump_show_stack(u32 modid, u32 reason)
{
    if (!(reason == AP_DUMP_REASON_ARM)) {
        if (modid == 0x11000025 || modid == 0x1100002A) {
            dump_error("not current task exc\n");
            show_stack(find_task_by_vpid(reason), NULL, KERN_ERR);
        } else {
            show_stack(current, NULL, KERN_ERR);
        }
    }
}

/*
 * 功能描述: 保存用户数据区
 */
void dump_save_usr_data(const char *data, u32 length)
{
    u32 len = 0;
    void *addr = NULL;
    dump_field_map_s *pfield = NULL;
    u32 usr_data_size = dump_get_u32_cfg(DUMP_CFG_USERDATA_SIZE);

    if ((data != NULL) && (length)) {
        pfield = (dump_field_map_s *)bsp_dump_get_field_map(DUMP_MODEMAP_USER_DATA);
        addr = (void *)bsp_dump_get_field_addr(DUMP_MODEMAP_USER_DATA);
        len = (length > usr_data_size) ? usr_data_size : length;

        if (addr != NULL) {
            if (memcpy_s((void *)addr, usr_data_size, (const void *)(uintptr_t)data, (size_t)len) != EOK) {
                dump_debug("err\n");
            }
        }

        if (pfield != NULL) {
            pfield->length = len;
        }
    }
    dump_print("dump save usr data finish\n");
    return;
}

/******************************* 对外提供的dump hook接口,注册为MDMAP SYNC WORK ***************************/
void dump_hook_callback(const dump_exc_log_desc_s *exc_info, void *priv_data)
{
    dump_hook phook = NULL;
    UNUSED(exc_info);

    if (priv_data != NULL) {
        phook = (dump_hook)priv_data;
        phook();
    } else {
        dump_error("hook inv %pK\n", priv_data);
    }
}

dump_handle bsp_dump_register_hook(const char *name, dump_hook func)
{
    dump_work_s work;
    int ret;

    UNUSED(name);

    if (!dump_get_log_strategy_init_state()) {
        dump_log_strategy_init();
    }

    if (!dump_is_subsys_drv_prepare_done()) {
        dump_subsys_drv_prepare_all();
    }

    if (!dump_is_minidump()) {
        return BSP_OK;
    }

    work.work_type = DUMP_WORK_SYNC;
    work.work_fn = dump_hook_callback;
    work.prior = DUMP_WORK_PRIO_SYNC_HOOK_OTHER_MODULE_HIGH;
    work.priv_data = func;
    ret = dump_subsys_add_work(DUMP_SUBSYS_MDMAP, &work);
    if (dump_is_err(ret)) {
        return BSP_ERROR;
    }
    return (dump_handle)ret;
}
EXPORT_SYMBOL_GPL(bsp_dump_register_hook);

int bsp_dump_unregister_hook(dump_handle handle)
{
    int ret;
    if (!dump_is_subsys_drv_prepare_done()) {
        return BSP_ERROR;
    }
    if (handle == 0) {
        return BSP_ERROR;
    }
    ret = dump_subsys_del_work(DUMP_SUBSYS_MDMAP, handle);
    if (dump_is_err(ret)) {
        return BSP_ERROR;
    }
    return BSP_OK;
}
EXPORT_SYMBOL_GPL(bsp_dump_unregister_hook);

int bsp_dump_register_async_callback(enum dump_callback_subsys subsys, u32 prio, dump_hook func)
{
    dump_work_s work;
    int ret;

    static const int dump_subsys_map[DUMP_CALLBACK_BUTT] = { DUMP_SUBSYS_MDMAP, DUMP_SUBSYS_CP, DUMP_SUBSYS_COMM };

    if (subsys >= DUMP_CALLBACK_BUTT || prio > DUMP_MOUDLE_CALLBACK_PRIO_LOWEST) {
        return BSP_ERROR;
    }

    if (!dump_get_log_strategy_init_state()) {
        dump_log_strategy_init();
    }

    if (!dump_is_subsys_drv_prepare_done()) {
        dump_subsys_drv_prepare_all();
    }

    if (!dump_is_minidump()) {
        return BSP_OK;
    }

    work.work_type = DUMP_WORK_ASYNC;
    work.work_fn = dump_hook_callback;
    work.prior = DUMP_WORK_PRIO_ASYNC_HOOK_OTHER_MODULE_HIGH + prio;
    work.priv_data = func;
    ret = dump_subsys_add_work(dump_subsys_map[subsys], &work);
    if (dump_is_err(ret)) {
        return BSP_ERROR;
    }
    return (dump_handle)ret;
}

/******************************* mdmap_dump_works:mdmap dump真正要做的事情 ***************************/
void dump_save_ap_dts_file(const char *dir_name)
{
    unsigned long paddr = 0;
    unsigned int size = 0;
    void *vaddr = NULL;
    paddr = mdrv_mem_region_get("acore_dts_ddr", &size);
    if (paddr == 0 || size == 0) {
        dump_error("acore_dts_ddr no exsit!\n");
        return;
    }
    vaddr = __va(paddr);
    (void)bsp_dump_log_save(dir_name, "ap_dts.bin", vaddr, (const void *)(uintptr_t)paddr, size);
}

void dump_mdmap_done(const dump_exc_log_desc_s *exc_info, void *priv_data)
{
    UNUSED(exc_info);
    UNUSED(priv_data);
    dump_set_save_done(DUMP_SUBSYS_MDMAP);
    dump_print("mdmap_callback finish\n");
}

void dump_mdm_secos_log(const dump_exc_log_desc_s *exc_info, void *priv_data)
{
    UNUSED(exc_info);
    UNUSED(priv_data);
    dump_call_tee_service(DUMP_TEE_CMD_TEE_DUMP_CB);
}

__init void dump_mdmap_log_init(void)
{
    dump_work_s work;

    work.work_type = DUMP_WORK_ASYNC;
    work.work_fn = dump_mdmap_done;
    work.prior = DUMP_WORK_PRIO_ASYNC_HOOK_SET_SUBSYS_DONE;
    work.priv_data = NULL;
    dump_subsys_add_work(DUMP_SUBSYS_MDMAP, &work);

    if (dump_is_minidump()) {
        work.work_fn = dump_mdm_secos_log;
        work.prior = DUMP_WORK_PRIO_ASYNC_HOOK_DUMP_MDM_SECOS;
        dump_subsys_add_work(DUMP_SUBSYS_MDMAP, &work);
        (void)bsp_dump_register_log_notifier(DUMP_SAVE_AP_DTS_DDR, dump_save_ap_dts_file);
        dump_ko_load_info_init();
        dump_lastkmsg_init();
    }
}