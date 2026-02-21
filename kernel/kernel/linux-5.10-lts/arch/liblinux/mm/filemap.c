// SPDX-License-Identifier: GPL-2.0-only
#include <linux/page-flags.h>
#include <linux/pagemap.h>

void page_endio(struct page *p, bool is_write, int err)
{
	if (!is_write) {
		if (!err) {
			SetPageUptodate(p);
		} else {
			ClearPageUptodate(p);
			SetPageError(p);
		}
		unlock_page(p);
	} else {
		if (err) {
			struct address_space *mapping;

			SetPageError(p);
			mapping = page_mapping(p);
			if (mapping)
				mapping_set_error(mapping, err);
		}
		end_page_writeback(p);
	}
}
EXPORT_SYMBOL_GPL(page_endio);
