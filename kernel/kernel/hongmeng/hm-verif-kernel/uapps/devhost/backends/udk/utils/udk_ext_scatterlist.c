/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Scatterlist handling healpers of UDK
 * Author: Huawei OS Kernel Lab
 * Create: Sat Mar 09 15:17:02 2024
 */
#include <limits.h>

#include <libalgo/rbtree.h>
#include <libhwsecurec/securec.h>
#include <libmem/utils.h>

#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <intapi/mem/linear_map.h>
#include <intapi/mem/sysmem.h>

#include <internal/init.h>
#include <udk/lib/scatterlist.h>
#include <udk/log.h>
#include <udk/iolib.h>

/* sg_next - return the next scatterlist entry in a list */
struct udk_ext_scatterlist *sg_next(struct udk_ext_scatterlist *sg)
{
	if (sg == NULL || sg_is_last(sg)) {
		return NULL;
	}

	sg++;

	if (unlikely(sg_is_chain(sg))) {
		sg = sg_chain_ptr(sg);
	}

	return sg;
}

/*
 * sg_last - return the last scatterlist entry in a list
 * @sgl:	First entry in the scatterlist
 * @nents:	Number of entries in the scatterlist
 */
struct udk_ext_scatterlist *sg_last(struct udk_ext_scatterlist *sgl, unsigned int nents)
{
	struct udk_ext_scatterlist *sg, *ret = NULL;
	unsigned int i;

	if (sgl == NULL) {
		return NULL;
	}

	for_each_ext_sg(sgl, sg, nents, i) {
		ret = sg;
	}

	BUG_ON(!sg_is_last(ret));
	return ret;
}

/* Initialize SG table */
void sg_init_table(struct udk_ext_scatterlist *sgl, unsigned int nents)
{
	if (sgl != NULL && nents > 0) {
		NOFAIL(memset_s(sgl, sizeof(*sgl) * nents, 0, sizeof(*sgl) * nents));
		sg_init_marker(sgl, nents);
	}
}

static struct udk_ext_scatterlist *sg_malloc(unsigned int nents)
{
	return (struct udk_ext_scatterlist *)udk_malloc(nents * sizeof(struct udk_ext_scatterlist));
}

static void sg_free(struct udk_ext_scatterlist *sg)
{
	udk_free(sg);
}

/* __sg_free_table - Free a previously mapped sg table */
static void __sg_free_table(struct udk_ext_sg_table *table, unsigned int max_ents,
			    unsigned int nents_first_chunk, sg_free_fn *free_fn)
{
	struct udk_ext_scatterlist *sgl, *next;
	unsigned curr_max_ents = nents_first_chunk ?: max_ents;

	sgl = table->sgl;
	while (table->orig_nents > 0) {
		unsigned int alloc_size = table->orig_nents;
		unsigned int sg_size;

		/*
		 * If we have more than max_ents segments left,
		 * then assign 'next' to the sg table after the current one.
		 * sg_size is then one less than alloc size, since the last
		 * element is the chain pointer.
		 */
		if (alloc_size > curr_max_ents) {
			next = sg_chain_ptr(&sgl[curr_max_ents - 1]);
			alloc_size = curr_max_ents;
			sg_size = alloc_size - 1;
		} else {
			sg_size = alloc_size;
			next = NULL;
		}

		table->orig_nents -= sg_size;
		if (nents_first_chunk > 0) {
			nents_first_chunk = 0;
		} else {
			free_fn(sgl);
		}
		sgl = next;
		curr_max_ents = max_ents;
	}

	table->sgl = NULL;
}

/**
 * sg_free_table - Free a previously allocated sg table
 * @table:	The mapped sg table header
 *
 **/
void sg_free_table(struct udk_ext_sg_table *table)
{
	if (table != NULL && table->sgl != NULL) {
		__sg_free_table(table, SG_MAX_SINGLE_ALLOC, 0, sg_free);
	}
}

static int __sg_alloc_table(struct udk_ext_sg_table *table,
			    unsigned int nents, unsigned int max_ents,
			    struct udk_ext_scatterlist *first_chunk,
			    unsigned int nents_first_chunk,
			    sg_alloc_fn *alloc_fn)
{
	struct udk_ext_scatterlist *sg = NULL;
	struct udk_ext_scatterlist *prv = NULL;
	unsigned int left = nents;
	unsigned curr_max_ents = nents_first_chunk ?: max_ents;
	unsigned prv_max_ents = 0U;

	mem_zero_s(*table);

	do {
		unsigned int sg_size = 0U;
		unsigned int alloc_size = left;

		if (alloc_size > curr_max_ents) {
			alloc_size = curr_max_ents;
			sg_size = alloc_size - 1;
		} else {
			sg_size = alloc_size;
		}

		left -= sg_size;

		if (first_chunk != NULL) {
			sg = first_chunk;
			first_chunk = NULL;
		} else {
			sg = alloc_fn(alloc_size);
		}
		if (unlikely(sg == NULL)) {
			/*
			 * Adjust entry count to reflect that the last
			 * entry of the previous table won't be used for
			 * linkage.  Without this, sg_free() may get
			 * confused.
			 */
			if (prv != NULL) {
				table->orig_nents += 1;
				table->nents = table->orig_nents;
			}

			return E_HM_NOMEM;
		}

		sg_init_table(sg, alloc_size);
		table->orig_nents += sg_size;
		table->nents = table->orig_nents;

		/*
		 * If this is the first mapping, assign the sg table header.
		 * If this is not the first mapping, chain previous part.
		 */
		if (prv == NULL) {
			table->sgl = sg;
		 } else {
			sg_chain(prv, prv_max_ents, sg);
		 }

		/*
		 * If no more entries after this one, mark the end
		 */
		if (left == 0) {
			sg_mark_end(&sg[sg_size - 1]);
		}

		prv = sg;
		prv_max_ents = curr_max_ents;
		curr_max_ents = max_ents;
	} while (left > 0);

	return E_HM_OK;
}

/* sg_alloc_table - Allocate and initialize an sg table */
int sg_alloc_table(struct udk_ext_sg_table *table, unsigned int nents)
{
	int ret;

	if (table == NULL || nents == 0 || nents > SG_MAX_NENT_ALLOC) {
		return E_HM_INVAL;
	}

	ret = __sg_alloc_table(table, nents, SG_MAX_SINGLE_ALLOC,
			       NULL, 0, sg_malloc);
	if (unlikely(ret != E_HM_OK)) {
		__sg_free_table(table, SG_MAX_SINGLE_ALLOC, 0, sg_free);
	}

	return ret;
}
