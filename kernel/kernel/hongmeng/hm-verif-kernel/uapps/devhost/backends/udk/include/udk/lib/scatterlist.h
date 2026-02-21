/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: scatter list for UDK
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 12 14:56:34 2023
 */
#ifndef __UDK_SCATTERLIST_H__
#define __UDK_SCATTERLIST_H__

#include <hongmeng/panic.h>
#include <hongmeng/types.h>
#include <udk/types.h>

#define for_each_sg(sglist, sg, nr, __i)				\
	for ((__i) = 0, (sg) = (sglist); (__i) < (nr); (__i)++, (sg)++)

struct udk_scatterlist {
	unsigned long dma_address;
	unsigned int dma_length;
};

struct udk_sg_table {
	struct udk_scatterlist *sgl; /* the list */
	unsigned int nents; /* number of mapped entries */
};

struct udk_ext_scatterlist {
	unsigned long page_link;
	unsigned int offset;
	unsigned int length;
	unsigned long dma_address;
	unsigned int dma_length;
};

struct udk_ext_sg_table {
	struct udk_ext_scatterlist *sgl; /* the list */
	unsigned int nents;  /* number of mapped entries */
	unsigned int orig_nents; /* original size of list */
};

/*
 * Maximum number of entries that will be allocated in one piece, if
 * a list larger than this is required then chaining will be utilized.
 */
#define SG_MAX_SINGLE_ALLOC		(PAGE_SIZE / sizeof(struct udk_ext_scatterlist))
#define SG_MAX_NENT_ALLOC		(1U << 30U)

#define SG_CHAIN	0x01UL
#define SG_END		0x02UL

#define sg_is_chain(sg)		(((sg)->page_link & SG_CHAIN) != 0U)
#define sg_is_last(sg)		(((sg)->page_link & SG_END) != 0U)
#define sg_chain_ptr(sg)	\
	((struct udk_ext_scatterlist *) ((sg)->page_link & ~(SG_CHAIN | SG_END)))

typedef struct udk_ext_scatterlist *(sg_alloc_fn)(unsigned int nents);
typedef void (sg_free_fn)(struct udk_ext_scatterlist *sg);

#define sg_dma_address(sg)	((sg)->dma_address)

#ifdef CONFIG_NEED_SG_DMA_LENGTH
#define sg_dma_len(sg)		((sg)->dma_length)
#else
#define sg_dma_len(sg)		((sg)->length)
#endif

static inline void __sg_chain(struct udk_ext_scatterlist *chain_sg,
			      struct udk_ext_scatterlist *sgl)
{
	/*
	 * offset and length are unused for chain entry. Clear them.
	 */
	chain_sg->offset = 0;
	chain_sg->length = 0;

	/*
	 * Set lowest bit to indicate a link pointer, and make sure to clear
	 * the termination bit if it happens to be set.
	 */
	chain_sg->page_link = (ptr_to_ulong(sgl) | SG_CHAIN) & ~SG_END;
}

/* sg_chain - Chain two sglists together */
static inline void sg_chain(struct udk_ext_scatterlist *prv, unsigned int prv_nents,
			    struct udk_ext_scatterlist *sgl)
{
	__sg_chain(&prv[prv_nents - 1], sgl);
}

/* sg_assign_paddr - Assign a given page physical address to an SG entry */
static inline void sg_assign_paddr(struct udk_ext_scatterlist *sg, uint64_t paddr)
{
	unsigned long page_link = sg->page_link & (SG_CHAIN | SG_END);

	BUG_ON_D(paddr & (SG_CHAIN | SG_END));
	BUG_ON_D(sg_is_chain(sg));
	sg->page_link = page_link | paddr;
}

/* sg_set_page - Set sg entry to point at given paddr of page */
static inline void sg_set_paddr(struct udk_ext_scatterlist *sg, uint64_t paddr,
			       unsigned int len, unsigned int offset)
{
	sg_assign_paddr(sg, paddr);
	sg->offset = offset;
	sg->length = len;
}

static inline uint64_t fetch_sg_page_pa(struct udk_ext_scatterlist *sg)
{
	BUG_ON_D(sg_is_chain(sg));

	return ((sg)->page_link & ~(SG_CHAIN | SG_END));
}

/*
 * Loop over each sg element, following the pointer to a new list if necessary
 */
#define for_each_ext_sg(sglist, sg, nr, __i)	\
	for ((__i) = 0, (sg) = (sglist); (__i) < (nr); (__i)++, (sg) = sg_next(sg))

/*
 * Loop over each sg element in the given sg_table object.
 */
#define for_each_ext_sgtable_sg(sgt, sg, i)		\
	for_each_ext_sg((sgt)->sgl, (sg), (sgt)->orig_nents, (i))

/* Mark the end of the scatterlist */
static inline void sg_mark_end(struct udk_ext_scatterlist *sg)
{
	/* Set termination bit, clear potential chain bit */
	sg->page_link |= SG_END;
	sg->page_link &= ~SG_CHAIN;
}

static inline void sg_init_marker(struct udk_ext_scatterlist *sgl, unsigned int nents)
{
	sg_mark_end(&sgl[nents - 1]);
}

int sg_alloc_table(struct udk_ext_sg_table *table, unsigned int nents);
void sg_free_table(struct udk_ext_sg_table *table);
struct udk_ext_scatterlist *sg_next(struct udk_ext_scatterlist *sg);
void sg_init_table(struct udk_ext_scatterlist *sgl, unsigned int nents);

#endif /* __UDK_SCATTERLIST_H__ */
