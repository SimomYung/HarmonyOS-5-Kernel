/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: libdh core sysif helpers
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 21 13:43:57 2020
 */
#include <libdevhost/devhost.h>

#include <dlfcn.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <hmasm/page.h>
#include <sys/types.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <libsysif/devmgr/api.h>
#include <libsysif/devhost/server.h>
#include <libsysif/devmgr/types.h>
#include <libhmactv/actv.h>
#include <libhmsync/raw_thread.h>
#include <libhmucap/ucap.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libhmsrv_sys/hm_path.h>
#include <libhmsrv_sys/hm_procmgr.h>

#include "internals.h"

static cref_t libdh_ap = ~0UL;
static rref_t g_devmgr_ap = 0;
static unsigned int devmgr_cnode_idx = ~0U;
static bool devmgr_ready = false;
static unsigned long long bootfdt_shm_id;
static unsigned int bootfdt_fwtype;

static int __wait_devmgr_ready(rref_t *devmgr_rref)
{
	int err;
	/* wait until devmgr is ready */
	do {
		(void)hm_thread_yield();
		err = hm_path_acquire(DEVMGR_PATH, devmgr_rref);
	} while (err == E_HM_POSIX_NOENT);

	return err;
}

static
int __register_devmgr_sysif(void)
{
	rref_t devmgr_rref = ERR_TO_REF(E_HM_INVAL);
	struct sysarg_cnode_statrref statrref = { 0 };
	int err;
	int ret;

	err = __wait_devmgr_ready(&devmgr_rref);
	if (err == E_HM_OK) {
		/* query devmgr cnode_idx by rref */
		err = sysfast_cnode_stat_rref(&statrref, devmgr_rref,
					CNODE_STATRREF_SRC_CNODE_IDX);
		if (err < 0) {
			ret = hm_path_release(DEVMGR_PATH, devmgr_rref);
			if (ret != 0) {
				hm_warn("hm_path_release failed %s\n", hmstrerror(ret));
			}
		}
	}

	if (err == E_HM_OK) {
		devmgr_cnode_idx = statrref.src_cnode_idx;
		g_devmgr_ap = devmgr_rref;
	}

	return err;
}

static int is_devmgr_spawned(void)
{
	int err;
	pid_t ppid;
	unsigned int cnode_idx;

	ppid = hm_getppid();
	if (ppid < 0) {
		err = E_HM_INVAL;
	} else {
		err = hm_get_cnode_idx(ppid, &cnode_idx);
		if (err < 0) {
			err = E_HM_INVAL;
		} else if (cnode_idx != devmgr_cnode_idx) {
			err = E_HM_INVAL;
		} else {
			err = E_HM_OK;
		}
	}

	return err;
}

static
int __do_register_libdh(unsigned int flags, struct hm_actv_buf isolate_group, uref_t ap_uref)
{
	int ret = E_HM_OK;
	uref_t devmgr_ap_uref = 0;
	struct sysarg_cnode_statrref statrref;
	struct __actvret_drvcall_devmgr_core_register_devhost register_dh_ret;

	mem_zero_s(statrref);
	/* query devmgr cnode_idx by rref */
	ret = sysfast_cnode_stat_rref(&statrref, g_devmgr_ap,
				CNODE_STATRREF_SRC_CNODE_IDX |
				CNODE_STATRREF_USLOT_IDX);
	if (ret == E_HM_OK) {
		devmgr_ap_uref = ((uint64_t)statrref.src_cnode_idx << 32U) | statrref.uslot_idx;
		if (IS_REF_ERR(devmgr_ap_uref)) {
			ret = REF_TO_ERR(devmgr_ap_uref);
		}
	}

	if (ret == E_HM_OK) {
		/* register to devmgr */
		ret = actvcapcall_drvcall_devmgr_core_register_devhost(g_devmgr_ap, flags,
								       ap_uref, isolate_group,
								       devmgr_ap_uref, &register_dh_ret);
		if (ret == E_HM_OK) {
			bootfdt_shm_id = register_dh_ret.resource.bootfdt_shm_id;
			bootfdt_fwtype = register_dh_ret.resource.bootfdt_fwtype;
		}
	}
	return ret;
}

int libdh_firmware_data(uint64_t shm_key, void **buf, unsigned long *size)
{
	void *map_data = NULL;
	size_t shm_rlen = 0;
	int shm_id = -1;
	int ret = E_HM_OK;

	if (buf == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		shm_id = hm_ashm_open_auth(shm_key, libdh_get_devmgr_cnode_idx(),
					   0, SHM_O_RDWR, &shm_rlen);
		if (shm_id < 0) {
			ret = shm_id;
		}
	}

	if (ret == E_HM_OK) {
		map_data = hm_mem_mmap_posix(NULL, shm_rlen, MPROT_READ | MPROT_WRITE,
					     MFLAG_MAP_ANONYMOUS | MFLAG_MAP_SHARED,
					     shm_id, 0, &ret);
		if (ret == E_HM_OK) {
			ret = hm_ashm_close(shm_id);
			if (ret != E_HM_OK) {
				hm_warn("close shmem failed, ret=%s\n", hmstrerror(ret));
				(void)hm_mem_munmap(map_data, shm_rlen);
			}
		} else {
			(void)hm_ashm_close(shm_id);
		}
	}

	if (ret == E_HM_OK) {
		*buf = map_data;
		if (size != NULL) {
			*size = shm_rlen;
		}
	}
	return ret;
}

static void *g_boot_fdt = NULL;
static unsigned long g_boot_fdt_size = 0;

int libdh_acquire_bootfdt(void **buf, unsigned long *size, unsigned int fwtype_mask)
{
	int ret = E_HM_OK;
	if ((bootfdt_shm_id == 0) || (buf == NULL)) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		if ((fwtype_mask & bootfdt_fwtype) == 0) {
			*buf = NULL;
		} else {
			if (g_boot_fdt == NULL) {
				ret = libdh_firmware_data(bootfdt_shm_id, &g_boot_fdt, &g_boot_fdt_size);
			}

			if (ret == E_HM_OK)
				*buf = g_boot_fdt;
			if ((ret == E_HM_OK) && (size != NULL))
				*size = g_boot_fdt_size;
		}
	}
	return ret;
}

static
int __register_libdh(const char *name, cref_t ap_cref, unsigned int flags)
{
	struct hm_actv_buf isolate_group;
	struct __actvpool_grant grant;
	uref_t ap_uref;
	int ret = E_HM_OK;

	/* build isolate_group name */
	hm_actv_buf_init(&isolate_group, name, strlen(name) + 1U);

	/* export ap to devmgr */
	grant.credential = 0ULL;
	grant.pre_alloc_actv_cref = 0ULL;
	ap_uref = hm_ucap_export(0, devmgr_cnode_idx,
				 /* flags= */
				 CNODE_GRANT_PREPARE_FLAG_QUERY_SRC_CNODE_IDX |
				 CNODE_GRANT_PREPARE_FLAG_QUERY_USLOT_IDX,
				 ap_cref,
				 /* rights= */
				 CRIGHT_ACTVPOOL_CALL,
				 /* args= */
				 ptr_to_ulong(&grant));
	if (IS_REF_ERR(ap_uref)) {
		ret = REF_TO_ERR(ap_uref);
	}

	if (ret == E_HM_OK) {
		ret = __do_register_libdh(flags, isolate_group, ap_uref);
		if (ret < 0) {
			if (hm_ucap_revoke(0, ap_uref) < 0) {
				hm_warn("hm_ucap_revoke failed\n");
			}
		}
	}

	return ret;
}

static int __do_init_ap(const char *name, cref_t ap_cref, unsigned int flags)
{
	int ret = E_HM_OK;

	/* check if devhost is started by devmgr */
	if (dh_flag_test(flags, DH_FLAGS_STANDALONE)) {
		if (is_devmgr_spawned() != E_HM_OK) {
			ret = E_HM_INVAL;
		}
	}

	/* connect to devmgr */
	if (ret == E_HM_OK) {
		ret = __register_libdh(name, ap_cref, flags);
	}

	if (ret == E_HM_OK) {
		devmgr_ready = true;
		if (!dh_flag_test(flags, DH_FLAGS_STANDALONE)) {
			ret = actvxactcapcall_drvcall_devmgr_core_devhost_init_done(
							false,  true, g_devmgr_ap);
		}
	}

	return ret;
}

int libdh_init_ap(const char *name, cref_t ap_cref, unsigned int flags)
{
	int ret = E_HM_OK;

	if ((name == NULL) || (strlen(name) == 0U) ||
	    IS_REF_ERR(ap_cref)) {
		ret = E_HM_INVAL;
	}

	/* initialize devmgr sysif */
	if (ret == E_HM_OK) {
		ret = __register_devmgr_sysif();
	}

	/* record libdh_ap */
	if (ret == E_HM_OK) {
		libdh_ap = ap_cref;

		ret = __do_init_ap(name, ap_cref, flags);
		if (ret < 0) {
			libdh_ap = ~0UL;
			if (hm_path_release(DEVMGR_PATH, g_devmgr_ap) < 0) {
				hm_warn("failed to release DEVMGR_PATH\n");
			}
		}
	}

	return ret;
}

uref_t libdh_ap_grant(rref_t tgt_rref)
{
	struct __actvpool_grant grant;

	grant.credential = 0ULL;
	grant.pre_alloc_actv_cref = 0ULL;

	return hm_ucap_grant(0, tgt_rref,
			     /* flags= */
			     CNODE_GRANT_PREPARE_FLAG_QUERY_SRC_CNODE_IDX |
			     CNODE_GRANT_PREPARE_FLAG_AUTO_RELEASE_UREF |
			     CNODE_GRANT_PREPARE_FLAG_GRANT_CAN_MOVE,
			     libdh_ap,
			     /* rights= */
			     CRIGHT_ACTVPOOL_CALL,
			     /* args= */
			     ptr_to_ulong(&grant));
}

uref_t libdh_ap_export(unsigned int tgt_cidx)
{
	struct __actvpool_grant grant;

	grant.credential = 0ULL;
	grant.pre_alloc_actv_cref = 0ULL;

	return hm_ucap_export(0, (uint32_t)tgt_cidx,
			      /* flags= */
			      CNODE_GRANT_PREPARE_FLAG_QUERY_SRC_CNODE_IDX |
			      CNODE_GRANT_PREPARE_FLAG_QUERY_USLOT_IDX,
			      libdh_ap,
			      /* rights= */
			      CRIGHT_ACTVPOOL_CALL,
			      /* args= */
			      ptr_to_ulong(&grant));
}

rref_t libdh_get_devmgr_ap(void)
{
	return g_devmgr_ap;
}

unsigned int libdh_get_devmgr_cnode_idx(void)
{
	return devmgr_cnode_idx;
}

cref_t libdh_get_dh_ap(void)
{
	return libdh_ap;
}

bool libdh_is_devmgr_ready(void)
{
	return devmgr_ready;
}
