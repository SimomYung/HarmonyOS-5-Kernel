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

int __override mm_account_pinned_pages(struct mmpin *mmp, size_t size)
{
	unsigned long num_pg;
	struct user_struct *user;
	int ret = 0;

	if (capable(CAP_IPC_LOCK) || !size)
		return 0;

	num_pg = (size >> PAGE_SHIFT) + 2; /* worst case */
	trace_ldk_vh_charge_memlock(num_pg, -ENOBUFS, &ret);
	if (ret < 0)
		return ret;

	user = mmp->user ? : current_user();
	if (!mmp->user) {
		mmp->user = get_uid(user);
		mmp->num_pg = num_pg;
	} else {
		mmp->num_pg += num_pg;
	}

	return 0;
}

void __override mm_unaccount_pinned_pages(struct mmpin *mmp)
{
	if (mmp->user) {
		trace_ldk_vh_uncharge_memlock(mmp->num_pg);
		free_uid(mmp->user);
	}
}
