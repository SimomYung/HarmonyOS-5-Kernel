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
#include <product_config.h>
#include <securec.h>
#include <linux/init_syscalls.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <bsp_dt.h>
#include <of.h>
#include <osl_list.h>
#include <osl_malloc.h>
#include <bsp_print.h>
#include <bsp_slice.h>
#include <bsp_file_ops.h>
#include <dump_exc_mgr.h>
#include <dump_subsys_mgr.h>
#include <dump_infra.h>
#include "dump_logs.h"
#include "dump_log_strategy.h"
#include "dump_log_compress.h"

struct dump_zip_stru g_zip_ctrl;

static unsigned int compdev_poll(struct file *file, poll_table *wait)
{
    unsigned int ret;
    dump_print("poll waiting.\n");
    poll_wait(file, &g_zip_ctrl.comp_log_ctrl->wq, wait);
    ret = g_zip_ctrl.comp_log_ctrl->trigger_flag ? POLLIN : 0;
    dump_print("poll done once.\n");
    return ret;
}

static ssize_t compdev_read(struct file *file, char __user *buf, size_t count, loff_t *pos)
{
    ssize_t ret = 1;
    dump_print("read zipdev.\n");
    if ((buf == NULL) || (count != sizeof(g_zip_ctrl.zipintf_info))) {
        return BSP_ERROR;
    }
    mutex_lock(&g_zip_ctrl.comp_log_ctrl->mutex);
    ret = copy_to_user(buf, (void *)&g_zip_ctrl.zipintf_info, sizeof(g_zip_ctrl.zipintf_info));
    if (ret) {
        dump_error("copy to user failed.\n");
    }
    g_zip_ctrl.comp_log_ctrl->trigger_flag = 0;

    mutex_unlock(&g_zip_ctrl.comp_log_ctrl->mutex);

    dump_print("read zipdev done.\n");
    return ret;
}

u32 dump_get_comp_service_status(void)
{
    if (g_zip_ctrl.comp_log_ctrl == NULL) {
        return 0;
    }
    if (g_zip_ctrl.comp_log_ctrl->fopen_cnt != 0) {
        return 1;
    }
    return 0;
}

static int compdev_open(struct inode *inode, struct file *file)
{
    mutex_lock(&g_zip_ctrl.comp_log_ctrl->mutex);
    g_zip_ctrl.comp_log_ctrl->fopen_cnt++;
    mutex_unlock(&g_zip_ctrl.comp_log_ctrl->mutex);
    dump_print("open zipdev.\n");
    return 0;
}

static int compdev_release(struct inode *inode, struct file *file)
{
    mutex_lock(&g_zip_ctrl.comp_log_ctrl->mutex);
    if (g_zip_ctrl.comp_log_ctrl->fopen_cnt != 0) {
        g_zip_ctrl.comp_log_ctrl->fopen_cnt--;
    }
    mutex_unlock(&g_zip_ctrl.comp_log_ctrl->mutex);
    dump_error("%s entry\n", __func__);
    return 0;
}

static const struct file_operations g_comp_dev_fops = {
    .read = compdev_read,
    .poll = compdev_poll,
    .open = compdev_open,
    .release = compdev_release,
};

int dump_comp_dev_register(struct zipintf_info_s *zip_info)
{
    int ret = 0;

    g_zip_ctrl.comp_log_ctrl = kzalloc(sizeof(*g_zip_ctrl.comp_log_ctrl), GFP_KERNEL);
    if (g_zip_ctrl.comp_log_ctrl == NULL) {
        ret = EINVAL;
        goto out;
    }

    g_zip_ctrl.comp_log_ctrl->zip_info = zip_info;

    g_zip_ctrl.comp_log_ctrl->misc.minor = MISC_DYNAMIC_MINOR;
    g_zip_ctrl.comp_log_ctrl->misc.name = kstrdup("zipdev", GFP_KERNEL);
    if (g_zip_ctrl.comp_log_ctrl->misc.name == NULL) {
        ret = EINVAL;
        goto out_free_log;
    }

    g_zip_ctrl.comp_log_ctrl->misc.fops = &g_comp_dev_fops;
    g_zip_ctrl.comp_log_ctrl->misc.parent = NULL;

    init_waitqueue_head(&g_zip_ctrl.comp_log_ctrl->wq);
    mutex_init(&g_zip_ctrl.comp_log_ctrl->mutex);

    /* finally, initialize the misc device for this log */
    ret = misc_register(&g_zip_ctrl.comp_log_ctrl->misc);
    if (ret) { /*lint !e730: (Info -- Boolean argument to function)*/
        dump_error("failed to register misc device for log '%s'!\n",
                   g_zip_ctrl.comp_log_ctrl->misc.name); /*lint !e429*/
        goto out_free_name;
    }

    dump_print("created zip dev '%s'\n", g_zip_ctrl.comp_log_ctrl->misc.name);
    return 0;

out_free_name:
    kfree(g_zip_ctrl.comp_log_ctrl->misc.name);
    g_zip_ctrl.comp_log_ctrl->misc.name = NULL;
out_free_log:
    kfree(g_zip_ctrl.comp_log_ctrl);
    g_zip_ctrl.comp_log_ctrl = NULL;
out:
    return ret;
}

int dump_get_compress_list(void)
{
    device_node_s *dev = NULL;
    u32 listsize = 0;
    u32 index = 0;
    int ret;
    const char *temp = NULL;

    dev = bsp_dt_find_compatible_node(NULL, NULL, "driver,dump_compress");
    if (dev == NULL) {
        dump_error("dump compress dts node no find!\n");
        return -1;
    }

    if (bsp_dt_property_read_u32(dev, "listsize", &listsize)) {
        listsize = 0;
        dump_error("fail to get listsize!\n");
        return 0;
    }
    g_zip_ctrl.zipintf_info.mfilenum = listsize;

    for (index = 0; index < listsize; index++) {
        ret = bsp_dt_property_read_string_index(dev, "dump_compress_name", index, &temp);
        if (ret != 0 || temp == NULL) {
            dump_error("fail to read dump_compress_name\n");
        } else {
            if (snprintf_s(g_zip_ctrl.zipintf_info.pfile_list[index], MAX_COMPRESS_FILE_NAME,
                MAX_COMPRESS_FILE_NAME - 1, "%s", (char *)temp) < 0) {
                dump_error("snprintf err\n");
            }
        }
    }

    return 0;
}

int dump_trigger_compress(const char *logpath, int pathlen, struct dump_file_save_info_s *datainfo)
{
    if (!dump_is_fulldump()) {
        return DUMP_OK;
    }
    if (pathlen >= COMPRESS_FILE_PATH_LEN) {
        dump_error("pathlen %d too big.\n", pathlen);
        return DUMP_ERRNO_INVA_PARAM;
    }
    dump_print("dump_trigger_compress here for path %s.\n", logpath);

    if (g_zip_ctrl.comp_log_ctrl == NULL || logpath == NULL || datainfo == NULL) {
        dump_error("param is NULL.\n");
        return DUMP_ERRNO_INVA_PARAM;
    }

    if (snprintf_s(g_zip_ctrl.zipintf_info.mfilepath, COMPRESS_FILE_PATH_LEN, COMPRESS_FILE_PATH_LEN - 1, "%s",
        (char *)logpath) < 0) {
        dump_error("snprintf err\n");
        return DUMP_ERRNO_MEM_OPS;
    }
    if (memcpy_s(&g_zip_ctrl.zipintf_info.saveinfo, sizeof(struct dump_file_save_info_s), datainfo,
        sizeof(struct dump_file_save_info_s)) != EOK) {
        dump_error("memcpy err\n");
        return DUMP_ERRNO_MEM_OPS;
    }
    dump_print("dump_trigger_compress wakeup for path %s, file %s.\n", g_zip_ctrl.zipintf_info.mfilepath,
        g_zip_ctrl.zipintf_info.pfile_list[0]);
    mutex_lock(&g_zip_ctrl.comp_log_ctrl->mutex);
    g_zip_ctrl.comp_log_ctrl->trigger_flag = 1;
    mutex_unlock(&g_zip_ctrl.comp_log_ctrl->mutex);
    wake_up_interruptible(&g_zip_ctrl.comp_log_ctrl->wq);

    return DUMP_OK;
}

void dump_wait_compress_done(const char *log_path)
{
    char compfilename[COMPRESS_FILE_PATH_LEN] = {0};
    int count = 0;

    if (!dump_get_comp_service_status()) {
        dump_print("no compress service\n");
        return;
    }

    if (!dump_is_fulldump()) {
        return;
    }
    if (log_path == NULL) {
        dump_error("log_path err\n");
        return;
    }

    // wait compress done
    dump_print("[0x%x] wait compress done\n", bsp_get_slice_value());

    if (g_zip_ctrl.zipintf_info.mfilenum > 0) {
        count = 0;
        if (snprintf_s(compfilename, COMPRESS_FILE_PATH_LEN, COMPRESS_FILE_PATH_LEN - 1, "%sCOMPDONE", log_path) < 0) {
            dump_error("snprintf err.\n");
        }
        while (bsp_eaccess(compfilename)) {
            if (count++ < DUMP_COMPRESS_TIMEOUT_CNTS) {
                msleep(DUMP_COMPRESS_WAIT_PER_CNT);
            } else {
                break;
            }
        }
    }
    dump_print("[0x%x] compress done with count %d.\n", bsp_get_slice_value(), count);
}

void dump_compress_all_logs(const dump_exc_log_desc_s *exc_info, void *priv_data)
{
    struct dump_file_save_info_s data_info;
    const char *dir_name = NULL;
    
    UNUSED(priv_data);
    if (!dump_valid_exc_log_path(exc_info)) {
        dump_error("err logpath");
        return;
    }
    dir_name = (const char *)exc_info->log_path;

    (void)memset_s(&data_info, sizeof(struct dump_file_save_info_s), 0, sizeof(struct dump_file_save_info_s));
    if (strncpy_s(data_info.dstfilename, sizeof(data_info.dstfilename), "modem_dump.bin", strlen("modem_dump.bin"))
        < 0) {
        dump_error("cpy err\n");
        return;
    }
    data_info.magic = DUMP_FILE_TRANS_MAGIC;
    data_info.links = DUMP_FILE_LINKS_MAGIC;
    if (dump_trigger_compress(dir_name, sizeof(exc_info->log_path), &(data_info)) == DUMP_OK) {
        dump_wait_compress_done(dir_name);
    }
}

__init void dump_logzip_work_init(void)
{
    dump_work_s work;

    work.work_type = DUMP_WORK_ASYNC;
    work.work_fn = dump_compress_all_logs;
    work.prior = DUMP_WORK_PRIO_ASYNC_HOOK_COMMON_COMPRESS_LOG;
    work.priv_data = NULL;
    dump_subsys_add_work(DUMP_SUBSYS_COMM, &work);
}

__init void dump_logzip_init(void)
{
    if (!dump_is_fulldump()) {
        return;
    }

    if (dump_get_compress_list()) {
        dump_error("get compress list failed !\n");
        return;
    }

    if (dump_comp_dev_register(&g_zip_ctrl.zipintf_info)) {
        dump_error("comp_dev_register fail\n");
        return;
    }

    dump_logzip_work_init();
}