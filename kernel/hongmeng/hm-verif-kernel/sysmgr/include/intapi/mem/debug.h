#ifndef INTAPI_MEM_DEBUG_H
#define INTAPI_MEM_DEBUG_H

#include <libstrict/strict.h>
#include <libhmlog/hmlog.h>
#include <intapi/mem/page.h>

struct page_s;
struct lru_cache_s;

void page_check_free_status(struct page_s *page);

bool page_is_lru_locked(const struct page_s *pg, const struct lru_cache_s *lru_cache);
void dump_page_content(struct page_s *page);
void page_dump_metadata(struct page_s *page);
void dump_pa_info(uint64_t pa);

void page_dump_for_ptr(uintptr_t ptr);

#endif /* INTAPI_MEM_DEBUG_H */
