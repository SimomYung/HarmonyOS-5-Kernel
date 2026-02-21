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

#include "voice_proxy.h"

#include <linux/of_platform.h>
#include <linux/kthread.h>

#include "securec.h"
#include "voice_proxy_fusion.h"
#include "mdrv_msg.h"
#include "msg_id.h"

#define LOG_TAG "voice_proxy"
#define MESSAGE_CALLBACKS_SIZE 20
#define COMMAND_CALLBACKS_SIZE 20
#define SIGN_INIT_CALLBACKS_SIZE 20

/* receive message from modem the size of msg_id (bytes) */
#define VOICE_PROXY_MSG_ID_SIZE (4)

#define DTS_COMP_VOICE_PROXY_NAME "hisilicon,voice_proxy"

#ifndef UNUSED_PARAMETER
#define UNUSED_PARAMETER(x) (void)(x)
#endif

struct modem_msg_para {
    uint16_t msg_id;
};

/* handle the reply message from modem,record msg_id and data */
struct voice_proxy_rev_msg_buf {
    int8_t *msg_buf; /* received data buff from msglite */
    int32_t msg_buf_len;
    int32_t msg_id; /* the first 4 bytes is msg_id in the received data from msglite */
    int8_t *out_buf_ptr; /* pointer to the behind of cmd_id */
    int32_t out_buf_len;
};

/*
 * this queue is used for telling the write thread that the type
 * of new message
 */
LIST_HEAD(proxy_command_queue);

/* the message callback is used for handling message from modem */
static_t struct voice_proxy_msg_handle message_callbacks[MESSAGE_CALLBACKS_SIZE];

/* the command callback is used for get data from data queue by command */
static_t struct voice_proxy_cmd_handle command_callbacks[COMMAND_CALLBACKS_SIZE];

/* the sign init callback is used for init write sign */
static_t struct voice_proxy_sign_init sign_init_callbacks[SIGN_INIT_CALLBACKS_SIZE];

struct voice_proxy_priv {
    struct device *dev;
    /* this lock is used for handling the queue of proxy_command_queue */
    spinlock_t command_lock;
    wait_queue_head_t command_waitq;
    int32_t command_wait_flag;
    struct task_struct *write_thread;

    struct msg_chn_hdl *msgchn_hdl;
};

static struct voice_proxy_priv priv;

void voice_proxy_register_msg_callback(uint16_t msg_id, voice_proxy_msg_cb callback)
{
    int32_t i;

    if (!callback) {
        AUDIO_LOGE("register_msg_callback fail, param callback is NULL");
        return;
    }

    for (i = 0; i < MESSAGE_CALLBACKS_SIZE; i++) {
        if (!message_callbacks[i].msg_id) {
            message_callbacks[i].msg_id = msg_id;
            message_callbacks[i].callback = callback;
            return;
        }
    }

    AUDIO_LOGE("register_msg_callback fail, message_callbacks is full");
}

void voice_proxy_deregister_msg_callback(uint16_t msg_id)
{
    int32_t i;

    for (i = 0; i < MESSAGE_CALLBACKS_SIZE; i++) {
        if (message_callbacks[i].msg_id == msg_id) {
            message_callbacks[i].msg_id = 0;
            message_callbacks[i].callback = 0;
            return;
        }
    }

    AUDIO_LOGE("deregister_msg_callback fail, msg_id is invalid");
}

void voice_proxy_register_cmd_callback(uint16_t msg_id, voice_proxy_cmd_cb callback)
{
    int32_t i;

    if (!callback) {
        AUDIO_LOGE("register_cmd_callback fail, param callback is NULL");
        return;
    }

    for (i = 0; i < COMMAND_CALLBACKS_SIZE; i++) {
        if (!command_callbacks[i].msg_id) {
            command_callbacks[i].msg_id = msg_id;
            command_callbacks[i].callback = callback;
            return;
        }
    }

    AUDIO_LOGE("register_cmd_callback fail, command_callbacks is full");
}

void voice_proxy_deregister_cmd_callback(uint16_t msg_id)
{
    int32_t i;

    for (i = 0; i < COMMAND_CALLBACKS_SIZE; i++) {
        if (command_callbacks[i].msg_id == msg_id) {
            command_callbacks[i].msg_id = 0;
            command_callbacks[i].callback = 0;
            return;
        }
    }

    AUDIO_LOGE("deregister_cmd_callback fail, invalid msg_id");
}

void voice_proxy_register_sign_init_callback(voice_proxy_sign_init_cb callback)
{
    int32_t i = 0;

    if (!callback) {
        AUDIO_LOGE("register_sign_init_callback fail, param callback is NULL");
        return;
    }

    for (i = 0; i < SIGN_INIT_CALLBACKS_SIZE; i++) {
        if (!sign_init_callbacks[i].callback) {
            sign_init_callbacks[i].callback = callback;
            return;
        }
    }

    AUDIO_LOGE("register_sign_init_callback fail, sign_init_callbacks is full");
}

void voice_proxy_deregister_sign_init_callback(voice_proxy_sign_init_cb callback)
{
    int32_t i = 0;

    if (!callback) {
        AUDIO_LOGE("deregister_sign_init_callback, param callback is NULL");
        return;
    }

    for (i = 0; i < SIGN_INIT_CALLBACKS_SIZE; i++) {
        if (sign_init_callbacks[i].callback == callback) {
            sign_init_callbacks[i].callback = NULL;
            return;
        }
    }

    AUDIO_LOGE("deregister_sign_init_callback fail, sign_init_callbacks is full");
}

static_t void write_sign_init(void)
{
    int32_t i;

    for (i = 0; i < SIGN_INIT_CALLBACKS_SIZE; i++) {
        if (sign_init_callbacks[i].callback)
            sign_init_callbacks[i].callback();
    }
}

int32_t voice_proxy_create_data_node(struct voice_proxy_data_node **node,
    int8_t *data, int32_t size)
{
    struct voice_proxy_data_node *n = NULL;

    if (node == NULL || data == NULL) {
        AUDIO_LOGE("input parameter invalid");
        return -EINVAL;
    }

    n = kzalloc(sizeof(*n) + size, GFP_ATOMIC);
    if (n == NULL) {
        AUDIO_LOGE("kzalloc failed");
        return -ENOMEM;
    }

    if (memcpy_s(n->list_data.data, (4 + size), data, size)) {   /* 4: n->list_data.data fix size */
        AUDIO_LOGE("memcpy_s failed");
        kfree(n);
        return -EFAULT;
    }
    n->list_data.size = size;
    *node = n;

    return 0;
}

int32_t voice_proxy_add_data(voice_proxy_add_data_cb callback, int8_t *data, uint32_t size, uint16_t msg_id)
{
    int32_t ret;
    struct voice_proxy_cmd_node *command = NULL;

    if (!callback || !data) {
        AUDIO_LOGE("proxy_add_data fail, param is NULL");
        return -EINVAL;
    }

    command = kzalloc(sizeof(*command), GFP_ATOMIC);
    if (command == NULL) {
        AUDIO_LOGE("kzalloc failed");
        return -ENOMEM;
    }

    command->msg_id = msg_id;

    spin_lock_bh(&priv.command_lock);
    ret = callback(data, size);
    if (ret < 0) {
        spin_unlock_bh(&priv.command_lock);
        kfree(command);
        return ret;
    }

    list_add_tail(&command->list_node, &proxy_command_queue);
    priv.command_wait_flag++;
    spin_unlock_bh(&priv.command_lock);
    wake_up(&priv.command_waitq);

    return ret;
}

/*
 * this function will get data from callback which register by other module,
 * the size is in&out param, as a in param,it show the size of data,
 * as a out param, it show the substantial data size which get from callback.
 */
static_t int32_t write_thread_get_data(int8_t *data, uint32_t *size, uint16_t *msg_id)
{
    int32_t ret = 0;
    int32_t i;
    struct voice_proxy_cmd_node *command = NULL;

    if (!data || !size || !msg_id) {
        AUDIO_LOGE("input param is NULL");
        return -EINVAL;
    }

    if (list_empty_careful(&proxy_command_queue)) {
        AUDIO_LOGE("proxy_command_queue is empty");
        return -EINVAL;
    }

    command = list_first_entry(&proxy_command_queue, struct voice_proxy_cmd_node, list_node);

    for (i = 0; i < COMMAND_CALLBACKS_SIZE; i++) {
        if ((command_callbacks[i].msg_id == command->msg_id) && (command_callbacks[i].callback != NULL)) {
            command_callbacks[i].callback(data, size, msg_id);
            break;
        }
    }

    if (i == COMMAND_CALLBACKS_SIZE) {
        AUDIO_LOGE("invalid msg id:0x%x", command->msg_id);
        ret = -EINVAL;
    }

    if (*size > VOICE_PROXY_LIMIT_PARAM_SIZE) {
        AUDIO_LOGE("data size error, size(%d)>(%d)", *size, VOICE_PROXY_LIMIT_PARAM_SIZE);
        ret = -EINVAL;
    }

    list_del_init(&command->list_node);
    kfree(command);

    return ret;
}

/*
 * send the voice data to modem
 */
static_t int voice_proxy_write_thread(void *arg)
{
    int32_t ret = 0;
    uint16_t msg_id = 0;
    uint32_t size = VOICE_PROXY_LIMIT_PARAM_SIZE;
    int8_t *data = NULL;

    UNUSED_PARAMETER(arg);

    data = kzalloc((size_t)size, GFP_ATOMIC);
    if (!data) {
        AUDIO_LOGE("kzalloc failed");
        return -ENOMEM;
    }

    write_sign_init();

    while (!kthread_should_stop()) {
        ret = wait_event_interruptible(priv.command_waitq, priv.command_wait_flag != 0);
        if (ret) {
            if (ret != -ERESTARTSYS)
                AUDIO_LOGE("wait event interruptible fail, 0x%x", ret);
            else
                AUDIO_LOGE("wait event failed,ERESTARTSYS");
            continue;
        }

        spin_lock_bh(&priv.command_lock);
        priv.command_wait_flag = 0;

        while (!list_empty_careful(&proxy_command_queue)) {
            size = VOICE_PROXY_LIMIT_PARAM_SIZE;

            ret = write_thread_get_data(data, &size, &msg_id);

            spin_unlock_bh(&priv.command_lock);

            if ((ret) || (!size)) {
                spin_lock_bh(&priv.command_lock);
                continue;
            }

            ret = voice_proxy_send_msg(data, size);
            if (ret)
                AUDIO_LOGE("voice_proxy_send_msg fail, ret:%d", ret);

            spin_lock_bh(&priv.command_lock);
        }
        spin_unlock_bh(&priv.command_lock);
    }

    kfree(data);
    return 0;
}

static_t void destory_thread(void)
{
    if (!IS_ERR(priv.write_thread)) {
        kthread_stop(priv.write_thread);
        spin_lock_bh(&priv.command_lock);
        priv.command_wait_flag++;
        spin_unlock_bh(&priv.command_lock);
        wake_up(&priv.command_waitq);
        priv.write_thread = NULL;
    }
}

static_t int32_t voice_proxy_create_thread(void)
{
    int32_t ret = 0;

    priv.write_thread = kthread_run(voice_proxy_write_thread, NULL, "voice proxy write");

    if (IS_ERR(priv.write_thread)) {
        AUDIO_LOGE("call kthread_run fail");
        ret = -EBUSY;
    }

    return ret;
}

static_t bool voice_proxy_is_msg_invalid(const struct msg_addr *src, void *data, uint32_t len)
{
    if (unlikely(src == NULL)) {
        AUDIO_LOGE("src null");
        return true;
    }

    if (unlikely(data == NULL)) {
        AUDIO_LOGE("msg null");
        return true;
    }

    if (len < VOICE_PROXY_MSG_ID_SIZE || len > VOICE_PROXY_LIMIT_PARAM_SIZE) {
        AUDIO_LOGE("param error, msg len: %d", len);
        return true;
    }

    return false;
}

static_t int voice_proxy_rcv_msg(const struct msg_addr *src, void *data, unsigned len)
{
    int32_t i = 0;
    int ret = 0;
    uint16_t msg_id = 0;
    struct voice_proxy_rev_msg_buf *rev_msg = NULL;
    struct modem_msg_para *msg_para = NULL;

    AUDIO_LOGH("entry");

    if (voice_proxy_is_msg_invalid(src, data, len)) {
        ret = -EINVAL;
        goto EXIT;
    }

    rev_msg = kzalloc(sizeof(*rev_msg), GFP_ATOMIC);
    if (rev_msg == NULL) {
        AUDIO_LOGE("rev_msg kzalloc failed");
        ret = -ENOMEM;
        goto EXIT;
    }

    rev_msg->msg_buf_len = (int)len;
    rev_msg->msg_buf = kzalloc((size_t)VOICE_PROXY_LIMIT_PARAM_SIZE, GFP_ATOMIC);
    if (rev_msg->msg_buf == NULL) {
        AUDIO_LOGE("malloc msg_buf failed");
        ret = -ENOMEM;
        goto EXIT;
    }

    if (memcpy_s(rev_msg->msg_buf, len, data, len)) {
        AUDIO_LOGE("memcpy_s msg fail");
        ret = -EINVAL;
        goto EXIT;
    }

    msg_para = (struct modem_msg_para *)rev_msg->msg_buf;
    msg_id = msg_para->msg_id;

    for (i = 0; i < MESSAGE_CALLBACKS_SIZE; i++) {
        if ((message_callbacks[i].msg_id == msg_id) && (message_callbacks[i].callback != NULL)) {
            message_callbacks[i].callback((int8_t *)rev_msg->msg_buf, (uint32_t)rev_msg->msg_buf_len);
            break;
        }
    }

    if (i == MESSAGE_CALLBACKS_SIZE)
        AUDIO_LOGE("callback fail, msg_id is invalid");

EXIT:
    if (rev_msg) {
        if (rev_msg->msg_buf) {
            kfree(rev_msg->msg_buf);
            rev_msg->msg_buf = NULL;
        }
        kfree(rev_msg);
    }
    return ret;
}

int voice_proxy_send_msg(int8_t *buf, uint32_t len)
{
    struct msg_addr dst = {
        .core = MSG_CORE_TSP,
        .chnid = MSG_CHN_VOICE_PROXY,
    };

    AUDIO_LOGH("entry");

    if (mdrv_msgchn_lite_sendto(priv.msgchn_hdl, &dst, buf, len) != 0) {
        AUDIO_LOGE("mdrv_msgchn_lite_sendto fail");
        return -EINVAL;
    }

    return 0;
}

static_t int voice_proxy_open_msgchn(void)
{
    struct msgchn_attr attr = {0};

    mdrv_msgchn_attr_init(&attr);
    attr.chnid = MSG_CHN_VOICE_PROXY;
    attr.coremask = MSG_CORE_MASK(MSG_CORE_TSP);
    attr.lite_notify = voice_proxy_rcv_msg;

    priv.msgchn_hdl = mdrv_msgchn_lite_open(&attr);
    if (priv.msgchn_hdl == NULL) {
        AUDIO_LOGE("mdrv_msgchn_lite_open fail");
        return -EINVAL;
    }

    return 0;
}

static_t int voice_proxy_probe(struct platform_device *pdev)
{
    int32_t ret = 0;

    (void)memset_s(&priv, sizeof(priv), 0, sizeof(priv));

    priv.dev = &pdev->dev;
    priv.command_wait_flag = 0;

    spin_lock_init(&priv.command_lock);
    init_waitqueue_head(&priv.command_waitq);
    AUDIO_LOGI("voice proxy prob, pdev name[%s]", pdev->name);

    ret = voice_proxy_open_msgchn();
    if (ret) {
        AUDIO_LOGE("msgchn open fail");
        return ret;
    }

    ret = voice_proxy_create_thread();
    if (ret) {
        AUDIO_LOGE("voice_proxy_create_thread fail");
        return ret;
    }

    of_platform_populate(pdev->dev.of_node, NULL, NULL, &pdev->dev);

    return ret;
}

static_t int voice_proxy_remove(struct platform_device *pdev)
{
    UNUSED_PARAMETER(pdev);

    destory_thread();

    return 0;
}

static_t const struct of_device_id modem_voice_proxy_match_table[] = {
    {
        .compatible = DTS_COMP_VOICE_PROXY_NAME,
        .data = NULL,
    },
    {
    }
};

static_t struct platform_driver voice_proxy_driver = {
    .driver = {
        .name  = "voice proxy",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(modem_voice_proxy_match_table),
    },
    .probe = voice_proxy_probe,
    .remove = voice_proxy_remove,
};

int voice_proxy_init(void)
{
    int32_t ret;

    ret = platform_driver_register(&voice_proxy_driver);
    if (ret)
        AUDIO_LOGE("voice proxy driver register fail,ERROR is %d", ret);

    return ret;
}

static_t void voice_proxy_exit(void)
{
    platform_driver_unregister(&voice_proxy_driver);
}
#if !IS_ENABLED(CONFIG_MODEM_DRIVER)
module_init(voice_proxy_init);
module_exit(voice_proxy_exit);
#endif
