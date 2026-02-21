/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Contexthub softtimer driver stub
 * Create: 2024-4-7
 */

#include "smart_softtimer.h"
#include <linux/list.h>

/*
 * Add soft timer
 */
void smart_softtimer_add(struct softtimer_list *timer)
{
}
/*
 * Delete soft timer
 */
int smart_softtimer_delete(struct softtimer_list *timer)
{
	return 0;
}
/*
 * Create a soft timer
 */
int smart_softtimer_create(struct softtimer_list *timer, softtimer_func func,
		unsigned long para, unsigned int timeout)
{
	return 0;
}
/*
 * Modify soft timer
 */
int smart_softtimer_modify(struct softtimer_list *timer, unsigned int new_expire_time)
{
	return 0;
}