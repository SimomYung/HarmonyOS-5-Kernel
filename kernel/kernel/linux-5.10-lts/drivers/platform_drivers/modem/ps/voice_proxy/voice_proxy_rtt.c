/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
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

#include <linux/miscdevice.h>
#include <linux/ioctl.h>
#include "voice_proxy.h"
#include "securec.h"

#define LOG_TAG "voice_proxy_rtt"
#define VOICE_PROXY_RTT_WAKE_UP_READ  _IO('P',  0x1)
#define VOICE_PROXY_RTT_PERMISSION 0660
#define MAX_RTT_DATA_LEN 500
#define PROXY_RTT_RX_MIN_SIZE sizeof(struct voice_proxy_rtt_rx_ind)
#define PROXY_RTT_RX_MAX_SIZE (PROXY_RTT_RX_MIN_SIZE + MAX_RTT_DATA_LEN)
#define PROXY_RTT_TX_MIN_SIZE sizeof(struct proxy_voice_rtt_tx_ntf)
#define PROXY_RTT_TX_MAX_SIZE (PROXY_RTT_TX_MIN_SIZE + MAX_RTT_DATA_LEN)
#define PROXY_RTT_RX_QUEUE_MAX_SIZE           (20)
#define PROXY_RTT_TX_QUEUE_MAX_SIZE           (20)

#ifndef unused_parameter
#define unused_parameter(x) (void)(x)
#endif

/* this queue is used for saving the data which is sent to user */
LIST_HEAD(rtt_rx_queue);
/* this queue is used for saving the data which is received from user */
LIST_HEAD(rtt_tx_queue);

struct rtt_priv {
    struct mutex read_lock;

    spinlock_t write_lock;

    wait_queue_head_t read_waitq;

    int32_t read_wait_flag;

    /* this is used for counting the size of rtt_rx_queue */
    int32_t rx_cnt;

    /* this is used for counting the size of rtt_tx_queue */
    int32_t tx_cnt;

    struct mutex ioctl_mutex;
};

static struct rtt_priv g_rtt_priv;

static_t void rtt_clear_queue(struct list_head *queue, uint32_t buf_size)
{
    uint32_t cnt = 0;
    struct voice_proxy_data_node *node = NULL;

    while (!list_empty_careful(queue)) {
        cnt++;
        if (cnt > buf_size) {
            AUDIO_LOGE("rtt:clear queue abnormal, cnt is %u", cnt);
            break;
        }

        node = list_first_entry(queue, struct voice_proxy_data_node, list_node);
        list_del_init(&node->list_node);
        kfree(node);
        node = NULL;
    }

    AUDIO_LOGI("rtt:clear queue cnt is %u", cnt);
}

static_t int32_t rtt_add_rx_data(int8_t *rev_buf, uint32_t buf_size)
{
    int32_t ret;
    struct voice_proxy_data_node *node = NULL;

    mutex_lock(&g_rtt_priv.read_lock);
    if (g_rtt_priv.rx_cnt >= PROXY_RTT_RX_QUEUE_MAX_SIZE) {
        g_rtt_priv.read_wait_flag++;
        mutex_unlock(&g_rtt_priv.read_lock);
        wake_up(&g_rtt_priv.read_waitq);
        return -ENOMEM;
    }
    mutex_unlock(&g_rtt_priv.read_lock);

    ret = voice_proxy_create_data_node(&node, rev_buf, (int)buf_size);
    if (ret) {
        AUDIO_LOGE("data_node kzalloc failed");
        return -EFAULT;
    }

    mutex_lock(&g_rtt_priv.read_lock);
    list_add_tail(&node->list_node, &rtt_rx_queue);
    g_rtt_priv.rx_cnt++;
    g_rtt_priv.read_wait_flag++;
    mutex_unlock(&g_rtt_priv.read_lock);
    wake_up(&g_rtt_priv.read_waitq);

    return 0;
}

static_t void rtt_get_tx_data(int8_t *data, uint32_t *size)
{
    struct voice_proxy_data_node *node = NULL;

    spin_lock_bh(&g_rtt_priv.write_lock);
    if (!list_empty_careful(&rtt_tx_queue)) {
        node = list_first_entry(&rtt_tx_queue, struct voice_proxy_data_node, list_node);

        list_del_init(&node->list_node);

        if (g_rtt_priv.tx_cnt > 0) {
            g_rtt_priv.tx_cnt--;
        }

        if (*size < (uint32_t)node->list_data.size) {
            AUDIO_LOGE("invalid size = %d, list_data.size = %d", *size, node->list_data.size);
            kfree(node);
            node = NULL;
            *size = 0;
            spin_unlock_bh(&g_rtt_priv.write_lock);
            return;
        }

        *size = (uint32_t)node->list_data.size;
        (void)memcpy_s(data, (size_t)*size, node->list_data.data, (size_t)*size);

        kfree(node);
        node = NULL;
        spin_unlock_bh(&g_rtt_priv.write_lock);
    } else {
        spin_unlock_bh(&g_rtt_priv.write_lock);
        *size = 0;
    }
}

static_t void rtt_receive_rx_ind(int8_t *rev_buf, uint32_t buf_size)
{
    int32_t ret;

    if (rev_buf == NULL) {
        AUDIO_LOGE("%s fail, param rev_buf is NULL", __FUNCTION__);
        return;
    }

    if ((buf_size <= PROXY_RTT_TX_MIN_SIZE) || (buf_size > PROXY_RTT_TX_MAX_SIZE)) {
        AUDIO_LOGE("rtt_receive_rx_ind invalid buf_size is %d", buf_size);
        return;
    }

    ret = rtt_add_rx_data(rev_buf, buf_size);
    if (ret) {
        AUDIO_LOGE("send rtt rx data to read func failed");
        return;
    }
}

static_t void rtt_handle_tx_ntf(int8_t *data, uint32_t *size, uint16_t *msg_id)
{
    if (data == NULL || size == NULL || msg_id == NULL) {
        AUDIO_LOGE("%s fail, param is NULL", __FUNCTION__);
        return;
    }

    rtt_get_tx_data(data, size);
    *msg_id = ID_PROXY_VOICE_RTT_TX_NTF;
}

static_t bool is_input_para_valid(struct file *file, char *buf, size_t size)
{
    if (file == NULL || buf == NULL) {
        AUDIO_LOGE("file is nul");
        return false;
    }

    if (file->f_flags & O_NONBLOCK) {
        AUDIO_LOGE("file->f_flags & O_NONBLOCK  fail");
        return false;
    }

    if (size <= PROXY_RTT_TX_MIN_SIZE) {
        AUDIO_LOGE("param err, size %zd, little than PROXY_RTT_TX_MIN_SIZE %ld", size, PROXY_RTT_TX_MIN_SIZE);
        return false;
    }

    return true;
}

static_t ssize_t rtt_read(struct file *file, char __user *user_buf, size_t size, loff_t *ppos)
{
    struct voice_proxy_data_node *node = NULL;
    int ret = 0;

    if (!is_input_para_valid(file, user_buf, size)) {
        AUDIO_LOGE("invalid input params");
        return -EINVAL;
    }

    mutex_lock(&g_rtt_priv.read_lock);
    if (list_empty_careful(&rtt_rx_queue)) {
        mutex_unlock(&g_rtt_priv.read_lock);
        ret = wait_event_interruptible(g_rtt_priv.read_waitq, g_rtt_priv.read_wait_flag > 0);
        if (ret) {
            if (ret != -ERESTARTSYS)
                AUDIO_LOGE("wait interruptible fail,0x%x", ret);
            return -EBUSY;
        }
        mutex_lock(&g_rtt_priv.read_lock);
    }

    g_rtt_priv.read_wait_flag = 0;

    if (!list_empty_careful(&rtt_rx_queue)) {
        node = list_first_entry(&rtt_rx_queue, struct voice_proxy_data_node, list_node);

        list_del_init(&node->list_node);
        if (g_rtt_priv.rx_cnt > 0)
            g_rtt_priv.rx_cnt--;

        if (size < node->list_data.size) {
            AUDIO_LOGE("size(%zd), little list_data.size(%d)", size, node->list_data.size);
            kfree(node);
            node = NULL;
            mutex_unlock(&g_rtt_priv.read_lock);
            return -EAGAIN;
        }

        if (copy_to_user(user_buf, node->list_data.data, node->list_data.size)) {
            AUDIO_LOGE("copy_to_user fail");
            ret = -EFAULT;
        } else {
            ret = node->list_data.size;
        }
        kfree(node);
        node = NULL;
        mutex_unlock(&g_rtt_priv.read_lock);
    } else {
        mutex_unlock(&g_rtt_priv.read_lock);
        ret = -EAGAIN;
        AUDIO_LOGE("list is empty, read again");
    }

    return ret;
}

static_t int32_t rtt_add_tx_data(int8_t *data, uint32_t size)
{
    int32_t ret;
    struct voice_proxy_data_node *node = NULL;

    spin_lock_bh(&g_rtt_priv.write_lock);
    if (g_rtt_priv.tx_cnt >= PROXY_RTT_TX_QUEUE_MAX_SIZE) {
        spin_unlock_bh(&g_rtt_priv.write_lock);
        AUDIO_LOGE("out of queue, tx cnt %d greater than MAX %d", g_rtt_priv.tx_cnt, PROXY_RTT_TX_QUEUE_MAX_SIZE);
        return -ENOMEM;
    }
    spin_unlock_bh(&g_rtt_priv.write_lock);

    ret = voice_proxy_create_data_node(&node, data, (int)size);
    if (ret != 0) {
        AUDIO_LOGE("node kzalloc failed");
        return -EFAULT;
    }
    spin_lock_bh(&g_rtt_priv.write_lock);
    list_add_tail(&node->list_node, &rtt_tx_queue);
    g_rtt_priv.tx_cnt++;
    spin_unlock_bh(&g_rtt_priv.write_lock);

    return (int32_t)size;
}

static_t int32_t rtt_dispatch_write_msg(int8_t *data, uint32_t size)
{
    int32_t ret;
    uint16_t msg_id = *((uint16_t*)data);

    switch (msg_id) {
        case ID_PROXY_VOICE_RTT_TX_NTF:
            ret = voice_proxy_add_data(rtt_add_tx_data, data, size, ID_PROXY_VOICE_RTT_TX_NTF);
            break;
        default:
            AUDIO_LOGE("%s fail msgid [0x%x]", __FUNCTION__, msg_id);
            ret = -EFAULT;
            break;
    }
    return ret;
}

static_t ssize_t rtt_write(struct file *filp, const char __user *buff, size_t size, loff_t *offp)
{
    int32_t ret;
    int8_t data[PROXY_RTT_TX_MAX_SIZE] = {0};

    unused_parameter(filp);
    unused_parameter(offp);

    if (!buff) {
        AUDIO_LOGE("rtt:write fail,param buff is NULL");
        return -EPERM;
    }

    if ((size <= PROXY_RTT_TX_MIN_SIZE) || (size > PROXY_RTT_TX_MAX_SIZE)) {
        AUDIO_LOGE("para error, size:%zd greater than max size %d", size, PROXY_RTT_TX_MAX_SIZE);
        return -EINVAL;
    }

    if (copy_from_user(data, buff, size) != 0) {
        AUDIO_LOGE("copy_from_user fail");
        return -EFAULT;
    }

    ret = rtt_dispatch_write_msg(data, size);
    if (ret <= 0) {
        AUDIO_LOGE("call rtt_dispatch_write_msg fail");
        return -EFAULT;
    }

    return (int)size;
}

static_t void rtt_wake_up_read(void)
{
    mutex_lock(&g_rtt_priv.read_lock);
    g_rtt_priv.read_wait_flag++;
    mutex_unlock(&g_rtt_priv.read_lock);
    wake_up(&g_rtt_priv.read_waitq);
}

static_t long rtt_ioctl(struct file *fd, unsigned int cmd, unsigned long arg)
{
    long ret = 0;

    unused_parameter(fd);
    unused_parameter(arg);

    mutex_lock(&g_rtt_priv.ioctl_mutex);
    switch (cmd) {
    case VOICE_PROXY_RTT_WAKE_UP_READ:
        rtt_wake_up_read();
        break;
    default:
        ret = -EINVAL;
        break;
    }
    mutex_unlock(&g_rtt_priv.ioctl_mutex);

    return ret;
}

static_t int rtt_open(struct inode *finode, struct file *fd)
{
    unused_parameter(finode);
    unused_parameter(fd);
    AUDIO_LOGI("%s start", __FUNCTION__);

    spin_lock_bh(&g_rtt_priv.write_lock);
    rtt_clear_queue(&rtt_tx_queue, PROXY_RTT_TX_QUEUE_MAX_SIZE);
    g_rtt_priv.tx_cnt = 0;
    spin_unlock_bh(&g_rtt_priv.write_lock);

    return 0;
}

static_t int rtt_close(struct inode *node, struct file *filp)
{
    unused_parameter(node);
    unused_parameter(filp);

    rtt_wake_up_read();
    return 0;
}

static_t const struct file_operations rtt_misc_fops = {
    .owner = THIS_MODULE,
    .open = rtt_open,
    .read = rtt_read,
    .write = rtt_write,
    .release = rtt_close,
    .unlocked_ioctl = rtt_ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl = rtt_ioctl,
#endif
};

static_t struct miscdevice rtt_misc_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "voice_proxy_rtt",
    .fops = &rtt_misc_fops,
    .mode = VOICE_PROXY_RTT_PERMISSION,
};

static_t int rtt_probe(void)
{
    int32_t ret;
    (void)memset_s(&g_rtt_priv, sizeof(g_rtt_priv), 0, sizeof(g_rtt_priv));
    AUDIO_LOGI("%s start", __FUNCTION__);

    g_rtt_priv.read_wait_flag = 0;
    mutex_init(&g_rtt_priv.read_lock);
    spin_lock_init(&g_rtt_priv.write_lock);
    init_waitqueue_head(&g_rtt_priv.read_waitq);
    mutex_init(&g_rtt_priv.ioctl_mutex);

    ret = misc_register(&rtt_misc_device);
    if (ret != 0) {
        AUDIO_LOGE("rtt misc register fail");
        return ret;
    }

    voice_proxy_register_msg_callback(ID_VOICE_PROXY_RTT_RX_IND, rtt_receive_rx_ind);

    voice_proxy_register_cmd_callback(ID_PROXY_VOICE_RTT_TX_NTF, rtt_handle_tx_ntf);

    return ret;
}

static_t int rtt_remove(void)
{
    AUDIO_LOGI("%s start", __FUNCTION__);

    mutex_destroy(&g_rtt_priv.read_lock);
    mutex_destroy(&g_rtt_priv.ioctl_mutex);
    misc_deregister(&rtt_misc_device);

    voice_proxy_deregister_msg_callback(ID_VOICE_PROXY_RTT_RX_IND);

    voice_proxy_deregister_cmd_callback(ID_PROXY_VOICE_RTT_TX_NTF);

    return 0;
}

int rtt_init(void)
{
    int32_t ret;
    AUDIO_LOGI("%s start", __FUNCTION__);

    ret = rtt_probe();
    if (ret) {
        AUDIO_LOGE("voice proxy rtt driver register fail %d", ret);
    }

    return ret;
}

static_t void rtt_exit(void)
{
    AUDIO_LOGI("%s start", __FUNCTION__);
    rtt_remove();
}

#if !IS_ENABLED(CONFIG_MODEM_DRIVER)
module_init(rtt_init);
module_exit(rtt_exit);
#endif
