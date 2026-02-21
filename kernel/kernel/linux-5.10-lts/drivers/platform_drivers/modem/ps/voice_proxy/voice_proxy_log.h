/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
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

#ifndef VOICE_PROXY_LOG_H
#define VOICE_PROXY_LOG_H

#include <linux/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define VOICE_PROXY_LOG_LVL_HIGH    (1U << 0)
#define VOICE_PROXY_LOG_LVL_ERR    (1U << 1)
#define VOICE_PROXY_LOG_LVL_INFO    (1U << 2)
#define VOICE_PROXY_LOG_LVL_DBG    (1U << 3)

#define AUDIO_LOGH(fmt, ...) do { \
    if (voice_proxy_log_level & VOICE_PROXY_LOG_LVL_HIGH) \
        pr_err("[VOICE_PROXY]: [HIGH] <%s> "fmt"\n", __func__, ##__VA_ARGS__); \
} while (0)

#define AUDIO_LOGE(fmt, ...) do { \
    if (voice_proxy_log_level & VOICE_PROXY_LOG_LVL_ERR) \
        pr_err("[VOICE_PROXY]: [ERR] <%s> "fmt"\n", __func__, ##__VA_ARGS__); \
} while (0)

#define AUDIO_LOGI(fmt, ...) do { \
    if (voice_proxy_log_level & VOICE_PROXY_LOG_LVL_INFO) \
        pr_warn("[VOICE_PROXY]: [INFO] <%s> "fmt"\n", __func__, ##__VA_ARGS__); \
} while (0)

#define AUDIO_LOGD(fmt, ...) do { \
    if (voice_proxy_log_level & VOICE_PROXY_LOG_LVL_DBG) \
        pr_notice("[VOICE_PROXY]: [DBG] <%s> "fmt"\n", __func__, ##__VA_ARGS__); \
} while (0)

extern unsigned int voice_proxy_log_level;

#ifdef __cplusplus
}
#endif

#endif /* VOICE_PROXY_LOG_H */
