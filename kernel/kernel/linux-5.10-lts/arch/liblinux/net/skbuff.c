// SPDX-License-Identifier: GPL-2.0-only
#include <linux/skbuff.h>
#include <net/sock.h>

#include <trace/hooks/liblinux.h>

static
void hook_sk_page_frag(void *args, struct sock *sk, struct page_frag **pfrag)
{
	/*
	* The original implementation may return task_frag in current.
	* In hm, current is now simulated per activation.
	* So here we always use sk_frag.
	*/
	*pfrag = &sk->sk_frag;
}

INIT_VENDOR_HOOK(ldk_rvh_sk_page_frag, hook_sk_page_frag);
