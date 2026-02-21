/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2023. All rights reserved.
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

#ifndef __MDRV_BMI_DLOAD_H__
#define __MDRV_BMI_DLOAD_H__
/**
 * @brief 升级接口通用错误码
 */
#define DLOAD_GET_OEMINFO_ERR (-10)

typedef enum {
    DLOAD_UPDATE_ALL_DONE,      /* 升级已全部完成 */
    DLOAD_UPDATING_SINGLE,      /* 单边升级中 */
    DLOAD_UPDATE_SINGLE_DONE,   /* 单边升级完成 */
    DLOAD_BACKUPING,            /* 备份中 */
} dload_state_e;
/**
 * @brief 获取升级状态
 *
 * @attention
 * <ul><li>未进行OTA升级的新单板会读取oeminfo错误。</li></ul>
 *
 * @retval >=0, dload_state_e
 * @retval <0, 获取错误，详见升级接口通用错误码。
 */
int mdrv_dload_get_status(void);

typedef enum {
    UPDATE_RESULT_FAIL,        /* 升级失败 */
    UPDATE_RESULT_SUCCESS,     /* 升级成功 */
} update_result_e;
/**
 * @brief 获取升级结果
 *
 * @attention
 * <ul><li>此结果仅支持在升级全部完成后查询，查询的结果为上一次的升级结果。</li></ul>
 *
 * @retval >=0, update_result_e
 * @retval <0, 获取错误，详见升级接口通用错误码。
 */
int mdrv_dload_get_update_result(void);

typedef enum {
    BACKUP_NOT_TRIGGER, /* 备份未触发，当触发升级后会置此结果，直到触发备份流程 */
    BACKUP_FAIL,        /* 备份失败 */
    BACKUP_SUCCESS,     /* 备份成功 */
} backup_result_e;
/**
 * @brief 获取备份结果
 *
 * @attention
 * <ul><li>此结果仅支持在升级全部完成后查询，查询的结果为上一次的备份结果。</li></ul>
 *
 * @retval >=0, 备份结果，详见backup_result_e。
 * @retval <0, 获取错误，详见升级接口通用错误码。
 */
int mdrv_dload_get_backup_result(void);

/**
 * @brief 获取升级进度
 *
 * @retval 升级进度，范围0-100。
 */
unsigned int mdrv_dload_get_update_rate(void);

typedef enum {
    UPDATE_PROGRESSING = 10,                   /* 升级进行中，非异常 */
    UPDATE_FAILED_COMPONENT_STATUS_ERROR = 20, /* 升级状态错误 */
    UPDATE_FAILED_PACKET_NOT_EXIST = 21,       /* 升级文件不存在 */
    UPDATE_FAILED_NO_PACKET_PATH = 22,         /* 无法获取升级文件路径 */
    UPDATE_FAILED_INIT_ERROR = 23,             /* 全局变量初始化失败 */
    UPDATE_FAILED_DEVICE_INIT_ERROR = 24,      /* 设备创建失败 */
    UPDATE_FAILED_ID_ERROR = 30,               /* 在线升级ID不匹配 */
    UPDATE_FAILED_OLD_HASH_ERROR = 31,         /* old hash校验不匹配 */
    UPDATE_FAILED_PUBKEY_ERROR = 32,           /* pubkey校验不匹配 */
    UPDATE_FAILED_SIGN_VERIFY_FAIL = 33,       /* 在线升级签名验证失败 */
    UPDATE_FAILED_PARSE_PACKET_ERROR = 40,     /* 数据包解析处理失败 */
    UPDATE_FAILED_FLASH_PACKET_ERROR = 41,     /* 数据包烧写处理失败 */
    UPDATE_FAILED_WAIT_PARAL_ERROR = 42,       /* 等待并行烧写完成失败 */
    UPDATE_FAILED_SWITCH_PART_ERROR = 43,      /* 切分区失败 */
    UPDATE_FAILED_FINAL = 50,                  /* 其他原因升级失败 */
    UPDATE_SUCCESSFUL_HAVE_DATA = 90,          /* 升级成功，非异常 */
} update_errno_e;
/**
 * @brief 获取升级错误码
 *
 * @attention
 * <ul><li>此错误码返回的是升级触发成功后单边升级时的异常原因。</li></ul>
 *
 * @retval >=0, update_errno_e
 * @retval <0, 获取错误，详见升级接口通用错误码。
 */
int mdrv_dload_get_update_errno(void);
#endif
