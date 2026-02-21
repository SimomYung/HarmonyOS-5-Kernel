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

#ifndef __DUMP_LOG_STRATEGY_H__
#define __DUMP_LOG_STRATEGY_H__

#include <osl_types.h>
#include <bsp_dump.h>
#include <bsp_dt.h>
#include <dump_subsys.h>

#define DUMP_FILE_NAME_MAX_SIZE (32)
#define DUMP_LOG_STRATEGY_ATTR_SIZE (3)
#define DUMP_FILE_TRANS_MAGIC (0x56786543)
#define DUMP_FILE_LINKS_MAGIC (0xcdefabcd)
#define DUMP_FILE_SYS_OFFSET 28
#define SUBSYS_COMM_ATTR 0xf
#define DUMP_FILE_NAME_LEN 28

#define DUMP_ATTR_SAVE_DEFAULT BIT(0)
#define DUMP_ATTR_SAVE_EXTRA_MDM BIT(1)
#define DUMP_ATTR_SAVE_MBB BIT(2)
#define DUMP_ATTR_SAVE_PHONE BIT(3)
#define DUMP_ATTR_SAVE_MINIDUMP BIT(4)
#define DUMP_ATTR_SAVE_FULLDUMP BIT(5)
#define DUMP_ATTR_SAVE_FEATURE_LTEV BIT(6)
#define DUMP_ATTR_SAVE_DDR_UNSEC_ACCESS BIT(7)
#define DUMP_ATTR_SAVE_SEC_DUMP BIT(8)
#define DUMP_ATTR_NOT_SAVE_TO_FS BIT(9)
#define DUMP_ATTR_UPLOAD BIT(10)
#define DUMP_ATTR_CRYPTO BIT(11)

#define DUMP_ATTR_DECODE_PLAT(attr) ((attr) & (DUMP_ATTR_SAVE_EXTRA_MDM | DUMP_ATTR_SAVE_MBB | DUMP_ATTR_SAVE_PHONE))
#define DUMP_ATTR_DECODE_DUMP_LEVEL(attr) ((attr) & (DUMP_ATTR_SAVE_MINIDUMP | DUMP_ATTR_SAVE_FULLDUMP))
#define DUMP_ATTR_DECODE_FEATURE(attr) ((attr) & (DUMP_ATTR_SAVE_DDR_UNSEC_ACCESS | DUMP_ATTR_SAVE_SEC_DUMP))
#define DUMP_ATTR_DECODE_DDR_ACCESS(attr) ((attr) & DUMP_ATTR_SAVE_DDR_UNSEC_ACCESS)
#define DUMP_ATTR_DECODE_NOT_SAVE_TO_FLASH(attr) ((attr) & DUMP_ATTR_NOT_SAVE_TO_FS)
#define DUMP_ATTR_DECODE_UPLOAD(attr) ((attr) & DUMP_ATTR_UPLOAD)

#define DUMP_MATCH_ATTRIBUTE(except_attr, curr_attr) (((except_attr) & (curr_attr)) == (except_attr))
#define DUMP_EXISTS_ATTRIBUTE(except_attr, curr_attr) (((except_attr) & (curr_attr)) != 0)
#define DUMP_SEC_ATTR_MATCH(except_attr, curr_attr) \
    (((except_attr) & (curr_attr)) == (DUMP_ATTR_SAVE_DDR_UNSEC_ACCESS | DUMP_ATTR_SAVE_SEC_DUMP))

struct dump_dstname_list {
    char filename[DUMP_FILE_NAME_MAX_SIZE];
};

struct dump_file_save_info_s {
    u32 magic; /* 0x56786543 */
    u32 links; /* 0xcdefabcd */
    char filename[DUMP_FILE_NAME_MAX_SIZE];
    char dstfilename[DUMP_FILE_NAME_MAX_SIZE];
    u32 filenum;
    u32 totalnum;
    u32 packetnum;   /* 包序号 */
    u32 totalpacket; /* 总包数 */
    u32 filelength;  /* 文件实际长度 */
    u32 realLength;  /* 单个包实际长度 */
    u32 isAppend;    /* 是否增加文件 */
    u32 maxLength;   /* 文件最大长度 */
};

struct dump_file_save_strategy {
    u32 is_compress;
    u32 is_append;
    struct dump_file_save_info_s data_head;
};

typedef struct {
    dump_save_file_id_e id;
    bool save_flag;
    u32 attr;
    char name[DUMP_FILE_NAME_LEN];
} dump_file_attr_s;

typedef struct {
    bool inited;
    u32 current_attr;
    /* file_cfg */
    u32 file_num;
    u32 list_len;
    struct dump_file_save_strategy *log_strategy;
    /* link_compress */
    bool link_enable;
    u32 log_sum;
    dump_file_attr_s *file_list;
} dump_file_info_s;

bool dump_get_log_strategy_init_state(void);
int dump_log_strategy_init(void);
int dump_file_attr_init(device_node_s *dev);
bool dump_get_link_status(void);
bool dump_is_minidump(void);
bool dump_is_fulldump(void);
bool dump_is_secdump(void);
bool dump_is_file_need_save(dump_save_file_id_e id);
bool dump_file_is_secdump_attr(dump_save_file_id_e id);
u32 dump_get_save_log_prio(dump_save_file_id_e id);
dump_access_mdmddr_type_e dump_get_access_mdmddr_type(void);
int dump_match_log_strategy(const char *file_name, struct dump_file_save_strategy *output);
enum dump_subsys dump_get_file_subsys(dump_save_file_id_e id);
bool dump_get_file_save_flag(dump_save_file_id_e id);
const dump_file_attr_s *dump_get_file_attr_by_name(const char *file_name);
dump_file_attr_s *dump_get_file_attr(dump_save_file_id_e id);

#endif
