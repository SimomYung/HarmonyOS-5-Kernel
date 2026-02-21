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
#include <product_config.h>
#include <linux/dma-mapping.h>
#include <osl_types.h>
#include <osl_sem.h>
#include <osl_thread.h>
#include <mdrv_diag.h>
#include <bsp_version.h>
#include <bsp_sec_call.h>
#include <bsp_odt.h>
#include <bsp_dump.h>
#include "ppm.h"
#include "scm_dst.h"
#include "scm_src.h"
#include "diag_debug.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SCM_CNF_DST_SIZE CONFIG_DIAG_CNFDSTCH_SIZE
#define SCM_CNF_DST_THRESHOLD CONFIG_DIAG_CNFDSTCH_THRESHOLD
#define SCM_IND_DST_SIZE CONFIG_DIAG_LOGDSTCH_SIZE
#define SCM_IND_DST_80PERCENT_SIZE (SCM_IND_DST_SIZE - (SCM_IND_DST_SIZE >> 3))
#define SCM_IND_DST_THRESHOLD CONFIG_DIAG_LOGDSTCH_THRESHOLD
#define SCM_NO_CODER_DST_SIZE (1024 * 1024)
#define SCM_NO_CODER_DST_THRESHOLD (768 * 1024)
#define SCM_DEBUGGING_DST_SIZE (128 * 1024)
#define SCM_DEBUGGING_DST_THRESHOLD (16 * 1024)

scm_dst_cfg_s g_dst_chan_info[SCM_DST_CHAN_BUTT] = {
    {
        .init_state = SCM_CHANNEL_UNINIT,
        .chan_id = ODT_CODER_DST_OM_CNF,
        .ppm_chan = PPM_CNF_CODER_CHAN,
        .buff_len = SCM_CNF_DST_SIZE,
        .threshold = SCM_CNF_DST_THRESHOLD,
        .timeout_mode = ODT_TIMEOUT_TFR_SHORT,
        .virt_buff = NULL,
        .phy_buff = NULL,
        .func = NULL,
    },
    {
        .init_state = SCM_CHANNEL_UNINIT,
        .chan_id = ODT_CODER_DST_OM_IND,
        .ppm_chan = PPM_IND_CODER_CHAN,
        .buff_len = SCM_IND_DST_SIZE,
        .threshold = SCM_IND_DST_THRESHOLD,
        .timeout_mode = ODT_TIMEOUT_TFR_SHORT,
        .virt_buff = NULL,
        .phy_buff = NULL,
        .func = NULL,
    },
    {
        .init_state = SCM_CHANNEL_UNINIT,
        .chan_id = ODT_CODER_DST_OM_NO_CODER_IND, // 非编码
        .ppm_chan = PPM_IND_NOCODER_CHAN,
        .buff_len = SCM_NO_CODER_DST_SIZE,
        .threshold = SCM_NO_CODER_DST_THRESHOLD,
        .timeout_mode = ODT_TIMEOUT_TFR_LONG,
        .virt_buff = NULL,
        .phy_buff = NULL,
        .func = NULL,
    },
    {
        .init_state = SCM_CHANNEL_UNINIT,
        .chan_id = ODT_CODER_DST_LOGSERVER, // MXT和dump共用
        .ppm_chan = PPM_DEBUGGING_CODER_CHAN,
        .buff_len = SCM_DEBUGGING_DST_SIZE,
        .threshold = SCM_DEBUGGING_DST_THRESHOLD,
        .timeout_mode = ODT_TIMEOUT_TFR_SHORT,
        .virt_buff = NULL,
        .phy_buff = NULL,
        .func = NULL,
    },
};

void scm_save_ind_chan_buffer(const char *dir)
{
    int ret;
    scm_dst_cfg_s *ind_chan_info = &g_dst_chan_info[SCM_IND_DST_CHAN];

    if (ind_chan_info->virt_buff == NULL) {
        diag_err("not save diag_data\n");
        return;
    }

    ret = bsp_dump_log_save(dir, "diag_data.bin", (const void *)(uintptr_t)ind_chan_info->virt_buff, \
        (const void *)(uintptr_t)ind_chan_info->phy_buff, ind_chan_info->buff_len);
    if (ret) {
        diag_err("dump save diag_data fail:0x%x\n", ret);
    } else {
        diag_err("dump save diag_data ok\n");
    }
}

void scm_dst_chan_reset(unsigned int dst_chan_id)
{
    odt_buffer_rw_s dstbuf = {0};

    (void)bsp_odt_get_read_buff(dst_chan_id, &dstbuf);

    (void)bsp_odt_read_data_done(dst_chan_id, dstbuf.size + dstbuf.rb_size);
}

int scm_rls_dst_buffer(unsigned int dst_chan_id, unsigned int data_len)
{
    int ret;
    unsigned int chan_id = ODT_REAL_CHAN_ID(dst_chan_id);

    if (data_len == 0) {
        return 0;
    }

    ret = bsp_odt_read_data_done(dst_chan_id, data_len);
    if (ret) {
        diag_err("rls dst buffer fail,chan_id=0x%x, ret = 0x%x\n", chan_id, ret);
    }

    return ret;
}

int scm_dst_data_send_handler(unsigned int dst_chan_id, unsigned int len)
{
    int ret;

    ret = bsp_odt_read_data_done(dst_chan_id, len);
    if (ret) {
        diag_err("encoder dstdata sendcb release dst chan buff fail:%d\n", ret);
        return ret;
    }

    return ret;
}

static int scm_dst_data_send(unsigned int chan_id)
{
    unsigned char *virt_addr = NULL;
    unsigned int ret;
    unsigned int data_size;
    odt_buffer_rw_s dst_buff;
    unsigned int real_id = (chan_id & 0xf);

    (void)bsp_odt_get_read_buff(real_id, &dst_buff);

    data_size = dst_buff.size + dst_buff.rb_size;

    if (((chan_id == SCM_IND_DST_CHAN) && (data_size >= SCM_IND_DST_80PERCENT_SIZE)) || \
        ((chan_id == SCM_NO_CODER_DST_CHAN) && (data_size >= SCM_NO_CODER_DST_THRESHOLD))) {
        g_diag_dbg.mntn_data.scm_dst_dbg[chan_id].odt_buff_over_80_percent++;
    }

    if ((data_size == 0) || (dst_buff.buffer == NULL) || (dst_buff.size == 0)) {
        diag_dbg_dst_lost(chan_id, ODT_BUFF_ERR, data_size);
        (void)bsp_odt_read_data_done(real_id, data_size);
        return DIAG_ERRNO_GET_ODT_DST_BUF_FAIL;
    }

    virt_addr = (unsigned char *)dst_buff.buffer - g_dst_chan_info[real_id].phy_buff + g_dst_chan_info[real_id].virt_buff;

    /* 当发送是通过USB并且发送长度大于1MB的时候，需要限制发送长度，在usb send中只发送1MB */
    ret = ppm_data_send(g_dst_chan_info[real_id].ppm_chan, virt_addr, dst_buff.buffer, dst_buff.size);
    if(ret != PPM_SEND_ASYNC) {
        if (ret < 0 || ret != dst_buff.size) {
            diag_dbg_dst_lost(chan_id, PPM_SEND_FAIL, dst_buff.size);
        }
        g_diag_dbg.mntn_data.scm_dst_dbg[chan_id].odt_send_len += dst_buff.size;
        (void)bsp_odt_read_data_done(real_id, dst_buff.size); /* 当发USB发送失败时，会清空所有buffer */
    } else {
        diag_dbg_send_usb_start(chan_id);
    }
    return 0;
}

static int scm_dst_chan_cfg(odt_dst_chan_cfg_s *chan_cfg, scm_dst_cfg_s *chan_info)
{
    int ret;

    chan_cfg->encdst_thrh = DIAG_CNF_DST_OVERFLOW_THRESHOLD;
    chan_cfg->coder_dst_setbuf.output_start = chan_info->phy_buff;
    chan_cfg->coder_dst_setbuf.output_end = (chan_info->phy_buff + chan_info->buff_len) - 1;
    chan_cfg->coder_dst_setbuf.threshold = chan_info->threshold;
    chan_cfg->encdst_timeout_mode = chan_info->timeout_mode;

    ret = bsp_odt_coder_set_dst_chan(chan_info->chan_id, chan_cfg);
    if (ret) {
        diag_err("diag dst chan cfg fail:%d, chan_enum = 0x%x\n", ret, chan_info->chan_id);
        chan_info->init_state = SCM_CHANNEL_CFG_FAIL;
        return ret;
    }

    (void)bsp_odt_register_read_cb(chan_info->chan_id, scm_dst_data_send);
    (void)bsp_odt_dst_channel_enable(chan_info->chan_id);
    chan_info->init_state = SCM_CHANNEL_INIT_SUCC;

    return ret;
}

static int scm_cnf_dst_chan_init(void)
{
    int ret;
    odt_dst_chan_cfg_s chan_cfg = {0};
    struct platform_device *pdev = NULL;
    scm_dst_cfg_s *cnf_chan_info = &g_dst_chan_info[SCM_CNF_DST_CHAN];
    const bsp_version_info_s *ver = bsp_get_version_info();

    pdev = bsp_get_odt_pdev();
    if (pdev == NULL) {
        diag_err("encoder cnf dst chan get odt pdev fail\n");
        return DIAG_ERRNO_GET_PDEV_FAIL;
    }

    cnf_chan_info->virt_buff = (unsigned char *)dma_alloc_coherent(&pdev->dev, cnf_chan_info->buff_len,
        (dma_addr_t *)&cnf_chan_info->phy_buff, GFP_KERNEL);
    if (cnf_chan_info->virt_buff== NULL) {
        diag_err("diag cnf dst chan malloc dma buffer fail, buff_len = 0x%x\n", cnf_chan_info->buff_len);
        cnf_chan_info->init_state = SCM_CHANNEL_MEM_FAIL;
        return DIAG_ERRNO_MALLOC_FAIL;
    }

    if ((ver != NULL) && ((ver->plat_type == PLAT_ESL) || (ver->plat_type == PLAT_HYBRID))) {
        cnf_chan_info->threshold = 0x20;
    }

    ret = scm_dst_chan_cfg(&chan_cfg, cnf_chan_info);
    if (ret) {
        dma_free_coherent(&pdev->dev, cnf_chan_info->buff_len, (void *)cnf_chan_info->virt_buff,
            (dma_addr_t)(uintptr_t)cnf_chan_info->phy_buff);
        diag_err("scm cnf dst chan fail:0x%x\n", ret);
        return ret;
    }
    bsp_odt_dst_trans_id_disable(cnf_chan_info->chan_id);

    bsp_odt_encdst_dsm_init(cnf_chan_info->chan_id, ODT_DEST_DSM_ENABLE);
    return 0;
}

static int scm_ind_dst_chan_init(void)
{
    odt_dst_chan_cfg_s chan_cfg = {0};
    odt_encdst_buf_log_cfg_s log_cfg = {0};
    scm_dst_cfg_s *ind_chan_info = &g_dst_chan_info[SCM_IND_DST_CHAN];
    const bsp_version_info_s *ver = bsp_get_version_info();

    bsp_odt_get_log_cfg(&log_cfg);

    ind_chan_info->virt_buff = (unsigned char *)(uintptr_t)log_cfg.vir_buffer;
    ind_chan_info->phy_buff  = (unsigned char *)(uintptr_t)log_cfg.phy_buffer_addr;
    ind_chan_info->buff_len = log_cfg.buffer_size;

    if ((ver != NULL) && ((ver->plat_type == PLAT_ESL) || (ver->plat_type == PLAT_HYBRID))) {
        ind_chan_info->threshold = 0x200;
    }

    bsp_odt_dst_trans_id_disable(ind_chan_info->chan_id);

    return scm_dst_chan_cfg(&chan_cfg, ind_chan_info);
}

static int scm_no_coder_dst_chan_init(void)
{
    int ret;
    odt_dst_chan_cfg_s chan_cfg = {0};
    struct platform_device *pdev = NULL;
    scm_dst_cfg_s *scm_chan_info = &g_dst_chan_info[SCM_NO_CODER_DST_CHAN];

    pdev = bsp_get_odt_pdev();
    if (pdev == NULL) {
        diag_err("encoder no coder dst chan get odt pdev fail\n");
        return DIAG_ERRNO_GET_PDEV_FAIL;
    }

    scm_chan_info->virt_buff = (unsigned char *)dma_alloc_coherent(&pdev->dev, scm_chan_info->buff_len,
        (dma_addr_t *)&scm_chan_info->phy_buff, GFP_KERNEL);
    if (scm_chan_info->virt_buff == NULL) {
        diag_err("diag no coder dst chan malloc dma buffer fail, buff_len = 0x%x\n", scm_chan_info->buff_len);
        scm_chan_info->init_state = SCM_CHANNEL_MEM_FAIL;
        return DIAG_ERRNO_MALLOC_FAIL;
    }

    ret = scm_dst_chan_cfg(&chan_cfg, scm_chan_info);
    if (ret) {
        diag_err("scm no coder dst chan cfg fail:0x%x\n", ret);
        goto failure;
    }

    bsp_odt_dst_trans_id_disable(scm_chan_info->chan_id);

    return ret;
failure:
    dma_free_coherent(&pdev->dev, scm_chan_info->buff_len, (void *)scm_chan_info->virt_buff,
            (dma_addr_t)(uintptr_t)scm_chan_info->phy_buff);
    bsp_odt_dst_trans_id_disable(scm_chan_info->chan_id);
    return ret;
}

static int scm_debugging_dst_chan_init(void)
{
    int ret;
    odt_dst_chan_cfg_s chan_cfg = {0};
    struct platform_device *pdev = NULL;
    scm_dst_cfg_s *scm_chan_info = &g_dst_chan_info[SCM_DEBUGGING_DST_CHAN];

    pdev = bsp_get_odt_pdev();
    if (pdev == NULL) {
        diag_err("encoder debugging dst chan get odt pdev fail\n");
        return DIAG_ERRNO_GET_PDEV_FAIL;
    }

    scm_chan_info->virt_buff = (unsigned char *)dma_alloc_coherent(&pdev->dev, scm_chan_info->buff_len,
        (dma_addr_t *)&scm_chan_info->phy_buff, GFP_KERNEL);
    if (scm_chan_info->virt_buff== NULL) {
        diag_err("diag debugging dst chan malloc dma buffer fail, buff_len = 0x%x\n", scm_chan_info->buff_len);
        scm_chan_info->init_state = SCM_CHANNEL_MEM_FAIL;
        return DIAG_ERRNO_MALLOC_FAIL;
    }

    ret = scm_dst_chan_cfg(&chan_cfg, scm_chan_info);
    if (ret) {
        dma_free_coherent(&pdev->dev, scm_chan_info->buff_len, (void *)scm_chan_info->virt_buff,
            (dma_addr_t)(uintptr_t)scm_chan_info->phy_buff);
        diag_err("scm debugging dst chan cfg fail:0x%x\n", ret);
        return ret;
    }

    bsp_odt_dst_trans_id_disable(scm_chan_info->chan_id);

    bsp_odt_encdst_dsm_init(scm_chan_info->chan_id, ODT_DEST_DSM_ENABLE);
    return 0;
}

int diag_coder_data_send_handler(ppm_chan_e chn, unsigned char *data, unsigned int len)
{
    int ret = -1;

    switch (chn) {
        case PPM_CNF_CODER_CHAN:
            g_diag_dbg.mntn_data.scm_dst_dbg[SCM_CNF_DST_CHAN].odt_send_len += len;
            g_diag_dbg.mntn_data.scm_dst_dbg[SCM_CNF_DST_CHAN].odt_send_complete_len += len;
            diag_dbg_send_finish(SCM_CNF_DST_CHAN);
            ret = scm_dst_data_send_handler(ODT_CODER_DST_OM_CNF, len);
            break;
        case PPM_IND_CODER_CHAN:
            g_diag_dbg.mntn_data.scm_dst_dbg[SCM_IND_DST_CHAN].odt_send_len += len;
            g_diag_dbg.mntn_data.scm_dst_dbg[SCM_IND_DST_CHAN].odt_send_complete_len += len;
            diag_dbg_send_finish(SCM_IND_DST_CHAN);
            ret = scm_dst_data_send_handler(ODT_CODER_DST_OM_IND, len);
            break;
        case PPM_DEBUGGING_CODER_CHAN:
            g_diag_dbg.mntn_data.scm_dst_dbg[SCM_DEBUGGING_DST_CHAN].odt_send_len += len;
            g_diag_dbg.mntn_data.scm_dst_dbg[SCM_DEBUGGING_DST_CHAN].odt_send_complete_len += len;
            diag_dbg_send_finish(SCM_DEBUGGING_DST_CHAN);
            ret = scm_dst_data_send_handler(ODT_CODER_DST_LOGSERVER, len);
            break;
        default:
            diag_err("chan type: 0x%x err, len: %d\n", chn, len);
    }

    if (ret) {
        diag_err("scm send handle fail, ret:0x%x", ret);
    }

    return ret;
}

int scm_dst_ppm_init(void)
{
    int ret;

    ret = ppm_register_send_cb(PPM_CNF_CODER_CHAN, diag_coder_data_send_handler);
    if (ret) {
        diag_err("ppm cnf register send cb fail ret: 0x%x\n", ret);
        return ret;
    }
    ret = ppm_register_send_cb(PPM_IND_CODER_CHAN, diag_coder_data_send_handler);
    if (ret) {
        diag_err("ppm ind register send cb fail ret: 0x%x\n", ret);
        return ret;
    }
    ret = ppm_register_send_cb(PPM_DEBUGGING_CODER_CHAN, diag_coder_data_send_handler);
    if (ret) {
        diag_err("ppm debugging register send cb fail ret: 0x%x\n", ret);
        return ret;
    }

    return ret;
}

int scm_dst_chan_init(void)
{
    int ret;

    ret = scm_dst_ppm_init();
    if (ret) {
        diag_err("scm_dst_ppm_init fail ret: 0x%x\n", ret);
        return ret;
    }

    ret = scm_cnf_dst_chan_init();
    if (ret) {
        diag_err("scm_cnf_dst_chan_init fail ret: 0x%x\n", ret);
        return ret;
    }

    ret = scm_ind_dst_chan_init();
    if (ret) {
        diag_err("scm_ind_dst_chan_init fail ret: 0x%x\n", ret);
        return ret;
    }
    bsp_dump_register_log_notifier(DUMP_SAVE_DIAG_CYCLELOG, scm_save_ind_chan_buffer);

    ret = scm_no_coder_dst_chan_init();
    if (ret) {
        diag_err("scm_no_coder_dst_chan_init fail ret: 0x%x\n", ret);
        return ret;
    }

    ret = scm_debugging_dst_chan_init();
    if (ret) {
        diag_err("scm_debugging_dst_chan_init fail ret: 0x%x\n", ret);
        return ret;
    }

    return ret;
}

int bsp_diag_debugging_dst_chan_require(void)
{
    int ret;
    unsigned int dst_chan_id = g_dst_chan_info[SCM_DEBUGGING_DST_CHAN].chan_id;

    diag_err("dump require mxt chan entry\n");

    bsp_odt_stop(ODT_CODER_SRC_AP_MPROBE);

    diag_trigger_disconn(DIAG_TOOL_TYPE_MXT);

    bsp_odt_free_enc_dst_chan(dst_chan_id);

    bsp_odt_encdst_dsm_init(dst_chan_id, ODT_DEST_DSM_DISABLE);

    ret = bsp_odt_wait_single_chan_idle(ODT_CODER_SRC_AP_MPROBE);
    if (ret) {
        diag_err("wait mxt ap src chan fail:0x%x\n", ret);
        return ret;
    }

    ret = bsp_odt_wait_single_chan_idle(ODT_CODER_SRC_TSP_MPROBE);
    if (ret) {
        diag_err("wait mx cap src chan fail:0x%x\n", ret);
        return ret;
    }

    diag_err("dump require mxt chan ok\n");

    return ret;
}

int bsp_diag_debugging_dst_chan_release(void)
{
    int ret;
    unsigned int dst_chan_id = g_dst_chan_info[SCM_DEBUGGING_DST_CHAN].chan_id;
    odt_dst_chan_cfg_s chan_cfg = {0};

    if(g_dst_chan_info[SCM_DEBUGGING_DST_CHAN].init_state != SCM_CHANNEL_INIT_SUCC) {
        diag_err("no need cfg mxt dst chan\n");
        return 0;
    }

    diag_err("dump release mxt chan entry\n");

    bsp_odt_free_enc_dst_chan(dst_chan_id);

    ret = scm_dst_chan_cfg(&chan_cfg, &g_dst_chan_info[SCM_DEBUGGING_DST_CHAN]);
    if (ret) {
        diag_err("config mxt dst chan fail:0x%x\n", ret);
        return ret;
    }

    bsp_odt_start(ODT_CODER_SRC_AP_MPROBE);

    bsp_odt_encdst_dsm_init(dst_chan_id, ODT_DEST_DSM_ENABLE);

    bsp_odt_dst_channel_enable(dst_chan_id);

    diag_err("dump release mxt chan ok\n");

    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
