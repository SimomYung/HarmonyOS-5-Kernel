/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Hmseharmony header file for sel debug
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 31 11:49:29 2023
 */
#ifndef HMSEL_DEBUG_H
#define HMSEL_DEBUG_H

#ifdef HMSEL_DEBUG_ENABLE
#define SEL_DEBUG(...) hm_debug(__VA_ARGS__)
static inline void print_con(const struct hmsel_context *con)
{
	if (con->len != 0) {
		if (con->str != NULL) {
			SEL_DEBUG("invalid con %s\n", con->str);
		}
	} else {
		SEL_DEBUG("valid con %"PRIu32":%"PRIu32":%"PRIu32"\n", con->user, con->role, con->type);
	}
}
#define PRINT_CON(context) print_con(context)
#else
#define SEL_DEBUG(...)
#define PRINT_CON(...)
#endif

#endif
