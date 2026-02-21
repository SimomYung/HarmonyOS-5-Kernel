/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Create actv pool for hmtracemgr
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 9 16:30:16 2019
 */
#include <hmasm/page.h>
#include <libhmsrv_sys/hm_procmgr.h>
#include <libhmsync/raw_thread.h>
#include <libsysif/hmtracemgr/server.h>
#include <libmem/utils.h>
#include "hmtracemgr_core.h"

#define HMTRACEMGR_ACTVP00L_NAME	"hmtracemgr_actvpool"
#define HMTRACEMGR_ACTV_STACK_SIZE	(PAGE_SIZE * 2u)
#define HMTRACEMGR_ACTVS_NR		1u
#define HMTRACEMGR_ACTV_BUF_SIZE	512UL

struct actv_buf_s {
	uint8_t buf[HMTRACEMGR_ACTV_BUF_SIZE];
} __aligned(PAGE_SIZE);

static struct actv_buf_s actv_bufs[HMTRACEMGR_ACTVS_NR];
static struct kobj_self_mng g_obj[HMTRACEMGR_ACTVS_NR];

static void delete_actvs(unsigned int end)
{
	int rc;
	unsigned int i;
	for (i = 0u; i < end; i++) {
		rc = hm_ucap_delete_actv(&g_obj[i]);
		if (rc != E_HM_OK) {
			hm_warn("hmtracemgr delete actv %u failed: %s\n",
				i, hmstrerror(rc));
		}
	}
}

static int setup_actvs(cref_t ap_cref)
{
	int rc;
	unsigned int i;
	struct actv_attr attr;
	struct actv actv;
	size_t tls_size = raw_thread_tls_size();
	/*
	 * The stack size is positively related to the TLS size.
	 * It must be an integer multiple of PAGE_SIZE
	 */
	size_t stack_size = HMTRACEMGR_ACTV_STACK_SIZE +
		PAGE_ALIGN_DOWN(tls_size);

	for (i = 0u; i < HMTRACEMGR_ACTVS_NR; i++) {
		mem_zero_s(attr);
		attr.actv_pool = ap_cref;
		attr.tsd_size = 0;
		attr.actvret_size = sizeof(union __actvret_hmtracecall);
		attr.stack_size = stack_size;
		attr.no_link_thread = 1;
		attr.recv_buf = &actv_bufs[i].buf[0];
		attr.recv_buf_sz = sizeof(actv_bufs[i].buf);
		attr.action_table = sysif_hmtracemgr_actvcapcallhdlr_table;
		attr.action_table_sz = sizeof(void*) * __hmtracemgr_method_MAX;

		rc = hm_actv_create(&actv, &attr);
		if (rc != E_HM_OK) {
			delete_actvs(i);
			hm_warn("hmtracemgr create actv failed: %s\n",
				hmstrerror(rc));
			return rc;
		}
		g_obj[i] = actv.obj;

		rc = syscap_ActivationReady(actv.obj.cref);
		if (rc < 0) {
			delete_actvs(i + 1);
			hm_warn("hmtracemgr ready actv failed\n");
			return E_HM_RESFAULT;
		}
	}
	return E_HM_OK;
}

int hmtracemgr_init(pid_t pid)
{
	int rc;
	cref_t ap_cref;
	struct actv_bind_ret_rref ret_rref;
	char buf[HMTRACEMGR_PATH_MAX_SIZE];

	/*
	 * Since copy_from_user is not supported in the case of page fault,
	 * we should touch the string before creating the serverpool;
	 * after the copy function is improved, it can be deleted here.
	 */
	hm_info("pid=%d hmtracemgr serverpool name: %s(%zu)\n", pid,
		HMTRACEMGR_ACTVP00L_NAME, strlen(HMTRACEMGR_ACTVP00L_NAME));

	ap_cref = hm_ucap_new_actvpool(HMTRACEMGR_ACTVP00L_NAME);
	if (IS_REF_ERR(ap_cref)) {
		rc = REF_TO_ERR(ap_cref);
		hm_warn("hmtracemgr create actv pool failed: %s\n", hmstrerror(rc));
		return rc;
	}

	rc = hm_actv_pool_bind_sysmgr(ap_cref, 0, &ret_rref);
	if (rc != E_HM_OK) {
		hm_warn("hmtracemgr actvpool bind sysmgr failed: %s\n", hmstrerror(rc));
		goto del_actv_pool_out;
	}

	rc = setup_actvs(ap_cref);
	if (rc != E_HM_OK) {
		hm_warn("hmtracemgr setup actvs failed: %s\n", hmstrerror(rc));
		goto del_actv_pool_out;
	}

	rc = snprintf_s(buf, (size_t)HMTRACEMGR_PATH_MAX_SIZE,
			(size_t)(HMTRACEMGR_PATH_MAX_SIZE - 1), HMTRACEMGR_PATH_FMT, pid);
	if (rc < 0) {
		hm_warn("hmtracemgr sprintf failed\n");
		rc = E_HM_POSIX_FAULT;
		goto del_actvs_out;
	}

	rc = hm_path_add_with_mode(ap_cref, 0UL, buf, __U(S_IRUSR) | __U(S_IRGRP));
	if (rc != E_HM_OK) {
		hm_warn("hmtracemgr add path failed: %s\n", hmstrerror(rc));
		goto del_actvs_out;
	}
	return E_HM_OK;

del_actvs_out:
	delete_actvs(HMTRACEMGR_ACTVS_NR);

del_actv_pool_out:
	(void)hm_ucap_delete_actvpool(ap_cref);
	return rc;
}
