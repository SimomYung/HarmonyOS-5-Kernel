// SPDX-License-Identifier: GPL-2.0-only
#include <linux/list.h>
#include <linux/mman.h>
#include <linux/spinlock.h>

struct chunk_data {
	struct list_head list;
	struct pcpu_chunk *chunk;
	int nr_pages;
	struct page *pages[];
};
static LIST_HEAD(pcpu_chunks);
static DEFINE_SPINLOCK(pcpu_chunks_lock);

static void __pcpu_unmap_pages(unsigned long addr, int nr_pages);
static int __pcpu_map_pages(unsigned long addr, struct page **pages, int nr_pages)
{
	int i;
	int err;

	for (i = 0; i < nr_pages; i++) {
		err = liblinux_pal_vm_mmap(page_to_pfn(pages[i]) << PAGE_SHIFT,
					   addr + (i << PAGE_SHIFT),
					   PAGE_SIZE, PROT_READ | PROT_WRITE,
					   LIBLINUX_PAL_REMAP_CACHE);
		if (err < 0) {
			goto err;
		}
	}
	return 0;
err:
	__pcpu_unmap_pages(addr, i);
	return err;
}

static void __pcpu_unmap_pages(unsigned long addr, int nr_pages)
{
	int ret = liblinux_pal_vm_unpopulate((void *)addr, (unsigned long)nr_pages << PAGE_SHIFT);
	if (ret < 0) {
		pr_warn("%s: vm unpopulate failed, ret=%d\n", __func__, ret);
	}
}

static void pcpu_unmap_pages(struct pcpu_chunk *chunk,
			     struct page **pages, int page_start, int page_end)
{
	unsigned int cpu;
	for_each_possible_cpu(cpu) {
		__pcpu_unmap_pages(pcpu_chunk_addr(chunk, cpu, page_start),
				   page_end - page_start);
	}
}

static void pcpu_free_pages(struct pcpu_chunk *chunk,
			    struct page **pages, int page_start, int page_end)
{
	unsigned int cpu;
	int i;

	for_each_possible_cpu(cpu) {
		 for (i = page_start; i < page_end; i++) {
			 struct page **pagep = &pages[pcpu_page_idx(cpu, i)];
			 if (*pagep) {
				 __free_page(*pagep);
				 *pagep = NULL;
			 }
		 }
	}
}

static int pcpu_alloc_pages(struct pcpu_chunk *chunk,
			    struct page **pages, int page_start, int page_end, gfp_t gfp)
{
	unsigned int cpu, tcpu;
	int i;

	for_each_possible_cpu(cpu) {
		for (i = page_start; i < page_end; i++) {
			struct page **pagep = &pages[pcpu_page_idx(cpu, i)];
			*pagep = alloc_pages_node(cpu_to_node(cpu), gfp, 0);
			if (!(*pagep)) {
				goto err;
			}
		}
	}
	return 0;

err:
	while (--i >= page_start) {
		__free_page(pages[pcpu_page_idx(cpu, i)]);
		pages[pcpu_page_idx(cpu, i)] = NULL;
	}
	for_each_possible_cpu(tcpu) {
		if (tcpu == cpu)
			break;
		for (i = page_start; i < page_end; i++) {
			__free_page(pages[pcpu_page_idx(tcpu, i)]);
			pages[pcpu_page_idx(cpu, i)] = NULL;
		}
	}
	return -ENOMEM;
}

static int pcpu_map_pages(struct pcpu_chunk *chunk,
			  struct page **pages, int page_start, int page_end)
{
	unsigned int cpu, tcpu;
	int i, err;

	for_each_possible_cpu(cpu) {
		err = __pcpu_map_pages(pcpu_chunk_addr(chunk, cpu, page_start),
				       &pages[pcpu_page_idx(cpu, page_start)],
				       page_end - page_start);
		if (err < 0) {
			goto err;
		}

		for (i = page_start; i < page_end; i++) {
			pcpu_set_page_chunk(pages[pcpu_page_idx(cpu, i)],
					    chunk);
		}
	}
	return 0;
err:
	for_each_possible_cpu(tcpu) {
		if (tcpu == cpu)
			break;
		__pcpu_unmap_pages(pcpu_chunk_addr(chunk, tcpu, page_start),
				   page_end - page_start);
	}
	return err;
}

static int pcpu_populate_chunk(struct pcpu_chunk *chunk,
			       int page_start, int page_end, gfp_t gfp)
{
	struct chunk_data *cdata = (struct chunk_data *)chunk->data;
	struct page **pages = cdata->pages;

	if (pcpu_alloc_pages(chunk, pages, page_start, page_end, gfp))
		return -ENOMEM;

	if (pcpu_map_pages(chunk, pages, page_start, page_end)) {
		pcpu_free_pages(chunk, pages, page_start, page_end);
		return -ENOMEM;
	}

	return 0;
}

static void pcpu_depopulate_chunk(struct pcpu_chunk *chunk,
				  int page_start, int page_end)
{
	struct chunk_data *cdata = (struct chunk_data *)chunk->data;
	struct page **pages = cdata->pages;
	pcpu_unmap_pages(chunk, pages, page_start, page_end);
	pcpu_free_pages(chunk, pages, page_start, page_end);
}

static struct pcpu_chunk *pcpu_create_chunk(enum pcpu_chunk_type type,
					    gfp_t gfp)
{
	const int nr_pages = pcpu_group_sizes[0] >> PAGE_SHIFT;
	struct pcpu_chunk *chunk;
	struct chunk_data *cdata;
	struct page **pages;
	void *base;
	unsigned long flags;

	chunk = pcpu_alloc_chunk(type, gfp);
	if (!chunk)
		return NULL;

	cdata = pcpu_mem_zalloc(sizeof(*cdata) + nr_pages * sizeof(pages[0]),
				gfp);
	if (!cdata) {
		pcpu_free_chunk(chunk);
		return NULL;
	}
	INIT_LIST_HEAD(&cdata->list);
	cdata->chunk = chunk;
	cdata->nr_pages = nr_pages;

	base = liblinux_pal_vm_prepare(0, pcpu_group_sizes[0],
				       PROT_READ | PROT_WRITE,
				       LIBLINUX_PAL_REMAP_CACHE);
	if (base == NULL) {
		pcpu_mem_free(cdata);
		pcpu_free_chunk(chunk);
		return NULL;
	}

	chunk->data = cdata;
	chunk->base_addr = base;

	spin_lock_irqsave(&pcpu_chunks_lock, flags);
	list_add_tail_rcu(&cdata->list, &pcpu_chunks);
	spin_unlock_irqrestore(&pcpu_chunks_lock, flags);

	pcpu_stats_chunk_alloc();
	trace_percpu_create_chunk(chunk->base_addr);

	return chunk;
}

static void pcpu_destroy_chunk(struct pcpu_chunk *chunk)
{
	const int nr_pages = pcpu_group_sizes[0] >> PAGE_SHIFT;
	unsigned long flags;
	int i;

	if (!chunk)
		return;

	pcpu_stats_chunk_dealloc();
	trace_percpu_destroy_chunk(chunk->base_addr);

	if (chunk->data) {
		struct chunk_data *cdata = (struct chunk_data *)chunk->data;
		struct page **pages = cdata->pages;
		for (i = 0; i < nr_pages; i++) {
			if (pages[i] != NULL) {
				__free_page(pages[i]);
			}
		}
		spin_lock_irqsave(&pcpu_chunks_lock, flags);
		list_del_rcu(&cdata->list);
		spin_unlock_irqrestore(&pcpu_chunks_lock, flags);
		pcpu_mem_free(cdata);
	}

	liblinux_pal_vm_unmap((void *)chunk->base_addr);

	pcpu_free_chunk(chunk);
}

static struct page *pcpu_addr_to_page(void *addr)
{
	struct chunk_data *cdata;
	struct page *pg = NULL;

	rcu_read_lock();
	list_for_each_entry_rcu(cdata, &pcpu_chunks, list) {
		if ((addr >= cdata->chunk->base_addr) &&
		    ((addr - cdata->chunk->base_addr) >> PAGE_SHIFT) < cdata->nr_pages) {
			pg = cdata->pages[(addr - cdata->chunk->base_addr) >> PAGE_SHIFT];
			BUG_ON(pcpu_get_page_chunk(pg) != cdata->chunk);
			break;
		}
	}
	rcu_read_unlock();

	return pg;
}

static int __init pcpu_verify_alloc_info(const struct pcpu_alloc_info *ai)
{
	/* all units must be in a single group */
	if (ai->nr_groups != 1) {
		pr_crit("can't handle more than one group\n");
		return -EINVAL;
	}
	return 0;
}
