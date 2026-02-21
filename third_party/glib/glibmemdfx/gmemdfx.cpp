/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

#include "gmemdfx.h"
#include <unordered_map>
#include <vector>
#include <hilog/log.h>
#include <unistd.h>
#include "gmemdfxdump.h"
#include "dfx_dump_catcher.h"
#include "param_wrapper.h"
#include "string_ex.h"

#undef LOG_DOMAIN
#define LOG_DOMAIN 0xD002B00

#define __LOG(func, fmt, args...)                                                       \
    do {                                                                                      \
        (void)func(LABEL, "{%{public}s():%{public}d} " fmt, __FUNCTION__, __LINE__, ##args);  \
    } while (0)

#define LOGE(fmt, ...) __LOG(::OHOS::HiviewDFX::HiLog::Error, fmt, ##__VA_ARGS__)

#define POINTER_MASK 0x00FFFFFF
#define FAKE_POINTER(addr) (POINTER_MASK & reinterpret_cast<uintptr_t>(addr))

struct MemInfo {
    uint64_t count = 0;
    uint64_t size = 0;
    std::string str;
    intptr_t mem;
};

struct PoolInfo {
    uint64_t count = 0;
    uint64_t size = 0;
    uint64_t alignment = 0;
    uint64_t lastTid = 0;
    intptr_t mem;
};

namespace {
    constexpr OHOS::HiviewDFX::HiLogLabel LABEL = {LOG_CORE, LOG_DOMAIN, "AVGlibMemDfx"};
    static std::unordered_map<void *, MemInfo> memMap;
    static std::unordered_map<void *, PoolInfo> poolMap;
    static uint64_t memCount = 0;
    static uint64_t poolCount = 0;
    static std::mutex mutex;
    static bool enableDump = false;
    static unsigned int dumpSize = 0;
    static unsigned int dumpStart = 0;
    static unsigned int dumpCount = 0;
    static bool dumpOpen = false;
}

void GMemPoolAllocDfx(void *mem, unsigned int alignment, unsigned int size)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (!dumpOpen || mem == nullptr) {
        return;
    }
    if (poolMap.find(mem) != poolMap.end()) {
        LOGE("the mem 0x%{public}06" PRIXPTR " is already allocated", FAKE_POINTER(mem));
        return;
    }

    poolMap[mem] = {poolCount++, size, alignment, gettid(), (intptr_t)mem};
}

void GMemPoolFreeDfx(void *mem)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (!dumpOpen || mem == nullptr) {
        return;
    }
    if (mem != nullptr && poolMap.erase(mem) == 0) {
        LOGE("the mem 0x%{public}06" PRIXPTR " is already free", FAKE_POINTER(mem));
    }
}

void GMemAllocDfx(void *mem, unsigned int size)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (!dumpOpen || mem == nullptr) {
        return;
    }
    if (memMap.find(mem) != memMap.end()) {
        LOGE("the mem 0x%{public}06" PRIXPTR " is already allocated", FAKE_POINTER(mem));
        return;
    }
    std::string str;
    if (enableDump && size == dumpSize && (memCount - dumpStart) % dumpCount == 0) {
        OHOS::HiviewDFX::DfxDumpCatcher dumpLog;
        bool ret = dumpLog.DumpCatch(getpid(), gettid(), str);
        if (!ret) {
            LOGE("dump error");
        }
    }

    memMap[mem] = {memCount++, size, str, (intptr_t)mem};
}

void GChainMemFreeDfx(void *mem_chain, unsigned long next_offset)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (!dumpOpen || mem_chain == nullptr) {
        return;
    }
    void *next = mem_chain;
    while (next) {
        uint8_t *current = (uint8_t *)next;
        next = *(void **)(current + next_offset);
        if (current != nullptr && memMap.erase(current) == 0) {
            LOGE("the mem 0x%{public}06" PRIXPTR " is already free", FAKE_POINTER(current));
        }
    }
}

void GMemFreeDfx(void *mem)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (!dumpOpen || mem == nullptr) {
        return;
    }
    if (mem != nullptr && memMap.erase(mem) == 0) {
        LOGE("the mem 0x%{public}06" PRIXPTR " is already free", FAKE_POINTER(mem));
    }
}

void InitParameter()
{
    std::string dumpSizeStr;
    std::string dumpStartStr;
    std::string dumpCountStr;
    std::string dumpOpenStr;
    int32_t size;
    int32_t start;
    int32_t count;
    int32_t res = OHOS::system::GetStringParameter("sys.media.dump.mem.size", dumpSizeStr, "");
    if (res == 0 && !dumpSizeStr.empty()) {
        OHOS::StrToInt(dumpSizeStr, size);
        dumpSize = size;
        enableDump = dumpSize == 0 ? false :true;
    } else {
        enableDump = false;
    }
    res = OHOS::system::GetStringParameter("sys.media.dump.mem.start", dumpStartStr, "");
    if (res == 0 && !dumpStartStr.empty()) {
        OHOS::StrToInt(dumpStartStr, start);
        dumpStart = start;
    } else {
        dumpStart = 0;
    }
    res = OHOS::system::GetStringParameter("sys.media.dump.mem.count", dumpCountStr, "");
    if (res == 0 && !dumpCountStr.empty()) {
        OHOS::StrToInt(dumpCountStr, count);
        dumpCount = count;
    } else {
        dumpCount = 1;
    }
    res = OHOS::system::GetStringParameter("sys.media.dump.mem.open", dumpOpenStr, "");
    if (res == 0 && !dumpOpenStr.empty()) {
        dumpOpen = dumpOpenStr == "TRUE" ? true : false;
    } else {
        dumpOpen = false;
    }
}

void GetGMemDump(std::string &str)
{
    std::unordered_map<void *, MemInfo> memMapCopy;
    {
        std::lock_guard<std::mutex> lock(mutex);
        InitParameter();
        memMapCopy = memMap;
    }
    std::vector<std::pair<void *, MemInfo>> memInfoVec(memMapCopy.begin(), memMapCopy.end());
    std::sort(memInfoVec.begin(), memInfoVec.end(), [&](auto &left, auto &right) {
        return left.second.count < right.second.count;
    });
    for (auto iter = memInfoVec.begin(); iter != memInfoVec.end(); iter++) {
        str += "count:";
        str += std::to_string(iter->second.count) + ";";
        str += "size:";
        str += std::to_string(iter->second.size) + "\n";
        str += iter->second.str + "\n";
    }
}

void GetGMemPoolDump(std::string &str)
{
    std::unordered_map<void *, PoolInfo> poolMapCopy;
    {
        std::lock_guard<std::mutex> lock(mutex);
        InitParameter();
        poolMapCopy = poolMap;
    }
    std::vector<std::pair<void *, PoolInfo>> poolInfoVec(poolMapCopy.begin(), poolMapCopy.end());
    std::sort(poolInfoVec.begin(), poolInfoVec.end(), [&](auto &left, auto &right) {
        return left.second.count < right.second.count;
    });
    for (auto iter = poolInfoVec.begin(); iter != poolInfoVec.end(); iter++) {
        str += "count:";
        str += std::to_string(iter->second.count) + ";";
        str += "size:";
        str += std::to_string(iter->second.size) + "\n";
        str += "alignment:";
        str += std::to_string(iter->second.alignment) + "\n";
        str += "lastTid:";
        str += std::to_string(iter->second.lastTid) + "\n";
    }
}