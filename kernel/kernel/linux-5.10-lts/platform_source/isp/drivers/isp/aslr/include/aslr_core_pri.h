/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: isp aslr core header file.
 * Create: 2025/04/02
 */

#ifndef _ASLR_CORE_PRI_H
#define _ASLR_CORE_PRI_H

#include <linux/types.h>
#include <linux/elf.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define LOS_OK                              0
#define LOS_NOK                             1

#define OS_R_ARM_AARCH64_RELATIVE           1027

#define LD_ELF_R_TYPE(info)                 ((info) & 0xFFFFFFFFUL)

#define LD_ELF_RELA                         Elf64_Rela
#define LD_SHT_RELA                         SHT_RELA

#define ASLR_MAGIC                          0x5A5A5C5C

typedef struct {
    uint32_t relocType;
    uint32_t shType;        /* REL section or RELA section. */
    uintptr_t position;     /* the address of the place being relocated. */
    uintptr_t symAdd;       /* the address of the symbol. */
    ssize_t addend;         /* the addend for the relocation. */
    uintptr_t baseSegment;  /* the addressing origin of the output segment defining the symbol S. */
    uintptr_t gotAddr;      /* the address of the Global Offset Table. */
    /*
     * T is 1 if the target symbol S has type STT_FUNC and
     * the symbol addresses a Thumb instruction; it is 0 otherwise.
     */
    uint8_t thumbFunc;
#define RESERVE_NUM 7
    uint8_t reserved[RESERVE_NUM];
} LDRelocParam;

typedef struct {
    uintptr_t imageStart;
    uintptr_t imageEnd;
    uintptr_t aslrImageStart;
    uintptr_t aslrImageEnd;
    uintptr_t reldynStart;
    uintptr_t reldynEnd;
    uintptr_t startup;
    uint32_t magic;
} ImageLinkInfo;

typedef struct {
    uintptr_t shType;
    ImageLinkInfo *linkInfo;
} AslrImageInfo;

typedef struct {
    uintptr_t relTable;
    uintptr_t relTableSize;
    uintptr_t relEntrySize;
    uintptr_t relCount;
} AslrRelInfo;

typedef struct {
    AslrRelInfo relInfo;
    LDRelocParam relocParam;
    AslrImageInfo imageInfo;
    uintptr_t random;
} AslrCB;

typedef struct {
    LD_ELF_RELA rela;
} AslrRelaItem;

extern uint32_t ArchAslrRelocImp(const LD_ELF_RELA *rela, const LDRelocParam *relocParam);
extern uintptr_t OsAslrDoProcess(AslrCB *data);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _ASLR_CORE_PRI_H */
