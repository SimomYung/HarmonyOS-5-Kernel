/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: isp aslr header file.
 * Create: 2025/2/26
 */

#ifndef _EXT_ASLR_H
#define _EXT_ASLR_H

#include <linux/types.h>
#include <linux/elf.h>
#include <linux/fs.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct ElfFile {
    struct file *filp;
    const Elf64_Ehdr *ehdr;
    const Elf64_Phdr *phdr;
    const Elf64_Shdr *shdr;
    const char *shStrTab;
} ElfFile;

extern ElfFile *elf_load(const char *fullpath);
extern void elf_destroy(ElfFile *elf);
extern void *aslr_cb_create(const ElfFile *elf);
extern void aslr_cb_destroy(void *aslr);
int do_aslr_reloc(void *aslr, uint32_t random);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _EXT_ASLR_H */
