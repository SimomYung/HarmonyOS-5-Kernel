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
#include <linux/miscdevice.h>
#include "securec.h"

#define LOG_TAG "voice_proxy_volte"
#define VOICE_PROXY_VOLTE_DATA_COUNT_MAX 100000000
#define DTS_COMP_VOICE_PROXY_VOLTE_NAME "hisilicon,voice_proxy_volte"
#define VOICE_PROXY_VOLTE_QUEUE_SIZE_MAX 5000 /* ensure negotiation succuss during long interval */

#define CODEC_AMR 0
#define CODEC_AMRWB 5
#define CODEC_EVS 9
#define EVS_PRIMARY_FRAME_TYPE_24400 6
#define EVS_PRIMARY_FRAME_TYPE_SID 12

#ifndef UNUSED_PARAMETER
#define UNUSED_PARAMETER(x) (void)(x)
#endif

enum {
    SEC_KEY_NEGO_ENABLE,
    SEC_KEY_NEGO_SUCC_DISABLE,
    SEC_KEY_NEGO_FAIL_DISABLE,
};

/*
 * this queue is used for saving the struct of voice_proxy_lte_rx_notify when
 * the voice data is sent to tfagent. when the tfagent return the decrypted
 * voice data, proxy will get the struct of voice_proxy_lte_rx_notify and push
 * the decrypted voice data to it, finally send the this struct to modem.
 */
LIST_HEAD(decrypting_queue);
LIST_HEAD(decrypted_queue);

/*
 * this queue is used for saving the struct of voice_proxy_lte_tx_notify when the
 * voice data is sent to tfagent. when the tfagent return the encrypted voice data,
 * proxy will get the struct of voice_proxy_lte_tx_notify and push the encrypted
 * voice data to it, finally send the this struct to modem.
 */
LIST_HEAD(encrypting_queue);
LIST_HEAD(encrypted_queue);

/* this queue is used for saving the data which is sent to tfagent */
LIST_HEAD(pull_queue);

struct volte_priv {
    /* it is used for handling the queue of decrypting_queue/encrypting_queue */
    spinlock_t push_lock;

    /* they are used for sending data to tfagent */
    spinlock_t pull_lock;
    wait_queue_head_t pull_waitq;
    int32_t pull_wait_flag;

    /* this is used for counting the size of decrypting_queue */
    int32_t decrypting_cnt;

    /* this is used for counting the size of decrypted_queue */
    int32_t decrypted_cnt;

    /* this is used for counting the size of encrypting_queue */
    int32_t encrypting_cnt;

    /* this is used for counting the size of encrypted_queue */
    int32_t encrypted_cnt;

    uint32_t rx_cnt;
    uint32_t tx_cnt;
};

static struct volte_priv priv;
static_t const short amrnb_frame_length[] = {
    12, 13, 15, 17, 19, 20, 26, 31, 5, 0, 0, 0, 0, 0, 0, 0
};

static_t const short amrwb_frame_length[] = {
    17, 23, 32, 36, 40, 46, 50, 58, 60, 5, 0, 0, 0, 0, 0, 0
};
/* 80~320速率暂不支持 */
static_t const short evs_frame_length[] = {
    7, 18, 20, 24, 33, 41, 61, 80, 120, 160, 240, 320, 6, 0, 0, 0
};

static_t void volte_sign_init(void)
{
    priv.rx_cnt = 0;
    priv.tx_cnt = 0;
}

static_t int32_t volte_add_decrypted_data(int8_t *data, uint32_t size)
{
    struct voice_proxy_data_node *node = NULL;

    UNUSED_PARAMETER(size);

    if (priv.decrypted_cnt > VOICE_PROXY_VOLTE_QUEUE_SIZE_MAX) {
        AUDIO_LOGE("out of queue, decrypted_cnt(%d) > QUEUE_SIZE_MAX(%d)", priv.decrypted_cnt, VOICE_PROXY_VOLTE_QUEUE_SIZE_MAX);
        return -ENOMEM;
    }

    node = (struct voice_proxy_data_node *)data;

    list_add_tail(&node->list_node, &decrypted_queue);
    priv.decrypted_cnt++;

    return 0;
}

static_t int32_t volte_add_encrypted_data(int8_t *data, uint32_t size)
{
    struct voice_proxy_data_node *node = NULL;

    UNUSED_PARAMETER(size);

    if (priv.encrypted_cnt > VOICE_PROXY_VOLTE_QUEUE_SIZE_MAX) {
        AUDIO_LOGE("out of queue, encrypted_cnt(%d) > QUEUE_SIZE_MAX(%d)", priv.encrypted_cnt, VOICE_PROXY_VOLTE_QUEUE_SIZE_MAX);
        return -ENOMEM;
    }

    node = (struct voice_proxy_data_node *)data;

    list_add_tail(&node->list_node, &encrypted_queue);
    priv.encrypted_cnt++;

    return 0;
}

static_t int32_t volte_send_data(struct voice_proxy_data_node *node, struct send_tfagent_data *buf)
{
    int32_t ret = 0;
    struct voice_proxy_lte_rx_notify *rx = NULL;
    struct voice_proxy_lte_tx_notify *tx = NULL;

    /* fill handled voice buf to the packege of msglite */
    if (buf->data_type == TF_TO_PROXY_DECRYPTED_DATA) {
        rx = (struct voice_proxy_lte_rx_notify *)node->list_data.data;
        (void)memset_s(rx->data, sizeof(rx->data), 0, sizeof(rx->data));
        (void)memcpy_s((int8_t *)rx->data, PROXY_VOICE_CODEC_MAX_DATA_LEN * sizeof(uint16_t), buf->data,
            PROXY_VOICE_CODEC_MAX_DATA_LEN * sizeof(uint16_t));
        rx->msg_id = ID_PROXY_VOICE_LTE_RX_NTF;
        node->list_data.size = sizeof(*rx) + rx->para_len;

        ret = voice_proxy_add_data(volte_add_decrypted_data, (int8_t *)node,
            (unsigned int)sizeof(*node), ID_PROXY_VOICE_LTE_RX_NTF);
    } else {
        tx = (struct voice_proxy_lte_tx_notify *)node->list_data.data;
        (void)memset_s(tx->data, sizeof(tx->data), 0, sizeof(tx->data));
        (void)memcpy_s((int8_t *)tx->data, PROXY_VOICE_CODEC_MAX_DATA_LEN * sizeof(uint16_t), buf->data,
            PROXY_VOICE_CODEC_MAX_DATA_LEN * sizeof(uint16_t));
        tx->msg_id = ID_PROXY_VOICE_LTE_TX_NTF;
        node->list_data.size = sizeof(*tx) + tx->para_len;

        ret = voice_proxy_add_data(volte_add_encrypted_data, (int8_t *)node,
            (unsigned int)sizeof(*node), ID_PROXY_VOICE_LTE_TX_NTF);
    }

    if (ret) {
        AUDIO_LOGE("send node fail, ret:%d", ret);
        kfree(node);
    }

    return ret;
}

/*
 * this function is called by tfagent kernel to return the voice data which
 * have encrypted/decrypted.
 */
int32_t proxy_push_data(void *data)
{
    struct send_tfagent_data *buf = NULL;
    struct voice_proxy_data_node *node = NULL;

    if (!data) {
        AUDIO_LOGE("param data is NULL");
        return -EINVAL;
    }

    buf = (struct send_tfagent_data *)data;
    while (1) {
        spin_lock_bh(&priv.push_lock);
        node = NULL;
        if (buf->data_type == TF_TO_PROXY_DECRYPTED_DATA) {
            if (!list_empty_careful(&decrypting_queue)) {
                node = list_first_entry(&decrypting_queue, struct voice_proxy_data_node, list_node);

                list_del_init(&node->list_node);
                priv.decrypting_cnt--;
            }
        } else if (buf->data_type == TF_TO_PROXY_ENCRYPTED_DATA) {
            if (!list_empty_careful(&encrypting_queue)) {
                node = list_first_entry(&encrypting_queue, struct voice_proxy_data_node, list_node);

                list_del_init(&node->list_node);
                priv.encrypting_cnt--;
            }
        } else {
            AUDIO_LOGE("data type is error, data_type:%d", buf->data_type);
            spin_unlock_bh(&priv.push_lock);
            return -EINVAL;
        }
        spin_unlock_bh(&priv.push_lock);

        if (node) {
            if (node->list_data.id < buf->id) {
                AUDIO_LOGE("ignore this node");
                kfree(node);
                node = NULL;
            } else if (node->list_data.id == buf->id) {
                volte_send_data(node, buf);
                break;
            } else {
                AUDIO_LOGE("no avail node");
                kfree(node);
                return -EINVAL;
            }
        }  else {
            AUDIO_LOGE("node is null");
            break;
        }
    }

    return 0;
}

/*
 * this function is called by tfagent kernel to get voice data for
 * encrypt/decrypt
 */
int32_t proxy_pull_data(int8_t *data, int32_t size)
{
    int32_t ret = 0;
    struct send_tfagent_data buf;
    struct voice_proxy_data_node *node = NULL;

    if (data == NULL) {
        AUDIO_LOGE("data is null");
        return -EINVAL;
    }

    if (size < sizeof(buf)) {
        AUDIO_LOGE("param error,size(%d)<buf size(%ld)", size, sizeof(buf));
        return -EINVAL;
    }

    (void)memset_s(&buf, sizeof(buf), 0, sizeof(buf));

    spin_lock_bh(&priv.pull_lock);
    if (list_empty_careful(&pull_queue)) {
        spin_unlock_bh(&priv.pull_lock);
        ret = wait_event_interruptible(priv.pull_waitq,
            priv.pull_wait_flag > 0);
        if (ret) {
            if (ret != -ERESTARTSYS)
                AUDIO_LOGE("wait event interruptible fail, 0x%x", ret);

            return -EBUSY;
        }
        spin_lock_bh(&priv.pull_lock);
    }

    priv.pull_wait_flag = 0;

    if (!list_empty_careful(&pull_queue)) {
        node = list_first_entry(&pull_queue, struct voice_proxy_data_node, list_node);

        list_del_init(&node->list_node);

        spin_unlock_bh(&priv.pull_lock);

        if (node->list_data.size <= size) {
            (void)memcpy_s(data, size, node->list_data.data, node->list_data.size);
            ret = node->list_data.size;
        } else {
            AUDIO_LOGE("data size err, data size(%d)>size(%d)", node->list_data.size, size);
            ret = -EFAULT;
        }

        kfree(node);
    } else {
        spin_unlock_bh(&priv.pull_lock);
        ret = -EAGAIN;
    }

    return ret;
}

void proxy_wake_up_pull(void)
{
    spin_lock_bh(&priv.pull_lock);
    priv.pull_wait_flag++;
    spin_unlock_bh(&priv.pull_lock);
    wake_up(&priv.pull_waitq);
}

int32_t proxy_enable_sec_key_negotiation(int32_t enable)
{
    int32_t ret;
    struct voice_proxy_voice_encrypt_key_end msg = {0};
    struct voice_proxy_data_node *node = NULL;

    if (enable != SEC_KEY_NEGO_ENABLE &&
        enable != SEC_KEY_NEGO_SUCC_DISABLE &&
        enable != SEC_KEY_NEGO_FAIL_DISABLE) {
        AUDIO_LOGE("param error, enable:%d", enable);
        return -EINVAL;
    }

    if (enable == SEC_KEY_NEGO_ENABLE) {
        msg.msg_id = ID_PROXY_VOICE_ENCRYPT_KEY_BEGIN;
    } else if (enable == SEC_KEY_NEGO_SUCC_DISABLE) {
        msg.msg_id = ID_PROXY_VOICE_ENCRYPT_KEY_END;
        msg.encrypt_negotiation_result = true;
    } else {
        msg.msg_id = ID_PROXY_VOICE_ENCRYPT_KEY_END;
        msg.encrypt_negotiation_result = false;
        AUDIO_LOGE("negotiation fail, free node");

        spin_lock_bh(&priv.push_lock);
        while (!list_empty_careful(&decrypting_queue)) {
            node = list_first_entry(&decrypting_queue,
                struct voice_proxy_data_node, list_node);
            list_del_init(&node->list_node);
            priv.decrypting_cnt--;
            kfree(node);
            node = NULL;
        }

        while (!list_empty_careful(&encrypting_queue)) {
            node = list_first_entry(&encrypting_queue,
                struct voice_proxy_data_node, list_node);
            list_del_init(&node->list_node);
            priv.encrypting_cnt--;
            kfree(node);
            node = NULL;
        }
        spin_unlock_bh(&priv.push_lock);
    }

    ret = voice_proxy_send_msg((int8_t *)&msg, (unsigned int)sizeof(msg));
    if (ret)
        AUDIO_LOGE("voice_proxy_send_msg fail:%d", ret);

    return ret;
}

static_t bool is_queue_range_valid(int32_t type)
{
    if (type == PROXY_TO_TF_UNDECRYPT_DATA) {
        if (priv.decrypting_cnt > VOICE_PROXY_VOLTE_QUEUE_SIZE_MAX) {
            AUDIO_LOGE("out of queue, decrypting_cnt(%d)>(%d)",
                priv.decrypting_cnt, VOICE_PROXY_VOLTE_QUEUE_SIZE_MAX);
            return false;
        }
    } else {
        if (priv.encrypting_cnt > VOICE_PROXY_VOLTE_QUEUE_SIZE_MAX) {
            AUDIO_LOGE("out of queue, encrypting_cnt(%d)>(%d)",
                priv.encrypting_cnt, VOICE_PROXY_VOLTE_QUEUE_SIZE_MAX);
            return false;
        }
    }

    return true;
}

static_t void add_data_to_queue(struct voice_proxy_data_node *node, int32_t type)
{
    spin_lock_bh(&priv.push_lock);
    if (type == PROXY_TO_TF_UNDECRYPT_DATA) {
        list_add_tail(&node->list_node, &decrypting_queue);
        priv.decrypting_cnt++;
    } else {
        list_add_tail(&node->list_node, &encrypting_queue);
        priv.encrypting_cnt++;
    }
    spin_unlock_bh(&priv.push_lock);
}

static_t void send_data_to_tfagent(struct voice_proxy_data_node *tf_node)
{
    spin_lock_bh(&priv.pull_lock);
    list_add_tail(&tf_node->list_node, &pull_queue);
    priv.pull_wait_flag++;
    spin_unlock_bh(&priv.pull_lock);
    wake_up(&priv.pull_waitq);

    if (priv.rx_cnt >= VOICE_PROXY_VOLTE_DATA_COUNT_MAX)
        priv.rx_cnt = 0;

    if (priv.tx_cnt >= VOICE_PROXY_VOLTE_DATA_COUNT_MAX)
        priv.tx_cnt = 0;
}

static_t int get_voice_data_len(uint16_t codec_type, uint16_t frame_type)
{
    int data_len = 0;
    if (codec_type == CODEC_AMR) {
        data_len = amrnb_frame_length[frame_type];
    } else if (codec_type == CODEC_AMRWB) {
        data_len = amrwb_frame_length[frame_type];
    } else if (codec_type == CODEC_EVS) {
        data_len = evs_frame_length[frame_type];
    }
    return data_len;
}

static_t int get_voice_data(int8_t *rev_buf, struct send_tfagent_data *buf, struct voice_proxy_data_node *node)
{
    struct voice_proxy_lte_rx_notify *rx = NULL;
    struct voice_proxy_lte_tx_notify *tx = NULL;

    if (buf->data_type == PROXY_TO_TF_UNDECRYPT_DATA) {
        rx = (struct voice_proxy_lte_rx_notify *)rev_buf;
        buf->buf_size = get_voice_data_len(rx->codec_type, rx->frame_type);
        if (buf->buf_size != 0) {
            (void)memcpy_s(buf->data, buf->buf_size, (int8_t *)rx->data, buf->buf_size);
        }
        node->list_data.id = priv.rx_cnt;
        buf->id = node->list_data.id;
        priv.rx_cnt++;
    } else {
        tx = (struct voice_proxy_lte_tx_notify *)rev_buf;
        buf->buf_size = get_voice_data_len(tx->codec_type, tx->frame_type);
        if (buf->buf_size != 0) {
            (void)memcpy_s(buf->data, buf->buf_size, (int8_t *)tx->data, buf->buf_size);
        }
        node->list_data.id = priv.tx_cnt;
        buf->id = node->list_data.id;
        priv.tx_cnt++;
    }

    return 0;
}

static_t int32_t volte_add_pull_data(int8_t *rev_buf, uint32_t buf_size, int32_t type)
{
    int32_t ret;
    struct send_tfagent_data buf;
    struct voice_proxy_data_node *node = NULL;
    struct voice_proxy_data_node *tf_node = NULL;

    if (!is_queue_range_valid(type))
        goto OUT;

    ret = voice_proxy_create_data_node(&node, rev_buf, (int)buf_size);
    if (ret) {
        AUDIO_LOGE("kzalloc node failed %d", ret);
        return ret;
    }

    buf.data_type = type;
    (void)memset_s(buf.data, sizeof(buf.data), 0, sizeof(buf.data));

    /*
     * 1.get the voice data from the structure of
     * voice_proxy_lte_rx_notify/voice_proxy_lte_tx_notify
     */
    ret = get_voice_data(rev_buf, &buf, node);
    if (ret)
        goto OUT;

    ret = voice_proxy_create_data_node(&tf_node, (int8_t *)&buf, (int)sizeof(buf));
    if (ret) {
        AUDIO_LOGE("kzalloc push_node failed %d", ret);
        kfree(node);
        goto OUT;
    }

    /*
     * 2.save the structure for decrypted/encrypted voice
     * data and then send the structure to modem
     */
    add_data_to_queue(node, type);

    /* 3.send voice data to tfagent for decrypting/encrypting */
    send_data_to_tfagent(tf_node);

    return ret;

OUT:
    spin_lock_bh(&priv.pull_lock);
    priv.pull_wait_flag++;
    spin_unlock_bh(&priv.pull_lock);
    wake_up(&priv.pull_waitq);

    return -ENOMEM;
}

static_t void volte_receive_undecrypt_ntf(int8_t *rev_buf, uint32_t buf_size)
{
    int32_t ret;

    if (!rev_buf) {
        AUDIO_LOGE("receive_undecrypt_ntf fail, param rev_buf is NULL");
        return;
    }

    struct voice_proxy_lte_rx_notify *rx = (struct voice_proxy_lte_rx_notify *)rev_buf;
    if ((buf_size < sizeof(struct voice_proxy_lte_rx_notify)) ||
        ((buf_size - sizeof(struct voice_proxy_lte_rx_notify)) < rx->para_len)) {
        AUDIO_LOGE("volte_receive_undecrypt_ntf invalid buf_size");
        return;
    }

    if ((rx->codec_type != CODEC_AMR) && (rx->codec_type != CODEC_AMRWB) && (rx->codec_type != CODEC_EVS)) {
        AUDIO_LOGE("volte_receive_undecrypt_ntf invalid codec_type %d", rx->codec_type);
        return;
    }

    if ((rx->codec_type == CODEC_AMR) && (rx->frame_type >= ARRAY_SIZE(amrnb_frame_length))) {
        AUDIO_LOGE("volte_receive_undecrypt_ntf codec_type %d", rx->codec_type);
        AUDIO_LOGE("volte_receive_undecrypt_ntf invalid frame_type %d", rx->frame_type);
        return;
    }

    if ((rx->codec_type == CODEC_AMRWB) && (rx->frame_type >= ARRAY_SIZE(amrwb_frame_length))) {
        AUDIO_LOGE("volte_receive_undecrypt_ntf codec_type %d", rx->codec_type);
        AUDIO_LOGE("volte_receive_undecrypt_ntf invalid frame_type %d", rx->frame_type);
        return;
    }

    if ((rx->codec_type == CODEC_EVS) &&
        (((rx->frame_type > EVS_PRIMARY_FRAME_TYPE_24400) && (rx->frame_type < EVS_PRIMARY_FRAME_TYPE_SID)) ||
         (rx->frame_type >= ARRAY_SIZE(evs_frame_length)))) {
        AUDIO_LOGE("volte_receive_undecrypt_ntf codec_type %d", rx->codec_type);
        AUDIO_LOGE("volte_receive_undecrypt_ntf invalid frame_type %d", rx->frame_type);
        return;
    }

    AUDIO_LOGE("volte_receive_undecrypt_ntf");

    ret = volte_add_pull_data(rev_buf, buf_size, PROXY_TO_TF_UNDECRYPT_DATA);
    if (ret) {
        AUDIO_LOGE("volte_add_pull_data failed");
        return;
    }
}

static_t void volte_receive_unencrypt_ntf(int8_t *rev_buf, uint32_t buf_size)
{
    int32_t ret;

    if (!rev_buf) {
        AUDIO_LOGE("receive_unencrypt_ntf fail, param rev_buf is NULL");
        return;
    }

    struct voice_proxy_lte_tx_notify *tx = (struct voice_proxy_lte_tx_notify *)rev_buf;
    if ((buf_size < sizeof(struct voice_proxy_lte_tx_notify)) ||
        ((buf_size - sizeof(struct voice_proxy_lte_tx_notify)) < tx->para_len)) {
        AUDIO_LOGE("volte_receive_unencrypt_ntf invalid buf_size");
        return;
    }

    if ((tx->codec_type != CODEC_AMR) && (tx->codec_type != CODEC_AMRWB) && (tx->codec_type != CODEC_EVS)) {
        AUDIO_LOGE("volte_receive_unencrypt_ntf invalid codec_type %d", tx->codec_type);
        return;
    }

    if ((tx->codec_type == CODEC_AMR) && (tx->frame_type >= ARRAY_SIZE(amrnb_frame_length))) {
        AUDIO_LOGE("volte_receive_unencrypt_ntf codec_type %d", tx->codec_type);
        AUDIO_LOGE("volte_receive_unencrypt_ntf invalid frame_type %d", tx->frame_type);
        return;
    }

    if ((tx->codec_type == CODEC_AMRWB) && (tx->frame_type >= ARRAY_SIZE(amrwb_frame_length))) {
        AUDIO_LOGE("volte_receive_unencrypt_ntf codec_type %d", tx->codec_type);
        AUDIO_LOGE("volte_receive_unencrypt_ntf invalid frame_type %d", tx->frame_type);
        return;
    }

    if ((tx->codec_type == CODEC_EVS) &&
        (((tx->frame_type > EVS_PRIMARY_FRAME_TYPE_24400) && (tx->frame_type < EVS_PRIMARY_FRAME_TYPE_SID)) ||
         (tx->frame_type >= ARRAY_SIZE(evs_frame_length)))) {
        AUDIO_LOGE("volte_receive_unencrypt_ntf codec_type %d", tx->codec_type);
        AUDIO_LOGE("volte_receive_unencrypt_ntf invalid frame_type %d", tx->frame_type);
        return;
    }

    ret = volte_add_pull_data(rev_buf, buf_size, PROXY_TO_TF_UNENCRYPT_DATA);
    if (ret) {
        AUDIO_LOGE("volte_add_pull_data failed");
        return;
    }
}

static_t void volte_get_decryped_data(int8_t *data, uint32_t *size)
{
    struct voice_proxy_data_node *node = NULL;

    if (!list_empty_careful(&decrypted_queue)) {
        node = list_first_entry(&decrypted_queue, struct voice_proxy_data_node, list_node);

        if (*size < (uint32_t)node->list_data.size) {
            AUDIO_LOGE("Size is invalid, size = %d, list_data.size = %d", *size, node->list_data.size);
            return;
        }

        *size = (uint32_t)node->list_data.size;
        (void)memcpy_s(data, (size_t)*size, node->list_data.data, (size_t)*size);

        list_del_init(&node->list_node);
        kfree(node);

        priv.decrypted_cnt--;
    } else {
        *size = 0;
    }
}

static_t void volte_get_encrypted_data(int8_t *data, uint32_t *size)
{
    struct voice_proxy_data_node *node = NULL;

    if (!list_empty_careful(&encrypted_queue)) {
        node = list_first_entry(&encrypted_queue, struct voice_proxy_data_node, list_node);

        if (*size < (uint32_t)node->list_data.size) {
            AUDIO_LOGE("Size is invalid, size = %d, list_data.size = %d", *size, node->list_data.size);
            return;
        }

        *size = (uint32_t)node->list_data.size;
        (void)memcpy_s(data, (size_t)*size, node->list_data.data, (size_t)*size);

        list_del_init(&node->list_node);
        kfree(node);

        priv.encrypted_cnt--;
    } else {
        *size = 0;
    }
}


static_t void volte_handle_decrypted_ntf(int8_t *data, uint32_t *size, uint16_t *msg_id)
{
    if (!data || !size || !msg_id) {
        AUDIO_LOGE("handle_decrypted_ntf fail, param is NULL");
        return;
    }

    volte_get_decryped_data(data, size);
    *msg_id = ID_PROXY_VOICE_LTE_RX_NTF;
}

static_t void volte_handle_encrypted_ntf(int8_t *data, uint32_t *size, uint16_t *msg_id)
{
    if (!data || !size || !msg_id) {
        AUDIO_LOGE("handle_encrypted_ntf fail, param is NULL");
        return;
    }

    volte_get_encrypted_data(data, size);
    *msg_id = ID_PROXY_VOICE_LTE_TX_NTF;
}

static_t int volte_probe(struct platform_device *pdev)
{
    int32_t ret = 0;

    (void)memset_s(&priv, sizeof(priv), 0, sizeof(priv));

    AUDIO_LOGI("voice proxy volte prob,pdev name[%s]", pdev->name);

    priv.pull_wait_flag = 0;

    spin_lock_init(&priv.push_lock);
    spin_lock_init(&priv.pull_lock);
    init_waitqueue_head(&priv.pull_waitq);

    volte_sign_init();

    voice_proxy_register_msg_callback(ID_VOICE_PROXY_LTE_RX_NTF, volte_receive_undecrypt_ntf);
    voice_proxy_register_msg_callback(ID_VOICE_PROXY_LTE_TX_NTF, volte_receive_unencrypt_ntf);
    voice_proxy_register_cmd_callback(ID_PROXY_VOICE_LTE_RX_NTF, volte_handle_decrypted_ntf);
    voice_proxy_register_cmd_callback(ID_PROXY_VOICE_LTE_TX_NTF, volte_handle_encrypted_ntf);
    voice_proxy_register_sign_init_callback(volte_sign_init);

    return ret;
}

static_t int volte_remove(struct platform_device *pdev)
{

    UNUSED_PARAMETER(pdev);

    voice_proxy_deregister_msg_callback(ID_VOICE_PROXY_LTE_RX_NTF);
    voice_proxy_deregister_msg_callback(ID_VOICE_PROXY_LTE_TX_NTF);

    voice_proxy_deregister_cmd_callback(ID_PROXY_VOICE_LTE_RX_NTF);
    voice_proxy_deregister_cmd_callback(ID_PROXY_VOICE_LTE_TX_NTF);

    voice_proxy_deregister_sign_init_callback(volte_sign_init);

    return 0;
}

static_t const struct of_device_id volte_match_table[] = {
    {
        .compatible = DTS_COMP_VOICE_PROXY_VOLTE_NAME,
        .data = NULL,
    },
    {
    }
};

static_t struct platform_driver volte_driver = {
    .driver = {
        .name  = "voice proxy volte",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(volte_match_table),
    },
    .probe = volte_probe,
    .remove = volte_remove,
};

int volte_init(void)
{
    int32_t ret;

    AUDIO_LOGI("Audio:voice proxy volte init");

    ret = platform_driver_register(&volte_driver);
    if (ret)
        AUDIO_LOGE("voice proxy volte driver register fail,ERROR is %d", ret);

    return ret;
}

static_t void volte_exit(void)
{
    platform_driver_unregister(&volte_driver);
}
#if !IS_ENABLED(CONFIG_MODEM_DRIVER)
module_init(volte_init);
module_exit(volte_exit);
#endif