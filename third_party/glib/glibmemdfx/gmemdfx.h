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
#ifndef G_MEM_DFX_H
#define G_MEM_DFX_H

#ifdef __cplusplus
extern "C" {
#endif

void __attribute__((visibility("default"))) GMemAllocDfx(void *mem, unsigned int size);
void __attribute__((visibility("default"))) GChainMemFreeDfx(void *mem_chain, unsigned long next_offset);
void __attribute__((visibility("default"))) GMemFreeDfx(void *mem);

void __attribute__((visibility("default"))) GMemPoolAllocDfx(void *mem, unsigned int alignment, unsigned int size);
void __attribute__((visibility("default"))) GMemPoolFreeDfx(void *mem);
#ifdef __cplusplus
}
#endif
#endif