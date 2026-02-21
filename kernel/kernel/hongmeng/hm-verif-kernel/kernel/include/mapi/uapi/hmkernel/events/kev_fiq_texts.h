/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: enumerate and functions used for fiq text
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 13 15:57:36 2020
 */

#ifndef UAPI_HMKERNEL_EVENTS_KEV_FIQ_TEXTS_H
#define UAPI_HMKERNEL_EVENTS_KEV_FIQ_TEXTS_H

#define KEV_FIQ_TEXT_LIST \
	DEFINE_KEV_FIQ_TEXT(EL0CE, enter FIQ centry from EL0) \
	DEFINE_KEV_FIQ_TEXT(EL0CX, exit FIQ centry) \
	DEFINE_KEV_FIQ_TEXT(EL1CE, enter FIQ centry from EL1) \
	DEFINE_KEV_FIQ_TEXT(EL1CX, exit FIQ centry) \
	DEFINE_KEV_FIQ_TEXT(USRCE, enter FIQ centry from user) \
	DEFINE_KEV_FIQ_TEXT(USRCX, exit FIQ centry) \
	DEFINE_KEV_FIQ_TEXT(KENCE, enter FIQ centry from kernel) \
	DEFINE_KEV_FIQ_TEXT(KENCX, exit FIQ centry) \
	DEFINE_KEV_FIQ_TEXT(WDTHE, enter watchdog handler) \
	DEFINE_KEV_FIQ_TEXT(WDTHX, exit watchdog handler) \
	DEFINE_KEV_FIQ_TEXT(LASTWORDHE, enter lastword_fiq handler) \
	DEFINE_KEV_FIQ_TEXT(LASTWORDHX, exit lastword_fiq handler)

enum kev_fiq_text_id {
#define DEFINE_KEV_FIQ_TEXT(NAME, ...) KEV_FIQ_TEXT_##NAME,
	KEV_FIQ_TEXT_LIST
	KEV_FIQ_TEXT_NR,
#undef DEFINE_KEV_FIQ_TEXT
};

static const char *kev_fiq_texts[KEV_FIQ_TEXT_NR] = {
#define DEFINE_KEV_FIQ_TEXT(NAME, text, ...) \
	[(unsigned int)KEV_FIQ_TEXT_##NAME] = #text,
	KEV_FIQ_TEXT_LIST
#undef DEFINE_KEV_FIQ_TEXT
};

static inline const char *kev_fiq_text_of_id(unsigned int id)
{
	const char *text = "invalid fiq text id";
	if (id < (unsigned int)KEV_FIQ_TEXT_NR) {
		text = kev_fiq_texts[id];
	}
	return text;
}

#endif
