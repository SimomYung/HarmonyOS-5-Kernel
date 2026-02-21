/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony header file for avtab common
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */

#ifndef HMSEL_AVTAB_COMMON_H
#define HMSEL_AVTAB_COMMON_H

#define HM_AVTAB_ALLOWED		0x0001
#define HM_AVTAB_AUDITALLOW		0x0002
#define HM_AVTAB_AUDITDENY		0x0004
#define HM_AVTAB_AV			(HM_AVTAB_ALLOWED | HM_AVTAB_AUDITALLOW | HM_AVTAB_AUDITDENY)
#define HM_AVTAB_TRANSITION		0x0010
#define HM_AVTAB_MEMBER			0x0020
#define HM_AVTAB_CHANGE			0x0040
#define HM_AVTAB_TYPE			(HM_AVTAB_TRANSITION | HM_AVTAB_MEMBER | HM_AVTAB_CHANGE)
/* extended permissions */
#define HM_AVTAB_XPERMS_ALLOWED		0x0100
#define HM_AVTAB_XPERMS_AUDITALLOW	0x0200
#define HM_AVTAB_XPERMS_DONTAUDIT	0x0400
#define HM_AVTAB_XPERMS			(HM_AVTAB_XPERMS_ALLOWED | \
					HM_AVTAB_XPERMS_AUDITALLOW | \
					HM_AVTAB_XPERMS_DONTAUDIT)
#define HM_AVTAB_ENABLED_OLD		0x80000000 /* reserved for used in cond_avtab */
#define HM_AVTAB_ENABLED		0x8000 /* reserved for used in cond_avtab */

struct hmsel_avtab_key {
	uint16_t stype;
	uint16_t ttype;
	uint16_t tclass;
	uint16_t specified;
};

#endif
