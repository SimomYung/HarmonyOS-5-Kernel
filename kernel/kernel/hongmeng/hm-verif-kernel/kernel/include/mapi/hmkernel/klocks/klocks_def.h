/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Kernel subsystem locks
 * Author: Huawei OS Kernel Lab
 * Create: Aug 05 11:06 2020
 */

#ifndef MAPI_HMKERNEL_KLOCKS_KLOCKS_DEF_H
#define MAPI_HMKERNEL_KLOCKS_KLOCKS_DEF_H

#include <hmkernel/types.h>

/*
 * Reconsider when you want to introduce a new lock type.
 *
 * DO _NOT_ introduce a lock type for each critical section, e.g., for a list,
 * for a rbtree.
 *
 * We recommand each submodule share a same lock type, and each lock in the
 * submodule form a lock class.
 */
/* Kernel subsystem lock definitions */
/* =========== BEGIN LOCK DEFINITIONS ========== */
/*
 * cspace_lock scope:
 * - cspace operations
 */
rwlock_def(cspace)

/*
 * cnode_lock scope:
 * - cnode operations & resources
 */
lock_def(cnode)

/*
 * pages_lock scope:
 * - pages
 * - msegs
 */
lock_def(pages)

/*
 * vspace_lock scope:
 * - vspace
 * - pgtable
 * - kshare
 *
 * We introduce dummy per object lock for user vspace, and only the kvspace lock
 * is used to protect the kvspace.
 *
 * Use {un}lock_vspace_obj to protect the user vspace.
 * the vspace lock defined here only for macro compatible
 * and generate a vspace LOCK type.
 */
lock_def(vspace)
lock_def(kvspace)

/*
 * pmem_lock scope:
 * - pmem
 */
lock_def(pmem)

/*
 * driver_lock scope:
 * - various drivers
 */
lock_def(driver)

/*
 * irq_lock scope:
 * - irqctrl resources
 */
lock_def(irq)

/*
 * timekeep_lock scope:
 * - timekeep
 */
rwlock_def(timekeep)

/*
 * kconsole_lock scope:
 * - kconsole lock protects one-line print to ensure no char crossing
 */
kticketlock_def(kconsole)

/*
 * kconsole_input_lock scope:
 * - kconsole_input lock protects the read_data of kconsole_input
 * - kconsole_input lock do not reply on any other lock and it may
 *   happen when serial rx interrupt comes.
 */
lock_def(kconsole_input)

/*
 * timer_lock scope:
 * - timer_rbt/timer_timerwheel
 */
kticketlock_def(timer)

/*
 * sched_lock scope:
 * - sched_lock protects per tcb properties, but not rq
 */
lock_def(sched)

/* futex_lock scope:
 * - futex_lock protects futex related data structure. Intended to be per-obj lock
 */
lock_def(futex)

/*
 * rtfutex_lock scope:
 * - rtfutex_lock protects task related rtfutex data structure. Intended to be per-task lock
 */
lock_def(rtfutex)

/*
 * cpu_rq_lock:
 * - cpu_rq_lock protects all kinds of sched class's runqueue associated with this cpu
 */
lock_def(cpu_rq)

/*
 * vnotify_lock scope:
 * - vnotify_lock protects vnotify data structure in host driver side.
 */
lock_def(vnotify)

/*
 * hypervisor scope:
 * Protect sensitive data in the VM and VCPU capability objects
 */
rwlock_def(vm)
rwlock_def(vcpu)

/*
 * kserial_buf_lock scope:
 * - kserial_buf_lock protects kernel serial buf
 */
lock_def(kserial_buf)

/*
 * watchdog_state_lock scope:
 * - watchdog_state_lock protects kernel enable/disable watchdog
 */
lock_def(watchdog_state)

/*
 * kev_lock scope:
 * - kev_lock protect the kmsg ringbuffer metadata. Now only klog needs it.
 */
ticketlock_def(kev)

/*
 * rqgrp_lock scope:
 * - rqgrp_lock protects the rq grp updates of bandwidth statistics and stat
 */
lock_def(rqgrp)

/*
 * channel_lock scope:
 * - channel_lock protects channel based IPC.
 */
lock_def(channel)

/*
 * lsyscall scope:
 * - lsyscall info locks.
 */
rwlock_def(lsyscall)

/*
 * stop_machine_state_lock scope:
 * - stop_machine_state_lock protects stop_machine_state_s struct for stop machine.
 */
lock_def(stop_machine_state)

/*
 * lt_lock scope:
 * - lt_lock protects hongmeng load tracking properties.
 */
lock_def(lt)

rwlock_def(pgstrtbl)

/*
 * dsu scope:
 * - LOCK_dsu protects dsu operation context.
 */
lock_def(dsu_pmu)

/*
 * pmu scope:
 * - LOCK_pmu protects pmu operation context.
 */
lock_def(pmu)

/*
 * sched_ind_data scope:
 * - sched_ind_data_lock protect the data of all clusters in sched_indicator.
 */
lock_def(sched_ind_data)

/*
 * cpuctx scope
 * - cpuctx_lock protects cpu context cap state transition
 */
lock_def(cpuctx)

/*
 * eventpoll_page_pool scope:
 * - eventpoll_page_pool protects the page pool for poll/epoll.
 * - eventpoll_page protects the page.
 */
lock_def(eventpoll_page_pool)
lock_def(eventpoll_page)

/*
 * deadline_group_lock scope:
 * - deadline_group_lock protects all deadline groups properties.
 */
lock_def(deadline)

kticketlock_def(xdep_infer)

/*
 * isolate_lock scope:
 * - isolate_lock protects the isolate properties of cpus in the cluster.
 */
lock_def(isolate)

/*
 * epoll scope:
 * - epitem protects the struct epitem_s.
 * - ep_rbtree protects the rbtree of epitem_s.
 * - ep_readylist protects the ready list of epitem_s.
 * - ep_waiterlist protects the waiter list of epwait_s.
 * - ep_entrylist protects the entry list of epwait_s.
 * - ep_release will correctly serialize the operation of close and epoll_ctl.
 */
lock_def(epoll)
lock_def(ep_release)

/*
 * virq scope
 * virq protects the virt irq operation
 */
rwlock_def(virq)

/*
 * page_pool scope:
 * - page_pool protects the global page pool.
 */
lock_def(page_pool)

/*
 * pi scope:
 * - pi protects the priority inheritance field, per-tcb lock.
 */
lock_def(pi)

/*
 * fdtable scope:
 * - fdtable protects the struct cap_fdtable_s per cnode.
 */
lock_def(fdtable)

/*
 * actv_pool scope:
 * - actv_pool_lock protects actv_pool, per-obj lock.
 */
kspinlock_def(actv_pool)

/*
 * ppoll scope:
 * - ppoll protects the struct ppoll_s per tcb.
 */
lock_def(ppoll)

/*
 * poll scope:
 * - poll_wait_list protects the wait list of poll.
 */
lock_def(poll_wait_list)

/*
 * select scope:
 * - select protects the struct select_s per tcb.
 */
lock_def(select)

/*
 * hmpsf scope:
 * - hmpsf protects the struct hmpsf_module_s.
 *
 * NOTE: lock_trace is running out of bits in the 64-bit bitmap. So, we work around this problem by
 * letting locks of hmpsf reuse a common lock type (LOCK_hmpsf). This effectively disables the dead
 * lock detection among these locks. In the future, after lock_trace supports general bit map (with
 * support for more than 64 locks), we will refactor the locks of hmpsf to use separate lock types.
 *
 */
lock_def(hmpsf)

/*
 * hmprobe scope:
 * - hmprobe protects the struct inside hmprobe module.
 */
lock_def(hmprobe)

/*
 * keventfd scope:
 * - keventfd_ctx protects the struct keventfd_ctx.
 * - ulwt_monitorfd_ctx->lock which protects the state of the unique monitorfd of a process
 *   also uses this lock type.
 */
lock_def(keventfd_ctx)

/*
 * ulwt monitor scope
 * 1. ulwt_monitor protects all ulwt monitor operations and all data structures
 *    specific to ulwt in the system as an unique lock.
 * 2. As a per-CNode kspinlock, ulwt_monitor protects data structures related to
 *    ulwt kinfo in a CNode. Refer to kobj_process->ulwt_kinfo_lock_wrapper.ulwt_kinfo_lock.
 */
kspinlock_def(ulwt_monitor)

/*
 * ctrlmem_polllist scop:
 * - ctrlmem_polllist protects polllist revents
 */
lock_def(ctrlmem_polllist)

/*
 * jump_label scope:
 * - jump_label protects jump_label switch.
 */
lock_def(jump_label)

/*
 * sysevent scope:
 * - sysevent_rbuffer protects read operations on rbuffer.
 */
lock_def(sysevent_ringbuffer)

/*
 * perf_tp_events scope:
 * - perf_tp_events protects the perf_events list of tracepoint.
 */
rwlock_def(perf_tp_events)

/* cpufreq_chg scope
 * - cpufreq_chg protects the process of reporting
 *   the CPU frequency adjustment request .
 */
kspinlock_def(cpufreq_chg)

/*
 * cluster_state scope:
 * - cluster_state protects the update oprations of cluster state.
 */
rwlock_def(cluster_state)

rwlock_def(mips_mem_state)

/* power_track scope
 * - power_track protects the process of updating
 *   the CPU time_in_state data.
 */
kspinlock_def(power_track)

kspinlock_def(sched_avg)
rwlock_def(whitelist)

rwlock_def(custom_em_state) /* custom energy model state lock */
kspinlock_def(custom_em) /* custom energy model hook lock */

/*
 * curr_actv scope:
 * - curr_actv protects the use of tcb->curr_actv in other cpus and
 * the operation of Activation_destroy.
 */
lock_def(curr_actv)

/*
 * actv_binder scope:
 * - actv binder operations & resources
 */
lock_def(actv_binder)

/*
 * supervsidx scope:
 * - Protects the OPs of supervsidx.
 */
kspinlock_def(supervsidx)

#ifdef CONFIG_HISI_SMMU
/*
 * smmu domain scope:
 * - protects the global smmu_domain_list.
 */
lock_def(smmu_domain_list)

/*
 * smmu svm domain scope:
 * - protects the global smmu_svm_domain_list.
 */
lock_def(smmu_svm_domain_list)

/*
 * smmu tcu scope:
 * - protects the global smmu tcu status.
 */
lock_def(smmu_media1_tcu)
lock_def(smmu_media2_tcu)
lock_def(smmu_npu_tcu)
lock_def(smmu_pcie_tcu)
lock_def(smmu_pcie1_tcu)

/*
 * smmu_pool scope:
 * - smmu_pool protects the global page pool.
 */
lock_def(smmu_page_pool)
#endif

/* Add several lock types for testing when >= 64 types; currently at 62, adding 5 more */
#ifdef __HOST_LLT__
lock_def(test_lock_1)
lock_def(test_lock_2)
lock_def(test_lock_3)
lock_def(test_lock_4)
lock_def(test_lock_5)
lock_order(4, test_lock_1, test_lock_2, test_lock_3, test_lock_4, test_lock_5)
lock_order(3, test_lock_2, test_lock_3, test_lock_4, test_lock_5)
lock_order(2, test_lock_3, test_lock_4, test_lock_5)
lock_order(1, test_lock_4, test_lock_5)
lock_order(0, test_lock_5, NULL)
#endif

/* =========== END LOCK DEFINITIONS ========== */
/*
 * Reconsider when you want to introduce a new lock type.
 *
 * DO _NOT_ introduce a lock type for each critical section, e.g., for a list,
 * for a rbtree.
 *
 * We recommand each submodule share a same lock type, and each lock in the
 * submodule form a lock class.
 */
/* Lock partial order DAG */
lock_order(8, cspace, driver, timekeep, kconsole, kconsole_input, kserial_buf, kev, pgstrtbl, perf_tp_events)
lock_order(37, cnode, vm, vcpu, driver, fdtable, vspace, kvspace, pmem, cspace, pages, irq, \
	   timekeep, kconsole, timer, sched, kconsole_input, cpu_rq, vnotify, lsyscall, \
	   kserial_buf, kev, channel, pgstrtbl, dsu_pmu, pmu, sched_ind_data, \
	   cpuctx, page_pool, actv_pool, isolate, keventfd_ctx, eventpoll_page, \
	   eventpoll_page_pool, ulwt_monitor, ctrlmem_polllist, ep_release, epoll, perf_tp_events)
lock_order(9, pages, pmem, driver, timekeep, kconsole, kconsole_input, \
	   kserial_buf, kev, pgstrtbl, perf_tp_events)
/* Here we make an appointment that vspace should be grabbed first */
lock_order(21, vspace, kvspace, pmem, pages, driver, timekeep, kconsole, timer, \
	   kconsole_input, kserial_buf, kev, pgstrtbl, cspace, futex, cpuctx, rtfutex, pi, cpu_rq, isolate, deadline, lt, perf_tp_events)
lock_order(11, kvspace, pmem, pages, driver, timekeep, kconsole, timer, \
	   kconsole_input, kserial_buf, kev, pgstrtbl, perf_tp_events)
lock_order(8, pmem, driver, timekeep, kconsole, kconsole_input, kserial_buf, kev, pgstrtbl, perf_tp_events)
lock_order(7, driver, timekeep, kconsole, kconsole_input, kserial_buf, kev, pgstrtbl, perf_tp_events)
lock_order(8, irq, driver, timekeep, kconsole, kconsole_input, \
	   kserial_buf, kev, pgstrtbl, perf_tp_events)
lock_order(3, timekeep, kconsole_input, pgstrtbl, perf_tp_events)
lock_order(6, kconsole, timekeep, kconsole_input, kserial_buf, kev, pgstrtbl, perf_tp_events)
lock_order(2, kconsole_input, pgstrtbl, perf_tp_events)
lock_order(15, timer, irq, timekeep, kconsole, kconsole_input, kserial_buf, kev, pgstrtbl, deadline, \
	   sched_ind_data, pmu, hmpsf, mips_mem_state, perf_tp_events, cluster_state, custom_em_state)
/* Protect local global variables in xdep_infer.c */
lock_order(4, xdep_infer, kev, kconsole, timekeep, perf_tp_events)
lock_order(35, sched, whitelist, irq, timer, timekeep, kconsole, kconsole_input, cpu_rq, \
	   kserial_buf, xdep_infer, kev, pgstrtbl, futex, vspace, channel, dsu_pmu, pmu, \
	   rqgrp, lt, sched_ind_data, cpuctx, deadline, rtfutex, pi, isolate, \
	   ulwt_monitor, perf_tp_events, hmpsf, cpufreq_chg, cluster_state,	\
	   mips_mem_state, curr_actv, driver, stop_machine_state, sched_avg, custom_em_state)
lock_order(19, futex, irq, kvspace, driver, timekeep, kconsole, timer, \
	   pages, kconsole_input, kserial_buf, kev, rqgrp, pgstrtbl, lt, \
	   sched_ind_data, cpuctx, ulwt_monitor, cpu_rq, curr_actv, perf_tp_events)
lock_order(17, cpu_rq, timekeep, kconsole, kev, rqgrp, pgstrtbl, timer, lt, irq, deadline, \
           sched_ind_data, isolate, hmpsf, cpufreq_chg, cluster_state, curr_actv, perf_tp_events, custom_em_state)
lock_order(27, vnotify, vm, vcpu, futex, vspace, kvspace, irq, driver, timekeep, \
	   kconsole, timer, pages, sched, kconsole_input, kserial_buf, kev, pgstrtbl, \
	   sched_ind_data, cpuctx, epoll, fdtable, eventpoll_page_pool, \
	   eventpoll_page, page_pool, poll_wait_list, select, ppoll, perf_tp_events)
lock_order(2, kserial_buf, pgstrtbl, perf_tp_events)
lock_order(6, watchdog_state, timekeep, kconsole, kconsole_input, kev, pgstrtbl, perf_tp_events)
lock_order(3, kev, timekeep, pgstrtbl, perf_tp_events)
lock_order(5, rqgrp, pgstrtbl, timekeep, kev, kconsole, perf_tp_events)
lock_order(9, channel, timekeep, kconsole, kev, pgstrtbl, timer, cpuctx, ulwt_monitor, curr_actv, perf_tp_events)
lock_order(5, lsyscall, pgstrtbl, page_pool, timekeep, kconsole_input, perf_tp_events)
lock_order(2, stop_machine_state, pgstrtbl, perf_tp_events)
lock_order(12, lt, deadline, timekeep, kev, kconsole, timer, isolate, hmpsf, curr_actv, perf_tp_events, irq, sched_avg, custom_em_state)
lock_order(1, pgstrtbl, perf_tp_events)
lock_order(4, dsu_pmu, kev, timekeep, kconsole, perf_tp_events)
lock_order(4, pmu, kev, timekeep, kconsole, perf_tp_events)
lock_order(5, sched_ind_data, kev, timekeep, pgstrtbl, kconsole_input, perf_tp_events)
lock_order(12, cpuctx, timer, timekeep, pmu, kev, kconsole, kconsole_input, \
	   kserial_buf, irq, pgstrtbl, jump_label, perf_tp_events, hmpsf)
lock_order(9, deadline, sched_ind_data, timekeep, kev, kconsole, hmpsf, cpufreq_chg, cluster_state, perf_tp_events, custom_em_state)
lock_order(4, page_pool, timekeep, kconsole_input, pgstrtbl, perf_tp_events)
lock_order(17, rtfutex, kvspace, timer, futex, pi, cpu_rq, timekeep, kconsole, kev, cpuctx, deadline, \
	   pgstrtbl, lt, isolate, rqgrp, hmpsf, ulwt_monitor, perf_tp_events)
lock_order(11, pi, cpu_rq, timekeep, kconsole, cpuctx, timer, kev, futex, isolate, lt, ulwt_monitor, perf_tp_events)
lock_order(4, eventpoll_page_pool, kev, timekeep, kconsole, perf_tp_events)
lock_order(6, eventpoll_page, eventpoll_page_pool, page_pool, kev, timekeep, kconsole, perf_tp_events)
lock_order(16, fdtable, kev, page_pool, timekeep, kconsole, kconsole_input, pgstrtbl, actv_pool, lsyscall, \
	   eventpoll_page_pool, eventpoll_page, ulwt_monitor, ctrlmem_polllist, ep_release, epoll, select, \
	   perf_tp_events)
lock_order(19, actv_pool, sched, vspace, kvspace, timekeep, pages, kserial_buf, kev, pgstrtbl, \
	   kconsole, cpuctx, pmu, dsu_pmu, futex, timer, sched_ind_data, eventpoll_page_pool, eventpoll_page, \
	   curr_actv, perf_tp_events)
lock_order(9, ppoll, sched, timer, timekeep, kev, kconsole, kconsole_input, irq, curr_actv, perf_tp_events)
lock_order(1, poll_wait_list, perf_tp_events)
lock_order(9, select, sched, timer, timekeep, kev, kconsole, kconsole_input, irq, curr_actv, perf_tp_events)
lock_order(3, virq, cnode, sched, perf_tp_events)
lock_order(10, epoll, sched, timekeep, kev, kconsole, eventpoll_page_pool, eventpoll_page, page_pool,	\
	   ulwt_monitor, curr_actv, perf_tp_events)
lock_order(10, ep_release, sched, timekeep, kev, kconsole, eventpoll_page, eventpoll_page_pool, page_pool, \
	   ctrlmem_polllist, epoll, perf_tp_events)
lock_order(5, isolate, timer, timekeep, kev, perf_tp_events, irq)
lock_order(14, keventfd_ctx, eventpoll_page, eventpoll_page_pool, page_pool, epoll, sched, timekeep, \
	   kev, kconsole, fdtable, ppoll, ulwt_monitor, ep_release, ctrlmem_polllist, perf_tp_events)
lock_order(5, hmpsf, timekeep, kev, kconsole, hmprobe, perf_tp_events)
lock_order(5, hmprobe, jump_label, timekeep, kev, kconsole, perf_tp_events)
lock_order(8, ulwt_monitor, kconsole, cpuctx, timekeep, kev, cpu_rq, timer, curr_actv, perf_tp_events)
lock_order(11, ctrlmem_polllist, timekeep, kev, kconsole, ppoll, epoll, eventpoll_page, eventpoll_page_pool, \
	   page_pool, ulwt_monitor, select, perf_tp_events)
lock_order(1, jump_label, perf_tp_events)
lock_order(1, sysevent_ringbuffer, perf_tp_events)
lock_order(3, cpufreq_chg, kev, sched_ind_data, perf_tp_events)
lock_order(2, cluster_state, cpufreq_chg, perf_tp_events)
lock_order(1, power_track, perf_tp_events)
lock_order(5, mips_mem_state, kev, kconsole, timekeep, custom_em_state, perf_tp_events)
lock_order(4, curr_actv, kconsole, timekeep, kev, perf_tp_events)
lock_order(1, actv_binder, perf_tp_events)
lock_order(12, vm, vcpu, actv_pool, sched, futex, cpuctx, timer, irq, kconsole, kev, timekeep, perf_tp_events, vspace)
lock_order(11, vcpu, actv_pool, sched, futex, cpuctx, timer, irq, kconsole, kev, timekeep, curr_actv, perf_tp_events)
lock_order(0, perf_tp_events, NULL)
lock_order(2, sched_avg, kev, perf_tp_events)
lock_order(1, supervsidx, perf_tp_events)
#ifdef CONFIG_HISI_SMMU
lock_order(7, smmu_domain_list, kev, sched, timekeep, kconsole, kconsole_input, smmu_page_pool, perf_tp_events)
lock_order(7, smmu_svm_domain_list, kev, sched, timekeep, kconsole, kconsole_input, smmu_page_pool, perf_tp_events)
lock_order(7, smmu_media1_tcu, kev, sched, timekeep, kconsole, kconsole_input, pgstrtbl, perf_tp_events)
lock_order(7, smmu_media2_tcu, kev, sched, timekeep, kconsole, kconsole_input, pgstrtbl, perf_tp_events)
lock_order(7, smmu_npu_tcu, kev, sched, timekeep, kconsole, kconsole_input, pgstrtbl, perf_tp_events)
lock_order(7, smmu_pcie_tcu, kev, sched, timekeep, kconsole, kconsole_input, pgstrtbl, perf_tp_events)
lock_order(7, smmu_pcie1_tcu, kev, sched, timekeep, kconsole, kconsole_input, pgstrtbl, perf_tp_events)
lock_order(7, smmu_page_pool, kev, sched, timekeep, kconsole, kconsole_input, pgstrtbl, perf_tp_events)
#endif
lock_order(3, custom_em_state, custom_em, kev, perf_tp_events)
lock_order(2, custom_em, kev, perf_tp_events)
lock_order(2, whitelist, kev, perf_tp_events)
#undef MAPI_HMKERNEL_KLOCKS_KLOCKS_DEF_H
#endif
