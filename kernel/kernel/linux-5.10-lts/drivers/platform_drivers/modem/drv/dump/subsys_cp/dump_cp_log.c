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
#include <linux/delay.h>
#include <osl_types.h>
#include <osl_malloc.h>
#include <mdrv_memory_layout.h>
#include <bsp_dump_mem.h>
#include <bsp_dump.h>
#include <bsp_coresight.h>
#include <bsp_fiq.h>
#include <bsp_slice.h>
#include <bsp_mdm_str.h>
#include <dump_infra.h>
#include <dump_teeos_service.h>
#include <dump_subsys_mgr.h>
#include <dump_logs.h>
#include <dump_log_strategy.h>
#include <dump_frame.h>
#include <dump_area.h>
#include <dump_teeos_service.h>
#include "dump_cp_log.h"

/******************************* cp_dump_works:cp dump真正要做的事情 ***************************/
void dump_save_share_unsec_log(const char *dir_name)
{
    int ret;
    ret = dump_save_ddr_file(dir_name, "share_unsec_ddr", "share_unsec.bin", 0);
    if (ret == BSP_OK) {
        dump_print("save share_unsec.bin finished\n");
    }
}
DUMP_LLT_EXPORT_SYMBOL(dump_save_share_unsec_log);

void dump_save_share_nsro_log(const char *dir_name)
{
    int ret;
    ret = dump_save_ddr_file(dir_name, "share_nsro_ddr", "share_nsro.bin", 0);
    if (ret == BSP_OK) {
        dump_print("save share_nsro.bin finished\n");
    }
}
DUMP_LLT_EXPORT_SYMBOL(dump_save_share_nsro_log);

#define MTEE_TEMP_BUFFER_BLOCK_SIZE (128 * 1024)

void dump_save_mtee_ddr(const char *dir_name)
{
    u8 *dst_buffer = NULL;
    dump_sec_read_mem_packet_s pkt = {
        .sec_mem_id = DUMP_MTEE_DDR_MEM_ID,
        .dst_size = MTEE_TEMP_BUFFER_BLOCK_SIZE,
        .offset_ptr = 0,
    };
    int ret;
    unsigned long addr;
    unsigned int ddr_size = 0;
    char dst_name[MODEM_DUMP_FILE_NAME_LENGTH] = { 0 };
    struct file *fd = NULL;

    if (snprintf_s(dst_name, sizeof(dst_name), sizeof(dst_name) - 1, "%s%s", dir_name, "modem_mtee_ddr.bin") < 0) {
        dump_error("snpf err\n");
        return;
    }

    addr = mdrv_mem_region_get("mdm_tee_ddr", &ddr_size);

    if ((addr == 0) || (ddr_size == 0)) {
        dump_print("no mtee ddr save\n");
        return;
    }

    dst_buffer = kmalloc(MTEE_TEMP_BUFFER_BLOCK_SIZE, GFP_KERNEL);

    if (dst_buffer == NULL) {
        dump_error("no temp buffer\n");
        return;
    }

    pkt.dst_paddr = (u64)(uintptr_t)virt_to_phys(dst_buffer);

    fd = dump_open(dst_name, O_APPEND | O_CREAT | O_RDWR, DUMP_LOG_FILE_AUTH);
    if (fd == NULL) {
        dump_error("fail to open %s,while mode is create\n", dst_name);
        goto out;
    }

    while (pkt.offset_ptr < ddr_size) {
        pkt.read_size = 0;
        ret = dump_call_tee_service_ext(DUMP_TEE_CMD_SECMEM_READ, &pkt, sizeof(pkt));
        if (ret != BSP_OK || pkt.read_size == 0 || pkt.read_size > MTEE_TEMP_BUFFER_BLOCK_SIZE) {
            dump_error("get mtee ddr err\n");
            break;
        }
        ret = dump_write_sync(fd, dst_buffer, (u32)pkt.read_size);
        if (ret != (int)pkt.read_size) {
            dump_error("wsize 0x%x, exp 0x%x\n", ret, pkt.read_size);
            break;
        }
        pkt.offset_ptr += pkt.read_size;
    }

    dump_close(fd);
out:
    kfree(dst_buffer);
}
DUMP_LLT_EXPORT_SYMBOL(dump_save_mtee_ddr);

int dump_get_cp_save_done(void)
{
    int flag;
    struct dump_area_mntn_addr_info_s area_info = { NULL, NULL, 0 };
    struct dump_area_s *dump_area_info = NULL;

    flag = dump_get_area_info(DUMP_AREA_LR, &area_info);
    if (flag == BSP_ERROR || area_info.vaddr == NULL) {
        dump_error("fail to find cp area head\n");
        return BSP_ERROR;
    }
    dump_area_info = ((struct dump_area_s *)(area_info.vaddr));
    flag = dump_area_info->area_head.share_info.done_flag;
    if (flag == DUMP_SAVE_SUCCESS) {
        return BSP_OK;
    }
    return BSP_ERROR;
}

int dump_wait_cp_save_done(u32 ms)
{
    u32 time_start;

    time_start = bsp_get_elapse_ms();

    do {
        if (BSP_OK == dump_get_cp_save_done()) {
            dump_print("mdmcp save done\n");
            return BSP_OK;
        }

        if (ms <= (bsp_get_elapse_ms() - time_start)) {
            dump_error("wait mdmcp done timeout!\n");
            return BSP_ERROR;
        }
        msleep(DUMP_SLEEP_5MS_NUM);
    } while (1);
    return BSP_ERROR;
}

void dump_cp_wait_done(const dump_exc_log_desc_s *exc_info, void *priv_data)
{
    int ret;
    UNUSED(exc_info);
    UNUSED(priv_data);

    dump_print("begin to wait cp save done\n");
    ret = dump_wait_cp_save_done(DUMP_WAIT_15S_NUM);
    if (ret != BSP_ERROR) {
        return;
    }

    /* 发送FIQ */
    ret = bsp_send_cp_fiq(FIQ_DUMP);
    if (ret == BSP_ERROR) {
        dump_error("fail to send cp fiq!\n");
        /* 发FIQ失败，可能是C核，再等3S */
        ret = dump_wait_cp_save_done(DUMP_WAIT_3S_NUM);
    } else {
        dump_print("trig cp fiq process success\n");
        ret = dump_wait_cp_save_done(DUMP_WAIT_15S_NUM);
    }
    
    if (ret == BSP_ERROR) {
        dump_error("ipc fiq save cp log both fail!\n");
    } else {
        dump_print("fiq save cp log success\n");
    }
    return;
}

/* 事务较少，不一一拆分了，节省内存 */
void dump_cp_work_after_done(const dump_exc_log_desc_s *exc_info, void *priv_data)
{
    UNUSED(exc_info);
    UNUSED(priv_data);
    /* 如果当前在STR模式下，就不执行这些回调；现在由于teeos里访问了syncbuffer，所以STR模式下一定不能执行 */
    if (bsp_mdm_str_get_status() == PHASE_ENTER_MDM_STR) {
        return;
    }
    /* 保存modem trace&寄存器，该接口中会判断之前是否保存过，不会重复保存，这里不用再判断了 */
    (void)bsp_coresight_stop_cp();
    (void)dump_call_tee_service(DUMP_TEE_CMD_CP_DUMP);
}

void dump_cp_done(const dump_exc_log_desc_s *exc_info, void *priv_data)
{
    UNUSED(exc_info);
    UNUSED(priv_data);
    dump_set_save_done(DUMP_SUBSYS_CP);
    dump_print("cp_callback finish\n");
}

/******************************* CP狗超时在异常上报前更新baseinfo ***************************/
#define DUMP_TASKINFO_FIELD_STAGE_TASKNAME 0xAAAA0001
#define DUMP_TASKINFO_FIELD_STAGE_TASKTCB 0xAAAA0002
#define DUMP_CP_TASK_STACK_LAYERS 8
typedef struct {
    u32 pid;
    u32 core;
    u32 entry;
    u32 status;
    u32 priority;
    u32 stack_base;
    u32 stack_end;
    char name[DUMP_NAME_LEN];
    u32 dump_stack[DUMP_CP_TASK_STACK_LAYERS];
    u32 regs[0]; /* regs个数由dts中配置 */
} dump_cp_task_info_s;

typedef struct {
    u32 field_stage;
    u16 task_num;
    u16 regs_num;
    u32 regs_mask_l;
    u32 regs_mask_h;
    dump_cp_task_info_s task_tcbs[0];
} dump_cp_taskinfo_field_head_s;

#define DUMP_CP_TASK_NUM_MAX 256
#define DUMP_CP_TASK_REGS_MAX 37
#define DUMP_SINGLE_TCB_SIZE(nreg) (sizeof(dump_cp_task_info_s) + sizeof(u32) * (nreg))
#define DUMP_TCB_NEXT(tcb_addr, nreg) ((dump_cp_task_info_s *)((uintptr_t)(tcb_addr) + DUMP_SINGLE_TCB_SIZE(nreg)))

void dump_get_cp_task_name_by_id(u32 task_id, char *task_name, u32 task_name_len)
{
    dump_cp_taskinfo_field_head_s *task_field = NULL;
    dump_cp_task_info_s *tcb = NULL;
    u32 task_cnt, reg_num, task_index;

    task_field = (dump_cp_taskinfo_field_head_s *)bsp_dump_get_field_addr(DUMP_LRCCPU_TASK_TCB_SMP_NEW);
    if (task_field == NULL) {
        dump_error("fail to get cp task tcb field\n");
        return;
    }

    task_cnt = (u32)task_field->task_num;
    reg_num = (u32)task_field->regs_num;
    if (((task_field->field_stage != DUMP_TASKINFO_FIELD_STAGE_TASKNAME) &&
        (task_field->field_stage != DUMP_TASKINFO_FIELD_STAGE_TASKTCB)) ||
        (task_cnt > DUMP_CP_TASK_NUM_MAX) || (reg_num > DUMP_CP_TASK_REGS_MAX)) {
        dump_error("cp task tcb err\n");
        return;
    }

    tcb = &task_field->task_tcbs[0];

    for (task_index = 0; task_index < task_cnt; task_index++) {
        if (tcb->pid == task_id) {
            if (memcpy_s(task_name, task_name_len, tcb->name, sizeof(tcb->name)) != EOK) {
                dump_debug("err\n");
            }
            break;
        }
        tcb = DUMP_TCB_NEXT(tcb, reg_num);
    }
}

void dump_save_cp_baseinfo_single_core(dump_base_info_s *base_info)
{
    dump_cpu_info_s *cpuinfo = NULL;

    dump_print("modem cp is single core\n");
    base_info->reboot_cpu = 0;

    cpuinfo = (dump_cpu_info_s *)bsp_dump_get_field_addr(DUMP_CP_CPUINFO_START);
    if (cpuinfo == NULL) {
        dump_error("fail to get modem_cp_cpuinfo\n");
        return;
    }

    if ((cpuinfo->current_int & 0xFFFF0000) == DUMP_FLAG_INT_ENTER) {
        cpuinfo->current_ctx = DUMP_CTX_INT;
    } else if ((cpuinfo->current_int & 0xFFFF0000) == DUMP_FLAG_INT_EXIT) {
        cpuinfo->current_ctx = DUMP_CTX_TASK;
    }
    cpuinfo->current_int &= 0xFFFF;

    if (cpuinfo->current_ctx == DUMP_CTX_TASK) {
        base_info->reboot_task = cpuinfo->current_task;
        dump_get_cp_task_name_by_id(base_info->reboot_task, (char *)(base_info->task_name),
            sizeof(base_info->task_name));
        base_info->reboot_int = (u32)(-1);
        base_info->reboot_context = DUMP_CTX_TASK;
    } else {
        base_info->reboot_task = (u32)(-1);
        base_info->reboot_int = cpuinfo->current_int;
        base_info->reboot_context = DUMP_CTX_INT;
    }
}

void dump_save_cp_baseinfo_multi_core(dump_base_info_s *base_info)
{
    dump_cp_reboot_contex_s *reboot_contex = NULL;
    dump_cpu_info_s *cpuinfo = NULL;
    u8 *addr = NULL;
    u32 i;

    base_info->reboot_cpu = 0xff;
    addr = (u8 *)bsp_dump_get_field_addr(DUMP_LRCCPU_REBOOTCONTEX);
    if (addr == NULL) {
        dump_error("fail to get cp reboot field\n");
        return;
    }

    for (i = 0; i < base_info->cpu_max_num; i++) {
        cpuinfo = (dump_cpu_info_s *)bsp_dump_get_field_addr(DUMP_CP_CPUINFO_START + i);
        if (cpuinfo == NULL) {
            dump_error("fail to get modem_cp_cpuinfo field\n");
            return;
        }
        if ((cpuinfo->current_int & 0xFFFF0000) == DUMP_FLAG_INT_ENTER) {
            cpuinfo->current_ctx = DUMP_CTX_INT;
        } else if ((cpuinfo->current_int & 0xFFFF0000) == DUMP_FLAG_INT_EXIT) {
            cpuinfo->current_ctx = DUMP_CTX_TASK;
        }
        cpuinfo->current_int &= 0xFFFF;
        reboot_contex = (dump_cp_reboot_contex_s *)((uintptr_t)(addr) + i * sizeof(dump_cp_reboot_contex_s));
        if (cpuinfo->current_ctx == DUMP_CTX_TASK) {
            reboot_contex->reboot_context = DUMP_CTX_TASK;
            reboot_contex->reboot_task = cpuinfo->current_task;
            dump_get_cp_task_name_by_id(reboot_contex->reboot_task, (char *)(reboot_contex->task_name),
                sizeof(base_info->task_name));
            reboot_contex->reboot_int = (u32)(-1);
        } else {
            reboot_contex->reboot_task = (u32)(-1);
            reboot_contex->reboot_int = cpuinfo->current_int;
            reboot_contex->reboot_context = DUMP_CTX_INT;
        }
        reboot_contex->task_name[DUMP_NAME_LEN - 1] = '\0';
    }
}

dump_base_info_s *dump_save_cp_base_info_wdt(u32 mod_id)
{
    dump_base_info_s *base_info = NULL;
    u8 *addr = NULL;

    addr = bsp_dump_get_field_addr(DUMP_LRCCPU_BASE_INFO_SMP);
    if (addr == NULL) {
        dump_error("fail to get cp base info\n");
        return NULL;
    }
    base_info = (dump_base_info_s *)addr;
    if (base_info->mod_id == DRV_ERRNO_EARLYPM_EXC) {
        dump_error("earlypm exc, baseinfo is ok\n");
        return base_info;
    }
    base_info->mod_id = mod_id;
    base_info->arg1 = DUMP_REASON_WDT;
    base_info->arg2 = 0;
    base_info->arg3 = 0;
    base_info->arg3_length = 0;
    base_info->reboot_reason = DUMP_REASON_WDT;
    base_info->reboot_time = bsp_get_slice_value();

    if (base_info->cpu_max_num == 1) {
        dump_save_cp_baseinfo_single_core(base_info);
    } else {
        dump_save_cp_baseinfo_multi_core(base_info);
    }
    dump_print("save cp base info ok\n");
    return base_info;
}
DUMP_LLT_EXPORT_SYMBOL(dump_save_cp_base_info_wdt);

__init void dump_cp_log_init(void)
{
    dump_work_s work;

    work.work_type = DUMP_WORK_ASYNC;
    work.work_fn = dump_cp_done;
    work.prior = DUMP_WORK_PRIO_ASYNC_HOOK_SET_SUBSYS_DONE;
    work.priv_data = NULL;
    dump_subsys_add_work(DUMP_SUBSYS_CP, &work);

    if (!dump_is_minidump()) {
        return;
    }

    work.work_fn = dump_cp_wait_done;
    work.prior = DUMP_WORK_PRIO_ASYNC_HOOK_WAIT_DONE;
    dump_subsys_add_work(DUMP_SUBSYS_CP, &work);

    work.work_fn = dump_cp_work_after_done;
    work.prior = DUMP_WORK_PRIO_ASYNC_HOOK_PLAT_DUMP_AFTER_DONE;
    dump_subsys_add_work(DUMP_SUBSYS_CP, &work);

    bsp_dump_register_log_notifier(DUMP_SAVE_SHARE_UNSEC, dump_save_share_unsec_log);
    bsp_dump_register_log_notifier(DUMP_SAVE_SHARE_NSRO, dump_save_share_nsro_log);
    bsp_dump_register_log_notifier(DUMP_SAVE_MTEE_DDR, dump_save_mtee_ddr);
}
