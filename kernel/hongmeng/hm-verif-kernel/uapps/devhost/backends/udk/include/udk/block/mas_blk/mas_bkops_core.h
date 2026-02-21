/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: bkops core framework
 */

#ifndef __MAS_BKOPS_CORE_H__
#define __MAS_BKOPS_CORE_H__

#ifdef CONFIG_MAS_BKOPS
void mas_bkops_init(struct udk_request_queue *q);
int mas_blk_bkops_enable(unsigned int cmd, unsigned long arg);
#ifdef CONFIG_MAS_DEBUG_FS
/* this is for debug purpose and page is long enough for now */
int mas_queue_bkops_enable_show(void *ctx,
	void *dst, size_t pos, size_t size, size_t *rsize);
#endif
#else
static inline int mas_blk_bkops_enable(unsigned int cmd, unsigned long arg)
{
	UNUSED(cmd);
	UNUSED(arg);
	return 0;
}
#endif
#endif /* __MAS_BKOPS_CORE_H__ */
