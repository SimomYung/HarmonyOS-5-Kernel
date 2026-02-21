/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: libdh actviation helpers
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 21 13:43:57 2020
 */
#include <libdevhost/devhost.h>

#include <unistd.h>
#include <sys/types.h>
#include <hmkernel/trigger.h>
#include <hmkernel/sched/sched.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/syscall.h>
#include <libsched/priority.h>
#include <libsysif/devhost/server.h>
#include <libhmlog/hmlog.h>
#include <libhmactv/actv.h>
#include <libhmsync/raw_thread.h>
#include <libhmucap/ucap.h>
#include <libhmsrv_sys/hm_thread.h>

void libdh_apconfig_set_default(struct actvpool_configs *configs, const char *ap_name)
{
	mem_zero_s(*configs);

	NOFAIL(strncpy_s(configs->ap_name, ACTVPOOL_NAME_LENGTH_MAX, ap_name, strlen(ap_name)));
	configs->actv_init_num = LIBDH_ACTV_NUM_INIT;
	configs->actv_max_num = LIBDH_ACTV_NUM_MAX;
	configs->tsd_size = LIBDH_ACTV_TSD_SIZE;
	configs->stack_size = LIBDH_ACTV_STACK_SIZE;
	configs->actvret_size = sizeof(union __actvret_drvcall_devhost);
	configs->recvbuf_size = LIBDH_ACTV_BUF_SIZE;
	configs->action_table = sysif_devhost_actvcapcallhdlr_table;
	configs->action_table_size = sizeof(void*) * (size_t)__devhost_method_MAX;
	configs->p = NULL;
}

int libdh_ap_create(struct actvpool_configs *configs, cref_t *ap_cref_out /* out */)
{
	cref_t ap_cref = 0;
	int err;

	if (configs == NULL || ap_cref_out == NULL) {
		return E_HM_INVAL;
	}

	err = hm_setup_actvpool(configs, &ap_cref);
	if (err < 0) {
		return err;
	}

	*ap_cref_out = ap_cref;

	return E_HM_OK;
}

int libdh_ap_elastic(const struct actvpool_configs *configs)
{
	return hm_actvpool_setup_elastic(configs);
}
