// SPDX-License-Identifier: GPL-2.0-only
#include <asm/page.h>
#include <asm/io.h>
#include <liblinux/pal.h>
#include <linux/export.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <crypto/if_alg.h>
#include <crypto/skcipher.h>

#include <trace/hooks/liblinux.h>

static int do_alg_copy_to_user_pages(struct af_alg_sgl *sgl, struct msghdr *msg, size_t *len)
{
	unsigned int i;
	size_t copylen, rc;

	for(i = 0; i < sgl->npages && *len > 0; i++) {
		copylen = min_t(size_t, sgl->sg[i].length, *len);

		rc = copy_page_to_iter(sgl->pages[i], sgl->sg[i].offset, copylen, &msg->msg_iter);
		if (rc != copylen) {
			return -EFAULT;
		}

		*len -= copylen;
	}

	return 0;
}

static void hook_alg_copy_to_user_pages(void *args, void *msgi, void *areqi, size_t len, int *err)
{
	struct af_alg_rsgl *rsgl;
	struct msghdr *msg = (struct msghdr *)msgi;
	struct af_alg_async_req *areq = (struct af_alg_async_req *)areqi;

	if (*err != 0 || len == 0) {
		return;
	}

	iov_iter_revert(&msg->msg_iter, len);
	if (msg_data_left(msg) < len) {
		*err = -EINVAL;
		return;
	}

	list_for_each_entry(rsgl, &areq->rsgl_list, list) {
		*err = do_alg_copy_to_user_pages(&rsgl->sgl, msg, &len);
		if (*err != 0 || len == 0) {
			return;
		}
	}
}
INIT_VENDOR_HOOK(ldk_rvh_alg_copy_to_user_pages, hook_alg_copy_to_user_pages);
