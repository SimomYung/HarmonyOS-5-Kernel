/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
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

#ifndef VOICE_PROXY_H
#define VOICE_PROXY_H

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/proc_fs.h>
#include <linux/semaphore.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/unistd.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/list.h>
#include <linux/wait.h>
#ifdef CONFIG_COMPAT
#include <linux/compat.h>
#endif
#include <linux/of.h>

#include "voice_proxy_interface.h"
#include "voice_proxy_log.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The size limit for the in and out parameters of read/write */
#define VOICE_PROXY_LIMIT_PARAM_SIZE (1500)
/* QUEUE_SIZE_MUST_GREATER_THAN_SECRET_KEY_NEGOTIATION_SIZE(500) */
#define VOICE_PROXY_QUEUE_SIZE_MAX 600

#if (defined(LLT_OS_VER))
#define static_t
#else
#define static_t static
#endif

enum send_tfagent_data_type {
    VOLTE_NONE,
    PROXY_TO_TF_UNDECRYPT_DATA,
    PROXY_TO_TF_UNENCRYPT_DATA,
    TF_TO_PROXY_DECRYPTED_DATA,
    TF_TO_PROXY_ENCRYPTED_DATA,
    VOLTE_MAX,
};

enum voice_modem_no {
    VOICE_MC_MODEM0 = 0,
    VOICE_MC_MODEM1,
    VOICE_MC_MODEM_NUM_BUTT
};

struct send_tfagent_data {
    int32_t data_type;
    uint32_t id;
    int32_t buf_size;
    int16_t data[PROXY_VOICE_CODEC_MAX_DATA_LEN];
};

struct proxy_voice_cnf_cmd_code {
    struct list_head list_node;
    uint16_t msg_id;
    uint16_t modem_no;
    uint32_t channel_id;
};

struct voice_proxy_cmd_node {
    struct list_head list_node;
    uint16_t msg_id;
    uint16_t modem_no;
};

struct voice_proxy_data_buf {
    uint32_t id;
    int32_t size;
    int8_t data[4];
};

struct voice_proxy_data_node {
    struct list_head list_node;
    struct voice_proxy_data_buf list_data;
};

typedef void (*voice_proxy_sign_init_cb)(void);
typedef void (*voice_proxy_msg_cb)(int8_t *rev_buf, uint32_t buf_size);
typedef void (*voice_proxy_cmd_cb)(int8_t *data, uint32_t *size, uint16_t *msg_id);

typedef int32_t (*voice_proxy_add_data_cb)(int8_t *data, uint32_t size);

struct voice_proxy_sign_init {
    voice_proxy_sign_init_cb callback;
};

struct voice_proxy_msg_handle {
    uint16_t msg_id;
    voice_proxy_msg_cb callback;
};

struct voice_proxy_cmd_handle {
    uint16_t msg_id;
    voice_proxy_cmd_cb callback;
};

int32_t voice_proxy_create_data_node(struct voice_proxy_data_node **node, int8_t *data, int32_t size);
void voice_proxy_register_msg_callback(uint16_t msg_id, voice_proxy_msg_cb callback);
void voice_proxy_deregister_msg_callback(uint16_t msg_id);
void voice_proxy_register_cmd_callback(uint16_t msg_id, voice_proxy_cmd_cb callback);
void voice_proxy_deregister_cmd_callback(uint16_t msg_id);
void voice_proxy_register_sign_init_callback(voice_proxy_sign_init_cb cb);
void voice_proxy_deregister_sign_init_callback(voice_proxy_sign_init_cb cb);
int32_t voice_proxy_add_data(voice_proxy_add_data_cb callback, int8_t *data, uint32_t size, uint16_t msg_id);
int voice_proxy_send_msg(int8_t *buf, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* end of voice_proxy.h */
