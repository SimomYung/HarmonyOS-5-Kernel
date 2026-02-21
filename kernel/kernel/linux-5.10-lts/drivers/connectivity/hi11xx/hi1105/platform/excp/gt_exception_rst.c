/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description: gt_exception_function
 * Author: @CompanyNameTag
 */
#include "plat_debug.h"
#include "plat_exception_rst.h"
#include "oal_hcc_host_if.h"
#include "plat_firmware.h"
#include "oal_kernel_file.h"
#include "bfgx_dev.h"
#include "gt_exception_rst.h"

int32_t gt_device_reset(void)
{
    ps_print_info("reset gt device by gt_en gpio\n");
    /* To implement later */

    return EXCEPTION_SUCCESS;
}

void gt_memdump_finish(void)
{
    g_gt_memdump_cfg.is_working = 0;
}

int32_t gt_notice_hal_memdump(void)
{
    return notice_hal_memdump(&g_gt_memdump_cfg, CMD_READM_GT_PCIE);
}

int32_t gt_memdump_enquenue(uint8_t *buf_ptr, uint16_t count)
{
    return excp_memdump_queue(buf_ptr, count, &g_gt_memdump_cfg);
}

#ifdef _PRE_PLATFORM_SAVE_UNCOMPRESS_PANIC_FILE
#define SAVE_FILE_PRIVILEGE_NUM 0644
#ifndef PANIC_FILE_CFG_DIR
#define PANIC_FILE_CFG_DIR FIRMWARE_GF61_CFG_DIR
#endif

#define GT_PANIC_VERSION_MAGIC_NUM 0xf061dead
#pragma pack(1)
#define EXCEP_CPU_CORE_NUM 2
struct excp_stack_frame_stru {
    unsigned long r0;
    unsigned long r1;
    unsigned long r2;
    unsigned long r3;
    unsigned long r4;
    unsigned long r5;
    unsigned long r6;
    unsigned long r7;
    unsigned long r8;
    unsigned long r9;
    unsigned long r10;
    unsigned long fp;
    unsigned long ip;
    unsigned long sp;
    unsigned long lr;
    unsigned long pc;
    unsigned long cpsr;
};
struct gtcpu_memdump_hdr_stru {
    uint32_t panic_type;   /* abort,wdt or other */
    uint32_t version;            /* for script parse */
    uint32_t reg_store_addr;     /* reference to g_ulRegStoreAddr */
};
typedef struct _gtcpu_panic_memdump_stru_ {
    struct gtcpu_memdump_hdr_stru hdr;   /* 24B */
    struct excp_stack_frame_stru st_regs[EXCEP_CPU_CORE_NUM];
    unsigned long addr[EXCEP_CPU_CORE_NUM];
    unsigned int fsr[EXCEP_CPU_CORE_NUM];
    uint32_t dfsr[EXCEP_CPU_CORE_NUM];
    uint32_t ifsr[EXCEP_CPU_CORE_NUM];
    uint32_t dfar[EXCEP_CPU_CORE_NUM];
    uint32_t ifar[EXCEP_CPU_CORE_NUM];
#define MAX_BACKTRACE_LEVEL 16
    uint32_t back_trace_magic_num;
    uint32_t stack_backtrace[EXCEP_CPU_CORE_NUM][MAX_BACKTRACE_LEVEL];
} gtcpu_panic_memdump_stru;
#pragma pack()

static void gt_exception_print_panic_info(gtcpu_panic_memdump_stru *info)
{
    int i = 0;
    int j = 0;
    for (i = 0; i < EXCEP_CPU_CORE_NUM; i++) {
        ps_print_err("gt device panic scene core%d register\n", i);
        ps_print_err("r0 0x%lx, r1 0x%lx, r2 0x%lx, r3 0x%lx, r4 0x%lx, r5 0x%lx, r6 0x%lx, r7 0x%lx\n",
            info->st_regs[i].r0,  info->st_regs[i].r1, info->st_regs[i].r2,  info->st_regs[i].r3,
            info->st_regs[i].r4, info->st_regs[i].r5, info->st_regs[i].r6, info->st_regs[i].r7);
        ps_print_err("r8 0x%lx, r9 0x%lx, r10 0x%lx, fp 0x%lx, ip 0x%lx, sp 0x%lx, cpsr 0x%lx\n", info->st_regs[i].r8,
            info->st_regs[i].r9, info->st_regs[i].r10,  info->st_regs[i].fp, info->st_regs[i].ip, info->st_regs[i].sp,
            info->st_regs[i].cpsr);
        ps_print_err("lr 0x%lx, pc 0x%lx\n", info->st_regs[i].lr, info->st_regs[i].pc);
        ps_print_err("dfsr 0x%x, ifsr 0x%x, dfar 0x%x, ifar 0x%x\n",
            info->dfsr[i], info->ifsr[i], info->dfar[i], info->ifar[i]);
        ps_print_err("fsr 0x%x, exception_addr 0x%lx\n", info->fsr[i], info->addr[i]);
        for (j = 0; j < MAX_BACKTRACE_LEVEL; j++) {
            ps_print_err("pc_backtrace level%d: 0x%x\n", j, info->stack_backtrace[i][j]);
        }
    }
}
static void gt_exception_dump_device_panic_mem(struct st_wifi_dump_mem_info *pst_mem_dump_info,
    uint8_t *data_buf, uint32_t size)
{
    gtcpu_panic_memdump_stru *gt_panic_dump = NULL;
    uint32_t val;
    int32_t i;
    int8_t *pst_file_name = (pst_mem_dump_info->file_name ?
                ((int8_t *)pst_mem_dump_info->file_name) : (int8_t *)"default: ");
    if (!oal_strcmp("gt_device_panic_mem", pst_file_name) && (size >= GT_CPU_PANIC_MEMDUMP_SIZE)) {
        oal_io_print("gt device panic mem size %d\n", size);
        for (i = 0; i < size; i = i + sizeof(val)) {
            val = *(uint32_t *)(data_buf + i);
            if ((val == GT_PANIC_VERSION_MAGIC_NUM) && (i > 1)) {
                gt_panic_dump = (gtcpu_panic_memdump_stru *) (data_buf + (i - 1 * sizeof(uint32_t)));
                break;
            }
        }
        if (gt_panic_dump == NULL) {
            ps_print_err("gt panic mem parse fail\n");
            return;
        }
        gt_exception_print_panic_info(gt_panic_dump);
    }
}

STATIC int32_t gt_exception_recv_device_memdump(uint8_t *data_buf, int32_t len)
{
    int32_t ret = -EFAIL;
    uint8_t retry = 3; /* 重复试3次 */
    int32_t lenbuf = 0;

    if (data_buf == NULL) {
        ps_print_err("%s data_buf is NULL\n", __FUNCTION__);
        return ret;
    }

    ps_print_dbg("gt exception recv len is [%d]\n", len);

    while (len > lenbuf) {
        ret = read_msg(data_buf + lenbuf, len - lenbuf, FILES_BIN_READ);
        if (ret > 0) {
            lenbuf += ret;
        } else {
            retry--;
            lenbuf = 0;
            if (retry == 0) {
                ret = -EFAIL;
                ps_print_err("exception recv memdump time out\n");
                break;
            }
        }
    }
    return ret;
}

OAL_STATIC int32_t gt_read_device_mem(unsigned long addr, uint8_t *data_buf,
    uint32_t read_size)
{
    uint8_t buf_tx[SEND_BUF_LEN];
    int32_t ret = OAL_SUCC;

    memset_s(buf_tx, SEND_BUF_LEN, 0, SEND_BUF_LEN);
    ret = snprintf_s(buf_tx, sizeof(buf_tx), sizeof(buf_tx) - 1, "%s%c0x%lx%c%u%c",
                        RMEM_CMD_KEYWORD,
                        COMPART_KEYWORD,
                        addr,
                        COMPART_KEYWORD,
                        read_size,
                        COMPART_KEYWORD);
    if (ret < 0) {
        ps_print_err("log str format err line[%d]\n", __LINE__);
        return ret;
    }
    ps_print_dbg("read mem cmd:[%s]\n", buf_tx);
    send_msg(buf_tx, os_str_len(buf_tx), FILES_CMD_SEND);
    return gt_exception_recv_device_memdump(data_buf, read_size);
}

OAL_STATIC void gt_exception_save_mem_to_file(struct st_wifi_dump_mem_info *mem_dump_info,
    uint8_t *data_buf, int32_t data_buf_size, struct file *fp)
{
    uint32_t remainder = mem_dump_info->size;
    uint32_t size = 0;
    uint32_t offset = 0;
    int32_t ret;

    while (remainder > 0) {
        size = (remainder > data_buf_size) ? data_buf_size : remainder;
        ret = gt_read_device_mem(mem_dump_info->mem_addr + offset, data_buf, size);
        if (ret < 0) {
            ps_print_err("gt mem dump fail, filename is [%s],ret=%d\n", mem_dump_info->file_name, ret);
            return;
        }
        ret = oal_kernel_file_write(fp, data_buf, size);
        if (ret < 0) {
            oal_io_print("vfs write failed!");
            break;
        }
#ifdef CONFIG_PRINTK
        if (offset == 0) {
            gt_exception_dump_device_panic_mem(mem_dump_info, data_buf, size);
        }
#endif
        offset += size;
        remainder -= size;
    }
}

OAL_STATIC int32_t gt_exception_save_panic_file(const char *file_name, struct st_wifi_dump_mem_info *mem_dump_info,
    uint8_t *data_buf, int32_t data_buf_size)
{
    struct file *fp = NULL;
    mm_segment_t fs;
#define FILE_NAME_LEN_MAX 256
    uint8_t open_file_name[FILE_NAME_LEN_MAX];
    memset_s(open_file_name, sizeof(open_file_name), 0, sizeof(open_file_name));
    if (strcpy_s(open_file_name, FILE_NAME_LEN_MAX, PANIC_FILE_CFG_DIR) != EOK) {
        goto securec_err;
    }
    if (strcat_s(open_file_name, FILE_NAME_LEN_MAX - strlen(PANIC_FILE_CFG_DIR), "/") != EOK) {
        goto securec_err;
    }
    if (strcat_s(open_file_name, FILE_NAME_LEN_MAX - strlen(PANIC_FILE_CFG_DIR) - 1, file_name) != EOK) {
        goto securec_err;
    }
    ps_print_info("save painc file [%s]\n", open_file_name);
    fs = get_fs();
    set_fs(KERNEL_DS);
    fp = filp_open(open_file_name, O_RDWR | O_CREAT, SAVE_FILE_PRIVILEGE_NUM);
    if (oal_is_err_or_null(fp)) {
        oal_io_print("create file error,fp = 0x%p, filename is [%s]\n", fp, open_file_name);
        set_fs(fs);
        return -OAL_EINVAL;
    }
    gt_exception_save_mem_to_file(mem_dump_info, data_buf, data_buf_size, fp);
#if (KERNEL_VERSION(2, 6, 35) <= LINUX_VERSION_CODE)
    vfs_fsync(fp, 0);
#else
    vfs_fsync(fp, fp->f_path.dentry, 0);
#endif

    filp_close(fp, NULL);
    set_fs(fs);
    return OAL_SUCC;
securec_err:
     ps_print_err("%s strcpy_s/strcat_s err!\n", __FUNCTION__);
     return OAL_FAIL;
}

int32_t gt_exception_dump_original_file(hcc_bus *hi_bus, uint8_t *data_buf, uint32_t data_buf_size)
{
    int i;
    int32_t count = hi_bus->mem_size;
    struct st_wifi_dump_mem_info *mem_dump_info = hi_bus->mem_info;
    int32_t ret = OAL_FAIL;

    if (mem_dump_info == NULL) {
        ps_print_info("gt exception mem dump info not init\n");
        return ret;
    }
    for (i = 0; i < count; i++) {
        ps_print_info("mem dump data size [%d]\n",  mem_dump_info[i].size);
        ret = gt_exception_save_panic_file(mem_dump_info[i].file_name, &mem_dump_info[i], data_buf, data_buf_size);
        if (ret < 0) {
            ps_print_info("mem dump file %s fail\n",  mem_dump_info[i].file_name);
            break;
        }
    }
    return ret;
}
#endif

int32_t gt_exception_work_submit(hcc_bus *hi_bus, uint32_t wifi_excp_type)
{
    struct st_exception_info *pst_exception_data = get_exception_info_reference();
    if ((pst_exception_data == NULL) || (hi_bus == NULL)) {
        ps_print_err("get exception info reference is error\n");
        return -EXCEPTION_FAIL;
    }

#ifdef _PRE_WLAN_FEATURE_DFR
    if (oal_work_is_busy(&pst_exception_data->gt_excp_worker)) {
        ps_print_warning("GT DFR %pF Worker is Processing:doing gt excp_work...need't submit\n",
                         (void *)pst_exception_data->gt_excp_worker.func);
        return -OAL_EBUSY;
    } else if (oal_work_is_busy(&pst_exception_data->gt_excp_recovery_worker)) {
        ps_print_warning(
            "GT DFR %pF Recovery_Worker is Processing:doing gt gt_excp_recovery_worker ...need't submit\n",
            (void *)pst_exception_data->gt_excp_recovery_worker.func);
        return -OAL_EBUSY;
    } else if ((in_plat_exception_reset() == OAL_TRUE) && (pst_exception_data->subsys_type == SUBSYS_GT)) {
        ps_print_warning("GT DFR %pF Recovery_Worker is Processing:doing  plat wifi recovery ...need't submit\n",
                         (void *)pst_exception_data->gt_excp_worker.func);
        return -OAL_EBUSY;
    } else {
        int32_t ret = -OAL_EFAIL;
        ps_print_err("gt exception bus type %d\n", hi_bus->bus_type);
#ifdef _PRE_PLAT_FEATURE_USB
        /* USB总线时，DEVICE挂死，需要等待device reset, USB总线才可用,HOST USB重新枚举需要时间 */
        if (hi_bus->bus_type == HCC_BUS_USB) {
            oal_msleep(20000); /* 等待20000ms，防止枚举时间不够 */
        }
#endif
        ret = wifi_exception_mem_dump(hi_bus);
        if (ret < 0) {
            ps_print_err("Panic File Save Failed!");
        } else {
            ps_print_info("Panic File Save OK!");
        }

        /* 为了在没有开启DFR  的情况下也能mem dump, 故在此处作判断 */
        if (pst_exception_data->exception_reset_enable != OAL_TRUE) {
            ps_print_info("plat exception reset not enable!");
            return EXCEPTION_SUCCESS;
        }

        ps_print_err("GT DFR %pF Worker Submit, excp_type[%d]\n",
                     (void *)pst_exception_data->gt_excp_worker.func, wifi_excp_type);
        pst_exception_data->gt_excp_type = wifi_excp_type;
        hcc_bus_disable_state(hi_bus, OAL_BUS_STATE_ALL);
        queue_work(pst_exception_data->gt_exception_workqueue, &pst_exception_data->gt_excp_worker);
    }
#else
    ps_print_warning("Geting GT DFR, but _PRE_WLAN_FEATURE_DFR not open!");
#endif  // _PRE_WLAN_FEATURE_DFR
    return OAL_SUCC;
}

int32_t plat_gt_exception_rst_register(uintptr_t data)
{
    struct st_wifi_dfr_callback *pst_gt_callback = NULL;

    struct st_exception_info *pst_exception_data = get_exception_info_reference();
    if (pst_exception_data == NULL) {
        ps_print_err("gt get exception info reference is error\n");
        return -EXCEPTION_FAIL;
    }

    if (data == 0) {
        ps_print_err("gt exception param data is null\n");
        return -EXCEPTION_FAIL;
    }

    /* wifi异常回调函数注册 */
    pst_gt_callback = (struct st_wifi_dfr_callback *)data;
    pst_exception_data->gt_callback = pst_gt_callback;

    return EXCEPTION_SUCCESS;
}
EXPORT_SYMBOL_GPL(plat_gt_exception_rst_register);

int32_t gt_device_mem_dump(hcc_bus *hi_bus, int32_t excep_type)
{
    int32_t ret = -EFAIL;
    uint8_t *data_buf = NULL;
    uint32_t sdio_transfer_limit = hcc_get_max_trans_size(hcc_get_handler(HCC_EP_GT_DEV));
    struct st_wifi_dump_mem_info *mem_dump_info = hi_bus->mem_info;

    if (!ps_is_device_log_enable()) {
        return 0;
    }
#ifdef _PRE_PLAT_FEATURE_USB
/* bootloader bulk size大小为512  */
#define USB_PATHC_READMEM_SIZE 512
    if (hi_bus->bus_type == HCC_BUS_USB) {
        sdio_transfer_limit = USB_PATHC_READMEM_SIZE;
    }
#endif
    sdio_transfer_limit = oal_min(PAGE_SIZE, sdio_transfer_limit);
    if ((mem_dump_info == NULL) || (sdio_transfer_limit == 0)) {
        ps_print_err("pst_wifi_dump_info is NULL, or sdio_transfer_limit is 0\n");
        return -EFAIL;
    }

    do {
        ps_print_info("try to malloc mem dump buf len is [%d]\n", sdio_transfer_limit);
        data_buf = (uint8_t *)os_kmalloc_gfp(sdio_transfer_limit);
        if (data_buf == NULL) {
            ps_print_warning("malloc mem  len [%d] fail, continue to try in a smaller size\n", sdio_transfer_limit);
            sdio_transfer_limit = sdio_transfer_limit >> 1;
        }
    } while ((data_buf == NULL) && (sdio_transfer_limit >= MIN_FIRMWARE_FILE_TX_BUF_LEN));

    if (data_buf == NULL) {
        ps_print_err("data_buf KMALLOC failed\n");
        return -ENOMEM;
    }

    ps_print_info("mem dump data buf len is [%d]\n", sdio_transfer_limit);
#ifdef _PRE_PLATFORM_SAVE_UNCOMPRESS_PANIC_FILE
    ret = gt_exception_dump_original_file(hi_bus, data_buf, sdio_transfer_limit);
    oal_reference(gt_exception_dump_compress_panic_file);
#else
    ret = gt_exception_dump_compress_panic_file(mem_dump_info, hi_bus, data_buf, sdio_transfer_limit);
#endif
    os_mem_kfree(data_buf);
    return ret;
}

