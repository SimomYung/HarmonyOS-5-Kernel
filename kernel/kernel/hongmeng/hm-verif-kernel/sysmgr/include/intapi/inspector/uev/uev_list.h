/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: uev list in sysmgr
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 28 14:36:33 2020
 */

UEV_POINT(SIGNALMGR, signal_process,
	  UEV_ARG(5, struct process_s *, target,
		  struct thread_s *, target_thread,
		  __u32, src_cnode_idx, struct thread_s *, sender,
		  const struct __siginfo*, siginfo),
	  UEV_TRACE_FIELD(5, char, tname[UEV_MAX_NAME], __u64, tid,
			  char, sname[UEV_MAX_NAME], __u64, sid, __u64, signum),
	  UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "signal_process");
			   uev_data_write_string(&trace_data, ((target_thread == NULL) ? "none" : target_thread->name));
			   uev_data_write_int(&trace_data, ((target_thread == NULL) ? -1 : thread_tid_of(target_thread)));
			   uev_data_write_string(&trace_data, ((sender == NULL) ? "none" : sender->name));
			   uev_data_write_int(&trace_data, ((sender == NULL) ? -1 : thread_tid_of(sender)));
			   uev_data_write_int(&trace_data, ((siginfo == NULL) ? 0 : (int)siginfo->signum));),
	  UEV_TRACE_EFMT_FIELD(OUTPUT_EVTNAME(event)
			       OUTPUT_STRING(target)
			       OUTPUT_NUM(int, tid)
			       OUTPUT_STRING(sender)
			       OUTPUT_NUM(int, tid)
			       OUTPUT_NUM(int, signum)))

UEV_POINT(SIGNALMGR, signal_deliver,
	  UEV_ARG(4, struct thread_s *, target, struct thread_s *, sender,
		  int, signum, const struct __siginfo*, siginfo),
	  UEV_TRACE_FIELD(5, char, tname[UEV_MAX_NAME], __u64, tid,
			  char, sname[UEV_MAX_NAME], __u64, sid, __u64, signum),
	  UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "signal_deliver");
			   uev_data_write_string(&trace_data, ((target == NULL) ? "none" : target->name));
			   uev_data_write_int(&trace_data, ((target == NULL) ? -1 : thread_tid_of(target)));
			   uev_data_write_string(&trace_data, ((sender == NULL) ? "none" : sender->name));
			   uev_data_write_int(&trace_data, ((sender == NULL) ? -1 : thread_tid_of(sender)));
			   uev_data_write_int(&trace_data, ((siginfo == NULL) ? 0 : (int)siginfo->signum));),
	  UEV_TRACE_EFMT_FIELD(OUTPUT_EVTNAME(event)
			       OUTPUT_STRING(target)
			       OUTPUT_NUM(int, tid)
			       OUTPUT_STRING(sender)
			       OUTPUT_NUM(int, tid)
			       OUTPUT_NUM(int, signum)))

UEV_POINT(PROCESSMGR, procmgr_exit,
	  UEV_ARG(2, struct process_s *, process, int, exit_status),
	  UEV_TRACE_FIELD(3, char, pname[UEV_MAX_NAME], int, pid, int, exit_status),
	  UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "procmgr_exit");
			   uev_data_write_string(&trace_data, process_name_of(process));
			   uev_data_write_int(&trace_data, process_pid_of(process));
			   uev_data_write_int(&trace_data, exit_status);),
	  UEV_TRACE_EFMT_FIELD(OUTPUT_EVTNAME(event)
			       OUTPUT_STRING(name)
			       OUTPUT_NUM(int, pid)
			       OUTPUT_NUM(int, exit_status)))

UEV_POINT(PROCESSMGR, process_cleanup,
	  UEV_ARG(1, struct process_s *, process),
	  UEV_TRACE_FIELD(0),
	  UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "process_cleanup");
			   uev_data_write_string(&trace_data, process_name_of(process));
			   uev_data_write_int(&trace_data, process_pid_of(process));
			   uev_data_write_int(&trace_data, process_exit_status_of(process));),
	  UEV_TRACE_EFMT_FIELD(OUTPUT_EVTNAME(event)
			       OUTPUT_STRING(name)
			       OUTPUT_NUM(int, pid)
			       OUTPUT_NUM(int, exit_status)))

UEV_POINT(PROCESSMGR, procmgr_execve,
	  UEV_ARG(2, struct process_s *, process, struct thread_s *, thread),
	  UEV_TRACE_FIELD(2, char, pname[UEV_MAX_NAME], __u64, pid),
	  UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "procmgr_execve");
			   uev_data_write_string(&trace_data, process_name_of(process));
			   uev_data_write_int(&trace_data, process_pid_of(process));),
	  UEV_TRACE_EFMT_FIELD(OUTPUT_EVTNAME(event)
			       OUTPUT_STRING(name)
			       OUTPUT_NUM(int, pid)))

UEV_POINT(PROCESSMGR, procmgr_fork,
	  UEV_ARG(2, struct process_s *, parent, struct process_s *, child),
	  UEV_TRACE_FIELD(2, __u64, ppid, __u64, pid),
	  UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "procmgr_fork");
			   uev_data_write_int(&trace_data, (parent == NULL ? PID_NONE : process_pid_of(parent)));
			   uev_data_write_int(&trace_data, (child == NULL ? PID_NONE : process_pid_of(child)));),
	  UEV_TRACE_EFMT_FIELD(OUTPUT_EVTNAME(event)
			       OUTPUT_NUM(int, ppid)
			       OUTPUT_NUM(int, pid)))

UEV_POINT(PROCESSMGR, procmgr_spawn,
	  UEV_ARG(2, struct process_s *, parent, struct process_s *, child),
	  UEV_TRACE_FIELD(2, __u64, ppid, __u64, pid),
	  UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "procmgr_spawn");
			   uev_data_write_int(&trace_data, (parent == NULL ? PID_NONE : process_pid_of(parent)));
			   uev_data_write_int(&trace_data, (child == NULL ? PID_NONE : process_pid_of(child)));),
	  UEV_TRACE_EFMT_FIELD(OUTPUT_EVTNAME(event)
			       OUTPUT_NUM(int, ppid)
			       OUTPUT_NUM(int, pid)))

UEV_POINT(PROCESSMGR, task_newtask,
	  UEV_ARG(2, struct process_s *, process, __u64, clone_flags),
	  UEV_TRACE_FIELD(4, __u32, pid, char, comm[HMCAP_THREAD_NAME_SIZE],
			  __u64, clone_flags, __u16, oom_score_adj),
	  UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "task_newtask");
			   uev_data_write_int(&trace_data, (process == NULL ? PID_NONE : process_pid_of(process)));
			   uev_data_write_string(&trace_data, process_name_of(process));
			   uev_data_write_u64(&trace_data, clone_flags);
			   uev_data_write_int(&trace_data, process_oom_score_adj_of(process));),
	  UEV_TRACE_EFMT_FIELD(OUTPUT_EVTNAME(event)
			       OUTPUT_NUM(int, pid)
			       OUTPUT_STRING(comm)
			       OUTPUT_HEX_NUM(uint64_t, clone_flags)
			       OUTPUT_NUM(short, oom_score_adj)))

UEV_POINT(THREADMGR, thread_create,
	  UEV_ARG(2, struct thread_s *, caller, struct thread_s *, target),
	  UEV_TRACE_FIELD(4, char, cname[UEV_MAX_NAME], __u64, cid,
			  char, tname[UEV_MAX_NAME], __u64, tid),
	  UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "thread_create");
			   uev_data_write_string(&trace_data, ((caller == NULL) ? "none" : caller->name));
			   uev_data_write_int(&trace_data, ((caller == NULL) ? PID_NONE : thread_tid_of(caller)));
			   uev_data_write_string(&trace_data, ((target == NULL) ? "none" : target->name));
			   uev_data_write_int(&trace_data, ((target == NULL) ? PID_NONE : thread_tid_of(target)))),
	  UEV_TRACE_EFMT_FIELD(OUTPUT_EVTNAME(event)
			       OUTPUT_STRING(caller)
			       OUTPUT_NUM(int, tid)
			       OUTPUT_STRING(target)
			       OUTPUT_NUM(int, tid)))

UEV_POINT(THREADMGR, thread_exit,
	  UEV_ARG(1, struct thread_s *, thread),
	  UEV_TRACE_FIELD(2, char, tname[UEV_MAX_NAME], __u64, tid),
	  UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "thread_exit");
			   uev_data_write_string(&trace_data, thread->name);
			   uev_data_write_int(&trace_data, thread_tid_of(thread));),
	  UEV_TRACE_EFMT_FIELD(OUTPUT_EVTNAME(event)
			       OUTPUT_STRING(name)
			       OUTPUT_NUM(int, tid)))

UEV_POINT(THREADMGR, task_rename,
	  UEV_ARG(2, struct thread_s *, thread, const char *, newcomm),
	  UEV_TRACE_FIELD(4, __u32, pid, char, oldcomm[HMCAP_THREAD_NAME_SIZE],
			  char, newcomm[HMCAP_THREAD_NAME_SIZE], __u16, oom_score_adj),
	  UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "task_rename");
			   uev_data_write_int(&trace_data, ((thread == NULL) ? PID_NONE : thread_tid_of(thread)));
			   uev_data_write_string(&trace_data, thread->name);
			   uev_data_write_string(&trace_data, newcomm);
			   uev_data_write_int(&trace_data, ((thread == NULL) ? -1 :
					      process_oom_score_adj_of(thread->process)));),
	  UEV_TRACE_EFMT_FIELD(OUTPUT_EVTNAME(event)
			       OUTPUT_NUM(int, pid)
			       OUTPUT_STRING(oldcomm)
			       OUTPUT_STRING(newcomm)
			       OUTPUT_NUM(short, oom_score_adj)))

UEV_POINT(THREADMGR, sched_process_exit,
	  UEV_ARG(1, struct thread_s *, thread),
	  UEV_TRACE_FIELD(3, char, comm[HMCAP_THREAD_NAME_SIZE], int, pid, int, prio),
	  UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "sched_process_exit");
			   uev_data_write_string(&trace_data, ((thread == NULL) ? " " : thread_name_of(thread)));
			   uev_data_write_int(&trace_data, ((thread == NULL) ? PID_NONE : thread_tid_of(thread)));
			   uev_data_write_int(&trace_data, ((thread == NULL) ? -1 : thread_getpriority(thread)));),
	  UEV_TRACE_EFMT_FIELD(OUTPUT_EVTNAME(event)
			       OUTPUT_STRING(comm)
			       OUTPUT_NUM(int, pid)
			       OUTPUT_NUM(int, prio)))

UEV_POINT(THREADMGR, sched_process_free,
	  UEV_ARG(1, struct thread_s *, thread),
	  UEV_TRACE_FIELD(3, char, comm[HMCAP_THREAD_NAME_SIZE], int, pid, int, prio),
	  UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "sched_process_free");
			   uev_data_write_string(&trace_data, ((thread == NULL) ? " " : thread_name_of(thread)));
			   uev_data_write_int(&trace_data, ((thread == NULL) ? PID_NONE : thread_tid_of(thread)));
			   uev_data_write_int(&trace_data, ((thread == NULL) ? -1 : thread_getpriority(thread)));),
	  UEV_TRACE_EFMT_FIELD(OUTPUT_EVTNAME(event)
			       OUTPUT_STRING(comm)
			       OUTPUT_NUM(int, pid)
			       OUTPUT_NUM(int, prio)))

#ifdef CONFIG_RESMGR
UEV_POINT(RESMGR, cgroup_attach_task,
	  UEV_ARG(3, struct res_group *, rgrp, struct process_s *, process, const char *, comm),
	  UEV_TRACE_FIELD(6, int, dst_root, int, dst_id, int, dst_level,
			  char, dst_path[UEV_MAX_NAME], __u32, pid, char, comm[HMCAP_THREAD_NAME_SIZE]),
	  UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "cgroup_attach_task");
			   uev_data_write_int(&trace_data, 0);
			   uev_data_write_int(&trace_data, 0);
			   uev_data_write_int(&trace_data, 0);
			   uev_data_write_string(&trace_data, rgrp->hash_key == NULL ? " " : rgrp->hash_key);
			   uev_data_write_int(&trace_data, (process == NULL ? PID_NONE : process_pid_of(process)));
			   uev_data_write_string(&trace_data, comm);),
	  UEV_TRACE_EFMT_FIELD(OUTPUT_EVTNAME(event)
			       OUTPUT_NUM(int, dst_root)
			       OUTPUT_NUM(int, dst_id)
			       OUTPUT_NUM(int, dst_level)
			       OUTPUT_STRING(dst_path)
			       OUTPUT_NUM(int, pid)
			       OUTPUT_STRING(comm)))

UEV_POINT(RESMGR, cgroup_mkdir,
	  UEV_ARG(1, struct res_group *, rgrp),
	  UEV_TRACE_FIELD(4, int, root, int, gid, int, level, char, path[UEV_MAX_NAME]),
	  UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "cgroup_mkdir");
			   uev_data_write_int(&trace_data, 0);
			   uev_data_write_int(&trace_data, 0);
			   uev_data_write_int(&trace_data, 0);
			   uev_data_write_string(&trace_data, rgrp->hash_key == NULL ? " " : rgrp->hash_key);),
	  UEV_TRACE_EFMT_FIELD(OUTPUT_EVTNAME(event)
			       OUTPUT_NUM(int, root)
			       OUTPUT_NUM(int, id)
			       OUTPUT_NUM(int, level)
			       OUTPUT_STRING(path)))

UEV_POINT(RESMGR, cgroup_rmdir,
	  UEV_ARG(1, struct res_group *, rgrp),
	  UEV_TRACE_FIELD(4, int, root, int, gid, int, level, char, path[UEV_MAX_NAME]),
	  UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "cgroup_rmdir");
			   uev_data_write_int(&trace_data, 0);
			   uev_data_write_int(&trace_data, 0);
			   uev_data_write_int(&trace_data, 0);
			   uev_data_write_string(&trace_data, rgrp->hash_key == NULL ? " " : rgrp->hash_key);),
	  UEV_TRACE_EFMT_FIELD(OUTPUT_EVTNAME(event)
			       OUTPUT_NUM(int, root)
			       OUTPUT_NUM(int, id)
			       OUTPUT_NUM(int, level)
			       OUTPUT_STRING(path)))

UEV_POINT(RESMGR, cgroup_freeze,
	  UEV_ARG(1, struct res_group *, rgrp),
	  UEV_TRACE_FIELD(4, int, root, int, gid, int, level, char, path[UEV_MAX_NAME]),
	  UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "cgroup_freeze");
			   uev_data_write_int(&trace_data, 0);
			   uev_data_write_int(&trace_data, 0);
			   uev_data_write_int(&trace_data, 0);
			   uev_data_write_string(&trace_data, rgrp->hash_key == NULL ? " " : rgrp->hash_key);),
	  UEV_TRACE_EFMT_FIELD(OUTPUT_EVTNAME(event)
			       OUTPUT_NUM(int, root)
			       OUTPUT_NUM(int, id)
			       OUTPUT_NUM(int, level)
			       OUTPUT_STRING(path)))

UEV_POINT(RESMGR, cgroup_unfreeze,
	  UEV_ARG(1, struct res_group *, rgrp),
	  UEV_TRACE_FIELD(4, int, root, int, gid, int, level, char, path[UEV_MAX_NAME]),
	  UEV_TRACE_ASSIGN(uev_data_write_string(&trace_data, "cgroup_unfreeze");
			   uev_data_write_int(&trace_data, 0);
			   uev_data_write_int(&trace_data, 0);
			   uev_data_write_int(&trace_data, 0);
			   uev_data_write_string(&trace_data, rgrp->hash_key == NULL ? " " : rgrp->hash_key);),
	  UEV_TRACE_EFMT_FIELD(OUTPUT_EVTNAME(event)
			       OUTPUT_NUM(int, root)
			       OUTPUT_NUM(int, id)
			       OUTPUT_NUM(int, level)
			       OUTPUT_STRING(path)))
#endif
