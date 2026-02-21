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
#include <linux/kernel.h>
#include <linux/fcntl.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/version.h>
#include <linux/init_syscalls.h>
#include <securec.h>
#include <mdrv_memory_layout.h>
#include <osl_list.h>
#include <osl_malloc.h>
#include <bsp_dump.h>
#include <bsp_rfile.h>
#include <bsp_dt.h>
#include <bsp_file_ops.h>
#include <dump_area.h>
#include <dump_infra.h>
#include <dump_exc_mgr.h>
#include <dump_subsys_mgr.h>
#include "dump_logs.h"
#include "dump_extra_logs.h"
#include "dump_log_strategy.h"
#include "dump_log_parser.h"
#include "dump_log_uploader.h"
#include "dump_log_compress.h"

static int dump_check_path(const char *path)
{
    if (strlen(path) >= RDR_DUMP_DIR_PTH_LEN) {
        dump_error("path too long, len=%d\n", (int)strlen(path));
        return BSP_ERROR;
    }

    if (strstr(path, "../")) {
        dump_error("path is not allowed, len=%s\n", path);
        return BSP_ERROR;
    }

    return BSP_OK;
}

int dump_mkdir(const char *dir)
{
    int ret;
    unsigned long fs;
    int mode = DUMP_LOG_MBB_DIR_AUTH;

    ret = dump_check_path(dir);
    if (ret) {
        return ret;
    }

    if (bsp_dump_get_product_type() == DUMP_PHONE) {
        mode = DUMP_LOG_PHONE_DIR_AUTH;
    }

    fs = get_fs();
    set_fs((mm_segment_t)KERNEL_DS);
    ret = bsp_file_mkdir(dir, mode);
    set_fs(fs);
    if (ret && ret != -EEXIST) {
        dump_error("sys_mkdir fail,ret=%d\n",ret);
        return ret;
    }

    return BSP_OK;
}

struct file *dump_open(const char *path, int flags, int file_mode)
{
    s64 ret;
    unsigned long fs;
    char log_path[RDR_DUMP_FILE_PATH_LEN] = {0};
    char *p = NULL;
    struct file *fd = NULL;

    ret = strcpy_s(log_path, sizeof(log_path), path);
    if (ret) {
        dump_error("strcpy_s error, ret = %lld\n",ret);
        return NULL;
    }

    p = strrchr(log_path, '/');
    if ((p != NULL) && (p != log_path)) {
        /* 查看上一级目录是否存在，如果不存在则创建此目录 */
        *p = '\0';
        ret = dump_mkdir((const char *)log_path);
        if (ret) {
            dump_error("dump_mkdir fail, ret=%lld\n", ret);
        }
    }

    fs = get_fs();
    set_fs((mm_segment_t)KERNEL_DS);
    fd = filp_open(path, flags, file_mode);
    set_fs(fs);
    /* 调用dump_open的用户，只判断了是否为NULL，但linux返回的是错误码，因此这里需要转换一下 */
    if (IS_ERR_OR_NULL(fd)) {
        dump_error("get fd err, ret=0x%lx\n", PTR_ERR((const void *)fd));
        return NULL;
    }
    return fd;
}

int dump_close(struct file *fd)
{
    int ret;
    unsigned long fs;

    fs = get_fs();
    set_fs((mm_segment_t)KERNEL_DS);

    ret = filp_close(fd, NULL);

    set_fs(fs);

    return ret;
}

int dump_write_sync(struct file *fd, const s8 *ptr, u32 size)
{
    int ret = 0; /* 不保存又不上传的文件，默认返回0；但是不应该出现这种情况，除非dts里特意配置 */
    unsigned long fs;
    loff_t pos = 0;
    const dump_file_attr_s *attrp = dump_get_file_attr_by_name((const char *)fd->f_path.dentry->d_name.name);
    u32 uplaod_file_flag = (attrp == NULL) ? 0 : DUMP_ATTR_DECODE_UPLOAD(attrp->attr);
    u32 not_save_file_flag = (attrp == NULL) ? 0 : DUMP_ATTR_DECODE_NOT_SAVE_TO_FLASH(attrp->attr);

    if (!not_save_file_flag) {
        fs = get_fs();
        set_fs((mm_segment_t)KERNEL_DS);
        pos = vfs_llseek(fd, 0, SEEK_END);
        ret = (int)kernel_write(fd, ptr, (size_t)size, &pos);
        (void)vfs_fsync(fd, 0);
        set_fs(fs);
    } else if (uplaod_file_flag) {
        ret = size;
    }
    if (uplaod_file_flag) {
        dump_upload_file(attrp->id, fd, ptr, size);
    }

    return ret;
}

static int dump_lseek(struct file *fd, long offset, int whence)
{

    int ret;
    unsigned long old_fs;

    old_fs = get_fs();
    set_fs((mm_segment_t)KERNEL_DS);

    ret = vfs_llseek(fd, offset, (u32)whence);

    set_fs(old_fs);

    return ret;
}

static int dump_remove(const char *path)
{
    int ret;
    unsigned long fs;

    fs = get_fs();
    set_fs((unsigned long)KERNEL_DS);
    ret = do_unlinkat(AT_FDCWD, getname(path));
    set_fs(fs);

    return ret;
}

void dump_save_file(const char *file_name, const void *addr, const void *phy_addr, u32 len)
{
    int ret;
    struct file *fd = NULL;
    int bytes;

    UNUSED(phy_addr);
    if (file_name == NULL || addr == NULL || len == 0) {
        return;
    }

    fd = dump_open(file_name, O_CREAT | O_RDWR | O_SYNC, DUMP_LOG_FILE_AUTH);
    if (fd == NULL) {
        dump_error("fail to creat file %s \n", file_name);
        return;
    }

    bytes = dump_write_sync(fd, addr, len);

    if (bytes != len) {
        dump_error("write data to %s failed, bytes %d, len %d\n", file_name, bytes, len);
        (void)dump_close(fd);
        return;
    }

    ret = dump_close(fd);
    if (ret != 0) {
        dump_error("fail to close file, ret = %d\n", ret);
        return;
    }
}

int dump_append_file(const char *dir, const char *filename, const void *address, u32 length, u32 max_size)
{
    int ret = BSP_OK;
    struct file *fd = NULL;
    u32 bytes;
    int len;

    ret = dump_mkdir(dir);
    if (ret < 0) {
        dump_error("fail to create om dir ! ret = %d\n", ret);
        goto out;
    }
    fd = dump_open(filename, O_APPEND | O_CREAT | O_RDWR, DUMP_LOG_FILE_AUTH);
    if (fd == NULL) {
        dump_error("fail to open %s,while mode is create\n", filename);
        ret = BSP_ERROR;
        goto out;
    }
    len = dump_lseek(fd, 0, SEEK_END);
    if (len == BSP_ERROR) {
        dump_error("fail to seek ! ret = %d\n", len);
        ret = BSP_ERROR;
        goto out1;
    }

    if ((len + length) >= max_size) {
        (void)dump_close(fd);
        ret = dump_remove(filename);
        if (ret != BSP_OK) {
            dump_error("fail to remove ! ret = %d\n", ret);
            goto out;
        }
        fd = dump_open(filename, O_CREAT | O_RDWR, DUMP_LOG_FILE_AUTH);
        if (fd == NULL) {
            dump_error("fail to create %s !\n", filename);
            ret = BSP_ERROR;
            goto out;
        }
    }

    bytes = (u32)dump_write_sync(fd, address, length);
    if (bytes != length) {
        dump_error("fail to write data ! ret = %d\n", bytes);
        ret = BSP_ERROR;
        goto out1;
    }
    (void)dump_close(fd);
    return BSP_OK;

out1:
    (void)dump_close(fd);
out:
    return ret;
}

int bsp_dump_log_save(const char *dir, const char *file_name, const void *address, const void *phy_addr, u32 length)
{
    return dump_log_save(dir, file_name, address, phy_addr, length);
}
EXPORT_SYMBOL(bsp_dump_log_save);

int bsp_dump_log_append(const char *dir, const char *file_name, const void *address, const void *phy_addr, u32 length)
{
    char dst_name[MODEM_DUMP_FILE_NAME_LENGTH] = {
        0,
    };
    if (dir == NULL || file_name == NULL || address == NULL || length == 0) {
        dump_error("input err\n");
        return BSP_ERROR;
    }
    if (snprintf_s(dst_name, sizeof(dst_name), sizeof(dst_name) - 1, "%s%s", dir, file_name) < 0) {
        dump_error("snpf err\n");
        return BSP_ERROR;
    }
    return dump_append_file(dir, dst_name, address, length, 0xFFFFFFFF);
}
EXPORT_SYMBOL(bsp_dump_log_append);

int dump_log_save(const char *dir, const char *file_name, const void *address, const void *phy_addr, u32 length)
{
    bool link_enble = dump_get_link_status();
    char dst_name[MODEM_DUMP_FILE_NAME_LENGTH] = {
        0,
    };
    struct dump_file_save_strategy log_strategy;
    int ret = BSP_ERROR;

    if (dir == NULL || file_name == NULL || address == NULL || length == 0) {
        dump_error("input err\n");
        return BSP_ERROR;
    }
    if (link_enble == false) {
        goto no_link;
    }
    /* log append */
    ret = dump_match_log_strategy(file_name, &log_strategy);
    if (ret) {
        goto no_link;
    }
    log_strategy.data_head.filelength = length;
    /* append log */
    if (0 >
        snprintf_s(dst_name, sizeof(dst_name), sizeof(dst_name) - 1, "%s%s", dir, log_strategy.data_head.dstfilename)) {
        bsp_info("snp err,no link\n");
        goto no_link;
    }
    /* append head */
    if (dump_append_file(dir, dst_name, &log_strategy.data_head, sizeof(log_strategy.data_head), 0xFFFFFFFF)) {
        dump_error("add head err\n");
        return BSP_ERROR;
    }
    /* append data */
    if (dump_append_file(dir, dst_name, address, log_strategy.data_head.filelength, 0xFFFFFFFF)) {
        dump_error("add data err\n");
        return BSP_ERROR;
    }
    return BSP_OK;
no_link:
    if (snprintf_s(dst_name, sizeof(dst_name), sizeof(dst_name) - 1, "%s%s", dir, file_name) < 0) {
        dump_error("snpf err\n");
        return BSP_ERROR;
    }
    dump_save_file(dst_name, address, phy_addr, length);
    return BSP_OK;
}
/*
 * 功能描述: 创建文件接口
 */
int dump_create_file(const char *filename)
{
    struct file *fd = NULL;

    if (!filename) {
        return BSP_ERROR;
    }
    fd = dump_open(filename, O_CREAT | O_RDWR | O_SYNC, DUMP_LOG_FILE_AUTH);
    if (fd == NULL) {
        dump_error("fail to creat file %s \n", filename);
        return BSP_ERROR;
    }

    (void)vfs_fsync(fd, 0);
    (void)dump_close(fd);

    return BSP_OK;
}

/*
 * 功能描述: 创建cp_log的目录保证hidp等工具能够正常导log
 */
int dump_create_dir(const char *path)
{
    char *dir_name = NULL;
    u32 len;
    int ret = BSP_OK;

    if (path == NULL) {
        dump_error("path null\n");
        return BSP_ERROR;
    }
    len = strnlen(path, RDR_DUMP_DIR_PTH_LEN);
    if (len >= RDR_DUMP_DIR_PTH_LEN) {
        dump_error("path too long:%d\n", len);
        return BSP_ERROR;
    }
    dump_print("create dir path is %s\n", path);

    /* rfile创建目录不允许使用/结束路径 */
    if (path[len - 1] == '/') {
        dir_name = (char *)kmalloc((len + 1), GFP_KERNEL);
        if (dir_name == NULL) {
            dump_error("fail to malloc memry \n");
            return BSP_ERROR;
        }
        if (memset_s(dir_name, (len + 1), '\0', (len + 1)) != EOK) {
            dump_debug("err\n");
        }
        if (memcpy_s(dir_name, (len + 1), path, (len - 1)) != EOK) {
            dump_debug("err\n");
        }
        ret = dump_mkdir(dir_name);
    } else {
        ret = dump_mkdir(path);
    }

    if (dir_name != NULL) {
        kfree(dir_name);
    }
    if (ret < 0) {
        dump_error("fail to create dir , ret=0x%x\n ", ret);
    }

    return ret;
}

int dump_save_ddr_file(const char *dir_name, const char *ddr_region, const char *file_name, bool head_flag)
{
    unsigned long addr;
    unsigned int size = 0;
    unsigned int file_size = 0;
    void *vaddr = NULL;
    int ret;

    addr = mdrv_mem_region_get(ddr_region, &size);
    if (addr == 0 || size == 0) {
        dump_error("%s no exsit!\n", ddr_region);
        return BSP_OK;
    }
    vaddr = (char *)ioremap_wc((phys_addr_t)(uintptr_t)addr, size);
    if (vaddr == NULL) {
        dump_error("ioremap %s addr fail\n", ddr_region);
        return BSP_ERROR;
    }
    if (head_flag == true) {
        file_size = ((dump_log_head_s *)(uintptr_t)vaddr)->dump_size;
        size = (file_size > size) ? size : file_size;
    }
    ret = dump_log_save(dir_name, file_name, (u8 *)vaddr, (void *)((uintptr_t)addr), size);
    if (ret == BSP_ERROR) {
        dump_error("save %s failed\n", file_name);
        iounmap(vaddr);
        return BSP_ERROR;
    }
    iounmap(vaddr);
    return BSP_OK;
}

void dump_save_log_work(const dump_exc_log_desc_s *exc_info, void *priv_data)
{
    save_hook pfunc = (save_hook)priv_data;
    if (!dump_valid_exc_log_path(exc_info) || (priv_data == NULL)) {
        dump_error("param err\n");
        return;
    }
    pfunc((const char *)exc_info->log_path);

}

int bsp_dump_register_log_notifier(dump_save_file_id_e id, save_hook func)
{
    dump_work_s work;
    int ret;
    if (id >= DUMP_SAVE_LOG_BUTT || func == NULL) {
        dump_error("input error param\n");
        return BSP_ERROR;
    }

    if (!dump_get_log_strategy_init_state()) {
        dump_log_strategy_init();
    }

    if (!dump_is_subsys_drv_prepare_done()) {
        dump_subsys_drv_prepare_all();
    }

    if (!dump_is_file_need_save(id)) {
        dump_error("id=%d does not need to be saved\n", id);
        return BSP_OK;
    }

    work.work_type = DUMP_WORK_ASYNC;
    work.work_fn = dump_save_log_work;
    work.prior = dump_get_save_log_prio(id);
    work.priv_data = func;

    ret = dump_subsys_add_work(DUMP_SUBSYS_COMM, &work);
    if (!dump_is_err(ret)) {
        dump_print("file_id=%d,register ok!\n", id);
        ret = BSP_OK;
    } else {
        dump_print("file_id=%d,register fail %d!\n", id, ret);
        ret = BSP_ERROR;
    }
    return ret;
}
EXPORT_SYMBOL(bsp_dump_register_log_notifier);

__init void dump_logs_init(void)
{
    dump_log_parser_init();
    dump_logzip_init();
    dump_uploader_init();
    return;
}

int dump_logs_prepare_proc(const dump_exc_log_desc_s *exc_desc)
{
    int log_dir_status;
    if (!dump_valid_exc_log_path(exc_desc)) {
        return DUMP_ERRNO_INVA_PARAM;
    }

    // 首先使能工作目录：创建本地log目录，创建extra log目录，初始化uploader，并使能dump fs inst(后续按需实现，此后才允许文件写入)
    dump_uploader_prepare();
    log_dir_status = dump_create_dir((const char *)exc_desc->log_path);
    dump_prepare_extra_logs_log_path(exc_desc->log_path);
    if(log_dir_status != BSP_OK) {
        dump_error("creat dir error\n");
    }
    return DUMP_OK;
}

void dump_logs_teardown_proc(const dump_exc_log_desc_s *exc_desc)
{
    UNUSED(exc_desc);
    /* 重置upload file desc */
    dump_uploader_reset_proc();
}
