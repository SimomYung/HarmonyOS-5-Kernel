/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description: firmware function module
 * Author: @CompanyNameTag
 */

#ifdef _PRE_COLDBOOT_FEATURE
#include <linux/uaccess.h>
#include <linux/syscalls.h>
#include <linux/kthread.h>
#include "plat_firmware_flash.h"
#include "plat_firmware.h"
#include "plat_firmware_util.h"
#include "plat_debug.h"
#include "plat_cali.h"
#include "plat_cust.h"
#include "plat_common_clk.h"
#include "cali_data_ops_default.h"
#include "board.h"
#include "oal_util.h"

#define DOWNLOAD_CMD_COUNT      64
#define DOWNLOAD_FILE_PATH_LEN  512
#define DOWNLOAD_FILE_COUNT     2
#define DOWNLOAD_COM_FILE_COUNT 1
#define FIRMWARE_PARTION        "/dev/block/by-name/reserved2"
#define PLAT_CUST_MAX_BUF_LEN   512

typedef struct firmware_cmd_globals {
    int32_t count;                               /* 存储每个cfg文件解析后有效的命令个数 */
    cmd_type_info cmd[DOWNLOAD_CMD_COUNT];       /* 存储每个cfg文件的有效命令 */
} firmware_cmd_globals;

typedef struct firmware_file_desc {
    uint8_t file_path[DOWNLOAD_FILE_PATH_LEN];
    int32_t flash_offset;
    int32_t file_len;
} firmware_file_desc;

typedef struct firmware_file_globals {
    int32_t count;
    firmware_file_desc file_desc[DOWNLOAD_FILE_COUNT];
} firmware_file_globals;

typedef struct firmware_flash_map {
    firmware_cmd_globals cmd_cfg;
    firmware_file_globals file_cfg;
    mpxx_bfgx_cali_data_stru cali_data;
} firmware_flash_map;

typedef struct firmware_com_file_globals {
    int32_t count;
    firmware_file_desc file_desc[DOWNLOAD_COM_FILE_COUNT];
} firmware_com_file_globals;

typedef struct firmware_plat_cust {
    int32_t real_size;
    uint8_t plat_cust_data[PLAT_CUST_MAX_BUF_LEN];
} firmware_plat_cust;

typedef struct firmware_com_flash_map {
    firmware_cmd_globals cmd_cfg;
    firmware_com_file_globals file_cfg;
    uint8_t dcxo_info[DCXO_DEVICE_MAX_BUF_LEN];
    firmware_plat_cust plat_cust;
} firmware_com_flash_map;

static firmware_flash_map g_firmware_map;
static firmware_com_flash_map g_firmware_com_map;
static int32_t g_flash_file_offset = 0;

STATIC const char* firmware_get_partion(void)
{
    mpxx_board_info *bd_info = get_hi110x_board_info();
    if (bd_info->coldboot_partion != NULL) {
        return bd_info->coldboot_partion;
    } else {
        return FIRMWARE_PARTION;
    }
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
STATIC int32_t firmware_partion_write(const char *partion, loff_t from, uint8_t *buf, size_t len)
{
    int ret;
    int fd;
    mm_segment_t oldfs = get_fs();
    set_fs(KERNEL_DS);
    fd = (int)oal_sys_open(partion, O_RDWR, (int)S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd < 0) {
        ps_print_err("oal_sys_open %s block device failed, and fd = %d!\n",
                     partion, fd);
        ret = -ENODEV;
        goto out;
    }

    ret = (int)oal_sys_lseek((unsigned int)fd, from, SEEK_SET);
    if (ret == -1) {
        ps_print_err("oal_sys_lseek error, from = 0x%llx, len = 0x%zx, ret = 0x%x.\n",
                     from, len, ret);
        ret = -EIO;
        goto out;
    }

    ret = (int)oal_sys_write((unsigned int)fd, (char *)buf, len);
    if (ret == -1) {
        ps_print_err("oal_sys_write error, from = 0x%llx, len = 0x%zx, ret = 0x%x.\n",
                     from, len, ret);
        ret = -EIO;
        goto out;
    }

    ret = oal_sys_fsync(fd);
    if (ret < 0) {
        ps_print_err("oal_sys_fsync error, from = 0x%llx, len = 0x%zx, ret = 0x%x.\n",
                     from, len, ret);
        ret = -EIO;
        goto out;
    }
    oal_sys_close((unsigned int)fd);
    set_fs(oldfs);
    return SUCC;
out:
    if (fd >= 0) {
        oal_sys_close((unsigned int)fd);
    }
    set_fs(oldfs);
    return ret;
}
#else
STATIC long full_rw_file(os_kernel_file *fp, char *buf, size_t buf_size, bool read, loff_t pos)
{
    long bytes_total_to_rw = (long)buf_size;
    long bytes_total_rw = 0L;
    long bytes_this_time = 0L;
    char *ptemp = buf;

    while (bytes_total_to_rw > 0) {
        bytes_this_time = read ?
        vfs_read(fp, ptemp, bytes_total_to_rw, &pos) :
        vfs_write(fp, ptemp, bytes_total_to_rw, &pos);
        if (read ? (bytes_this_time <= 0) : (bytes_this_time < 0)) {
            ps_print_err("bf_sys_read or bf_sys_write failed!\n");
            break;
        }
        ptemp += bytes_this_time;
        bytes_total_to_rw -= bytes_this_time;
        bytes_total_rw += bytes_this_time;
    }
    return bytes_total_rw;
}

STATIC int rw_part(const char *dev_path, loff_t offset, char *buf, size_t buf_size, bool read)
{
    os_kernel_file *fp = NULL;
    int ret = -1;
    long bytes_total;
    mm_segment_t fs;

    if (dev_path == NULL || buf == NULL) {
        ps_print_err("dev_pat or buf is null\n");
        return -EFAIL;
    }

    fs = get_fs();
    set_fs(KERNEL_DS);
    fp = filp_open(dev_path, read ? O_RDONLY : O_WRONLY, 0);
    if (IS_ERR(fp)) {
        ps_print_err("Open file [%s] failed!\n", dev_path);
        goto out;
    }

    bytes_total = full_rw_file(fp, buf, buf_size, read, offset);
    if ((long long)buf_size != bytes_total) {
        ps_print_err("rw [%s] failed!, result: %ld, it should be: %lld\n",
                     dev_path, bytes_total, (long long)buf_size);
        goto out;
    } else {
        ret = 0;
    }

out:
    if (!IS_ERR(fp)) {
        vfs_fsync(fp, 0);
        filp_close(fp, NULL);
    }

    set_fs(fs);

    return ret;
}

STATIC int32_t firmware_partion_write(const char *dev_path, loff_t offset, const char *buf, size_t buf_size)
{
    return rw_part(dev_path, offset, (char *)buf, buf_size, false);
}
#endif

STATIC int32_t firmware_file_save(const char *partion, os_kernel_file *fp, uint32_t file_len, uint32_t flash_offset)
{
    uint32_t per_send_len;
    uint32_t send_count;
    int32_t rdlen;
    int32_t ret;
    uint32_t i;
    uint32_t offset = 0;

    uint8_t *buf = NULL;
    uint32_t buf_len = MIN_FIRMWARE_FILE_TX_BUF_LEN;

    buf = oal_memalloc(buf_len);
    if (buf == NULL) {
        return -EFAIL;
    }

    per_send_len = (buf_len > file_len) ? file_len : buf_len;
    send_count = (file_len + per_send_len - 1) / per_send_len;

    for (i = 0; i < send_count; i++) {
        rdlen = oal_file_read_ext(fp, fp->f_pos, buf, per_send_len);
        if (rdlen > 0) {
            ps_print_dbg("len of kernel_read is [%d], i=%d\n", rdlen, i);
            fp->f_pos += rdlen;
        } else {
            oal_free(buf);
            ps_print_err("len of kernel_read is error! ret=[%d], i=%d\n", rdlen, i);
            return (rdlen < 0) ? rdlen : -EFAIL;
        }

        ret = firmware_partion_write(partion, flash_offset + offset, buf, rdlen);
        if (ret < 0) {
            oal_free(buf);
            ps_print_err("firmware partion write file data fail\n");
            return -EFAIL;
        }
        ps_print_dbg("firmware_partion_write ret[%d] offset[%d]:write_len[%d]\n",
                     ret, flash_offset + offset, rdlen);
        offset += rdlen;
    }

    if (offset != file_len) {
        oal_free(buf);
        ps_print_err("file write len is err! write len is [%d], file len is [%d]\n", offset, file_len);
        return -EFAIL;
    }

    oal_free(buf);
    return SUCC;
}

STATIC int32_t firmware_file_open_get_len(char *path, char *pre_directory,
    os_kernel_file **fp, uint32_t *file_len)
{
    int32_t ret;
    char *frw_abs_path = path;
    // 如果相对路径，需要加上路径前缀
    if (pre_directory != NULL) {
        uint32_t path_len = (uint32_t)(OAL_STRLEN(pre_directory) + sizeof(char) + OAL_STRLEN(path) + 1);
        frw_abs_path = (uint8_t *)os_kzalloc_gfp(path_len);
        if (frw_abs_path == NULL) {
            return -ENOMEM;
        }
        ret = sprintf_s(frw_abs_path, path_len, "%s%c%s", pre_directory, '/', path);
        ps_print_info("firmware_set_firmware_file absolute path %s, ret = %d", frw_abs_path, ret);
    }

    ret = file_open_get_len(frw_abs_path, fp, file_len);
    if (pre_directory != NULL) {
        os_mem_kfree(frw_abs_path);
    }
    if (ret < 0) {
        ps_print_err("parse file len fail, path[%s]!\n", frw_abs_path);
        return -EFAIL;
    }
    return SUCC;
}

STATIC int32_t firmware_set_firmware_file_desc(firmware_file_globals *file_cfg, const char *partion,
    cmd_type_info *cfg_cmd_info, uint8_t *pre_directory, bool com_contain)
{
    unsigned long addr;
    char *path = NULL;
    int32_t ret;
    uint32_t file_len;
    uint32_t flash_offset = sizeof(firmware_flash_map);
    firmware_file_desc *file_desc = NULL;
    uint32_t file_count;
    os_kernel_file *fp = NULL;

    ret = parse_file_cmd(cfg_cmd_info->cmd_para, &addr, &path);
    if (ret < 0) {
        ps_print_err("parse file cmd fail, cmd[%s]!\n", cfg_cmd_info->cmd_para);
        return -EFAIL;
    }

    if (com_contain) {
        flash_offset += sizeof(firmware_com_flash_map) + g_flash_file_offset;
    }

    ret = firmware_file_open_get_len(path, pre_directory, &fp, &file_len);
    if (ret != SUCC) {
        return ret;
    }

    file_desc = file_cfg->file_desc;
    file_count = file_cfg->count;
    ret = firmware_file_save(partion, fp, file_len, flash_offset);
    if (ret == SUCC) {
        (file_desc + file_count)->flash_offset = flash_offset;
        (file_desc + file_count)->file_len = file_len;
        g_flash_file_offset += file_len;
        (void)strncpy_s((file_desc + file_count)->file_path, DOWNLOAD_FILE_PATH_LEN, path, strlen(path));
        ps_print_info("firmware save file desc[%d]:flash_offset[%d], file_len[%d], file_path[%s]\n",
                      file_count, flash_offset, file_len, path);
        file_cfg->count++;
    } else {
        ps_print_info("firmware save file fail, file_path[%s]\n", path);
    }

    oal_file_close(fp);
    return ret;
}

STATIC void firmware_plat_cust_data_save(void)
{
    errno_t ret_err;
    uint8_t *plat_cust_addr = (uint8_t *)get_plat_cust_addr();
    if (plat_cust_addr == NULL) {
        ps_print_err("firmware get plat cust addr fail");
        return;
    }
    ret_err = memcpy_s(g_firmware_com_map.plat_cust.plat_cust_data, PLAT_CUST_MAX_BUF_LEN,
        plat_cust_addr, get_plat_cust_len());
    if (ret_err == EOK) {
        g_firmware_com_map.plat_cust.real_size = get_plat_cust_len();
        ps_print_info("firmware plat cust data get succ\n");
    } else {
        ps_print_info("firmware plat cust data get fail\n");
    }
}

STATIC void firmware_dcxo_data_save(void)
{
    errno_t ret_err;
    uint8_t *dcxo_data = (uint8_t *)get_dcxo_data_buf_addr();
    if (dcxo_data == NULL) {
        ps_print_err("firmware get dcxo addr fail");
        return;
    }

    ret_err = memcpy_s(g_firmware_com_map.dcxo_info, DCXO_DEVICE_MAX_BUF_LEN, dcxo_data, DCXO_DEVICE_MAX_BUF_LEN);
    if (ret_err == EOK) {
        ps_print_info("firmware dcxo data get succ\n");
    } else {
        ps_print_info("firmware dcxo data get fail\n");
    }
}

STATIC bool firmware_cfg_cmd_save_comc(const char *partion)
{
    int32_t ret;
    int32_t i;
    uint32_t cmd_count;
    cmd_type_info *cfg_cmd_info = NULL;
    cmd_type_info *save_cmd_info = NULL;
    firmware_globals* cfg_info = get_firmare_cfg_info();

    cmd_count = cfg_info->count[COMC_CFG];
    if ((cmd_count >= DOWNLOAD_CMD_COUNT) || (cmd_count == 0)) {
        ps_print_err("cmc save error cfg_count: %d, max_save_count: %d\n", cmd_count, DOWNLOAD_CMD_COUNT);
        return false;
    }

    if (cmd_count + g_firmware_com_map.cmd_cfg.count >= DOWNLOAD_CMD_COUNT) {
        ps_print_err("cmc cmd save error cfg_count: %d, max_save_count: %d\n", cmd_count, DOWNLOAD_CMD_COUNT);
        return false;
    }

    cfg_cmd_info = cfg_info->cmd[COMC_CFG];
    save_cmd_info = g_firmware_com_map.cmd_cfg.cmd;
    for (i = 0; i < cmd_count; i++) {
        ret = memcpy_s(save_cmd_info + i, sizeof(cmd_type_info), cfg_cmd_info + i, sizeof(cmd_type_info));
        if (unlikely(ret != EOK)) {
            ps_print_err("cmc cfg info copy failed\n");
            return false;
        }

        if ((cfg_cmd_info + i)->cmd_type == FILE_TYPE_CMD) {
            if (g_firmware_com_map.file_cfg.count >= DOWNLOAD_COM_FILE_COUNT) {
                ps_print_err("comc cmd cfg file[%d] save failed\n", g_firmware_com_map.file_cfg.count);
                return false;
            }
            ret = firmware_set_firmware_file_desc((firmware_file_globals *)&g_firmware_com_map.file_cfg, partion,
                cfg_cmd_info + i, cfg_info->file_info.pre_directory, true);
            if (ret != SUCC) {
                return false;
            }
        }
        ps_print_info("cmc firmware save cmd[%d]:type[%d], name[%s], para[%s]\n",
                      i, (cfg_cmd_info + i)->cmd_type, (cfg_cmd_info + i)->cmd_name, (cfg_cmd_info + i)->cmd_para);
    }
    g_firmware_com_map.cmd_cfg.count = cmd_count;
    ps_print_info("cmc firmware save cmd count:%d\n", cmd_count);
    return true;
}

STATIC void firmware_cfg_cmd_save_bfgx(const char *partion, bool com_contain)
{
    int32_t ret;
    int32_t i;
    uint32_t cmd_count;
    cmd_type_info *cfg_cmd_info = NULL;
    cmd_type_info *save_cmd_info = NULL;
    firmware_globals* cfg_info = get_firmare_cfg_info();
    int32_t bt_cfg_type = BFGX_CFG;

    cmd_count = cfg_info->count[BFGX_CFG];
    if (cmd_count == 0) {
        cmd_count = cfg_info->count[B_CFG];
        bt_cfg_type = B_CFG;
    }
    if (cmd_count + g_firmware_map.cmd_cfg.count >= DOWNLOAD_CMD_COUNT) {
        ps_print_err("bfgx cmd save error cfg_count: %d, max_save_count: %d\n", cmd_count, DOWNLOAD_CMD_COUNT);
        return;
    }

    cfg_cmd_info = cfg_info->cmd[bt_cfg_type];
    save_cmd_info = &(g_firmware_map.cmd_cfg.cmd[g_firmware_map.cmd_cfg.count]);
    for (i = 0; i < cmd_count; i++) {
        ret = memcpy_s(save_cmd_info + i, sizeof(cmd_type_info), cfg_cmd_info + i, sizeof(cmd_type_info));
        if (unlikely(ret != EOK)) {
            ps_print_err("bfgx cmd cfg info copy failed\n");
            return;
        }

        if ((cfg_cmd_info + i)->cmd_type == FILE_TYPE_CMD) {
            if (g_firmware_map.file_cfg.count >= DOWNLOAD_FILE_COUNT) {
                ps_print_err("bfgx cmd cfg file[%d] save failed\n", g_firmware_map.file_cfg.count);
                return;
            }
            ret = firmware_set_firmware_file_desc(&g_firmware_map.file_cfg, partion,
                cfg_cmd_info + i, cfg_info->file_info.pre_directory, com_contain);
            if (ret != SUCC) {
                return;
            }
        }
        ps_print_info("bfgx firmware save cmd[%d]:type[%d], name[%s], para[%s]\n",
                      i, (cfg_cmd_info + i)->cmd_type, (cfg_cmd_info + i)->cmd_name, (cfg_cmd_info + i)->cmd_para);
    }
    g_firmware_map.cmd_cfg.count += cmd_count;
    ps_print_info("bfgx firmware save cmd count:%d\n", cmd_count);
}

STATIC void firmware_cali_data_save(void)
{
    int32_t ret;
    mpxx_bfgx_cali_data_stru *cali_data = &g_firmware_map.cali_data;

    ret = get_bfgx_cali_data((uint8_t *)cali_data, sizeof(mpxx_bfgx_cali_data_stru), 0); // 第三个参数0，实际用不到
    if (ret == SUCC) {
        ps_print_info("firmware bfgx cali data get succ\n");
    } else {
        ps_print_info("firmware bfgx cali data get fail\n");
    }
}

void firmware_coldboot_partion_save_data(void)
{
    int32_t ret;
    bool com_contain;
    const char *partion = firmware_get_partion();

    (void)memset_s(&g_firmware_map, sizeof(g_firmware_map), 0, sizeof(g_firmware_map));
    (void)memset_s(&g_firmware_com_map, sizeof(g_firmware_com_map), 0, sizeof(g_firmware_com_map));
    g_flash_file_offset = 0;

    com_contain = firmware_cfg_cmd_save_comc(partion);
    firmware_cfg_cmd_save_bfgx(partion, com_contain);
    firmware_cali_data_save();

    ret = firmware_partion_write(partion, 0, (uint8_t *)&g_firmware_map, sizeof(g_firmware_map));
    if (ret == SUCC) {
        ps_print_info("firmware partion write firmware map data succ\n");
    } else {
        ps_print_info("firmware partion write firmware map data fail\n");
    }

    if (com_contain) {
        firmware_plat_cust_data_save();
        firmware_dcxo_data_save();
        ret = firmware_partion_write(partion, sizeof(g_firmware_map),
            (uint8_t *)&g_firmware_com_map, sizeof(g_firmware_com_map));
        if (ret == SUCC) {
            ps_print_info("firmware partion write firmware com map data succ\n");
        } else {
            ps_print_info("firmware partion write firmware com map data fail\n");
        }
    }
}

#ifdef _PRE_COLDBOOT_THREAD_FEATURE
int firmware_coldboot_thread(void *data)
{
    firmware_coldboot_partion_save_data();
    return 0;
}
#endif

void firmware_coldboot_partion_save(void)
{
#ifdef _PRE_COLDBOOT_THREAD_FEATURE
    kthread_run(firmware_coldboot_thread, NULL, "coldboot_save");
#else
    firmware_coldboot_partion_save_data();
#endif
}
#endif
