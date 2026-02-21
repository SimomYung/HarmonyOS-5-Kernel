/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Driver notifier interfaces of UDK
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 29 10:52:40 2020
 */
#ifndef	__UDK_NOTIFIER_H__
#define	__UDK_NOTIFIER_H__

/* the notifier succeeds and continues to run the list */
#define UDK_NOTIFY_OK		0x0000
/* the notifier fails and continues to run the list */
#define UDK_NOTIFY_FAIL		0x0002
/* the notifier list should stop */
#define UDK_NOTIFY_STOP		0x8000
/* the notifier succeeds and stops */
#define UDK_NOTIFY_OK_STOP	(UDK_NOTIFY_OK | UDK_NOTIFY_STOP)
/* the notifier fails and stops */
#define UDK_NOTIFY_FAIL_STOP	(UDK_NOTIFY_FAIL | UDK_NOTIFY_STOP)

#define UDK_NOTIFY_TO_ERR(ret) ((ret) & (~UDK_NOTIFY_STOP))

struct udk_notifier;
struct udk_notifier_list;
struct udk_blocking_notifier_list;
typedef int (*udk_notifier_func)(struct udk_notifier *notifier, unsigned long event, void *data);

struct udk_notifier *udk_notifier_alloc(udk_notifier_func func, int priority);
void udk_notifier_destroy(struct udk_notifier *notifier);

struct udk_notifier_list *udk_notifier_list_alloc(void);
void udk_notifier_list_destroy(struct udk_notifier_list *list);
int udk_notifier_list_insert(struct udk_notifier_list *list, struct udk_notifier *notifier);
int udk_notifier_list_delete(struct udk_notifier_list *list, struct udk_notifier *notifier);
int udk_notifier_list_call(const struct udk_notifier_list *list, unsigned long event, void *data);
int udk_notifier_list_call_robust(const struct udk_notifier_list *list,
				  unsigned long event, unsigned long revent, void *data);

struct udk_blocking_notifier_list *udk_blocking_notifier_list_alloc(void);
void udk_blocking_notifier_list_destroy(struct udk_blocking_notifier_list *list);
int udk_blocking_notifier_list_insert(struct udk_blocking_notifier_list *list, struct udk_notifier *notifier);
int udk_blocking_notifier_list_delete(struct udk_blocking_notifier_list *list, struct udk_notifier *notifier);
int udk_blocking_notifier_list_call(struct udk_blocking_notifier_list *list, unsigned long event, void *data);
int udk_blocking_notifier_list_call_robust(struct udk_blocking_notifier_list *list,
					   unsigned long event, unsigned long revent, void *data);

#endif /* __UDK_NOTIFIER_H__ */
