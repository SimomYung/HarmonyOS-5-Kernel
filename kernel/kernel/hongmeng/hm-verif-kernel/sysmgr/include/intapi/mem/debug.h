#ifndef INTAPI_MEM_DEBUG_H
#define INTAPI_MEM_DEBUG_H

#include <libstrict/strict.h>
#include <libhmlog/hmlog.h>
#include <intapi/mem/page.h>

struct lru_cache_s;

#define QUAD_DWORD_LEN (4 * sizeof(unsigned long))
#define __QUAD_DWORD_DUMP(va)							\
	do {									\
		unsigned long *qbuf = (unsigned long *)va;			\
		hm_error("0x%016lx: %016lx %016lx %016lx %016lx\n",		\
			 vaddr_of(qbuf), qbuf[0], qbuf[1], qbuf[2], qbuf[3]);	\
	} while (0)
#define DUMP_MEM_QUAD_DWORD(va, bytes)				\
	do {							\
		BUG_ON(((va) & (QUAD_DWORD_LEN - 1)) != 0);	\
		uintptr_t st = (uintptr_t)(va);			\
		uintptr_t ed = st + (uintptr_t)(bytes);		\
		while (st < ed) {				\
			__QUAD_DWORD_DUMP(st);			\
			st += QUAD_DWORD_LEN;			\
		}						\
	} while (0)

void page_check_free_status(struct page_s *page);
void page_dump_metadata(const struct page_s *page);
void page_dump_content(const struct page_s *page);
bool page_is_lru_locked(const struct page_s *pg, const struct lru_cache_s *lru_cache);

void dump_pa_info(uint64_t pa);

/* check type of sysmgr linear map va */
void dbg_check_page_type(uintptr_t sva, int tid);

void page_dump_for_ptr(uintptr_t ptr);

__attribute__((noinline)) void dbg_dump_page_meta_of_sysva(uintptr_t va);
void dbg_dump_pgtable_reference(struct page_s *page);
#endif /* INTAPI_MEM_DEBUG_H */
