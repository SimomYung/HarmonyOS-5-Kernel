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
#include <linux/delay.h>
#include <linux/export.h>
#include <linux/ktime.h>
#include <linux/rtc.h>
#include <of.h>
#include <securec.h>
#include <osl_malloc.h>
#include <mdrv_bmi_om.h>
#include <bsp_slice.h>
#include <bsp_dump.h>
#include <bsp_file_ops.h>
#include <dump_infra.h>
#include <dump_rdr_wrapper.h>
#include "dump_log_strategy.h"
#include "dump_logs.h"
#include "dump_extra_logs.h"

/* 仅IOT eng版本启用，其余版本不启用，限制eng主要是为了安全考虑 */

#define DUMP_EXTRA_FS_TIMEOUT_MAX 50 /* 1次是0.1s, 50表示最多5s */
#define DUMP_EXTRA_LOG_LINK_FILE "base_path.log"
typedef struct {
    u32 feature_on;
    u32 extra_file_num;
    char extra_log_dir[RDR_DUMP_FILE_PATH_LEN];
} dump_extra_logs_cfg_s;

enum dump_extra_log_path_state {
    DUMP_PATH_STATE_INVALID = 0,
    DUMP_PATH_STATE_USE_RDR_PATH = 1,
    DUMP_PATH_STATE_USE_EXTRA_PATH = 2,
    DUMP_PATH_STATE_BUTT,
};

typedef struct {
    dump_fs_state_func fs_state_check;
    enum dump_extra_log_path_state path_state;
    char log_path[RDR_DUMP_FILE_PATH_LEN];
} dump_extra_logs_ctrl_s;

__ro_after_init dump_extra_logs_cfg_s g_dump_extra_logs_cfg;
dump_extra_logs_ctrl_s g_dump_extra_ctrl;

unsigned int mdrv_om_set_extra_log_cfg(dump_extra_log_cfg_s *cfg)
{
    if (cfg == NULL || cfg->fs_state_cb == NULL) {
        return -1;
    }

    g_dump_extra_ctrl.fs_state_check = cfg->fs_state_cb;
    return 0;
}

const char *dump_get_extra_logs_log_path(const char *rdr_log_path)
{
    const char *ret_path = NULL;
    switch (g_dump_extra_ctrl.path_state) {
        case DUMP_PATH_STATE_USE_RDR_PATH:
            ret_path = rdr_log_path;
            break;
        case DUMP_PATH_STATE_USE_EXTRA_PATH:
            ret_path = (const char *)g_dump_extra_ctrl.log_path;
            break;
        case DUMP_PATH_STATE_INVALID:
        default:
            break;
    }
    return ret_path;
}

unsigned int dump_extra_log_fs_state_default_func(void)
{
    /* 未初始化不会走到这个函数 */
    return !bsp_eaccess(g_dump_extra_logs_cfg.extra_log_dir);
}

int dump_wait_extra_log_partion_done(void)
{
    int time = DUMP_EXTRA_FS_TIMEOUT_MAX;
    dump_fs_state_func fs_check_func =  g_dump_extra_ctrl.fs_state_check;

    /* 如果不存在检测fs_state的回调 */
    if (g_dump_extra_ctrl.fs_state_check == NULL) {
        fs_check_func = dump_extra_log_fs_state_default_func;
    }
    while (!fs_check_func() && time) {
        set_current_state(TASK_INTERRUPTIBLE);
        (void)schedule_timeout(HZ / 10); /* 10 means wait 100ms */
        time--;
    }
    if (fs_check_func()) {
        return 0;
    }
    return -1;
}

void dump_create_extra_dir_link_info(const char *dst_dir, const char *src_dir)
{
    struct file *fd = NULL;
    char filename[RDR_DUMP_FILE_PATH_LEN] = { 0 };
    if (snprintf_s(filename, sizeof(filename), sizeof(filename) - 1, "%s%s", dst_dir, DUMP_EXTRA_LOG_LINK_FILE) == -1) {
        dump_print("fail gen link file name %s\n", dst_dir);
        return;
    }

    fd = dump_open(filename, O_CREAT | O_RDWR | O_SYNC, DUMP_LOG_FILE_AUTH);
    if (fd == NULL) {
        dump_print("fail to creat file %s\n", filename);
        return;
    }

    (void)dump_write_sync(fd, src_dir, strnlen(src_dir, RDR_DUMP_FILE_PATH_LEN));

    (void)dump_close(fd);
}

u32 dump_prepare_extra_logs_log_path(const char *rdr_log_path)
{
    time64_t txc = 0;
    struct rtc_time tm;
    char date[DUMP_DATETIME_LEN] = { 0 };
    int ret;
    unsigned int path_len;

    /* 没有init成功则说明没有extra log要保存，其余fulldump保存到RDR路径，后续extra fulldump和普通fulldump拆开后，这里可改为INVALID */
    if (!g_dump_extra_logs_cfg.feature_on) {
        g_dump_extra_ctrl.path_state = DUMP_PATH_STATE_USE_RDR_PATH;
        return 0;
    }

    path_len = strnlen(g_dump_extra_logs_cfg.extra_log_dir, RDR_DUMP_FILE_PATH_LEN);
    /* 有extra fulldump但没定制log路径 */
    if (path_len == 0) {
        g_dump_extra_ctrl.path_state = DUMP_PATH_STATE_USE_RDR_PATH;
        return 0;
    }

    (void)memset_s(g_dump_extra_ctrl.log_path, RDR_DUMP_FILE_PATH_LEN, 0, RDR_DUMP_FILE_PATH_LEN);
    ret = strncmp(g_dump_extra_logs_cfg.extra_log_dir, rdr_log_path, path_len);
    /* 配置的extra_log path和基础log path一样，不新建目录，直接存到rdr_log_path里 */
    if (ret == 0) {
        g_dump_extra_ctrl.path_state = DUMP_PATH_STATE_USE_RDR_PATH;
        return 0;
    }

    /* 后面说明有extra log，并且定制路径，如果log路径准备失败，则不允许保存log */
    g_dump_extra_ctrl.path_state = DUMP_PATH_STATE_INVALID;
    ret = dump_wait_extra_log_partion_done();
    if (ret) {
        dump_error("wait extra log partion ready timeout\n");
        return -1;
    }

    txc = ktime_get_real_seconds();
    rtc_time64_to_tm(txc, &tm);
    if (snprintf_s(date, sizeof(date), sizeof(date) - 1, "%4d%02d%02d%02d%02d%02d-%08x", tm.tm_year +
        DUMP_LOG_YEAR_START, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, bsp_get_slice_value()) < 0) {
        dump_debug("er\n");
    }
    ret = rdr_create_exc_dir(g_dump_extra_logs_cfg.extra_log_dir, g_dump_extra_ctrl.log_path, RDR_DUMP_FILE_PATH_LEN,
                             date);
    if (ret) {
        dump_error("create extra log fail\n");
        (void)memset_s(g_dump_extra_ctrl.log_path, RDR_DUMP_FILE_PATH_LEN, 0, RDR_DUMP_FILE_PATH_LEN);
        return -1;
    }
    g_dump_extra_ctrl.path_state = DUMP_PATH_STATE_USE_EXTRA_PATH;
    dump_create_extra_dir_link_info(g_dump_extra_ctrl.log_path, rdr_log_path);
    return 0;
}

u32 dump_get_extra_logs_file_num(void)
{
    device_node_s *extra_file_node = bsp_dt_find_node_by_path("/dump_extra_flash");
    if (extra_file_node != NULL) {
        return (u32)bsp_dt_get_child_count(extra_file_node);
    }
    return 0;
}

int dump_extra_logs_log_path_init(const char *path)
{
    u32 len;
    if (path == NULL) {
        return -1;
    }

    len = strnlen(path, RDR_DUMP_FILE_PATH_LEN);
    if (len == 0 || len >= RDR_DUMP_FILE_PATH_LEN) {
        dump_error("cust extra_dir len err %d!\n", len);
        return -1;
    }
    /* 目录需要以'/'结尾 */
    if (path[len - 1] != '/') {
        dump_error("cust extra_dir format err\n");
        return -1;
    }

    if (strcpy_s(g_dump_extra_logs_cfg.extra_log_dir, RDR_DUMP_FILE_PATH_LEN, path) != EOK) {
        dump_error("cpy cust extra_dir fail\n");
        (void)memset_s(g_dump_extra_logs_cfg.extra_log_dir, RDR_DUMP_FILE_PATH_LEN, 0, RDR_DUMP_FILE_PATH_LEN);
        return -1;
    }
    return 0;
}

/* 该函数用于初始化log strategy，初始化阶段最早，不能添加work */
void dump_extra_logs_strategy_init(void)
{
    device_node_s *extra_file_node = bsp_dt_find_node_by_path("/dump_extra_flash");
    device_node_s *child = NULL;
    const char *path = NULL;
    if (extra_file_node == NULL) {
        dump_warning("no cust extra flash\n");
        return;
    }
    g_dump_extra_logs_cfg.extra_file_num = dump_get_extra_logs_file_num();
    if (g_dump_extra_logs_cfg.extra_file_num == 0) {
        dump_warning("no cust extra flash file\n");
        return;
    }

    if (bsp_dt_property_read_string(extra_file_node, "extra_dir_path", &path)) {
        dump_warning("no cust extra_dir_path, use default\n");
    } else {
        if (dump_extra_logs_log_path_init(path)) {
            /* 初始化log目录失败，直接返回，不往下执行 */
            return;
        }
    }

    bsp_dt_for_each_child_of_node(extra_file_node, child)
    {
        if (bsp_dt_device_is_available(child)) {
            (void)dump_file_attr_init(child);
        }
    }
    g_dump_extra_logs_cfg.feature_on = 1;
}

EXPORT_SYMBOL(mdrv_om_set_extra_log_cfg);
