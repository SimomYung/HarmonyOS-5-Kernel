/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Define operations of taskstat
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 14 17:51:10 2019
 */

#include <libmem/types.h>
#include <libhmlog/hmlog.h>
#include <hongmeng/types.h>
#include <hmkernel/const.h>
#include <libbunch/bunch_ipc.h>
#include <libsysif/sysmgr/api.h>
#include <hmsysmgr/process/pid.h>
#include <hmsysmgr/mgrs_stat.h>
#include <libprocfs/taskstat.h>
#include <libhwsecurec/securec.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <libhmsrv_sys/hm_sysctrl.h>

#define SIZE_KB		1024U

struct procfs_seq {
	bool create;
	struct bunch *bunch;
};

struct task_seq {
	struct procfs_seq seq;
	rref_t rref;
	int32_t cur_task;
	int32_t task_nr;
	int32_t cur_proc;
	int32_t proc_nr;
	struct mem_size_info mem_size;
};

static void seq_free(struct task_seq *tseq)
{
	struct procfs_seq *seq = &tseq->seq;
	if (seq->bunch != NULL) {
		bunch_ipc_destroy(seq->bunch);
		seq->bunch = NULL;
		mem_zero_s(*seq);
	}
}

static void task_seq_init(struct task_seq *tseq)
{
	tseq->seq.create = true;
	tseq->proc_nr = bunch_read_int32(tseq->seq.bunch);
	tseq->cur_task = 0;
	tseq->cur_proc = 0;
}

static struct task_seq *task_seq_create(void)
{
	struct task_seq *tseq = NULL;

	tseq = malloc(sizeof(struct task_seq));
	if (tseq == NULL) {
		hm_info("no memory to allocate task bunch\n");
	} else {
		mem_zero_s(*tseq);
		tseq->seq.bunch = bunch_ipc_create(0);
		if (tseq->seq.bunch == NULL) {
			free(tseq);
			tseq = NULL;
			hm_info("bunch_ipc_create nomem\n");
		}
	}

	return tseq;
}

static void task_seq_destroy(struct task_seq *tseq)
{
	if (tseq != NULL) {
		if (tseq->seq.bunch != NULL) {
			bunch_ipc_destroy(tseq->seq.bunch);
			tseq->seq.bunch = NULL;
		}
		free(tseq);
	}
}

static inline bool is_flag_task_bit(unsigned int flag, enum task_flag task)
{
	return (flag & task_flag_mask((unsigned int)(task))) != 0U;
}

static int fetch_proc_stats(pid_t pid, struct task_seq *tseq, unsigned int proc_flag)
{
	int err;
	unsigned int flags = 0U;

	if (is_flag_task_bit(proc_flag, LIBPROCFS_FLAG_THREAD)) {
		flags |= (unsigned int)task_flag_mask(PROC_FLAGS_WITH_THREAD);
	}
	if (is_flag_task_bit(proc_flag, LIBPROCFS_FLAG_MEM)) {
		flags |= (unsigned int)task_flag_mask(PROC_FLAGS_WITH_MEM);
	}
	if (is_flag_task_bit(proc_flag, LIBPROCFS_FLAG_TIME)) {
		flags |= (unsigned int)task_flag_mask(PROC_FLAGS_WITH_TIME);
	}
	if (is_flag_task_bit(proc_flag, LIBPROCFS_FLAG_HGUARD)) {
		flags |= (unsigned int)task_flag_mask(PROC_FLAGS_WITH_HGUARD);
	}

	if (tseq->rref != 0ULL) {
		err = hm_server_proc_stat(tseq->rref, pid, flags, tseq->seq.bunch);
	} else {
		err = hm_proc_stat(pid, flags, tseq->seq.bunch);
	}
	if (err != 0) {
		hm_info("hm_proc_stat ret:%s, pid=%d, flags=%u\n",
			 hmstrerror(err), pid, flags);
	} else {
		task_seq_init(tseq);
	}

	return err;
}

static void free_procfs_all(struct hm_procfs *procfs)
{
	task_seq_destroy(procfs->task_seq);
	procfs->task_seq = NULL;

	if (procfs->cpu_stats != NULL) {
		free(procfs->cpu_stats);
		procfs->cpu_stats = NULL;
	}
}

static void task_mem_parser(struct mem_size_info *size_info,
			    struct bunch *pbunch)
{
	bool has_meminfo = false;

	has_meminfo = bunch_read_bool(pbunch);
	if (has_meminfo) {
		size_info->vmem_total_size = bunch_read_uint64(pbunch) / SIZE_KB;
		size_info->vmem_using_size = bunch_read_uint64(pbunch) / SIZE_KB;
		size_info->vmem_rw_size = bunch_read_uint64(pbunch) / SIZE_KB;
		size_info->vmem_share_size = bunch_read_uint64(pbunch) / SIZE_KB;
		size_info->vmem_none_size = bunch_read_uint64(pbunch) / SIZE_KB;
		size_info->p_pmem_size = bunch_read_uint64(pbunch) / SIZE_KB;
		size_info->a_pmem_size = bunch_read_uint64(pbunch) / SIZE_KB;
		size_info->priv_size = bunch_read_uint64(pbunch) / SIZE_KB;
		size_info->share_size = bunch_read_uint64(pbunch) / SIZE_KB;
		size_info->aver_size = bunch_read_uint64(pbunch) / SIZE_KB;
		size_info->phymem_size = bunch_read_uint64(pbunch) / SIZE_KB;
		size_info->dirty_total_size = bunch_read_uint64(pbunch) / SIZE_KB;
		size_info->dirty_share_size = bunch_read_uint64(pbunch) / SIZE_KB;
		size_info->stack_size = bunch_read_uint64(pbunch) / SIZE_KB;
		size_info->actv_size = bunch_read_uint64(pbunch) / SIZE_KB;
	}
}

static void parse_sched_basic(struct bunch *bunch, struct task_stats *task)
{
	const char *policy = NULL;
	const char *nice = NULL;
	const char *rt_prio = NULL;
	const char *prio = NULL;
	/* task rq */
	task->rq = bunch_read_uint32(bunch);
	/* sched policy */
	mem_zero_a(task->policy);
	policy = bunch_read_string(bunch);
	if (policy != NULL) {
		NOFAIL(strncpy_s(task->policy, sizeof(task->policy),
				policy, sizeof(task->policy) - 1UL));
	}
	/* thread rt prio */
	mem_zero_a(task->rt_prio);
	rt_prio = bunch_read_string(bunch);
	if (rt_prio != NULL) {
		NOFAIL(strncpy_s(task->rt_prio, sizeof(task->rt_prio),
				rt_prio, sizeof(task->rt_prio) - 1UL));
	}
	/* task nice */
	mem_zero_a(task->nice);
	nice = bunch_read_string(bunch);
	if (nice != NULL) {
		NOFAIL(strncpy_s(task->nice, sizeof(task->nice),
				nice, sizeof(task->nice) - 1UL));
	}
	/* sched priority */
	prio = bunch_read_string(bunch);
	if (prio != NULL) {
		NOFAIL(strncpy_s(task->priority, sizeof(task->priority),
				 prio, sizeof(task->priority) - 1UL));
	}
}

static void parse_basic(struct bunch *bunch, struct task_stats *task)
{
	const char *res = NULL;
	const char *sep = NULL;

	/* task name */
	mem_zero_a(task->comm);
	res = bunch_read_string(bunch);
	if (res != NULL) {
		sep = strrchr(res, (int)'/');
		if (sep != NULL) {
			res = sep + 1;
		}
		NOFAIL(strncpy_s(task->comm, sizeof(task->comm), res, sizeof(task->comm) - 1));
	}

	/* task tid */
	task->pid = bunch_read_int32(bunch);
	/* task state */
	mem_zero_a(task->state);
	res = bunch_read_string(bunch);
	if (res != NULL) {
		NOFAIL(strncpy_s(task->state, sizeof(task->state),
				 res, sizeof(task->state) - 1UL));
	}
	/* tcb_cref */
	task->cref = bunch_read_uint64(bunch);
	/* tcb_sched */
	task->sched_cnt = bunch_read_uint64(bunch);
	/* skip cur cpu */
	(void)bunch_read_uint32(bunch);
	/* sched basic parse */
	parse_sched_basic(bunch, task);
	/* task tgid */
	task->tgid = bunch_read_int32(bunch);
	/* task ppid */
	task->ppid = bunch_read_int32(bunch);
	/* task pgid */
	task->pgid = (uint32_t)bunch_read_int32(bunch);
	/* task uid */
	task->uid = bunch_read_uint32(bunch);
	/* task block reason */
	mem_zero_a(task->block);
	res = bunch_read_string(bunch);
	if (res != NULL) {
		NOFAIL(strncpy_s(task->block, sizeof(task->block),
				 res, sizeof(task->block) - 1UL));
	}

	/* skip sec mac label */
	(void)bunch_read_string(bunch);
	/* skip inner sec mac label */
	(void)bunch_read_string(bunch);

	/* prev task cref */
	task->prev_tcb_cref = bunch_read_uint64(bunch);
	/* preempt tcb cref */
	task->preempt_tcb_cref = bunch_read_uint64(bunch);
}

static int marshall_task(struct task_seq *tseq, struct task_stats *task,
			 unsigned int flag)
{
	struct bunch *bunch = tseq->seq.bunch;
	int err = E_HM_OK;

	for (; tseq->cur_proc < tseq->proc_nr; tseq->cur_proc++) {
		if (tseq->cur_task == 0) {
			if (is_flag_task_bit(flag, LIBPROCFS_FLAG_MEM)) {
				task_mem_parser(&tseq->mem_size, bunch);
			}
			tseq->task_nr = bunch_read_int32(bunch);
			if (tseq->task_nr == 0) {
				continue;
			}
			(void)bunch_read_string(bunch);
		} else if (tseq->cur_task >= tseq->task_nr) {
			tseq->cur_task = 0;
			continue;
		} else {
			/* do nothing */
		}
		break;
	}

	if (tseq->cur_proc >= tseq->proc_nr) {
		err = E_HM_POSIX_NOENT;
	} else {
		/* parse basic info of the task */
		parse_basic(bunch, task);

		if (is_flag_task_bit(flag, LIBPROCFS_FLAG_TIME)) {
			/* task time */
			task->total_time = bunch_read_uint64(bunch);
			task->run_time = bunch_read_uint64(bunch);
			task->ready_time = bunch_read_uint64(bunch);
		}

		/* task mem: the memory of the thread is equal to the process of which */
		task->vsize = (unsigned long)(tseq->mem_size.vmem_using_size);
		task->rwsize = (unsigned long)(tseq->mem_size.vmem_rw_size);
		task->rss = (unsigned long)(tseq->mem_size.phymem_size);
		task->share = (unsigned long)(tseq->mem_size.share_size);
		task->stack = (unsigned long)(tseq->mem_size.stack_size);

		tseq->cur_task++;
	}

	return err;
}

int hm_procfs_taskstat(struct hm_procfs *procfs, struct task_stats *stats,
		       unsigned int flag)
{
	int err = E_HM_OK;
	struct task_seq *tseq = NULL;

	if ((procfs == NULL) || (stats == NULL)) {
		err =  E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		tseq = (struct task_seq *)(procfs->task_seq);
		if (tseq == NULL) {
			hm_warn("call hm_procfs_taskstat firstly\n");
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK &&
	    (!tseq->seq.create || (is_flag_task_bit(flag, LIBPROCFS_FLAG_REFRESH)))) {
		err = fetch_proc_stats(PID_NONE, tseq, flag);
		if (err != E_HM_OK) {
			hm_info("can't fetch process stats from sysmgr\n");
		}
	}

	if (err == E_HM_OK) {
		err = marshall_task(tseq, stats, flag);
		if ((err != E_HM_OK) && (err != E_HM_POSIX_NOENT)) {
			hm_warn("can't inflate %s stats error: %s\n",
				(is_flag_task_bit(flag, LIBPROCFS_FLAG_THREAD)) ?
				"thread" : "process", hmstrerror(err));
		}

		if (err != E_HM_OK) {
			seq_free(tseq);
		}
	}

	return err;
}

int hm_procfs_inflate(struct hm_procfs *procfs)
{
	int err;

	if ((procfs == NULL) || (procfs->task_seq == NULL)) {
		hm_info("procfs isn't open correctly\n");
		err = E_HM_INVAL;
	} else {
		err = fetch_proc_stats(PID_NONE, procfs->task_seq, __U(1));
		if (err != E_HM_OK) {
			hm_info("can't fetch process stats from sysmgr\n");
		}
	}

	return err;
}

struct hm_procfs *hm_procfs_open(void)
{
	struct hm_procfs *procfs = NULL;
	struct task_seq *tseq = NULL;

	procfs = malloc(sizeof(struct hm_procfs));
	if (procfs == NULL) {
		hm_info("no memory to allocate procfs handler\n");
	} else {
		tseq = task_seq_create();
		if (tseq == NULL) {
			hm_info("no memory to allocate task seq\n");
			free(procfs);
			procfs = NULL;
		} else {
			mem_zero_s(*procfs);
			procfs->task_seq = tseq;
		}
	}

	return procfs;
}

int hm_procfs_register_rref(struct hm_procfs *procfs, rref_t rref)
{
	struct task_seq *tseq = NULL;
	int err = E_HM_OK;

	if (procfs == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		tseq = (struct task_seq *)(procfs->task_seq);
		if (tseq == NULL) {
			hm_info("task seq has not been created yet\n");
			err = E_HM_INVAL;
		}
	}

	if (err == E_HM_OK) {
		tseq->rref = rref;
	}

	return err;
}

void hm_procfs_close(struct hm_procfs *procfs)
{
	if (procfs != NULL) {
		free_procfs_all(procfs);
		free(procfs);
	}
}

static int cpustat_parse(struct hm_procfs *procfs, struct bunch *bunch)
{
	uint32_t i, cpu_nr;
	struct cpu_stat_entry *pentry = NULL;
	int err = E_HM_OK;

	if (procfs == NULL || bunch == NULL) {
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		cpu_nr = bunch_read_uint32(bunch);
		if (cpu_nr == 0U) {
			hm_info("cpu number in system is zero\n");
			err = E_HM_NOSYS;
		} else if (procfs->cpu_stats == NULL) {
			procfs->cpu_nr = cpu_nr;
			procfs->cpu_stats = malloc(cpu_nr * sizeof(struct cpu_stat_entry));
			if (procfs->cpu_stats == NULL) {
				hm_info("no memory to allocate cpu stats\n");
				err = E_HM_NOMEM;
			}
		} else if (cpu_nr != procfs->cpu_nr) {
			hm_info("cpu number in system changed, from %u to %u\n", procfs->cpu_nr, cpu_nr);
			err = E_HM_NOSYS;
		} else {
			/* do nothing */
		}
	}

	if (err == E_HM_OK) {
		pentry = procfs->cpu_stats;
		for (i = 0U; i < cpu_nr; i++) {
			pentry->online = bunch_read_bool(bunch);
			pentry->run_time = bunch_read_uint64(bunch);
			pentry->idle_time = bunch_read_uint64(bunch);
			pentry++;
		}
	}

	return err;
}

int hm_procfs_cpustat(struct hm_procfs *procfs)
{
	unsigned int flags;
	struct bunch *bunch = NULL;
	int err = E_HM_OK;

	if (procfs == NULL) {
		hm_info("procfs handler is NULL\n");
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		bunch = bunch_ipc_create(0);
		if (bunch == NULL) {
			err = E_HM_NOMEM;
			hm_info("bunch_ipc_create nomem\n");
		}
	}

	if (err == E_HM_OK) {
		flags = 0x01U << CPU_FLAGS_WITH_STATE;
		flags |= 0x01U << CPU_FLAGS_WITH_TIME;
		err = hm_sysctrl_cpu_stat(flags, bunch);
		if (err != 0) {
			hm_info("hm_sysctrl_cpu_stat ret:%s\n", hmstrerror(err));
		}
	}

	if (err == E_HM_OK) {
		err = cpustat_parse(procfs, bunch);
	}

	bunch_ipc_destroy(bunch);

	return err;
}

int hm_procfs_memstat(struct mem_stats *pmem)
{
	int err = E_HM_OK;
	int type;
	bool has_meminfo = false;
	struct bunch *bunch = NULL;
	unsigned int flags = task_flag_mask((int)MEMQUERY_FLAG_SUMMARY);

	if (pmem == NULL) {
		hm_info("pmem is NULL, not valid\n");
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		bunch = bunch_ipc_create(0);
		if (bunch == NULL) {
			err = E_HM_NOMEM;
			hm_info("hm_procfs_memstat bunch create nomem\n");
		}
	}

	if (err == E_HM_OK) {
		err = hm_mem_stat(flags, bunch);
		if (err != E_HM_OK) {
			hm_info("hm_procfs_memstat mem_stat rc:%s, flags=%u\n",
				hmstrerror(err), flags);
		}
	}

	if (err == E_HM_OK) {
		type = bunch_read_int32(bunch);
		if (type == (int)MEMQUERY_FLAG_SUMMARY) {
			has_meminfo = bunch_read_bool(bunch);
			if (has_meminfo) {
				pmem->total = (unsigned long)(bunch_read_uint64(bunch) / SIZE_KB);
				pmem->free = (unsigned long)(bunch_read_uint64(bunch) / SIZE_KB);
				pmem->share = (unsigned long)(bunch_read_uint64(bunch) / SIZE_KB);
				pmem->kobj = (unsigned long)(bunch_read_uint64(bunch) / SIZE_KB);
				pmem->cp_bitmap = (unsigned long)(bunch_read_uint64(bunch) / SIZE_KB);
				pmem->free_cache = (unsigned long)(bunch_read_uint64(bunch) / SIZE_KB);
				pmem->cache = (unsigned long)(bunch_read_uint64(bunch) / SIZE_KB);
			} else {
				hm_info("no memory information\n");
				err = E_HM_NOSYS;
			}
		} else {
			err = E_HM_NOSYS;
		}
	}

	/* allow NULL bunch pointer */
	bunch_ipc_destroy(bunch);

	return err;
}

int hm_server_memstat(rref_t server, struct mem_stats *pmem)
{
	int type;
	int err = E_HM_OK;
	bool has_meminfo = false;
	struct bunch *bunch = NULL;
	unsigned int flags = task_flag_mask((unsigned int)MEMQUERY_FLAG_SUMMARY);

	if (pmem == NULL) {
		hm_info("pmem is NULL, not valid\n");
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		bunch = bunch_ipc_create(0);
		if (bunch == NULL) {
			err = E_HM_NOMEM;
			hm_info("bunch_ipc_create nomem\n");
		}
	}

	if (err == E_HM_OK) {
		if (server != 0ULL) {
			err = hm_server_mem_stat(server, flags, bunch);
		} else {
			err = hm_mem_stat(flags, bunch);
		}

		if (err != E_HM_OK) {
			hm_info("hm_server_memstat rc:%s, flags=%u\n", hmstrerror(err), flags);
		}
	}

	if (err == E_HM_OK) {
		type = bunch_read_int32(bunch);
		if (type == (int)MEMQUERY_FLAG_SUMMARY) {
			has_meminfo = bunch_read_bool(bunch);
			if (!has_meminfo) {
				hm_info("no memory information\n");
				err = E_HM_NOSYS;
			} else {
				pmem->total = (unsigned long)(bunch_read_uint64(bunch) / SIZE_KB);
				pmem->free = (unsigned long)(bunch_read_uint64(bunch) / SIZE_KB);
				err = E_HM_OK;
			}
		} else {
			hm_info("memory type is not MEMQUERY_FLAG_SUMMARY\n");
			err = E_HM_NOSYS;
		}
	}

	bunch_ipc_destroy(bunch);

	return err;
}

/* read the statisitcs of single process */
int hm_procstat_single(pid_t pid, unsigned int proc_flag, struct task_stats *stats)
{
	int err = E_HM_OK;
	struct task_seq tseq;

	mem_zero_s(tseq);

	if (pid <= PID_RESERVED) {
		hm_info("pid is not valid\n");
		err = E_HM_INVAL;
	}

	if ((err == E_HM_OK) && (stats == NULL)) {
		hm_info("stats is NULL, not valid\n");
		err = E_HM_INVAL;
	}

	if ((err == E_HM_OK) &&
	    is_flag_task_bit(proc_flag, LIBPROCFS_FLAG_THREAD)) {
		hm_info("with thread is not support now\n");
		err = E_HM_INVAL;
	}

	if (err == E_HM_OK) {
		mem_zero_s(tseq);
		tseq.seq.bunch = bunch_ipc_create(0);
		if (tseq.seq.bunch == NULL) {
			err = E_HM_NOMEM;
			hm_info("bunch_ipc_create nomem\n");
		}
	}

	if (err == E_HM_OK) {
		err = fetch_proc_stats(pid, &tseq, proc_flag);
		if (err != E_HM_OK) {
			hm_info("can't fetch process stats from sysmgr\n");
		}
	}

	if (err == E_HM_OK) {
		err = marshall_task(&tseq, stats, proc_flag);
		if (err != E_HM_OK) {
			hm_warn("can't read process stats error: %d\n", err);
		}
	}

	if (tseq.seq.bunch != NULL) {
		bunch_ipc_destroy(tseq.seq.bunch);
	}

	return err;
}
