/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Thread related implementation for vsyscall
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 24 15:03:55 2019
 */

#include <sys/prctl.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsrv_sys/hm_signal.h>
#include <libhmsrv_sys/hm_module.h>
#include <libhwsecurec/securec.h>
#include <libsysif/sysmgr/api.h>
#include <hongmeng/syscall.h>
#include <hongmeng/errno.h>
#include <hmkernel/errno.h>
#include <hmkernel/sched/sched.h>
#include <libhmlog/hmlog.h>
#include <libsched/priority.h>
#include <hmkernel/sched_module.h>
#include <libhmsync/thread.h>
#include <hmkernel/sched/priority.h>
#include <hmasm/lsyscall.h>
#include <sched.h>

int hm_thread_kvic_source_bind(cref_t tcb_cref, unsigned int source_id,
			       unsigned int flags)
{
	int err = E_HM_OK;

	err = syscap_ThreadKvicSourceBind(tcb_cref, source_id, flags);
	if (err < 0) {
		hm_debug("KvicSourceBind failed, cref: %"PRIxref
			 "source id: %u, flags: %u, err=%s\n",
			 tcb_cref, source_id, flags, hmstrerror(err));
	}

	return err;
}

int hm_thread_kvic_source_unbind(cref_t tcb_cref, unsigned int source_id)
{
	int err = E_HM_OK;

	err = syscap_ThreadKvicSourceUnbind(tcb_cref, source_id);
	if (err < 0) {
		hm_debug("KvicSourceUnbind failed, cref: %"PRIxref
			 " source id: %u, err=%s\n",
			 tcb_cref, source_id, hmstrerror(err));
	}

	return err;
}

int hm_thread_kvic_kev_source_bind(cref_t tcb_cref, unsigned int source_id,
				   unsigned int flags)
{
	int err = E_HM_OK;

	err = syscap_ThreadKvicKevSourceBind(tcb_cref, source_id, flags);
	if (err < 0) {
		hm_debug("KvicKevSourceBind failed, cref: %"PRIxref
			 "source id: %u, flags: %u, err=%s\n",
			 tcb_cref, source_id, flags, hmstrerror(err));
	}

	return err;
}

int hm_thread_kvic_kev_source_unbind(cref_t tcb_cref, unsigned int source_id)
{
	int err = E_HM_OK;

	err = syscap_ThreadKvicKevSourceUnbind(tcb_cref, source_id);
	if (err < 0) {
		hm_debug("KvicKevSourceUnbind failed, cref: %"PRIxref
			 " source id: %u, err=%s\n",
			 tcb_cref, source_id, hmstrerror(err));
	}

	return err;
}

int hm_thread_setname(cref_t thread_cref, const char *new_name, char *old_name)
{
	int err = E_HM_OK;
	struct threadmgr_thread_name name_buf;

	mem_zero_s(name_buf);
	if (old_name != NULL) {
		err = syscap_ThreadSetName(thread_cref, NULL, &name_buf.name[0]);
		if (err != E_HM_OK) {
			hm_error("syscap_ThreadSetName failed, err=%s\n", hmstrerror(err));
		}
		(void)safe_copy(old_name, &name_buf.name[0], HMCAP_THREAD_NAME_SIZE);
		mem_zero_s(name_buf);
	}

	if (err == E_HM_OK && new_name != NULL) {
		err = safe_copy(&name_buf.name[0], new_name, HMCAP_THREAD_NAME_SIZE - 1);
		if (err < 0) {
			err = E_HM_POSIX_FAULT;
		} else {
			err = actvcall_hmcall_thread_setname(thread_cref, name_buf);
			if (err != E_HM_OK) {
				hm_error("actvcall_hmcall_thread_setname failed, err=%s\n", hmstrerror(err));
			}
		}
	}

	return err;
}

int hm_thread_set_pdeath_signal(int signum)
{
	return lsyscall_proc_prctl(PR_SET_PDEATHSIG, (unsigned long)signum, 0, 0, 0);
}

int hm_thread_get_pdeath_signal(int *signum)
{
	return lsyscall_proc_prctl(PR_GET_PDEATHSIG, ptr_to_ulong(signum), 0, 0, 0);
}

cref_t hm_thread_self_cref(void)
{
	return hm_actv_local_sched_thread_cref_of();
}

cref_t hm_thread_effective_cref(void)
{
	return hm_actv_local_thread_cref_of();
}

cref_t hm_thread_vspace_cref(cref_t thread_cref)
{
	int err;
	cref_t vspace_cref = 0ULL;
	if (thread_cref == 0ULL) {
		err = sysfast_thread_vspace_cref(&vspace_cref);
	} else {
		err = syscap_ThreadVSpaceOf(thread_cref, &vspace_cref);
	}
	if (err < 0 || IS_REF_ERR(vspace_cref)) {
		vspace_cref = ERR_TO_REF(E_HM_INVAL);
	}

	return vspace_cref;
}

cref_t hm_thread_tid2cref(int tid)
{
	cref_t ret;
	if (tid < 0) {
		ret = ERR_TO_REF(E_HM_INVAL);
	} else {
		ret = thread_tid2cref(tid);
	}
	return ret;
}

pid_t hm_thread_cref2tid(cref_t cref)
{
	int err;
	int tid;
	struct __actvret_hmcall_thread_cref2tid call_ret;

	mem_zero_s(call_ret);
	err = actvxactcall_hmcall_thread_cref2tid(true, false, cref, &call_ret);
	if (err != E_HM_OK) {
		tid = err;
	} else {
		tid = call_ret.tid;
	}
	return tid;
}


/*
 * hm_thread_nstid_of_root - return all tids of the thread under different pidns
 * (Only can be used in root pid ns)
 *
 * @root_tid: root_tid of thread
 * @tid_array: An array that holds all tids
 * @len: array len
 *
 * Return 0 when getting all tids successfully
 * Return E_HM_xx when an error occurred
 */
int hm_thread_nstid_of_root(int root_tid, int *tid_array, int len)
{
	return actvcall_hmcall_thread_nstid_of_root(root_tid, tid_array, len);
}

int hm_thread_gettid(void)
{
	return (int)lsyscall_thread_gettid();
}

int hm_thread_main_thread_name_of_cnode_idx(uint32_t cnode_idx, char *name, uint32_t len)
{
	int err;
	struct threadmgr_thread_name name_buf;

	err = actvcall_hmcall_thread_main_thread_name_of_cnode_idx(cnode_idx, &name_buf);
	if (err == E_HM_OK) {
		err = safe_copy(name, name_buf.name, len);
		name[len - 1] = '\0';
	}

	return err;
}

int hm_thread_get_name_by_root_ns_tid(int root_ns_tid, char *name, uint32_t len)
{
	int err;
	struct threadmgr_thread_name name_buf = {0};

	err = actvcall_hmcall_thread_get_name_by_root_ns_tid(root_ns_tid, &name_buf);
	if (err == E_HM_OK) {
		err = strcpy_s(name, len, name_buf.name);
		if (err == EOK && len > 0) {
			name[len - 1] = '\0';
			err = E_HM_OK;
		} else {
			err = E_HM_FAULT;
		}
	}

	return err;
}

int __hm_thread_stat(cref_t tcb_cref, struct sysarg_thread_stat *stat,
		     struct __arch_fpsimd_state *fpsimd, unsigned long flag)
{
	int err = E_HM_OK;

	if (stat == NULL) {
		hm_error("hm_thread_stat failed, err=%s\n", hmstrerror(E_HM_INVAL));
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		if (tcb_cref == 0ULL) {
			err = sysfast_thread_stat(stat, fpsimd, flag);
		} else {
			err = syscap_ThreadStat(tcb_cref, stat, fpsimd, flag);
		}
		if (err < 0) {
			hm_error("hm_thread_stat failed, err=%s\n", hmstrerror(err));
		} else {
			err = E_HM_OK;
		}
	}

	return err;
}

int hm_thread_stat(cref_t tcb_cref, struct sysarg_thread_stat *stat, unsigned long flag)
{
	return __hm_thread_stat(tcb_cref, stat, NULL, flag);
}

int hm_thread_kexec_load_files(struct softreboot_info files)
{
	return actvcall_hmcall_thread_kexec_load_files(files);
}

int hm_thread_set_tickless(int tid, bool tickless)
{
	return actvcall_hmcall_thread_set_tickless(tid, tickless);
}

int hm_thread_sched_setparam(int tid, struct sched_param *param_user)
{
	return lsyscall_thread_sched_setparam(tid, param_user);
}

int hm_thread_sched_getparam(int tid, struct sched_param *param_user)
{
	return lsyscall_thread_sched_getparam(tid, param_user);
}

int hm_thread_setpriority(int which, int tid, int nice)
{
	return lsyscall_thread_setpriority(which, tid, nice);
}

int hm_thread_sched_setscheduler(int tid, int policy, struct sched_param *param_user)
{
	return lsyscall_thread_sched_setscheduler(tid, policy, param_user);
}

int hm_thread_sched_getscheduler(int tid)
{
	return lsyscall_thread_sched_getscheduler(tid);
}

int hm_thread_sched_setattr(int tid, struct sched_attr *uattr, unsigned int flags)
{
	return lsyscall_thread_sched_setattr(tid, uattr, flags);
}

int hm_thread_sched_getattr(int tid, struct sched_attr *uattr,
			    unsigned int size, unsigned int flags)
{
	return lsyscall_thread_sched_getattr(tid, uattr, size, flags);
}

int hm_thread_sched_setattr_native(int tid, struct sched_attr *uattr, unsigned int flags)
{
	return actvxactcall_hmcall_thread_sched_setattr_native(true, false, tid, *uattr, flags);
}

int hm_thread_sched_getattr_native(int tid, struct sched_attr *uattr,
				   unsigned int size, unsigned int flags)
{
	struct __actvret_hmcall_thread_sched_getattr_native attr_ret;
	int err = E_HM_INVAL;

	mem_zero_s(attr_ret);
	if (uattr != NULL) {
		err = actvxactcall_hmcall_thread_sched_getattr_native(true, false, tid, size, flags, &attr_ret);
		if (err == E_HM_OK) {
			*uattr = attr_ret.attr;
		}
	}

	return err;
}

int hm_thread_sched_getdefaultscheduler(void)
{
	int default_scheduler;
#ifdef CONFIG_ICES_SCHEDULER
	default_scheduler = SCHED_OTHER;
#else
	default_scheduler = SCHED_RR;
#endif
	return default_scheduler;
}

int hm_thread_sched_getdefaultpriority(void)
{
	int default_priority;
#ifdef CONFIG_ICES_SCHEDULER
	default_priority = 0;
#else
	default_priority = HM_PRIORITY_USER_DEFAULT;
#endif
	return default_priority;
}

int hm_thread_set_runqueue(int tid, unsigned int newrq)
{
	return actvcall_hmcall_thread_set_runqueue(tid, newrq);
}

int hm_thread_yield(void)
{
	return sysfast_sched_yield();
}

int hm_thread_suspend(void)
{
	return sysfast_thread_suspend(NULL, 0);
}

int hm_thread_suspend_ex(cref_t cref)
{
	return syscap_ThreadSuspend(cref, NULL, 0);
}

int hm_thread_resume(cref_t cref)
{
	return syscap_ThreadResume(cref);
}

int hm_thread_freeze(int tid, int flag)
{
	return actvcall_hmcall_thread_freeze(tid, flag);
}

int hm_thread_set_freezable(void)
{
	return actvcall_hmcall_proc_pm_set_freezable();
}

bool hm_thread_freezing(void)
{
	int err;

	err = actvcall_hmcall_proc_pm_freezing();

	return (err == HM_THREAD_NEED_FREEZING);
}

int hm_thread_try_to_freeze(void)
{
	return actvcall_hmcall_proc_pm_try_to_freeze();
}

int hm_thread_thaw(int tid)
{
	return actvcall_hmcall_thread_thaw(tid);
}

int hm_thread_get_seq(int tid, uint64_t *seq)
{
	int err;
	struct __actvret_hmcall_thread_getseq ret;

	mem_zero_s(ret);
	err = actvcall_hmcall_thread_getseq(tid, &ret);
	if (err < 0) {
		hm_debug("hm thread get seq failed, err=%s\n",
			 hmstrerror(err));
		/* Currently pid seq is using main thread seq (tid == pid).
		 * ZOMBIE process cleaned threads list, for it return 0
		 * to avoid being treated as NONExist.
		 */
		if (err == E_HM_SRCH) {
			*seq = 0;
			err = E_HM_OK;
		}
	} else {
		*seq = ret.seq;
		err = E_HM_OK;
	}
	return err;
}

int hm_thread_sched_setaffinity(int tid, size_t size, void *cpuset)
{
	return lsyscall_thread_sched_setaffinity(tid, size, cpuset);
}

int hm_thread_sched_getaffinity(int tid, size_t size, void *cpuset)
{
	return lsyscall_thread_sched_getaffinity(tid, size, cpuset);
}

int hm_thread_sched_setaffinity_direct(int tid, size_t size, void *cpuset)
{
	return actvcall_hmcall_thread_sched_setaffinity_direct(tid, size, cpuset);
}

int hm_thread_sched_getaffinity_direct(int tid, size_t size, void *cpuset)
{
	return actvcall_hmcall_thread_sched_getaffinity_direct(tid, size, cpuset);
}

int hm_thread_sched_trans(cref_t src_cref, cref_t dst_cref, bool restore)
{
	return actvxactcall_hmcall_thread_sched_trans(true, false, src_cref, dst_cref, restore);
}

int hm_thread_attach_cpu_context(cref_t thread, cref_t context,
				 enum __tcb_attach_type type)
{
	return syscap_ThreadAttachCpuContext(thread, context, (unsigned int)type);
}

int hm_thread_detach_cpu_context(cref_t thread, cref_t *pcontext,
				 enum __tcb_attach_type type)
{
	return syscap_ThreadDetachCpuContext(thread, pcontext, (unsigned int) type);
}

int hm_thread_get_tokens(cref_t cref, __u64 *token, __u64 *ftoken)
{
	// 2 is to token and ftoken
	unsigned long long tokens[2];
	int ret = actvcall_hmcall_thread_get_token(cref, tokens);
	if (ret != E_HM_OK) {
		return ret;
	}

	*token = tokens[0];
	*ftoken = tokens[1];
	NOFAIL(memset_s(tokens, sizeof(tokens), 0, sizeof(tokens)));
	return ret;
}

int hm_thread_bind_cpu(void)
{
	return syscap_ModuleIoctl(thread_module_sched_ref(), __SCHED_BIND_CPU,
				  NULL, 0, NULL, 0);
}

int hm_thread_unbind_cpu(void)
{
	return syscap_ModuleIoctl(thread_module_sched_ref(), __SCHED_UNBIND_CPU,
				  NULL, 0, NULL, 0);
}

int hm_thread_set_pin_preempted(void)
{
	return syscap_ModuleIoctl(thread_module_sched_ref(), __SCHED_PIN_PREEMPTED_TCB,
				  NULL, 0, NULL, 0);
}

#ifdef CONFIG_SECCOMP
void hm_thread_set_kill_signal(int tid)
{
	actvcall_hmcall_thread_set_exitsignal(tid);
}
#endif
