/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: isp aslr implemantation.
 * Create: 2025/2/26
 */

#include <stddef.h>

#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/fcntl.h>
#include <linux/elf.h>
#include <linux/fs.h>

#include "aslr_core_pri.h"

#include "ext_aslr.h"

static int _file_read(struct file *filp, loff_t offset, void *buf, size_t len)
{
    if (vfs_llseek(filp, offset, SEEK_SET) < 0) {
        return -1;
    }
	loff_t pos = filp->f_pos;
    ssize_t n = vfs_read(filp, buf, len, &pos);
    if (n < 0 || (size_t)n != len) {
        return -1;
    }
    return len;
}

static void *file_read(struct file *filp, loff_t offset, size_t len)
{
    void *buf = vzalloc(len);
    if (buf == NULL) {
        return NULL;
    }
    if (_file_read(filp, offset, buf, len) < 0) {
        vfree(buf);
        return NULL;
    }
    return buf;
}

static bool check_elf_ident(const Elf64_Ehdr *ehdr)
{
    const uint8_t magic[SELFMAG] = ELFMAG;
    bool magic_ok = memcmp(ehdr->e_ident, magic, SELFMAG) == 0;
    bool class_ok = ehdr->e_ident[EI_CLASS] == ELFCLASS64;
    bool data_ok = ehdr->e_ident[EI_DATA] == ELFDATA2LSB;
    bool version_ok = ehdr->e_ident[EI_VERSION] == EV_CURRENT;
    return magic_ok && class_ok && data_ok && version_ok;
}

static bool check_elf(const Elf64_Ehdr *ehdr)
{
    bool size_ok = ehdr->e_ehsize == sizeof(Elf64_Ehdr) &&
                   ehdr->e_phentsize == sizeof(Elf64_Phdr) &&
                   ehdr->e_shentsize == sizeof(Elf64_Shdr);
    bool ov_ok = (sizeof(ehdr->e_shentsize) + sizeof(ehdr->e_shnum) <= sizeof(u32)) &&
                 (sizeof(ehdr->e_phentsize) + sizeof(ehdr->e_phnum) <= sizeof(u32));
    bool eident_ok = check_elf_ident(ehdr);
    bool machine_ok = ehdr->e_machine == EM_AARCH64;
    return size_ok && ov_ok && eident_ok && machine_ok;
}

ElfFile *elf_load(const char *fullpath)
{
    ElfFile *elf = (ElfFile *)kzalloc(sizeof(ElfFile), GFP_KERNEL);
    if (elf == NULL) {
        return NULL;
    }

    struct file *filp = filp_open(fullpath, O_RDONLY, 0400);
    if (IS_ERR_OR_NULL(filp)) {
        goto FREE_ELF;
    }

    const Elf64_Ehdr *ehdr = (const Elf64_Ehdr *)file_read(filp, 0, sizeof(Elf64_Ehdr));
    if (ehdr == NULL) {
        goto CLOSE_FILE;
    }
    if (!check_elf(ehdr)) {
        goto FREE_EHDR;
    }
    const Elf64_Phdr *phdr = (const Elf64_Phdr *)file_read(filp, ehdr->e_phoff, ehdr->e_phnum * sizeof(Elf64_Phdr));
    if (phdr == NULL) {
        goto FREE_EHDR;
    }
    const Elf64_Shdr *shdr = (const Elf64_Shdr *)file_read(filp, ehdr->e_shoff, ehdr->e_shnum * sizeof(Elf64_Shdr));
    if (shdr == NULL) {
        goto FREE_PHDR;
    }
    const char *shStrTab = (const char *)file_read(filp, shdr[ehdr->e_shstrndx].sh_offset,
                                                   shdr[ehdr->e_shstrndx].sh_size);
    if (shStrTab == NULL) {
        goto FREE_SHDR;
    }

    elf->filp = filp;
    elf->ehdr = ehdr;
    elf->phdr = phdr;
    elf->shdr = shdr;
    elf->shStrTab = shStrTab;

    return elf;

FREE_SHDR:
    vfree((void *)shdr);
FREE_PHDR:
    vfree((void *)phdr);
FREE_EHDR:
    vfree((void *)ehdr);
CLOSE_FILE:
    filp_close(filp, NULL);
FREE_ELF:
    kfree(elf);
    return NULL;
}

void elf_destroy(ElfFile *elf)
{
    vfree((void *)elf->shStrTab);
    vfree((void *)elf->shdr);
    vfree((void *)elf->phdr);
    vfree((void *)elf->ehdr);
    filp_close(elf->filp, NULL);
    kfree(elf);
}

static const Elf64_Shdr *find_section_by_name(const ElfFile *elf, const char *name)
{
    uint32_t i = 0;
    for (i = 0; i < elf->ehdr->e_shnum; i++) {
        const char *sectName = elf->shStrTab + elf->shdr[i].sh_name;
        if (strcmp(sectName, name) == 0) {
            return &elf->shdr[i];
        }
    }
    return NULL;
}

void *aslr_cb_create(const ElfFile *elf)
{
    AslrCB *aslrCB = (AslrCB *)kzalloc(sizeof(AslrCB), GFP_KERNEL);
    if (aslrCB == NULL) {
        return NULL;
    }

    const Elf64_Shdr *shdr = find_section_by_name(elf, ".rela.dyn");
    if (shdr == NULL) {
        goto FREE_ASLR_CB;
    }

    ImageLinkInfo *linkInfo = (ImageLinkInfo *)kzalloc(sizeof(ImageLinkInfo), GFP_KERNEL);
    if (linkInfo == NULL) {
        goto FREE_ASLR_CB;
    }

    void *reldynStart = file_read(elf->filp, shdr->sh_offset, shdr->sh_size);
    if (reldynStart == NULL) {
        goto FREE_LINK_INFO;
    }

    linkInfo->imageStart = elf->ehdr->e_entry;
    linkInfo->imageEnd = elf->phdr[elf->ehdr->e_phnum - 1].p_vaddr + elf->phdr[elf->ehdr->e_phnum - 1].p_memsz;
    linkInfo->aslrImageStart = elf->ehdr->e_entry;
    linkInfo->aslrImageEnd = elf->ehdr->e_entry;
    linkInfo->reldynStart = (uintptr_t)reldynStart;
    linkInfo->reldynEnd = (uintptr_t)reldynStart + shdr->sh_size;
    linkInfo->startup = elf->ehdr->e_entry;
    linkInfo->magic = ASLR_MAGIC;

    aslrCB->imageInfo.shType = LD_SHT_RELA;
    aslrCB->imageInfo.linkInfo = linkInfo;

    aslrCB->relInfo.relTable = linkInfo->reldynStart;
    aslrCB->relInfo.relTableSize = shdr->sh_size;
    aslrCB->relInfo.relEntrySize = sizeof(Elf64_Rela);
    aslrCB->relInfo.relCount = shdr->sh_size / sizeof(Elf64_Rela);

    aslrCB->random = 0;

    return aslrCB;

FREE_LINK_INFO:
    kfree(linkInfo);
FREE_ASLR_CB:
    kfree(aslrCB);
    return NULL;
}

void aslr_cb_destroy(void *aslr)
{
    AslrCB *aslrCB = (AslrCB *)aslr;
    if (aslrCB == NULL) {
        return;
    }

    vfree((void *)aslrCB->imageInfo.linkInfo->reldynStart);
    aslrCB->imageInfo.linkInfo->reldynStart = 0;
    kfree(aslrCB->imageInfo.linkInfo);
    aslrCB->imageInfo.linkInfo = NULL;
    kfree(aslrCB);
}

int do_aslr_reloc(void *aslr, uint32_t random)
{
    AslrCB *aslrCB = (AslrCB *)aslr;
    aslrCB->random = random;

    uintptr_t ret = OsAslrDoProcess(aslrCB);
    if (ret != LOS_OK) {
        return -1;
    }
    return 0;
}
