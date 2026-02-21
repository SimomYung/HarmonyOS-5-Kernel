/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 25 12:57:57 2018
 */

/* Never change this guard: it should be same for each arch */
#ifndef UAPI_ARCH_ASM_KERN_SYSCALL_H
#define UAPI_ARCH_ASM_KERN_SYSCALL_H

#include <hmkernel/const.h>

/* divide svcno into 4 parts:
 *
 * 1  bit : proxycall or normal call
 * 1  bit : 32 or 64 bit
 * 9  bits: method
 * 5  bits: cap
 * */
#ifndef ____SYSCALL_SVCNO
# define ____SYSCALL_SVCNO(is_proxy, cap, method)	\
	(((is_proxy) << __U(15)) | (((method) << __U(5)) | (cap)))
#endif

#ifndef __SYSCALL_SVCNO
# define __SYSCALL_SVCNO(cap, method)		____SYSCALL_SVCNO(__U(0), cap, method)
#endif

#ifndef __SYSCALL_PROXY_SVCNO
# define __SYSCALL_PROXY_SVCNO(cap, method)	____SYSCALL_SVCNO(__U(1), cap, method)
#endif

#ifndef __FAST_SYSCALL_SVCNO
# define __FAST_SYSCALL_SVCNO(method)		__SYSCALL_SVCNO(__U(0), method)
#endif

#ifndef __CAP_SYSCALL_SVCNO
# define __CAP_SYSCALL_SVCNO(cap, method)	__SYSCALL_SVCNO(cap, method)
#endif

#ifndef __CAP_SYSCALL_PROXY_SVCNO
# define __CAP_SYSCALL_PROXY_SVCNO(cap, method)	__SYSCALL_PROXY_SVCNO(cap, method)
#endif

#ifndef ____SYSCALL32_SVCNO
# define ____SYSCALL32_SVCNO(is_proxy, cap, method)	\
	(((is_proxy) << __U(15)) | ((((method) << __U(5)) | (cap)) | (__U(1) << __U(14))))
#endif

#ifndef __SYSCALL32_SVCNO
# define __SYSCALL32_SVCNO(cap, method)		____SYSCALL32_SVCNO(__U(0), cap, method)
#endif

#ifndef __SYSCALL32_PROXY_SVCNO
# define __SYSCALL32_PROXY_SVCNO(cap, method)	____SYSCALL32_SVCNO(__U(1), cap, method)
#endif

#ifndef __FAST_SYSCALL32_SVCNO
# define __FAST_SYSCALL32_SVCNO(method)		__SYSCALL32_SVCNO(__U(0), method)
#endif

#ifndef __CAP_SYSCALL32_SVCNO
# define __CAP_SYSCALL32_SVCNO(cap, method)	__SYSCALL32_SVCNO(cap, method)
#endif

#ifndef __CAP_SYSCALL32_PROXY_SVCNO
# define __CAP_SYSCALL32_PROXY_SVCNO(cap, method)	__SYSCALL32_PROXY_SVCNO(cap, method)
#endif

#ifndef __FAST_SYSCALL
# define __FAST_SYSCALL(name, number)
#endif

#ifndef __FAST_SYSCALL_RESERVED
# define __FAST_SYSCALL_RESERVED(number)
#endif

#ifdef __HOST_LLT__
#define LINUX_SYSCALL_SYSNO __FAST_SYSCALL_SVCNO(0)
#else
#ifndef LINUX_SYSCALL_SYSNO
#define LINUX_SYSCALL_SYSNO 0
#endif
#endif
#ifndef LINUX_SYSCALL_NOEMU_SYSNO
#define LINUX_SYSCALL_NOEMU_SYSNO 0xffffU
#endif

__FAST_SYSCALL(linux_syscall, 0)
__FAST_SYSCALL(sched_yield, 1)
__FAST_SYSCALL(syslog, 2)
#define __FAST_SYSCALL_FUTEX_WAIT			__FAST_SYSCALL_SVCNO(3)
__FAST_SYSCALL(futex_wait, 3)
#define __FAST_SYSCALL_FUTEX_WAKE			__FAST_SYSCALL_SVCNO(4)
__FAST_SYSCALL(futex_wake, 4)
__FAST_SYSCALL(futex_wake_ex, 5)
__FAST_SYSCALL(futex_requeue, 6)
__FAST_SYSCALL(flush_cache, 7)
__FAST_SYSCALL(futex_wait_ex, 8)

/*
 * compat rpccall that used for 32bit client,
 * the syscall num is same as arm version.
 */
__FAST_SYSCALL(rpc_compat_g1_nobuf, 9)
__FAST_SYSCALL(rpc_compat_g1_sendbuf, 10)
__FAST_SYSCALL(rpc_compat_g1_retbuf, 11)
__FAST_SYSCALL(rpc_compat_g1_fallback, 12)
__FAST_SYSCALL(wait_for_event, 13)
#define __FAST_SYSCALL_RPC_WFE			__FAST_SYSCALL_SVCNO(13)
__FAST_SYSCALL(actv_done_init, 14)
#define __FAST_SYSCALL_ACTV_DONE_INIT		__FAST_SYSCALL_SVCNO(14)

__FAST_SYSCALL(actv_exit, 15)

/* NOTE: Do not change callno of modulecall,
 * since liblinux has used hard-code 16 for smc adaption */
__FAST_SYSCALL(module_modif_call, 16)

#define __FAST_SYSCALL_BINDER_RPC_WFE		__FAST_SYSCALL_SVCNO(17)
__FAST_SYSCALL_RESERVED(17)
__FAST_SYSCALL(actv_binder_ioctl, 18)

__FAST_SYSCALL(rpc_read_xact_vm, 19)
__FAST_SYSCALL(rpc_write_xact_vm, 20)
__FAST_SYSCALL(rpc_copy_in_xact_vm, 21)
__FAST_SYSCALL(rpc_strcpy_xact_vm, 22)
__FAST_SYSCALL(trigger_wait, 23)
__FAST_SYSCALL(trigger_done, 24)
__FAST_SYSCALL(trigger_read, 25)

__FAST_SYSCALL(tock_read, 26)

/*
 * compat rpccall that used for 32bit client,
 * the syscall num is same as arm version.
 */
__FAST_SYSCALL(rpc_compat_g2_nobuf, 27)
__FAST_SYSCALL(rpc_compat_g2_sendbuf, 28)
__FAST_SYSCALL(rpc_compat_g2_retbuf, 29)
__FAST_SYSCALL(rpc_compat_g2_fallback, 30)

__FAST_SYSCALL(bitlock_wait, 31)

__FAST_SYSCALL(thread_vspace_cref, 36)
__FAST_SYSCALL(bitlock_wake, 37)
__FAST_SYSCALL(thread_set_runqueue, 38)
__FAST_SYSCALL(thread_get_cpu, 39)
__FAST_SYSCALL(lock_wait, 40)
__FAST_SYSCALL(thread_suspend, 41)
__FAST_SYSCALL_RESERVED(42)
__FAST_SYSCALL_RESERVED(43)
__FAST_SYSCALL(thread_set_thread_area, 44)
__FAST_SYSCALL(thread_stat, 45)
__FAST_SYSCALL(thread_set_name, 46)
__FAST_SYSCALL(thread_trigger_detach_one, 47)

__FAST_SYSCALL(thread_cref, 48)
__FAST_SYSCALL(thread_hmsignal_mask, 49)
__FAST_SYSCALL(thread_hmsignal_pending, 50)
__FAST_SYSCALL(thread_hmsignal_unset, 51)

__FAST_SYSCALL(thread_effective_cref, 52)

__FAST_SYSCALL(rpc_strcpy_caller_vm, 53)
__FAST_SYSCALL(rpc_read_caller_vm, 54)
__FAST_SYSCALL(rpc_write_caller_vm, 55)

__FAST_SYSCALL(miscdata_read, 56)
__FAST_SYSCALL(miscdata_write, 57)
__FAST_SYSCALL(miscdata_readv, 58)
__FAST_SYSCALL(miscdata_writev, 59)
__FAST_SYSCALL(miscdata_readv_cas, 60)
__FAST_SYSCALL(miscdata_writev_cas, 61)

__FAST_SYSCALL(actv_load_stat, 62)

__FAST_SYSCALL(cspace_free_cnode_idx, 63)

__FAST_SYSCALL(set_tls, 64)

__FAST_SYSCALL(actv_set_local_area, 65)

__FAST_SYSCALL(actv_cref, 66)

__FAST_SYSCALL(remap_one_pfn, 67)

__FAST_SYSCALL(thread_flush_caller, 68)

__FAST_SYSCALL(linux_sysret, 69)

__FAST_SYSCALL(rpc_g1_nobuf, 70)
#define __FAST_SYSCALL_RPC_GROUP1_NOBUF		__FAST_SYSCALL_SVCNO(70)
__FAST_SYSCALL(rpc_g1_sendbuf, 71)
#define __FAST_SYSCALL_RPC_GROUP1_SENDBUF	__FAST_SYSCALL_SVCNO(71)
__FAST_SYSCALL(rpc_g1_retbuf, 72)
#define __FAST_SYSCALL_RPC_GROUP1_RETBUF	__FAST_SYSCALL_SVCNO(72)
__FAST_SYSCALL(rpc_g1_fallback, 73)
#define __FAST_SYSCALL_RPC_GROUP1_FALLBACK	__FAST_SYSCALL_SVCNO(73)
__FAST_SYSCALL(rpc_g2_nobuf, 74)
#define __FAST_SYSCALL_RPC_GROUP2_NOBUF		__FAST_SYSCALL_SVCNO(74)
__FAST_SYSCALL(rpc_g2_sendbuf, 75)
#define __FAST_SYSCALL_RPC_GROUP2_SENDBUF	__FAST_SYSCALL_SVCNO(75)
__FAST_SYSCALL(rpc_g2_retbuf, 76)
#define __FAST_SYSCALL_RPC_GROUP2_RETBUF	__FAST_SYSCALL_SVCNO(76)
__FAST_SYSCALL(rpc_g2_fallback, 77)
#define __FAST_SYSCALL_RPC_GROUP2_FALLBACK	__FAST_SYSCALL_SVCNO(77)

__FAST_SYSCALL(unmap, 78)

__FAST_SYSCALL(mprotect, 79)

__FAST_SYSCALL(futex_wait_simple, 80)
#define __FAST_SYSCALL_FUTEX_WAIT_SIMPLE	__FAST_SYSCALL_SVCNO(80)

__FAST_SYSCALL(futex_wake_simple, 81)
#define __FAST_SYSCALL_FUTEX_WAKE_SIMPLE	__FAST_SYSCALL_SVCNO(81)

__FAST_SYSCALL(clone_master_area, 82)

__FAST_SYSCALL(kconsole_print, 83)

__FAST_SYSCALL(kconsole_enable, 84)
__FAST_SYSCALL(kconsole_disable, 85)

__FAST_SYSCALL(idle_select_stat, 86)

__FAST_SYSCALL(schedctrl, 87)

__FAST_SYSCALL(membarrier, 88)

__FAST_SYSCALL(klog_rm_kconsole, 89)

/*
 * we should have at least following ctrlmem_write syscall:
 * ctrlmem_write_x
 * ctrlmem_write64_x
 * ctrlmem_write_cas_x
 * ctrlmem_writev_x
 * Where 'x' is the number of extra arguments.
 *
 * Consider ctrlmem_writev:
 * For ctrlmem_writev:
 *   we put argument at head register, then a pointer
 *   to vector elements array. sizeof the array takes following register.
 *   then use left registers to delivers some elements without fetching
 *   from array.
 *
 * Number of elements delivered through registers and array can be encoded
 * in elem_nr.
 *
 * finally use other registers to deliver vector pair as many as possible.
 * Use last usable register to deliver a pointer point other part of elems.
 * e.g.
 *   ctrlmem_writev_3(unsigned long arg1, unsigned long arg2, unsigned long arg3,
 *                    struct vect_elem_s *elems, unsigned long elem_nr,
 *                    void *p1, unsigned long v1)
 *   ctrlmem_writev_2(unsigned long arg1, unsigned long arg2,
 *                    struct vect_elem_s *elems, unsigned long elem_nr,
 *                    void *p1, unsigned long v1)
 *                                               (1 register unused because
 *                                                not able to pass a full element pairwise)
 *   ctrlmem_writev_1(unsigned long arg1,
 *                    struct vect_elem_s *elems, unsigned long elem_nr,
 *                    void *p1, unsigned long v1, void *p2, unsigned long v2)
 *
 *   ctrlmem_writev_0(struct vect_elem_s *elems, unsigned long elem_nr,
 *                    void *p1, unsigned long v1, void *p2, unsigned long v2)
 *
 * Which means writev support at most 3 arguments.
 *
 * So makes all ctrlmem write support at most 3 arguments.
 *
 * ctrlmem_write_arg0/1/2/3    (base: 2 register for pointer and val, at most 5 extra arguments and use 3)
 * ctrlmem_write64_0/1/2/3  (for arm only. base: 3 registers for pointer and val_lo/val_hi,
 *                           4 extra arguments and use 3)
 * ctrlmem_cas_0/1/2/3      (base: 3 registers for pointer, expect and new.
 *                           At most 4 extra arguments and use 3)
 */
__FAST_SYSCALL(ctrlmem_write_arg0, 90)

__FAST_SYSCALL(__ctrlmem_write_r32_v32_arg0, 91)
__FAST_SYSCALL(__ctrlmem_write_r32_v64_arg0, 92)
__FAST_SYSCALL(__ctrlmem_write_r64_v32_arg0, 93)
__FAST_SYSCALL(__ctrlmem_write_r64_v64_arg0, 94)

__FAST_SYSCALL(__ctrlmem_write_r32_v32_arg1, 95)
__FAST_SYSCALL(__ctrlmem_write_r32_v64_arg1, 96)
__FAST_SYSCALL(__ctrlmem_write_r64_v32_arg1, 97)
__FAST_SYSCALL(__ctrlmem_write_r64_v64_arg1, 98)

__FAST_SYSCALL(__ctrlmem_write_r32_v32_arg2, 99)
__FAST_SYSCALL(__ctrlmem_write_r32_v64_arg2, 100)
__FAST_SYSCALL(__ctrlmem_write_r64_v32_arg2, 101)
__FAST_SYSCALL(__ctrlmem_write_r64_v64_arg2, 102)

__FAST_SYSCALL(__ctrlmem_write_r32_v32_arg3, 103)
__FAST_SYSCALL(__ctrlmem_write_r32_v64_arg3, 104)
__FAST_SYSCALL(__ctrlmem_write_r64_v32_arg3, 105)
__FAST_SYSCALL(__ctrlmem_write_r64_v64_arg3, 106)

__FAST_SYSCALL(__ctrlmem_write_r32_v32_arg4, 107)
__FAST_SYSCALL(__ctrlmem_write_r32_v64_arg4, 108)
__FAST_SYSCALL(__ctrlmem_write_r64_v32_arg4, 109)
__FAST_SYSCALL(__ctrlmem_write_r64_v64_arg4, 110)

__FAST_SYSCALL(__ctrlmem_write_r32_v32_arg5, 111)
__FAST_SYSCALL(__ctrlmem_write_r32_v64_arg5, 112)
__FAST_SYSCALL(__ctrlmem_write_r64_v32_arg5, 113)
__FAST_SYSCALL(__ctrlmem_write_r64_v64_arg5, 114)

__FAST_SYSCALL(__cnode_stat_rref32, 115)
__FAST_SYSCALL(__cnode_stat_rref64, 116)

__FAST_SYSCALL(ppoll, 117)
__FAST_SYSCALL(epoll_wait, 118)
__FAST_SYSCALL(epoll_pwait, 119)

__FAST_SYSCALL(flush_cache_by_pa, 120)
__FAST_SYSCALL(actv_cred, 121)

__FAST_SYSCALL(pselect6, 122)

__FAST_SYSCALL(epoll_ctl, 123)

__FAST_SYSCALL(thread_close_on_exec, 124)

__FAST_SYSCALL(rtc_time_ctrl, 125)

__FAST_SYSCALL(__ctrlmem_cas_r32_v32_arg0, 126)
__FAST_SYSCALL(__ctrlmem_cas_r32_v64_arg0, 127)
__FAST_SYSCALL(__ctrlmem_cas_r64_v32_arg0, 128)
__FAST_SYSCALL(__ctrlmem_cas_r64_v64_arg0, 129)

__FAST_SYSCALL(__ctrlmem_cas_r32_v32_arg1, 130)
__FAST_SYSCALL(__ctrlmem_cas_r32_v64_arg1, 131)
__FAST_SYSCALL(__ctrlmem_cas_r64_v32_arg1, 132)
__FAST_SYSCALL(__ctrlmem_cas_r64_v64_arg1, 133)

__FAST_SYSCALL(__ctrlmem_cas_r32_v32_arg2, 134)
__FAST_SYSCALL(__ctrlmem_cas_r32_v64_arg2, 135)
__FAST_SYSCALL(__ctrlmem_cas_r64_v32_arg2, 136)
__FAST_SYSCALL(__ctrlmem_cas_r64_v64_arg2, 137)

__FAST_SYSCALL(__ctrlmem_cas_r32_v32_arg3, 138)
__FAST_SYSCALL(__ctrlmem_cas_r32_v64_arg3, 139)
__FAST_SYSCALL(__ctrlmem_cas_r64_v32_arg3, 140)
__FAST_SYSCALL(__ctrlmem_cas_r64_v64_arg3, 141)

__FAST_SYSCALL(__ctrlmem_cas_r32_v32_arg4, 142)
__FAST_SYSCALL(__ctrlmem_cas_r32_v64_arg4, 143)
__FAST_SYSCALL(__ctrlmem_cas_r64_v32_arg4, 144)
__FAST_SYSCALL(__ctrlmem_cas_r64_v64_arg4, 145)

__FAST_SYSCALL(__ctrlmem_cas_r32_v32_arg5, 146)
__FAST_SYSCALL(__ctrlmem_cas_r32_v64_arg5, 147)
__FAST_SYSCALL(__ctrlmem_cas_r64_v32_arg5, 148)
__FAST_SYSCALL(__ctrlmem_cas_r64_v64_arg5, 149)

__FAST_SYSCALL(ulwt_thread_monitor, 150)
__FAST_SYSCALL(restart_syscall, 151)

__FAST_SYSCALL(kernel_read, 152)
__FAST_SYSCALL(kernel_write, 153)
__FAST_SYSCALL(kernel_close, 154)
__FAST_SYSCALL(keventfd_create, 155)
__FAST_SYSCALL(hvgr_mmu_ctrl, 156)
__FAST_SYSCALL(rpc_zero_xact_vm, 157)
__FAST_SYSCALL(poll, 158)
__FAST_SYSCALL(select, 159)
__FAST_SYSCALL(lsyscall_private_futex, 160)

__FAST_SYSCALL(actv_fpu_begin, 161)
__FAST_SYSCALL(actv_fpu_end, 162)
__FAST_SYSCALL(pause, 163)

__FAST_SYSCALL(clock_gettime, 164)
__FAST_SYSCALL(gettimeofday, 165)
__FAST_SYSCALL(nanosleep, 166)
__FAST_SYSCALL(clock_nanosleep, 167)
__FAST_SYSCALL(dopra_tick, 168)

__FAST_SYSCALL(arm_flush_cache, 169)
__FAST_SYSCALL(flush_caller_pgtbl_dcache, 170)
__FAST_SYSCALL(pgtable_va_to_pa, 171)
__FAST_SYSCALL(oplog_wait, 172)
__FAST_SYSCALL(pm_idle_enter, 173)
__FAST_SYSCALL(hvc_call, 174)
__FAST_SYSCALL(peri_dma_write, 175)

/*
 * the ext fastcall should be the last fastcalls, the EXT_FASTCALL_BASE
 * should be increased synchronously when add new normal fastcalls.
 */
#define __EXT_FASTCALL_CALLNO_BASE		176
#define __EXT_FASTCALL_NR_GLOBAL		21
#define __EXT_FASTCALL_NR_PER_CNODE		5
#define __EXT_FASTCALL_NR_PER_THREAD		5
#define __EXT_FASTCALL_NR_TOTAL			(__EXT_FASTCALL_NR_GLOBAL + \
						 __EXT_FASTCALL_NR_PER_CNODE + \
						 __EXT_FASTCALL_NR_PER_THREAD)
#define __EXT_FASTCALL_CALLNO_BASE_GLOBAL	__EXT_FASTCALL_CALLNO_BASE
#define __EXT_FASTCALL_CALLNO_BASE_PER_CNODE	(__EXT_FASTCALL_CALLNO_BASE + \
						 __EXT_FASTCALL_NR_GLOBAL)
#define __EXT_FASTCALL_CALLNO_BASE_PER_THREAD	(__EXT_FASTCALL_CALLNO_BASE + \
						 __EXT_FASTCALL_NR_GLOBAL + \
						 __EXT_FASTCALL_NR_PER_CNODE)
#define __EXT_FASTCALL_CALLNO_MAX		(__EXT_FASTCALL_CALLNO_BASE + \
						 __EXT_FASTCALL_NR_TOTAL - 1)
#define __EXT_FASTCALL_IS_GLOBAL(scno)		((scno) >= (unsigned int)(__EXT_FASTCALL_CALLNO_BASE_GLOBAL) && \
						 (scno) < (unsigned int)(__EXT_FASTCALL_CALLNO_BASE_PER_CNODE))
#define __EXT_FASTCALL_IS_PER_CNODE(scno)	((scno) >= (unsigned int)(__EXT_FASTCALL_CALLNO_BASE_PER_CNODE) && \
						 (scno) < (unsigned int)(__EXT_FASTCALL_CALLNO_BASE_PER_THREAD))
#define __EXT_FASTCALL_IS_PER_THREAD(scno)	((scno) >= (unsigned int)(__EXT_FASTCALL_CALLNO_BASE_PER_THREAD) && \
						 (scno) <= (unsigned int)(__EXT_FASTCALL_CALLNO_MAX))
#define __EXT_FASTCALL_IS_VALID(scno)		((scno) >= (unsigned int)(__EXT_FASTCALL_CALLNO_BASE) && \
						 (scno) <= (unsigned int)(__EXT_FASTCALL_CALLNO_MAX))
#define __EXT_FASTCALL_MKCALLNO_GLOBAL(index)	(__EXT_FASTCALL_CALLNO_BASE_GLOBAL + (index))
#define __EXT_FASTCALL_MKCALLNO_PER_CNODE(index) (__EXT_FASTCALL_CALLNO_BASE_PER_CNODE + (index))
#define __EXT_FASTCALL_MKCALLNO_PER_THREAD(index) (__EXT_FASTCALL_CALLNO_BASE_PER_THREAD + (index))
#define __EXT_FASTCALL_MKINDEX_GLOBAL(scno)	((scno) - (unsigned int)(__EXT_FASTCALL_CALLNO_BASE_GLOBAL))
#define __EXT_FASTCALL_MKINDEX_PER_CNODE(scno)	((scno) - (unsigned int)(__EXT_FASTCALL_CALLNO_BASE_PER_CNODE))
#define __EXT_FASTCALL_MKINDEX_PER_THREAD(scno) ((scno) - (unsigned int)(__EXT_FASTCALL_CALLNO_BASE_PER_THREAD))

/* ext fastcall type: GLOBAL */
__FAST_SYSCALL(ext_global_0, (__EXT_FASTCALL_CALLNO_BASE_GLOBAL))
__FAST_SYSCALL(ext_global_1, (__EXT_FASTCALL_CALLNO_BASE_GLOBAL + 1))
__FAST_SYSCALL(ext_global_2, (__EXT_FASTCALL_CALLNO_BASE_GLOBAL + 2))
__FAST_SYSCALL(ext_global_3, (__EXT_FASTCALL_CALLNO_BASE_GLOBAL + 3))
__FAST_SYSCALL(ext_global_4, (__EXT_FASTCALL_CALLNO_BASE_GLOBAL + 4))  /* watchdog config for CDC */
__FAST_SYSCALL(ext_global_5, (__EXT_FASTCALL_CALLNO_BASE_GLOBAL + 5))  /* vnotify_notify_channel */
__FAST_SYSCALL(ext_global_6, (__EXT_FASTCALL_CALLNO_BASE_GLOBAL + 6))  /* vnotify_notify_ctrl_channel */
__FAST_SYSCALL(ext_global_7, (__EXT_FASTCALL_CALLNO_BASE_GLOBAL + 7))  /* vnotify_read_chn_msg */
__FAST_SYSCALL(ext_global_8, (__EXT_FASTCALL_CALLNO_BASE_GLOBAL + 8))  /* vnotify_update_futex */
__FAST_SYSCALL(ext_global_9, (__EXT_FASTCALL_CALLNO_BASE_GLOBAL + 9))  /* vnotify_read_status */
__FAST_SYSCALL(ext_global_10, (__EXT_FASTCALL_CALLNO_BASE_GLOBAL + 10))  /* vnotify_inject_event */
__FAST_SYSCALL(ext_global_11, (__EXT_FASTCALL_CALLNO_BASE_GLOBAL + 11))  /* vnotify_clear_vspace */
__FAST_SYSCALL(ext_global_12, (__EXT_FASTCALL_CALLNO_BASE_GLOBAL + 12))  /* iomem_atomic_op_32 */
__FAST_SYSCALL(ext_global_13, (__EXT_FASTCALL_CALLNO_BASE_GLOBAL + 13))  /* cpufreq_update */
__FAST_SYSCALL(ext_global_14, (__EXT_FASTCALL_CALLNO_BASE_GLOBAL + 14))  /* vnotify_query_capacity */
__FAST_SYSCALL(ext_global_15, (__EXT_FASTCALL_CALLNO_BASE_GLOBAL + 15))  /* vnotify_attach_fd */
__FAST_SYSCALL(ext_global_16, (__EXT_FASTCALL_CALLNO_BASE_GLOBAL + 16))  /* vnotify_bind_epoll */
__FAST_SYSCALL(ext_global_17, (__EXT_FASTCALL_CALLNO_BASE_GLOBAL + 17))  /* vnotify_select_channel */
__FAST_SYSCALL(ext_global_18, (__EXT_FASTCALL_CALLNO_BASE_GLOBAL + 18))  /* vnotify_ppoll_channel */
__FAST_SYSCALL(ext_global_19, (__EXT_FASTCALL_CALLNO_BASE_GLOBAL + 19))  /* block_thread */
__FAST_SYSCALL(ext_global_20, (__EXT_FASTCALL_CALLNO_BASE_GLOBAL + 20))  /* unblock_threads */
/* ext fastcall type: PER_CNODE */
__FAST_SYSCALL(ext_per_cnode_0, (__EXT_FASTCALL_CALLNO_BASE_PER_CNODE))
__FAST_SYSCALL(ext_per_cnode_1, (__EXT_FASTCALL_CALLNO_BASE_PER_CNODE + 1))
__FAST_SYSCALL(ext_per_cnode_2, (__EXT_FASTCALL_CALLNO_BASE_PER_CNODE + 2))
__FAST_SYSCALL(ext_per_cnode_3, (__EXT_FASTCALL_CALLNO_BASE_PER_CNODE + 3))
__FAST_SYSCALL(ext_per_cnode_4, (__EXT_FASTCALL_CALLNO_BASE_PER_CNODE + 4))
/* ext fastcall type: PER_THREAD */
__FAST_SYSCALL(ext_per_thread_0, (__EXT_FASTCALL_CALLNO_BASE_PER_THREAD))
__FAST_SYSCALL(ext_per_thread_1, (__EXT_FASTCALL_CALLNO_BASE_PER_THREAD + 1))
__FAST_SYSCALL(ext_per_thread_2, (__EXT_FASTCALL_CALLNO_BASE_PER_THREAD + 2))
__FAST_SYSCALL(ext_per_thread_3, (__EXT_FASTCALL_CALLNO_BASE_PER_THREAD + 3))
__FAST_SYSCALL(ext_per_thread_4, (__EXT_FASTCALL_CALLNO_BASE_PER_THREAD + 4))

#endif
