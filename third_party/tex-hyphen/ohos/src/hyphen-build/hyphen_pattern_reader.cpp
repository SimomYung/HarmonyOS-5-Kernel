/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
#include "hyphen_pattern.h"

#include <codecvt>
#include <cstdio>
#include <cerrno>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <map>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unicode/utf.h>
#include <unicode/utf8.h>
#include <unistd.h>

using namespace std;

namespace OHOS::Hyphenate {

vector<uint16_t> ConvertToUtf16(const string& utf8Str)
{
    int32_t i = 0;
    UChar32 c = 0;
    vector<uint16_t> target;
    const int32_t textLength = utf8Str.size();
    while (i < textLength) {
        U8_NEXT(reinterpret_cast<const uint8_t*>(utf8Str.c_str()), i, textLength, c);
        if (U16_LENGTH(c) == 1) {
            target.push_back(c);
        } else {
            target.push_back(U16_LEAD(c));
            target.push_back(U16_TRAIL(c));
        }
    }
    return target;
}

struct Pattern {
    uint8_t patterns[8]; // dynamic
};

struct ArrayOf16bits {
    uint16_t count;
    uint16_t codes[3]; // dynamic
};

struct Header {
    uint8_t magic1;
    uint8_t magic2;
    uint8_t minCp;
    uint8_t maxCp;
    uint32_t toc;
    uint32_t mappings;
    uint32_t version;

    inline uint16_t CodeOffset(uint16_t code, const ArrayOf16bits* maps = nullptr) const
    {
        if (maps && (code < minCp || code > maxCp)) {
            for (size_t i = maps->count; i != 0;) {
                i -= HYPHEN_BASE_CODE_SHIFT;
                if (maps->codes[i] == code) {
                    // cout << "resolved mapping ix: " << static_cast<int>(m->codes[i + 1]) << endl;
                    auto offset = maps->codes[i + 1];
                    return (maxCp - minCp) * HYPHEN_BASE_CODE_SHIFT + (offset - maxCp) * HYPHEN_BASE_CODE_SHIFT + 1;
                }
            }
            return MaxCount(maps);
        }
        if (maps) {
            // + 1 because previous end is before next start
            // 2x because every second value to beginning addres
            return (code - minCp) * HYPHEN_BASE_CODE_SHIFT + 1;
        } else {
            if (code < minCp || code > maxCp) {
                return maxCp + 1;
            }
            return (code - minCp);
        }
    }

    inline static void ToLower(uint16_t& code)
    {
        if (code == '.') {
            code = '`';
        } else if (code == '\'') {
            code = '^';
        } else if (code == '-') {
            code = '_';
        } else {
            code = tolower(code);
        }
        cout << "tolower: " << hex << static_cast<int>(code) << endl;
    }

    inline uint16_t MaxCount(const ArrayOf16bits* maps) const
    {
        // need to write this in binary provider !!
        return (maxCp - minCp) * HYPHEN_BASE_CODE_SHIFT + maps->count;
    }
};

struct CodeInfo {
    int32_t OpenPatFile(const char* filePath);
    int32_t GetHeader();
    int32_t GetCodeInfo(uint16_t code);
    void ProcessPattern(const size_t& offset, vector<uint8_t>& result, bool direct);
    bool ProcessDirect(const std::vector<uint16_t>& target, const size_t& offset);
    void ProcessLinear(const std::vector<uint16_t>& target, const size_t& offset, vector<uint8_t>& result);
    bool ProcessNextCode(const std::vector<uint16_t>& target, const size_t& offset);
    void ClearResource();
    Header* fHeader{nullptr};
    uint8_t* fAddress{nullptr};
    FILE* fFile{nullptr};
    size_t fFileSize{0};
    uint16_t fMaxCount{0};
    PathType fType{PathType::PATTERN};
    uint16_t fOffset{0};
    uint16_t fCode{0};
    uint32_t fIndex{0};
    uint32_t fNextOffset;
    uint16_t* fStaticOffset{nullptr};
    ArrayOf16bits* fMappings{nullptr};
};

int32_t CodeInfo::OpenPatFile(const char* filePath)
{
    cout << "Attempt to mmap " << filePath << endl;

    FILE* file = fopen(filePath, "r");
    if (file == nullptr) {
        cerr << "FATAL: " << errno << endl;
        return FAILED;
    }

    struct stat st;
    if (fstat(fileno(file), &st) != 0) {
        cerr << "FATAL: fstat" << endl;
        fclose(file);
        return FAILED;
    }
    size_t length = st.st_size;
    uint8_t* address = static_cast<uint8_t*>(mmap(nullptr, length, PROT_READ, MAP_PRIVATE, fileno(file), 0u));
    if (address == MAP_FAILED) {
        cerr << "FATAL: mmap" << endl;
        fclose(file);
        return FAILED;
    }

    cout << "Magic: " << hex << *reinterpret_cast<uint32_t*>(address) << dec << endl;
    this->fFile = file;
    this->fFileSize = length;
    this->fAddress = address;
    return SUCCEED;
}

static std::vector<uint16_t> GetInputWord(const char* input)
{
    const std::string utf8Str = "." + std::string(input) + ".";
    std::vector<uint16_t> target = ConvertToUtf16(utf8Str);
    for (auto& code : target) {
        Header::ToLower(code);
    }
    return target;
}

int32_t CodeInfo::GetHeader()
{
    fHeader = reinterpret_cast<Header*>(fAddress);
    uint16_t minCp = fHeader->minCp;
    uint16_t maxCp = fHeader->maxCp;
    // get master table, it always is in direct mode
    fMappings = reinterpret_cast<ArrayOf16bits*>(reinterpret_cast<uint32_t*>(fAddress + fHeader->mappings));
    // this is actually beyond the real 32 bit address, but just to have an offset that
    // is clearly out of bounds without recalculating it again
    fMaxCount = fHeader->MaxCount(fMappings);
    cout << "min/max: " << minCp << "/" << maxCp << " count " << static_cast<int>(fMaxCount) << endl;
    cout << "size of top level mappings: " << static_cast<int>(fMappings->count) << endl;
    if (minCp == maxCp && fMappings->count == 0) {
        cerr << "### unexpected min/max in input file-> exit" << endl;
        return FAILED;
    }
    return SUCCEED;
}

void CodeInfo::ClearResource()
{
    (void)munmap(fAddress, fFileSize);
    fAddress = nullptr;
    (void)fclose(fFile);
    fFile = nullptr;
    fFileSize = 0;
}

int32_t CodeInfo::GetCodeInfo(uint16_t code)
{
    fType = PathType::PATTERN;
    this->fCode = code;
    this->fIndex = 0;
    fOffset = fHeader->CodeOffset(code, fMappings);
    if (fOffset == fMaxCount) {
        cout << hex << char(code) << " unable to map, contiue straight" << endl;
        return FAILED;
    }

    // previous entry end
    uint32_t baseOffset =
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t*>(fAddress + fHeader->toc) + fOffset - 1);
    uint32_t initialValue = *(reinterpret_cast<uint32_t*>(fAddress + fHeader->toc) + fOffset);
    fType = static_cast<PathType>(initialValue >> SHIFT_BITS_30);
    // direct and pairs need to have offset different from zero
    if (initialValue == 0 && (fType == PathType::DIRECT || fType == PathType::PAIRS)) {
        cout << char(code) << " is not in main dict, contiue straight" << endl;
        return FAILED;
    }
    // base offset is 16 bit
    fStaticOffset = reinterpret_cast<uint16_t*>(fAddress + HYPHEN_BASE_CODE_SHIFT * baseOffset);

    // get a subtable according character
    // once: read as 32bit, the rest of the access will be 16bit (13bit for offsets)
    fNextOffset = (initialValue & 0x3fffffff);

    cout << hex << baseOffset << " top level code: 0x" << hex << static_cast<int>(code) <<
        " starting with offset: 0x" << hex << fOffset << " table-offset 0x" << fNextOffset << endl;
    return SUCCEED;
}

void CodeInfo::ProcessPattern(const size_t& offset, vector<uint8_t>& result, bool direct)
{
    cout << "direct : " << direct << " " << hex << fNextOffset << endl;
    uint16_t poffset = 0;
    if (direct && (fHeader->version >> 0x18) >= 0x2) {
        poffset = *(reinterpret_cast<uint16_t*>(fAddress) + fNextOffset + (fHeader->version & 0xffff));
    } else {
        poffset = *(fStaticOffset + fNextOffset);
    }
    fNextOffset++; // there now is always at least pattern count before next node
    if (!poffset) {
        return;
    }
    uint16_t count = (poffset >> 0xc) * 0x4; // patterns are padded to 4 byte arrays.
                                             // to save bits, the count is multiplied by four
    poffset = 0xfff & poffset;

    //   if we have reached pattern, apply it to result
    auto p = reinterpret_cast<const Pattern*>(fAddress + poffset);
    if (count != 0) {
        cout << "Node with a pattern, count " << count << hex << " offset: " << poffset << endl;
        size_t i = 0;
        for (size_t j = offset - fIndex; j < result.size() && i < count; j++) {
            cout << "    " << static_cast<int>(j) << ": pattern index: " << i << " value: 0x" << hex
                 << static_cast<int>(p->patterns[i]) << endl;
            result[j] = std::max(result[j], (p->patterns[i]));
            i++;
        }
    }
}

bool CodeInfo::ProcessDirect(const std::vector<uint16_t>& target, const size_t& offset)
{
    // resolve new code point
    if (fIndex == offset) { // should never be the case
        cout << "# break loop on direct" << endl;
        return true;
    }

    fIndex++;
    fCode = target[offset - fIndex];
    fOffset = fHeader->CodeOffset(fCode);
    if (fHeader->minCp != fHeader->maxCp && fOffset > fHeader->maxCp) {
        cout << "# break loop on direct" << endl;
        return true;
    }

    auto nextValue = *(fStaticOffset + fNextOffset + fOffset);
    fNextOffset = nextValue & 0x3fff;
    fType = static_cast<PathType>(nextValue >> SHIFT_BITS_14);
    cout << "  found direct: " << char(fCode) << " : " << hex << nextValue << " with offset: " << fNextOffset << endl;
    return false;
}

void CodeInfo::ProcessLinear(const std::vector<uint16_t>& target, const size_t& offset, vector<uint8_t>& result)
{
    auto p = reinterpret_cast<const ArrayOf16bits*>(fStaticOffset + fNextOffset);
    auto count = p->count;

    fIndex++;
    cout << "# linear " << offset << " " << fIndex << endl;
    if (fIndex > offset || count > (offset - fIndex + 1)) {
        // the pattern is longer than the remaining word
        cout << "# break loop on linear " << offset << " " << fIndex << endl;
        return;
    }
    // check the rest of the string
    for (auto j = 0; j < count; j++) {
        cout << "    linear " << offset << " index: " << j << " value: " << hex << static_cast<int>(p->codes[j]) <<
            " vs " << static_cast<int>(target[offset - fIndex]) << endl;
        if (p->codes[j] != target[offset - fIndex]) {
            return;
        } else {
            fIndex++;
        }
    }
    // if we reach the end, apply pattern
    fNextOffset += count + 1; // array items + one for the count
    fIndex--;                 // because of recursion
    ProcessPattern(offset, result, false);
    if (*(fStaticOffset + fNextOffset) != 0 && offset > count) { // peek if there is more to come
        // make it tail recursive to save stack
        return ProcessLinear(target, offset, result);
    }
}

bool CodeInfo::ProcessNextCode(const std::vector<uint16_t>& target, const size_t& offset)
{
    // resolve new code point
    if (fIndex == offset) { // should detect this sooner
        cout << "# break loop on pairs" << endl;
        return true;
    }
    auto p = reinterpret_cast<const ArrayOf16bits*>(fStaticOffset + fNextOffset);
    uint16_t count = p->count;
    fIndex++;
    cout << "  continue to value pairs with size: " << count << " and code '" <<
        static_cast<int>(target[offset - fIndex]) << "'" << endl;

    //     check pairs, array is sorted (but small)
    bool match = false;
    for (size_t j = 0; j < count; j += HYPHEN_BASE_CODE_SHIFT) {
        cout << "    checking pair: " << j << " value: " << hex << static_cast<int>(p->codes[j]) << " vs " <<
            static_cast<int>(target[offset - fIndex]) << endl;
        if (p->codes[j] == target[offset - fIndex]) {
            fCode = target[offset - fIndex];
            cout << "      new value pair in : 0x" << j << " with code 0x" << hex << static_cast<int>(fCode) << "'" <<
                endl;
            fOffset = fHeader->CodeOffset(fCode);
            if (fHeader->minCp != fHeader->maxCp && fOffset > fHeader->maxCp) {
                cout << "# could not resolve debug offset in pairs" << endl;
            }

            fNextOffset = p->codes[j + 1] & 0x3fff;
            fType = static_cast<PathType>(p->codes[j + 1] >> SHIFT_BITS_14);
            match = true;
            break;
        } else if (p->codes[j] > target[offset - fIndex]) {
            break;
        }
    }
    if (!match) {
        cout << "# break loop on pairs" << endl;
        return true;
    }
    return false;
}

void PrintResult(const vector<uint8_t>& result, const vector<uint16_t>& target)
{
    cout << dec << "result size: " << result.size() << " while expecting " << target.size() << endl;
    if (result.size() <= target.size() + 1) {
        size_t i = 0;
        for (auto bp : result) {
            cout << hex << static_cast<int>(target[i++]) << ": " << to_string(bp) << endl;
        }
    }
}

bool InitializeCodeInfo(OHOS::Hyphenate::CodeInfo& codeInfo, const char* filePath)
{
    if (codeInfo.OpenPatFile(filePath) != SUCCEED) {
        return false;
    }
    if (codeInfo.GetHeader() != SUCCEED) {
        codeInfo.ClearResource();
        return false;
    }
    return true;
}

void ProcessCodeLoop(OHOS::Hyphenate::CodeInfo& codeInfo, const std::vector<uint16_t>& target, size_t i,
                     std::vector<uint8_t>& result)
{
    bool continueLoop = true;
    while (continueLoop) {
        std::cout << "#loop c: '" << codeInfo.fCode << "' starting with offset: 0x" << std::hex << codeInfo.fOffset <<
            " table-offset 0x" << codeInfo.fNextOffset << " index: " << codeInfo.fIndex << std::endl;

        codeInfo.ProcessPattern(i, result, codeInfo.fType == OHOS::Hyphenate::PathType::PATTERN);
        if (codeInfo.fType == OHOS::Hyphenate::PathType::PATTERN) {
            continueLoop = false;
        } else if (codeInfo.fType == OHOS::Hyphenate::PathType::DIRECT) {
            if (codeInfo.ProcessDirect(target, i)) {
                continueLoop = false;
            }
        } else if (codeInfo.fType == OHOS::Hyphenate::PathType::LINEAR) {
            codeInfo.ProcessLinear(target, i, result);
            continueLoop = false;
        } else {
            if (codeInfo.ProcessNextCode(target, i)) {
                continueLoop = false;
            }
        }
    }
}

void ProcessCodeInfo(OHOS::Hyphenate::CodeInfo& codeInfo, const std::vector<uint16_t>& target,
                     std::vector<uint8_t>& result)
{
    for (size_t i = target.size() - 1; i != 0; --i) {
        if (codeInfo.GetCodeInfo(target[i]) != SUCCEED) {
            continue;
        }
        codeInfo.fIndex = 0;
        ProcessCodeLoop(codeInfo, target, i, result);
    }
}

int32_t HyphenReader::Read(const char* filePath, const std::vector<uint16_t>& utf16Target) const
{
    CodeInfo codeInfo;
    if (!InitializeCodeInfo(codeInfo, filePath)) {
        return FAILED;
    }

    std::vector<uint8_t> result(utf16Target.size(), 0);
    ProcessCodeInfo(codeInfo, utf16Target, result);

    codeInfo.ClearResource();
    PrintResult(result, utf16Target);
    return SUCCEED;
}
} // namespace OHOS::Hyphenate

namespace {
constexpr size_t ARG_NUM = 2;

std::vector<uint16_t> CheckArgs(int argc, char** argv)
{
    std::vector<uint16_t> target;
    if (argc != 3) { // 3: valid argument number
        cout << "usage: './hyphen hyph-en-us.hpb <mytestword>' " << endl;
        return target;
    }
    target = OHOS::Hyphenate::GetInputWord(argv[ARG_NUM]);
    if (target.empty()) {
        cout << "usage: './hyphen hyph-en-us.hpb <mytestword>' " << endl;
    }
    return target;
}
} // namespace

int main(int argc, char** argv)
{
    std::vector<uint16_t> target = CheckArgs(argc, argv);
    if (target.empty()) {
        return FAILED;
    }

    OHOS::Hyphenate::HyphenReader hyphenReader;
    return hyphenReader.Read(argv[1], target);
}
