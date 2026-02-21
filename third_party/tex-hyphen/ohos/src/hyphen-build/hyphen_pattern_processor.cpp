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

#include <algorithm>
#include <cstddef>
#include <climits>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <map>
#include <unicode/utf.h>
#include <unicode/utf8.h>

using namespace std;

// to enable more information on development time
// define VERBOSE_PATTERNS

namespace OHOS::Hyphenate {
// upper limit for direct pointing arrays
#define MAXIMUM_DIRECT_CODE_POINT 0x7a

struct Leaf {
    uint16_t offset{0};
    uint16_t usecount{0};
};

struct Rule {
    uint16_t offset{0};
    map<uint16_t, vector<vector<uint16_t>>> patterns;
    map<uint16_t, Leaf> uniqLeafs;
};

static map<vector<uint8_t>, Rule> g_allRules;

vector<uint16_t> ConvertToUtf16(const string& utf8Str)
{
    int32_t i = 0;
    UChar32 c = 0;
    vector<uint16_t> target;
    const int32_t textLength = static_cast<int32_t>(utf8Str.size());
    while (i < textLength) {
        U8_NEXT(reinterpret_cast<const uint8_t*>(utf8Str.c_str()), i, textLength, c);
        if (U16_LENGTH(c) == 1) {
            target.push_back(c);
        } else {
            target.push_back(U16_LEAD(static_cast<uint32_t>(c)));
            target.push_back(U16_TRAIL(static_cast<uint32_t>(c)));
        }
    }
    return target;
}

// Recursive path implementation.
// Collects static information and the leafs that provide access to patterns
// The implementation is reversed to pattern code point order; end to beginning of pattern;
struct Path {
    explicit Path(const vector<uint16_t>& path, const vector<uint8_t>* pat)
    {
        count++;
        size_t targetIndex = path.size();
        if (targetIndex > 0) {
            code = path[--targetIndex];
        }
        if ((code <= MAXIMUM_DIRECT_CODE_POINT)) {
            maximumCP = max(maximumCP, code);
            minimumCP = min(minimumCP, code);
        }

        // Process children recursively
        if (targetIndex > 0) {
            Process(path, targetIndex, pat);
        } else {
            // store pattern to leafs
            pattern = pat;
            leafCount++;
        }
    }

    void Process(const vector<uint16_t>& path, size_t targetIndex, const vector<uint8_t>* pat)
    {
        if (targetIndex == 0) {
            pattern = pat;
            return;
        }

        uint16_t key = path[--targetIndex];
        if (auto ite = paths.find(key); ite != paths.end()) {
            ite->second.Process(path, targetIndex, pat);
        } else {
            if (key > MAXIMUM_DIRECT_CODE_POINT) {
                // if we have direct children with distinct code points, we need to use
                // value pairs
                haveNoncontiguousChildren = true;
            }
            vector<uint16_t> substr(path.cbegin(), path.cbegin() + targetIndex + 1);
            // recurse
            paths.emplace(key, Path(substr, pat));
        }
    }

    // The graph is built using pattern end characters
    // while the rules may have different leaf nodes
    // Check the dictionary terminated graph for unified rules and leafs
    void FindSharedLeaves()
    {
        if (paths.size() != 0) {
            for (auto& path : paths) {
                path.second.FindSharedLeaves();
            }
        } else if (g_allRules.count(*pattern) != 0) {
            auto ite = g_allRules[*pattern].uniqLeafs.find(code);
            if (ite != g_allRules[*pattern].uniqLeafs.cend()) {
                ite->second.usecount += 1;
            }
        }
    }

    // Once this node is reached, we can access pattern
    // however traversing further may be needed
    bool HasPattern() const { return pattern != nullptr; }

    // This instance of Path and its children implement a straight path without ambquity.
    // No need to traverse through tables to reach pattern.
    // Calculate the depth of the graph.
    bool IsLinear() const
    {
        if (paths.size() == 0) {
            return true;
        } else if (paths.size() == 1) {
            return paths.begin()->second.IsLinear();
        }
        return false;
    }

    // debug print misc info
    void Print(size_t indent) const
    {
#ifdef VERBOSE_PATTERNS
        indent += HYPHEN_INDENT_INCREMENT;
        for (size_t i = 0; i < indent; i++) {
            cout << " ";
        }
        if (indent == ROOT_INDENT) {
            cout << char(code) << "rootsize***: " << paths.size();
        } else {
            cout << char(code) << "***: " << paths.size();
        }
        if (paths.size() >= LARGE_PATH_SIZE)
            cout << " LARGE";
        else if (IsLinear()) {
            cout << " LINEAR";
        } else {
            cout << " @@@";
        }

        cout << endl;
        if (paths.size() == 0) {
            return;
        }
        for (auto path : paths) {
            path.second.Print(indent);
        }
        cout << endl;
#endif
    }

    static void WritePacked(vector<uint16_t>& data, ostream& out, bool writeCount = true)
    {
        uint16_t size = data.size();
        if (writeCount) {
            out.write(reinterpret_cast<const char*>(&size), sizeof(size));
        }

        for (size_t i = 0; i < data.size(); i++) {
            uint16_t bytes = data[i];
            out.write(reinterpret_cast<const char*>(&bytes), sizeof(bytes));
        }
    }

    static uint16_t WritePacked(const vector<uint8_t>& data, ostream& out, bool writeSize = true)
    {
        constexpr size_t ALIGN_4BYTES = 0x03;
        uint16_t size = data.size();
        if (writeSize) {
            out.write(reinterpret_cast<const char*>(&size), sizeof(size));
        }

        if ((data.size() & ALIGN_4BYTES) != 0) {
            cerr << "### uint8_t vectors should be aligned in 4 bytes !!!" << endl;
            size = size & ~ALIGN_4BYTES;
        }

        /* convert uint8 to uint32 */
        for (size_t i = 0; i < size; i += BYTES_PRE_WORD) {
            uint32_t bytes = data[i] | (data[i + 1] << 8) | (data[i + 2] << 16) | (data[i + 3] << 24);
            out.write(reinterpret_cast<const char*>(&bytes), sizeof(bytes));
        }
        return size;
    }

    // no need to twiddle the bytes or words currently
    static void WritePacked(uint32_t word, ostream& out)
    {
        out.write(reinterpret_cast<const char*>(&word), sizeof(word));
    }

    // We make assumption that 14 bytes is enough to represent offset
    // so we get two first bits in the array for path type
    // we have two bytes on the offset arrays
    // for these
    enum class PathType : uint8_t { PATTERN = 0, LINEAR = 1, PAIRS = 2, DIRECT = 3 };

    static void WritePackedLine(const Path& pathSrc, ostream& out, PathType& type)
    {
        bool wroteSomething{false};
        vector<uint16_t> output;

        // we do NOT need to write local pattern if we don't have children
        if (pathSrc.paths.empty()) {
            type = PathType::PATTERN;
            return;
        }

        type = PathType::LINEAR;
        auto ite = pathSrc.paths.cbegin();
        const auto* path = &(ite->second);
        auto localPattern = path->pattern;
        output.push_back(path->code);

        while (path) {
            if (localPattern) {
                // if we have children, they need to be checked when collecting rules
                if (output.size() > 0) {
                    WritePacked(output, out);
                }
                path->WritePatternOrNull(out);
                output.clear();
                localPattern = nullptr;
                wroteSomething = true;
            } else {
                // traverse further
                if (!path->paths.empty()) {
                    auto itr = path->paths.cbegin();
                    path = &(itr->second);
                    localPattern = path->pattern;
                    output.push_back(path->code);
                } else {
                    break;
                }
            }
        }
        if (!wroteSomething) {
            cerr << "Did not write anything linear" << endl;
            type = PathType::PATTERN;
        } else {
            // mark array end so that reader knows when to stop recursing
            uint16_t size = 0;
            out.write(reinterpret_cast<const char*>(&size), sizeof(size));
        }
    }

    void WritePatternOrNull(ostream& out) const
    {
        uint16_t size = 0;
        if (HasPattern()) {
            auto ite = g_allRules.find(*pattern);
            size = ite->second.offset;
        }

        out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    }

    void WriteTypedNode(ostream& out, uint32_t offset, uint32_t& pos, PathType& type) const
    {
        // check if we are linear or should write a table
        if (IsLinear()) {
            WritePatternOrNull(out);
            WritePackedLine(*this, out, type);
        } else if ((paths.size() < static_cast<size_t>(maximumCP - minimumCP) / HYPHEN_BASE_CODE_SHIFT)
                   || haveNoncontiguousChildren) {
            // Using dense table, i.e. value pairs
            vector<uint16_t> output;
            for (const auto& path : paths) {
                output.push_back(path.first);
                output.push_back(path.second.Write(out, offset));
            }
            pos = static_cast<uint32_t>(out.tellp()); // our header is after children data
            type = PathType::PAIRS;
            WritePatternOrNull(out);
            WritePacked(output, out);
        } else {
            // Direct pointing, initialize full mapping table
            vector<uint16_t> output;
            output.resize(maximumCP - minimumCP + 1, 0);
            if ((output.size() & 0x1) != 0) {
                output.push_back(0); // pad
            }
            for (const auto& path : paths) {
                // traverse children recursively (dfs)
                if (path.first >= minimumCP && path.first <= maximumCP) {
                    output[path.first - minimumCP] = path.second.Write(out, offset);
                } else {
                    cerr << " ### Encountered distinct code point 0x'" << hex << static_cast<int>(path.first) <<
                        " when writing direct array" << endl;
                }
            }
            pos = static_cast<uint32_t>(out.tellp()); // children first
            WritePatternOrNull(out);                  // pattern first
            WritePacked(output, out, false);          // then children table
        }
    }

    uint16_t Write(ostream& out, uint32_t offset = 0, uint32_t* endPos = nullptr) const
    {
        if (HasPattern() && paths.size() == 0) { // currently only leafs are shared
            // if we have a shared leaf for shared pattern, use it
            if (auto ite = g_allRules[*pattern].uniqLeafs.find(code); ite != g_allRules[*pattern].uniqLeafs.cend()) {
                if (ite->second.offset != 0) {
                    return ite->second.offset;
                }
            }
        }

        PathType type = PathType::DIRECT;
        uint32_t pos = static_cast<uint32_t>(out.tellp());
        uint32_t oPos = pos;

        WriteTypedNode(out, offset, pos, type);

        // return overall offset in 16bit
        CheckThatDataFits(pos, offset, out, type, oPos);
        if (endPos) {
            *endPos = static_cast<uint32_t>(out.tellp()) >> 1;
        }
        return (((pos >> 1) - offset) | (static_cast<uint32_t>(type) << SHIFT_BITS_14));
    }

    void CheckThatDataFits(uint32_t& pos, uint32_t offset, ostream& out, PathType& type, uint32_t oPos) const
    {
        // return overall offset in 16bit
        if (((pos >> 1) > offset) && ((pos >> 1) - offset) > 0x3fff) {
            cerr << " ### Cannot fit offset " << hex << pos << " : " << offset
                 << " into 14 bits, dropping node" << endl;
            out.seekp(oPos, ios_base::beg); // roll back to the beginning of this entry
            if (!out.good()) {
                // failing to roll back, terminate
                cerr << "Could not roll back outfile, terminating" << endl;
                exit(-1);
            }
            WritePatternOrNull(out);
            type = PathType::PATTERN;
            pos = static_cast<uint32_t>(out.tellp());
        }
    }

    static size_t count;
    static size_t leafCount;
    static uint16_t minimumCP;
    static uint16_t maximumCP;

    uint16_t code{0};
    map<uint16_t, Path> paths;
    const vector<uint8_t>* pattern{nullptr};
    bool haveNoncontiguousChildren{false};
};

size_t Path::count{0};
size_t Path::leafCount{0};
uint16_t Path::minimumCP = 0x7a;
uint16_t Path::maximumCP = 0x5f;

// Struct to hold all the patterns that end with the code.
struct PatternHolder {
    uint16_t code{0};
    map<vector<uint16_t>, vector<uint8_t>> patterns;
    map<uint16_t, Path> paths;
};

struct CpRange {
    uint16_t minimumCp{0};
    uint16_t maximumCp{0};
};

struct PathOffset {
    PathOffset(uint32_t o, uint32_t e, uint16_t t, uint16_t c) : offset(o), end(e), type(t), code(c) {}
    int32_t offset;
    int32_t end;
    uint32_t type;
    uint16_t code;
};

struct WriteOffestsParams {
    WriteOffestsParams(vector<PathOffset> offsets, uint32_t mappingsPos, CpRange cpRange)
        : fOffsets(offsets), fMappingsPos(mappingsPos), fCpRange(cpRange)
    {
    }
    vector<PathOffset> fOffsets;
    uint32_t fMappingsPos;
    CpRange fCpRange;
    uint16_t fCommonNodeOffset;
};

void processSection(const string& line, map<string, vector<string>>& sections, vector<string>*& current)
{
    string pat;
    for (size_t i = 1; i < line.size() && !iswspace(line[i]) && line[i] != '{'; i++) {
        pat += line[i];
    }
    cout << "resolved section: " << pat << endl;
    if (!pat.empty()) {
        sections[pat] = vector<string>();
        current = &sections[pat];
    }
}

static void ProcessContent(const string& line, vector<string>* current)
{
    string pat;
    for (auto code : line) {
        if (iswspace(code)) {
            if (!pat.empty()) {
                current->push_back(pat);
            }
            pat.clear();
            continue;
        }
        if (code == '%') {
            break;
        }
        // cout << code;
        pat += code;
    }
    if (!pat.empty()) {
        current->push_back(pat);
    }
}

static void ProcessLine(const string& line, vector<string>*& current, vector<string>& uncategorized,
                        map<string, vector<string>>& sections)
{
    string pat;
    if (line.empty()) {
        return;
    } else if (line[0] == '\\') {
        processSection(line, sections, current);
    } else if (line[0] == '}') {
        current = &uncategorized;
    } else {
        ProcessContent(line, current);
    }
}

static int32_t ResolveSectionsFromFile(const std::string& fileName, map<string, vector<string>>& sections)
{
    char resolvedPath[PATH_MAX] = {0};
    if (fileName.size() > PATH_MAX) {
        cout << "The file name is too long" << endl;
        return FAILED;
    }
    if (realpath(fileName.c_str(), resolvedPath) == nullptr) {
        cout << "file name exception" << endl;
        return FAILED;
    }

    ifstream input(resolvedPath);
    if (!input.good()) {
        cerr << "could not open '" << resolvedPath << "' for reading" << endl;
        return FAILED;
    }

    string line;
    vector<string> uncategorized;
    vector<string>* current = &uncategorized;
    while (getline(input, line)) {
        ProcessLine(line, current, uncategorized, sections);
    }

    cout << "Uncategorized data size: " << uncategorized.size() << endl;
    cout << "Amount of sections: " << sections.size() << endl;
    for (const auto& section : sections) {
        cout << "  '" << section.first << "' size: " << section.second.size() << endl;
    }
    return SUCCEED;
}

static vector<uint16_t> ProcessWord(const string& wordString)
{
    auto word = ConvertToUtf16(wordString);
    vector<uint16_t> result;
    bool addedBreak = false;
    for (const auto code : word) {
        if (code == '-') {
            result.push_back(BREAK_FLAG);
            addedBreak = true;
        } else {
            if (!addedBreak) {
                result.push_back(NO_BREAK_FLAG);
            }
            result.push_back(code);
            addedBreak = false;
        }
    }
    // match exceptions in full words only
    result.insert(result.cbegin(), '.');
    result.push_back('.');
    cout << "Adding exception: " << wordString << endl;
    return result;
}

static void ResolvePatternsFromSections(map<string, vector<string>>& sections, vector<vector<uint16_t>>& utf16Patterns)
{
    for (const auto& pattern : sections["patterns"]) {
        utf16Patterns.push_back(ConvertToUtf16(pattern));
    }
    for (const auto& word : sections["hyphenation"]) {
        utf16Patterns.push_back(ProcessWord(word));
    }
}

static void CollectLeaves(const vector<uint16_t>& pattern, uint16_t& ix)
{
    for (size_t i = pattern.size(); i > 0;) {
        if (!isdigit(pattern[--i])) {
            ix = pattern[i];
            break;
        }
    }
}

static void ProcessPattern(const vector<uint16_t>& pattern, vector<uint16_t>& codepoints, vector<uint8_t>& rules)
{
    bool addedRule = false;
    for (size_t i = 0; i < pattern.size(); i++) {
        uint16_t code = pattern[i];
        if (isdigit(code)) {
            rules.push_back(code - '0');
            addedRule = true;
        } else {
            if (!addedRule) {
                rules.push_back(0);
            }
            // These have been collected empirically from the existing pattern files.
            // Remap typical distinct codepoints
            // below 'a' to the beginning of contiguous range
            // This same thing needs to be done in 'tolower'
            // when parsing the results on runtime
            if (code == '.') {
                code = '`';
            } else if (code == '-') {
                code = '_';
            } else if (code == '\'') {
                code = '^';
            }
            codepoints.push_back(code);
            addedRule = false;
        }
    }
}

static void PadRules(vector<uint8_t>& rules)
{
    while ((rules.size() % PADDING_SIZE) != 0) {
        if (rules.back() == 0) {
            rules.pop_back();
        } else {
            break;
        }
    }
    while ((rules.size() % PADDING_SIZE) != 0) {
        rules.push_back(0);
    }
}

void ResolveLeavesFromPatterns(const vector<vector<uint16_t>>& utf16Patterns, map<uint16_t, PatternHolder>& leaves)
{
    for (const auto& pattern : utf16Patterns) {
        uint16_t ix{0};
        CollectLeaves(pattern, ix);
        if (ix == 0) {
            continue;
        }

        if (leaves.find(ix) == leaves.end()) {
            leaves[ix] = {PatternHolder()};
        }

        vector<uint16_t> codepoints;
        vector<uint8_t> rules;
        ProcessPattern(pattern, codepoints, rules);

        leaves[ix].code = ix;
        if (leaves[ix].patterns.find(codepoints) != leaves[ix].patterns.cend()) {
            cerr << "### Multiple definitions for pattern with size: " << codepoints.size() << endl;
            cerr << "###";
            for (auto codepoint : codepoints) {
                cerr << " 0x" << hex << static_cast<int>(codepoint);
            }
            cerr << endl;
        }

        PadRules(rules);
        leaves[ix].patterns[codepoints] = rules;
        // collect a list of unique rules
        if (auto it = OHOS::Hyphenate::g_allRules.find(rules); it != OHOS::Hyphenate::g_allRules.end()) {
            it->second.patterns[ix].push_back(codepoints);
        } else {
            OHOS::Hyphenate::g_allRules[rules] = Rule();
            Hyphenate::g_allRules[rules].patterns[ix].push_back(codepoints);
        }
    }

    cout << "leaves: " << leaves.size() << endl;
    cout << "unique rules: " << OHOS::Hyphenate::g_allRules.size() << endl;
}

static void BreakLeavesIntoPaths(map<uint16_t, PatternHolder>& leaves, CpRange& range, int& countPat)
{
    bool printCounts = true;
    // break leave information to Path instances
    for (auto& leave : leaves) {
        cout << "  '" << char(leave.first) << "' rootsize: " << leave.second.patterns.size() << endl;
        for (const auto& pat : leave.second.patterns) {
            if (auto ite = leave.second.paths.find(pat.first[pat.first.size() - 1]); ite != leave.second.paths.end()) {
                ite->second.Process(pat.first, pat.first.size() - 1, &pat.second);
            } else {
                leave.second.paths.emplace(pat.first[pat.first.size() - 1], Path(pat.first, &pat.second));
            }
#ifdef VERBOSE_PATTERNS
            cout << "    '";
            for (const auto& digit : pat.first) {
                cout << "'0x" << hex << static_cast<int>(digit) << "' ";
            }
            cout << "' size: " << pat.second.size() << endl;
            cout << "       ";
#endif
            for (const auto& digit : pat.second) {
                (void)digit;
                countPat++;
#ifdef VERBOSE_PATTERNS
                cout << "'" << to_string(digit) << "' ";
            }
            cout << endl;
#else
            }
#endif
        }

        // collect some stats
        for (auto path : leave.second.paths) {
            if (printCounts) {
                cout << "leafs-nodes: " << path.second.leafCount << " / " << path.second.count << endl;
                cout << "min-max: " << path.second.minimumCP << " / " << path.second.maximumCP << endl;
                range.minimumCp = path.second.minimumCP;
                range.maximumCp = path.second.maximumCP;
                break;
            }
            path.second.Print(HYPHEN_DEFAULT_INDENT);
        }
    }
}

const size_t FULL_TALBLE = 4;

static uint32_t InitOutFileHead(ofstream& out)
{
    // reserve space for:
    // - header
    // - main toc. and
    // - mapping array for large code points
    // - version
    for (size_t i = FULL_TALBLE; i != 0; i--) {
        uint32_t bytes{0};
        out.write(reinterpret_cast<const char*>(&bytes), sizeof(bytes));
    }
    return FULL_TALBLE * 2; // return 2 multiple talble size, check this number
}

static int32_t FormatOutFileHead(ofstream& out, const WriteOffestsParams& params, const uint32_t toc)
{
    out.seekp(ios::beg); // roll back to the beginning
    if (!out.good()) {
        cerr << "failed to write toc" << endl;
        return FAILED;
    }

    // very minimalistic magic, perhaps more would be in order including
    // possible version number
    uint32_t header = ('H' | ('H' << 8) | (params.fCpRange.minimumCp << 16) | (params.fCpRange.maximumCp << 24));
    // write header
    out.write(reinterpret_cast<const char*>(&header), sizeof(header));
    // write toc
    out.write(reinterpret_cast<const char*>(&toc), sizeof(toc));
    // write mappings
    out.write(reinterpret_cast<const char*>(&params.fMappingsPos), sizeof(params.fMappingsPos));
    // write binary version 8 top bits, using the lower 24 bits for common node offset without
    // needing to increase header size overall offset on the binary file
    // we may want to change this at some point
    const uint32_t version = (0x2 << 0x18) | params.fCommonNodeOffset;
    out.write(reinterpret_cast<const char*>(&version), sizeof(version));

    return SUCCEED;
}

void ProcessUniqueRule(std::pair<const vector<uint8_t>, Rule>& uniqueRule)
{
    for (auto ite : uniqueRule.second.patterns) {
        for (auto rule : ite.second) {
            if (!uniqueRule.second.uniqLeafs.count(*rule.cbegin())) {
                uniqueRule.second.uniqLeafs[*rule.cbegin()] = {0, 0};
            }
        }
    }
}

void WriteUniqueRules(ofstream& out)
{
    for (auto& uniqueRule : OHOS::Hyphenate::g_allRules) {
        uint32_t pos = static_cast<uint32_t>(out.tellp());
        uint16_t size = Path::WritePacked(uniqueRule.first, out, false) / 0x4; // save bits by padding size
        uniqueRule.second.offset = (size << 0xc) | pos;
        ProcessUniqueRule(uniqueRule);
        if ((pos >> 0xc) != 0) {
            cerr << "PATTERNS: RUNNING OUT OF ADDRESS SPACE, file a bug" << endl;
            exit(-1);
        }
    }
}

void WriteSharedLeafs(ofstream& out, uint16_t& pos, uint32_t& end)
{
    for (auto& uniqueRule : OHOS::Hyphenate::g_allRules) {
        cout << "###### UniqueRule with " << uniqueRule.second.patterns.size() << " leaves" << endl;
        for (auto& sharedLeaf : uniqueRule.second.uniqLeafs) {
            if (sharedLeaf.second.usecount > 0) {
                Path path({sharedLeaf.first}, &uniqueRule.first);
                sharedLeaf.second.offset = path.Write(out, pos, &end);
                cout << "found unique " << hex << static_cast<int>(sharedLeaf.first) <<
                    " wrote: '" << sharedLeaf.second.offset << "' " << endl;
            }
        }
    }
}

uint16_t CheckSharedLeaves(ofstream& out, map<uint16_t, PatternHolder>& leaves)
{
    // check how many of the unique rules remain valid once all the rules are combined
    for (auto& leave : leaves) {
        for (auto& path : leave.second.paths) {
            path.second.FindSharedLeaves();
        }
    }
    uint32_t end{0};
    if ((out.tellp() % 1) != 0) {
        out.write(reinterpret_cast<const char*>(&end), 1);
    }
    uint16_t pos = static_cast<uint16_t>(out.tellp()) >> 1;
    cout << "NOW THIS IS PURE MAGIC NUMBER FOR NOW: " << hex << pos << endl;
    // pad first offset with 16bit zero to make empty patterns ignore the zero offset
    out.write(reinterpret_cast<const char*>(&end), 2);
    WriteSharedLeafs(out, pos, end);
    return pos;
}

static bool WriteLeavePathsToOutFile(map<uint16_t, PatternHolder>& leaves, const CpRange& range, ofstream& out,
                                     uint32_t& tableOffset, vector<PathOffset>& offsets)
{
    // unique rules have no offset
    WriteUniqueRules(out);
    // shared nodes offset needs to be stored to header
    auto sharedOffset = CheckSharedLeaves(out, leaves);

    vector<Path*> bigOnes;
    bool hasDirect{false};
    for (auto& leave : leaves) {
        for (auto& path : leave.second.paths) {
            if (path.first < range.minimumCp || path.first > range.maximumCp) {
                bigOnes.push_back(&path.second);
                continue;
            }
            uint32_t end{0};
            uint16_t value = path.second.Write(out, tableOffset, &end);
            uint16_t offset = value & 0x3fff;
            uint32_t type = value & 0x0000c000;
            uint16_t code = path.first;
            cout << "direct:" << hex << static_cast<int>(code) << ": " << tableOffset << " : " << end << " type " <<
                type << endl;
            tableOffset = end;
            offsets.push_back(PathOffset(offset, end, type, code));
            hasDirect = true;
        }
    }

    // write distinc code points array after the direct ones
    for (auto path : bigOnes) {
        uint32_t end{0};
        uint16_t value = path->Write(out, tableOffset, &end);
        uint16_t offset = value & 0x3fff;
        uint32_t type = value & 0x0000c000;
        uint16_t code = path->code;
        cout << "distinct: 0x" << hex << static_cast<int>(code) << ": " << hex << tableOffset << " : " << end <<
            " type " << type << dec << endl;
        tableOffset = end;
        offsets.push_back(PathOffset(offset, end, type, code));
    }

    offsets.push_back(PathOffset(sharedOffset, 0, 0, 0));
    return hasDirect;
}

void ProcessDirectPointingValues(std::vector<PathOffset>::const_iterator& lastEffectiveIterator, std::ofstream& out,
                                 WriteOffestsParams& params, uint32_t& currentEnd, bool hasDirect)
{
    for (size_t i = params.fCpRange.minimumCp; i <= params.fCpRange.maximumCp; i++) {
        auto iterator = params.fOffsets.cbegin();
        while (iterator != params.fOffsets.cend()) {
            if (iterator->code == i) {
                break;
            }
            iterator++;
        }
        if (iterator == params.fOffsets.cend()) {
            if (!hasDirect) {
                break;
            }
            uint32_t dummy{0};
            Path::WritePacked(dummy, out);
            Path::WritePacked(currentEnd, out);
            std::cout << "Direct: padded " << std::endl;
            continue;
        }
        lastEffectiveIterator = iterator;
        uint32_t type = static_cast<uint32_t>(iterator->type);
        uint32_t bytes = static_cast<uint32_t>(iterator->offset) | type << 16;
        currentEnd = iterator->end;
        std::cout << "Direct: " << std::hex << "o: 0x" << iterator->offset << " e: 0x" << iterator->end << " t: 0x" <<
            type << " c: 0x" << bytes << std::endl;
        Path::WritePacked(bytes, out);
        Path::WritePacked(currentEnd, out);
    }
}

void ProcessDistinctCodepoints(std::vector<PathOffset>::const_iterator& lastEffectiveIterator, std::ofstream& out,
                               WriteOffestsParams& params, std::vector<uint16_t>& mappings, uint32_t& currentEnd)
{
    auto pos = params.fCpRange.maximumCp;
    if (params.fCpRange.maximumCp != 0) {
        pos++;
    }
    if (lastEffectiveIterator != params.fOffsets.cbegin()) {
        ++lastEffectiveIterator;
    }
    while (lastEffectiveIterator != params.fOffsets.cend()) {
        mappings.push_back(lastEffectiveIterator->code);
        mappings.push_back(pos++);
        uint32_t type = static_cast<uint32_t>(lastEffectiveIterator->type);
        uint32_t bytes = static_cast<uint32_t>(lastEffectiveIterator->offset) | type << 16;
        currentEnd = lastEffectiveIterator->end;
        std::cout << "Distinct: " << std::hex << "code: 0x" << static_cast<int>(lastEffectiveIterator->code) <<
            " o: 0x" << lastEffectiveIterator->offset << " e: 0x" << lastEffectiveIterator->end << " t: " << type <<
            " c: 0x" << bytes << std::endl;
        Path::WritePacked(bytes, out);
        Path::WritePacked(currentEnd, out);
        ++lastEffectiveIterator;
    }
}

static void WriteOffestsToOutFile(ofstream& out, WriteOffestsParams& params, uint32_t currentEnd, bool hasDirect)
{
    if (!params.fOffsets.empty() && params.fOffsets.rbegin()->code == 0) {
        params.fCommonNodeOffset = params.fOffsets.rbegin()->offset;
        params.fOffsets.pop_back();
    }

    auto lastEffectiveIterator = params.fOffsets.cbegin();
    vector<uint16_t> mappings;

    ProcessDirectPointingValues(lastEffectiveIterator, out, params, currentEnd, hasDirect);
    // If we don't have direct code points, mapped ones will have to be differently
    // handled
    if (!hasDirect) {
        params.fCpRange.minimumCp = 0;
        params.fCpRange.maximumCp = 0;
    }

    if (lastEffectiveIterator != params.fOffsets.cend()) {
        // distinct codepoints that cannot be addressed by flat array index
        ProcessDistinctCodepoints(lastEffectiveIterator, out, params, mappings, currentEnd);
    }

    params.fMappingsPos = static_cast<uint32_t>(out.tellp());

    if (!mappings.empty()) {
        Path::WritePacked(mappings, out);
    } else {
        uint32_t dummy{0};
        Path::WritePacked(dummy, out);
    }
}

std::string GetFileNameWithoutSuffix(const std::string& filePath)
{
    size_t lastSlashPos = filePath.find_last_of("/\\");
    size_t lastDotPos = filePath.find_last_of(".");
    std::string fileName = filePath.substr(lastSlashPos + 1, lastDotPos - lastSlashPos - 1);
    return fileName;
}

void CreateDirectory(const std::string& folderPath)
{
    if (mkdir(folderPath.c_str(), 0755) == 0) { // 0755 means the owner has read, write, and execute permissions,
        std::cout << "Directory created successfully: " << folderPath << std::endl;
    } else {
        std::cout << "Directory already exists: " << folderPath << std::endl;
    }
}

void HyphenProcessor::Proccess(const std::string& filePath, const std::string& outFilePath) const
{
    map<string, vector<string>> sections;
    if (ResolveSectionsFromFile(filePath, sections) != SUCCEED) {
        return;
    }

    char resolvedPath[PATH_MAX] = {0};
    if (outFilePath.size() > PATH_MAX) {
        cout << "The file name is too long" << endl;
        return;
    }
    if (realpath(outFilePath.c_str(), resolvedPath) == nullptr) {
        CreateDirectory(resolvedPath);
    }

    vector<vector<uint16_t>> utf16Patterns;
    ResolvePatternsFromSections(sections, utf16Patterns);

    map<uint16_t, PatternHolder> leaves;
    ResolveLeavesFromPatterns(utf16Patterns, leaves);

    CpRange range = {0, 0};
    int countPat = 0;
    BreakLeavesIntoPaths(leaves, range, countPat);

    string filename = GetFileNameWithoutSuffix(filePath);
    std::cout << "output file: " << (outFilePath + "/" + filename + ".hpb") << std::endl;
    ofstream out((outFilePath + "/" + filename + ".hpb"), ios::binary);
    uint32_t tableOffset = InitOutFileHead(out);
    vector<PathOffset> offsets;
    uint32_t toc = 0;

    bool hasDirect = WriteLeavePathsToOutFile(leaves, range, out, tableOffset, offsets);
    toc = static_cast<uint32_t>(out.tellp());
    if ((toc % 0x4) != 0) {
        out.write(reinterpret_cast<const char*>(&toc), toc % 0x4);
        toc = static_cast<uint32_t>(out.tellp());
    }
    // and main table offsets
    cout << "Produced " << offsets.size() << " paths with z: " << toc << endl;

    uint32_t currentEnd = FULL_TALBLE * 2; // initial offset (in 16 bits)
    Path::WritePacked(currentEnd, out);

    uint32_t mappingsPos = 0;
    WriteOffestsParams writeOffestsParams(offsets, mappingsPos, range);
    WriteOffestsToOutFile(out, writeOffestsParams, currentEnd, hasDirect);
    if (FormatOutFileHead(out, writeOffestsParams, toc) != SUCCEED) {
        cout << "DONE: With " << to_string(countPat) << "patterns (8bit)" << endl;
    }
}
} // namespace OHOS::Hyphenate

int main(int argc, char** argv)
{
    if (argc != 3) { // 3: valid argument number
        cout << "usage: './transform hyph-en-us.tex ./out/'" << endl;
        return FAILED;
    }

    // open output
    string filePath = argv[1];
    string outFilePath = argv[2];

    OHOS::Hyphenate::HyphenProcessor hyphenProcessor;
    hyphenProcessor.Proccess(filePath, outFilePath);

    return SUCCEED;
}
