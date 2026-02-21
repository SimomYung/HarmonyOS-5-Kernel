// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: the security_guard_collect.h interface include
 * Create: 2022-7-23
 */

#ifndef _HW_KERNEL_SG_COLLECT_H_
#define _HW_KERNEL_SG_COLLECT_H_

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/list.h>
#define MAX_EVENT_CONTENT_LEN  900
typedef void (*sg_callback)(void);

typedef struct {
	unsigned long event_id; // 风险因子id，全网唯一，在云端申请
	unsigned int version; // 风险因子生产者的版本号
	unsigned int content_len; // 风险数据长度
	char content[0]; // 数组长度不超过900字节，风险数据 json字符串
} event_info;

/*
 * register_report_security_callback - scanner regist to the kernel sg.
 * Description: accept each scanner module regist to the kernel sg
 * @callbackfunc,the scanner trigger func of each module
 * @return: Result of regist.
 *     0, regist correctly.
 *     1, regist failed.
 */
#ifdef CONFIG_HW_KERNEL_SG
unsigned int register_report_security_callback(const sg_callback callback);
#else
static inline unsigned int register_report_security_callback(const sg_callback callback)
{
	return 0;
}
#endif

/*
 * report_security_info - scanner module upload result to kernel sg
 * Description: upload the scanner result to kernel sg
 * @result, scanner result as event_info
 * @return: Result of loading.
 *     0, upload correctly.
 *     1, upload failed.
 */
#ifdef CONFIG_HW_KERNEL_SG
unsigned int report_security_info(const event_info *info);
#else
static inline unsigned int report_security_info(const event_info *info)
{
	return 0;
}
#endif

#endif /* _HW_KERNEL_SG_COLLECT_H_ */
