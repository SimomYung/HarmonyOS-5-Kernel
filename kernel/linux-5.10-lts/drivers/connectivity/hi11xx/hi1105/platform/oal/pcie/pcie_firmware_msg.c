/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description:pcie firmware download module
 * Author: @CompanyNameTag
 */

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE

#define HISI_LOG_TAG "[PCIEF]"
#include "pcie_firmware_msg.h"
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0))
#include <linux/sched/task_stack.h>
#endif
#include "pcie_host_mpxx.h"
#include "plat_firmware.h"
#include "pcie_soc.h"
#include "../../inc/oal/mp17c/pcie_ctrl_rb_regs.h"

#define TRANS_ALIGN_SIZE   4
#define MAX_READ_SIZE      128  /* ack接收size */
#define MAX_CMD_SIZE       512  /* 命令发送size */

#define MEMCPY_TIMEOUT     3000

OAL_STATIC int32_t oal_pcie_firmwre_h2d_msg_buf_rd_update(oal_pcie_res *pst_pci_res)
{
    uint32_t rd;
    pci_addr_map st_map;

    st_map.va = pst_pci_res->frw_res.h2d_ctl.ctrl_daddr.va + OAL_OFFSET_OF(frw_ringbuf_t, rd);
    st_map.pa = pst_pci_res->frw_res.h2d_ctl.ctrl_daddr.pa + OAL_OFFSET_OF(frw_ringbuf_t, rd);

    rd = oal_readl(st_map.va);
    if (rd == 0xFFFFFFFF) {
        if (oal_pcie_check_link_state(pst_pci_res) == OAL_FALSE) {
            pci_print_log(PCI_LOG_ERR, "link down[va:0x%lx, pa:0x%lx]",
                          st_map.va, st_map.pa);
            return -OAL_ENODEV;
        }
    }
    pci_print_log(PCI_LOG_DBG, "h2d rd update:[rd=0x%x]", rd);
    pst_pci_res->frw_res.share_mem.h2d_buf.rd = rd;
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_firmwre_h2d_msg_buf_wr_update(oal_pcie_res *pst_pci_res)
{
    uint32_t wr_back;
    pci_addr_map st_map;
    frw_ringbuf_t *ringbuf = &pst_pci_res->frw_res.share_mem.h2d_buf;

    st_map.va = pst_pci_res->frw_res.h2d_ctl.ctrl_daddr.va + OAL_OFFSET_OF(frw_ringbuf_t, wr);

    oal_writel(ringbuf->wr, st_map.va);

    wr_back = oal_readl(st_map.va);
    if (wr_back != ringbuf->wr) {
        pci_print_log(PCI_LOG_ERR, "pcie h2d msg wr write failed, wr_back=%u, host_wr=%u",
                      wr_back, ringbuf->wr);
        declare_dft_trace_key_info("oal_pcie_firmwre_h2d_msg_buf_wr_update", OAL_DFT_TRACE_FAIL);
        return -OAL_EFAIL;
    }

    pci_print_log(PCI_LOG_DBG, "pcie h2d msg wr, wr_back=0x%x, host_wr=0x%x", wr_back, ringbuf->wr);
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_firmwre_d2h_msg_buf_rd_update(oal_pcie_res *pst_pci_res)
{
    pci_addr_map st_map;

    st_map.va = pst_pci_res->frw_res.d2h_ctl.ctrl_daddr.va + OAL_OFFSET_OF(frw_ringbuf_t, rd);

    oal_writel(pst_pci_res->frw_res.share_mem.d2h_buf.rd, st_map.va);
    pci_print_log(PCI_LOG_DBG, "d2h rd update:[rd=0x%x]",oal_readl(st_map.va));
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_firmwre_d2h_msg_buf_wr_update(oal_pcie_res *pst_pci_res)
{
    uint32_t wr_back;
    pci_addr_map st_map;

    st_map.va = pst_pci_res->frw_res.d2h_ctl.ctrl_daddr.va + OAL_OFFSET_OF(frw_ringbuf_t, wr);
    st_map.pa = pst_pci_res->frw_res.d2h_ctl.ctrl_daddr.pa + OAL_OFFSET_OF(frw_ringbuf_t, wr);

    wr_back = oal_readl(st_map.va);
    if (wr_back == 0xFFFFFFFF) {
        if (oal_pcie_check_link_state(pst_pci_res) == OAL_FALSE) {
            pci_print_log(PCI_LOG_ERR, "d2h msg ringbuf wr update: link down[va:0x%lx, pa:0x%lx]",
                          st_map.va, st_map.pa);
            return -OAL_ENODEV;
        }
    }

    pst_pci_res->frw_res.share_mem.d2h_buf.wr = wr_back;
    pci_print_log(PCI_LOG_DBG, "d2h wr update:[wr=0x%x]", wr_back);
    return OAL_SUCC;
}


OAL_STATIC void oal_pcie_firmware_msg_h2d_buf_write(oal_pcie_res *pci_res, uint8_t *buff, int32_t len)
{
    errno_t ret;
    uint8_t cmd[MAX_CMD_SIZE];
    frw_ringbuf_t *ringbuf = &pci_res->frw_res.share_mem.h2d_buf;

    if (MAX_CMD_SIZE > ringbuf->size) {
        pci_print_log(PCI_LOG_ERR, "invalid cmd len, ringbuf_size=%d, cmd_buf=%d", ringbuf->size, MAX_CMD_SIZE);
        return;
    }

    ret = memcpy_s(cmd, MAX_CMD_SIZE, buff, len);
    if (ret != EOK) {
        pci_print_log(PCI_LOG_ERR, "memcpy fail, len=%d, ret=%d", len, ret);
        return;
    }

    /* 加载命令发送都是串行执行, 每次发送命令, 可以从buffer头开始写入 */
    oal_pcie_io_trans(pci_res->frw_res.h2d_ctl.data_daddr.va, (uintptr_t)cmd, MAX_CMD_SIZE);
    ringbuf->wr += (uint32_t)len;

    pci_print_log(PCI_LOG_DBG, "pcie msg send, cmd: %s", buff);
}

OAL_STATIC int32_t oal_pcie_firmware_msg_d2h_buf_read(oal_pcie_res *pci_res, uint8_t *buff, int32_t len)
{
    errno_t ret;
    uint8_t rd_ack[MAX_READ_SIZE];
    uint32_t recv_len;
    frw_ringbuf_t *ringbuf = &pci_res->frw_res.share_mem.d2h_buf;

    ringbuf = &pci_res->frw_res.share_mem.d2h_buf;
    recv_len = ringbuf->wr - ringbuf->rd;

    if (recv_len > MAX_READ_SIZE) {
        pci_print_log(PCI_LOG_ERR, "invalid recv_len len, recv_len=%d, rd_buf=%d", recv_len, MAX_READ_SIZE);
        return -OAL_EFAIL;
    }

    if (recv_len == 0) {
        pci_print_log(PCI_LOG_ERR, "recv_len=%d, wr=%d, rd=%d", recv_len, ringbuf->wr, ringbuf->rd);
        return -OAL_EFAIL;
    }

    oal_pcie_io_trans((uintptr_t)rd_ack, pci_res->frw_res.d2h_ctl.data_daddr.va, MAX_READ_SIZE);
    ret = memcpy_s(buff, len, rd_ack, recv_len);
    if (ret != EOK) {
        pci_print_log(PCI_LOG_ERR, "memcpy fail, ret=%d", ret);
        return -OAL_EFAIL;
    }
    buff[len - 1] = '\0';
    ringbuf->rd += recv_len;

    pci_print_log(PCI_LOG_DBG, "pcie msg read, ack: %s", buff);
    return OAL_SUCC;
}

/* 更新h2d, ete memcpy src和len */
OAL_STATIC int32_t oal_pcie_firmwre_h2d_memcpy_addr_len_update(oal_pcie_res *pst_pci_res)
{
    uint32_t wr_back;
    pci_addr_map st_map;
    ete_memcpy_info *cpy_info = &pst_pci_res->frw_res.share_mem.cpy_info;

    st_map.va = pst_pci_res->frw_res.cpy_map.va + OAL_OFFSET_OF(ete_memcpy_info, h2d_src);
    oal_pcie_io_trans(st_map.va, (uintptr_t)(void*)(&cpy_info->h2d_src), sizeof(uint64_t));

    st_map.va = pst_pci_res->frw_res.cpy_map.va + OAL_OFFSET_OF(ete_memcpy_info, h2d_len);
    oal_writel(cpy_info->h2d_len, st_map.va);
    wr_back = oal_readl(st_map.va);
    if (wr_back != cpy_info->h2d_len) {
        pci_print_log(PCI_LOG_ERR, "pcie h2d memcpy len write failed, wr_back=%u, host_wr=%u",
                      wr_back, cpy_info->h2d_len);
        declare_dft_trace_key_info("oal_pcie_firmwre_h2d_memcpy_addr_len_update", OAL_DFT_TRACE_FAIL);
        return -OAL_EFAIL;
    }

    pci_print_log(PCI_LOG_DBG, "pcie h2d memcpy len write wr update:[len=0x%x]", wr_back);
    return OAL_SUCC;
}

/* 更新d2h, ete memcpy dst, len由device填写 */
OAL_STATIC int32_t oal_pcie_firmwre_d2h_memcpy_addr_update(oal_pcie_res *pst_pci_res)
{
    uint32_t rd_back, old_val;
    pci_addr_map st_map;
    ete_memcpy_info *cpy_info = &pst_pci_res->frw_res.share_mem.cpy_info;

    st_map.va = pst_pci_res->frw_res.cpy_map.va + OAL_OFFSET_OF(ete_memcpy_info, d2h_dst);
    oal_pcie_io_trans(st_map.va, (uintptr_t)(void*)(&cpy_info->d2h_dst), sizeof(uint64_t));

    rd_back = oal_readl(st_map.va);
    old_val = (uint32_t)(cpy_info->d2h_dst);
    if (rd_back != old_val) {
        pci_print_log(PCI_LOG_ERR, "pcie d2h memcpy d2h_dst write failed, rd_back=%u, host_wr=%u",
                      rd_back, old_val);
        declare_dft_trace_key_info("oal_pcie_firmwre_d2h_memcpy_addr_update", OAL_DFT_TRACE_FAIL);
        return -OAL_EFAIL;
    }

    pci_print_log(PCI_LOG_DBG, "pcie d2h memcpy addr write wr update:[addr=0x%x]", rd_back);
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_firmware_wait_for_dev_complete(oal_pcie_res *pci_res, uint32_t timeout)
{
    long timeleft;

    /* wait for ack completion */
    timeleft = oal_wait_for_completion_interruptible_timeout(&pci_res->frw_res.dev_done, msecs_to_jiffies(timeout));
    if (!timeleft) {
        pci_print_log(PCI_LOG_ERR, "0x%x ms timeout", timeout);
        return -OAL_EFAIL;
    }
    return OAL_SUCC;
}

void oal_pcie_firmware_tx_memcpy_trigger(oal_pcie_res *pst_pci_res)
{
    oal_pcie_h2d_int(pst_pci_res);
}

int32_t oal_pcie_firmware_msg_send_proc(oal_pcie_res *pci_res, uint8_t *buff, int32_t len)
{
    uint32_t size;

    size = pci_res->frw_res.share_mem.h2d_buf.size;
    if (size < (uint32_t)len) {
        pci_print_log(PCI_LOG_ERR, "buf len[%d] is larger than h2d_buf_size[%u]", len, size);
        return -OAL_EINVAL;
    }

    oal_pcie_firmwre_h2d_msg_buf_rd_update(pci_res);
    oal_pcie_firmware_msg_h2d_buf_write(pci_res, buff, len);
    /* msg发送后等待ack回复，故在此初始化信号量 */
    oal_reinit_completion(pci_res->frw_res.dev_done);
    pci_print_log(PCI_LOG_DBG, "reinit_completion msg send");
    oal_pcie_firmwre_h2d_msg_buf_wr_update(pci_res);
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_firmware_bin_send_proc(oal_pcie_res *pci_res, uint8_t *buff, int32_t len)
{
    int32_t ret;
    dma_addr_t pci_dma_addr;
    uint64_t pci_slave_addr;
    oal_pci_dev_stru *pst_pci_dev = NULL;
    ete_memcpy_info *cpy_info = &pci_res->frw_res.share_mem.cpy_info;

    pst_pci_dev = pcie_res_to_dev(pci_res);
    if (pst_pci_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_dev is null");
        return -OAL_ENODEV;
    }

    if (object_is_on_stack(buff) == OAL_TRUE) {
        pci_print_log(PCI_LOG_ERR, "object is onstack");
        return -OAL_EINVAL;
    }

    pci_print_log(PCI_LOG_DBG, "ete memcpy files bin, write to device");

    pci_dma_addr = dma_map_single(&pst_pci_dev->dev, buff, len, PCI_DMA_TODEVICE);
    pcie_if_hostca_to_devva(0, pci_dma_addr, &pci_slave_addr);
    cpy_info->h2d_src = pci_slave_addr;
    cpy_info->h2d_len = (uint32_t)len;
    pci_res->frw_res.dma_addr = pci_dma_addr;
    pci_res->frw_res.unmap_flag = OAL_TRUE;
    oal_pcie_firmwre_h2d_memcpy_addr_len_update(pci_res);

    /* 触发搬运 */
    oal_reinit_completion(pci_res->frw_res.dev_done);
    pci_print_log(PCI_LOG_DBG, "reinit_completion send bin");
    oal_pcie_firmware_tx_memcpy_trigger(pci_res);

    ret = oal_pcie_firmware_wait_for_dev_complete(pci_res, MEMCPY_TIMEOUT);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "wait for h2d memcpy complete fail");
    }

    dma_unmap_single(&pst_pci_dev->dev, pci_res->frw_res.dma_addr, cpy_info->h2d_len, PCI_DMA_TODEVICE);
    pci_print_log(PCI_LOG_DBG, "h2d dma unmap");
    oal_pcie_firmware_tx_memcpy_trigger(pci_res);
    return ret;
}

OAL_STATIC int32_t oal_pcie_firmware_bin_read_proc(oal_pcie_res *pci_res, uint8_t *buff, int32_t len)
{
    int32_t ret;
    dma_addr_t pci_dma_addr;
    pci_addr_map st_map;
    uint64_t pci_slave_addr;
    oal_pci_dev_stru *pst_pci_dev = NULL;
    ete_memcpy_info *cpy_info = &pci_res->frw_res.share_mem.cpy_info;

    pst_pci_dev = pcie_res_to_dev(pci_res);
    if (pst_pci_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "pst_pcie_dev is null");
        return -OAL_ENODEV;
    }

    if (object_is_on_stack(buff) == OAL_TRUE) {
        pci_print_log(PCI_LOG_ERR, "object is onstack");
        return -OAL_EINVAL;
    }

    pci_print_log(PCI_LOG_DBG, "ete memcpy files bin, read from device");

    pci_dma_addr = dma_map_single(&pst_pci_dev->dev, buff, len, PCI_DMA_FROMDEVICE);
    pcie_if_hostca_to_devva(0, pci_dma_addr, &pci_slave_addr);
    cpy_info->d2h_dst = pci_slave_addr;
    pci_res->frw_res.dma_addr = pci_dma_addr;
    pci_res->frw_res.unmap_flag = OAL_TRUE;
    oal_pcie_firmwre_d2h_memcpy_addr_update(pci_res);

    /* 触发搬运 */
    oal_reinit_completion(pci_res->frw_res.dev_done);
    pci_print_log(PCI_LOG_DBG, "reinit_completion read bin");
    oal_pcie_firmware_tx_memcpy_trigger(pci_res);

    if (oal_pcie_firmware_wait_for_dev_complete(pci_res, MEMCPY_TIMEOUT) != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "wait for d2h memcpy complete fail");
        ret = -OAL_EFAUL;
    } else {
        st_map.va = pci_res->frw_res.cpy_map.va + OAL_OFFSET_OF(ete_memcpy_info, d2h_len);
        ret = oal_readl(st_map.va);
    }

    dma_unmap_single(&pst_pci_dev->dev, pci_res->frw_res.dma_addr, len, PCI_DMA_FROMDEVICE);
    pci_print_log(PCI_LOG_DBG, "d2h dma unmap");
    return ret;
}

int32_t oal_pcie_firmware_msg_read(oal_pcie_linux_res *pst_pcie_lres, uint8_t *buff, int32_t len, uint32_t timeout,
                                   uint32_t file_flag)
{
    int32_t ret = -OAL_EFAUL;
    oal_pcie_res *pci_res = pst_pcie_lres->pst_pci_res;

    if ((pci_res == NULL) || (buff == NULL)) {
        pci_print_log(PCI_LOG_ERR, "pci_res or buff is null");
        return -OAL_EFAUL;
    }

    if (file_flag == FILES_BIN_READ) {
        ret = oal_pcie_firmware_bin_read_proc(pci_res, buff, len);
    } else if (file_flag == FILES_CMD_SEND) {
        if (oal_pcie_firmware_wait_for_dev_complete(pci_res, timeout) != OAL_SUCC) {
            pci_print_log(PCI_LOG_ERR, "wait for d2h msg complete fail");
            return -OAL_EFAIL;
        }
        oal_pcie_firmwre_d2h_msg_buf_wr_update(pci_res);
        ret = oal_pcie_firmware_msg_d2h_buf_read(pci_res, buff, len);
        oal_pcie_firmwre_d2h_msg_buf_rd_update(pci_res);
    } else {
        pci_print_log(PCI_LOG_ERR, "invalid file_flag=%d", file_flag);
    }

    return ret;
}

int32_t oal_pcie_firmware_msg_write(oal_pcie_linux_res *pst_pcie_lres, uint8_t *buff, int32_t len, uint32_t file_flag)
{
    int32_t ret = -OAL_EFAUL;
    oal_pcie_res *pci_res = pst_pcie_lres->pst_pci_res;

    if ((pci_res == NULL) || (buff == NULL)) {
        pci_print_log(PCI_LOG_ERR, "pci_res or buff is null");
        return -OAL_EFAUL;
    }

    if (file_flag == FILES_BIN_SEND) { /* 二进制加载 */
        ret = oal_pcie_firmware_bin_send_proc(pci_res, buff, len);
    } else if (file_flag == FILES_CMD_SEND) {
        ret = oal_pcie_firmware_msg_send_proc(pci_res, buff, len);
    } else {
        pci_print_log(PCI_LOG_ERR, "invalid file_flag=%d", file_flag);
    }

    return ret;
}

OAL_STATIC void oal_pcie_firmware_msg_d2h_rx_isr(void *data)
{
    oal_pcie_res *pci_res = (oal_pcie_res *)data;
    oal_complete(&pci_res->frw_res.dev_done);
    pci_print_log(PCI_LOG_DBG, "d2h_rx_msg_isr\n");
}

void oal_pcie_firmware_msg_int_func_register(oal_pcie_res *pcie_res)
{
    pcie_frw_cb_group_stru frw_isr_cb;
    memset_s(&frw_isr_cb, sizeof(pcie_frw_cb_group_stru), 0, sizeof(pcie_frw_cb_group_stru));

    frw_isr_cb.pcie_d2h_rx_isr_cb.pf_func = oal_pcie_firmware_msg_d2h_rx_isr;
    frw_isr_cb.pcie_d2h_rx_isr_cb.para = (void*)pcie_res;
    oal_pcie_frw_int_func_register(&frw_isr_cb);
}

int32_t oal_pcie_firmware_msg_ringbuf_res_map(oal_pcie_res *pcie_res)
{
    int32_t ret;
    pci_addr_map addr_map;
    uintptr_t buf_addr;

    buf_addr = pcie_res->frw_res.share_mem.h2d_buf.base_addr;
    ret = oal_pcie_inbound_ca_to_va(pcie_res, buf_addr, &addr_map);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "pcie send quit 0x%8x unmap, failed!\n", (uint32_t)buf_addr);
        return -OAL_EFAUL;
    }
    pcie_res->frw_res.h2d_ctl.data_daddr = addr_map;

    addr_map.va = pcie_res->frw_res.sharemem_map.va + OAL_OFFSET_OF(frw_share_mem, h2d_buf);
    addr_map.pa =  pcie_res->frw_res.sharemem_map.pa + OAL_OFFSET_OF(frw_share_mem, h2d_buf);
    pcie_res->frw_res.h2d_ctl.ctrl_daddr = addr_map;

    buf_addr = pcie_res->frw_res.share_mem.d2h_buf.base_addr;
    ret = oal_pcie_inbound_ca_to_va(pcie_res, buf_addr, &addr_map);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "pcie send quit 0x%8x unmap, failed!\n", (uint32_t)buf_addr);
        return -OAL_EFAUL;
    }
    pcie_res->frw_res.d2h_ctl.data_daddr = addr_map;

    addr_map.va = pcie_res->frw_res.sharemem_map.va + OAL_OFFSET_OF(frw_share_mem, d2h_buf);
    addr_map.pa =  pcie_res->frw_res.sharemem_map.pa + OAL_OFFSET_OF(frw_share_mem, d2h_buf);
    pcie_res->frw_res.d2h_ctl.ctrl_daddr = addr_map;
    return OAL_SUCC;
}

void oal_pcie_firmware_msg_int_mask(oal_pcie_res *pst_pci_res)
{
    if (pst_pci_res->chip_info.cb.frw_msg_int_mask != NULL) {
        pst_pci_res->chip_info.cb.frw_msg_int_mask(pst_pci_res);
    }
}

void oal_pcie_firmware_msg_int_unmask(oal_pcie_res *pst_pci_res)
{
    if (pst_pci_res->chip_info.cb.frw_msg_int_unmask != NULL) {
        pst_pci_res->chip_info.cb.frw_msg_int_unmask(pst_pci_res);
    }
}

void oal_pcie_firmware_msg_set_ch(oal_pcie_res *pst_pcie_res)
{
    pci_addr_map addr_map;
    int32_t ret;

    ret = oal_pcie_inbound_ca_to_va(pst_pcie_res, pst_pcie_res->chip_info.addr_info.ch_sel,
                                    &addr_map);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "set ch val: 0x%8x unmap, failed!\n", pst_pcie_res->chip_info.addr_info.ch_sel);
        return;
    }

    oal_writel(PCIE_SDIO_SEL_REG_VALUE, (void *)addr_map.va);
}

int32_t oal_pcie_firmware_msg_sharemem_update(oal_pcie_res *pcie_res)
{
    oal_pcie_io_trans((uintptr_t)(void *)(&pcie_res->frw_res.share_mem), \
                      pcie_res->frw_res.sharemem_map.va, sizeof(frw_share_mem));
    return OAL_SUCC;
}

/*
 * dma传输加载镜像依赖消息交互流程
 *                      安全模式             非安全模式
 * pcie直接加载         不支持(异常)             支持
 * dma传输(ete)           支持                  支持
 */
OAL_STATIC int32_t oal_pcie_firmware_dl_mode_check(oal_pcie_res *pcie_res)
{
    if (mpxx_firmware_download_mode() != MODE_COMBO) {
        pci_print_log(PCI_LOG_ERR, "dma_trans but dl_mode is not MODE_COMBO");
        return -OAL_EFAUL;
    }
    return OAL_SUCC;
}

int32_t oal_firmware_msg_download_pre(hcc_bus *bus)
{
    int32_t ret;
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)bus->data;
    oal_pcie_res *pcie_res = pst_pci_lres->pst_pci_res;

    if (pcie_res->chip_info.boot_cap.bits.dma_trans == OAL_FALSE) {
        pci_print_log(PCI_LOG_INFO, "ignore dma_trans!");
        return OAL_SUCC;
    }

    if (oal_pcie_firmware_dl_mode_check(pcie_res) != OAL_SUCC) {
        return -OAL_EFAUL;
    }

    oal_pcie_firmware_msg_set_ch(pcie_res);
    oal_pcie_firmware_msg_int_unmask(pcie_res);
    ret = oal_pcie_firmware_msg_sharemem_update(pcie_res);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR,"share mem update fail!\n");
        return -EFAIL;
    }

#ifdef _PRE_CONFIG_PCIE_SHARED_INTX_IRQ
    oal_enable_pcie_irq_with_request(pst_pci_lres);
#else
    oal_enable_pcie_irq(pst_pci_lres);
#endif
    return OAL_SUCC;
}

void oal_firmware_msg_download_post(hcc_bus *bus)
{
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)bus->data;
    oal_pcie_res *pcie_res = pst_pci_lres->pst_pci_res;
 
    if (pcie_res->chip_info.boot_cap.bits.dma_trans == OAL_FALSE) {
        pci_print_log(PCI_LOG_INFO, "ignore dma_trans!");
        return;
    }

    oal_pcie_firmware_msg_int_mask(pcie_res);
#ifdef _PRE_CONFIG_PCIE_SHARED_INTX_IRQ
    oal_disable_pcie_irq_with_free(pst_pci_lres);
#else
    oal_disable_pcie_irq(pst_pci_lres);
#endif
}

int32_t oal_pcie_firmware_msg_init(oal_pcie_res *pcie_res)
{
    int32_t ret;
    pci_addr_map addr_map;
    uintptr_t share_mem;

    if (pcie_res == NULL) {
        pci_print_log(PCI_LOG_ERR, "pcie_res is null");
        return -OAL_ENODEV;
    }

    if (pcie_res->chip_info.boot_cap.bits.dma_trans == OAL_FALSE) {
        pci_print_log(PCI_LOG_INFO, "dma_trans is false, ignore frm msg init!");
        return OAL_SUCC;
    }

    ret = oal_pcie_inbound_ca_to_va(pcie_res, pcie_res->chip_info.addr_info.boot_sharemem, &addr_map);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "0x%8x unmap, failed!\n", pcie_res->chip_info.addr_info.boot_sharemem);
        return -OAL_EFAUL;
    }

    /* 从boot_sharemem存储地址中读出share mem起始地址 */
    share_mem = oal_readl((void *)addr_map.va);
    ret = oal_pcie_inbound_ca_to_va(pcie_res, share_mem, &addr_map);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "0x%8x unmap, failed!\n", (uint32_t)share_mem);
        return -OAL_EFAUL;
    }
    pcie_res->frw_res.sharemem_map = addr_map;
    oal_pcie_io_trans((uintptr_t)(void *)(&pcie_res->frw_res.share_mem), addr_map.va, sizeof(frw_share_mem));

    addr_map.va = pcie_res->frw_res.sharemem_map.va + OAL_OFFSET_OF(frw_share_mem, cpy_info);
    addr_map.pa =  pcie_res->frw_res.sharemem_map.pa + OAL_OFFSET_OF(frw_share_mem, cpy_info);
    pcie_res->frw_res.cpy_map = addr_map;

    ret = oal_pcie_firmware_msg_ringbuf_res_map(pcie_res);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "msg ringbuf res map failed!\n");
        return -OAL_EFAUL;
    }

    oal_init_completion(&pcie_res->frw_res.dev_done);
    oal_pcie_firmware_msg_int_func_register(pcie_res);
    pci_print_log(PCI_LOG_INFO, "oal_pcie_firmware_msg_init succ, share_mem=%lu", sizeof(frw_share_mem));
    return OAL_SUCC;
}
#endif
