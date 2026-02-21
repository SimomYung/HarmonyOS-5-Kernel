/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OHOS_AUDIO_LOG_H
#define OHOS_AUDIO_LOG_H

#ifndef LOG_TAG
#define LOG_TAG "PulseAudio"
#endif

#include <stdio.h>

#include "hilog/log.h"
#ifdef FEATURE_HITRACE_METER
#include "hitrace_meter_c.h"
#define HITRACE_AUDIO_TAG (1ULL << 35)
#endif

#undef LOG_DOMAIN
#define LOG_DOMAIN 0xD002B12
#ifndef OHOS_DEBUG
#define DECORATOR_HILOG(op, fmt, args...) \
    do {                                  \
        op(LOG_CORE, "[%{public}s]" fmt, __FUNCTION__, ##args);        \
    } while (0)
#else
#define DECORATOR_HILOG(op, fmt, args...)                                                \
    do {                                                                                 \
        op(LOG_CORE, "{%s()-%s:%d} " fmt, __FUNCTION__, __FILENAME__, __LINE__, ##args); \
    } while (0)
#endif

#define AUDIO_DEBUG_LOG(fmt, ...) DECORATOR_HILOG(HILOG_DEBUG, fmt, ##__VA_ARGS__)
#define AUDIO_ERR_LOG(fmt, ...) DECORATOR_HILOG(HILOG_ERROR, fmt, ##__VA_ARGS__)
#define AUDIO_WARNING_LOG(fmt, ...) DECORATOR_HILOG(HILOG_WARN, fmt, ##__VA_ARGS__)
#define AUDIO_INFO_LOG(fmt, ...) DECORATOR_HILOG(HILOG_INFO, fmt, ##__VA_ARGS__)
#define AUDIO_FATAL_LOG(fmt, ...) DECORATOR_HILOG(HILOG_FATAL, fmt, ##__VA_ARGS__)

#define AUDIO_OK 0
#define AUDIO_INVALID_PARAM (-1)
#define AUDIO_INIT_FAIL (-2)
#define AUDIO_ERR (-3)
#define AUDIO_PERMISSION_DENIED (-4)

#define CHECK_AND_RETURN_RET_LOG(cond, ret, fmt, ...)  \
    do {                                               \
        if (!(cond)) {                                 \
            AUDIO_ERR_LOG(fmt, ##__VA_ARGS__);            \
            return ret;                                \
        }                                              \
    } while (0)

#define CHECK_AND_RETURN_LOG(cond, fmt, ...)           \
    do {                                               \
        if (!(cond)) {                                 \
            AUDIO_ERR_LOG(fmt, ##__VA_ARGS__);            \
            return;                                    \
        }                                              \
    } while (0)

#define CHECK_AND_BREAK_LOG(cond, fmt, ...)            \
    do {                                               \
        if (!(cond)) {                                 \
            AUDIO_ERR_LOG(fmt, ##__VA_ARGS__);            \
            break;                                     \
        }                                              \
    } while (0)


#ifndef OHOS_DEBUG
#define DECORATOR_PRERELEASE_HILOG(op, fmt, args...) \
    do {                                  \
        op(LOG_ONLY_PRERELEASE, "[%{public}s]" fmt, __FUNCTION__, ##args);        \
    } while (0)
#else
#define DECORATOR_PRERELEASE_HILOG(op, fmt, args...)                                                \
    do {                                                                                 \
        op(LOG_ONLY_PRERELEASE, "{%s()-%s:%d} " fmt, __FUNCTION__, __FILENAME__, __LINE__, ##args); \
    } while (0)
#endif

#define AUDIO_PRERELEASE_LOGD(fmt, ...) DECORATOR_PRERELEASE_HILOG(HILOG_DEBUG, fmt, ##__VA_ARGS__)
#define AUDIO_PRERELEASE_LOGE(fmt, ...) DECORATOR_PRERELEASE_HILOG(HILOG_ERROR, fmt, ##__VA_ARGS__)
#define AUDIO_PRERELEASE_LOGW(fmt, ...) DECORATOR_PRERELEASE_HILOG(HILOG_WARN, fmt, ##__VA_ARGS__)
#define AUDIO_PRERELEASE_LOGI(fmt, ...) DECORATOR_PRERELEASE_HILOG(HILOG_INFO, fmt, ##__VA_ARGS__)
#define AUDIO_PRERELEASE_LOGF(fmt, ...) DECORATOR_PRERELEASE_HILOG(HILOG_FATAL, fmt, ##__VA_ARGS__)

#define CHECK_AND_RETURN_RET_PRELOG(cond, ret, fmt, ...)  \
    do {                                                  \
        if (!(cond)) {                                    \
            AUDIO_PRERELEASE_LOGE(fmt, ##__VA_ARGS__);    \
            return ret;                                   \
        }                                                 \
    } while (0)

#define CHECK_AND_RETURN_PRELOG(cond, fmt, ...)           \
    do {                                                  \
        if (!(cond)) {                                    \
            AUDIO_PRERELEASE_LOGE(fmt, ##__VA_ARGS__);    \
            return;                                       \
        }                                                 \
    } while (0)

#define CHECK_AND_BREAK_PRELOG(cond, fmt, ...)            \
    if (1) {                                              \
        if (!(cond)) {                                    \
            AUDIO_PRERELEASE_LOGE(fmt, ##__VA_ARGS__);    \
            break;                                        \
        }                                                 \
    } else void (0)

#define CHECK_AND_CONTINUE_PRELOG(cond, fmt, ...)         \
    if (1) {                                              \
        if (!(cond)) {                                    \
            AUDIO_PRERELEASE_LOGD(fmt, ##__VA_ARGS__);    \
            continue;                                     \
        }                                                 \
    } else void (0)

//  for trace
// CallEnd is needed. Take care of the goto.
inline void CallStart(const char *traceName)
{
#ifdef FEATURE_HITRACE_METER
    HiTraceStartTrace(HITRACE_AUDIO_TAG, traceName);
#endif
}

inline void CallEnd()
{
#ifdef FEATURE_HITRACE_METER
    HiTraceFinishTrace(HITRACE_AUDIO_TAG);
#endif
}
#endif // OHOS_AUDIO_LOG_H
