/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Implementation of reading procfs node by pid
 * Author: Huawei OS Kernel Lab
 * Create: Sat Sep 18 13:05:06 2021
 */

#include <libsec/cred.h>
#include <libsec/utility.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>

static int procfs_read_pid_judge_size(pid_t pid, const char *node_name,
				      int32_t shm_id, size_t shm_len)
{
	struct hm_actv_buf name_buf;
	int ret = E_HM_OK;

	hm_actv_buf_init(&name_buf, node_name, strlen(node_name) + 1);
	int size = actvcall_hmcall_procfs_pid_read(pid, CALLER_PIDNSID, 0,
						   (unsigned long)(unsigned int)shm_id,
						   shm_len, name_buf);
	if (size < 0) {
		ret = size;
		hm_error("failed to read process information, err=%s\n",
			 hmstrerror(ret));
	}

	if (size == 0) {
		hm_error("message size is 0\n");
		ret = E_HM_INVAL;
	}

	if (size > 0) {
		ret = size;
	}
	return ret;
}

static void pid_node_shm_free(int32_t shm_id, size_t shm_len,
			      void *shm_vaddr)
{
	int rc = E_HM_OK;
	if (shm_vaddr == NULL) {
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		if (shm_vaddr != ulong_to_ptr(MAP_FAILED, void)) {
			rc = hm_mem_munmap(shm_vaddr, shm_len);
			if (rc < 0) {
				hm_warn("failed to munmap shm, err=%s\n",
					hmstrerror(rc));
			}
		}
	}
	if (rc == E_HM_OK) {
		rc = hm_ashm_close(shm_id);
		if (rc < 0) {
			hm_warn("failed to close shm, err=%s\n", hmstrerror(rc));
		}
	}
	if (rc == E_HM_OK) {
		rc = hm_ashm_unlink(shm_id);
		if (rc < 0) {
			hm_warn("failed to unlink shm, err=%s\n", hmstrerror(rc));
		}
	}
}

static int pid_node_shm_alloc(int32_t *shm_id, void **shm_vaddr, size_t *shm_len)
{
	int ret;
	int32_t id;
	void *vaddr = NULL;
	size_t len = 0UL;
	size_t open_len = __U(1) << __PAGE_SHIFT;

	id = hm_ashm_open_auth(0, 0U, open_len, SHM_O_CREAT | SHM_O_EXCL | SHM_O_RDWR,
			  &len);
	// > 0 means a valid shm_id; < 0 means error code; == 0 is impossible
	if (id > 0) {
		vaddr = hm_mem_mmap_posix(NULL, len,
			MPROT_READ | MPROT_WRITE,
			MFLAG_MAP_ANONYMOUS | MFLAG_MAP_SHARED | MFLAG_MAP_POPULATE,
			id, 0, &ret);
		if (ret != E_HM_OK) {
			pid_node_shm_free(id, len, vaddr);
		}
		*shm_id = id;
		*shm_vaddr = vaddr;
		*shm_len = len;
	} else {
		ret = id;
		hm_error("failed to open or mmap share memory, err=%s\n",
			 hmstrerror(ret));
	}

	return ret;
}

/* return 'exe' of process by read procfs */
int __attribute__((weak)) sec_log_read_pid_exe(pid_t pid, const char *node_name, char **data)
{
	int ret = E_HM_INVAL;
	int32_t shm_id = 0;
	void *shm_vaddr = NULL;
	size_t shm_len = 0UL;
	int size;
	size_t true_size;

	ret = pid_node_shm_alloc(&shm_id, &shm_vaddr, &shm_len);
	if (ret != E_HM_OK) {
		return ret;
	}
	size = procfs_read_pid_judge_size(pid, node_name, shm_id,
						shm_len);
	if (size < 0) {
		pid_node_shm_free(shm_id, shm_len, shm_vaddr);
		ret = size;
	} else {
		true_size = (unsigned int)size;
		*data = malloc(true_size + 1UL);
		if (*data == NULL) {
			hm_error("malloc failed\n");
			pid_node_shm_free(shm_id, shm_len, shm_vaddr);
			ret = E_HM_NOMEM;
		} else {
			NOFAIL(memcpy_s(*data, true_size + 1UL,
					shm_vaddr, true_size));
			pid_node_shm_free(shm_id, shm_len, shm_vaddr);
			(*data)[size] = '\0';
		}
	}
	return ret;
}

/* return 'exe' of process */
int sec_log_read_pid_node(const struct sec_chk_cred *cred, pid_t *pid,
			  const char *node_name, char **data)
{
	int ret;
	struct sec_cred_basic *basic;

	basic = basic_cred_of_sec_cred(cred);
	ret = sec_log_read_pid_exe(basic->pid, node_name, data);
	*pid = basic->pid;

	return ret;
}
