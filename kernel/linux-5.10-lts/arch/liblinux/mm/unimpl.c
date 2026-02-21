// SPDX-License-Identifier: GPL-2.0-only
#include <linux/mm_types.h>
#include <liblinux/unimpl.h>

/* mm/memory-failure.c */
#ifdef CONFIG_MEMORY_FAILURE
atomic_long_t num_poisoned_pages __read_mostly = ATOMIC_LONG_INIT(0);
#endif

LIBLINUX_EXPORTED_UNIMPL(flush_dcache_page);

/* mm/gup.c */
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(get_user_pages_fast, int, -ENOSYS);
LIBLINUX_EXPORTED_UNIMPL(unpin_user_page);
/* If you implement unpin_user_page_dirty_lock interface, delete hook_xdp_unpin_user_pages in the net/xdp/xdp_umem.c. */
LIBLINUX_EXPORTED_UNIMPL(unpin_user_pages_dirty_lock);

/* mm/vmscan.c */
atomic64_t shrink_msleep_count;
#ifdef CONFIG_DMABUF_HEAPS_CPA
LIBLINUX_UNIMPL_RET(cpa_shrink_all_memory, unsigned long, 0);
#endif
LIBLINUX_GPL_EXPORTED_UNIMPL(check_move_unevictable_pages);

/* mm/filemap.o */
LIBLINUX_EXPORTED_UNIMPL(end_page_writeback);
LIBLINUX_EXPORTED_UNIMPL_RET(file_check_and_advance_wb_err, int, -ENOSYS);
LIBLINUX_EXPORTED_UNIMPL_RET(file_write_and_wait_range, int, -ENOSYS);
LIBLINUX_EXPORTED_UNIMPL_RET(grab_cache_page_write_begin, void *, NULL);
LIBLINUX_EXPORTED_UNIMPL_RET(pagecache_get_page, void *, NULL);
LIBLINUX_EXPORTED_UNIMPL_RET(pagecache_write_begin, int, -ENOSYS);
LIBLINUX_EXPORTED_UNIMPL_RET(pagecache_write_end, int, -ENOSYS);
LIBLINUX_EXPORTED_UNIMPL_RET(read_cache_page, void *, ERR_PTR(-ENOMEM));
LIBLINUX_EXPORTED_UNIMPL_RET(read_cache_page_gfp, void *, ERR_PTR(-ENOMEM));
LIBLINUX_EXPORTED_UNIMPL(unlock_page);
LIBLINUX_EXPORTED_UNIMPL(__filemap_set_wb_err);
LIBLINUX_EXPORTED_UNIMPL(__lock_page);
LIBLINUX_EXPORTED_UNIMPL_RET(generic_file_readonly_mmap, int, -ENOSYS);
LIBLINUX_UNIMPL_RET(__add_to_page_cache_locked, int, -ENOSYS);

/* mm/page-writeback.c */
unsigned int dirty_writeback_interval = 5 * 100;
int laptop_mode;
LIBLINUX_UNIMPL(account_page_dirtied);
LIBLINUX_EXPORTED_UNIMPL(balance_dirty_pages_ratelimited);
LIBLINUX_EXPORTED_UNIMPL_RET(bdi_set_max_ratio, int, 0);
LIBLINUX_UNIMPL_RET(bdi_set_min_ratio, int, 0);
LIBLINUX_UNIMPL(global_dirty_limits);
LIBLINUX_UNIMPL(laptop_io_completion);
LIBLINUX_UNIMPL(laptop_mode_timer_fn);
LIBLINUX_EXPORTED_UNIMPL_RET(redirty_page_for_writepage, int, -ENOSYS);
LIBLINUX_EXPORTED_UNIMPL_RET(set_page_dirty, int, 0);
LIBLINUX_EXPORTED_UNIMPL_RET(set_page_dirty_lock, int, 0);
LIBLINUX_GPL_EXPORTED_UNIMPL(wait_for_stable_page);
LIBLINUX_UNIMPL_RET(wb_calc_thresh, unsigned long, 0);
LIBLINUX_EXPORTED_UNIMPL(__cancel_dirty_page);
LIBLINUX_UNIMPL_RET(__set_page_dirty_no_writeback, int, 0);
LIBLINUX_EXPORTED_UNIMPL_RET(__test_set_page_writeback, int, 0);

/* mm/swap.o */
LIBLINUX_EXPORTED_UNIMPL_RET(pagevec_lookup_range, unsigned, 0);
LIBLINUX_EXPORTED_UNIMPL(__pagevec_release);
LIBLINUX_UNIMPL(lru_add_drain_all);
LIBLINUX_EXPORTED_UNIMPL(mark_page_accessed);
LIBLINUX_UNIMPL(lru_add_drain_cpu);

/* mm/truncate.o */
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(invalidate_inode_pages2_range, int, 0);
LIBLINUX_EXPORTED_UNIMPL_RET(invalidate_mapping_pages, unsigned long, 0);
LIBLINUX_EXPORTED_UNIMPL(truncate_inode_pages);
LIBLINUX_EXPORTED_UNIMPL(truncate_inode_pages_range);
LIBLINUX_EXPORTED_UNIMPL_RET(try_to_release_page, int, 0);
LIBLINUX_EXPORTED_UNIMPL(pagecache_isize_extended);

/* mm/memory.c */
LIBLINUX_EXPORTED_UNIMPL(unmap_mapping_range);
LIBLINUX_EXPORTED_UNIMPL_RET(vm_insert_pages, int, 0);
/* If you implement vm_insert_page interface, delete hook_sk_vm_insert_page in the packet_mmap function. */
LIBLINUX_EXPORTED_UNIMPL_RET(vm_insert_page, int, 0);
LIBLINUX_UNIMPL(zap_page_range);
LIBLINUX_GPL_EXPORTED_UNIMPL_RET(access_process_vm, int, 0);

/* mm/compaction.c */
LIBLINUX_UNIMPL(cpa_compact_nodes);

/* mm/oom_kill.c */
LIBLINUX_UNIMPL_RET(find_lock_task_mm, void *, NULL);

/* mm/gup.c */
LIBLINUX_EXPORTED_UNIMPL_RET(get_user_pages, long, -ENOSYS)
/* If you implement pin_user_pages interface, delete hook_xdp_pin_user_pages in the net/xdp/xdp_umem.c. */
LIBLINUX_EXPORTED_UNIMPL_RET(pin_user_pages, long, -ENOSYS)
LIBLINUX_EXPORTED_UNIMPL_RET(pin_user_pages_remote, long, -ENOSYS)



/* mm/rmap.c */
LIBLINUX_EXPORTED_UNIMPL_RET(page_mkclean, int, 0)

/* mm/migrate.c */
LIBLINUX_UNIMPL_RET(alloc_migration_target, void *, NULL);

/* mm/compaction.c */
LIBLINUX_UNIMPL_RET(isolate_freepages_range, long, 0);
LIBLINUX_UNIMPL_RET(isolate_migratepages_range, long, 0);
LIBLINUX_UNIMPL_RET(migrate_pages, int, -ENOSYS);
LIBLINUX_UNIMPL_RET(migrate_prep, int, 0);
LIBLINUX_UNIMPL_RET(reclaim_clean_pages_from_list, unsigned int, 0);
LIBLINUX_UNIMPL(putback_movable_pages);

/* mm/page_isolation.c */
LIBLINUX_UNIMPL_RET(start_isolate_page_range, int, -ENOSYS);
LIBLINUX_UNIMPL_RET(test_pages_isolated, int, -ENOSYS);
LIBLINUX_UNIMPL(undo_isolate_page_range);

/* mm/hotplug.c */
#ifdef CONFIG_MEMORY_HOTPLUG
bool movable_node_enabled = false;
void get_online_mems(void) {}
void put_online_mems(void) {}
LIBLINUX_UNIMPL_RET(try_online_node, int, 0);
#endif

/* arch/arm64/mm/mmu.c */
LIBLINUX_UNIMPL(__set_fixmap);

/* mm/vmalloc.c */
struct vm_struct;
LIBLINUX_UNIMPL_RET(find_vm_area, struct vm_struct *, NULL);
LIBLINUX_UNIMPL(unmap_kernel_range);
