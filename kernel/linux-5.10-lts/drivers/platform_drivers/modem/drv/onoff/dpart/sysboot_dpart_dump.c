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
#include <linux/sched.h>
#include <linux/cache.h>
#include <uapi/linux/sched/types.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/timekeeping.h>
#include <linux/rtc.h>
#include <bsp_dt.h>
#include <bsp_slice.h>
#include <bsp_sysboot.h>
#include <bsp_onoff.h>
#include <bsp_dump_mem.h>
#include <securec.h>
#include <bsp_print.h>
#include <bsp_file_ops.h>
#include <bsp_dump.h>
#include <osl_sem.h>
#include "../bsp_modem_boot.h"
#include "sysboot_dpart_dump.h"
#include "sysboot_dpart_policy.h"


#ifdef CONFIG_MODEM_ONOFF_PARA
#define THIS_MODU mod_onoff

#define SYSBOOT_DUMP_START_YEAD 1900
#define SYSBOOT_DUMP_DATA_BUFFER_SIZE_24 24
#define SYSBOOT_DUMP_SCHED_PRIORITY 25
#define SYSBOOT_DUMP_HISTORY_LOG_MAXSIZE (8 * 1024)
#define SYSBOOT_DUMP_LOG_PATH_LEN_128 128
#define SYSBOOT_DUMP_CURR_LOG_FILE_NAME "history.log"
#define SYSBOOT_DUMP_OLD_LOG_FILE_NAME "history_old.log"
#define SYSBOOT_DUMP_DEFAULT_LOG_DIR_PATH "/data/dpart_logs/"
#define SYSBOOT_DUMP_DEFAULT_CURR_LOG_PATH "/data/dpart_logs/history.log"
#define SYSBOOT_DUMP_DEFAULT_OLD_LOG_PATH "/data/dpart_logs/history_old.log"

typedef struct {
    char dump_log_dir[SYSBOOT_DUMP_LOG_PATH_LEN_128];
    char curr_log_path[SYSBOOT_DUMP_LOG_PATH_LEN_128];
    char old_log_path[SYSBOOT_DUMP_LOG_PATH_LEN_128];
} sysboot_dpart_dump_cust_cfg;

__ro_after_init sysboot_dpart_dump_cust_cfg g_sysboot_dpart_dump_cust_cfg = {
    .dump_log_dir = SYSBOOT_DUMP_DEFAULT_LOG_DIR_PATH,
    .curr_log_path = SYSBOOT_DUMP_DEFAULT_CURR_LOG_PATH,
    .old_log_path = SYSBOOT_DUMP_DEFAULT_OLD_LOG_PATH,
};

const char *g_sysboot_dpart_exception_type_string[SYSBOOT_MAX_EXCEPTION] = {
    "none",
    "boot",
    "runtime",
};

const char *g_sysboot_dpart_exception_stage_string[SYSBOOT_MAX_STAGE_EXCEPTION] = {
    "none",
    "xloader",
    "hiboot",
    "ap",
    "lpmcu",
    "modem",
};

u32 g_sysboot_dump_main_idx2oeminfo_dump_data_id[DPART_DUMP_MAIN_IDX_MAX] = {
    OEMINFO_PRIVATE_DPART_DUMP_DATA0,
    OEMINFO_PRIVATE_DPART_DUMP_DATA1,
    OEMINFO_PRIVATE_DPART_DUMP_DATA2,
};

struct sysboot_dpart_dump_info g_sysboot_dpart_dump_info = {0};

static void sysboot_dump_boot_data(struct sysboot_dpart_dump_info_boot_data *boot_data, u32 exception_type, u32 exception_stage)
{
    unsigned int ap_state = bsp_sysboot_get_ap_state();
    unsigned int modem_state = mdrv_sysboot_is_modem_ready();
    struct sysboot_dpart_info *dpart_info = sysboot_get_dpart_info();

    if (ap_state == 1 && modem_state == 1) {
        boot_data->sysboot_state = SYSBOOT_BOOT_SUCCESS;
    } else {
        boot_data->sysboot_state = SYSBOOT_BOOT_FAILED;
    }
    boot_data->exception_type = exception_type;
    boot_data->exception_stage = exception_stage;
    boot_data->total_boot_times = dpart_info->runtime_info.oeminfo_total_boot_times + dpart_info->runtime_info.reg_info.member.curr_boot_times;
    boot_data->dpart_enable = dpart_info->runtime_info.dpart_enable;
    boot_data->dpart_policy = dpart_info->runtime_info.dpart_policy;
    boot_data->boot_retry_times = dpart_info->runtime_info.boot_retry_times;
    boot_data->curr_boot_retry_times = dpart_info->runtime_info.reg_info.member.curr_boot_retry_times;
    boot_data->boot_part_switched = dpart_info->runtime_info.boot_part_switched;
    boot_data->default_boot_part = dpart_info->runtime_info.default_boot_part;
    boot_data->escape_state = dpart_info->runtime_info.reg_info.member.escape_state;
    boot_data->image_load_failed = dpart_info->runtime_info.image_load_failed;
    boot_data->image_resotre_state1 = 0;
    boot_data->image_resotre_state2 = 0;
}

static void sysboot_dump_update_data(struct sysboot_dpart_dump_info_update_data *update_data)
{
    return;
}

static void update_dump_head_info(struct sysboot_dpart_dump_info_head *head)
{
    head->dpart_dump_sub_idx++;
    if (head->dpart_dump_sub_idx >= DPART_DUMP_SUB_IDX_MAX) {
        head->dpart_dump_sub_idx = 0;
        head->dpart_dump_main_idx++;
        if (head->dpart_dump_main_idx >= DPART_DUMP_MAIN_IDX_MAX) {
            head->dpart_dump_main_idx = 0;
        }
    }
    head->dpart_dump_cnt++;
}

void sysboot_dpart_dump_oeminfo_data(struct sysboot_dpart_dump_info_boot_data *boot_data, struct sysboot_dpart_dump_info_update_data *update_data)
{
    int ret;
    struct sysboot_dpart_dump_info_head *head = &g_sysboot_dpart_dump_info.head;
    struct sysboot_dpart_dump_info_data *data = &g_sysboot_dpart_dump_info.data[head->dpart_dump_main_idx];
    struct sysboot_dpart_dump_info_data_node *data_node = &data->node[head->dpart_dump_sub_idx];
    u32 oeminfo_data_id = g_sysboot_dump_main_idx2oeminfo_dump_data_id[head->dpart_dump_main_idx];

    data_node->boot_data = *boot_data;
    data_node->update_data = *update_data;
    update_dump_head_info(head);

    data_node->dpart_dump_data_magic = DPART_DUMP_DATA_MAGIC;
    ret = bsp_oeminfo_private_rgn_write_sync(oeminfo_data_id, data, sizeof(struct sysboot_dpart_dump_info_data));
    if (ret) {
        bsp_err("daprt dump:write dump data for id%d failed, ret=0x%x\n", oeminfo_data_id, ret);
        return;
    }
    ret = bsp_oeminfo_private_rgn_write_sync(OEMINFO_PRIVATE_DPART_DUMP_HEAD, head, sizeof(struct sysboot_dpart_dump_info_head));
    if (ret) {
        bsp_err("daprt dump:write dump head failed, ret=0x%x\n", ret);
    }
    return;
}

void sysboot_dpart_dump_init(void)
{
    (void)bsp_oeminfo_private_rgn_read(OEMINFO_PRIVATE_DPART_DUMP_HEAD, &g_sysboot_dpart_dump_info.head, sizeof(struct sysboot_dpart_dump_info_head));
    if (g_sysboot_dpart_dump_info.head.dpart_dump_head_magic != DPART_DUMP_HEAD_MAGIC) {
        g_sysboot_dpart_dump_info.head.dpart_dump_head_magic = DPART_DUMP_HEAD_MAGIC;
        g_sysboot_dpart_dump_info.head.dpart_dump_cnt = 0;
        g_sysboot_dpart_dump_info.head.dpart_dump_main_idx = 0;
        g_sysboot_dpart_dump_info.head.dpart_dump_sub_idx = 0;
        return;
    }

    if (g_sysboot_dpart_dump_info.head.dpart_dump_main_idx >= DPART_DUMP_MAIN_IDX_MAX) {
        bsp_err("[sysboot] dpart_dump_main_idx(0x%x) is err\n", g_sysboot_dpart_dump_info.head.dpart_dump_main_idx);
        g_sysboot_dpart_dump_info.head.dpart_dump_main_idx = 0;
    }

    if (g_sysboot_dpart_dump_info.head.dpart_dump_sub_idx >= DPART_DUMP_SUB_IDX_MAX) {
        bsp_err("[sysboot] dpart_dump_sub_idx(0x%x) is err\n", g_sysboot_dpart_dump_info.head.dpart_dump_sub_idx);
        g_sysboot_dpart_dump_info.head.dpart_dump_sub_idx = 0;
    }

    if (g_sysboot_dpart_dump_info.head.dpart_dump_cnt >= DPART_DUMP_CNT_MAX) {
        bsp_err("[sysboot] dpart_dump_cnt(0x%x) is err\n", g_sysboot_dpart_dump_info.head.dpart_dump_cnt);
        g_sysboot_dpart_dump_info.head.dpart_dump_cnt = 0;
    }

    (void)bsp_oeminfo_private_rgn_read(OEMINFO_PRIVATE_DPART_DUMP_DATA0, &g_sysboot_dpart_dump_info.data[0],
            sizeof(struct sysboot_dpart_dump_info_data));
    (void)bsp_oeminfo_private_rgn_read(OEMINFO_PRIVATE_DPART_DUMP_DATA1, &g_sysboot_dpart_dump_info.data[1],
            sizeof(struct sysboot_dpart_dump_info_data));
    (void)bsp_oeminfo_private_rgn_read(OEMINFO_PRIVATE_DPART_DUMP_DATA2, &g_sysboot_dpart_dump_info.data[2],
            sizeof(struct sysboot_dpart_dump_info_data));

    return;
}

void sysboot_dpart_dump_to_oeminfo(struct sysboot_dpart_dump_info_boot_data *boot_data, struct sysboot_dpart_dump_info_update_data *update_data)
{
    sysboot_dpart_dump_init();
    sysboot_dpart_dump_oeminfo_data(boot_data, update_data);
}

static int sysboot_get_failed_part_name(unsigned int image_load_failed)
{
    unsigned int i, ret;
    int is_back_part = bsp_sysboot_boot_is_back();

    (void)memset_s(g_sysboot_dpart_dump_info.part_name_arr, sizeof(g_sysboot_dpart_dump_info.part_name_arr),
            0, sizeof(g_sysboot_dpart_dump_info.part_name_arr));

    bsp_err("image_load_failed:0x%x\n", image_load_failed);
    for (i = 0; i < BOOT_IMG_ID_MAX; i++) {
        if (image_load_failed & 0x1U) {
            ret = (unsigned int)strcat_s(g_sysboot_dpart_dump_info.part_name_arr,
                    sizeof(g_sysboot_dpart_dump_info.part_name_arr), sysboot_dpart_get_part_name_by_img_type(i));
            if (is_back_part) {
                ret |= (unsigned int)strcat_s(g_sysboot_dpart_dump_info.part_name_arr, sizeof(g_sysboot_dpart_dump_info.part_name_arr), "bk");
            }
            ret |= (unsigned int)strcat_s(g_sysboot_dpart_dump_info.part_name_arr, sizeof(g_sysboot_dpart_dump_info.part_name_arr), " ");
            if (ret) {
                bsp_err("get part_name %s failed, ret = 0x%x\n", sysboot_dpart_get_part_name_by_img_type(i), ret);
                return -1;
            }
        }

        image_load_failed = image_load_failed >> 1;
    }
    return 0;
}

static int sysboot_get_img_restore_string(u32 img_restore_state1, u32 img_restore_state2)
{
    unsigned int ret, i;
    u64 img_restore_state = img_restore_state2;
    img_restore_state = (img_restore_state << 32U) | img_restore_state1;

    (void)memset_s(g_sysboot_dpart_dump_info.img_restore_arr, sizeof(g_sysboot_dpart_dump_info.img_restore_arr),
            0, sizeof(g_sysboot_dpart_dump_info.img_restore_arr));
    for (i = 0; i < SYSBOOT_IMG_NUMS_MAX; i++) {
        if (img_restore_state & 0x3U) {
            ret = (unsigned int)strcat_s(g_sysboot_dpart_dump_info.img_restore_arr, sizeof(g_sysboot_dpart_dump_info.img_restore_arr), "SUCC ");
            if (ret) {
                bsp_err("get img restore string failed, ret = 0x%x\n", ret);
                return -1;
            }
        }

        img_restore_state = img_restore_state >> 2U;
    }

    return 0;
}

static void sysboot_dump_get_data_timestamp(char *date, u32 len)
{
    time64_t txc = 0;
    struct rtc_time tm;
    /* fill date&tick */
    if (len == 0)
        return;
    txc = ktime_get_real_seconds();
    rtc_time64_to_tm(txc, &tm);
    if (snprintf_s(date, len, len - 1, "%4d%02d%02d%02d%02d%02d-%08x", tm.tm_year + SYSBOOT_DUMP_START_YEAD, tm.tm_mon + 1,
        tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, bsp_get_slice_value()) < 0) {
        bsp_err("sysboot dump get data timestamp failed\n");
    }
    return;
}

static int sysboot_dump_gen_history_log(struct sysboot_dpart_dump_info_boot_data *boot_data, struct sysboot_dpart_dump_info_update_data *update_data)
{
    int ret;
    char *sysboot_state;
    char *dpart_enable;
    char *dpart_policy;
    char *boot_part_switched;
    char *default_boot_part;
    char *enter_escape;
    char *load_failed_img;
    char *restore_img;
    const char *exception_type;
    const char *exception_stage;
    char date[SYSBOOT_DUMP_DATA_BUFFER_SIZE_24];

    (void)memset_s(date, sizeof(date), 0, sizeof(date));
    (void)memset_s(g_sysboot_dpart_dump_info.dump_history, sizeof(g_sysboot_dpart_dump_info.dump_history),
            0, sizeof(g_sysboot_dpart_dump_info.dump_history));
    sysboot_dump_get_data_timestamp(date, sizeof(date));
    ret = sysboot_get_failed_part_name(boot_data->image_load_failed);
    if (ret) {
        return ret;
    }
    ret = sysboot_get_img_restore_string(boot_data->image_resotre_state1, boot_data->image_resotre_state2);
    if (ret) {
        return ret;
    }

    sysboot_state = (boot_data->sysboot_state == SYSBOOT_BOOT_SUCCESS) ? "SUCCESS" : "FAILED";
    exception_type = g_sysboot_dpart_exception_type_string[boot_data->exception_type];
    exception_stage = g_sysboot_dpart_exception_stage_string[boot_data->exception_stage];
    dpart_enable = (boot_data->dpart_enable == 1) ? "YES" : "NO";
    dpart_policy = (boot_data->dpart_policy == AA_BOOT_POLICY) ? "AA" : "AB";
    boot_part_switched = (boot_data->boot_part_switched == 1) ? "YES" : "NO";
    default_boot_part = (boot_data->default_boot_part == BOOT_FROM_A_PART_FLAG) ? "MAIN" : "BACK";
    enter_escape = (boot_data->escape_state == 1) ? "YES" : "NO";
    load_failed_img = g_sysboot_dpart_dump_info.part_name_arr;
    restore_img = g_sysboot_dpart_dump_info.img_restore_arr;

    ret = snprintf_s(g_sysboot_dpart_dump_info.dump_history, sizeof(g_sysboot_dpart_dump_info.dump_history),
        sizeof(g_sysboot_dpart_dump_info.dump_history) - 1,
        "[%s]:sysboot_state[%s],exception_type[%s],exception_stage[%s],total_boot_times[%d],dpart_enable[%s],dpart_policy[%s],boot_retry_times[%d],\
        curr_boot_retry_times[%d],boot_part_switched[%s],default_boot_part[%s],enter_escape[%s],load_failed_img[%s],restore_img[%s]\n",\
        date, sysboot_state, exception_type, exception_stage, boot_data->total_boot_times, dpart_enable, dpart_policy,\
        boot_data->boot_retry_times, boot_data->curr_boot_retry_times, boot_part_switched, default_boot_part,\
        enter_escape, load_failed_img, restore_img);
    if (ret < 0) {
        bsp_err("gen history log failed, ret = 0x%x\n", ret);
        return ret;
    }

    return 0;
}

static int sysboot_save_to_old_log(void)
{
    int ret;
    int file_size;
    loff_t from = 0;
    void *data_buf = NULL;
    struct file *old_fp = NULL;
    struct file *curr_fp = NULL;

    ret = bsp_eaccess(g_sysboot_dpart_dump_cust_cfg.old_log_path);
    if (ret == 0) { // old log存在，删除
        (void)bsp_unlink(g_sysboot_dpart_dump_cust_cfg.old_log_path);
    }
    old_fp = filp_open(g_sysboot_dpart_dump_cust_cfg.old_log_path, O_RDWR | O_CREAT, 0660);
    if (IS_ERR_OR_NULL(old_fp)) {
        bsp_err("fail to open file %s!\n", g_sysboot_dpart_dump_cust_cfg.old_log_path);
        return -1;
    }

    curr_fp = filp_open(g_sysboot_dpart_dump_cust_cfg.curr_log_path, O_RDWR | O_CREAT, 0660);
    if (IS_ERR_OR_NULL(curr_fp)) {
        bsp_err("fail to open file %s!\n", g_sysboot_dpart_dump_cust_cfg.curr_log_path);
        filp_close(old_fp, NULL);
        return -1;
    }
    file_size = i_size_read(file_inode(curr_fp));

    data_buf = kmalloc(file_size, GFP_KERNEL);
    if (data_buf == NULL) {
        bsp_err("malloc mem failed\n");
        ret = -1;
        goto out;
    }
    ret = kernel_read(curr_fp, data_buf, file_size, &from);
    if (ret != file_size) {
        bsp_err("read dpart log err,data_len %d,read_len %d!\n", file_size, ret);
        ret = -1;
        goto out;
    }

    from = 0;
    ret = kernel_write(old_fp, data_buf, file_size,  &from);
    if (ret != file_size) {
        bsp_err("write dpart log err,data_len %d,write_len %d!\n", file_size, ret);
        ret = -1;
        goto out;
    }
    ret = 0;
    kfree(data_buf);
out:
    filp_close(curr_fp, NULL);
    filp_close(old_fp, NULL);
    return ret;
}

static void sysboot_dump_save_file(void)
{
    int file_size, ret;
    loff_t offset;
    int mode = 0660;
    struct file *fp  = NULL;

    ret = bsp_eaccess(g_sysboot_dpart_dump_cust_cfg.dump_log_dir);
    if (ret) { // log路径不存在
        ret = bsp_file_mkdir(g_sysboot_dpart_dump_cust_cfg.dump_log_dir, mode);
        if (ret) {
            bsp_err("mkdir %s fail,ret=%d\n", g_sysboot_dpart_dump_cust_cfg.dump_log_dir, ret);
            return;
        }
    }

    fp = filp_open(g_sysboot_dpart_dump_cust_cfg.curr_log_path, O_RDWR | O_CREAT, mode);
    if (IS_ERR_OR_NULL(fp)) {
        bsp_err("fail to open file %s!\n", g_sysboot_dpart_dump_cust_cfg.curr_log_path);
        return;
    }
    file_size = i_size_read(file_inode(fp));

    if (file_size + strlen(g_sysboot_dpart_dump_info.dump_history) > SYSBOOT_DUMP_HISTORY_LOG_MAXSIZE) {
        bsp_err("curr log len=%d,data_len=%d,max_len=%d\n", file_size, 
                strlen(g_sysboot_dpart_dump_info.dump_history), SYSBOOT_DUMP_HISTORY_LOG_MAXSIZE);
        filp_close(fp, NULL);
        ret = sysboot_save_to_old_log();
        if (ret) {
            return;
        }

        fp = filp_open(g_sysboot_dpart_dump_cust_cfg.curr_log_path, O_RDWR | O_CREAT | O_TRUNC, 0660);
        if (IS_ERR_OR_NULL(fp)) {
            bsp_err("fail to open file %s!\n", g_sysboot_dpart_dump_cust_cfg.curr_log_path);
            return;
        }
        file_size = 0;
    }

    ret = vfs_llseek(fp, 0, SEEK_END);
    if (ret < 0) {
        bsp_err("seek end file fail, ret=0x%x\n", ret);
        goto out;
    }

    offset = file_size;
    ret = (int)kernel_write(fp, g_sysboot_dpart_dump_info.dump_history, strlen(g_sysboot_dpart_dump_info.dump_history), (loff_t *)&offset);
    if ((unsigned int)ret != strlen(g_sysboot_dpart_dump_info.dump_history)) {
        bsp_err("fail to write file:%s, ret=0x%x\n", g_sysboot_dpart_dump_info.dump_history, ret);
        goto out;
    }
    (void)vfs_fsync(fp, 0);

out:
    filp_close(fp, NULL);
    return;
}

static void sysboot_dpart_dump_to_data_part(struct sysboot_dpart_dump_info_boot_data *boot_data, 
                                        struct sysboot_dpart_dump_info_update_data *update_data)
{
    int ret;
    ret = sysboot_dump_gen_history_log(boot_data, update_data);
    if (ret) {
        return;
    }

    return sysboot_dump_save_file();
}

static void sysboot_dpart_dump(u32 exception_type, u32 exception_stage)
{
    struct sysboot_dpart_dump_info_boot_data boot_data = {0};
    struct sysboot_dpart_dump_info_update_data update_data = {0};

    if (g_sysboot_dpart_dump_info.inited == 0) {
        bsp_err("sysboot dpart dump not init\n");
        return;
    }

    osl_sem_down(&g_sysboot_dpart_dump_info.sysboot_dump_sem);
    sysboot_dump_boot_data(&boot_data, exception_type, exception_stage);
    sysboot_dump_update_data(&update_data);

    sysboot_dpart_dump_to_data_part(&boot_data, &update_data);
    if (exception_type != SYSBOOT_NONE_EXCEPTION) {
        sysboot_dpart_dump_to_oeminfo(&boot_data, &update_data);
    }
    osl_sem_up(&g_sysboot_dpart_dump_info.sysboot_dump_sem);
    return;
}

// avb校验失败时回调用
void bsp_sysboot_dpart_sync_dump(void)
{
    struct sysboot_dpart_info *dpart_info = sysboot_get_dpart_info();
    unsigned int ap_state = bsp_sysboot_get_ap_state();
    unsigned int modem_state = mdrv_sysboot_is_modem_ready();
    unsigned int exception_type = SYSBOOT_BOOT_EXCEPTION;

    dpart_info->runtime_info.image_load_failed |= (0x1 << BOOT_AVB_PART);

    if (modem_state != 1) {
        dpart_info->runtime_info.image_load_failed |= (0x1 << BOOT_IMG_MODEMFW);
    }

    if (ap_state == 1 && modem_state == 1) {
        exception_type = SYSBOOT_RUNTIME_EXCEPTION;
    }

    sysboot_dpart_dump(exception_type, SYSBOOT_AP_EXCEPTION);
}
EXPORT_SYMBOL(bsp_sysboot_dpart_sync_dump);

// 系统启动成功时调用
void bsp_sysboot_dpart_async_dump(void)
{
    osl_sem_up(&g_sysboot_dpart_dump_info.task_trigger_sem);
}
EXPORT_SYMBOL(bsp_sysboot_dpart_async_dump);

void sysboot_dpart_dump_hook(void)
{
    struct sysboot_dpart_info *dpart_info = sysboot_get_dpart_info();
    const struct dump_global_base_info_s *exc_info = bsp_dump_get_cur_exc_baseinfo();
    unsigned int ap_state = bsp_sysboot_get_ap_state();
    unsigned int modem_state = mdrv_sysboot_is_modem_ready();
    unsigned int exception_type = SYSBOOT_BOOT_EXCEPTION;

    if (exc_info == NULL) {
        return;
    }

    if (modem_state != 1) {
        dpart_info->runtime_info.image_load_failed |= (0x1 << BOOT_IMG_MODEMFW);
    }

    if (ap_state == 1 && modem_state == 1) {
        exception_type = SYSBOOT_RUNTIME_EXCEPTION;
    }

    if (exc_info->e_core == (u32)DUMP_EXC_CORE_CP) {
        sysboot_dpart_dump(exception_type, SYSBOOT_MODEM_EXCEPTION);
    } else if (exc_info->e_core == (u32)DUMP_EXC_CORE_AP || exc_info->e_core == (u32)DUMP_EXC_CORE_MODEMAP) {
        sysboot_dpart_dump(exception_type, SYSBOOT_AP_EXCEPTION);
    } else if (exc_info->e_core == (u32)DUMP_EXC_CORE_LPMCU) {
        sysboot_dpart_dump(exception_type, SYSBOOT_LPMCU_EXCEPTION);
    }
}

int sysboot_dump_task(void *data)
{
    osl_sem_down(&g_sysboot_dpart_dump_info.task_trigger_sem);
    sysboot_dpart_dump(SYSBOOT_NONE_EXCEPTION, SYSBOOT_NONE_STAGE_EXCEPTION);
    return 0;
}

int sysboot_dump_tsk_init(void)
{
    int ret;
    struct sched_param sch_para = {0};
    osl_sem_init(0, &g_sysboot_dpart_dump_info.task_trigger_sem);

    sch_para.sched_priority = SYSBOOT_DUMP_SCHED_PRIORITY;

    g_sysboot_dpart_dump_info.taskid = kthread_run(sysboot_dump_task, NULL, "sysboot_dump_task");
    if (IS_ERR_OR_NULL(g_sysboot_dpart_dump_info.taskid)) {
        bsp_err("fail to create sysboot dump kthread, ret = %ld\n", PTR_ERR(g_sysboot_dpart_dump_info.taskid));
        return -ECHILD;
    }
    ret = sched_setscheduler(g_sysboot_dpart_dump_info.taskid, SCHED_FIFO, &sch_para);
    if (ret) {
        bsp_err("sched_setscheduler fail\n");
    }

    return ret;
}

static void sysboot_dpart_dump_cust_dir_init(const char *path)
{
    unsigned int len;
    if (path == NULL) {
        return;
    }

    len = strlen(path);
    if (len == 0 || len >= SYSBOOT_DUMP_LOG_PATH_LEN_128) {
        bsp_err("cust sysboot_dpart_dump_dir len err %d!\n", len);
        return;
    }
    /* 目录需要以'/'结尾 */
    if (path[len - 1] != '/') {
        bsp_err("cust sysboot_dpart_dump_dir format err\n");
        return;
    }

    if (strcpy_s(g_sysboot_dpart_dump_cust_cfg.dump_log_dir, SYSBOOT_DUMP_LOG_PATH_LEN_128, path) != EOK) {
        bsp_err("cpy cust exc_dir fail\n");
        goto err_resume_sysboot_dump_cust_dir;
    }

    if (sprintf_s(g_sysboot_dpart_dump_cust_cfg.curr_log_path, SYSBOOT_DUMP_LOG_PATH_LEN_128, "%s%s", path,
        SYSBOOT_DUMP_CURR_LOG_FILE_NAME) == -1) {
        bsp_err("gen cust history log fail\n");
        goto err_resume_sysboot_dump_history_log;
    }

    if (sprintf_s(g_sysboot_dpart_dump_cust_cfg.old_log_path, SYSBOOT_DUMP_LOG_PATH_LEN_128, "%s%s", path,
        SYSBOOT_DUMP_OLD_LOG_FILE_NAME) == -1) {
        bsp_err("gen cust old history log fail\n");
        goto err_resume_sysboot_dump_old_history_log;
    }

    return;

err_resume_sysboot_dump_old_history_log:
    if (strcpy_s(g_sysboot_dpart_dump_cust_cfg.old_log_path, SYSBOOT_DUMP_LOG_PATH_LEN_128,
        SYSBOOT_DUMP_DEFAULT_OLD_LOG_PATH) != EOK) {
        bsp_err("sysboot dump fatal err, resume old history log path fail\n");
    }

err_resume_sysboot_dump_history_log:
    if (strcpy_s(g_sysboot_dpart_dump_cust_cfg.curr_log_path, SYSBOOT_DUMP_LOG_PATH_LEN_128,
        SYSBOOT_DUMP_DEFAULT_CURR_LOG_PATH) != EOK) {
        bsp_err("sysboot dump fatal err, resume history log path fail\n");
    }
err_resume_sysboot_dump_cust_dir:
    if (strcpy_s(g_sysboot_dpart_dump_cust_cfg.dump_log_dir, SYSBOOT_DUMP_LOG_PATH_LEN_128,
        SYSBOOT_DUMP_DEFAULT_LOG_DIR_PATH) != EOK) {
        bsp_err("sysboot dump fatal err, resume log dir fail\n");
    }
}

int bsp_sysboot_dump_init(void)
{
    int ret;
    const char *path = NULL;
    struct device_node *dev = NULL;

    osl_sem_init(1, &g_sysboot_dpart_dump_info.sysboot_dump_sem);
    ret = sysboot_dump_tsk_init();
    if (ret) {
        return ret;
    }

    ret = bsp_dump_register_async_callback(DUMP_MDMAP_CALLBACK, 1, sysboot_dpart_dump_hook);
    if (ret == BSP_ERROR) {
        bsp_err("register sysboot dump callback failed\n");
    }

    dev = bsp_dt_find_node_by_path("/sysboot_cfg_cust");
    if (dev == NULL) {
        bsp_err("no cust /sysboot_cfg_cust node.\n");
        goto out;
    }

    ret = bsp_dt_property_read_string(dev, "sysboot_dpart_dir_path", &path);
    if (ret) {
        bsp_err("no cust sysboot_dpart_dir_path!\n");
    } else {
        sysboot_dpart_dump_cust_dir_init(path);
    }
out:
    g_sysboot_dpart_dump_info.inited = 1;
    return ret;
}

#if !IS_ENABLED(CONFIG_MODEM_DRIVER)
module_init(bsp_sysboot_dump_init);
#endif

#else //CONFIG_MODEM_ONOFF_PARA

// avb校验失败时回调用
void bsp_sysboot_dpart_sync_dump(void)
{
    return;
}

// 系统启动成功时调用
void bsp_sysboot_dpart_async_dump(void)
{
    return;
}

#endif