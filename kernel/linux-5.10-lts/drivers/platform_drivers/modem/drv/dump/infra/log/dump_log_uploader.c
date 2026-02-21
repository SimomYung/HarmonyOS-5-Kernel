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
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <of.h>
#include <osl_list.h>
#include <osl_malloc.h>
#include <bsp_dt.h>
#include <bsp_print.h>
#include <bsp_slice.h>
#include <bsp_dfxcert.h>
#include <bsp_dump.h>
#include <dump_errno.h>
#include <dump_infra.h>
#include <dump_exc_mgr.h>
#include <dump_subsys_mgr.h>
#include "dump_log_strategy.h"
#include "dump_genl.h"
#include "dump_log_uploader.h"

/* 当前需要往上位机上报的log数目不多，且各版本数量无明显差异，因此这个结构体成员直接静态配置长度信息，不通过log_strategy动态申请；
 * 若后续长度增加，或者版本间差异变大；需要在初始化时，获取log_strategy中当前版本配置的可上传文件数目并动态申请内存
 */
#define DUMP_UPLOAD_FD_MAX 32
struct dump_upload_fd_desc {
    dump_save_file_id_e id[DUMP_UPLOAD_FD_MAX];
    u32 offset[DUMP_UPLOAD_FD_MAX];
    u32 used_nums;
};

struct dump_upload_fd_desc g_dump_upload_only_file_desc;
dump_upload_ctrl_s g_dump_upload_ctrl;
u32 g_dump_upload_datarate_ms = DUMP_UPLOAD_DEFAULT_DATARATE_PER_MS;

int dump_send_app_cmd(uint16_t cmd);
int dump_uploader_prepare(void);

/* 初始化和单独复位时会使用 */
void dump_upload_only_file_desc_init(void)
{
    int i;
    for (i = 0; i < DUMP_UPLOAD_FD_MAX; i++) {
        g_dump_upload_only_file_desc.id[i] = -1;
        g_dump_upload_only_file_desc.offset[i] = 0;
    }
}

u32 dump_inc_and_get_upload_file_last_write_ptr(dump_save_file_id_e id, u32 len)
{
    int i;
    u32 offset = 0;
    for (i = 0; (i < DUMP_UPLOAD_FD_MAX) && (g_dump_upload_only_file_desc.id[i] != -1); i++) {
        if (g_dump_upload_only_file_desc.id[i] == id) {
            offset = g_dump_upload_only_file_desc.offset[i];
            g_dump_upload_only_file_desc.offset[i] += len;
            break;
        }
    }
    if (i == DUMP_UPLOAD_FD_MAX) {
        dump_error("upload only file desc exceed\n");
        return 0;
    }
    if (g_dump_upload_only_file_desc.id[i] == -1) {
        g_dump_upload_only_file_desc.id[i] = id;
        g_dump_upload_only_file_desc.offset[i] = len;
    }
    return offset;
}

u32 dump_upload_timeout_ms(u32 data_len)
{
    u32 timeout;
    timeout = data_len / g_dump_upload_datarate_ms;
    if (data_len % g_dump_upload_datarate_ms) {
        timeout += 1;
    }
    return timeout;
}

u32 dump_upload_get_last_pkt_timeout_ms_left(void)
{
    u32 delta_time;
    u32 start_ms = g_dump_upload_ctrl.last_send_time_ms;
    u32 timeout_ms = g_dump_upload_ctrl.last_send_timeout_ms;
    /* 第一包数据，默认提供一定timeout，增加稳定性; 防止前几包数据由于网络波动而被dump主动丢包 */
    if (start_ms == 0) {
        return  DUMP_UPLOAD_INIT_TIMEOUT_VAL_MS;
    }
    delta_time = bsp_get_elapse_ms();
    delta_time = (delta_time >= start_ms) ? (delta_time - start_ms) : (0xFFFFFFFF - start_ms + delta_time);
    if (timeout_ms > delta_time) {
        return timeout_ms - delta_time;
    }
    return 0;
}

void dump_upload_wait_last_pkt_timeout(void)
{
    u32 timeout_ms = dump_upload_get_last_pkt_timeout_ms_left();
    if (timeout_ms != 0) {
        msleep(timeout_ms);
    }
}

/* 调试使用，正式版本不要调用 */
void dump_set_upload_datarate(u32 rate)
{
    g_dump_upload_datarate_ms = rate;
}

bool dump_get_upload_auth(void)
{
    int auth = bsp_dfxcert_status_get(OM_FULLDUMP_UPLOAD);
    bool result = (auth == DFXCERT_PORT_SUPPORT);
    return result;
}

int dump_wait_server_state(void)
{
    int ret;
    u32 cur_ms, delta_ms;
    u32 start_ms = g_dump_upload_ctrl.server_connect_trigger_time;

    if (g_dump_upload_ctrl.server_connected) {
        return 0;
    }

    if (g_dump_upload_ctrl.server_connect_trigger_time == 0) {
        ret = dump_uploader_prepare();
        if (ret < 0) {
            return -1;
        }
    }
    cur_ms = bsp_get_elapse_ms();
    delta_ms = (cur_ms >= start_ms) ? (cur_ms - start_ms) : (0xFFFFFFFF - start_ms + cur_ms);
    while (!g_dump_upload_ctrl.server_connected && delta_ms < DUMP_UPLOAD_SERVER_CONNECT_TIMEOUT_MS) {
        msleep(DUMP_UPLOAD_WAIT_LAST_PKT_DONE_DELTA_MS);
        cur_ms = bsp_get_elapse_ms();
        delta_ms = (cur_ms >= start_ms) ? (cur_ms - start_ms) : (0xFFFFFFFF - start_ms + cur_ms);
    }

    if (!g_dump_upload_ctrl.server_connected) {
        return -1;
    }

    return 0;
}

int dump_check_can_upload(dump_save_file_id_e file_id)
{
    const dump_file_attr_s *fattr = NULL;
    int ret;

    fattr = dump_get_file_attr(file_id);
    if (fattr == NULL) {
        dump_debug("%d no upload strategy\n", file_id);
        return DUMP_NOT_UPLOAD;
    }

    if (!DUMP_MATCH_ATTRIBUTE(DUMP_ATTR_UPLOAD, fattr->attr)) {
        return DUMP_NOT_UPLOAD;
    }

    if (!g_dump_upload_ctrl.genl_connected) {
        dump_debug("no agent con\n");
        return DUMP_NOT_UPLOAD;
    }
    /* 如果是minidump，不检查证书状态 */
    if (!DUMP_MATCH_ATTRIBUTE(DUMP_ATTR_SAVE_MINIDUMP, fattr->attr)) {
        if (!g_dump_upload_ctrl.feature_on) {
            g_dump_upload_ctrl.feature_on = dump_get_upload_auth();
        }

        if (!g_dump_upload_ctrl.feature_on) {
            dump_debug("auth fail\n");
            return DUMP_NOT_UPLOAD;
        }
    }

    ret = dump_wait_server_state();
    if (ret != 0) {
        return DUMP_NOT_UPLOAD;
    }

    return DUMP_MATCH_ATTRIBUTE(DUMP_ATTR_CRYPTO, fattr->attr) ? DUMP_UPLOAD_CRYPTO : DUMP_UPLOAD_NOCRYPTO;
}

void dump_upload_fill_file_header(const char *file_name)
{
    const dump_exc_log_desc_s *exc_desc = dump_get_current_exc_desc();
    const char *log_path = NULL;
    /* 不应该走到该分支，但如果log路径被清空了(比如存log超时，或者被踩了)，不应该影响上传该文件 */
    if (!dump_valid_exc_log_path(exc_desc)) {
        dump_error("log path err\n");
        log_path = "/unknown/";
    } else {
        log_path = (const char *)exc_desc->log_path;
    }
    (void)memset_s(g_dump_upload_ctrl.pkt_head.filename, RDR_DUMP_FILE_PATH_LEN, 0, RDR_DUMP_FILE_PATH_LEN);
    if (snprintf_s(g_dump_upload_ctrl.pkt_head.filename, RDR_DUMP_FILE_PATH_LEN, RDR_DUMP_FILE_PATH_LEN - 1, "%s%s",
                   log_path, file_name) < 0) {
        dump_debug("sncpy fname err\n");
    }
}

void dump_upload_file(dump_save_file_id_e file_id, struct file *file, const char *data, u32 data_len)
{
    int ret, upload_auth;
    dump_genl_pkt_ctrl_s dump_adv_ctrl;
    u32 offset = dump_inc_and_get_upload_file_last_write_ptr(file_id, data_len);

    upload_auth = dump_check_can_upload(file_id);
    if (upload_auth == DUMP_NOT_UPLOAD) {
        dump_debug("%s not upload\n", (const char *)file->f_path.dentry->d_name.name);
        return;
    }
    g_dump_upload_ctrl.total_data_len += data_len + sizeof(g_dump_upload_ctrl.pkt_head);

    dump_adv_ctrl.msg_type = DUMP_KERNEL_MSG_TYPE_UPLOAD;
    dump_adv_ctrl.file_id = (u16)file_id;
    dump_adv_ctrl.file_offset = offset;

    osl_sem_down(&g_dump_upload_ctrl.upload_sem);

    /* 要分两次发数据，增加1ms间隔 */
    dump_adv_ctrl.timeout_ms = dump_upload_timeout_ms(data_len + sizeof(g_dump_upload_ctrl.pkt_head)) + 1;
    /* 当前在每一包数据发送完成时，为了节约时间，没有强制等待到达流控超时时间才发下一包，因此可能会出现上一包发送完成，
     * 但是网络拥塞的情况，因此需要将上次剩余的时间补充到这次的超时里 */
    dump_adv_ctrl.timeout_ms += dump_upload_get_last_pkt_timeout_ms_left();
    /* dump_upload_get_last_pkt_timeout_ms_left会根据上次的last_send_time_ms，last_send_timeout_ms计算超时，
     * 因此在调用该函数后，再去更新这两个成员 */
    g_dump_upload_ctrl.last_send_time_ms = bsp_get_elapse_ms();
    g_dump_upload_ctrl.last_send_timeout_ms = dump_adv_ctrl.timeout_ms;
    dump_adv_ctrl.op = (upload_auth == DUMP_UPLOAD_CRYPTO) ? DUMP_KERNEL_UPLOAD_OP_CRYPTO_FILE_NAME :
                                                             DUMP_KERNEL_UPLOAD_OP_NOCRYPTO_FILE_NAME;

    dump_upload_fill_file_header((const char *)file->f_path.dentry->d_name.name);
    /* 包头小于一条dump genlmsg的长度，不会被拆包，因此返回值只会是 errno/包长 */
    ret = dump_genl_send_data_advance((u8 *)&g_dump_upload_ctrl.pkt_head, sizeof(g_dump_upload_ctrl.pkt_head),
                                      &dump_adv_ctrl);
    if (ret <= 0) {
        dump_debug("upload file header fail ret %d\n", ret);
        goto out;
    }
    g_dump_upload_ctrl.send_len += ret;

    dump_adv_ctrl.op = (upload_auth == DUMP_UPLOAD_CRYPTO) ? DUMP_KERNEL_UPLOAD_OP_CRYPTO_FILE_DATA :
                                                             DUMP_KERNEL_UPLOAD_OP_NOCRYPTO_FILE_DATA;
    dump_adv_ctrl.timeout_ms = dump_upload_get_last_pkt_timeout_ms_left();
    if (dump_adv_ctrl.timeout_ms == 0) {
        dump_debug("upload file header timeout, no data send\n");
        goto out;
    }
    ret = dump_genl_send_data_advance((u8 *)data, data_len, &dump_adv_ctrl);
    if (ret <= 0) {
        dump_debug("upload file data fail ret %d\n", ret);
        goto out;
    }
    g_dump_upload_ctrl.send_len += ret;

out:
    osl_sem_up(&g_dump_upload_ctrl.upload_sem);
}

static inline void dump_set_log_upload_connect_state(u32 stat, struct genl_info *info)
{
    g_dump_upload_ctrl.genl_connected = !!stat;
    if (g_dump_upload_ctrl.genl_connected) {
        dump_genl_set_port_info(info);
    } else {
        /* clear port info 可能导致正在发送流程的dump netlink访问空指针，因此断开连接时不清除genl的连接信息；这样只会导致
         * 当前数据发送失败，而不会panic
         */
    }
}

int dump_send_app_cmd(uint16_t cmd)
{
    dump_genl_pkt_ctrl_s dump_adv_ctrl;
    struct dump_kernel_cmd_pkt data_pkt;
    int ret;

    dump_adv_ctrl.msg_type = DUMP_KERNEL_MSG_TYPE_CMD;
    dump_adv_ctrl.op = cmd;
    dump_adv_ctrl.file_id = 0xFFFF;
    dump_adv_ctrl.file_offset = 0xFFFFFFFF;
    dump_adv_ctrl.timeout_ms = dump_upload_timeout_ms(sizeof(data_pkt));

    data_pkt.msg_type = cmd;
    data_pkt.data = 0;
    ret = dump_genl_send_data_advance((u8 *)&data_pkt, sizeof(data_pkt), &dump_adv_ctrl);
    if (ret != sizeof(data_pkt)) {
        dump_error("send cmd[%d] fail ret 0x%x\n", cmd, ret);
        return -1;
    }
    return 0;
}

int dump_logagent_app_data_handle(unsigned char *data, int data_len, struct genl_info *info)
{
    struct dump_app_cmd_pkt *head = (struct dump_app_cmd_pkt *)data;

    if (data_len != sizeof(struct dump_app_cmd_pkt)) {
        dump_error("err genl appdata len\n");
        return -1;
    }

    switch (head->cmd) {
        case DUMP_APP_CMD_SET_CONN_STATE:
            dump_set_log_upload_connect_state(head->data, info);
            break;
        case DUMP_APP_CMD_SET_SERVER_STATE:
            g_dump_upload_ctrl.server_connected = !!(head->data);
            break;
        case DUMP_APP_CMD_SET_UPLOAD_LEN:
            g_dump_upload_ctrl.processed_len += head->data;
            break;
        default:
            dump_error("unsupport app cmd:0x%x\n", head->cmd);
            break;
    }

    return 0;
}

int dump_genl_rcv_handler(struct sk_buff *skb, struct genl_info *info)
{
    unsigned char *data = NULL;
    int data_len;
    int ret;

    ret = dump_genl_rcv_para_check(skb, info);
    if (ret) {
        return ret;
    }

    data = (unsigned char *)genlmsg_data(info->genlhdr);
    data_len = genlmsg_len(info->genlhdr);
    if (data_len < 0) {
        return -1;
    }
    return dump_logagent_app_data_handle(data, data_len, info);
}

int dump_uploader_prepare(void)
{
    int ret;

    /* 当前主线外销MBB不支持单独复位，连接server只允许触发一次，如果有单独复位，则应在单独复位的回调中清理相应标记 */
    if (!g_dump_upload_ctrl.server_connect_trigger_time) {
        g_dump_upload_ctrl.server_connect_trigger_time = bsp_get_elapse_ms();
    } else {
        return 0;
    }
    /* minidump 默认允许上报，因此不检查证书状态 */
    if (!g_dump_upload_ctrl.genl_connected) {
        return -1;
    }

    ret = dump_send_app_cmd(DUMP_KERNEL_CMD_OP_CONNECT);
    if (ret < 0) {
        /* 发送失败后，把发送时间设置为timeout时间 */
        g_dump_upload_ctrl.server_connect_trigger_time = bsp_get_elapse_ms() - DUMP_UPLOAD_SERVER_CONNECT_TIMEOUT_MS;
        dump_error("notify app disconnect ret %d\n", ret);
        return -1;
    }
    return ret;
}

int dump_uploader_reset_proc(void)
{
    dump_upload_only_file_desc_init();
    g_dump_upload_ctrl.server_connect_trigger_time = 0;
    g_dump_upload_ctrl.total_data_len = 0;
    g_dump_upload_ctrl.send_len = 0;
    g_dump_upload_ctrl.processed_len = 0;
    g_dump_upload_ctrl.last_send_time_ms = 0;
    g_dump_upload_ctrl.last_send_timeout_ms = 0;
    return 0;
}

void dump_uploader_wait_done(const dump_exc_log_desc_s *exc_info, void *priv_data)
{
    int ret;
    u32 timeout_ms = dump_upload_get_last_pkt_timeout_ms_left();

    UNUSED(exc_info);
    UNUSED(priv_data);
    if (!g_dump_upload_ctrl.genl_connected || !g_dump_upload_ctrl.server_connected) {
        return;
    }
    ret = dump_send_app_cmd(DUMP_KERNEL_CMD_OP_REPORT_SEND_DATA);
    if (ret < 0) {
        dump_error("notify app report ret %d\n", ret);
    }
    ret = dump_send_app_cmd(DUMP_KERNEL_CMD_OP_DISCONNECT);
    if (ret < 0) {
        dump_error("notify app disconnect ret %d\n", ret);
        return;
    }

    msleep(DUMP_UPLOAD_WAIT_LAST_PKT_DONE_DELTA_MS); /* 等待50ms，主要是等待APP响应REPORT SEND DATA CMD */

    timeout_ms = (timeout_ms > DUMP_UPLOAD_WAIT_LAST_PKT_DONE_TIMEOUT_MS) ? DUMP_UPLOAD_WAIT_LAST_PKT_DONE_TIMEOUT_MS :
                                                                            timeout_ms;
    if (timeout_ms) {
        msleep(timeout_ms);
    }

    dump_print("upload done, total[0x%x], send[0x%x],proc[0x%x]\n", g_dump_upload_ctrl.total_data_len,
               g_dump_upload_ctrl.send_len, g_dump_upload_ctrl.processed_len);
    return;
}

__init void dump_upload_cfg_init(void)
{
    device_node_s *dev = NULL;
    u32 ret;
    u32 data_rate = DUMP_UPLOAD_DEFAULT_DATARATE_PER_MS;
    dev = bsp_dt_find_node_by_path("/dump_upload_cfg");
    if (dev == NULL) {
        dump_error("no upload cfg, use default\n");
        return;
    }
    ret = (u32)bsp_dt_property_read_u32(dev, "datarate", &data_rate);
    if (ret != 0) {
        dump_error("no datarate cfg, use default\n");
        return;
    }
    g_dump_upload_datarate_ms = data_rate;
}

__init int dump_uploader_init(void)
{
    dump_work_s work;
    int ret = dump_genl_init();
    dump_upload_cfg_init();
    dump_upload_only_file_desc_init();
    osl_sem_init(1, &g_dump_upload_ctrl.upload_sem);

    work.work_type = DUMP_WORK_ASYNC;
    work.work_fn = dump_uploader_wait_done;
    work.prior = DUMP_WORK_PRIO_ASYNC_HOOK_COMMON_UPLOADER_WAITDONE;
    work.priv_data = NULL;
    dump_subsys_add_work(DUMP_SUBSYS_COMM, &work);
    
    return ret;
}
