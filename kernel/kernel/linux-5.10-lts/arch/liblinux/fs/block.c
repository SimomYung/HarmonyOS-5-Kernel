// SPDX-License-Identifier: GPL-2.0-only
#include <linux/compiler.h>
#include <linux/fs.h>
#include <linux/slab.h>

#include "check.h"

void * __override read_part_sector(struct parsed_partitions *state,
				   sector_t n, Sector *p)
{
	struct bio *bio = NULL;
	struct page *page = NULL;
	int ret;

	if (n > get_capacity(state->bdev->bd_disk)) {
		state->access_beyond_eod = true;
		return NULL;
	}

	bio = bio_alloc(GFP_KERNEL, 1);
	if (!bio) {
		pr_err("%s: bio_alloc bio failed\n", __func__);
		return NULL;
	}

	page = alloc_page(GFP_KERNEL);
	if (!page) {
		pr_err("%s: alloc page for sector failed\n", __func__);
		goto out_free_bio;
	}

	bio_set_dev(bio, state->bdev);
	bio->bi_opf = READ;
	bio->bi_iter.bi_sector = (n & (~((1 << (PAGE_SHIFT - 9)) - 1)));
	bio_add_page(bio, page, PAGE_SIZE, 0);

	ret = submit_bio_wait(bio);
	if (ret) {
		pr_err("%s: submit_bio_wait failed, ret=%d\n", __func__, ret);
		goto out_free_bio;
	}
	bio_put(bio);

	p->v = page;
	return (unsigned char *)page_address(page) +
			((n & ((1 << (PAGE_SHIFT - 9)) - 1)) << SECTOR_SHIFT);
out_free_bio:
	bio_free_pages(bio);
	bio_put(bio);
	p->v = NULL;
	return NULL;
}
