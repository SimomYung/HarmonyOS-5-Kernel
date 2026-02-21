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
#include "tf_agent.h"

#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/string.h>
#include <linux/of.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include "voice_proxy_log.h"
#include "voice_proxy_volte.h"
#include "voice_proxy.h"
#include "securec.h"

#ifndef UNUSED
#define UNUSED(x) ((void)(x))
#endif

#define TF_AGENT_NAME "tf_agent"
#define VOICE_PACKET_SIZE_MAX 256
#define TF_AGENT_PERMISSION 0660

struct tf_agent {
    int packet_size_max;

    char read_packet[VOICE_PACKET_SIZE_MAX];
    char write_packet[VOICE_PACKET_SIZE_MAX];

    struct mutex read_lock;
    struct mutex write_lock;
};

static_t struct tf_agent *tf_agent_priv;
static_t bool tf_agent_support_flag = true;
static_t ssize_t tf_agent_read(struct file *file, char __user *buf,
            size_t count, loff_t *pos)
{
    struct tf_agent *priv = tf_agent_priv;
    int proxy_read;
    int ret;

    UNUSED(pos);
    if ((file == NULL) || (buf == NULL) || (priv == NULL)) {
        AUDIO_LOGE("%s: file, buf or priv is NULL\n", __func__);
        return -EINVAL;
    }

    mutex_lock(&priv->read_lock);
    if ((count <= 0) || (count > priv->packet_size_max)) {
        AUDIO_LOGE("%s: invalid count %d,%d\n", __func__,
            (int)count, priv->packet_size_max);
        ret = -EINVAL;
        goto err_out;
    }

    if (file->f_flags & O_NONBLOCK) {
        AUDIO_LOGI("%s: not block, skip\n", __func__);
        ret = -EAGAIN;
        goto err_out;
    }

    (void)memset_s(priv->read_packet, sizeof(priv->read_packet), 0, sizeof(priv->read_packet));
    AUDIO_LOGD("%s: read_packet len %ld\n", __func__,
        sizeof(priv->read_packet));
    // receive from voice proxy
    proxy_read = proxy_pull_data((int8_t *)priv->read_packet,
        (int32_t)count);
    if (proxy_read < 0) {
        AUDIO_LOGI("%s: read proxy FIFO failed %d\n",
            __func__, proxy_read);
        ret = -EFAULT;
        goto err_out;
    }

    if (copy_to_user(buf, priv->read_packet, count)) {
        AUDIO_LOGE("%s: copy_to_user failed\n", __func__);
        ret = -EFAULT;
        goto err_out;
    }

    AUDIO_LOGD("%s: read success %d,%d\n", __func__,
        (int)count, proxy_read);
    ret = proxy_read;

err_out:
    mutex_unlock(&priv->read_lock);
    return ret;
}

static_t ssize_t tf_agent_write(struct file *file, const char __user *buf,
            size_t count, loff_t *pos)
{
    struct tf_agent *priv = tf_agent_priv;
    int ret;

    UNUSED(file);
    UNUSED(pos);
    if ((buf == NULL) || (priv == NULL)) {
        AUDIO_LOGE("%s: buf or priv is NULL\n", __func__);
        return -EINVAL;
    }

    mutex_lock(&priv->write_lock);
    if ((count <= 0) || (count > priv->packet_size_max)) {
        AUDIO_LOGE("%s: invalid count %d,%d\n", __func__,
            (int)count, priv->packet_size_max);
        ret = -EINVAL;
        goto err_out;
    }

    (void)memset_s(priv->write_packet, sizeof(priv->write_packet), 0, sizeof(priv->write_packet));
    AUDIO_LOGD("%s: write_packet len %ld\n", __func__,
        sizeof(priv->write_packet));
    if (copy_from_user(priv->write_packet, buf, count)) {
        AUDIO_LOGE("%s: copy_from_user failed\n", __func__);
        ret = -EFAULT;
        goto err_out;
    }

    // send to voice proxy
    ret = proxy_push_data((void *)priv->write_packet);
    if (ret < 0) {
        AUDIO_LOGE("%s: write proxy FIFO failed %d\n", __func__, ret);
        ret = -EFAULT;
        goto err_out;
    }

    AUDIO_LOGD("%s: write success %d,%d\n", __func__,
        (int)count, ret);
    ret = count;

err_out:
    mutex_unlock(&priv->write_lock);
    return ret;
}

static_t long tf_agent_ioctl(struct file *file, unsigned int cmd,
            unsigned long arg)
{
    long ret = 0;

    UNUSED(file);
    UNUSED(arg);
    AUDIO_LOGD("%s: cmd 0x%x begin\n", __func__, cmd);
    switch (cmd) {
    case CLOSE_READ:
        proxy_wake_up_pull();
        break;
    case SEC_KEY_NEG_START:
        ret = proxy_enable_sec_key_negotiation(0); // enable
        break;
    case SEC_KEY_NEG_END_SUCCESS:
        ret = proxy_enable_sec_key_negotiation(1); // succ_disable
        break;
    case SEC_KEY_NEG_END_FAIL:
        ret = proxy_enable_sec_key_negotiation(2); // fail_disable
        break;
    default:
        AUDIO_LOGI("%s: invalid cmd 0x%x\n", __func__, cmd);
        return -EINVAL;
    }

    AUDIO_LOGD("%s: cmd 0x%x end %ld\n", __func__, cmd, ret);
    return ret;
}

static_t int tf_agent_close(struct inode *node, struct file *file)
{
    UNUSED(node);
    UNUSED(file);
    return 0;
}

static_t const struct file_operations tf_agent_fops = {
    .owner = THIS_MODULE,
    .open = simple_open,
    .read = tf_agent_read,
    .write = tf_agent_write,
    .unlocked_ioctl = tf_agent_ioctl,
#ifdef CONFIG_COMPAT
    .compat_ioctl = tf_agent_ioctl,
#endif
    .release = tf_agent_close,
};

static_t struct miscdevice tf_agent_miscdev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = TF_AGENT_NAME,
    .fops = &tf_agent_fops,
    .mode = TF_AGENT_PERMISSION,
};

static_t int tf_agent_probe(struct platform_device *pdev)
{
    struct tf_agent *priv = NULL;
    int size;
    int ret;

    AUDIO_LOGI("%s: enter\n", __func__);
    if (pdev == NULL) {
        AUDIO_LOGE("%s: pdev is NULL\n", __func__);
        return -EINVAL;
    }

    priv = kzalloc(sizeof(*priv), GFP_KERNEL);
    if (priv == NULL)
        return -ENOMEM;

    priv->packet_size_max = VOICE_PACKET_SIZE_MAX;
    if (!of_property_read_u32(pdev->dev.of_node,
        "voice_packet_size_max", &size)) {
        AUDIO_LOGI("get packet size success %d\n", size);
        if (size < VOICE_PACKET_SIZE_MAX)
            priv->packet_size_max = size;
    } else {
        AUDIO_LOGI("get packet size failed, use default\n");
    }

    ret = misc_register(&tf_agent_miscdev);
    if (ret) {
        AUDIO_LOGE("%s: misc register failed %d", __func__, ret);
        goto err_out;
    }

    mutex_init(&priv->read_lock);
    mutex_init(&priv->write_lock);

    tf_agent_priv = priv;
    AUDIO_LOGD("%s: success\n", __func__);
    return 0;

err_out:
    if (priv != NULL)
        kfree(priv);
    return ret;
}

static_t int tf_agent_remove(struct platform_device *pdev)
{
    UNUSED(pdev);
    kfree(tf_agent_priv);
    tf_agent_priv = NULL;

    misc_deregister(&tf_agent_miscdev);
    return 0;
}

static_t const struct of_device_id tf_agent_match[] = {
    { .compatible = "huawei,tf_agent", },
    {},
};
MODULE_DEVICE_TABLE(of, tf_agent_match);

static_t struct platform_driver tf_agent_driver = {
    .driver = {
        .name = TF_AGENT_NAME,
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(tf_agent_match),
    },
    .probe = tf_agent_probe,
    .remove = tf_agent_remove,
};

int tf_agent_init(void)
{
    if (!tf_agent_support_flag) {
        AUDIO_LOGI("%s: tf_agent not supported", __func__);
        return 0;
    }
    AUDIO_LOGI("%s: tf_agent supported", __func__);

    return platform_driver_register(&tf_agent_driver);
}

static_t void tf_agent_exit(void)
{
    platform_driver_unregister(&tf_agent_driver);
}
#if !IS_ENABLED(CONFIG_MODEM_DRIVER)
module_init(tf_agent_init);
module_exit(tf_agent_exit);
#endif
