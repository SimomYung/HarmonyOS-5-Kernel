/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBLINUX_NET_SKBUFF_H__
#define __LIBLINUX_NET_SKBUFF_H__

#define skb_unshare __skb_unshare
#include_next <linux/skbuff.h>
#undef skb_unshare

static inline struct sk_buff *skb_unshare(struct sk_buff *skb, gfp_t pri)
{
	return skb;
}

#endif	/* __LIBLINUX_NET_SKBUFF_H__ */
