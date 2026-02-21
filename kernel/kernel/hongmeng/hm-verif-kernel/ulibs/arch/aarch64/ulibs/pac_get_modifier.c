/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: typeid as modifier for pac
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 13 15:06:28 2025
 */

#include <libhmactv/actv.h>
#include <libhmlog/hmlog.h>
#include <libsysif/utils_server.h>

extern int actv_hdlr_init(struct arch_actv_local *actv_local);

unsigned long PAC_MODIFIER_VOID;
unsigned long PAC_MODIFIER_COMB_KOBJ_HDLR;
unsigned long PAC_MODIFIER_SELF_INIT_HDLR;
unsigned long PAC_MODIFIER_ACTV_HDLR_INIT;
unsigned long PAC_MODIFIER_HMLOG_VPRINTF;
unsigned long PAC_MODIFIER_SYSIF_HDLR;

void pac_get_modifier(void)
{
	PAC_MODIFIER_VOID = __builtin_get_modifier_bytype(void(*)(void));
	PAC_MODIFIER_COMB_KOBJ_HDLR = __builtin_get_modifier_bytype(&actv_pure_stack_comb_kobj);
	PAC_MODIFIER_SELF_INIT_HDLR = __builtin_get_modifier_bytype(&actv_pure_self_init_hdlr);
	PAC_MODIFIER_ACTV_HDLR_INIT = __builtin_get_modifier_bytype(&actv_hdlr_init);
	PAC_MODIFIER_HMLOG_VPRINTF = __builtin_get_modifier_bytype(&__hmlog_vprintf);
	PAC_MODIFIER_SYSIF_HDLR = __builtin_get_modifier_bytype(&sysif_actvhdlr_default_handler);
}
