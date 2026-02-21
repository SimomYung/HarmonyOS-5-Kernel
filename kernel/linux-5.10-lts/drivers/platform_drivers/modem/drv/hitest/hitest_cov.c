/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2017-2020. All rights reserved.
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
#include <linux/module.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/file.h>
#include <linux/version.h>
#include <linux/syscalls.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#include <linux/time.h>
#include <linux/rtc.h>
#include <securec.h>
#include <bsp_file_ops.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define THIS_MODU mod_ecdc

#define HITEST_MSG_TYPE 0xf1fe
#define HITEST_STR_LEN 32
#define HITEST_DATA_LEN_EACH_TIME 0x2000
#define HITEST_FILE_NAME_LEN 256

#define hitest_err(fmt, ...) pr_err("[hitest]: " fmt, ##__VA_ARGS__)

struct cov_data_hdr {
    unsigned int msg_type;
    unsigned int data_len;
    char build_id[HITEST_STR_LEN];
    char project_name[HITEST_STR_LEN];
    unsigned char data[0];
};

extern const char* Hitest_Agent_GetBuildIDS(void);
extern const char* Hitest_Agent_GetPrjNameS(void);
extern unsigned long Hitest_Agent_GetCovDataSizeS(void);
extern int Hitest_Coverage_GetCoverageDataS(unsigned int *pCovData, unsigned long ulLen);
extern int Hitest_Coverage_InitCoverageData(void);
extern int Hitest_Coverage_GetCoverageLittleEndianData(unsigned int *pCovData, unsigned long ulLen);
extern int Hitest_Coverage_MergeCoverageData(unsigned int *pCovData, unsigned long ulLen);
extern int Hitest_Coverage_MergeCoverageLittleEndianData(unsigned int *pCovData, unsigned long ulLen);

extern int Hitest_Agent_GetStartTime(char *pStartTime);

int __attribute__((weak)) Hitest_Agent_GetStartTime(char *pStartTime)
{
    if((void *)0 == pStartTime) {
		return 0;
	}
    return 0;
}

int __attribute__((weak)) Hitest_Coverage_GetCoverageLittleEndianData(unsigned int *pCovData, unsigned long ulLen)
{
    int total = ulLen / sizeof(unsigned int);
    int i;

    for (i = 0; i < total; i++) {
        pCovData[i] = 0x4D41414D;
    }
    return 1;
}

int __attribute__((weak)) Hitest_Coverage_MergeCoverageData(unsigned int *pCovData, unsigned long ulLen)
{
    int total = ulLen / sizeof(unsigned int);
    int i;

    for (i = 0; i < total; i++) {
        pCovData[i] = 0x4D41414D;
    }
    return 1;
}

int __attribute__((weak)) Hitest_Coverage_MergeCoverageLittleEndianData(unsigned int *pCovData, unsigned long ulLen)
{
    int total = ulLen / sizeof(unsigned int);
    int i;

    for (i = 0; i < total; i++) {
        pCovData[i] = 0x4D41414D;
    }
    return 1;
}

const char*  __attribute__((weak)) Hitest_Agent_GetBuildIDS(void)
{
    return "hitest0";
}

const char*  __attribute__((weak)) Hitest_Agent_GetPrjNameS(void)
{
    return "hitest1";
}

unsigned long __attribute__((weak)) Hitest_Agent_GetCovDataSizeS(void)
{
    return 0x4000;
}

int __attribute__((weak)) Hitest_Coverage_GetCoverageDataS(unsigned int *pCovData, unsigned long ulLen)
{
    int total = ulLen / sizeof(unsigned int);
    int i;

    for (i = 0; i < total; i++) {
        pCovData[i] = 0x4D41414D;
    }
    return 1;
}

int __attribute__((weak)) Hitest_Coverage_InitCoverageData(void)
{
    return 0;
}

static int creat_time_str(char *time_str, unsigned int len)
{
    int ret;
    struct timespec64 tv;
    struct rtc_time tm;

    ktime_get_ts64(&tv);
    rtc_time64_to_tm(tv.tv_sec, &tm);

    ret = sprintf_s(time_str, len, "%04d", tm.tm_year + 1900);
    if (ret < 0) {
        hitest_err("fail sprintf_s time_str\n");
        return -1;
    }

    ret = sprintf_s(time_str + 4, len, "%02d", tm.tm_mon + 1);
    if (ret < 0) {
        hitest_err("fail sprintf_s time_str\n");
        return -1;
    }

    ret = sprintf_s(time_str + 6, len, "%02d", tm.tm_mday);
    if (ret < 0) {
        hitest_err("fail sprintf_s time_str\n");
        return -1;
    }

    ret = sprintf_s(time_str + 8, len, "%02d", tm.tm_hour);
    if (ret < 0) {
        hitest_err("fail sprintf_s time_str\n");
        return -1;
    }

    ret = sprintf_s(time_str + 10, len, "%02d", tm.tm_min);
    if (ret < 0) {
        hitest_err("fail sprintf_s time_str\n");
        return -1;
    }

    ret = sprintf_s(time_str + 12, len, "%02d", 0);
    if (ret < 0) {
        hitest_err("fail sprintf_s time_str\n");
        return -1;
    }

    hitest_err("time_str:%s\n", time_str);
    return 0;
}

int creat_file_name(char *file_name, struct cov_data_hdr *hdr, char *time_str, unsigned int len)
{
    int ret;
    ret = strcat_s(file_name, len, hdr->project_name);
    if (ret < 0) {
        hitest_err("fail strcat_s file_name:%s\n", file_name);
        return -1;
    }
    ret = strcat_s(file_name, len, "_");
    if (ret < 0) {
        hitest_err("fail strcat_s file_name:%s\n", file_name);
        return -1;
    }
    ret = strcat_s(file_name, len, hdr->build_id);
    if (ret < 0) {
        hitest_err("fail strcat_s file_name:%s\n", file_name);
        return -1;
    }
    ret = strcat_s(file_name, len, "_A");
    if (ret < 0) {
        hitest_err("fail strcat_s file_name:%s\n", file_name);
        return -1;
    }
    ret = strcat_s(file_name, len, time_str);
    if (ret < 0) {
        hitest_err("fail strcat_s file_name:%s\n", file_name);
        return -1;
    }
    ret = strcat_s(file_name, len, "_");
    if (ret < 0) {
        hitest_err("fail strcat_s file_name:%s\n", file_name);
        return -1;
    }
    ret = strcat_s(file_name, len, "0000");
    if (ret < 0) {
        hitest_err("fail strcat_s file_name:%s\n", file_name);
        return -1;
    }
    ret = strcat_s(file_name, len, "_");
    if (ret < 0) {
        hitest_err("fail strcat_s file_name:%s\n", file_name);
        return -1;
    }
    ret = strcat_s(file_name, len, "1234");
    if (ret < 0) {
        hitest_err("fail strcat_s file_name:%s\n", file_name);
        return -1;
    }
    hitest_err("file_name:%s\n", file_name);
    return 0;
}

void creat_cov_file(struct cov_data_hdr *hdr)
{
    mm_segment_t old_fs;
    struct file *fp = NULL;
    loff_t pos = 0;
    int ret;
    char file_name[HITEST_FILE_NAME_LEN] = "/data/hisi_logs/linuxavatar_";
    char time_str[HITEST_STR_LEN] = "";

    hitest_err("project_name:%s\n", hdr->project_name);
    hitest_err("build_id:%s\n", hdr->build_id);

    ret = creat_time_str(time_str, HITEST_STR_LEN);
    if (ret) {
        return;
    }

    ret = creat_file_name(file_name, hdr, time_str, HITEST_FILE_NAME_LEN);
    if (ret) {
        return;
    }

    old_fs = get_fs();
    set_fs((mm_segment_t)KERNEL_DS);
    ret = bsp_file_mkdir("/data/hisi_logs", 0750);
    set_fs(old_fs);
    if (ret && ret != -EEXIST) {
        hitest_err("mkdir file fail:%d\n", ret);
        return;
    }

    fp = filp_open(file_name, O_CREAT | O_RDWR | O_APPEND, 0);
    if (IS_ERR_OR_NULL(fp)) {
        hitest_err("open file fail:0x%lx\n", PTR_ERR(fp));
        return;
    }

    old_fs = get_fs();
    set_fs(KERNEL_DS);
    pos = fp->f_pos;
    ret = kernel_write(fp, hdr->data, hdr->data_len, &pos);
    fp->f_pos = pos;
    set_fs(old_fs);
    if (ret != hdr->data_len) {
        hitest_err("write file fail\n");
    }

    filp_close(fp, NULL);
}
int get_cov_data(void)
{
    unsigned int *cov_data = NULL;
    unsigned long data_len;
    const char *build_id =  NULL;
    const char *project_name = NULL;
    struct cov_data_hdr *hdr = NULL;
    int ret;

    build_id = Hitest_Agent_GetBuildIDS();
    project_name = Hitest_Agent_GetPrjNameS();
    data_len = Hitest_Agent_GetCovDataSizeS();

    if (data_len <= 0) {
        hitest_err("cov_data is NULL!\n");
        return -1;
    }

    cov_data = kzalloc(sizeof(struct cov_data_hdr) + data_len, GFP_KERNEL);
    if (NULL == cov_data) {
        hitest_err("[NCSAgentSendCoverageData]: Allocate memory error!\n");
        return -1;
    }

    (void)memset_s(cov_data, (sizeof(struct cov_data_hdr) + data_len), 0, (sizeof(struct cov_data_hdr) + data_len));
    hdr = (struct cov_data_hdr*)cov_data;
    hdr->msg_type = HITEST_MSG_TYPE;
    hdr->data_len = data_len;
    ret = memcpy_s(hdr->project_name, HITEST_STR_LEN, project_name, strlen(project_name));
    if (ret != 0) {
        hitest_err("mem cpy project_name failed:%d\n", ret);
        return -1;
    }
    ret = memcpy_s(hdr->build_id, HITEST_STR_LEN, build_id, strlen(build_id));
    if (ret != 0) {
        hitest_err("mem cpy build_id failed:%d\n", ret);
        return -1;
    }

    if (Hitest_Coverage_GetCoverageDataS((unsigned int *)hdr->data, data_len) != 1) {
        hitest_err("Error: get coverage data failed!\n");
        return -1;
    }

    creat_cov_file(hdr);

    return 0;
}

int hitest_get_cov(void)
{
    int ret;
    /* 采集覆盖率 */
    ret = get_cov_data();
    if (ret) {
        hitest_err("Hitest failed to collect code coverage!\n");
    }

    /* 清除覆盖率 */
    Hitest_Coverage_InitCoverageData();
    hitest_err("get cov end\n");
    return 0;
}

EXPORT_SYMBOL(hitest_get_cov);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

/* lint -restore */
