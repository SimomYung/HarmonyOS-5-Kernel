/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Exported APIs of devhost
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#include "types.h"

#include <vfs.h>
#include <devhost_api.h>
#include <lib/dlist.h>
#include <sys/mman.h>
#include <libalgo/idr.h>
#include <libhmpm/shutdown.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <libhmirq/intr.h>
#include <libsysif/fs/api.h>
#include <libsysif/devmgr/api.h>
#include <libsysif/devmgr/types.h>
#include <libsysif/sysmgr/api.h>
#include <libsysif/devhost/api.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_rwlock.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmucap/ucap.h>
#include <libdevhost/compatible.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libhmsrv_sys/hm_signal.h>
#include <libhmsrv_sys/hm_sysctrl.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libhwsecurec/securec.h>

#include "devhost.h"
#include "devhost_bio.h"
#include "devhost_irq.h"
#include <devhost/log.h>
#include <devhost/file.h>
#ifdef CONFIG_SEHARMONY
#include <devhost/security_seharmony.h>
#endif
#include "devhost_backend.h"
#include "utils/utils.h"

void devhost_svrp_release(uint64_t rref)
{
	int err = E_HM_OK;

	err = hm_ucap_reject(0, rref);
	if (err < 0) {
		dh_warn("hm_ucap_reject failed, err=%s\n", hmstrerror(err));
	}
}

void devhost_shm_release(void *shm_vaddr, size_t shm_len)
{
	int err = E_HM_OK;

	err = hm_mem_munmap(shm_vaddr, shm_len);
	if (err < 0) {
		dh_warn("release shmem failed, err=%s\n", hmstrerror(err));
	}
}

int devhost_block_end_io(void *private, int bi_error, unsigned int rq_idx)
{
	int ret = E_HM_OK;
	struct block_ctx *ctx = (struct block_ctx *)private;

	if (ctx == NULL) {
		return -EINVAL;
	}

	ret = actvcapcall_fscall_endio_io_done(ctx->rref_notify,
					       ctx->service_id,
					       rq_idx, bi_error);
	return -hmerrno2posix(ret);
}

int devhost_block_queue_end_io(void *private, int bi_error, unsigned int idx)
{
	int ret = E_HM_OK;
	struct sring_buf *end_srbuf = NULL;
	struct bd_io *bd_io = NULL;
	struct block_ctx *ctx = (struct block_ctx *)private;
	uint64_t rref_notify;
	uint64_t service_id;

	if ((ctx == NULL) || (ctx->end_srbuf == NULL)) {
		return -EINVAL;
	}
	end_srbuf = ctx->end_srbuf;
	rref_notify = ctx->rref_notify;
	service_id = ctx->service_id;
	bd_io = (struct bd_io *)end_srbuf->data_vaddr;
	bd_io = bd_io + idx;
	if ((uintptr_t)end_srbuf->data_vaddr + end_srbuf->data_size <
	    (uintptr_t)bd_io) {
		return -EINVAL;
	}
	bd_io->error = bi_error;
	(void)raw_rwlock_rdlock(&end_srbuf->rwlock);
	if (bd_io->in_endio == true) {
		dh_warn("bd_io may already process");
		BUG_ON_D(bd_io->in_endio, "bd_io in_endio:%d", (int)bd_io->in_endio);
	}

	bd_io->in_endio = true;
	devhost_update_bio_time(&bd_io->end);
	(void)sring_buf_push(end_srbuf, bd_io);
	if (sring_is_onwork(end_srbuf)) {
		(void)raw_rwlock_unlock(&end_srbuf->rwlock);
		return 0;
	}
	(void)raw_rwlock_unlock(&end_srbuf->rwlock);

	/* After processing sring_buf_push bd_io, fs have chance to deal with io,
	* and free io node data.(example: here bd_io is the last I/O data, and fs
	* async work is processing the I/O of the node) So ctx may have been release. */
	ret = actvcapcall_fscall_endio_io_done(rref_notify,
					       service_id,
					       INVALID_IO_IDX, 0);
	if (ret != E_HM_OK) {
		dh_warn("fscall end_io faild, err=%s\n", hmstrerror(ret));
	}
	return -hmerrno2posix(ret);
}

int devhost_timer_accept_grant(uint64_t src_cnode, uint64_t src_uref,
			       unsigned long flags, uint64_t *accepted_rref)
{
	rref_t recv_rref;
	if (accepted_rref == NULL) {
		return -EINVAL;
	}
	recv_rref = hm_ucap_accept_grant((cref_t)src_cnode,
					 (uref_t)src_uref, flags);
	if (IS_REF_ERR(recv_rref)) {
		dh_error("devhost: timer accept grant failed, err=%s\n",
			 hmstrerror(REF_TO_ERR(recv_rref)));
		return -hmerrno2posix(REF_TO_ERR(recv_rref));
	}

	/* successfully received */
	*accepted_rref = (uint64_t)recv_rref;
	return 0;
}

int __devhost_shm_mem_destroy(int shm_id)
{
	int ret = E_HM_OK;
	ret = hm_ashm_close(shm_id);
	if (ret < 0) {
		return ret;
	}
	return hm_ashm_unlink(shm_id);
}

int __devhost_shm_mem_alloc(size_t size, uint64_t oflags, unsigned long long *paddr,
			    int *ret_id)
{
	int err = E_HM_OK;
	int shm_id;
	struct shm_node_info shm_info;

	BUG_ON(paddr == NULL);

	shm_id = hm_ashm_open_auth(0, 0U, size, oflags, NULL);
	if (shm_id < 0) {
		return shm_id;
	}

	err = hm_mem_query_shm_info(shm_id, &shm_info);
	if (err != E_HM_OK) {
		dh_error("query shm info failed, err=%s\n", hmstrerror(err));
		(void)__devhost_shm_mem_destroy(shm_id);
		return err;
	}

	*paddr = shm_info.paddr;
	if (ret_id != NULL) {
		*ret_id = shm_id;
	}

	return E_HM_OK;
}

void *devhost_backend_mem_alloc(size_t size, uint32_t mprot,
				unsigned long long *rpaddr,
				int *rshmid)
{
	int err = E_HM_OK;
	int shm_id = -1;
	off_t offset = 0;
	void *rvaddr = NULL;
	void *backend_addr = NULL;
	unsigned long long paddr = 0;
	uint32_t flags =  MFLAG_MAP_SHARED | MFLAG_MAP_ANONYMOUS |
			  MFLAG_MAP_POPULATE;
	uint64_t oflags = SHM_O_CREAT | SHM_O_EXCL | SHM_O_CELL | SHM_O_RDWR;

	if (mprot & MPROT_DMA) {
		oflags |= SHM_O_DMA;
	}

	err = __devhost_shm_mem_alloc(size, oflags, &paddr, &shm_id);
	if (err < 0) {
		return MAP_FAILED;
	}

	backend_addr = devhost_backend_prepare_map(paddr, (unsigned long)size);
	if (backend_addr == MAP_FAILED) {
		(void)__devhost_shm_mem_destroy(shm_id);
		return MAP_FAILED;
	}
	if (backend_addr != NULL) {
		flags |= MFLAG_MAP_FIXED;
	}

	/* The map operation does not require MPROT_DMA */
	mprot &= (~MPROT_DMA);
	rvaddr = hm_mem_mmap(backend_addr, size, (uint32_t)mprot,
			     flags, shm_id, offset);
	if (rvaddr == MAP_FAILED) {
		dh_error("shm map failed\n");
		devhost_backend_prepare_unmap(backend_addr, (unsigned long)size);
		(void)__devhost_shm_mem_destroy(shm_id);
		return MAP_FAILED;
	}

	if (rpaddr != NULL) {
		*rpaddr = paddr;
	}
	if (rshmid != NULL) {
		*rshmid = shm_id;
	}

	return rvaddr;
}

void devhost_backend_mem_free(int shmid, void *addr, size_t size)
{
	int err = E_HM_OK;
	err = hm_mem_munmap(addr, size);
	if (err != E_HM_OK) {
		dh_warn("hm_mem_munmap failed, rc=%s\n", hmstrerror(err));
	}

	devhost_backend_prepare_unmap(addr, (unsigned long)size);
	err = __devhost_shm_mem_destroy(shmid);
	if (err != E_HM_OK) {
		dh_warn("shm_mem_destroy failed, rc=%s\n", hmstrerror(err));
	}
}

int
devhost_shutdown(void)
{
	int err = E_HM_OK;
	err = hm_shut_down();
	return -hmerrno2posix(err);
}

int
devhost_snapshot(void)
{
	int err = E_HM_OK;
	err = hm_reboot("snapshot");
	return -hmerrno2posix(err);
}

void devhost_kill_process_group(int sid, int signum)
{
	int ret = E_HM_OK;

	ret = hm_signal_fgpgrp(sid, signum);
	if (ret == E_HM_SRCH) {
		dh_debug("send sig to foreground process but it was exited, sig=%d\n", signum);
	} else if (ret != 0) {
		dh_warn("send sig to foreground process failed, "
			 "sig=%d, group_of_session=%d, rc=%s\n",
			 signum, sid, hmstrerror(ret));
	}
}

void devhost_kill_process(int pid, int signum)
{
	int ret = E_HM_OK;

	ret = hm_kill(pid, signum);
	if (ret == E_HM_SRCH) {
		dh_debug("send sig to process but it was exited, sig=%d, pid=%d\n",
			 signum, pid);
	} else if (ret != 0) {
		dh_warn("send sig to process failed, sig=%d, pid=%d, ret=%s\n",
			 signum, pid, hmstrerror(ret));
	}
}

int devhost_kvic_prepare_irq(unsigned int kvic_irq_type,
			     void *arg, unsigned int arg_len)
{
	return __devhost_kvic_prepare_irq(kvic_irq_type, arg, arg_len);
}

/*
 * Kvic is not an irqctrl in kernel,
 * and 0 is returned as the kvic id, this value has no effect.
 * This func will be removed in irq optimization AR.
 */
int devhost_kvic_default_ctrl(void)
{
	return 0;
}

static int check_pmuen(void)
{
	int err = E_HM_OK;
	unsigned int is_cpuctx_attached = 0;

	err = hm_sysctrl_check_cpuctx(&is_cpuctx_attached);
	if (err < 0) {
		dh_error("failed to call hm_sysctrl_check_cpuctx, err=%s\n",
			 hmstrerror(err));
		return err;
	}

	return is_cpuctx_attached != 0 ? 1 : 0;
}

int devhost_pmu_enable(void)
{
	int err = E_HM_OK;

	err = check_pmuen();
	if (err < 0) {
		dh_error("failed to query PMU status\n");
		return -1;
	}

	if (err == 1) {
		dh_warn("PMU already enabled\n");
		return 0;
	}

	err = hm_sysctrl_attach_cpuctx();
	if (err < 0) {
		dh_error("failed to call hm_sysctrl_attach_cpuctx, err=%s\n",
			 hmstrerror(err));
	}

	return err;
}

int devhost_pmu_disable(void)
{
	int err = E_HM_OK;

	err = check_pmuen();
	if (err < 0) {
		dh_error("failed to query PMU status\n");
		return -1;
	}

	if (err == 0) {
		dh_warn("PMU already disabled\n");
		return 0;
	}

	err = hm_sysctrl_detach_cpuctx();
	if (err < 0) {
		dh_error("failed to call hm_sysctrl_detach_cpuctx, ret=%s\n",
			 hmstrerror(err));
	}

	return err;
}

/* devhost call procmgr to get session id via ulib */
int devhost_mysid(void)
{
	int ret = E_HM_OK;
	ret = hm_mysid();
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}
	return ret;
}

int devhost_getfgpgrp(int sid)
{
	int ret = hm_getfgpgrp(sid);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}
	return ret;
}

int devhost_setfgpgrp(int sid, int pgrp)
{
	int ret = hm_setfgpgrp(sid, pgrp);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}
	return ret;
}

int devhost_current_pid(void)
{
	int pid = 0;
	__u32 cnode_idx;

	pid = hm_actv_local_thread_vpid_of();
	if (pid > 0) {
		return pid;
	}

	cnode_idx = __REF_CNODE_IDX_OF(hm_thread_effective_cref());
	pid = hm_pid_of_cnode_idx((uint32_t)cnode_idx);
	if (pid < 0) {
		return -hmerrno2posix(pid);
	}

	return pid;
}

int devhost_current_sid_pgid(int *sid, int *pgid)
{
	int ret = E_HM_OK;

	ret = hm_get_sid_pgid(sid, pgid);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}

	return ret;
}

int devhost_getpgid(int pid)
{
	int pgid;

	pgid = actvxactcall_hmcall_proc_getpgid_xact(true, false, pid);
	if (pgid < 0) {
		return -hmerrno2posix(pgid);
	}
	return pgid;
}

int devhost_current_pgid(void)
{
	int pid;
	pid = devhost_current_pid();
	if (pid < 0) {
		dh_error("devhost_current_pid failed, ret=%d\n", pid);
		return pid;
	}
	return devhost_getpgid(pid);
}

int devhost_getsid(int pid)
{
	int sid;

	sid = actvxactcall_hmcall_proc_getsid_xact(true, false, pid);
	if (sid < 0) {
		return -hmerrno2posix(sid);
	}

	return sid;
}

int devhost_kconsole_print(const char* str, size_t size)
{
	int ret;
	if (str == NULL) {
		return -EINVAL;
	}
	ret = sysfast_kconsole_print(str, size, NULL);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}
	return ret;
}

int devhost_kconsole_enable(void)
{
	int ret;

	ret = sysfast_kconsole_enable(NULL);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}
	return ret;
}

int devhost_kconsole_disable(void)
{
	int ret;

	ret = sysfast_kconsole_disable(NULL);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}
	return ret;
}

int devhost_session_set_ttyinfo(int sid, const char *name, unsigned int devno)
{
	int ret;
	struct ttyinfo_s info;

	if (name == NULL) {
		return -EINVAL;
	}

	mem_zero_s(info);
	if (strncpy_s(info.name, TTY_NAME_MAX, name, strnlen(name, TTY_NAME_MAX)) != 0) {
		return -EINVAL;
	}

	info.devno = devno;
	ret =  actvxactcall_hmcall_proc_session_set_ttyinfo_xact(true, false, sid, info);
	if (ret < 0) {
		return -hmerrno2posix(ret);
	}
	return ret;
}

int devhost_mem_resv_mem(const void *vaddr, unsigned long len)
{
	struct __actvret_hmcall_mem_resv_mem actv_ret;
	int ret;

	if (len == 0) {
		return E_HM_INVAL;
	}
	mem_zero_s(actv_ret);
	ret = actvcall_hmcall_mem_resv_mem(vaddr, len, &actv_ret);
	if (ret != E_HM_OK) {
		dh_error("devhost mem resv mem(len=0x%lx) failed, err=%s\n", len, hmstrerror(ret));
		return ret;
	}
	return ret;
}

#ifdef CONFIG_SEHARMONY
int __attribute((weak))
devhost_seharmony_common_checker(int callno, ...)
{
	UNUSED(callno);
	return E_HM_OK;
}
#endif
