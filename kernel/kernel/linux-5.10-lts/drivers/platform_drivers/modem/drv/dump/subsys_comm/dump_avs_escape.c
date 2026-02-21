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
#include <product_config.h>
#include <securec.h>
#include <linux/delay.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include <linux/workqueue.h>
#include <osl_types.h>
#include <osl_malloc.h>
#include <bsp_dt.h>
#include <msg_id.h>
#include <bsp_msg.h>
#include <bsp_dump.h>
#include <bsp_file_ops.h>
#include <dump_infra.h>
#include <dump_rdr_wrapper.h>
#include <dump_logs.h>
#include <dump_exc_mgr.h>
#include <dump_subsys_mgr.h>
#include "dump_avs_escape.h"

#ifndef CONFIG_BBOX_LOG_DIR
#define DUMP_LOG_ROOT "/data/hisi_logs"
#define DUMP_AVS_ESCAPE_RESET_LOG_DIR "/data/hisi_logs/modem_log"
#define DUMP_AVS_ESCAPE_RESET_LOG_PATH ("/data/hisi_logs/modem_log" "/reset.log")

#error NO BBOX_LOG_DIR DEFINED

#else
// CONFIG_BBOX_LOG_DIR 以'/'结尾,例如"/data/xx_logs/"
#define DUMP_LOG_ROOT CONFIG_BBOX_LOG_DIR
#define DUMP_AVS_ESCAPE_RESET_LOG_DIR (CONFIG_BBOX_LOG_DIR "modem_log")
#define DUMP_AVS_ESCAPE_RESET_LOG_PATH (CONFIG_BBOX_LOG_DIR "modem_log/reset.log")
#endif

#define DUMP_AVS_RSTLOG_TASK_PRIO 96

#define DUMP_WAIT_AVS_RESETLOG_CNT 600
#define DUMP_WAIT_AVS_RESETLOG_SLEEP_MS 500

#define DUMP_AVS_RST_INFO_MAGIC 0x5A5A0001
#define DUMP_AVS_RST_POLICY_NUM 8
#define DUMP_AVS_RST_RECORD_NUM 128

// 设置一个上限，一般modid段不会太多；若该值增加太大，那dump_avs_dts_cfg的实现需要修改，不能用静态成员，需要malloc内存
#define DUMP_AVS_SOC_POLICY_MODID_MAX_NUM 16

#define DUMP_AVS_SOC_POLICY_ON 0
#define DUMP_AVS_SOC_POLICY_OFF 1

enum dump_avs_policy {
    DUMP_AVS_POLICY_SOC_AVS_ONOFF = 0,
    // 枚举数量需小于 DUMP_AVS_RST_POLICY_NUM
};

struct dump_avs_rst_record {
    u32 modid;
    u32 time; /* 1970年开始的秒数 */
};

struct dump_avs_policy_record {
    u32 from_seq_id;
    u32 policy;
};

struct dump_avs_rst_info {
    u32 magic;
    u32 seq_id;
    u32 resv0;
    u32 resv1;
    struct dump_avs_policy_record policy_info[DUMP_AVS_RST_POLICY_NUM];
    struct dump_avs_rst_record rst_info[DUMP_AVS_RST_RECORD_NUM];
};

struct dump_avs_dts_cfg {
    struct {
        u32 off_time_th;
        u32 off_freq_th;
        u32 on_time_th;
        u32 on_freq_th;
        u32 modid_num;
    } soc_policy;
    u32 soc_modid[DUMP_AVS_SOC_POLICY_MODID_MAX_NUM];
    u32 soc_modid_end[DUMP_AVS_SOC_POLICY_MODID_MAX_NUM];
    u32 inited;
};

struct dump_avs_escape_cfg {
    struct dump_avs_dts_cfg dts_cfg;
    struct dump_avs_rst_info *rst_info;
    struct dump_avs_rst_info *rst_info_share;
};

struct dump_avs_ctrl {
    u32 log_inited;
    msg_chn_t msg_hdl;
    struct workqueue_struct *wq;
    struct work_struct init_work;
    struct work_struct sync_data_work;
};

enum dump_avs_errno_e {
    DUMP_AVS_OK = 0,
    DUMP_AVS_CFG_NODE_NOT_FOUND = -1,
    DUMP_AVS_CFG_ATTR_NOT_FOUND = -2,
    DUMP_AVS_DIR_CREATE_FAIL = -3,
    DUMP_AVS_FILE_CREATE_FAIL = -4,
    DUMP_AVS_FILE_OPEN_FAIL = -5,
    DUMP_AVS_FILE_READ_FAIL = -6,
    DUMP_AVS_FILE_UPDATE_FAIL = -7,
    DUMP_AVS_FILE_PARA_CHECK_FAIL = -8,
    DUMP_AVS_MALLOC_FAIL = -9,
    DUMP_AVS_MAP_PCTRL = -10,
    DUMP_AVS_CFG_ATTR_CHECK_FAIL = -11,
    DUMP_AVS_TASK_INIT_FAIL = -12,
    DUMP_AVS_RST_INFO_NULL = -13,
    DUMP_AVS_RST_INFO_SHARE_MEM_NULL = -14,
};

/* CP-AP msg */
#define DUMP_AVS_MSG_TYPE_GET_AVS_DATA 0xDADA0001
#define DUMP_AVS_MSG_TYPE_SYNC_AVS_DATA 0xDADA0002

struct dump_avs_msg_head {
    u32 msg_type;
    u32 data[0];
};

typedef void (*dump_avs_policy_func_t)(void);
void dump_avs_update_soc_policy(void);

__ro_after_init struct dump_avs_escape_cfg g_dump_avs_escape_cfg;
struct dump_avs_ctrl g_dump_avs_ctrl;
const dump_avs_policy_func_t g_dump_avs_policy_cbs[] = {
    dump_avs_update_soc_policy,
};

enum dump_avs_status dump_avs_get_state(void)
{
    u32 cur_seq_id, curr_policy, policy_time;
    // 未初始化，或者没有复位记录(seq_id=0,  该值不会回卷，=0一定是没有复位log记录)
    if (!g_dump_avs_ctrl.log_inited || (g_dump_avs_escape_cfg.rst_info->seq_id == 0)) {
        return DUMP_AVS_ALREADY_ENABLE;
    }
    cur_seq_id = g_dump_avs_escape_cfg.rst_info->seq_id;
    curr_policy = g_dump_avs_escape_cfg.rst_info->policy_info[DUMP_AVS_POLICY_SOC_AVS_ONOFF].policy;
    policy_time = g_dump_avs_escape_cfg.rst_info->policy_info[DUMP_AVS_POLICY_SOC_AVS_ONOFF].from_seq_id;
    if (curr_policy == DUMP_AVS_SOC_POLICY_OFF) {
        if (policy_time != cur_seq_id) {
            return DUMP_AVS_ALREADY_DISABLE;
        } else {
            return DUMP_AVS_ENABLE_TO_DISABLE;
        }
    } else {
        if (policy_time != cur_seq_id) {
            return DUMP_AVS_ALREADY_ENABLE;
        } else {
            return DUMP_AVS_DISABEL_TO_ENABLE;
        }
    }
    return DUMP_AVS_ALREADY_ENABLE;
}

/**********msg处理******************/
void dump_avs_sync_data_to_cp(struct work_struct *work)
{
    int ret;
    struct msg_addr dst;
    struct dump_avs_msg_head sync_msg;

    if (!g_dump_avs_ctrl.log_inited) {
        dump_print("avs esc init fail, drop sync msg\n");
    }

    dst.core = MSG_CORE_TSP;
    dst.chnid = MSG_CHN_DUMP;

    sync_msg.msg_type = DUMP_AVS_MSG_TYPE_SYNC_AVS_DATA;
    ret = bsp_msg_lite_sendto(g_dump_avs_ctrl.msg_hdl, &dst, &sync_msg, sizeof(struct dump_avs_msg_head));
    if (ret) {
        dump_error("send msg ret %d\n", ret);
    }
}

int dump_avs_get_data_msg_proc(void *msg, u32 len)
{
    if (len != 0) {
        dump_print("avs data msg len err\n");
        return -1;
    }
    // 可能与init work并发，使用active=1的workqueue消除并发，保证send_data前，init_work已经在workqueue中执行了
    queue_work(g_dump_avs_ctrl.wq, &g_dump_avs_ctrl.sync_data_work);
    return 0;
}

int dump_avs_msglite_proc(const struct msg_addr *src, void *msg, u32 len)
{
    u32 payload_len;
    struct dump_avs_msg_head *msg_head = (struct dump_avs_msg_head *)msg;
    if ((src == NULL) || (msg == NULL) || (len == 0)) {
        dump_print("avs msg para null\n");
        return -1;
    }

    if (src->core != MSG_CORE_TSP || src->chnid != MSG_CHN_DUMP || len < sizeof(struct dump_avs_msg_head)) {
        dump_print("avs msg para inv\n");
        return -1;
    }

    if (msg_head->msg_type != DUMP_AVS_MSG_TYPE_GET_AVS_DATA) {
        dump_print("avs msg type inv\n");
        return -1;
    }

    payload_len = len - sizeof(struct dump_avs_msg_head);
    dump_avs_get_data_msg_proc(&msg_head->data[0], payload_len);

    return 0;
}

/* 当前只有avs escape使用，因此dump msglite初始化放在这里 */
__init int dump_avs_msglite_init(void)
{
    int ret;
    struct msgchn_attr attr = {0};

    bsp_msgchn_attr_init(&attr);
    attr.chnid = MSG_CHN_DUMP;
    attr.coremask = MSG_CORE_MASK(MSG_CORE_TSP);
    attr.lite_notify = dump_avs_msglite_proc;

    ret = bsp_msg_lite_open(&g_dump_avs_ctrl.msg_hdl, &attr);
    if (ret) {
        dump_error("msg open err %d\n", ret);
        return -1;
    }
    return 0;
}

void dump_avs_share_data_update(void)
{
    int ret;
    if (g_dump_avs_escape_cfg.rst_info_share != NULL) {
        ret = memcpy_s(g_dump_avs_escape_cfg.rst_info_share, sizeof(struct dump_avs_rst_info),
                       g_dump_avs_escape_cfg.rst_info, sizeof(struct dump_avs_rst_info));
        if (ret != EOK) {
            dump_error("cpy avs share mem fail\n");
        }
    } else {
        dump_print("no rstlog share mem\n");
    }
}

int dump_avs_record_flush(void)
{
    struct file *fd = NULL;
    unsigned long fs;
    int ret;
    loff_t pos = 0;

    if (g_dump_avs_escape_cfg.rst_info == NULL) {
        return DUMP_AVS_RST_INFO_NULL;
    }

    dump_avs_share_data_update();

    fd = dump_open(DUMP_AVS_ESCAPE_RESET_LOG_PATH, O_CREAT | O_RDWR | O_SYNC, DUMP_LOG_FILE_AUTH);
    if (fd == NULL) {
        return DUMP_AVS_FILE_OPEN_FAIL;
    }

    fs = get_fs();
    set_fs((mm_segment_t)KERNEL_DS);

    ret = kernel_write(fd, g_dump_avs_escape_cfg.rst_info, sizeof(struct dump_avs_rst_info), &pos);
    if (ret != sizeof(struct dump_avs_rst_info)) {
        dump_error("dump avs file write fail\n");
    }

    (void)vfs_fsync(fd, 0);
    (void)filp_close(fd, NULL);
    set_fs(fs);
    return DUMP_AVS_OK;
}

void dump_avs_update_record(u32 modid, u32 time)
{
    u32 cur_idx = g_dump_avs_escape_cfg.rst_info->seq_id % DUMP_AVS_RST_RECORD_NUM;
    g_dump_avs_escape_cfg.rst_info->rst_info[cur_idx].modid = modid;
    g_dump_avs_escape_cfg.rst_info->rst_info[cur_idx].time = time;
    g_dump_avs_escape_cfg.rst_info->seq_id++;
}

int dump_avs_update_one_policy(u32 policy_id, u32 policy_data)
{
    if (policy_id >= DUMP_AVS_RST_POLICY_NUM) {
        dump_print("avs policy id 0x%x err\n", policy_id);
        return -1;
    }
    g_dump_avs_escape_cfg.rst_info->policy_info[policy_id].from_seq_id = g_dump_avs_escape_cfg.rst_info->seq_id;
    g_dump_avs_escape_cfg.rst_info->policy_info[policy_id].policy = policy_data;
    return 0;
}

u32 dump_avs_soc_policy_revert(u32 policy)
{
    return (policy == DUMP_AVS_SOC_POLICY_ON) ? DUMP_AVS_SOC_POLICY_OFF : DUMP_AVS_SOC_POLICY_ON;
}

bool dump_avs_soc_policy_id_match(u32 modid)
{
    u32 i;
    for (i = 0; i < g_dump_avs_escape_cfg.dts_cfg.soc_policy.modid_num; i++) {
        if ((modid >= g_dump_avs_escape_cfg.dts_cfg.soc_modid[i]) &&
            (modid <= g_dump_avs_escape_cfg.dts_cfg.soc_modid_end[i])) {
            return true;
        }
    }
    return false;
}

void dump_avs_update_soc_policy(void)
{
    u32 cur_seq_id = g_dump_avs_escape_cfg.rst_info->seq_id;
    u32 curr_policy = g_dump_avs_escape_cfg.rst_info->policy_info[DUMP_AVS_POLICY_SOC_AVS_ONOFF].policy;
    u32 record_num = cur_seq_id >= DUMP_AVS_RST_RECORD_NUM ? DUMP_AVS_RST_RECORD_NUM : cur_seq_id;
    u32 start_idx = g_dump_avs_escape_cfg.rst_info->policy_info[DUMP_AVS_POLICY_SOC_AVS_ONOFF].from_seq_id;
    u32 freq_th = (curr_policy == DUMP_AVS_SOC_POLICY_ON) ? g_dump_avs_escape_cfg.dts_cfg.soc_policy.off_freq_th :
                                                            g_dump_avs_escape_cfg.dts_cfg.soc_policy.on_freq_th;
    u32 time_th = (curr_policy == DUMP_AVS_SOC_POLICY_ON) ? g_dump_avs_escape_cfg.dts_cfg.soc_policy.off_time_th :
                                                            g_dump_avs_escape_cfg.dts_cfg.soc_policy.on_time_th;
    u32 first_time = 0;
    u32 last_time = 0;
    u32 i, idx;

    if (!g_dump_avs_escape_cfg.dts_cfg.inited) {
        dump_print("no avs esc cfg\n");
    }

    // 异常处理，start index不应大于current index, 不用考虑回卷，因为即使最快1分钟复位1次，U32数据也足够记录了
    if (start_idx > cur_seq_id) {
        g_dump_avs_escape_cfg.rst_info->policy_info[DUMP_AVS_POLICY_SOC_AVS_ONOFF].from_seq_id = 0;
        g_dump_avs_escape_cfg.rst_info->policy_info[DUMP_AVS_POLICY_SOC_AVS_ONOFF].policy = 0;
        return;
    }
    // 计算可用复位信息记录数
    if (cur_seq_id - start_idx < record_num) {
        record_num = cur_seq_id - start_idx;
    }

    for (i = 0; i < record_num && freq_th != 0; i++) {
        idx = (cur_seq_id - i -1) % DUMP_AVS_RST_RECORD_NUM;
        if (!dump_avs_soc_policy_id_match(g_dump_avs_escape_cfg.rst_info->rst_info[idx].modid)) {
            continue;
        }
        freq_th--;
        // 是否是第一条数据
        if (last_time == 0) {
            last_time = g_dump_avs_escape_cfg.rst_info->rst_info[idx].time;
        }
        // 是否是最后一条数据
        if (freq_th == 0) {
            first_time = g_dump_avs_escape_cfg.rst_info->rst_info[idx].time;
            break;
        }
    }
    // 当freq_th和time_th都满足条件时，更新策略
    if ((freq_th == 0) && (last_time - first_time > 0) && (last_time - first_time <= time_th)) {
        curr_policy = dump_avs_soc_policy_revert(curr_policy);
        dump_avs_update_one_policy(DUMP_AVS_POLICY_SOC_AVS_ONOFF, curr_policy);
    }
}

void dump_avs_update_policy(void)
{
    u32 i;
    for (i = 0; i < ARRAY_SIZE(g_dump_avs_policy_cbs); i++) {
        g_dump_avs_policy_cbs[i]();
    }
}

void dump_avs_record_once(u32 modid)
{
    time64_t txc = ktime_get_real_seconds();
    u32 cur_time = (u32)txc;

    dump_avs_update_record(modid, cur_time);
    dump_avs_update_policy();
    dump_avs_record_flush();
}

void dump_save_avs_escape_log(const dump_exc_log_desc_s *exc_desc, void *priv_data)
{
    UNUSED(priv_data);
    if (g_dump_avs_ctrl.log_inited != 1) {
        return;
    }
    if (!dump_valid_exc_desc(exc_desc)) {
        dump_debug("exc_desc inv\n");
        return;
    }

    if (exc_desc->exc_info->subsys_info.e_from_subsys != DUMP_SUBSYS_CP) {
        dump_print("avs_chk:not cp_exc\n");
        return;
    }

    // normal reset的数据无意义，不进行记录，为后面预留，当前CP复位没有normal reset的
    if (exc_desc->exc_info->exc_cfg->exc_type != CP_S_NORMALRESET) {
        dump_avs_record_once(exc_desc->exc_info->subsys_info.modid);
    }
}

int dump_avs_file_init(struct file *fd)
{
    int ret;
    loff_t pos = 0;
    g_dump_avs_escape_cfg.rst_info->magic = DUMP_AVS_RST_INFO_MAGIC;
    g_dump_avs_escape_cfg.rst_info->seq_id = 0;
    ret = kernel_write(fd, g_dump_avs_escape_cfg.rst_info, sizeof(struct dump_avs_rst_info), &pos);
    if (ret != sizeof(struct dump_avs_rst_info)) {
        dump_error("dump avs file init fail %d\n", ret);
        /* 写文件失败不影响重新统计异常信息，只是之前的信息丢失了，默认关avs，并重新统计异常复位信息 */
    }
    return DUMP_AVS_OK;
}

int dump_read_avs_log_from_file(struct file *fd)
{
    int ret;
    ret = kernel_read(fd, g_dump_avs_escape_cfg.rst_info, sizeof(struct dump_avs_rst_info), 0);
    if (ret != sizeof(struct dump_avs_rst_info)) {
        dump_error("dump avs file read fail\n");
        return dump_avs_file_init(fd);
    }
    if (g_dump_avs_escape_cfg.rst_info->magic != DUMP_AVS_RST_INFO_MAGIC) {
        dump_error("dump avs file check fail\n");
        return dump_avs_file_init(fd);
    }
    return DUMP_AVS_OK;
}

int dump_avs_log_init(void)
{
    int ret = DUMP_AVS_OK;
    unsigned long fs;
    loff_t pos = 0;
    struct file *fd = NULL;

    fd = dump_open(DUMP_AVS_ESCAPE_RESET_LOG_PATH, O_CREAT | O_RDWR | O_SYNC, DUMP_LOG_FILE_AUTH);
    if (fd == NULL) {
        return DUMP_AVS_FILE_OPEN_FAIL;
    }

    fs = get_fs();
    set_fs((mm_segment_t)KERNEL_DS);
    pos = vfs_llseek(fd, 0, SEEK_END);
    if (pos < sizeof(struct dump_avs_rst_info)) {
        ret = dump_avs_file_init(fd);
    } else {
        ret = dump_read_avs_log_from_file(fd);
    }

    (void)vfs_fsync(fd, 0);
    (void)filp_close(fd, NULL);
    set_fs(fs);

    return ret;
}

__init int dump_avs_cfg_init(void)
{
    int ret = DUMP_AVS_OK;
    device_node_s *dev_node = NULL;

    dev_node = bsp_dt_find_compatible_node(NULL, NULL, "driver,dump_avs_escape");
    if (unlikely(dev_node == NULL)) {
        ret = DUMP_AVS_CFG_NODE_NOT_FOUND;
        goto err;
    }
    if (bsp_dt_property_read_u32_array(dev_node, "cfg", (u32 *)&g_dump_avs_escape_cfg.dts_cfg.soc_policy,
                                       sizeof(g_dump_avs_escape_cfg.dts_cfg.soc_policy) / sizeof(u32))) {
        ret = DUMP_AVS_CFG_ATTR_NOT_FOUND;
        goto err;
    }
    if ((g_dump_avs_escape_cfg.dts_cfg.soc_policy.off_freq_th > DUMP_AVS_RST_RECORD_NUM) ||
        (g_dump_avs_escape_cfg.dts_cfg.soc_policy.on_freq_th > DUMP_AVS_RST_RECORD_NUM) ||
        (g_dump_avs_escape_cfg.dts_cfg.soc_policy.modid_num > DUMP_AVS_SOC_POLICY_MODID_MAX_NUM)) {
        ret = DUMP_AVS_CFG_ATTR_CHECK_FAIL;
        goto err;
    }

    if (bsp_dt_property_read_u32_array(dev_node, "modid", (u32 *)&g_dump_avs_escape_cfg.dts_cfg.soc_modid,
                                       g_dump_avs_escape_cfg.dts_cfg.soc_policy.modid_num)) {
        ret = DUMP_AVS_CFG_ATTR_NOT_FOUND;
        goto err;
    }
    if (bsp_dt_property_read_u32_array(dev_node, "modid_end", (u32 *)&g_dump_avs_escape_cfg.dts_cfg.soc_modid_end,
                                       g_dump_avs_escape_cfg.dts_cfg.soc_policy.modid_num)) {
        ret = DUMP_AVS_CFG_ATTR_NOT_FOUND;
        goto err;
    }

    g_dump_avs_escape_cfg.dts_cfg.inited = 1;
    return DUMP_AVS_OK;

err:
    (void)memset_s(&g_dump_avs_escape_cfg.dts_cfg, sizeof(struct dump_avs_dts_cfg), 0, sizeof(struct dump_avs_dts_cfg));
    return ret;
}

void dump_avs_file_init_work(struct work_struct *work)
{
    int ret;
    unsigned long fs;
    struct kstat stat;
    int timeout = DUMP_WAIT_AVS_RESETLOG_CNT;
    u32 ready = 0;

    while (timeout) {
        timeout--;
        fs = get_fs();
        set_fs(KERNEL_DS);
        ret = vfs_stat(DUMP_LOG_ROOT, &stat);
        set_fs(fs);
        if (ret) {
            msleep(DUMP_WAIT_AVS_RESETLOG_SLEEP_MS);
            continue; 
        }
        ready = 1;
        break;
    }
    if (!ready) {
        dump_error("check logdir ret %d\n", ret);
        return;
    }
    ret = dump_avs_log_init();
    if (ret != DUMP_AVS_OK) {
        dump_error("init dump_avs cfg ret %d\n", ret);
        return;
    }

    dump_avs_share_data_update();
    g_dump_avs_ctrl.log_inited = 1;
    return;
}

__init int dump_avs_escape_init(void)
{
    int ret;
    dump_work_s work;

    /* 申请成功后，后面禁止释放；因为即使没有dts_cfg或者没有成功创建文件，该变量设计上，也会在复位、msg等场景使用；
     * 这样启动依然有机会进行avs止血方案，只是每次掉电后之前的策略数据会丢失
     */
    g_dump_avs_escape_cfg.rst_info = osl_malloc(sizeof(struct dump_avs_rst_info));
    if (g_dump_avs_escape_cfg.rst_info == NULL) {
        return DUMP_AVS_MALLOC_FAIL;
    }
    (void)memset_s(g_dump_avs_escape_cfg.rst_info, sizeof(struct dump_avs_rst_info), 0, sizeof(struct dump_avs_rst_info));

    g_dump_avs_escape_cfg.rst_info_share = (struct dump_avs_rst_info *)bsp_dump_register_field(
        DUMP_MODEMAP_MDM_RST_HISTORY, "CP_RST_HISTORY", sizeof(struct dump_avs_rst_info), 0);

    INIT_WORK(&g_dump_avs_ctrl.init_work, dump_avs_file_init_work);
    INIT_WORK(&g_dump_avs_ctrl.sync_data_work, dump_avs_sync_data_to_cp);
    g_dump_avs_ctrl.wq = alloc_workqueue("mdmdump_avs", WQ_UNBOUND | WQ_HIGHPRI, 1);
    queue_work(g_dump_avs_ctrl.wq, &g_dump_avs_ctrl.init_work);

    /* A核初始化早，一般不需要msg通知CP，但由于流程是并发的，实际无法保证A核读完文件后，C核dump才初始化，保险起见，还是增加msg交互 */
    dump_avs_msglite_init();

    work.work_type = DUMP_WORK_ASYNC;
    work.work_fn = dump_save_avs_escape_log;
    work.prior = DUMP_WORK_PRIO_ASYNC_HOOK_COMMON_UPDATE_AVS_ESCAPE;
    work.priv_data = NULL;
    dump_subsys_add_work(DUMP_SUBSYS_COMM, &work);

    ret = dump_avs_cfg_init();
    if (ret != DUMP_AVS_OK) {
        dump_error("init dump_soc_avs cfg ret %d\n", ret);
        return ret;
    }

    return DUMP_AVS_OK;
}