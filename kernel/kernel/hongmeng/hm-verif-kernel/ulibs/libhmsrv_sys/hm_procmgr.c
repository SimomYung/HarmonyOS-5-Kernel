/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Native interfaces of process-related operation
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 03 10:59:31 2018
 */
#include <sys/wait.h>
#include <sys/prctl.h>

#include <hmsysmgr/nsmgr/nsid.h>
#include <libhmlog/hmlog_ratelimit.h>
#include <libhmsync/bitops/generic.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <libhmsrv_sys/hm_signal.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_init.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmucap/ucap.h>
#include <libhwsecurec/securec.h>
#include <libmem/types.h>
#include <hmsysmgr/mgrs_stat.h>
#include <hmsysmgr/process/pid.h>
#include <libhmsync/raw_mutex.h>
#include <libsysif/sysmgr/api.h>
#include <libbunch/bunch_ipc.h>
#include <hmkernel/barrier.h>
#include <libhmsync/thread.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <fs_server.h>
#include <fd.h>
#include <libstrict/strict.h>
#include <sched.h>

#ifndef __HOST_UT__
#include <clone.h>
#endif

#define MAX_ARG_STRINGS 0x7FFFFFFF

static int __hm_spawn_ex(const struct hm_spawn_attr *attr,
			 struct hm_spawn_req *spawn_req,
			 struct __actvret_hmcall_proc_spawn *ret)
{
	unsigned int attrp_flags = 0;

	if (attr->attrp != NULL) {
		attrp_flags = (unsigned int)attr->attrp->__flags;
	}

	if ((attrp_flags & (unsigned int)POSIX_SPAWN_SETPGROUP) != 0u) {
		spawn_req->flags |= SPAWN_CONFIG_FLAGS_MASK_SETPGROUP;
	}

	mem_zero_s(*ret);

	return actvxactcall_hmcall_proc_spawn(false, true, *spawn_req, ret);
}

int hm_spawn_ex(const struct hm_spawn_attr *attr,
		struct __actvret_hmcall_proc_spawn *pret)
{
	int rc = E_HM_OK;
	struct hm_spawn_req spawn_req;
	struct __actvret_hmcall_proc_spawn ret;

	if (attr == NULL) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		mem_zero_s(spawn_req);
		spawn_req.flags = attr->flags;
		spawn_req.exec_req.layout_end = attr->layout_end;
		spawn_req.exec_req.path = ptr_to_u64(attr->path);
		spawn_req.exec_req.argv = ptr_to_u64(attr->argv);
		spawn_req.exec_req.envp = ptr_to_u64(attr->envp);
		spawn_req.process_affinity.affinity = attr->process_affinity.affinity;
		spawn_req.process_affinity.default_affinity = attr->process_affinity.default_affinity;
		spawn_req.process_affinity.is_privileged_affinity = attr->process_affinity.is_privileged_affinity;

		rc = __hm_spawn_ex(attr, &spawn_req, &ret);
		if (rc == E_HM_OK && pret != NULL) {
			*pret = ret;
		}
	}

	return rc;
}

int hm_spawn(pid_t *pid, const char *path,
	     const posix_spawn_file_actions_t *file_action,
	     const posix_spawnattr_t *attrp,
	     const char *const argv[], const char *const envp[])
{
	int rc;
	struct __actvret_hmcall_proc_spawn ret;
	struct hm_spawn_attr attr = {
		.path = path,
		.argv = argv,
		.envp = envp,
		.file_action = file_action,
		.attrp = attrp,
		.flags = 0U,
		.layout_end = 0ULL,
		.process_affinity = {
			.affinity = __cpuset_empty,
			.default_affinity = __cpuset_empty,
			.is_privileged_affinity = false,
		},
	};

	mem_zero_s(ret);
	rc = hm_spawn_ex(&attr, &ret);
	if (rc == E_HM_OK && pid != NULL) {
		(void)safe_copy(pid, &ret.pid, sizeof(pid_t));
	}
	return rc;
}

void hm_spawn_attr_init(struct hm_spawn_attr *attr)
{
	attr->path = NULL;
	attr->argv = NULL;
	attr->envp = NULL;
	attr->file_action = NULL;
	attr->attrp = NULL;
	attr->flags = 0U;
	attr->layout_end = 0ULL;
	attr->process_affinity.affinity = __cpuset_full;
	attr->process_affinity.default_affinity = __cpuset_full;
	attr->process_affinity.is_privileged_affinity = false;
}

void hm_spawn_set_affinity(struct hm_spawn_attr *attr,
			   const struct process_affinity *process_affinity,
			   unsigned int flags)
{
	if ((flags & SPAWN_CONFIG_FLAGS_MASK_SETAFFINITY) != 0U) {
		attr->flags |= SPAWN_CONFIG_FLAGS_MASK_SETAFFINITY;
		attr->process_affinity.affinity = process_affinity->affinity;
	}

	if ((flags & SPAWN_CONFIG_FLAGS_MASK_SETDEFAULTAFFINITY) != 0U) {
		attr->flags |= SPAWN_CONFIG_FLAGS_MASK_SETDEFAULTAFFINITY;
		attr->process_affinity.default_affinity = process_affinity->default_affinity;
	}
	attr->process_affinity.is_privileged_affinity = process_affinity->is_privileged_affinity;
}

void hm_spawn_set_suspend_state(struct hm_spawn_attr *attr)
{
	attr->flags |= SPAWN_CONFIG_FLAGS_MASK_SUSPEND;
}

int hm_spawn_with_attr(pid_t *pid, const char *path,
		       const posix_spawn_file_actions_t *file_action,
		       const posix_spawnattr_t *attrp,
		       const char *const argv[], const char *const envp[],
		       struct hm_spawn_attr *attr)
{
	int rc;
	struct __actvret_hmcall_proc_spawn ret;

	attr->path = path;
	attr->argv = argv;
	attr->envp = envp;
	attr->file_action = file_action;
	attr->attrp = attrp;
	mem_zero_s(ret);
	rc = hm_spawn_ex(attr, &ret);
	if (rc == E_HM_OK && pid != NULL) {
		(void)safe_copy(pid, &ret.pid, sizeof(pid_t));
	}
	return rc;
}

int hm_prlimit_by_pid(pid_t pid, unsigned int type,
		      const struct rlimit *new_rlim,
		      struct rlimit *old_rlim)
{
	return lsyscall_proc_prlimit(pid, (int)type, new_rlim, old_rlim);
}

int hm_rlimit_by_cnode_idx(uint32_t cnode_idx, unsigned int type,
			   const struct rlimit *new_rlim,
			   struct rlimit *old_rlim)
{
	int err;
	struct __actvret_hmcall_proc_rlimit_of ret;

	/* currently only vfs use this api with new_rlim set to NULL, just ignore it
	   and remove the param will remove later */
	UNUSED(new_rlim);
	err = actvcall_hmcall_proc_rlimit_of(cnode_idx, type, &ret);
	if (err == E_HM_OK) {
		old_rlim->rlim_cur = ret.rlim_cur;
		old_rlim->rlim_max = ret.rlim_max;
	}
	return err;
}

int hm_waitpid(pid_t pid, unsigned int options, int *wstatus)
{
	return lsyscall_proc_wait4(pid, wstatus, (int)options, NULL);
}

int hm_exit(int wstatus)
{
	return lsyscall_proc_exit((int)wstatus);
}

pid_t hm_getpid(void)
{
	return (pid_t)lsyscall_proc_getpid();
}

uid_t hm_getuid(void)
{
	return (uid_t)lsyscall_proc_getuid();
}

int hm_setresuid(uid_t ruid, uid_t euid, uid_t suid)
{
#ifdef __NR_setresuid32
	return lsyscall_proc_setresuid32(ruid, euid, suid);
#else
	return lsyscall_proc_setresuid(ruid, euid, suid);
#endif
}

int hm_setresgid(gid_t rgid, gid_t egid, gid_t sgid)
{
#ifdef __NR_setresgid32
	return lsyscall_proc_setresgid32(rgid, egid, sgid);
#else
	return lsyscall_proc_setresgid(rgid, egid, sgid);
#endif
}

int hm_setgroups(uint32_t data_size, const gid_t *groups)
{
#ifdef __NR_setgroups32
	return lsyscall_proc_setgroups32(data_size, groups);
#else
	return lsyscall_proc_setgroups(data_size, groups);
#endif
}

pid_t hm_pid_of_cnode_idx_in_ns(uint32_t cnode_idx, unsigned int nsid)
{
	return actvcall_hmcall_proc_pid_of(cnode_idx, nsid);
}

pid_t hm_pid_of_cnode_idx(uint32_t cnode_idx)
{
	return actvcall_hmcall_proc_pid_of(cnode_idx, CALLER_PIDNSID);
}

pid_t hm_getppid(void)
{
	return lsyscall_proc_getppid();
}

int hm_get_cnode_idx(pid_t pid, uint32_t *cnode_idx)
{
	int err;
	struct __actvret_hmcall_proc_cnode_idx_of ret;

	mem_zero_s(ret);
	err = actvcall_hmcall_proc_cnode_idx_of(pid, CALLER_PIDNSID, &ret);
	if (err == 0 && cnode_idx != NULL) {
		*cnode_idx = ret.cnode_idx;
	}

	return err;
}

int hm_get_cnode_idx_in_pidns(pid_t pid, uint32_t nsid, uint32_t *cnode_idx)
{
	int err;
	struct __actvret_hmcall_proc_cnode_idx_of ret;

	mem_zero_s(ret);
	err = actvxactcall_hmcall_proc_cnode_idx_of(true, false, pid, nsid, &ret);
	if (err == 0 && cnode_idx != NULL) {
		*cnode_idx = ret.cnode_idx;
	}

	return err;
}

int hm_get_cnode_idx_in_caller_pidns(pid_t pid, uint32_t *cnode_idx)
{
	int err;
	struct __actvret_hmcall_proc_cnode_idx_of ret;

	mem_zero_s(ret);
	err = actvxactcall_hmcall_proc_cnode_idx_of(true, false, pid, CALLER_PIDNSID, &ret);
	if (err == 0 && cnode_idx != NULL) {
		*cnode_idx = ret.cnode_idx;
	}

	return err;
}

struct proc_stat_config {
	rref_t server;
	pid_t pid;
	unsigned int flags;
};

static int proc_stat_ipc(struct bunch_ipc_attr *attr, void *data)
{
	struct proc_stat_config *config = NULL;

	config = (struct proc_stat_config *)data;
	return actvcall_hmcall_proc_stat(config->pid, config->flags, *attr);
}

static int server_proc_stat_ipc(struct bunch_ipc_attr *attr, void *data)
{
	struct proc_stat_config *config = NULL;

	config = (struct proc_stat_config *)data;
	return actvcapcall_hmcall_proc_stat(config->server, config->pid, config->flags, *attr);
}

static int proc_stat_common(rref_t server, pid_t pid,
			    unsigned int flags, struct bunch *bunch)
{
	int err = E_HM_OK;
	struct proc_stat_config config;
	struct bunch_ipc_reader_ctx ctx;

	if (pid < PID_NONE || pid == PID_RESERVED || bunch == NULL) {
		hm_debug("invalid pid=%d, or invalid bunch=0x%lx\n", pid, vaddr_of(bunch));
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		mem_zero_s(config);
		config.pid = pid;
		config.flags = flags;
		config.server = server;
		ctx.target_info = (uint64_t)hmsrv_sysmgr_cnode_idx();
		ctx.private_data = ptr_to_void(&config);
		if (server == (rref_t)0) {
			ctx.ipc = proc_stat_ipc;
		} else {
			ctx.ipc = server_proc_stat_ipc;
		}
	}

	if (err == E_HM_OK) {
		err = bunch_ipc_request(bunch, &ctx);
		if (err < 0) {
			hm_info("bunch ipc try request data with shm: %s\n",
				hmstrerror(err));
		}
	}

	return err;
}

int hm_proc_stat(pid_t pid, unsigned int flags, struct bunch *bunch)
{
	int err = E_HM_OK;

	err = proc_stat_common(0, pid, flags, bunch);
	if (err != E_HM_OK) {
		hm_debug("proc stat failed, err=%s\n", hmstrerror(err));
	}
	return err;
}

int hm_dubai_binder_stats_request(void *stats, int *count, int max_count)
{
	return actvcall_hmcall_proc_binder_stats_request(stats, count, max_count);
}

int hm_dubai_cpustat_request(void *stat, int *count, size_t size, int type)
{
	return actvcall_hmcall_proc_dubai_cpu_stat(stat, count, size, type);
}

int hm_abnormal_task_load_stat_request(void *task_load_stat, size_t stat_size,
				       void *load_chk_args, size_t args_size)
{
	return actvcall_hmcall_proc_abnormal_task_load_stat_request(task_load_stat, stat_size,
								    load_chk_args, args_size);
}

int hm_dubai_proc_cmdline_request(uid_t uid, pid_t pid, char *cmdline, size_t size, char *comm, size_t comm_size)
{
	return actvcall_hmcall_proc_dubai_proc_cmdline(uid, pid, cmdline, size, comm, comm_size);
}

int hm_dubai_pids_of_uid_request(struct uid_info *info, size_t uid_info_size,
				 struct uid_pid_set *pid_set, size_t pid_set_size)
{
	return actvcall_hmcall_proc_dubai_pids_of_uid(info, (void *)uid_info_size, (void *)pid_set, pid_set_size);
}

int hm_dubai_decomposed_procs_update(struct decomposed_proc_info *proc_info, int count)
{
	return actvcall_hmcall_proc_dubai_decomposed_procs_update((void *)proc_info, count);
}

int hm_server_proc_stat(rref_t server, pid_t pid, unsigned int flags,
			struct bunch *bunch)
{
	int err = E_HM_OK;

	if (IS_REF_VALID(server) == false) {
		hm_debug("invalid rref=%llx\n", server);
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		err = proc_stat_common(server, pid, flags, bunch);
		if (err != E_HM_OK) {
			hm_debug("server proc stat failed, err=%s\n", hmstrerror(err));
		}
	}

	return err;
}

pid_t hm_getpgid(pid_t pid)
{
	return lsyscall_proc_getpgid(pid);
}

/*
 * Suppose procss (pid=CPID) calls setpgid(pid, pgid).
 *
 * input arguments analyze as below:
 *   pgid     | pid<0   | pid=0               | pid>0
 * ------------------------------------------------------------
 *    <0      |            EINVAL
 * ------------------------------------------------------------
 *    =0      | EINVAL  | pid=CPID, pgid=CPID | pid, pgid=pid
 * ------------------------------------------------------------
 *    >0      | ESRCH   | pid=CPID, pgid      | pid, pgid
 *
 * Current support errors:
 *   1) EINVAL: a. pgid < 0, b. pid < 0 && pgid == 0;
 *   2) ESRCH: a. pid does not match any process.
 *             b. pid is not the calling process and not a child of the calling
 *                process.
 *   3) EPERM: a. process whose pid equal to pgid is not a group leader.
 *             b. process indicated by pgid does not exist.
 *
 * NOTE:
 *   Errors of EACCES and EPERM related to session currently are not supported.
 */
pid_t hm_getsid(pid_t pid)
{
	int err = E_HM_SRCH;

	if (pid >= 0) {
		err = lsyscall_proc_getsid(pid);
	}
	return err;
}

int hm_setfgpgrp(pid_t sid, pid_t pgid)
{
	return actvxactcall_hmcall_proc_setfgpgrp(true, false, sid, pgid);
}

pid_t hm_getfgpgrp(pid_t sid)
{
	return actvxactcall_hmcall_proc_getfgpgrp(true, false, sid);
}

pid_t hm_mysid(void)
{
	int rc = E_HM_OK;
	struct __actvret_hmcall_proc_mysid ret;

	rc = actvxactcall_hmcall_proc_mysid(true, false, &ret);
	if (rc == E_HM_OK) {
		rc = ret.sid;
	}

	return rc;
}

int hm_get_sid_pgid(pid_t *sid, pid_t *pgid)
{
	int rc = E_HM_OK;
	struct __actvret_hmcall_proc_mysid ret;

	if (sid == NULL || pgid == NULL) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		rc = actvxactcall_hmcall_proc_mysid(true, false, &ret);
		if (rc == E_HM_OK) {
			*sid = ret.sid;
			*pgid = ret.pgid;
		}
	}

	return rc;
}

int hm_signal_fgpgrp(pid_t sid, int signum)
{
	return actvcall_hmcall_proc_signal_fgpgrp(sid, signum);
}

int hm_signal_check_ignore(pid_t pid, int signum)
{
	return actvcall_hmcall_proc_signal_check_ignore(pid, signum);
}

int hm_session_set_ttyinfo(pid_t sid, const struct ttyinfo_s *info)
{
	struct ttyinfo_s ttyinfo;
	int rc = E_HM_OK;

	if (info != NULL) {
		if (safe_copy(&ttyinfo, info, sizeof(struct ttyinfo_s)) < 0) {
			rc = E_HM_POSIX_FAULT;
		}
	}
	if (rc == E_HM_OK) {
		rc = actvcall_hmcall_proc_session_set_ttyinfo(sid, ttyinfo);
	}
	return rc;
}

int hm_session_get_ttyinfo(struct ttyinfo_s *info, bool fwd)
{
	int rc = E_HM_OK;
	struct __actvret_hmcall_proc_session_get_ttyinfo ret;

	if (info == NULL) {
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		mem_zero_s(ret);
		rc = actvxactcall_hmcall_proc_session_get_ttyinfo(fwd, true, &ret);
	}

	if (rc == E_HM_OK) {
		if (safe_copy(info, &ret.info, sizeof(*info)) < 0) {
			rc = E_HM_POSIX_FAULT;
		}
	}

	return rc;
}

int hm_set_coredump(pid_t pid, enum coredump_request request,
		    union coredump_attr value)
{
	int err = E_HM_OK;

	if (pid < PID_NONE || pid == PID_RESERVED) {
		hm_error("invalid pid=%d\n", pid);
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		err = actvcall_hmcall_proc_set_coredump(pid, value, request);
	}

	return err;
}

int hm_get_coredump(pid_t pid, enum coredump_request request,
		    union coredump_attr *result)
{
	int err = E_HM_OK;
	struct __actvret_hmcall_proc_get_coredump ret;

	if (pid < PID_NONE || pid == PID_RESERVED) {
		hm_error("invalid pid=%d\n", pid);
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		if (result == NULL) {
			hm_error("invalid result pointer\n");
			err = E_HM_INVAL;
		}
	}
	if (err == E_HM_OK) {
		if (request == COREDUMP_INVAL || request >= COREDUMP_MAX) {
			hm_error("invalid request=%u\n", (unsigned int)request);
			err = E_HM_INVAL;
		}
	}
	if (err == E_HM_OK) {
		mem_zero_s(ret);
		err = actvcall_hmcall_proc_get_coredump(pid, request, &ret);
	}
	if (err == E_HM_OK) {
		*result = ret.result;
	}
	return err;
}

int hm_nsid_of_cnode_idx(uint32_t cnode_idx, int flag, uint32_t *id)
{
	int err = E_HM_OK;
	struct __actvret_hmcall_proc_nsid_of_cnode_idx ret;

	if (id == NULL) {
		hm_debug("invalid id pointer\n");
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		mem_zero_s(ret);
		err = actvcall_hmcall_proc_nsid_of_cnode_idx(cnode_idx, flag, &ret);
		if (err >= 0) {
			*id = ret.nsid;
		}
	}
	return err;
}

pid_t hm_fork(void)
{
#ifdef SYS_fork
	return lsyscall_proc_fork();
#else
	return lsyscall_proc_clone(SIGCHLD, 0, 0, 0, 0);
#endif
}

int hm_pdebug_thread_attach(pid_t tid)
{
	return actvcall_hmcall_proc_debug_thread(PDEBUG_THREAD_ATTACH, tid, ptr_to_type(false, void *));
}

int hm_pdebug_thread_seize(pid_t tid)
{
	return actvcall_hmcall_proc_debug_thread(PDEBUG_THREAD_SEIZE, tid, ptr_to_type(false, void *));
}

int hm_pdebug_thread_detach(pid_t tid)
{
	return  actvcall_hmcall_proc_debug_thread(PDEBUG_THREAD_DETACH, tid, NULL);
}

int hm_pdebug_set_event_options(pid_t tid, uint64_t options)
{
	return  actvcall_hmcall_proc_debug_thread(PDEBUG_THREAD_SET_EVENT_OPTION, tid,
						  u64_to_ptr(options, void));
}

int hm_pdebug_get_eventmsg(pid_t tid, unsigned long *out)
{
	int err;

	if (out == NULL) {
		err = E_HM_INVAL;
	} else {
		err = actvcall_hmcall_proc_debug_thread(PDEBUG_THREAD_GET_EVENT_MSG,
							tid, (void *)out);
	}
	return err;
}

int hm_pdebug_thread_read_regs(pid_t tid, struct arch_regs *tregs)
{
	int err = E_HM_OK;

	if (tregs == NULL) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		err = actvcall_hmcall_proc_debug_thread(PDEBUG_THREAD_READ_REGS, tid,
							tregs);
	}
	return err;
}

int hm_pdebug_thread_write_regs(pid_t tid, struct arch_regs *tregs)
{
	int err = E_HM_OK;

	if (tregs == NULL) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		err = actvcall_hmcall_proc_debug_thread(PDEBUG_THREAD_WRITE_REGS, tid,
							tregs);
	}
	return err;
}

int hm_pdebug_vspace_read(pid_t tid, unsigned long addr, unsigned long * const data)
{
	int err = E_HM_OK;
	struct __actvret_hmcall_proc_debug_vspace pdret;

	if (data == NULL) {
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		mem_zero_s(pdret);
		err = actvcall_hmcall_proc_debug_vspace(PDEBUG_VSPACE_READ, tid, (uintptr_t)addr, 0, &pdret);
		if (err >= 0) {
			*data = (unsigned long)pdret.data;
		}
	}
	return err;
}

int hm_pdebug_vspace_write(pid_t tid, unsigned long addr, unsigned long data)
{
	struct __actvret_hmcall_proc_debug_vspace pdret;

	mem_zero_s(pdret);
	return actvcall_hmcall_proc_debug_vspace(PDEBUG_VSPACE_WRITE, tid, (uintptr_t)addr, data, &pdret);
}

int hm_pdebug_thread_suspend(pid_t tid)
{
	return actvcall_hmcall_proc_debug_thread(PDEBUG_THREAD_SUSPEND, tid, NULL);
}

int hm_pdebug_thread_interrupt(pid_t tid)
{
	return actvcall_hmcall_proc_debug_thread(PDEBUG_THREAD_INTERRUPT, tid, NULL);
}

int hm_pdebug_thread_resume(pid_t tid, unsigned int sig)
{
	return actvcall_hmcall_proc_debug_thread(PDEBUG_THREAD_RESUME, tid, &sig);
}

/*
 * hm_pdebug_wait_status wait for thread status change, and return current event in type of pdebug_message
 * tid: target thread id
 * event: current pdebug event
 * option: specify PDEBUG_WOPT_NOHANG for return immediately
 *         specify PDEBUG_WOPT_NOFETCH for not clean up wait status after return
 * return E_HM_OK if success
 * return E_HM_SRCH if thread not found
 * return E_HM_INVAL if input parameter not valid
*/
int hm_pdebug_wait_status(pid_t tid, struct pdebug_event *event, int option)
{
	return actvcall_hmcall_proc_debug_wait_status(tid, event, option);
}

int hm_pdebug_thread_get_siginfo(pid_t tid, struct __siginfo *siginfo)
{
	return actvcall_hmcall_proc_debug_thread(PDEBUG_THREAD_GET_SIGINFO, tid,
						 siginfo);
}

int hm_pdebug_thread_get_new_threadinfo(pid_t tid, struct pdebug_new_thr_info *newthread)
{
	return actvcall_hmcall_proc_debug_thread(PDEBUG_THREAD_GET_NEWTID, tid,
						 newthread);
}

int hm_pdebug_thread_get_uctx(int tid)
{
	return actvcall_hmcall_proc_debug_thread(PDEBUG_THREAD_GET_UCTX_32BIT, tid, NULL);
}

int hm_pdebug_seccomp_event(void *arg)
{
	return actvcall_hmcall_proc_notify_seccomp_event(arg);
}

int hm_seccomp_refresh_data(void *data, unsigned long data_size)
{
	int err = E_HM_OK;
	struct __actvret_hmcall_proc_seccomp_refresh_data ret;

	if (data == NULL) {
		hm_error("invalid id pointer\n");
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		mem_zero_s(ret);
		err = actvcall_hmcall_proc_seccomp_refresh_data(&ret);
	}

	if (err == E_HM_OK) {
		if (memcpy_s(data, data_size, (void *)&ret.sc_data, sizeof(ret.sc_data)) != E_HM_OK) {
			hm_error("copy sc_data failed\n");
			err = E_HM_POSIX_FAULT;
		}
	}

	return err;
}

int hm_pdebug_config_ss_disable(pid_t tid)
{
	int err = actvcall_hmcall_proc_config_debugctx(DEBUG_CONFIG_SS_DISABLE, tid,
					NULL, NULL);
	if(err != E_HM_OK) {
		hm_warn("config ss disable failed, err = %s\n", hmstrerror(err));
	}
	return err;
}

int hm_pdebug_config_ss_enable(pid_t tid)
{
	int err = actvcall_hmcall_proc_config_debugctx(DEBUG_CONFIG_SS_ENABLE, tid,
				NULL, NULL);
	if(err != E_HM_OK) {
		hm_warn("config ss enable failed, err = %s\n", hmstrerror(err));
	}
	return err;
}

int hm_pdebug_get_debug_info(pid_t tid, struct __debug_config_result *result)
{
	int err = actvcall_hmcall_proc_config_debugctx(DEBUG_CONFIG_GET_DEBUG_INFO, tid,
				NULL, result);
	if(err != E_HM_OK) {
		hm_warn("get debug info failed, err = %s\n", hmstrerror(err));
	}
	return err;
}

int hm_pdebug_config_wp_reg(pid_t tid, struct __debug_config_arg *arg)
{
	int err = actvcall_hmcall_proc_config_debugctx(DEBUG_CONFIG_SET_WP_REG, tid, arg, NULL);
	if(err != E_HM_OK) {
		hm_warn("config wp reg failed, err = %s\n", hmstrerror(err));
	}
	return err;
}

int hm_pdebug_config_bp_reg(pid_t tid, struct __debug_config_arg *arg)
{
	int err = actvcall_hmcall_proc_config_debugctx(DEBUG_CONFIG_SET_BP_REG, tid, arg, NULL);
	if (err != E_HM_OK) {
		hm_warn("config bp reg failed, err = %s\n", hmstrerror(err));
	}
	return err;
}

static int proc_elf_load_info_ipc(struct bunch_ipc_attr *attr, void *data)
{
	pid_t pid = *(pid_t *)data;
	return actvcall_hmcall_proc_elf_load_info(pid, *attr);
}

int hm_proc_elf_load_info(pid_t pid, struct bunch *bunch)
{
	int err = E_HM_OK;
	struct bunch_ipc_reader_ctx ctx;

	if (pid < PID_NONE || pid == PID_RESERVED || bunch == NULL) {
		hm_error("invalid pid=%d, or invalid bunch=0x%lx\n", pid, vaddr_of(bunch));
		err = E_HM_INVAL;
	}
	if (err == E_HM_OK) {
		ctx.ipc = proc_elf_load_info_ipc;
		ctx.private_data = ptr_to_void(&pid);
		ctx.target_info = (uint64_t)hmsrv_sysmgr_cnode_idx();
		err = bunch_ipc_request(bunch, &ctx);
		if (err < 0) {
			hm_info("bunch ipc try request data with shm: %s\n",
				hmstrerror(err));
		}
	}

	return err;
}

int hm_proc_show_pte(const void *vaddr)
{
	return actvcall_hmcall_proc_show_pte(vaddr);
}

int hm_proc_register_event(unsigned int type, const struct event_args_s *args, bool fwd)
{
	char unused;
	int err = E_HM_OK;

	if (args == NULL) {
		err = E_HM_INVAL;
	}

	if (safe_copy(&unused, args, sizeof(char)) < 0) {
		UNUSED(unused);
		err = E_HM_POSIX_FAULT;
	}

	if (err == E_HM_OK) {
		err = actvxactcall_hmcall_proc_register_event(fwd, false, type, *args);
	}

	return err;
}

#ifdef CONFIG_DAC
int hm_proc_getdumpable(void)
{
	return lsyscall_proc_prctl(PR_GET_DUMPABLE, 0, 0, 0, 0);
}

int hm_proc_setdumpable(unsigned long dumpable)
{
	return lsyscall_proc_prctl(PR_SET_DUMPABLE, dumpable, 0, 0, 0);
}
#else
int hm_proc_getdumpable(void)
{
	return E_HM_NOSYS;
}

int hm_proc_setdumpable(unsigned long dumpable)
{
	UNUSED(dumpable);
	return E_HM_NOSYS;
}
#endif

#ifdef CONFIG_SUBREAPER
int hm_proc_set_child_subreaper(unsigned long child_subreaper)
{
	return lsyscall_proc_prctl(PR_SET_CHILD_SUBREAPER, child_subreaper, 0, 0, 0);
}

int hm_proc_get_child_subreaper(int *const child_subreaper)
{
	return lsyscall_proc_prctl(PR_GET_CHILD_SUBREAPER, ptr_to_ulong(child_subreaper), 0, 0, 0);
}
#else
int hm_proc_set_child_subreaper(unsigned long child_subreaper)
{
	UNUSED(child_subreaper);
	return E_HM_NOSYS;
}

int hm_proc_get_child_subreaper(int *const child_subreaper)
{
	UNUSED(child_subreaper);
	return E_HM_NOSYS;
}
#endif

#ifdef CONFIG_NO_NEW_PRIVS
int hm_proc_set_privs(unsigned long arg2, unsigned long arg3,
		      unsigned long arg4, unsigned long arg5)
{
	return lsyscall_proc_prctl(PR_SET_NO_NEW_PRIVS, arg2, arg3, arg4, arg5);
}

int hm_proc_get_privs(unsigned long arg2, unsigned long arg3,
		      unsigned long arg4, unsigned long arg5)
{
	return lsyscall_proc_prctl(PR_GET_NO_NEW_PRIVS, arg2, arg3, arg4, arg5);
}
#else
int hm_proc_set_privs(unsigned long arg2, unsigned long arg3,
		      unsigned long arg4, unsigned long arg5)
{
	UNUSED(arg2, arg3, arg4, arg5);
	return E_HM_NOSYS;
}

int hm_proc_get_privs(unsigned long arg2, unsigned long arg3,
		      unsigned long arg4, unsigned long arg5)
{
	UNUSED(arg2, arg3, arg4, arg5);
	return E_HM_NOSYS;
}
#endif

int hm_proc_current_pgrp_isorphaned(void)
{
	int ret;
	ret = actvxactcall_hmcall_proc_current_pgrp_isorphaned(true, true);
	if (ret < 0) {
		hm_debug("hm process check current is pgrp_orphaned failed, err=%s\n",
			 hmstrerror(ret));
	}

	return ret;
}

int hm_proc_get_nr(int type, pid_t pid)
{
	return actvcall_hmcall_proc_get_nr(type, pid);
}

int hm_proc_get_cred_by_id(void *cred, size_t len,
			   unsigned long long proc_id, int flags)
{
	return actvcall_hmcall_proc_get_cred_by_id(cred, len, proc_id, CALLER_PIDNSID, flags);
}

int hm_proc_get_cred_by_id_in_pidns(void *cred, size_t len,
				    unsigned long long proc_id, unsigned int nsid, int flags)
{
	return actvcall_hmcall_proc_get_cred_by_id(cred, len, proc_id, nsid, flags);
}

int hm_proc_sched_stat(int type, pid_t id, struct proc_sched_stat_s * sched_stat, int num)
{
	return actvcall_hmcall_proc_sched_stat(type, id, sched_stat, num);
}

int hm_proc_sync_cred_for_ap(cref_t ap_cref)
{
	int ret;
	ret = actvcall_hmcall_proc_sync_cred_for_ap(ap_cref);
	if (ret < 0) {
		hm_debug("hm process sync cred for ap failed, err=%s\n",
			 hmstrerror(ret));
	}

	return ret;
}

int hm_proc_current_exec_path(char *buf, size_t len)
{
	return actvcall_hmcall_proc_current_exec_path(buf, len);
}

int hm_proc_vtid_to_root_tid(pid_t pid)
{
	int ret;
	struct __actvret_hmcall_proc_pid_to_pidinns ret_pid = {0};
	ret = actvcall_hmcall_proc_pid_to_pidinns(pid, UINT_MAX, ID_ROOT_PID, &ret_pid);
	if (ret != E_HM_OK) {
		hm_error_ratelimited("can't get root pid from pid\n");
	} else {
		ret = ret_pid.to_pid;
	}
	return ret;
}