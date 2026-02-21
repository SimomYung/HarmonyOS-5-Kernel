/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
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
#include <securec.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/wait.h>
#include <linux/mutex.h>
#include <linux/syscalls.h>
#include <linux/err.h>
#include <osl_types.h>
#include <osl_list.h>
#include <osl_malloc.h>
#include <bsp_print.h>
#include <bsp_slice.h>
#include <bsp_file_ops.h>
#include <dump_area.h>
#include <dump_frame.h>
#include "dump_logs.h"
#include "dump_log_strategy.h"
#include "dump_log_parser.h"

/* 该文件进行日志明文化，由于原始log日志是root权限的，且用户态app不能为root进程，因此当前在kernel中直接写日志
 * 后续log权限不为root后，需要迁移到到用户态
 */
#undef THIS_MODU
#define THIS_MODU mod_dump

struct parsefile_info_s {
    int file_len;
    int parse_flag;
    char file_name[DUMP_FILE_NAME_MAX_SIZE];
};

#define BASE64GROUP 3
#define DUMP_PARSE_U32_NUM_PER_LINE 4
#define DUMP_PARSE_RAW_DATA_LEN_PER_LINE 40
#define DUMP_PARSE_SINGLE_FILE_MAX (1 * 1024 * 1024) // 1MB

#ifdef CONFIG_HISI_MODEM_OHOS

static void parse_save_remain(struct file *poutfile, char *temp_char, int max_len, const u32 *data, int remain_count,
                              loff_t pos)
{
    int group_num = remain_count / sizeof(u32);
    int ret = -1;
    remain_count = remain_count % sizeof(u32);
    switch (group_num) {
        case 1:
            ret = snprintf_s(temp_char, max_len, max_len - 1, "%x\n", data[0]);
            break;
        case 2: /* 2 is group index */
            ret = snprintf_s(temp_char, max_len, max_len - 1, "%x %x\n", data[0], data[1]);
            break;
        case 3: /* 3 is group index */
            ret = snprintf_s(temp_char, max_len, max_len - 1, "%x %x %x\n", data[0], data[1], data[2]); /* 2 is index */
            break;
        default:
            break;
    }
    if (ret > 0) {
        (void)kernel_write(poutfile, temp_char, (size_t)ret, &pos);
    } else {
        dump_error("ret = %d\n", ret);
        (void)kernel_write(poutfile, "ERROR\n", sizeof("ERROR\n"), &pos);
    }
    if (remain_count != 0) {
        data += group_num;
        dump_print("remain_count %x\n", remain_count);
        (void)kernel_write(poutfile, data, (size_t)remain_count, &pos);
    }
}

static void parsefiledbin(struct file *poutfile, const u8 *addr, u32 size, loff_t pos)
{
    int cnt, group_num, remain_count, file_len, ret;
    char temp_char[DUMP_PARSE_RAW_DATA_LEN_PER_LINE];
    const u32 *data = (const u32 *)addr;

    if (size == 0) {
        return;
    }
    group_num = size / (DUMP_PARSE_U32_NUM_PER_LINE * sizeof(u32));
    remain_count = size % (DUMP_PARSE_U32_NUM_PER_LINE * sizeof(u32));
    file_len = size;
    ret = snprintf_s(temp_char, sizeof(temp_char), sizeof(temp_char) - 1, "\nlen%x\n", file_len);
    if (ret > 0) {
        (void)kernel_write(poutfile, temp_char, (size_t)ret, &pos);
    } else {
        dump_error("temp char err\n");
    }
    for (cnt = 0; cnt < group_num; cnt++) {
        ret = snprintf_s(temp_char, sizeof(temp_char), sizeof(temp_char) - 1, "%x %x %x %x\n", data[0], data[1],
                         data[2], data[3]); /* 2,3 is offset in line */
        if (ret > 0) {
            (void)kernel_write(poutfile, temp_char, (size_t)ret, &pos);
        } else {
            dump_error("ret = %d\n", ret);
            (void)kernel_write(poutfile, "ERROR\n", sizeof("ERROR\n"), &pos);
        }
        data += DUMP_PARSE_U32_NUM_PER_LINE;
    }
    if (remain_count != 0) {
        parse_save_remain(poutfile, temp_char, sizeof(temp_char), data, remain_count, pos);
    }

    return;
}

void parse_modem_dump_data(struct file *fd, const u8 *mntn_addr, u32 size, loff_t pos)
{
    parsefiledbin(fd, mntn_addr, size, pos);
}

void parse_modem_dump(const char *path)
{
    int ret;
    struct parsefile_info_s outfile_info = { 0 };
    struct dump_global_area_ctrl_s global_area = { 0 };
    char file_name[MODEM_DUMP_FILE_NAME_LENGTH] = {0};
    struct file *fd = NULL;
    loff_t pos, old_pos;
    unsigned long fs;

    if (snprintf_s(file_name, MODEM_DUMP_FILE_NAME_LENGTH, MODEM_DUMP_FILE_NAME_LENGTH - 1, "%s%s",
                    path, "modem_dump.txt") < 0) {
        dump_error("er\n");
        return;
    }

    ret = dump_get_global_info(&global_area);
    if (ret != BSP_OK || global_area.virt_addr == NULL) {
        dump_print("no modem mntn data\n");
        return;
    }
    outfile_info.file_len = global_area.length;
    outfile_info.parse_flag = 1;
    if (strncpy_s(outfile_info.file_name, DUMP_FILE_NAME_MAX_SIZE, "modem_dump.txt", strlen("modem_dump.txt")) != 0) {
        dump_print("build outfile err\n");
    }
    fd = dump_open((const char *)file_name, O_CREAT | O_RDWR, DUMP_LOG_FILE_AUTH);
    if (fd == NULL) {
        dump_error("open %s error!\n", file_name);
        return;
    }

    fs = get_fs();
    set_fs((mm_segment_t)KERNEL_DS);

    pos = vfs_llseek(fd, 0, SEEK_END);
    old_pos = pos;
    (void)kernel_write(fd, &outfile_info, sizeof(outfile_info), &pos);

    parse_modem_dump_data(fd, (const u8 *)global_area.virt_addr, global_area.length, pos);

    pos = vfs_llseek(fd, 0, SEEK_END);
    outfile_info.file_len = (pos - old_pos) > 0 ? (pos - old_pos) : 0;
    (void)kernel_write(fd, &outfile_info, sizeof(outfile_info), &old_pos);
    (void)vfs_fsync(fd, 0);

    set_fs(fs);

    dump_close(fd);
}

unsigned int dump_get_log_file_data_from_append_file(struct file *fd, const char *src_file,
                                                     struct dump_file_save_strategy *log_strategy)
{
    int ret;
    loff_t pos, file_max_len;
    u32 total_len;
    u32 ret_len = 0;

    file_max_len = vfs_llseek(fd, 0, SEEK_END);
    pos = vfs_llseek(fd, 0, SEEK_SET);
    ret = kernel_read(fd, &log_strategy->data_head, sizeof(log_strategy->data_head), &pos);
    while (ret == sizeof(log_strategy->data_head) && (log_strategy->data_head.magic == DUMP_FILE_TRANS_MAGIC)){
        total_len = log_strategy->data_head.filelength;
        if (strncmp(src_file, log_strategy->data_head.filename, strnlen(src_file, DUMP_FILE_NAME_MAX_SIZE)) == 0) {
            vfs_llseek(fd, pos, SEEK_SET);
            ret_len = total_len;
            break;
        }
        if (file_max_len - pos <= total_len) {
            return 0;
        }
        pos += total_len;
        ret = kernel_read(fd, &log_strategy->data_head, sizeof(log_strategy->data_head), &pos);
    }
    return ret_len;
}

struct file *dump_get_log_file_data(const char *path, const char *src_file, u32 *file_len)
{
    int ret;
    struct file *fd = NULL;
    struct dump_file_save_strategy log_strategy = { 0 };
    char file_name[MODEM_DUMP_FILE_NAME_LENGTH] = { 0 };
    unsigned long fs;
    u32 total_len = 0;

    ret = dump_match_log_strategy(src_file, &log_strategy);
    fs = get_fs();
    set_fs((mm_segment_t)KERNEL_DS);
    if (ret) {
        if (snprintf_s(file_name, MODEM_DUMP_FILE_NAME_LENGTH, strnlen(path, MODEM_DUMP_FILE_NAME_LENGTH) +
                       strnlen(src_file, DUMP_FILE_NAME_MAX_SIZE), "%s%s", path, src_file) < 0) {
            dump_error("er\n");
            goto out;
        }
        fd = dump_open((char *)file_name, O_RDONLY, DUMP_LOG_FILE_AUTH);
        if (fd != NULL) {
            total_len = vfs_llseek(fd, 0, SEEK_END);
            vfs_llseek(fd, 0, SEEK_SET);
        }
    } else {
        if (snprintf_s(file_name, MODEM_DUMP_FILE_NAME_LENGTH, strnlen(path, MODEM_DUMP_FILE_NAME_LENGTH) +
                       strnlen(log_strategy.data_head.dstfilename, DUMP_FILE_NAME_MAX_SIZE), "%s%s", path,
                       log_strategy.data_head.dstfilename) < 0) {
            dump_error("er\n");
            goto out;
        }
        fd = dump_open((char *)file_name, O_RDONLY, DUMP_LOG_FILE_AUTH);
        if (fd == NULL) {
            goto out;
        }
        total_len = dump_get_log_file_data_from_append_file(fd, src_file, &log_strategy);
        if (total_len == 0) {
            dump_close(fd);
            fd = NULL;
        }
    }
out:
    set_fs(fs);
    *file_len = total_len;
    return fd;
}

static inline unsigned int dump_parse_bin_base64(char *out_buf, u32 out_buf_size, const u8 *data, unsigned int size)
{
    unsigned int temp_data = 0;
    int i;
    if (out_buf_size < size + 1) {
        return 0;
    }
    for (i = 0; i < (int)size; i++) {
        temp_data = (temp_data << 8) + data[i]; /* 1字节占用8bit */
    }
    for (i = (int)size; i >= 0; i--) {
        out_buf[i] = '?' + (temp_data & 0x3F); /* 6bit mask */
        temp_data = temp_data >> 6; /* 24bit->32bit, 一次处理6bit */
    }
    return size + 1;
}

void dump_parse_other_file_core(const char *log_path, const char *src_file, const char *dst_file, const u8 *data,
                                u32 file_len)
{
    struct parsefile_info_s outfile_info = { 0 };
    unsigned int cnt, group_num, remain_count, buf_idx;
    char file_name[MODEM_DUMP_FILE_NAME_LENGTH] = {0};
    struct file *poutfile = NULL;
    loff_t pos;
    unsigned long fs, ret;

    if (snprintf_s(file_name, MODEM_DUMP_FILE_NAME_LENGTH, MODEM_DUMP_FILE_NAME_LENGTH - 1, "%s%s", log_path,
                   dst_file) < 0) {
        return;
    }
    poutfile = dump_open((const char *)file_name, O_CREAT | O_RDWR | O_APPEND, DUMP_LOG_FILE_AUTH);
    if (poutfile == NULL) {
        dump_error("open %s error!\n", file_name);
        return;
    }
    group_num = file_len / BASE64GROUP;
    remain_count = file_len % BASE64GROUP;
    outfile_info.file_len =
        group_num * (BASE64GROUP + 1) + (remain_count == 0 ? 0 : remain_count + 1) + sizeof(outfile_info);
    outfile_info.parse_flag = 1;
    if (strncpy_s(outfile_info.file_name, DUMP_FILE_NAME_MAX_SIZE, src_file, DUMP_FILE_NAME_MAX_SIZE - 1) != EOK) {
        dump_error("err\n");
    }

    fs = get_fs();
    set_fs((mm_segment_t)KERNEL_DS);

    pos = vfs_llseek(poutfile, 0, SEEK_END);
    (void)kernel_write(poutfile, &outfile_info, sizeof(outfile_info), &pos);
    cnt = 0;
    buf_idx = 0;
    while (cnt < file_len) {
        remain_count = file_len - cnt;
        group_num = (remain_count >= BASE64GROUP) ? BASE64GROUP : remain_count;
        ret = dump_parse_bin_base64(&file_name[buf_idx], MODEM_DUMP_FILE_NAME_LENGTH - buf_idx, &data[cnt], group_num);
        if (ret != 0) {
            buf_idx += ret;
            cnt += group_num;
        } else {
            (void)kernel_write(poutfile, file_name, buf_idx, &pos);
            buf_idx = 0;
        }
    }
    if (buf_idx != 0) {
        (void)kernel_write(poutfile, file_name, buf_idx, &pos);
    }

    (void)vfs_fsync(poutfile, 0);
    set_fs(fs);
    dump_close(poutfile);
}

void dump_parse_other_file(const char *log_path, const char *src_file, const char *dst_file)
{
    u32 file_len = 0;
    struct file *fd_src = NULL;
    int ret;
    loff_t pos;
    u8 *data = NULL;
    unsigned long fs;

    fd_src = dump_get_log_file_data(log_path, src_file, &file_len);
    if (fd_src == NULL) {
        dump_error("%s not exist", src_file);
        return;
    }
    if ((file_len == 0) || (file_len > DUMP_PARSE_SINGLE_FILE_MAX)) {
        dump_close(fd_src);
        dump_error("%s len err %d\n", src_file, file_len);
        return;
    }

    data = vmalloc(file_len);
    if (data == NULL) {
        dump_close(fd_src);
        dump_error("malloc fail\n");
        return;
    }

    fs = get_fs();
    set_fs((mm_segment_t)KERNEL_DS);
    pos = vfs_llseek(fd_src, 0, SEEK_CUR);
    ret = kernel_read(fd_src, data, file_len, &pos);
    set_fs(fs);
    dump_close(fd_src);
    if (ret != (int)file_len) {
        vfree(data);
        dump_error("read %s fail %d\n", src_file, ret);
        return;
    }

    dump_parse_other_file_core(log_path, src_file, dst_file, data, file_len);
    vfree(data);
    return;
}

void dump_parse_all_logs(const char *log_path)
{
    char file_name[MODEM_DUMP_FILE_NAME_LENGTH] = {0};
    if (log_path == NULL) {
        dump_error("logpath NULL\n");
        return;
    }

    parse_modem_dump(log_path);
    /* 已下文件目前需要从log里读取然后明文化，下个版本直接对原始数据流进行明文化
     * 当前虽然要读取文件，但原始log文件是root-system的，且只有root有写权限，不会增加风险
     */
    dump_parse_other_file(log_path, "logmem.bin", "modem_dump.txt");
    dump_parse_other_file(log_path, "phy_dump.bin", "phy_dump.txt");
    dump_parse_other_file(log_path, "rfdsp_dump.bin", "rfdsp_dump.txt");

    if (snprintf_s(file_name, MODEM_DUMP_FILE_NAME_LENGTH, MODEM_DUMP_FILE_NAME_LENGTH - 1, "%s%s", log_path,
                   "DONE") < 0) {
        return;
    }
    dump_create_file(file_name);
}

#else

void dump_parse_all_logs(const char *log_path)
{
    return;
}
#endif
