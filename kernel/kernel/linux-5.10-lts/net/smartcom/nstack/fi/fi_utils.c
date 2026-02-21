/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 */

#include "fi_utils.h"
#include <linux/stddef.h>
#include <linux/string.h>
#include "fi.h"

#undef HWLOG_TAG
#define HWLOG_TAG SMARTCOM_FI_KERNEL

struct sk_buff *fi_get_netlink_skb(int type, int length, char **data)
{
	struct nlmsghdr *nlh = NULL;
	struct sk_buff *pskb_out = NULL;

	pskb_out = nlmsg_new(length, GFP_ATOMIC);
	if (pskb_out == NULL) {
		fi_loge("Out of memry");
		return NULL;
	}
	nlh = nlmsg_put(pskb_out, 0, 0, type, length, 0);
	if (nlh == NULL) {
		kfree_skb(pskb_out);
		return NULL;
	}
	*data = nlmsg_data(nlh);
	return pskb_out;
}

void fi_enqueue_netlink_skb(struct sk_buff *pskb)
{
	if (skb_queue_len(&g_fi_context.skb_queue) > FI_NL_SKB_QUEUE_MAXLEN) {
		fi_logi("skb_queue len too many, discard the skb");
		kfree_skb(pskb);
		return;
	}

	if (g_fi_context.mproute_app_num > 0)
		nlmsg_hdr(pskb)->nlmsg_type |= FI_NL_MSG_TYPE_MASK;

	NETLINK_CB(pskb).dst_group = 0; /* For unicast */ /*lint !e545*/
	skb_queue_tail(&g_fi_context.skb_queue, pskb);
	up(&g_fi_context.netlink_sync_sema);
}

int fi_netlink_thread(void *data)
{
	struct sk_buff *skb = NULL;
	struct nlmsghdr *nlh = NULL;
	uint32_t netlink_pid = 0;
	fi_logd("fi thread start");

	while (1) {
		if (kthread_should_stop()) {
			fi_logd("fi thread stop");
			break;
		}
		/* netlink thread will block at this semaphone when no data coming. */
		fi_logd("fi_netlink_thread start sema success!");
		down(&g_fi_context.netlink_sync_sema);
		fi_logd("fi_netlink_thread get sema success!");
		do {
			skb = skb_dequeue(&g_fi_context.skb_queue);
			if (skb != NULL) {
				nlh = nlmsg_hdr(skb);
				if (g_fi_context.mproute_pid &&
					(((nlh->nlmsg_type & FI_NL_MSG_TYPE_MASK) >> FI_NL_MSG_TYPE_MASK_LEN) == FI_MPROUTE_SERVICE)) {
					fi_logd("netlink_unicast mproute_pid is [%d]", g_fi_context.mproute_pid);
					netlink_pid = g_fi_context.mproute_pid;
				} else if (g_fi_context.nl_pid) {
					fi_logd("netlink_unicast nl_pid is [%d]", g_fi_context.nl_pid);
					netlink_pid = g_fi_context.nl_pid;
				}

				if (netlink_pid) {
					nlh->nlmsg_type &= ~FI_NL_MSG_TYPE_MASK;
					netlink_unicast(g_fi_context.nlfd, skb, netlink_pid, MSG_DONTWAIT);
					fi_logd("netlink_unicast a message [%u]", nlh->nlmsg_type);
					netlink_pid = 0;
				} else {
					fi_logd("netlink_pid is null");
					kfree_skb(skb);
				}
			} else {
				fi_logd("skb is null");
			}
		} while (!skb_queue_empty(&g_fi_context.skb_queue));
	}
	fi_logd("fi thread while stop");
	return 0;
}

void fi_empty_netlink_skb_queue(void)
{
	struct sk_buff *skb = NULL;
	while (!skb_queue_empty(&g_fi_context.skb_queue)) {
		skb = skb_dequeue(&g_fi_context.skb_queue);
		if (skb != NULL)
			kfree_skb(skb);
	}
}
