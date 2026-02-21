/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: UDK stub functions
 * Author: Huawei OS Kernel Lab
 * Create: Thur Jul 20 10:50:54 2023
 */

#define DEFINE_UDK_STUB(func, ret) \
	int func(void); \
	int func(void) { return ret; }

#define DEFINE_UDK_STUB_WEAK(func, ret) \
	int func(void); \
	int __attribute__((weak)) func(void) { return ret; }

/*
 * Fusion udk doesn't support dynamic link,
 * following code never be called.
 */
DEFINE_UDK_STUB(dlsym, 0)
DEFINE_UDK_STUB(dlclose, 0)
DEFINE_UDK_STUB(dlerror, 0)
DEFINE_UDK_STUB(dlopen_from_mem, 0)
DEFINE_UDK_STUB(dlopen, 0)

/*
 * These stub functions are for partial linked vfs.o in LTO mode.
 * And the partial linking should transform to static linking after
 * symbol naming conflict problem is resolved.
 *
 * These stubs are declared as weak symbols to avoid confliction
 * in UT.
 */
DEFINE_UDK_STUB_WEAK(devhost_dma_pool_alloc, 0)
DEFINE_UDK_STUB_WEAK(devhost_dma_pool_free, 0)
DEFINE_UDK_STUB_WEAK(pool_offset2va, 0)
DEFINE_UDK_STUB_WEAK(pool_offset2pa, 0)
DEFINE_UDK_STUB_WEAK(pool_va2offset, 0)

#ifndef __HOST_UT__
DEFINE_UDK_STUB_WEAK(devhost_device_capable_net, 0)
DEFINE_UDK_STUB_WEAK(devhost_pal_request_module, 0)
DEFINE_UDK_STUB_WEAK(devhost_register_loader, 0)
DEFINE_UDK_STUB_WEAK(devhost_postinit, 0)
DEFINE_UDK_STUB_WEAK(devhost_disable_irq, 0)
DEFINE_UDK_STUB_WEAK(devhost_enable_local_irq, 0)
DEFINE_UDK_STUB_WEAK(devhost_disable_local_irq, 0)
DEFINE_UDK_STUB_WEAK(devhost_restore_local_irq, 0)
DEFINE_UDK_STUB_WEAK(devhost_save_local_irq, 0)
DEFINE_UDK_STUB_WEAK(devhost_save_local_flags, 0)
DEFINE_UDK_STUB_WEAK(devhost_unregister_netdev, 0)
DEFINE_UDK_STUB_WEAK(devhost_native_net_rx, 0)
DEFINE_UDK_STUB_WEAK(devhost_native_net_rx_ex, 0)
DEFINE_UDK_STUB_WEAK(devhost_register_netdev, 0)
DEFINE_UDK_STUB_WEAK(devhost_shutdown, 0)
DEFINE_UDK_STUB_WEAK(devhost_snapshot, 0)
DEFINE_UDK_STUB_WEAK(devhost_caller_has_ability, 0)
DEFINE_UDK_STUB_WEAK(devhost_caller_has_capability, 0)
DEFINE_UDK_STUB_WEAK(devhost_caller_has_ab_and_cap, 0)
DEFINE_UDK_STUB_WEAK(devhost_kbox_create, 0)
DEFINE_UDK_STUB_WEAK(devhost_kbox_open, 0)
DEFINE_UDK_STUB_WEAK(devhost_kbox_write, 0)
DEFINE_UDK_STUB_WEAK(devhost_kbox_close, 0)
DEFINE_UDK_STUB_WEAK(devhost_kconsole_print, 0)
DEFINE_UDK_STUB_WEAK(devhost_kconsole_enable, 0)
DEFINE_UDK_STUB_WEAK(devhost_kconsole_disable, 0)
DEFINE_UDK_STUB_WEAK(devhost_kvic_prepare_irq, 0)
DEFINE_UDK_STUB_WEAK(devhost_kill_process_group, 0)
DEFINE_UDK_STUB_WEAK(devhost_kill_process, 0)
DEFINE_UDK_STUB_WEAK(devhost_mysid, 0)
DEFINE_UDK_STUB_WEAK(devhost_getfgpgrp, 0)
DEFINE_UDK_STUB_WEAK(devhost_setfgpgrp, 0)
DEFINE_UDK_STUB_WEAK(devhost_current_pid, 0)
DEFINE_UDK_STUB_WEAK(devhost_current_pgid, 0)
DEFINE_UDK_STUB_WEAK(devhost_getsid, 0)
DEFINE_UDK_STUB_WEAK(devhost_getpgid, 0)
DEFINE_UDK_STUB_WEAK(devhost_session_set_ttyinfo, 0)
#endif
