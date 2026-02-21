/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: API of Fast syscall list
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 18 03:53:14 2018
 */

#ifndef ULIBS_INCLUDE_HONGMENG_FAST_SYSCALL_LIST_H
#define ULIBS_INCLUDE_HONGMENG_FAST_SYSCALL_LIST_H

#include <hmkernel/attributes/hmattributes.h>
struct peri_dma_write_info;
struct hm_hicrypto_transfer_info;

__hmattr_tag(SYSCALL);

#if !defined(__SYSCALL_FAST_DEFINE0) || \
    !defined(__SYSCALL_FAST_DEFINE1) || \
    !defined(__SYSCALL_FAST_DEFINE2) || \
    !defined(__SYSCALL_FAST_DEFINE3) || \
    !defined(__SYSCALL_FAST_DEFINE4) || \
    !defined(__SYSCALL_FAST_DEFINE5) || \
    !defined(__SYSCALL_FAST_DEFINE6) || \
    !defined(__SYSCALL_FAST_DEFINE7)
#error "You cannot include this header file directly."
#endif

#include <hmkernel/capability.h>

struct __block_param_s;

__SYSCALL_FAST_DEFINE0(linux_syscall)
__SYSCALL_FAST_DEFINE0(linux_sysret)
__SYSCALL_FAST_DEFINE4(syslog, unsigned long, level, const char *, str,
		       unsigned long, size, const cref_t *, p_perm_cref)
__SYSCALL_FAST_DEFINE3(kconsole_print, const char *, str, unsigned long, size,
		       const cref_t *, p_perm_cref)
__SYSCALL_FAST_DEFINE1(kconsole_enable, const cref_t *, p_perm_cref)
__SYSCALL_FAST_DEFINE1(kconsole_disable, const cref_t *, p_perm_cref)
__SYSCALL_FAST_DEFINE1(klog_rm_kconsole, const cref_t *, p_perm_cref)
__SYSCALL_FAST_DEFINE6(futex_wait, volatile int *, ptr, int, key, __u64 *, utimeout,
		       struct __hmsigmask *, sigmask,
		       unsigned long, flags, unsigned long, bitset)
__SYSCALL_FAST_DEFINE4(bitlock_wait, int *, ptr, int, lock_bit, int, wait_bit,
		       const struct __block_param_s *, block_param_uaddr)
__SYSCALL_FAST_DEFINE3(bitlock_wake, int *, ptr, int, num, int *, has_waiter)
__SYSCALL_FAST_DEFINE8(lock_wait, volatile int *, uaddr, int, val,
		       struct __hmsigmask *, sigmask,
		       unsigned long, flags, unsigned long, bitset,
		       unsigned long, bitset2, unsigned long, data,
		       const struct __block_param_s *, block_param_uaddr)
__SYSCALL_FAST_DEFINE8(futex_wait_ex, volatile int *, ptr, int, key, __u64 *, utimeout,
		       struct __hmsigmask *, sigmask,
		       unsigned long, flags, unsigned long, bitset,
			   unsigned long, bitset2, unsigned long, data)
__SYSCALL_FAST_DEFINE3(futex_wait_simple, int *, ptr, int, key,
		       unsigned long, mask_type)
__SYSCALL_FAST_DEFINE4(futex_wake, volatile int *, ptr, unsigned int, num,
		       unsigned long, flags, unsigned long, bitset)
__SYSCALL_FAST_DEFINE5(futex_wake_ex, volatile int *, ptr, unsigned int, num,
		       unsigned long, flags, unsigned long, bitset, unsigned long, bitset2)
__SYSCALL_FAST_DEFINE2(futex_wake_simple, int *, ptr, unsigned int, num)
__SYSCALL_FAST_DEFINE4(futex_requeue, volatile int *, src_ptr, volatile int *, dst_ptr,
		       unsigned int, deq_num, unsigned int, req_num)
__SYSCALL_FAST_DEFINE3(flush_cache, unsigned long, cmd, unsigned long, start, unsigned long, end)
__SYSCALL_FAST_DEFINE3(flush_caller_pgtbl_dcache, const void *, kobj_vspace, unsigned long, start, unsigned long, end)
__SYSCALL_FAST_DEFINE2(pgtable_va_to_pa, unsigned long, vaddr, __u64 *, rpa)
__SYSCALL_FAST_DEFINE2(hvc_call, unsigned long *, in_regs, unsigned long *, out_regs)
__SYSCALL_FAST_DEFINE1(peri_dma_write, struct peri_dma_write_info *, info)
__SYSCALL_FAST_DEFINE6(hicrypto_drv_core, __u64, arg0, __u64, arg1, __u64, arg2, __u64, arg3, __u64, arg4, __u64, arg5)

__SYSCALL_FAST_DEFINE3(flush_cache_by_pa, unsigned long, cmd, unsigned long long, pstart, unsigned long long, pend)
__SYSCALL_FAST_DEFINE3(arm_flush_cache, unsigned long, start, unsigned long, end, unsigned long, flags)
__SYSCALL_FAST_DEFINE0(rpc_g1_nobuf)
__SYSCALL_FAST_DEFINE0(rpc_g1_sendbuf)
__SYSCALL_FAST_DEFINE0(rpc_g1_retbuf)
__SYSCALL_FAST_DEFINE0(rpc_g1_fallback)
__SYSCALL_FAST_DEFINE0(rpc_g2_nobuf)
__SYSCALL_FAST_DEFINE0(rpc_g2_sendbuf)
__SYSCALL_FAST_DEFINE0(rpc_g2_retbuf)
__SYSCALL_FAST_DEFINE0(rpc_g2_fallback)

__SYSCALL_FAST_DEFINE4(wait_for_event, long, retval,
		       void *, data_ptr, void *, buf_ptr,
		       unsigned long, data_buf_size)
__SYSCALL_FAST_DEFINE3(actv_binder_ioctl,
		       int, fd, unsigned long, cmd, unsigned long, arg)
__SYSCALL_FAST_DEFINE3(actv_done_init, long, retval,
		       void *, buf_ptr, unsigned long, data_buf_size)

__SYSCALL_FAST_DEFINE0(sched_yield)

__SYSCALL_FAST_DEFINE2(rpc_zero_xact_vm, void *, dst,
		       unsigned long, size)
__SYSCALL_FAST_DEFINE3(rpc_read_xact_vm,
		     void *, dst, const void *, src,
		     unsigned long, size)
__SYSCALL_FAST_DEFINE3(rpc_write_xact_vm,
		     void *, dst, const void *, src,
		     unsigned long, size)
__SYSCALL_FAST_DEFINE3(rpc_read_caller_vm,
		     void *, dst, const void *, src,
		     unsigned long, size)
__SYSCALL_FAST_DEFINE3(rpc_write_caller_vm,
		     void *, dst, const void *, src,
		     unsigned long, size)
__SYSCALL_FAST_DEFINE3(rpc_copy_in_xact_vm,
		     void *, dst, const void *, src,
		     unsigned long, size)
__SYSCALL_FAST_DEFINE3(rpc_strcpy_xact_vm, void *, dst, const void *, src,
		       unsigned long, size)
__SYSCALL_FAST_DEFINE3(rpc_strcpy_caller_vm, void *, dst, const void *, src,
		       unsigned long, size)
__SYSCALL_FAST_DEFINE1(remap_one_pfn, const struct sysarg_vspace_remap_one_pfn *, args)
__SYSCALL_FAST_DEFINE2(trigger_wait, unsigned long, flags,
		       const struct __hmsigmask *, sigmask)
__SYSCALL_FAST_DEFINE0(trigger_done)
__SYSCALL_FAST_DEFINE2(trigger_read, void *, buf, unsigned long, buf_size)
__SYSCALL_FAST_DEFINE0(oplog_wait)
__SYSCALL_FAST_DEFINE2(tock_read, unsigned int, clkid, __u64 *, utock)
__SYSCALL_FAST_DEFINE2(rtc_time_ctrl, struct __rtc_time_s *, time, enum __rtc_ops, op)

__SYSCALL_FAST_DEFINE2(idle_select_stat, unsigned long, cpuid, void *, info)

__SYSCALL_FAST_DEFINE3(unmap, cref_t *, vspace_cref,
		       unsigned long, vstart,
		       unsigned long, vend)
__SYSCALL_FAST_DEFINE4(clone_master_area, cref_t *, src_vspace_cref, cref_t *, dst_vspace_cref,
		       unsigned long, vstart, unsigned long, size)

__SYSCALL_FAST_DEFINE1(mprotect, const struct sysarg_vspace_mprotect *, args)
/* fast call used for default_ref only calller,
 * orders keep same in all related files with captype_thread.h */
__SYSCALL_FAST_DEFINE1(thread_vspace_cref, cref_t *, presult)
__SYSCALL_FAST_DEFINE1(thread_set_runqueue, unsigned long, newrq)
__SYSCALL_FAST_DEFINE1(thread_get_cpu, unsigned int *, pcpu)
__SYSCALL_FAST_DEFINE2(thread_suspend, const unsigned int *, uaddr, unsigned int, val)
__SYSCALL_FAST_DEFINE3(thread_stat, struct sysarg_thread_stat *, presult,
		       struct __arch_fpsimd_state *, fpsimd,
		       unsigned long, flags)
__SYSCALL_FAST_DEFINE2(thread_set_name, const char *, uin, char *, uout)
__SYSCALL_FAST_DEFINE0(thread_trigger_detach_one)

__SYSCALL_FAST_DEFINE1(thread_cref, cref_t *, presult)
__SYSCALL_FAST_DEFINE1(thread_effective_cref, cref_t *, presult)
__SYSCALL_FAST_DEFINE5(schedctrl, unsigned int, cmd, void *, args, unsigned long, args_size,
		       void *, ret, unsigned long, ret_size)
__SYSCALL_FAST_DEFINE2(thread_hmsignal_mask, const struct __hmsigmask *, uin,
		       struct __hmsigmask *, uout)
__SYSCALL_FAST_DEFINE1(thread_hmsignal_pending, struct __hmsigmask *, dst)
__SYSCALL_FAST_DEFINE1(thread_hmsignal_unset, struct __hmsigmask *, unset_hmsignals)

__SYSCALL_FAST_DEFINE5(hvgr_mmu_ctrl, unsigned int, cmd, void *, args, unsigned long, args_size,
                       void *, ret, unsigned long, ret_size)

__SYSCALL_FAST_DEFINE5(smmu_syscall, unsigned int, cmd, void *, args, unsigned long, args_size,
                       void *, ret, unsigned long, ret_size)

/*
 * __u64 here is common way in a syscall when reading data through a user pointer.
 * internally, miscdata read from kernel is `unsigned long`. In syscall boundary,
 * kernel converts it to __u64 to make sure arm32/64 see samething.
 */
__SYSCALL_FAST_DEFINE3(miscdata_read, __u64 *, dest, unsigned long, miscdata, unsigned long, access)
__SYSCALL_FAST_DEFINE3(miscdata_write, unsigned long, v, unsigned long, miscdata, unsigned long, access)
/*
 * In these vectorized accessing syscalls, miscdata are treated as memory, not
 * a group of values, so dest/data should be same format as them in kernel, and
 * format transformation will not be done. User should know this difference that,
 * for a same miscdata, reading it using miscdata_read and miscdata_readv gives
 * different result on a 32 bit system:
 * miscdata_read need 8 bytes to store a 32-bit unsigned long, miscdata_readv needs
 * only 4 bytes.
 *
 * for sz, nids * sizeof(unsigned long) is suggest when we knows that
 * app and kernel are in same data model.
 *
 * Avoid using it if user and kernel are possible to be in different data model.
 */
__SYSCALL_FAST_DEFINE4(miscdata_readv, void *, dest, unsigned long, sz,
		       unsigned long, miscdata, unsigned long, access)
__SYSCALL_FAST_DEFINE4(miscdata_writev, void *, data, unsigned long, sz,
		       unsigned long, miscdata, unsigned long, access)

__SYSCALL_FAST_DEFINE6(miscdata_readv_cas, void *, dest, unsigned long, sz,
		       unsigned long, miscdata, unsigned long, access,
		       unsigned long, expectval, unsigned long, newval)
__SYSCALL_FAST_DEFINE5(miscdata_writev_cas, void *, data, unsigned long, sz,
		       unsigned long, miscdata, unsigned long, access,
		       unsigned long, expectval)

__SYSCALL_FAST_DEFINE5(actv_load_stat, struct sysarg_actv_stat *, actv_out,
		       struct __arch_fpsimd_state *, fpsimd,
		       cref_t *, actv_chain_iter,
		       const cref_t *, p_thread_cref,
		       unsigned long, flags)
__SYSCALL_FAST_DEFINE1(cspace_free_cnode_idx, __u32, cnode_idx)

__SYSCALL_FAST_DEFINE1(set_tls, void *, address)

__SYSCALL_FAST_DEFINE2(actv_set_local_area, unsigned long, flags,
		       struct __actv_local_area_k *, area)

__SYSCALL_FAST_DEFINE1(actv_cref, cref_t *, p_cref)

__SYSCALL_FAST_DEFINE1(actv_cred, __u64 *, pv)

__SYSCALL_FAST_DEFINE1(thread_flush_caller,
		       const struct sysarg_execve_config *, execve_conf)

__SYSCALL_FAST_DEFINE1(thread_close_on_exec, int, server)

__SYSCALL_FAST_DEFINE2(membarrier, int, cmd, unsigned int, flags)

__SYSCALL_FAST_DEFINE3(ulwt_thread_monitor, unsigned long, op, unsigned long, arg1, unsigned long, arg2)

__SYSCALL_FAST_DEFINE4(epoll_wait, int, epfd, void *, events, int, maxevents, int, timeout)

__SYSCALL_FAST_DEFINE2(ctrlmem_write_arg0, void *, dest, unsigned long, val)

__SYSCALL_FAST_DEFINE2(__ctrlmem_write_r32_v32_arg0, void *, dest, unsigned long, v)
__SYSCALL_FAST_DEFINE3(__ctrlmem_write_r32_v64_arg0, void *, dest, unsigned long, v_lo, unsigned long, v_hi)
__SYSCALL_FAST_DEFINE2(__ctrlmem_write_r64_v32_arg0, void *, dest, unsigned long, v)
__SYSCALL_FAST_DEFINE2(__ctrlmem_write_r64_v64_arg0, void *, dest, unsigned long, v)

__SYSCALL_FAST_DEFINE3(__ctrlmem_write_r32_v32_arg1, void *, dest, unsigned long, v,
		       unsigned long, arg1)
__SYSCALL_FAST_DEFINE4(__ctrlmem_write_r32_v64_arg1, void *, dest, unsigned long, v_lo, unsigned long, v_hi,
		       unsigned long, arg1)
__SYSCALL_FAST_DEFINE3(__ctrlmem_write_r64_v32_arg1, void *, dest, unsigned long, v,
		       unsigned long, arg1)
__SYSCALL_FAST_DEFINE3(__ctrlmem_write_r64_v64_arg1, void *, dest, unsigned long, v,
		       unsigned long, arg1)

__SYSCALL_FAST_DEFINE4(__ctrlmem_write_r32_v32_arg2, void *, dest, unsigned long, v,
		       unsigned long, arg1, unsigned long, arg2)
__SYSCALL_FAST_DEFINE5(__ctrlmem_write_r32_v64_arg2, void *, dest, unsigned long, v_lo, unsigned long, v_hi,
		       unsigned long, arg1, unsigned long, arg2)
__SYSCALL_FAST_DEFINE4(__ctrlmem_write_r64_v32_arg2, void *, dest, unsigned long, v,
		       unsigned long, arg1, unsigned long, arg2)
__SYSCALL_FAST_DEFINE4(__ctrlmem_write_r64_v64_arg2, void *, dest, unsigned long, v,
		       unsigned long, arg1, unsigned long, arg2)

__SYSCALL_FAST_DEFINE5(__ctrlmem_write_r32_v32_arg3, void *, dest, unsigned long, v,
		       unsigned long, arg1, unsigned long, arg2, unsigned long, arg3)
__SYSCALL_FAST_DEFINE6(__ctrlmem_write_r32_v64_arg3, void *, dest, unsigned long, v_lo, unsigned long, v_hi,
		       unsigned long, arg1, unsigned long, arg2, unsigned long, arg3)
__SYSCALL_FAST_DEFINE5(__ctrlmem_write_r64_v32_arg3, void *, dest, unsigned long, v,
		       unsigned long, arg1, unsigned long, arg2, unsigned long, arg3)
__SYSCALL_FAST_DEFINE5(__ctrlmem_write_r64_v64_arg3, void *, dest, unsigned long, v,
		       unsigned long, arg1, unsigned long, arg2, unsigned long, arg3)

__SYSCALL_FAST_DEFINE6(__ctrlmem_write_r32_v32_arg4, void *, dest, unsigned long, v,
		       unsigned long, arg1, unsigned long, arg2, unsigned long, arg3,
		       unsigned long, arg4)
__SYSCALL_FAST_DEFINE7(__ctrlmem_write_r32_v64_arg4, void *, dest, unsigned long, v_lo, unsigned long, v_hi,
		       unsigned long, arg1, unsigned long, arg2, unsigned long, arg3,
		       unsigned long, arg4)
__SYSCALL_FAST_DEFINE6(__ctrlmem_write_r64_v32_arg4, void *, dest, unsigned long, v,
		       unsigned long, arg1, unsigned long, arg2, unsigned long, arg3,
		       unsigned long, arg4)
__SYSCALL_FAST_DEFINE6(__ctrlmem_write_r64_v64_arg4, void *, dest, unsigned long, v,
		       unsigned long, arg1, unsigned long, arg2, unsigned long, arg3,
		       unsigned long, arg4)

__SYSCALL_FAST_DEFINE7(__ctrlmem_write_r32_v32_arg5, void *, dest, unsigned long, v,
		       unsigned long, arg1, unsigned long, arg2, unsigned long, arg3,
		       unsigned long, arg4, unsigned long, arg5)
__SYSCALL_FAST_DEFINE8(__ctrlmem_write_r32_v64_arg5, void *, dest, unsigned long, v_lo, unsigned long, v_hi,
		       unsigned long, arg1, unsigned long, arg2, unsigned long, arg3,
		       unsigned long, arg4, unsigned long, arg5)
__SYSCALL_FAST_DEFINE7(__ctrlmem_write_r64_v32_arg5, void *, dest, unsigned long, v,
		       unsigned long, arg1, unsigned long, arg2, unsigned long, arg3,
		       unsigned long, arg4, unsigned long, arg5)
__SYSCALL_FAST_DEFINE7(__ctrlmem_write_r64_v64_arg5, void *, dest, unsigned long, v,
		       unsigned long, arg1, unsigned long, arg2, unsigned long, arg3,
		       unsigned long, arg4, unsigned long, arg5)

__SYSCALL_FAST_DEFINE3(__ctrlmem_cas_r32_v32_arg0, void *, dest, unsigned long, ori_v,
		       unsigned long, v)
__SYSCALL_FAST_DEFINE5(__ctrlmem_cas_r32_v64_arg0, void *, dest, unsigned long, ori_v_lo, unsigned long, ori_v_hi,
		       unsigned long, v_lo, unsigned long, v_hi)
__SYSCALL_FAST_DEFINE3(__ctrlmem_cas_r64_v32_arg0, void *, dest, unsigned long, ori_v,
		       unsigned long, v)
__SYSCALL_FAST_DEFINE3(__ctrlmem_cas_r64_v64_arg0, void *, dest, unsigned long, ori_v,
		       unsigned long, v)

__SYSCALL_FAST_DEFINE4(__ctrlmem_cas_r32_v32_arg1, void *, dest, unsigned long, ori_v,
		       unsigned long, v, unsigned long, arg1)
__SYSCALL_FAST_DEFINE6(__ctrlmem_cas_r32_v64_arg1, void *, dest, unsigned long, ori_v_lo, unsigned long, ori_v_hi,
		       unsigned long, v_lo, unsigned long, v_hi, unsigned long, arg1)
__SYSCALL_FAST_DEFINE4(__ctrlmem_cas_r64_v32_arg1, void *, dest, unsigned long, ori_v,
		       unsigned long, v,
		       unsigned long, arg1)
__SYSCALL_FAST_DEFINE4(__ctrlmem_cas_r64_v64_arg1, void *, dest, unsigned long, ori_v,
		       unsigned long, v,
		       unsigned long, arg1)

__SYSCALL_FAST_DEFINE5(__ctrlmem_cas_r32_v32_arg2, void *, dest, unsigned long, ori_v, unsigned long, v,
		       unsigned long, arg1, unsigned long, arg2)
__SYSCALL_FAST_DEFINE7(__ctrlmem_cas_r32_v64_arg2, void *, dest, unsigned long, ori_v_lo, unsigned long, ori_v_hi,
		       unsigned long, v_lo, unsigned long, v_hi,
		       unsigned long, arg1, unsigned long, arg2)
__SYSCALL_FAST_DEFINE5(__ctrlmem_cas_r64_v32_arg2, void *, dest, unsigned long, ori_v, unsigned long, v,
		       unsigned long, arg1, unsigned long, arg2)
__SYSCALL_FAST_DEFINE5(__ctrlmem_cas_r64_v64_arg2, void *, dest, unsigned long, ori_v, unsigned long, v,
		       unsigned long, arg1, unsigned long, arg2)

__SYSCALL_FAST_DEFINE6(__ctrlmem_cas_r32_v32_arg3, void *, dest, unsigned long, ori_v, unsigned long, v,
		       unsigned long, arg1, unsigned long, arg2, unsigned long, arg3)
__SYSCALL_FAST_DEFINE8(__ctrlmem_cas_r32_v64_arg3, void *, dest, unsigned long, ori_v_lo, unsigned long, ori_v_hi,
		       unsigned long, v_lo, unsigned long, v_hi,
		       unsigned long, arg1, unsigned long, arg2, unsigned long, arg3)
__SYSCALL_FAST_DEFINE6(__ctrlmem_cas_r64_v32_arg3, void *, dest, unsigned long, ori_v, unsigned long, v,
		       unsigned long, arg1, unsigned long, arg2, unsigned long, arg3)
__SYSCALL_FAST_DEFINE6(__ctrlmem_cas_r64_v64_arg3, void *, dest, unsigned long, ori_v, unsigned long, v,
		       unsigned long, arg1, unsigned long, arg2, unsigned long, arg3)

__SYSCALL_FAST_DEFINE7(__ctrlmem_cas_r32_v32_arg4, void *, dest, unsigned long, ori_v, unsigned long, v,
		       unsigned long, arg1, unsigned long, arg2, unsigned long, arg3,
		       unsigned long, arg4)
__SYSCALL_FAST_DEFINE9(__ctrlmem_cas_r32_v64_arg4, void *, dest, unsigned long, ori_v_lo, unsigned long, ori_v_hi,
		       unsigned long, v_lo, unsigned long, v_hi,
		       unsigned long, arg1, unsigned long, arg2, unsigned long, arg3, unsigned long, arg4)
__SYSCALL_FAST_DEFINE7(__ctrlmem_cas_r64_v32_arg4, void *, dest, unsigned long, ori_v,
		       unsigned long, v, unsigned long, arg1, unsigned long, arg2, unsigned long, arg3,
		       unsigned long, arg4)
__SYSCALL_FAST_DEFINE7(__ctrlmem_cas_r64_v64_arg4, void *, dest, unsigned long, ori_v, unsigned long, v,
		       unsigned long, arg1, unsigned long, arg2, unsigned long, arg3,
		       unsigned long, arg4)

__SYSCALL_FAST_DEFINE8(__ctrlmem_cas_r32_v32_arg5, void *, dest, unsigned long, ori_v, unsigned long, v,
		       unsigned long, arg1, unsigned long, arg2, unsigned long, arg3,
		       unsigned long, arg4, unsigned long, arg5)
__SYSCALL_FAST_DEFINE10(__ctrlmem_cas_r32_v64_arg5, void *, dest, unsigned long, ori_v_lo, unsigned long, ori_v_hi,
			unsigned long, v_lo, unsigned long, v_hi,
			unsigned long, arg1, unsigned long, arg2, unsigned long, arg3,
			unsigned long, arg4, unsigned long, arg5)
__SYSCALL_FAST_DEFINE8(__ctrlmem_cas_r64_v32_arg5, void *, dest, unsigned long, ori_v, unsigned long, v,
		       unsigned long, arg1, unsigned long, arg2, unsigned long, arg3,
		       unsigned long, arg4, unsigned long, arg5)
__SYSCALL_FAST_DEFINE8(__ctrlmem_cas_r64_v64_arg5, void *, dest, unsigned long, ori_v, unsigned long, v,
		       unsigned long, arg1, unsigned long, arg2, unsigned long, arg3,
		       unsigned long, arg4, unsigned long, arg5)

#if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
__SYSCALL_FAST_DEFINE4(__cnode_stat_rref32, struct sysarg_cnode_statrref *, presult,
		       unsigned long, v_hi, unsigned long, v_lo, unsigned long, flags)
#else
__SYSCALL_FAST_DEFINE4(__cnode_stat_rref32, struct sysarg_cnode_statrref *, presult,
		       unsigned long, v_lo, unsigned long, v_hi, unsigned long, flags)
#endif
__SYSCALL_FAST_DEFINE3(__cnode_stat_rref64, struct sysarg_cnode_statrref *, presult,
		       unsigned long, v, unsigned long, flags)

__SYSCALL_FAST_DEFINE0(restart_syscall)
__SYSCALL_FAST_DEFINE0(actv_fpu_begin)
__SYSCALL_FAST_DEFINE0(actv_fpu_end)
__SYSCALL_FAST_DEFINE2(pm_idle_enter, unsigned int, index, void *, sys_stat)

/* survive the stupid code checker */
#undef ULIBS_INCLUDE_HONGMENG_FAST_SYSCALL_LIST_H
#endif
