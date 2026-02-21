/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 06 22:00:34 2020
 */

#include <libhmsrv_sys/hm_ipc_shm.h>

#include <libmem/utils.h>
#include <libsysif/sysmgr/api.h>
#include <libstrict/strict.h>

#ifdef CONFIG_SYSVIPC
int hm_ipc_shmget(int key, size_t size, int flags)
{
	int err;

	err = lsyscall_ipc_shmget(key, (unsigned long)size, flags);
	if (err < 0) {
		hm_debug("hmcall_ipc_shmget failed with error: %s\n",
			 hmstrerror(err));
	}

	return err;
}

void *hm_ipc_shmat(int shmid, const void *addr, int flags, int *errn)
{
	long long err;

	err = lsyscall_ipc_shmat(shmid, addr, flags);
	if (err < 0) {
		hm_debug("hmcall_ipc_shmat failed with error: %s\n",
			 hmstrerror(err));
		if (errn != NULL) {
			*errn = (int)err;
		}
		return NULL;
	}
	if (errn != NULL) {
		*errn = E_HM_OK;
	}

	return u64_to_ptr(err, void);
}

int hm_ipc_shmdt(const void *addr)
{
	int err;

	err = lsyscall_ipc_shmdt(addr);
	if (err < 0) {
		hm_debug("hmcall_ipc_shmdt failed with error: %s\n",
			 hmstrerror(err));
		return err;
	}
	return E_HM_OK;
}

int hm_ipc_shmctl(int shmid, int cmd, struct shmid_ds *buf)
{
	int err;

	err = lsyscall_ipc_shmctl(shmid, cmd, buf);
	if (err < 0) {
		hm_debug("hm_ipc_shmctl failed with error: %s\n",
			 hmstrerror(err));
	}

	return err;
}

int hm_ipc_popluate_shm(int32_t shm_id, const void *vaddr,
			unsigned long long offset,
			unsigned long long *dst,
			unsigned long size)
{
	int rc;

	rc = actvcall_hmcall_ipc_populate_shm(shm_id, vaddr, offset, dst, size);
	if (rc < 0) {
		hm_debug("actvcall_hmcall_ipc_populate_shm failed with error: %s\n",
			 hmstrerror(rc));
	}

	return rc;
}

int hm_ipc_shm_setacl(int shmid, const char *attr, size_t size)
{
	int err;

	err = actvcall_hmcall_ipc_shm_setacl(shmid, attr, (unsigned long)size);
	if (err < 0) {
		hm_debug("hmcall_ipc_shm_setacl failed with error: %s\n",
			 hmstrerror(err));
	}

	return err;
}

int hm_ipc_shm_getacl(int shmid, char *attr, size_t size)
{
	int err;

	err = actvcall_hmcall_ipc_shm_getacl(shmid, attr, (unsigned long)size);
	if (err < 0) {
		hm_debug("hmcall_ipc_shm_getacl failed with error: %s\n",
			 hmstrerror(err));
	}

	return err;
}

int hm_ipc_shm_removeacl(int shmid)
{
	int err;

	err = actvcall_hmcall_ipc_shm_removeacl(shmid);
	if (err < 0) {
		hm_debug("hmcall_ipc_shm_removeacl failed with error: %s\n",
			 hmstrerror(err));
	}

	return err;
}
#else
int hm_ipc_shmget(int key, size_t size, int flags)
{
	UNUSED(key, size, flags);
	return E_HM_NOSYS;
}

void *hm_ipc_shmat(int shmid, const void *addr, int flags, int *errn)
{
	UNUSED(shmid, addr, flags);
	*errn = E_HM_NOSYS;
	return NULL;
}

int hm_ipc_shmdt(const void *addr)
{
	UNUSED(addr);
	return E_HM_NOSYS;
}

int hm_ipc_shmctl(int shmid, int cmd, struct shmid_ds *buf)
{
	UNUSED(shmid, cmd, buf);
	return E_HM_NOSYS;
}

int hm_ipc_popluate_shm(int32_t shm_id, const void *vaddr,
			unsigned long long offset,
			unsigned long long *dst,
			unsigned long size)
{
	UNUSED(shm_id, vaddr, offset, dst, size);
	return E_HM_NOSYS;
}

int hm_ipc_shm_setacl(int shmid, const char *attr, size_t size)
{
	UNUSED(shmid, attr, size);
	return E_HM_NOSYS;
}

int hm_ipc_shm_getacl(int shmid, char *attr, size_t size)
{
	UNUSED(shmid, attr, size);
	return E_HM_NOSYS;
}

int hm_ipc_shm_removeacl(int shmid)
{
	UNUSED(shmid);
	return E_HM_NOSYS;
}
#endif
