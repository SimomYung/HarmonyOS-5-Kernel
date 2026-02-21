/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Exported devhost APIs
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#include <lnxbase/export.h>

#include <hongmeng/types.h>
#include <lnxbase/ksymtab.h>
#include <lnxbase/page_pool.h>
#include <libhmsrv_sys/hm_timer.h>

#include <devhost/pm.h>
#include <devhost/kbox.h>
#include <devhost/net.h>
#include <devhost/rpc.h>
#include <devhost/file.h>
#include <devhost/crypto.h>
#include <devhost/device.h>
#include <devhost/memory.h>
#include <devhost/plugin.h>
#include <devhost/backend.h>
#include <devhost/netlink.h>
#include <devhost/process.h>
#include <devhost/security.h>
#include <devhost/interrupt.h>

#include "internal.h"

/* devhost symbols */
EXPORT_SYMBOL(devhost_create_irq_mapping);
EXPORT_SYMBOL(devhost_irq_translate);
EXPORT_SYMBOL(devhost_free_irq);
EXPORT_SYMBOL(devhost_request_threaded_irq);
EXPORT_SYMBOL(devhost_disable_irq);
EXPORT_SYMBOL(devhost_enable_irq);
EXPORT_SYMBOL(devhost_set_wake_irq);
EXPORT_SYMBOL(devhost_backend_register_umap_ops);
EXPORT_SYMBOL(devhost_task_exit_notify_register);
EXPORT_SYMBOL(devhost_postinit);
EXPORT_SYMBOL(devhost_irq_set_type);
EXPORT_SYMBOL(devhost_irq_setaffinity);
EXPORT_SYMBOL(devhost_check_irq_state);

/* net_if.c */
EXPORT_SYMBOL(pool_va2offset);
EXPORT_SYMBOL(pool_offset2va);
EXPORT_SYMBOL(pool_offset2pa);

/* devhost_api.c */
EXPORT_SYMBOL(devhost_register_driver);
EXPORT_SYMBOL(devhost_acquire_driver);
EXPORT_SYMBOL(devhost_register_device);
EXPORT_SYMBOL(devhost_unregister_device);
EXPORT_SYMBOL(devhost_acquire_device);
EXPORT_SYMBOL(devhost_devinfo_put);
EXPORT_SYMBOL(devhost_svrp_release);
EXPORT_SYMBOL(devhost_shm_release);
EXPORT_SYMBOL(devhost_rpc_invoke);
EXPORT_SYMBOL(devhost_timer_accept_grant);
EXPORT_SYMBOL(devhost_netcall_nic_rx);
EXPORT_SYMBOL(devhost_netcall_nic_rx_ex);
EXPORT_SYMBOL(devhost_acquire_netdev);
EXPORT_SYMBOL(devhost_netdev_update_features);
EXPORT_SYMBOL(devhost_register_netdev);
EXPORT_SYMBOL(devhost_unregister_netdev);
EXPORT_SYMBOL(devhost_shutdown);
EXPORT_SYMBOL(devhost_snapshot);
EXPORT_SYMBOL(devhost_kill_process_group);
EXPORT_SYMBOL(devhost_kill_process);
EXPORT_SYMBOL(devhost_kvic_prepare_irq);
EXPORT_SYMBOL(devhost_kvic_default_ctrl);
EXPORT_SYMBOL(devhost_pmu_enable);
EXPORT_SYMBOL(devhost_pmu_disable);
EXPORT_SYMBOL(devhost_netlink_register_entr);
EXPORT_SYMBOL(devhost_session_set_ttyinfo);
EXPORT_SYMBOL(devhost_register_rtc_ops);

#ifdef CONFIG_DEVHOST_PM_PLUGIN
EXPORT_SYMBOL(devhost_pm_init);
EXPORT_SYMBOL(devhost_pmqos_request_add);
EXPORT_SYMBOL(devhost_pmqos_request_update);
EXPORT_SYMBOL(devhost_pmqos_request_remove);
#endif

EXPORT_SYMBOL(devhost_wakelock_release);
EXPORT_SYMBOL(devhost_wakelock_acquire);
EXPORT_SYMBOL(devhost_freq_driver_init_done);

EXPORT_SYMBOL(devhost_mysid);
EXPORT_SYMBOL(devhost_current_pid);
EXPORT_SYMBOL(devhost_current_pgid);
EXPORT_SYMBOL(devhost_getsid);
EXPORT_SYMBOL(devhost_getpgid);
EXPORT_SYMBOL(devhost_getfgpgrp);
EXPORT_SYMBOL(devhost_setfgpgrp);
EXPORT_SYMBOL(devhost_caller_has_ability);
EXPORT_SYMBOL(devhost_caller_has_capability);
EXPORT_SYMBOL(devhost_caller_has_ab_and_cap);

/* device.c */
EXPORT_SYMBOL(devhost_device_init);
EXPORT_SYMBOL(devhost_device_destroy);
EXPORT_SYMBOL(devhost_device_set_autoload);
EXPORT_SYMBOL(devhost_device_set_isolate);
EXPORT_SYMBOL(devhost_device_set_probeable);
EXPORT_SYMBOL(devhost_device_capable_devfs);
EXPORT_SYMBOL(devhost_device_capable_devfs_ex);
EXPORT_SYMBOL(devhost_device_capable_blk);
EXPORT_SYMBOL(devhost_device_capable_directmap_shm);
EXPORT_SYMBOL(devhost_device_capable_pm);
EXPORT_SYMBOL(devhost_device_capable_net);
EXPORT_SYMBOL(devhost_device_capable_pclk);
EXPORT_SYMBOL(devhost_device_set_anonymous);

/* devhost_timer.c */
EXPORT_SYMBOL(hm_clock_settime);
EXPORT_SYMBOL(hm_clock_gettime);

/* vtimer.c */
EXPORT_SYMBOL(lnxbase_vtimer_init);
EXPORT_SYMBOL(lnxbase_vtimer_set_next_event);

/* rpc_if.c */
EXPORT_SYMBOL(devhost_register_rpc);

/* lnxbase_file.c */
EXPORT_SYMBOL(lnxbase_anon_devinfo_init);

EXPORT_SYMBOL(devhost_kconsole_print);
EXPORT_SYMBOL(devhost_kconsole_enable);
EXPORT_SYMBOL(devhost_kconsole_disable);

/* lnxbase.c */
EXPORT_SYMBOL(lnxbase_shrink_mem);
EXPORT_SYMBOL(lnxbase_support_device_autoprobe);
EXPORT_SYMBOL(__lnxbase_acquire_bootfdt);

/* lnxbase/module.c */
EXPORT_SYMBOL(lnxbase_fill_mod_symbol);
EXPORT_SYMBOL(lnxbase_drop_mod_symbol);

/* lnxbase/sync.c */
EXPORT_SYMBOL(lnxbase_dev_synchronize);
EXPORT_SYMBOL(lnxbase_invoke_on_cpu);

/* lnxbase/page_pool.c */
EXPORT_SYMBOL(lnxbase_page_pool_get);

/* lnxbase/power.c */
EXPORT_SYMBOL(lnxbase_wakelock_stat_enqueue);

/* lnxbase/cache.c */
EXPORT_SYMBOL(lnxbase_flush_cache_range_by_pa);

/* lnxbase/rtc.c */
EXPORT_SYMBOL(lnxbase_rtc_read_pid_and_name);

/* lnxbase/backtrace.c */
EXPORT_SYMBOL(lnxbase_resolve_caller);
