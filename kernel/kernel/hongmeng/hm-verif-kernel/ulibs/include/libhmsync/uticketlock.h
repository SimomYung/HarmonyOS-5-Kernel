/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 * Description: User space ticketlock structure definition and api
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 7 15:59:22 2025
 */
#ifndef ULIBS_INCLUDE_LIBHMSYNC_UTICKETLOCK_H
#define ULIBS_INCLUDE_LIBHMSYNC_UTICKETLOCK_H

#include <hongmeng/types.h>
#include <vsync/ticketlock.h>

struct uticketlock_s {
	struct ticketlock_s l;
};

#define UTICKETLOCK_INITIALIZER			\
{						\
	.l = TICKETLOCK_INIT(),			\
}

static inline void uticketlock_init(struct uticketlock_s *utl)
{
	ticketlock_init(&utl->l);
}

void disable_uticketlock(void);
void enable_uticketlock(void);
_Bool is_uticketlock_disabled(void);

int uticketlock_lock(struct uticketlock_s *utl);
void uticketlock_unlock(struct uticketlock_s *utl);
int uticketlock_trylock(struct uticketlock_s *utl);

#endif
