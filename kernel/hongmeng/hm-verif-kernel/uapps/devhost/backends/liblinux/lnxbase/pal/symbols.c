/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Exported liblinux PAL APIs
 * Author: Huawei OS Kernel Lab
 * Create: Wed Mar 10 15:50:21 2021
 */
#include <hmkernel/compiler.h>
#include <lnxbase/export.h>
#include <lnxbase/vmap.h>
#include <liblinux/pal.h>

/* export liblinux PAL APIs */
/* liblinux/mm.c */
EXPORT_SYMBOL(liblinux_pal_remap_pfn_range);
EXPORT_SYMBOL(liblinux_pal_remap_pa_range_batch);
EXPORT_SYMBOL(liblinux_pal_copy_from_user);
EXPORT_SYMBOL(liblinux_pal_copy_to_user);
EXPORT_SYMBOL(liblinux_pal_copy_in_user);
EXPORT_SYMBOL(liblinux_pal_copy_from_caller);
EXPORT_SYMBOL(liblinux_pal_copy_to_caller);
EXPORT_SYMBOL(liblinux_pal_copy_msghdr_from_user);
EXPORT_SYMBOL(liblinux_pal_copy_msghdr_to_user);
EXPORT_SYMBOL(liblinux_pal_iofast_page_info_init);
EXPORT_SYMBOL(liblinux_pal_vm_prepare);
EXPORT_SYMBOL(liblinux_pal_vm_mmap);
EXPORT_SYMBOL(liblinux_pal_vm_unmap);

/* liblinux/thread.c */
EXPORT_SYMBOL(liblinux_pal_thread_get_my_data);
EXPORT_SYMBOL(liblinux_pal_thread_inactive);
EXPORT_SYMBOL(liblinux_pal_thread_actv_cref);
EXPORT_SYMBOL(liblinux_pal_thread_sched_cref);
/* liblinux/lock.c */
EXPORT_SYMBOL(liblinux_pal_futex_timedwait_ex);
EXPORT_SYMBOL(liblinux_pal_futex_wake);
EXPORT_SYMBOL(liblinux_pal_rpc_invoke);
#ifdef CONFIG_USER_FUTEX
EXPORT_SYMBOL(liblinux_pal_thread_block);
EXPORT_SYMBOL(liblinux_pal_thread_unblock);
#endif /* CONFIG_USER_FUTEX */
EXPORT_SYMBOL(liblinux_pal_mutex_init);
EXPORT_SYMBOL(liblinux_pal_mutex_destroy);
EXPORT_SYMBOL(liblinux_pal_mutex_lock);
EXPORT_SYMBOL(liblinux_pal_mutex_trylock);
EXPORT_SYMBOL(liblinux_pal_mutex_unlock);
EXPORT_SYMBOL(liblinux_pal_mutex_is_locked);
EXPORT_SYMBOL(liblinux_pal_cond_init);
EXPORT_SYMBOL(liblinux_pal_cond_destroy);
EXPORT_SYMBOL(liblinux_pal_cond_wait);
EXPORT_SYMBOL(liblinux_pal_cond_signal);
EXPORT_SYMBOL(liblinux_pal_cond_broadcast);
EXPORT_SYMBOL(liblinux_pal_sem_init);
EXPORT_SYMBOL(liblinux_pal_sem_trywait);
EXPORT_SYMBOL(liblinux_pal_sem_wait);
EXPORT_SYMBOL(liblinux_pal_sem_timedwait);
EXPORT_SYMBOL(liblinux_pal_sem_post);
EXPORT_SYMBOL(liblinux_pal_sem_getvalue);
EXPORT_SYMBOL(liblinux_pal_rwlock_tryrdlock);
EXPORT_SYMBOL(liblinux_pal_rwlock_rdlock);
EXPORT_SYMBOL(liblinux_pal_rwlock_trywrlock);
EXPORT_SYMBOL(liblinux_pal_rwlock_wrlock);
EXPORT_SYMBOL(liblinux_pal_rwlock_unlock);
/* liblinux/dma.c */
EXPORT_SYMBOL(liblinux_pal_dma_alloc);
EXPORT_SYMBOL(liblinux_pal_dma_alloc_coherent);
EXPORT_SYMBOL(liblinux_pal_dma_free);
EXPORT_SYMBOL(liblinux_pal_free_dma_pool);
EXPORT_SYMBOL(liblinux_pal_native_net_rx);
EXPORT_SYMBOL(liblinux_pal_alloc_dma_pool_ex);
EXPORT_SYMBOL(liblinux_pal_flush_cache_range);
EXPORT_SYMBOL(liblinux_pal_flush_pgtbl_cache);
/* liblinux/wakelock.c */
EXPORT_SYMBOL(liblinux_pal_ws_activate);
EXPORT_SYMBOL(liblinux_pal_ws_deactivate);
/* liblinux/mm/kasan.c */
EXPORT_SYMBOL(liblinux_pal_kasan_poison);
EXPORT_SYMBOL(liblinux_pal_kasan_check);
EXPORT_SYMBOL(liblinux_pal_kasan_report);
/* lnxbase/pal/cap.c */
EXPORT_SYMBOL(liblinux_pal_in_group_p);
EXPORT_SYMBOL(liblinux_pal_in_egroup_p);
EXPORT_SYMBOL(liblinux_pal_capable);
/* lnxbase/pal/seharmony.c */
#ifdef CONFIG_SEHARMONY
EXPORT_SYMBOL(liblinux_pal_sel_load_policy);
EXPORT_SYMBOL(liblinux_pal_sel_init_ctx);
EXPORT_SYMBOL(liblinux_pal_sel_update_enforce);
EXPORT_SYMBOL(liblinux_pal_sel_avc_check);
EXPORT_SYMBOL(liblinux_pal_sel_transition_sid);
EXPORT_SYMBOL(liblinux_pal_sel_initialized);
EXPORT_SYMBOL(liblinux_pal_sel_policycap_extsockclass);
EXPORT_SYMBOL(liblinux_pal_sel_allow_unknown);
#endif
/* lnxbase/pal/livepatch.c */
EXPORT_SYMBOL(liblinux_pal_klp_for_each_thread);
/* lnxbase/pal/net.c */
EXPORT_SYMBOL(liblinux_pal_is_compat_client);
/* lnxbase/pal/file.c */
EXPORT_SYMBOL(liblinux_pal_ksys_open);
EXPORT_SYMBOL(liblinux_pal_ksys_close);
EXPORT_SYMBOL(liblinux_pal_ksys_ioctl);
EXPORT_SYMBOL(liblinux_pal_vfs_open);
EXPORT_SYMBOL(liblinux_pal_vfs_close);
EXPORT_SYMBOL(liblinux_pal_vfs_ioctl);
EXPORT_SYMBOL(liblinux_pal_sync_wakeup_trad);
EXPORT_SYMBOL(liblinux_pal_sync_wakeup);
EXPORT_SYMBOL(liblinux_pal_sync_open);
EXPORT_SYMBOL(liblinux_pal_sync_close);
/* lnxbase/pal/vmap.c */
EXPORT_SYMBOL(vmap_tree_foreach);
EXPORT_SYMBOL(vmap_tree_foreach_trylock);
