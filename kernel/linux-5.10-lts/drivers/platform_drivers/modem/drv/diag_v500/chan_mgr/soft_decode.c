/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
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
#include <linux/module.h>
#include <securec.h>
#include <osl_sem.h>
#include <osl_thread.h>
#include <osl_malloc.h>
#include <bsp_rfile.h>
#include <bsp_dump.h>
#include <bsp_slice.h>
#include <bsp_omp.h>
#include "ppm.h"
#include "decode_algorithm.h"
#include "soft_decode.h"
#include "msg_chan.h"
#include "scm_dst.h"
#include "diag_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

soft_decode_info_s g_soft_decode_info;

static void soft_decode_rb_flush(om_ring_s *ring_buffer)
{
    unsigned long lock_level;

    spin_lock_irqsave(&g_soft_decode_info.spinlock, lock_level);
    ring_buffer->write_ptr = 0;
    ring_buffer->read_ptr = 0;
    spin_unlock_irqrestore(&g_soft_decode_info.spinlock, lock_level);
}

static void soft_decode_get_rb_data_size(om_ring_s *ring_buffer, rw_buffer_s *rw_buff)
{
    if (ring_buffer->read_ptr <= ring_buffer->write_ptr) {
        /* 写指针大于读指针，直接计算 */
        rw_buff->buffer = ring_buffer->buf + ring_buffer->read_ptr;
        rw_buff->size = ring_buffer->write_ptr - ring_buffer->read_ptr;
        rw_buff->rb_buffer = NULL;
        rw_buff->rb_size = 0;
    } else {
        /* 读指针大于写指针，需要考虑回卷 */
        rw_buff->buffer = ring_buffer->buf + ring_buffer->read_ptr;
        rw_buff->size = ring_buffer->buf_size - ring_buffer->read_ptr;
        rw_buff->rb_buffer = ring_buffer->buf;
        rw_buff->rb_size = ring_buffer->write_ptr;
    }
}

static int soft_decode_get_data_from_rb(om_ring_s *ring_buffer, rw_buffer_s rw_buff, char *buffer, unsigned int data_len)
{
    int ret;

    if (data_len == 0) {
        return 0;
    }

    ret = memcpy_s(buffer, data_len, rw_buff.buffer, rw_buff.size);
    if (ret) {
        diag_err("memcpy_s fail, ret=0x%x\n", ret);
        return ret;
    }

    if (rw_buff.rb_size != 0) {
        ret = memcpy_s(buffer + rw_buff.size, data_len - rw_buff.size, rw_buff.rb_buffer, data_len - rw_buff.size);
        if (ret) {
            diag_err("memcpy_s fail, ret=0x%x\n", ret);
            return ret;
        }

        ring_buffer->read_ptr = data_len - rw_buff.size;
    } else {
        ring_buffer->read_ptr += data_len;
    }

    return 0;
}

static void soft_decode_get_rb_idle_size(om_ring_s *ring_buffer, rw_buffer_s *rw_buff)
{
    if (ring_buffer->write_ptr < ring_buffer->read_ptr) {
        /* 读指针大于写指针，直接计算 */
        rw_buff->buffer = ring_buffer->buf + ring_buffer->write_ptr;
        rw_buff->size = (ring_buffer->read_ptr - ring_buffer->write_ptr - 1);
        rw_buff->rb_buffer = NULL;
        rw_buff->rb_size = 0;
    } else {
        /* 写指针大于读指针，需要考虑回卷 */
        if (ring_buffer->read_ptr != 0) {
            rw_buff->buffer = ring_buffer->buf + ring_buffer->write_ptr;
            rw_buff->size = ring_buffer->buf_size - ring_buffer->write_ptr;
            rw_buff->rb_buffer = ring_buffer->buf;
            rw_buff->rb_size = ring_buffer->read_ptr - 1;
        } else {
            rw_buff->buffer = ring_buffer->buf + ring_buffer->write_ptr;
            rw_buff->size = ring_buffer->buf_size - ring_buffer->write_ptr - 1;
            rw_buff->rb_buffer = NULL;
            rw_buff->rb_size = 0;
        }
    }
}

static int soft_decode_put_data_to_rb(om_ring_s *ring_buffer, rw_buffer_s rw_buffer, const unsigned char *buffer, int data_len)
{
    unsigned int size, rb_size;
    int ret;

    if (data_len == 0) {
        return 0;
    }

    if (rw_buffer.size > (unsigned int)data_len) {
        if ((rw_buffer.buffer) && (rw_buffer.size)) {
            ret = memcpy_s(((unsigned char *)rw_buffer.buffer), rw_buffer.size, buffer, data_len);
            if (ret != EOK) {
                diag_err("memory copy fail 0x%x\n", ret);
                return ret;
            }
        }

        ring_buffer->write_ptr += data_len;
    } else {
        if ((rw_buffer.buffer) && (rw_buffer.size)) {
            size = rw_buffer.size;
            ret = memcpy_s(((unsigned char *)rw_buffer.buffer), rw_buffer.size, buffer, size);
            if (ret != EOK) {
                diag_err("memory copy fail 0x%x\n", ret);
                return ret;
            }
        } else {
            size = 0;
        }

        rb_size = data_len - rw_buffer.size;
        if (rb_size && rw_buffer.rb_buffer != NULL) {
            ret = memcpy_s((unsigned char *)rw_buffer.rb_buffer, rw_buffer.rb_size, ((unsigned char *)buffer + size), rb_size);
            if (ret != EOK) {
                diag_err("memory copy fail 0x%x\n", ret);
                return ret;
            }
        }

        ring_buffer->write_ptr = rb_size;
    }

    return 0;
}

static void soft_decode_data_dispatch(om_hdlc_s *hdlc_ctrl)
{
    // get port type
    if (g_soft_decode_info.dst_data_proc_cb != NULL) {
        g_soft_decode_info.dst_data_proc_cb(DIAG_PORT_TYPE_CODER, hdlc_ctrl->decap_buff + sizeof(char),
            hdlc_ctrl->info_len - sizeof(char));
    }
    return;
}

static int soft_decode_data_recv(const unsigned char *buffer, unsigned int len)
{
    rw_buffer_s rw_buffer;
    int ret;
    soft_decode_info_s *sd_info = &g_soft_decode_info;

    soft_decode_get_rb_idle_size(sd_info->rcv_ctrl.ring_buffer, &rw_buffer);
    if ((rw_buffer.size + rw_buffer.rb_size) < len) {
        sd_info->debug_info.rb_info.buff_no_enough++;
        return BSP_ERROR;
    }

    ret = soft_decode_put_data_to_rb(sd_info->rcv_ctrl.ring_buffer, rw_buffer, buffer, len);
    if (ret) {
        diag_err("send to ringbuffer fail:0x%x\n", ret);
        sd_info->debug_info.rb_info.rbuff_put_err++;
        return ret;
    }

    g_diag_dbg.diag_msp_proc_dbg.dl_cnf_coder_up_task++;
    osl_sem_up(&(sd_info->rcv_ctrl.sem_id));
    return BSP_OK;
}

int diag_hdlc_decode_data(unsigned char *buffer, unsigned int len)
{
    int ret;
    unsigned long lock_level;
    soft_decode_info_s *sd_info = &g_soft_decode_info;

    spin_lock_irqsave(&sd_info->spinlock, lock_level);
    ret = soft_decode_data_recv(buffer, len);
    spin_unlock_irqrestore(&sd_info->spinlock, lock_level);
    return ret;
}

static void soft_decode_send_to_hdlc_dec(om_hdlc_s *hdlc_ctrl, const unsigned char *data, unsigned int len)
{
    int ret;
    unsigned int i;
    unsigned char tmp_char;
    soft_decode_info_s *sd_info = container_of(hdlc_ctrl, soft_decode_info_s, hdlc_entity);

    for (i = 0; i < len; i++) {
        tmp_char = (unsigned char)data[i];
        ret = om_hdlc_decap(hdlc_ctrl, tmp_char);
        if (ret == HDLC_SUCC) {
            sd_info->debug_info.hdlc_decap_data.data_len += hdlc_ctrl->info_len;
            sd_info->debug_info.hdlc_decap_data.num++;
            soft_decode_data_dispatch(hdlc_ctrl);
        } else if (ret == HDLC_NOT_HDLC_FRAME) {
            /* 不是完整分帧,继续HDLC解封装 */
            continue;
        } else {
            sd_info->debug_info.frame_decap_data++;
        }
    }

    return;
}

static unsigned int soft_decode_hdlc_init(om_hdlc_s *hdlc_ctrl)
{
    /* 申请用于HDLC解封装的缓存 */
    hdlc_ctrl->decap_buff = (unsigned char *)osl_malloc(HDLC_BUFFER_SIZE);
    if (hdlc_ctrl->decap_buff == NULL) {
        diag_err("decap_buff malloc fail!\n");
        return BSP_ERROR;
    }

    hdlc_ctrl->decap_buff_size = HDLC_BUFFER_SIZE;

    /* 初始化HDLC解封装控制上下文 */
    om_hdlc_init(hdlc_ctrl);

    return BSP_OK;
}

static int soft_decode_recv_task(void *para)
{
    int ret;
    unsigned int len;
    rw_buffer_s rw_buffer;
    soft_decode_info_s *sd_info = &g_soft_decode_info;
    unsigned char *temp_buff;

    for (;;) {
        osl_sem_down(&(sd_info->rcv_ctrl.sem_id));

        g_diag_dbg.diag_msp_proc_dbg.dl_cnf_coder_task_begin++;
        soft_decode_get_rb_data_size(sd_info->rcv_ctrl.ring_buffer, &rw_buffer);
        if ((rw_buffer.size + rw_buffer.rb_size) == 0) {
            g_diag_dbg.diag_msp_proc_dbg.dl_cnf_coder_task_buff_empty++;
            continue;
        }
        len = rw_buffer.size + rw_buffer.rb_size;
        temp_buff = (unsigned char *)osl_malloc(len);
        if (temp_buff == NULL) {
            diag_err("temp_buff malloc fail, len=0x%x\n", len);
            continue;
        }

        ret = soft_decode_get_data_from_rb(sd_info->rcv_ctrl.ring_buffer, rw_buffer,
            temp_buff, len);
        if (ret) {
            diag_err("get data from ringbuffer fail:0x%x\n", ret);
            soft_decode_rb_flush(sd_info->rcv_ctrl.ring_buffer);
            sd_info->debug_info.rb_info.rbuff_flush++;
            osl_free(temp_buff);
            continue;
        }
        sd_info->debug_info.get_info.data_len += len;

        /* 调用HDLC解封装函数 */
        soft_decode_send_to_hdlc_dec(&sd_info->hdlc_entity, temp_buff, len);
        osl_free(temp_buff);
    }

    return 0;
}

int soft_decode_init(void)
{
    int ret;
    OSL_TASK_ID task_id;
    soft_decode_info_s *sd_info = &g_soft_decode_info;

    spin_lock_init(&sd_info->spinlock);

    sd_info->rcv_ctrl.ring_buffer = (om_ring_s *)osl_malloc(sizeof(om_ring_s));
    if (sd_info->rcv_ctrl.ring_buffer == NULL) {
        diag_err("ring_buffer ctrl malloc fail\n");
        return BSP_ERROR;
    }
    soft_decode_rb_flush(sd_info->rcv_ctrl.ring_buffer);

    sd_info->rcv_ctrl.ring_buffer->buf = (char *)osl_malloc(RING_BUFFER_SIZE);
    if (sd_info->rcv_ctrl.ring_buffer->buf == NULL) {
        diag_err("ring_buffer malloc fail\n");
        return BSP_ERROR;        
    }
    sd_info->rcv_ctrl.ring_buffer->buf_size = RING_BUFFER_SIZE;

    osl_sem_init(0, &(sd_info->rcv_ctrl.sem_id));

    (void)memset_s(&(sd_info->debug_info), sizeof(sd_info->debug_info), 0, sizeof(sd_info->debug_info));

    if (soft_decode_hdlc_init(&sd_info->hdlc_entity) != 0) {
        diag_err("HDLC Init Fail.\n");
        sd_info->debug_info.hdlc_init_err++;
        goto err;
    }
   
    /* 注册OM配置数据接收自处理任务 */
    ret = (unsigned int)osl_task_init("soft_dec", SOFT_DECODE_TASK_PRIORITY, SOFT_DECODE_TASK_STACK_SIZE,
        (OSL_TASK_FUNC)soft_decode_recv_task, (void *)sd_info, &task_id);
    if (ret) {
        diag_err("soft_decode_recv_task init Fail.\n");
        goto err;
    }

    diag_crit("[init]soft decode init ok\n");
    return BSP_OK;

err:
    osl_free(sd_info->rcv_ctrl.ring_buffer->buf);
    osl_free(sd_info->rcv_ctrl.ring_buffer);
    return BSP_ERROR;
}

void diag_coder_proc_register(soft_decode_dst_cb func)
{
    if (g_soft_decode_info.dst_data_proc_cb == NULL) {
        g_soft_decode_info.dst_data_proc_cb = func;
    }

    return;
}

void diag_coder_data_rcv_handler(unsigned char *data, unsigned int len)
{
    int ret;

    ret = diag_hdlc_decode_data(data, len);
    if (ret) {
        g_diag_dbg.diag_msp_proc_dbg.dl_cnf_coder_decode_fail++;
        diag_err("decode recv data fail\n");
    }
}

int diag_cnf_coder_chan_reset(void)
{
    scm_dst_chan_reset(SCM_CNF_DST_CHAN);
    return 0;
}

int diag_coder_disconn_handler(ppm_chan_e chn, ppmstate_e stat)
{
    int ret;

    if (stat != PPM_STAT_DISCONNECT) {
        diag_debug("stat:0x%x err\n", stat);
        return 0;
    }

    switch (chn) {
        case PPM_CNF_CODER_CHAN:
            ret = diag_cnf_coder_chan_reset();
            break;
        case PPM_IND_CODER_CHAN:
            ret = diag_trigger_disconn(DIAG_TOOL_TYPE_DIAGTOOL);
            break;
        case PPM_DEBUGGING_CODER_CHAN:
            ret = diag_trigger_disconn(DIAG_TOOL_TYPE_MXT);
            break;
        default:
            ret = DIAG_PPM_CHAN_TYPE_ERR;
            diag_err("diag disconn event chan:0x%x err\n", chn);
    }

    if (ret) {
        diag_err("diag disconn handle fail:0x%x\n", ret);
    }

    return ret;
}

int diag_coder_chan_init(void)
{
    int ret;

    ret = ppm_register_recv_cb(PPM_CNF_CODER_CHAN, diag_coder_data_rcv_handler);
    if (ret) {
        diag_err("diag regist coder ppm chan recv fail, ret:0x%x\n", ret);
        return ret;
    }

    ret = ppm_register_event_cb(PPM_CNF_CODER_CHAN, diag_coder_disconn_handler);
    if (ret) {
        diag_err("diag regist cnf coder ppm chan event fail, ret:0x%x\n", ret);
    }

    ret = ppm_register_event_cb(PPM_IND_CODER_CHAN, diag_coder_disconn_handler);
    if (ret) {
        diag_err("diag regist ind coder ppm chan event fail, ret:0x%x\n", ret);
    }

    ret = ppm_register_recv_cb(PPM_DEBUGGING_CODER_CHAN, diag_coder_data_rcv_handler);
    if (ret) {
        diag_err("diag regist coder ppm chan recv fail, ret:0x%x\n", ret);
        return ret;
    }

    ret = ppm_register_event_cb(PPM_DEBUGGING_CODER_CHAN, diag_coder_disconn_handler);
    if (ret) {
        diag_err("diag regist debugging ppm chan event fail, ret:0x%x\n", ret);
    }

    ret = soft_decode_init();
    if (ret) {
        diag_err("soft decode init fail, ret:0x%x\n", ret);
        return ret;
    }

    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
