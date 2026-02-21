/*
 * Copyright (c) Huawei Technologies CO., Ltd. 2023-2030. All rights reserved.
 * Description: soc_powerstat_interface.h
 * Author: Hisilicon
 * Create: 2023-12-7
 */
#ifndef __SPMI_SEC_WHITELIST_H__
#define __SPMI_SEC_WHITELIST_H__

/*
#define SPMI_SLAVEID_PMU 9
#define SPMI_SLAVEID_SUBPMU1 2
#define SPMI_SLAVEID_SUBPMU2 3
*/
#define SPMI_SLAVEID_EOF 16

struct spmi_sec_reg {
    unsigned char  sid;
    unsigned short addr;
};

/*
static struct spmi_sec_reg spmi_sec_whitelist_pmu[] = {
    {SPMI_SLAVEID_PMU, 0xC58},
    {SPMI_SLAVEID_PMU, 0xC59},
    {SPMI_SLAVEID_PMU, 0xC68},

    {SPMI_SLAVEID_EOF, 0x0},
};

static struct spmi_sec_reg spmi_sec_whitelist_subpmu1[] = {
    {SPMI_SLAVEID_SUBPMU1, 0xC04},
    {SPMI_SLAVEID_SUBPMU1, 0xC05},

    {SPMI_SLAVEID_EOF, 0x0},
};

static struct spmi_sec_reg spmi_sec_whitelist_subpmu2[] = {
    {SPMI_SLAVEID_SUBPMU2, 0xC0C},
    {SPMI_SLAVEID_SUBPMU2, 0xC0D},

    {SPMI_SLAVEID_EOF, 0x0},
};
*/

static struct spmi_sec_reg* spmi_sec_whitelist[SPMI_SLAVEID_EOF] = {
/*
    [SPMI_SLAVEID_PMU] = spmi_sec_whitelist_pmu,
    [SPMI_SLAVEID_SUBPMU1] = spmi_sec_whitelist_subpmu1,
    [SPMI_SLAVEID_SUBPMU2] = spmi_sec_whitelist_subpmu2,
*/
};

#endif /* end of spmi_sec_whitelist.h */
